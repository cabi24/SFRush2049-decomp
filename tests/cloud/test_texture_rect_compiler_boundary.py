"""Receipt/source bindings and strict diagnostic ineligibility; no compiler required."""
import hashlib
import json
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/texture_rect_compiler_boundary'

class ReceiptTests(unittest.TestCase):
    def setUp(self):
        self.verification = json.loads((HERE/'verification.json').read_text())
        self.allocation = json.loads((HERE/'allocation_receipt.json').read_text())

    def test_frozen_baseline_binding(self):
        receipt = self.verification
        source = ROOT/receipt['source']
        self.assertEqual(hashlib.sha256(source.read_bytes()).hexdigest(), receipt['source_sha256'])
        self.assertEqual(receipt['baseline']['differing'], 4)
        self.assertEqual(receipt['baseline']['total'], 445)

    def test_complete_source_bindings(self):
        self.assertEqual(len(self.verification['source_controls']), 3)
        for receipt in self.verification['source_controls']:
            source = ROOT/receipt['source']
            digest = hashlib.sha256(source.read_bytes()).hexdigest()
            self.assertEqual(digest, receipt['source_sha256'])
            self.assertEqual(digest, self.allocation[source.stem]['source_sha256'])
            self.assertEqual(receipt['comparison']['differing'],
                             self.allocation[source.stem]['source_comparison_differing'])

    def test_no_diagnostic_or_source_is_accepted(self):
        self.assertFalse(self.verification['accepted'])
        for receipt in self.verification['controls']:
            self.assertTrue(receipt['diagnostic_only'])
            self.assertFalse(receipt['eligible_for_promotion'])
        for receipt in self.verification['source_controls']:
            self.assertEqual(receipt['input_kind'], 'C source')
            self.assertFalse(receipt['accepted'])
            self.assertFalse(receipt['eligible_for_promotion'])

    def test_full_comparisons_have_no_unknown_or_extra_words(self):
        receipts = self.verification['controls'] + self.verification['source_controls']
        for receipt in receipts:
            comparison = receipt['comparison']
            self.assertEqual(comparison['total'], 445)
            self.assertEqual(comparison['extra_words'], 0)
            for key in ['unresolved', 'unverified', 'errors']:
                self.assertEqual(comparison[key], [])

    def test_native_membership_reproduces_normalized_divisor(self):
        tested = 0
        for receipt in self.allocation.values():
            for decision in receipt['decisions']:
                membership = decision.get('native_listing_membership')
                if membership is None:
                    continue
                count = len(membership['occurrence_block_ids']) + len(membership['default_live_block_ids'])
                expected = count if count < 3 else 2 + ((count - 2) >> 2)
                self.assertEqual(count, membership['normalization_input'])
                self.assertEqual(expected, membership['normalized_nocs'])
                self.assertEqual(expected, int(decision['nocs']))
                tested += 1
        self.assertEqual(tested, 12)

    def test_allocator_controls_have_no_forcing(self):
        for receipt in self.allocation.values():
            self.assertTrue(receipt['no_force_controls'])
            self.assertTrue(receipt['optimized_ucode_equal_to_stock'])
            for decision in receipt['decisions']:
                self.assertEqual(decision['forced'], '-2')

if __name__ == '__main__':
    unittest.main()
