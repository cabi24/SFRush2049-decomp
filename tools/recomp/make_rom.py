#!/usr/bin/env python3
"""Rebuild the recompiler's synthetic ROM from the retail ROM, without the repo.

gen_config.py writes `rom/rush2049.recomp.z64` (the retail ROM unchanged + the
inflated game image and runtime images A/B appended at 64 KiB-aligned offsets)
next to the N64Recomp config it generates. The gameplay-test package
(tools/recomp/package.py) ships the config and its manifest.json but no
ROM-derived bytes; this script recreates the synthetic ROM on the tester's
machine from their own ROM and refuses anything that is not byte-identical to
what the coordinator built (every image's sha256 and the whole file's sha256
come from manifest.json).

    python make_rom.py --rom baserom.us.z64 --manifest config/manifest.json \
                       --out config/rom/rush2049.recomp.z64

Standard library only (Python 3.9+). The streams are raw deflate (zlib
window bits -15), as tools/conveyor/pipeline/ovl_image.py inflates them.
"""
import argparse
import hashlib
import json
import sys
import zlib
from pathlib import Path


class RomError(RuntimeError):
    pass


def sha1(data):
    return hashlib.sha1(data).hexdigest()


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def inflate_at(rom, offset):
    z = zlib.decompressobj(-15)
    try:
        out = z.decompress(rom[offset:])
    except zlib.error as exc:
        raise RomError("no deflate stream at ROM 0x%X (%s)" % (offset, exc))
    if not z.eof:
        raise RomError("truncated deflate stream at ROM 0x%X" % offset)
    return out


def build(rom, manifest):
    """The synthetic ROM bytes for `manifest` (gen_config manifest.json)."""
    if len(rom) >= 4 and rom[:4] != b"\x80\x37\x12\x40":
        raise RomError("the ROM is not a big-endian .z64 image (first word %s); convert .n64/.v64 first"
                       % rom[:4].hex())
    want = manifest.get("rom_sha1")
    if want and sha1(rom) != want:
        raise RomError("ROM SHA-1 %s is not the US retail ROM %s" % (sha1(rom), want))
    if manifest.get("replacements", {}).get("image"):
        raise RomError("this manifest has a replacement image (a --replace-set build); "
                       "make_rom only rebuilds stock configs")
    synth = manifest["synthetic_rom"]
    sections = {s["name"].lstrip("."): s for s in manifest["sections"]}
    out = bytearray(rom)
    out.extend(b"\0" * (synth["size"] - len(out)))
    images = manifest["runtime_contract"]["images"]
    for img in images:
        sec = sections.get(img["name"])
        if sec is None:
            raise RomError("manifest has no section for image %s" % img["name"])
        data = inflate_at(rom, int(img["stream_rom"], 16))
        if len(data) != img["size"] or sha256(data) != sec["image_sha256"]:
            raise RomError("image %s inflated from ROM %s is not the expected one (%d B, sha256 %s)"
                           % (img["name"], img["stream_rom"], len(data), sha256(data)[:16]))
        at = int(img["section_rom"], 16)
        out[at:at + len(data)] = data
    static = sections.get("static")
    if static is not None:
        at = int(static["rom"], 16)
        if sha256(bytes(out[at:at + static["size"]])) != static["image_sha256"]:
            raise RomError("boot segment bytes differ from the manifest")
    if len(out) != synth["size"] or sha256(bytes(out)) != synth["sha256"]:
        raise RomError("synthetic ROM sha256 %s differs from the manifest's %s"
                       % (sha256(bytes(out))[:16], synth["sha256"][:16]))
    return bytes(out)


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--rom", required=True, help="retail San Francisco Rush 2049 (US) ROM, .z64")
    p.add_argument("--manifest", required=True, help="manifest.json of the generated config")
    p.add_argument("--out", required=True, help="where to write the synthetic ROM")
    args = p.parse_args(argv)
    try:
        rom = Path(args.rom).read_bytes()
        manifest = json.loads(Path(args.manifest).read_text())
        data = build(rom, manifest)
    except (OSError, ValueError, KeyError, RomError) as exc:
        print("make_rom: %s" % exc, file=sys.stderr)
        return 2
    out = Path(args.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(data)
    print("make_rom: wrote %s (%d bytes, sha256 %s) = manifest synthetic ROM" % (out, len(data), sha256(data)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
