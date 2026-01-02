# dll_get_priority

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000C490` |
| **Category** | `dll` |
| **Status** | `TODO` |
| **Instructions** | ~6 |

## Description

get thread priority helper

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `dll` category.

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
./work/dll/dll_get_priority/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/dll/dll_get_priority

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/dll/dll_get_priority --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

No specific reference available.

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: CC50.s
# Address: 0x8000C490

glabel func_8000C490
    /* D090 8000C490 14800003 */  bnez       $a0, .L8000C4A0
    /* D094 8000C494 00000000 */   nop
    /* D098 8000C498 3C048003 */  lui        $a0, %hi(D_8002C3E0)
    /* D09C 8000C49C 8C84C3E0 */  lw         $a0, %lo(D_8002C3E0)($a0)
  .L8000C4A0:
    /* D0A0 8000C4A0 03E00008 */  jr         $ra
    /* D0A4 8000C4A4 8C820004 */   lw        $v0, 0x4($a0)
```

## Tips for This Category

