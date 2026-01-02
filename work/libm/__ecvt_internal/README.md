# __ecvt_internal

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80003C3C` |
| **Category** | `libm` |
| **Status** | `TODO` |
| **Instructions** | ~654 |

## Description

float to string internal helper

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
./work/libm/__ecvt_internal/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libm/__ecvt_internal

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libm/__ecvt_internal --watch
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
# Address: 0x80003C3C

glabel func_80003C3C
    /* 483C 80003C3C 27BDFF88 */  addiu      $sp, $sp, -0x78
    /* 4840 80003C40 AFBF003C */  sw         $ra, 0x3C($sp)
    /* 4844 80003C44 AFA60080 */  sw         $a2, 0x80($sp)
    /* 4848 80003C48 AFA70084 */  sw         $a3, 0x84($sp)
    /* 484C 80003C4C F7AC0078 */  sdc1       $fa0, 0x78($sp)
    /* 4850 80003C50 AFB20038 */  sw         $s2, 0x38($sp)
    /* 4854 80003C54 AFB10034 */  sw         $s1, 0x34($sp)
    /* 4858 80003C58 AFB00030 */  sw         $s0, 0x30($sp)
    /* 485C 80003C5C F7B40028 */  sdc1       $fs0, 0x28($sp)
    /* 4860 80003C60 AFA0005C */  sw         $zero, 0x5C($sp)
    /* 4864 80003C64 AFA00060 */  sw         $zero, 0x60($sp)
    /* 4868 80003C68 AFA00064 */  sw         $zero, 0x64($sp)
    /* 486C 80003C6C D7A40078 */  ldc1       $ft0, 0x78($sp)
    /* 4870 80003C70 44803800 */  mtc1       $zero, $ft1f
    /* 4874 80003C74 44803000 */  mtc1       $zero, $ft1
    /* 4878 80003C78 00000000 */  nop
    /* 487C 80003C7C 4626203C */  c.lt.d     $ft0, $ft1
    /* 4880 80003C80 00000000 */  nop
    /* 4884 80003C84 45000008 */  bc1f       .L80003CA8
    /* 4888 80003C88 00000000 */   nop
    /* 488C 80003C8C D7A80078 */  ldc1       $ft2, 0x78($sp)
    /* 4890 80003C90 46204287 */  neg.d      $ft3, $ft2
    /* 4894 80003C94 F7AA0078 */  sdc1       $ft3, 0x78($sp)
    /* 4898 80003C98 8FAF0088 */  lw         $t7, 0x88($sp)
    /* 489C 80003C9C 240E002D */  addiu      $t6, $zero, 0x2D
    /* 48A0 80003CA0 10000003 */  b          .L80003CB0
    /* 48A4 80003CA4 A1EE0000 */   sb        $t6, 0x0($t7)
  .L80003CA8:
    /* 48A8 80003CA8 8FB80088 */  lw         $t8, 0x88($sp)
    /* 48AC 80003CAC A3000000 */  sb         $zero, 0x0($t8)
  .L80003CB0:
    /* 48B0 80003CB0 D7AC0078 */  ldc1       $fa0, 0x78($sp)
    /* 48B4 80003CB4 0C000A28 */  jal        func_800028A0
    /* 48B8 80003CB8 27A60050 */   addiu     $a2, $sp, 0x50
    /* 48BC 80003CBC 46200506 */  mov.d      $fs0, $fv0
    /* 48C0 80003CC0 8FB90090 */  lw         $t9, 0x90($sp)
    /* 48C4 80003CC4 27280001 */  addiu      $t0, $t9, 0x1
    /* 48C8 80003CC8 AFA80090 */  sw         $t0, 0x90($sp)
    /* 48CC 80003CCC 01008825 */  or         $s1, $t0, $zero
    /* 48D0 80003CD0 3C013FF0 */  lui        $at, (0x3FF00000 >> 16)
    /* 48D4 80003CD4 44819800 */  mtc1       $at, $ft5f
    /* 48D8 80003CD8 D7B00078 */  ldc1       $ft4, 0x78($sp)
    /* 48DC 80003CDC 44809000 */  mtc1       $zero, $ft5
    /* 48E0 80003CE0 00000000 */  nop
    /* 48E4 80003CE4 4630903E */  c.le.d     $ft5, $ft4
    /* 48E8 80003CE8 00000000 */  nop
    /* 48EC 80003CEC 4500002A */  bc1f       .L80003D98
    /* 48F0 80003CF0 00000000 */   nop
    /* 48F4 80003CF4 D7A40050 */  ldc1       $ft0, 0x50($sp)
    /* 48F8 80003CF8 44803800 */  mtc1       $zero, $ft1f
    /* 48FC 80003CFC 44803000 */  mtc1       $zero, $ft1
    /* 4900 80003D00 8FB00094 */  lw         $s0, 0x94($sp)
    /* 4904 80003D04 46262032 */  c.eq.d     $ft0, $ft1
    /* 4908 80003D08 2610FFFF */  addiu      $s0, $s0, -0x1
    /* 490C 80003D0C 45010020 */  bc1t       .L80003D90
    /* 4910 80003D10 00000000 */   nop
  .L80003D14:
    /* 4914 80003D14 3C014024 */  lui        $at, (0x40240000 >> 16)
    /* 4918 80003D18 44815800 */  mtc1       $at, $ft3f
    /* 491C 80003D1C D7A80050 */  ldc1       $ft2, 0x50($sp)
    /* 4920 80003D20 44805000 */  mtc1       $zero, $ft3
    /* 4924 80003D24 27A60050 */  addiu      $a2, $sp, 0x50
    /* 4928 80003D28 0C000A28 */  jal        func_800028A0
    /* 492C 80003D2C 462A4303 */   div.d     $fa0, $ft2, $ft3
    /* 4930 80003D30 F7A00048 */  sdc1       $fv0, 0x48($sp)
    /* 4934 80003D34 3C018003 */  lui        $at, %hi(D_8002D6C0)
    /* 4938 80003D38 D432D6C0 */  ldc1       $ft5, %lo(D_8002D6C0)($at)
    /* 493C 80003D3C D7B00048 */  ldc1       $ft4, 0x48($sp)
    /* 4940 80003D40 3C014024 */  lui        $at, (0x40240000 >> 16)
    /* 4944 80003D44 44813800 */  mtc1       $at, $ft1f
    /* 4948 80003D48 46328100 */  add.d      $ft0, $ft4, $ft5
    /* 494C 80003D4C 44803000 */  mtc1       $zero, $ft1
    /* 4950 80003D50 2610FFFF */  addiu      $s0, $s0, -0x1
    /* 4954 80003D54 46262202 */  mul.d      $ft2, $ft0, $ft1
    /* 4958 80003D58 4620428D */  trunc.w.d  $ft3, $ft2
    /* 495C 80003D5C 440A5000 */  mfc1       $t2, $ft3
    /* 4960 80003D60 00000000 */  nop
    /* 4964 80003D64 254B0030 */  addiu      $t3, $t2, 0x30
    /* 4968 80003D68 A20B0001 */  sb         $t3, 0x1($s0)
    /* 496C 80003D6C D7B00050 */  ldc1       $ft4, 0x50($sp)
    /* 4970 80003D70 44809800 */  mtc1       $zero, $ft5f
    /* 4974 80003D74 44809000 */  mtc1       $zero, $ft5
    /* 4978 80003D78 8FAC0060 */  lw         $t4, 0x60($sp)
    /* 497C 80003D7C 46328032 */  c.eq.d     $ft4, $ft5
    /* 4980 80003D80 258D0001 */  addiu      $t5, $t4, 0x1
    /* 4984 80003D84 AFAD0060 */  sw         $t5, 0x60($sp)
    /* 4988 80003D88 4500FFE2 */  bc1f       .L80003D14
    /* 498C 80003D8C 00000000 */   nop
  .L80003D90:
    /* 4990 80003D90 10000003 */  b          .L80003DA0
    /* 4994 80003D94 00000000 */   nop
  .L80003D98:
    /* 4998 80003D98 8FB00094 */  lw         $s0, 0x94($sp)
    /* 499C 80003D9C 2610FFFF */  addiu      $s0, $s0, -0x1
  .L80003DA0:
    /* 49A0 80003DA0 8FB2008C */  lw         $s2, 0x8C($sp)
    /* 49A4 80003DA4 24010045 */  addiu      $at, $zero, 0x45
    /* 49A8 80003DA8 12410072 */  beq        $s2, $at, .L80003F74
    /* 49AC 80003DAC 00000000 */   nop
    /* 49B0 80003DB0 24010047 */  addiu      $at, $zero, 0x47
    /* 49B4 80003DB4 12410156 */  beq        $s2, $at, .L80004310
    /* 49B8 80003DB8 00000000 */   nop
    /* 49BC 80003DBC 24010065 */  addiu      $at, $zero, 0x65
    /* 49C0 80003DC0 1241006C */  beq        $s2, $at, .L80003F74
    /* 49C4 80003DC4 00000000 */   nop
    /* 49C8 80003DC8 24010066 */  addiu      $at, $zero, 0x66
    /* 49CC 80003DCC 12410006 */  beq        $s2, $at, .L80003DE8
    /* 49D0 80003DD0 00000000 */   nop
    /* 49D4 80003DD4 24010067 */  addiu      $at, $zero, 0x67
    /* 49D8 80003DD8 1241014D */  beq        $s2, $at, .L80004310
    /* 49DC 80003DDC 00000000 */   nop
    /* 49E0 80003DE0 10000218 */  b          .L80004644
    /* 49E4 80003DE4 00000000 */   nop
  .L80003DE8:
    /* 49E8 80003DE8 8FAE0060 */  lw         $t6, 0x60($sp)
    /* 49EC 80003DEC 11C00010 */  beqz       $t6, .L80003E30
    /* 49F0 80003DF0 00000000 */   nop
    /* 49F4 80003DF4 8FAF0094 */  lw         $t7, 0x94($sp)
    /* 49F8 80003DF8 26100001 */  addiu      $s0, $s0, 0x1
    /* 49FC 80003DFC 020F082B */  sltu       $at, $s0, $t7
    /* 4A00 80003E00 10200009 */  beqz       $at, .L80003E28
    /* 4A04 80003E04 00000000 */   nop
  .L80003E08:
    /* 4A08 80003E08 92180000 */  lbu        $t8, 0x0($s0)
    /* 4A0C 80003E0C 26100001 */  addiu      $s0, $s0, 0x1
    /* 4A10 80003E10 26310001 */  addiu      $s1, $s1, 0x1
    /* 4A14 80003E14 A238FFFF */  sb         $t8, -0x1($s1)
    /* 4A18 80003E18 8FB90094 */  lw         $t9, 0x94($sp)
    /* 4A1C 80003E1C 0219082B */  sltu       $at, $s0, $t9
    /* 4A20 80003E20 1420FFF9 */  bnez       $at, .L80003E08
    /* 4A24 80003E24 00000000 */   nop
  .L80003E28:
    /* 4A28 80003E28 10000004 */  b          .L80003E3C
    /* 4A2C 80003E2C 00000000 */   nop
  .L80003E30:
    /* 4A30 80003E30 24080030 */  addiu      $t0, $zero, 0x30
    /* 4A34 80003E34 A2280000 */  sb         $t0, 0x0($s1)
    /* 4A38 80003E38 26310001 */  addiu      $s1, $s1, 0x1
  .L80003E3C:
    /* 4A3C 80003E3C 8FA90080 */  lw         $t1, 0x80($sp)
    /* 4A40 80003E40 15200005 */  bnez       $t1, .L80003E58
    /* 4A44 80003E44 00000000 */   nop
    /* 4A48 80003E48 8FAA0084 */  lw         $t2, 0x84($sp)
    /* 4A4C 80003E4C 314B0008 */  andi       $t3, $t2, 0x8
    /* 4A50 80003E50 11600004 */  beqz       $t3, .L80003E64
    /* 4A54 80003E54 00000000 */   nop
  .L80003E58:
    /* 4A58 80003E58 240C002E */  addiu      $t4, $zero, 0x2E
    /* 4A5C 80003E5C A22C0000 */  sb         $t4, 0x0($s1)
    /* 4A60 80003E60 26310001 */  addiu      $s1, $s1, 0x1
  .L80003E64:
    /* 4A64 80003E64 44802800 */  mtc1       $zero, $ft0f
    /* 4A68 80003E68 44802000 */  mtc1       $zero, $ft0
    /* 4A6C 80003E6C 00000000 */  nop
    /* 4A70 80003E70 4624A032 */  c.eq.d     $fs0, $ft0
    /* 4A74 80003E74 00000000 */  nop
    /* 4A78 80003E78 4501002F */  bc1t       .L80003F38
    /* 4A7C 80003E7C 00000000 */   nop
    /* 4A80 80003E80 8FAD0080 */  lw         $t5, 0x80($sp)
    /* 4A84 80003E84 11A0001B */  beqz       $t5, .L80003EF4
    /* 4A88 80003E88 00000000 */   nop
  .L80003E8C:
    /* 4A8C 80003E8C 3C014024 */  lui        $at, (0x40240000 >> 16)
    /* 4A90 80003E90 44813800 */  mtc1       $at, $ft1f
    /* 4A94 80003E94 44803000 */  mtc1       $zero, $ft1
    /* 4A98 80003E98 27A60048 */  addiu      $a2, $sp, 0x48
    /* 4A9C 80003E9C 4626A302 */  mul.d      $fa0, $fs0, $ft1
    /* 4AA0 80003EA0 0C000A28 */  jal        func_800028A0
    /* 4AA4 80003EA4 00000000 */   nop
    /* 4AA8 80003EA8 46200506 */  mov.d      $fs0, $fv0
    /* 4AAC 80003EAC D7A80048 */  ldc1       $ft2, 0x48($sp)
    /* 4AB0 80003EB0 26310001 */  addiu      $s1, $s1, 0x1
    /* 4AB4 80003EB4 4620428D */  trunc.w.d  $ft3, $ft2
    /* 4AB8 80003EB8 440F5000 */  mfc1       $t7, $ft3
    /* 4ABC 80003EBC 00000000 */  nop
    /* 4AC0 80003EC0 25F80030 */  addiu      $t8, $t7, 0x30
    /* 4AC4 80003EC4 A238FFFF */  sb         $t8, -0x1($s1)
    /* 4AC8 80003EC8 8FB90080 */  lw         $t9, 0x80($sp)
    /* 4ACC 80003ECC 2728FFFF */  addiu      $t0, $t9, -0x1
    /* 4AD0 80003ED0 11000008 */  beqz       $t0, .L80003EF4
    /* 4AD4 80003ED4 AFA80080 */   sw        $t0, 0x80($sp)
    /* 4AD8 80003ED8 44808800 */  mtc1       $zero, $ft4f
    /* 4ADC 80003EDC 44808000 */  mtc1       $zero, $ft4
    /* 4AE0 80003EE0 00000000 */  nop
    /* 4AE4 80003EE4 4630A032 */  c.eq.d     $fs0, $ft4
    /* 4AE8 80003EE8 00000000 */  nop
    /* 4AEC 80003EEC 4500FFE7 */  bc1f       .L80003E8C
    /* 4AF0 80003EF0 00000000 */   nop
  .L80003EF4:
    /* 4AF4 80003EF4 44809800 */  mtc1       $zero, $ft5f
    /* 4AF8 80003EF8 44809000 */  mtc1       $zero, $ft5
    /* 4AFC 80003EFC 00000000 */  nop
    /* 4B00 80003F00 4632A032 */  c.eq.d     $fs0, $ft5
    /* 4B04 80003F04 00000000 */  nop
    /* 4B08 80003F08 4501000B */  bc1t       .L80003F38
    /* 4B0C 80003F0C 00000000 */   nop
    /* 4B10 80003F10 8FAA0088 */  lw         $t2, 0x88($sp)
    /* 4B14 80003F14 2629FFFF */  addiu      $t1, $s1, -0x1
    /* 4B18 80003F18 AFA90010 */  sw         $t1, 0x10($sp)
    /* 4B1C 80003F1C 4620A306 */  mov.d      $fa0, $fs0
    /* 4B20 80003F20 00003025 */  or         $a2, $zero, $zero
    /* 4B24 80003F24 8FA70090 */  lw         $a3, 0x90($sp)
    /* 4B28 80003F28 AFA00014 */  sw         $zero, 0x14($sp)
    /* 4B2C 80003F2C 0C00119D */  jal        func_80004674
    /* 4B30 80003F30 AFAA0018 */   sw        $t2, 0x18($sp)
    /* 4B34 80003F34 AFA20090 */  sw         $v0, 0x90($sp)
  .L80003F38:
    /* 4B38 80003F38 8FAB0080 */  lw         $t3, 0x80($sp)
    /* 4B3C 80003F3C 01609025 */  or         $s2, $t3, $zero
    /* 4B40 80003F40 256CFFFF */  addiu      $t4, $t3, -0x1
    /* 4B44 80003F44 12400009 */  beqz       $s2, .L80003F6C
    /* 4B48 80003F48 AFAC0080 */   sw        $t4, 0x80($sp)
  .L80003F4C:
    /* 4B4C 80003F4C 240D0030 */  addiu      $t5, $zero, 0x30
    /* 4B50 80003F50 A22D0000 */  sb         $t5, 0x0($s1)
    /* 4B54 80003F54 8FAE0080 */  lw         $t6, 0x80($sp)
    /* 4B58 80003F58 26310001 */  addiu      $s1, $s1, 0x1
    /* 4B5C 80003F5C 01C09025 */  or         $s2, $t6, $zero
    /* 4B60 80003F60 25CFFFFF */  addiu      $t7, $t6, -0x1
    /* 4B64 80003F64 1640FFF9 */  bnez       $s2, .L80003F4C
    /* 4B68 80003F68 AFAF0080 */   sw        $t7, 0x80($sp)
  .L80003F6C:
    /* 4B6C 80003F6C 100001B5 */  b          .L80004644
    /* 4B70 80003F70 00000000 */   nop
  .L80003F74:
    /* 4B74 80003F74 8FB80060 */  lw         $t8, 0x60($sp)
    /* 4B78 80003F78 1300003E */  beqz       $t8, .L80004074
    /* 4B7C 80003F7C 00000000 */   nop
    /* 4B80 80003F80 92190001 */  lbu        $t9, 0x1($s0)
    /* 4B84 80003F84 26100001 */  addiu      $s0, $s0, 0x1
    /* 4B88 80003F88 26310001 */  addiu      $s1, $s1, 0x1
    /* 4B8C 80003F8C A239FFFF */  sb         $t9, -0x1($s1)
    /* 4B90 80003F90 8FA80080 */  lw         $t0, 0x80($sp)
    /* 4B94 80003F94 15000005 */  bnez       $t0, .L80003FAC
    /* 4B98 80003F98 00000000 */   nop
    /* 4B9C 80003F9C 8FA90084 */  lw         $t1, 0x84($sp)
    /* 4BA0 80003FA0 312A0008 */  andi       $t2, $t1, 0x8
    /* 4BA4 80003FA4 11400004 */  beqz       $t2, .L80003FB8
    /* 4BA8 80003FA8 00000000 */   nop
  .L80003FAC:
    /* 4BAC 80003FAC 240B002E */  addiu      $t3, $zero, 0x2E
    /* 4BB0 80003FB0 A22B0000 */  sb         $t3, 0x0($s1)
    /* 4BB4 80003FB4 26310001 */  addiu      $s1, $s1, 0x1
  .L80003FB8:
    /* 4BB8 80003FB8 8FAC0080 */  lw         $t4, 0x80($sp)
    /* 4BBC 80003FBC 11800012 */  beqz       $t4, .L80004008
    /* 4BC0 80003FC0 00000000 */   nop
    /* 4BC4 80003FC4 8FAD0094 */  lw         $t5, 0x94($sp)
    /* 4BC8 80003FC8 26100001 */  addiu      $s0, $s0, 0x1
    /* 4BCC 80003FCC 020D082B */  sltu       $at, $s0, $t5
    /* 4BD0 80003FD0 1020000D */  beqz       $at, .L80004008
    /* 4BD4 80003FD4 00000000 */   nop
  .L80003FD8:
    /* 4BD8 80003FD8 920E0000 */  lbu        $t6, 0x0($s0)
    /* 4BDC 80003FDC 26310001 */  addiu      $s1, $s1, 0x1
    /* 4BE0 80003FE0 A22EFFFF */  sb         $t6, -0x1($s1)
    /* 4BE4 80003FE4 8FAF0080 */  lw         $t7, 0x80($sp)
    /* 4BE8 80003FE8 25F8FFFF */  addiu      $t8, $t7, -0x1
    /* 4BEC 80003FEC 13000006 */  beqz       $t8, .L80004008
    /* 4BF0 80003FF0 AFB80080 */   sw        $t8, 0x80($sp)
    /* 4BF4 80003FF4 8FB90094 */  lw         $t9, 0x94($sp)
    /* 4BF8 80003FF8 26100001 */  addiu      $s0, $s0, 0x1
    /* 4BFC 80003FFC 0219082B */  sltu       $at, $s0, $t9
    /* 4C00 80004000 1420FFF5 */  bnez       $at, .L80003FD8
    /* 4C04 80004004 00000000 */   nop
  .L80004008:
    /* 4C08 80004008 8FA80080 */  lw         $t0, 0x80($sp)
    /* 4C0C 8000400C 15000015 */  bnez       $t0, .L80004064
    /* 4C10 80004010 00000000 */   nop
    /* 4C14 80004014 8FA90094 */  lw         $t1, 0x94($sp)
    /* 4C18 80004018 26100001 */  addiu      $s0, $s0, 0x1
    /* 4C1C 8000401C 0209082B */  sltu       $at, $s0, $t1
    /* 4C20 80004020 10200010 */  beqz       $at, .L80004064
    /* 4C24 80004024 00000000 */   nop
    /* 4C28 80004028 4480A800 */  mtc1       $zero, $fs0f
    /* 4C2C 8000402C 4480A000 */  mtc1       $zero, $fs0
    /* 4C30 80004030 00000000 */  nop
    /* 4C34 80004034 262AFFFF */  addiu      $t2, $s1, -0x1
    /* 4C38 80004038 AFAA0010 */  sw         $t2, 0x10($sp)
    /* 4C3C 8000403C 920B0000 */  lbu        $t3, 0x0($s0)
    /* 4C40 80004040 8FAC0088 */  lw         $t4, 0x88($sp)
    /* 4C44 80004044 44806800 */  mtc1       $zero, $fa0f
    /* 4C48 80004048 44806000 */  mtc1       $zero, $fa0
    /* 4C4C 8000404C 27A60060 */  addiu      $a2, $sp, 0x60
    /* 4C50 80004050 8FA70090 */  lw         $a3, 0x90($sp)
    /* 4C54 80004054 AFAB0014 */  sw         $t3, 0x14($sp)
    /* 4C58 80004058 0C00119D */  jal        func_80004674
    /* 4C5C 8000405C AFAC0018 */   sw        $t4, 0x18($sp)
    /* 4C60 80004060 AFA20090 */  sw         $v0, 0x90($sp)
  .L80004064:
    /* 4C64 80004064 8FAD0060 */  lw         $t5, 0x60($sp)
    /* 4C68 80004068 25AEFFFF */  addiu      $t6, $t5, -0x1
    /* 4C6C 8000406C 10000040 */  b          .L80004170
    /* 4C70 80004070 AFAE0060 */   sw        $t6, 0x60($sp)
  .L80004074:
    /* 4C74 80004074 44802800 */  mtc1       $zero, $ft0f
    /* 4C78 80004078 44802000 */  mtc1       $zero, $ft0
    /* 4C7C 8000407C 00000000 */  nop
    /* 4C80 80004080 4624A032 */  c.eq.d     $fs0, $ft0
    /* 4C84 80004084 00000000 */  nop
    /* 4C88 80004088 4501002C */  bc1t       .L8000413C
    /* 4C8C 8000408C 00000000 */   nop
    /* 4C90 80004090 240FFFFF */  addiu      $t7, $zero, -0x1
    /* 4C94 80004094 AFAF0060 */  sw         $t7, 0x60($sp)
  .L80004098:
    /* 4C98 80004098 3C014024 */  lui        $at, (0x40240000 >> 16)
    /* 4C9C 8000409C 44813800 */  mtc1       $at, $ft1f
    /* 4CA0 800040A0 44803000 */  mtc1       $zero, $ft1
    /* 4CA4 800040A4 27A60048 */  addiu      $a2, $sp, 0x48
    /* 4CA8 800040A8 4626A302 */  mul.d      $fa0, $fs0, $ft1
    /* 4CAC 800040AC 0C000A28 */  jal        func_800028A0
    /* 4CB0 800040B0 00000000 */   nop
    /* 4CB4 800040B4 46200506 */  mov.d      $fs0, $fv0
    /* 4CB8 800040B8 D7A80048 */  ldc1       $ft2, 0x48($sp)
    /* 4CBC 800040BC 44805800 */  mtc1       $zero, $ft3f
    /* 4CC0 800040C0 44805000 */  mtc1       $zero, $ft3
    /* 4CC4 800040C4 00000000 */  nop
    /* 4CC8 800040C8 462A4032 */  c.eq.d     $ft2, $ft3
    /* 4CCC 800040CC 00000000 */  nop
    /* 4CD0 800040D0 45010003 */  bc1t       .L800040E0
    /* 4CD4 800040D4 00000000 */   nop
    /* 4CD8 800040D8 10000005 */  b          .L800040F0
    /* 4CDC 800040DC 00000000 */   nop
  .L800040E0:
    /* 4CE0 800040E0 8FB80060 */  lw         $t8, 0x60($sp)
    /* 4CE4 800040E4 2719FFFF */  addiu      $t9, $t8, -0x1
    /* 4CE8 800040E8 1000FFEB */  b          .L80004098
    /* 4CEC 800040EC AFB90060 */   sw        $t9, 0x60($sp)
  .L800040F0:
    /* 4CF0 800040F0 D7B00048 */  ldc1       $ft4, 0x48($sp)
    /* 4CF4 800040F4 26310001 */  addiu      $s1, $s1, 0x1
    /* 4CF8 800040F8 4620848D */  trunc.w.d  $ft5, $ft4
    /* 4CFC 800040FC 44099000 */  mfc1       $t1, $ft5
    /* 4D00 80004100 00000000 */  nop
    /* 4D04 80004104 252A0030 */  addiu      $t2, $t1, 0x30
    /* 4D08 80004108 A22AFFFF */  sb         $t2, -0x1($s1)
    /* 4D0C 8000410C 8FAB0080 */  lw         $t3, 0x80($sp)
    /* 4D10 80004110 15600005 */  bnez       $t3, .L80004128
    /* 4D14 80004114 00000000 */   nop
    /* 4D18 80004118 8FAC0084 */  lw         $t4, 0x84($sp)
    /* 4D1C 8000411C 318D0008 */  andi       $t5, $t4, 0x8
    /* 4D20 80004120 11A00004 */  beqz       $t5, .L80004134
    /* 4D24 80004124 00000000 */   nop
  .L80004128:
    /* 4D28 80004128 240E002E */  addiu      $t6, $zero, 0x2E
    /* 4D2C 8000412C A22E0000 */  sb         $t6, 0x0($s1)
    /* 4D30 80004130 26310001 */  addiu      $s1, $s1, 0x1
  .L80004134:
    /* 4D34 80004134 1000000E */  b          .L80004170
    /* 4D38 80004138 00000000 */   nop
  .L8000413C:
    /* 4D3C 8000413C 240F0030 */  addiu      $t7, $zero, 0x30
    /* 4D40 80004140 A22F0000 */  sb         $t7, 0x0($s1)
    /* 4D44 80004144 26310001 */  addiu      $s1, $s1, 0x1
    /* 4D48 80004148 8FB80080 */  lw         $t8, 0x80($sp)
    /* 4D4C 8000414C 17000005 */  bnez       $t8, .L80004164
    /* 4D50 80004150 00000000 */   nop
    /* 4D54 80004154 8FB90084 */  lw         $t9, 0x84($sp)
    /* 4D58 80004158 33280008 */  andi       $t0, $t9, 0x8
    /* 4D5C 8000415C 11000004 */  beqz       $t0, .L80004170
    /* 4D60 80004160 00000000 */   nop
  .L80004164:
    /* 4D64 80004164 2409002E */  addiu      $t1, $zero, 0x2E
    /* 4D68 80004168 A2290000 */  sb         $t1, 0x0($s1)
    /* 4D6C 8000416C 26310001 */  addiu      $s1, $s1, 0x1
  .L80004170:
    /* 4D70 80004170 44802800 */  mtc1       $zero, $ft0f
    /* 4D74 80004174 44802000 */  mtc1       $zero, $ft0
    /* 4D78 80004178 00000000 */  nop
    /* 4D7C 8000417C 4624A032 */  c.eq.d     $fs0, $ft0
    /* 4D80 80004180 00000000 */  nop
    /* 4D84 80004184 4501002F */  bc1t       .L80004244
    /* 4D88 80004188 00000000 */   nop
    /* 4D8C 8000418C 8FAA0080 */  lw         $t2, 0x80($sp)
    /* 4D90 80004190 1140001B */  beqz       $t2, .L80004200
    /* 4D94 80004194 00000000 */   nop
  .L80004198:
    /* 4D98 80004198 3C014024 */  lui        $at, (0x40240000 >> 16)
    /* 4D9C 8000419C 44813800 */  mtc1       $at, $ft1f
    /* 4DA0 800041A0 44803000 */  mtc1       $zero, $ft1
    /* 4DA4 800041A4 27A60048 */  addiu      $a2, $sp, 0x48
    /* 4DA8 800041A8 4626A302 */  mul.d      $fa0, $fs0, $ft1
    /* 4DAC 800041AC 0C000A28 */  jal        func_800028A0
    /* 4DB0 800041B0 00000000 */   nop
    /* 4DB4 800041B4 46200506 */  mov.d      $fs0, $fv0
    /* 4DB8 800041B8 D7A80048 */  ldc1       $ft2, 0x48($sp)
    /* 4DBC 800041BC 26310001 */  addiu      $s1, $s1, 0x1
    /* 4DC0 800041C0 4620428D */  trunc.w.d  $ft3, $ft2
    /* 4DC4 800041C4 440C5000 */  mfc1       $t4, $ft3
    /* 4DC8 800041C8 00000000 */  nop
    /* 4DCC 800041CC 258D0030 */  addiu      $t5, $t4, 0x30
    /* 4DD0 800041D0 A22DFFFF */  sb         $t5, -0x1($s1)
    /* 4DD4 800041D4 8FAE0080 */  lw         $t6, 0x80($sp)
    /* 4DD8 800041D8 25CFFFFF */  addiu      $t7, $t6, -0x1
    /* 4DDC 800041DC 11E00008 */  beqz       $t7, .L80004200
    /* 4DE0 800041E0 AFAF0080 */   sw        $t7, 0x80($sp)
    /* 4DE4 800041E4 44808800 */  mtc1       $zero, $ft4f
    /* 4DE8 800041E8 44808000 */  mtc1       $zero, $ft4
    /* 4DEC 800041EC 00000000 */  nop
    /* 4DF0 800041F0 4630A032 */  c.eq.d     $fs0, $ft4
    /* 4DF4 800041F4 00000000 */  nop
    /* 4DF8 800041F8 4500FFE7 */  bc1f       .L80004198
    /* 4DFC 800041FC 00000000 */   nop
  .L80004200:
    /* 4E00 80004200 44809800 */  mtc1       $zero, $ft5f
    /* 4E04 80004204 44809000 */  mtc1       $zero, $ft5
    /* 4E08 80004208 00000000 */  nop
    /* 4E0C 8000420C 4632A032 */  c.eq.d     $fs0, $ft5
    /* 4E10 80004210 00000000 */  nop
    /* 4E14 80004214 4501000B */  bc1t       .L80004244
    /* 4E18 80004218 00000000 */   nop
    /* 4E1C 8000421C 8FB90088 */  lw         $t9, 0x88($sp)
    /* 4E20 80004220 2638FFFF */  addiu      $t8, $s1, -0x1
    /* 4E24 80004224 AFB80010 */  sw         $t8, 0x10($sp)
    /* 4E28 80004228 4620A306 */  mov.d      $fa0, $fs0
    /* 4E2C 8000422C 27A60060 */  addiu      $a2, $sp, 0x60
    /* 4E30 80004230 8FA70090 */  lw         $a3, 0x90($sp)
    /* 4E34 80004234 AFA00014 */  sw         $zero, 0x14($sp)
    /* 4E38 80004238 0C00119D */  jal        func_80004674
    /* 4E3C 8000423C AFB90018 */   sw        $t9, 0x18($sp)
    /* 4E40 80004240 AFA20090 */  sw         $v0, 0x90($sp)
  .L80004244:
    /* 4E44 80004244 8FA80080 */  lw         $t0, 0x80($sp)
    /* 4E48 80004248 01009025 */  or         $s2, $t0, $zero
    /* 4E4C 8000424C 2509FFFF */  addiu      $t1, $t0, -0x1
    /* 4E50 80004250 12400009 */  beqz       $s2, .L80004278
    /* 4E54 80004254 AFA90080 */   sw        $t1, 0x80($sp)
  .L80004258:
    /* 4E58 80004258 240A0030 */  addiu      $t2, $zero, 0x30
    /* 4E5C 8000425C A22A0000 */  sb         $t2, 0x0($s1)
    /* 4E60 80004260 8FAB0080 */  lw         $t3, 0x80($sp)
    /* 4E64 80004264 26310001 */  addiu      $s1, $s1, 0x1
    /* 4E68 80004268 01609025 */  or         $s2, $t3, $zero
    /* 4E6C 8000426C 256CFFFF */  addiu      $t4, $t3, -0x1
    /* 4E70 80004270 1640FFF9 */  bnez       $s2, .L80004258
    /* 4E74 80004274 AFAC0080 */   sw        $t4, 0x80($sp)
  .L80004278:
    /* 4E78 80004278 8FAD005C */  lw         $t5, 0x5C($sp)
    /* 4E7C 8000427C 11A0001D */  beqz       $t5, .L800042F4
    /* 4E80 80004280 00000000 */   nop
    /* 4E84 80004284 8FAE0084 */  lw         $t6, 0x84($sp)
    /* 4E88 80004288 31CF0008 */  andi       $t7, $t6, 0x8
    /* 4E8C 8000428C 15E00019 */  bnez       $t7, .L800042F4
    /* 4E90 80004290 00000000 */   nop
    /* 4E94 80004294 8FB80090 */  lw         $t8, 0x90($sp)
    /* 4E98 80004298 0311082B */  sltu       $at, $t8, $s1
    /* 4E9C 8000429C 1020000F */  beqz       $at, .L800042DC
    /* 4EA0 800042A0 00000000 */   nop
    /* 4EA4 800042A4 9239FFFF */  lbu        $t9, -0x1($s1)
    /* 4EA8 800042A8 24010030 */  addiu      $at, $zero, 0x30
    /* 4EAC 800042AC 2631FFFF */  addiu      $s1, $s1, -0x1
    /* 4EB0 800042B0 1721000A */  bne        $t9, $at, .L800042DC
    /* 4EB4 800042B4 00000000 */   nop
  .L800042B8:
    /* 4EB8 800042B8 8FA80090 */  lw         $t0, 0x90($sp)
    /* 4EBC 800042BC 0111082B */  sltu       $at, $t0, $s1
    /* 4EC0 800042C0 10200006 */  beqz       $at, .L800042DC
    /* 4EC4 800042C4 00000000 */   nop
    /* 4EC8 800042C8 9229FFFF */  lbu        $t1, -0x1($s1)
    /* 4ECC 800042CC 24010030 */  addiu      $at, $zero, 0x30
    /* 4ED0 800042D0 2631FFFF */  addiu      $s1, $s1, -0x1
    /* 4ED4 800042D4 1121FFF8 */  beq        $t1, $at, .L800042B8
    /* 4ED8 800042D8 00000000 */   nop
  .L800042DC:
    /* 4EDC 800042DC 922A0000 */  lbu        $t2, 0x0($s1)
    /* 4EE0 800042E0 2401002E */  addiu      $at, $zero, 0x2E
    /* 4EE4 800042E4 15410002 */  bne        $t2, $at, .L800042F0
    /* 4EE8 800042E8 00000000 */   nop
    /* 4EEC 800042EC 2631FFFF */  addiu      $s1, $s1, -0x1
  .L800042F0:
    /* 4EF0 800042F0 26310001 */  addiu      $s1, $s1, 0x1
  .L800042F4:
    /* 4EF4 800042F4 02202025 */  or         $a0, $s1, $zero
    /* 4EF8 800042F8 8FA50060 */  lw         $a1, 0x60($sp)
    /* 4EFC 800042FC 0C00121E */  jal        func_80004878
    /* 4F00 80004300 8FA6008C */   lw        $a2, 0x8C($sp)
    /* 4F04 80004304 00408825 */  or         $s1, $v0, $zero
    /* 4F08 80004308 100000CE */  b          .L80004644
    /* 4F0C 8000430C 00000000 */   nop
  .L80004310:
    /* 4F10 80004310 8FAB0080 */  lw         $t3, 0x80($sp)
    /* 4F14 80004314 15600004 */  bnez       $t3, .L80004328
    /* 4F18 80004318 00000000 */   nop
    /* 4F1C 8000431C 8FAC0080 */  lw         $t4, 0x80($sp)
    /* 4F20 80004320 258D0001 */  addiu      $t5, $t4, 0x1
    /* 4F24 80004324 AFAD0080 */  sw         $t5, 0x80($sp)
  .L80004328:
    /* 4F28 80004328 8FAE0060 */  lw         $t6, 0x60($sp)
    /* 4F2C 8000432C 8FAF0080 */  lw         $t7, 0x80($sp)
    /* 4F30 80004330 01EE082A */  slt        $at, $t7, $t6
    /* 4F34 80004334 14200010 */  bnez       $at, .L80004378
    /* 4F38 80004338 00000000 */   nop
    /* 4F3C 8000433C 15C00018 */  bnez       $t6, .L800043A0
    /* 4F40 80004340 00000000 */   nop
    /* 4F44 80004344 44802800 */  mtc1       $zero, $ft0f
    /* 4F48 80004348 44802000 */  mtc1       $zero, $ft0
    /* 4F4C 8000434C 00000000 */  nop
    /* 4F50 80004350 4624A032 */  c.eq.d     $fs0, $ft0
    /* 4F54 80004354 00000000 */  nop
    /* 4F58 80004358 45010011 */  bc1t       .L800043A0
    /* 4F5C 8000435C 00000000 */   nop
    /* 4F60 80004360 3C018003 */  lui        $at, %hi(D_8002D6C8)
    /* 4F64 80004364 D426D6C8 */  ldc1       $ft1, %lo(D_8002D6C8)($at)
    /* 4F68 80004368 4626A03C */  c.lt.d     $fs0, $ft1
    /* 4F6C 8000436C 00000000 */  nop
    /* 4F70 80004370 4500000B */  bc1f       .L800043A0
    /* 4F74 80004374 00000000 */   nop
  .L80004378:
    /* 4F78 80004378 8FB80080 */  lw         $t8, 0x80($sp)
    /* 4F7C 8000437C 2719FFFF */  addiu      $t9, $t8, -0x1
    /* 4F80 80004380 AFB90080 */  sw         $t9, 0x80($sp)
    /* 4F84 80004384 8FA8008C */  lw         $t0, 0x8C($sp)
    /* 4F88 80004388 2509FFFE */  addiu      $t1, $t0, -0x2
    /* 4F8C 8000438C AFA9008C */  sw         $t1, 0x8C($sp)
    /* 4F90 80004390 240A0001 */  addiu      $t2, $zero, 0x1
    /* 4F94 80004394 AFAA005C */  sw         $t2, 0x5C($sp)
    /* 4F98 80004398 1000FEF6 */  b          .L80003F74
    /* 4F9C 8000439C 00000000 */   nop
  .L800043A0:
    /* 4FA0 800043A0 8FAB0060 */  lw         $t3, 0x60($sp)
    /* 4FA4 800043A4 11600012 */  beqz       $t3, .L800043F0
    /* 4FA8 800043A8 00000000 */   nop
    /* 4FAC 800043AC 8FAC0094 */  lw         $t4, 0x94($sp)
    /* 4FB0 800043B0 26100001 */  addiu      $s0, $s0, 0x1
    /* 4FB4 800043B4 020C082B */  sltu       $at, $s0, $t4
    /* 4FB8 800043B8 1020000B */  beqz       $at, .L800043E8
    /* 4FBC 800043BC 00000000 */   nop
  .L800043C0:
    /* 4FC0 800043C0 920D0000 */  lbu        $t5, 0x0($s0)
    /* 4FC4 800043C4 26100001 */  addiu      $s0, $s0, 0x1
    /* 4FC8 800043C8 26310001 */  addiu      $s1, $s1, 0x1
    /* 4FCC 800043CC A22DFFFF */  sb         $t5, -0x1($s1)
    /* 4FD0 800043D0 8FB80094 */  lw         $t8, 0x94($sp)
    /* 4FD4 800043D4 8FAF0080 */  lw         $t7, 0x80($sp)
    /* 4FD8 800043D8 0218082B */  sltu       $at, $s0, $t8
    /* 4FDC 800043DC 25EEFFFF */  addiu      $t6, $t7, -0x1
    /* 4FE0 800043E0 1420FFF7 */  bnez       $at, .L800043C0
    /* 4FE4 800043E4 AFAE0080 */   sw        $t6, 0x80($sp)
  .L800043E8:
    /* 4FE8 800043E8 10000004 */  b          .L800043FC
    /* 4FEC 800043EC 00000000 */   nop
  .L800043F0:
    /* 4FF0 800043F0 24190030 */  addiu      $t9, $zero, 0x30
    /* 4FF4 800043F4 A2390000 */  sb         $t9, 0x0($s1)
    /* 4FF8 800043F8 26310001 */  addiu      $s1, $s1, 0x1
  .L800043FC:
    /* 4FFC 800043FC 8FA80080 */  lw         $t0, 0x80($sp)
    /* 5000 80004400 15000005 */  bnez       $t0, .L80004418
    /* 5004 80004404 00000000 */   nop
    /* 5008 80004408 8FA90084 */  lw         $t1, 0x84($sp)
    /* 500C 8000440C 312A0008 */  andi       $t2, $t1, 0x8
    /* 5010 80004410 11400007 */  beqz       $t2, .L80004430
    /* 5014 80004414 00000000 */   nop
  .L80004418:
    /* 5018 80004418 240B0001 */  addiu      $t3, $zero, 0x1
    /* 501C 8000441C AFAB0064 */  sw         $t3, 0x64($sp)
    /* 5020 80004420 240C002E */  addiu      $t4, $zero, 0x2E
    /* 5024 80004424 A22C0000 */  sb         $t4, 0x0($s1)
    /* 5028 80004428 10000002 */  b          .L80004434
    /* 502C 8000442C 26310001 */   addiu     $s1, $s1, 0x1
  .L80004430:
    /* 5030 80004430 AFA00064 */  sw         $zero, 0x64($sp)
  .L80004434:
    /* 5034 80004434 44804800 */  mtc1       $zero, $ft2f
    /* 5038 80004438 44804000 */  mtc1       $zero, $ft2
    /* 503C 8000443C 00000000 */  nop
    /* 5040 80004440 4628A032 */  c.eq.d     $fs0, $ft2
    /* 5044 80004444 00000000 */  nop
    /* 5048 80004448 45010051 */  bc1t       .L80004590
    /* 504C 8000444C 00000000 */   nop
    /* 5050 80004450 8FAD0080 */  lw         $t5, 0x80($sp)
    /* 5054 80004454 11A0003D */  beqz       $t5, .L8000454C
    /* 5058 80004458 00000000 */   nop
  .L8000445C:
    /* 505C 8000445C 3C014024 */  lui        $at, (0x40240000 >> 16)
    /* 5060 80004460 44815800 */  mtc1       $at, $ft3f
    /* 5064 80004464 44805000 */  mtc1       $zero, $ft3
    /* 5068 80004468 27A60048 */  addiu      $a2, $sp, 0x48
    /* 506C 8000446C 462AA302 */  mul.d      $fa0, $fs0, $ft3
    /* 5070 80004470 0C000A28 */  jal        func_800028A0
    /* 5074 80004474 00000000 */   nop
    /* 5078 80004478 46200506 */  mov.d      $fs0, $fv0
    /* 507C 8000447C D7B00048 */  ldc1       $ft4, 0x48($sp)
    /* 5080 80004480 26310001 */  addiu      $s1, $s1, 0x1
    /* 5084 80004484 4620848D */  trunc.w.d  $ft5, $ft4
    /* 5088 80004488 440E9000 */  mfc1       $t6, $ft5
    /* 508C 8000448C 00000000 */  nop
    /* 5090 80004490 25D80030 */  addiu      $t8, $t6, 0x30
    /* 5094 80004494 A238FFFF */  sb         $t8, -0x1($s1)
    /* 5098 80004498 D7A40048 */  ldc1       $ft0, 0x48($sp)
    /* 509C 8000449C 44803800 */  mtc1       $zero, $ft1f
    /* 50A0 800044A0 44803000 */  mtc1       $zero, $ft1
    /* 50A4 800044A4 00000000 */  nop
    /* 50A8 800044A8 46262032 */  c.eq.d     $ft0, $ft1
    /* 50AC 800044AC 00000000 */  nop
    /* 50B0 800044B0 4501FFEA */  bc1t       .L8000445C
    /* 50B4 800044B4 00000000 */   nop
    /* 50B8 800044B8 8FB90080 */  lw         $t9, 0x80($sp)
    /* 50BC 800044BC 2728FFFF */  addiu      $t0, $t9, -0x1
    /* 50C0 800044C0 11000022 */  beqz       $t0, .L8000454C
    /* 50C4 800044C4 AFA80080 */   sw        $t0, 0x80($sp)
    /* 50C8 800044C8 44804800 */  mtc1       $zero, $ft2f
    /* 50CC 800044CC 44804000 */  mtc1       $zero, $ft2
    /* 50D0 800044D0 00000000 */  nop
    /* 50D4 800044D4 4628A032 */  c.eq.d     $fs0, $ft2
    /* 50D8 800044D8 00000000 */  nop
    /* 50DC 800044DC 4501001B */  bc1t       .L8000454C
    /* 50E0 800044E0 00000000 */   nop
  .L800044E4:
    /* 50E4 800044E4 3C014024 */  lui        $at, (0x40240000 >> 16)
    /* 50E8 800044E8 44815800 */  mtc1       $at, $ft3f
    /* 50EC 800044EC 44805000 */  mtc1       $zero, $ft3
    /* 50F0 800044F0 27A60048 */  addiu      $a2, $sp, 0x48
    /* 50F4 800044F4 462AA302 */  mul.d      $fa0, $fs0, $ft3
    /* 50F8 800044F8 0C000A28 */  jal        func_800028A0
    /* 50FC 800044FC 00000000 */   nop
    /* 5100 80004500 46200506 */  mov.d      $fs0, $fv0
    /* 5104 80004504 D7B00048 */  ldc1       $ft4, 0x48($sp)
    /* 5108 80004508 26310001 */  addiu      $s1, $s1, 0x1
    /* 510C 8000450C 4620848D */  trunc.w.d  $ft5, $ft4
    /* 5110 80004510 440A9000 */  mfc1       $t2, $ft5
    /* 5114 80004514 00000000 */  nop
    /* 5118 80004518 254B0030 */  addiu      $t3, $t2, 0x30
    /* 511C 8000451C A22BFFFF */  sb         $t3, -0x1($s1)
    /* 5120 80004520 8FAC0080 */  lw         $t4, 0x80($sp)
    /* 5124 80004524 258DFFFF */  addiu      $t5, $t4, -0x1
    /* 5128 80004528 11A00008 */  beqz       $t5, .L8000454C
    /* 512C 8000452C AFAD0080 */   sw        $t5, 0x80($sp)
    /* 5130 80004530 44802800 */  mtc1       $zero, $ft0f
    /* 5134 80004534 44802000 */  mtc1       $zero, $ft0
    /* 5138 80004538 00000000 */  nop
    /* 513C 8000453C 4624A032 */  c.eq.d     $fs0, $ft0
    /* 5140 80004540 00000000 */  nop
    /* 5144 80004544 4500FFE7 */  bc1f       .L800044E4
    /* 5148 80004548 00000000 */   nop
  .L8000454C:
    /* 514C 8000454C 44803800 */  mtc1       $zero, $ft1f
    /* 5150 80004550 44803000 */  mtc1       $zero, $ft1
    /* 5154 80004554 00000000 */  nop
    /* 5158 80004558 4626A032 */  c.eq.d     $fs0, $ft1
    /* 515C 8000455C 00000000 */  nop
    /* 5160 80004560 4501000B */  bc1t       .L80004590
    /* 5164 80004564 00000000 */   nop
    /* 5168 80004568 8FAE0088 */  lw         $t6, 0x88($sp)
    /* 516C 8000456C 262FFFFF */  addiu      $t7, $s1, -0x1
    /* 5170 80004570 AFAF0010 */  sw         $t7, 0x10($sp)
    /* 5174 80004574 4620A306 */  mov.d      $fa0, $fs0
    /* 5178 80004578 00003025 */  or         $a2, $zero, $zero
    /* 517C 8000457C 8FA70090 */  lw         $a3, 0x90($sp)
    /* 5180 80004580 AFA00014 */  sw         $zero, 0x14($sp)
    /* 5184 80004584 0C00119D */  jal        func_80004674
    /* 5188 80004588 AFAE0018 */   sw        $t6, 0x18($sp)
    /* 518C 8000458C AFA20090 */  sw         $v0, 0x90($sp)
  .L80004590:
    /* 5190 80004590 8FB80084 */  lw         $t8, 0x84($sp)
    /* 5194 80004594 33190008 */  andi       $t9, $t8, 0x8
    /* 5198 80004598 13200010 */  beqz       $t9, .L800045DC
    /* 519C 8000459C 00000000 */   nop
    /* 51A0 800045A0 8FA80080 */  lw         $t0, 0x80($sp)
    /* 51A4 800045A4 01009025 */  or         $s2, $t0, $zero
    /* 51A8 800045A8 2509FFFF */  addiu      $t1, $t0, -0x1
    /* 51AC 800045AC 12400009 */  beqz       $s2, .L800045D4
    /* 51B0 800045B0 AFA90080 */   sw        $t1, 0x80($sp)
  .L800045B4:
    /* 51B4 800045B4 240A0030 */  addiu      $t2, $zero, 0x30
    /* 51B8 800045B8 A22A0000 */  sb         $t2, 0x0($s1)
    /* 51BC 800045BC 8FAB0080 */  lw         $t3, 0x80($sp)
    /* 51C0 800045C0 26310001 */  addiu      $s1, $s1, 0x1
    /* 51C4 800045C4 01609025 */  or         $s2, $t3, $zero
    /* 51C8 800045C8 256CFFFF */  addiu      $t4, $t3, -0x1
    /* 51CC 800045CC 1640FFF9 */  bnez       $s2, .L800045B4
    /* 51D0 800045D0 AFAC0080 */   sw        $t4, 0x80($sp)
  .L800045D4:
    /* 51D4 800045D4 1000001B */  b          .L80004644
    /* 51D8 800045D8 00000000 */   nop
  .L800045DC:
    /* 51DC 800045DC 8FAD0064 */  lw         $t5, 0x64($sp)
    /* 51E0 800045E0 11A00018 */  beqz       $t5, .L80004644
    /* 51E4 800045E4 00000000 */   nop
    /* 51E8 800045E8 8FAF0090 */  lw         $t7, 0x90($sp)
    /* 51EC 800045EC 01F1082B */  sltu       $at, $t7, $s1
    /* 51F0 800045F0 1020000F */  beqz       $at, .L80004630
    /* 51F4 800045F4 00000000 */   nop
    /* 51F8 800045F8 922EFFFF */  lbu        $t6, -0x1($s1)
    /* 51FC 800045FC 24010030 */  addiu      $at, $zero, 0x30
    /* 5200 80004600 2631FFFF */  addiu      $s1, $s1, -0x1
    /* 5204 80004604 15C1000A */  bne        $t6, $at, .L80004630
    /* 5208 80004608 00000000 */   nop
  .L8000460C:
    /* 520C 8000460C 8FB80090 */  lw         $t8, 0x90($sp)
    /* 5210 80004610 0311082B */  sltu       $at, $t8, $s1
    /* 5214 80004614 10200006 */  beqz       $at, .L80004630
    /* 5218 80004618 00000000 */   nop
    /* 521C 8000461C 9239FFFF */  lbu        $t9, -0x1($s1)
    /* 5220 80004620 24010030 */  addiu      $at, $zero, 0x30
    /* 5224 80004624 2631FFFF */  addiu      $s1, $s1, -0x1
    /* 5228 80004628 1321FFF8 */  beq        $t9, $at, .L8000460C
    /* 522C 8000462C 00000000 */   nop
  .L80004630:
    /* 5230 80004630 92280000 */  lbu        $t0, 0x0($s1)
    /* 5234 80004634 2401002E */  addiu      $at, $zero, 0x2E
    /* 5238 80004638 11010002 */  beq        $t0, $at, .L80004644
    /* 523C 8000463C 00000000 */   nop
    /* 5240 80004640 26310001 */  addiu      $s1, $s1, 0x1
  .L80004644:
    /* 5244 80004644 8FA90090 */  lw         $t1, 0x90($sp)
    /* 5248 80004648 10000003 */  b          .L80004658
    /* 524C 8000464C 02291023 */   subu      $v0, $s1, $t1
    /* 5250 80004650 10000001 */  b          .L80004658
    /* 5254 80004654 00000000 */   nop
  .L80004658:
    /* 5258 80004658 8FBF003C */  lw         $ra, 0x3C($sp)
    /* 525C 8000465C D7B40028 */  ldc1       $fs0, 0x28($sp)
    /* 5260 80004660 8FB00030 */  lw         $s0, 0x30($sp)
    /* 5264 80004664 8FB10034 */  lw         $s1, 0x34($sp)
    /* 5268 80004668 8FB20038 */  lw         $s2, 0x38($sp)
    /* 526C 8000466C 03E00008 */  jr         $ra
    /* 5270 80004670 27BD0078 */   addiu     $sp, $sp, 0x78
```

## Tips for This Category

