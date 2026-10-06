import hashlib
import importlib.util
import json
from pathlib import Path
import unittest

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('event_ring_verify', HERE/'verify.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


class PacketTests(unittest.TestCase):
    def test_receipt_binding(self):
        receipt = json.loads((HERE/'verification.json').read_text())
        self.assertEqual(receipt['status'], 'MATCH')
        self.assertEqual(receipt['native_sha256'], v.TARGET)
        self.assertEqual(receipt['source_sha256'], v.sha(v.SOURCE))
        for name, digest in receipt['packet_sha256'].items():
            self.assertEqual(v.sha(HERE/name), digest)
        self.assertEqual(receipt['elf']['function_bytes'], 396)
        self.assertEqual(receipt['elf']['differing_words'], 0)
        self.assertEqual(receipt['accepted_or_coverage_bytes'], 0)

    def test_fixture_domain(self):
        cases = v.fixtures()
        self.assertEqual(len(cases), 2560)
        self.assertEqual({x[3] for x in cases}, set(range(-128, 128)))
        self.assertEqual({x[5] for x in cases}, set(range(-128, 128)))
        self.assertEqual({(x[1], x[2], x[6], x[7]) for x in cases},
                         {(a, b, c, d) for a in range(4) for b in range(4)
                          for c in range(4) for d in range(1, 5)})

    def test_ordered_gates(self):
        case = (0, 0, 0, -1, 0, -1, 3, 1)
        before, after = v.fixture(case), v.expected(case)
        self.assertEqual(after.get(0x80395ED0, 1), 3)
        self.assertEqual(after.get(0x80152818+931, 1), 255)
        self.assertEqual(after.get(0x80149428, 1), (before.get(0x80149428, 1)-1) & 255)
        after = v.expected((0, 0, 0, -1, 1, -1, 3, 1))
        self.assertEqual(after.get(0x80152818+931, 1), 0)
        self.assertEqual(after.get(0x80395ED0, 1), 3)
        after = v.expected((0, 0, 0, 0, 0, -1, 3, 1))
        self.assertEqual(after.get(0x80395ED0, 1), 0)
        self.assertEqual(after.get(0x80395E70+3*24, 1), 1)


if __name__ == '__main__':
    unittest.main()
