#!/usr/bin/env python3
"""
Setup work directories for function matching.

Creates a work/ directory with subdirectories for each function,
containing target assembly and status files.
"""

import os
import re
import sys
from pathlib import Path

# Project root
PROJECT_ROOT = Path(__file__).parent.parent
SYMBOL_FILE = PROJECT_ROOT / "symbol_addrs.us.txt"
ASM_DIR = PROJECT_ROOT / "asm" / "us"
WORK_DIR = PROJECT_ROOT / "work"

def parse_symbols():
    """Parse symbol_addrs.us.txt and return list of functions."""
    functions = []
    with open(SYMBOL_FILE, 'r') as f:
        for line in f:
            if 'type:func' in line:
                match = re.match(r'(\w+)\s*=\s*(0x[0-9A-Fa-f]+);\s*//\s*type:func\s*(.*)', line.strip())
                if match:
                    name, addr, comment = match.groups()
                    functions.append({
                        'name': name,
                        'addr': addr,
                        'addr_int': int(addr, 16),
                        'comment': comment.strip()
                    })
    return sorted(functions, key=lambda x: x['addr_int'])

def categorize_function(func):
    """Categorize a function into a module."""
    name = func['name']
    addr = func['addr_int']

    if name in ['memchr', 'memset', 'strchr', 'strlen', 'memcpy', 'bcopy', 'bzero', 'bzero_alt']:
        return 'libc'
    elif name.startswith('__fix') or name.startswith('__float') or name.startswith('__') and 'di3' in name:
        return 'libgcc'
    elif name in ['modf', 'modff', '__isinf', '__isnan', 'sinf', 'cosf', 'sqrtf', 'fcvt', '__ecvt_internal', '__round_helper']:
        return 'libm'
    elif name.startswith('osVi') or name.startswith('__osVi'):
        return 'libultra/vi'
    elif name.startswith('osAi') or name.startswith('__osAi'):
        return 'libultra/ai'
    elif name.startswith('osPi') or name.startswith('__osPi'):
        return 'libultra/pi'
    elif name.startswith('osSi') or name.startswith('__osSi'):
        return 'libultra/si'
    elif name.startswith('osSp') or name.startswith('__osSp'):
        return 'libultra/sp'
    elif name.startswith('osDp') or name.startswith('__osDp'):
        return 'libultra/dp'
    elif name.startswith('osPfs') or name.startswith('__osPfs'):
        return 'libultra/pfs'
    elif name.startswith('osCont') or name.startswith('__osCont'):
        return 'libultra/cont'
    elif name.startswith('osMotor') or name.startswith('__osMotor'):
        return 'libultra/motor'
    elif name.startswith('gu'):
        return 'libultra/gu'
    elif 'timer' in name.lower() or 'Timer' in name:
        return 'libultra/timer'
    elif 'thread' in name.lower() or 'Thread' in name:
        return 'libultra/thread'
    elif name.startswith('osInval') or name.startswith('osWriteback'):
        return 'libultra/cache'
    elif 'tlb' in name.lower() or 'TLB' in name:
        return 'libultra/tlb'
    elif name.startswith('os') or name.startswith('__os'):
        return 'libultra/os'
    elif 'inflate' in name.lower() or 'huft' in name.lower() or 'lzss' in name.lower():
        return 'inflate'
    elif name.startswith('dll_'):
        return 'dll'
    elif name.startswith('__sc') or name.startswith('osSc'):
        return 'scheduler'
    elif addr >= 0x80086A50 and addr < 0x80090000:
        return 'game/render'
    elif addr >= 0x80090000 and addr < 0x800A0000:
        return 'game/entity'
    elif addr >= 0x800A0000 and addr < 0x800B0000:
        return 'game/physics'
    elif addr >= 0x800B0000 and addr < 0x800C0000:
        return 'game/audio'
    elif addr >= 0x800C0000 and addr < 0x800D0000:
        return 'game/state'
    elif addr >= 0x800D0000 and addr < 0x800E0000:
        return 'game/ai'
    elif addr >= 0x800E0000 and addr < 0x800F0000:
        return 'game/menu'
    elif addr >= 0x800F0000 and addr < 0x80100000:
        return 'game/race'
    elif addr >= 0x80100000 and addr < 0x80110000:
        return 'game/vehicle'
    elif addr >= 0x80110000 and addr < 0x80125000:
        return 'game/misc'
    else:
        return 'boot'

def find_asm_file(addr_int):
    """Find which .s file contains a given address."""
    # Build a map of file -> address range
    for asm_file in sorted(ASM_DIR.glob("*.s")):
        with open(asm_file, 'r') as f:
            content = f.read()
            # Look for address references
            addrs = re.findall(r'/\* [A-F0-9]+ (8[0-9A-F]{7}) ', content)
            if addrs:
                file_addrs = [int(a, 16) for a in addrs]
                if file_addrs and min(file_addrs) <= addr_int <= max(file_addrs):
                    return asm_file
    return None

def extract_function_asm(func, asm_files_cache):
    """Extract assembly for a function from the .s files."""
    name = func['name']
    addr = func['addr']
    addr_int = func['addr_int']

    # Try to find by glabel
    glabel_pattern = f"glabel {name}"
    func_pattern = f"glabel func_{addr_int:08X}"

    for asm_file in sorted(ASM_DIR.glob("*.s")):
        try:
            with open(asm_file, 'r') as f:
                content = f.read()
        except:
            continue

        # Check if this file contains our function
        if glabel_pattern not in content and func_pattern not in content:
            # Also try matching address in comments
            if addr[2:].upper() not in content:
                continue

        lines = content.split('\n')
        in_func = False
        func_lines = []

        for i, line in enumerate(lines):
            if glabel_pattern in line or func_pattern in line:
                in_func = True
                func_lines.append(line)
                continue

            if in_func:
                # Check for end of function
                if line.strip().startswith('glabel') or line.strip().startswith('endlabel'):
                    if 'endlabel' in line and name in line:
                        func_lines.append(line)
                    break
                func_lines.append(line)

        if func_lines:
            return '\n'.join(func_lines), asm_file.name

    return None, None

def get_compiler_flags(category):
    """Get recommended compiler flags for a category."""
    if category in ['libc', 'libm', 'libultra/gu', 'inflate', 'dll']:
        return '-g0 -O2 -mips2 -G 0 -non_shared'
    elif category.startswith('libultra') or category == 'scheduler':
        return '-g0 -O1 -mips2 -G 0 -non_shared'
    elif category.startswith('game'):
        return '-g0 -O2 -mips2 -G 0 -non_shared'
    else:
        return '-g0 -O2 -mips2 -G 0 -non_shared'

def create_base_c(func, category):
    """Create a template base.c file for a function."""
    name = func['name']
    comment = func['comment']
    flags = get_compiler_flags(category)

    template = f'''/*
 * Function: {name}
 * Address:  {func['addr']}
 * Category: {category}
 * Status:   TODO
 *
 * {comment if comment else 'No description available.'}
 *
 * Compiler flags: {flags}
 */

/* Add includes as needed */
/* #include "types.h" */

/* TODO: Implement this function */
void {name}(void) {{
    /* Stub implementation */
}}
'''
    return template

def main():
    print("Setting up work directories for function matching...")
    print(f"Project root: {PROJECT_ROOT}")

    # Parse symbols
    print("\nParsing symbols...")
    functions = parse_symbols()
    print(f"Found {len(functions)} functions")

    # Known matches
    known_matches = {'strlen', 'osCreateMesgQueue', 'guMtxIdentF'}

    # Create work directory
    WORK_DIR.mkdir(exist_ok=True)

    # Stats
    created = 0
    skipped = 0
    no_asm = 0

    print("\nCreating function directories...")

    for i, func in enumerate(functions):
        name = func['name']
        category = categorize_function(func)

        # Create directory
        func_dir = WORK_DIR / category / name
        func_dir.mkdir(parents=True, exist_ok=True)

        # Extract assembly
        asm_content, asm_file = extract_function_asm(func, {})

        # Write target.s
        target_file = func_dir / "target.s"
        if asm_content:
            with open(target_file, 'w') as f:
                f.write(f"# Source: {asm_file}\n")
                f.write(f"# Address: {func['addr']}\n\n")
                f.write(asm_content)
        else:
            with open(target_file, 'w') as f:
                f.write(f"# Target assembly not found\n")
                f.write(f"# Address: {func['addr']}\n")
                f.write(f"# Search in asm/us/*.s for this address\n")
            no_asm += 1

        # Write base.c template
        base_file = func_dir / "base.c"
        if not base_file.exists():
            with open(base_file, 'w') as f:
                f.write(create_base_c(func, category))

        # Write STATUS file
        status_file = func_dir / "STATUS"
        if name in known_matches:
            status = "MATCHING"
        else:
            status = "TODO"
        with open(status_file, 'w') as f:
            f.write(status)

        # Write info.txt with metadata
        info_file = func_dir / "info.txt"
        with open(info_file, 'w') as f:
            f.write(f"name: {name}\n")
            f.write(f"address: {func['addr']}\n")
            f.write(f"category: {category}\n")
            f.write(f"compiler_flags: {get_compiler_flags(category)}\n")
            f.write(f"comment: {func['comment']}\n")

        created += 1

        if (i + 1) % 100 == 0:
            print(f"  Progress: {i + 1}/{len(functions)}")

    print(f"\nDone!")
    print(f"  Created: {created} function directories")
    print(f"  No ASM found: {no_asm} functions")
    print(f"\nWork directory: {WORK_DIR}")

    # Print category summary
    print("\nCategories created:")
    for cat_dir in sorted(WORK_DIR.iterdir()):
        if cat_dir.is_dir():
            count = len(list(cat_dir.rglob("STATUS")))
            print(f"  {cat_dir.name}: {count} functions")

if __name__ == "__main__":
    main()
