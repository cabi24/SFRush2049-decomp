# osInvalICache_full

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80008460` |
| **Category** | `libultra/cache` |
| **Status** | `TODO` |
| **Instructions** | ~29 |

## Description

invalidate instruction cache (cache op 0x00)

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/cache` category.

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
./work/libultra/cache/osInvalICache_full/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/cache/osInvalICache_full

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/cache/osInvalICache_full --watch
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
# Source: 8E10.s
# Address: 0x80008460

glabel func_80008460
    /* 9060 80008460 18A00011 */  blez       $a1, .L800084A8
    /* 9064 80008464 00000000 */   nop
    /* 9068 80008468 240B4000 */  addiu      $t3, $zero, 0x4000
    /* 906C 8000846C 00AB082B */  sltu       $at, $a1, $t3
    /* 9070 80008470 1020000F */  beqz       $at, .L800084B0
    /* 9074 80008474 00000000 */   nop
    /* 9078 80008478 00804025 */  or         $t0, $a0, $zero
    /* 907C 8000847C 00854821 */  addu       $t1, $a0, $a1
    /* 9080 80008480 0109082B */  sltu       $at, $t0, $t1
    /* 9084 80008484 10200008 */  beqz       $at, .L800084A8
    /* 9088 80008488 00000000 */   nop
    /* 908C 8000848C 310A001F */  andi       $t2, $t0, 0x1F
    /* 9090 80008490 2529FFE0 */  addiu      $t1, $t1, -0x20
    /* 9094 80008494 010A4023 */  subu       $t0, $t0, $t2
  .L80008498:
    /* 9098 80008498 BD100000 */  cache      0x10, 0x0($t0) /* handwritten instruction */
    /* 909C 8000849C 0109082B */  sltu       $at, $t0, $t1
    /* 90A0 800084A0 1420FFFD */  bnez       $at, .L80008498
    /* 90A4 800084A4 25080020 */   addiu     $t0, $t0, 0x20
  .L800084A8:
    /* 90A8 800084A8 03E00008 */  jr         $ra
    /* 90AC 800084AC 00000000 */   nop
  .L800084B0:
    /* 90B0 800084B0 3C088000 */  lui        $t0, 0x8000
    /* 90B4 800084B4 010B4821 */  addu       $t1, $t0, $t3
    /* 90B8 800084B8 2529FFE0 */  addiu      $t1, $t1, -0x20
  .L800084BC:
    /* 90BC 800084BC BD000000 */  cache      0x00, 0x0($t0) /* handwritten instruction */
    /* 90C0 800084C0 0109082B */  sltu       $at, $t0, $t1
    /* 90C4 800084C4 1420FFFD */  bnez       $at, .L800084BC
    /* 90C8 800084C8 25080020 */   addiu     $t0, $t0, (0x80000020 & 0xFFFF)
    /* 90CC 800084CC 03E00008 */  jr         $ra
    /* 90D0 800084D0 00000000 */   nop
```

## Tips for This Category

