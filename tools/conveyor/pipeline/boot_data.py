"""The boot segment past the static code, labelled in place (ROM 0x283D0-0x2F4E0).

    python3 -m tools.conveyor.pipeline.boot_data report
    python3 -m tools.conveyor.pipeline.boot_data asm      # regenerate asm/us/boot_data/*.s (splat)
    python3 -m tools.conveyor.pipeline.boot_data bin ELF OUT --source NAME   # Makefile step

splat ends the static code at ROM 0x283D0 (RAM 0x800277D0) because the function
scanner (pipeline.ovl_targets) stops on hand-written code it cannot parse; the
rest of the boot segment, up to the entrypoint's BSS clear at 0x8002E8E0, is the
first 28,944 B of the ROM `data` bin. It holds three kinds of bytes:

  cpu       ten hand-written R4300 routines: nine libgsm short-term/RPE/long-term
            synthesis kernels called by lib_26f60's decoder, and a D-cache
            create-dirty-exclusive loop called from six static functions;
  rsp_text  RSP microcode text (rspboot, a microcode no task names, F3DEX NoN
            fifo 2.08, aspMain), all linked .text before C .data;
  rsp_data  the F3DEX and aspMain data blocks the two tasks point at;
  data      the segment's C .data/.rodata (from __osScCurAudioTask at 0x8002AFA0).

The bytes stay in assets/us/data.bin (110 research receipts pin its sha256),
but the cpu routines are built from source: `boot_data asm` writes their
assembly (splat's disassembly, inline tables as data) to asm/us/boot_data/,
and the Makefile assembles each file and composes it into its data.bin slot
(compose_data --extra, like the game blob and runtime images), so the
extracted bytes there are never read and the full-ROM SHA-1 judges the
assembly. They are static coverage "from assembly" (asm_matched.lock.json).
tests/conveyor/test_boot_data.py re-proves every label against the ROM.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
ASM_DIR = REPO / "asm" / "us" / "boot_data"

ROM_START = 0x283D0
VRAM_START = 0x800277D0
VRAM_END = 0x8002E8E0          # entrypoint clears BSS from here to the game image

# (vaddr, size, name, role). Sizes tile their cpu region; an inline table rides
# with the routine that reads it (0x800278BC, 0x80027978).
CPU_ROUTINES = (
    (0x800277D0, 0xF4, "func_800277D0", "GSM RPE decoding (APCM inverse quantization, grid positioning)"),
    (0x800278C4, 0xBC, "func_800278C4", "GSM long-term synthesis into the decoder state"),
    (0x80027980, 0x1C4, "func_80027980", "GSM short-term synthesis filter loop (bal from 0x80027B44)"),
    (0x80027B44, 0x154, "func_80027B44", "GSM short-term synthesis driver"),
    (0x80027C98, 0xA4, "func_80027C98", "GSM LAR coefficients pass (bal; calls a PC-relative coefficient fn)"),
    (0x80027D3C, 0x18, "func_80027D3C", "GSM LAR interpolation, first variant (tail: 0x80027D7C)"),
    (0x80027D54, 0x10, "func_80027D54", "GSM LAR interpolation, second variant (tail: 0x80027D7C)"),
    (0x80027D64, 0x18, "func_80027D64", "GSM LAR interpolation, third variant (tail: 0x80027D7C)"),
    (0x80027D7C, 0x54, "func_80027D7C", "GSM LARp_to_rp (constants 11059/20070/26112)"),
    (0x8002C5E0, 0x1C, "func_8002C5E0", "D-cache create-dirty-exclusive loop over [a0, a1)"),
)

# (start, end, kind, label): tiles VRAM_START..VRAM_END.
REGIONS = (
    (0x800277D0, 0x80027DD0, "cpu", "GSM decoder kernels (9 routines)"),
    (0x80027DD0, 0x80027EA0, "rsp_text", "rspboot (both tasks' ucode_boot; size = end - start)"),
    (0x80027EA0, 0x80028CC0, "rsp_text", "microcode no task references (probably a second audio "
                                         "microcode: a copy of aspMain's resample table ends the "
                                         "data before F3DEX's)"),
    (0x80028CC0, 0x8002A050, "rsp_text", "F3DEX NoN fifo 2.08 graphics microcode (gfx task ucode)"),
    (0x8002A050, 0x8002AFA0, "rsp_text", "aspMain audio microcode (audio task ucode)"),
    (0x8002AFA0, 0x8002C5E0, "data", "static .data/.rodata"),
    (0x8002C5E0, 0x8002C5FC, "cpu", "func_8002C5E0 (hand-written, placed among data)"),
    (0x8002C5FC, 0x8002DDF0, "data", "static .data/.rodata (ends with a copy of aspMain's 512 B resample table)"),
    (0x8002DDF0, 0x8002E210, "rsp_data", "F3DEX data (gfx task ucode_data; version string)"),
    (0x8002E210, 0x8002E440, "rsp_data", "aspMain data (audio task ucode_data)"),
    (0x8002E440, 0x8002E8E0, "data", "static .data/.rodata (build-info record, 07-17-00 MERRILL)"),
)


# The assembly the build composes into data.bin: file -> (vaddr, end). The
# Makefile's BOOT_COMPOSE_ARGS name the same slots (data.bin offset = vaddr -
# VRAM_START); tests/conveyor/test_boot_data.py holds them equal.
SOURCES = {
    "kernels.s": (0x800277D0, 0x80027DD0),
    "func_8002C5E0.s": (0x8002C5E0, 0x8002C5FC),
}
# (vaddr, size, reader): data words inside the code, read PC-relatively after
# a never-taken `bltzal $zero`. spimdisasm decodes them as (unassemblable)
# instructions, so `asm` writes them as .word.
INLINE_TABLES = ((0x800278BC, 8, 0x800277D0), (0x80027978, 8, 0x800278C4))

_WORD_LINE = re.compile(r"/\*\s+[0-9A-F]+\s+(?P<vaddr>[0-9A-F]{8})\s+(?P<word>[0-9A-F]{8})\s+\*/")


def rom_offset(vaddr):
    return vaddr - VRAM_START + ROM_START


def coverage():
    """The cpu routines: static functions living in the data bin."""
    return {"functions": len(CPU_ROUTINES), "bytes": sum(size for _, size, _, _ in CPU_ROUTINES)}


def source_of(vaddr):
    """The asm/us/boot_data file holding vaddr, or None."""
    for name, (lo, hi) in SOURCES.items():
        if lo <= vaddr < hi:
            return name
    return None


def source_words(path):
    """{vaddr: word} from a boot_data .s's `/* off vaddr word */` comments."""
    return {int(m["vaddr"], 16): int(m["word"], 16)
            for m in map(_WORD_LINE.search, Path(path).read_text().splitlines()) if m}


_BOOT_VAR = re.compile(r"^(BOOT_\w+)\s*:=\s*\$\(BOOT_DIR\)/(\S+)\.bin\s*$", re.M)
_BOOT_EXTRA = re.compile(r"--extra \$\((BOOT_\w+)\):(0x[0-9A-Fa-f]+):(0x[0-9A-Fa-f]+)")


def makefile_slots(makefile=None):
    """{source: (data.bin offset, length)} the Makefile composes."""
    text = Path(makefile or REPO / "Makefile").read_text()
    names = {var: stem + ".s" for var, stem in _BOOT_VAR.findall(text)}
    return {names[var]: (int(off, 16), int(length, 16))
            for var, off, length in _BOOT_EXTRA.findall(text) if var in names}


def expected_slots():
    return {name: (lo - VRAM_START, hi - lo) for name, (lo, hi) in SOURCES.items()}


def tables_as_data(text, lo=VRAM_START, hi=VRAM_END):
    """splat's disassembly of [lo, hi) with its INLINE_TABLES rewritten as .word data."""
    for vaddr, size, reader in INLINE_TABLES:
        if not lo <= vaddr < hi:
            continue
        func, data = f"func_{vaddr:08X}", f"D_{vaddr:08X}"
        block = re.compile(rf"(?:/\* Handwritten function \*/\n)?nonmatching {func}, 0x{size:X}\n\n"
                           rf"glabel {func}\n(?P<body>.*?)endlabel {func}\n", re.S)
        m = block.search(text)
        if m is None:
            raise ValueError(f"no splat block for the inline table at 0x{vaddr:08X}")
        lines = []
        for line in m["body"].splitlines():
            w = _WORD_LINE.search(line)
            lines.append(f"    {w.group(0)}  .word 0x{w['word']}")
        repl = (f"/* inline table, read PC-relatively by func_{reader:08X} (bltzal $zero) */\n"
                f"dlabel {data}\n" + "\n".join(lines) + f"\nenddlabel {data}\n")
        text = text[:m.start()] + repl + text[m.end():]
        text = re.sub(rf"\b{func}\b", data, text)
    return text


def generate_asm(out_dir=ASM_DIR, splat_python=None):
    """Write asm/us/boot_data/*.s: splat (spimdisasm) disassembles the ROM ranges
    in a scratch copy of the config; the repo's own files are not touched."""
    splat_python = splat_python or os.environ.get(
        "SPLAT_PYTHON", str(Path.home() / ".splat-venv" / "bin" / "python"))
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        for name in ("splat.us.yaml", "symbol_addrs.us.txt", "reloc_addrs.us.txt"):
            shutil.copy(REPO / name, tmp / name)
        (tmp / "baserom.us.z64").symlink_to(REPO / "baserom.us.z64")
        yaml = (tmp / "splat.us.yaml").read_text()
        head = "  - type: bin\n    start: 0x283D0\n    name: data\n"
        kern_end, c5e0, c5e0_end = (rom_offset(SOURCES["kernels.s"][1]), rom_offset(0x8002C5E0),
                                    rom_offset(SOURCES["func_8002C5E0.s"][1]))
        if yaml.count(head) != 1:
            raise ValueError("splat.us.yaml: the data bin no longer starts at 0x283D0")
        yaml = yaml.replace("      - [0x26F60, c, rom/lib_26f60]\n",
                            "      - [0x26F60, c, rom/lib_26f60]\n      - [0x283D0, asm]\n")
        yaml = yaml.replace(head, f"  - type: bin\n    start: 0x{kern_end:X}\n    name: data0\n"
                                  f"  - name: c5e0\n    type: code\n    start: 0x{c5e0:X}\n"
                                  f"    vram: 0x8002C5E0\n    subsegments:\n      - [0x{c5e0:X}, asm]\n"
                                  f"  - type: bin\n    start: 0x{c5e0_end:X}\n    name: data\n")
        (tmp / "splat.us.yaml").write_text(yaml)
        # splat 0.41 accepts one ';' per definition line; the copy only.
        lines = []
        for line in (tmp / "symbol_addrs.us.txt").read_text().splitlines():
            if not line.strip().startswith("//") and line.count(";") > 1:
                head_, _, rest = line.partition(";")
                line = head_ + ";" + rest.replace(";", ",")
            lines.append(line)
        lines += [f"{name} = 0x{v:08X}; // type:func" for v, _, name, _ in CPU_ROUTINES]
        (tmp / "symbol_addrs.us.txt").write_text("\n".join(lines) + "\n")
        subprocess.run([splat_python, "-m", "splat", "split", "splat.us.yaml"], cwd=tmp,
                       check=True, capture_output=True)
        out_dir = Path(out_dir)
        out_dir.mkdir(parents=True, exist_ok=True)
        header = ("/* Boot segment tail, hand-written: generated by `pipeline.boot_data asm` from splat's\n"
                  " * disassembly; the Makefile assembles it and composes it into its assets/us/data.bin\n"
                  " * slot. Regenerate rather than hand-edit. */\n")
        for name, src in (("kernels.s", f"{rom_offset(0x800277D0):X}.s"), ("func_8002C5E0.s", f"{c5e0:X}.s")):
            text = (tmp / "asm" / "us" / src).read_text()
            (out_dir / name).write_text(header + tables_as_data(text, *SOURCES[name]))
    return sorted(out_dir.glob("*.s"))


def report():
    for start, end, kind, label in REGIONS:
        print(f"0x{start:08X}-0x{end:08X}  ROM 0x{rom_offset(start):05X}  {end - start:6d} B  "
              f"{kind:8s}  {label}")
    cov = coverage()
    print(f"cpu routines: {cov['functions']} ({cov['bytes']} B), counted as static functions")


def slot_bytes(elf, source, objcopy="mips-linux-gnu-objcopy"):
    """The assembled source's exact slot bytes: its .text, whose bytes past the
    slot must be the assembler's zero padding (a nonzero byte means the source
    grew past its slot, which must fail, not be cut off)."""
    with tempfile.TemporaryDirectory() as tmp:
        out = Path(tmp) / "text.bin"
        subprocess.run([objcopy, "-O", "binary", "-j", ".text", str(elf), str(out)], check=True)
        return trim_to_slot(out.read_bytes(), source)


def trim_to_slot(data, source):
    lo, hi = SOURCES[source]
    if len(data) < hi - lo or any(data[hi - lo:]):
        raise ValueError(f"{source}: assembled {len(data)} bytes; its slot holds exactly {hi - lo}")
    return data[:hi - lo]


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("command", choices=["report", "asm", "bin"])
    parser.add_argument("elf", nargs="?", help="bin: the linked assembly")
    parser.add_argument("out", nargs="?", help="bin: the slot bytes to write")
    parser.add_argument("--source", choices=sorted(SOURCES), help="bin: which boot_data file")
    parser.add_argument("--objcopy", default="mips-linux-gnu-objcopy")
    args = parser.parse_args(argv)
    if args.command == "bin":
        if not (args.elf and args.out and args.source):
            parser.error("bin needs ELF OUT --source NAME")
        Path(args.out).write_bytes(slot_bytes(args.elf, args.source, args.objcopy))
        return 0
    if args.command == "asm":
        for path in generate_asm():
            print(f"wrote {path.relative_to(REPO)}")
        return 0
    report()
    return 0


if __name__ == "__main__":
    sys.exit(main())
