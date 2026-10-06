"""Focused positive and rejection tests for this research packet."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest

WORK = Path(__file__).resolve().parent
ROOT = WORK.parents[3]
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(WORK))
import native_behavior as behavior
import verify
from tools.cloud import score


def instruction(op, rs=0, rt=0, immediate=0):
    return (op<<26)|(rs<<21)|(rt<<16)|(immediate&65535)


def special(rs=0, rt=0, rd=0, shift=0, fn=0):
    return (rs<<21)|(rt<<16)|(rd<<11)|(shift<<6)|fn


class PacketTests(unittest.TestCase):
    def setUp(self):
        self.evidence = json.loads((WORK/'evidence.json').read_text())
        score.ASM_DIR = ROOT/'asm/us/boot_tail'

    def test_zero_credit(self):
        self.assertEqual(self.evidence['new_matching_bytes'], 0)
        self.assertEqual(self.evidence['status'], 'COMPLETE-NONMATCH')
        self.assertFalse(any(row['accepted'] for row in self.evidence['compiled_rows']))

    def test_source_identity(self):
        self.assertEqual(hashlib.sha256(verify.SOURCE.read_bytes()).hexdigest(), self.evidence['source_sha256'])

    def test_no_partial_body_or_padding_claim(self):
        row = self.evidence['compiled_rows'][0]
        self.assertEqual(row['function_bytes'], 1160)
        self.assertEqual(row['text_section_bytes'], 1168)
        self.assertEqual(row['tail_padding_bytes'], 8)
        self.assertTrue(row['tail_padding_all_zero'])
        self.assertEqual(row['comparison']['differing'], 206)
        self.assertEqual(row['comparison']['total'], 290)
        self.assertEqual(row['frame_bytes'], 120)

    def test_all_relocations_resolved(self):
        for row in self.evidence['compiled_rows']:
            for field in ('unresolved','unverified','errors'):
                self.assertEqual(row['comparison'][field], [])
            self.assertEqual(row['nonempty_owned_data_sections'], [])

    def test_callers(self):
        self.assertEqual(self.evidence['direct_tail_callers'],
                         [{'function':'func_8001558C','site':'0x80015660'},
                          {'function':'func_8001558C','site':'0x8001568c'}])

    def test_layout(self):
        self.assertEqual(self.evidence['native_layout']['sizes'], verify.SIZES)
        self.assertEqual(self.evidence['native_layout']['offsets'], verify.OFFSETS)

    def test_complete_reachable_coverage(self):
        result = self.evidence['behavior']
        self.assertEqual(result['cases'], 1782)
        self.assertEqual(result['native_instruction_coverage'], 288)
        self.assertEqual(result['uncovered_native_offsets'], [276,312])
        self.assertTrue(result['all_reachable_native_instructions_covered'])
        target = score.targets()['func_800178B0']
        self.assertEqual(behavior.reachable_offsets(target), set(range(0,1160,4))-{276,312})

    def test_host_sanitizer_coverage(self):
        result = self.evidence['host_behavior']
        self.assertEqual(result['status'], 'PASS')
        self.assertEqual(result['cases'], 16896)
        self.assertIn('-fsanitize=undefined,address', result['flags'])
        self.assertEqual(result['source_sha256'], hashlib.sha256((WORK/'test_host.c').read_bytes()).hexdigest())

    def test_big_endian_memory(self):
        m = behavior.Memory()
        m.map(16, 8)
        m.write(16, 4, 0xFEDCBA98)
        self.assertEqual([m.read(16+i,1) for i in range(4)], [254,220,186,152])
        self.assertEqual(m.read(18,2), 0xBA98)
        m.write(16, 2, 0x12345)
        self.assertEqual(m.read(16,4), 0x2345BA98)

    def test_unmapped_memory_rejected(self):
        with self.assertRaises(AssertionError):
            behavior.Memory().read(0, 1)

    def test_out_of_bounds_write_rejected(self):
        m = behavior.Memory()
        m.map(4, 4)
        with self.assertRaises(AssertionError):
            m.write(6, 4, 0)

    def test_overlapping_maps_rejected(self):
        m = behavior.Memory()
        m.map(4, 8)
        with self.assertRaises(AssertionError):
            m.map(8, 8)

    def simple(self, instructions):
        return behavior.Machine(instructions).run((8,None,False,False,0))[0]

    def test_return_delay_slot(self):
        self.assertEqual(self.simple([special(rs=31,fn=8), instruction(9,rt=2,immediate=17)]), 17)

    def test_taken_delayed_branch(self):
        words = [instruction(9,rt=2,immediate=1), instruction(4,immediate=2),
                 instruction(9,rs=2,rt=2,immediate=3), instruction(9,rs=2,rt=2,immediate=100),
                 special(rs=31,fn=8), instruction(9,rs=2,rt=2,immediate=7)]
        self.assertEqual(self.simple(words), 11)

    def test_likely_annuls_only_when_not_taken(self):
        for is_equal, expected in [(False,8),(True,108)]:
            words = [instruction(9,rt=2,immediate=1),
                     instruction(20,rs=2,rt=2 if is_equal else 0,immediate=1),
                     instruction(9,rs=2,rt=2,immediate=100), special(rs=31,fn=8),
                     instruction(9,rs=2,rt=2,immediate=7)]
            self.assertEqual(self.simple(words), expected)

    def test_unknown_opcode_rejected(self):
        with self.assertRaisesRegex(AssertionError, 'opcode'):
            self.simple([instruction(63)])

    def test_nested_delay_branch_rejected(self):
        with self.assertRaises(AssertionError):
            self.simple([instruction(4,immediate=1), instruction(4,immediate=1), special(rs=31,fn=8),0])

    def test_real_source_mutation_rejected(self):
        with tempfile.TemporaryDirectory(prefix='sequence-negative-') as tmp:
            tmp = Path(tmp)
            source = tmp/'mutant.c'
            original = verify.SOURCE.read_text()
            self.assertEqual(original.count('context->group = slot + 23;'),1)
            source.write_text(original.replace('context->group = slot + 23;', 'context->group = slot + 24;'))
            obj = tmp/'mutant.o'
            score.compile_single(source, score.DEFAULT_FLAGS, obj)
            with self.assertRaisesRegex(AssertionError, 'behavior mismatch'):
                behavior.run(obj)

    def test_five_real_inputs_no_volatile_or_asm(self):
        # The native interface remains five pointers; helper call arguments are checked by trace.
        source = verify.SOURCE.read_text()
        declaration = source[source.index('u32 func_800178B0('):source.index('\n{', source.index('u32 func_800178B0('))]
        self.assertEqual(declaration.count('*'),5)
        self.assertNotIn('volatile', source)
        self.assertNotIn('__asm', source)


if __name__ == '__main__':
    unittest.main()
