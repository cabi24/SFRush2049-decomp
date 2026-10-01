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


def test_image_symbols_keeps_every_name_that_shares_an_address():
    """A data-symbol label at a function's address must not shadow the
    function's own name (entity_flags_apply vs frame_sync at 0x80092360)."""
    document = {"regions": [{"entries": [
        {"kind": "function", "target_id": "entity_flags_apply",
         "vaddr": 0x80092360}]}]}

    provides = blob_splice.image_symbols(
        document, symbols={0x80092360: "frame_sync", 0x80152000: "pad_config"})

    assert provides == {"entity_flags_apply": 0x80092360,
                        "frame_sync": 0x80092360, "pad_config": 0x80152000}


def test_an_address_name_resolves_to_the_address_it_spells():
    assert blob_splice.address_named("func_803914b4") == 0x803914B4
    assert blob_splice.address_named("D_80152000") == 0x80152000
    assert blob_splice.address_named("entity_flags_apply") is None
    assert blob_splice.address_named("func_8009") is None


@pytest.mark.parametrize("storage", [".bss\n.space 16", ".data\n.word 7, 8, 9, 10", "common"])
def test_unit_defined_global_resolves_to_existing_image_storage(tmp_path, storage):
    """A defined global must use its image address, including LO16 carry.

    Definitions let as1 share a high-address load; emitting fresh storage
    beside the function instead silently changes every reference to it.
    """
    import shutil
    import struct
    import subprocess
    from tools.conveyor.pipeline import blob_build

    if not all(shutil.which(t) for t in
               ("mips-linux-gnu-as", "mips-linux-gnu-ld",
                "mips-linux-gnu-objcopy", "mips-linux-gnu-readelf")):
        pytest.skip("MIPS binutils absent")
    src, obj = tmp_path / "fn.s", tmp_path / "fn.o"
    data = (".comm D_8013C300,16,4\n" if storage == "common" else
            storage.split("\n", 1)[0] + "\n.globl D_8013C300\n"
            ".type D_8013C300,@object\nD_8013C300:\n" +
            storage.split("\n", 1)[1] + "\n")
    src.write_text(
        '.section .text.fn,"ax",@progbits\n.set noreorder\n'
        '.globl fn\n.type fn,@function\nfn:\n'
        'lui $at,%hi(D_8013C300)\n'
        'sw $zero,%lo(D_8013C300)($at)\n'
        'jr $ra\nnop\n.size fn,.-fn\n' + data)
    subprocess.run(["mips-linux-gnu-as", "-mips2", "-o", str(obj), str(src)],
                   check=True, capture_output=True)
    # A conflicting provide for fn must not move its own compiled definition.
    body = blob_splice.link_function(
        obj, "fn", 0x80086A50, 16,
        {"D_8013C300": 0x8013C300, "fn": 0x80090000}, work=tmp_path / "link")
    assert body == struct.pack(">IIII", 0x3C018014, 0xAC20C300, 0x03E00008, 0)
    assert blob_splice.defined_data_symbols(obj) == {"D_8013C300"}
    assert blob_build.function_symbols(obj)["fn"][0] == 0
