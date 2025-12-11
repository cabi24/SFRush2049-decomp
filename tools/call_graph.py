#!/usr/bin/env python3
"""
call_graph.py - Build and display function call graphs

Usage:
    ./tools/call_graph.py 0x800FD464              # Show calls from this function
    ./tools/call_graph.py 0x800FD464 --depth 3    # Limit depth
    ./tools/call_graph.py 0x800FD464 --callers    # Show who calls this function
    ./tools/call_graph.py --hot                   # Show most-called functions
    ./tools/call_graph.py --orphans               # Show functions never called
    ./tools/call_graph.py --dot > graph.dot       # Output graphviz format
"""

import argparse
import re
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).parent.parent
DISASM_FILE = PROJECT_ROOT / "build" / "game_code_disasm.txt"
SYMBOL_FILE = PROJECT_ROOT / "symbol_addrs.us.txt"


def load_symbols():
    """Load known symbol names"""
    symbols = {}
    if SYMBOL_FILE.exists():
        with open(SYMBOL_FILE) as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#') or line.startswith('//'):
                    continue
                match = re.match(r'(\w+)\s*=\s*0x([0-9A-Fa-f]+)', line)
                if match:
                    name, addr = match.groups()
                    symbols[int(addr, 16)] = name
    return symbols


def build_call_graph():
    """Parse disassembly and build bidirectional call graph"""

    if not DISASM_FILE.exists():
        print(f"Error: {DISASM_FILE} not found")
        return None, None, None

    # calls[func] = set of functions it calls
    # callers[func] = set of functions that call it
    calls = defaultdict(set)
    callers = defaultdict(set)
    func_addrs = set()

    current_func = None

    with open(DISASM_FILE) as f:
        for line in f:
            match = re.match(r'^([0-9A-Fa-f]+):\s+(\w+)\s*(.*)', line)
            if not match:
                continue

            addr = int(match.group(1), 16)
            instr = match.group(2)
            args = match.group(3)

            # Detect function start
            if instr == 'addiu' and 'sp,sp,-' in args:
                current_func = addr
                func_addrs.add(addr)

            # Track calls
            if instr == 'jal' and current_func:
                call_match = re.search(r'0x([0-9A-Fa-f]+)', args)
                if call_match:
                    target = int(call_match.group(1), 16)
                    calls[current_func].add(target)
                    callers[target].add(current_func)

    return calls, callers, func_addrs


def get_name(addr, symbols):
    """Get function name or generate one"""
    return symbols.get(addr, f"func_{addr:08X}")


def print_tree(addr, calls, symbols, depth=0, max_depth=3, visited=None):
    """Recursively print call tree"""
    if visited is None:
        visited = set()

    indent = "│   " * depth
    name = get_name(addr, symbols)

    if addr in visited:
        print(f"{indent}├── {name} (recursive)")
        return
    if depth >= max_depth:
        if calls.get(addr):
            print(f"{indent}├── {name} (...)")
        else:
            print(f"{indent}├── {name}")
        return

    visited = visited | {addr}

    call_count = len(calls.get(addr, set()))
    if call_count > 0:
        print(f"{indent}├── {name} ({call_count} calls)")
    else:
        print(f"{indent}├── {name} (leaf)")

    for target in sorted(calls.get(addr, set())):
        print_tree(target, calls, symbols, depth + 1, max_depth, visited)


def main():
    parser = argparse.ArgumentParser(description='Analyze function call graph')
    parser.add_argument('address', nargs='?', help='Function address to analyze')
    parser.add_argument('--depth', type=int, default=2, help='Max depth to display')
    parser.add_argument('--callers', action='store_true', help='Show callers instead of callees')
    parser.add_argument('--hot', action='store_true', help='Show most-called functions')
    parser.add_argument('--orphans', action='store_true', help='Show never-called functions')
    parser.add_argument('--leaf', action='store_true', help='Show leaf functions (no calls)')
    parser.add_argument('--dot', action='store_true', help='Output graphviz DOT format')
    parser.add_argument('--limit', type=int, default=30, help='Max results')
    args = parser.parse_args()

    symbols = load_symbols()
    result = build_call_graph()
    if not result[0]:
        return
    calls, callers, func_addrs = result

    if args.hot:
        print("Most-called functions:")
        print()
        sorted_funcs = sorted(func_addrs, key=lambda a: len(callers.get(a, set())), reverse=True)
        for addr in sorted_funcs[:args.limit]:
            name = get_name(addr, symbols)
            caller_count = len(callers.get(addr, set()))
            callee_count = len(calls.get(addr, set()))
            print(f"  0x{addr:08X}  {name:<35} called by {caller_count:>3} funcs, calls {callee_count:>3}")
        return

    if args.orphans:
        print("Functions never called (entry points or dead code):")
        print()
        orphans = [a for a in func_addrs if not callers.get(a)]
        for addr in sorted(orphans)[:args.limit]:
            name = get_name(addr, symbols)
            callee_count = len(calls.get(addr, set()))
            print(f"  0x{addr:08X}  {name:<35} calls {callee_count} funcs")
        print(f"\nTotal: {len(orphans)} orphan functions")
        return

    if args.leaf:
        print("Leaf functions (make no calls):")
        print()
        leaves = [a for a in func_addrs if not calls.get(a)]
        for addr in sorted(leaves)[:args.limit]:
            name = get_name(addr, symbols)
            caller_count = len(callers.get(addr, set()))
            print(f"  0x{addr:08X}  {name:<35} called by {caller_count} funcs")
        print(f"\nTotal: {len(leaves)} leaf functions")
        return

    if args.dot:
        print("digraph calls {")
        print("  rankdir=LR;")
        print("  node [shape=box];")
        for func, targets in calls.items():
            fname = get_name(func, symbols)
            for target in targets:
                tname = get_name(target, symbols)
                print(f'  "{fname}" -> "{tname}";')
        print("}")
        return

    if not args.address:
        parser.print_help()
        return

    # Parse address
    addr_str = args.address
    if addr_str.startswith('0x'):
        addr = int(addr_str, 16)
    else:
        addr = int(addr_str, 16)

    name = get_name(addr, symbols)

    if args.callers:
        print(f"Functions that call {name} (0x{addr:08X}):")
        print()
        caller_list = sorted(callers.get(addr, set()))
        if not caller_list:
            print("  (none - this is an entry point or never called)")
        for caller in caller_list:
            cname = get_name(caller, symbols)
            print(f"  0x{caller:08X}  {cname}")
    else:
        print(f"Call tree for {name} (0x{addr:08X}):")
        print()
        print_tree(addr, calls, symbols, max_depth=args.depth)


if __name__ == '__main__':
    main()
