import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from amatch import search  # noqa: E402

NEED_IDO = unittest.skipUnless((ROOT / "tools" / "cloud" / "ido" / "cc").exists(),
                               "IDO missing (tools/cloud/setup.sh)")
CORPUS = ROOT / "cloud" / "work" / "tools" / "amatch" / "corpus.py"

TARGET = "abcdefgh"
ALPHA = "abcdefgh"


class Mut:
    def __init__(self, name, new_src, cost=1.0, target="any"):
        self.name, self.new_src, self.cost, self.target, self.id = name, new_src, cost, target, name


class StandIn:
    """Tiny stand-in mutator: the 'source' is a string; a mutation sets one position."""

    @staticmethod
    def mutations(src, fn=None, catalog=None):
        out = []
        for i in range(len(src)):
            for ch in ALPHA:
                if ch != src[i]:
                    out.append(Mut(f"set{i}:{ch}", src[:i] + ch + src[i + 1:], 1.0))
        return out


def fake_score(text):
    t = len(TARGET)
    ex = sum(1 for a, b in zip(text, TARGET) if a == b)
    return {"strict_diff": t - ex, "size": len(text), "target_size": t, "extra": 0,
            "aligned_exact": ex, "aligned_opcode": ex, "aligned_opcode_reg": ex,
            "target_words": t, "matched": text == TARGET, "unverified": [], "unresolved": [],
            "errors": [], "err": None, "secs": 0.0, "cached": False}


def fake_eval(jobs):
    return [fake_score(j["src"]) for j in jobs]


class FnSearch(unittest.TestCase):
    def run_fn(self, start, **kw):
        with tempfile.TemporaryDirectory() as tmp:
            kw.setdefault("budget", 600)
            r = search.run_fn(start, "f", flags="-O2", jobs=2, out=tmp, mutator=StandIn,
                              evaluator=fake_eval, verify=lambda n: (True, "ok"), **kw)
            log = json.loads(Path(r["log_path"]).read_text())
            best = Path(r["best_src_path"]).read_text()
        return r, log, best

    def test_finds_match_three_mutations_away(self):
        r, log, best = self.run_fn("abcdxxxh".replace("x", "a"), seed=1)
        self.assertTrue(r["matched"], r)
        self.assertEqual(best, TARGET)
        self.assertLessEqual(r["evals"], 600)
        self.assertTrue(log["builder_matched"])

    def test_deterministic_same_seed(self):
        a = self.run_fn("hhhhhhhh", seed=7, budget=300)
        b = self.run_fn("hhhhhhhh", seed=7, budget=300)
        ha = [e.get("hash") for e in a[1]["candidates"]]
        hb = [e.get("hash") for e in b[1]["candidates"]]
        self.assertEqual(ha, hb)
        self.assertEqual(a[0]["evals"], b[0]["evals"])

    def test_budget_respected_and_log_has_every_eval(self):
        r, log, _ = self.run_fn("hhhhhhhh", seed=3, budget=50)
        self.assertLessEqual(r["evals"], 50)
        scored = [e for e in log["candidates"] if "hash" in e]
        self.assertEqual(len(scored), r["evals"])
        self.assertEqual(len({e["hash"] for e in scored}), len(scored))   # tabu: no repeats
        self.assertIn("score", scored[0])
        self.assertEqual(scored[0]["path"], [])

    def test_baseline_match_stops_immediately(self):
        r, _, _ = self.run_fn(TARGET)
        self.assertEqual(r["evals"], 1)
        self.assertTrue(r["matched"])

    def test_failed_verification_is_not_reported_matched(self):
        with tempfile.TemporaryDirectory() as tmp:
            r = search.run_fn("abcdefgx", "f", flags="-O2", jobs=1, out=tmp, mutator=StandIn,
                              evaluator=fake_eval, verify=lambda n: (False, "score.py says no"),
                              budget=200)
        self.assertFalse(r["matched"])
        self.assertIn("verify_failed", r)

    def test_improves_objective_when_no_match_reachable(self):
        r, log, _ = self.run_fn("hhhhhhhh", seed=2, budget=40)
        self.assertGreater(r["best_score"]["aligned_exact"], log["baseline"]["aligned_exact"])

    def test_adaptive_weights_favor_improving_class(self):
        class Two:
            @staticmethod
            def mutations(src, fn=None, catalog=None):
                good = [Mut(f"good:{i}", src[:i] + TARGET[i] + src[i + 1:]) for i in range(len(src))
                        if src[i] != TARGET[i]]
                bad = [Mut(f"bad:{i}", src[:i] + "z" + src[i + 1:]) for i in range(len(src))]
                return good + bad
        s = search.Searcher("fn", {"src.c": "hhhhhhhh"}, fn="f", jobs=2, seed=4, budget=120,
                            mutator=Two, evaluator=fake_eval)
        s.run()
        self.assertGreater(s.weights.get("good", 1.0), s.weights.get("bad", 1.0))

    def test_mut_class(self):
        self.assertEqual(search.mut_class("loop-form:for->do@L12"), "loop-form")
        self.assertEqual(search.mut_class("decl_order(3)"), "decl_order")


def fake_group_eval(jobs):
    """Members A (matches iff text has 'A1') and B (closeness of the rest to 'bbbb')."""
    out = []
    for j in jobs:
        text = j["overrides"]["files"]["group.c"]
        a = fake_score("abcdefgh" if "A1" in text else "xxxxxxxx")
        body = text.replace("A1", "")
        b = fake_score("".join("b" if c == "b" else "x" for c in body.ljust(8)[:8]).replace("bbbbbbbb", "abcdefgh"))
        out.append({"kind": "group", "err": None, "matched": a["matched"] and b["matched"],
                    "members": {"A": dict(a, name="A"), "B": dict(b, name="B")}, "context": {},
                    "secs": 0.0, "cached": False})
    return out


class GroupMut:
    @staticmethod
    def mutations(src, fn=None, catalog=None):
        out = [Mut("dropA", src.replace("A1", "", 1), 1.0)] if "A1" in src else []
        for i in range(len(src)):
            if src[i] not in "A1":
                out.append(Mut(f"b:{i}", src[:i] + "b" + src[i + 1:], 1.0))
        return [m for m in out if m.new_src != src]


class GroupSearch(unittest.TestCase):
    def test_preserves_matching_member(self):
        with tempfile.TemporaryDirectory() as tmp:
            g = Path(tmp) / "g"
            g.mkdir()
            (g / "group.json").write_text(json.dumps(
                {"members": ["A", "B"], "files": ["group.c"], "keep": [], "flags": "-O3",
                 "claims": ["A"]}))
            (g / "group.c").write_text("A1xxxxxx")
            r = search.run_group(g, budget=150, jobs=2, seed=5, out=Path(tmp) / "out",
                                 mutator=GroupMut, evaluator=fake_group_eval,
                                 verify=lambda n: (True, "ok"))
            log = json.loads(Path(r["log_path"]).read_text())
            best = (Path(r["best_src_path"]) / "group.c").read_text()
        self.assertIn("A1", best)                     # A never lost
        rejected = [e for e in log["candidates"] if e.get("rejected")]
        self.assertTrue(rejected)                      # dropA candidates were refused
        for e in rejected:
            self.assertTrue(any("dropA" in p for p in e["path"]))
        self.assertEqual(log["kind"], "group")


class Objective(unittest.TestCase):
    def test_ordering(self):
        base = dict(fake_score("abcdefgx"))
        better = dict(fake_score("abcdefgh"))
        worse = dict(fake_score("xxxxxxxx"))
        kb, _ = search.fn_objective(base, 0)
        kg, _ = search.fn_objective(better, 0)
        kw, _ = search.fn_objective(worse, 0)
        self.assertLess(kg, kb)
        self.assertLess(kb, kw)
        err = dict(base, err="boom")
        self.assertLess(kw, search.fn_objective(err, 0)[0])
        # tie-break: equal score, lower cost wins
        self.assertLess(search.fn_objective(base, 1)[0], search.fn_objective(base, 2)[0])


class Real(unittest.TestCase):
    """Real IDO runs: skip cleanly without IDO or corpus data."""

    def materialize(self, fn, which, path):
        r = subprocess.run([sys.executable, str(CORPUS), "materialize", fn, which, "--out", str(path)],
                           capture_output=True, text=True, cwd=ROOT)
        if r.returncode != 0:
            self.skipTest("corpus materialize unavailable: " + r.stderr[-200:])

    @NEED_IDO
    def test_goal_matches_at_baseline_and_verifies(self):
        with tempfile.TemporaryDirectory() as tmp:
            f = Path(tmp) / "g.c"
            self.materialize("Input_SetAnalogBounds", "goal", f)
            r = search.run_fn(f.read_text(), "Input_SetAnalogBounds", jobs=1, budget=5,
                              out=Path(tmp) / "o", mutator=StandIn)
        self.assertTrue(r["matched"], r)
        self.assertEqual(r["evals"], 1)

    @NEED_IDO
    def test_real_fix_found_with_standin_mutator(self):
        class ParamType:
            @staticmethod
            def mutations(src, fn=None, catalog=None):
                out = []
                if "s16 arg1" in src:
                    out.append(Mut("param-type:s16->s32", src.replace("s16 arg1", "s32 arg1"), 1.0))
                if "int arg2" in src:
                    out.append(Mut("param-type:int->s32", src.replace("int arg2", "s32 arg2"), 1.0))
                if "long arg3" in src:
                    out.append(Mut("param-type:long->s32", src.replace("long arg3", "s32 arg3"), 1.0))
                out.append(Mut("noise:pad", src + "\n", 0.1))
                return out
        with tempfile.TemporaryDirectory() as tmp:
            f = Path(tmp) / "s.c"
            self.materialize("Input_SetAnalogBounds", "start", f)
            r = search.run_fn(f.read_text(), "Input_SetAnalogBounds", flags=None, jobs=2, budget=60,
                              seed=1, out=Path(tmp) / "o", mutator=ParamType)
        self.assertTrue(r["matched"], r)


if __name__ == "__main__":
    unittest.main()
