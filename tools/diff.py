#!/usr/bin/env python3
"""
Function diff tool for N64 decompilation.
Compares the assembly of a function between the baserom and the built ROM.

Usage:
    ./tools/diff.py func_80001234
    ./tools/diff.py func_80001234 -m  # show only mismatched lines
    ./tools/diff.py func_80001234 -w  # watch mode (rebuild on changes)
"""

import argparse
import os
import re
import subprocess
import sys
from pathlib import Path

# Configuration
BUILD_DIR = "build/us"
BASEROM = "baserom.us.z64"
BUILT_ROM = f"{BUILD_DIR}/rush2049.us.z64"
ELF = f"{BUILD_DIR}/rush2049.us.elf"
MAP_FILE = f"{BUILD_DIR}/rush2049.us.map"
CROSS = os.environ.get("CROSS", "mips-linux-gnu-")
OBJDUMP = f"{CROSS}objdump"

def get_function_addresses(func_name: str) -> tuple:
    """Get the start and end addresses of a function from the map file."""
    if not os.path.exists(MAP_FILE):
        print(f"Error: Map file not found: {MAP_FILE}")
        print("Please run 'make' first to build the ROM")
        sys.exit(1)

    # Try to find function in the ELF
    try:
        result = subprocess.run(
            [OBJDUMP, "-t", ELF],
            capture_output=True, text=True, check=True
        )
        for line in result.stdout.splitlines():
            if func_name in line and ".text" in line:
                parts = line.split()
                addr = int(parts[0], 16)
                size = int(parts[4], 16) if len(parts) > 4 else 0
                return addr, addr + size
    except subprocess.CalledProcessError:
        pass

    # Fallback: extract address from function name
    match = re.match(r'func_([0-9A-Fa-f]+)', func_name)
    if match:
        addr = int(match.group(1), 16)
        # Default to 0x100 bytes if size unknown
        return addr, addr + 0x100

    print(f"Error: Could not find function '{func_name}'")
    sys.exit(1)

def vram_to_rom(vram_addr: int) -> int:
    """Convert VRAM address to ROM offset."""
    # Main code starts at ROM 0x1000, VRAM 0x80000400
    if vram_addr >= 0x80000400:
        return vram_addr - 0x80000400 + 0x1000
    return vram_addr

def disassemble_rom(rom_path: str, vram_start: int, vram_end: int) -> list:
    """Disassemble a range of bytes from a ROM file."""
    rom_start = vram_to_rom(vram_start)
    rom_end = vram_to_rom(vram_end)

    if not os.path.exists(rom_path):
        return []

    with open(rom_path, 'rb') as f:
        f.seek(rom_start)
        data = f.read(rom_end - rom_start)

    if not data:
        return []

    # Write to temp file and disassemble
    temp_bin = "/tmp/diff_func.bin"
    with open(temp_bin, 'wb') as f:
        f.write(data)

    try:
        result = subprocess.run(
            [OBJDUMP, "-D", "-b", "binary", "-m", "mips:4000",
             "-EB", "--adjust-vma", hex(vram_start), temp_bin],
            capture_output=True, text=True
        )
        lines = []
        for line in result.stdout.splitlines():
            # Parse disassembly lines
            if re.match(r'\s+[0-9a-f]+:', line):
                lines.append(line.strip())
        return lines
    finally:
        if os.path.exists(temp_bin):
            os.remove(temp_bin)

def disassemble_elf(func_name: str) -> list:
    """Disassemble a function from the built ELF."""
    if not os.path.exists(ELF):
        return []

    try:
        result = subprocess.run(
            [OBJDUMP, "-d", ELF],
            capture_output=True, text=True
        )
        in_func = False
        lines = []
        for line in result.stdout.splitlines():
            if f"<{func_name}>:" in line:
                in_func = True
                continue
            if in_func:
                if re.match(r'[0-9a-f]+ <', line):
                    break  # Next function
                if re.match(r'\s+[0-9a-f]+:', line):
                    lines.append(line.strip())
        return lines
    except subprocess.CalledProcessError:
        return []

def normalize_line(line: str) -> str:
    """Normalize a disassembly line for comparison."""
    # Remove address prefix
    line = re.sub(r'^[0-9a-f]+:\s+', '', line)
    # Remove raw bytes
    line = re.sub(r'^[0-9a-f]+ \s+', '', line)
    # Normalize register names and spacing
    line = re.sub(r'\s+', ' ', line).strip()
    return line

def diff_lines(base_lines: list, built_lines: list, mismatch_only: bool = False) -> None:
    """Print a colored diff of two disassembly outputs."""
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RESET = '\033[0m'

    max_len = max(len(base_lines), len(built_lines))

    matches = 0
    mismatches = 0

    for i in range(max_len):
        base = base_lines[i] if i < len(base_lines) else "<missing>"
        built = built_lines[i] if i < len(built_lines) else "<missing>"

        base_norm = normalize_line(base)
        built_norm = normalize_line(built)

        if base_norm == built_norm:
            matches += 1
            if not mismatch_only:
                print(f"  {base}")
        else:
            mismatches += 1
            print(f"{RED}- {base}{RESET}")
            print(f"{GREEN}+ {built}{RESET}")

    total = matches + mismatches
    if total > 0:
        pct = (matches / total) * 100
        print()
        print(f"{YELLOW}Match: {matches}/{total} ({pct:.1f}%){RESET}")

def main():
    parser = argparse.ArgumentParser(description="Compare function assembly between ROMs")
    parser.add_argument("function", help="Function name (e.g., func_80001234)")
    parser.add_argument("-m", "--mismatch", action="store_true",
                        help="Show only mismatched lines")
    parser.add_argument("-w", "--watch", action="store_true",
                        help="Watch mode: rebuild and compare on file changes")
    parser.add_argument("-s", "--size", type=int, default=None,
                        help="Override function size in bytes")

    args = parser.parse_args()

    print(f"Comparing {args.function}...")
    print()

    # Get function addresses
    vram_start, vram_end = get_function_addresses(args.function)
    if args.size:
        vram_end = vram_start + args.size

    print(f"VRAM: 0x{vram_start:08X} - 0x{vram_end:08X}")
    print(f"ROM:  0x{vram_to_rom(vram_start):06X} - 0x{vram_to_rom(vram_end):06X}")
    print()

    # Get disassembly from both sources
    print("=== BASEROM ===")
    base_lines = disassemble_rom(BASEROM, vram_start, vram_end)

    print("=== BUILT ===")
    built_lines = disassemble_elf(args.function)
    if not built_lines:
        built_lines = disassemble_rom(BUILT_ROM, vram_start, vram_end)

    print()
    print("=== DIFF ===")
    diff_lines(base_lines, built_lines, args.mismatch)

if __name__ == "__main__":
    main()
