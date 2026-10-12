"""Pinned compilers (IDO 5.3 / 7.1): identity, install, toolkit swap, lock binding."""
import argparse
import hashlib
import io
import json
import os
import tarfile
from pathlib import Path

import pytest

from tools.conveyor import compilers
from tools.conveyor.bundles import build_toolkit
from tools.conveyor.pipeline import lock as lockmod

REPO = Path(__file__).resolve().parents[2]


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def _fake_table():
    """Two fake compilers whose fingerprinted files are the bytes b'<name>:<file>'."""
    table = {}
    for name in ("ido5.3", "ido7.1"):
        build = {f: _sha(f"{name}:{f}".encode()) for f in compilers.FINGERPRINT_FILES}
        table[name] = dict(compilers.COMPILERS[name], builds={"fake": build})
    return table


def _fake_ido(path, name, skip=()):
    path.mkdir(parents=True, exist_ok=True)
    for f in compilers.FINGERPRINT_FILES:
        if f not in skip:
            (path / f).write_bytes(f"{name}:{f}".encode())
    (path / "err.english.cc").write_bytes(b"messages")
    return path


# --- identity -------------------------------------------------------------------

def test_identify_dir_names_each_compiler(tmp_path):
    table = _fake_table()
    assert compilers.identify_dir(_fake_ido(tmp_path / "a", "ido5.3"), table) == "ido5.3"
    assert compilers.identify_dir(_fake_ido(tmp_path / "b", "ido7.1"), table) == "ido7.1"


def test_identify_refuses_partial_or_mixed_builds(tmp_path):
    table = _fake_table()
    assert compilers.identify_dir(_fake_ido(tmp_path / "a", "ido5.3", skip=("uopt",)), table) is None
    mixed = _fake_ido(tmp_path / "m", "ido5.3")
    (mixed / "ugen").write_bytes(b"ido7.1:ugen")
    assert compilers.identify_dir(mixed, table) is None


def test_identify_toolkit_reads_ido_members(tmp_path):
    ido = _fake_ido(tmp_path / "ido", "ido7.1")
    tk = tmp_path / "tk.tar.gz"
    with tarfile.open(tk, "w:gz") as tar:
        for f in ido.iterdir():
            tar.add(f, arcname=f"ido/{f.name}")
    assert compilers.identify_toolkit(tk, _fake_table()) == "ido7.1"
    assert compilers.identify(tk, _fake_table()) == "ido7.1"


def test_known_builds_are_complete_and_distinct():
    seen = {}
    for name, spec in compilers.COMPILERS.items():
        if "wraps" in spec:  # driver wrapper: no archive, no builds of its own
            assert spec["builds"] == {} and spec["archive_sha256"] is None
            assert spec["wraps"] in compilers.COMPILERS and "wraps" not in compilers.COMPILERS[spec["wraps"]]
            continue
        assert len(spec["archive_sha256"]) == 64
        for build in spec["builds"].values():
            assert set(build) == set(compilers.FINGERPRINT_FILES)
            key = tuple(build[f] for f in compilers.FINGERPRINT_FILES)
            assert key not in seen
            seen[key] = name
    assert compilers.COMPILERS[compilers.DEFAULT]["make_cc"] is None
    assert compilers.COMPILERS["ido7.1"]["make_cc"] == "$(IDO71_CC)"


# --- segment pins ---------------------------------------------------------------

def test_segment_pins_normalize_and_refuse_unknown(tmp_path):
    pins = tmp_path / "rc.json"
    assert compilers.load_segment_pins(pins) == {}
    pins.write_text(json.dumps({"default": "ido5.3", "segments": {"0x0011640": "ido7.1"}}))
    loaded = compilers.load_segment_pins(pins)
    assert loaded == {"0x11640": "ido7.1"}
    assert compilers.segment_compiler("0x11640", loaded) == "ido7.1"
    assert compilers.segment_compiler("0x10d60", loaded) == "ido5.3"
    pins.write_text(json.dumps({"segments": {"0x11640": "ido6.0"}}))
    with pytest.raises(SystemExit, match="unknown compiler"):
        compilers.load_segment_pins(pins)
    pins.write_text(json.dumps({"default": "ido7.1", "segments": {}}))
    with pytest.raises(SystemExit, match="default must be"):
        compilers.load_segment_pins(pins)


def test_lock_compiler_defaults_to_53():
    assert compilers.lock_compiler({}) == "ido5.3"
    assert compilers.lock_compiler({"compiler": "ido7.1"}) == "ido7.1"
    with pytest.raises(SystemExit):
        compilers.lock_compiler({"compiler": "gcc"})


# --- install --------------------------------------------------------------------

def _archive(tmp_path, name, members):
    path = tmp_path / f"{name}.tgz"
    with tarfile.open(path, "w:gz") as tar:
        for member, data in members.items():
            info = tarfile.TarInfo(member)
            info.size, info.mode = len(data), 0o755
            tar.addfile(info, io.BytesIO(data))
    return path


def _patch_install_table(monkeypatch, archive):
    table = _fake_table()
    table["ido7.1"]["archive_sha256"] = _sha(archive.read_bytes())
    monkeypatch.setattr(compilers, "COMPILERS", table)
    real = compilers.identify_dir
    monkeypatch.setattr(compilers, "identify_dir", lambda p, c=None: real(p, table))


def test_install_verifies_extracts_and_identifies(tmp_path, monkeypatch):
    members = {f: f"ido7.1:{f}".encode() for f in compilers.FINGERPRINT_FILES}
    archive = _archive(tmp_path, "good", dict(members, **{"err.english.cc": b"m"}))
    _patch_install_table(monkeypatch, archive)
    dest = compilers.install("ido7.1", tmp_path / "out", archive)
    assert (dest / "cfe").read_bytes() == b"ido7.1:cfe"
    assert (dest / "cc").stat().st_mode & 0o111
    assert compilers.install("ido7.1", dest, archive) == dest  # idempotent


def test_install_refuses_wrong_archive_before_extracting(tmp_path, monkeypatch):
    members = {f: f"ido7.1:{f}".encode() for f in compilers.FINGERPRINT_FILES}
    good = _archive(tmp_path, "good", members)
    _patch_install_table(monkeypatch, good)
    bad = _archive(tmp_path, "bad", dict(members, cc=b"evil"))
    with pytest.raises(SystemExit, match="archive sha256"):
        compilers.install("ido7.1", tmp_path / "out", bad)
    assert not (tmp_path / "out").exists()


def test_install_refuses_unsafe_members_and_wrong_contents(tmp_path, monkeypatch):
    unsafe = _archive(tmp_path, "unsafe", {"../cc": b"x"})
    _patch_install_table(monkeypatch, unsafe)
    with pytest.raises(SystemExit, match="refusing archive member"):
        compilers.install("ido7.1", tmp_path / "out", unsafe)
    wrong = _archive(tmp_path, "wrong", {f: f"ido5.3:{f}".encode()
                                         for f in compilers.FINGERPRINT_FILES})
    _patch_install_table(monkeypatch, wrong)
    with pytest.raises(SystemExit, match="do not match a known ido7.1"):
        compilers.install("ido7.1", tmp_path / "out", wrong)


# --- toolkit swap ---------------------------------------------------------------

def _members(path):
    with tarfile.open(path) as tar:
        return {m.name: (tar.extractfile(m).read(), m.mode) for m in tar.getmembers()}


def test_swap_ido_keeps_everything_else_and_is_deterministic(tmp_path):
    base = build_toolkit._write_bundle([
        ("ido/cc", b"ido5.3:cc", 0o755), ("ido/uopt", b"ido5.3:uopt", 0o755),
        ("jobs/runner.py", b"print(1)\n", 0o644), ("bin/objdump", b"\x7fELF", 0o755),
        ("shim/conveyor_shim.h", b"/* shim */\n", 0o644)], tmp_path / "base.tar.gz")
    ido71 = _fake_ido(tmp_path / "ido71", "ido7.1")
    a = build_toolkit.swap_ido(base, ido71, tmp_path / "a.tar.gz")
    b = build_toolkit.swap_ido(base, ido71, tmp_path / "b.tar.gz")
    assert a.read_bytes() == b.read_bytes()
    old, new = _members(base), _members(a)
    for name in ("jobs/runner.py", "bin/objdump", "shim/conveyor_shim.h"):
        assert new[name] == old[name]
    ido = {n for n in new if n.startswith("ido/")}
    assert ido == {f"ido/{f.name}" for f in ido71.iterdir()}
    assert new["ido/cfe"][0] == b"ido7.1:cfe"
    manifest = json.loads(new["manifest.json"][0])
    assert manifest["contents"] == sorted(n for n in new if n != "manifest.json")
    assert compilers.identify_toolkit(a, _fake_table()) == "ido7.1"


# --- lock binding ---------------------------------------------------------------

SRC = "int f(int a) { return a + 1; }\n"


def _lock_args(tmp_path, **kw):
    (tmp_path / "f.c").write_text(SRC)
    base = dict(spec="f.c:f", flags="-g0 -O2 -mips2 -G 0 -non_shared", target=None,
                skip_verify=False, toolkit=None, wait=1, data=str(tmp_path),
                lockfile=str(tmp_path / "lock.json"), coordinator="http://x", token="t")
    base.update(kw)
    return argparse.Namespace(**base)


@pytest.fixture
def lock_env(tmp_path, monkeypatch):
    monkeypatch.setattr(lockmod, "REPO", tmp_path)
    monkeypatch.setattr(lockmod, "require_promotable_population", lambda *a: None)
    calls = []

    def verify(spec, flagset, target_id, args, toolkit_sha=None):
        calls.append(toolkit_sha)
        return 0, toolkit_sha or "pinned53"
    monkeypatch.setattr(lockmod, "verify_entry", verify)
    monkeypatch.setattr(lockmod, "toolkit_compiler",
                        lambda sha, args: {"tk71": "ido7.1", "tk53": "ido5.3"}[sha])
    return calls


def test_lock_add_with_toolkit_records_compiler(tmp_path, lock_env):
    args = _lock_args(tmp_path, toolkit="tk71")
    lockmod.cmd_add(args)
    entry = json.loads(Path(args.lockfile).read_text())["f.c:f"]
    assert lock_env == ["tk71"]
    assert entry["toolkit_sha"] == "tk71" and entry["compiler"] == "ido7.1"
    assert entry["verified"] == "score0"


def test_lock_add_without_toolkit_is_unchanged(tmp_path, lock_env):
    args = _lock_args(tmp_path)
    lockmod.cmd_add(args)
    entry = json.loads(Path(args.lockfile).read_text())["f.c:f"]
    assert lock_env == [None]
    assert entry["toolkit_sha"] == "pinned53" and "compiler" not in entry


def test_lock_add_toolkit_refuses_skip_verify(tmp_path, lock_env):
    with pytest.raises(SystemExit, match="cannot be combined"):
        lockmod.cmd_add(_lock_args(tmp_path, toolkit="tk71", skip_verify=True))


def test_lock_verify_reproves_compiler_entries_with_their_toolkit(tmp_path, lock_env):
    args = _lock_args(tmp_path)
    Path(args.lockfile).write_text(json.dumps({
        "f.c:f": {"body_sha256": "x", "target_id": "f", "flagset": args.flags,
                  "verified": "score0", "toolkit_sha": "tk71", "compiler": "ido7.1"},
        "g.c:g": {"body_sha256": "y", "target_id": "g", "flagset": args.flags,
                  "verified": "score0", "toolkit_sha": "old53"}}))
    lockmod.cmd_verify(argparse.Namespace(**dict(vars(args), spec=None)))
    assert sorted(lock_env, key=str) == [None, "tk71"]


def test_toolkit_compiler_refuses_unknown_toolkit(tmp_path, monkeypatch):
    blobs = tmp_path / "blobs"
    from tools.conveyor.coordinator.store import BlobStore
    store = BlobStore(blobs)
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w:gz") as tar:
        info = tarfile.TarInfo("ido/cc")
        info.size = 3
        tar.addfile(info, io.BytesIO(b"odd"))
    sha = store.put_bytes(buf.getvalue())
    with pytest.raises(SystemExit, match="not a known build"):
        lockmod.toolkit_compiler(sha, argparse.Namespace(data=str(tmp_path)))


def test_installed_dir_prefers_builder_then_cloud_then_none(tmp_path):
    from tools.conveyor import compilers
    assert compilers.installed_dir("ido7.1", repo=tmp_path) is None
    cloud = tmp_path / compilers.COMPILERS["ido7.1"]["cloud_dir"]
    cloud.mkdir(parents=True)
    (cloud / "cc").write_bytes(b"x")
    assert compilers.installed_dir("ido7.1", repo=tmp_path) == cloud
    builder = tmp_path / compilers.COMPILERS["ido7.1"]["builder_dir"]
    builder.mkdir(parents=True)
    (builder / "cc").write_bytes(b"x")
    assert compilers.installed_dir("ido7.1", repo=tmp_path) == builder
    with pytest.raises(SystemExit):
        compilers.installed_dir("ido9.9", repo=tmp_path)


def test_tu_cc_follows_the_segment_pin(tmp_path):
    from tools.conveyor import compilers
    pins = {"0x1a660": "ido7.1"}
    assert compilers.tu_cc("lib_10d60", "/ido53/cc", pins, tmp_path) == Path("/ido53/cc")
    with pytest.raises(AssertionError, match="run tools/cloud/setup.sh"):
        compilers.tu_cc("lib_1a660", "/ido53/cc", pins, tmp_path)
    cloud = tmp_path / compilers.COMPILERS["ido7.1"]["cloud_dir"]
    cloud.mkdir(parents=True)
    (cloud / "cc").write_bytes(b"x")
    assert compilers.tu_cc("0x1a660", "/ido53/cc", pins, tmp_path) == cloud / "cc"


# --- driver wrapper (w26a: ido5.3s, tools/ido_sas/cc) ------------------------------

def _wrapper_table(wrapper_bytes):
    table = _fake_table()
    table["ido5.3s"] = dict(compilers.COMPILERS["ido5.3s"], wrapper_sha256=_sha(wrapper_bytes))
    return table


def test_wrapper_pin_is_the_committed_driver():
    spec = compilers.COMPILERS["ido5.3s"]
    assert spec["wrapper_sha256"] == _sha((REPO / spec["builder_dir"] / "cc").read_bytes())
    assert spec["make_cc"] == "$(IDO53S_CC)" and compilers.is_wrapper("ido5.3s")
    assert not compilers.is_wrapper("ido5.3")


def test_identify_wrapper_dir_follows_the_wrapped_compiler(tmp_path):
    table = _wrapper_table(b"#!wrapper")
    root = tmp_path / "tools"
    (root / "ido_sas").mkdir(parents=True)
    (root / "ido_sas" / "cc").write_bytes(b"#!wrapper")
    assert compilers.identify_dir(root / "ido_sas", table) is None      # nothing wrapped
    _fake_ido(root / "ido-static-recomp" / "build" / "out", "ido5.3")
    assert compilers.identify_dir(root / "ido_sas", table) == "ido5.3s"
    _fake_ido(root / "ido_sas" / "ido53", "ido7.1")                      # first dir wins
    assert compilers.identify_dir(root / "ido_sas", table) is None
    (root / "ido_sas" / "cc").write_bytes(b"#!edited")
    assert compilers.identify_dir(root / "ido_sas", table) is None


def test_identify_wrapper_toolkit(tmp_path):
    table = _wrapper_table(b"#!wrapper")
    ido = _fake_ido(tmp_path / "ido53", "ido5.3")
    members = [("ido/cc", b"#!wrapper", 0o755)] + [
        (f"ido/ido53/{f.name}", f.read_bytes(), 0o755) for f in ido.iterdir()]
    tk = build_toolkit._write_bundle(members, tmp_path / "tk.tar.gz")
    assert compilers.identify_toolkit(tk, table) == "ido5.3s"
    bad = build_toolkit._write_bundle([("ido/cc", b"#!wrapper", 0o755)] + [
        (f"ido/ido53/{f.name}", b"x", 0o755) for f in ido.iterdir()], tmp_path / "bad.tar.gz")
    assert compilers.identify_toolkit(bad, table) is None


def test_install_refuses_wrapper():
    with pytest.raises(SystemExit, match="driver wrapper"):
        compilers.install("ido5.3s")


def _load_driver():
    import importlib.machinery
    import importlib.util
    path = REPO / "tools" / "ido_sas" / "cc"
    loader = importlib.machinery.SourceFileLoader("ido_sas_cc", str(path))
    spec = importlib.util.spec_from_loader("ido_sas_cc", loader)
    mod = importlib.util.module_from_spec(spec)
    loader.exec_module(mod)
    return mod


def test_driver_strips_loc_option_and_comments():
    drv = _load_driver()
    text = ("\t.verstamp\t3 19\n\t.option\tO2\n\t.option\tpic0\n\t.loc\t2 5\n"
            " #   5\ts32 f(void) {\n\tsubu\t$sp, 40\n\t.livereg\t0x1,0\n")
    assert drv.strip_asm(text) == "\t.verstamp\t3 19\n\t.option\tpic0\n\tsubu\t$sp, 40\n\t.livereg\t0x1,0\n"
    flags, inputs, out = drv.split_args(["-c", "-G", "0", "-O2", "-I", "inc", "-o", "x.o", "a.c"])
    assert flags == ["-c", "-G", "0", "-O2", "-I", "inc"] and inputs == ["a.c"] and out == "x.o"
    assert drv.absolute_includes(["-I", "inc", "-Isrc/rom", "-O2"]) == [
        "-I", os.path.abspath("inc"), "-I" + os.path.abspath("src/rom"), "-O2"]
