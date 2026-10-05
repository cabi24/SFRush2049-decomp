"""Hash-bound negative authentic-group control, without compiler dependency."""
import hashlib
import json
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/texture_rect_compile_context'
NAME = 'func_80087110'


class AuthenticGroupControlTests(unittest.TestCase):
    def setUp(self):
        self.receipt = json.loads((HERE / 'verification.json').read_text())

    def test_inputs_still_match_recorded_sources(self):
        for filename, expected in self.receipt['source_bindings'].items():
            self.assertEqual(hashlib.sha256((ROOT / filename).read_bytes()).hexdigest(), expected)

    def test_existing_recipe_is_unchanged(self):
        original = self.receipt['existing_group_recipe']
        self.assertEqual(original, json.loads((ROOT / 'src/blob/groups/gfx_modes/group.json').read_text()))
        augmented = self.receipt['augmented_group_recipe']
        self.assertEqual(augmented['files'], original['files'] + ['texture_rectangle.c'])
        self.assertEqual(augmented['members'], original['members'] + [NAME])
        self.assertEqual(augmented['keep'], original['keep'] + [NAME])
        self.assertEqual(augmented['flags'], original['flags'])
        self.assertEqual(augmented['claims'], [])

    def test_complete_rectangle_receipt_is_identical(self):
        baseline = self.receipt['results']['standalone'][NAME]
        group = self.receipt['results']['augmented_group'][NAME]
        self.assertEqual(baseline, group)
        self.assertEqual(group['total'], 445)
        self.assertEqual(group['differing'], 4)
        self.assertEqual(group['elf_function_bytes'], 1780)
        self.assertTrue(group['exact_extent'])
        self.assertEqual(group['residual_offsets'], ['0x4c8', '0x4cc', '0x4d0', '0x4d4'])
        self.assertEqual(len(group['fully_relocated_body_sha256']), 64)

    def test_all_existing_bodies_remain_exact(self):
        original = self.receipt['results']['accepted_group']
        new = self.receipt['results']['augmented_group']
        self.assertEqual(len(original), 7)
        self.assertEqual(sum(row['total'] for row in original.values()), 3381)
        for name, baseline in original.items():
            self.assertEqual(new[name], baseline)
            self.assertTrue(new[name]['strict_score_zero'])
            self.assertTrue(new[name]['exact_extent'])
        self.assertEqual(original['func_80086A50']['notes'],
                         ['own .rodata verified at 0x80123870..0x80123884'])
        self.assertIsNone(original['func_80086A50']['fully_relocated_body_sha256'])

    def test_no_excess_or_uncertainty_is_hidden(self):
        for scenario in self.receipt['results'].values():
            for row in scenario.values():
                self.assertEqual(row['extra_words'], 0)
                for key in ['unresolved', 'unverified', 'errors']:
                    self.assertEqual(row[key], [])

    def test_native_inventory_identifies_real_callers_and_only_stubs(self):
        inventory = self.receipt['native_inventory']
        self.assertEqual(inventory['target_callee_count'], 0)
        self.assertEqual({(row['caller'], row['offset']) for row in inventory['direct_callers']},
                         {('Input_ProcessGameplayPad', '0x914'),
                          ('Input_ProcessGameplayPad', '0x9e8'),
                          ('audio_doppler_calc', '0x3f8')})
        definitions = inventory['typed_definition_inventory']
        self.assertEqual(len(definitions), 2)
        for row in definitions:
            self.assertTrue(row['empty_body_after_comment_removal'])
            self.assertEqual(hashlib.sha256((ROOT / row['source']).read_bytes()).hexdigest(),
                             row['source_sha256'])

    def test_research_control_cannot_claim_promotion(self):
        self.assertFalse(self.receipt['accepted'])
        self.assertFalse(self.receipt['eligible_for_promotion'])
        self.assertTrue(self.receipt['all_original_sources_unchanged'])
        self.assertTrue(self.receipt['rectangle_receipt_unchanged'])
        self.assertTrue(all(self.receipt['neighbor_receipts_unchanged'].values()))


if __name__ == '__main__':
    unittest.main()
