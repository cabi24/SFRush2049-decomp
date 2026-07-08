import json
import tarfile

import pytest

from tools.conveyor.bundles import manifest as manifestmod
from tools.conveyor.bundles.build_job import build_job_bundle


def _manifest(**kw):
    m = {
        "job_type": "compile_score",
        "toolkit_sha": "t" * 64,
        "cells": [{"candidate": "game.c:game", "flagset": "-O2", "target": "func_800FD464"}],
    }
    m.update(kw)
    return m


def test_manifest_sha_stable_under_key_order():
    a = {"job_type": "compile_score", "toolkit_sha": "t" * 64, "cells": []}
    b = {"cells": [], "toolkit_sha": "t" * 64, "job_type": "compile_score"}
    assert manifestmod.manifest_sha(a) == manifestmod.manifest_sha(b)


def test_validate_rejects_bad_manifests():
    with pytest.raises(ValueError):
        manifestmod.validate({"job_type": "nope"})
    with pytest.raises(ValueError):
        manifestmod.validate({"job_type": "compile_score", "toolkit_sha": "t" * 64})
    with pytest.raises(ValueError):  # search jobs must be budgeted
        manifestmod.validate(
            {
                "job_type": "permuter_search",
                "toolkit_sha": "t" * 64,
                "target_id": "f",
                "seed_file": "s.c",
                "target_file": "t.o",
                "compile_flags": "-O2",
            }
        )


def test_bundle_roundtrip(tmp_path):
    m = _manifest()
    out, sha = build_job_bundle(
        m,
        {"seed.c": b"int f(void){return 1;}\n", "target.o": b"\x7fELFfake"},
        tmp_path / "job.tar.gz",
    )
    with tarfile.open(out) as tar:
        names = sorted(tar.getnames())
        assert names == ["inputs/seed.c", "inputs/target.o", "manifest.json"]
        extracted = tar.extractfile("inputs/seed.c").read()
        assert extracted == b"int f(void){return 1;}\n"
        # The returned sha is the identity of the manifest as shipped in the
        # bundle (with input_shas folded in) — what nodes recompute and the
        # coordinator uses as the cache key.
        shipped = json.loads(tar.extractfile("manifest.json").read())
        assert sha == manifestmod.manifest_sha(shipped)
        assert set(shipped["input_shas"]) == {"seed.c", "target.o"}


def test_bundle_sha_tracks_input_content(tmp_path):
    """Same manifest, different input bytes -> different manifest_sha.

    Guards the result cache key: without this, editing a source file and
    resubmitting would short-circuit to the stale cached result."""
    m = _manifest()
    _, sha_a = build_job_bundle(m, {"seed.c": b"int f(void){return 1;}\n"},
                                tmp_path / "a.tar.gz")
    _, sha_b = build_job_bundle(m, {"seed.c": b"int f(void){return 2;}\n"},
                                tmp_path / "b.tar.gz")
    assert sha_a != sha_b


def test_bundle_bytes_deterministic(tmp_path):
    m = _manifest()
    files = {"a.c": b"aaa", "b.c": b"bbb"}
    p1, _ = build_job_bundle(m, files, tmp_path / "one.tar.gz")
    p2, _ = build_job_bundle(m, dict(reversed(list(files.items()))), tmp_path / "two.tar.gz")
    assert p1.read_bytes() == p2.read_bytes()
