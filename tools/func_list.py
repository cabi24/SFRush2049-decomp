#!/usr/bin/env python3
"""
func_list.py - List and analyze functions from disassembly

Usage:
    ./tools/func_list.py                    # List all functions
    ./tools/func_list.py --sort-size        # Sort by size (smallest first)
    ./tools/func_list.py --leaf-only        # Only show leaf functions (no calls)
    ./tools/func_list.py --has-calls        # Only show functions that make calls
    ./tools/func_list.py --min-size 100     # Minimum instruction count
    ./tools/func_list.py --max-size 50      # Maximum instruction count
    ./tools/func_list.py --search "float"   # Search for pattern in disasm
"""

import argparse
import re
import os
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).parent.parent
DISASM_FILE = PROJECT_ROOT / "build" / "game_code_disasm.txt"
SYMBOL_FILE = PROJECT_ROOT / "symbol_addrs.us.txt"


def load_symbols():
    """Load known symbol names from symbol_addrs.us.txt"""
    symbols = {}
    if SYMBOL_FILE.exists():
        with open(SYMBOL_FILE) as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#') or line.startswith('//'):
                    continue
                # Format: name = 0xADDRESS; // comment
                match = re.match(r'(\w+)\s*=\s*0x([0-9A-Fa-f]+)', line)
                if match:
                    name, addr = match.groups()
                    symbols[int(addr, 16)] = name
    return symbols


def parse_disasm():
    """Parse disassembly file and extract function info"""
    if not DISASM_FILE.exists():
        print(f"Error: {DISASM_FILE} not found")
        print("Run: make extract_game_code first")
        return []

    functions = []
    current_func = None
    current_instrs = []
    current_calls = []
    current_has_float = False
    current_has_branch_back = False
    last_addr = 0

    with open(DISASM_FILE) as f:
        for line in f:
            # Match instruction lines: "80086A50: addiu sp,sp,-32"
            match = re.match(r'^([0-9A-Fa-f]+):\s+(\w+)\s*(.*)', line)
            if not match:
                continue

            addr = int(match.group(1), 16)
            instr = match.group(2)
            args = match.group(3)

            # Detect function start (stack frame setup)
            if instr == 'addiu' and 'sp,sp,-' in args:
                # Save previous function
                if current_func:
                    size = last_addr - current_func + 4
                    functions.append({
                        'addr': current_func,
                        'size': size,
                        'instrs': len(current_instrs),
                        'calls': current_calls,
                        'is_leaf': len(current_calls) == 0,
                        'has_float': current_has_float,
                        'has_loop': current_has_branch_back,
                    })

                current_func = addr
                current_instrs = []
                current_calls = []
                current_has_float = False
                current_has_branch_back = False

            if current_func:
                current_instrs.append((addr, instr, args))

                # Track calls
                if instr == 'jal':
                    call_match = re.search(r'0x([0-9A-Fa-f]+)', args)
                    if call_match:
                        current_calls.append(int(call_match.group(1), 16))

                # Track float operations
                if instr in ['lwc1', 'swc1', 'add.s', 'sub.s', 'mul.s', 'div.s',
                            'c.lt.s', 'c.le.s', 'c.eq.s', 'bc1t', 'bc1f',
                            'cvt.s.w', 'cvt.w.s', 'mov.s', 'neg.s', 'abs.s']:
                    current_has_float = True

                # Track backward branches (loops)
                if instr in ['beq', 'bne', 'blez', 'bgtz', 'bltz', 'bgez']:
                    branch_match = re.search(r'0x([0-9A-Fa-f]+)', args)
                    if branch_match:
                        target = int(branch_match.group(1), 16)
                        if target < addr:
                            current_has_branch_back = True

                last_addr = addr

    # Don't forget last function
    if current_func:
        size = last_addr - current_func + 4
        functions.append({
            'addr': current_func,
            'size': size,
            'instrs': len(current_instrs),
            'calls': current_calls,
            'is_leaf': len(current_calls) == 0,
            'has_float': current_has_float,
            'has_loop': current_has_branch_back,
        })

    return functions


def main():
    parser = argparse.ArgumentParser(description='List functions from disassembly')
    parser.add_argument('--sort-size', action='store_true', help='Sort by size (smallest first)')
    parser.add_argument('--sort-calls', action='store_true', help='Sort by call count')
    parser.add_argument('--leaf-only', action='store_true', help='Only leaf functions')
    parser.add_argument('--has-calls', action='store_true', help='Only functions with calls')
    parser.add_argument('--has-float', action='store_true', help='Only functions with float ops')
    parser.add_argument('--has-loop', action='store_true', help='Only functions with loops')
    parser.add_argument('--min-size', type=int, help='Minimum instruction count')
    parser.add_argument('--max-size', type=int, help='Maximum instruction count')
    parser.add_argument('--limit', type=int, default=50, help='Max results to show')
    parser.add_argument('--csv', action='store_true', help='Output as CSV')
    args = parser.parse_args()

    symbols = load_symbols()
    functions = parse_disasm()

    if not functions:
        return

    # Apply filters
    if args.leaf_only:
        functions = [f for f in functions if f['is_leaf']]
    if args.has_calls:
        functions = [f for f in functions if not f['is_leaf']]
    if args.has_float:
        functions = [f for f in functions if f['has_float']]
    if args.has_loop:
        functions = [f for f in functions if f['has_loop']]
    if args.min_size:
        functions = [f for f in functions if f['instrs'] >= args.min_size]
    if args.max_size:
        functions = [f for f in functions if f['instrs'] <= args.max_size]

    # Apply sorting
    if args.sort_size:
        functions.sort(key=lambda f: f['instrs'])
    elif args.sort_calls:
        functions.sort(key=lambda f: len(f['calls']), reverse=True)

    # Output
    if args.csv:
        print("address,name,size_bytes,instrs,calls,is_leaf,has_float,has_loop")
        for f in functions[:args.limit]:
            name = symbols.get(f['addr'], f"func_{f['addr']:08X}")
            print(f"0x{f['addr']:08X},{name},{f['size']},{f['instrs']},{len(f['calls'])},{f['is_leaf']},{f['has_float']},{f['has_loop']}")
    else:
        print(f"Found {len(functions)} functions")
        print()
        print(f"{'Address':<12} {'Name':<30} {'Size':>6} {'Instrs':>6} {'Calls':>5} {'Flags'}")
        print("-" * 80)

        for f in functions[:args.limit]:
            name = symbols.get(f['addr'], f"func_{f['addr']:08X}")
            if len(name) > 28:
                name = name[:25] + "..."

            flags = []
            if f['is_leaf']:
                flags.append('leaf')
            if f['has_float']:
                flags.append('float')
            if f['has_loop']:
                flags.append('loop')

            print(f"0x{f['addr']:08X}  {name:<30} {f['size']:>5}B {f['instrs']:>5}  {len(f['calls']):>4}  {','.join(flags)}")

        if len(functions) > args.limit:
            print(f"\n... and {len(functions) - args.limit} more (use --limit to show more)")


if __name__ == '__main__':
    main()
