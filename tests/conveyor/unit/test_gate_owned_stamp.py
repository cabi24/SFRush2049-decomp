"""w24t: the builder gate cleans build/us when the owned-slot inputs changed
since the last successful build there (promote.owned_stamp_guard), instead of
failing with "owned ROM slot address changed" on a correct tree."""
import json
import subprocess
import types

from tools.conveyor.pipeline import promote as P


def _repo(tmp_path):
    r = tmp_path / "repo"
    (r / "src/rom").mkdir(parents=True)
    (r / "rom_owned_data.json").write_text(json.dumps({"slots": []}))
    (r / "rush2049.us.ld").write_text("SECTIONS {}\n")
    (r / "Makefile").write_text("all:\n")
    (r / "src/rom/lib_x.c").write_text("int x;\n")
    (r / "build/us").mkdir(parents=True)
    (r / "build/us/old.o").write_text("stale")
    return r


def _gate(r, monkeypatch):
    """Run the guard exactly as the gates chain it, with `true` for make."""
    monkeypatch.setattr(P, "owned_slot_inputs", lambda repo=None: [
        "rom_owned_data.json", "rush2049.us.ld", "Makefile", "rom_owned_storage.ld"])
    pre, post = P.owned_stamp_guard(r)
    out = subprocess.run(["bash", "-c", f"cd {r} && {pre} && touch src/rom/lib_x.c && true && {post}"],
                         capture_output=True, text=True)
    assert out.returncode == 0, out.stderr
    return out.stdout


def test_unstamped_build_dir_is_cleaned_once(tmp_path, monkeypatch):
    r = _repo(tmp_path)
    out = _gate(r, monkeypatch)
    assert P.GATE_CLEAN_MARK in out and "stamp none" in out
    assert not (r / "build/us/old.o").exists()
    assert len((r / P.OWNED_STAMP).read_text().strip()) == 64
    (r / "build/us/kept.o").write_text("fresh")
    assert _gate(r, monkeypatch) == ""          # same inputs: no clean
    assert (r / "build/us/kept.o").exists()


def test_owned_registry_change_cleans(tmp_path, monkeypatch):
    r = _repo(tmp_path)
    _gate(r, monkeypatch)
    (r / "build/us/kept.o").write_text("fresh")
    (r / "rom_owned_data.json").write_text(json.dumps({"slots": [{"owner": "f"}]}))
    out = _gate(r, monkeypatch)
    assert P.GATE_CLEAN_MARK in out and "rm -rf build/us" in out
    assert not (r / "build/us/kept.o").exists()
    # a file appearing (new .bss/storage slot output) also changes the hash
    (r / "build/us/kept.o").write_text("fresh")
    (r / "rom_owned_storage.ld").write_text("/* new */\n")
    assert P.GATE_CLEAN_MARK in _gate(r, monkeypatch)


def test_tu_edit_does_not_clean(tmp_path, monkeypatch):
    r = _repo(tmp_path)
    _gate(r, monkeypatch)
    (r / "build/us/kept.o").write_text("fresh")
    (r / "src/rom/lib_x.c").write_text("int y;\n")
    assert _gate(r, monkeypatch) == ""
    assert (r / "build/us/kept.o").exists()


def test_missing_build_dir_is_not_reported(tmp_path, monkeypatch):
    r = _repo(tmp_path)
    subprocess.run(["rm", "-rf", str(r / "build")], check=True)
    assert _gate(r, monkeypatch) == ""
    assert (r / P.OWNED_STAMP).is_file()


def test_failed_make_writes_no_stamp(tmp_path, monkeypatch):
    r = _repo(tmp_path)
    monkeypatch.setattr(P, "owned_slot_inputs", lambda repo=None: ["rom_owned_data.json"])
    pre, post = P.owned_stamp_guard(r)
    subprocess.run(["bash", "-c", f"cd {r} && {pre} && false && {post}"], capture_output=True)
    assert not (r / P.OWNED_STAMP).exists()


def test_owned_slot_inputs_cover_registry_and_companions_not_tus():
    paths = P.owned_slot_inputs()
    assert "rom_owned_data.json" in paths and "rush2049.us.ld" in paths
    assert any(p.endswith(".bss.s") for p in paths)      # passthrough companions
    assert not any(p.startswith("src/rom/") and p.endswith(".c") for p in paths)
    assert "baserom.us.z64" not in paths


def test_via_builder_gate_chains_guard_and_logs(monkeypatch, capsys, tmp_path):
    seen = []

    def fake_run(cmd, cwd=None, timeout=1200):
        seen.append(cmd)
        stdout = "GATE-CLEAN: owned-slot inputs changed\nROM matches!\n" if cmd[0] == "ssh" else ""
        return types.SimpleNamespace(returncode=0, stdout=stdout, stderr="")

    monkeypatch.setattr(P, "_run", fake_run)
    monkeypatch.setattr(P.owned_data, "promotion_paths", lambda repo, tu: [])
    tu = "src/rom/lib_x.c"
    monkeypatch.setattr(P, "REPO", tmp_path)
    (tmp_path / "src/rom").mkdir(parents=True)
    (tmp_path / tu).write_text("int x;\n")
    ok, _ = P._build_and_gate(True, tu)
    assert ok
    remote = seen[-1][-1]
    assert remote.index(P.OWNED_STAMP) < remote.index("touch " + tu)
    assert remote.index("make COMPILER=ido") < remote.index('echo "$H" > $S') < remote.index("make test")
    assert "builder gate: GATE-CLEAN:" in capsys.readouterr().out
