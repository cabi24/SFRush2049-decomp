# viScheduleTick

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800014F0` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~19 |

## Description

schedule next tick deadline

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
./work/boot/viScheduleTick/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/viScheduleTick

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/viScheduleTick --watch
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
# Source: 1050.s
# Address: 0x800014F0

glabel func_800014F0
    /* 20F0 800014F0 3C0E8003 */  lui        $t6, %hi(D_8002EB64)
    /* 20F4 800014F4 8DCEEB64 */  lw         $t6, %lo(D_8002EB64)($t6)
    /* 20F8 800014F8 3C018003 */  lui        $at, %hi(D_8002EBA0)
    /* 20FC 800014FC AC2EEBA0 */  sw         $t6, %lo(D_8002EBA0)($at)
    /* 2100 80001500 3C018003 */  lui        $at, %hi(D_8002AFB4)
    /* 2104 80001504 C424AFB4 */  lwc1       $ft0, %lo(D_8002AFB4)($at)
    /* 2108 80001508 3C198003 */  lui        $t9, %hi(D_8002EB64)
    /* 210C 8000150C 8F39EB64 */  lw         $t9, %lo(D_8002EB64)($t9)
    /* 2110 80001510 46046182 */  mul.s      $ft1, $fa0, $ft0
    /* 2114 80001514 3C018003 */  lui        $at, %hi(D_8002EB9C)
    /* 2118 80001518 4600320D */  trunc.w.s  $ft2, $ft1
    /* 211C 8000151C 44184000 */  mfc1       $t8, $ft2
    /* 2120 80001520 00000000 */  nop
    /* 2124 80001524 03194021 */  addu       $t0, $t8, $t9
    /* 2128 80001528 AC28EB9C */  sw         $t0, %lo(D_8002EB9C)($at)
    /* 212C 8000152C 03E00008 */  jr         $ra
    /* 2130 80001530 00000000 */   nop
    /* 2134 80001534 03E00008 */  jr         $ra
    /* 2138 80001538 00000000 */   nop
```

## Tips for This Category

