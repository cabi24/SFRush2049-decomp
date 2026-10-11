"""Runtime-image splicing: gates, lock, check and coverage (ovl_splice).

IDO is replaced by a fake compiler that assembles the words a source lists,
so every gate after the compile (link at the image address, extent and
excess-word checks, retail comparison, the whole-image gate, lock writing)
runs for real with mips binutils."""
import hashlib
import json
import re
import shutil
import struct
import subprocess

import pytest

from tools.conveyor.pipeline import blob_layout, blob_splice, ovl_image, ovl_layout, ovl_splice

BASE = ovl_image.BASE
HAS_BINUTILS = all(shutil.which(t) for t in
                   ("mips-linux-gnu-as", "mips-linux-gnu-ld", "mips-linux-gnu-objcopy",
                    "mips-linux-gnu-readelf", "mips-linux-gnu-nm"))
JR_RA, NOP = 0x03E00008, 0x00000000
WORDS = [0x27BDFFE8, 0xAFBF0014, JR_RA, NOP,      # f0
         0x24020001, JR_RA, NOP, NOP,             # f1
         0x12345678, 0x9ABCDEF0]                  # data tail
FUNCTIONS = [("f0", 0, 4), ("f1", 4, 4)]


def _source(name, words, flags="-g0 -O2 -mips2 -G 0 -non_shared"):
    return (f"/* flags: {flags} */\n/* words: " + " ".join(f"{w:08X}" for w in words)
            + f" */\nvoid {name}(void) {{}}\n")


def fake_compile(image, sources, out_dir=None):
    """Assemble the listed words into .text.<name>, as compile_on_builder
    leaves an object (section renamed, provenance written)."""
    out_dir = out_dir or ovl_splice.obj_dir(image)
    out_dir.mkdir(parents=True, exist_ok=True)
    objects = {}
    for name, path in sources.items():
        text = path.read_text()
        words = re.search(r"/\* words: ([0-9A-F ]+) \*/", text).group(1).split()
        asm = out_dir / f"{name}.s"
        asm.write_text(f'.section .text.{name}, "ax", @progbits\n.globl {name}\n'
                       f'.type {name}, @function\n{name}:\n'
                       + "".join(f".word 0x{w}\n" for w in words))
        obj = out_dir / f"{name}.o"
        subprocess.run(["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-o", str(obj),
                        str(asm)], check=True, capture_output=True)
        blob_splice.write_provenance(obj, blob_splice.source_sha(text), ovl_splice.TOOLKIT)
        objects[name] = obj
    return objects, {}


@pytest.fixture
def repo(tmp_path, monkeypatch):
    """A synthetic runtime image A in a throwaway repo root."""
    data = b"".join(struct.pack(">I", w) for w in WORDS)
    (tmp_path / "build").mkdir()
    image = tmp_path / "build" / "image.bin"
    image.write_bytes(data)
    target = tmp_path / "asm" / "us" / "ovl_a"
    target.mkdir(parents=True)
    (target / "extents.json").write_text(json.dumps({
        "image": "A", "size": len(data), "base": f"0x{BASE:08X}", "rom_offset": "0xB5C534",
        "image_sha256": hashlib.sha256(data).hexdigest(), "text_end": f"0x{BASE + 32:08X}",
        "functions": [{"name": n, "address": f"0x{BASE + 4 * s:08X}", "size": 4 * c,
                       "evidence": ["test"]} for n, s, c in FUNCTIONS]}))
    monkeypatch.setattr(ovl_splice, "REPO", tmp_path)
    monkeypatch.setattr(ovl_layout, "REPO", tmp_path)
    monkeypatch.setattr(ovl_splice, "symbols", lambda image: {})
    monkeypatch.setattr(ovl_splice, "src_dir", lambda image: tmp_path / "src" / f"ovl_{image}")
    monkeypatch.setattr(ovl_layout, "work_dir", lambda image, build=None: tmp_path / "w" / image)
    monkeypatch.setattr(ovl_layout, "asm_dir", lambda image, build=None: tmp_path / "w" / image / "asm")
    monkeypatch.setattr(ovl_layout, "script_path", lambda image, build=None: tmp_path / "w" / image / "x.ld")
    monkeypatch.setattr(ovl_layout, "load_extents",
                        lambda image, target_dir=None: json.loads(
                            (tmp_path / "asm" / "us" / f"ovl_{image}" / "extents.json").read_text())
                        if image == "a" else {"functions": [], "size": 4, "rom_offset": "0x0"})
    real_derive = ovl_layout.derive
    monkeypatch.setattr(ovl_layout, "derive", lambda name, *a, **k: real_derive(
        name, image, target, output=tmp_path / "layout.json"))
    return tmp_path


def _write(repo, name, words):
    src = repo / "incoming" / f"{name}.c"
    src.parent.mkdir(exist_ok=True)
    src.write_text(_source(name, words))
    return src


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips binutils absent")
def test_a_retail_body_is_spliced_locked_and_checked(repo):
    lockfile = repo / "ovl_matched.lock.json"
    r = ovl_splice.splice("a", {"f1": _write(repo, "f1", WORDS[4:8])}, lockfile,
                          compile_fn=fake_compile)
    assert r["spliced"] == ["f1"] and not r["refused"] and r["image_ok"], r
    lock = json.loads(lockfile.read_text())
    assert set(lock) == {"a", "b"} and set(lock["a"]) == {"f1"} and lock["b"] == {}
    entry = lock["a"]["f1"]
    assert entry["source"] == "src/ovl_a/f1.c" and entry["verified"] == "image_gate"
    assert entry["image"] == "A" and entry["flagset"] == "-g0 -O2 -mips2 -G 0 -non_shared"
    assert ovl_splice.check(lockfile) == []
    bodies = ovl_splice.spliced_bodies("a", lock["a"], require_provenance=True)
    assert bodies == {"f1": struct.pack(">4I", *WORDS[4:8])}
    assert ovl_splice.coverage(lockfile)["a"]["bytes"] == 16
    (repo / "src" / "ovl_a" / "f1.c").write_text("/* edited */\n")
    assert ovl_splice.check(lockfile) == [("a:f1", "source hash drifted")]


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips binutils absent")
def test_wrong_short_or_padded_bodies_are_refused_and_nothing_changes(repo):
    lockfile = repo / "ovl_matched.lock.json"
    files = {"f0": _write(repo, "f0", [0x27BDFFE8, 0xAFBF0018, JR_RA, NOP]),
             "f1": _write(repo, "f1", WORDS[4:8] + [0x1]),
             "zz": _write(repo, "zz", [JR_RA, NOP])}
    r = ovl_splice.splice("a", files, lockfile, compile_fn=fake_compile)
    assert r["spliced"] == [] and r["image_ok"]
    assert "differs from the retail extent at +0x4" in r["refused"]["f0"]
    assert "extra words" in r["refused"]["f1"]
    assert "not a function of image A" in r["refused"]["zz"]
    assert ovl_splice.load_lock(lockfile) == {"a": {}, "b": {}}
    assert not (repo / "src" / "ovl_a").exists()
    short = {"f0": _write(repo, "f0", [0x27BDFFE8, JR_RA])}
    r = ovl_splice.splice("a", short, lockfile, compile_fn=fake_compile)
    assert "shorter than" in r["refused"]["f0"]


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips binutils absent")
def test_a_refused_respliced_body_keeps_the_locked_object(repo):
    lockfile = repo / "ovl_matched.lock.json"
    ovl_splice.splice("a", {"f0": _write(repo, "f0", WORDS[0:4])}, lockfile,
                      compile_fn=fake_compile)
    obj = ovl_splice.obj_dir("a") / "f0.o"
    before = obj.read_bytes()
    r = ovl_splice.splice("a", {"f0": _write(repo, "f0", [0x27BDFFE8, 0xAFBF0018, JR_RA, NOP])},
                          lockfile, compile_fn=fake_compile)
    assert "f0" in r["refused"] and obj.read_bytes() == before
    lock = json.loads(lockfile.read_text())["a"]
    assert ovl_splice.spliced_bodies("a", lock, require_provenance=True)["f0"] == \
        struct.pack(">4I", *WORDS[0:4])


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips binutils absent")
def test_a_locked_body_without_its_object_never_falls_back_to_passthrough(repo):
    lockfile = repo / "ovl_matched.lock.json"
    ovl_splice.splice("a", {"f0": _write(repo, "f0", WORDS[0:4])}, lockfile,
                      compile_fn=fake_compile)
    (ovl_splice.obj_dir("a") / "f0.o").unlink()
    with pytest.raises(blob_splice.LockedBodyError, match="object missing"):
        ovl_splice.spliced_bodies("a", json.loads(lockfile.read_text())["a"])
    r = ovl_splice.rebuild("a", lockfile, compile_fn=fake_compile)
    assert r["image_ok"] and r["bodies"] == 1


def test_check_fails_closed_on_a_malformed_lock(repo):
    lockfile = repo / "ovl_matched.lock.json"
    lockfile.write_text(json.dumps({"f0": {"source": "x"}}))
    assert ovl_splice.check(lockfile)[0][0] == "ovl_matched.lock.json"
    lockfile.write_text("{")
    assert ovl_splice.check(lockfile)[0][0] == "ovl_matched.lock.json"
    lockfile.write_text(json.dumps({"a": {"nope": {"source": "src/x.c"}}, "b": {}}))
    assert ovl_splice.check(lockfile) == [("a:nope", "not a function of image A")]
    lockfile.unlink()
    assert ovl_splice.check(lockfile) == []


def test_packet_claims_become_members_and_the_rest_context(tmp_path):
    (tmp_path / "group.json").write_text(json.dumps({
        "members": ["a1", "a2", "a3"], "claims": ["a2"], "context": ["c0"],
        "files": ["group.c"], "keep": ["a1"], "flags": "-O3", "external_sources": ["x.c"]}))
    spec = ovl_splice.packet_spec(tmp_path)
    assert spec["members"] == ["a2"] and spec["context"] == ["c0", "a1", "a3"]
    assert "claims" not in spec and "external_sources" not in spec
    (tmp_path / "group.json").write_text(json.dumps({
        "members": ["a1"], "claims": [], "files": [], "keep": [], "flags": "-O3"}))
    with pytest.raises(ovl_splice.SpliceError, match="research only"):
        ovl_splice.packet_spec(tmp_path)


def test_flags_and_image_selection():
    assert ovl_splice.flags_of("/* flags: -g0 -O3 -mips2 */\nint x;") == "-g0 -O3 -mips2"
    assert ovl_splice.flags_of("int x;") == blob_layout.DEFAULT_FLAGSET

    class Args:
        targets, image = ["asm/us/ovl_b"], None
    assert ovl_splice._images(Args, ["cloud/matches/ovl_a/func_8038F454.c"]) == ["a", "b"]


def test_committed_lock_is_consistent():
    """The checked-in lock passes check (it is what make check-matched runs)."""
    assert ovl_splice.check() == []
