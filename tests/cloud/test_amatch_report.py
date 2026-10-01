"""Residual reports remain readable on workers without MIPS binutils."""
import subprocess
import sys
import unittest
from pathlib import Path
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from amatch import report  # noqa: E402


class PortableDisassembly(unittest.TestCase):
    def setUp(self):
        self.old_cache = dict(report._dis_cache)
        report._dis_cache.clear()

    def tearDown(self):
        report._dis_cache.clear()
        report._dis_cache.update(self.old_cache)

    def test_frame_sections_without_binutils(self):
        want = [0x27BDFFE0, 0xAFBF001C, 0x8FBF001C, 0x03E00008, 0x27BD0020]
        got = [0x27BDFFD8, 0xAFBF0024, 0x8FBF0024, 0x03E00008, 0x27BD0028]
        with patch.object(report.score, "_run", side_effect=FileNotFoundError), \
                patch.object(report.score, "disasm_word", return_value=""):
            text = "\n".join(report.sec_frame(want, got))
        prologue, epilogue = text.split("target epilogue | ours epilogue")
        for section in (prologue, epilogue):
            self.assertIn("addiu sp,sp,-32", section)
            self.assertIn("sw ra,28(sp)", section)
            self.assertIn("lw ra,36(sp)", section)
            self.assertIn("jr ra", section)
            self.assertIn("addiu sp,sp,40", section)

    def test_partial_objdump_keeps_native_text_and_decodes_missing_words(self):
        result = subprocess.CompletedProcess([], 0, "  0: 03e00008  jr ra\n", "")
        with patch.object(report.score, "_run", return_value=result), \
                patch.object(report.score, "disasm_word", return_value=""):
            decoded = report.disasm([0x03E00008, 0x27BDFFE0, None])
        self.assertEqual(decoded[0x03E00008], "jr ra")
        self.assertEqual(decoded[0x27BDFFE0], "addiu sp,sp,-32")
        self.assertNotIn(None, decoded)

    def test_float_saves_and_negative_offsets(self):
        self.assertEqual(report._portable_disasm(0xF7B40018), "sdc1 f20,24(sp)")
        self.assertEqual(report._portable_disasm(0xD7B40018), "ldc1 f20,24(sp)")
        self.assertEqual(report._portable_disasm(0x8FA4FFFC), "lw a0,-4(sp)")
        self.assertEqual(report._portable_disasm(0), "nop")

    def test_control_flow_fallback_does_not_invent_cached_pc_targets(self):
        text = report._portable_disasm(0x10850003)  # beq a0,a1,PC+16
        self.assertEqual(text, "beq (uses=a0,a1; defs=none)")


if __name__ == "__main__":
    unittest.main()
