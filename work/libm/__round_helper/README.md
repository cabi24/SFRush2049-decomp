# __round_helper

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80004674` |
| **Category** | `libm` |
| **Status** | `TODO` |
| **Instructions** | ~129 |

## Description

rounding helper for float conversion

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
./work/libm/__round_helper/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libm/__round_helper

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libm/__round_helper --watch
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
# Address: 0x80004674

glabel func_80004674
    /* 5274 80004674 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 5278 80004678 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 527C 8000467C AFA60028 */  sw         $a2, 0x28($sp)
    /* 5280 80004680 AFA7002C */  sw         $a3, 0x2C($sp)
    /* 5284 80004684 F7AC0020 */  sdc1       $fa0, 0x20($sp)
    /* 5288 80004688 D7A40020 */  ldc1       $ft0, 0x20($sp)
    /* 528C 8000468C 44803800 */  mtc1       $zero, $ft1f
    /* 5290 80004690 44803000 */  mtc1       $zero, $ft1
    /* 5294 80004694 00000000 */  nop
    /* 5298 80004698 46262032 */  c.eq.d     $ft0, $ft1
    /* 529C 8000469C 00000000 */  nop
    /* 52A0 800046A0 4501000B */  bc1t       .L800046D0
    /* 52A4 800046A4 00000000 */   nop
    /* 52A8 800046A8 3C014024 */  lui        $at, (0x40240000 >> 16)
    /* 52AC 800046AC 44815800 */  mtc1       $at, $ft3f
    /* 52B0 800046B0 D7A80020 */  ldc1       $ft2, 0x20($sp)
    /* 52B4 800046B4 44805000 */  mtc1       $zero, $ft3
    /* 52B8 800046B8 27A60018 */  addiu      $a2, $sp, 0x18
    /* 52BC 800046BC 462A4302 */  mul.d      $fa0, $ft2, $ft3
    /* 52C0 800046C0 0C000A28 */  jal        func_800028A0
    /* 52C4 800046C4 00000000 */   nop
    /* 52C8 800046C8 10000007 */  b          .L800046E8
    /* 52CC 800046CC 00000000 */   nop
  .L800046D0:
    /* 52D0 800046D0 93AE0037 */  lbu        $t6, 0x37($sp)
    /* 52D4 800046D4 25CFFFD0 */  addiu      $t7, $t6, -0x30
    /* 52D8 800046D8 448F8000 */  mtc1       $t7, $ft4
    /* 52DC 800046DC 00000000 */  nop
    /* 52E0 800046E0 468084A1 */  cvt.d.w    $ft5, $ft4
    /* 52E4 800046E4 F7B20018 */  sdc1       $ft5, 0x18($sp)
  .L800046E8:
    /* 52E8 800046E8 3C014010 */  lui        $at, (0x40100000 >> 16)
    /* 52EC 800046EC 44813800 */  mtc1       $at, $ft1f
    /* 52F0 800046F0 D7A40018 */  ldc1       $ft0, 0x18($sp)
    /* 52F4 800046F4 44803000 */  mtc1       $zero, $ft1
    /* 52F8 800046F8 00000000 */  nop
    /* 52FC 800046FC 4624303C */  c.lt.d     $ft1, $ft0
    /* 5300 80004700 00000000 */  nop
    /* 5304 80004704 45000036 */  bc1f       .L800047E0
    /* 5308 80004708 00000000 */   nop
  .L8000470C:
    /* 530C 8000470C 8FB80030 */  lw         $t8, 0x30($sp)
    /* 5310 80004710 2401002E */  addiu      $at, $zero, 0x2E
    /* 5314 80004714 93190000 */  lbu        $t9, 0x0($t8)
    /* 5318 80004718 17210004 */  bne        $t9, $at, .L8000472C
    /* 531C 8000471C 00000000 */   nop
    /* 5320 80004720 8FA80030 */  lw         $t0, 0x30($sp)
    /* 5324 80004724 2509FFFF */  addiu      $t1, $t0, -0x1
    /* 5328 80004728 AFA90030 */  sw         $t1, 0x30($sp)
  .L8000472C:
    /* 532C 8000472C 8FAA0030 */  lw         $t2, 0x30($sp)
    /* 5330 80004730 914B0000 */  lbu        $t3, 0x0($t2)
    /* 5334 80004734 256C0001 */  addiu      $t4, $t3, 0x1
    /* 5338 80004738 A14C0000 */  sb         $t4, 0x0($t2)
    /* 533C 8000473C 8FAD0030 */  lw         $t5, 0x30($sp)
    /* 5340 80004740 91AE0000 */  lbu        $t6, 0x0($t5)
    /* 5344 80004744 29C1003A */  slti       $at, $t6, 0x3A
    /* 5348 80004748 10200003 */  beqz       $at, .L80004758
    /* 534C 8000474C 00000000 */   nop
    /* 5350 80004750 10000021 */  b          .L800047D8
    /* 5354 80004754 00000000 */   nop
  .L80004758:
    /* 5358 80004758 8FB80030 */  lw         $t8, 0x30($sp)
    /* 535C 8000475C 240F0030 */  addiu      $t7, $zero, 0x30
    /* 5360 80004760 A30F0000 */  sb         $t7, 0x0($t8)
    /* 5364 80004764 8FB90030 */  lw         $t9, 0x30($sp)
    /* 5368 80004768 8FA8002C */  lw         $t0, 0x2C($sp)
    /* 536C 8000476C 17280016 */  bne        $t9, $t0, .L800047C8
    /* 5370 80004770 00000000 */   nop
    /* 5374 80004774 8FA90028 */  lw         $t1, 0x28($sp)
    /* 5378 80004778 11200009 */  beqz       $t1, .L800047A0
    /* 537C 8000477C 00000000 */   nop
    /* 5380 80004780 8FAC0030 */  lw         $t4, 0x30($sp)
    /* 5384 80004784 240B0031 */  addiu      $t3, $zero, 0x31
    /* 5388 80004788 A18B0000 */  sb         $t3, 0x0($t4)
    /* 538C 8000478C 8FAA0028 */  lw         $t2, 0x28($sp)
    /* 5390 80004790 8D4D0000 */  lw         $t5, 0x0($t2)
    /* 5394 80004794 25AE0001 */  addiu      $t6, $t5, 0x1
    /* 5398 80004798 10000009 */  b          .L800047C0
    /* 539C 8000479C AD4E0000 */   sw        $t6, 0x0($t2)
  .L800047A0:
    /* 53A0 800047A0 8FAF0030 */  lw         $t7, 0x30($sp)
    /* 53A4 800047A4 24190031 */  addiu      $t9, $zero, 0x31
    /* 53A8 800047A8 25F8FFFF */  addiu      $t8, $t7, -0x1
    /* 53AC 800047AC AFB80030 */  sw         $t8, 0x30($sp)
    /* 53B0 800047B0 A3190000 */  sb         $t9, 0x0($t8)
    /* 53B4 800047B4 8FA8002C */  lw         $t0, 0x2C($sp)
    /* 53B8 800047B8 2509FFFF */  addiu      $t1, $t0, -0x1
    /* 53BC 800047BC AFA9002C */  sw         $t1, 0x2C($sp)
  .L800047C0:
    /* 53C0 800047C0 10000005 */  b          .L800047D8
    /* 53C4 800047C4 00000000 */   nop
  .L800047C8:
    /* 53C8 800047C8 8FAB0030 */  lw         $t3, 0x30($sp)
    /* 53CC 800047CC 256CFFFF */  addiu      $t4, $t3, -0x1
    /* 53D0 800047D0 1000FFCE */  b          .L8000470C
    /* 53D4 800047D4 AFAC0030 */   sw        $t4, 0x30($sp)
  .L800047D8:
    /* 53D8 800047D8 1000001F */  b          .L80004858
    /* 53DC 800047DC 00000000 */   nop
  .L800047E0:
    /* 53E0 800047E0 8FAD0038 */  lw         $t5, 0x38($sp)
    /* 53E4 800047E4 2401002D */  addiu      $at, $zero, 0x2D
    /* 53E8 800047E8 91AE0000 */  lbu        $t6, 0x0($t5)
    /* 53EC 800047EC 15C1001A */  bne        $t6, $at, .L80004858
    /* 53F0 800047F0 00000000 */   nop
  .L800047F4:
    /* 53F4 800047F4 8FAA0030 */  lw         $t2, 0x30($sp)
    /* 53F8 800047F8 2401002E */  addiu      $at, $zero, 0x2E
    /* 53FC 800047FC 914F0000 */  lbu        $t7, 0x0($t2)
    /* 5400 80004800 15E10004 */  bne        $t7, $at, .L80004814
    /* 5404 80004804 00000000 */   nop
    /* 5408 80004808 8FB90030 */  lw         $t9, 0x30($sp)
    /* 540C 8000480C 2738FFFF */  addiu      $t8, $t9, -0x1
    /* 5410 80004810 AFB80030 */  sw         $t8, 0x30($sp)
  .L80004814:
    /* 5414 80004814 8FA80030 */  lw         $t0, 0x30($sp)
    /* 5418 80004818 24010030 */  addiu      $at, $zero, 0x30
    /* 541C 8000481C 91090000 */  lbu        $t1, 0x0($t0)
    /* 5420 80004820 11210003 */  beq        $t1, $at, .L80004830
    /* 5424 80004824 00000000 */   nop
    /* 5428 80004828 1000000B */  b          .L80004858
    /* 542C 8000482C 00000000 */   nop
  .L80004830:
    /* 5430 80004830 8FAB0030 */  lw         $t3, 0x30($sp)
    /* 5434 80004834 8FAC002C */  lw         $t4, 0x2C($sp)
    /* 5438 80004838 156C0003 */  bne        $t3, $t4, .L80004848
    /* 543C 8000483C 00000000 */   nop
    /* 5440 80004840 8FAD0038 */  lw         $t5, 0x38($sp)
    /* 5444 80004844 A1A00000 */  sb         $zero, 0x0($t5)
  .L80004848:
    /* 5448 80004848 8FAE0030 */  lw         $t6, 0x30($sp)
    /* 544C 8000484C 25CAFFFF */  addiu      $t2, $t6, -0x1
    /* 5450 80004850 1000FFE8 */  b          .L800047F4
    /* 5454 80004854 AFAA0030 */   sw        $t2, 0x30($sp)
  .L80004858:
    /* 5458 80004858 10000003 */  b          .L80004868
    /* 545C 8000485C 8FA2002C */   lw        $v0, 0x2C($sp)
    /* 5460 80004860 10000001 */  b          .L80004868
    /* 5464 80004864 00000000 */   nop
  .L80004868:
    /* 5468 80004868 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 546C 8000486C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 5470 80004870 03E00008 */  jr         $ra
    /* 5474 80004874 00000000 */   nop
```

## Tips for This Category

