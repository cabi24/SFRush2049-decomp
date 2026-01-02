# get_viewport_pos

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80001E84` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~18 |

## Description

get current viewport position

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `boot` category.

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
./work/boot/get_viewport_pos/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/get_viewport_pos

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/get_viewport_pos --watch
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
# Source: 21F0.s
# Address: 0x80001E84

glabel func_80001E84
    /* 2A84 80001E84 3C0E8003 */  lui        $t6, %hi(gViewportX)
    /* 2A88 80001E88 85CEEBF8 */  lh         $t6, %lo(gViewportX)($t6)
    /* 2A8C 80001E8C 3C188003 */  lui        $t8, %hi(gViewportOffsetX)
    /* 2A90 80001E90 000E78C0 */  sll        $t7, $t6, 3
    /* 2A94 80001E94 030FC021 */  addu       $t8, $t8, $t7
    /* 2A98 80001E98 8718AFFC */  lh         $t8, %lo(gViewportOffsetX)($t8)
    /* 2A9C 80001E9C AC980000 */  sw         $t8, 0x0($a0)
    /* 2AA0 80001EA0 3C198003 */  lui        $t9, %hi(gViewportX)
    /* 2AA4 80001EA4 8739EBF8 */  lh         $t9, %lo(gViewportX)($t9)
    /* 2AA8 80001EA8 3C098003 */  lui        $t1, %hi(gViewportOffsetY)
    /* 2AAC 80001EAC 001940C0 */  sll        $t0, $t9, 3
    /* 2AB0 80001EB0 01284821 */  addu       $t1, $t1, $t0
    /* 2AB4 80001EB4 8529AFFE */  lh         $t1, %lo(gViewportOffsetY)($t1)
    /* 2AB8 80001EB8 ACA90000 */  sw         $t1, 0x0($a1)
    /* 2ABC 80001EBC 03E00008 */  jr         $ra
    /* 2AC0 80001EC0 00000000 */   nop
    /* 2AC4 80001EC4 03E00008 */  jr         $ra
    /* 2AC8 80001EC8 00000000 */   nop
```

## Tips for This Category

