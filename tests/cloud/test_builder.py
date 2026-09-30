import re
import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from amatch import builder  # noqa: E402

SCORE = ROOT / "tools" / "cloud" / "score.py"
NEED_IDO = unittest.skipUnless((ROOT / "tools" / "cloud" / "ido" / "cc").exists(),
                               "IDO missing (tools/cloud/setup.sh)")


def parse_summary(line):
    line = line.strip()
    m = re.match(r"(\d+)/\d+ words differ", line)
    return int(m.group(1)) if m else 0


def score_py_fn(path, fn, flags):
    p = subprocess.run([sys.executable, str(SCORE), "fn", str(path), fn, "--flags", flags],
                       capture_output=True, text=True, cwd=ROOT)
    summary = [l for l in p.stdout.splitlines() if l.startswith("  ") and not l.startswith("    +")]
    return p.returncode == 0, parse_summary(summary[-1]) if summary else None


def score_py_group(d):
    p = subprocess.run([sys.executable, str(SCORE), "group", str(d)], capture_output=True,
                       text=True, cwd=ROOT)
    res, cur = {}, None
    for line in p.stdout.splitlines():
        if line.startswith("    +"):
            continue
        if re.match(r"\S.*:$", line) and not line.startswith(("Members", "Context")):
            cur = line[:-1]
        elif line.startswith("  ") and cur:
            res[cur] = (line.strip().startswith("MATCH"), parse_summary(line))
            cur = None
    return res


class CacheFree(unittest.TestCase):
    def test_missing_target_is_clean(self):
        r = builder.compile_fn("int x;", "no_such_function_xyz", use_cache=False)
        self.assertTrue(r["err"])
        self.assertFalse(r["matched"])


@NEED_IDO
class Validate(unittest.TestCase):
    def test_compile_error_is_a_result(self):
        r = builder.compile_fn("void func_8008A38C( { syntax error", "func_8008A38C", use_cache=False)
        self.assertTrue(r["err"])
        self.assertFalse(r["matched"])
        self.assertEqual(r["aligned_exact"], 0)

    def test_matches_agree_with_score_py(self):
        targets = builder.get_targets()
        files = [p for p in sorted((ROOT / "cloud" / "matches").glob("*.c")) if p.stem in targets][:12]
        self.assertGreaterEqual(len(files), 10)
        for p in files:
            text = p.read_text()
            flags = builder.parse_flags_comment(text) or builder.DEFAULT_FLAGS
            for fl in (flags, builder.DEFAULT_FLAGS.replace("-O2", "-O1")):
                with self.subTest(file=p.name, flags=fl):
                    want_m, want_d = score_py_fn(p, p.stem, fl)
                    r = builder.compile_fn(text, p.stem, fl, use_cache=False)
                    self.assertIsNone(r["err"])
                    self.assertEqual(r["matched"], want_m)
                    self.assertEqual(r["strict_diff"], want_d)
                    self.assertLessEqual(r["aligned_exact"], r["target_words"])
                    if r["matched"]:
                        self.assertEqual(r["aligned_exact"], r["target_words"])

    def test_groups_agree_with_score_py(self):
        for name in ("func_800B9B64", "camera_scene_manager", "draw_number"):
            d = ROOT / "cloud" / "work" / "ipa-groups" / name
            with self.subTest(group=name):
                want = score_py_group(d)
                r = builder.compile_group(d, use_cache=False)
                self.assertIsNone(r["err"])
                got = {**r["context"], **r["members"]}
                self.assertEqual(set(want), set(got))
                for n, (m, diff) in want.items():
                    self.assertEqual(got[n]["strict_diff"], diff, n)
                    if n in r["members"]:
                        self.assertEqual(got[n]["matched"], m, n)

    def test_cache_and_parallel(self):
        p = ROOT / "cloud" / "matches" / "func_8008A38C.c"
        text = p.read_text()
        flags = builder.parse_flags_comment(text)
        jobs = [{"kind": "fn", "src": text, "fn": p.stem, "flags": flags},
                {"kind": "fn", "src": text, "fn": p.stem, "flags": builder.DEFAULT_FLAGS},
                {"kind": "fn", "src": "garbage(", "fn": p.stem}]
        rs = builder.score_many(jobs, jobs_n=2)
        self.assertEqual([r["matched"] for r in rs], [True, False, False])
        self.assertTrue(rs[2]["err"])
        again = builder.compile_fn(text, p.stem, flags)
        self.assertTrue(again["cached"])
        self.assertEqual(again["strict_diff"], rs[0]["strict_diff"])

    def test_group_overrides(self):
        d = ROOT / "cloud" / "work" / "ipa-groups" / "draw_number"
        base = builder.compile_group(d, use_cache=False)
        bad = builder.compile_group(d, {"files": {"dn.c": "this is not C ("}}, use_cache=False)
        self.assertTrue(bad["err"])
        self.assertFalse(bad["matched"])
        self.assertEqual(set(bad["members"]), set(base["members"]))

    def test_group_with_external_targets(self):
        d = ROOT / "cloud" / "work" / "ipa-groups" / "catchup_logic"
        r = builder.compile_group(d, use_cache=False)
        self.assertIn("func_801084D4", r["members"])
        self.assertGreater(r["members"]["func_801084D4"]["target_size"], 0)


if __name__ == "__main__":
    unittest.main()
