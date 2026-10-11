#!/usr/bin/env python3
"""Compose the ROM's `data` segment with the game-code blob built from sources.

    compose_data.py <data.bin> <game_code.deflate> <out> --slot OFF --length N
                    [--extra STREAM:OFF:N ...]

`data.bin` is the extracted data segment (ROM 0x283D0-0xC00000). The game-code
blob occupies [slot, slot+length) inside it. The output is data.bin's bytes
before the slot, the blob built by `pipeline.blob_rom`, then data.bin's bytes
after it — the original compressed bytes in the slot are never read.

Each `--extra` composes one more stream the same way (the runtime images A/B,
`pipeline.ovl_rom`). Slots may not overlap.

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


def compose_all(data, streams):
    """`streams` is [(blob, slot, length), ...]; refuses overlapping slots."""
    spans = sorted((slot, slot + length) for _, slot, length in streams)
    for (_, end), (start, _) in zip(spans, spans[1:]):
        if start < end:
            raise ValueError(f"slots overlap at {start:#x} — refusing")
    for blob, slot, length in streams:
        data = compose(data, blob, slot, length)
    return data


def _extra(value):
    path, slot, length = value.rsplit(":", 2)
    return path, int(slot, 0), int(length, 0)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("data")
    parser.add_argument("blob")
    parser.add_argument("out")
    parser.add_argument("--slot", type=lambda v: int(v, 0), required=True)
    parser.add_argument("--length", type=lambda v: int(v, 0), required=True)
    parser.add_argument("--extra", type=_extra, action="append", default=[],
                        metavar="STREAM:SLOT:LENGTH")
    args = parser.parse_args(argv)
    try:
        streams = [(Path(args.blob).read_bytes(), args.slot, args.length)]
        streams += [(Path(p).read_bytes(), s, n) for p, s, n in args.extra]
        composed = compose_all(Path(args.data).read_bytes(), streams)
    except (OSError, ValueError) as exc:
        sys.exit(f"compose_data: {exc}")
    Path(args.out).write_bytes(composed)
    return 0


if __name__ == "__main__":
    sys.exit(main())
