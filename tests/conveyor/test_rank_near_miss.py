"""Strict ranking must retain uncertainty and remove only plain MATCH results."""
import subprocess

import pytest

from tools.cloud import rank_near_miss as rank

ORIGINAL = """# Single-function near misses

| function | score | source | flags |
|---|---|---|---|
| a | 5 | permuter | `-g0 -O2 -mips2 -G 0 -non_shared` |
| b | 40 | m2c sweep | `-g0 -O1 -mips2 -G 0 -non_shared` |
"""


@pytest.mark.parametrize("summary,exit_code,words,matched", [
    ("MATCH", 0, 0, True),
    ("1/20 words differ", 1, 1, False),
    ("2/20 words differ (3 extra words (nonzero beyond target length))", 1, 5, False),
    ("MISMATCH (3 extra words (nonzero beyond target length))", 1, 3, False),
    ("MATCH (2 section-relative relocations unverified: .rodata+0x0, .rodata+0x4)", 1, 0, False),
    ("NOT VERIFIED (unresolved symbols: unknown)", 1, 0, False),
    ("NOT VERIFIED (unsupported relocation type 42 at .text+0x0)", 1, 0, False),
])
def test_interprets_strict_summaries_and_preserves_recorded_flags(
        tmp_path, monkeypatch, summary, exit_code, words, matched):
    entry = rank.read_entries(ORIGINAL)[1]

    def run(command, **kwargs):
        assert command[2:] == ["fn", str(tmp_path / "b/base.c"), "b", "--flags=" + entry.flags]
        assert "--allow-unverified" not in command
        return subprocess.CompletedProcess(command, exit_code, "b:\n  " + summary + "\n", "")

    monkeypatch.setattr(rank.subprocess, "run", run)
    result = rank.evaluate(entry, tmp_path)
    assert (result.words, result.matched, result.summary) == (words, matched, summary)


@pytest.mark.parametrize("summary,stderr,code", [
    ("MATCH", "", 1), ("1/20 words differ", "", 0),
    ("", "target integrity check failed: bad hash", 1),
    ("", "Traceback (most recent call last): runtime failure", 1),
    ("", "compiler missing: run tools/cloud/setup.sh", 1),
    ("", "unknown output format", 1), ("", "terminated", -9),
])
def test_tool_or_integrity_failures_abort_instead_of_ranking_as_zero(
        tmp_path, monkeypatch, summary, stderr, code):
    monkeypatch.setattr(rank.subprocess, "run", lambda *a, **kw:
                        subprocess.CompletedProcess(a[0], code, summary, stderr))
    with pytest.raises(RuntimeError):
        rank.evaluate(rank.read_entries(ORIGINAL)[0], tmp_path)


def test_compile_failure_is_unscored_not_a_false_match(tmp_path, monkeypatch):
    monkeypatch.setattr(rank.subprocess, "run", lambda *a, **kw:
                        subprocess.CompletedProcess(a[0], 1, "", "IDO compile failed:\ninvalid C"))
    result = rank.evaluate(rank.read_entries(ORIGINAL)[0], tmp_path)
    assert result.words is None and not result.matched
    assert "invalid C" in result.summary


def test_render_sorts_by_strict_count_and_preserves_matches_for_reruns():
    a, b = rank.read_entries(ORIGINAL)
    c = rank.Entry("c", 2, "permuter", a.flags)
    d = rank.Entry("d", 3, "permuter", a.flags)
    e = rank.Entry("e", 4, "permuter", a.flags)
    results = [rank.Result(a, 7, "7/10 words differ"), rank.Result(b, 1, "1/10 words differ"),
               rank.Result(c, 0, "MATCH", True), rank.Result(d, None, "Not scored: compile failed"),
               rank.Result(e, 0, "NOT VERIFIED (unresolved symbols: unknown)")]
    rendered = rank.render(results)
    worklist, matches = rendered.split("## Strict matches removed from the worklist")
    assert worklist.index("| e |") < worklist.index("| b |") < worklist.index("| a |") < worklist.index("| d |")
    assert "| c |" not in worklist and "| c |" in matches
    assert "| b | 40 | 1 | m2c sweep |" in worklist
    assert sorted(rank.read_entries(rendered), key=lambda e: e.name) == [a, b, c, d, e]
    assert rank.render(list(reversed(results))) == rendered


@pytest.mark.parametrize("text", ["no table", ORIGINAL + ORIGINAL,
                                  ORIGINAL.replace("| a |", "| ../a |"),
                                  ORIGINAL.replace("`-g0 -O1 -mips2 -G 0 -non_shared`", "``")])
def test_invalid_index_cannot_silently_drop_rows(text):
    with pytest.raises(ValueError):
        rank.read_entries(text)


@pytest.fixture
def index(tmp_path, monkeypatch):
    path = tmp_path / "INDEX.md"
    path.write_text(ORIGINAL)
    for name in ("a", "b"):
        (tmp_path / name).mkdir()
        (tmp_path / name / "base.c").write_text("int f(void);")
    (tmp_path / "SHA256SUMS").write_text("fixed manifest")
    monkeypatch.setattr(rank.score, "ASM_DIR", tmp_path)
    monkeypatch.setattr(rank.score, "targets", lambda: {})
    monkeypatch.setattr(rank.score, "image_symbols", lambda: {})
    monkeypatch.setattr(rank.score, "ido", lambda name: "/compiler")
    return path


def test_late_failure_leaves_original_index_untouched(index, monkeypatch):
    def evaluate(entry, directory):
        if entry.name == "b":
            raise RuntimeError("compiler died")
        return rank.Result(entry, 0, "MATCH", True)

    monkeypatch.setattr(rank, "evaluate", evaluate)
    with pytest.raises(RuntimeError, match="compiler died"):
        rank.rank(index)
    assert index.read_text() == ORIGINAL


@pytest.mark.parametrize("changed", ["INDEX.md", "SHA256SUMS"])
def test_concurrent_edit_aborts_without_overwriting(index, monkeypatch, changed):
    def evaluate(entry, directory):
        (index.parent / changed).write_text("concurrent edit")
        return rank.Result(entry, 1, "1/10 words differ")

    monkeypatch.setattr(rank, "evaluate", evaluate)
    with pytest.raises(RuntimeError, match="changed during ranking"):
        rank.rank(index)
    assert index.read_text() == ("concurrent edit" if changed == "INDEX.md" else ORIGINAL)


def test_rerun_is_byte_identical_and_rechecks_reported_matches(index, monkeypatch):
    calls = []

    def evaluate(entry, directory):
        calls.append(entry.name)
        return rank.Result(entry, 0 if entry.name == "a" else 1,
                           "MATCH" if entry.name == "a" else "1/10 words differ", entry.name == "a")

    monkeypatch.setattr(rank, "evaluate", evaluate)
    rank.rank(index)
    first = index.read_bytes()
    rank.rank(index)
    assert index.read_bytes() == first
    assert calls == ["a", "b", "b", "a"]
    assert not list(index.parent.glob(".rank-*"))
