"""tools/recomp/oracle.py diff logic on hand-written logs (no ROM, no host).
The end-to-end run needs the private ROM and the recomp host and skips
without them."""
import importlib.util
import os
import sys
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location("recomp_oracle", REPO / "tools/recomp/oracle.py")
oracle = importlib.util.module_from_spec(spec)
sys.modules["recomp_oracle"] = oracle
spec.loader.exec_module(oracle)


def write_log(path, frames, repl=None, events=(), stacks=()):
    """frames: [(dl, {page: hash}, stk)] -> a host-format oracle log; pages
    are written only when they change (as the host does)."""
    lines = ["# rush2049 frame oracle v1: pages=2048 page_bytes=4096 replaced=%d" % len(repl or {})]
    for top in stacks:
        lines.append("S 0 0x%08X" % top)
    prev = {}
    for i, (dl, pages, stk) in enumerate(frames, 1):
        for f, text in events:
            if f == i:
                lines.append("E %d %s" % (i, text))
        lines.append("F %d dl=%s dls=1 dlb=8 ram=0 stk=%s" % (i, dl, stk))
        changed = {p: h for p, h in pages.items() if prev.get(p) != h}
        prev.update(pages)
        if changed:
            lines.append("P" + "".join(" %d:%s" % (p, h) for p, h in sorted(changed.items())))
    for name, calls in (repl or {}).items():
        lines.append("R %s %d" % (name, calls))
    lines.append("END %d frames" % len(frames))
    Path(path).write_text("\n".join(lines) + "\n")
    return oracle.Run(path)


BASE = [("d1", {0: "a", 5: "b"}, "s1"), ("d2", {0: "a", 5: "c"}, "s2"), ("d3", {0: "a", 5: "c", 9: "x"}, "s3")]


def test_log_parsing_reconstructs_pages(tmp_path):
    run = write_log(tmp_path / "a.log", BASE, repl={"r=ido:f": 3}, events=[(2, "load ovl_b")],
                    stacks=[0x80030000])
    assert run.next_frame() and run.frame == 1 and run.pages == {0: "a", 5: "b"}
    assert run.next_frame() and run.pages == {0: "a", 5: "c"} and run.fields["dl"] == "d2"
    assert run.next_frame() and run.pages[9] == "x"
    assert not run.next_frame()
    assert run.events == [(2, "load ovl_b")] and run.repl == {"r=ido:f": 3}
    assert run.end == (3, "frames") and run.stacks == [0x80030000]


def test_identical_runs_are_equivalent(tmp_path):
    r = oracle.compare(write_log(tmp_path / "c.log", BASE, repl={"x": 5}), [write_log(tmp_path / "b.log", BASE)])
    assert r["verdict"] == "EQUIVALENT" and r["first_diff"] is None and r["frames"] == 3


def test_page_difference_reports_first_frame_and_pages(tmp_path):
    cand = [BASE[0], ("d2", {0: "a", 5: "Z"}, "s2"), BASE[2]]
    r = oracle.compare(write_log(tmp_path / "c.log", cand), [write_log(tmp_path / "b.log", BASE)])
    assert r["verdict"] == "DIFFERENT"
    assert r["first_diff"]["frame"] == 2 and r["first_diff"]["pages"] == [5] and not r["first_diff"]["dl"]
    text = "\n".join(oracle.report(r, {5: ["gThing"]}))
    assert "DIFFERENT at frame 2" in text and "0x80005000-0x80005FFF  gThing" in text


def test_display_list_difference(tmp_path):
    cand = [BASE[0], BASE[1], ("dX", BASE[2][1], "s3")]
    r = oracle.compare(write_log(tmp_path / "c.log", cand), [write_log(tmp_path / "b.log", BASE)])
    assert r["first_diff"]["frame"] == 3 and r["first_diff"]["dl"] and r["first_diff"]["pages"] == []


def test_pages_baselines_disagree_on_are_not_judged(tmp_path):
    other = [("d1", {0: "a", 5: "q"}, "s1"), ("d2", {0: "a", 5: "q"}, "s2"), ("d3", {0: "a", 5: "q", 9: "x"}, "s3")]
    cand = [("d1", {0: "a", 5: "w"}, "s1"), ("d2", {0: "a", 5: "w"}, "s2"), ("d3", {0: "a", 5: "w", 9: "x"}, "s3")]
    r = oracle.compare(write_log(tmp_path / "c.log", cand),
                       [write_log(tmp_path / "b1.log", BASE), write_log(tmp_path / "b2.log", other)])
    assert r["verdict"] == "EQUIVALENT"
    assert r["unstable_pages"] == {"0x80005000-0x80005FFF": 3}
    # ... but a page they agree on still is
    cand[2] = ("d3", {0: "a", 5: "w", 9: "y"}, "s3")
    r = oracle.compare(write_log(tmp_path / "c2.log", cand),
                       [write_log(tmp_path / "b3.log", BASE), write_log(tmp_path / "b4.log", other)])
    assert r["verdict"] == "DIFFERENT" and r["first_diff"]["pages"] == [9]


def test_display_lists_baselines_disagree_on_end_the_horizon(tmp_path):
    other = [BASE[0], ("dQ", BASE[1][1], "s2"), BASE[2]]
    cand = [BASE[0], ("dW", BASE[1][1], "s2"), ("dW", BASE[2][1], "s3")]
    r = oracle.compare(write_log(tmp_path / "c.log", cand),
                       [write_log(tmp_path / "b1.log", BASE), write_log(tmp_path / "b2.log", other)])
    assert r["verdict"] == "EQUIVALENT" and r["horizon"] == 1


def test_stack_only_difference_is_reported_not_judged(tmp_path):
    cand = [(d, p, "other") for d, p, _s in BASE]
    r = oracle.compare(write_log(tmp_path / "c.log", cand), [write_log(tmp_path / "b.log", BASE)])
    assert r["verdict"] == "EQUIVALENT" and r["stack_diff_frame"] == 1
    assert any("thread-stack contents differ from frame 1" in l for l in oracle.report(r))


def test_unexercised_replacement(tmp_path):
    r = oracle.compare(write_log(tmp_path / "c.log", BASE, repl={"a": 4, "b": 0}),
                       [write_log(tmp_path / "b.log", BASE)])
    assert r["verdict"] == "NOT EXERCISED"


def test_event_difference(tmp_path):
    r = oracle.compare(write_log(tmp_path / "c.log", BASE, events=[(3, "load ovl_b")]),
                       [write_log(tmp_path / "b.log", BASE, events=[(2, "load ovl_b")])])
    assert r["verdict"] == "DIFFERENT" and r["first_diff"] == {"frame": None, "events": True}


def test_frame_numbering_mismatch_is_an_error(tmp_path):
    a = write_log(tmp_path / "a.log", BASE)
    p = tmp_path / "b.log"
    p.write_text("F 7 dl=d1 dls=1 dlb=8 ram=0 stk=s\n")
    with pytest.raises(oracle.OracleError):
        oracle.compare(oracle.Run(p), [a])
    p.write_text("Q bogus\n")
    with pytest.raises(oracle.OracleError):
        list(oracle.iter_log(open(p)))


def test_stack_ranges_follow_the_host_rule():
    assert oracle.stack_ranges([0x80030000, 0x80031000, 0x80040000]) == [
        (0x80030000 - oracle.MAX_STACK, 0x80030000), (0x80030000, 0x80031000),
        (0x80040000 - oracle.MAX_STACK, 0x80040000)]


def test_word_diff_skips_ranges_and_names_words(tmp_path):
    a = bytearray(64)
    b = bytearray(64)
    b[8:12] = b"\x00\x00\x00\x10"
    b[32:36] = b"\xff\xff\xff\xff"
    syms = tmp_path / "d.toml"
    syms.write_text('    { name = "gFlags", vram = 0x80000008 },\n')
    out = oracle.word_diff(bytes(a), bytes(b), syms, skip=[(0x80000020, 0x80000040)])
    assert out == [(0x80000008, "00000000", "00000010", "gFlags+0x0")]


def test_cli_compare(tmp_path, capsys):
    write_log(tmp_path / "a.log", BASE)
    write_log(tmp_path / "b.log", BASE, repl={"r": 1})
    assert oracle.main(["compare", str(tmp_path / "a.log"), str(tmp_path / "b.log"),
                        "--json", str(tmp_path / "v.json")]) == 0
    assert "EQUIVALENT over 3 frames" in capsys.readouterr().out
    assert (tmp_path / "v.json").is_file()


HOST = os.environ.get("RUSH2049_RECOMP_HOST")


@pytest.mark.skipif(not HOST or not Path(HOST).is_file(),
                    reason="private input: set RUSH2049_RECOMP_HOST to a built host (and its synthetic ROM)")
def test_end_to_end_stock_is_equivalent_to_itself(tmp_path):
    rom = os.environ["RUSH2049_RECOMP_ROM"]
    rc = oracle.main(["run", "--stock-host", HOST, "--repl-host", HOST, "--rom-stock", rom, "--rom-repl", rom,
                      "--out", str(tmp_path), "--frames", "300"])
    assert rc == 0
