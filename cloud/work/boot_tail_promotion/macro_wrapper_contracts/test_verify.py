"""Fast regression tests for the read-only wrapper repair proof."""
import copy
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import unittest
from unittest import mock

HERE = Path(__file__).resolve().parent
SPEC = importlib.util.spec_from_file_location('macro_wrapper_verify', HERE / 'verify.py')
verify = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(verify)


def without_comments(text):
    return verify.normalize(re.sub(r'/\*.*?\*/', '', text, flags=re.S))


class WrapperContractTests(unittest.TestCase):
    def test_repairs_only_unify_the_accepted_type_names(self):
        for name in verify.PAIRS:
            legacy = (HERE.parent / 'sources' / (name + '.c')).read_text()
            repaired = (HERE / 'sources' / (name + '.c')).read_text()
            for kind in ('MacroState', 'MacroControl', 'MacroCommand'):
                legacy = legacy.replace(kind + '_' + name[5:], kind + '_80023818')
            self.assertEqual(without_comments(legacy), without_comments(repaired))

    def test_repaired_contract_is_the_accepted_callee_contract(self):
        accepted = (HERE.parent / 'sources/func_80023818.c').read_text()
        declaration = re.search(r'extern void func_80023754\(.*?;', accepted, re.S)[0]
        for name in verify.PAIRS:
            repaired = (HERE / 'sources' / (name + '.c')).read_text()
            self.assertIn(verify.normalize(declaration), verify.normalize(repaired))
            self.assertIn('typedef struct MacroState_80023818 MacroState_80023818;', repaired)
            self.assertIn('typedef struct MacroControl_80023818 MacroControl_80023818;', repaired)

    def test_offsets_and_masks(self):
        for name, (offset, mask) in verify.PAIRS.items():
            repaired = (HERE / 'sources' / (name + '.c')).read_text()
            self.assertIn('+ 0x%X)' % offset, repaired)
            self.assertIn('command, 0x%08X)' % mask, repaired)
            self.assertEqual(repaired.splitlines()[0], '/* flags: ' + verify.FLAGS + ' */')

    def test_short_function_cannot_borrow_neighbor_bytes(self):
        with self.assertRaisesRegex(ValueError, 'short function'):
            verify.extent([1, 2, 3, 4], {'f': 0, 'neighbor': 4}, 'f', 8)

    def test_nonzero_excess_fails_even_if_target_prefix_matches(self):
        with self.assertRaisesRegex(ValueError, 'nonzero function overflow'):
            verify.extent([1, 2, 3], {'f': 0}, 'f', 8)

    def test_zero_object_padding_allowed_only_outside_exact_tu_extent(self):
        self.assertEqual(verify.extent([1, 2, 0], {'f': 0}, 'f', 8), (0, 12))
        with self.assertRaisesRegex(ValueError, 'slot extent changed'):
            verify.extent([1, 2, 0], {'f': 0}, 'f', 8, exact=True)

    def test_missing_and_duplicate_passthroughs_fail_closed(self):
        name = next(iter(verify.PAIRS))
        sources = {name: HERE / 'sources' / (name + '.c')}
        contexts = {name: {'preamble': []}}
        with self.assertRaisesRegex(ValueError, 'missing/duplicate'):
            verify.splice('', sources, contexts)
        pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/%s.s")' % name
        with self.assertRaisesRegex(ValueError, 'missing/duplicate'):
            verify.splice(pragma + '\n' + pragma, sources, contexts)

    def test_decl_dedup_keeps_existing_accepted_contract(self):
        name = next(iter(verify.PAIRS))
        source = HERE / 'sources' / (name + '.c')
        declaration = 'typedef struct MacroState_80023818 MacroState_80023818;'
        pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/%s.s")' % name
        text = declaration + '\n' + pragma
        result = verify.splice(text, {name: source}, {name: {'preamble': [declaration]}})
        self.assertEqual(result.count(declaration), 1)
        self.assertNotIn(pragma, result)
        self.assertIn(verify.body(source), result)


def promoted_fixture(names):
    """Simulate a maintainer transaction entirely as strings/dicts in memory."""
    text = verify.base_text('src/rom/lib_22300.c')
    locks = json.loads(verify.base_text('matched.lock.json'))
    for name in names:
        pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/%s.s")' % name
        source = HERE / 'sources' / (name + '.c')
        text = text.replace(pragma, verify.body(source), 1)
        previous = 'cloud/work/boot_tail_promotion/sources/' + name + '.c:' + name
        entry = copy.deepcopy(locks.pop(previous))
        entry['body_sha256'] = verify.source_body_hashes(text)[name]
        locks['src/rom/lib_22300.c:' + name] = entry
    return text, locks


class WrapperLifecycleTests(unittest.TestCase):
    def test_current_state_accepts_partial_and_full_promotions(self):
        for count in (0, 1, 8):
            names = list(verify.PAIRS)[:count]
            text, locks = promoted_fixture(names)
            accepted, promoted, pending = verify.current_state(text, locks)
            self.assertEqual(len(accepted), 15 + count)
            self.assertEqual(promoted, names)
            self.assertEqual(pending, list(verify.PAIRS)[count:])

    def test_raw_encoding_differences_require_verified_body_ownership(self):
        with mock.patch.object(verify.score, 'symbols', return_value={'f': 0}), \
             mock.patch.object(verify.score, 'targets', return_value={'f': [1, 2]}):
            with mock.patch.object(verify.score, 'text_words', side_effect=[[1, 2, 0], [1, 3, 0]]):
                self.assertFalse(verify.check_unowned_bytes('baseline', 'current', {'f'}))
            with mock.patch.object(verify.score, 'text_words', side_effect=[[1, 2, 0], [1, 3, 0]]):
                with self.assertRaisesRegex(ValueError, 'unverified passthrough or padding'):
                    verify.check_unowned_bytes('baseline', 'current', set())
            with mock.patch.object(verify.score, 'text_words', side_effect=[[1, 2, 0], [1, 2, 1]]):
                with self.assertRaisesRegex(ValueError, 'unverified passthrough or padding'):
                    verify.check_unowned_bytes('baseline', 'current', {'f'})

    def test_missing_baseline_lock_is_rejected(self):
        text, locks = promoted_fixture([])
        del locks['src/rom/lib_22300.c:func_80023818']
        with self.assertRaisesRegex(ValueError, 'baseline accepted lock missing'):
            verify.current_state(text, locks)

    def test_wrong_promoted_body_hash_is_rejected(self):
        text, locks = promoted_fixture(['func_80023844'])
        locks['src/rom/lib_22300.c:func_80023844']['body_sha256'] = '0' * 64
        with self.assertRaisesRegex(ValueError, 'current locked body changed'):
            verify.current_state(text, locks)

    def test_unlocked_c_body_cannot_bypass_passthrough_check(self):
        text, locks = promoted_fixture(['func_80023844'])
        del locks['src/rom/lib_22300.c:func_80023844']
        with self.assertRaisesRegex(ValueError, 'unlocked candidate'):
            verify.current_state(text, locks)

    def require_toolchain(self):
        def unavailable(reason):
            if os.environ.get('REQUIRE_TOOLCHAIN') == '1':
                self.fail('REQUIRE_TOOLCHAIN=1: ' + reason)
            self.skipTest(reason)
        if not (Path(verify.score.IDO) / 'cc').is_file():
            unavailable('IDO unavailable')
        for tool in ('mips-linux-gnu-as', 'mips-linux-gnu-objdump', 'mips-linux-gnu-readelf', 'cc'):
            if not shutil.which(tool):
                unavailable('C compiler unavailable' if tool == 'cc' else tool + ' unavailable')

    def replay_promoted_fixture(self, count):
        self.require_toolchain()
        text, locks = promoted_fixture(list(verify.PAIRS)[:count])
        previous_targets = verify.score.ASM_DIR
        report = verify.run(text, locks)
        self.assertEqual(verify.score.ASM_DIR, previous_targets)
        self.assertEqual(report['result'], 'PASS')
        self.assertTrue(report['temporary_lifecycle_fixture'])
        self.assertEqual(report['baseline_existing_locked_functions'], 15)
        self.assertEqual(report['current_locked_functions'], 15 + count)
        self.assertEqual(len(report['already_promoted_candidates']), count)
        self.assertEqual(len(report['overlaid_candidates']), 8 - count)
        self.assertEqual(len(report['results']), 23)
        self.assertTrue(all(row['all_full_relocated_bytes_equal'] for row in report['results']))
        self.assertTrue(report['negative_controls']['legacy_full_tu_redeclaration_reproduced'])
        self.assertTrue(report['unverified_passthrough_and_padding_bytes_unchanged'])
        self.assertTrue(report['all_raw_tu_text_bytes_equal'])

    def test_partial_promotion_replays_frozen_baseline_and_actual_current_tu(self):
        self.replay_promoted_fixture(1)

    def test_complete_promotion_keeps_verifying_all_eight_repair_sources(self):
        self.replay_promoted_fixture(8)

    def test_promoted_body_is_still_byte_checked_even_with_updated_fixture_lock(self):
        self.require_toolchain()
        text, locks = promoted_fixture(list(verify.PAIRS))
        text = text.replace('+ 0xD6)', '+ 0xD7)')
        locks['src/rom/lib_22300.c:func_80023844']['body_sha256'] = verify.source_body_hashes(text)['func_80023844']
        with self.assertRaisesRegex(ValueError, 'func_80023844: 1/11 words differ'):
            verify.run(text, locks)


if __name__ == '__main__':
    unittest.main()
