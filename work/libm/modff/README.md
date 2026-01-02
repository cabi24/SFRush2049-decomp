# modff

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80002A64` |
| **Category** | `libm` |
| **Status** | `TODO` |
| **Instructions** | ~99 |

## Description

float version of modf

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libm` category.

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
./work/libm/modff/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libm/modff

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libm/modff --watch
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
# Source: 34A0.s
# Address: 0x80002A64

glabel func_80002A64
    /* 3664 80002A64 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* 3668 80002A68 E7AC0004 */  swc1       $fa0, 0x4($sp)
    /* 366C 80002A6C C7A40004 */  lwc1       $ft0, 0x4($sp)
    /* 3670 80002A70 44803000 */  mtc1       $zero, $ft1
    /* 3674 80002A74 00000000 */  nop
    /* 3678 80002A78 4604303C */  c.lt.s     $ft1, $ft0
    /* 367C 80002A7C 00000000 */  nop
    /* 3680 80002A80 45000003 */  bc1f       .L80002A90
    /* 3684 80002A84 00000000 */   nop
    /* 3688 80002A88 10000003 */  b          .L80002A98
    /* 368C 80002A8C 46002386 */   mov.s     $fa1, $ft0
  .L80002A90:
    /* 3690 80002A90 C7AE0004 */  lwc1       $fa1, 0x4($sp)
    /* 3694 80002A94 46007387 */  neg.s      $fa1, $fa1
  .L80002A98:
    /* 3698 80002A98 3C014B00 */  lui        $at, (0x4B000000 >> 16)
    /* 369C 80002A9C 44814000 */  mtc1       $at, $ft2
    /* 36A0 80002AA0 00000000 */  nop
    /* 36A4 80002AA4 460E403E */  c.le.s     $ft2, $fa1
    /* 36A8 80002AA8 00000000 */  nop
    /* 36AC 80002AAC 45000008 */  bc1f       .L80002AD0
    /* 36B0 80002AB0 00000000 */   nop
    /* 36B4 80002AB4 C7AA0004 */  lwc1       $ft3, 0x4($sp)
    /* 36B8 80002AB8 E4AA0000 */  swc1       $ft3, 0x0($a1)
    /* 36BC 80002ABC 44800000 */  mtc1       $zero, $fv0
    /* 36C0 80002AC0 10000049 */  b          .L80002BE8
    /* 36C4 80002AC4 00000000 */   nop
    /* 36C8 80002AC8 10000021 */  b          .L80002B50
    /* 36CC 80002ACC 00000000 */   nop
  .L80002AD0:
    /* 36D0 80002AD0 3C014B00 */  lui        $at, (0x4B000000 >> 16)
    /* 36D4 80002AD4 44818000 */  mtc1       $at, $ft4
    /* 36D8 80002AD8 00000000 */  nop
    /* 36DC 80002ADC 46107480 */  add.s      $ft5, $fa1, $ft4
    /* 36E0 80002AE0 E7B20008 */  swc1       $ft5, 0x8($sp)
    /* 36E4 80002AE4 3C014B00 */  lui        $at, (0x4B000000 >> 16)
    /* 36E8 80002AE8 44812000 */  mtc1       $at, $ft0
    /* 36EC 80002AEC C7A60008 */  lwc1       $ft1, 0x8($sp)
    /* 36F0 80002AF0 46043201 */  sub.s      $ft2, $ft1, $ft0
    /* 36F4 80002AF4 E7A80008 */  swc1       $ft2, 0x8($sp)
    /* 36F8 80002AF8 C7AA0008 */  lwc1       $ft3, 0x8($sp)
    /* 36FC 80002AFC 460A703C */  c.lt.s     $fa1, $ft3
    /* 3700 80002B00 00000000 */  nop
    /* 3704 80002B04 45000006 */  bc1f       .L80002B20
    /* 3708 80002B08 00000000 */   nop
    /* 370C 80002B0C 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 3710 80002B10 44819000 */  mtc1       $at, $ft5
    /* 3714 80002B14 C7B00008 */  lwc1       $ft4, 0x8($sp)
    /* 3718 80002B18 46128181 */  sub.s      $ft1, $ft4, $ft5
    /* 371C 80002B1C E7A60008 */  swc1       $ft1, 0x8($sp)
  .L80002B20:
    /* 3720 80002B20 C7A40008 */  lwc1       $ft0, 0x8($sp)
    /* 3724 80002B24 44804000 */  mtc1       $zero, $ft2
    /* 3728 80002B28 00000000 */  nop
    /* 372C 80002B2C 4604403C */  c.lt.s     $ft2, $ft0
    /* 3730 80002B30 00000000 */  nop
    /* 3734 80002B34 45000003 */  bc1f       .L80002B44
    /* 3738 80002B38 00000000 */   nop
    /* 373C 80002B3C 10000004 */  b          .L80002B50
    /* 3740 80002B40 00000000 */   nop
  .L80002B44:
    /* 3744 80002B44 C7AA0008 */  lwc1       $ft3, 0x8($sp)
    /* 3748 80002B48 46005407 */  neg.s      $ft4, $ft3
    /* 374C 80002B4C E7B00008 */  swc1       $ft4, 0x8($sp)
  .L80002B50:
    /* 3750 80002B50 C7B20004 */  lwc1       $ft5, 0x4($sp)
    /* 3754 80002B54 C7A60008 */  lwc1       $ft1, 0x8($sp)
    /* 3758 80002B58 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 375C 80002B5C 44814000 */  mtc1       $at, $ft2
    /* 3760 80002B60 46069101 */  sub.s      $ft0, $ft5, $ft1
    /* 3764 80002B64 46082032 */  c.eq.s     $ft0, $ft2
    /* 3768 80002B68 00000000 */  nop
    /* 376C 80002B6C 45000006 */  bc1f       .L80002B88
    /* 3770 80002B70 00000000 */   nop
    /* 3774 80002B74 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 3778 80002B78 44818000 */  mtc1       $at, $ft4
    /* 377C 80002B7C C7AA0008 */  lwc1       $ft3, 0x8($sp)
    /* 3780 80002B80 46105480 */  add.s      $ft5, $ft3, $ft4
    /* 3784 80002B84 E7B20008 */  swc1       $ft5, 0x8($sp)
  .L80002B88:
    /* 3788 80002B88 C7A60004 */  lwc1       $ft1, 0x4($sp)
    /* 378C 80002B8C 44802000 */  mtc1       $zero, $ft0
    /* 3790 80002B90 00000000 */  nop
    /* 3794 80002B94 4606203E */  c.le.s     $ft0, $ft1
    /* 3798 80002B98 00000000 */  nop
    /* 379C 80002B9C 45000009 */  bc1f       .L80002BC4
    /* 37A0 80002BA0 00000000 */   nop
    /* 37A4 80002BA4 C7A80008 */  lwc1       $ft2, 0x8($sp)
    /* 37A8 80002BA8 E4A80000 */  swc1       $ft2, 0x0($a1)
    /* 37AC 80002BAC C7AA0004 */  lwc1       $ft3, 0x4($sp)
    /* 37B0 80002BB0 C7B00008 */  lwc1       $ft4, 0x8($sp)
    /* 37B4 80002BB4 1000000C */  b          .L80002BE8
    /* 37B8 80002BB8 46105001 */   sub.s     $fv0, $ft3, $ft4
    /* 37BC 80002BBC 10000008 */  b          .L80002BE0
    /* 37C0 80002BC0 00000000 */   nop
  .L80002BC4:
    /* 37C4 80002BC4 C7B20008 */  lwc1       $ft5, 0x8($sp)
    /* 37C8 80002BC8 46009187 */  neg.s      $ft1, $ft5
    /* 37CC 80002BCC E4A60000 */  swc1       $ft1, 0x0($a1)
    /* 37D0 80002BD0 C7A40004 */  lwc1       $ft0, 0x4($sp)
    /* 37D4 80002BD4 C7A80008 */  lwc1       $ft2, 0x8($sp)
    /* 37D8 80002BD8 10000003 */  b          .L80002BE8
    /* 37DC 80002BDC 46082000 */   add.s     $fv0, $ft0, $ft2
  .L80002BE0:
    /* 37E0 80002BE0 10000001 */  b          .L80002BE8
    /* 37E4 80002BE4 00000000 */   nop
  .L80002BE8:
    /* 37E8 80002BE8 03E00008 */  jr         $ra
    /* 37EC 80002BEC 27BD0010 */   addiu     $sp, $sp, 0x10
```

## Tips for This Category

