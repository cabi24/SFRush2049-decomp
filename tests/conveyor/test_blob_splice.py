"""008 splice: verified C into the image, gated (contract §14-§16)."""
import json

import pytest

from tools.conveyor.pipeline import blob_splice


def _lock(tmp_path, entries):
    path = tmp_path / "blob_matched.lock.json"
    path.write_text(json.dumps(entries, indent=2, sort_keys=True) + "\n")
    return path


def test_lock_records_provenance_and_round_trips(tmp_path):
    path = _lock(tmp_path, {})
    entries = {"fn": {"source": "src/blob/fn.c", "source_sha256": "a" * 64,
                      "flagset": "-O2", "toolkit_sha": "t" * 64,
                      "verified": "image_gate", "verified_at": "2026-09-24"}}
    blob_splice.save_lock(entries, path)
    assert blob_splice.load_lock(path) == entries
    assert blob_splice.spliced_targets(blob_splice.load_lock(path)) == {"fn"}


def test_check_refuses_a_body_whose_source_drifted(tmp_path, monkeypatch):
    src = tmp_path / "src" / "blob"
    src.mkdir(parents=True)
    (src / "fn.c").write_text("void fn(void) {}\n")
    monkeypatch.setattr(blob_splice, "REPO", tmp_path)
    path = _lock(tmp_path, {
        "fn": {"source": "src/blob/fn.c",
               "source_sha256": blob_splice.source_sha("void fn(void) {}\n"),
               "flagset": "-O2", "toolkit_sha": "t", "verified": "image_gate",
               "verified_at": "2026-09-24"},
        "gone": {"source": "src/blob/gone.c", "source_sha256": "b" * 64,
                 "flagset": "-O2", "toolkit_sha": "t", "verified": "image_gate",
                 "verified_at": "2026-09-24"},
    })

    assert blob_splice.check(path) == [("gone", "source missing")]

    (src / "fn.c").write_text("void fn(void) { return; }\n")
    assert blob_splice.check(path) == [("fn", "source hash drifted"),
                                       ("gone", "source missing")]


def test_generated_region_omits_a_spliced_function(tmp_path):
    """Its object provides the section; two definitions would not link."""
    from tools.conveyor.pipeline import blob_tu

    region = {"name": "r", "vaddr_start": 0x80086A50, "vaddr_end": 0x80086A60,
              "entries": [
                  {"kind": "function", "vaddr": 0x80086A50, "size": 8,
                   "target_id": "kept"},
                  {"kind": "function", "vaddr": 0x80086A58, "size": 8,
                   "target_id": "gone"}]}
    image = bytes(16)

    text = blob_tu.render_region(region, image, "image.bin", spliced={"gone"})

    assert ".section .text.kept" in text
    assert ".section .text.gone" not in text
    assert "gone: spliced" in text          # the omission is explained in place


def test_coverage_counts_only_spliced_functions_and_states_the_caveat(tmp_path, capsys):
    document = {
        "image": {"size": 1000},
        "totals": {"functions": 3},
        "regions": [{"entries": [
            {"kind": "function", "vaddr": 0, "size": 40, "target_id": "a"},
            {"kind": "function", "vaddr": 40, "size": 60, "target_id": "b"},
            {"kind": "opaque", "vaddr": 100, "size": 900}]}],
    }
    path = _lock(tmp_path, {"a": {"source": "src/blob/a.c",
                                  "source_sha256": "x" * 64, "flagset": "-O2",
                                  "toolkit_sha": "t", "verified": "image_gate",
                                  "verified_at": "2026-09-24"}})

    stats = blob_splice.coverage(document, path)

    assert stats["functions"] == 1 and stats["total_functions"] == 3
    assert stats["bytes"] == 40 and stats["percent"] == pytest.approx(4.0)
    blob_splice._print_coverage(stats)
    out = capsys.readouterr().out
    assert "image coverage is not cartridge coverage" in out
