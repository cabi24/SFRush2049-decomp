import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from ipakit import mipsdec, load_corpus  # noqa: E402
from ipakit.mipsdec import decode, mask_names  # noqa: E402

PC = 0x80100000


def d(word, pc=PC):
    return decode(word, pc)


def rtype(rs, rt, rd, fn, sa=0):
    return (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | fn


def itype(op, rs, rt, imm):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


class DecodeTests(unittest.TestCase):
    def test_addiu_sp(self):
        i = d(0x27BDFFE8)
        self.assertEqual(i.mnem, "addiu")
        self.assertEqual(mask_names(i.uses), ["sp"])
        self.assertEqual(mask_names(i.defs), ["sp"])
        self.assertEqual(i.imm, -24)

    def test_store_reads_both_registers(self):
        i = d(0xAFBF0014)                      # sw ra,20(sp)
        self.assertEqual(i.kind, "store")
        self.assertEqual(mask_names(i.uses), ["sp", "ra"])
        self.assertEqual(i.defs, 0)
        self.assertEqual(i.mem, (29, 20))

    def test_load_writes_rt(self):
        i = d(itype(35, 29, 16, 8))            # lw s0,8(sp)
        self.assertEqual(mask_names(i.defs), ["s0"])
        self.assertEqual(i.kind, "load")

    def test_lwl_merges_old_value(self):
        i = d(itype(34, 4, 8, 0))              # lwl t0,0(a0)
        self.assertEqual(mask_names(i.uses), ["a0", "t0"])

    def test_zero_register_is_never_a_def_or_use(self):
        i = d(rtype(0, 0, 4, 0x25))            # or a0,zero,zero (clear)
        self.assertEqual(i.uses, 0)
        self.assertEqual(mask_names(i.defs), ["a0"])
        self.assertEqual(d(0).mnem, "nop")

    def test_jal_target_and_call_kind(self):
        i = d(0x0C021FA0, 0x80000004)
        self.assertEqual((i.kind, i.target, i.delay), ("jal", 0x80087E80, True))
        self.assertEqual(mask_names(i.defs), ["ra"])

    def test_jr_ra_is_ret_other_jr_is_indirect(self):
        self.assertEqual(d(0x03E00008).kind, "ret")
        i = d(rtype(25, 0, 0, 8))              # jr t9
        self.assertEqual(i.kind, "jr")
        self.assertEqual(mask_names(i.uses), ["t9"])

    def test_jalr(self):
        i = d(rtype(25, 0, 31, 9))             # jalr ra,t9
        self.assertEqual(i.kind, "jalr")
        self.assertEqual(mask_names(i.uses), ["t9"])
        self.assertTrue(i.delay)

    def test_branch_target_and_likely(self):
        b = d(itype(4, 4, 5, 3))               # beq a0,a1,+3
        self.assertEqual((b.kind, b.target, b.likely), ("branch", PC + 4 + 12, False))
        bl = d(itype(20, 4, 0, 0xFFFE))        # beql a0,zero,-2
        self.assertEqual((bl.kind, bl.target, bl.likely), ("branch", PC + 4 - 8, True))
        self.assertEqual(bl.mnem, "beql")
        self.assertEqual(d(itype(21, 4, 5, 1)).likely, True)       # bnel
        self.assertEqual(d(0x4500FFFE).likely, False)               # bc1f
        self.assertEqual(d(0x4503FFFE).likely, True)                # bc1tl
        self.assertEqual(mask_names(d(0x4500FFFE).uses), ["fcc"])

    def test_unconditional_branch(self):
        self.assertEqual(d(0x10000003).kind, "b")                   # beq zero,zero
        self.assertEqual(d(itype(4, 4, 4, 3)).kind, "b")            # beq a0,a0

    def test_bal_writes_ra(self):
        i = d(itype(1, 0, 17, 2))                                   # bgezal zero (bal)
        self.assertEqual(mask_names(i.defs), ["ra"])

    def test_fp_single_and_double_pairs(self):
        mul_s = d(0x46006302)                  # mul.s f12,f12,f0
        self.assertEqual(mask_names(mul_s.uses), ["f0", "f12"])
        self.assertEqual(mask_names(mul_s.defs), ["f12"])
        add_d = d((17 << 26) | (17 << 21) | (14 << 16) | (12 << 11) | (2 << 6))   # add.d f2,f12,f14
        self.assertEqual(mask_names(add_d.uses), ["f12", "f13", "f14", "f15"])
        self.assertEqual(mask_names(add_d.defs), ["f2", "f3"])
        ldc1 = d(itype(53, 29, 20, 24))
        self.assertEqual(mask_names(ldc1.defs), ["f20", "f21"])
        sdc1 = d(itype(61, 29, 20, 24))
        self.assertEqual(mask_names(sdc1.uses), ["sp", "f20", "f21"])
        self.assertEqual(mask_names(d(itype(57, 29, 14, 4)).uses), ["sp", "f14"])   # swc1

    def test_fp_moves_and_compare(self):
        mtc1 = d((17 << 26) | (4 << 21) | (1 << 16) | (24 << 11))     # mtc1 at,f24
        self.assertEqual(mask_names(mtc1.uses), ["at"])
        self.assertEqual(mask_names(mtc1.defs), ["f24"])
        mfc1 = d((17 << 26) | (0 << 21) | (5 << 16) | (6 << 11))      # mfc1 a1,f6
        self.assertEqual(mask_names(mfc1.defs), ["a1"])
        self.assertEqual(mask_names(mfc1.uses), ["f6"])
        c = d((17 << 26) | (16 << 21) | (14 << 16) | (12 << 11) | 0x3C)   # c.lt.s f12,f14
        self.assertEqual(mask_names(c.defs), ["fcc"])
        self.assertEqual(c.mnem, "c.lt.s")

    def test_mult_div_hilo(self):
        m = d(rtype(4, 5, 0, 0x19))            # multu a0,a1
        self.assertEqual(mask_names(m.defs), ["hi", "lo"])
        self.assertEqual(mask_names(d(rtype(0, 0, 2, 0x12)).uses), ["lo"])   # mflo v0

    def test_shifts(self):
        s = d(rtype(0, 4, 8, 0, 2))            # sll t0,a0,2
        self.assertEqual((mask_names(s.uses), mask_names(s.defs)), (["a0"], ["t0"]))
        v = d(rtype(5, 4, 8, 4))               # sllv t0,a0,a1
        self.assertEqual(mask_names(v.uses), ["a0", "a1"])

    def test_no_unknown_opcodes_in_game_code(self):
        c = load_corpus(discover=False)
        bad = {}
        for f in c.funcs.values():
            for k, w in enumerate(f.words):
                i = decode(w, f.addr + 4 * k)
                if i.mnem.startswith("?"):
                    bad.setdefault(f.name, []).append(hex(w))
        self.assertEqual(bad, {})

    def test_delay_flag_is_set_exactly_on_control_transfers(self):
        c = load_corpus(discover=False)
        f = c.funcs["slot_state_setup"]
        kinds = {decode(w, f.addr + 4 * k).kind for k, w in enumerate(f.words) if decode(w, 0).delay}
        self.assertTrue(kinds <= {"branch", "b", "j", "jal", "jalr", "ret", "jr"})
        self.assertIn("jal", kinds)


class JumpTableTests(unittest.TestCase):
    def test_switch_tables_resolve_inside_their_function(self):
        from ipakit import analyze
        c = load_corpus(discover=False)
        total = 0
        for f in c.funcs.values():
            ins = mipsdec.decode_words(f.words, f.addr)
            for k, i in enumerate(ins):
                if i.kind == "jr":
                    tb = mipsdec.jump_table(ins, k, c.word_at, f.addr, f.end)
                    self.assertIsNotNone(tb, f.name)
                    self.assertTrue(all(f.addr <= t < f.end and t % 4 == 0 for t in tb))
                    total += 1
        self.assertGreater(total, 20)


if __name__ == "__main__":
    unittest.main()
