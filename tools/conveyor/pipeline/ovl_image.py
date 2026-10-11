"""The two runtime-image streams: constants, inflate, exact recompression (R16).

    python3 -m tools.conveyor.pipeline.ovl_image check     # constants vs the ROM
    python3 -m tools.conveyor.pipeline.ovl_image extract   # build/ovl/ovl_<x>.bin

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

Interface for the layout/splice lane: `STREAMS`, `BASE`, `inflate(image)` and
`deflate(data)`. `inflate` reads only the ROM (never build/ outputs) and
checks the stream ends exactly at its slot end and the image's SHA-256.
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
OUT = REPO / "build" / "ovl"
ROM_SIZE = 0xC00000

# The compressor parameters (a fact about the original build, fixed in
# tools/deflate104/deflate104.c); recorded here for reports and tests.
DEFLATE = {"zlib": "1.0.4", "level": 9, "window_bits": -15, "mem_level": 8,
           "strategy": "Z_DEFAULT_STRATEGY", "flush": "single Z_FINISH"}

Stream = namedtuple("Stream", "image pointer rom_offset length image_size "
                              "image_sha256 stream_sha256 followed_by")

STREAMS = {
    "a": Stream("A", 0x8002B020, 0xB5C534, 80272, 194128,
                "0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667",
                "ebd4b2c19a1a49663aa28cf2a7c9040858ba9bdcfb86f35ff6a797da9805710a",
                "image B's stream"),
    "b": Stream("B", 0x8002B024, 0xB6FEC4, 23639, 43888,
                "b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd",
                "0e1292c1a5b2df46a67690fc42f514f1be5598a004b89b5315ccf246885592e7",
                "zero fill to the end of the ROM"),
}


class OvlImageError(RuntimeError):
    pass


def stream(image):
    try:
        return STREAMS[image.lower()]
    except KeyError:
        raise OvlImageError(f"unknown runtime image {image!r} "
                            f"(known: {', '.join(sorted(STREAMS))})") from None


def end(image):
    s = stream(image)
    return s.rom_offset + s.length


def slot(image):
    """The stream's offset inside data.bin (what compose_data's --slot takes)."""
    return stream(image).rom_offset - DATA_ROM_START


def _rom(rom):
    if rom is None:
        if not BASEROM.is_file():
            raise OvlImageError(f"{BASEROM} is absent (the ROM is never committed)")
        return BASEROM.read_bytes()
    return Path(rom).read_bytes() if isinstance(rom, (str, Path)) else bytes(rom)


def rom_stream(image, rom=None):
    s = stream(image)
    return _rom(rom)[s.rom_offset:s.rom_offset + s.length]


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
        raise OvlImageError(f"image {s.image}: inflated image ({len(out)} B) is "
                            "not the recorded retail image")
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
        pointer = struct.unpack_from(">I", data, s.pointer - ovl_targets.STATIC_DELTA)[0]
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


def extract(out=OUT, rom=None):
    """Write the retail images to build/ovl/ (a stand-in until splice exists)."""
    out = Path(out)
    out.mkdir(parents=True, exist_ok=True)
    data = _rom(rom)
    paths = {}
    for key in sorted(STREAMS):
        path = out / f"ovl_{key}.retail.bin"
        path.write_bytes(inflate(key, data))
        paths[key] = path
    return paths


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    c = sub.add_parser("check")
    c.add_argument("--rom", default=None)
    c.add_argument("--no-recompress", action="store_true")
    e = sub.add_parser("extract")
    e.add_argument("--rom", default=None)
    e.add_argument("--out", default=str(OUT))
    args = parser.parse_args(argv)
    try:
        if args.command == "extract":
            for key, path in extract(args.out, args.rom).items():
                print(f"image {key.upper()}: {path}")
            return 0
        problems = check(args.rom, recompress=not args.no_recompress)
    except OvlImageError as exc:
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
