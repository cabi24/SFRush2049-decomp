#!/usr/bin/env python3
"""
extract_func.py - Extract a single function's assembly from disassembly

Usage:
    ./tools/extract_func.py 0x800CA3B4              # Extract by address
    ./tools/extract_func.py playgame                # Extract by name
    ./tools/extract_func.py 0x800CA3B4 --context 5  # Add 5 lines before/after
    ./tools/extract_func.py 0x800CA3B4 --m2c        # Format for mips_to_c
"""

import argparse
import re
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent
DISASM_FILE = PROJECT_ROOT / "build" / "game_code_disasm.txt"
SYMBOL_FILE = PROJECT_ROOT / "symbol_addrs.us.txt"


def load_symbols():
    """Load symbols as both name->addr and addr->name"""
    name_to_addr = {}
    addr_to_name = {}
    if SYMBOL_FILE.exists():
        with open(SYMBOL_FILE) as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#') or line.startswith('//'):
                    continue
                match = re.match(r'(\w+)\s*=\s*0x([0-9A-Fa-f]+)', line)
                if match:
                    name, addr = match.groups()
                    addr_int = int(addr, 16)
                    name_to_addr[name] = addr_int
                    addr_to_name[addr_int] = name
    return name_to_addr, addr_to_name


def find_function(lines, target_addr):
    """Find function boundaries containing target_addr"""
    func_start = None
    func_end = None

    for i, line in enumerate(lines):
        match = re.match(r'^([0-9A-Fa-f]+):\s+(\w+)\s*(.*)', line)
        if not match:
            continue

        addr = int(match.group(1), 16)
        instr = match.group(2)
        args = match.group(3)

        # Detect function start
        if instr == 'addiu' and 'sp,sp,-' in args:
            if addr <= target_addr:
                func_start = i
            elif func_start is not None and func_end is None:
                func_end = i
                break

        # Detect function end (jr $ra followed by delay slot)
        if func_start is not None and func_end is None:
            if instr == 'jr' and 'ra' in args:
                # Function ends after the delay slot (next instruction)
                func_end = i + 2  # Include delay slot

    if func_start is not None and func_end is None:
        # Function goes to end of file
        func_end = len(lines)

    return func_start, func_end


def extract_function(target_addr, context=0, m2c_format=False):
    """Extract function assembly"""

    if not DISASM_FILE.exists():
        print(f"Error: {DISASM_FILE} not found")
        return None

    with open(DISASM_FILE) as f:
        lines = f.readlines()

    func_start, func_end = find_function(lines, target_addr)

    if func_start is None:
        print(f"Error: Could not find function containing 0x{target_addr:08X}")
        return None

    # Apply context
    start = max(0, func_start - context)
    end = min(len(lines), func_end + context)

    func_lines = lines[start:end]

    if m2c_format:
        # Format for mips_to_c
        output = []
        output.append(f"glabel func_{target_addr:08X}")
        for line in lines[func_start:func_end]:
            line = line.strip()
            if not line:
                continue
            # Convert "ADDR: INSTR ARGS" to "/* ADDR */ INSTR ARGS"
            match = re.match(r'^([0-9A-Fa-f]+):\s+(.*)', line)
            if match:
                addr, rest = match.groups()
                output.append(f"/* {addr} */ {rest}")
        return '\n'.join(output)
    else:
        return ''.join(func_lines)


def main():
    parser = argparse.ArgumentParser(description='Extract function assembly')
    parser.add_argument('target', help='Function address (0x...) or name')
    parser.add_argument('--context', '-c', type=int, default=0,
                       help='Lines of context before/after')
    parser.add_argument('--m2c', action='store_true',
                       help='Format output for mips_to_c')
    args = parser.parse_args()

    name_to_addr, addr_to_name = load_symbols()

    # Resolve target to address
    target = args.target
    if target.startswith('0x'):
        target_addr = int(target, 16)
    elif target.startswith('func_'):
        target_addr = int(target[5:], 16)
    elif target in name_to_addr:
        target_addr = name_to_addr[target]
    else:
        # Try as hex anyway
        try:
            target_addr = int(target, 16)
        except ValueError:
            print(f"Error: Unknown function '{target}'")
            print("Try an address like 0x800CA3B4 or a known symbol name")
            return

    result = extract_function(target_addr, args.context, args.m2c)
    if result:
        # Add header
        name = addr_to_name.get(target_addr, f"func_{target_addr:08X}")
        if not args.m2c:
            print(f"/* {name} @ 0x{target_addr:08X} */")
            print()
        print(result)


if __name__ == '__main__':
    main()
