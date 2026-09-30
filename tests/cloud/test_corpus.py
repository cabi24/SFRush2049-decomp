"""Offline tests for cloud/work/tools/amatch/corpus.py (committed corpus_data only).

IDO-dependent checks skip cleanly when tools/cloud/ido/cc is missing.
Run: python3 -m unittest tests.cloud.test_corpus   (or pytest tests/cloud/test_corpus.py)
"""
import json
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools" / "amatch"))
import corpus  # noqa: E402


class CorpusData(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.idx = corpus.load_index()
        cls.hl = corpus.header_lines()

    def test_size(self):
        self.assertGreaterEqual(len(corpus.with_start(self.idx)), 40)

    def test_files_and_fields(self):
        for e in self.idx["entries"]:
            self.assertTrue((corpus.DATA / e["goal_file"]).exists(), e["fn"])
            self.assertIn(e["status"], ("ok", "no_start"))
            if e["status"] == "ok":
                self.assertTrue((corpus.DATA / e["start_file"]).exists(), e["fn"])
                self.assertTrue(set(e["edit"]["classes"]) <= set(corpus.CLASSES), e["fn"])
                self.assertIn(e["edit"]["primary"], e["edit"]["classes"])
            else:
                self.assertIsNone(e["start_file"])
                self.assertTrue(e["no_start_reason"])

    def test_goal_defines_function_and_has_flags(self):
        for e in self.idx["entries"]:
            src = corpus.read_source(e, "goal")
            body = corpus.extract_fn(src, e["fn"]) if e["kind"] == "single" else src
            self.assertTrue(body and "{" in body, e["fn"])
            self.assertIn("-mips2", e["flags"])

    def test_start_differs_from_goal(self):
        for e in corpus.with_start(self.idx):
            s, g = corpus.read_source(e, "start"), corpus.read_source(e, "goal")
            if e["kind"] == "single":
                s, g = corpus.extract_fn(s, e["fn"]), corpus.extract_fn(g, e["fn"])
            self.assertNotEqual(corpus.norm_ws(s), corpus.norm_ws(g), e["fn"])

    def test_pack_roundtrip(self):
        for e in self.idx["entries"][:25]:
            if not e.get("packed"):
                continue
            full = corpus.read_source(e, "goal")
            self.assertEqual(corpus.unpack(corpus.pack(full, self.hl), self.hl), full)

    def test_stats_md_matches_index(self):
        allc, _ = corpus.histogram(self.idx)
        self.assertEqual(corpus.stats_text(self.idx), (corpus.DATA / "STATS.md").read_text())
        self.assertEqual(sum(allc.values()) >= len(corpus.with_start(self.idx)), True)

    def test_score_records_present_for_singles(self):
        for e in corpus.with_start(self.idx):
            if e["kind"] == "single":
                sc = e["start"]["score"]
                self.assertTrue(sc["compiles"] and not sc["matched"], e["fn"])
                self.assertGreater(sc["strict_diff"], 0, e["fn"])


class Classifier(unittest.TestCase):
    def cls(self, a, b):
        return set(corpus.classify(None, None, "f", textwrap.dedent(a), textwrap.dedent(b))["classes"])

    def test_loop_form(self):
        a = "void f(s32 n) {\n s32 i;\n for (i = 0; i < n; i++) { g(i); }\n}"
        b = "void f(s32 n) {\n s32 i;\n i = 0;\n do { g(i); i++; } while (i < n);\n}"
        self.assertIn("loop-form", self.cls(a, b))

    def test_type_change(self):
        a = "void f(s16 a) {\n g(a);\n}"
        b = "void f(s32 a) {\n g(a);\n}"
        self.assertIn("type-change", self.cls(a, b))

    def test_local_dropped_and_order(self):
        a = "s32 f(s32 a) {\n s32 x;\n s32 y;\n x = a + 1;\n y = x;\n return y;\n}"
        b = "s32 f(s32 a) {\n s32 y;\n return a + 1;\n}"
        self.assertIn("local-dropped", self.cls(a, b))
        c = "s32 f(s32 a) {\n s32 y;\n s32 x;\n x = a + 1;\n y = x;\n return y;\n}"
        self.assertIn("decl-order", self.cls(a, c))

    def test_operand_flip_and_literal(self):
        a = "s32 f(s32 a, s32 b) {\n return a + b;\n}"
        b = "s32 f(s32 a, s32 b) {\n return b + a;\n}"
        self.assertIn("operand-flip", self.cls(a, b))
        c = "f32 f(f32 a) {\n return a * 1;\n}"
        d = "f32 f(f32 a) {\n return a * 1.0f;\n}"
        self.assertIn("literal-type", self.cls(c, d))

    def test_case_order(self):
        a = "void f(s32 a) {\n switch (a) {\n case 1: g(); break;\n case 2: h(); break;\n }\n}"
        b = "void f(s32 a) {\n switch (a) {\n case 2: h(); break;\n case 1: g(); break;\n }\n}"
        self.assertIn("case-order", self.cls(a, b))

    def test_extern_to_defined(self):
        a = "extern s32 D_1;\ns32 f(void) {\n return D_1;\n}"
        b = "s32 D_1;\ns32 f(void) {\n return D_1;\n}"
        r = corpus.classify(a, b, "f")
        self.assertIn("extern-to-defined", r["classes"])


class Bench(unittest.TestCase):
    def test_bench_with_stub(self):
        with tempfile.TemporaryDirectory() as d:
            stub = Path(d) / "stub.py"
            stub.write_text(textwrap.dedent('''
                import json, sys
                fn = sys.argv[2]
                assert "--json" in sys.argv and "--budget" in sys.argv and "--flags" in sys.argv
                src = open(sys.argv[1]).read()
                assert fn in src
                ok = sum(map(ord, fn)) % 2 == 0
                print("noise")
                print(json.dumps({"matched": ok, "evals": 7, "seconds": 0.1,
                                  "best_score": {"strict_diff": 0 if ok else 3}}))
            '''))
            out = Path(d) / "res.json"
            rows, results = corpus.bench("%s %s" % (sys.executable, stub), 5, 3, limit=8, out_json=out)
            self.assertEqual(rows["ALL"]["n"], 8)
            self.assertEqual(rows["ALL"]["hit"], sum(r["matched"] for _, r in results))
            self.assertEqual(len(json.loads(out.read_text())), 8)

    def test_bench_bad_command(self):
        rows, results = corpus.bench("%s -c pass" % sys.executable, 1, 1, limit=1)
        self.assertEqual(rows["ALL"]["err"], 1)
        self.assertEqual(rows["ALL"]["hit"], 0)


@unittest.skipUnless(corpus.ido_available(), "IDO not installed")
class Scoring(unittest.TestCase):
    def test_goal_scores_match(self):
        idx = corpus.load_index()
        singles = [e for e in idx["entries"] if e["kind"] == "single"][:2]
        for e in singles:
            with tempfile.TemporaryDirectory() as d:
                f = Path(d) / (e["fn"] + ".c")
                corpus.main(["materialize", e["fn"], "goal", "--out", str(f)])
                self.assertTrue(corpus.score_fn(f, e["fn"], e["flags"])["matched"], e["fn"])

    def test_recorded_start_score(self):
        idx = corpus.load_index()
        e = next(x for x in corpus.with_start(idx) if x["kind"] == "single")
        with tempfile.TemporaryDirectory() as d:
            f = Path(d) / (e["fn"] + ".c")
            corpus.main(["materialize", e["fn"], "start", "--out", str(f)])
            sc = corpus.score_fn(f, e["fn"], e["flags"])
        self.assertEqual(sc["strict_diff"], e["start"]["score"]["strict_diff"])


if __name__ == "__main__":
    unittest.main()
