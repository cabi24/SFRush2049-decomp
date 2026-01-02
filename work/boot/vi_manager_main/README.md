# vi_manager_main

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80006BB0` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~99 |

## Description

VI manager main loop (handles VI_RETRACE, PRENMI events)

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
./work/boot/vi_manager_main/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/vi_manager_main

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/vi_manager_main --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Check `reference/repos/rushtherock/game/drones.c`

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 7630.s
# Address: 0x80006BB0

glabel func_80006BB0
    /* 77B0 80006BB0 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* 77B4 80006BB4 AFBF0034 */  sw         $ra, 0x34($sp)
    /* 77B8 80006BB8 AFB70030 */  sw         $s7, 0x30($sp)
    /* 77BC 80006BBC 0080B825 */  or         $s7, $a0, $zero
    /* 77C0 80006BC0 AFB6002C */  sw         $s6, 0x2C($sp)
    /* 77C4 80006BC4 AFB50028 */  sw         $s5, 0x28($sp)
    /* 77C8 80006BC8 AFB40024 */  sw         $s4, 0x24($sp)
    /* 77CC 80006BCC AFB30020 */  sw         $s3, 0x20($sp)
    /* 77D0 80006BD0 AFB2001C */  sw         $s2, 0x1C($sp)
    /* 77D4 80006BD4 AFB10018 */  sw         $s1, 0x18($sp)
    /* 77D8 80006BD8 AFB00014 */  sw         $s0, 0x14($sp)
    /* 77DC 80006BDC 0C003198 */  jal        func_8000C660
    /* 77E0 80006BE0 AFA00044 */   sw        $zero, 0x44($sp)
    /* 77E4 80006BE4 94430002 */  lhu        $v1, 0x2($v0)
    /* 77E8 80006BE8 3C018003 */  lui        $at, %hi(D_80036700)
    /* 77EC 80006BEC 2416000E */  addiu      $s6, $zero, 0xE
    /* 77F0 80006BF0 14600004 */  bnez       $v1, .L80006C04
    /* 77F4 80006BF4 A4236700 */   sh        $v1, %lo(D_80036700)($at)
    /* 77F8 80006BF8 24030001 */  addiu      $v1, $zero, 0x1
    /* 77FC 80006BFC 3C018003 */  lui        $at, %hi(D_80036700)
    /* 7800 80006C00 A4236700 */  sh         $v1, %lo(D_80036700)($at)
  .L80006C04:
    /* 7804 80006C04 3C138003 */  lui        $s3, %hi(D_80037C5C)
    /* 7808 80006C08 3C128003 */  lui        $s2, %hi(D_80037C58)
    /* 780C 80006C0C 3C118003 */  lui        $s1, %hi(D_80037C50)
    /* 7810 80006C10 26317C50 */  addiu      $s1, $s1, %lo(D_80037C50)
    /* 7814 80006C14 26527C58 */  addiu      $s2, $s2, %lo(D_80037C58)
    /* 7818 80006C18 26737C5C */  addiu      $s3, $s3, %lo(D_80037C5C)
    /* 781C 80006C1C 2415000D */  addiu      $s5, $zero, 0xD
    /* 7820 80006C20 27B40044 */  addiu      $s4, $sp, 0x44
  .L80006C24:
    /* 7824 80006C24 8EE4000C */  lw         $a0, 0xC($s7)
  .L80006C28:
    /* 7828 80006C28 02802825 */  or         $a1, $s4, $zero
    /* 782C 80006C2C 0C001C9C */  jal        func_80007270
    /* 7830 80006C30 24060001 */   addiu     $a2, $zero, 0x1
    /* 7834 80006C34 8FAE0044 */  lw         $t6, 0x44($sp)
    /* 7838 80006C38 95C30000 */  lhu        $v1, 0x0($t6)
    /* 783C 80006C3C 10750005 */  beq        $v1, $s5, .L80006C54
    /* 7840 80006C40 00000000 */   nop
    /* 7844 80006C44 1076002A */  beq        $v1, $s6, .L80006CF0
    /* 7848 80006C48 00000000 */   nop
    /* 784C 80006C4C 1000FFF6 */  b          .L80006C28
    /* 7850 80006C50 8EE4000C */   lw        $a0, 0xC($s7)
  .L80006C54:
    /* 7854 80006C54 0C00319C */  jal        func_8000C670
    /* 7858 80006C58 00000000 */   nop
    /* 785C 80006C5C 3C038003 */  lui        $v1, %hi(D_80036700)
    /* 7860 80006C60 94636700 */  lhu        $v1, %lo(D_80036700)($v1)
    /* 7864 80006C64 3C018003 */  lui        $at, %hi(D_80036700)
    /* 7868 80006C68 2463FFFF */  addiu      $v1, $v1, -0x1
    /* 786C 80006C6C 306FFFFF */  andi       $t7, $v1, 0xFFFF
    /* 7870 80006C70 15E0000E */  bnez       $t7, .L80006CAC
    /* 7874 80006C74 A42F6700 */   sh        $t7, %lo(D_80036700)($at)
    /* 7878 80006C78 0C003198 */  jal        func_8000C660
    /* 787C 80006C7C 00000000 */   nop
    /* 7880 80006C80 8C580010 */  lw         $t8, 0x10($v0)
    /* 7884 80006C84 00408025 */  or         $s0, $v0, $zero
    /* 7888 80006C88 00003025 */  or         $a2, $zero, $zero
    /* 788C 80006C8C 53000005 */  beql       $t8, $zero, .L80006CA4
    /* 7890 80006C90 96030002 */   lhu       $v1, 0x2($s0)
    /* 7894 80006C94 8C440010 */  lw         $a0, 0x10($v0)
    /* 7898 80006C98 0C001D78 */  jal        func_800075E0
    /* 789C 80006C9C 8C450014 */   lw        $a1, 0x14($v0)
    /* 78A0 80006CA0 96030002 */  lhu        $v1, 0x2($s0)
  .L80006CA4:
    /* 78A4 80006CA4 3C018003 */  lui        $at, %hi(D_80036700)
    /* 78A8 80006CA8 A4236700 */  sh         $v1, %lo(D_80036700)($at)
  .L80006CAC:
    /* 78AC 80006CAC 8E790000 */  lw         $t9, 0x0($s3)
    /* 78B0 80006CB0 8E500000 */  lw         $s0, 0x0($s2)
    /* 78B4 80006CB4 27280001 */  addiu      $t0, $t9, 0x1
    /* 78B8 80006CB8 0C00325C */  jal        func_8000C970
    /* 78BC 80006CBC AE680000 */   sw        $t0, 0x0($s3)
    /* 78C0 80006CC0 8E2D0004 */  lw         $t5, 0x4($s1)
    /* 78C4 80006CC4 00508023 */  subu       $s0, $v0, $s0
    /* 78C8 80006CC8 8E2C0000 */  lw         $t4, 0x0($s1)
    /* 78CC 80006CCC 020D7821 */  addu       $t7, $s0, $t5
    /* 78D0 80006CD0 240A0000 */  addiu      $t2, $zero, 0x0
    /* 78D4 80006CD4 01ED082B */  sltu       $at, $t7, $t5
    /* 78D8 80006CD8 002A7021 */  addu       $t6, $at, $t2
    /* 78DC 80006CDC 01CC7021 */  addu       $t6, $t6, $t4
    /* 78E0 80006CE0 AE420000 */  sw         $v0, 0x0($s2)
    /* 78E4 80006CE4 AE2E0000 */  sw         $t6, 0x0($s1)
    /* 78E8 80006CE8 1000FFCE */  b          .L80006C24
    /* 78EC 80006CEC AE2F0004 */   sw        $t7, 0x4($s1)
  .L80006CF0:
    /* 78F0 80006CF0 0C003047 */  jal        func_8000C11C
    /* 78F4 80006CF4 00000000 */   nop
    /* 78F8 80006CF8 1000FFCB */  b          .L80006C28
    /* 78FC 80006CFC 8EE4000C */   lw        $a0, 0xC($s7)
    /* 7900 80006D00 00000000 */  nop
    /* 7904 80006D04 00000000 */  nop
    /* 7908 80006D08 00000000 */  nop
    /* 790C 80006D0C 00000000 */  nop
    /* 7910 80006D10 8FBF0034 */  lw         $ra, 0x34($sp)
    /* 7914 80006D14 8FB00014 */  lw         $s0, 0x14($sp)
    /* 7918 80006D18 8FB10018 */  lw         $s1, 0x18($sp)
    /* 791C 80006D1C 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 7920 80006D20 8FB30020 */  lw         $s3, 0x20($sp)
    /* 7924 80006D24 8FB40024 */  lw         $s4, 0x24($sp)
    /* 7928 80006D28 8FB50028 */  lw         $s5, 0x28($sp)
    /* 792C 80006D2C 8FB6002C */  lw         $s6, 0x2C($sp)
    /* 7930 80006D30 8FB70030 */  lw         $s7, 0x30($sp)
    /* 7934 80006D34 03E00008 */  jr         $ra
    /* 7938 80006D38 27BD0050 */   addiu     $sp, $sp, 0x50
```

## Tips for This Category

