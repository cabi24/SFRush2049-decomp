"""Pinned IDO compilers: identity, installation and per-segment selection.

The cartridge was built with two IDO releases. libultra (and every static lock
before wave 17) is IDO 5.3; the Rush-specific boot-tail TUs (lib_11050 ...
lib_26ab0) are IDO 7.1 (cloud/work/frontier/w17g, w17k; see
docs/COMPILER_SETTINGS.md). A compiler is named here once, with:

* the decompals/ido-static-recomp v1.2 release archive and its sha256 (what
  CI/cloud boxes download, the same pattern as tools/cloud/setup.sh);
* where the builder keeps it (`builder_dir`) and the Makefile variable that
  selects it for one ROM TU (`make_cc`; None = the default $(CC));
* fingerprints: sha256 of the files that decide code generation, one set per
  known build (the conveyor toolkit's 5.3 was built from source, so it hashes
  differently from the release). A toolkit or directory is identified only when
  every fingerprinted file matches one known build; anything else is unknown
  and every consumer refuses it (fail closed).

`rom_compilers.json` (repo root) pins converted static segments to a
non-default compiler. A segment absent from it builds with the default (5.3),
exactly as before the file existed.

    python3 -m tools.conveyor.compilers install ido7.1 [--dest DIR] [--archive F]
    python3 -m tools.conveyor.compilers identify PATH   # ido dir or toolkit tar
    python3 -m tools.conveyor.compilers segments        # the committed pins

Stdlib only (runs on the Pi, the builder and CI).
"""
import argparse
import hashlib
import io
import json
import shutil
import sys
import tarfile
import tempfile
import urllib.request
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SEGMENT_PINS = REPO / "rom_compilers.json"
DEFAULT = "ido5.3"

# Files whose bytes decide what a compile produces (driver, front end,
# optimizer, code generator, assembler).
FINGERPRINT_FILES = ("cc", "cfe", "uopt", "ugen", "as1")

_RELEASE = "https://github.com/decompals/ido-static-recomp/releases/download/v1.2/"

COMPILERS = {
    "ido5.3": {
        "release": _RELEASE + "ido-5.3-recomp-linux.tar.gz",
        "archive_sha256": "ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506",
        "builder_dir": "tools/ido-static-recomp/build/out",
        "cloud_dir": "tools/cloud/ido",
        "make_cc": None,
        "builds": {
            # conveyor toolkits d4c39cbc / 796ae99a / 1e21f523 / 18117e74 (ido/),
            # and the builder's tools/ido-static-recomp/build/out
            "toolkit-source-build": {
                "cc": "d153db6456fd04acb8d6236e4b4355ac9f992d15065eb714d1563ab66b3666e5",
                "cfe": "7e3f7f4e7574a3bef6e945df75251ea897b67e0cc90162e628b0190c75fef229",
                "uopt": "7a3de75a232c5a2206cd1a22eb9f468f569e670a31ba087e46496075cde86415",
                "ugen": "f6670428faaa05e578125fc541398c9c1d9d488f0bbf65e12aa9689df1649d0f",
                "as1": "dedfa33720b91a9b72916df8c46ac6d92c6244c366d3302eaacfdfa7335972f9",
            },
            "release-v1.2": {
                "cc": "76d796c9591c9f5504949f85b29c32a6039a663791afe2f36f34ca18425b64d0",
                "cfe": "7033dfb267f39a6e5381c0769f4fc46bbde193cfec771314429d8367ace898f2",
                "uopt": "4e11956bdce9cac4c2934320cb29c55d1f3d3a0281ee594fafa5ebec88df4dd5",
                "ugen": "1734564701d39db213d6fe9c8f2cff16b82b28939fb1418348de479b41b7ac72",
                "as1": "39c9750c189df37d4df0f33e5eddec9d4e743cbecad0326af3dda437826dc695",
            },
        },
    },
    "ido7.1": {
        "release": _RELEASE + "ido-7.1-recomp-linux.tar.gz",
        "archive_sha256": "0d411696e178fcca34c31c3bf02011b928d7fd9c1fa7f8bf45070e0781b58e15",
        "builder_dir": "tools/ido-static-recomp/build71/out",
        "cloud_dir": "tools/cloud/ido71",
        "make_cc": "$(IDO71_CC)",
        "builds": {
            "release-v1.2": {
                "cc": "d886205c468987e49d1b294cc01c3d6bc886a879f4babb2f5035728d1df6029b",
                "cfe": "c264f0c1f3ece0d15c2cd2ff73fff20381e98ed09f630c8087e24010e2a5ce1a",
                "uopt": "b21294d653fa414ba97413dc2e17c17bb417f7699166b330ed00a6a3723f03a5",
                "ugen": "a8bcc5d4dad3b67428cde0dac6002c1e1ef3f98ea69746ff05300cdab913d227",
                "as1": "52160d61a732d209eed15e0ce6736eee23118e1c34ca5b9ec61474fe1a4011e3",
            },
        },
    },
}


def require_known(name):
    """`name` if it is a pinned compiler; SystemExit otherwise."""
    if name not in COMPILERS:
        raise SystemExit(f"unknown compiler {name!r} (known: {', '.join(sorted(COMPILERS))})")
    return name


def identify_hashes(hashes, compilers=None):
    """The compiler whose known build matches every fingerprinted file in
    `hashes` ({file: sha256}), or None."""
    compilers = COMPILERS if compilers is None else compilers
    for name, spec in sorted(compilers.items()):
        for build in spec["builds"].values():
            if all(hashes.get(f) == build[f] for f in FINGERPRINT_FILES):
                return name
    return None


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def identify_dir(path, compilers=None):
    """Compiler of an unpacked IDO directory (cc, cfe, ... at its top)."""
    path = Path(path)
    hashes = {f: _sha((path / f).read_bytes()) for f in FINGERPRINT_FILES
              if (path / f).is_file()}
    return identify_hashes(hashes, compilers)


def identify_toolkit(tar_path, compilers=None):
    """Compiler of a conveyor toolkit bundle (its `ido/` directory)."""
    hashes = {}
    with tarfile.open(tar_path) as tar:
        for f in FINGERPRINT_FILES:
            try:
                member = tar.getmember(f"ido/{f}")
            except KeyError:
                continue
            hashes[f] = _sha(tar.extractfile(member).read())
    return identify_hashes(hashes, compilers)


def identify(path, compilers=None):
    path = Path(path)
    return identify_dir(path, compilers) if path.is_dir() else identify_toolkit(path, compilers)


# --- per-segment pins -----------------------------------------------------------

def load_segment_pins(path=None):
    """{segment yaml_name (lower-case hex): compiler} from rom_compilers.json.
    Missing file = no pins. Every named compiler must be pinned here."""
    path = SEGMENT_PINS if path is None else Path(path)
    if not path.is_file():
        return {}
    data = json.loads(path.read_text())
    if data.get("default", DEFAULT) != DEFAULT:
        raise SystemExit(f"{path.name}: default must be {DEFAULT!r}")
    out = {}
    for seg, name in data.get("segments", {}).items():
        out[hex(int(seg, 16))] = require_known(name)
    return out


def segment_compiler(yaml_name, pins):
    return pins.get(hex(int(yaml_name, 16)), DEFAULT)


def lock_compiler(entry):
    """The compiler a lock entry was proven with (legacy entries: 5.3)."""
    return require_known(entry.get("compiler") or DEFAULT)


def installed_dir(name, repo=None):
    """Where `name` is installed on this box: the builder dir, else the cloud
    dir (tools/cloud/setup.sh), else None. Callers that compile a pinned TU
    use `installed_dir(segment_compiler(...)) / "cc"` instead of the 5.3 path."""
    spec = COMPILERS[require_known(name)]
    root = REPO if repo is None else Path(repo)
    for key in ("builder_dir", "cloud_dir"):
        where = root / spec[key]
        if (where / "cc").is_file():
            return where
    return None


def tu_cc(tu, default_cc, pins=None, repo=None):
    """The `cc` a research packet or test must use to rebuild ROM TU `tu`
    (`lib_1a660` or `0x1a660`): `default_cc` (the 5.3 scorer compiler) unless
    rom_compilers.json pins the segment to another compiler, which must then be
    installed on this box. Raises AssertionError with the conftest toolchain
    marker when it is not, so the test skips (or fails under REQUIRE_TOOLCHAIN)."""
    yaml_name = tu if tu.startswith("0x") else "0x" + tu[len("lib_"):]
    name = segment_compiler(yaml_name, load_segment_pins() if pins is None else pins)
    if name == DEFAULT:
        return Path(default_cc)
    where = installed_dir(name, repo)
    if where is None:
        raise AssertionError(f"{tu}: {name} compiler absent; run tools/cloud/setup.sh")
    return where / "cc"


# --- installation ---------------------------------------------------------------

def install(name, dest=None, archive=None):
    """Download (or take `archive`), verify the pinned sha256 BEFORE
    extraction, unpack into `dest` (default: the builder dir) and confirm the
    unpacked files identify as `name`. Idempotent."""
    spec = COMPILERS[require_known(name)]
    dest = Path(dest) if dest else REPO / spec["builder_dir"]
    if (dest / "cc").is_file() and identify_dir(dest) == name:
        print(f"{name} already installed: {dest}")
        return dest
    data = Path(archive).read_bytes() if archive else urllib.request.urlopen(
        spec["release"], timeout=300).read()
    got = _sha(data)
    if got != spec["archive_sha256"]:
        raise SystemExit(f"{name}: archive sha256 {got} != pinned {spec['archive_sha256']}")
    with tempfile.TemporaryDirectory() as tmp:
        with tarfile.open(fileobj=io.BytesIO(data)) as tar:
            for member in tar.getmembers():
                if not (member.isfile() or member.isdir()) or member.name.startswith(("/", "..")) \
                        or ".." in Path(member.name).parts:
                    raise SystemExit(f"{name}: refusing archive member {member.name!r}")
                if member.isfile():
                    out = Path(tmp) / member.name
                    out.parent.mkdir(parents=True, exist_ok=True)
                    out.write_bytes(tar.extractfile(member).read())
                    out.chmod(0o755 if member.mode & 0o111 else 0o644)
        if identify_dir(tmp) != name:
            raise SystemExit(f"{name}: unpacked files do not match a known {name} build")
        dest.mkdir(parents=True, exist_ok=True)
        for f in Path(tmp).iterdir():
            shutil.copy2(f, dest / f.name)
    print(f"{name} installed: {dest}")
    return dest


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("install")
    p.add_argument("name")
    p.add_argument("--dest", default=None)
    p.add_argument("--archive", default=None, help="local copy of the release archive")
    p = sub.add_parser("identify")
    p.add_argument("path")
    sub.add_parser("segments")
    args = parser.parse_args(argv)
    if args.command == "install":
        install(args.name, args.dest, args.archive)
    elif args.command == "identify":
        name = identify(args.path)
        print(name or "unknown")
        return 0 if name else 1
    else:
        for seg, name in sorted(load_segment_pins().items(), key=lambda kv: int(kv[0], 16)):
            print(f"{seg}\t{name}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
