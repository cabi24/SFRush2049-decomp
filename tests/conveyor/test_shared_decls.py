"""Frontier plan workstream E step 1: pure logic of the shared-declaration generator."""
import collections

from tools.conveyor.pipeline import shared_decls as sd


def ti(text):
    decls = sd.parse_declaration("extern " + text)
    assert len(decls) == 1, decls
    return decls[0][1]


def ev(*accesses, **kw):
    """ev("lh", "lh", ("lw", 4)) -> Evidence; a tuple is an indexed access with that stride."""
    out = sd.Evidence()
    for n, acc in enumerate(accesses):
        ref = {"f": kw.get("f", "fn%d" % n), "addr": 0x80150000, "kind": "load", "mn": acc}
        if isinstance(acc, tuple):
            ref.update(mn=acc[0], indexed=True, stride=acc[1])
        if ref["mn"] == "addiu":
            ref["kind"] = "form"
        elif ref["mn"].startswith("s"):
            ref["kind"] = "store"
        out.add(ref)
    return out


def cand(text, users=0, files=None, default=False):
    c = sd.Cand(ti(text))
    c.users = ["u%d.c" % i for i in range(users)]
    c.files = c.users + ["d%d.c" % i for i in range((files or users) - users)]
    c.default = default
    return c


# --- parsing ----------------------------------------------------------------

def test_parse_declaration_lists_qualifiers_and_primitive_spellings():
    decls = sd.parse_declaration("extern s32 D_80140AD8, *D_80140B08, D_80140BD8[4]")
    assert [(n, sd.spell(t)) for n, t in decls] == [
        ("D_80140AD8", "s32 $"), ("D_80140B08", "s32 *$"), ("D_80140BD8", "s32 $[4]")]
    assert sd.spell(ti("volatile unsigned char D_8012E700")) == "volatile u8 $"
    assert ti("unsigned D_80110000").base == "u32"
    assert ti("int D_80110000") == ti("s32 D_80110000")
    assert ti("char D_80110000[]").base == "char"          # distinct C type from u8
    assert ti("u8 D_80150B70[][0x98]").dims == (None, 0x98)
    assert ti("struct CamNode *D_801391F0").base == "struct CamNode"
    assert sd.is_struct_typed(ti("Ent D_8012E700[]"))
    assert sd.is_struct_typed(ti("void (*D_80110000)(void)"))
    assert not sd.is_struct_typed(ti("void *D_80110000"))
    assert not sd.is_struct_typed(ti("const char *D_80110000"))


def test_function_prototypes_are_not_globals():
    assert sd.parse_declaration("extern s32 osRecvMesg(OSMesgQueue *, void **, s32)") == []


def test_scalar_typedef_aliases_resolve_to_builtins():
    aliases = sd.scalar_aliases("typedef s32 M2C_UNK; typedef unsigned short ushort_t; typedef struct A B;")
    assert aliases == {"M2C_UNK": "s32", "ushort_t": "u16"}
    assert sd.parse_declaration("extern M2C_UNK D_80152770", aliases)[0][1] == ti("s32 D_80152770")


def test_parse_source_maps_names_and_separates_use_from_declaration():
    text = """
    typedef s32 M2C_UNK;
    extern s16 active_player_count;   /* comment D_80999999 */
    extern s32 D_80110000, D_80110004;
    extern u8 D_80110008[];
u8 D_80110680[6];   /* definitions are recognised at column 0 only */
    #if 0
    extern f32 D_80110000;
    #endif
    s32 f(void) { return active_player_count + D_80110004 + D_80110680[1]; }
    """
    decls, used = sd.parse_source(text, {"active_player_count": 0x8014A108})
    got = {addr: sd.spell(t, name) for addr, name, t in decls}
    assert got == {0x8014A108: "s16 active_player_count", 0x80110000: "s32 D_80110000",
                   0x80110004: "s32 D_80110004", 0x80110008: "u8 D_80110008[]",
                   0x80110680: "u8 D_80110680[6]"}
    assert used == {0x8014A108, 0x80110004, 0x80110680}


def test_uses_inside_group_standin_functions_do_not_count():
    text = """
    extern s32 D_80110000;
    extern s32 D_80110004;
    void member(void) { D_80110004 = 1; }
    void context_only(s32 a) { if (a) { D_80110000 = 2; } }
    """
    assert [n for n, _a, _b in sd.function_spans(text)] == ["member", "context_only"]
    _decls, used = sd.parse_source(text, standins={"context_only"})
    assert used == {0x80110004}
    _decls, used = sd.parse_source(text)
    assert used == {0x80110000, 0x80110004}


# --- comparison ---------------------------------------------------------------

def test_compare_classes():
    assert sd.compare(ti("s16 a"), ti("s16 a")) == "identical"
    assert sd.compare(ti("volatile s16 a"), ti("s16 a")) == "qualifier"
    assert sd.compare(ti("s32 a[4]"), ti("s32 a[]")) == "bound"
    assert sd.compare(ti("s32 a[4]"), ti("s32 a[8]")) == "shape"
    assert sd.compare(ti("char a"), ti("u8 a")) == "repr"
    assert sd.compare(ti("s16 a"), ti("u16 a")) == "signedness"
    assert sd.compare(ti("u32 a[]"), ti("s32 a[4]")) == "signedness"
    assert sd.compare(ti("s16 a"), ti("s32 a")) == "shape"
    assert sd.compare(ti("s32 a"), ti("f32 a")) == "shape"
    assert sd.compare(ti("s32 a"), ti("s32 *a")) == "shape"
    assert sd.compare(ti("s32 a"), ti("s32 a[]")) == "shape"
    assert sd.compare(ti("u8 *a"), ti("void *a")) == "shape"


# --- retail evidence ------------------------------------------------------------

def test_settled_type_from_access_widths():
    assert ev("lh", "lh", "sh").settled_type() == ("s16", "high")
    assert ev("lhu").settled_type() == ("u16", "medium")
    assert ev("lwc1", "swc1").settled_type() == ("f32", "high")
    assert ev("lw", "sw").settled_type() == ("s32", "width")      # signedness not observable
    assert ev("lb", "lbu").settled_type()[0] is None
    assert ev("sb").settled_type()[0] is None
    assert ev("lw", "lbu").settled_type()[0] is None
    assert ev("sb").width_type() == "s8"
    assert ev("lbu", "lbu", "lb").width_type() == "u8"
    assert ev("lw", "lbu").width_type() is None


def test_decl_vs_evidence():
    assert sd.decl_vs_evidence(ti("s16 a"), ev("lh", "sh")) == "ok"
    assert sd.decl_vs_evidence(ti("u16 a"), ev("lh", "sh")) == "signedness"
    assert sd.decl_vs_evidence(ti("u16 a"), ev("lh", "lhu")) == "ok"         # mixed loads settle nothing
    assert sd.decl_vs_evidence(ti("s32 a"), ev("lh")) == "width"
    assert sd.decl_vs_evidence(ti("s32 a"), ev("lwc1")) == "width"
    assert sd.decl_vs_evidence(ti("u8 *a"), ev("lw", "sw")) == "ok"
    assert sd.decl_vs_evidence(ti("s32 a"), ev(("lw", 4))) == "shape"
    assert sd.decl_vs_evidence(ti("s32 a[]"), ev(("lw", 4))) == "ok"
    assert sd.decl_vs_evidence(ti("s32 a"), ev("addiu")) == "no-evidence"
    assert sd.decl_vs_evidence(ti("s32 a"), None) == "no-evidence"
    assert sd.decl_vs_evidence(ti("Ent a[]"), ev("lw")) == "n/a"


def test_scan_stride_of_one_is_an_unrecovered_scale_not_a_byte_stride():
    e = ev(("lwc1", 1))
    assert e.indexed == 1 and e.stride() is None
    assert sd.decl_vs_evidence(ti("f32 a"), e) == "shape"


# --- precedence ------------------------------------------------------------------

def test_retail_beats_a_used_declaration_of_the_wrong_signedness():
    choice = sd.choose([cand("s16 a", users=1), cand("u16 a", users=3, files=400, default=True)],
                       ev("lh", "lh", "sh"))
    assert sd.spell(choice.ti) == "s16 $"
    assert (choice.status, choice.basis, choice.confidence) == ("resolved", "evidence", "high")
    assert [sd.spell(c.ti) for c, _cat, _v in choice.dissent] == ["u16 $"]


def test_used_declaration_beats_unused_generated_default():
    choice = sd.choose([cand("u32 a", users=2), cand("s32 a", users=0, files=400, default=True)],
                       ev("lw", "sw"))
    assert sd.spell(choice.ti) == "u32 $"
    assert choice.status == "superseded" and choice.dissent == []


def test_generated_default_is_the_fallback():
    choice = sd.choose([cand("s32 a", users=0, files=400, default=True)], None)
    assert (sd.spell(choice.ti), choice.status, choice.basis) == ("s32 $", "clean", "default")


def test_signedness_retail_cannot_settle_stays_open():
    choice = sd.choose([cand("s32 a", users=3, files=400, default=True), cand("u32 a", users=1)],
                       ev("lw", "sw"))
    assert choice.status == "open" and sd.spell(choice.ti) == "s32 $"
    assert choice.confidence == "medium"


def test_volatile_is_never_promoted_and_is_not_a_conflict_by_itself():
    choice = sd.choose([cand("volatile s16 a", users=2), cand("u16 a", users=0, files=400, default=True)],
                       ev("lh", "lh", "sh"))
    assert sd.spell(choice.ti) == "s16 $" and "volatile" not in choice.ti.quals
    assert choice.status == "superseded"
    # a volatile view that fits only its own function loses to retail and is not counted as a rival
    choice = sd.choose([cand("volatile s16 a", users=2), cand("u16 a", users=0, files=400, default=True)],
                       ev("lhu", "lhu", "sh"))
    assert sd.spell(choice.ti) == "u16 $" and choice.status == "clean"


def test_scalar_declaration_of_an_indexed_address_becomes_an_array():
    choice = sd.choose([cand("s32 a", users=1, files=400, default=True)], ev("lw", ("lw", 4)))
    assert sd.spell(choice.ti) == "s32 $[]"
    assert (choice.status, choice.basis, choice.confidence) == ("resolved", "evidence", "high")
    wide = sd.choose([cand("s32 a", users=1, files=400, default=True)], ev(("lw", 0x10)))
    assert wide.bucket == "struct"                       # stride wider than the element: a record


def test_array_bounds_merge():
    choice = sd.choose([cand("s32 a[4]", users=0, files=400, default=True), cand("s32 a[]", users=2)],
                       ev(("lw", 4)))
    assert sd.spell(choice.ti) == "s32 $[4]" and choice.status == "clean"
    choice = sd.choose([cand("u8 a[4]", users=1), cand("u8 a[8]", users=1)], ev(("lbu", 1)))
    assert sd.spell(choice.ti) == "u8 $[]" and choice.status == "open"


def test_struct_typed_addresses_are_left_for_later():
    choice = sd.choose([cand("Ent a[]", users=2), cand("s32 a", users=0, files=400, default=True)], ev("lw"))
    assert choice.bucket == "struct" and choice.ti is None
    choice = sd.choose([cand("s32 a", users=1)], ev("lw"), record_array={"base": 0x8012E700, "stride": 0x44})
    assert choice.bucket == "struct"


def test_no_declaration_fits_takes_the_type_from_retail():
    choice = sd.choose([cand("u8 a", users=0, files=400, default=True)], ev("lb", "lb", "sb"))
    assert (sd.spell(choice.ti), choice.status, choice.confidence) == ("s8 $", "resolved", "high")
    choice = sd.choose([cand("s32 a", users=0, files=400, default=True)], ev("sb"))
    assert (sd.spell(choice.ti), choice.status, choice.confidence) == ("s8 $", "open", "low")


# --- header round trip and check ---------------------------------------------------

HEADER = """\
extern s16 active_player_count;    /* 8014A108 | use 3/3 decl 9 | lh:2 */
extern s32 D_80110000[];           /* 80110000 | use 1/1 decl 9 | lw:1 idx[0x4] */
extern u16 D_80110010;             /* 80110010 | use 0/0 decl 9 | lhu:2 */
#if 0
TODO(struct) 8012E700 | Ent D_8012E700[] x2/2 | lw:3
#endif
"""


def test_parse_header_and_check_source():
    declared, todo = sd.parse_header(HEADER)
    assert {a: sd.spell(t, n) for a, (n, t) in declared.items()} == {
        0x8014A108: "s16 active_player_count", 0x80110000: "s32 D_80110000[]", 0x80110010: "u16 D_80110010"}
    assert todo == {0x8012E700}
    decls = [(0x8014A108, "D_8014A108", ti("volatile s16 a")), (0x80110000, "D_80110000", ti("s32 a[4]")),
             (0x80110010, "D_80110010", ti("s16 a")), (0x8012E700, "D_8012E700", ti("s32 a")),
             (0x80119999, "D_80119999", ti("s32 a"))]
    rows = sd.check_source(decls, {0x8014A108, 0x80110000}, declared, todo)
    assert [r[3] for r in rows] == ["qualifier", "bound", "signedness", "struct_todo", "absent"]
    assert sd.worst_class(rows) == "absent"
    assert sd.worst_class(rows, used_only=True) == "bound"
    assert sd.worst_class([]) == "identical"


# --- call-argument scan ---------------------------------------------------------------

def _lui(rt, imm): return (0x0F << 26) | (rt << 16) | imm
def _addiu(rt, rs, imm): return (0x09 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)
def _lw(rt, rs, imm): return (0x23 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)
def _sw(rt, rs, imm): return (0x2B << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)
def _jal(target): return (0x03 << 26) | ((target & 0x0FFFFFFF) >> 2)


RECV, JAM, WORK = 0x80007270, 0x800075E0, 0x80095F00
NAMES = {RECV: "osRecvMesg", JAM: "osJamMesg", WORK: "work"}


def test_scan_calls_reads_arguments_including_the_delay_slot():
    words = [_lui(4, 0x8015), _jal(RECV), _addiu(4, 4, 0x2770),      # osRecvMesg(&D_80152770)
             _jal(WORK), 0,
             _lui(4, 0x8015), _addiu(4, 4, 0x2770), _jal(JAM), 0]
    calls, _bumps = sd.scan_calls(words, 0x800A0F6C, NAMES)
    assert [(c, a.get(0)) for _pc, c, a in calls] == [
        ("osRecvMesg", ("abs", 0x80152770)), ("work", None), ("osJamMesg", ("abs", 0x80152770))]
    assert sd.critical_sections(calls) == {("abs", 0x80152770)}
    typed, _wrappers = sd.infer_sdk_types({"f": calls})
    assert {t for (t, _callee) in typed[("abs", 0x80152770)]} == {"OSMesgQueue"}


def test_wrapper_forwarding_its_argument_types_its_callers_argument():
    wrapper = sd.scan_calls([_jal(JAM), 0], 0x8010FC2C, NAMES)[0]          # a0 passed straight through
    caller = sd.scan_calls([_lui(4, 0x8014), _jal(0x8010FC2C), _addiu(4, 4, 0x61D0)], 0x800B0000,
                           {0x8010FC2C: "call_osjammsg"})[0]
    typed, wrappers = sd.infer_sdk_types({"call_osjammsg": wrapper, "caller": caller})
    assert wrappers == {"call_osjammsg": {0: "OSMesgQueue"}}
    assert ("abs", 0x801461D0) in typed


def test_display_list_cursor_idiom_is_counted():
    words = [_lui(3, 0x8015), _lw(2, 3, 0x9438 - 0x10000), _addiu(8, 2, 8), _sw(8, 3, 0x9438 - 0x10000)]
    _calls, bumps = sd.scan_calls(words, 0x80086A50, {})
    assert bumps == collections.Counter({0x80149438: 1})


# --- names audit rules -------------------------------------------------------------------

def test_audit_flags_queue_named_as_struct_and_scalar_named_as_array():
    names = [(0x801461D0, "gMainGameStruct", "symbol_addrs.us.txt", "main game state structure", False),
             (0x8014A110, "gTrackDataA", "symbol_addrs.us.txt", "track data array A", False),
             (0x80142B08, "gStartTime", "symbol_addrs.us.txt", "race start (s32)", False)]
    typed = {("abs", 0x801461D0): collections.Counter({("OSMesgQueue", "osRecvMesg"): 3})}
    evidence = {0x8014A110: ev("lw", "lw", "lw", "sw"), 0x80142B08: ev("lh", "lh")}
    findings = sd.audit_globals(names, evidence, typed, collections.Counter(), {}, lambda a: None,
                                {("abs", 0x801461D0): {"f"}})
    by_rule = {f["rule"]: f for f in findings}
    assert by_rule["sdk-arg"]["contradicted"] and by_rule["sdk-arg"]["proposal"] == "gLockQueue_801461D0 : OSMesgQueue"
    assert by_rule["scalar-not-aggregate"]["addr"] == 0x8014A110
    assert by_rule["type-claim"]["addr"] == 0x80142B08 and "s16" in by_rule["type-claim"]["proposal"]


def test_name_bound_to_two_addresses_is_reported():
    names = [(0x801174B4, "gstate", "symbol_addrs.us.txt", "", False)]
    multi_addr, _multi_name = sd.audit_name_collisions(names, {"gstate": 0x801146EC})
    assert [a for a, _s in multi_addr["gstate"]] == [0x801146EC, 0x801174B4]
