"""Mark an IDO `-mips3 -32` object as 32-bit-mode o32 code (EF_MIPS_32BITMODE).

    python3 -m tools.conveyor.pipeline.o32abi OBJECT --cflags "<the TU's CFLAGS>"

IDO compiles `-mips3 -32` (ultralib's flags for libc/ll.c, llcvt.c) to an
ELF32 object whose e_flags say MIPS III without EF_MIPS_32BITMODE (0x100).
GNU ld then refuses to link it beside the -mips2 objects ("linking 32-bit
code with 64-bit code"). The code is o32 32-bit-mode code, so the flag is
correct; OoT's tools/set_o32abi_bit.py applies the same fix.

Fail-closed: a TU whose CFLAGS lack `-mips3` and `-32` is left untouched
(exit 0, no write); a matching TU whose object is not a big-endian ELF32 MIPS
relocatable at MIPS III is refused (exit 1). Only the four e_flags bytes change.
"""
import argparse
import shlex
import struct
import sys
from pathlib import Path

EF_MIPS_32BITMODE = 0x100
EF_MIPS_ARCH = 0xF0000000
EF_MIPS_ARCH_3 = 0x20000000
E_FLAGS_OFFSET = 36  # ELF32: e_ident[16] e_type e_machine e_version e_entry e_phoff e_shoff
EM_MIPS = 8
ET_REL = 1


def wants_32bitmode(cflags):
    """True iff the compile flags select IDO's `-mips3 -32` mode."""
    tokens = shlex.split(cflags) if isinstance(cflags, str) else list(cflags)
    return "-mips3" in tokens and "-32" in tokens


def set_32bitmode(path):
    """Set EF_MIPS_32BITMODE in place. Returns (old_flags, new_flags)."""
    path = Path(path)
    data = bytearray(path.read_bytes())
    if len(data) < 52 or data[:4] != b"\x7fELF":
        raise ValueError(f"{path}: not an ELF file")
    if data[4] != 1 or data[5] != 2:
        raise ValueError(f"{path}: not a big-endian ELF32 object")
    e_type, e_machine = struct.unpack_from(">HH", data, 16)
    if e_machine != EM_MIPS or e_type != ET_REL:
        raise ValueError(f"{path}: not a MIPS relocatable (type {e_type}, machine {e_machine})")
    (flags,) = struct.unpack_from(">I", data, E_FLAGS_OFFSET)
    if flags & EF_MIPS_ARCH != EF_MIPS_ARCH_3:
        raise ValueError(f"{path}: e_flags 0x{flags:08x} is not MIPS III; refusing to mark it")
    new = flags | EF_MIPS_32BITMODE
    if new != flags:
        struct.pack_into(">I", data, E_FLAGS_OFFSET, new)
        path.write_bytes(bytes(data))
    return flags, new


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("object")
    parser.add_argument("--cflags", required=True,
                        help="the TU's effective CFLAGS (one string)")
    args = parser.parse_args(argv)
    if not wants_32bitmode(args.cflags):
        return 0
    try:
        set_32bitmode(args.object)
    except (OSError, ValueError) as exc:
        print(f"o32abi: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
