import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
sys.path.insert(0, str(Path(__file__).resolve().parent))
from ipakit import deps  # noqa: E402
import test_ipakit_analyze as T  # noqa: E402  (synthetic-corpus helpers)


class DepsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.m = deps.model_cache()

    def test_slot_state_setup_is_an_ipa_callee_reading_s2(self):
        cls, ev = self.m.classify('slot_state_setup')
        self.assertIn(cls, ('IPA-leaf', 'IPA-both'))
        texts = ' | '.join(e['text'] for e in ev)
        self.assertIn('reads s2 at entry', texts)
        self.assertIn('writes s3 with no save', texts)
        # every caller must set s2: the closure of the callee contains all of them
        reasons, _ = self.m.minimal_closure(['slot_state_setup'])
        callers = set(self.m.callers['slot_state_setup'])
        self.assertTrue(callers <= set(reasons))
        self.assertGreater(len(callers), 5)

    def test_callers_of_slot_state_setup_include_unregistered_heads(self):
        self.assertIn('func_80107EDC', self.m.callers['slot_state_setup'])

    def test_func_80107EDC_group(self):
        cls, ev = self.m.classify('func_80107EDC')
        self.assertEqual(cls, 'IPA-caller')
        reasons, _ = self.m.minimal_closure(['func_80107EDC'])
        self.assertEqual(sorted(reasons), ['func_80107EDC', 'slot_state_setup'])
        self.assertEqual(self.m.corpus.funcs['func_80107EDC'].words.__len__(), 158)

    def test_func_8008B640_group(self):
        cls, _ = self.m.classify('func_8008B640')
        self.assertEqual(cls, 'IPA-leaf')
        reasons, notes = self.m.minimal_closure(['physics_velocity_integrate_b', 'physics_velocity_integrate_f', 'model_bounds_calc'])
        self.assertIn('physics_velocity_integrate_a', reasons)
        self.assertIn('physics_velocity_integrate_b', notes['address_taken'])      # reached through a table: root
        r2, _ = self.m.minimal_closure(['physics_velocity_integrate_a'], 'chain')
        self.assertIn('func_8008B640', r2)

    def test_func_800EA3F4_group_is_pure_abi(self):
        for n in ('func_800EA3F4', 'func_8008B3C8', 'vector_copy_scale', 'func_800CFDEC', 'func_800E8CB8'):
            self.assertEqual(self.m.classify(n)[0], 'ABI', n)
        reasons, _ = self.m.minimal_closure(['func_800EA3F4'], 'full')
        self.assertEqual(sorted(reasons), ['func_800EA3F4'])

    def test_mp_target_steer_pos_closure_is_its_caller(self):
        reasons, _ = self.m.minimal_closure(['MP_TargetSteerPos'], 'chain')
        self.assertIn('camera_transform', reasons)

    def test_edges_are_register_specific(self):
        ev = [e for e in self.m.evidence('func_800E681C') if e['kind'] == 'live-across']
        self.assertTrue(ev)
        for e in ev:
            self.assertTrue(e['explained'])
            bit = 1 << deps.REGBIT[e['reg']]
            self.assertEqual(self.m.cm.trans[e['callee']] & bit, 0)
        self.assertEqual({e['callee'] for e in ev}, {'func_800E6460', 'func_800E627C'})

    def test_minimal_is_subset_of_conservative_bound_modes(self):
        for seeds in (['MP_TargetSteerPos'], ['func_800B9B64'], ['slot_state_setup'], ['model_bounds_calc']):
            d, _ = self.m.minimal_closure(seeds, 'direct')
            c, _ = self.m.minimal_closure(seeds, 'chain')
            f, _ = self.m.minimal_closure(seeds, 'full')
            self.assertTrue(set(d) <= set(c) <= set(f), seeds)

    def test_address_taken_roots_reported(self):
        self.assertIn('audio_channel_setup', self.m.addr_taken)               # raw pointer in a descriptor table
        self.assertTrue(any(v[0].startswith('data') for v in self.m.addr_taken.values()))
        self.assertTrue(any(v[0].startswith('code') for v in self.m.addr_taken.values()))
        self.assertGreater(len(self.m.indirect), 10)                          # jalr callers

    def test_validation_locked_groups(self):
        rows = {(r['group'], r['kind']): r for r in deps.validate(self.m, 'chain') if 'skipped' not in r}
        for g in ('MP_TargetSteerPos', 'frontier_path_graph_links', 'audio_frame_update', 'frontier_camera_scene_manager'):
            r = rows[(g, 'locked')]
            self.assertEqual(r['precision'], 1.0, g)
            self.assertGreaterEqual(r['recall'], 0.85, g)
        for r in rows.values():
            self.assertGreaterEqual(r['recall'], 0.0)
        locked = [r for r in rows.values() if r['kind'] == 'locked']
        self.assertGreaterEqual(len(locked), 17)

    def test_flagged_count_and_bounded_comparison(self):
        st = deps.flagged_stats(self.m)
        self.assertGreaterEqual(st['flagged'], 300)
        self.assertLessEqual(st['flagged'], 400)
        self.assertEqual(st['finite'], st['flagged'])
        self.assertGreater(st['within_cap'], st['bounded_discovery'])

    def test_tail_label_is_not_classified(self):
        self.assertEqual(self.m.classify('highscore_entry_anim')[0], 'TAIL')


class ClosureSynthetic(unittest.TestCase):
    def test_free_register_chain_goes_up_only_while_unrestored(self):
        def leaf(base, a):
            return [T.addiu('s3', 'zero', 1), T.JR_RA, T.NOP]
        def mid(base, a):
            return [T.addiu('sp', 'sp', -24), T.sw('ra', 20), T.jal(a.get('leaf', T.IMAGE_BASE)), T.NOP, T.lw('ra', 20), T.JR_RA, T.addiu('sp', 'sp', 24)]
        def top(base, a):
            return [T.addiu('sp', 'sp', -24), T.sw('ra', 20), T.jal(a.get('mid', T.IMAGE_BASE)), T.NOP, T.lw('ra', 20), T.JR_RA, T.addiu('sp', 'sp', 24)]
        def saver(base, a):
            return [T.addiu('sp', 'sp', -32), T.sw('ra', 28), T.sw('s3', 24), T.jal(a.get('top', T.IMAGE_BASE)), T.NOP,
                    T.lw('s3', 24), T.lw('ra', 28), T.JR_RA, T.addiu('sp', 'sp', 32)]
        def above(base, a):
            return [T.addiu('sp', 'sp', -24), T.sw('ra', 20), T.jal(a.get('saver', T.IMAGE_BASE)), T.NOP, T.lw('ra', 20), T.JR_RA, T.addiu('sp', 'sp', 24)]
        c, _ = T.build([('leaf', leaf), ('mid', mid), ('top', top), ('saver', saver), ('above', above)])
        m = deps.Model(c)
        direct, _ = m.minimal_closure(['leaf'], 'direct')
        self.assertEqual(sorted(direct), ['leaf', 'mid'])
        chain, _ = m.minimal_closure(['leaf'], 'chain')
        # mid and top leave s3 clobbered (transitively), saver saves/restores it so the chain stops there
        self.assertEqual(sorted(chain), ['leaf', 'mid', 'saver', 'top'])

    def test_callee_subtree_only_in_full_mode(self):
        def A(base, a):
            return T.caller_with_live_t2('mid')(base, a)
        def mid(base, a):
            return [T.addiu('sp', 'sp', -24), T.sw('ra', 20), T.jal(a.get('leaf', T.IMAGE_BASE)), T.NOP, T.lw('ra', 20), T.JR_RA, T.addiu('sp', 'sp', 24)]
        c, _ = T.build([('A', A), ('mid', mid), ('leaf', T.leaf_ret())])
        m = deps.Model(c)
        d, _ = m.minimal_closure(['A'], 'chain')
        self.assertEqual(sorted(d), ['A', 'mid'])
        f, _ = m.minimal_closure(['A'], 'full')
        self.assertEqual(sorted(f), ['A', 'leaf', 'mid'])


if __name__ == "__main__":
    unittest.main()
