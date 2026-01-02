# osCreateMesgQueue

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80006A00` |
| **Category** | `libultra/os` |
| **Status** | `MATCHING` |
| **Instructions** | ~11 |

## Description

No description available.

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/os` category.

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
./work/libultra/os/osCreateMesgQueue/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osCreateMesgQueue

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osCreateMesgQueue --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Standard N64 libultra function - check other decomp projects (sm64, oot)

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 7600.s
# Address: 0x80006A00

glabel func_80006A00
    /* 7600 80006A00 3C0E8003 */  lui        $t6, %hi(D_8002C3D0)
    /* 7604 80006A04 3C0F8003 */  lui        $t7, %hi(D_8002C3D0)
    /* 7608 80006A08 25CEC3D0 */  addiu      $t6, $t6, %lo(D_8002C3D0)
    /* 760C 80006A0C 25EFC3D0 */  addiu      $t7, $t7, %lo(D_8002C3D0)
    /* 7610 80006A10 AC8E0000 */  sw         $t6, 0x0($a0)
    /* 7614 80006A14 AC8F0004 */  sw         $t7, 0x4($a0)
    /* 7618 80006A18 AC800008 */  sw         $zero, 0x8($a0)
    /* 761C 80006A1C AC80000C */  sw         $zero, 0xC($a0)
    /* 7620 80006A20 AC860010 */  sw         $a2, 0x10($a0)
    /* 7624 80006A24 03E00008 */  jr         $ra
    /* 7628 80006A28 AC850014 */   sw        $a1, 0x14($a0)
```

## Tips for This Category

