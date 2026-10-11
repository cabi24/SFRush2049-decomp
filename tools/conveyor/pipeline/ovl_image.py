"""The two runtime images ("overlays") as bytes: stream constants and inflate.

    python3 -m tools.conveyor.pipeline.ovl_image extract [--image a|b ...]

Shared by the image path (ovl_layout / ovl_splice: layout, splice, image gate)
and the stream path (ovl_rom: recompression and ROM compose). Keep this module
small and dependency-free so both lanes can rely on the same interface:

    IMAGES[x]          the constants of image x ("a" or "b")
    inflate(x, rom)    the decompressed retail image, hash-checked
    image_path(x)      build/ovl/<x>/image.bin (where extract writes it)
    ensure(x)          image_path(x), extracted from the ROM if missing or stale

Both images are raw DEFLATE streams (windowBits -15) in the cartridge, loaded
at 0x8038A400; image A is the front end, image B the race HUD/stunts/weapons.
Inflating is deterministic for any zlib, so the stdlib suffices here; only the
compressor must be the vendored zlib 1.0.4 (tools/deflate104, used by ovl_rom).
The result is checked against the image_sha256 that ovl_targets recorded in
asm/us/ovl_<x>/extents.json, so a wrong ROM or offset is an error, never a
silently different image.
"""
import argparse
import hashlib
import json
import sys
import zlib
from pathlib import Path

from . import targets as targetsmod

REPO = targetsmod.REPO
BASEROM = targetsmod.BASEROM
BASE = 0x8038A400
STATIC_DELTA = 0x80000400 - 0x1000      # boot segment: vram -> ROM offset
WINDOW = 0x100000                       # longest raw stream we read from the ROM

IMAGES = {
    "a": {"rom_offset": 0xB5C534, "pointer": 0x8002B020, "base": BASE,
          "size": 194128, "text_end": 0x803AF978},
    "b": {"rom_offset": 0xB6FEC4, "pointer": 0x8002B024, "base": BASE,
          "size": 43888, "text_end": 0x80393B34},
}


class ImageError(RuntimeError):
    pass


def target_dir(image):
    return REPO / "asm" / "us" / f"ovl_{image}"


def image_path(image, build=None):
    return Path(build or REPO / "build") / "ovl" / image / "image.bin"


def expected_sha256(image):
    return json.loads((target_dir(image) / "extents.json").read_text())["image_sha256"]


def _check(image):
    if image not in IMAGES:
        raise ImageError(f"unknown runtime image {image!r} (expected one of {sorted(IMAGES)})")
    return IMAGES[image]


def inflate_stream(rom, offset):
    """(image bytes, compressed stream length) of the raw stream at offset."""
    z = zlib.decompressobj(-15)
    data = z.decompress(rom[offset:offset + WINDOW])
    if not z.eof:
        raise ImageError(f"raw deflate at 0x{offset:X} did not end")
    return data, min(len(rom) - offset, WINDOW) - len(z.unused_data)


def inflate(image, rom=None):
    """The retail image `image` ("a"/"b"), from `rom` bytes (default: baserom)."""
    info = _check(image)
    rom = BASEROM.read_bytes() if rom is None else rom
    pointer = int.from_bytes(rom[info["pointer"] - STATIC_DELTA:][:4], "big")
    if pointer != info["rom_offset"]:
        raise ImageError(f"image {image.upper()}: boot pointer 0x{info['pointer']:08X} "
                         f"holds 0x{pointer:X}, expected 0x{info['rom_offset']:X}")
    data, _ = inflate_stream(rom, info["rom_offset"])
    if len(data) != info["size"]:
        raise ImageError(f"image {image.upper()}: {len(data)} bytes, expected {info['size']}")
    want = expected_sha256(image)
    got = hashlib.sha256(data).hexdigest()
    if got != want:
        raise ImageError(f"image {image.upper()}: sha256 {got[:12]}... != extents.json "
                         f"{want[:12]}...")
    return data


def ensure(image, build=None, rom=None):
    """Path of the extracted image, (re)written when missing or stale."""
    _check(image)
    path = image_path(image, build)
    if path.is_file() and hashlib.sha256(path.read_bytes()).hexdigest() == expected_sha256(image):
        return path
    data = inflate(image, rom)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)
    return path


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    e = sub.add_parser("extract")
    e.add_argument("--image", action="append", choices=sorted(IMAGES))
    args = parser.parse_args()
    try:
        for image in args.image or sorted(IMAGES):
            path = ensure(image)
            print(f"image {image.upper()}: {path.stat().st_size} B -> "
                  f"{path.relative_to(REPO)} (sha256 {expected_sha256(image)[:12]}...)")
    except (ImageError, OSError) as exc:
        sys.exit(f"ovl_image: {exc}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
