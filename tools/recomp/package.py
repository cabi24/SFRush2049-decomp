#!/usr/bin/env python3
"""Build the gameplay-test package `rush2049-recomp-test-v<N>.tar.gz` (Phase 4).

Everything a tester needs to build and run the recompiled game on their own
machine (primary target: the owner's Windows 11 gaming laptop), except RT64
(cloned from GitHub at the pinned commit; INSTALL.md gives the URL and commit)
and, unless --with-rom, the ROM:

    rush2049-recomp-test-vN/
      README.md INSTALL.md TEST_PROCESS.md REPORT_TEMPLATE.md CHANGES.md VERSION.txt
      MANIFEST.sha256                 sha256 of every other file in the package
      src/N64Recomp/                  pinned source snapshot (no .git), with submodules
      src/N64ModernRuntime/           pinned source snapshot (no .git), with submodules
      rush/tools/recomp/              our runtime, scripts, patches/identity/replacement
                                      inputs, make_rom.py, oracle.py, docs (no tests)
      config/                         the generated N64Recomp config of the stock build:
                                      rush2049.recomp.toml, *.syms.toml, manifest.json,
                                      rush2049_images.h, rush2049_repl.h, rush2049.rsp_audio.toml
                                      (names and addresses only; no ROM bytes, no recompiled C)
      prebuilt/linux-x86_64/          quick-start host binary built on rocky
      rom/baserom.us.z64              only with --with-rom (owner's private copy)

N (the package version) and the source pins come from the `package-pins` block
of tools/recomp/VERSIONS.md; every recomp phase that changes what a tester
runs bumps it and adds a CHANGES entry (tools/recomp/package/CHANGES.md).
The documents are templates in tools/recomp/package/ with @@KEY@@ fields.

    python3 tools/recomp/package.py --n64recomp ~/rush2049/recomp/src/N64Recomp \
        --runtime ~/rush2049/recomp/src/N64ModernRuntime --config build/stock \
        --binary build/stock/host-rt64/rush2049_recomp --out ~/rush2049/recomp/dist \
        [--with-rom ~/rush2049/recomp/rom/baserom.us.z64]

The archive is deterministic for the same inputs (sorted members, fixed
mtime/owner, gzip mtime 0). Standard library only. Never commit the tarball.
"""
import argparse
import gzip
import hashlib
import io
import json
import re
import subprocess
import sys
import tarfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
VERSIONS = HERE / "VERSIONS.md"
TEMPLATES = HERE / "package"
DOCS = ("README.md", "INSTALL.md", "TEST_PROCESS.md", "REPORT_TEMPLATE.md", "CHANGES.md")
CONFIG_FILES = ("rush2049.recomp.toml", "rush2049.syms.toml", "rush2049.datasyms.toml", "manifest.json",
                "rush2049_images.h", "rush2049_repl.h", "rush2049.rsp_audio.toml")
# tools/recomp files that are not for the tester (repo-only tooling stays out)
RUSH_EXCLUDE = re.compile(r"(^|/)(__pycache__|package\.py$|package/)")
SNAPSHOT_EXCLUDE = {".git", "__pycache__", ".github"}
MTIME = 1760227200          # 2025-10-12 00:00 UTC: fixed member mtime
US_SHA1 = "3f99351d7bb61656614bdb2aa1a90cfe55d1922c"


class PackageError(RuntimeError):
    pass


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def read_pins(path=VERSIONS):
    """{key: value} from the `<!-- package-pins ... -->` block of VERSIONS.md."""
    text = Path(path).read_text()
    m = re.search(r"<!--\s*package-pins\s*\n(.*?)-->", text, re.S)
    if not m:
        raise PackageError(f"{path}: no package-pins block")
    pins = {}
    for line in m.group(1).splitlines():
        line = line.strip()
        if line and not line.startswith("#"):
            k, _, v = line.partition("=")
            pins[k.strip()] = v.strip()
    for key in ("package_version", "n64recomp", "n64modernruntime", "rt64_url", "rt64"):
        if key not in pins:
            raise PackageError(f"{path}: package-pins lacks {key}")
    if not pins["package_version"].isdigit():
        raise PackageError("package_version must be an integer")
    return pins


def git_head(path):
    try:
        out = subprocess.run(["git", "-C", str(path), "rev-parse", "HEAD"], capture_output=True, text=True)
    except OSError:
        return None
    return out.stdout.strip() if out.returncode == 0 else None


def snapshot(root):
    """[(relative path, file)] of a source tree without VCS metadata, sorted."""
    root = Path(root)
    out = []
    for p in sorted(root.rglob("*")):
        rel = p.relative_to(root)
        if any(part in SNAPSHOT_EXCLUDE for part in rel.parts):
            continue
        if p.is_symlink() or not p.is_file():
            continue
        out.append((rel.as_posix(), p))
    return out


def rush_files(tools_recomp=HERE):
    out = []
    for rel, p in snapshot(tools_recomp):
        if not RUSH_EXCLUDE.search(rel):
            out.append((rel, p))
    return out


def render(template, fields):
    def sub(m):
        key = m.group(1)
        if key not in fields:
            raise PackageError(f"template field @@{key}@@ has no value")
        return str(fields[key])
    return re.sub(r"@@([A-Z0-9_]+)@@", sub, template)


def build(n64recomp, runtime, config, binary, out_dir, pins, with_rom=None, templates=TEMPLATES,
          tools_recomp=HERE, check_pins=True, repo_commit=None):
    """Write the package; returns {path, name, sha256, size, files}."""
    version = int(pins["package_version"])
    name = "rush2049-recomp-test-v%d" % version
    for label, path, pin in (("N64Recomp", n64recomp, pins["n64recomp"]),
                             ("N64ModernRuntime", runtime, pins["n64modernruntime"])):
        if not Path(path).is_dir():
            raise PackageError(f"{label} source {path} not found")
        head = git_head(path)
        if check_pins and head is not None and head != pin:
            raise PackageError(f"{label} checkout is at {head}, VERSIONS.md pins {pin}")
    config = Path(config)
    manifest = json.loads((config / "manifest.json").read_text())
    if manifest.get("replacements", {}).get("replaced"):
        raise PackageError("config is a replacement/capture build; package the stock config")
    binary = Path(binary) if binary else None
    if binary is not None and not binary.is_file():
        raise PackageError(f"prebuilt binary {binary} not found")
    rom_bytes = None
    if with_rom:
        rom_bytes = Path(with_rom).read_bytes()
        if hashlib.sha1(rom_bytes).hexdigest() != US_SHA1:
            raise PackageError("--with-rom: not the US retail ROM (SHA-1 %s)" % hashlib.sha1(rom_bytes).hexdigest())

    members = {}     # package-relative path -> (bytes, mode)

    def add(rel, data, mode=0o644):
        if rel in members:
            raise PackageError(f"duplicate package path {rel}")
        members[rel] = (data, mode)

    def add_file(rel, p):
        mode = 0o755 if p.stat().st_mode & 0o111 else 0o644
        add(rel, p.read_bytes(), mode)

    for rel, p in snapshot(n64recomp):
        add_file("src/N64Recomp/" + rel, p)
    for rel, p in snapshot(runtime):
        add_file("src/N64ModernRuntime/" + rel, p)
    for rel, p in rush_files(tools_recomp):
        add_file("rush/tools/recomp/" + rel, p)
    for f in CONFIG_FILES:
        p = config / f
        if not p.is_file():
            raise PackageError(f"config file {p} missing (run gen_config.py for the stock build)")
        add("config/" + f, p.read_bytes())
    bin_sha = "-"
    if binary is not None:
        data = binary.read_bytes()
        bin_sha = sha256(data)
        add("prebuilt/linux-x86_64/rush2049_recomp", data, 0o755)
    if rom_bytes is not None:
        add("rom/baserom.us.z64", rom_bytes)

    fields = {
        "VERSION": "v%d" % version, "PACKAGE": name,
        "N64RECOMP_COMMIT": pins["n64recomp"], "RUNTIME_COMMIT": pins["n64modernruntime"],
        "RT64_URL": pins["rt64_url"], "RT64_COMMIT": pins["rt64"],
        "REPO_COMMIT": repo_commit or git_head(REPO) or "unknown",
        "SYNTH_SHA256": manifest["synthetic_rom"]["sha256"], "SYNTH_SIZE": manifest["synthetic_rom"]["size"],
        "ROM_SHA1": manifest.get("rom_sha1", US_SHA1), "BINARY_SHA256": bin_sha,
        "WITH_ROM": "yes (rom/baserom.us.z64, private)" if rom_bytes is not None else "no",
    }
    for doc in DOCS:
        add(doc, render((Path(templates) / doc).read_text(), fields).encode())
    add("VERSION.txt", ("package %s\nrepo commit %s\nN64Recomp %s\nN64ModernRuntime %s\nRT64 %s @ %s\n"
                        "synthetic ROM sha256 %s\nprebuilt binary sha256 %s\nROM included %s\n"
                        % (fields["VERSION"], fields["REPO_COMMIT"], fields["N64RECOMP_COMMIT"],
                           fields["RUNTIME_COMMIT"], fields["RT64_URL"], fields["RT64_COMMIT"],
                           fields["SYNTH_SHA256"], bin_sha, fields["WITH_ROM"])).encode())
    manifest_lines = ["%s  %s" % (sha256(members[k][0]), k) for k in sorted(members)]
    add("MANIFEST.sha256", ("\n".join(manifest_lines) + "\n").encode())

    out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    tar_path = out_dir / (name + ("-with-rom" if rom_bytes is not None else "") + ".tar.gz")
    raw = io.BytesIO()
    with tarfile.open(fileobj=raw, mode="w", format=tarfile.PAX_FORMAT) as tar:
        dirs = set()
        for rel in sorted(members):
            parts = rel.split("/")[:-1]
            for i in range(1, len(parts) + 1):
                dirs.add("/".join(parts[:i]))
        entries = [(d, None) for d in dirs] + [(r, members[r]) for r in members]
        entries.append(("", None))
        for rel, item in sorted(entries, key=lambda e: e[0]):
            info = tarfile.TarInfo(name + ("/" + rel if rel else ""))
            info.mtime = MTIME
            info.uid = info.gid = 0
            info.uname = info.gname = ""
            if item is None:
                info.type = tarfile.DIRTYPE
                info.mode = 0o755
                tar.addfile(info)
            else:
                data, mode = item
                info.size = len(data)
                info.mode = mode
                tar.addfile(info, io.BytesIO(data))
    with open(tar_path, "wb") as f:
        with gzip.GzipFile(filename="", mode="wb", fileobj=f, mtime=0, compresslevel=9) as gz:
            gz.write(raw.getvalue())
    data = tar_path.read_bytes()
    return {"path": str(tar_path), "name": name, "version": version, "sha256": sha256(data), "size": len(data),
            "files": len(members), "with_rom": rom_bytes is not None}


def verify(tar_path):
    """Check MANIFEST.sha256 against an archive's contents; returns the file count."""
    with tarfile.open(tar_path, "r:gz") as tar:
        files = {m.name.split("/", 1)[1]: tar.extractfile(m).read() for m in tar.getmembers() if m.isfile()}
    if "MANIFEST.sha256" not in files:
        raise PackageError("no MANIFEST.sha256")
    listed = {}
    for line in files["MANIFEST.sha256"].decode().splitlines():
        digest, _, rel = line.partition("  ")
        listed[rel] = digest
    others = {k for k in files if k != "MANIFEST.sha256"}
    if set(listed) != others:
        raise PackageError("manifest lists %d files, archive has %d" % (len(listed), len(others)))
    for rel, digest in listed.items():
        if sha256(files[rel]) != digest:
            raise PackageError(f"{rel}: sha256 mismatch")
    return len(listed)


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--n64recomp", help="N64Recomp checkout at the pinned commit")
    p.add_argument("--runtime", help="N64ModernRuntime checkout (recursive) at the pinned commit")
    p.add_argument("--config", help="stock gen_config output dir (build/stock or build/recomp)")
    p.add_argument("--binary", default=None, help="prebuilt Linux x86_64 host (rush2049_recomp)")
    p.add_argument("--out", default=str(REPO / "build" / "recomp" / "dist"), help="output directory")
    p.add_argument("--with-rom", default=None, metavar="ROM",
                   help="include this ROM as rom/baserom.us.z64 (owner's private copy only; never commit)")
    p.add_argument("--versions", default=str(VERSIONS), help="VERSIONS.md with the package-pins block")
    p.add_argument("--verify", default=None, metavar="TARBALL", help="only check an archive's MANIFEST")
    p.add_argument("--repo-commit", default=None, help="repo commit the tools come from (default: git HEAD)")
    args = p.parse_args(argv)
    try:
        if args.verify:
            print("verified %d files" % verify(args.verify))
            return 0
        if not (args.n64recomp and args.runtime and args.config):
            p.error("--n64recomp, --runtime and --config are required")
        res = build(args.n64recomp, args.runtime, args.config, args.binary, args.out, read_pins(args.versions),
                    with_rom=args.with_rom, repo_commit=args.repo_commit)
        verify(res["path"])
    except (OSError, ValueError, KeyError, PackageError) as exc:
        print("package: %s" % exc, file=sys.stderr)
        return 2
    print("package %s: %s (%d files, %d bytes, sha256 %s)%s" % (
        res["name"], res["path"], res["files"], res["size"], res["sha256"],
        " WITH ROM: private, never commit or share" if res["with_rom"] else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main())
