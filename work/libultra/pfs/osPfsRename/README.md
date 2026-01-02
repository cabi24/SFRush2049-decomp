# osPfsRename

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000A700` |
| **Category** | `libultra/pfs` |
| **Status** | `TODO` |
| **Instructions** | ~118 |

## Description

rename controller pak file

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/pfs` category.

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
./work/libultra/pfs/osPfsRename/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pfs/osPfsRename

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pfs/osPfsRename --watch
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
# Source: B300.s
# Address: 0x8000A700

glabel func_8000A700
    /* B300 8000A700 27BDFE90 */  addiu      $sp, $sp, -0x170
    /* B304 8000A704 AFB20024 */  sw         $s2, 0x24($sp)
    /* B308 8000A708 AFA50174 */  sw         $a1, 0x174($sp)
    /* B30C 8000A70C 30AEFFFF */  andi       $t6, $a1, 0xFFFF
    /* B310 8000A710 01C02825 */  or         $a1, $t6, $zero
    /* B314 8000A714 00809025 */  or         $s2, $a0, $zero
    /* B318 8000A718 AFBF0034 */  sw         $ra, 0x34($sp)
    /* B31C 8000A71C AFB50030 */  sw         $s5, 0x30($sp)
    /* B320 8000A720 AFB4002C */  sw         $s4, 0x2C($sp)
    /* B324 8000A724 AFB30028 */  sw         $s3, 0x28($sp)
    /* B328 8000A728 AFB10020 */  sw         $s1, 0x20($sp)
    /* B32C 8000A72C 11C00003 */  beqz       $t6, .L8000A73C
    /* B330 8000A730 AFB0001C */   sw        $s0, 0x1C($sp)
    /* B334 8000A734 14C00003 */  bnez       $a2, .L8000A744
    /* B338 8000A738 02402025 */   or        $a0, $s2, $zero
  .L8000A73C:
    /* B33C 8000A73C 1000005D */  b          .L8000A8B4
    /* B340 8000A740 24020005 */   addiu     $v0, $zero, 0x5
  .L8000A744:
    /* B344 8000A744 8FAF0180 */  lw         $t7, 0x180($sp)
    /* B348 8000A748 27B8016C */  addiu      $t8, $sp, 0x16C
    /* B34C 8000A74C AFB80014 */  sw         $t8, 0x14($sp)
    /* B350 8000A750 0C0028D4 */  jal        func_8000A350
    /* B354 8000A754 AFAF0010 */   sw        $t7, 0x10($sp)
    /* B358 8000A758 50400004 */  beql       $v0, $zero, .L8000A76C
    /* B35C 8000A75C 92590065 */   lbu       $t9, 0x65($s2)
    /* B360 8000A760 10000055 */  b          .L8000A8B8
    /* B364 8000A764 8FBF0034 */   lw        $ra, 0x34($sp)
    /* B368 8000A768 92590065 */  lbu        $t9, 0x65($s2)
  .L8000A76C:
    /* B36C 8000A76C 02402025 */  or         $a0, $s2, $zero
    /* B370 8000A770 53200008 */  beql       $t9, $zero, .L8000A794
    /* B374 8000A774 8E48005C */   lw        $t0, 0x5C($s2)
    /* B378 8000A778 0C003A14 */  jal        func_8000E850
    /* B37C 8000A77C 00002825 */   or        $a1, $zero, $zero
    /* B380 8000A780 50400004 */  beql       $v0, $zero, .L8000A794
    /* B384 8000A784 8E48005C */   lw        $t0, 0x5C($s2)
    /* B388 8000A788 1000004B */  b          .L8000A8B8
    /* B38C 8000A78C 8FBF0034 */   lw        $ra, 0x34($sp)
    /* B390 8000A790 8E48005C */  lw         $t0, 0x5C($s2)
  .L8000A794:
    /* B394 8000A794 8FA9016C */  lw         $t1, 0x16C($sp)
    /* B398 8000A798 8E440004 */  lw         $a0, 0x4($s2)
    /* B39C 8000A79C 8E450008 */  lw         $a1, 0x8($s2)
    /* B3A0 8000A7A0 01093021 */  addu       $a2, $t0, $t1
    /* B3A4 8000A7A4 30CAFFFF */  andi       $t2, $a2, 0xFFFF
    /* B3A8 8000A7A8 01403025 */  or         $a2, $t2, $zero
    /* B3AC 8000A7AC 0C003A34 */  jal        func_8000E8D0
    /* B3B0 8000A7B0 27A70048 */   addiu     $a3, $sp, 0x48
    /* B3B4 8000A7B4 10400003 */  beqz       $v0, .L8000A7C4
    /* B3B8 8000A7B8 93B0004E */   lbu       $s0, 0x4E($sp)
    /* B3BC 8000A7BC 1000003E */  b          .L8000A8B8
    /* B3C0 8000A7C0 8FBF0034 */   lw        $ra, 0x34($sp)
  .L8000A7C4:
    /* B3C4 8000A7C4 92420064 */  lbu        $v0, 0x64($s2)
    /* B3C8 8000A7C8 93B3004F */  lbu        $s3, 0x4F($sp)
    /* B3CC 8000A7CC 24150001 */  addiu      $s5, $zero, 0x1
    /* B3D0 8000A7D0 0202082A */  slt        $at, $s0, $v0
    /* B3D4 8000A7D4 10200026 */  beqz       $at, .L8000A870
    /* B3D8 8000A7D8 27B40044 */   addiu     $s4, $sp, 0x44
    /* B3DC 8000A7DC 27B10068 */  addiu      $s1, $sp, 0x68
    /* B3E0 8000A7E0 02402025 */  or         $a0, $s2, $zero
  .L8000A7E4:
    /* B3E4 8000A7E4 02202825 */  or         $a1, $s1, $zero
    /* B3E8 8000A7E8 00003025 */  or         $a2, $zero, $zero
    /* B3EC 8000A7EC 0C003CE9 */  jal        func_8000F3A4
    /* B3F0 8000A7F0 320700FF */   andi      $a3, $s0, 0xFF
    /* B3F4 8000A7F4 10400003 */  beqz       $v0, .L8000A804
    /* B3F8 8000A7F8 02402025 */   or        $a0, $s2, $zero
    /* B3FC 8000A7FC 1000002E */  b          .L8000A8B8
    /* B400 8000A800 8FBF0034 */   lw        $ra, 0x34($sp)
  .L8000A804:
    /* B404 8000A804 02202825 */  or         $a1, $s1, $zero
    /* B408 8000A808 326600FF */  andi       $a2, $s3, 0xFF
    /* B40C 8000A80C 320700FF */  andi       $a3, $s0, 0xFF
    /* B410 8000A810 0C002A36 */  jal        func_8000A8D8
    /* B414 8000A814 AFB40010 */   sw        $s4, 0x10($sp)
    /* B418 8000A818 10400003 */  beqz       $v0, .L8000A828
    /* B41C 8000A81C 02402025 */   or        $a0, $s2, $zero
    /* B420 8000A820 10000025 */  b          .L8000A8B8
    /* B424 8000A824 8FBF0034 */   lw        $ra, 0x34($sp)
  .L8000A828:
    /* B428 8000A828 02202825 */  or         $a1, $s1, $zero
    /* B42C 8000A82C 24060001 */  addiu      $a2, $zero, 0x1
    /* B430 8000A830 0C003CE9 */  jal        func_8000F3A4
    /* B434 8000A834 320700FF */   andi      $a3, $s0, 0xFF
    /* B438 8000A838 10400003 */  beqz       $v0, .L8000A848
    /* B43C 8000A83C 97AC0044 */   lhu       $t4, 0x44($sp)
    /* B440 8000A840 1000001D */  b          .L8000A8B8
    /* B444 8000A844 8FBF0034 */   lw        $ra, 0x34($sp)
  .L8000A848:
    /* B448 8000A848 56AC0004 */  bnel       $s5, $t4, .L8000A85C
    /* B44C 8000A84C 92420064 */   lbu       $v0, 0x64($s2)
    /* B450 8000A850 10000007 */  b          .L8000A870
    /* B454 8000A854 92420064 */   lbu       $v0, 0x64($s2)
    /* B458 8000A858 92420064 */  lbu        $v0, 0x64($s2)
  .L8000A85C:
    /* B45C 8000A85C 93B00044 */  lbu        $s0, 0x44($sp)
    /* B460 8000A860 93B30045 */  lbu        $s3, 0x45($sp)
    /* B464 8000A864 0202082A */  slt        $at, $s0, $v0
    /* B468 8000A868 5420FFDE */  bnel       $at, $zero, .L8000A7E4
    /* B46C 8000A86C 02402025 */   or        $a0, $s2, $zero
  .L8000A870:
    /* B470 8000A870 0202082A */  slt        $at, $s0, $v0
    /* B474 8000A874 14200003 */  bnez       $at, .L8000A884
    /* B478 8000A878 27A40048 */   addiu     $a0, $sp, 0x48
    /* B47C 8000A87C 1000000D */  b          .L8000A8B4
    /* B480 8000A880 24020003 */   addiu     $v0, $zero, 0x3
  .L8000A884:
    /* B484 8000A884 0C002164 */  jal        func_80008590
    /* B488 8000A888 24050020 */   addiu     $a1, $zero, 0x20
    /* B48C 8000A88C 8E4D005C */  lw         $t5, 0x5C($s2)
    /* B490 8000A890 8FAE016C */  lw         $t6, 0x16C($sp)
    /* B494 8000A894 8E440004 */  lw         $a0, 0x4($s2)
    /* B498 8000A898 8E450008 */  lw         $a1, 0x8($s2)
    /* B49C 8000A89C 01AE3021 */  addu       $a2, $t5, $t6
    /* B4A0 8000A8A0 30CFFFFF */  andi       $t7, $a2, 0xFFFF
    /* B4A4 8000A8A4 01E03025 */  or         $a2, $t7, $zero
    /* B4A8 8000A8A8 AFA00010 */  sw         $zero, 0x10($sp)
    /* B4AC 8000A8AC 0C003DA0 */  jal        func_8000F680
    /* B4B0 8000A8B0 27A70048 */   addiu     $a3, $sp, 0x48
  .L8000A8B4:
    /* B4B4 8000A8B4 8FBF0034 */  lw         $ra, 0x34($sp)
  .L8000A8B8:
    /* B4B8 8000A8B8 8FB0001C */  lw         $s0, 0x1C($sp)
    /* B4BC 8000A8BC 8FB10020 */  lw         $s1, 0x20($sp)
    /* B4C0 8000A8C0 8FB20024 */  lw         $s2, 0x24($sp)
    /* B4C4 8000A8C4 8FB30028 */  lw         $s3, 0x28($sp)
    /* B4C8 8000A8C8 8FB4002C */  lw         $s4, 0x2C($sp)
    /* B4CC 8000A8CC 8FB50030 */  lw         $s5, 0x30($sp)
    /* B4D0 8000A8D0 03E00008 */  jr         $ra
    /* B4D4 8000A8D4 27BD0170 */   addiu     $sp, $sp, 0x170
```

## Tips for This Category

