#!/usr/bin/env python3
"""
Merge decompilation sources for Rush 2049 N64

Combines output from:
1. Ghidra (ghidra/ghidra_decompiled.c) - best for control flow, types
2. Ollama (ollama_analysis/*.txt) - best for context, naming
3. Manual stubs (src/game/*.c) - ground truth structure

Usage:
  python tools/merge_decompilation.py <address>     # Show merged output for one function
  python tools/merge_decompilation.py --list        # List all available functions
  python tools/merge_decompilation.py --update <file> # Update a src/ file with best available
"""

import re
import sys
import os
from pathlib import Path

# Project root
PROJECT_ROOT = Path(__file__).parent.parent

# Source files
GHIDRA_FILE = PROJECT_ROOT / "ghidra" / "ghidra_decompiled.c"
OLLAMA_DIR = PROJECT_ROOT / "ollama_analysis"
SRC_GAME_DIR = PROJECT_ROOT / "src" / "game"

def parse_ghidra_functions(filepath):
    """Extract functions from Ghidra output."""
    functions = {}
    if not filepath.exists():
        return functions

    content = filepath.read_text()

    # Match function definitions: /* 80XXXXXX */ followed by function
    pattern = r'/\*\s*(80[0-9a-fA-F]{6})\s*\*/\s*\n+(?:.*?\n)*?(\w+)\s+FUN_80([0-9a-fA-F]{6})\s*\((.*?)\)\s*\{(.*?)^\}'

    # Simpler pattern - just find function starts
    func_pattern = r'(?:^/\*\s*(80[0-9a-fA-F]{6})\s*\*/\s*$|^(\w+)\s+(FUN_80[0-9a-fA-F]{6})\s*\()'

    current_addr = None
    current_func_start = None
    brace_depth = 0

    lines = content.split('\n')
    i = 0
    while i < len(lines):
        line = lines[i]

        # Check for address comment
        addr_match = re.match(r'/\*\s*(80[0-9a-fA-F]{6})\s*\*/', line.strip())
        if addr_match:
            current_addr = addr_match.group(1).lower()
            i += 1
            continue

        # Check for function definition
        func_match = re.match(r'^(\w+)\s+(FUN_80[0-9a-fA-F]{6})\s*\((.*)', line)
        if func_match and current_addr:
            return_type = func_match.group(1)
            func_name = func_match.group(2)

            # Extract full function body
            func_lines = [line]
            brace_depth = line.count('{') - line.count('}')
            i += 1

            while i < len(lines) and brace_depth >= 0:
                func_lines.append(lines[i])
                brace_depth += lines[i].count('{') - lines[i].count('}')
                if brace_depth == 0 and '}' in lines[i]:
                    break
                i += 1

            func_body = '\n'.join(func_lines)
            functions[current_addr] = {
                'source': 'ghidra',
                'name': func_name,
                'code': func_body,
                'return_type': return_type
            }
            current_addr = None

        i += 1

    return functions

def parse_ollama_functions(directory):
    """Extract functions from Ollama analysis files."""
    functions = {}
    if not directory.exists():
        return functions

    for filepath in directory.glob("*.txt"):
        content = filepath.read_text()

        # Look for function patterns like:
        # func_800XXXXX or 0x800XXXXX:
        # Followed by C code

        # Pattern 1: func_800XXXXX { ... }
        pattern = r'(?:func_|FUN_)(80[0-9a-fA-F]{6})\s*\([^)]*\)\s*\{([^}]+(?:\{[^}]*\}[^}]*)*)\}'

        for match in re.finditer(pattern, content, re.MULTILINE | re.DOTALL):
            addr = match.group(1).lower()
            body = match.group(0)

            if addr not in functions or len(body) > len(functions[addr].get('code', '')):
                functions[addr] = {
                    'source': f'ollama:{filepath.name}',
                    'name': f'func_{addr}',
                    'code': body
                }

        # Pattern 2: Address headers like "=== 0x800XXXXX ===" or "// 800XXXXX"
        sections = re.split(r'(?:===\s*0x|//\s*)(80[0-9a-fA-F]{6})', content)
        for i in range(1, len(sections), 2):
            if i + 1 < len(sections):
                addr = sections[i].lower()
                code = sections[i + 1]

                # Extract C-like code block
                code_match = re.search(r'((?:void|int|u\d+|s\d+|float)\s+\w+\s*\([^)]*\)\s*\{[^}]+\})', code, re.DOTALL)
                if code_match and addr not in functions:
                    functions[addr] = {
                        'source': f'ollama:{filepath.name}',
                        'name': f'func_{addr}',
                        'code': code_match.group(1)
                    }

    return functions

def parse_src_stubs(directory):
    """Extract function stubs from src/ directory."""
    stubs = {}
    if not directory.exists():
        return stubs

    for filepath in directory.glob("*.c"):
        content = filepath.read_text()

        # Find function declarations with addresses
        # Pattern: /* Address: 0x800XXXXX */ or func_800XXXXX
        pattern = r'(?:/\*\*?[^*]*Address:\s*0x(80[0-9a-fA-F]{6})[^*]*\*/\s*)?(?:void|int|u\d+|s\d+|float)\s+(func_80[0-9a-fA-F]{6})\s*\([^)]*\)\s*;'

        for match in re.finditer(pattern, content):
            addr_from_comment = match.group(1)
            func_name = match.group(2)
            addr_from_name = func_name.replace('func_', '').lower()

            addr = (addr_from_comment or addr_from_name).lower()

            stubs[addr] = {
                'source': f'stub:{filepath.name}',
                'name': func_name,
                'file': filepath.name,
                'code': match.group(0)
            }

    return stubs

def merge_function(addr, ghidra_funcs, ollama_funcs, stubs):
    """Merge available info for a single function."""
    result = {
        'address': f'0x{addr}',
        'sources': []
    }

    if addr in ghidra_funcs:
        result['ghidra'] = ghidra_funcs[addr]
        result['sources'].append('ghidra')

    if addr in ollama_funcs:
        result['ollama'] = ollama_funcs[addr]
        result['sources'].append('ollama')

    if addr in stubs:
        result['stub'] = stubs[addr]
        result['sources'].append('stub')

    return result

def format_merged_output(merged):
    """Format merged function info for display."""
    output = []
    output.append(f"=== Function at {merged['address']} ===")
    output.append(f"Sources: {', '.join(merged['sources'])}")
    output.append("")

    if 'stub' in merged:
        output.append(f"--- Stub ({merged['stub']['file']}) ---")
        output.append(merged['stub']['code'])
        output.append("")

    if 'ghidra' in merged:
        output.append("--- Ghidra ---")
        code = merged['ghidra']['code']
        # Truncate if too long
        if len(code) > 2000:
            code = code[:2000] + "\n... (truncated)"
        output.append(code)
        output.append("")

    if 'ollama' in merged:
        output.append(f"--- Ollama ({merged['ollama']['source']}) ---")
        output.append(merged['ollama']['code'])
        output.append("")

    return '\n'.join(output)

def list_all_functions():
    """List all functions from all sources."""
    ghidra_funcs = parse_ghidra_functions(GHIDRA_FILE)
    ollama_funcs = parse_ollama_functions(OLLAMA_DIR)
    stubs = parse_src_stubs(SRC_GAME_DIR)

    all_addrs = set(ghidra_funcs.keys()) | set(ollama_funcs.keys()) | set(stubs.keys())

    print(f"Total unique functions: {len(all_addrs)}")
    print(f"  Ghidra: {len(ghidra_funcs)}")
    print(f"  Ollama: {len(ollama_funcs)}")
    print(f"  Stubs: {len(stubs)}")
    print("")
    print("Address      Ghidra  Ollama  Stub    File")
    print("-" * 60)

    for addr in sorted(all_addrs):
        g = "X" if addr in ghidra_funcs else " "
        o = "X" if addr in ollama_funcs else " "
        s = "X" if addr in stubs else " "
        f = stubs[addr]['file'] if addr in stubs else ""
        print(f"0x{addr}   {g}       {o}       {s}       {f}")

def lookup_function(addr):
    """Look up a specific function."""
    # Normalize address
    addr = addr.lower().replace('0x', '').replace('func_', '')

    ghidra_funcs = parse_ghidra_functions(GHIDRA_FILE)
    ollama_funcs = parse_ollama_functions(OLLAMA_DIR)
    stubs = parse_src_stubs(SRC_GAME_DIR)

    merged = merge_function(addr, ghidra_funcs, ollama_funcs, stubs)

    if not merged['sources']:
        print(f"No function found at 0x{addr}")
        return

    print(format_merged_output(merged))

def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return

    arg = sys.argv[1]

    if arg == '--list':
        list_all_functions()
    elif arg == '--update' and len(sys.argv) > 2:
        # TODO: implement file update
        print("Update not yet implemented")
    else:
        lookup_function(arg)

if __name__ == '__main__':
    main()
