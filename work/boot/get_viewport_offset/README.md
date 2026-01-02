# get_viewport_offset

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80001ECC` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~24 |

## Description

get viewport offset from base

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
./work/boot/get_viewport_offset/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/get_viewport_offset

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/get_viewport_offset --watch
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
# Address: 0x80001ECC

glabel func_80001ECC
    /* 2ACC 80001ECC 3C0F8003 */  lui        $t7, %hi(gViewportX)
    /* 2AD0 80001ED0 85EFEBF8 */  lh         $t7, %lo(gViewportX)($t7)
    /* 2AD4 80001ED4 3C198003 */  lui        $t9, %hi(gViewportOffsetX)
    /* 2AD8 80001ED8 3C0E8003 */  lui        $t6, %hi(gViewportScaleX)
    /* 2ADC 80001EDC 000FC0C0 */  sll        $t8, $t7, 3
    /* 2AE0 80001EE0 0338C821 */  addu       $t9, $t9, $t8
    /* 2AE4 80001EE4 8739AFFC */  lh         $t9, %lo(gViewportOffsetX)($t9)
    /* 2AE8 80001EE8 85CEECB0 */  lh         $t6, %lo(gViewportScaleX)($t6)
    /* 2AEC 80001EEC 01D94023 */  subu       $t0, $t6, $t9
    /* 2AF0 80001EF0 AC880000 */  sw         $t0, 0x0($a0)
    /* 2AF4 80001EF4 3C0A8003 */  lui        $t2, %hi(gViewportX)
    /* 2AF8 80001EF8 854AEBF8 */  lh         $t2, %lo(gViewportX)($t2)
    /* 2AFC 80001EFC 3C0C8003 */  lui        $t4, %hi(gViewportOffsetY)
    /* 2B00 80001F00 3C098003 */  lui        $t1, %hi(gViewportScaleY)
    /* 2B04 80001F04 000A58C0 */  sll        $t3, $t2, 3
    /* 2B08 80001F08 018B6021 */  addu       $t4, $t4, $t3
    /* 2B0C 80001F0C 858CAFFE */  lh         $t4, %lo(gViewportOffsetY)($t4)
    /* 2B10 80001F10 8529ECB2 */  lh         $t1, %lo(gViewportScaleY)($t1)
    /* 2B14 80001F14 012C6823 */  subu       $t5, $t1, $t4
    /* 2B18 80001F18 ACAD0000 */  sw         $t5, 0x0($a1)
    /* 2B1C 80001F1C 03E00008 */  jr         $ra
    /* 2B20 80001F20 00000000 */   nop
    /* 2B24 80001F24 03E00008 */  jr         $ra
    /* 2B28 80001F28 00000000 */   nop
```

## Tips for This Category

