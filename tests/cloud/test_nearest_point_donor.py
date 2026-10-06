"""Source-bound negative nearest-point donor experiment."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import unittest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT/'cloud/work/frontier/dot_nearest_point_donor_20261005'

def module():
    spec=importlib.util.spec_from_file_location('nearest_point_donor',HERE/'verify.py')
    loaded=importlib.util.module_from_spec(spec);spec.loader.exec_module(loaded)
    return loaded

class NearestPointDonorTests(unittest.TestCase):
    def setUp(self):
        self.receipt=json.loads((HERE/'verification.json').read_text())

    def test_source_and_input_hashes(self):
        for name,expected in self.receipt['source_files'].items():
            self.assertEqual(hashlib.sha256((HERE/name).read_bytes()).hexdigest(),expected)
        for name,expected in self.receipt['input_files'].items():
            if name=='asm/us/blob/SHA256SUMS':
                continue    # changes with every splice: frozen provenance, not a lock
            self.assertEqual(hashlib.sha256((ROOT/name).read_bytes()).hexdigest(),expected)
        self.assertEqual(hashlib.sha256((HERE/'verify.py').read_bytes()).hexdigest(),self.receipt['verifier_sha256'])

    def test_no_matching_or_behavioral_claim(self):
        self.assertEqual(self.receipt['claims'],[])
        self.assertEqual(self.receipt['accepted_byte_gain'],0)
        self.assertFalse(self.receipt['behavioral_proof'])
        self.assertEqual(self.receipt['target_bytes'],196)
        self.assertEqual(self.receipt['caller_sites'],[dict(caller='stunt_combo_display',address='0x800D4474')])
        for row in self.receipt['experiments'].values():
            self.assertFalse(row['strict_match'])
            self.assertGreater(row['full_target_differing'],0)
            self.assertTrue(row['full_body_gnu_equal'])
            self.assertEqual(row['relocations'],4)

    def test_hypothesis_and_exact_excess_are_rejected(self):
        x=self.receipt['experiments']
        for n in ['vector_macro_unsigned_O3','vector_dotprod_unsigned_group']:
            self.assertEqual((x[n]['elf_bytes'],x[n]['frame_bytes'],x[n]['full_target_differing']),(216,16,43))
        for n in ['vector_macro_signed_O3','vector_dotprod_signed_group']:
            self.assertEqual((x[n]['elf_bytes'],x[n]['excess_elf_words']),(816,155))
            self.assertGreater(x[n]['excess_elf_words'],x[n]['canonical']['extra_words'])

    def test_no_automatic_match_submission(self):
        from tools.cloud.check_submissions import commands
        self.assertEqual(list(commands(ROOT,[str(p.relative_to(ROOT)) for p in HERE.iterdir() if p.is_file()])),[])

    def test_fresh_compiler_and_gnu_replay(self):
        mod=module()
        if not (mod.score.IDO/'cc').exists() or not shutil.which('mips-linux-gnu-ld'):
            self.skipTest('pinned IDO and MIPS GNU linker required')
        fresh,saved=mod.verify(),dict(self.receipt)
        for value in (fresh,saved):
            value['input_files']={k:h for k,h in value['input_files'].items()
                                  if k!='asm/us/blob/SHA256SUMS'}
        self.assertEqual(fresh,saved)
