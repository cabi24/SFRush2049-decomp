import random
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from amatch import aligned  # noqa: E402


def dp_lcs(a, b):
    prev = [0] * (len(b) + 1)
    for x in a:
        cur = [0]
        for j, y in enumerate(b):
            cur.append(prev[j] + 1 if x == y else max(prev[j + 1], cur[j]))
        prev = cur
    return prev[-1]


class AlignedTests(unittest.TestCase):
    def test_lcs_matches_dp(self):
        rnd = random.Random(1)
        for _ in range(300):
            alpha = rnd.choice([2, 4, 20])
            a = [rnd.randrange(alpha) for _ in range(rnd.randrange(0, 40))]
            b = [rnd.randrange(alpha) for _ in range(rnd.randrange(0, 40))]
            self.assertEqual(aligned.lcs_len(a, b), dp_lcs(a, b))
            pairs = aligned.lcs_pairs(a, b)
            self.assertEqual(len(pairs), dp_lcs(a, b))
            for (i, j) in pairs:
                self.assertEqual(a[i], b[j])
            self.assertEqual(pairs, sorted(set(pairs)))
            self.assertTrue(all(p[0] < q[0] and p[1] < q[1] for p, q in zip(pairs, pairs[1:])))

    def test_large_fast(self):
        rnd = random.Random(2)
        a = [rnd.randrange(1 << 32) for _ in range(2500)]
        b = list(a)
        del b[100:103]
        b.insert(900, 7)
        self.assertEqual(aligned.lcs_len(a, b), 2497)
        self.assertEqual(len(aligned.lcs_pairs(a, b)), 2497)

    def test_strict_and_masks(self):
        self.assertEqual(aligned.strict_diff([1, 2, 3], [1, 2, 3]), 0)
        self.assertEqual(aligned.strict_diff([1, 2, 3], [1, 9]), 2)
        self.assertEqual(aligned.strict_diff([0x3C011234], [0x3C010000], {0: 0xFFFF0000}), 0)

    def test_scores_with_insertion(self):
        lw = 0x27BDFFE8  # addiu sp,sp,-24
        want = [lw, 0x8C820000, 0xAFBF0014, 0x03E00008, 0x27BD0018]
        got = [lw, 0x00000000, 0x8C820000, 0xAFBF0014, 0x03E00008, 0x27BD0018]
        s = aligned.aligned_scores(want, got)
        self.assertEqual(s["aligned_exact"], 5)
        self.assertEqual(aligned.strict_diff(want, got), 4)

    def test_opcode_levels(self):
        a = [0x8C820004]  # lw v0,4(a0)
        b = [0x8CA30008]  # lw v1,8(a1)
        s = aligned.aligned_scores(a, b)
        self.assertEqual((s["aligned_exact"], s["aligned_opcode"], s["aligned_opcode_reg"]), (0, 1, 0))
        c = [0x8C820008]  # same regs, other offset
        s = aligned.aligned_scores(a, c)
        self.assertEqual((s["aligned_exact"], s["aligned_opcode_reg"]), (0, 1))

    def test_hunks(self):
        want = [1, 2, 3, 4, 5]
        got = [1, 2, 9, 4, 6, 5]
        h = aligned.diff_hunks(want, got)
        self.assertEqual(h, [(2, 2, "sub", 3, 9), (4, 4, "ins", None, 6)])
        self.assertEqual(aligned.diff_hunks(want, want), [])
        h = aligned.diff_hunks([1, 2, 3], [1, 3])
        self.assertEqual(h, [(1, 1, "del", 2, None)])

    def test_score_dict(self):
        d = aligned.score_dict([1, 2, 3], [1, 2, 3])
        self.assertTrue(d["matched"])
        d = aligned.score_dict([1, 2, 3], [1, 2, 3], unverified=["x"])
        self.assertFalse(d["matched"])
        for k in ("strict_diff", "size", "target_size", "extra", "aligned_exact", "aligned_opcode",
                  "aligned_opcode_reg", "target_words", "matched", "unverified"):
            self.assertIn(k, d)


if __name__ == "__main__":
    unittest.main()
