# viAddTicks

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000153C` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~15 |

## Description

add ticks to deadline

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
./work/boot/viAddTicks/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/viAddTicks

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/viAddTicks --watch
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
# Address: 0x8000153C

glabel func_8000153C
    /* 213C 8000153C 3C018003 */  lui        $at, %hi(D_8002AFB4)
    /* 2140 80001540 C424AFB4 */  lwc1       $ft0, %lo(D_8002AFB4)($at)
    /* 2144 80001544 3C0E8003 */  lui        $t6, %hi(D_8002EB9C)
    /* 2148 80001548 8DCEEB9C */  lw         $t6, %lo(D_8002EB9C)($t6)
    /* 214C 8000154C 46046182 */  mul.s      $ft1, $fa0, $ft0
    /* 2150 80001550 3C018003 */  lui        $at, %hi(D_8002EB9C)
    /* 2154 80001554 4600320D */  trunc.w.s  $ft2, $ft1
    /* 2158 80001558 44184000 */  mfc1       $t8, $ft2
    /* 215C 8000155C 00000000 */  nop
    /* 2160 80001560 01D8C821 */  addu       $t9, $t6, $t8
    /* 2164 80001564 AC39EB9C */  sw         $t9, %lo(D_8002EB9C)($at)
    /* 2168 80001568 03E00008 */  jr         $ra
    /* 216C 8000156C 00000000 */   nop
    /* 2170 80001570 03E00008 */  jr         $ra
    /* 2174 80001574 00000000 */   nop
```

## Tips for This Category

