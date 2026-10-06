"""Focused tests; no private ROM or image inputs required."""
import importlib.util
import json
from pathlib import Path
import unittest
spec=importlib.util.spec_from_file_location('packet',Path(__file__).with_name('verify.py'))
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
class Packet(unittest.TestCase):
    def test_case_count(self):self.assertEqual(len(list(v.cases())),18432)
    def test_signed_count_boundary(self):
        _,t,_=v.oracle(255,0,0,0);self.assertEqual(t[0][1],[0,0xfffffff8])
    def test_threshold_tie(self):
        c,var,seed,reuse=next(x for x in v.cases() if x[1]==1)
        m,_,_=v.oracle(c,var,seed,reuse)
        self.assertEqual((v.get(m,v.SEED,4)>>16)&32767,16384)
    def test_complete_proof(self):
        frozen=json.loads((v.PACKET/'verification.json').read_text())
        self.assertEqual(v.prove(),frozen)
if __name__=='__main__':unittest.main()
