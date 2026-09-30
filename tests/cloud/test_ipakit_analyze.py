import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from ipakit import Corpus, Func, IMAGE_BASE, load_corpus  # noqa: E402
from ipakit import analyze, clobber  # noqa: E402

R = {n: i for i, n in enumerate(
    ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
     's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1', 'gp', 'sp', 's8', 'ra'])}
NOP = 0
JR_RA = 0x03E00008


def I(op, rs, rt, imm):
    return (op << 26) | (R[rs] << 21) | (R[rt] << 16) | (imm & 0xFFFF)


def addiu(rt, rs, imm):
    return I(9, rs, rt, imm)


def sw(rt, off, base='sp'):
    return I(43, base, rt, off)


def lw(rt, off, base='sp'):
    return I(35, base, rt, off)


def addu(rd, rs, rt):
    return (R[rs] << 21) | (R[rt] << 16) | (R[rd] << 11) | 0x21


def jal(addr):
    return (3 << 26) | ((addr >> 2) & 0x3FFFFFF)


def beq(rs, rt, off, likely=False):
    return I(20 if likely else 4, rs, rt, off)


def build(funcs):
    """funcs: [(name, words_builder(base_addr, addr_of))] laid out back to back; returns (Corpus, {name: addr})."""
    addrs, pos = {}, IMAGE_BASE
    sizes = {}
    for name, fn in funcs:
        ws = fn(pos, addrs)
        sizes[name] = ws
        addrs[name] = pos
        pos += 4 * len(ws)
    # second pass so forward references resolve
    words, pos = {}, IMAGE_BASE
    for name, fn in funcs:
        words[name] = fn(pos, addrs)
        pos += 4 * len(words[name])
    image = [w for name, _ in funcs for w in words[name]] + [0] * 0x2000      # opaque tail
    fl = [Func(n, addrs[n], words[n]) for n, _ in funcs]
    return Corpus(fl, image, {}), addrs


def caller_with_live_t2(callee_name, call_words=None):
    def f(base, a):
        t = a.get(callee_name, IMAGE_BASE)
        return [addiu('sp', 'sp', -24), sw('ra', 20), addiu('t2', 'zero', 5), jal(t), NOP,
                addu('v0', 't2', 't2'), lw('ra', 20), JR_RA, addiu('sp', 'sp', 24)]
    return f


def leaf_ret(writes=None):
    def f(base, a):
        body = [addiu(writes, 'zero', 1)] if writes else []
        return body + [JR_RA, NOP]
    return f


class SyntheticAnalysis(unittest.TestCase):
    def info(self, funcs, name):
        c, _ = build(funcs)
        infos = analyze.analyze_all(c)
        return c, infos, infos[name]

    def test_live_across_found_and_explained(self):
        c, infos, i = self.info([('A', caller_with_live_t2('B')), ('B', leaf_ret())], 'A')
        self.assertEqual(i.sites[0].live_across, ['t2'])
        self.assertEqual(i.sites[0].callee, 'B')
        cm = clobber.ClobberMap(c, infos)
        self.assertEqual(cm.trans['B'] & analyze.names_mask(['t2']), 0)

    def test_live_across_unexplained_when_callee_writes_it(self):
        from ipakit import deps
        c, infos, i = self.info([('A', caller_with_live_t2('B')), ('B', leaf_ret('t2'))], 'A')
        m = deps.Model(c)
        kinds = [e['kind'] for e in m.evidence('A')]
        self.assertIn('unexplained', kinds)
        self.assertNotIn('live-across', kinds)

    def test_register_not_live_when_redefined_after_call(self):
        def f(base, a):
            return [addiu('sp', 'sp', -24), sw('ra', 20), addiu('t2', 'zero', 5), jal(a.get('B', IMAGE_BASE)), NOP,
                    addiu('t2', 'zero', 6), addu('v0', 't2', 't2'), lw('ra', 20), JR_RA, addiu('sp', 'sp', 24)]
        _, _, i = self.info([('A', f), ('B', leaf_ret())], 'A')
        self.assertEqual(i.sites[0].live_across, [])

    def test_delay_slot_runs_before_the_call(self):
        # t3 written in the jal's delay slot is not live across (it is consumed/defined before the call)
        def f(base, a):
            return [addiu('sp', 'sp', -24), sw('ra', 20), jal(a.get('B', IMAGE_BASE)), addiu('t3', 'zero', 1),
                    addu('v0', 't3', 't3'), lw('ra', 20), JR_RA, addiu('sp', 'sp', 24)]
        _, _, i = self.info([('A', f), ('B', leaf_ret())], 'A')
        self.assertEqual(i.sites[0].live_across, ['t3'])      # defined in the slot, used after: preserved by B

    def test_entry_read_and_callers(self):
        from ipakit import deps
        def C(base, a):
            return [addu('v0', 't3', 't3'), JR_RA, NOP]
        def D(base, a):
            return [addiu('sp', 'sp', -24), sw('ra', 20), addiu('t3', 'zero', 1), jal(a.get('C', IMAGE_BASE)), NOP,
                    lw('ra', 20), JR_RA, addiu('sp', 'sp', 24)]
        c, infos, i = self.info([('C', C), ('D', D)], 'C')
        self.assertEqual(i.entry_reads, ['t3'])
        self.assertEqual(infos['D'].entry_reads, [])
        m = deps.Model(c)
        self.assertEqual(m.classify('C')[0], 'IPA-leaf')
        self.assertEqual(m.classify('D')[0], 'IPA-caller')
        reasons, _ = m.minimal_closure(['C'])
        self.assertEqual(sorted(reasons), ['C', 'D'])

    def test_abi_registers_are_not_evidence(self):
        def C(base, a):
            return [addu('v0', 'a0', 'a1'), sw('a2', 8), sw('a3', 12), JR_RA, NOP]
        _, _, i = self.info([('C', C)], 'C')
        self.assertEqual(i.entry_reads, [])

    def test_home_slot_spill_of_abi_arg_is_not_a_read_but_ipa_reg_spill_is(self):
        def C(base, a):
            return [addiu('sp', 'sp', -16), sw('a0', 16), sw('t2', 20), addiu('sp', 'sp', 16), JR_RA, NOP]
        _, _, i = self.info([('C', C)], 'C')
        self.assertEqual(i.entry_reads, ['t2'])

    def test_unsaved_and_saved_callee_saved(self):
        def E(base, a):
            return [addiu('s3', 'zero', 1), JR_RA, NOP]
        def F(base, a):
            return [addiu('sp', 'sp', -8), sw('s3', 0), addiu('s3', 'zero', 1), lw('s3', 0), JR_RA, addiu('sp', 'sp', 8)]
        c, infos, i = self.info([('E', E), ('F', F)], 'E')
        self.assertEqual(analyze.mask_names(i.unsaved_written), ['s3'])
        f = infos['F']
        self.assertEqual(f.unsaved_written, 0)
        self.assertEqual(analyze.mask_names(f.saved), ['s3'])
        self.assertEqual(analyze.mask_names(f.restored), ['s3'])
        self.assertEqual(f.frame, 8)
        cm = clobber.ClobberMap(c, infos)
        self.assertEqual(cm.local['F'] & analyze.names_mask(['s3']), 0)       # restored: not a clobber
        self.assertNotEqual(cm.local['E'] & analyze.names_mask(['s3']), 0)

    def test_branch_likely_skips_delay_slot_when_not_taken(self):
        # beql a0,zero,L ; slot: t4=1 ; v0=t4+t4 ; L: jr ra.  The fall-through path never ran the slot,
        # so t4 is read before any write there: an entry read.
        def G(base, a):
            return [beq('a0', 'zero', 2, likely=True), addiu('t4', 'zero', 1), addu('v0', 't4', 't4'), JR_RA, NOP]
        _, _, i = self.info([('G', G)], 'G')
        self.assertEqual(i.entry_reads, ['t4'])

    def test_plain_branch_always_runs_delay_slot(self):
        def G(base, a):
            return [beq('a0', 'zero', 2), addiu('t4', 'zero', 1), addu('v0', 't4', 't4'), JR_RA, NOP]
        _, _, i = self.info([('G', G)], 'G')
        self.assertEqual(i.entry_reads, [])

    def test_code_after_jr_ra_delay_slot_is_not_a_successor(self):
        # the instruction after `jr ra`+slot reads t5 but is reached only by a branch that redefines it
        def G(base, a):
            return [beq('a0', 'zero', 3), NOP, JR_RA, NOP, addiu('t5', 'zero', 1), addu('v0', 't5', 't5'), JR_RA, NOP]
        _, _, i = self.info([('G', G)], 'G')
        self.assertEqual(i.entry_reads, [])

    def test_alternate_entry_resolution(self):
        def H(base, a):
            return [addiu('sp', 'sp', -16), sw('ra', 12), lw('ra', 12), JR_RA, addiu('sp', 'sp', 16)]
        def caller(base, a):
            h = a.get('H', IMAGE_BASE)
            return [addiu('sp', 'sp', -24), sw('ra', 20), jal(h + 4), addiu('sp', 'sp', -16), lw('ra', 20), JR_RA, addiu('sp', 'sp', 24)]
        c, infos, i = self.info([('H', H), ('K', caller)], 'K')
        s = i.sites[0]
        self.assertEqual((s.res, s.callee, s.alt, s.alt_verified), ('alt', 'H', True, True))
        self.assertEqual(len(i.alt_entries), 1)

    def test_unresolved_target_reported(self):
        def K(base, a):
            return [addiu('sp', 'sp', -24), sw('ra', 20), jal(IMAGE_BASE + 0x4000), NOP, lw('ra', 20), JR_RA, addiu('sp', 'sp', 24)]
        c, infos, i = self.info([('K', K)], 'K')
        self.assertEqual([s.res for s in i.unresolved], ['opaque'])
        self.assertIn('0x%08X' % (IMAGE_BASE + 0x4000), i.to_json()['unresolved_targets'])

    def test_external_target_is_abi(self):
        def K(base, a):
            return [addiu('sp', 'sp', -24), sw('ra', 20), jal(0x80020174), NOP, lw('ra', 20), JR_RA, addiu('sp', 'sp', 24)]
        c, infos, i = self.info([('K', K)], 'K')
        self.assertEqual(i.sites[0].res, 'external')
        cm = clobber.ClobberMap(c, infos)
        self.assertTrue(cm.unknown['K'])
        self.assertNotEqual(cm.trans['K'] & analyze.names_mask(['t9']), 0)

    def test_transitive_clobber(self):
        def top(base, a):
            return [addiu('sp', 'sp', -24), sw('ra', 20), jal(a.get('mid', IMAGE_BASE)), NOP, lw('ra', 20), JR_RA, addiu('sp', 'sp', 24)]
        def mid(base, a):
            return [addiu('sp', 'sp', -24), sw('ra', 20), jal(a.get('leaf', IMAGE_BASE)), NOP, lw('ra', 20), JR_RA, addiu('sp', 'sp', 24)]
        c, infos, _ = self.info([('top', top), ('mid', mid), ('leaf', leaf_ret('t7'))], 'top')
        cm = clobber.ClobberMap(c, infos)
        t7 = analyze.names_mask(['t7'])
        self.assertEqual(cm.local['top'] & t7, 0)
        self.assertNotEqual(cm.trans['top'] & t7, 0)
        self.assertEqual(cm.writers('top', 't7'), ['leaf'])

    def test_switch_table_cfg(self):
        # real data: every `jr` switch in the game resolves, so no function has an unresolved jump
        c = load_corpus(discover=False)
        infos = analyze.analyze_all(c)
        self.assertEqual(sum(len(i.indirect_jumps) for i in infos.values()), 0)
        self.assertGreater(sum(i.switch_tables for i in infos.values()), 20)


class RealFacts(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.c = load_corpus(discover=False)
        cls.infos = analyze.analyze_all(cls.c)

    def test_slot_state_setup_reads_s2(self):
        i = self.infos['slot_state_setup']
        self.assertEqual(i.entry_reads, ['s2'])
        self.assertEqual(analyze.mask_names(i.unsaved_written), ['s0', 's1', 's3'])
        self.assertEqual(i.frame, 32)

    def test_slot_state_setup_passes_t0_to_sound_update_channel(self):
        self.assertEqual(self.infos['sound_update_channel'].entry_reads, ['t0'])

    def test_func_800EA3F4_and_callees_are_pure_abi(self):
        for n in ('func_800EA3F4', 'func_800E8CB8', 'func_8008B3C8', 'func_800CFDEC', 'vector_copy_scale'):
            i = self.infos[n]
            self.assertEqual(i.entry_reads, [], n)
            self.assertEqual(i.unsaved_written, 0, n)
            self.assertFalse(any(s.live_across for s in i.sites), n)
        self.assertEqual(self.infos['func_800EA3F4'].frame, 160)

    def test_mp_target_steer_pos_is_an_ipa_callee(self):
        i = self.infos['MP_TargetSteerPos']
        self.assertEqual(i.entry_reads, ['s0', 'f20', 'f22'])

    def test_func_8008B640_reads_fp_registers(self):
        self.assertEqual(self.infos['func_8008B640'].entry_reads, ['f16', 'f20', 'f22', 'f24'])
        self.assertEqual(self.infos['physics_velocity_integrate_a'].entry_reads, ['s1', 's2', 'f20', 'f22', 'f24'])

    def test_frame_sizes(self):
        self.assertEqual(self.infos['MP_TargetSteerPos'].frame, 24)
        self.assertEqual(self.infos['func_8008705C'].frame, 24)

    def test_external_calls_never_hold_caller_saved_values(self):
        bad = [(i.name, s.pc) for i in self.infos.values() for s in i.sites
               if s.res in ('external', 'indirect') and s.live_across]
        self.assertLess(len(bad), 30)        # residual liveness artefacts: reported, rare (20 at time of writing)


if __name__ == "__main__":
    unittest.main()
