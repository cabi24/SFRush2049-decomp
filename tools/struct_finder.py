#!/usr/bin/env python3
"""
struct_finder.py - Analyze memory access patterns to infer struct layouts

Usage:
    ./tools/struct_finder.py 0x80152818           # Analyze accesses to this address
    ./tools/struct_finder.py --all                # Find all potential structs
    ./tools/struct_finder.py --output structs.h  # Generate header file
"""

import argparse
import re
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).parent.parent
DISASM_FILE = PROJECT_ROOT / "build" / "game_code_disasm.txt"


def analyze_struct(base_addr, disasm_lines):
    """Analyze all accesses to a base address to infer struct layout"""

    # Track offset -> {type hints, reference count, example instructions}
    offsets = defaultdict(lambda: {'count': 0, 'types': set(), 'examples': []})

    # Patterns for memory access
    # lw $reg, offset($base) - word load
    # lh/lhu $reg, offset($base) - halfword
    # lb/lbu $reg, offset($base) - byte
    # lwc1 $freg, offset($base) - float load
    # sw/sh/sb/swc1 - stores

    base_hex = f"{base_addr:08X}"
    base_hi = f"0x{(base_addr >> 16) & 0xFFFF:04X}"
    base_lo = base_addr & 0xFFFF

    # Find all lui that load the high part of our address
    lui_regs = set()
    for line in disasm_lines:
        if f'lui' in line and base_hi.lower() in line.lower():
            match = re.search(r'lui\s+\$?(\w+)', line)
            if match:
                lui_regs.add(match.group(1))

    # Now find memory accesses using those registers
    for i, line in enumerate(disasm_lines):
        for reg in lui_regs:
            # Look for addiu to set up the full address
            if f'addiu' in line and reg in line:
                # Track which register now has the address
                pass

        # Direct offset access pattern
        for instr, type_hint in [('lw', 's32'), ('sw', 's32'),
                                  ('lwc1', 'f32'), ('swc1', 'f32'),
                                  ('lh', 's16'), ('lhu', 'u16'), ('sh', 's16'),
                                  ('lb', 's8'), ('lbu', 'u8'), ('sb', 's8')]:
            if instr in line:
                # Match: lw $t0, 0x10($s0)
                match = re.search(rf'{instr}\s+\$?\w+,\s*(-?\d+|0x[0-9a-fA-F]+)\(\$?(\w+)\)', line)
                if match:
                    offset_str, base_reg = match.groups()
                    try:
                        if offset_str.startswith('0x'):
                            offset = int(offset_str, 16)
                        else:
                            offset = int(offset_str)

                        # Only track reasonable offsets (struct fields)
                        if 0 <= offset < 0x1000:
                            offsets[offset]['count'] += 1
                            offsets[offset]['types'].add(type_hint)
                            if len(offsets[offset]['examples']) < 3:
                                offsets[offset]['examples'].append(line.strip())
                    except ValueError:
                        pass

    return offsets


def find_all_base_addresses(disasm_lines):
    """Find addresses that are frequently used as struct bases"""

    # Look for lui patterns that are used with many different offsets
    lui_pattern = re.compile(r'lui\s+\$?(\w+),\s*(0x[0-9a-fA-F]+)')

    base_candidates = defaultdict(int)

    for line in disasm_lines:
        match = lui_pattern.search(line)
        if match:
            high_val = int(match.group(2), 16)
            # Only consider addresses in RAM range
            if 0x8000 <= high_val <= 0x8020:
                base_addr = high_val << 16
                base_candidates[base_addr] += 1

    return base_candidates


def generate_struct_header(name, offsets):
    """Generate C struct definition from offset analysis"""

    lines = [f"typedef struct {name} {{"]

    sorted_offsets = sorted(offsets.keys())
    prev_end = 0

    for offset in sorted_offsets:
        info = offsets[offset]

        # Add padding if needed
        if offset > prev_end:
            gap = offset - prev_end
            lines.append(f"    u8 pad_{prev_end:02X}[0x{gap:X}];")

        # Determine type
        types = info['types']
        if 'f32' in types:
            type_str = 'f32'
            size = 4
        elif 's32' in types or 'u32' in types:
            type_str = 's32'
            size = 4
        elif 's16' in types or 'u16' in types:
            type_str = 's16'
            size = 2
        else:
            type_str = 's8'
            size = 1

        lines.append(f"    {type_str} field_{offset:02X};  /* +0x{offset:02X}, refs: {info['count']} */")
        prev_end = offset + size

    lines.append(f"}} {name};")
    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(description='Analyze struct layouts from disassembly')
    parser.add_argument('address', nargs='?', help='Base address to analyze (e.g., 0x80152818)')
    parser.add_argument('--all', action='store_true', help='Find all potential structs')
    parser.add_argument('--min-refs', type=int, default=5, help='Minimum references to report')
    parser.add_argument('--output', help='Output header file')
    args = parser.parse_args()

    if not DISASM_FILE.exists():
        print(f"Error: {DISASM_FILE} not found")
        print("Run: make extract_game_code first")
        return

    with open(DISASM_FILE) as f:
        disasm_lines = f.readlines()

    if args.all:
        bases = find_all_base_addresses(disasm_lines)
        print(f"Found {len(bases)} potential struct base addresses:")
        print()
        for addr, count in sorted(bases.items(), key=lambda x: -x[1])[:20]:
            print(f"  0x{addr:08X}: {count} lui references")
        return

    if not args.address:
        parser.print_help()
        return

    # Parse address
    addr_str = args.address
    if addr_str.startswith('0x'):
        base_addr = int(addr_str, 16)
    else:
        base_addr = int(addr_str, 16)  # Assume hex anyway

    print(f"Analyzing struct at 0x{base_addr:08X}")
    print()

    offsets = analyze_struct(base_addr, disasm_lines)

    if not offsets:
        print("No field accesses found. Try --all to find struct candidates.")
        return

    # Filter by minimum references
    offsets = {k: v for k, v in offsets.items() if v['count'] >= args.min_refs}

    print(f"Found {len(offsets)} fields with >= {args.min_refs} references:")
    print()
    print(f"{'Offset':<10} {'Type':<8} {'Refs':>5}  Examples")
    print("-" * 70)

    for offset in sorted(offsets.keys()):
        info = offsets[offset]
        types = ','.join(sorted(info['types']))
        example = info['examples'][0] if info['examples'] else ''
        if len(example) > 40:
            example = example[:37] + '...'
        print(f"+0x{offset:04X}    {types:<8} {info['count']:>5}  {example}")

    # Generate struct definition
    print()
    print("=" * 70)
    print("Suggested struct definition:")
    print("=" * 70)
    print()
    struct_name = f"Struct_{base_addr:08X}"
    print(generate_struct_header(struct_name, offsets))

    if args.output:
        with open(args.output, 'w') as f:
            f.write(f"/* Auto-generated struct for 0x{base_addr:08X} */\n\n")
            f.write(generate_struct_header(struct_name, offsets))
            f.write("\n")
        print(f"\nWrote to {args.output}")


if __name__ == '__main__':
    main()
