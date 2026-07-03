"""Build the toolkit bundle: everything a node needs beyond Python.

Contents (unpacked as $CONVEYOR_TOOLKIT on nodes):
    ido/                IDO recomp binaries (cc, as1, ugen, ...)
    bin/objdump         mips objdump (+ ldd-copied shared libs in lib/)
    decomp-permuter/    vendored permuter source (Scorer + engine)
    jobs/               conveyor job executor code
    shim/               arcade compatibility headers
    manifest.json       {kind, arch, entry, created_from}

Must run on x86-64 (the binaries it packs are x86-64). Deterministic tar so
identical inputs produce an identical toolkit sha.
"""
import argparse
import gzip
import io
import json
import subprocess
import tarfile
from pathlib import Path

_EXCLUDE_DIRS = {"__pycache__", ".git", ".mypy_cache", "tests"}


def _iter_files(root, prefix):
    root = Path(root)
    for path in sorted(root.rglob("*")):
        if any(part in _EXCLUDE_DIRS for part in path.parts):
            continue
        if path.is_file():
            yield path, f"{prefix}/{path.relative_to(root)}"


def _ldd_libs(binary):
    """Shared libraries a binary needs, for copying into the toolkit."""
    try:
        out = subprocess.run(
            ["ldd", str(binary)], capture_output=True, text=True, check=True
        ).stdout
    except (subprocess.CalledProcessError, FileNotFoundError):
        return []
    libs = []
    for line in out.splitlines():
        parts = line.split()
        if "=>" in parts:
            target = parts[parts.index("=>") + 1]
            if target.startswith("/") and "ld-linux" not in target:
                libs.append(Path(target))
    return libs


def _python_dep_dirs(names=("pycparser", "toml")):
    """Locate the permuter's pure-Python deps on the build machine so nodes
    never need pip. Packed at the toolkit root; permuter_search puts the
    toolkit on PYTHONPATH."""
    import importlib

    dirs = []
    for name in names:
        try:
            module = importlib.import_module(name)
        except ImportError:
            raise SystemExit(
                f"toolkit build requires the '{name}' package installed here "
                f"(it gets bundled for the nodes): pip install {name}"
            )
        dirs.append((name, Path(module.__file__).parent))
    return dirs


def build_toolkit(ido_dir, objdump_path, permuter_dir, shim_dir, jobs_dir, out_path):
    entries = []  # (archive_name, source_path, mode)

    for path, name in _iter_files(ido_dir, "ido"):
        entries.append((name, path, 0o755))
    objdump_path = Path(objdump_path)
    entries.append(("bin/objdump", objdump_path, 0o755))
    for lib in _ldd_libs(objdump_path):
        entries.append((f"lib/{lib.name}", lib, 0o644))
    for path, name in _iter_files(permuter_dir, "decomp-permuter"):
        entries.append((name, path, 0o644))
    for dep_name, dep_dir in _python_dep_dirs():
        for path, name in _iter_files(dep_dir, dep_name):
            entries.append((name, path, 0o644))
    for path, name in _iter_files(jobs_dir, "jobs"):
        entries.append((name, path, 0o644))
    for path, name in _iter_files(shim_dir, "shim"):
        entries.append((name, path, 0o644))

    manifest = {
        "kind": "conveyor-toolkit",
        "arch": "x86_64-linux",
        "entry": "jobs/runner.py",
        "contents": sorted(name for name, _, _ in entries),
    }

    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w") as tar:
        info = tarfile.TarInfo("manifest.json")
        data = json.dumps(manifest, sort_keys=True, separators=(",", ":")).encode()
        info.size, info.mtime, info.uid, info.gid, info.mode = len(data), 0, 0, 0, 0o644
        tar.addfile(info, io.BytesIO(data))
        for name, src, mode in sorted(entries):
            info = tarfile.TarInfo(name)
            payload = src.read_bytes()
            info.size, info.mtime, info.uid, info.gid, info.mode = (
                len(payload), 0, 0, 0, mode,
            )
            info.uname = info.gname = ""
            tar.addfile(info, io.BytesIO(payload))

    out_path = Path(out_path)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "wb") as f:
        with gzip.GzipFile(fileobj=f, mode="wb", mtime=0, filename="") as gz:
            gz.write(buf.getvalue())
    return out_path


def main():
    repo = Path(__file__).resolve().parents[2]  # tools/
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--ido", default=str(repo / "ido-static-recomp/build/out"))
    parser.add_argument("--objdump", default="/usr/bin/mips-linux-gnu-objdump")
    parser.add_argument("--permuter", default=str(repo / "decomp-permuter"))
    parser.add_argument("--shim", default=str(repo / "conveyor/seeds/shim"))
    parser.add_argument("--jobs", default=str(repo / "conveyor/jobs"))
    parser.add_argument("--out", required=True)
    args = parser.parse_args()
    out = build_toolkit(args.ido, args.objdump, args.permuter, args.shim, args.jobs, args.out)
    import hashlib

    sha = hashlib.sha256(Path(out).read_bytes()).hexdigest()
    print(f"toolkit: {out}\nsha256:  {sha}")


if __name__ == "__main__":
    main()
