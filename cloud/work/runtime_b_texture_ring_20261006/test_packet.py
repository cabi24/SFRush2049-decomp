"""Focused mandatory replay and identity tests for image B's texture-ring init."""
import importlib.util
import json
from pathlib import Path
import unittest
import shutil
import pytest

PACKET=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('texture_ring_verify',PACKET/'verify.py')
v=importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)

class PacketTests(unittest.TestCase):
    def test_saved_inputs(self):
        receipt=json.loads((PACKET/'verification.json').read_text())
        self.assertEqual(receipt['source_sha256'],v.sha(v.SOURCE))
        self.assertEqual((receipt['image'],receipt['address'],receipt['bytes']),('B','0x8038a8cc',144))
        self.assertNotIn('inputs_sha256', receipt)
        for path,digest in receipt['packet_sha256'].items():self.assertEqual(v.sha(PACKET/path),digest)
    def test_count_narrowing(self):
        self.assertEqual([v.signed(n-1,8) for n in [0,1,128,129,255]],[-1,0,127,-128,-2])
    def test_helper_effect_survives(self):
        final,snapshot=v.expected(129,15)
        self.assertEqual(len(snapshot),136)
        self.assertEqual(len(final),136)
        self.assertEqual(snapshot[v.RING],0)
        self.assertEqual(v.get(snapshot,v.RING+2,2),0)
        self.assertEqual(final[v.RING],7)
        self.assertEqual(v.get(final,v.RING+2,2),15*4097)
        self.assertEqual(final[v.COUNT],242)
        for i in range(25):self.assertEqual(v.get(final,v.RING+4+i*4,4),0)
    def test_unmapped_and_misaligned_memory_rejected(self):
        with self.assertRaises(AssertionError):v.get({},0,4)
        with self.assertRaises(AssertionError):v.put({},0,1,4)
        with self.assertRaises(AssertionError):v.get({0:0,1:0,2:0,3:0,4:0},1,4)
    def test_unknown_instruction_rejected(self):
        with self.assertRaises(AssertionError):v.execute([0xffffffff]*36,0,0)
    def test_exact_full_replay(self):
        if not (v.score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
            pytest.skip('pinned IDO and MIPS GNU linker required')
        self.assertEqual(v.portable_receipt(v.prove()),v.portable_receipt(json.loads((PACKET/'verification.json').read_text())))

if __name__=='__main__':unittest.main()
