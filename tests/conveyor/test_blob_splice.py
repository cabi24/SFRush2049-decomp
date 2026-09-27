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


def test_a_spliced_function_contributes_its_compiled_bytes(tmp_path):
    """Spliced C supplies bytes, not a section: IDO pads .text to 16 bytes,
    so linking its object into the image would overwrite the next function."""
    import struct

    from tools.conveyor.pipeline import blob_tu

    region = {"name": "r", "vaddr_start": 0x80086A50, "vaddr_end": 0x80086A60,
              "entries": [
                  {"kind": "function", "vaddr": 0x80086A50, "size": 8,
                   "target_id": "kept"},
                  {"kind": "function", "vaddr": 0x80086A58, "size": 8,
                   "target_id": "compiled"}]}
    image = struct.pack(">IIII", 0x11111111, 0x22222222, 0x33333333, 0x44444444)
    body = struct.pack(">II", 0xAAAABBBB, 0xCCCCDDDD)

    text = blob_tu.render_region(region, image, "image.bin",
                                 spliced={"compiled": body})

    assert "    .word 0x11111111" in text          # untouched neighbour
    assert "    .word 0xAAAABBBB" in text          # compiled body
    assert "    .word 0x33333333" not in text      # its image words replaced
    assert "compiled from src/blob/compiled.c" in text
    # both functions still define their symbols at their own addresses
    assert text.index(".section .text.kept") < text.index(".section .text.compiled")


def test_coverage_counts_only_spliced_functions_and_names_the_rom_path(tmp_path, capsys):
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
    # Since 009 the ROM's blob is compressed from this image, so the output
    # says how the functions reach the cartridge instead of disclaiming it.
    assert "cartridge coverage" in out and "blob_rom" in out


def test_a_permuter_win_splices_its_winning_source_not_the_seed(tmp_path):
    """The seed of a permuter-matched function scored nonzero by definition;
    splicing it gets refused. Four refusals were exactly this."""
    import io
    import tarfile

    from tools.conveyor.coordinator import db as dbmod

    def result_blob(score, body):
        buf = io.BytesIO()
        with tarfile.open(fileobj=buf, mode="w:gz") as tar:
            for name, data in (("result.json", json.dumps(
                    {"payload": {"final_best_score": score}}).encode()),
                               ("best.c", body.encode())):
                info = tarfile.TarInfo(name)
                info.size = len(data)
                tar.addfile(info, io.BytesIO(data))
        return buf.getvalue()

    blobs = tmp_path / "blobs"
    blobs.mkdir()
    (blobs / "lose").write_bytes(result_blob(40, "void f(void) { /* stalled */ }"))
    (blobs / "win").write_bytes(result_blob(0, "void f(void) { /* winner */ }"))
    conn = dbmod.connect(tmp_path / "db.sqlite")
    with dbmod.tx(conn):
        for job, sha, when in (("a", "win", "2026-09-20"), ("b", "lose", "2026-09-26")):
            conn.execute(
                "INSERT INTO work_unit (job_id,job_type,target_id,manifest_sha,"
                "state,result_sha,created_at,updated_at)"
                " VALUES (?,'permuter_search','f','m','DONE',?,?,?)",
                (job, sha, when, when))

    assert "winner" in blob_splice.winning_search_source(conn, "f", blobs)
    assert blob_splice.winning_search_source(conn, "nobody", blobs) is None
