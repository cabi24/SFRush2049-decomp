"""The two runtime images A/B as bytes: constants, inflate, exact recompression (R16).

    python3 -m tools.conveyor.pipeline.ovl_image check                 # constants vs the ROM
    python3 -m tools.conveyor.pipeline.ovl_image extract [--image a|b] # build/ovl/<x>/image.bin

The one module both halves of the runtime-image path import, with one set of
constants: ovl_layout / ovl_splice (layout, splice, image gate) and ovl_rom
(recompression, ROM compose).

The cartridge holds two raw-deflate code images besides the game image, both
loaded at 0x8038A400 (see ovl_targets). The boot segment holds only each
stream's ROM address (D_8002B020 / D_8002B024); nothing records a length.
The game blob and the two streams are contiguous in the data segment:

    0xB0CB10  game code   326,180 B  -> 647,072 B image   (blob_rom)
    0xB5C534  image A      80,272 B  -> 194,128 B image
    0xB6FEC4  image B      23,639 B  ->  43,888 B image
    0xB75B1B  zero fill to the end of the ROM (0xC00000)

Exact recompression: vendored zlib 1.0.4 (tools/zlib-1.0.4, the deflate half)
driven by tools/deflate104 -- level 9, windowBits -15 (raw), memLevel 8,
default strategy, one deflate(Z_FINISH) call -- reproduces both streams byte
for byte from their inflated images, exactly as it does the game blob. A sweep
of every level 0-9 x windowBits -15..-8 x memLevel 1-9 x strategy 0-2 (about
1,790 sets per image, 2026-10-11) matched that one set only. Nothing here
accepts a stream that differs from the ROM's: `prove` and `check` compare
whole streams, and `ovl_rom` refuses to compose a stream of another length.
Inflating is deterministic for any zlib, so the stdlib does it.

Interface:

    STREAMS[x] / IMAGES[x]   the constants of image x ("a" or "b"); one table
    BASE                     0x8038A400
    inflate(x, rom=None)     the retail image, read from the ROM only (never
                             build/), gated on the stream ending exactly at its
                             slot end and on the recorded size and SHA-256
    deflate(data)            exact cartridge compression (deflate104)
    image_path(x)            build/ovl/<x>/image.bin: the retail image
    ensure(x)                image_path(x), (re)extracted when missing or stale
    rebuilt_path(x)          build/ovl/<x>/rebuilt.bin: the image `ovl_layout
                             build` linked from the locked C and gated
                             byte-identical (rebuilt.json records it); this is
                             what `ovl_rom produce|rom` compresses
    inflate_stream(rom, off) (image, stream length) of a raw stream, ungated
"""
import argparse
import hashlib
import struct
import subprocess
import sys
import tempfile
import zlib
from collections import namedtuple
from pathlib import Path

from . import blob_rom, ovl_targets
from .owned_data import DATA_ROM_START

REPO = ovl_targets.REPO
BASEROM = ovl_targets.BASEROM
BASE = ovl_targets.BASE
STATIC_DELTA = ovl_targets.STATIC_DELTA   # boot segment: vram -> ROM offset
OUT = REPO / "build" / "ovl"
ROM_SIZE = 0xC00000
WINDOW = 0x100000                         # longest raw stream inflate_stream reads

# The compressor parameters (a fact about the original build, fixed in
# tools/deflate104/deflate104.c); recorded here for reports and tests.
DEFLATE = {"zlib": "1.0.4", "level": 9, "window_bits": -15, "mem_level": 8,
           "strategy": "Z_DEFAULT_STRATEGY", "flush": "single Z_FINISH"}

Stream = namedtuple("Stream", "image pointer rom_offset length image_size "
                              "image_sha256 stream_sha256 followed_by text_end",
                    defaults=(None,))

STREAMS = {
    "a": Stream("A", 0x8002B020, 0xB5C534, 80272, 194128,
                "0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667",
                "ebd4b2c19a1a49663aa28cf2a7c9040858ba9bdcfb86f35ff6a797da9805710a",
                "image B's stream", 0x803AF978),
    "b": Stream("B", 0x8002B024, 0xB6FEC4, 23639, 43888,
                "b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd",
                "0e1292c1a5b2df46a67690fc42f514f1be5598a004b89b5315ccf246885592e7",
                "zero fill to the end of the ROM", 0x80393B34),
}
IMAGES = STREAMS        # the layout/splice name for the same table


class OvlImageError(RuntimeError):
    pass


ImageError = OvlImageError


def stream(image):
    try:
        return STREAMS[image.lower()]
    except (KeyError, AttributeError):
        raise OvlImageError(f"unknown runtime image {image!r} "
                            f"(known: {', '.join(sorted(STREAMS))})") from None


def end(image):
    s = stream(image)
    return s.rom_offset + s.length


def slot(image):
    """The stream's offset inside data.bin (what compose_data's --slot takes)."""
    return stream(image).rom_offset - DATA_ROM_START


# --- paths ---------------------------------------------------------------------

def target_dir(image):
    """asm/us/ovl_<x>: the scanned targets (extents.json, symbols.json)."""
    return ovl_targets.OUT / f"ovl_{stream(image).image.lower()}"


def expected_sha256(image):
    return stream(image).image_sha256


def image_path(image, build=None):
    return Path(build or REPO / "build") / "ovl" / image / "image.bin"


def rebuilt_path(image, build=None):
    return Path(build or REPO / "build") / "ovl" / image / "rebuilt.bin"


def rebuilt_record(image, build=None):
    return rebuilt_path(image, build).with_suffix(".json")


# --- the ROM -------------------------------------------------------------------

def _rom(rom):
    if rom is None:
        if not BASEROM.is_file():
            raise OvlImageError(f"{BASEROM} is absent (the ROM is never committed)")
        return BASEROM.read_bytes()
    return Path(rom).read_bytes() if isinstance(rom, (str, Path)) else bytes(rom)


def rom_stream(image, rom=None):
    s = stream(image)
    return _rom(rom)[s.rom_offset:s.rom_offset + s.length]


def inflate_stream(rom, offset):
    """(image bytes, compressed stream length) of the raw stream at `offset`.
    No gates; `inflate` is the checked interface."""
    z = zlib.decompressobj(-15)
    try:
        data = z.decompress(rom[offset:offset + WINDOW])
    except zlib.error as exc:
        raise OvlImageError(f"raw deflate at 0x{offset:X} does not inflate: {exc}") from exc
    if not z.eof:
        raise OvlImageError(f"raw deflate at 0x{offset:X} did not end")
    return data, min(len(rom) - offset, WINDOW) - len(z.unused_data)


def inflate(image, rom=None):
    """The retail image `image` ('a' or 'b'), inflated from the ROM's stream.

    Raises OvlImageError unless the raw stream ends exactly at the slot end
    and the image has the recorded size and SHA-256."""
    s = stream(image)
    data = _rom(rom)
    z = zlib.decompressobj(-15)
    try:
        out = z.decompress(data[s.rom_offset:s.rom_offset + s.length])
    except zlib.error as exc:
        raise OvlImageError(f"image {s.image}: stream at 0x{s.rom_offset:X} "
                            f"does not inflate: {exc}") from exc
    if not z.eof or z.unused_data:
        raise OvlImageError(f"image {s.image}: stream does not end at "
                            f"0x{s.rom_offset + s.length:X}")
    if len(out) != s.image_size or hashlib.sha256(out).hexdigest() != s.image_sha256:
        raise OvlImageError(f"image {s.image}: inflated image ({len(out)} B, sha256 "
                            f"{hashlib.sha256(out).hexdigest()[:12]}...) is not the "
                            "recorded retail image")
    return out


def deflate(data):
    """`data` compressed exactly as the cartridge's streams were (deflate104)."""
    cli = blob_rom.build_compressor()
    with tempfile.TemporaryDirectory(prefix="ovlimage-") as work:
        src, dst = Path(work) / "image.bin", Path(work) / "image.deflate"
        src.write_bytes(bytes(data))
        proc = subprocess.run([str(cli), str(src), str(dst)],
                              capture_output=True, text=True)
        if proc.returncode:
            raise OvlImageError("deflate104 failed: " + proc.stderr[:300])
        return dst.read_bytes()


def prove(image, rom=None):
    """The exact-recompression proof for one image: deflate(inflate) == ROM."""
    s = stream(image)
    data = _rom(rom)
    built = deflate(inflate(image, data))
    original = rom_stream(image, data)
    first = next((i for i, (x, y) in enumerate(zip(built, original)) if x != y),
                 None if len(built) == len(original) else min(len(built), len(original)))
    return {"image": s.image, "stream_bytes": len(built), "rom_bytes": len(original),
            "matches": built == original, "first_difference": first}


def check(rom=None, recompress=True):
    """Problems with the committed constants against the ROM ([] = all good)."""
    data = _rom(rom)
    problems = []
    if len(data) != ROM_SIZE:
        problems.append(f"ROM is {len(data)} bytes, expected {ROM_SIZE}")
    if blob_rom.ROM_OFFSET + blob_rom.LENGTH != STREAMS["a"].rom_offset:
        problems.append("game blob does not end where image A's stream starts")
    for key, s in sorted(STREAMS.items()):
        pointer = struct.unpack_from(">I", data, s.pointer - STATIC_DELTA)[0]
        if pointer != s.rom_offset:
            problems.append(f"image {s.image}: D_{s.pointer:08X} holds 0x{pointer:X}, "
                            f"constant says 0x{s.rom_offset:X}")
        raw = data[s.rom_offset:s.rom_offset + s.length]
        if hashlib.sha256(raw).hexdigest() != s.stream_sha256:
            problems.append(f"image {s.image}: stream SHA-256 differs")
        try:
            inflate(key, data)
        except OvlImageError as exc:
            problems.append(str(exc))
            continue
        if recompress:
            r = prove(key, data)
            if not r["matches"]:
                problems.append(f"image {s.image}: deflate104 gives {r['stream_bytes']} B, "
                                f"first difference at +0x{r['first_difference']:X}")
    if end("a") != STREAMS["b"].rom_offset:
        problems.append("image A's stream does not end where image B's starts")
    if any(data[end("b"):]):
        problems.append(f"bytes after image B's stream (0x{end('b'):X}) are not zero fill")
    return problems


# --- extracted retail images ---------------------------------------------------

def _fresh(path, image):
    return path.is_file() and \
        hashlib.sha256(path.read_bytes()).hexdigest() == stream(image).image_sha256


def ensure(image, build=None, rom=None):
    """image_path(image), (re)written from the ROM when missing or stale."""
    path = image_path(image, build)
    if _fresh(path, image):
        return path
    data = inflate(image, rom)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)
    return path


def extract(build=None, rom=None, images=None):
    """{image: path}: the retail images under build/ovl/<x>/image.bin."""
    images = images or sorted(STREAMS)
    data = None if rom is None and all(_fresh(image_path(k, build), k) for k in images) \
        else _rom(rom)
    return {key: ensure(key, build, data) for key in images}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    c = sub.add_parser("check")
    c.add_argument("--rom", default=None)
    c.add_argument("--no-recompress", action="store_true")
    e = sub.add_parser("extract")
    e.add_argument("--rom", default=None)
    e.add_argument("--image", action="append", choices=sorted(STREAMS))
    args = parser.parse_args(argv)
    try:
        if args.command == "extract":
            for key, path in extract(rom=args.rom, images=args.image).items():
                print(f"image {key.upper()}: {path.stat().st_size} B -> {path} "
                      f"(sha256 {stream(key).image_sha256[:12]}...)")
            return 0
        problems = check(args.rom, recompress=not args.no_recompress)
    except (OvlImageError, OSError) as exc:
        print(f"ovl_image FAILED: {exc}")
        return 1
    for key, s in sorted(STREAMS.items()):
        print(f"image {s.image}: D_{s.pointer:08X} -> ROM 0x{s.rom_offset:06X}..0x{end(key):06X} "
              f"({s.length} B, data.bin slot 0x{slot(key):06X}) -> {s.image_size} B image; "
              f"followed by {s.followed_by}")
    for p in problems:
        print("  PROBLEM: " + p)
    if problems:
        print(f"ovl_image check: {len(problems)} problem(s)")
    else:
        print("ovl_image check: OK" + ("" if args.no_recompress else
                                       " (both streams recompressed byte for byte)"))
    return 0 if not problems else 1


if __name__ == "__main__":
    sys.exit(main())
