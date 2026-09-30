import json
import shutil
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "cloud" / "work" / "tools"))
from amatch import builder, probe  # noqa: E402

NEED_IDO = unittest.skipUnless((ROOT / "tools" / "cloud" / "ido" / "cc").exists(),
                               "IDO missing (tools/cloud/setup.sh)")
MODE_BYTE = ROOT / "cloud" / "work" / "ipa-groups" / "mode_byte_set"

SRC = """\
typedef signed short s16;
extern int G;
extern void ext(void *p, void *q);
int helper(int a, int b, int c);
int helper(int a, int b, int c)
{
    int t = a;   /* decl */
    if (0) { switch (a) { case 1: G = 3; } }
    G = a + b;
    return t;
}
void caller(s16 x)
{
    helper(x, 1, 2);
    ext(0, 0);
    helper(3, 4, 5);
}
"""


def asm_i(op, rs, rt, imm):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def asm_r(rs, rt, rd, fn):
    return (rs << 21) | (rt << 16) | (rd << 11) | fn


T0, T1, A1, V0, SP, RA, S0 = 8, 9, 5, 2, 29, 31, 16
WORDS = [
    asm_i(9, SP, SP, -24),          # addiu sp,sp,-24
    asm_i(43, SP, RA, 20),          # sw ra,20(sp)
    asm_i(9, 0, T0, 0),             # addiu t0,zero,0   (call setup)
    0x0C000000,                     # jal
    0,                              # nop (delay)
    asm_r(T1, A1, V0, 33),          # addu v0,t1,a1     (reads t1, a1 after the call)
    asm_i(35, SP, RA, 20),          # lw ra,20(sp)
    asm_i(9, SP, SP, 24),           # addiu sp,sp,24
    0x03E00008,                     # jr ra
    0,
]


class CScan(unittest.TestCase):
    def test_mask_keeps_length_and_blanks_comments(self):
        t = 'int a; /* x(y) */ char *s = "f(1)"; // z(2)\nint b;'
        m = probe.mask_c(t)
        self.assertEqual(len(m), len(t))
        self.assertNotIn("x(y)", m)
        self.assertNotIn("f(1)", m)
        self.assertNotIn("z(2)", m)
        self.assertIn("int b;", m)

    def test_parse_defs_decls(self):
        p = probe.parse(SRC)
        self.assertEqual([f.name for f in p.defs], ["helper", "caller"])
        self.assertEqual([f.name for f in p.decls], ["ext", "helper"])
        h = p.find("helper", "def")
        self.assertEqual([q.name for q in h.params], ["a", "b", "c"])
        self.assertEqual(p.find("caller", "def").params[0].type, "s16")
        self.assertEqual(p.find("ext").params[0].cls, "ptr")
        self.assertEqual(len(p.calls_in_bodies("helper")), 2)

    def test_params_change_fixes_defs_decls_and_calls(self):
        out = probe.tx_params({"a.c": SRC}, "helper", 2, False)["a.c"]
        p = probe.parse(out)
        self.assertEqual(len(p.find("helper", "def").params), 2)
        self.assertIn("helper(x, 1);", out)
        self.assertIn("helper(3, 4);", out)
        out = probe.tx_params({"a.c": SRC}, "helper", 5, True)["a.c"]
        self.assertIn("int __p3, int __p4", out)
        self.assertIn("helper(x, 1, 2, 0, 0);", out)
        self.assertIn("__probe_sink += (int) __p4;", out)
        self.assertIn("extern int __probe_sink;", probe.with_sink_decl({"a.c": out})["a.c"])
        self.assertEqual(len(probe.parse(out).defs), 2)

    def test_demote_turns_dropped_params_into_locals(self):
        out = probe.tx_params({"a.c": SRC}, "helper", 1, False, demote=True)["a.c"]
        self.assertIn("int b = 0; int c = 0;", out)
        self.assertEqual(len(probe.parse(out).find("helper", "def").params), 1)

    def test_kr_decl(self):
        out = probe.tx_params({"a.c": SRC}, "ext", None, False)["a.c"]
        self.assertIn("extern void ext();", out)
        self.assertEqual(out.count("ext(0, 0);"), 1)

    def test_ret_and_argtype(self):
        out = probe.tx_ret({"a.c": SRC}, "helper", "void")["a.c"]
        self.assertIn("void helper(int a, int b, int c)\n{", out)
        self.assertIn("return;", out)
        out = probe.tx_ret({"a.c": SRC}, "ext", "int")["a.c"]
        self.assertIn("extern int ext(", out)
        out = probe.tx_argtype({"a.c": SRC}, "helper", 1, "unsigned char")["a.c"]
        self.assertIn("int helper(int a, unsigned char b, int c)", out)

    def test_inline_toggle_inserts_after_declarations(self):
        files = {"a.c": SRC}
        self.assertTrue(probe.has_dead_switch(files, "helper"))
        off = probe.tx_inline(files, "helper", False)
        self.assertFalse(probe.has_dead_switch(off, "helper"))
        on = probe.tx_inline(off, "helper", True)
        self.assertTrue(probe.has_dead_switch(on, "helper"))
        t = on["a.c"]
        self.assertLess(t.index("int t = a;"), t.index("if (0)"))
        self.assertLess(t.index("if (0)"), t.index("G = a + b;"))
        on2 = probe.tx_inline(files, "caller", True)["a.c"]
        body = on2[on2.index("void caller(s16 x)"):]
        self.assertLess(body.index("if (0)"), body.index("helper(x"))
        self.assertIn("switch (x)", body)

    def test_sites_add_and_remove(self):
        files, keep = probe.tx_sites({"a.c": SRC}, ["caller"], "helper", 2)
        self.assertEqual(keep, ["caller", "__probe_site_helper_0", "__probe_site_helper_1"])
        self.assertIn("helper(1, 1, 1);", files["a.c"])
        self.assertIn("helper(2, 2, 2);", files["a.c"])
        self.assertEqual(probe.count_call_statements({"a.c": SRC}, "helper"), 2)
        out = probe.tx_sites_remove({"a.c": SRC}, "helper", "last")["a.c"]
        self.assertEqual(probe.count_call_statements({"a.c": out}, "helper"), 1)
        self.assertEqual(probe.tx_sites_remove({"a.c": SRC}, "helper", "all")["a.c"].count("helper("), 2)

    def test_order_keeps_functions(self):
        out = probe.tx_order({"a.c": SRC}, "reverse", "caller")["a.c"]
        p = probe.parse(out)
        self.assertEqual([f.name for f in p.defs], ["caller", "helper"])
        self.assertIn("void caller(s16 x);", out)

    def test_patch(self):
        out = probe.tx_patch({"a.c": SRC}, {"name": "n", "find": r"helper\(3, 4, 5\)", "replace": "helper(9, 9, 9)",
                                             "edits": [{"find": "G = a \\+ b", "replace": "G = b"}],
                                             "append": "int extra;"})["a.c"]
        self.assertIn("helper(9, 9, 9)", out)
        self.assertIn("G = b;", out)
        self.assertTrue(out.rstrip().endswith("int extra;"))


class Registers(unittest.TestCase):
    def test_signature(self):
        s = probe.analyze_words(WORDS)
        self.assertEqual(s["frame"], 24)
        self.assertEqual(sorted(s["entry"]), ["a1", "t1"])      # t0 is defined before use; ra/sp ignored
        self.assertEqual(s["setup"], [["t0"]])                   # li t0,0 just before the jal, unread
        self.assertEqual(sorted(s["across"][0]), ["a1", "t1"])   # live across the call
        self.assertEqual(s["unsaved"], [])
        self.assertEqual(s["calls"], 1)

    def test_unsaved_s_register(self):
        words = [asm_i(9, SP, SP, -8), asm_i(9, 0, S0, 1), asm_r(S0, 0, V0, 33), 0x03E00008, asm_i(9, SP, SP, 8)]
        self.assertEqual(probe.analyze_words(words)["unsaved"], ["s0"])
        saved = [words[0], asm_i(43, SP, S0, 0)] + words[1:]
        s = probe.analyze_words(saved)
        self.assertEqual(s["unsaved"], [])
        self.assertEqual(s["entry"], [])                         # the sw s0 is a save, not a read

    def test_abi_expected(self):
        ps = [probe.Param(x) for x in ("int a", "float b", "void *c")]
        self.assertEqual(probe.abi_expected(ps), ["a0", "a1", "a2"])
        ps = [probe.Param(x) for x in ("float a", "float b", "int c")]
        self.assertEqual(probe.abi_expected(ps), ["f12", "f14", "a2"])

    def test_distance_and_diff(self):
        a = probe.analyze_words(WORDS)
        b = json.loads(json.dumps(a))
        self.assertEqual(probe.sig_dist(a, b), 0)
        b["setup"] = [["t1"]]
        self.assertEqual(probe.sig_dist(a, b), 2)
        self.assertEqual(list(probe.sig_diff(a, b)), ["setup"])
        self.assertEqual(probe.sig_dist(None, b), 99)


class Variants(unittest.TestCase):
    def setUp(self):
        self.tmp = Path(tempfile.mkdtemp(prefix="probe-test-"))
        (self.tmp / "g.c").write_text(SRC)
        (self.tmp / "group.json").write_text(json.dumps({
            "members": ["caller"], "files": ["g.c"], "keep": ["caller"],
            "flags": "-g0 -O3 -mips2 -G 0 -non_shared", "context": [], "targets": {}, "claims": []}))

    def tearDown(self):
        shutil.rmtree(self.tmp, ignore_errors=True)

    def test_matrix_shape_and_realize(self):
        ctx = probe.Ctx(self.tmp, "caller")
        self.assertEqual(probe.default_focus(ctx), ["caller", "ext", "helper"])
        vs = probe.build_variants(ctx, probe.ALL_AXES, ["helper"])
        labels = [v.label for v in vs]
        self.assertEqual(labels[0], "base")
        for want in ("sites(helper)=+3", "sites(helper)=-1", "params(helper)=d0-demote", "params(helper)=d5+use",
                     "ret(helper)=void", "args(helper)=p0:short", "inline(helper)=off", "flags=-O2",
                     "order=reverse", "keep(helper)=in", "keep(caller)=out"):
            self.assertIn(want, labels)
        self.assertNotIn("ret(helper)=int", labels)              # already int
        by = {v.label: v for v in vs}
        files, keep, flags, rev = probe.realize(ctx, by["keep(helper)=in"])
        self.assertEqual(keep, ["caller", "helper"])
        files, keep, flags, rev = probe.realize(ctx, by["flags=-O2"])
        self.assertIn("-O2", flags)
        files, keep, flags, rev = probe.realize(ctx, by["sites(helper)=+2"])
        self.assertEqual(len(keep), 3)
        self.assertEqual(len(probe.parse(files["g.c"]).defs), 4)
        ext_ref = by["params(ext)=K&R()"] if "params(ext)=K&R()" in by else None
        self.assertIsNone(ext_ref)                               # only when ext is a focus

    def test_every_variant_source_still_parses(self):
        ctx = probe.Ctx(self.tmp, "caller")
        for v in probe.build_variants(ctx, probe.ALL_AXES, ["helper", "ext"]):
            files, keep, flags, rev = probe.realize(ctx, v)
            p = probe.parse(files["g.c"])
            self.assertIn("caller", [f.name for f in p.defs], v.label)

    def test_rule_discovery_series(self):
        sig = lambda reg: {"entry": ["a0"], "setup": [[reg]], "across": [[]], "frame": 24, "unsaved": []}
        rows = []
        for i, (val, reg) in enumerate([("base", "t0"), ("d3+use", "t1"), ("d4+use", "t2"), ("ret", "t0")]):
            rows.append({"id": i, "label": "x", "axis": "base" if i == 0 else ("ret" if val == "ret" else "params"),
                         "focus": None if i == 0 else "callee", "value": val, "err": None, "sig": {"m": sig(reg)},
                         "strict_diff": 0, "aligned_exact": 5, "reg_dist": 0, "matched": True})
        res = {"rows": rows, "by_id": {r["id"]: r for r in rows}, "member": "m"}
        rules = probe.discover_rules(res)
        self.assertEqual(len(rules["series"]), 1)
        s = rules["series"][0]
        self.assertEqual((s["axis"], s["field"], s["function"]), ("params", "setup", "m"))
        self.assertEqual(s["values"]["d4+use"], [["t2"]])
        res["rules"] = rules
        self.assertIn("d3+use:#1[t1]", probe.rule_sentences(res)[0])
        self.assertEqual(rules["no_effect"], ["x"])

    def test_render_without_ido(self):
        row = {"id": 0, "label": "base", "axis": "base", "focus": None, "value": "base", "err": None, "sig": {},
               "strict_diff": 3, "aligned_exact": 10, "aligned_opcode": 10, "aligned_opcode_reg": 9, "size": 12,
               "target_size": 12, "matched": False, "group_strict": 3, "reg_dist": None}
        res = {"group": "g", "member": "m", "axes": ["params"], "focus": ["f"], "aux": [], "target_sig": None,
               "target_words": None, "stage1": 1, "total": 1, "secs": 0.1, "rows": [row], "by_id": {0: row}}
        res["rules"] = probe.discover_rules(res)
        md = probe.render_markdown(res)
        self.assertIn("| 1 | base | 3 |", md)
        self.assertIn("Rule discovery", md)
        json.dumps(probe.to_json(res))


def _targets_ok():
    try:
        return bool(builder.get_targets())
    except BaseException:
        return False


@NEED_IDO
@unittest.skipUnless(MODE_BYTE.exists() and _targets_ok(), "mode_byte_set group or asm targets missing")
class Integration(unittest.TestCase):
    def test_mode_byte_set_register_rule(self):
        res = probe.probe(MODE_BYTE, "mode_byte_set", ["params", "sites"], focus=["func_80096288"], jobs_n=2,
                          combine=0)
        self.assertNotIn("error", res)
        base = res["by_id"][0]
        self.assertTrue(base["matched"])
        self.assertEqual(base["sig"]["mode_byte_set"]["setup"], [["t0"]])
        regs = {}
        for r in res["rows"]:
            if r["axis"] == "params" and not r["err"]:
                regs[r["value"]] = r["sig"]["mode_byte_set"]["setup"][0]
        self.assertEqual(regs["d3+use"], ["t1"])
        self.assertEqual(regs["d4+use"], ["t2"])
        self.assertEqual(regs["d4"], ["t0"])                     # declared but unused: no change
        self.assertTrue(any(s["axis"] == "params" for s in res["rules"]["series"]))
        self.assertIn("Rule discovery", probe.render_markdown(res))


if __name__ == "__main__":
    unittest.main()
