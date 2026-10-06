"""Mandatory identity and fail-closed research proof replay."""
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import unittest
P=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('initializer_verify',P/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
class PacketTests(unittest.TestCase):
    def test_saved_inputs(self):
        r=json.loads((P/'verification.json').read_text());self.assertEqual(v.sha(v.SOURCE),r['source_sha256'])
        for p,h in r['packet_sha256'].items():self.assertEqual(v.sha(P/p),h)
        for p,h in r['inputs_sha256'].items():self.assertEqual(v.sha(v.ROOT/p),h)
    def test_high_boundary(self):self.assertEqual([v.signed(x-1,8) for x in [0,1,128,129,255]],[-1,0,127,-128,-2])
    def test_memory_bounds(self):
        m=v.fixture(0,0)
        with self.assertRaises(AssertionError):m.get(0,4)
        with self.assertRaises(AssertionError):m.put(v.COUNT+1,0,4)
    def test_optimized_python_rejected(self):
        r=subprocess.run([sys.executable,'-O',str(P/'verify.py'),'--check'],capture_output=True,text=True)
        self.assertNotEqual(r.returncode,0);self.assertIn('assertions enabled',r.stderr)
    def test_full_frozen_replay(self):self.assertEqual(v.prove(),json.loads((P/'verification.json').read_text()))
if __name__=='__main__':unittest.main()
