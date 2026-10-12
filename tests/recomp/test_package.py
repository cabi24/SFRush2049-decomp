"""Phase 4 (w24r): gameplay-test package (tools/recomp/package.py), the
standalone synthetic-ROM builder (tools/recomp/make_rom.py) and the portable
call-interception list. Stub trees only: no ROM, no compiler."""
import hashlib
import importlib.util
import json
import re
import sys
import tarfile
import zlib
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]


def _load(name, rel):
    spec = importlib.util.spec_from_file_location(name, REPO / rel)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


pk = _load("recomp_package", "tools/recomp/package.py")
mr = _load("recomp_make_rom", "tools/recomp/make_rom.py")


def _stub(tmp_path):
    n64 = tmp_path / "N64Recomp"
    (n64 / "src").mkdir(parents=True)
    (n64 / "src" / "main.cpp").write_text("int main(){}\n")
    (n64 / ".git").mkdir()
    (n64 / ".git" / "HEAD").write_text("ref: x\n")
    rt = tmp_path / "N64ModernRuntime"
    (rt / "N64Recomp" / "include").mkdir(parents=True)
    (rt / "N64Recomp" / "include" / "recomp.h").write_text("/* h */\n")
    (rt / "N64Recomp" / ".git").write_text("gitdir: ../.git/modules/N64Recomp\n")   # submodule link file
    cfg = tmp_path / "stock"
    cfg.mkdir()
    for f in pk.CONFIG_FILES:
        (cfg / f).write_text("x\n")
    (cfg / "manifest.json").write_text(json.dumps({
        "rom_sha1": pk.US_SHA1, "replacements": {"image": None, "replaced": []},
        "synthetic_rom": {"sha256": "ab" * 32, "size": 123}}))
    (cfg / "rom").mkdir()
    (cfg / "rom" / "rush2049.recomp.z64").write_bytes(b"ROMBYTES")      # must never be packaged
    binary = tmp_path / "rush2049_recomp"
    binary.write_bytes(b"\x7fELF fake")
    binary.chmod(0o755)
    pins = {"package_version": "7", "n64recomp": "1" * 40, "n64modernruntime": "2" * 40,
            "rt64_url": "https://github.com/rt64/rt64", "rt64": "3" * 40}
    return n64, rt, cfg, binary, pins


def _members(path):
    with tarfile.open(path, "r:gz") as tar:
        return {m.name: (tar.extractfile(m).read() if m.isfile() else None, m) for m in tar.getmembers()}


def test_package_from_stub_tree_has_a_valid_manifest(tmp_path):
    n64, rt, cfg, binary, pins = _stub(tmp_path)
    res = pk.build(n64, rt, cfg, binary, tmp_path / "dist", pins, check_pins=False, repo_commit="c0ffee")
    assert Path(res["path"]).name == "rush2049-recomp-test-v7.tar.gz" and res["version"] == 7
    assert pk.verify(res["path"]) == res["files"] - 1
    m = _members(res["path"])
    root = "rush2049-recomp-test-v7/"
    names = {k[len(root):] for k, (d, _i) in m.items() if d is not None}
    for doc in pk.DOCS + ("VERSION.txt", "MANIFEST.sha256"):
        assert doc in names
    assert "src/N64Recomp/src/main.cpp" in names and "src/N64ModernRuntime/N64Recomp/include/recomp.h" in names
    assert not any("/.git" in n or n.endswith(".git") for n in names)
    assert "prebuilt/linux-x86_64/rush2049_recomp" in names
    assert m[root + "prebuilt/linux-x86_64/rush2049_recomp"][1].mode == 0o755
    assert {"config/" + f for f in pk.CONFIG_FILES} <= names
    assert not any(n.startswith("rom/") or n.endswith(".z64") for n in names)       # no ROM bytes
    assert "rush/tools/recomp/make_rom.py" in names and "rush/tools/recomp/runtime/CMakeLists.txt" in names
    assert "rush/tools/recomp/package.py" not in names
    assert not any(n.startswith("rush/tools/recomp/package/") for n in names)
    for doc in pk.DOCS:
        text = m[root + doc][0].decode()
        assert "@@" not in text, doc
    install = m[root + "INSTALL.md"][0].decode()
    assert "3" * 40 in install and "https://github.com/rt64/rt64" in install and "rush2049-recomp-test-v7" in install
    assert b"c0ffee" in m[root + "VERSION.txt"][0]


def test_package_is_deterministic(tmp_path):
    n64, rt, cfg, binary, pins = _stub(tmp_path)
    a = pk.build(n64, rt, cfg, binary, tmp_path / "d1", pins, check_pins=False, repo_commit="x")
    b = pk.build(n64, rt, cfg, binary, tmp_path / "d2", pins, check_pins=False, repo_commit="x")
    assert a["sha256"] == b["sha256"]


def test_with_rom_is_explicit_and_checked(tmp_path, monkeypatch):
    n64, rt, cfg, binary, pins = _stub(tmp_path)
    rom = tmp_path / "baserom.us.z64"
    rom.write_bytes(b"not the rom")
    with pytest.raises(pk.PackageError, match="not the US retail ROM"):
        pk.build(n64, rt, cfg, binary, tmp_path / "d", pins, with_rom=rom, check_pins=False)
    monkeypatch.setattr(pk, "US_SHA1", hashlib.sha1(b"not the rom").hexdigest())
    res = pk.build(n64, rt, cfg, binary, tmp_path / "d", pins, with_rom=rom, check_pins=False)
    assert res["path"].endswith("-with-rom.tar.gz")
    assert "rush2049-recomp-test-v7/rom/baserom.us.z64" in _members(res["path"])


def test_tampered_archive_fails_verify(tmp_path):
    n64, rt, cfg, binary, pins = _stub(tmp_path)
    res = pk.build(n64, rt, cfg, binary, tmp_path / "d", pins, check_pins=False)
    m = _members(res["path"])
    bad = tmp_path / "bad.tar.gz"
    with tarfile.open(bad, "w:gz") as tar:
        import io
        for name, (data, info) in sorted(m.items()):
            if data is not None and name.endswith("INSTALL.md"):
                data = data + b"tampered"
                info.size = len(data)
            tar.addfile(info, io.BytesIO(data) if data is not None else None)
    with pytest.raises(pk.PackageError, match="INSTALL.md"):
        pk.verify(bad)


def test_refuses_replacement_configs_and_unknown_template_fields(tmp_path):
    n64, rt, cfg, binary, pins = _stub(tmp_path)
    doc = json.loads((cfg / "manifest.json").read_text())
    doc["replacements"]["replaced"] = [{"id": "x"}]
    (cfg / "manifest.json").write_text(json.dumps(doc))
    with pytest.raises(pk.PackageError, match="stock"):
        pk.build(n64, rt, cfg, binary, tmp_path / "d", pins, check_pins=False)
    with pytest.raises(pk.PackageError, match="NOPE"):
        pk.render("a @@NOPE@@ b", {})


def test_committed_versions_pins_and_templates():
    pins = pk.read_pins()
    assert int(pins["package_version"]) >= 1
    for key in ("n64recomp", "n64modernruntime", "rt64"):
        assert re.match(r"^[0-9a-f]{40}$", pins[key]), key
    for doc in pk.DOCS:
        assert (pk.TEMPLATES / doc).is_file()
    changes = (pk.TEMPLATES / "CHANGES.md").read_text()
    assert "## v%s " % pins["package_version"] in changes, "CHANGES.md needs a section for the package version"


# ------------------------------------------------------------------ make_rom

def _fake_rom():
    game, a, b = bytes(range(256)) * 40, b"A" * 3000, b"B" * 1000
    rom = bytearray(b"\x80\x37\x12\x40" + b"\0" * 0x2000)
    offs = {}
    for name, data in (("game", game), ("ovl_a", a), ("ovl_b", b)):
        c = zlib.compressobj(9, zlib.DEFLATED, -15)
        offs[name] = len(rom)
        rom += c.compress(data) + c.flush()
    rom = bytes(rom)
    secs = [{"name": ".static", "rom": "0x00001000", "size": 0x100,
             "image_sha256": hashlib.sha256(rom[0x1000:0x1100]).hexdigest()}]
    imgs, out, at = [], bytearray(rom), 0x10000
    for name, data in (("game", game), ("ovl_a", a), ("ovl_b", b)):
        secs.append({"name": "." + name, "rom": "0x%08X" % at, "size": len(data),
                     "image_sha256": hashlib.sha256(data).hexdigest()})
        imgs.append({"name": name, "stream_rom": "0x%08X" % offs[name], "section_rom": "0x%08X" % at,
                     "size": len(data)})
        out.extend(b"\0" * (at + len(data) - len(out)))
        out[at:at + len(data)] = data
        at += 0x10000
    manifest = {"rom_sha1": hashlib.sha1(rom).hexdigest(), "sections": secs,
                "runtime_contract": {"images": imgs}, "replacements": {"image": None},
                "synthetic_rom": {"size": len(out), "sha256": hashlib.sha256(bytes(out)).hexdigest()}}
    return rom, manifest, bytes(out)


def test_make_rom_rebuilds_the_synthetic_rom(tmp_path):
    rom, manifest, want = _fake_rom()
    assert mr.build(rom, manifest) == want
    (tmp_path / "r.z64").write_bytes(rom)
    (tmp_path / "m.json").write_text(json.dumps(manifest))
    assert mr.main(["--rom", str(tmp_path / "r.z64"), "--manifest", str(tmp_path / "m.json"),
                    "--out", str(tmp_path / "o" / "s.z64")]) == 0
    assert (tmp_path / "o" / "s.z64").read_bytes() == want


def test_make_rom_refuses_wrong_inputs():
    rom, manifest, _ = _fake_rom()
    with pytest.raises(mr.RomError, match="SHA-1"):
        mr.build(rom[:-1] + b"\x01", manifest)
    with pytest.raises(mr.RomError, match="z64"):
        mr.build(b"\x37\x80\x40\x12" + rom[4:], manifest)
    bad = json.loads(json.dumps(manifest))
    bad["synthetic_rom"]["sha256"] = "0" * 64
    with pytest.raises(mr.RomError, match="synthetic ROM"):
        mr.build(rom, bad)
    bad = json.loads(json.dumps(manifest))
    bad["sections"][1]["image_sha256"] = "0" * 64
    with pytest.raises(mr.RomError, match="game"):
        mr.build(rom, bad)


# ------------------------------------------------------------------ portable interception list

def test_wrapped_names_agree_between_cmake_and_header():
    cmake = (REPO / "tools/recomp/runtime/CMakeLists.txt").read_text()
    m = re.search(r"set\(RUSH_WRAPPED(.*?)\)", cmake, re.S)
    listed = set(m.group(1).split())
    header = (REPO / "tools/recomp/runtime/src/rush_wrap.h").read_text()
    mapped = set(re.findall(r"#define __real_(\w+) (\w+)", header))
    assert {a for a, b in mapped if a == b} == listed
    src = "".join(p.read_text() for p in (REPO / "tools/recomp/runtime/src").glob("*.cpp"))
    defined = set(re.findall(r"void __wrap_(\w+)\(", src))
    assert defined == listed
    assert "-Wl,--wrap" not in cmake
