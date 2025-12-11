#!/usr/bin/env python3
"""
decomp_me.py - Prepare Rush 2049 functions for decomp.me

Usage:
    ./tools/decomp_me.py func_80006A00              # Prepare function for decomp.me
    ./tools/decomp_me.py func_80006A00 --open       # Open in browser (requires account)
    ./tools/decomp_me.py --list                     # List ready functions from m2c_output

decomp.me is a collaborative decompilation site where you can:
1. Paste target assembly
2. Write C code attempts
3. See diff percentage towards matching
4. Share scratches with the community
"""

import argparse
import json
import os
import re
import subprocess
import sys
import urllib.parse
import webbrowser
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent
ASM_DIR = PROJECT_ROOT / "asm" / "us"
M2C_OUTPUT = PROJECT_ROOT / "m2c_output"
INCLUDE_DIR = PROJECT_ROOT / "include"

# decomp.me N64 compiler preset
# Rush 2049 uses IDO 5.3 based on analysis of code patterns
DECOMP_ME_CONFIG = {
    "platform": "n64",
    "compiler": "ido5.3",  # or ido7.1
    "compiler_flags": "-O2 -g0 -mips2",
    "diff_label": "",
}

def find_function_in_asm(func_name: str) -> tuple:
    """Find which .s file contains a function and extract it."""
    for asm_file in sorted(ASM_DIR.glob("*.s")):
        content = asm_file.read_text()
        if f"glabel {func_name}" in content:
            # Extract function assembly
            lines = content.split('\n')
            func_lines = []
            in_func = False
            for line in lines:
                if f"glabel {func_name}" in line:
                    in_func = True
                if in_func:
                    func_lines.append(line)
                    if line.strip().startswith("endlabel") or (in_func and "glabel" in line and func_name not in line):
                        break
                    # Also stop at jr $ra followed by delay slot
                    if "jr" in line and "$ra" in line and len(func_lines) > 1:
                        # Get one more line for delay slot
                        continue
            return asm_file, '\n'.join(func_lines)
    return None, None

def convert_to_decomp_me_format(asm_text: str) -> str:
    """Convert spimdisasm output to decomp.me format."""
    lines = []
    for line in asm_text.split('\n'):
        # Skip includes and directives
        if line.strip().startswith('.') or line.strip().startswith('#'):
            continue
        # Skip comments-only lines
        if line.strip().startswith('/*') and '*/' in line and not any(x in line for x in ['lui', 'addiu', 'sw', 'lw', 'jr', 'jal']):
            continue
        # Convert spimdisasm format to simpler format
        # Example: /* 7600 80006A00 3C0E8003 */  lui  $t6, %hi(D_8002C3D0)
        match = re.search(r'/\*.*?\*/\s*(\w+)\s+(.*)', line)
        if match:
            instr = match.group(1)
            operands = match.group(2).strip()
            lines.append(f"    {instr:8} {operands}")
        elif 'glabel' in line:
            func_name = line.split('glabel')[1].strip()
            lines.append(f"glabel {func_name}")
        elif 'endlabel' in line:
            continue  # decomp.me doesn't use endlabel
    return '\n'.join(lines)

def get_m2c_output(func_name: str) -> str:
    """Get m2c decompiled output for a function if available."""
    # Search m2c_output files
    for c_file in M2C_OUTPUT.glob("*.c"):
        content = c_file.read_text()
        # Look for the function
        if func_name in content:
            # Extract just this function
            lines = content.split('\n')
            func_lines = []
            in_func = False
            brace_count = 0
            for line in lines:
                if func_name in line and '(' in line:
                    in_func = True
                if in_func:
                    func_lines.append(line)
                    brace_count += line.count('{') - line.count('}')
                    if brace_count == 0 and '{' in ''.join(func_lines):
                        break
            if func_lines:
                return '\n'.join(func_lines)
    return None

def get_basic_context() -> str:
    """Get basic type definitions for context."""
    return '''// Rush 2049 types
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

// N64 OS types
typedef s32 OSMesgQueue;
typedef void* OSMesg;
'''

def create_decomp_me_scratch(func_name: str, asm_text: str, source_code: str = None) -> dict:
    """Create a decomp.me scratch configuration."""
    scratch = {
        "name": func_name,
        "target_asm": convert_to_decomp_me_format(asm_text),
        "context": get_basic_context(),
        "source_code": source_code or f"void {func_name}(void) {{\n    // TODO: decompile\n}}\n",
        **DECOMP_ME_CONFIG
    }
    return scratch

def generate_decomp_me_url(scratch: dict) -> str:
    """Generate a decomp.me URL with pre-filled data."""
    # decomp.me uses query params for new scratches
    base_url = "https://decomp.me/new"
    params = {
        "platform": scratch["platform"],
        "compiler": scratch["compiler"],
        "compiler_flags": scratch["compiler_flags"],
        "name": scratch["name"],
    }
    return f"{base_url}?" + urllib.parse.urlencode(params)

def save_scratch_files(func_name: str, scratch: dict):
    """Save scratch data to files for manual upload."""
    output_dir = PROJECT_ROOT / "decomp_me_scratches"
    output_dir.mkdir(exist_ok=True)

    func_dir = output_dir / func_name
    func_dir.mkdir(exist_ok=True)

    # Save assembly
    (func_dir / "target.s").write_text(scratch["target_asm"])

    # Save context
    (func_dir / "context.h").write_text(scratch["context"])

    # Save source
    (func_dir / "source.c").write_text(scratch["source_code"])

    # Save config
    (func_dir / "config.json").write_text(json.dumps({
        "platform": scratch["platform"],
        "compiler": scratch["compiler"],
        "compiler_flags": scratch["compiler_flags"],
    }, indent=2))

    return func_dir

def list_ready_functions():
    """List functions that have m2c output ready."""
    print("Functions with m2c output ready for decomp.me:")
    print("-" * 50)

    for c_file in sorted(M2C_OUTPUT.glob("*.c")):
        content = c_file.read_text()
        funcs = re.findall(r'^(\w+)\s+func_([0-9A-Fa-f]+)\s*\(', content, re.MULTILINE)
        if funcs:
            print(f"\n{c_file.name}:")
            for ret_type, addr in funcs[:10]:  # Limit to first 10
                print(f"  func_{addr}")
            if len(funcs) > 10:
                print(f"  ... and {len(funcs) - 10} more")

def main():
    parser = argparse.ArgumentParser(
        description="Prepare Rush 2049 functions for decomp.me",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument("function", nargs="?",
                        help="Function name (e.g., func_80006A00)")
    parser.add_argument("--open", action="store_true",
                        help="Open decomp.me in browser")
    parser.add_argument("--list", action="store_true",
                        help="List functions with m2c output")
    parser.add_argument("-o", "--output",
                        help="Output directory for scratch files")

    args = parser.parse_args()

    if args.list:
        list_ready_functions()
        return

    if not args.function:
        parser.print_help()
        return

    func_name = args.function

    # Find function assembly
    asm_file, asm_text = find_function_in_asm(func_name)
    if not asm_file:
        print(f"Function {func_name} not found in asm/us/*.s files")
        sys.exit(1)

    print(f"Found {func_name} in {asm_file.name}")

    # Get m2c output if available
    m2c_code = get_m2c_output(func_name)
    if m2c_code:
        print(f"Found m2c output for {func_name}")

    # Create scratch
    scratch = create_decomp_me_scratch(func_name, asm_text, m2c_code)

    # Save files
    output_dir = save_scratch_files(func_name, scratch)
    print(f"\nScratch files saved to: {output_dir}/")
    print(f"  target.s   - Assembly to match")
    print(f"  context.h  - Type definitions")
    print(f"  source.c   - Starting C code")
    print(f"  config.json - Compiler settings")

    # Generate URL
    url = generate_decomp_me_url(scratch)
    print(f"\ndecomp.me URL: {url}")

    if args.open:
        webbrowser.open(url)
        print("Opened in browser. Paste the assembly from target.s manually.")
    else:
        print("\nTo use:")
        print("1. Go to https://decomp.me/new")
        print("2. Select Platform: N64, Compiler: IDO 5.3")
        print("3. Paste target.s content as Target Assembly")
        print("4. Paste context.h content as Context")
        print("5. Paste source.c content as Source Code")
        print("6. Click 'Create scratch'")

if __name__ == "__main__":
    main()
