"""R16: runtime-image stream constants, inflate and exact recompression."""
import hashlib
import json
import shutil
import zlib
from pathlib import Path

import pytest

from tools.conveyor.pipeline import blob_rom, ovl_image
from tools.conveyor.pipeline.owned_data import DATA_ROM_START

REPO = Path(__file__).resolve().parents[2]
needs_gcc = pytest.mark.skipif(shutil.which("gcc") is None, reason="gcc absent")
needs_rom = pytest.mark.skipif(not ovl_image.BASEROM.is_file(), reason="needs the ROM")


def _raw(data, level=9):
    z = zlib.compressobj(level, zlib.DEFLATED, -15)
    return z.compress(data) + z.flush()


def _fake(monkeypatch, image, offset=0x100, trailing=b""):
    stream = _raw(image)
    rom = bytearray(0x100) + stream + trailing + bytes(16)
    monkeypatch.setitem(ovl_image.STREAMS, "a", ovl_image.Stream(
        "A", 0x8002B020, offset, len(stream) + len(trailing), len(image),
        hashlib.sha256(image).hexdigest(), "", "test"))
    return bytes(rom)


# --- constants (no ROM needed) -------------------------------------------------

@pytest.mark.parametrize("key", ["a", "b"])
def test_constants_agree_with_the_committed_targets(key):
    extents = json.loads((REPO / "asm" / "us" / f"ovl_{key}" / "extents.json").read_text())
    s = ovl_image.STREAMS[key]
    assert int(extents["rom_offset"], 16) == s.rom_offset
    assert int(extents["pointer"], 16) == s.pointer
    assert extents["size"] == s.image_size
    assert extents["image_sha256"] == s.image_sha256
    assert int(extents["base"], 16) == ovl_image.BASE


def test_the_three_streams_are_contiguous_and_slots_are_data_relative():
    assert blob_rom.ROM_OFFSET + blob_rom.LENGTH == ovl_image.STREAMS["a"].rom_offset
    assert ovl_image.end("a") == ovl_image.STREAMS["b"].rom_offset
    assert ovl_image.end("b") < ovl_image.ROM_SIZE
    assert ovl_image.slot("a") == 0xB5C534 - DATA_ROM_START == 0xB34164
    assert ovl_image.slot("b") == 0xB47AF4


def test_unknown_image_is_refused():
    with pytest.raises(ovl_image.OvlImageError, match="unknown"):
        ovl_image.stream("c")


# --- inflate gates (synthetic ROM) ---------------------------------------------

def test_inflate_returns_the_recorded_image(monkeypatch):
    image = bytes(range(256)) * 40
    assert ovl_image.inflate("a", _fake(monkeypatch, image)) == image


def test_inflate_refuses_a_stream_that_ends_before_its_slot(monkeypatch):
    rom = _fake(monkeypatch, b"x" * 1000, trailing=b"\x01\x02")
    with pytest.raises(ovl_image.OvlImageError, match="does not end"):
        ovl_image.inflate("a", rom)


def test_inflate_refuses_an_image_with_another_hash(monkeypatch):
    rom = _fake(monkeypatch, b"y" * 1000)
    s = ovl_image.STREAMS["a"]
    monkeypatch.setitem(ovl_image.STREAMS, "a", s._replace(image_sha256="0" * 64))
    with pytest.raises(ovl_image.OvlImageError, match="not the recorded"):
        ovl_image.inflate("a", rom)


# --- the compressor ------------------------------------------------------------

@needs_gcc
def test_deflate_round_trips_and_is_deterministic():
    data = b"".join(i.to_bytes(4, "big") for i in range(5000)) + b"\0" * 999
    once = ovl_image.deflate(data)
    assert once == ovl_image.deflate(data)
    assert zlib.decompress(once, -15) == data


# --- the proof against the ROM -------------------------------------------------

@needs_gcc
@needs_rom
def test_check_passes_against_the_rom():
    assert ovl_image.check() == []


@needs_gcc
@needs_rom
@pytest.mark.parametrize("key", ["a", "b"])
def test_retail_streams_recompress_byte_for_byte_and_a_changed_byte_does_not(key):
    rom = ovl_image.BASEROM.read_bytes()
    assert ovl_image.prove(key, rom)["matches"]
    image = bytearray(ovl_image.inflate(key, rom))
    image[len(image) // 3] ^= 0x01
    assert ovl_image.deflate(bytes(image)) != ovl_image.rom_stream(key, rom)


# --- the check command ---------------------------------------------------------

def test_check_without_the_rom_fails_loudly(monkeypatch, tmp_path, capsys):
    monkeypatch.setattr(ovl_image, "BASEROM", tmp_path / "absent.z64")
    assert ovl_image.main(["check"]) == 1
    assert "absent" in capsys.readouterr().out


@needs_gcc
@needs_rom
def test_check_cli_passes_and_catches_a_moved_pointer_and_a_corrupt_stream(tmp_path, capsys):
    assert ovl_image.main(["check"]) == 0
    assert "OK (both streams recompressed byte for byte)" in capsys.readouterr().out
    rom = bytearray(ovl_image.BASEROM.read_bytes())
    pointer = ovl_image.STREAMS["b"].pointer - ovl_image.ovl_targets.STATIC_DELTA
    rom[pointer + 3] ^= 0x04                                  # D_8002B024 moved
    rom[ovl_image.STREAMS["a"].rom_offset + 5000] ^= 0x10     # image A's stream damaged
    bad = tmp_path / "bad.z64"
    bad.write_bytes(bytes(rom))
    assert ovl_image.main(["check", "--rom", str(bad)]) == 1
    out = capsys.readouterr().out
    assert "D_8002B024 holds" in out and "image A: stream SHA-256 differs" in out
