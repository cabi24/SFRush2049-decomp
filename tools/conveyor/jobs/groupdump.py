#!/usr/bin/env python3
"""objdump stand-in for group searches.

    groupdump.py SPEC.json FILE

The decomp-permuter scores a candidate by running an "objdump command" on its
object and diffing the disassembly against the target's. For a whole-program
group the candidate object holds every function of the group, and only one is
being matched, so this prints the disassembly of just that function, as linked
into the image (calls and data addresses resolved). FILE is either

  * the target: a stub with a bare ELF header, then the retail words (e_type 0);
  * a candidate group object (ELF relocatable, e_type 1).

SPEC.json: {"target", "size", "vaddr", "retail" (hex), "slices" {fn: vaddr},
"symbols" {name: address}}.
"""
import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))

import group_elf  # noqa: E402

STUB_HEADER = 52


def stub_bytes(words):
    """The target file: a minimal ELF header (so the permuter accepts it) + words."""
    import struct
    header = b"\x7fELF" + bytes([1, 2, 1, 0]) + b"\0" * 8 + struct.pack(
        ">HHIIIIIHHHHHH", 0, 8, 1, 0, 0, 0, 0, STUB_HEADER, 0, 0, 40, 0, 0)
    assert len(header) == STUB_HEADER
    return header + words


def words_of(spec, data):
    if int.from_bytes(data[16:18], "big") == 0:            # the target stub
        return data[STUB_HEADER:]
    obj = group_elf.Obj(data)
    return group_elf.target_words(
        obj, spec["target"], spec["size"], spec["slices"], spec["symbols"],
        bytes.fromhex(spec["retail"]))


def objdump_path():
    import scoring
    return scoring._objdump_path()


def main(argv):
    spec = json.loads(Path(argv[1]).read_text())
    words = words_of(spec, Path(argv[2]).read_bytes())
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as f:
        f.write(words)
        raw = f.name
    try:
        sys.stdout.flush()
        return subprocess.call(
            [objdump_path(), "-D", "-b", "binary", "-m", "mips:4300", "-EB", "-z",
             f"--adjust-vma=0x{spec['vaddr']:08x}", raw])
    finally:
        os.unlink(raw)


if __name__ == "__main__":
    sys.exit(main(sys.argv))
