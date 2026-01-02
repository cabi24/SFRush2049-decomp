# fcvt

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80002CD0` |
| **Category** | `libm` |
| **Status** | `TODO` |
| **Instructions** | ~987 |

## Description

convert float to string

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
./work/libm/fcvt/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libm/fcvt

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libm/fcvt --watch
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
# Address: 0x80002CD0

glabel func_80002CD0
    /* 38D0 80002CD0 27BDFDD8 */  addiu      $sp, $sp, -0x228
    /* 38D4 80002CD4 AFBF0044 */  sw         $ra, 0x44($sp)
    /* 38D8 80002CD8 AFA40228 */  sw         $a0, 0x228($sp)
    /* 38DC 80002CDC AFA5022C */  sw         $a1, 0x22C($sp)
    /* 38E0 80002CE0 AFA60230 */  sw         $a2, 0x230($sp)
    /* 38E4 80002CE4 AFB60040 */  sw         $s6, 0x40($sp)
    /* 38E8 80002CE8 AFB5003C */  sw         $s5, 0x3C($sp)
    /* 38EC 80002CEC AFB40038 */  sw         $s4, 0x38($sp)
    /* 38F0 80002CF0 AFB30034 */  sw         $s3, 0x34($sp)
    /* 38F4 80002CF4 AFB20030 */  sw         $s2, 0x30($sp)
    /* 38F8 80002CF8 AFB1002C */  sw         $s1, 0x2C($sp)
    /* 38FC 80002CFC AFB00028 */  sw         $s0, 0x28($sp)
    /* 3900 80002D00 AFA001D8 */  sw         $zero, 0x1D8($sp)
    /* 3904 80002D04 8FAE0228 */  lw         $t6, 0x228($sp)
    /* 3908 80002D08 AFAE0074 */  sw         $t6, 0x74($sp)
    /* 390C 80002D0C 3C188003 */  lui        $t8, %hi(D_8002D4D0)
    /* 3910 80002D10 2718D4D0 */  addiu      $t8, $t8, %lo(D_8002D4D0)
    /* 3914 80002D14 8F010000 */  lw         $at, 0x0($t8)
    /* 3918 80002D18 8F090004 */  lw         $t1, 0x4($t8)
    /* 391C 80002D1C 27AF0064 */  addiu      $t7, $sp, 0x64
    /* 3920 80002D20 ADE10000 */  sw         $at, 0x0($t7)
    /* 3924 80002D24 ADE90004 */  sw         $t1, 0x4($t7)
    /* 3928 80002D28 8F09000C */  lw         $t1, 0xC($t8)
    /* 392C 80002D2C 8F010008 */  lw         $at, 0x8($t8)
    /* 3930 80002D30 ADE9000C */  sw         $t1, 0xC($t7)
    /* 3934 80002D34 ADE10008 */  sw         $at, 0x8($t7)
    /* 3938 80002D38 3C0B8003 */  lui        $t3, %hi(D_8002D4E0)
    /* 393C 80002D3C 256BD4E0 */  addiu      $t3, $t3, %lo(D_8002D4E0)
    /* 3940 80002D40 8D610000 */  lw         $at, 0x0($t3)
    /* 3944 80002D44 8D6E0004 */  lw         $t6, 0x4($t3)
    /* 3948 80002D48 27AA0054 */  addiu      $t2, $sp, 0x54
    /* 394C 80002D4C AD410000 */  sw         $at, 0x0($t2)
    /* 3950 80002D50 AD4E0004 */  sw         $t6, 0x4($t2)
    /* 3954 80002D54 8D6E000C */  lw         $t6, 0xC($t3)
    /* 3958 80002D58 8D610008 */  lw         $at, 0x8($t3)
    /* 395C 80002D5C AD4E000C */  sw         $t6, 0xC($t2)
    /* 3960 80002D60 AD410008 */  sw         $at, 0x8($t2)
    /* 3964 80002D64 8FB0022C */  lw         $s0, 0x22C($sp)
    /* 3968 80002D68 AFA00210 */  sw         $zero, 0x210($sp)
  .L80002D6C:
    /* 396C 80002D6C 92110000 */  lbu        $s1, 0x0($s0)
    /* 3970 80002D70 02009825 */  or         $s3, $s0, $zero
    /* 3974 80002D74 1220000B */  beqz       $s1, .L80002DA4
    /* 3978 80002D78 00000000 */   nop
    /* 397C 80002D7C 24010025 */  addiu      $at, $zero, 0x25
    /* 3980 80002D80 12210008 */  beq        $s1, $at, .L80002DA4
    /* 3984 80002D84 00000000 */   nop
  .L80002D88:
    /* 3988 80002D88 92110001 */  lbu        $s1, 0x1($s0)
    /* 398C 80002D8C 26100001 */  addiu      $s0, $s0, 0x1
    /* 3990 80002D90 12200004 */  beqz       $s1, .L80002DA4
    /* 3994 80002D94 00000000 */   nop
    /* 3998 80002D98 24010025 */  addiu      $at, $zero, 0x25
    /* 399C 80002D9C 1621FFFA */  bne        $s1, $at, .L80002D88
    /* 39A0 80002DA0 00000000 */   nop
  .L80002DA4:
    /* 39A4 80002DA4 02139023 */  subu       $s2, $s0, $s3
    /* 39A8 80002DA8 1240000B */  beqz       $s2, .L80002DD8
    /* 39AC 80002DAC 00000000 */   nop
    /* 39B0 80002DB0 8FA40228 */  lw         $a0, 0x228($sp)
    /* 39B4 80002DB4 02602825 */  or         $a1, $s3, $zero
    /* 39B8 80002DB8 0C001F1A */  jal        func_80007C68
    /* 39BC 80002DBC 02403025 */   or        $a2, $s2, $zero
    /* 39C0 80002DC0 8FA80228 */  lw         $t0, 0x228($sp)
    /* 39C4 80002DC4 0112C821 */  addu       $t9, $t0, $s2
    /* 39C8 80002DC8 AFB90228 */  sw         $t9, 0x228($sp)
    /* 39CC 80002DCC 8FAF0210 */  lw         $t7, 0x210($sp)
    /* 39D0 80002DD0 01F2C021 */  addu       $t8, $t7, $s2
    /* 39D4 80002DD4 AFB80210 */  sw         $t8, 0x210($sp)
  .L80002DD8:
    /* 39D8 80002DD8 16200003 */  bnez       $s1, .L80002DE8
    /* 39DC 80002DDC 00000000 */   nop
    /* 39E0 80002DE0 10000384 */  b          .L80003BF4
    /* 39E4 80002DE4 00000000 */   nop
  .L80002DE8:
    /* 39E8 80002DE8 26100001 */  addiu      $s0, $s0, 0x1
    /* 39EC 80002DEC 0000A025 */  or         $s4, $zero, $zero
    /* 39F0 80002DF0 AFA001E8 */  sw         $zero, 0x1E8($sp)
    /* 39F4 80002DF4 AFA001F4 */  sw         $zero, 0x1F4($sp)
    /* 39F8 80002DF8 AFA0020C */  sw         $zero, 0x20C($sp)
    /* 39FC 80002DFC 2409FFFF */  addiu      $t1, $zero, -0x1
    /* 3A00 80002E00 AFA90208 */  sw         $t1, 0x208($sp)
    /* 3A04 80002E04 A3A00207 */  sb         $zero, 0x207($sp)
  .L80002E08:
    /* 3A08 80002E08 92110000 */  lbu        $s1, 0x0($s0)
    /* 3A0C 80002E0C 26100001 */  addiu      $s0, $s0, 0x1
  .L80002E10:
    /* 3A10 80002E10 262DFFE0 */  addiu      $t5, $s1, -0x20
    /* 3A14 80002E14 2DA10059 */  sltiu      $at, $t5, 0x59
    /* 3A18 80002E18 10200295 */  beqz       $at, .L80003870
    /* 3A1C 80002E1C 00000000 */   nop
    /* 3A20 80002E20 000D6880 */  sll        $t5, $t5, 2
    /* 3A24 80002E24 3C018003 */  lui        $at, %hi(jtbl_8002D558_main)
    /* 3A28 80002E28 002D0821 */  addu       $at, $at, $t5
    /* 3A2C 80002E2C 8C2DD558 */  lw         $t5, %lo(jtbl_8002D558_main)($at)
    /* 3A30 80002E30 01A00008 */  jr         $t5
    /* 3A34 80002E34 00000000 */   nop
    /* 3A38 80002E38 93AC0207 */  lbu        $t4, 0x207($sp)
    /* 3A3C 80002E3C 15800003 */  bnez       $t4, .L80002E4C
    /* 3A40 80002E40 00000000 */   nop
    /* 3A44 80002E44 240A0020 */  addiu      $t2, $zero, 0x20
    /* 3A48 80002E48 A3AA0207 */  sb         $t2, 0x207($sp)
  .L80002E4C:
    /* 3A4C 80002E4C 1000FFEE */  b          .L80002E08
    /* 3A50 80002E50 00000000 */   nop
    /* 3A54 80002E54 368B0008 */  ori        $t3, $s4, 0x8
    /* 3A58 80002E58 0160A025 */  or         $s4, $t3, $zero
    /* 3A5C 80002E5C 1000FFEA */  b          .L80002E08
    /* 3A60 80002E60 00000000 */   nop
    /* 3A64 80002E64 8FAE0230 */  lw         $t6, 0x230($sp)
    /* 3A68 80002E68 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3A6C 80002E6C 25C80003 */  addiu      $t0, $t6, 0x3
    /* 3A70 80002E70 0101C824 */  and        $t9, $t0, $at
    /* 3A74 80002E74 272F0004 */  addiu      $t7, $t9, 0x4
    /* 3A78 80002E78 AFAF0230 */  sw         $t7, 0x230($sp)
    /* 3A7C 80002E7C 8F380000 */  lw         $t8, 0x0($t9)
    /* 3A80 80002E80 07000003 */  bltz       $t8, .L80002E90
    /* 3A84 80002E84 AFB8020C */   sw        $t8, 0x20C($sp)
    /* 3A88 80002E88 1000FFDF */  b          .L80002E08
    /* 3A8C 80002E8C 00000000 */   nop
  .L80002E90:
    /* 3A90 80002E90 8FA9020C */  lw         $t1, 0x20C($sp)
    /* 3A94 80002E94 00096823 */  negu       $t5, $t1
    /* 3A98 80002E98 AFAD020C */  sw         $t5, 0x20C($sp)
    /* 3A9C 80002E9C 368C0010 */  ori        $t4, $s4, 0x10
    /* 3AA0 80002EA0 0180A025 */  or         $s4, $t4, $zero
    /* 3AA4 80002EA4 1000FFD8 */  b          .L80002E08
    /* 3AA8 80002EA8 00000000 */   nop
    /* 3AAC 80002EAC 240A002B */  addiu      $t2, $zero, 0x2B
    /* 3AB0 80002EB0 A3AA0207 */  sb         $t2, 0x207($sp)
    /* 3AB4 80002EB4 1000FFD4 */  b          .L80002E08
    /* 3AB8 80002EB8 00000000 */   nop
    /* 3ABC 80002EBC 92110000 */  lbu        $s1, 0x0($s0)
    /* 3AC0 80002EC0 26100001 */  addiu      $s0, $s0, 0x1
    /* 3AC4 80002EC4 3A35002A */  xori       $s5, $s1, 0x2A
    /* 3AC8 80002EC8 2EB50001 */  sltiu      $s5, $s5, 0x1
    /* 3ACC 80002ECC 12A00010 */  beqz       $s5, .L80002F10
    /* 3AD0 80002ED0 00000000 */   nop
    /* 3AD4 80002ED4 8FAB0230 */  lw         $t3, 0x230($sp)
    /* 3AD8 80002ED8 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3ADC 80002EDC 256E0003 */  addiu      $t6, $t3, 0x3
    /* 3AE0 80002EE0 01C14024 */  and        $t0, $t6, $at
    /* 3AE4 80002EE4 250F0004 */  addiu      $t7, $t0, 0x4
    /* 3AE8 80002EE8 AFAF0230 */  sw         $t7, 0x230($sp)
    /* 3AEC 80002EEC 8D120000 */  lw         $s2, 0x0($t0)
    /* 3AF0 80002EF0 06410004 */  bgez       $s2, .L80002F04
    /* 3AF4 80002EF4 00000000 */   nop
    /* 3AF8 80002EF8 2419FFFF */  addiu      $t9, $zero, -0x1
    /* 3AFC 80002EFC 10000002 */  b          .L80002F08
    /* 3B00 80002F00 AFB90208 */   sw        $t9, 0x208($sp)
  .L80002F04:
    /* 3B04 80002F04 AFB20208 */  sw         $s2, 0x208($sp)
  .L80002F08:
    /* 3B08 80002F08 1000FFBF */  b          .L80002E08
    /* 3B0C 80002F0C 00000000 */   nop
  .L80002F10:
    /* 3B10 80002F10 00009025 */  or         $s2, $zero, $zero
    /* 3B14 80002F14 2638FFD0 */  addiu      $t8, $s1, -0x30
    /* 3B18 80002F18 2F01000A */  sltiu      $at, $t8, 0xA
    /* 3B1C 80002F1C 1020000C */  beqz       $at, .L80002F50
    /* 3B20 80002F20 00000000 */   nop
  .L80002F24:
    /* 3B24 80002F24 00124880 */  sll        $t1, $s2, 2
    /* 3B28 80002F28 01324821 */  addu       $t1, $t1, $s2
    /* 3B2C 80002F2C 00094840 */  sll        $t1, $t1, 1
    /* 3B30 80002F30 01319021 */  addu       $s2, $t1, $s1
    /* 3B34 80002F34 2652FFD0 */  addiu      $s2, $s2, -0x30
    /* 3B38 80002F38 92110000 */  lbu        $s1, 0x0($s0)
    /* 3B3C 80002F3C 26100001 */  addiu      $s0, $s0, 0x1
    /* 3B40 80002F40 262DFFD0 */  addiu      $t5, $s1, -0x30
    /* 3B44 80002F44 2DA1000A */  sltiu      $at, $t5, 0xA
    /* 3B48 80002F48 1420FFF6 */  bnez       $at, .L80002F24
    /* 3B4C 80002F4C 00000000 */   nop
  .L80002F50:
    /* 3B50 80002F50 06410004 */  bgez       $s2, .L80002F64
    /* 3B54 80002F54 00000000 */   nop
    /* 3B58 80002F58 240CFFFF */  addiu      $t4, $zero, -0x1
    /* 3B5C 80002F5C 10000002 */  b          .L80002F68
    /* 3B60 80002F60 AFAC0208 */   sw        $t4, 0x208($sp)
  .L80002F64:
    /* 3B64 80002F64 AFB20208 */  sw         $s2, 0x208($sp)
  .L80002F68:
    /* 3B68 80002F68 1000FFA9 */  b          .L80002E10
    /* 3B6C 80002F6C 00000000 */   nop
    /* 3B70 80002F70 368A0020 */  ori        $t2, $s4, 0x20
    /* 3B74 80002F74 0140A025 */  or         $s4, $t2, $zero
    /* 3B78 80002F78 1000FFA3 */  b          .L80002E08
    /* 3B7C 80002F7C 00000000 */   nop
    /* 3B80 80002F80 00009025 */  or         $s2, $zero, $zero
  .L80002F84:
    /* 3B84 80002F84 00125880 */  sll        $t3, $s2, 2
    /* 3B88 80002F88 01725821 */  addu       $t3, $t3, $s2
    /* 3B8C 80002F8C 000B5840 */  sll        $t3, $t3, 1
    /* 3B90 80002F90 01719021 */  addu       $s2, $t3, $s1
    /* 3B94 80002F94 2652FFD0 */  addiu      $s2, $s2, -0x30
    /* 3B98 80002F98 92110000 */  lbu        $s1, 0x0($s0)
    /* 3B9C 80002F9C 26100001 */  addiu      $s0, $s0, 0x1
    /* 3BA0 80002FA0 262EFFD0 */  addiu      $t6, $s1, -0x30
    /* 3BA4 80002FA4 2DC1000A */  sltiu      $at, $t6, 0xA
    /* 3BA8 80002FA8 1420FFF6 */  bnez       $at, .L80002F84
    /* 3BAC 80002FAC 00000000 */   nop
    /* 3BB0 80002FB0 AFB2020C */  sw         $s2, 0x20C($sp)
    /* 3BB4 80002FB4 1000FF96 */  b          .L80002E10
    /* 3BB8 80002FB8 00000000 */   nop
    /* 3BBC 80002FBC 368F0002 */  ori        $t7, $s4, 0x2
    /* 3BC0 80002FC0 01E0A025 */  or         $s4, $t7, $zero
    /* 3BC4 80002FC4 1000FF90 */  b          .L80002E08
    /* 3BC8 80002FC8 00000000 */   nop
    /* 3BCC 80002FCC 36880004 */  ori        $t0, $s4, 0x4
    /* 3BD0 80002FD0 0100A025 */  or         $s4, $t0, $zero
    /* 3BD4 80002FD4 1000FF8C */  b          .L80002E08
    /* 3BD8 80002FD8 00000000 */   nop
    /* 3BDC 80002FDC 36990001 */  ori        $t9, $s4, 0x1
    /* 3BE0 80002FE0 0320A025 */  or         $s4, $t9, $zero
    /* 3BE4 80002FE4 1000FF88 */  b          .L80002E08
    /* 3BE8 80002FE8 00000000 */   nop
    /* 3BEC 80002FEC 8FB80230 */  lw         $t8, 0x230($sp)
    /* 3BF0 80002FF0 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3BF4 80002FF4 27B5007C */  addiu      $s5, $sp, 0x7C
    /* 3BF8 80002FF8 27090003 */  addiu      $t1, $t8, 0x3
    /* 3BFC 80002FFC 01216824 */  and        $t5, $t1, $at
    /* 3C00 80003000 25AC0004 */  addiu      $t4, $t5, 0x4
    /* 3C04 80003004 AFAC0230 */  sw         $t4, 0x230($sp)
    /* 3C08 80003008 8DAA0000 */  lw         $t2, 0x0($t5)
    /* 3C0C 8000300C 02A09825 */  or         $s3, $s5, $zero
    /* 3C10 80003010 A2AA0000 */  sb         $t2, 0x0($s5)
    /* 3C14 80003014 240B0001 */  addiu      $t3, $zero, 0x1
    /* 3C18 80003018 AFAB01DC */  sw         $t3, 0x1DC($sp)
    /* 3C1C 8000301C A3A00207 */  sb         $zero, 0x207($sp)
    /* 3C20 80003020 1000021E */  b          .L8000389C
    /* 3C24 80003024 00000000 */   nop
    /* 3C28 80003028 368E0001 */  ori        $t6, $s4, 0x1
    /* 3C2C 8000302C 01C0A025 */  or         $s4, $t6, $zero
    /* 3C30 80003030 328F0001 */  andi       $t7, $s4, 0x1
    /* 3C34 80003034 11E0000A */  beqz       $t7, .L80003060
    /* 3C38 80003038 00000000 */   nop
    /* 3C3C 8000303C 8FA80230 */  lw         $t0, 0x230($sp)
    /* 3C40 80003040 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3C44 80003044 25190003 */  addiu      $t9, $t0, 0x3
    /* 3C48 80003048 0321C024 */  and        $t8, $t9, $at
    /* 3C4C 8000304C 27090004 */  addiu      $t1, $t8, 0x4
    /* 3C50 80003050 AFA90230 */  sw         $t1, 0x230($sp)
    /* 3C54 80003054 8F0C0000 */  lw         $t4, 0x0($t8)
    /* 3C58 80003058 10000014 */  b          .L800030AC
    /* 3C5C 8000305C AFAC01F0 */   sw        $t4, 0x1F0($sp)
  .L80003060:
    /* 3C60 80003060 328D0004 */  andi       $t5, $s4, 0x4
    /* 3C64 80003064 11A00009 */  beqz       $t5, .L8000308C
    /* 3C68 80003068 00000000 */   nop
    /* 3C6C 8000306C 8FAA0230 */  lw         $t2, 0x230($sp)
    /* 3C70 80003070 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3C74 80003074 254B0003 */  addiu      $t3, $t2, 0x3
    /* 3C78 80003078 01617024 */  and        $t6, $t3, $at
    /* 3C7C 8000307C 25CF0004 */  addiu      $t7, $t6, 0x4
    /* 3C80 80003080 AFAF0230 */  sw         $t7, 0x230($sp)
    /* 3C84 80003084 10000008 */  b          .L800030A8
    /* 3C88 80003088 85D50002 */   lh        $s5, 0x2($t6)
  .L8000308C:
    /* 3C8C 8000308C 8FA80230 */  lw         $t0, 0x230($sp)
    /* 3C90 80003090 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3C94 80003094 25190003 */  addiu      $t9, $t0, 0x3
    /* 3C98 80003098 03214824 */  and        $t1, $t9, $at
    /* 3C9C 8000309C 25380004 */  addiu      $t8, $t1, 0x4
    /* 3CA0 800030A0 AFB80230 */  sw         $t8, 0x230($sp)
    /* 3CA4 800030A4 8D350000 */  lw         $s5, 0x0($t1)
  .L800030A8:
    /* 3CA8 800030A8 AFB501F0 */  sw         $s5, 0x1F0($sp)
  .L800030AC:
    /* 3CAC 800030AC 8FAC01F0 */  lw         $t4, 0x1F0($sp)
    /* 3CB0 800030B0 05810006 */  bgez       $t4, .L800030CC
    /* 3CB4 800030B4 00000000 */   nop
    /* 3CB8 800030B8 8FAD01F0 */  lw         $t5, 0x1F0($sp)
    /* 3CBC 800030BC 000D5023 */  negu       $t2, $t5
    /* 3CC0 800030C0 AFAA01F0 */  sw         $t2, 0x1F0($sp)
    /* 3CC4 800030C4 240B002D */  addiu      $t3, $zero, 0x2D
    /* 3CC8 800030C8 A3AB0207 */  sb         $t3, 0x207($sp)
  .L800030CC:
    /* 3CCC 800030CC 240F0001 */  addiu      $t7, $zero, 0x1
    /* 3CD0 800030D0 AFAF01EC */  sw         $t7, 0x1EC($sp)
    /* 3CD4 800030D4 1000016F */  b          .L80003694
    /* 3CD8 800030D8 00000000 */   nop
    /* 3CDC 800030DC 8FAE0230 */  lw         $t6, 0x230($sp)
    /* 3CE0 800030E0 31C80001 */  andi       $t0, $t6, 0x1
    /* 3CE4 800030E4 11000005 */  beqz       $t0, .L800030FC
    /* 3CE8 800030E8 00000000 */   nop
    /* 3CEC 800030EC 25D90007 */  addiu      $t9, $t6, 0x7
    /* 3CF0 800030F0 AFB90230 */  sw         $t9, 0x230($sp)
    /* 3CF4 800030F4 10000011 */  b          .L8000313C
    /* 3CF8 800030F8 2735FFEA */   addiu     $s5, $t9, -0x16
  .L800030FC:
    /* 3CFC 800030FC 8FB80230 */  lw         $t8, 0x230($sp)
    /* 3D00 80003100 33090002 */  andi       $t1, $t8, 0x2
    /* 3D04 80003104 11200005 */  beqz       $t1, .L8000311C
    /* 3D08 80003108 00000000 */   nop
    /* 3D0C 8000310C 270C000A */  addiu      $t4, $t8, 0xA
    /* 3D10 80003110 AFAC0230 */  sw         $t4, 0x230($sp)
    /* 3D14 80003114 10000008 */  b          .L80003138
    /* 3D18 80003118 2596FFD8 */   addiu     $s6, $t4, -0x28
  .L8000311C:
    /* 3D1C 8000311C 8FAD0230 */  lw         $t5, 0x230($sp)
    /* 3D20 80003120 2401FFF8 */  addiu      $at, $zero, -0x8
    /* 3D24 80003124 25AA0007 */  addiu      $t2, $t5, 0x7
    /* 3D28 80003128 01415824 */  and        $t3, $t2, $at
    /* 3D2C 8000312C 256F0008 */  addiu      $t7, $t3, 0x8
    /* 3D30 80003130 AFAF0230 */  sw         $t7, 0x230($sp)
    /* 3D34 80003134 01E0B025 */  or         $s6, $t7, $zero
  .L80003138:
    /* 3D38 80003138 02C0A825 */  or         $s5, $s6, $zero
  .L8000313C:
    /* 3D3C 8000313C D6A4FFF8 */  ldc1       $ft0, -0x8($s5)
    /* 3D40 80003140 F7A401F8 */  sdc1       $ft0, 0x1F8($sp)
    /* 3D44 80003144 0C000B18 */  jal        func_80002C60
    /* 3D48 80003148 D7AC01F8 */   ldc1      $fa0, 0x1F8($sp)
    /* 3D4C 8000314C 10400011 */  beqz       $v0, .L80003194
    /* 3D50 80003150 00000000 */   nop
    /* 3D54 80003154 D7A601F8 */  ldc1       $ft1, 0x1F8($sp)
    /* 3D58 80003158 44804800 */  mtc1       $zero, $ft2f
    /* 3D5C 8000315C 44804000 */  mtc1       $zero, $ft2
    /* 3D60 80003160 00000000 */  nop
    /* 3D64 80003164 4628303C */  c.lt.d     $ft1, $ft2
    /* 3D68 80003168 00000000 */  nop
    /* 3D6C 8000316C 45000003 */  bc1f       .L8000317C
    /* 3D70 80003170 00000000 */   nop
    /* 3D74 80003174 2408002D */  addiu      $t0, $zero, 0x2D
    /* 3D78 80003178 A3A80207 */  sb         $t0, 0x207($sp)
  .L8000317C:
    /* 3D7C 8000317C 3C138003 */  lui        $s3, %hi(D_8002D4F0)
    /* 3D80 80003180 2673D4F0 */  addiu      $s3, $s3, %lo(D_8002D4F0)
    /* 3D84 80003184 240E0003 */  addiu      $t6, $zero, 0x3
    /* 3D88 80003188 AFAE01DC */  sw         $t6, 0x1DC($sp)
    /* 3D8C 8000318C 100001C3 */  b          .L8000389C
    /* 3D90 80003190 00000000 */   nop
  .L80003194:
    /* 3D94 80003194 0C000AFC */  jal        func_80002BF0
    /* 3D98 80003198 D7AC01F8 */   ldc1      $fa0, 0x1F8($sp)
    /* 3D9C 8000319C 10400007 */  beqz       $v0, .L800031BC
    /* 3DA0 800031A0 00000000 */   nop
    /* 3DA4 800031A4 3C138003 */  lui        $s3, %hi(D_8002D4F4)
    /* 3DA8 800031A8 2673D4F4 */  addiu      $s3, $s3, %lo(D_8002D4F4)
    /* 3DAC 800031AC 24190003 */  addiu      $t9, $zero, 0x3
    /* 3DB0 800031B0 AFB901DC */  sw         $t9, 0x1DC($sp)
    /* 3DB4 800031B4 100001B9 */  b          .L8000389C
    /* 3DB8 800031B8 00000000 */   nop
  .L800031BC:
    /* 3DBC 800031BC 8FA90208 */  lw         $t1, 0x208($sp)
    /* 3DC0 800031C0 29210028 */  slti       $at, $t1, 0x28
    /* 3DC4 800031C4 14200010 */  bnez       $at, .L80003208
    /* 3DC8 800031C8 00000000 */   nop
    /* 3DCC 800031CC 24010067 */  addiu      $at, $zero, 0x67
    /* 3DD0 800031D0 12210004 */  beq        $s1, $at, .L800031E4
    /* 3DD4 800031D4 00000000 */   nop
    /* 3DD8 800031D8 24010047 */  addiu      $at, $zero, 0x47
    /* 3DDC 800031DC 16210004 */  bne        $s1, $at, .L800031F0
    /* 3DE0 800031E0 00000000 */   nop
  .L800031E4:
    /* 3DE4 800031E4 32980008 */  andi       $t8, $s4, 0x8
    /* 3DE8 800031E8 13000004 */  beqz       $t8, .L800031FC
    /* 3DEC 800031EC 00000000 */   nop
  .L800031F0:
    /* 3DF0 800031F0 8FAC0208 */  lw         $t4, 0x208($sp)
    /* 3DF4 800031F4 258DFFD9 */  addiu      $t5, $t4, -0x27
    /* 3DF8 800031F8 AFAD01F4 */  sw         $t5, 0x1F4($sp)
  .L800031FC:
    /* 3DFC 800031FC 240A0027 */  addiu      $t2, $zero, 0x27
    /* 3E00 80003200 10000007 */  b          .L80003220
    /* 3E04 80003204 AFAA0208 */   sw        $t2, 0x208($sp)
  .L80003208:
    /* 3E08 80003208 8FAB0208 */  lw         $t3, 0x208($sp)
    /* 3E0C 8000320C 2401FFFF */  addiu      $at, $zero, -0x1
    /* 3E10 80003210 15610003 */  bne        $t3, $at, .L80003220
    /* 3E14 80003214 00000000 */   nop
    /* 3E18 80003218 240F0006 */  addiu      $t7, $zero, 0x6
    /* 3E1C 8000321C AFAF0208 */  sw         $t7, 0x208($sp)
  .L80003220:
    /* 3E20 80003220 27B3007C */  addiu      $s3, $sp, 0x7C
    /* 3E24 80003224 A2600000 */  sb         $zero, 0x0($s3)
    /* 3E28 80003228 27AE007C */  addiu      $t6, $sp, 0x7C
    /* 3E2C 8000322C 25D9015C */  addiu      $t9, $t6, 0x15C
    /* 3E30 80003230 27A80206 */  addiu      $t0, $sp, 0x206
    /* 3E34 80003234 AFA80010 */  sw         $t0, 0x10($sp)
    /* 3E38 80003238 AFB9001C */  sw         $t9, 0x1C($sp)
    /* 3E3C 8000323C D7AC01F8 */  ldc1       $fa0, 0x1F8($sp)
    /* 3E40 80003240 8FA60208 */  lw         $a2, 0x208($sp)
    /* 3E44 80003244 02803825 */  or         $a3, $s4, $zero
    /* 3E48 80003248 AFB10014 */  sw         $s1, 0x14($sp)
    /* 3E4C 8000324C 0C000F0F */  jal        func_80003C3C
    /* 3E50 80003250 AFB30018 */   sw        $s3, 0x18($sp)
    /* 3E54 80003254 AFA201DC */  sw         $v0, 0x1DC($sp)
    /* 3E58 80003258 93A90206 */  lbu        $t1, 0x206($sp)
    /* 3E5C 8000325C 11200003 */  beqz       $t1, .L8000326C
    /* 3E60 80003260 00000000 */   nop
    /* 3E64 80003264 2418002D */  addiu      $t8, $zero, 0x2D
    /* 3E68 80003268 A3B80207 */  sb         $t8, 0x207($sp)
  .L8000326C:
    /* 3E6C 8000326C 926C0000 */  lbu        $t4, 0x0($s3)
    /* 3E70 80003270 15800002 */  bnez       $t4, .L8000327C
    /* 3E74 80003274 00000000 */   nop
    /* 3E78 80003278 26730001 */  addiu      $s3, $s3, 0x1
  .L8000327C:
    /* 3E7C 8000327C 10000187 */  b          .L8000389C
    /* 3E80 80003280 00000000 */   nop
    /* 3E84 80003284 328D0001 */  andi       $t5, $s4, 0x1
    /* 3E88 80003288 11A0000C */  beqz       $t5, .L800032BC
    /* 3E8C 8000328C 00000000 */   nop
    /* 3E90 80003290 8FAA0230 */  lw         $t2, 0x230($sp)
    /* 3E94 80003294 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3E98 80003298 8FAE0210 */  lw         $t6, 0x210($sp)
    /* 3E9C 8000329C 254B0003 */  addiu      $t3, $t2, 0x3
    /* 3EA0 800032A0 01617824 */  and        $t7, $t3, $at
    /* 3EA4 800032A4 25E80004 */  addiu      $t0, $t7, 0x4
    /* 3EA8 800032A8 AFA80230 */  sw         $t0, 0x230($sp)
    /* 3EAC 800032AC 0100A825 */  or         $s5, $t0, $zero
    /* 3EB0 800032B0 8EB9FFFC */  lw         $t9, -0x4($s5)
    /* 3EB4 800032B4 10000019 */  b          .L8000331C
    /* 3EB8 800032B8 AF2E0000 */   sw        $t6, 0x0($t9)
  .L800032BC:
    /* 3EBC 800032BC 32890004 */  andi       $t1, $s4, 0x4
    /* 3EC0 800032C0 1120000C */  beqz       $t1, .L800032F4
    /* 3EC4 800032C4 00000000 */   nop
    /* 3EC8 800032C8 8FB80230 */  lw         $t8, 0x230($sp)
    /* 3ECC 800032CC 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3ED0 800032D0 8FAB0210 */  lw         $t3, 0x210($sp)
    /* 3ED4 800032D4 270C0003 */  addiu      $t4, $t8, 0x3
    /* 3ED8 800032D8 01816824 */  and        $t5, $t4, $at
    /* 3EDC 800032DC 25AA0004 */  addiu      $t2, $t5, 0x4
    /* 3EE0 800032E0 AFAA0230 */  sw         $t2, 0x230($sp)
    /* 3EE4 800032E4 0140A825 */  or         $s5, $t2, $zero
    /* 3EE8 800032E8 8EAFFFFC */  lw         $t7, -0x4($s5)
    /* 3EEC 800032EC 1000000B */  b          .L8000331C
    /* 3EF0 800032F0 A5EB0000 */   sh        $t3, 0x0($t7)
  .L800032F4:
    /* 3EF4 800032F4 8FA80230 */  lw         $t0, 0x230($sp)
    /* 3EF8 800032F8 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3EFC 800032FC 8FB80210 */  lw         $t8, 0x210($sp)
    /* 3F00 80003300 250E0003 */  addiu      $t6, $t0, 0x3
    /* 3F04 80003304 01C1C824 */  and        $t9, $t6, $at
    /* 3F08 80003308 27290004 */  addiu      $t1, $t9, 0x4
    /* 3F0C 8000330C AFA90230 */  sw         $t1, 0x230($sp)
    /* 3F10 80003310 0120A825 */  or         $s5, $t1, $zero
    /* 3F14 80003314 8EACFFFC */  lw         $t4, -0x4($s5)
    /* 3F18 80003318 AD980000 */  sw         $t8, 0x0($t4)
  .L8000331C:
    /* 3F1C 8000331C 10000233 */  b          .L80003BEC
    /* 3F20 80003320 00000000 */   nop
    /* 3F24 80003324 368D0001 */  ori        $t5, $s4, 0x1
    /* 3F28 80003328 01A0A025 */  or         $s4, $t5, $zero
    /* 3F2C 8000332C 328A0001 */  andi       $t2, $s4, 0x1
    /* 3F30 80003330 1140000A */  beqz       $t2, .L8000335C
    /* 3F34 80003334 00000000 */   nop
    /* 3F38 80003338 8FAB0230 */  lw         $t3, 0x230($sp)
    /* 3F3C 8000333C 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3F40 80003340 256F0003 */  addiu      $t7, $t3, 0x3
    /* 3F44 80003344 01E14024 */  and        $t0, $t7, $at
    /* 3F48 80003348 250E0004 */  addiu      $t6, $t0, 0x4
    /* 3F4C 8000334C AFAE0230 */  sw         $t6, 0x230($sp)
    /* 3F50 80003350 8D190000 */  lw         $t9, 0x0($t0)
    /* 3F54 80003354 10000014 */  b          .L800033A8
    /* 3F58 80003358 AFB901F0 */   sw        $t9, 0x1F0($sp)
  .L8000335C:
    /* 3F5C 8000335C 32890004 */  andi       $t1, $s4, 0x4
    /* 3F60 80003360 11200009 */  beqz       $t1, .L80003388
    /* 3F64 80003364 00000000 */   nop
    /* 3F68 80003368 8FB80230 */  lw         $t8, 0x230($sp)
    /* 3F6C 8000336C 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3F70 80003370 270C0003 */  addiu      $t4, $t8, 0x3
    /* 3F74 80003374 01816824 */  and        $t5, $t4, $at
    /* 3F78 80003378 25AA0004 */  addiu      $t2, $t5, 0x4
    /* 3F7C 8000337C AFAA0230 */  sw         $t2, 0x230($sp)
    /* 3F80 80003380 10000008 */  b          .L800033A4
    /* 3F84 80003384 95B50002 */   lhu       $s5, 0x2($t5)
  .L80003388:
    /* 3F88 80003388 8FAB0230 */  lw         $t3, 0x230($sp)
    /* 3F8C 8000338C 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3F90 80003390 256F0003 */  addiu      $t7, $t3, 0x3
    /* 3F94 80003394 01E17024 */  and        $t6, $t7, $at
    /* 3F98 80003398 25C80004 */  addiu      $t0, $t6, 0x4
    /* 3F9C 8000339C AFA80230 */  sw         $t0, 0x230($sp)
    /* 3FA0 800033A0 8DD50000 */  lw         $s5, 0x0($t6)
  .L800033A4:
    /* 3FA4 800033A4 AFB501F0 */  sw         $s5, 0x1F0($sp)
  .L800033A8:
    /* 3FA8 800033A8 AFA001EC */  sw         $zero, 0x1EC($sp)
    /* 3FAC 800033AC 100000B8 */  b          .L80003690
    /* 3FB0 800033B0 00000000 */   nop
    /* 3FB4 800033B4 8FB90230 */  lw         $t9, 0x230($sp)
    /* 3FB8 800033B8 2401FFFC */  addiu      $at, $zero, -0x4
    /* 3FBC 800033BC 27290003 */  addiu      $t1, $t9, 0x3
    /* 3FC0 800033C0 0121C024 */  and        $t8, $t1, $at
    /* 3FC4 800033C4 270C0004 */  addiu      $t4, $t8, 0x4
    /* 3FC8 800033C8 AFAC0230 */  sw         $t4, 0x230($sp)
    /* 3FCC 800033CC 8F0A0000 */  lw         $t2, 0x0($t8)
    /* 3FD0 800033D0 AFAA01F0 */  sw         $t2, 0x1F0($sp)
    /* 3FD4 800033D4 240D0002 */  addiu      $t5, $zero, 0x2
    /* 3FD8 800033D8 AFAD01EC */  sw         $t5, 0x1EC($sp)
    /* 3FDC 800033DC 3C0B8003 */  lui        $t3, %hi(D_8002D4F8)
    /* 3FE0 800033E0 256BD4F8 */  addiu      $t3, $t3, %lo(D_8002D4F8)
    /* 3FE4 800033E4 AFAB01D8 */  sw         $t3, 0x1D8($sp)
    /* 3FE8 800033E8 368F0040 */  ori        $t7, $s4, 0x40
    /* 3FEC 800033EC 01E0A025 */  or         $s4, $t7, $zero
    /* 3FF0 800033F0 24110078 */  addiu      $s1, $zero, 0x78
    /* 3FF4 800033F4 100000A6 */  b          .L80003690
    /* 3FF8 800033F8 00000000 */   nop
    /* 3FFC 800033FC 8FA80230 */  lw         $t0, 0x230($sp)
    /* 4000 80003400 2401FFFC */  addiu      $at, $zero, -0x4
    /* 4004 80003404 250E0003 */  addiu      $t6, $t0, 0x3
    /* 4008 80003408 01C1C824 */  and        $t9, $t6, $at
    /* 400C 8000340C 27290004 */  addiu      $t1, $t9, 0x4
    /* 4010 80003410 AFA90230 */  sw         $t1, 0x230($sp)
    /* 4014 80003414 8F330000 */  lw         $s3, 0x0($t9)
    /* 4018 80003418 16600003 */  bnez       $s3, .L80003428
    /* 401C 8000341C 00000000 */   nop
    /* 4020 80003420 3C138003 */  lui        $s3, %hi(D_8002D50C)
    /* 4024 80003424 2673D50C */  addiu      $s3, $s3, %lo(D_8002D50C)
  .L80003428:
    /* 4028 80003428 8FAC0208 */  lw         $t4, 0x208($sp)
    /* 402C 8000342C 05800019 */  bltz       $t4, .L80003494
    /* 4030 80003430 00000000 */   nop
    /* 4034 80003434 02602025 */  or         $a0, $s3, $zero
    /* 4038 80003438 00002825 */  or         $a1, $zero, $zero
    /* 403C 8000343C 0C0009CC */  jal        func_80002730
    /* 4040 80003440 8FA60208 */   lw        $a2, 0x208($sp)
    /* 4044 80003444 AFA20050 */  sw         $v0, 0x50($sp)
    /* 4048 80003448 8FB80050 */  lw         $t8, 0x50($sp)
    /* 404C 8000344C 1300000D */  beqz       $t8, .L80003484
    /* 4050 80003450 00000000 */   nop
    /* 4054 80003454 8FAA0050 */  lw         $t2, 0x50($sp)
    /* 4058 80003458 01536823 */  subu       $t5, $t2, $s3
    /* 405C 8000345C AFAD01DC */  sw         $t5, 0x1DC($sp)
    /* 4060 80003460 8FAB01DC */  lw         $t3, 0x1DC($sp)
    /* 4064 80003464 8FAF0208 */  lw         $t7, 0x208($sp)
    /* 4068 80003468 01EB082A */  slt        $at, $t7, $t3
    /* 406C 8000346C 10200003 */  beqz       $at, .L8000347C
    /* 4070 80003470 00000000 */   nop
    /* 4074 80003474 8FA80208 */  lw         $t0, 0x208($sp)
    /* 4078 80003478 AFA801DC */  sw         $t0, 0x1DC($sp)
  .L8000347C:
    /* 407C 8000347C 10000003 */  b          .L8000348C
    /* 4080 80003480 00000000 */   nop
  .L80003484:
    /* 4084 80003484 8FAE0208 */  lw         $t6, 0x208($sp)
    /* 4088 80003488 AFAE01DC */  sw         $t6, 0x1DC($sp)
  .L8000348C:
    /* 408C 8000348C 10000004 */  b          .L800034A0
    /* 4090 80003490 00000000 */   nop
  .L80003494:
    /* 4094 80003494 0C001F10 */  jal        func_80007C40
    /* 4098 80003498 02602025 */   or        $a0, $s3, $zero
    /* 409C 8000349C AFA201DC */  sw         $v0, 0x1DC($sp)
  .L800034A0:
    /* 40A0 800034A0 A3A00207 */  sb         $zero, 0x207($sp)
    /* 40A4 800034A4 100000FD */  b          .L8000389C
    /* 40A8 800034A8 00000000 */   nop
    /* 40AC 800034AC 36890001 */  ori        $t1, $s4, 0x1
    /* 40B0 800034B0 0120A025 */  or         $s4, $t1, $zero
    /* 40B4 800034B4 32990001 */  andi       $t9, $s4, 0x1
    /* 40B8 800034B8 1320000A */  beqz       $t9, .L800034E4
    /* 40BC 800034BC 00000000 */   nop
    /* 40C0 800034C0 8FAC0230 */  lw         $t4, 0x230($sp)
    /* 40C4 800034C4 2401FFFC */  addiu      $at, $zero, -0x4
    /* 40C8 800034C8 25980003 */  addiu      $t8, $t4, 0x3
    /* 40CC 800034CC 03015024 */  and        $t2, $t8, $at
    /* 40D0 800034D0 254D0004 */  addiu      $t5, $t2, 0x4
    /* 40D4 800034D4 AFAD0230 */  sw         $t5, 0x230($sp)
    /* 40D8 800034D8 8D4B0000 */  lw         $t3, 0x0($t2)
    /* 40DC 800034DC 10000014 */  b          .L80003530
    /* 40E0 800034E0 AFAB01F0 */   sw        $t3, 0x1F0($sp)
  .L800034E4:
    /* 40E4 800034E4 328F0004 */  andi       $t7, $s4, 0x4
    /* 40E8 800034E8 11E00009 */  beqz       $t7, .L80003510
    /* 40EC 800034EC 00000000 */   nop
    /* 40F0 800034F0 8FA80230 */  lw         $t0, 0x230($sp)
    /* 40F4 800034F4 2401FFFC */  addiu      $at, $zero, -0x4
    /* 40F8 800034F8 250E0003 */  addiu      $t6, $t0, 0x3
    /* 40FC 800034FC 01C14824 */  and        $t1, $t6, $at
    /* 4100 80003500 25390004 */  addiu      $t9, $t1, 0x4
    /* 4104 80003504 AFB90230 */  sw         $t9, 0x230($sp)
    /* 4108 80003508 10000008 */  b          .L8000352C
    /* 410C 8000350C 95350002 */   lhu       $s5, 0x2($t1)
  .L80003510:
    /* 4110 80003510 8FAC0230 */  lw         $t4, 0x230($sp)
    /* 4114 80003514 2401FFFC */  addiu      $at, $zero, -0x4
    /* 4118 80003518 25980003 */  addiu      $t8, $t4, 0x3
    /* 411C 8000351C 03016824 */  and        $t5, $t8, $at
    /* 4120 80003520 25AA0004 */  addiu      $t2, $t5, 0x4
    /* 4124 80003524 AFAA0230 */  sw         $t2, 0x230($sp)
    /* 4128 80003528 8DB50000 */  lw         $s5, 0x0($t5)
  .L8000352C:
    /* 412C 8000352C AFB501F0 */  sw         $s5, 0x1F0($sp)
  .L80003530:
    /* 4130 80003530 240B0001 */  addiu      $t3, $zero, 0x1
    /* 4134 80003534 AFAB01EC */  sw         $t3, 0x1EC($sp)
    /* 4138 80003538 10000055 */  b          .L80003690
    /* 413C 8000353C 00000000 */   nop
    /* 4140 80003540 328F0001 */  andi       $t7, $s4, 0x1
    /* 4144 80003544 11E0000A */  beqz       $t7, .L80003570
    /* 4148 80003548 00000000 */   nop
    /* 414C 8000354C 8FA80230 */  lw         $t0, 0x230($sp)
    /* 4150 80003550 2401FFFC */  addiu      $at, $zero, -0x4
    /* 4154 80003554 250E0003 */  addiu      $t6, $t0, 0x3
    /* 4158 80003558 01C1C824 */  and        $t9, $t6, $at
    /* 415C 8000355C 27290004 */  addiu      $t1, $t9, 0x4
    /* 4160 80003560 AFA90230 */  sw         $t1, 0x230($sp)
    /* 4164 80003564 8F2C0000 */  lw         $t4, 0x0($t9)
    /* 4168 80003568 10000014 */  b          .L800035BC
    /* 416C 8000356C AFAC01F0 */   sw        $t4, 0x1F0($sp)
  .L80003570:
    /* 4170 80003570 32980004 */  andi       $t8, $s4, 0x4
    /* 4174 80003574 13000009 */  beqz       $t8, .L8000359C
    /* 4178 80003578 00000000 */   nop
    /* 417C 8000357C 8FAA0230 */  lw         $t2, 0x230($sp)
    /* 4180 80003580 2401FFFC */  addiu      $at, $zero, -0x4
    /* 4184 80003584 254D0003 */  addiu      $t5, $t2, 0x3
    /* 4188 80003588 01A15824 */  and        $t3, $t5, $at
    /* 418C 8000358C 256F0004 */  addiu      $t7, $t3, 0x4
    /* 4190 80003590 AFAF0230 */  sw         $t7, 0x230($sp)
    /* 4194 80003594 10000008 */  b          .L800035B8
    /* 4198 80003598 95750002 */   lhu       $s5, 0x2($t3)
  .L8000359C:
    /* 419C 8000359C 8FA80230 */  lw         $t0, 0x230($sp)
    /* 41A0 800035A0 2401FFFC */  addiu      $at, $zero, -0x4
    /* 41A4 800035A4 250E0003 */  addiu      $t6, $t0, 0x3
    /* 41A8 800035A8 01C14824 */  and        $t1, $t6, $at
    /* 41AC 800035AC 25390004 */  addiu      $t9, $t1, 0x4
    /* 41B0 800035B0 AFB90230 */  sw         $t9, 0x230($sp)
    /* 41B4 800035B4 8D350000 */  lw         $s5, 0x0($t1)
  .L800035B8:
    /* 41B8 800035B8 AFB501F0 */  sw         $s5, 0x1F0($sp)
  .L800035BC:
    /* 41BC 800035BC 240C0003 */  addiu      $t4, $zero, 0x3
    /* 41C0 800035C0 AFAC01EC */  sw         $t4, 0x1EC($sp)
    /* 41C4 800035C4 10000032 */  b          .L80003690
    /* 41C8 800035C8 00000000 */   nop
    /* 41CC 800035CC 3C188003 */  lui        $t8, %hi(D_8002D514)
    /* 41D0 800035D0 2718D514 */  addiu      $t8, $t8, %lo(D_8002D514)
    /* 41D4 800035D4 AFB801D8 */  sw         $t8, 0x1D8($sp)
    /* 41D8 800035D8 10000004 */  b          .L800035EC
    /* 41DC 800035DC 00000000 */   nop
    /* 41E0 800035E0 3C0A8003 */  lui        $t2, %hi(D_8002D528)
    /* 41E4 800035E4 254AD528 */  addiu      $t2, $t2, %lo(D_8002D528)
    /* 41E8 800035E8 AFAA01D8 */  sw         $t2, 0x1D8($sp)
  .L800035EC:
    /* 41EC 800035EC 328D0001 */  andi       $t5, $s4, 0x1
    /* 41F0 800035F0 11A0000A */  beqz       $t5, .L8000361C
    /* 41F4 800035F4 00000000 */   nop
    /* 41F8 800035F8 8FAF0230 */  lw         $t7, 0x230($sp)
    /* 41FC 800035FC 2401FFFC */  addiu      $at, $zero, -0x4
    /* 4200 80003600 25EB0003 */  addiu      $t3, $t7, 0x3
    /* 4204 80003604 01614024 */  and        $t0, $t3, $at
    /* 4208 80003608 250E0004 */  addiu      $t6, $t0, 0x4
    /* 420C 8000360C AFAE0230 */  sw         $t6, 0x230($sp)
    /* 4210 80003610 8D190000 */  lw         $t9, 0x0($t0)
    /* 4214 80003614 10000014 */  b          .L80003668
    /* 4218 80003618 AFB901F0 */   sw        $t9, 0x1F0($sp)
  .L8000361C:
    /* 421C 8000361C 32890004 */  andi       $t1, $s4, 0x4
    /* 4220 80003620 11200009 */  beqz       $t1, .L80003648
    /* 4224 80003624 00000000 */   nop
    /* 4228 80003628 8FAC0230 */  lw         $t4, 0x230($sp)
    /* 422C 8000362C 2401FFFC */  addiu      $at, $zero, -0x4
    /* 4230 80003630 25980003 */  addiu      $t8, $t4, 0x3
    /* 4234 80003634 03015024 */  and        $t2, $t8, $at
    /* 4238 80003638 254D0004 */  addiu      $t5, $t2, 0x4
    /* 423C 8000363C AFAD0230 */  sw         $t5, 0x230($sp)
    /* 4240 80003640 10000008 */  b          .L80003664
    /* 4244 80003644 95550002 */   lhu       $s5, 0x2($t2)
  .L80003648:
    /* 4248 80003648 8FAF0230 */  lw         $t7, 0x230($sp)
    /* 424C 8000364C 2401FFFC */  addiu      $at, $zero, -0x4
    /* 4250 80003650 25EB0003 */  addiu      $t3, $t7, 0x3
    /* 4254 80003654 01617024 */  and        $t6, $t3, $at
    /* 4258 80003658 25C80004 */  addiu      $t0, $t6, 0x4
    /* 425C 8000365C AFA80230 */  sw         $t0, 0x230($sp)
    /* 4260 80003660 8DD50000 */  lw         $s5, 0x0($t6)
  .L80003664:
    /* 4264 80003664 AFB501F0 */  sw         $s5, 0x1F0($sp)
  .L80003668:
    /* 4268 80003668 24190002 */  addiu      $t9, $zero, 0x2
    /* 426C 8000366C AFB901EC */  sw         $t9, 0x1EC($sp)
    /* 4270 80003670 32890008 */  andi       $t1, $s4, 0x8
    /* 4274 80003674 11200006 */  beqz       $t1, .L80003690
    /* 4278 80003678 00000000 */   nop
    /* 427C 8000367C 8FAC01F0 */  lw         $t4, 0x1F0($sp)
    /* 4280 80003680 11800003 */  beqz       $t4, .L80003690
    /* 4284 80003684 00000000 */   nop
    /* 4288 80003688 36980040 */  ori        $t8, $s4, 0x40
    /* 428C 8000368C 0300A025 */  or         $s4, $t8, $zero
  .L80003690:
    /* 4290 80003690 A3A00207 */  sb         $zero, 0x207($sp)
  .L80003694:
    /* 4294 80003694 8FAD0208 */  lw         $t5, 0x208($sp)
    /* 4298 80003698 05A00004 */  bltz       $t5, .L800036AC
    /* 429C 8000369C AFAD01E8 */   sw        $t5, 0x1E8($sp)
    /* 42A0 800036A0 2401FFDF */  addiu      $at, $zero, -0x21
    /* 42A4 800036A4 02815024 */  and        $t2, $s4, $at
    /* 42A8 800036A8 0140A025 */  or         $s4, $t2, $zero
  .L800036AC:
    /* 42AC 800036AC 27B3007C */  addiu      $s3, $sp, 0x7C
    /* 42B0 800036B0 2673015C */  addiu      $s3, $s3, 0x15C
    /* 42B4 800036B4 8FAF01F0 */  lw         $t7, 0x1F0($sp)
    /* 42B8 800036B8 15E00004 */  bnez       $t7, .L800036CC
    /* 42BC 800036BC 00000000 */   nop
    /* 42C0 800036C0 8FAB0208 */  lw         $t3, 0x208($sp)
    /* 42C4 800036C4 11600064 */  beqz       $t3, .L80003858
    /* 42C8 800036C8 00000000 */   nop
  .L800036CC:
    /* 42CC 800036CC 8FB501EC */  lw         $s5, 0x1EC($sp)
    /* 42D0 800036D0 12A0000C */  beqz       $s5, .L80003704
    /* 42D4 800036D4 00000000 */   nop
    /* 42D8 800036D8 24010001 */  addiu      $at, $zero, 0x1
    /* 42DC 800036DC 12A10020 */  beq        $s5, $at, .L80003760
    /* 42E0 800036E0 00000000 */   nop
    /* 42E4 800036E4 24010002 */  addiu      $at, $zero, 0x2
    /* 42E8 800036E8 12A10038 */  beq        $s5, $at, .L800037CC
    /* 42EC 800036EC 00000000 */   nop
    /* 42F0 800036F0 24010003 */  addiu      $at, $zero, 0x3
    /* 42F4 800036F4 12A10044 */  beq        $s5, $at, .L80003808
    /* 42F8 800036F8 00000000 */   nop
    /* 42FC 800036FC 1000004F */  b          .L8000383C
    /* 4300 80003700 00000000 */   nop
  .L80003704:
    /* 4304 80003704 8FA801F0 */  lw         $t0, 0x1F0($sp)
    /* 4308 80003708 2673FFFF */  addiu      $s3, $s3, -0x1
    /* 430C 8000370C 310E0007 */  andi       $t6, $t0, 0x7
    /* 4310 80003710 25D90030 */  addiu      $t9, $t6, 0x30
    /* 4314 80003714 A2790000 */  sb         $t9, 0x0($s3)
    /* 4318 80003718 8FA901F0 */  lw         $t1, 0x1F0($sp)
    /* 431C 8000371C 000960C2 */  srl        $t4, $t1, 3
    /* 4320 80003720 AFAC01F0 */  sw         $t4, 0x1F0($sp)
    /* 4324 80003724 8FB801F0 */  lw         $t8, 0x1F0($sp)
    /* 4328 80003728 1700FFF6 */  bnez       $t8, .L80003704
    /* 432C 8000372C 00000000 */   nop
    /* 4330 80003730 328D0008 */  andi       $t5, $s4, 0x8
    /* 4334 80003734 11A00008 */  beqz       $t5, .L80003758
    /* 4338 80003738 00000000 */   nop
    /* 433C 8000373C 926A0000 */  lbu        $t2, 0x0($s3)
    /* 4340 80003740 24010030 */  addiu      $at, $zero, 0x30
    /* 4344 80003744 11410004 */  beq        $t2, $at, .L80003758
    /* 4348 80003748 00000000 */   nop
    /* 434C 8000374C 240F0030 */  addiu      $t7, $zero, 0x30
    /* 4350 80003750 A26FFFFF */  sb         $t7, -0x1($s3)
    /* 4354 80003754 2673FFFF */  addiu      $s3, $s3, -0x1
  .L80003758:
    /* 4358 80003758 1000003F */  b          .L80003858
    /* 435C 8000375C 00000000 */   nop
  .L80003760:
    /* 4360 80003760 8FAB01F0 */  lw         $t3, 0x1F0($sp)
    /* 4364 80003764 2D61000A */  sltiu      $at, $t3, 0xA
    /* 4368 80003768 14200012 */  bnez       $at, .L800037B4
    /* 436C 8000376C 00000000 */   nop
  .L80003770:
    /* 4370 80003770 8FA801F0 */  lw         $t0, 0x1F0($sp)
    /* 4374 80003774 2401000A */  addiu      $at, $zero, 0xA
    /* 4378 80003778 2673FFFF */  addiu      $s3, $s3, -0x1
    /* 437C 8000377C 0101001B */  divu       $zero, $t0, $at
    /* 4380 80003780 00007010 */  mfhi       $t6
    /* 4384 80003784 25D90030 */  addiu      $t9, $t6, 0x30
    /* 4388 80003788 A2790000 */  sb         $t9, 0x0($s3)
    /* 438C 8000378C 8FA901F0 */  lw         $t1, 0x1F0($sp)
    /* 4390 80003790 2401000A */  addiu      $at, $zero, 0xA
    /* 4394 80003794 0121001B */  divu       $zero, $t1, $at
    /* 4398 80003798 00006012 */  mflo       $t4
    /* 439C 8000379C AFAC01F0 */  sw         $t4, 0x1F0($sp)
    /* 43A0 800037A0 00000000 */  nop
    /* 43A4 800037A4 8FB801F0 */  lw         $t8, 0x1F0($sp)
    /* 43A8 800037A8 2F01000A */  sltiu      $at, $t8, 0xA
    /* 43AC 800037AC 1020FFF0 */  beqz       $at, .L80003770
    /* 43B0 800037B0 00000000 */   nop
  .L800037B4:
    /* 43B4 800037B4 8FAD01F0 */  lw         $t5, 0x1F0($sp)
    /* 43B8 800037B8 2673FFFF */  addiu      $s3, $s3, -0x1
    /* 43BC 800037BC 25AA0030 */  addiu      $t2, $t5, 0x30
    /* 43C0 800037C0 A26A0000 */  sb         $t2, 0x0($s3)
    /* 43C4 800037C4 10000024 */  b          .L80003858
    /* 43C8 800037C8 00000000 */   nop
  .L800037CC:
    /* 43CC 800037CC 8FAF01F0 */  lw         $t7, 0x1F0($sp)
    /* 43D0 800037D0 8FA801D8 */  lw         $t0, 0x1D8($sp)
    /* 43D4 800037D4 2673FFFF */  addiu      $s3, $s3, -0x1
    /* 43D8 800037D8 31EB000F */  andi       $t3, $t7, 0xF
    /* 43DC 800037DC 01687021 */  addu       $t6, $t3, $t0
    /* 43E0 800037E0 91D90000 */  lbu        $t9, 0x0($t6)
    /* 43E4 800037E4 A2790000 */  sb         $t9, 0x0($s3)
    /* 43E8 800037E8 8FA901F0 */  lw         $t1, 0x1F0($sp)
    /* 43EC 800037EC 00096102 */  srl        $t4, $t1, 4
    /* 43F0 800037F0 AFAC01F0 */  sw         $t4, 0x1F0($sp)
    /* 43F4 800037F4 8FB801F0 */  lw         $t8, 0x1F0($sp)
    /* 43F8 800037F8 1700FFF4 */  bnez       $t8, .L800037CC
    /* 43FC 800037FC 00000000 */   nop
    /* 4400 80003800 10000015 */  b          .L80003858
    /* 4404 80003804 00000000 */   nop
  .L80003808:
    /* 4408 80003808 8FAD01F0 */  lw         $t5, 0x1F0($sp)
    /* 440C 8000380C 2673FFFF */  addiu      $s3, $s3, -0x1
    /* 4410 80003810 31AA0001 */  andi       $t2, $t5, 0x1
    /* 4414 80003814 254F0030 */  addiu      $t7, $t2, 0x30
    /* 4418 80003818 A26F0000 */  sb         $t7, 0x0($s3)
    /* 441C 8000381C 8FAB01F0 */  lw         $t3, 0x1F0($sp)
    /* 4420 80003820 000B4042 */  srl        $t0, $t3, 1
    /* 4424 80003824 AFA801F0 */  sw         $t0, 0x1F0($sp)
    /* 4428 80003828 8FAE01F0 */  lw         $t6, 0x1F0($sp)
    /* 442C 8000382C 15C0FFF6 */  bnez       $t6, .L80003808
    /* 4430 80003830 00000000 */   nop
    /* 4434 80003834 10000008 */  b          .L80003858
    /* 4438 80003838 00000000 */   nop
  .L8000383C:
    /* 443C 8000383C 3C138003 */  lui        $s3, %hi(D_8002D53C)
    /* 4440 80003840 2673D53C */  addiu      $s3, $s3, %lo(D_8002D53C)
    /* 4444 80003844 0C001F10 */  jal        func_80007C40
    /* 4448 80003848 02602025 */   or        $a0, $s3, $zero
    /* 444C 8000384C AFA201DC */  sw         $v0, 0x1DC($sp)
    /* 4450 80003850 10000005 */  b          .L80003868
    /* 4454 80003854 00000000 */   nop
  .L80003858:
    /* 4458 80003858 27B9007C */  addiu      $t9, $sp, 0x7C
    /* 445C 8000385C 03334823 */  subu       $t1, $t9, $s3
    /* 4460 80003860 252C015C */  addiu      $t4, $t1, 0x15C
    /* 4464 80003864 AFAC01DC */  sw         $t4, 0x1DC($sp)
  .L80003868:
    /* 4468 80003868 1000000C */  b          .L8000389C
    /* 446C 8000386C 00000000 */   nop
  .L80003870:
    /* 4470 80003870 16200003 */  bnez       $s1, .L80003880
    /* 4474 80003874 00000000 */   nop
    /* 4478 80003878 100000DE */  b          .L80003BF4
    /* 447C 8000387C 00000000 */   nop
  .L80003880:
    /* 4480 80003880 27B3007C */  addiu      $s3, $sp, 0x7C
    /* 4484 80003884 A2710000 */  sb         $s1, 0x0($s3)
    /* 4488 80003888 24180001 */  addiu      $t8, $zero, 0x1
    /* 448C 8000388C AFB801DC */  sw         $t8, 0x1DC($sp)
    /* 4490 80003890 A3A00207 */  sb         $zero, 0x207($sp)
    /* 4494 80003894 10000001 */  b          .L8000389C
    /* 4498 80003898 00000000 */   nop
  .L8000389C:
    /* 449C 8000389C 8FAD01DC */  lw         $t5, 0x1DC($sp)
    /* 44A0 800038A0 8FAA01F4 */  lw         $t2, 0x1F4($sp)
    /* 44A4 800038A4 01AA7821 */  addu       $t7, $t5, $t2
    /* 44A8 800038A8 AFAF01E4 */  sw         $t7, 0x1E4($sp)
    /* 44AC 800038AC 93AB0207 */  lbu        $t3, 0x207($sp)
    /* 44B0 800038B0 11600005 */  beqz       $t3, .L800038C8
    /* 44B4 800038B4 00000000 */   nop
    /* 44B8 800038B8 8FA801E4 */  lw         $t0, 0x1E4($sp)
    /* 44BC 800038BC 250E0001 */  addiu      $t6, $t0, 0x1
    /* 44C0 800038C0 10000007 */  b          .L800038E0
    /* 44C4 800038C4 AFAE01E4 */   sw        $t6, 0x1E4($sp)
  .L800038C8:
    /* 44C8 800038C8 32990040 */  andi       $t9, $s4, 0x40
    /* 44CC 800038CC 13200004 */  beqz       $t9, .L800038E0
    /* 44D0 800038D0 00000000 */   nop
    /* 44D4 800038D4 8FA901E4 */  lw         $t1, 0x1E4($sp)
    /* 44D8 800038D8 252C0002 */  addiu      $t4, $t1, 0x2
    /* 44DC 800038DC AFAC01E4 */  sw         $t4, 0x1E4($sp)
  .L800038E0:
    /* 44E0 800038E0 8FB801E8 */  lw         $t8, 0x1E8($sp)
    /* 44E4 800038E4 8FAD01E4 */  lw         $t5, 0x1E4($sp)
    /* 44E8 800038E8 01B8082A */  slt        $at, $t5, $t8
    /* 44EC 800038EC 10200003 */  beqz       $at, .L800038FC
    /* 44F0 800038F0 00000000 */   nop
    /* 44F4 800038F4 10000003 */  b          .L80003904
    /* 44F8 800038F8 AFB801E0 */   sw        $t8, 0x1E0($sp)
  .L800038FC:
    /* 44FC 800038FC 8FAA01E4 */  lw         $t2, 0x1E4($sp)
    /* 4500 80003900 AFAA01E0 */  sw         $t2, 0x1E0($sp)
  .L80003904:
    /* 4504 80003904 328F0030 */  andi       $t7, $s4, 0x30
    /* 4508 80003908 15E0001B */  bnez       $t7, .L80003978
    /* 450C 8000390C 00000000 */   nop
    /* 4510 80003910 8FAB020C */  lw         $t3, 0x20C($sp)
    /* 4514 80003914 8FA801E0 */  lw         $t0, 0x1E0($sp)
    /* 4518 80003918 01689023 */  subu       $s2, $t3, $t0
    /* 451C 8000391C 1A400016 */  blez       $s2, .L80003978
    /* 4520 80003920 00000000 */   nop
    /* 4524 80003924 2A410011 */  slti       $at, $s2, 0x11
    /* 4528 80003928 1420000C */  bnez       $at, .L8000395C
    /* 452C 8000392C 00000000 */   nop
  .L80003930:
    /* 4530 80003930 8FA40228 */  lw         $a0, 0x228($sp)
    /* 4534 80003934 27A50064 */  addiu      $a1, $sp, 0x64
    /* 4538 80003938 0C001F1A */  jal        func_80007C68
    /* 453C 8000393C 24060010 */   addiu     $a2, $zero, 0x10
    /* 4540 80003940 8FAE0228 */  lw         $t6, 0x228($sp)
    /* 4544 80003944 25D90010 */  addiu      $t9, $t6, 0x10
    /* 4548 80003948 AFB90228 */  sw         $t9, 0x228($sp)
    /* 454C 8000394C 2652FFF0 */  addiu      $s2, $s2, -0x10
    /* 4550 80003950 2A410011 */  slti       $at, $s2, 0x11
    /* 4554 80003954 1020FFF6 */  beqz       $at, .L80003930
    /* 4558 80003958 00000000 */   nop
  .L8000395C:
    /* 455C 8000395C 8FA40228 */  lw         $a0, 0x228($sp)
    /* 4560 80003960 27A50064 */  addiu      $a1, $sp, 0x64
    /* 4564 80003964 0C001F1A */  jal        func_80007C68
    /* 4568 80003968 02403025 */   or        $a2, $s2, $zero
    /* 456C 8000396C 8FA90228 */  lw         $t1, 0x228($sp)
    /* 4570 80003970 01326021 */  addu       $t4, $t1, $s2
    /* 4574 80003974 AFAC0228 */  sw         $t4, 0x228($sp)
  .L80003978:
    /* 4578 80003978 93AD0207 */  lbu        $t5, 0x207($sp)
    /* 457C 8000397C 11A0000A */  beqz       $t5, .L800039A8
    /* 4580 80003980 00000000 */   nop
    /* 4584 80003984 8FA40228 */  lw         $a0, 0x228($sp)
    /* 4588 80003988 27A50207 */  addiu      $a1, $sp, 0x207
    /* 458C 8000398C 0C001F1A */  jal        func_80007C68
    /* 4590 80003990 24060001 */   addiu     $a2, $zero, 0x1
    /* 4594 80003994 8FB80228 */  lw         $t8, 0x228($sp)
    /* 4598 80003998 270A0001 */  addiu      $t2, $t8, 0x1
    /* 459C 8000399C AFAA0228 */  sw         $t2, 0x228($sp)
    /* 45A0 800039A0 10000010 */  b          .L800039E4
    /* 45A4 800039A4 00000000 */   nop
  .L800039A8:
    /* 45A8 800039A8 328F0040 */  andi       $t7, $s4, 0x40
    /* 45AC 800039AC 11E0000D */  beqz       $t7, .L800039E4
    /* 45B0 800039B0 00000000 */   nop
    /* 45B4 800039B4 240B0030 */  addiu      $t3, $zero, 0x30
    /* 45B8 800039B8 27A80078 */  addiu      $t0, $sp, 0x78
    /* 45BC 800039BC A10B0000 */  sb         $t3, 0x0($t0)
    /* 45C0 800039C0 27AE0078 */  addiu      $t6, $sp, 0x78
    /* 45C4 800039C4 A1D10001 */  sb         $s1, 0x1($t6)
    /* 45C8 800039C8 8FA40228 */  lw         $a0, 0x228($sp)
    /* 45CC 800039CC 27A50078 */  addiu      $a1, $sp, 0x78
    /* 45D0 800039D0 0C001F1A */  jal        func_80007C68
    /* 45D4 800039D4 24060002 */   addiu     $a2, $zero, 0x2
    /* 45D8 800039D8 8FB90228 */  lw         $t9, 0x228($sp)
    /* 45DC 800039DC 27290002 */  addiu      $t1, $t9, 0x2
    /* 45E0 800039E0 AFA90228 */  sw         $t1, 0x228($sp)
  .L800039E4:
    /* 45E4 800039E4 328C0030 */  andi       $t4, $s4, 0x30
    /* 45E8 800039E8 24010020 */  addiu      $at, $zero, 0x20
    /* 45EC 800039EC 1581001B */  bne        $t4, $at, .L80003A5C
    /* 45F0 800039F0 00000000 */   nop
    /* 45F4 800039F4 8FAD020C */  lw         $t5, 0x20C($sp)
    /* 45F8 800039F8 8FB801E0 */  lw         $t8, 0x1E0($sp)
    /* 45FC 800039FC 01B89023 */  subu       $s2, $t5, $t8
    /* 4600 80003A00 1A400016 */  blez       $s2, .L80003A5C
    /* 4604 80003A04 00000000 */   nop
    /* 4608 80003A08 2A410011 */  slti       $at, $s2, 0x11
    /* 460C 80003A0C 1420000C */  bnez       $at, .L80003A40
    /* 4610 80003A10 00000000 */   nop
  .L80003A14:
    /* 4614 80003A14 8FA40228 */  lw         $a0, 0x228($sp)
    /* 4618 80003A18 27A50054 */  addiu      $a1, $sp, 0x54
    /* 461C 80003A1C 0C001F1A */  jal        func_80007C68
    /* 4620 80003A20 24060010 */   addiu     $a2, $zero, 0x10
    /* 4624 80003A24 8FAA0228 */  lw         $t2, 0x228($sp)
    /* 4628 80003A28 254F0010 */  addiu      $t7, $t2, 0x10
    /* 462C 80003A2C AFAF0228 */  sw         $t7, 0x228($sp)
    /* 4630 80003A30 2652FFF0 */  addiu      $s2, $s2, -0x10
    /* 4634 80003A34 2A410011 */  slti       $at, $s2, 0x11
    /* 4638 80003A38 1020FFF6 */  beqz       $at, .L80003A14
    /* 463C 80003A3C 00000000 */   nop
  .L80003A40:
    /* 4640 80003A40 8FA40228 */  lw         $a0, 0x228($sp)
    /* 4644 80003A44 27A50054 */  addiu      $a1, $sp, 0x54
    /* 4648 80003A48 0C001F1A */  jal        func_80007C68
    /* 464C 80003A4C 02403025 */   or        $a2, $s2, $zero
    /* 4650 80003A50 8FAB0228 */  lw         $t3, 0x228($sp)
    /* 4654 80003A54 01724021 */  addu       $t0, $t3, $s2
    /* 4658 80003A58 AFA80228 */  sw         $t0, 0x228($sp)
  .L80003A5C:
    /* 465C 80003A5C 8FAE01E8 */  lw         $t6, 0x1E8($sp)
    /* 4660 80003A60 8FB901E4 */  lw         $t9, 0x1E4($sp)
    /* 4664 80003A64 01D99023 */  subu       $s2, $t6, $t9
    /* 4668 80003A68 1A400016 */  blez       $s2, .L80003AC4
    /* 466C 80003A6C 00000000 */   nop
    /* 4670 80003A70 2A410011 */  slti       $at, $s2, 0x11
    /* 4674 80003A74 1420000C */  bnez       $at, .L80003AA8
    /* 4678 80003A78 00000000 */   nop
  .L80003A7C:
    /* 467C 80003A7C 8FA40228 */  lw         $a0, 0x228($sp)
    /* 4680 80003A80 27A50054 */  addiu      $a1, $sp, 0x54
    /* 4684 80003A84 0C001F1A */  jal        func_80007C68
    /* 4688 80003A88 24060010 */   addiu     $a2, $zero, 0x10
    /* 468C 80003A8C 8FA90228 */  lw         $t1, 0x228($sp)
    /* 4690 80003A90 252C0010 */  addiu      $t4, $t1, 0x10
    /* 4694 80003A94 AFAC0228 */  sw         $t4, 0x228($sp)
    /* 4698 80003A98 2652FFF0 */  addiu      $s2, $s2, -0x10
    /* 469C 80003A9C 2A410011 */  slti       $at, $s2, 0x11
    /* 46A0 80003AA0 1020FFF6 */  beqz       $at, .L80003A7C
    /* 46A4 80003AA4 00000000 */   nop
  .L80003AA8:
    /* 46A8 80003AA8 8FA40228 */  lw         $a0, 0x228($sp)
    /* 46AC 80003AAC 27A50054 */  addiu      $a1, $sp, 0x54
    /* 46B0 80003AB0 0C001F1A */  jal        func_80007C68
    /* 46B4 80003AB4 02403025 */   or        $a2, $s2, $zero
    /* 46B8 80003AB8 8FAD0228 */  lw         $t5, 0x228($sp)
    /* 46BC 80003ABC 01B2C021 */  addu       $t8, $t5, $s2
    /* 46C0 80003AC0 AFB80228 */  sw         $t8, 0x228($sp)
  .L80003AC4:
    /* 46C4 80003AC4 8FA40228 */  lw         $a0, 0x228($sp)
    /* 46C8 80003AC8 02602825 */  or         $a1, $s3, $zero
    /* 46CC 80003ACC 0C001F1A */  jal        func_80007C68
    /* 46D0 80003AD0 8FA601DC */   lw        $a2, 0x1DC($sp)
    /* 46D4 80003AD4 8FAA0228 */  lw         $t2, 0x228($sp)
    /* 46D8 80003AD8 8FAF01DC */  lw         $t7, 0x1DC($sp)
    /* 46DC 80003ADC 014F5821 */  addu       $t3, $t2, $t7
    /* 46E0 80003AE0 AFAB0228 */  sw         $t3, 0x228($sp)
    /* 46E4 80003AE4 8FB201F4 */  lw         $s2, 0x1F4($sp)
    /* 46E8 80003AE8 1A400016 */  blez       $s2, .L80003B44
    /* 46EC 80003AEC 00000000 */   nop
    /* 46F0 80003AF0 2A410011 */  slti       $at, $s2, 0x11
    /* 46F4 80003AF4 1420000C */  bnez       $at, .L80003B28
    /* 46F8 80003AF8 00000000 */   nop
  .L80003AFC:
    /* 46FC 80003AFC 8FA40228 */  lw         $a0, 0x228($sp)
    /* 4700 80003B00 27A50054 */  addiu      $a1, $sp, 0x54
    /* 4704 80003B04 0C001F1A */  jal        func_80007C68
    /* 4708 80003B08 24060010 */   addiu     $a2, $zero, 0x10
    /* 470C 80003B0C 8FA80228 */  lw         $t0, 0x228($sp)
    /* 4710 80003B10 250E0010 */  addiu      $t6, $t0, 0x10
    /* 4714 80003B14 AFAE0228 */  sw         $t6, 0x228($sp)
    /* 4718 80003B18 2652FFF0 */  addiu      $s2, $s2, -0x10
    /* 471C 80003B1C 2A410011 */  slti       $at, $s2, 0x11
    /* 4720 80003B20 1020FFF6 */  beqz       $at, .L80003AFC
    /* 4724 80003B24 00000000 */   nop
  .L80003B28:
    /* 4728 80003B28 8FA40228 */  lw         $a0, 0x228($sp)
    /* 472C 80003B2C 27A50054 */  addiu      $a1, $sp, 0x54
    /* 4730 80003B30 0C001F1A */  jal        func_80007C68
    /* 4734 80003B34 02403025 */   or        $a2, $s2, $zero
    /* 4738 80003B38 8FB90228 */  lw         $t9, 0x228($sp)
    /* 473C 80003B3C 03324821 */  addu       $t1, $t9, $s2
    /* 4740 80003B40 AFA90228 */  sw         $t1, 0x228($sp)
  .L80003B44:
    /* 4744 80003B44 328C0010 */  andi       $t4, $s4, 0x10
    /* 4748 80003B48 1180001B */  beqz       $t4, .L80003BB8
    /* 474C 80003B4C 00000000 */   nop
    /* 4750 80003B50 8FAD020C */  lw         $t5, 0x20C($sp)
    /* 4754 80003B54 8FB801E0 */  lw         $t8, 0x1E0($sp)
    /* 4758 80003B58 01B89023 */  subu       $s2, $t5, $t8
    /* 475C 80003B5C 1A400016 */  blez       $s2, .L80003BB8
    /* 4760 80003B60 00000000 */   nop
    /* 4764 80003B64 2A410011 */  slti       $at, $s2, 0x11
    /* 4768 80003B68 1420000C */  bnez       $at, .L80003B9C
    /* 476C 80003B6C 00000000 */   nop
  .L80003B70:
    /* 4770 80003B70 8FA40228 */  lw         $a0, 0x228($sp)
    /* 4774 80003B74 27A50064 */  addiu      $a1, $sp, 0x64
    /* 4778 80003B78 0C001F1A */  jal        func_80007C68
    /* 477C 80003B7C 24060010 */   addiu     $a2, $zero, 0x10
    /* 4780 80003B80 8FAA0228 */  lw         $t2, 0x228($sp)
    /* 4784 80003B84 254F0010 */  addiu      $t7, $t2, 0x10
    /* 4788 80003B88 AFAF0228 */  sw         $t7, 0x228($sp)
    /* 478C 80003B8C 2652FFF0 */  addiu      $s2, $s2, -0x10
    /* 4790 80003B90 2A410011 */  slti       $at, $s2, 0x11
    /* 4794 80003B94 1020FFF6 */  beqz       $at, .L80003B70
    /* 4798 80003B98 00000000 */   nop
  .L80003B9C:
    /* 479C 80003B9C 8FA40228 */  lw         $a0, 0x228($sp)
    /* 47A0 80003BA0 27A50064 */  addiu      $a1, $sp, 0x64
    /* 47A4 80003BA4 0C001F1A */  jal        func_80007C68
    /* 47A8 80003BA8 02403025 */   or        $a2, $s2, $zero
    /* 47AC 80003BAC 8FAB0228 */  lw         $t3, 0x228($sp)
    /* 47B0 80003BB0 01724021 */  addu       $t0, $t3, $s2
    /* 47B4 80003BB4 AFA80228 */  sw         $t0, 0x228($sp)
  .L80003BB8:
    /* 47B8 80003BB8 8FAE020C */  lw         $t6, 0x20C($sp)
    /* 47BC 80003BBC 8FB901E0 */  lw         $t9, 0x1E0($sp)
    /* 47C0 80003BC0 032E082A */  slt        $at, $t9, $t6
    /* 47C4 80003BC4 10200003 */  beqz       $at, .L80003BD4
    /* 47C8 80003BC8 00000000 */   nop
    /* 47CC 80003BCC 10000002 */  b          .L80003BD8
    /* 47D0 80003BD0 01C0A825 */   or        $s5, $t6, $zero
  .L80003BD4:
    /* 47D4 80003BD4 8FB501E0 */  lw         $s5, 0x1E0($sp)
  .L80003BD8:
    /* 47D8 80003BD8 8FA90210 */  lw         $t1, 0x210($sp)
    /* 47DC 80003BDC 01356021 */  addu       $t4, $t1, $s5
    /* 47E0 80003BE0 AFAC0210 */  sw         $t4, 0x210($sp)
    /* 47E4 80003BE4 8FAD0228 */  lw         $t5, 0x228($sp)
    /* 47E8 80003BE8 A1A00000 */  sb         $zero, 0x0($t5)
  .L80003BEC:
    /* 47EC 80003BEC 1000FC5F */  b          .L80002D6C
    /* 47F0 80003BF0 00000000 */   nop
  .L80003BF4:
    /* 47F4 80003BF4 8FB80228 */  lw         $t8, 0x228($sp)
    /* 47F8 80003BF8 A3000000 */  sb         $zero, 0x0($t8)
    /* 47FC 80003BFC 0C001F10 */  jal        func_80007C40
    /* 4800 80003C00 8FA40074 */   lw        $a0, 0x74($sp)
    /* 4804 80003C04 10000003 */  b          .L80003C14
    /* 4808 80003C08 00000000 */   nop
    /* 480C 80003C0C 10000001 */  b          .L80003C14
    /* 4810 80003C10 00000000 */   nop
  .L80003C14:
    /* 4814 80003C14 8FBF0044 */  lw         $ra, 0x44($sp)
    /* 4818 80003C18 8FB00028 */  lw         $s0, 0x28($sp)
    /* 481C 80003C1C 8FB1002C */  lw         $s1, 0x2C($sp)
    /* 4820 80003C20 8FB20030 */  lw         $s2, 0x30($sp)
    /* 4824 80003C24 8FB30034 */  lw         $s3, 0x34($sp)
    /* 4828 80003C28 8FB40038 */  lw         $s4, 0x38($sp)
    /* 482C 80003C2C 8FB5003C */  lw         $s5, 0x3C($sp)
    /* 4830 80003C30 8FB60040 */  lw         $s6, 0x40($sp)
    /* 4834 80003C34 03E00008 */  jr         $ra
    /* 4838 80003C38 27BD0228 */   addiu     $sp, $sp, 0x228
```

## Tips for This Category

