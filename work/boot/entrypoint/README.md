# entrypoint

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80000400` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~14 |

## Description

clears BSS, sets stack, jumps to main

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
./work/boot/entrypoint/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/entrypoint

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/entrypoint --watch
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
# Source: 1000.s
# Address: 0x80000400

glabel func_80000400
    /* 1000 80000400 3C088003 */  lui        $t0, %hi(D_8002E8E0)
    /* 1004 80000404 3C090005 */  lui        $t1, (0x58170 >> 16)
    /* 1008 80000408 2508E8E0 */  addiu      $t0, $t0, %lo(D_8002E8E0)
    /* 100C 8000040C 35298170 */  ori        $t1, $t1, (0x58170 & 0xFFFF)
  .L80000410:
    /* 1010 80000410 2129FFF8 */  addi       $t1, $t1, -0x8 /* handwritten instruction */
    /* 1014 80000414 AD000000 */  sw         $zero, 0x0($t0)
    /* 1018 80000418 AD000004 */  sw         $zero, 0x4($t0)
    /* 101C 8000041C 1520FFFC */  bnez       $t1, .L80000410
    /* 1020 80000420 21080008 */   addi      $t0, $t0, 0x8 /* handwritten instruction */
    /* 1024 80000424 3C0A8000 */  lui        $t2, %hi(func_800020F0)
    /* 1028 80000428 3C1D8008 */  lui        $sp, %hi(D_80086A50)
    /* 102C 8000042C 254A20F0 */  addiu      $t2, $t2, %lo(func_800020F0)
    /* 1030 80000430 01400008 */  jr         $t2
    /* 1034 80000434 27BD6A50 */   addiu     $sp, $sp, %lo(D_80086A50)
```

## Tips for This Category

