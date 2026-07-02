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
    assert sha == manifestmod.manifest_sha(m)
    with tarfile.open(out) as tar:
        names = sorted(tar.getnames())
        assert names == ["inputs/seed.c", "inputs/target.o", "manifest.json"]
        extracted = tar.extractfile("inputs/seed.c").read()
        assert extracted == b"int f(void){return 1;}\n"


def test_bundle_bytes_deterministic(tmp_path):
    m = _manifest()
    files = {"a.c": b"aaa", "b.c": b"bbb"}
    p1, _ = build_job_bundle(m, files, tmp_path / "one.tar.gz")
    p2, _ = build_job_bundle(m, dict(reversed(list(files.items()))), tmp_path / "two.tar.gz")
    assert p1.read_bytes() == p2.read_bytes()
