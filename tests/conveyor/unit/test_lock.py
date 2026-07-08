"""Regression lock for verified-matched functions (`pipeline.lock`)."""
from tools.conveyor.pipeline import lock as lockmod

FUNC = """\
/* header comment */
u32 strlen(const char *str) {
    const char *p;
    p = str;            /* trailing comment */
    if (*(u8 *)p != 0) {
        do {
            p++;
        } while (*(u8 *)p != 0);
    }
    return (u32)(p - str);
}
"""

REFLOWED = """\
u32 strlen(const char *str)
{
    const char *p;   // new comment style
    p = str;
    if (*(u8 *)p != 0)
    {
        do { p++; } while (*(u8 *)p != 0);
    }
    return (u32)(p - str);
}
"""


def test_normalize_ignores_comments_and_reflow():
    assert (lockmod.normalize_body(FUNC)
            == lockmod.normalize_body(REFLOWED))


def test_normalize_catches_code_changes():
    drifted = FUNC.replace("p++;", "++p;")
    assert (lockmod.normalize_body(FUNC)
            != lockmod.normalize_body(drifted))


def test_normalize_preserves_string_literals():
    a = lockmod.normalize_body('void f(void) { puts("a  b"); }')
    b = lockmod.normalize_body('void f(void) { puts("a b"); }')
    assert a != b
    assert '"a  b"' in a


def _repo(tmp_path):
    src = tmp_path / "src"
    src.mkdir()
    (src / "a.c").write_text(FUNC + "\nvoid other(void) { return; }\n")
    return tmp_path


def test_check_holds_then_flags_drift(tmp_path):
    repo = _repo(tmp_path)
    sha = lockmod.body_sha(repo / "src" / "a.c", "strlen")
    entries = {"src/a.c:strlen": {"body_sha256": sha}}
    assert lockmod.check(entries, repo=repo) == []

    # Reflow/comment edits keep the lock green…
    (repo / "src" / "a.c").write_text(REFLOWED)
    assert lockmod.check(entries, repo=repo) == []

    # …a code edit trips it.
    (repo / "src" / "a.c").write_text(REFLOWED.replace("p++;", "++p;"))
    problems = lockmod.check(entries, repo=repo)
    assert len(problems) == 1 and problems[0][0] == "src/a.c:strlen"


def test_check_flags_missing_function(tmp_path):
    repo = _repo(tmp_path)
    entries = {"src/a.c:gone": {"body_sha256": "x"}}
    (spec, why), = lockmod.check(entries, repo=repo)
    assert "not found" in why


def test_reduced_tu_keeps_context_and_target_only():
    tu = (
        '#include "types.h"\n'
        "extern int shared_state;\n"
        "static int helper(int x) { return x + 1; }\n"
        "int keep_me(int x) { return helper(x) + shared_state; }\n"
        "int drop_me(void) { return 0; }\n"
    )
    reduced = lockmod.reduced_tu(tu, "keep_me")
    assert "keep_me" in reduced
    assert "extern int shared_state;" in reduced
    assert '#include "types.h"' in reduced
    assert "{ return x + 1; }" not in reduced
    assert "drop_me" not in reduced


def test_resolve_headers_is_transitive(tmp_path):
    (tmp_path / "include" / "PR").mkdir(parents=True)
    (tmp_path / "include" / "types.h").write_text("typedef int s32;\n")
    (tmp_path / "include" / "PR" / "os.h").write_text(
        '#include "types.h"\ntypedef struct { s32 n; } OSQ;\n'
    )
    src = tmp_path / "src"
    src.mkdir()
    tu = src / "m.c"
    tu.write_text('#include "PR/os.h"\nvoid f(OSQ *q) { q->n = 0; }\n')

    found = lockmod.resolve_headers(tu, repo=tmp_path)
    assert set(found) == {"include/PR/os.h", "include/types.h"}
