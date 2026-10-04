"""Fit a self-contained boot-tail body into a ROM-aligned TU context (builder).

Converted ROM TUs begin with `#include "rom_tu.h"`, which is context-pinned and
cannot change. A cloud/matches/boot_tail body is self-contained C89 whose own
declarations sometimes repeat something rom_tu.h already declares (`typedef
unsigned char u8;`, `typedef struct OSMesgQueue_s OSMesgQueue;`, an SDK
prototype); IDO rejects those as redeclarations.

For each function this script, with the Makefile's ROM-TU compiler and flags:

1. compiles the body file exactly as verified (standalone) -> orig.o, and
   refuses it if the object carries any allocated data (.data/.rodata/.bss/
   .sdata/.sbss/.rdata): a promoted TU's data would shift the ROM and needs
   owned-slot work instead;
2. compiles `#include "rom_tu.h"` + the body's file-scope declarations + the
   body, dropping ONLY a declaration statement that IDO reports as a
   redeclaration of a name rom_tu.h already provides; any other error refuses;
3. requires the function's disassembly with relocations (`objdump -dr`) to be
   identical in both objects, so the rom_tu.h context cannot change codegen.

Output: JSON lines {function, status, reason, preamble[], dropped[]}.
Usage (repo root on the builder):
    python3 cloud/work/boot_tail_promotion/context_check.py OUT.jsonl fn[=source.c]...
"""
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.seeds.extract_candidates import extract_functions  # noqa: E402

ROOT = Path.cwd()
CC = ROOT / "tools/ido-static-recomp/build/out/cc"
# Makefile CFLAGS for src/rom/*.c at the -O2 default (no opt override).
CFLAGS = ["-G", "0", "-mips2", "-O2", "-non_shared", "-I" + str(ROOT / "include"),
          "-I" + str(ROOT / "include/PR"), "-D_LANGUAGE_C", "-Wab,-r4300_mul",
          "-Xcpluscomm", "-I" + str(ROOT / "src/rom")]
DATA_SECTIONS = (".data", ".rodata", ".bss", ".sdata", ".sbss", ".rdata", ".lit4", ".lit8")
REDECL = re.compile(r"line (\d+): redeclaration of '([A-Za-z_]\w*)'")


def split_statements(text):
    """Top-level declaration statements of a preamble, with their line spans.
    Comments, preprocessor lines and brace nesting are respected."""
    out, buf, depth, i, line, start_line = [], [], 0, 0, 1, None
    n = len(text)
    while i < n:
        c = text[i]
        if c == "/" and text.startswith("/*", i):
            j = text.index("*/", i + 2) + 2
            line += text.count("\n", i, j)
            i = j
            continue
        if c == "#" and depth == 0 and not "".join(buf).strip():
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append((line, line, text[i:j].strip()))
            buf = []
            i = j
            continue
        if not c.isspace() and start_line is None:
            start_line = line
        if c == "\n":
            line += 1
        buf.append(c)
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
        elif c == ";" and depth == 0:
            out.append((start_line, line, " ".join("".join(buf).split())))
            buf, start_line = [], None
        i += 1
    if "".join(buf).strip():
        raise ValueError("unterminated preamble statement")
    return out


def compile_c(src_text, obj):
    src = obj.with_suffix(".c")
    src.write_text(src_text)
    p = subprocess.run([str(CC), "-c"] + CFLAGS + ["-o", str(obj), str(src)],
                       capture_output=True, text=True)
    return p.returncode == 0, (p.stderr + p.stdout)


def data_sections(obj):
    out = subprocess.run(["mips-linux-gnu-readelf", "-SW", str(obj)],
                         capture_output=True, text=True, check=True).stdout
    found = []
    for row in out.splitlines():
        m = re.match(r"\s*\[\s*\d+\]\s+(\S+)\s+\S+\s+\S+\s+\S+\s+([0-9a-f]+)", row)
        if m and m.group(1) in DATA_SECTIONS and int(m.group(2), 16):
            found.append(f"{m.group(1)}:{int(m.group(2), 16)}")
    return found


def disasm(obj, fn):
    out = subprocess.run(["mips-linux-gnu-objdump", "-dr", "--disassemble=" + fn, str(obj)],
                         capture_output=True, text=True, check=True).stdout
    return [l.split(None, 1)[1] if ":" in l.split(None, 1)[0] else l.strip()
            for l in out.split(f"<{fn}>:", 1)[1].splitlines() if l.strip()]


def check(fn, tmp, source=None):
    path = ROOT / (source or f"cloud/matches/boot_tail/{fn}.c")
    text = path.read_text()
    (_, start, end), = list(extract_functions(text))
    body, preamble = text[start:end], text[:start]
    orig = tmp / f"{fn}.orig.o"
    ok, err = compile_c(text, orig)
    if not ok:
        return dict(status="refused", reason="standalone compile failed: " + err.strip()[:200])
    data = data_sections(orig)
    if data:
        return dict(status="refused", reason="object has data/rodata (" + ", ".join(data) + ")")
    stmts = [s for s in split_statements(preamble)]
    kept, dropped = [s[2] for s in stmts], []
    for _ in range(64):
        head = '#include "rom_tu.h"\n'
        src = head + "".join(s + "\n" for s in kept) + body + "\n"
        ctx = tmp / f"{fn}.ctx.o"
        ok, err = compile_c(src, ctx)
        if ok:
            break
        m = REDECL.search(err)
        if not m:
            first = next((l for l in err.splitlines() if "Error" in l), err.strip()[:200])
            return dict(status="refused", reason="rom_tu.h context error: " + first.strip()[:200])
        lineno, name = int(m.group(1)), m.group(2)
        # Map the error line back to a kept statement (line 1 is the include).
        cursor, victim = 2, None
        for k, s in enumerate(kept):
            span = s.count("\n") + 1
            if cursor <= lineno < cursor + span:
                victim = k
                break
            cursor += span
        if victim is None or not re.search(r"\b" + re.escape(name) + r"\b", kept[victim]):
            return dict(status="refused", reason=f"redeclaration of {name} outside the body preamble")
        dropped.append(kept.pop(victim))
    else:
        return dict(status="refused", reason="context fitting did not converge")
    a, b = disasm(orig, fn), disasm(ctx, fn)
    if a != b:
        return dict(status="refused", reason="rom_tu.h context changes codegen", dropped=dropped)
    return dict(status="ok", preamble=kept, dropped=dropped, words=sum(1 for l in a if "\t" in l and not l.startswith("R_")))


def main():
    out = Path(sys.argv[1])
    with tempfile.TemporaryDirectory() as t, out.open("w") as f:
        for arg in sys.argv[2:]:
            fn, _, source = arg.partition("=")
            try:
                row = check(fn, Path(t), source or None)
            except Exception as exc:  # recorded, never silently skipped
                row = dict(status="refused", reason=f"checker error: {exc}")
            row["function"] = fn
            row["source"] = source or f"cloud/matches/boot_tail/{fn}.c"
            f.write(json.dumps(row) + "\n")
            print(fn, row["status"], row.get("reason", ""))


if __name__ == "__main__":
    main()
