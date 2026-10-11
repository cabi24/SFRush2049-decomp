"""R16: composing the runtime-image streams into the cartridge (ovl_rom produce/rom)."""
import hashlib
import shutil
import subprocess
import sys
from pathlib import Path

import pytest

from tools import compose_data
from tools.conveyor.pipeline import blob_rom, ovl_image, ovl_rom

REPO = Path(__file__).resolve().parents[2]
needs_rom = pytest.mark.skipif(shutil.which("gcc") is None or not ovl_image.BASEROM.is_file(),
                               reason="needs gcc and the ROM")


# --- compose_data --extra -------------------------------------------------------

def test_compose_all_replaces_every_slot_and_nothing_else():
    data = bytes(range(32))
    out = compose_data.compose_all(data, [(b"AB", 2, 2), (b"XYZ", 10, 3)])
    assert out == data[:2] + b"AB" + data[4:10] + b"XYZ" + data[13:]


def test_compose_all_refuses_overlapping_slots():
    with pytest.raises(ValueError, match="overlap"):
        compose_data.compose_all(b"x" * 32, [(b"ABCD", 2, 4), (b"EF", 5, 2)])


def test_adjacent_slots_are_allowed():
    """Game blob, image A and image B are back to back in the ROM."""
    out = compose_data.compose_all(b"." * 8, [(b"AB", 0, 2), (b"CD", 2, 2), (b"EF", 4, 2)])
    assert out == b"ABCDEF.."


def test_the_cli_composes_extras_and_refuses_a_short_one(tmp_path):
    (tmp_path / "data.bin").write_bytes(b"." * 20)
    (tmp_path / "blob").write_bytes(b"GG")
    (tmp_path / "a").write_bytes(b"AAA")
    cli = [sys.executable, str(REPO / "tools" / "compose_data.py"),
           str(tmp_path / "data.bin"), str(tmp_path / "blob"), str(tmp_path / "out"),
           "--slot", "0", "--length", "2"]
    subprocess.run(cli + ["--extra", f"{tmp_path / 'a'}:0x2:3"], check=True)
    assert (tmp_path / "out").read_bytes() == b"GGAAA" + b"." * 15
    (tmp_path / "out").unlink()
    proc = subprocess.run(cli + ["--extra", f"{tmp_path / 'a'}:2:4"],
                          capture_output=True, text=True)
    assert proc.returncode and "refusing" in proc.stderr and not (tmp_path / "out").exists()


# --- the Makefile ---------------------------------------------------------------

def _make_var(text, name):
    return text.split(f"\n{name}")[1].split(":=", 1)[1].split()[0]


@pytest.mark.parametrize("key", ["a", "b"])
def test_makefile_slots_agree_with_ovl_image(key):
    makefile = (REPO / "Makefile").read_text()
    up = key.upper()
    assert int(_make_var(makefile, f"OVL_{up}_SLOT"), 16) == ovl_image.slot(key)
    assert int(_make_var(makefile, f"OVL_{up}_LEN"), 0) == ovl_image.stream(key).length
    assert _make_var(makefile, f"OVL_{up}_STREAM") == \
        str(ovl_rom.stream_path(key).relative_to(REPO))


def test_makefile_always_composes_the_streams_and_never_makes_them():
    """Default on, like GAME_BLOB: no opt-out that would keep data.bin's bytes."""
    makefile = (REPO / "Makefile").read_text()
    assert "OVL_COMPOSE)" not in makefile and "ifeq" not in makefile.split(
        "OVL_A_STREAM :=")[1].split("data.o:")[0]
    rule = makefile.split("$(BUILD_DIR)/$(ASSETS_DIR)/data.o:")[1].split("\n\n")[0]
    assert "$(OVL_STREAMS)" in rule.splitlines()[0] and "$(OVL_COMPOSE_ARGS)" in rule
    assert "\n$(OVL_A_STREAM):" not in makefile and "\nbuild/ovl/" not in makefile


def test_the_builder_sync_list_is_shared_with_blob_rom():
    """The 2026-10-11 lesson: the compiler pins must reach the builder."""
    synced = {local for local, _ in blob_rom.SYNC_FILES}
    assert {"Makefile", "tools/compose_data.py", "rom_compilers.json",
            "src/rom/opt_overrides.mk"} <= synced


def test_builder_rom_build_passes_compose_flags_and_needs_the_positive_line(monkeypatch):
    seen = []

    class Proc:
        stdout = "ROM matches!\nMAKE=0 TEST=0"

    monkeypatch.setattr(blob_rom, "_ssh", lambda cmd, timeout=3600: seen.append(cmd) or Proc)
    ok, _ = blob_rom.builder_rom_build("~/scratch/r", "-j2 OVL_COMPOSE=1", "~/scratch/r/log")
    assert ok and "cd ~/scratch/r " in seen[0]
    assert "make COMPILER=ido -j2 OVL_COMPOSE=1 " in seen[0]
    assert "make test -j2 OVL_COMPOSE=1 " in seen[0]
    Proc.stdout = "ROM does NOT match\nMAKE=2 TEST=2"
    assert not blob_rom.builder_rom_build("~/scratch/r", "-j2", "/x")[0]


# --- produce / rom gates (synthetic) ---------------------------------------------

def _synthetic(monkeypatch, stream_for):
    monkeypatch.setattr(ovl_image, "inflate", lambda key, rom=None: b"image-" + key.encode())
    monkeypatch.setattr(ovl_image, "deflate", stream_for)
    monkeypatch.setattr(ovl_image, "rom_stream",
                        lambda key, rom=None: b"R" * ovl_image.stream(key).length)


def test_produce_refuses_a_stream_that_does_not_fill_its_slot(monkeypatch, tmp_path):
    _synthetic(monkeypatch, lambda data: b"short")
    with pytest.raises(ovl_rom.OvlRomError, match="holds exactly"):
        ovl_rom.produce(out=tmp_path, stand_in=True)


def test_produce_labels_the_stand_in_and_reports_rom_identity(monkeypatch, tmp_path):
    rebuilt = tmp_path / "a.bin"
    rebuilt.write_bytes(b"rebuilt A")
    _synthetic(monkeypatch, lambda data: (b"R" if data == b"image-b" else b"Q")
               * ovl_image.stream("a" if data != b"image-b" else "b").length)
    r = ovl_rom.produce({"a": rebuilt}, out=tmp_path, stand_in=True)
    assert r["a"]["source"] == str(rebuilt) and not r["a"]["stream_matches_rom"]
    assert r["b"]["source"].endswith("(stand-in)") and r["b"]["stream_matches_rom"]
    assert (tmp_path / "ovl_b.deflate").stat().st_size == ovl_image.stream("b").length


def test_rom_refuses_to_spend_a_build_when_a_stream_differs(monkeypatch, tmp_path):
    blob = tmp_path / "game_code.deflate"
    blob.write_bytes(b"blob")
    monkeypatch.setattr(blob_rom, "BLOB_OUT", blob)
    monkeypatch.setattr(ovl_rom, "produce", lambda files=None, stand_in=False: {
        "a": {"image": "A", "stream_matches_rom": False},
        "b": {"image": "B", "stream_matches_rom": True}})
    monkeypatch.setattr(ovl_rom, "sync_streams",
                        lambda *a, **k: pytest.fail("must not sync"))
    with pytest.raises(ovl_rom.OvlRomError, match="pre-flight"):
        ovl_rom.rom()


# --- the rebuilt image is the default source ------------------------------------

def _rebuilt(monkeypatch, tmp_path, lock=None):
    from tools.conveyor.pipeline import ovl_layout, ovl_splice
    lock = lock or {"a": {"func_1": {"x": 1}}, "b": {}}
    monkeypatch.setattr(ovl_image, "rebuilt_path",
                        lambda image, build=None: tmp_path / image / "rebuilt.bin")
    monkeypatch.setattr(ovl_image, "rebuilt_record",
                        lambda image, build=None: tmp_path / image / "rebuilt.json")
    monkeypatch.setattr(ovl_splice, "load_lock", lambda path=None: lock)
    monkeypatch.setattr(ovl_splice, "check", lambda lockfile=None: [])
    for key in ("a", "b"):
        built = tmp_path / key / "link.bin"
        built.parent.mkdir(parents=True, exist_ok=True)
        built.write_bytes(b"image-" + key.encode())
        ovl_layout.publish_rebuilt(key, built, hashlib.sha256(built.read_bytes()).hexdigest(),
                                   lock[key], {})
    return lock


def test_produce_compresses_the_published_rebuilt_images(monkeypatch, tmp_path):
    _rebuilt(monkeypatch, tmp_path)
    seen = []
    monkeypatch.setattr(ovl_image, "inflate", lambda key, rom=None: pytest.fail("no stand-in"))
    monkeypatch.setattr(ovl_image, "deflate", lambda data: seen.append(data) or
                        b"R" * ovl_image.stream("a" if data == b"image-a" else "b").length)
    monkeypatch.setattr(ovl_image, "rom_stream",
                        lambda key, rom=None: b"R" * ovl_image.stream(key).length)
    r = ovl_rom.produce(out=tmp_path / "out")
    assert seen == [b"image-a", b"image-b"]
    assert r["a"]["source"] == "rebuilt image (1 locked functions)"
    assert all(p["stream_matches_rom"] for p in r.values())


def test_produce_refuses_a_missing_rebuilt_image_and_writes_nothing(monkeypatch, tmp_path):
    _rebuilt(monkeypatch, tmp_path)
    (tmp_path / "b" / "rebuilt.bin").unlink()
    monkeypatch.setattr(ovl_image, "deflate", lambda data: pytest.fail("must not compress"))
    with pytest.raises(ovl_rom.OvlRomError, match="no rebuilt image"):
        ovl_rom.produce(out=tmp_path / "out")
    assert not (tmp_path / "out").exists()


def test_produce_refuses_a_rebuilt_image_from_another_lock_or_altered(monkeypatch, tmp_path):
    lock = _rebuilt(monkeypatch, tmp_path)
    lock["a"]["func_2"] = {"y": 2}                      # spliced since the build
    with pytest.raises(ovl_rom.OvlRomError, match="another lock"):
        ovl_rom.produce(out=tmp_path / "out")
    del lock["a"]["func_2"]
    (tmp_path / "a" / "rebuilt.bin").write_bytes(b"image-A")
    with pytest.raises(ovl_rom.OvlRomError, match="not the image ovl_layout build recorded"):
        ovl_rom.produce(out=tmp_path / "out")


def test_produce_refuses_drifted_locked_sources(monkeypatch, tmp_path):
    from tools.conveyor.pipeline import ovl_splice
    _rebuilt(monkeypatch, tmp_path)
    monkeypatch.setattr(ovl_splice, "check", lambda lockfile=None: [("a:f", "source hash drifted")])
    with pytest.raises(ovl_rom.OvlRomError, match="drifted"):
        ovl_rom.produce(out=tmp_path / "out")


def test_rom_needs_the_game_blob(monkeypatch, tmp_path):
    monkeypatch.setattr(blob_rom, "BLOB_OUT", tmp_path / "absent.deflate")
    monkeypatch.setattr(ovl_rom, "produce", lambda *a, **k: pytest.fail("must not produce"))
    with pytest.raises(ovl_rom.OvlRomError, match="blob_rom produce"):
        ovl_rom.rom()


def test_blob_rom_sync_and_rom_refuse_absent_streams(monkeypatch, tmp_path):
    blob = tmp_path / "game_code.deflate"
    blob.write_bytes(b"blob")
    streams = {"a": tmp_path / "ovl_a.deflate", "b": tmp_path / "ovl_b.deflate"}
    streams["a"].write_bytes(b"A")
    monkeypatch.setattr(blob_rom, "OVL_STREAMS", streams)
    monkeypatch.setattr(blob_rom.subprocess, "run", lambda *a, **k: pytest.fail("no rsync"))
    monkeypatch.setattr(blob_rom, "_ssh", lambda *a, **k: pytest.fail("no ssh"))
    with pytest.raises(blob_rom.BlobRomError, match="ovl_b.deflate"):
        blob_rom.sync_to_builder(blob)
    monkeypatch.setattr(blob_rom, "produce", lambda *a, **k: pytest.fail("must not produce"))
    with pytest.raises(blob_rom.BlobRomError, match="ovl_b.deflate"):
        blob_rom.rom()


def test_unknown_image_files_are_refused():
    with pytest.raises(ovl_rom.OvlRomError, match="unknown"):
        ovl_rom.produce({"c": "x"})


# --- against the ROM -----------------------------------------------------------

@needs_rom
def test_identity_case_produces_the_rom_streams(tmp_path):
    r = ovl_rom.produce(out=tmp_path, stand_in=True)
    for key in ("a", "b"):
        assert r[key]["stream_matches_rom"] and r[key]["image_is_retail"]
        assert (tmp_path / f"ovl_{key}.deflate").read_bytes() == ovl_image.rom_stream(key)


@needs_rom
def test_the_drill_change_keeps_the_length_and_breaks_the_stream():
    image = ovl_image.inflate("b")
    offset, stream = ovl_rom.length_preserving_change("b", image)
    assert len(stream) == ovl_image.stream("b").length
    assert stream != ovl_image.rom_stream("b")
    assert 0 <= offset < len(image)
