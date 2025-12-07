#!/usr/bin/env python3
"""
Wrapper for mips_to_c for Rush 2049 decompilation.

Usage:
    ./tools/m2c.py func_80006A00              # Decompile by function name
    ./tools/m2c.py asm/us/7600.s              # Decompile entire file
    ./tools/m2c.py asm/us/5610.s -f 0         # Decompile first function in file
    ./tools/m2c.py asm/us/5610.s -f func_80004A10  # Decompile specific function
"""

import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

# Configuration
PROJECT_ROOT = Path(__file__).parent.parent
MIPS_TO_C = PROJECT_ROOT / "tools" / "mips_to_c" / "m2c.py"
ASM_DIR = PROJECT_ROOT / "asm" / "us"
CONTEXT_FILE = PROJECT_ROOT / "include" / "types.h"
INCLUDE_DIR = PROJECT_ROOT / "include"

def preprocess_context() -> Path:
    """Preprocess types.h for mips_to_c."""
    if not CONTEXT_FILE.exists():
        return None

    # Create preprocessed file in temp directory
    preprocessed = Path(tempfile.gettempdir()) / "rush2049_types.h"

    # Check if we need to regenerate
    if preprocessed.exists():
        if preprocessed.stat().st_mtime > CONTEXT_FILE.stat().st_mtime:
            return preprocessed

    # Run preprocessor
    result = subprocess.run(
        ["cpp", "-I", str(INCLUDE_DIR), str(CONTEXT_FILE), "-o", str(preprocessed)],
        capture_output=True, text=True
    )

    if result.returncode != 0:
        print(f"Warning: Failed to preprocess context: {result.stderr}", file=sys.stderr)
        return None

    return preprocessed

def find_asm_file_for_func(func_name: str) -> tuple:
    """Find which .s file contains a function."""
    # Extract address from func name if possible
    match = re.match(r'func_([0-9A-Fa-f]+)', func_name)
    if not match:
        return None, None

    addr = int(match.group(1), 16)

    # Search all .s files for this function
    for asm_file in sorted(ASM_DIR.glob("*.s")):
        content = asm_file.read_text()
        if f"glabel {func_name}" in content:
            return asm_file, func_name

    return None, None

def run_m2c(asm_file: Path, func_name: str = None, use_context: bool = True, extra_args: list = None):
    """Run mips_to_c on an assembly file."""
    cmd = [sys.executable, str(MIPS_TO_C)]

    # Add preprocessed context if requested
    if use_context:
        ctx = preprocess_context()
        if ctx:
            cmd.extend(["--context", str(ctx)])

    # Add function filter if specified
    if func_name:
        cmd.extend(["-f", func_name])

    # Add extra args
    if extra_args:
        cmd.extend(extra_args)

    # Add input file
    cmd.append(str(asm_file))

    # Run mips_to_c
    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        print(f"Error running mips_to_c:", file=sys.stderr)
        print(result.stderr, file=sys.stderr)
        return None

    return result.stdout

def main():
    parser = argparse.ArgumentParser(
        description="Decompile MIPS assembly to C using mips_to_c",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument("target",
                        help="Function name (func_XXXXXXXX) or .s file path")
    parser.add_argument("-f", "--function",
                        help="Specific function to decompile (when target is a file)")
    parser.add_argument("--no-context", action="store_true",
                        help="Don't use types.h as context")
    parser.add_argument("--stack-structs", action="store_true",
                        help="Include template structs for stack variables")
    parser.add_argument("-o", "--output",
                        help="Output file (default: stdout)")

    args = parser.parse_args()

    # Determine if target is a function name or file
    if args.target.endswith(".s"):
        asm_file = Path(args.target)
        if not asm_file.exists():
            asm_file = ASM_DIR / args.target
        func_name = args.function
    elif args.target.startswith("func_"):
        asm_file, func_name = find_asm_file_for_func(args.target)
        if not asm_file:
            print(f"Could not find function {args.target}", file=sys.stderr)
            sys.exit(1)
        print(f"Found {args.target} in {asm_file}", file=sys.stderr)
    else:
        # Try as file path
        asm_file = Path(args.target)
        if not asm_file.exists():
            asm_file = ASM_DIR / args.target
        func_name = args.function

    if not asm_file.exists():
        print(f"File not found: {asm_file}", file=sys.stderr)
        sys.exit(1)

    # Build extra args
    extra_args = []
    if args.stack_structs:
        extra_args.append("--stack-structs")

    # Run decompilation
    use_context = not args.no_context
    output = run_m2c(asm_file, func_name, use_context=use_context, extra_args=extra_args)

    if output:
        if args.output:
            Path(args.output).write_text(output)
            print(f"Output written to {args.output}")
        else:
            print(output)

if __name__ == "__main__":
    main()
