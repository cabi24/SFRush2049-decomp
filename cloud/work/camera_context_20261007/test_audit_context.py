"""Synthetic instruction-field tests, with no copied target instruction data."""
import unittest
from audit_context import shape, signed16


def insn(op, rs=0, rt=0, immediate=0):
    return op << 26 | rs << 21 | rt << 16 | (immediate & 65535)


class ShapeTests(unittest.TestCase):
    def test_sign_extension(self):
        self.assertEqual(signed16(65535), -1)
        self.assertEqual(signed16(32767), 32767)

    def test_saves_exclude_argument_homes_and_outgoing_arguments(self):
        words = [insn(9, 29, 29, -32), insn(43, 29, 31, 28),
                 insn(43, 29, 16, 24), insn(43, 29, 5, 36), insn(43, 29, 4, 16)]
        result = shape(words, {})
        self.assertEqual(result['stack_frame_bytes'], 32)
        self.assertEqual([x['register'] for x in result['prologue_saves']], ['ra', 's0'])

    def test_float_pair_and_fp_comparison(self):
        words = [insn(9, 29, 29, -32), insn(61, 29, 20, 16),
                 (17 << 26) | (16 << 21) | (4 << 6) | 50]
        result = shape(words, {})
        self.assertEqual(result['prologue_saves'][0]['register'], 'f20/f21')
        self.assertEqual(result['syntactic_fp_destinations'], [])

    def test_direct_call_count_and_offset(self):
        address = 0x80001000
        words = [0, 3 << 26 | (address >> 2 & 0x3ffffff)]
        result = shape(words, {'example': address})
        self.assertEqual(result['direct_calls'], [{'offset': '0x4', 'callee': 'example'}])
        self.assertEqual(result['call_counts'], {'example': 1})


if __name__ == '__main__':
    unittest.main()
