# __osException

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000C980` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~343 |

## Description

CPU exception handler (handwritten asm)

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/os` category.

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
./work/libultra/os/__osException/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/__osException

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/__osException --watch
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
# Source: D580.s
# Address: 0x8000C980

glabel func_8000C980
    /* D580 8000C980 3C1A8001 */  lui        $k0, %hi(D_8000C990) /* handwritten instruction */
    /* D584 8000C984 275AC990 */  addiu      $k0, $k0, %lo(D_8000C990) /* handwritten instruction */
    /* D588 8000C988 03400008 */  jr         $k0
    /* D58C 8000C98C 00000000 */   nop
  alabel D_8000C990
    /* D590 8000C990 3C1A8003 */  lui        $k0, %hi(D_80037DB0) /* handwritten instruction */
    /* D594 8000C994 275A7DB0 */  addiu      $k0, $k0, %lo(D_80037DB0) /* handwritten instruction */
    /* D598 8000C998 FF410020 */  sd         $at, 0x20($k0) /* handwritten instruction */
    /* D59C 8000C99C 401B6000 */  mfc0       $k1, $12 /* handwritten instruction */
    /* D5A0 8000C9A0 AF5B0118 */  sw         $k1, 0x118($k0) /* handwritten instruction */
    /* D5A4 8000C9A4 2401FFFC */  addiu      $at, $zero, -0x4
    /* D5A8 8000C9A8 0361D824 */  and        $k1, $k1, $at
    /* D5AC 8000C9AC 409B6000 */  mtc0       $k1, $12 /* handwritten instruction */
    /* D5B0 8000C9B0 FF480058 */  sd         $t0, 0x58($k0) /* handwritten instruction */
    /* D5B4 8000C9B4 FF490060 */  sd         $t1, 0x60($k0) /* handwritten instruction */
    /* D5B8 8000C9B8 FF4A0068 */  sd         $t2, 0x68($k0) /* handwritten instruction */
    /* D5BC 8000C9BC AF400018 */  sw         $zero, 0x18($k0) /* handwritten instruction */
    /* D5C0 8000C9C0 40086800 */  mfc0       $t0, $13 /* handwritten instruction */
    /* D5C4 8000C9C4 03404025 */  or         $t0, $k0, $zero
    /* D5C8 8000C9C8 3C1A8003 */  lui        $k0, %hi(D_8002C3E0) /* handwritten instruction */
    /* D5CC 8000C9CC 8F5AC3E0 */  lw         $k0, %lo(D_8002C3E0)($k0) /* handwritten instruction */
    /* D5D0 8000C9D0 DD090020 */  ld         $t1, 0x20($t0)
    /* D5D4 8000C9D4 FF490020 */  sd         $t1, 0x20($k0) /* handwritten instruction */
    /* D5D8 8000C9D8 DD090118 */  ld         $t1, 0x118($t0)
    /* D5DC 8000C9DC FF490118 */  sd         $t1, 0x118($k0) /* handwritten instruction */
    /* D5E0 8000C9E0 DD090058 */  ld         $t1, 0x58($t0)
    /* D5E4 8000C9E4 FF490058 */  sd         $t1, 0x58($k0) /* handwritten instruction */
    /* D5E8 8000C9E8 DD090060 */  ld         $t1, 0x60($t0)
    /* D5EC 8000C9EC FF490060 */  sd         $t1, 0x60($k0) /* handwritten instruction */
    /* D5F0 8000C9F0 DD090068 */  ld         $t1, 0x68($t0)
    /* D5F4 8000C9F4 FF490068 */  sd         $t1, 0x68($k0) /* handwritten instruction */
    /* D5F8 8000C9F8 8F5B0118 */  lw         $k1, 0x118($k0) /* handwritten instruction */
    /* D5FC 8000C9FC 00004012 */  mflo       $t0
    /* D600 8000CA00 FF480108 */  sd         $t0, 0x108($k0) /* handwritten instruction */
    /* D604 8000CA04 00004010 */  mfhi       $t0
    /* D608 8000CA08 3369FF00 */  andi       $t1, $k1, 0xFF00 /* handwritten instruction */
    /* D60C 8000CA0C FF420028 */  sd         $v0, 0x28($k0) /* handwritten instruction */
    /* D610 8000CA10 FF430030 */  sd         $v1, 0x30($k0) /* handwritten instruction */
    /* D614 8000CA14 FF440038 */  sd         $a0, 0x38($k0) /* handwritten instruction */
    /* D618 8000CA18 FF450040 */  sd         $a1, 0x40($k0) /* handwritten instruction */
    /* D61C 8000CA1C FF460048 */  sd         $a2, 0x48($k0) /* handwritten instruction */
    /* D620 8000CA20 FF470050 */  sd         $a3, 0x50($k0) /* handwritten instruction */
    /* D624 8000CA24 FF4B0070 */  sd         $t3, 0x70($k0) /* handwritten instruction */
    /* D628 8000CA28 FF4C0078 */  sd         $t4, 0x78($k0) /* handwritten instruction */
    /* D62C 8000CA2C FF4D0080 */  sd         $t5, 0x80($k0) /* handwritten instruction */
    /* D630 8000CA30 FF4E0088 */  sd         $t6, 0x88($k0) /* handwritten instruction */
    /* D634 8000CA34 FF4F0090 */  sd         $t7, 0x90($k0) /* handwritten instruction */
    /* D638 8000CA38 FF500098 */  sd         $s0, 0x98($k0) /* handwritten instruction */
    /* D63C 8000CA3C FF5100A0 */  sd         $s1, 0xA0($k0) /* handwritten instruction */
    /* D640 8000CA40 FF5200A8 */  sd         $s2, 0xA8($k0) /* handwritten instruction */
    /* D644 8000CA44 FF5300B0 */  sd         $s3, 0xB0($k0) /* handwritten instruction */
    /* D648 8000CA48 FF5400B8 */  sd         $s4, 0xB8($k0) /* handwritten instruction */
    /* D64C 8000CA4C FF5500C0 */  sd         $s5, 0xC0($k0) /* handwritten instruction */
    /* D650 8000CA50 FF5600C8 */  sd         $s6, 0xC8($k0) /* handwritten instruction */
    /* D654 8000CA54 FF5700D0 */  sd         $s7, 0xD0($k0) /* handwritten instruction */
    /* D658 8000CA58 FF5800D8 */  sd         $t8, 0xD8($k0) /* handwritten instruction */
    /* D65C 8000CA5C FF5900E0 */  sd         $t9, 0xE0($k0) /* handwritten instruction */
    /* D660 8000CA60 FF5C00E8 */  sd         $gp, 0xE8($k0) /* handwritten instruction */
    /* D664 8000CA64 FF5D00F0 */  sd         $sp, 0xF0($k0) /* handwritten instruction */
    /* D668 8000CA68 FF5E00F8 */  sd         $fp, 0xF8($k0) /* handwritten instruction */
    /* D66C 8000CA6C FF5F0100 */  sd         $ra, 0x100($k0) /* handwritten instruction */
    /* D670 8000CA70 11200011 */  beqz       $t1, .L8000CAB8
    /* D674 8000CA74 FF480110 */   sd        $t0, 0x110($k0) /* handwritten instruction */
    /* D678 8000CA78 3C088003 */  lui        $t0, %hi(D_8002C370)
    /* D67C 8000CA7C 2508C370 */  addiu      $t0, $t0, %lo(D_8002C370)
    /* D680 8000CA80 8D080000 */  lw         $t0, 0x0($t0)
    /* D684 8000CA84 2401FFFF */  addiu      $at, $zero, -0x1
    /* D688 8000CA88 01015026 */  xor        $t2, $t0, $at
    /* D68C 8000CA8C 3C01FFFF */  lui        $at, (0xFFFF00FF >> 16)
    /* D690 8000CA90 314AFF00 */  andi       $t2, $t2, 0xFF00
    /* D694 8000CA94 342100FF */  ori        $at, $at, (0xFFFF00FF & 0xFFFF)
    /* D698 8000CA98 012A6025 */  or         $t4, $t1, $t2
    /* D69C 8000CA9C 03615824 */  and        $t3, $k1, $at
    /* D6A0 8000CAA0 3108FF00 */  andi       $t0, $t0, 0xFF00
    /* D6A4 8000CAA4 016C5825 */  or         $t3, $t3, $t4
    /* D6A8 8000CAA8 01284824 */  and        $t1, $t1, $t0
    /* D6AC 8000CAAC 0361D824 */  and        $k1, $k1, $at
    /* D6B0 8000CAB0 AF4B0118 */  sw         $t3, 0x118($k0) /* handwritten instruction */
    /* D6B4 8000CAB4 0369D825 */  or         $k1, $k1, $t1
  .L8000CAB8:
    /* D6B8 8000CAB8 3C09A430 */  lui        $t1, %hi(MI_INTR_MASK_REG)
    /* D6BC 8000CABC 8D29000C */  lw         $t1, %lo(MI_INTR_MASK_REG)($t1)
    /* D6C0 8000CAC0 1120000B */  beqz       $t1, .L8000CAF0
    /* D6C4 8000CAC4 00000000 */   nop
    /* D6C8 8000CAC8 3C088003 */  lui        $t0, %hi(D_8002C370)
    /* D6CC 8000CACC 2508C370 */  addiu      $t0, $t0, %lo(D_8002C370)
    /* D6D0 8000CAD0 8D080000 */  lw         $t0, 0x0($t0)
    /* D6D4 8000CAD4 8F4C0128 */  lw         $t4, 0x128($k0) /* handwritten instruction */
    /* D6D8 8000CAD8 2401FFFF */  addiu      $at, $zero, -0x1
    /* D6DC 8000CADC 00084402 */  srl        $t0, $t0, 16
    /* D6E0 8000CAE0 01014026 */  xor        $t0, $t0, $at
    /* D6E4 8000CAE4 3108003F */  andi       $t0, $t0, 0x3F
    /* D6E8 8000CAE8 010C4024 */  and        $t0, $t0, $t4
    /* D6EC 8000CAEC 01284825 */  or         $t1, $t1, $t0
  .L8000CAF0:
    /* D6F0 8000CAF0 AF490128 */  sw         $t1, 0x128($k0) /* handwritten instruction */
    /* D6F4 8000CAF4 40087000 */  mfc0       $t0, $14 /* handwritten instruction */
    /* D6F8 8000CAF8 AF48011C */  sw         $t0, 0x11C($k0) /* handwritten instruction */
    /* D6FC 8000CAFC 8F480018 */  lw         $t0, 0x18($k0) /* handwritten instruction */
    /* D700 8000CB00 11000014 */  beqz       $t0, .L8000CB54
    /* D704 8000CB04 00000000 */   nop
    /* D708 8000CB08 4448F800 */  cfc1       $t0, $31
    /* D70C 8000CB0C 00000000 */  nop
    /* D710 8000CB10 AF48012C */  sw         $t0, 0x12C($k0) /* handwritten instruction */
    /* D714 8000CB14 F7400130 */  sdc1       $fv0, 0x130($k0)
    /* D718 8000CB18 F7420138 */  sdc1       $fv1, 0x138($k0)
    /* D71C 8000CB1C F7440140 */  sdc1       $ft0, 0x140($k0)
    /* D720 8000CB20 F7460148 */  sdc1       $ft1, 0x148($k0)
    /* D724 8000CB24 F7480150 */  sdc1       $ft2, 0x150($k0)
    /* D728 8000CB28 F74A0158 */  sdc1       $ft3, 0x158($k0)
    /* D72C 8000CB2C F74C0160 */  sdc1       $fa0, 0x160($k0)
    /* D730 8000CB30 F74E0168 */  sdc1       $fa1, 0x168($k0)
    /* D734 8000CB34 F7500170 */  sdc1       $ft4, 0x170($k0)
    /* D738 8000CB38 F7520178 */  sdc1       $ft5, 0x178($k0)
    /* D73C 8000CB3C F7540180 */  sdc1       $fs0, 0x180($k0)
    /* D740 8000CB40 F7560188 */  sdc1       $fs1, 0x188($k0)
    /* D744 8000CB44 F7580190 */  sdc1       $fs2, 0x190($k0)
    /* D748 8000CB48 F75A0198 */  sdc1       $fs3, 0x198($k0)
    /* D74C 8000CB4C F75C01A0 */  sdc1       $fs4, 0x1A0($k0)
    /* D750 8000CB50 F75E01A8 */  sdc1       $fs5, 0x1A8($k0)
  .L8000CB54:
    /* D754 8000CB54 40086800 */  mfc0       $t0, $13 /* handwritten instruction */
    /* D758 8000CB58 AF480120 */  sw         $t0, 0x120($k0) /* handwritten instruction */
    /* D75C 8000CB5C 24090002 */  addiu      $t1, $zero, 0x2
    /* D760 8000CB60 A7490010 */  sh         $t1, 0x10($k0) /* handwritten instruction */
    /* D764 8000CB64 3109007C */  andi       $t1, $t0, 0x7C
    /* D768 8000CB68 240A0024 */  addiu      $t2, $zero, 0x24
    /* D76C 8000CB6C 112A00B6 */  beq        $t1, $t2, .L8000CE48
    /* D770 8000CB70 00000000 */   nop
    /* D774 8000CB74 240A002C */  addiu      $t2, $zero, 0x2C
    /* D778 8000CB78 112A0105 */  beq        $t1, $t2, .L8000CF90
    /* D77C 8000CB7C 00000000 */   nop
    /* D780 8000CB80 240A0000 */  addiu      $t2, $zero, 0x0
    /* D784 8000CB84 152A00C9 */  bne        $t1, $t2, .L8000CEAC
    /* D788 8000CB88 00000000 */   nop
    /* D78C 8000CB8C 03688024 */  and        $s0, $k1, $t0
  .L8000CB90:
    /* D790 8000CB90 3209FF00 */  andi       $t1, $s0, 0xFF00
    /* D794 8000CB94 00095302 */  srl        $t2, $t1, 12
    /* D798 8000CB98 15400003 */  bnez       $t2, .L8000CBA8
    /* D79C 8000CB9C 00000000 */   nop
    /* D7A0 8000CBA0 00095202 */  srl        $t2, $t1, 8
    /* D7A4 8000CBA4 214A0010 */  addi       $t2, $t2, 0x10 /* handwritten instruction */
  .L8000CBA8:
    /* D7A8 8000CBA8 3C018003 */  lui        $at, %hi(D_8002D810)
    /* D7AC 8000CBAC 002A0821 */  addu       $at, $at, $t2
    /* D7B0 8000CBB0 902AD810 */  lbu        $t2, %lo(D_8002D810)($at)
    /* D7B4 8000CBB4 3C018003 */  lui        $at, %hi(jtbl_8002D830_main)
    /* D7B8 8000CBB8 002A0821 */  addu       $at, $at, $t2
    /* D7BC 8000CBBC 8C2AD830 */  lw         $t2, %lo(jtbl_8002D830_main)($at)
    /* D7C0 8000CBC0 01400008 */  jr         $t2
    /* D7C4 8000CBC4 00000000 */   nop
    /* D7C8 8000CBC8 2401DFFF */  addiu      $at, $zero, -0x2001
    /* D7CC 8000CBCC 1000FFF0 */  b          .L8000CB90
    /* D7D0 8000CBD0 02018024 */   and       $s0, $s0, $at
    /* D7D4 8000CBD4 2401BFFF */  addiu      $at, $zero, -0x4001
    /* D7D8 8000CBD8 1000FFED */  b          .L8000CB90
    /* D7DC 8000CBDC 02018024 */   and       $s0, $s0, $at
    /* D7E0 8000CBE0 40095800 */  mfc0       $t1, $11 /* handwritten instruction */
    /* D7E4 8000CBE4 40895800 */  mtc0       $t1, $11 /* handwritten instruction */
    /* D7E8 8000CBE8 0C0033B7 */  jal        func_8000CEDC
    /* D7EC 8000CBEC 24040018 */   addiu     $a0, $zero, 0x18
    /* D7F0 8000CBF0 3C01FFFF */  lui        $at, (0xFFFF7FFF >> 16)
    /* D7F4 8000CBF4 34217FFF */  ori        $at, $at, (0xFFFF7FFF & 0xFFFF)
    /* D7F8 8000CBF8 1000FFE5 */  b          .L8000CB90
    /* D7FC 8000CBFC 02018024 */   and       $s0, $s0, $at
    /* D800 8000CC00 3C098003 */  lui        $t1, %hi(D_8002C470)
    /* D804 8000CC04 2529C470 */  addiu      $t1, $t1, %lo(D_8002C470)
    /* D808 8000CC08 8D2A0008 */  lw         $t2, 0x8($t1)
    /* D80C 8000CC0C 2401F7FF */  addiu      $at, $zero, -0x801
    /* D810 8000CC10 02018024 */  and        $s0, $s0, $at
    /* D814 8000CC14 11400007 */  beqz       $t2, .L8000CC34
    /* D818 8000CC18 21290008 */   addi      $t1, $t1, 0x8 /* handwritten instruction */
    /* D81C 8000CC1C 0140F809 */  jalr       $t2
    /* D820 8000CC20 8D3D0004 */   lw        $sp, 0x4($t1)
    /* D824 8000CC24 10400003 */  beqz       $v0, .L8000CC34
    /* D828 8000CC28 00000000 */   nop
    /* D82C 8000CC2C 1000008C */  b          .L8000CE60
    /* D830 8000CC30 00000000 */   nop
  .L8000CC34:
    /* D834 8000CC34 0C0033B7 */  jal        func_8000CEDC
    /* D838 8000CC38 24040010 */   addiu     $a0, $zero, 0x10
    /* D83C 8000CC3C 1000FFD4 */  b          .L8000CB90
    /* D840 8000CC40 00000000 */   nop
    /* D844 8000CC44 3C088003 */  lui        $t0, %hi(D_8002C370)
    /* D848 8000CC48 2508C370 */  addiu      $t0, $t0, %lo(D_8002C370)
    /* D84C 8000CC4C 8D080000 */  lw         $t0, 0x0($t0)
    /* D850 8000CC50 3C11A430 */  lui        $s1, %hi(MI_INTR_REG)
    /* D854 8000CC54 8E310008 */  lw         $s1, %lo(MI_INTR_REG)($s1)
    /* D858 8000CC58 00084402 */  srl        $t0, $t0, 16
    /* D85C 8000CC5C 02288824 */  and        $s1, $s1, $t0
    /* D860 8000CC60 32290001 */  andi       $t1, $s1, 0x1
    /* D864 8000CC64 11200013 */  beqz       $t1, .L8000CCB4
    /* D868 8000CC68 00000000 */   nop
    /* D86C 8000CC6C 3C0CA404 */  lui        $t4, %hi(SP_STATUS_REG)
    /* D870 8000CC70 8D8C0010 */  lw         $t4, %lo(SP_STATUS_REG)($t4)
    /* D874 8000CC74 34098008 */  ori        $t1, $zero, 0x8008
    /* D878 8000CC78 3C01A404 */  lui        $at, %hi(SP_STATUS_REG)
    /* D87C 8000CC7C 318C0300 */  andi       $t4, $t4, 0x300
    /* D880 8000CC80 3231003E */  andi       $s1, $s1, 0x3E
    /* D884 8000CC84 11800007 */  beqz       $t4, .L8000CCA4
    /* D888 8000CC88 AC290010 */   sw        $t1, %lo(SP_STATUS_REG)($at)
    /* D88C 8000CC8C 0C0033B7 */  jal        func_8000CEDC
    /* D890 8000CC90 24040020 */   addiu     $a0, $zero, 0x20
    /* D894 8000CC94 12200042 */  beqz       $s1, .L8000CDA0
    /* D898 8000CC98 00000000 */   nop
    /* D89C 8000CC9C 10000005 */  b          .L8000CCB4
    /* D8A0 8000CCA0 00000000 */   nop
  .L8000CCA4:
    /* D8A4 8000CCA4 0C0033B7 */  jal        func_8000CEDC
    /* D8A8 8000CCA8 24040058 */   addiu     $a0, $zero, 0x58
    /* D8AC 8000CCAC 1220003C */  beqz       $s1, .L8000CDA0
    /* D8B0 8000CCB0 00000000 */   nop
  .L8000CCB4:
    /* D8B4 8000CCB4 32290008 */  andi       $t1, $s1, 0x8
    /* D8B8 8000CCB8 11200007 */  beqz       $t1, .L8000CCD8
    /* D8BC 8000CCBC 3C01A440 */   lui       $at, %hi(VI_CURRENT_REG)
    /* D8C0 8000CCC0 32310037 */  andi       $s1, $s1, 0x37
    /* D8C4 8000CCC4 AC200010 */  sw         $zero, %lo(VI_CURRENT_REG)($at)
    /* D8C8 8000CCC8 0C0033B7 */  jal        func_8000CEDC
    /* D8CC 8000CCCC 24040038 */   addiu     $a0, $zero, 0x38
    /* D8D0 8000CCD0 12200033 */  beqz       $s1, .L8000CDA0
    /* D8D4 8000CCD4 00000000 */   nop
  .L8000CCD8:
    /* D8D8 8000CCD8 32290004 */  andi       $t1, $s1, 0x4
    /* D8DC 8000CCDC 11200009 */  beqz       $t1, .L8000CD04
    /* D8E0 8000CCE0 00000000 */   nop
    /* D8E4 8000CCE4 24090001 */  addiu      $t1, $zero, 0x1
    /* D8E8 8000CCE8 3C01A450 */  lui        $at, %hi(AI_STATUS_REG)
    /* D8EC 8000CCEC 3231003B */  andi       $s1, $s1, 0x3B
    /* D8F0 8000CCF0 AC29000C */  sw         $t1, %lo(AI_STATUS_REG)($at)
    /* D8F4 8000CCF4 0C0033B7 */  jal        func_8000CEDC
    /* D8F8 8000CCF8 24040030 */   addiu     $a0, $zero, 0x30
    /* D8FC 8000CCFC 12200028 */  beqz       $s1, .L8000CDA0
    /* D900 8000CD00 00000000 */   nop
  .L8000CD04:
    /* D904 8000CD04 32290002 */  andi       $t1, $s1, 0x2
    /* D908 8000CD08 11200007 */  beqz       $t1, .L8000CD28
    /* D90C 8000CD0C 3C01A480 */   lui       $at, %hi(SI_STATUS_REG)
    /* D910 8000CD10 3231003D */  andi       $s1, $s1, 0x3D
    /* D914 8000CD14 AC200018 */  sw         $zero, %lo(SI_STATUS_REG)($at)
    /* D918 8000CD18 0C0033B7 */  jal        func_8000CEDC
    /* D91C 8000CD1C 24040028 */   addiu     $a0, $zero, 0x28
    /* D920 8000CD20 1220001F */  beqz       $s1, .L8000CDA0
    /* D924 8000CD24 00000000 */   nop
  .L8000CD28:
    /* D928 8000CD28 32290010 */  andi       $t1, $s1, 0x10
    /* D92C 8000CD2C 11200013 */  beqz       $t1, .L8000CD7C
    /* D930 8000CD30 00000000 */   nop
    /* D934 8000CD34 24090002 */  addiu      $t1, $zero, 0x2
    /* D938 8000CD38 3C01A460 */  lui        $at, %hi(PI_STATUS_REG)
    /* D93C 8000CD3C AC290010 */  sw         $t1, %lo(PI_STATUS_REG)($at)
    /* D940 8000CD40 3C098003 */  lui        $t1, %hi(D_8002C498)
    /* D944 8000CD44 2529C498 */  addiu      $t1, $t1, %lo(D_8002C498)
    /* D948 8000CD48 8D2A0000 */  lw         $t2, 0x0($t1)
    /* D94C 8000CD4C 3231002F */  andi       $s1, $s1, 0x2F
    /* D950 8000CD50 11400006 */  beqz       $t2, .L8000CD6C
    /* D954 8000CD54 00000000 */   nop
    /* D958 8000CD58 8D3D0004 */  lw         $sp, 0x4($t1)
    /* D95C 8000CD5C 0140F809 */  jalr       $t2
    /* D960 8000CD60 00402025 */   or        $a0, $v0, $zero
    /* D964 8000CD64 14400003 */  bnez       $v0, .L8000CD74
    /* D968 8000CD68 00000000 */   nop
  .L8000CD6C:
    /* D96C 8000CD6C 0C0033B7 */  jal        func_8000CEDC
    /* D970 8000CD70 24040040 */   addiu     $a0, $zero, 0x40
  .L8000CD74:
    /* D974 8000CD74 1220000A */  beqz       $s1, .L8000CDA0
    /* D978 8000CD78 00000000 */   nop
  .L8000CD7C:
    /* D97C 8000CD7C 32290020 */  andi       $t1, $s1, 0x20
    /* D980 8000CD80 11200007 */  beqz       $t1, .L8000CDA0
    /* D984 8000CD84 00000000 */   nop
    /* D988 8000CD88 24090800 */  addiu      $t1, $zero, 0x800
    /* D98C 8000CD8C 3C01A430 */  lui        $at, %hi(MI_MODE_REG)
    /* D990 8000CD90 3231001F */  andi       $s1, $s1, 0x1F
    /* D994 8000CD94 AC290000 */  sw         $t1, %lo(MI_MODE_REG)($at)
    /* D998 8000CD98 0C0033B7 */  jal        func_8000CEDC
    /* D99C 8000CD9C 24040048 */   addiu     $a0, $zero, 0x48
  .L8000CDA0:
    /* D9A0 8000CDA0 2401FBFF */  addiu      $at, $zero, -0x401
    /* D9A4 8000CDA4 1000FF7A */  b          .L8000CB90
    /* D9A8 8000CDA8 02018024 */   and       $s0, $s0, $at
    /* D9AC 8000CDAC 8F5B0118 */  lw         $k1, 0x118($k0) /* handwritten instruction */
    /* D9B0 8000CDB0 2401EFFF */  addiu      $at, $zero, -0x1001
    /* D9B4 8000CDB4 3C098003 */  lui        $t1, %hi(D_8002C36C)
    /* D9B8 8000CDB8 0361D824 */  and        $k1, $k1, $at
    /* D9BC 8000CDBC AF5B0118 */  sw         $k1, 0x118($k0) /* handwritten instruction */
    /* D9C0 8000CDC0 2529C36C */  addiu      $t1, $t1, %lo(D_8002C36C)
    /* D9C4 8000CDC4 8D2A0000 */  lw         $t2, 0x0($t1)
    /* D9C8 8000CDC8 11400003 */  beqz       $t2, .L8000CDD8
    /* D9CC 8000CDCC 2401EFFF */   addiu     $at, $zero, -0x1001
    /* D9D0 8000CDD0 10000023 */  b          .L8000CE60
    /* D9D4 8000CDD4 02018024 */   and       $s0, $s0, $at
  .L8000CDD8:
    /* D9D8 8000CDD8 240A0001 */  addiu      $t2, $zero, 0x1
    /* D9DC 8000CDDC AD2A0000 */  sw         $t2, 0x0($t1)
    /* D9E0 8000CDE0 0C0033B7 */  jal        func_8000CEDC
    /* D9E4 8000CDE4 24040070 */   addiu     $a0, $zero, 0x70
    /* D9E8 8000CDE8 3C0A8003 */  lui        $t2, %hi(D_8002C3D8)
    /* D9EC 8000CDEC 8D4AC3D8 */  lw         $t2, %lo(D_8002C3D8)($t2)
    /* D9F0 8000CDF0 2401EFFF */  addiu      $at, $zero, -0x1001
    /* D9F4 8000CDF4 02018024 */  and        $s0, $s0, $at
    /* D9F8 8000CDF8 8D5B0118 */  lw         $k1, 0x118($t2) /* handwritten instruction */
    /* D9FC 8000CDFC 0361D824 */  and        $k1, $k1, $at
    /* DA00 8000CE00 10000017 */  b          .L8000CE60
    /* DA04 8000CE04 AD5B0118 */   sw        $k1, 0x118($t2) /* handwritten instruction */
    /* DA08 8000CE08 2401FDFF */  addiu      $at, $zero, -0x201
    /* DA0C 8000CE0C 01014024 */  and        $t0, $t0, $at
    /* DA10 8000CE10 40886800 */  mtc0       $t0, $13 /* handwritten instruction */
    /* DA14 8000CE14 0C0033B7 */  jal        func_8000CEDC
    /* DA18 8000CE18 24040008 */   addiu     $a0, $zero, 0x8
    /* DA1C 8000CE1C 2401FDFF */  addiu      $at, $zero, -0x201
    /* DA20 8000CE20 1000FF5B */  b          .L8000CB90
    /* DA24 8000CE24 02018024 */   and       $s0, $s0, $at
    /* DA28 8000CE28 2401FEFF */  addiu      $at, $zero, -0x101
    /* DA2C 8000CE2C 01014024 */  and        $t0, $t0, $at
    /* DA30 8000CE30 40886800 */  mtc0       $t0, $13 /* handwritten instruction */
    /* DA34 8000CE34 0C0033B7 */  jal        func_8000CEDC
    /* DA38 8000CE38 24040000 */   addiu     $a0, $zero, 0x0
    /* DA3C 8000CE3C 2401FEFF */  addiu      $at, $zero, -0x101
    /* DA40 8000CE40 1000FF53 */  b          .L8000CB90
    /* DA44 8000CE44 02018024 */   and       $s0, $s0, $at
  .L8000CE48:
    /* DA48 8000CE48 24090001 */  addiu      $t1, $zero, 0x1
    /* DA4C 8000CE4C A7490012 */  sh         $t1, 0x12($k0) /* handwritten instruction */
    /* DA50 8000CE50 0C0033B7 */  jal        func_8000CEDC
    /* DA54 8000CE54 24040050 */   addiu     $a0, $zero, 0x50
    /* DA58 8000CE58 10000001 */  b          .L8000CE60
    /* DA5C 8000CE5C 00000000 */   nop
  .L8000CE60:
    /* DA60 8000CE60 3C0A8003 */  lui        $t2, %hi(D_8002C3D8)
    /* DA64 8000CE64 8D4AC3D8 */  lw         $t2, %lo(D_8002C3D8)($t2)
    /* DA68 8000CE68 8F490004 */  lw         $t1, 0x4($k0) /* handwritten instruction */
    /* DA6C 8000CE6C 8D4B0004 */  lw         $t3, 0x4($t2)
    /* DA70 8000CE70 012B082A */  slt        $at, $t1, $t3
    /* DA74 8000CE74 10200007 */  beqz       $at, .L8000CE94
    /* DA78 8000CE78 00000000 */   nop
    /* DA7C 8000CE7C 3C048003 */  lui        $a0, %hi(D_8002C3D8)
    /* DA80 8000CE80 03402825 */  or         $a1, $k0, $zero
    /* DA84 8000CE84 0C003431 */  jal        func_8000D0C4
    /* DA88 8000CE88 2484C3D8 */   addiu     $a0, $a0, %lo(D_8002C3D8)
    /* DA8C 8000CE8C 08003447 */  j          func_8000D11C
    /* DA90 8000CE90 00000000 */   nop
  .L8000CE94:
    /* DA94 8000CE94 3C098003 */  lui        $t1, %hi(D_8002C3D8)
    /* DA98 8000CE98 2529C3D8 */  addiu      $t1, $t1, %lo(D_8002C3D8)
    /* DA9C 8000CE9C 8D2A0000 */  lw         $t2, 0x0($t1)
    /* DAA0 8000CEA0 AF4A0000 */  sw         $t2, 0x0($k0) /* handwritten instruction */
    /* DAA4 8000CEA4 08003447 */  j          func_8000D11C
    /* DAA8 8000CEA8 AD3A0000 */   sw        $k0, 0x0($t1) /* handwritten instruction */
  .L8000CEAC:
    /* DAAC 8000CEAC 3C018003 */  lui        $at, %hi(D_8002C3E4)
    /* DAB0 8000CEB0 AC3AC3E4 */  sw         $k0, %lo(D_8002C3E4)($at) /* handwritten instruction */
    /* DAB4 8000CEB4 24090001 */  addiu      $t1, $zero, 0x1
    /* DAB8 8000CEB8 A7490010 */  sh         $t1, 0x10($k0) /* handwritten instruction */
    /* DABC 8000CEBC 24090002 */  addiu      $t1, $zero, 0x2
    /* DAC0 8000CEC0 A7490012 */  sh         $t1, 0x12($k0) /* handwritten instruction */
    /* DAC4 8000CEC4 400A4000 */  mfc0       $t2, $8 /* handwritten instruction */
    /* DAC8 8000CEC8 AF4A0124 */  sw         $t2, 0x124($k0) /* handwritten instruction */
    /* DACC 8000CECC 0C0033B7 */  jal        func_8000CEDC
    /* DAD0 8000CED0 24040060 */   addiu     $a0, $zero, 0x60
    /* DAD4 8000CED4 08003447 */  j          func_8000D11C
    /* DAD8 8000CED8 00000000 */   nop
```

## Tips for This Category

