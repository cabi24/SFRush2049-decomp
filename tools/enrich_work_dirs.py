#!/usr/bin/env python3
"""
Enrich work directories with self-contained documentation.

Each function directory will contain everything needed to work on it
in isolation, perfect for fresh AI contexts or local LLMs.
"""

import os
import re
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent
WORK_DIR = PROJECT_ROOT / "work"
SYMBOL_FILE = PROJECT_ROOT / "symbol_addrs.us.txt"
ASM_DIR = PROJECT_ROOT / "asm" / "us"

# Common types header
TYPES_H = '''/*
 * Common types for N64 decompilation
 * Include this in your implementation
 */

#ifndef TYPES_H
#define TYPES_H

/* Basic integer types */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

/* Floating point */
typedef float f32;
typedef double f64;

/* Volatile versions (for hardware registers) */
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

/* Pointer types */
typedef void* voidptr;

/* Boolean */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* TYPES_H */
'''

def get_compile_script(category, flags):
    """Generate compile.sh for a function."""
    return f'''#!/bin/bash
# Compile and test this function
# Run from this directory or pass the directory as argument

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR"

# Compiler settings (verified for this category)
CC="tools/ido-static-recomp/build/out/cc"
FLAGS="{flags}"

# Check if we're on the build machine
if [ ! -f "${{DIR}}/../../../../$CC" ]; then
    echo "ERROR: Must run on watchman (x86 build machine)"
    echo "Run: ssh watchman"
    echo "Then: cd /home/cburnes/projects/rush2049-decomp && $0"
    exit 1
fi

cd "${{DIR}}/../../../.."

# Compile
echo "Compiling base.c..."
$CC $FLAGS -c -o /tmp/func_test.o "$DIR/base.c" 2>&1

if [ $? -ne 0 ]; then
    echo "COMPILE FAILED"
    exit 1
fi

# Disassemble
echo ""
echo "=== COMPILED OUTPUT ==="
mips-linux-gnu-objdump -d /tmp/func_test.o

echo ""
echo "=== TARGET ASSEMBLY ==="
cat "$DIR/target.s"

echo ""
echo "Use tools/asmdiff.py for side-by-side comparison:"
echo "  python3 tools/asmdiff.py $DIR"
'''

def get_readme(func_name, addr, category, flags, comment, status, target_asm):
    """Generate comprehensive README.md for a function."""

    # Determine arcade reference hint
    arcade_hint = ""
    if "render" in func_name.lower() or "draw" in func_name.lower():
        arcade_hint = "Check `reference/repos/rushtherock/game/visuals.c`"
    elif "sound" in func_name.lower() or "audio" in func_name.lower():
        arcade_hint = "Check `reference/repos/rushtherock/game/audio.c`"
    elif "drone" in func_name.lower() or "ai" in func_name.lower():
        arcade_hint = "Check `reference/repos/rushtherock/game/drones.c`"
    elif "menu" in func_name.lower():
        arcade_hint = "Check `reference/repos/rushtherock/game/menus.c`"
    elif "checkpoint" in func_name.lower():
        arcade_hint = "Check `reference/repos/rushtherock/game/checkpoint.c`"
    elif "car" in func_name.lower() or "vehicle" in func_name.lower():
        arcade_hint = "Check `reference/repos/rushtherock/game/cars.c`"
    elif "physics" in func_name.lower() or "collision" in func_name.lower():
        arcade_hint = "Check `reference/repos/rushtherock/game/collision.c`"
    elif "tire" in func_name.lower():
        arcade_hint = "Check `reference/repos/rushtherock/game/tires.c`"
    elif category.startswith("libultra"):
        arcade_hint = "Standard N64 libultra function - check other decomp projects (sm64, oot)"
    elif category == "libc":
        arcade_hint = "Standard C library function - should match common implementations"

    # Count instructions in target
    instr_count = len([l for l in target_asm.split('\n') if '/*' in l and '*/' in l])

    return f'''# {func_name}

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `{addr}` |
| **Category** | `{category}` |
| **Status** | `{status}` |
| **Instructions** | ~{instr_count} |

## Description

{comment if comment else "No description available."}

## Compiler Settings

```bash
{flags}
```

These flags were determined by matching other functions in the `{category}` category.

## Files in This Directory

| File | Purpose |
|------|---------|
| `README.md` | This file - all context needed to work on this function |
| `base.c` | Your C implementation (edit this!) |
| `target.s` | Target assembly to match |
| `types.h` | Common type definitions |
| `compile.sh` | Script to compile and compare |
| `STATUS` | Current status (TODO/WIP/MATCHING) |

## How to Match This Function

### Step 1: Understand the Target

Look at `target.s` - this is the assembly your C code must produce.

Key things to note:
- Function prologue/epilogue (stack frame size)
- Register usage patterns
- Branch instructions (beq vs bne vs beql)
- Memory access patterns

### Step 2: Write Your Implementation

Edit `base.c` with your C implementation. Include `types.h` for common types.

```c
#include "types.h"

// Your implementation here
```

### Step 3: Compile and Compare

```bash
# On watchman (x86 build machine):
cd /home/cburnes/projects/rush2049-decomp
./work/{category}/{func_name}/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/{category}/{func_name}

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/{category}/{func_name} --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

{arcade_hint if arcade_hint else "No specific reference available."}

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
{target_asm}
```

## Tips for This Category

'''
    # Add category-specific tips
    if category == "libc":
        return readme + '''
### libc Functions

- Usually simple, straightforward implementations
- Match standard C library behavior
- Compiler uses `-O2` optimization
- Watch for loop unrolling differences
'''
    elif category == "libm":
        return readme + '''
### libm (Math) Functions

- Floating-point operations
- May use FPU registers ($f0-$f31)
- Compiler uses `-O2` optimization
- Check for polynomial approximations (sinf, cosf)
'''
    elif category.startswith("libultra"):
        if "os" in category:
            return readme + '''
### libultra/os Functions

- **Uses `-O1` optimization** (not -O2!)
- Often accesses hardware registers
- May disable/restore interrupts
- Message queue operations are common
'''
        elif "gu" in category:
            return readme + '''
### libultra/gu Functions

- Graphics utility functions
- Matrix operations (4x4 floats)
- Uses `-O2` optimization
- Often has loop-based patterns
'''
        else:
            return readme + '''
### libultra Functions

- Nintendo 64 SDK functions
- Usually uses `-O1` optimization
- Check sm64/oot decomps for reference implementations
'''
    elif category.startswith("game"):
        return readme + '''
### Game Functions

- Custom game logic
- Uses `-O2` optimization (assumed)
- Check arcade source for similar functions
- May reference global game state variables
'''
    else:
        return readme + '''
### General Tips

- Start by understanding what the function does
- Match the stack frame size first
- Pay attention to register allocation
- Branch-likely (beql/bnel) indicates -O2
'''

def main():
    print("Enriching work directories with self-contained documentation...")

    # Load symbol info for comments
    symbol_comments = {}
    with open(SYMBOL_FILE, 'r') as f:
        for line in f:
            if 'type:func' in line:
                match = re.match(r'(\w+)\s*=\s*(0x[0-9A-Fa-f]+);\s*//\s*type:func\s*(.*)', line.strip())
                if match:
                    name, addr, comment = match.groups()
                    symbol_comments[name] = comment.strip()

    count = 0
    for status_file in WORK_DIR.rglob("STATUS"):
        func_dir = status_file.parent
        func_name = func_dir.name

        # Read existing files
        status = status_file.read_text().strip()

        info_file = func_dir / "info.txt"
        info = {}
        if info_file.exists():
            for line in info_file.read_text().split('\n'):
                if ':' in line:
                    k, v = line.split(':', 1)
                    info[k.strip()] = v.strip()

        target_file = func_dir / "target.s"
        target_asm = target_file.read_text() if target_file.exists() else "# No target assembly available"

        # Get values
        addr = info.get('address', '0x????????')
        category = info.get('category', 'unknown')
        flags = info.get('compiler_flags', '-g0 -O2 -mips2 -G 0 -non_shared')
        comment = info.get('comment', '') or symbol_comments.get(func_name, '')

        # Create README.md
        readme = get_readme(func_name, addr, category, flags, comment, status, target_asm)
        (func_dir / "README.md").write_text(readme)

        # Create types.h
        (func_dir / "types.h").write_text(TYPES_H)

        # Create compile.sh
        compile_script = get_compile_script(category, flags)
        compile_sh = func_dir / "compile.sh"
        compile_sh.write_text(compile_script)
        compile_sh.chmod(0o755)

        count += 1
        if count % 100 == 0:
            print(f"  Progress: {count} directories enriched")

    print(f"\nDone! Enriched {count} function directories")
    print("\nEach directory now contains:")
    print("  - README.md (comprehensive documentation)")
    print("  - types.h (common type definitions)")
    print("  - compile.sh (build and test script)")
    print("  - base.c (implementation template)")
    print("  - target.s (assembly to match)")
    print("  - STATUS (current status)")

if __name__ == "__main__":
    main()
