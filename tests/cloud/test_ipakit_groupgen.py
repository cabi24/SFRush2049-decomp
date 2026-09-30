import json
import shutil
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from ipakit import Corpus, Func, IMAGE_BASE, deps, groupgen, sigs  # noqa: E402

IDO = ROOT / "tools" / "cloud" / "ido" / "cc"
HAVE_M2C = (ROOT / "tools" / "mips_to_c" / "m2c.py").exists() and shutil.which("mips-linux-gnu-objdump") is not None


class HelperTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which("mips-linux-gnu-objdump"), 'needs objdump')
    def test_disasm_prefers_registered_callee_over_alias(self):
        address = IMAGE_BASE + 16
        f = Func('caller', IMAGE_BASE, [(3 << 26) | ((address >> 2) & 0x3FFFFFF), 0, 0x03E00008, 0])
        callee = Func('canonical', address, [0x03E00008, 0])
        c = Corpus([f, callee], f.words + callee.words,
                   {'canonical': address, 'historical_alias': address})
        asm, _ = groupgen.disasm_for_m2c(c, f)
        self.assertRegex(asm, r'jal\s+canonical')
        self.assertNotIn('historical_alias', asm)

    def test_split_args_respects_nesting(self):
        self.assertEqual(groupgen.split_args("a, f(b, c), (d, e)[1], g"), ['a', 'f(b, c)', '(d, e)[1]', 'g'])
        self.assertEqual(groupgen.split_args(""), [])

    def test_find_calls_balanced(self):
        body = "void f(void) { g(1, h(2)); x = g(3); }"
        calls = groupgen.find_calls(body, 'g')
        self.assertEqual([c[2] for c in calls], ['1, h(2)', '3'])

    def test_reorder_calls_to_sigs_order(self):
        # callee (s1, a1, s2, a0): m2c passes ABI args first (a0, a1), then the IPA registers in sigs order (s1, s2)
        ev = {k: {'slot': None, 'kind': 'int', 'width': 'w', 'ptr': False, 'dbl': False, 'fl': False, 'narrow': None,
                  'store_narrow': None, 'notes': [], 'stack': False} for k in ('s1', 'a1', 's2', 'a0')}
        ev['a0']['slot'] = 3
        csig = sigs.build_sig('callee', ev)
        self.assertEqual([p.reg for p in csig.params], ['s1', 'a1', 's2', 'a0'])
        body = "void c(void) {\n    callee(A0, A1, S1, S2);\n}\n"
        new, rew, todo = groupgen.reorder_calls(body, 'callee', csig)
        self.assertIn('callee(S1, A1, S2, A0)', new)
        self.assertEqual((rew, todo), (1, 0))

    def test_reorder_skips_definition_and_prototype(self):
        ev = {k: {'slot': None, 'kind': 'int', 'width': 'w', 'ptr': False, 'dbl': False, 'fl': False, 'narrow': None,
                  'store_narrow': None, 'notes': [], 'stack': False} for k in ('t0', 'a0')}
        csig = sigs.build_sig('callee', ev)
        body = "void callee(s32 arg0, s32 ipa_t0) {\n}\nvoid callee(s32 arg0, s32 ipa_t0);\n"
        new, rew, todo = groupgen.reorder_calls(body, 'callee', csig)
        self.assertEqual(new, body)

    def test_declare_missing_locals(self):
        body = "void f(void) {\n    s32 sp10;\n    x = unksp48;\n    y = sp1A0[2];\n    z = sp10;\n}\n"
        out = groupgen.declare_missing(body)
        self.assertIn('s32 unksp48;', out)
        self.assertIn('s32 sp1A0[64];', out)
        self.assertEqual(out.count('s32 sp10;'), 1)

    def test_standin_text_two_sites(self):
        ev = {'a0': {'slot': None, 'kind': 'int', 'width': 'w', 'ptr': True, 'dbl': False, 'fl': False, 'narrow': None,
                     'store_narrow': None, 'notes': [], 'stack': False},
              'f14': {'slot': None, 'kind': 'int', 'width': 'w', 'ptr': False, 'dbl': False, 'fl': True, 'narrow': None,
                      'store_narrow': None, 'notes': [], 'stack': False}}
        sg = sigs.build_sig('callee', ev)
        t = groupgen.standin_text('callee', sg, 2)
        self.assertEqual(t.count('callee(0'), 2)
        self.assertIn('__standin_callee', t)

    def test_access_types(self):
        obs = [{'kind': 'access', 'mnemonic': 'lhu', 'address': 0x80100000}, {'kind': 'access', 'mnemonic': 'lhu', 'address': 0x80100000},
               {'kind': 'access', 'mnemonic': 'lw', 'address': 0x80100000}, {'kind': 'formation', 'mnemonic': 'addiu', 'address': 1}]
        self.assertEqual(groupgen.access_types(obs), {0x80100000: 'u16'})

    def test_raw_literal_becomes_extern(self):
        decls = {}
        out = groupgen.raw_to_externs("a = *(s16 *)0x80143A88; b = *(void *)0x801407FC;", {}, decls)
        self.assertEqual(out, "a = D_80143A88; b = D_801407FC;")
        self.assertEqual(decls['D_80143A88'][0], 's16')
        self.assertEqual(decls['D_801407FC'][0], 's32')


class PlanTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dm = deps.model_cache()

    def test_func_8008B640_chain_matches_real_group(self):
        plan = groupgen.make_plan(['func_8008B640'], 'chain', self.dm)
        real = json.loads((ROOT / 'cloud/work/ipa-groups/func_8008B640/group.json').read_text())
        self.assertEqual(set(plan.nodes), set(real['members']))
        self.assertEqual(set(plan.keep_list()), set(real['keep']))            # roots (address-taken) + stand-ins

    def test_standin_for_leaf_with_few_sites_and_none_for_heavy_leaf(self):
        plan = groupgen.make_plan(['audio_helper'], 'chain', self.dm)
        self.assertNotIn('audio_helper', plan.standins)                      # several in-group call sites
        self.assertNotIn('audio_helper', plan.keep)                          # an IPA leaf is never a root
        plan = groupgen.make_plan(['MP_TargetSteerPos'], 'chain', self.dm)
        self.assertEqual(plan.standins, ['MP_TargetSteerPos'])
        self.assertEqual(plan.keep, ['camera_transform'])

    def test_abi_roots_go_in_keep(self):
        plan = groupgen.make_plan(['func_800EA3F4'], 'chain', self.dm)
        self.assertEqual(plan.keep, ['func_800EA3F4'])
        self.assertEqual(plan.standins, [])

    def test_registered_head_uses_section_target(self):
        plan = groupgen.make_plan(['func_80107EDC'], 'chain', self.dm)
        self.assertNotIn('func_80107EDC', plan.targets)
        f = self.dm.corpus.funcs['func_80107EDC']
        self.assertFalse(f.discovered)
        self.assertEqual((f.addr, len(f.words)), (0x80107EDC, 158))
        self.assertIn('slot_state_setup', plan.nodes)
        self.assertIn('func_80107EDC', plan.keep)                            # address-taken (descriptor table)

    def test_unknown_seed_exits(self):
        with self.assertRaises(SystemExit):
            groupgen.make_plan(['not_a_function'], 'direct', self.dm)

    def test_group_json_shape(self):
        plan = groupgen.make_plan(['MP_TargetSteerPos'], 'direct', self.dm)
        j = groupgen.group_json(plan, 'x')
        self.assertEqual(j['flags'], '-g0 -O3 -mips2 -G 0 -non_shared')
        self.assertEqual(j['claims'], [])
        self.assertEqual(j['files'], ['group.c'])
        self.assertIn('__standin_MP_TargetSteerPos', j['keep'])


class EmitTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dm = deps.model_cache()
        cls.sm = sigs.SigModel(cls.dm.corpus, cls.dm.infos)

    def test_emit_stub_mode_offline(self):
        plan = groupgen.make_plan(['func_800B9B64'], 'chain', self.dm)
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / 'g'
            info = groupgen.emit(plan, out, self.dm, self.sm, m2c='off')
            text = (out / 'group.c').read_text()
            j = json.loads((out / 'group.json').read_text())
            self.assertEqual(j['members'], ['func_800B9B64', 'physics_friction_apply'])
            self.assertIn('__standin_func_800B9B64', j['keep'])
            self.assertIn('TODO: decompile func_800B9B64', text)
            self.assertIn('func_800B9B64(', text)
            self.assertEqual(text.count('func_800B9B64(0, 0, 0, 0, 0);'), 2)   # two stand-in call sites
            self.assertFalse(any(info['m2c'].values()))
            with self.assertRaises(SystemExit):
                groupgen.emit(plan, out, self.dm, self.sm, m2c='off')          # refuses to overwrite

    @unittest.skipUnless(IDO.exists(), 'IDO not installed (tools/cloud/setup.sh)')
    def test_stub_group_compiles(self):
        plan = groupgen.make_plan(['func_800B9B64'], 'chain', self.dm)
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / 'g'
            groupgen.emit(plan, out, self.dm, self.sm, m2c='off')
            res = groupgen.try_compile(out)
            self.assertIsNone(res['err'], res['err'])
            self.assertEqual(set(res['members']), {'func_800B9B64', 'physics_friction_apply'})

    @unittest.skipUnless(IDO.exists() and HAVE_M2C, 'needs IDO, tools/mips_to_c and mips-linux-gnu-objdump')
    def test_m2c_group_compiles_with_right_call_order(self):
        plan = groupgen.make_plan(['func_800B9B64'], 'chain', self.dm)
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / 'g'
            info, res = groupgen.emit_and_compile(plan, out, self.dm, self.sm)
            self.assertIsNone(res['err'], res['err'])
            text = (out / 'group.c').read_text()
            self.assertIn('ipa_t0', text)
            self.assertGreaterEqual(info['reordered_calls'], 1)

    @unittest.skipUnless(IDO.exists() and HAVE_M2C, 'needs IDO, tools/mips_to_c and mips-linux-gnu-objdump')
    def test_head_seed_builds_with_targets(self):
        plan = groupgen.make_plan(['func_80107EDC'], 'chain', self.dm)
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / 'g'
            info, res = groupgen.emit_and_compile(plan, out, self.dm, self.sm)
            self.assertIsNone(res['err'], res['err'])
            self.assertIn('func_80107EDC', res['members'])
            self.assertEqual(res['members']['func_80107EDC']['target_size'], 158)


class CompareTests(unittest.TestCase):
    def test_compare_func_8008B640_agrees(self):
        row = groupgen.compare_group('func_8008B640', 'func_8008B640', 'chain')
        self.assertEqual(row['members_missing'], [])
        self.assertEqual(row['members_extra'], [])
        self.assertEqual(row['keep_disagree_on_common'], [])
        self.assertEqual(row['standin_disagree_on_common'], [])
        self.assertTrue(all(p['arity'] for p in row['protos']))

    def test_compare_summary_text(self):
        row = groupgen.compare_group('MP_TargetSteerPos', 'members', 'chain')
        self.assertIn('membership', groupgen.compare_summary([row]))


if __name__ == '__main__':
    unittest.main()
