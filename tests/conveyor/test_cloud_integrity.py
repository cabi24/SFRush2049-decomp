"""F6: read only target bytes authenticated by the committed hash manifest."""
import hashlib
import json

import pytest

from tools.cloud import score
from tools.conveyor.pipeline import blob_tu


@pytest.fixture
def targets(tmp_path, monkeypatch):
    assembly = tmp_path / "region.s"
    assembly.write_text('.section .text.f, "ax", @progbits\n.word 0x03E00008\n.word 0x00000000\n')
    (tmp_path / "symbols.json").write_text(json.dumps({"symbols": {"f": "0x80010000"}}))
    blob_tu.write_target_hashes(tmp_path)
    monkeypatch.setattr(score, "ASM_DIR", tmp_path)
    monkeypatch.setattr(score, "_targets", None)
    monkeypatch.setattr(score, "_target_fingerprint", None)
    return tmp_path


def test_manifest_is_deterministic_and_covers_exact_file_bytes(targets):
    manifest = targets / "SHA256SUMS"
    first = manifest.read_bytes()
    blob_tu.write_target_hashes(targets)
    assert manifest.read_bytes() == first
    assert score.target_manifest() == {
        name: hashlib.sha256((targets / name).read_bytes()).hexdigest()
        for name in ("region.s", "symbols.json")}
    assert score.targets() == {"f": [0x03E00008, 0]}
    assert score.image_symbols() == {"f": 0x80010000}


@pytest.mark.parametrize("cached", [False, True])
def test_altered_word_refuses_scoring_even_after_cache_load(targets, cached):
    if cached:
        score.targets()
    source = targets / "region.s"
    source.write_text(source.read_text().replace("03E00008", "DEADBEEF"))
    with pytest.raises(SystemExit, match="SHA-256 mismatch"):
        # compare must refuse before opening/decoding even a supplied object.
        score.compare(targets / "unused.o", "f")


def test_changed_symbol_address_is_rejected(targets):
    path = targets / "symbols.json"
    path.write_text(path.read_text().replace("80010000", "80020000"))
    with pytest.raises(SystemExit, match="SHA-256 mismatch"):
        score.image_symbols()


def test_allow_unverified_does_not_bypass_integrity_failure(targets, monkeypatch):
    (targets / "region.s").write_text(".word 0xDEADBEEF\n")
    monkeypatch.setattr(score, "compile_single", lambda source, flags, out: None)
    monkeypatch.setattr("sys.argv", ["score.py", "fn", "dummy.c", "f", "--allow-unverified"])
    with pytest.raises(SystemExit, match="SHA-256 mismatch"):
        score.main()


@pytest.mark.parametrize("operation", ["delete", "add", "unlist"])
def test_region_set_must_match_manifest(targets, operation):
    if operation == "delete":
        (targets / "region.s").unlink()
    elif operation == "add":
        (targets / "new.s").write_text(".word 0x00000000\n")
    else:
        path = targets / "SHA256SUMS"
        path.write_text("\n".join(line for line in path.read_text().splitlines()
                                  if not line.endswith("region.s")) + "\n")
    with pytest.raises(SystemExit, match="region file set differs"):
        score.targets()


@pytest.mark.parametrize("kind", ["missing", "empty", "bad-hash", "traversal", "duplicate",
                                  "missing-symbols"])
def test_bad_manifest_fails_closed(targets, kind):
    path = targets / "SHA256SUMS"
    data = path.read_text()
    if kind == "missing":
        path.unlink()
    else:
        changed = {
            "empty": "", "bad-hash": data.replace(data[:64], "not-a-hash"),
            "traversal": data.replace("region.s", "../region.s"),
            "duplicate": data + data.splitlines()[0] + "\n",
            "missing-symbols": data.splitlines()[0] + "\n",
        }[kind]
        path.write_text(changed)
    with pytest.raises(SystemExit, match="target integrity check failed"):
        score.targets()


def test_missing_symbol_file_is_rejected(targets):
    (targets / "symbols.json").unlink()
    with pytest.raises(SystemExit, match="missing file or hash"):
        score.image_symbols()


def test_repaired_file_can_be_loaded_after_a_failed_check(targets):
    path = targets / "region.s"
    original = path.read_bytes()
    path.write_bytes(b"bad bytes")
    with pytest.raises(SystemExit, match="SHA-256 mismatch"):
        score.targets()
    path.write_bytes(original)
    assert score.targets() == {"f": [0x03E00008, 0]}
