import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from amatch import mutate  # noqa: E402


class LineLayout(unittest.TestCase):
    def variants(self, source, fn="f"):
        result = mutate.mutations(source, fn, ["line_join"])
        for mutation in result:
            self.assertEqual(mutate.sig_tokens_text(source), mutate.sig_tokens_text(mutation.new_src))
            self.assertFalse(mutation.semantic_risk)
            self.assertEqual(mutate.apply(source, mutation.id), mutation.new_src)
        return [mutation.new_src for mutation in result]

    def test_adjacent_stores_and_function_scope(self):
        source = "void f(int *p) {\n p[0]=1;\n p[1]=2;\n}\nvoid g(void) {\n h();\n}\n"
        variants = self.variants(source)
        self.assertTrue(any("p[0]=1; p[1]=2;" in text for text in variants))
        for text in variants:
            self.assertEqual(text[text.index("void g"):], source[source.index("void g"):])

    def test_compound_loop_boundary(self):
        source = "void f(void) {\n int i;\n for(i=0;i<4;i++)\n {\n g(i);\n h(i);\n }\n}\n"
        variants = self.variants(source)
        self.assertTrue(any("for(i=0;i<4;i++) {" in text for text in variants))
        self.assertTrue(any("{ g(i);" in text for text in variants))
        self.assertTrue(any("g(i); h(i);" in text for text in variants))

    def test_comments_are_never_merged_or_modified(self):
        source = "void f(void) {\n a(); // keep next statement live\n b(); /* block\n comment */\n c();\n}\n"
        for text in self.variants(source):
            self.assertIn("a(); // keep next statement live\n b();", text)
            self.assertIn("b(); /* block\n comment */\n c();", text)

    def test_preprocessor_directives_and_continuations_stay_on_lines(self):
        source = "#define DO(x) \\\n x\nvoid f(void) {\n a();\n#if ENABLED\n b();\n#endif\n c();\n}\n"
        for text in self.variants(source):
            self.assertIn("#define DO(x) \\\n x\n", text)
            self.assertIn("a();\n#if ENABLED\n", text)
            self.assertIn("b();\n#endif\n c();", text)

    def test_string_contents_and_escaped_line_are_preserved(self):
        source = 'void f(void) {\n puts("a; b"); \\\n puts("c");\n}\n'
        for text in self.variants(source):
            self.assertIn('puts("a; b"); \\\n puts("c");', text)

    def test_line_dependent_macros_disable_family(self):
        for source in ("void f(void) {\n a(__LINE__);\n b();\n}",
                       "#define LOCATION __LINE__\nvoid f(void) {\n a(LOCATION);\n b();\n}"):
            self.assertEqual(self.variants(source), [])

    def test_continued_line_comment_cannot_swallow_next_statement(self):
        source = "void f(void) {\n a(); // still a comment \\\n b();\n c();\n}\n"
        self.assertEqual(self.variants(source), [])


if __name__ == "__main__":
    unittest.main()
