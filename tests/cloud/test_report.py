import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from amatch import report, search  # noqa: E402

NEED_IDO = unittest.skipUnless((ROOT / "tools" / "cloud" / "ido" / "cc").exists(),
                               "IDO missing (tools/cloud/setup.sh)")
CORPUS = ROOT / "cloud" / "work" / "tools" / "amatch" / "corpus.py"

ADDIU_SP_M32 = 0x27BDFFE0
SW_RA_28 = 0xAFBF001C
SW_S0_16 = 0xAFB00010
LW_A0 = 0x8FA40018
LW_A1 = 0x8FA50018
JR_RA = 0x03E00008
NOP = 0


class Regs(unittest.TestCase):
    def test_regs_used(self):
        g, f = report.regs_used(0x00851021)              # addu v0,a0,a1
        self.assertEqual(g, {2, 4, 5})
        g, f = report.regs_used(0xC4A40000)              # lwc1 f4,0(a1)
        self.assertEqual((g, f), ({5}, {4}))
        self.assertEqual(report.regs_used(JR_RA)[0], {31})

    def test_frame_info(self):
        frame, saved = report.frame_info([ADDIU_SP_M32, SW_RA_28, SW_S0_16, NOP])
        self.assertEqual(frame, 32)
        self.assertEqual(saved, [("ra", 0x1C), ("s0", 0x10)])


class Sections(unittest.TestCase):
    def test_frame_section_flags_delta(self):
        want = [ADDIU_SP_M32, SW_RA_28, LW_A0, JR_RA, NOP]
        got = [0x27BDFFD8, SW_RA_28, LW_A1, JR_RA, NOP]
        txt = "\n".join(report.sec_frame(want, got))
        self.assertIn("target 32 bytes, ours 40 bytes", txt)
        self.assertIn("delta +8", txt)
        self.assertIn("target-only", txt)
        self.assertIn("a0", txt)

    def test_hunks_limit_and_register_tally(self):
        want = [LW_A0] * 3 + [JR_RA, NOP] + [LW_A0] * 40
        got = [LW_A1] * 3 + [JR_RA, NOP] + [LW_A1] * 40
        txt = "\n".join(report.sec_hunks(want, got, 10))
        self.assertIn("a0->a1 x43", txt)
        self.assertLessEqual(txt.count("\n- ["), 10)

    def test_no_diff(self):
        w = [LW_A0, JR_RA, NOP]
        self.assertIn("No aligned differences", "\n".join(report.sec_hunks(w, list(w), 10)))

    def test_inserted_word_reported(self):
        want = [LW_A0, JR_RA, NOP]
        got = [LW_A0, LW_A1, JR_RA, NOP]
        txt = "\n".join(report.sec_hunks(want, got, 20))
        self.assertIn("1 extra words in ours", txt)


class Tried(unittest.TestCase):
    LOG = {"kind": "fn", "evals": 5, "seconds": 1.0,
           "baseline": {"strict_diff": 10, "aligned_exact": 5, "size": 9, "target_size": 9},
           "best": {"path": ["loop-form:for"], "score": {}},
           "class_stats": {"loop-form": {"tried": 2, "improved": 1, "best_delta": 3.0, "weight": 1.6}},
           "candidates": [
               {"hash": "a", "path": [], "score": {"strict_diff": 10, "aligned_exact": 5}},
               {"hash": "b", "path": ["loop-form:for"], "score": {"strict_diff": 4, "aligned_exact": 8}},
               {"hash": "c", "path": ["decl-order:1"], "score": {"strict_diff": 12, "aligned_exact": 4}},
               {"hash": "d", "path": ["x"], "score": {"err": "boom"}},
           ]}

    def test_tried_lists_best_delta(self):
        txt = "\n".join(report.sec_tried(self.LOG))
        self.assertIn("`loop-form:for`: aligned +3, strict +6", txt)
        self.assertIn("1 compile errors", txt)
        self.assertIn("| loop-form | 2 | 1 |", txt)

    def test_untried_uses_mutator(self):
        class M:
            @staticmethod
            def mutations(src, fn=None):
                return [search_mut("loop-form:x"), search_mut("pad-local:y"), search_mut("pad-local:z")]

        def search_mut(n):
            return {"name": n}
        txt = "\n".join(report.sec_untried(self.LOG, "int x;", "f", M))
        self.assertIn("pad-local (2 sites)", txt)
        self.assertNotIn("loop-form (", txt)

    def test_untried_static_fallback(self):
        class Broken:
            @staticmethod
            def mutations(src, fn=None):
                raise RuntimeError("no")
        txt = "\n".join(report.sec_untried(self.LOG, "int x;", "f", Broken))
        self.assertIn("pad-local", txt)

    def test_no_log(self):
        self.assertIn("No search history", "\n".join(report.sec_tried(None)))


class Real(unittest.TestCase):
    @NEED_IDO
    def test_report_from_search_dir_and_bare(self):
        with tempfile.TemporaryDirectory() as tmp:
            f = Path(tmp) / "s.c"
            r = subprocess.run([sys.executable, str(CORPUS), "materialize", "func_8008AD04", "start",
                                "--out", str(f)], capture_output=True, text=True, cwd=ROOT)
            if r.returncode != 0:
                self.skipTest("corpus unavailable")
            flags = "-g0 -O2 -mips2 -G 0 -non_shared"
            bare = report.report_fn(f.read_text(), "func_8008AD04", flags, None, 30)
            self.assertIn("# Residual report: func_8008AD04", bare)
            self.assertIn("## Aligned diff", bare)
            self.assertIn("a1->a2", bare)

            class Noise:
                @staticmethod
                def mutations(src, fn=None, catalog=None):
                    return [{"name": "noise:ws", "new_src": src + "\n", "cost": 1.0}]
            res = search.run_fn(f.read_text(), "func_8008AD04", flags, budget=4, jobs=1,
                                out=Path(tmp) / "o", mutator=Noise)
            txt = report.report_dir(Path(tmp) / "o", 30, Noise)
            self.assertIn("Mutations tried", txt)
            self.assertIn("noise", txt)
            self.assertFalse(res["matched"])


if __name__ == "__main__":
    unittest.main()
