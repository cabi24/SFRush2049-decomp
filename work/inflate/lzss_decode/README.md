# lzss_decode

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80004AFC` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~149 |

## Description

LZSS back-reference decoder

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `inflate` category.

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
./work/inflate/lzss_decode/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/lzss_decode

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/lzss_decode --watch
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
# Source: 5610.s
# Address: 0x80004AFC

glabel func_80004AFC
    /* 56FC 80004AFC 27BDFF78 */  addiu      $sp, $sp, -0x88
    /* 5700 80004B00 AFB70044 */  sw         $s7, 0x44($sp)
    /* 5704 80004B04 AFB5003C */  sw         $s5, 0x3C($sp)
    /* 5708 80004B08 3C098008 */  lui        $t1, %hi(D_80084A50)
    /* 570C 80004B0C 25294A50 */  addiu      $t1, $t1, %lo(D_80084A50)
    /* 5710 80004B10 0080A825 */  or         $s5, $a0, $zero
    /* 5714 80004B14 00A0B825 */  or         $s7, $a1, $zero
    /* 5718 80004B18 AFBF004C */  sw         $ra, 0x4C($sp)
    /* 571C 80004B1C AFB00028 */  sw         $s0, 0x28($sp)
    /* 5720 80004B20 240E0001 */  addiu      $t6, $zero, 0x1
    /* 5724 80004B24 3C018003 */  lui        $at, %hi(D_80035480)
    /* 5728 80004B28 00004025 */  or         $t0, $zero, $zero
    /* 572C 80004B2C AFA8005C */  sw         $t0, 0x5C($sp)
    /* 5730 80004B30 AC2E5480 */  sw         $t6, %lo(D_80035480)($at)
    /* 5734 80004B34 02E08025 */  or         $s0, $s7, $zero
    /* 5738 80004B38 24051000 */  addiu      $a1, $zero, 0x1000
    /* 573C 80004B3C 01202025 */  or         $a0, $t1, $zero
    /* 5740 80004B40 AFA90080 */  sw         $t1, 0x80($sp)
    /* 5744 80004B44 AFB40038 */  sw         $s4, 0x38($sp)
    /* 5748 80004B48 AFB30034 */  sw         $s3, 0x34($sp)
    /* 574C 80004B4C AFB20030 */  sw         $s2, 0x30($sp)
    /* 5750 80004B50 0C002138 */  jal        func_800084E0
    /* 5754 80004B54 AFB1002C */   sw        $s1, 0x2C($sp)
    /* 5758 80004B58 8FA90080 */  lw         $t1, 0x80($sp)
    /* 575C 80004B5C 3C188003 */  lui        $t8, %hi(D_80035428)
    /* 5760 80004B60 27185428 */  addiu      $t8, $t8, %lo(D_80035428)
    /* 5764 80004B64 3C048003 */  lui        $a0, %hi(D_80035488)
    /* 5768 80004B68 240F1000 */  addiu      $t7, $zero, 0x1000
    /* 576C 80004B6C AFAF0014 */  sw         $t7, 0x14($sp)
    /* 5770 80004B70 24845488 */  addiu      $a0, $a0, %lo(D_80035488)
    /* 5774 80004B74 AFB80018 */  sw         $t8, 0x18($sp)
    /* 5778 80004B78 8FA8005C */  lw         $t0, 0x5C($sp)
    /* 577C 80004B7C 00002825 */  or         $a1, $zero, $zero
    /* 5780 80004B80 00003025 */  or         $a2, $zero, $zero
    /* 5784 80004B84 02A03825 */  or         $a3, $s5, $zero
    /* 5788 80004B88 0C00218C */  jal        func_80008630
    /* 578C 80004B8C AFA90010 */   sw        $t1, 0x10($sp)
    /* 5790 80004B90 26B91000 */  addiu      $t9, $s5, 0x1000
    /* 5794 80004B94 3C018003 */  lui        $at, %hi(D_800354A0)
    /* 5798 80004B98 AC3954A0 */  sw         $t9, %lo(D_800354A0)($at)
    /* 579C 80004B9C 8FA8005C */  lw         $t0, 0x5C($sp)
    /* 57A0 80004BA0 8FA90080 */  lw         $t1, 0x80($sp)
    /* 57A4 80004BA4 00002825 */  or         $a1, $zero, $zero
  .L80004BA8:
    /* 57A8 80004BA8 14A0000B */  bnez       $a1, .L80004BD8
    /* 57AC 80004BAC 00000000 */   nop
    /* 57B0 80004BB0 1D000005 */  bgtz       $t0, .L80004BC8
    /* 57B4 80004BB4 00000000 */   nop
    /* 57B8 80004BB8 0C001284 */  jal        func_80004A10
    /* 57BC 80004BBC AFB00054 */   sw        $s0, 0x54($sp)
    /* 57C0 80004BC0 8FB00054 */  lw         $s0, 0x54($sp)
    /* 57C4 80004BC4 24081000 */  addiu      $t0, $zero, 0x1000
  .L80004BC8:
    /* 57C8 80004BC8 91260000 */  lbu        $a2, 0x0($t1)
    /* 57CC 80004BCC 25290001 */  addiu      $t1, $t1, 0x1
    /* 57D0 80004BD0 2508FFFF */  addiu      $t0, $t0, -0x1
    /* 57D4 80004BD4 24050008 */  addiu      $a1, $zero, 0x8
  .L80004BD8:
    /* 57D8 80004BD8 30D80001 */  andi       $t8, $a2, 0x1
    /* 57DC 80004BDC 13000011 */  beqz       $t8, .L80004C24
    /* 57E0 80004BE0 24A5FFFF */   addiu     $a1, $a1, -0x1
    /* 57E4 80004BE4 1D000009 */  bgtz       $t0, .L80004C0C
    /* 57E8 80004BE8 00000000 */   nop
    /* 57EC 80004BEC AFA50060 */  sw         $a1, 0x60($sp)
    /* 57F0 80004BF0 AFA60058 */  sw         $a2, 0x58($sp)
    /* 57F4 80004BF4 0C001284 */  jal        func_80004A10
    /* 57F8 80004BF8 AFB00054 */   sw        $s0, 0x54($sp)
    /* 57FC 80004BFC 8FA50060 */  lw         $a1, 0x60($sp)
    /* 5800 80004C00 8FA60058 */  lw         $a2, 0x58($sp)
    /* 5804 80004C04 8FB00054 */  lw         $s0, 0x54($sp)
    /* 5808 80004C08 24081000 */  addiu      $t0, $zero, 0x1000
  .L80004C0C:
    /* 580C 80004C0C 91220000 */  lbu        $v0, 0x0($t1)
    /* 5810 80004C10 25290001 */  addiu      $t1, $t1, 0x1
    /* 5814 80004C14 2508FFFF */  addiu      $t0, $t0, -0x1
    /* 5818 80004C18 26F70001 */  addiu      $s7, $s7, 0x1
    /* 581C 80004C1C 1000003F */  b          .L80004D1C
    /* 5820 80004C20 A2E2FFFF */   sb        $v0, -0x1($s7)
  .L80004C24:
    /* 5824 80004C24 1D000009 */  bgtz       $t0, .L80004C4C
    /* 5828 80004C28 00000000 */   nop
    /* 582C 80004C2C AFA50060 */  sw         $a1, 0x60($sp)
    /* 5830 80004C30 AFA60058 */  sw         $a2, 0x58($sp)
    /* 5834 80004C34 0C001284 */  jal        func_80004A10
    /* 5838 80004C38 AFB00054 */   sw        $s0, 0x54($sp)
    /* 583C 80004C3C 8FA50060 */  lw         $a1, 0x60($sp)
    /* 5840 80004C40 8FA60058 */  lw         $a2, 0x58($sp)
    /* 5844 80004C44 8FB00054 */  lw         $s0, 0x54($sp)
    /* 5848 80004C48 24081000 */  addiu      $t0, $zero, 0x1000
  .L80004C4C:
    /* 584C 80004C4C 91220000 */  lbu        $v0, 0x0($t1)
    /* 5850 80004C50 2508FFFF */  addiu      $t0, $t0, -0x1
    /* 5854 80004C54 25290001 */  addiu      $t1, $t1, 0x1
    /* 5858 80004C58 304400F0 */  andi       $a0, $v0, 0xF0
    /* 585C 80004C5C 00042100 */  sll        $a0, $a0, 4
    /* 5860 80004C60 1D00000B */  bgtz       $t0, .L80004C90
    /* 5864 80004C64 3055000F */   andi      $s5, $v0, 0xF
    /* 5868 80004C68 AFA40074 */  sw         $a0, 0x74($sp)
    /* 586C 80004C6C AFA50060 */  sw         $a1, 0x60($sp)
    /* 5870 80004C70 AFA60058 */  sw         $a2, 0x58($sp)
    /* 5874 80004C74 0C001284 */  jal        func_80004A10
    /* 5878 80004C78 AFB00054 */   sw        $s0, 0x54($sp)
    /* 587C 80004C7C 8FA40074 */  lw         $a0, 0x74($sp)
    /* 5880 80004C80 8FA50060 */  lw         $a1, 0x60($sp)
    /* 5884 80004C84 8FA60058 */  lw         $a2, 0x58($sp)
    /* 5888 80004C88 8FB00054 */  lw         $s0, 0x54($sp)
    /* 588C 80004C8C 24081000 */  addiu      $t0, $zero, 0x1000
  .L80004C90:
    /* 5890 80004C90 91220000 */  lbu        $v0, 0x0($t1)
    /* 5894 80004C94 25290001 */  addiu      $t1, $t1, 0x1
    /* 5898 80004C98 2508FFFF */  addiu      $t0, $t0, -0x1
    /* 589C 80004C9C 00822021 */  addu       $a0, $a0, $v0
    /* 58A0 80004CA0 30840FFF */  andi       $a0, $a0, 0xFFF
    /* 58A4 80004CA4 14800015 */  bnez       $a0, .L80004CFC
    /* 58A8 80004CA8 00000000 */   nop
    /* 58AC 80004CAC 16A00013 */  bnez       $s5, .L80004CFC
    /* 58B0 80004CB0 00000000 */   nop
    /* 58B4 80004CB4 27B50084 */  addiu      $s5, $sp, 0x84
    /* 58B8 80004CB8 3C048003 */  lui        $a0, %hi(D_80035428)
    /* 58BC 80004CBC 24845428 */  addiu      $a0, $a0, %lo(D_80035428)
    /* 58C0 80004CC0 02A02825 */  or         $a1, $s5, $zero
    /* 58C4 80004CC4 0C001C9C */  jal        func_80007270
    /* 58C8 80004CC8 00003025 */   or        $a2, $zero, $zero
    /* 58CC 80004CCC 2416FFFF */  addiu      $s6, $zero, -0x1
    /* 58D0 80004CD0 14560008 */  bne        $v0, $s6, .L80004CF4
    /* 58D4 80004CD4 00000000 */   nop
  .L80004CD8:
    /* 58D8 80004CD8 3C048003 */  lui        $a0, %hi(D_80035428)
    /* 58DC 80004CDC 24845428 */  addiu      $a0, $a0, %lo(D_80035428)
    /* 58E0 80004CE0 02A02825 */  or         $a1, $s5, $zero
    /* 58E4 80004CE4 0C001C9C */  jal        func_80007270
    /* 58E8 80004CE8 00003025 */   or        $a2, $zero, $zero
    /* 58EC 80004CEC 1056FFFA */  beq        $v0, $s6, .L80004CD8
    /* 58F0 80004CF0 00000000 */   nop
  .L80004CF4:
    /* 58F4 80004CF4 1000000B */  b          .L80004D24
    /* 58F8 80004CF8 02F01023 */   subu      $v0, $s7, $s0
  .L80004CFC:
    /* 58FC 80004CFC 26B50001 */  addiu      $s5, $s5, 0x1
    /* 5900 80004D00 02E42023 */  subu       $a0, $s7, $a0
  .L80004D04:
    /* 5904 80004D04 90980000 */  lbu        $t8, 0x0($a0)
    /* 5908 80004D08 26B5FFFF */  addiu      $s5, $s5, -0x1
    /* 590C 80004D0C 26F70001 */  addiu      $s7, $s7, 0x1
    /* 5910 80004D10 24840001 */  addiu      $a0, $a0, 0x1
    /* 5914 80004D14 06A1FFFB */  bgez       $s5, .L80004D04
    /* 5918 80004D18 A2F8FFFF */   sb        $t8, -0x1($s7)
  .L80004D1C:
    /* 591C 80004D1C 1000FFA2 */  b          .L80004BA8
    /* 5920 80004D20 00063042 */   srl       $a2, $a2, 1
  .L80004D24:
    /* 5924 80004D24 8FBF004C */  lw         $ra, 0x4C($sp)
    /* 5928 80004D28 8FB00028 */  lw         $s0, 0x28($sp)
    /* 592C 80004D2C 8FB1002C */  lw         $s1, 0x2C($sp)
    /* 5930 80004D30 8FB20030 */  lw         $s2, 0x30($sp)
    /* 5934 80004D34 8FB30034 */  lw         $s3, 0x34($sp)
    /* 5938 80004D38 8FB40038 */  lw         $s4, 0x38($sp)
    /* 593C 80004D3C 8FB5003C */  lw         $s5, 0x3C($sp)
    /* 5940 80004D40 8FB60040 */  lw         $s6, 0x40($sp)
    /* 5944 80004D44 8FB70044 */  lw         $s7, 0x44($sp)
    /* 5948 80004D48 03E00008 */  jr         $ra
    /* 594C 80004D4C 27BD0088 */   addiu     $sp, $sp, 0x88
```

## Tips for This Category

