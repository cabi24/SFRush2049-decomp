# modf

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800028A0` |
| **Category** | `libm` |
| **Status** | `TODO` |
| **Instructions** | ~113 |

## Description

split double into integer and fractional parts

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
./work/libm/modf/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libm/modf

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libm/modf --watch
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
# Address: 0x800028A0

glabel func_800028A0
    /* 34A0 800028A0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 34A4 800028A4 F7AC0000 */  sdc1       $fa0, 0x0($sp)
    /* 34A8 800028A8 D7A40000 */  ldc1       $ft0, 0x0($sp)
    /* 34AC 800028AC 44803800 */  mtc1       $zero, $ft1f
    /* 34B0 800028B0 44803000 */  mtc1       $zero, $ft1
    /* 34B4 800028B4 00000000 */  nop
    /* 34B8 800028B8 4624303C */  c.lt.d     $ft1, $ft0
    /* 34BC 800028BC 00000000 */  nop
    /* 34C0 800028C0 45000003 */  bc1f       .L800028D0
    /* 34C4 800028C4 00000000 */   nop
    /* 34C8 800028C8 10000003 */  b          .L800028D8
    /* 34CC 800028CC 46202386 */   mov.d     $fa1, $ft0
  .L800028D0:
    /* 34D0 800028D0 D7AE0000 */  ldc1       $fa1, 0x0($sp)
    /* 34D4 800028D4 46207387 */  neg.d      $fa1, $fa1
  .L800028D8:
    /* 34D8 800028D8 3C014330 */  lui        $at, (0x43300000 >> 16)
    /* 34DC 800028DC 44814800 */  mtc1       $at, $ft2f
    /* 34E0 800028E0 44804000 */  mtc1       $zero, $ft2
    /* 34E4 800028E4 00000000 */  nop
    /* 34E8 800028E8 462E403E */  c.le.d     $ft2, $fa1
    /* 34EC 800028EC 00000000 */  nop
    /* 34F0 800028F0 45000009 */  bc1f       .L80002918
    /* 34F4 800028F4 00000000 */   nop
    /* 34F8 800028F8 D7AA0000 */  ldc1       $ft3, 0x0($sp)
    /* 34FC 800028FC F4CA0000 */  sdc1       $ft3, 0x0($a2)
    /* 3500 80002900 44800800 */  mtc1       $zero, $fv0f
    /* 3504 80002904 44800000 */  mtc1       $zero, $fv0
    /* 3508 80002908 10000054 */  b          .L80002A5C
    /* 350C 8000290C 00000000 */   nop
    /* 3510 80002910 10000027 */  b          .L800029B0
    /* 3514 80002914 00000000 */   nop
  .L80002918:
    /* 3518 80002918 3C014330 */  lui        $at, (0x43300000 >> 16)
    /* 351C 8000291C 44818800 */  mtc1       $at, $ft4f
    /* 3520 80002920 44808000 */  mtc1       $zero, $ft4
    /* 3524 80002924 00000000 */  nop
    /* 3528 80002928 46307480 */  add.d      $ft5, $fa1, $ft4
    /* 352C 8000292C F7B20008 */  sdc1       $ft5, 0x8($sp)
    /* 3530 80002930 3C014330 */  lui        $at, (0x43300000 >> 16)
    /* 3534 80002934 44812800 */  mtc1       $at, $ft0f
    /* 3538 80002938 D7A60008 */  ldc1       $ft1, 0x8($sp)
    /* 353C 8000293C 44802000 */  mtc1       $zero, $ft0
    /* 3540 80002940 00000000 */  nop
    /* 3544 80002944 46243201 */  sub.d      $ft2, $ft1, $ft0
    /* 3548 80002948 F7A80008 */  sdc1       $ft2, 0x8($sp)
    /* 354C 8000294C D7AA0008 */  ldc1       $ft3, 0x8($sp)
    /* 3550 80002950 462A703C */  c.lt.d     $fa1, $ft3
    /* 3554 80002954 00000000 */  nop
    /* 3558 80002958 45000008 */  bc1f       .L8000297C
    /* 355C 8000295C 00000000 */   nop
    /* 3560 80002960 3C013FF0 */  lui        $at, (0x3FF00000 >> 16)
    /* 3564 80002964 44819800 */  mtc1       $at, $ft5f
    /* 3568 80002968 D7B00008 */  ldc1       $ft4, 0x8($sp)
    /* 356C 8000296C 44809000 */  mtc1       $zero, $ft5
    /* 3570 80002970 00000000 */  nop
    /* 3574 80002974 46328181 */  sub.d      $ft1, $ft4, $ft5
    /* 3578 80002978 F7A60008 */  sdc1       $ft1, 0x8($sp)
  .L8000297C:
    /* 357C 8000297C D7A40008 */  ldc1       $ft0, 0x8($sp)
    /* 3580 80002980 44804800 */  mtc1       $zero, $ft2f
    /* 3584 80002984 44804000 */  mtc1       $zero, $ft2
    /* 3588 80002988 00000000 */  nop
    /* 358C 8000298C 4624403C */  c.lt.d     $ft2, $ft0
    /* 3590 80002990 00000000 */  nop
    /* 3594 80002994 45000003 */  bc1f       .L800029A4
    /* 3598 80002998 00000000 */   nop
    /* 359C 8000299C 10000004 */  b          .L800029B0
    /* 35A0 800029A0 00000000 */   nop
  .L800029A4:
    /* 35A4 800029A4 D7AA0008 */  ldc1       $ft3, 0x8($sp)
    /* 35A8 800029A8 46205407 */  neg.d      $ft4, $ft3
    /* 35AC 800029AC F7B00008 */  sdc1       $ft4, 0x8($sp)
  .L800029B0:
    /* 35B0 800029B0 D7B20000 */  ldc1       $ft5, 0x0($sp)
    /* 35B4 800029B4 D7A60008 */  ldc1       $ft1, 0x8($sp)
    /* 35B8 800029B8 3C013FF0 */  lui        $at, (0x3FF00000 >> 16)
    /* 35BC 800029BC 44814800 */  mtc1       $at, $ft2f
    /* 35C0 800029C0 46269101 */  sub.d      $ft0, $ft5, $ft1
    /* 35C4 800029C4 44804000 */  mtc1       $zero, $ft2
    /* 35C8 800029C8 00000000 */  nop
    /* 35CC 800029CC 46282032 */  c.eq.d     $ft0, $ft2
    /* 35D0 800029D0 00000000 */  nop
    /* 35D4 800029D4 45000008 */  bc1f       .L800029F8
    /* 35D8 800029D8 00000000 */   nop
    /* 35DC 800029DC 3C013FF0 */  lui        $at, (0x3FF00000 >> 16)
    /* 35E0 800029E0 44818800 */  mtc1       $at, $ft4f
    /* 35E4 800029E4 D7AA0008 */  ldc1       $ft3, 0x8($sp)
    /* 35E8 800029E8 44808000 */  mtc1       $zero, $ft4
    /* 35EC 800029EC 00000000 */  nop
    /* 35F0 800029F0 46305480 */  add.d      $ft5, $ft3, $ft4
    /* 35F4 800029F4 F7B20008 */  sdc1       $ft5, 0x8($sp)
  .L800029F8:
    /* 35F8 800029F8 D7A60000 */  ldc1       $ft1, 0x0($sp)
    /* 35FC 800029FC 44802800 */  mtc1       $zero, $ft0f
    /* 3600 80002A00 44802000 */  mtc1       $zero, $ft0
    /* 3604 80002A04 00000000 */  nop
    /* 3608 80002A08 4626203E */  c.le.d     $ft0, $ft1
    /* 360C 80002A0C 00000000 */  nop
    /* 3610 80002A10 45000009 */  bc1f       .L80002A38
    /* 3614 80002A14 00000000 */   nop
    /* 3618 80002A18 D7A80008 */  ldc1       $ft2, 0x8($sp)
    /* 361C 80002A1C F4C80000 */  sdc1       $ft2, 0x0($a2)
    /* 3620 80002A20 D7AA0000 */  ldc1       $ft3, 0x0($sp)
    /* 3624 80002A24 D7B00008 */  ldc1       $ft4, 0x8($sp)
    /* 3628 80002A28 1000000C */  b          .L80002A5C
    /* 362C 80002A2C 46305001 */   sub.d     $fv0, $ft3, $ft4
    /* 3630 80002A30 10000008 */  b          .L80002A54
    /* 3634 80002A34 00000000 */   nop
  .L80002A38:
    /* 3638 80002A38 D7B20008 */  ldc1       $ft5, 0x8($sp)
    /* 363C 80002A3C 46209187 */  neg.d      $ft1, $ft5
    /* 3640 80002A40 F4C60000 */  sdc1       $ft1, 0x0($a2)
    /* 3644 80002A44 D7A40000 */  ldc1       $ft0, 0x0($sp)
    /* 3648 80002A48 D7A80008 */  ldc1       $ft2, 0x8($sp)
    /* 364C 80002A4C 10000003 */  b          .L80002A5C
    /* 3650 80002A50 46282000 */   add.d     $fv0, $ft0, $ft2
  .L80002A54:
    /* 3654 80002A54 10000001 */  b          .L80002A5C
    /* 3658 80002A58 00000000 */   nop
  .L80002A5C:
    /* 365C 80002A5C 03E00008 */  jr         $ra
    /* 3660 80002A60 27BD0018 */   addiu     $sp, $sp, 0x18
```

## Tips for This Category

