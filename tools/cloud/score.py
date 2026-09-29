#!/usr/bin/env python3
"""Score C against Rush 2049 game-code targets using only the repository.

    # one function, compiled alone (the -O2 pipeline):
    python3 tools/cloud/score.py fn  path/to/file.c  func_name [--flags "-g0 -O2 -mips2 -G 0 -non_shared"]

    # an IDO -O3 interprocedural call group (010), from a group directory
    # holding group.json + its .c files:
    python3 tools/cloud/score.py group path/to/group_dir

Targets come from asm/us/blob/*.s: every game-code function is a section
`.text.<name>` of `.word`s, i.e. the exact bytes of the retail image. The
compiled function is compared word by word. A compiled object leaves
relocation fields zero while the target has linked addresses, so fields named
by a relocation (jal targets, %hi/%lo immediates) are masked. Everything else
must be equal. "MATCH" here is strong evidence; the ROM SHA-1 check is run by
the maintainers when a match is spliced (see CloudHandoff.md).

Needs tools/cloud/setup.sh (IDO 5.3 into tools/cloud/ido/). A MIPS objdump
(mips-linux-gnu-objdump) is optional and only used to disassemble diffs.
"""
import argparse
import json
import os
import re
import shlex
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
ASM_DIR = REPO / "asm" / "us" / "blob"
IDO = Path(os.environ.get("IDO_DIR", REPO / "tools" / "cloud" / "ido"))
OBJDUMP = "mips-linux-gnu-objdump"
DEFAULT_FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"
MASKS = {"R_MIPS_26": 0xFC000000, "R_MIPS_HI16": 0xFFFF0000, "R_MIPS_LO16": 0xFFFF0000}


# --- targets ---------------------------------------------------------------

_targets = None


def targets():
    """{function name: [words]} from every region file."""
    global _targets
    if _targets is None:
        _targets = {}
        for path in sorted(ASM_DIR.glob("*.s")):
            current = None
            for line in path.read_text().splitlines():
                m = re.match(r"\.section \.text\.(\S+?),", line.strip())
                if m:
                    current = _targets.setdefault(m.group(1), [])
                    continue
                if line.strip().startswith(".section"):
                    current = None
                    continue
                m = re.match(r"\s*\.word\s+(0x[0-9A-Fa-f]+)", line)
                if m and current is not None:
                    current.append(int(m.group(1), 16))
    return _targets


# --- object helpers (a minimal ELF32 big-endian reader: no binutils needed) ---

def _run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def _elf(obj):
    data = Path(obj).read_bytes()
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2:
        raise SystemExit(f"{obj}: not a 32-bit big-endian ELF")
    shoff, = struct.unpack_from(">I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from(">HHH", data, 0x2E)
    secs = []
    for i in range(shnum):
        name, typ, flags, addr, off, size, link, info, align, entsize = struct.unpack_from(
            ">IIIIIIIIII", data, shoff + i * shentsize)
        secs.append(dict(name=name, type=typ, off=off, size=size, link=link, info=info))
    strtab = secs[shstrndx]
    for sec in secs:
        raw = data[strtab["off"] + sec["name"]:]
        sec["name"] = raw[:raw.index(b"\0")].decode()
    return data, secs


def _text_index(secs):
    return next(i for i, s in enumerate(secs) if s["name"] == ".text")


def text_words(obj):
    data, secs = _elf(obj)
    t = secs[_text_index(secs)]
    raw = data[t["off"]:t["off"] + t["size"]]
    return list(struct.unpack(f">{len(raw) // 4}I", raw))


def symbols(obj):
    """{name: .text offset} for function symbols defined in .text."""
    data, secs = _elf(obj)
    text = _text_index(secs)
    out = {}
    for sec in secs:
        if sec["type"] != 2:                      # SHT_SYMTAB
            continue
        names = secs[sec["link"]]
        for k in range(sec["size"] // 16):
            st_name, value, size, info, other, shndx = struct.unpack_from(
                ">IIIBBH", data, sec["off"] + 16 * k)
            if shndx == text and (info & 0xF) == 2:   # STT_FUNC
                raw = data[names["off"] + st_name:]
                out[raw[:raw.index(b"\0")].decode()] = value
    return out


def reloc_masks(obj):
    """{.text offset: mask} for every relocation applied to .text."""
    data, secs = _elf(obj)
    text = _text_index(secs)
    rtypes = {4: "R_MIPS_26", 5: "R_MIPS_HI16", 6: "R_MIPS_LO16"}
    masks = {}
    for sec in secs:
        if sec["type"] != 9 or sec["info"] != text:   # SHT_REL for .text
            continue
        for k in range(sec["size"] // 8):
            offset, info = struct.unpack_from(">II", data, sec["off"] + 8 * k)
            masks[offset] = MASKS.get(rtypes.get(info & 0xFF, ""), 0xFFFFFFFF)
    return masks


def disasm_word(word):
    """Best-effort disassembly for display (any MIPS objdump), else blank."""
    for tool in (os.environ.get("MIPS_OBJDUMP"), OBJDUMP, "mips64-elf-objdump",
                 str(REPO / "tools" / "cloud" / "objdump")):
        if not tool:
            continue
        try:
            with tempfile.NamedTemporaryFile(suffix=".bin") as f:
                f.write(struct.pack(">I", word))
                f.flush()
                out = _run([tool, "-D", "-b", "binary", "-m", "mips:4300", "-EB", f.name]).stdout
        except OSError:
            continue
        for line in out.splitlines():
            m = re.match(r"\s*0:\s+[0-9a-f]{8}\s+(.*)", line)
            if m:
                return re.sub(r"\s+", " ", m.group(1))
    return ""


def compare(obj, name, start=None, show=12):
    """(differing words, total) for function `name` in `obj` vs its target."""
    want = targets().get(name)
    if want is None:
        raise SystemExit(f"no target section .text.{name} in {ASM_DIR}")
    words = text_words(obj)
    if start is None:
        start = symbols(obj).get(name)
        if start is None:
            raise SystemExit(f"{name} is not a defined function in the compiled object")
    masks = reloc_masks(obj)
    got = words[start // 4:start // 4 + len(want)]
    bad = []
    for i, w in enumerate(want):
        g = got[i] if i < len(got) else None
        mask = masks.get(start + 4 * i, 0xFFFFFFFF)
        if g is None or (g & mask) != (w & mask):
            bad.append(i)
    for i in bad[:show]:
        g = got[i] if i < len(got) else None
        print(f"    +0x{4*i:03x}  want {w_(want[i])}  got {w_(g) if g is not None else '(missing)'}")
    return len(bad), len(want)


def w_(word):
    return f"{word:08x} {disasm_word(word):28s}"


# --- builds ---------------------------------------------------------------------

def ido(tool):
    path = IDO / tool
    if not path.exists():
        raise SystemExit(f"{path} missing: run tools/cloud/setup.sh (or set IDO_DIR)")
    return str(path)


def compile_single(source, flags, out):
    proc = _run([ido("cc"), "-c", *shlex.split(flags), "-o", str(out), str(source)])
    if proc.returncode != 0 or not Path(out).exists():
        raise SystemExit("IDO compile failed:\n" + (proc.stderr or proc.stdout)[:3000])


def compile_group(group_dir, out):
    """Whole-program -O3 with uld -kp <keep list>: the build the ROM used
    (specs/010-ipa-call-groups/research/s2-s5-spikes.md)."""
    spec = json.loads((group_dir / "group.json").read_text())
    work = Path(tempfile.mkdtemp(prefix="grp-"))
    for name in spec["files"]:
        (work / name).write_text((group_dir / name).read_text())
    (work / "keep.txt").write_text("".join(k + "\n" for k in spec["keep"]))
    flags = shlex.split(spec["flags"])
    units = [re.sub(r"\.c$", ".u", f) for f in spec["files"]]
    common = ["-mips2", "-EB", "-g0", "-O3"]
    steps = [
        [ido("cc"), "-j", *flags, *spec["files"]],
        [ido("uld"), "-L/usr/lib/mips2/nonshared", "-_SYSTYPE_SVR4", "-mips2", "-non_shared",
         "-g0", "-no_AutoGnum", "-kp", "keep.txt", *units, "-ko", "linked"],
        [ido("usplit"), "-mips2", "-o", "split", "-t", "st", "linked"],
        [ido("umerge"), "-Olimit", "5000", *common, "split", "-o", "merged", "-t", "st"],
        [ido("uopt"), "-G", "0", "-Olimit", "5000", *common, "merged", "opt", "-t", "st", "optlog"],
        [ido("ugen"), "-G", "0", *common, "opt", "-o", "gen", "-t", "st", "-temp", "ugtmp"],
        [ido("as1"), "-elf", "-G", "0", "-p0", *common, "-Olimit", "5000", "gen", "-o", str(out),
         "-t", "st"],
    ]
    for step in steps:
        proc = _run(step, cwd=work)
        if proc.returncode != 0:
            raise SystemExit(f"{Path(step[0]).name} failed:\n" + (proc.stderr or proc.stdout)[:3000])
    return spec


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="mode", required=True)
    f = sub.add_parser("fn")
    f.add_argument("source")
    f.add_argument("name")
    f.add_argument("--flags", default=DEFAULT_FLAGS)
    g = sub.add_parser("group")
    g.add_argument("group_dir")
    args = parser.parse_args()

    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "out.o"
        if args.mode == "fn":
            compile_single(args.source, args.flags, obj)
            names = [args.name]
        else:
            spec = compile_group(Path(args.group_dir), obj)
            names = spec["members"] + spec.get("context", [])
        all_match = True
        for name in names:
            print(f"{name}:")
            bad, total = compare(obj, name)
            print(f"  {'MATCH' if bad == 0 else f'{bad}/{total} words differ'}")
            all_match &= bad == 0
    return 0 if all_match else 1


if __name__ == "__main__":
    sys.exit(main())
