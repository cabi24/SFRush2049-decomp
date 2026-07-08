"""Build a per-job bundle: manifest.json + inputs/, deterministic tar.gz.

The bundle is self-contained given its toolkit (FR-013): job executors see
only the unpacked bundle dir and $CONVEYOR_TOOLKIT.
"""
import gzip
import hashlib
import io
import tarfile
from pathlib import Path

from . import manifest as manifestmod


def _add_bytes(tar, name, data, mode=0o644):
    info = tarfile.TarInfo(name)
    info.size = len(data)
    info.mtime = 0
    info.uid = info.gid = 0
    info.uname = info.gname = ""
    info.mode = mode
    tar.addfile(info, io.BytesIO(data))


def build_job_bundle(manifest, input_files, out_path):
    """Pack a job bundle.

    manifest: dict (validated here); input_files: {archive_name: source_path_or_bytes}
    Returns (out_path, manifest_sha).

    Input content hashes are folded into the manifest (input_shas) before
    hashing: manifest_sha is the result cache key (FR-006), and a manifest
    that names its inputs without pinning their bytes would dedupe two jobs
    whose sources differ.
    """
    loaded = {}
    for name in sorted(input_files):
        src = input_files[name]
        loaded[name] = src if isinstance(src, bytes) else Path(src).read_bytes()
    manifest = dict(manifest)
    manifest["input_shas"] = {
        name: hashlib.sha256(data).hexdigest() for name, data in loaded.items()
    }
    manifestmod.validate(manifest)
    sha = manifestmod.manifest_sha(manifest)
    out_path = Path(out_path)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    # Deterministic gzip (mtime=0) over a deterministic tar (sorted, zeroed).
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w") as tar:
        _add_bytes(
            tar, "manifest.json", manifestmod.canonical_json(manifest).encode("ascii")
        )
        for name, data in sorted(loaded.items()):
            _add_bytes(tar, f"inputs/{name}", data)
    with open(out_path, "wb") as f:
        with gzip.GzipFile(fileobj=f, mode="wb", mtime=0, filename="") as gz:
            gz.write(buf.getvalue())
    return out_path, sha


def read_manifest(bundle_dir):
    """Read the manifest from an unpacked job bundle directory."""
    import json

    return json.loads((Path(bundle_dir) / "manifest.json").read_text())
