import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from ipakit import sigs  # noqa: E402


class ParseTests(unittest.TestCase):
    def test_parse_definition_types(self):
        src = "static u8 *foo(s32 a, f32 b, void (*cb)(int), u16 c[4]) {\n return 0;\n}\n"
        ret, ps, kr = sigs.parse_definition(src, 'foo')
        self.assertFalse(kr)
        self.assertEqual([sigs.type_class(t) for t, _ in ps], ['w', 'f32', 'ptr', 'ptr'])
        self.assertEqual(sigs._ret_class(ret), 'int')

    def test_parse_void_and_kr(self):
        self.assertEqual(sigs.parse_definition("void f(void) { }", 'f')[1:], ([], False))
        self.assertEqual(sigs.parse_definition("void f() { }", 'f')[1:], ([], True))
        self.assertIsNone(sigs.parse_definition("void g(void);", 'f'))

    def test_type_class(self):
        for t, c in (('s16', 's16'), ('u8', 'u8'), ('unsigned short', 'u16'), ('f64', 'f64'), ('Foo *', 'ptr'),
                     ('const char *', 'ptr'), ('s32', 'w'), ('u32', 'w'), ('signed char', 's8')):
            self.assertEqual(sigs.type_class(t), c, t)

    def test_reg_of_name(self):
        self.assertEqual(sigs.reg_of_name('arg2', 2), 'a2')
        self.assertEqual(sigs.reg_of_name('ipa_s1', 0), 's1')
        self.assertIsNone(sigs.reg_of_name('node', 0))


class BuildSigTests(unittest.TestCase):
    """build_sig on hand-made evidence: ordering by home slot, natural slots, register-order fill."""

    @staticmethod
    def ev(**kw):
        d = {'slot': None, 'kind': 'int', 'width': 'w', 'ptr': False, 'dbl': False, 'fl': False, 'narrow': None,
             'store_narrow': None, 'notes': [], 'stack': False}
        d.update(kw)
        return d

    def test_home_slot_gives_order_not_register_name(self):
        # (pos=a0, t2 slot 1, t5 slot 2, a1 slot 3, t0 slot 4): func_800E4300
        ev = {'a0': self.ev(ptr=True), 't2': self.ev(slot=1, narrow='s16'), 't5': self.ev(slot=2, narrow='s16'),
              'a1': self.ev(slot=3, narrow='s16'), 't0': self.ev(slot=4, narrow='s16')}
        s = sigs.build_sig('f', ev)
        self.assertEqual([p.reg for p in s.params], ['a0', 't2', 't5', 'a1', 't0'])
        self.assertEqual(s.prototype(), 'void f(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);')
        self.assertEqual(s.ipa_regs, sorted(['t2', 't5', 't0'], key=sigs._reg_rank))

    def test_slot_beyond_four_is_a_register_param(self):
        # physics_velocity_integrate_a: s1, a1, s2, f20, f22, f24, a0 stored to slot 6
        ev = {'s1': self.ev(ptr=True), 'a1': self.ev(), 's2': self.ev(), 'f20': self.ev(), 'f22': self.ev(),
              'f24': self.ev(), 'a0': self.ev(slot=6, narrow='s16')}
        s = sigs.build_sig('g', ev)
        self.assertEqual([p.reg for p in s.params], ['s1', 'a1', 's2', 'f20', 'f22', 'f24', 'a0'])
        self.assertEqual([p.ctype() for p in s.params], ['void *', 's32', 's32', 'f32', 'f32', 'f32', 's16'])
        self.assertIn('ipa_s1', s.prototype('m2c'))
        self.assertIn('s16 arg0)', s.prototype('m2c'))

    def test_ipa_abi_register_not_bound_to_natural_slot(self):
        # model_bounds_calc: s0 (flag) and a3 (obj): a3 is parameter 1, not 3
        ev = {'s0': self.ev(), 'a3': self.ev(ptr=True)}
        s = sigs.build_sig('h', ev)
        self.assertEqual([p.reg for p in s.params], ['s0', 'a3'])

    def test_pure_abi_keeps_natural_slots_and_gaps(self):
        s = sigs.build_sig('k', {'a2': self.ev()})
        self.assertEqual([p.slot for p in s.params], [0, 1, 2])
        self.assertEqual([p.reg for p in s.params], [None, None, 'a2'])

    def test_float_and_double(self):
        s = sigs.build_sig('m', {'f12': self.ev(dbl=True), 'f14': self.ev(), 'a2': self.ev()})
        self.assertEqual([p.ctype() for p in s.params], ['f64', 'f32', 's32'])
        self.assertEqual([p.slot for p in s.params], [0, 2, 3])


class CorpusTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.m = sigs.model_cache()

    def test_narrow_home_store(self):
        s = self.m.sig('func_8008A38C')                    # u16 arg0 (andi 0xffff, home store)
        self.assertEqual(s.prototype(), 'void func_8008A38C(u16 arg0);')
        self.assertEqual(s.params[0].slot, 0)

    def test_ipa_leaf_register_order(self):
        s = self.m.sig('physics_velocity_integrate_a')
        self.assertEqual([p.reg for p in s.params], ['s1', 'a1', 's2', 'f20', 'f22', 'f24', 'a0'])
        self.assertEqual(s.params[-1].ctype(), 's16')
        s = self.m.sig('func_800E4300')
        self.assertEqual([p.reg for p in s.params][1:], ['t2', 't5', 'a1', 't0'])

    def test_stack_arguments(self):
        s = self.m.sig('minimap_render')
        self.assertEqual(len(s.params), 7)
        self.assertEqual(s.ret, 's32')

    def test_return_types_from_callers(self):
        self.assertEqual(self.m.sig('func_800B98D8').ret, 's32')
        self.assertEqual(self.m.sig('func_8008A38C').ret, 'void')
        self.assertEqual(self.m.sig('func_800F92C8').ret, 'f32')

    def test_path_correlated_non_abi_read_is_not_a_param(self):
        self.assertFalse(self.m.sig('camera_reset').ipa_regs)

    def test_validation_accuracy_floor(self):
        rows = sigs.validate(self.m)
        self.assertGreater(len(rows), 300)
        summ = sigs.summarize(rows)['all']
        a, b = summ['arity']
        self.assertGreater(a / b, 0.94)
        a, b = summ['class relaxed (ptr~int)']
        self.assertGreater(a / b, 0.93)
        a, b = summ['return class']
        self.assertGreater(a / b, 0.94)


if __name__ == '__main__':
    unittest.main()
