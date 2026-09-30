import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from amatch import autopilot, triage  # noqa: E402

NEED_IDO = unittest.skipUnless((ROOT / "tools" / "cloud" / "ido" / "cc").exists(),
                               "IDO missing (tools/cloud/setup.sh)")


def sc(matched=False, strict=10, size=20, target=20, ex=10, op=18, orr=15, **kw):
    d = {"matched": matched, "strict_diff": strict, "size": size, "target_size": target, "target_words": target,
         "aligned_exact": ex, "aligned_opcode": op, "aligned_opcode_reg": orr, "extra": 0, "unresolved": [],
         "unverified": [], "err": None, "words": kw.pop("words", [])}
    d.update(kw)
    return d


class FakeBuilder:
    """compile_fn stub: score depends on whether the source contains GOOD."""

    @staticmethod
    def parse_flags_comment(src):
        import re
        m = re.match(r"\s*/\*\s*flags:\s*(.*?)\s*\*/", src)
        return m.group(1) if m else None

    @staticmethod
    def get_targets():
        return {"f": [0x27BDFFE8] + [0] * 19}


def fake_compile(src, fn, flags=None, **kw):
    if "GOOD" in src:
        return sc(True, 0, ex=20, op=20, orr=20, words=[0x27BDFFE8] + [0] * 19)
    if "BROKEN" in src:
        return sc(err="cfe: Error", words=[], size=0, ex=0, op=0, orr=0, strict=20)
    return sc(words=[0x27BDFFF0] + [0] * 19)


class Pure(unittest.TestCase):
    def test_shapes(self):
        self.assertEqual(autopilot.classify_shape(sc(ex=3, op=8, orr=5))[0], "structural_rewrite")
        self.assertEqual(autopilot.classify_shape(sc(size=40, ex=10, op=19, orr=19))[0], "structural_rewrite")
        self.assertEqual(autopilot.classify_shape(sc(size=23, ex=10, op=19, orr=19))[0], "extra_or_missing_insns")
        self.assertEqual(autopilot.classify_shape(sc(ex=10, op=19, orr=12))[0], "register_only")
        self.assertEqual(autopilot.classify_shape(sc(ex=10, op=19, orr=19))[0], "immediate_or_scheduling")
        self.assertEqual(autopilot.classify_shape(sc(ex=19, op=20, orr=20))[0], "near_miss")
        self.assertEqual(autopilot.classify_shape(sc(err="boom", words=[]))[0], "compile_error")
        self.assertEqual(autopilot.classify_shape(sc(err="no target x", words=[]), ipa=True)[0], "ipa_context_missing")
        self.assertEqual(autopilot.classify_shape(sc(unresolved=["foo"], ex=5))[0], "unresolved_symbols")

    def test_frame_shape(self):
        want = [0x27BDFFE8] + [0] * 19
        got = [0x27BDFFD8] + [0] * 19
        shape = autopilot.classify_shape(sc(ex=10, op=19, orr=12, words=got), want)
        self.assertEqual(shape[0], "frame_mismatch")

    def test_score_key_and_flags(self):
        self.assertLess(autopilot.score_key(sc(True, 0)), autopilot.score_key(sc(ex=19)))
        self.assertLess(autopilot.score_key(sc(ex=15, words=[1])), autopilot.score_key(sc(ex=5, words=[1])))
        self.assertEqual(autopilot.with_opt("-g0 -O2 -mips2", "-O1"), "-g0 -O1 -mips2")
        t = autopilot.with_flags_line("/* flags: -O1 */\nint x;\n", "-O2")
        self.assertTrue(t.startswith("/* flags: -O2 */\nint x;"))
        self.assertEqual(t.count("flags:"), 1)

    def test_compact_residual(self):
        rep = "# R\n\n## Score: `f`\n\n- a\n\n## Frame and registers\n\n- b\n\n## Aligned diff\n\nd1\nd2\n\n## Mutations tried\n\nX\n"
        out = autopilot.compact_residual(rep, 50)
        self.assertIn("- a", out)
        self.assertIn("d2", out)
        self.assertNotIn("Mutations tried", out)
        long = "## Aligned diff\n" + "\n".join("l%d" % i for i in range(200))
        self.assertIn("more lines cut", autopilot.compact_residual(long, 30))


class Triage(unittest.TestCase):
    def test_estimate_order(self):
        base = {"class": "ABI", "words": 60, "seeds": [], "m2c_seedable": True, "callers": 1}
        near = dict(base, seeds=["cloud/work/near-miss/x/base.c"], near_diff=2)
        ipa = dict(base, **{"class": "IPA-caller", "closure_words": 900, "closure_funcs": 12})
        p_base, p_near, p_ipa = (triage.estimate(f)[0] for f in (base, near, ipa))
        self.assertGreater(p_near, p_base)
        self.assertGreater(p_base, p_ipa)
        big = dict(base, words=1200)
        self.assertLess(triage.estimate(big)[1], triage.estimate(base)[1])
        reasons = triage.estimate(near)[2]
        self.assertTrue(any("near-miss" in r for r in reasons))

    def test_exclusions(self):
        with tempfile.TemporaryDirectory() as t:
            root = Path(t)
            (root / "src/blob/groups/g1").mkdir(parents=True)
            (root / "cloud/matches").mkdir(parents=True)
            (root / "cloud/work/ipa-groups/d1").mkdir(parents=True)
            (root / "cloud/work/near-miss/d").mkdir(parents=True)
            (root / "src/blob/a.c").write_text("int func_1(void) {\n return 0;\n}\n")
            (root / "src/blob/groups/g1/group.json").write_text(json.dumps({"members": ["func_2"], "claims": []}))
            (root / "cloud/matches/func_3.c").write_text("void func_3(int a) { }\n")
            (root / "cloud/work/ipa-groups/d1/group.json").write_text(
                json.dumps({"members": ["func_4", "func_5"], "claims": ["func_4"]}))
            (root / "cloud/work/near-miss/d/base.c").write_text("void func_6(void) { }\n")
            wanted = {"func_%d" % i for i in range(1, 8)}
            done = triage.spliced_names(wanted, root)
            self.assertEqual(set(done), {"func_1", "func_2", "func_3", "func_4"})
            self.assertEqual(triage.draft_groups(wanted, root), {"func_4": "d1", "func_5": "d1"})
            self.assertEqual(triage.draft_sources(wanted, root), {"func_6": ["cloud/work/near-miss/d/base.c"]})

    def test_real_cli(self):
        r = subprocess.run([sys.executable, str(ROOT / "cloud/work/tools/amatch/triage.py"), "--top", "3", "--json",
                            "--class", "abi", "--no-closure"], capture_output=True, text=True, cwd=ROOT)
        if r.returncode != 0:
            self.skipTest("triage needs the committed targets: " + r.stderr[-200:])
        rows = json.loads(r.stdout)
        self.assertEqual(len(rows), 3)
        self.assertTrue(all(x["class"] == "ABI" and x["reason"] for x in rows))
        self.assertGreaterEqual(rows[0]["priority"], rows[-1]["priority"])


class Stubbed(unittest.TestCase):
    def make(self, tmp, search, **kw):
        hooks = dict(compile_fn=fake_compile, search=search, verify_fn=lambda s, f, fl: (True, "ok"),
                     m2c_seed=lambda fn: ("int m2c;", {"m2c": {fn: True}}),
                     residual=lambda src, fn, flags, log, n: "RESIDUAL " + fn, repo=tmp)
        hooks.update(kw)
        ap = autopilot.Autopilot(out_dir=Path(tmp) / "ap", budget=5, seconds=1, jobs=1, log=lambda m: None,
                                 flag_sweep=False, **hooks)
        ap.builder = FakeBuilder
        return ap

    def test_flow_matched_and_unfinished_and_resume(self):
        calls = []

        def search(kind, path, fn, flags, budget, seconds, jobs, seed, out, timeout):
            calls.append(fn)
            out = Path(out)
            out.mkdir(parents=True, exist_ok=True)
            if fn == "win":
                (out / "best.c").write_text("/* GOOD */\nint win;\n")
                return {"matched": True, "evals": 7, "best_src_path": str(out / "best.c")}
            (out / "best.c").write_text("int lose;\n")
            return {"matched": False, "evals": 5, "best_src_path": str(out / "best.c")}

        with tempfile.TemporaryDirectory() as t:
            ap = self.make(t, search)
            FakeBuilder.get_targets = staticmethod(lambda: {"win": [0x27BDFFE8] + [0] * 19, "lose": [0x27BDFFE8] + [0] * 19,
                                                            "pre": [0] * 20})
            items = [{"fn": "win", "class": "ABI", "priority": 2}, {"fn": "lose", "class": "ABI", "priority": 1},
                     {"fn": "pre", "class": "ABI", "seed_files": ["seed_pre.c"]}]
            (Path(t) / "seed_pre.c").write_text("/* flags: -g0 -O1 */\n/* GOOD */\nint pre;\n")
            res = ap.run(items)
            self.assertEqual(sorted(res["matched"]), ["pre", "win"])
            self.assertEqual(res["needs_llm"], ["lose"])
            self.assertNotIn("pre", calls)                      # seed already matched: no search
            wl = json.loads(Path(res["worklist"]).read_text())
            self.assertEqual([w["fn"] for w in wl], ["lose"])
            self.assertIn(wl[0]["shape"], autopilot.SUGGEST)
            self.assertIn("RESIDUAL lose", Path(res["report"]).read_text())
            mf = Path(t) / "ap" / "out" / "matches" / "win.c"
            self.assertTrue(mf.read_text().startswith("/* flags: -g0 -O2 -mips2 -G 0 -non_shared */"))
            self.assertFalse((Path(t) / "cloud" / "matches").exists())    # dry run: repo untouched
            n = len(calls)
            ap2 = self.make(t, search)                                      # resume: nothing to do
            ap2.run(items)
            self.assertEqual(len(calls), n)
            ap2.run(items, redo=True)
            self.assertGreater(len(calls), n)

    def test_write_mode_and_no_overwrite(self):
        def search(kind, path, fn, flags, budget, seconds, jobs, seed, out, timeout):
            Path(out).mkdir(parents=True, exist_ok=True)
            (Path(out) / "best.c").write_text("/* GOOD */\nint win;\n")
            return {"matched": True, "best_src_path": str(Path(out) / "best.c")}

        with tempfile.TemporaryDirectory() as t:
            FakeBuilder.get_targets = staticmethod(lambda: {"win": [0x27BDFFE8] + [0] * 19})
            ap = self.make(t, search)
            ap.write = True
            ap.run([{"fn": "win", "class": "ABI"}])
            dest = Path(t) / "cloud" / "matches" / "win.c"
            self.assertTrue(dest.read_text().startswith("/* flags:"))
            dest.write_text("// hand edited\n")
            ap2 = self.make(t, search)
            ap2.write = True
            ap2.run([{"fn": "win", "class": "ABI"}], redo=True)
            self.assertEqual(dest.read_text(), "// hand edited\n")

    def test_verify_failure_is_not_a_match(self):
        def search(kind, path, fn, flags, budget, seconds, jobs, seed, out, timeout):
            Path(out).mkdir(parents=True, exist_ok=True)
            (Path(out) / "best.c").write_text("/* GOOD */\n")
            return {"matched": True, "best_src_path": str(Path(out) / "best.c")}

        with tempfile.TemporaryDirectory() as t:
            FakeBuilder.get_targets = staticmethod(lambda: {"win": [0x27BDFFE8] + [0] * 19})
            ap = self.make(t, search, verify_fn=lambda s, f, fl: (False, "score.py says no"))
            res = ap.run([{"fn": "win", "class": "ABI"}])
            self.assertEqual(res["matched"], [])
            self.assertIn("verify_failed", ap.state["items"]["win"])

    def test_seed_failure_reports_compile_error(self):
        with tempfile.TemporaryDirectory() as t:
            FakeBuilder.get_targets = staticmethod(lambda: {"x": [0] * 20})
            ap = self.make(t, lambda *a, **k: {}, m2c_seed=lambda fn: (_ for _ in ()).throw(RuntimeError("no m2c")))
            res = ap.run([{"fn": "x", "class": "ABI"}])
            self.assertEqual(res["needs_llm"], ["x"])
            self.assertEqual(ap.state["items"]["x"]["shape"], "compile_error")

    def test_group_flow(self):
        with tempfile.TemporaryDirectory() as t:
            root = Path(t)
            g = root / "cloud" / "work" / "ipa-groups" / "grp"
            g.mkdir(parents=True)
            (g / "group.c").write_text("seed")
            (g / "group.json").write_text(json.dumps({"members": ["a", "b", "__standin_c"], "files": ["group.c"],
                                                      "keep": ["a"], "flags": "-O3", "claims": []}))

            def cg(d, overrides=None, **kw):
                good = "good" in (Path(d) / "group.c").read_text()
                mem = {n: sc(good and n != "b", 0 if good and n != "b" else 5) for n in ("a", "b", "__standin_c")}
                return {"err": None, "members": mem, "matched": False}

            def search(kind, path, fn, flags, budget, seconds, jobs, seed, out, timeout):
                b = Path(out) / "best_group"
                b.mkdir(parents=True, exist_ok=True)
                (b / "group.c").write_text("good")
                (b / "group.json").write_text((Path(path) / "group.json").read_text())
                return {"matched": False, "best_src_path": str(b)}

            FakeBuilder.get_targets = staticmethod(lambda: {"a": [0] * 20, "b": [0] * 20})
            ap = self.make(t, search, compile_group=cg, verify_group=lambda d, c: (True, "ok"),
                           residual_group=lambda d, log, n: "GRES")
            res = ap.run([{"fn": "a", "class": "IPA-leaf", "group": "grp"}])
            self.assertEqual(res["matched"], ["a"])
            sj = json.loads((Path(t) / "ap" / "out" / "groups" / "grp" / "group.json").read_text())
            self.assertEqual(sj["claims"], ["a"])                       # b unmatched, stand-in never claimed
            self.assertEqual(json.loads((g / "group.json").read_text())["claims"], [])   # repo untouched
            res = ap.run([{"fn": "b", "class": "IPA-leaf", "group": "grp"}])
            self.assertEqual(res["needs_llm"], ["b"])
            self.assertEqual(ap.state["items"]["b"]["partial_claims"], ["a"])


@NEED_IDO
class RealIDO(unittest.TestCase):
    def test_selftest(self):
        r = subprocess.run([sys.executable, str(ROOT / "cloud/work/tools/amatch/autopilot.py"), "--selftest"],
                           capture_output=True, text=True, cwd=ROOT, timeout=900)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("PASS", r.stdout)


if __name__ == "__main__":
    unittest.main()
