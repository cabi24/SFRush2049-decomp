# __scHandleRDP

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80000ACC` |
| **Category** | `scheduler` |
| **Status** | `TODO` |
| **Instructions** | ~54 |

## Description

DP interrupt handler, reschedules graphics

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `scheduler` category.

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
./work/scheduler/__scHandleRDP/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/scheduler/__scHandleRDP

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/scheduler/__scHandleRDP --watch
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
# Address: 0x80000ACC

glabel func_80000ACC
    /* 16CC 80000ACC 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 16D0 80000AD0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 16D4 80000AD4 AFA40028 */  sw         $a0, 0x28($sp)
    /* 16D8 80000AD8 AFA00020 */  sw         $zero, 0x20($sp)
    /* 16DC 80000ADC AFA0001C */  sw         $zero, 0x1C($sp)
    /* 16E0 80000AE0 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 16E4 80000AE4 8DCF0278 */  lw         $t7, 0x278($t6)
    /* 16E8 80000AE8 15E00001 */  bnez       $t7, .L80000AF0
    /* 16EC 80000AEC 00000000 */   nop
  .L80000AF0:
    /* 16F0 80000AF0 8FB80028 */  lw         $t8, 0x28($sp)
    /* 16F4 80000AF4 24010001 */  addiu      $at, $zero, 0x1
    /* 16F8 80000AF8 8F190278 */  lw         $t9, 0x278($t8)
    /* 16FC 80000AFC 8F280010 */  lw         $t0, 0x10($t9)
    /* 1700 80000B00 11010001 */  beq        $t0, $at, .L80000B08
    /* 1704 80000B04 00000000 */   nop
  .L80000B08:
    /* 1708 80000B08 8FA90028 */  lw         $t1, 0x28($sp)
    /* 170C 80000B0C 8D2A0278 */  lw         $t2, 0x278($t1)
    /* 1710 80000B10 AFAA0024 */  sw         $t2, 0x24($sp)
    /* 1714 80000B14 8FAB0028 */  lw         $t3, 0x28($sp)
    /* 1718 80000B18 AD600278 */  sw         $zero, 0x278($t3)
    /* 171C 80000B1C 8FAC0024 */  lw         $t4, 0x24($sp)
    /* 1720 80000B20 2401FFFE */  addiu      $at, $zero, -0x2
    /* 1724 80000B24 8D8D0004 */  lw         $t5, 0x4($t4)
    /* 1728 80000B28 01A17024 */  and        $t6, $t5, $at
    /* 172C 80000B2C AD8E0004 */  sw         $t6, 0x4($t4)
    /* 1730 80000B30 8FA40028 */  lw         $a0, 0x28($sp)
    /* 1734 80000B34 0C000315 */  jal        func_80000C54
    /* 1738 80000B38 8FA50024 */   lw        $a1, 0x24($sp)
    /* 173C 80000B3C 8FAF0028 */  lw         $t7, 0x28($sp)
    /* 1740 80000B40 8DF80274 */  lw         $t8, 0x274($t7)
    /* 1744 80000B44 8DE90278 */  lw         $t1, 0x278($t7)
    /* 1748 80000B48 2F190001 */  sltiu      $t9, $t8, 0x1
    /* 174C 80000B4C 00194040 */  sll        $t0, $t9, 1
    /* 1750 80000B50 2D2A0001 */  sltiu      $t2, $t1, 0x1
    /* 1754 80000B54 010A5825 */  or         $t3, $t0, $t2
    /* 1758 80000B58 AFAB0018 */  sw         $t3, 0x18($sp)
    /* 175C 80000B5C 8FA40028 */  lw         $a0, 0x28($sp)
    /* 1760 80000B60 27A50020 */  addiu      $a1, $sp, 0x20
    /* 1764 80000B64 27A6001C */  addiu      $a2, $sp, 0x1C
    /* 1768 80000B68 0C0003FA */  jal        func_80000FE8
    /* 176C 80000B6C 8FA70018 */   lw        $a3, 0x18($sp)
    /* 1770 80000B70 8FAD0018 */  lw         $t5, 0x18($sp)
    /* 1774 80000B74 104D0005 */  beq        $v0, $t5, .L80000B8C
    /* 1778 80000B78 00000000 */   nop
    /* 177C 80000B7C 8FA40028 */  lw         $a0, 0x28($sp)
    /* 1780 80000B80 8FA50020 */  lw         $a1, 0x20($sp)
    /* 1784 80000B84 0C000388 */  jal        func_80000E20
    /* 1788 80000B88 8FA6001C */   lw        $a2, 0x1C($sp)
  .L80000B8C:
    /* 178C 80000B8C 10000001 */  b          .L80000B94
    /* 1790 80000B90 00000000 */   nop
  .L80000B94:
    /* 1794 80000B94 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1798 80000B98 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 179C 80000B9C 03E00008 */  jr         $ra
    /* 17A0 80000BA0 00000000 */   nop
```

## Tips for This Category

