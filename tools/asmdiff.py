#!/usr/bin/env python3
"""
Local decomp.me-like tool for comparing assembly
Usage: ./asmdiff.py <function_dir> [source.c]

Example:
  ./asmdiff.py ../permuter/__isnan
  ./asmdiff.py ../permuter/__isnan mytest.c

Watch mode (auto-reload on file change):
  ./asmdiff.py ../permuter/__isnan --watch
"""

import subprocess
import sys
import os
import tempfile
import difflib
import re
import time

# Colors
RED = "\033[91m"
GREEN = "\033[92m"
YELLOW = "\033[93m"
CYAN = "\033[96m"
RESET = "\033[0m"
BOLD = "\033[1m"
DIM = "\033[2m"

EMPTY = " " * 35

def normalize_asm(line):
    """Normalize assembly for comparison"""
    line = line.strip()
    line = re.sub(r"[0-9a-f]+ <[^>]+>", "LABEL", line)
    line = re.sub(r"\.L[0-9a-fA-F]+", "LABEL", line)
    line = re.sub(r"\$", "", line)
    line = re.sub(r"0x([0-9a-fA-F]+)", lambda m: hex(int(m.group(1), 16)), line)
    return line

def compile_and_disasm(source_file, func_name):
    """Compile C and return disassembly"""
    ido = "/home/cburnes/projects/rush2049-decomp/tools/ido-static-recomp/build/out/cc"
    flags = "-O2 -mips2 -g0 -G 0 -non_shared"

    with tempfile.NamedTemporaryFile(suffix=".o", delete=False) as tmp:
        tmp_o = tmp.name

    try:
        cmd = f"{ido} {flags} -c -o {tmp_o} {source_file} 2>&1"
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        if result.returncode != 0:
            return None, result.stderr

        cmd = f"mips-linux-gnu-objdump -d -M no-aliases {tmp_o}"
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)

        lines = result.stdout.split("\n")
        in_func = False
        asm_lines = []
        for line in lines:
            if f"<{func_name}>:" in line:
                in_func = True
                continue
            if in_func:
                if line.strip() == "" and asm_lines:
                    break
                if line.strip() and not line.startswith(" ") and ":" in line:
                    break
                match = re.match(r"\s*[0-9a-f]+:\s+[0-9a-f]+\s+(.+)", line)
                if match:
                    instr = match.group(1).strip()
                    asm_lines.append(instr)

        return asm_lines, None
    finally:
        if os.path.exists(tmp_o):
            os.unlink(tmp_o)

def load_target(func_dir, func_name):
    """Load target assembly from target.s"""
    target_s = os.path.join(func_dir, "target.s")
    if not os.path.exists(target_s):
        return None, f"No target.s found in {func_dir}"

    asm_lines = []
    with open(target_s) as f:
        in_func = False
        for line in f:
            line = line.strip()
            if func_name + ":" in line:
                in_func = True
                continue
            if in_func:
                if line.startswith(".L"):
                    continue
                if line.startswith("."):
                    continue
                if line.startswith("#"):
                    continue
                if not line:
                    continue
                if ":" in line:
                    continue
                asm_lines.append(line)

    return asm_lines, None

def show_diff(target, current, func_name):
    """Show side-by-side diff with colors"""

    t_norm = [normalize_asm(l) for l in target]
    c_norm = [normalize_asm(l) for l in current]

    matcher = difflib.SequenceMatcher(None, t_norm, c_norm)

    sep = "=" * 78
    dash = "-" * 78

    print(f"\n{BOLD}{sep}{RESET}")
    print(f"{BOLD}Function: {CYAN}{func_name}{RESET}")
    print(f"{BOLD}{sep}{RESET}")
    print(f"  # {CYAN}TARGET (original){RESET}                    {YELLOW}CURRENT (compiled){RESET}")
    print(dash)

    total = 0

    for tag, i1, i2, j1, j2 in matcher.get_opcodes():
        if tag == "equal":
            for i in range(i2 - i1):
                t_line = target[i1 + i][:35]
                c_line = current[j1 + i][:35]
                print(f"{GREEN}  {total:2} {t_line:<35}  {c_line:<35}{RESET}")
                total += 1
        elif tag == "replace":
            for i in range(max(i2 - i1, j2 - j1)):
                t_line = target[i1 + i][:35] if i1 + i < i2 else ""
                c_line = current[j1 + i][:35] if j1 + i < j2 else ""
                t_col = RED if t_line else ""
                c_col = YELLOW if c_line else ""
                print(f"! {total:2} {t_col}{t_line:<35}{RESET}  {c_col}{c_line:<35}{RESET}")
                total += 1
        elif tag == "delete":
            for i in range(i2 - i1):
                t_line = target[i1 + i][:35]
                print(f"{RED}- {total:2} {t_line:<35}  {EMPTY}{RESET}")
                total += 1
        elif tag == "insert":
            for i in range(j2 - j1):
                c_line = current[j1 + i][:35]
                print(f"{YELLOW}+ {total:2} {EMPTY}  {c_line:<35}{RESET}")
                total += 1

    print(dash)

    ratio = matcher.ratio()
    score = int((1 - ratio) * 1000)
    pct = ratio * 100

    score_color = GREEN if score == 0 else YELLOW if score < 200 else RED

    print(f"Target: {len(target)} | Current: {len(current)} | Match: {pct:.1f}%")
    print(f"Score: {score_color}{BOLD}{score}{RESET} (0 = perfect match)\n")

    return score

def watch_mode(func_dir, source_file, func_name):
    """Watch file and recompile on changes"""
    print(f"{CYAN}Watch mode: monitoring {source_file}{RESET}")
    print(f"{DIM}Press Ctrl+C to exit{RESET}\n")

    last_mtime = 0

    while True:
        try:
            mtime = os.path.getmtime(source_file)
            if mtime != last_mtime:
                last_mtime = mtime
                os.system("clear")

                target, err = load_target(func_dir, func_name)
                if err:
                    print(f"{RED}{err}{RESET}")
                    continue

                current, err = compile_and_disasm(source_file, func_name)
                if err:
                    print(f"{RED}Compile error:{RESET}\n{err}")
                    continue

                show_diff(target, current, func_name)
                print(f"{DIM}Watching {source_file}... (Ctrl+C to exit){RESET}")

            time.sleep(0.5)
        except KeyboardInterrupt:
            print(f"\n{CYAN}Exiting watch mode{RESET}")
            break

def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    func_dir = sys.argv[1]
    watch = "--watch" in sys.argv or "-w" in sys.argv

    args = [a for a in sys.argv[2:] if not a.startswith("-")]
    source_file = args[0] if args else os.path.join(func_dir, "base.c")

    func_name = os.path.basename(func_dir.rstrip("/"))

    if watch:
        target, err = load_target(func_dir, func_name)
        if err:
            print(f"{RED}{err}{RESET}")
            sys.exit(1)
        watch_mode(func_dir, source_file, func_name)
    else:
        print(f"Comparing {CYAN}{func_name}{RESET} from {source_file}...")

        target, err = load_target(func_dir, func_name)
        if err:
            print(f"{RED}{err}{RESET}")
            sys.exit(1)

        current, err = compile_and_disasm(source_file, func_name)
        if err:
            print(f"{RED}Compile error:{RESET}\n{err}")
            sys.exit(1)

        show_diff(target, current, func_name)

if __name__ == "__main__":
    main()
