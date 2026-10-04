#!/usr/bin/env python3
"""Compose the ROM's `data` segment with the game-code blob built from sources.

    compose_data.py <data.bin> <game_code.deflate> <out> --slot OFF --length N

`data.bin` is the extracted data segment (ROM 0x283D0-0xC00000). The game-code
blob occupies [slot, slot+length) inside it. The output is data.bin's bytes
before the slot, the blob built by `pipeline.blob_rom`, then data.bin's bytes
after it — the original compressed bytes in the slot are never read.

The blob must be exactly `length` bytes: anything else would shift every later
byte of the ROM, which is out of scope and must fail here, loudly, rather than
surface as an unexplained SHA-1 mismatch (009 SC-004).
"""
import argparse
import sys
from pathlib import Path


def compose(data, blob, slot, length):
    if len(blob) != length:
        raise ValueError(
            f"game-code blob is {len(blob)} bytes; the cartridge slot holds "
            f"exactly {length}. A different length would shift the rest of "
            f"the ROM — refusing.")
    if slot < 0 or slot + length > len(data):
        raise ValueError(f"slot [{slot:#x}, {slot + length:#x}) is outside the "
                         f"{len(data)}-byte data segment")
    return data[:slot] + blob + data[slot + length:]


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("data")
    parser.add_argument("blob")
    parser.add_argument("out")
    parser.add_argument("--slot", type=lambda v: int(v, 0), required=True)
    parser.add_argument("--length", type=lambda v: int(v, 0), required=True)
    args = parser.parse_args(argv)
    try:
        composed = compose(Path(args.data).read_bytes(),
                           Path(args.blob).read_bytes(), args.slot, args.length)
    except (OSError, ValueError) as exc:
        sys.exit(f"compose_data: {exc}")
    Path(args.out).write_bytes(composed)
    return 0


if __name__ == "__main__":
    sys.exit(main())
