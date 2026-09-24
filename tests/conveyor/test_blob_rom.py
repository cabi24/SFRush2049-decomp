"""009: the cartridge's game-code blob is built from sources."""
import shutil
import subprocess
import sys
from pathlib import Path

import pytest

from tools import compose_data
from tools.conveyor.pipeline import blob_rom

REPO = Path(__file__).resolve().parents[2]


# --- composition (the Makefile's data.o step) --------------------------------

def test_compose_replaces_exactly_the_slot():
    data = bytes(range(10)) * 2                  # 20 bytes
    composed = compose_data.compose(data, b"ABCD", slot=5, length=4)
    assert composed == data[:5] + b"ABCD" + data[9:]
    assert len(composed) == len(data)


def test_compose_never_reads_the_original_slot_bytes():
    """If the original bytes leaked through, the ROM gate would pass without
    proving the blob came from the pipeline."""
    data = b"\x00" * 4 + b"ORIGINAL" + b"\x00" * 4
    composed = compose_data.compose(data, b"REPLACED", slot=4, length=8)
    assert b"ORIGINAL" not in composed and b"REPLACED" in composed


@pytest.mark.parametrize("blob", [b"ABC", b"ABCDE"])
def test_a_wrong_length_blob_is_refused_not_padded_or_truncated(blob):
    with pytest.raises(ValueError, match="refusing"):
        compose_data.compose(b"x" * 20, blob, slot=5, length=4)


def test_a_slot_outside_the_segment_is_refused():
    with pytest.raises(ValueError, match="outside"):
        compose_data.compose(b"x" * 10, b"ABCD", slot=8, length=4)


def test_the_cli_fails_loudly_on_a_bad_blob(tmp_path):
    (tmp_path / "data.bin").write_bytes(b"x" * 20)
    (tmp_path / "blob").write_bytes(b"AB")
    proc = subprocess.run(
        [sys.executable, str(REPO / "tools" / "compose_data.py"),
         str(tmp_path / "data.bin"), str(tmp_path / "blob"),
         str(tmp_path / "out"), "--slot", "5", "--length", "4"],
        capture_output=True, text=True)
    assert proc.returncode != 0 and "refusing" in proc.stderr
    assert not (tmp_path / "out").exists()


def test_the_makefile_has_no_fallback_to_the_extracted_blob():
    """A missing blob must be a build failure: `data.o` depends on
    GAME_BLOB and nothing in the Makefile knows how to make it."""
    makefile = (REPO / "Makefile").read_text()
    assert "$(BUILD_DIR)/$(ASSETS_DIR)/data.o: $(ASSETS_DIR)/data.bin $(GAME_BLOB)" in makefile
    assert "GAME_BLOB      := build/blob/game_code.deflate" in makefile
    assert "\n$(GAME_BLOB):" not in makefile        # no rule silently makes it
    slot = int(makefile.split("GAME_BLOB_SLOT :=")[1].split()[0], 16)
    assert slot == blob_rom.ROM_OFFSET - 0x10000   # data segment starts at 0x10000


# --- the vendored compressor ---------------------------------------------------

@pytest.mark.skipif(shutil.which("gcc") is None, reason="gcc absent")
def test_vendored_zlib_is_the_deflate_half_only():
    names = {p.name for p in blob_rom.ZLIB_DIR.iterdir()}
    assert {"deflate.c", "trees.c", "zutil.c", "adler32.c"} <= names
    assert not any(n.startswith("inf") for n in names)   # no inflate surface
    assert "PROVENANCE.md" in names


@pytest.mark.skipif(shutil.which("gcc") is None or not blob_rom.BASEROM.is_file()
                    or not (REPO / "build" / "game_code.bin").is_file(),
                    reason="needs gcc, the ROM and the extracted image")
def test_deflate104_reproduces_the_cartridge_stream(tmp_path):
    cli = blob_rom.build_compressor()
    out = tmp_path / "blob"
    subprocess.run([str(cli), str(REPO / "build" / "game_code.bin"), str(out)],
                   check=True, capture_output=True)
    assert out.read_bytes() == blob_rom.original_stream()


def test_cartridge_coverage_keeps_the_two_populations_separate(monkeypatch, capsys):
    """Static and game code are different populations; summing denominators
    would hide how thin each still is."""
    from tools.conveyor.pipeline import blob_splice, layout

    monkeypatch.setattr(layout, "coverage", lambda: {
        "promoted_functions": 23, "static_functions": 230,
        "promoted_bytes": 1448, "static_bytes": 61440})
    monkeypatch.setattr(blob_splice, "coverage", lambda: {
        "functions": 56, "total_functions": 912,
        "bytes": 4168, "image_size": 647072})

    cov = blob_rom.cartridge_coverage()
    blob_rom.print_coverage(cov)
    out = capsys.readouterr().out

    assert "23/230" in out and "56/912" in out
    assert "linked from C: 79 functions, 5616 bytes" in out
    assert "/1142" not in out                     # no merged denominator
