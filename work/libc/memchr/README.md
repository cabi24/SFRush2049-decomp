# memchr

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80002730` |
| **Category** | `libc` |
| **Status** | `TODO` |
| **Instructions** | ~23 |

## Description

No description available.

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libc` category.

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
./work/libc/memchr/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libc/memchr

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libc/memchr --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Standard C library function - should match common implementations

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 3330.s
# Address: 0x80002730

glabel func_80002730
    /* 3330 80002730 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 3334 80002734 AFA40004 */  sw         $a0, 0x4($sp)
    /* 3338 80002738 00C03825 */  or         $a3, $a2, $zero
    /* 333C 8000273C 10E0000D */  beqz       $a3, .L80002774
    /* 3340 80002740 24C6FFFF */   addiu     $a2, $a2, -0x1
  .L80002744:
    /* 3344 80002744 8FAE0004 */  lw         $t6, 0x4($sp)
    /* 3348 80002748 91CF0000 */  lbu        $t7, 0x0($t6)
    /* 334C 8000274C 15E50003 */  bne        $t7, $a1, .L8000275C
    /* 3350 80002750 00000000 */   nop
    /* 3354 80002754 1000000B */  b          .L80002784
    /* 3358 80002758 8FA20004 */   lw        $v0, 0x4($sp)
  .L8000275C:
    /* 335C 8000275C 8FB80004 */  lw         $t8, 0x4($sp)
    /* 3360 80002760 27190001 */  addiu      $t9, $t8, 0x1
    /* 3364 80002764 AFB90004 */  sw         $t9, 0x4($sp)
    /* 3368 80002768 00C03825 */  or         $a3, $a2, $zero
    /* 336C 8000276C 14E0FFF5 */  bnez       $a3, .L80002744
    /* 3370 80002770 24C6FFFF */   addiu     $a2, $a2, -0x1
  .L80002774:
    /* 3374 80002774 10000003 */  b          .L80002784
    /* 3378 80002778 00001025 */   or        $v0, $zero, $zero
    /* 337C 8000277C 10000001 */  b          .L80002784
    /* 3380 80002780 00000000 */   nop
  .L80002784:
    /* 3384 80002784 03E00008 */  jr         $ra
    /* 3388 80002788 27BD0008 */   addiu     $sp, $sp, 0x8
```

## Tips for This Category

