# inflate_free_window

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80005464` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~318 |

## Description

free huffman decode tables

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
./work/inflate/inflate_free_window/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/inflate_free_window

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/inflate_free_window --watch
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
# Address: 0x80005464

glabel func_80005464
    /* 6064 80005464 27BDFFA0 */  addiu      $sp, $sp, -0x60
    /* 6068 80005468 AFA60068 */  sw         $a2, 0x68($sp)
    /* 606C 8000546C AFA7006C */  sw         $a3, 0x6C($sp)
    /* 6070 80005470 8FA8006C */  lw         $t0, 0x6C($sp)
    /* 6074 80005474 8FAE0068 */  lw         $t6, 0x68($sp)
    /* 6078 80005478 AFBE0038 */  sw         $fp, 0x38($sp)
    /* 607C 8000547C 3C1E8003 */  lui        $fp, %hi(D_8002B184)
    /* 6080 80005480 27DEB184 */  addiu      $fp, $fp, %lo(D_8002B184)
    /* 6084 80005484 00084840 */  sll        $t1, $t0, 1
    /* 6088 80005488 000E7840 */  sll        $t7, $t6, 1
    /* 608C 8000548C 03CFC021 */  addu       $t8, $fp, $t7
    /* 6090 80005490 03C95021 */  addu       $t2, $fp, $t1
    /* 6094 80005494 954B0000 */  lhu        $t3, 0x0($t2)
    /* 6098 80005498 97190000 */  lhu        $t9, 0x0($t8)
    /* 609C 8000549C AFB70034 */  sw         $s7, 0x34($sp)
    /* 60A0 800054A0 AFB40028 */  sw         $s4, 0x28($sp)
    /* 60A4 800054A4 AFB20020 */  sw         $s2, 0x20($sp)
    /* 60A8 800054A8 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 60AC 800054AC AFB00018 */  sw         $s0, 0x18($sp)
    /* 60B0 800054B0 3C108003 */  lui        $s0, %hi(D_800354C0)
    /* 60B4 800054B4 3C118003 */  lui        $s1, %hi(D_800354B0)
    /* 60B8 800054B8 3C128003 */  lui        $s2, %hi(D_800354BC)
    /* 60BC 800054BC 3C148003 */  lui        $s4, %hi(D_800354B4)
    /* 60C0 800054C0 3C178003 */  lui        $s7, %hi(D_800354B8)
    /* 60C4 800054C4 26F754B8 */  addiu      $s7, $s7, %lo(D_800354B8)
    /* 60C8 800054C8 269454B4 */  addiu      $s4, $s4, %lo(D_800354B4)
    /* 60CC 800054CC 8E5254BC */  lw         $s2, %lo(D_800354BC)($s2)
    /* 60D0 800054D0 263154B0 */  addiu      $s1, $s1, %lo(D_800354B0)
    /* 60D4 800054D4 8E1054C0 */  lw         $s0, %lo(D_800354C0)($s0)
    /* 60D8 800054D8 AFBF003C */  sw         $ra, 0x3C($sp)
    /* 60DC 800054DC AFB60030 */  sw         $s6, 0x30($sp)
    /* 60E0 800054E0 AFB5002C */  sw         $s5, 0x2C($sp)
    /* 60E4 800054E4 AFB30024 */  sw         $s3, 0x24($sp)
    /* 60E8 800054E8 AFA40060 */  sw         $a0, 0x60($sp)
    /* 60EC 800054EC AFA50064 */  sw         $a1, 0x64($sp)
    /* 60F0 800054F0 AFAB0044 */  sw         $t3, 0x44($sp)
    /* 60F4 800054F4 AFB90048 */  sw         $t9, 0x48($sp)
  .L800054F8:
    /* 60F8 800054F8 8FAC0068 */  lw         $t4, 0x68($sp)
    /* 60FC 800054FC 020C082B */  sltu       $at, $s0, $t4
    /* 6100 80005500 10200017 */  beqz       $at, .L80005560
    /* 6104 80005504 00000000 */   nop
  .L80005508:
    /* 6108 80005508 8E220000 */  lw         $v0, 0x0($s1)
    /* 610C 8000550C 8E8D0000 */  lw         $t5, 0x0($s4)
    /* 6110 80005510 004D082B */  sltu       $at, $v0, $t5
    /* 6114 80005514 10200009 */  beqz       $at, .L8000553C
    /* 6118 80005518 00000000 */   nop
    /* 611C 8000551C 244E0002 */  addiu      $t6, $v0, 0x2
    /* 6120 80005520 AE2E0000 */  sw         $t6, 0x0($s1)
    /* 6124 80005524 8E220000 */  lw         $v0, 0x0($s1)
    /* 6128 80005528 904FFFFF */  lbu        $t7, -0x1($v0)
    /* 612C 8000552C 9059FFFE */  lbu        $t9, -0x2($v0)
    /* 6130 80005530 000FC200 */  sll        $t8, $t7, 8
    /* 6134 80005534 10000004 */  b          .L80005548
    /* 6138 80005538 03191825 */   or        $v1, $t8, $t9
  .L8000553C:
    /* 613C 8000553C 0C0019B5 */  jal        func_800066D4
    /* 6140 80005540 00000000 */   nop
    /* 6144 80005544 00401825 */  or         $v1, $v0, $zero
  .L80005548:
    /* 6148 80005548 8FA90068 */  lw         $t1, 0x68($sp)
    /* 614C 8000554C 02034004 */  sllv       $t0, $v1, $s0
    /* 6150 80005550 26100010 */  addiu      $s0, $s0, 0x10
    /* 6154 80005554 0209082B */  sltu       $at, $s0, $t1
    /* 6158 80005558 1420FFEB */  bnez       $at, .L80005508
    /* 615C 8000555C 02489025 */   or        $s2, $s2, $t0
  .L80005560:
    /* 6160 80005560 8FAA0048 */  lw         $t2, 0x48($sp)
    /* 6164 80005564 8FAD0060 */  lw         $t5, 0x60($sp)
    /* 6168 80005568 024A5824 */  and        $t3, $s2, $t2
    /* 616C 8000556C 000B60C0 */  sll        $t4, $t3, 3
    /* 6170 80005570 018D1021 */  addu       $v0, $t4, $t5
    /* 6174 80005574 90530000 */  lbu        $s3, 0x0($v0)
    /* 6178 80005578 0040A825 */  or         $s5, $v0, $zero
    /* 617C 8000557C 2E610011 */  sltiu      $at, $s3, 0x11
    /* 6180 80005580 1420002C */  bnez       $at, .L80005634
    /* 6184 80005584 00000000 */   nop
  .L80005588:
    /* 6188 80005588 24010063 */  addiu      $at, $zero, 0x63
    /* 618C 8000558C 16610003 */  bne        $s3, $at, .L8000559C
    /* 6190 80005590 00000000 */   nop
    /* 6194 80005594 100000E5 */  b          .L8000592C
    /* 6198 80005598 24020001 */   addiu     $v0, $zero, 0x1
  .L8000559C:
    /* 619C 8000559C 92A20001 */  lbu        $v0, 0x1($s5)
    /* 61A0 800055A0 2673FFF0 */  addiu      $s3, $s3, -0x10
    /* 61A4 800055A4 02028023 */  subu       $s0, $s0, $v0
    /* 61A8 800055A8 0213082B */  sltu       $at, $s0, $s3
    /* 61AC 800055AC 10200016 */  beqz       $at, .L80005608
    /* 61B0 800055B0 00529006 */   srlv      $s2, $s2, $v0
  .L800055B4:
    /* 61B4 800055B4 8E220000 */  lw         $v0, 0x0($s1)
    /* 61B8 800055B8 8E8E0000 */  lw         $t6, 0x0($s4)
    /* 61BC 800055BC 004E082B */  sltu       $at, $v0, $t6
    /* 61C0 800055C0 10200009 */  beqz       $at, .L800055E8
    /* 61C4 800055C4 00000000 */   nop
    /* 61C8 800055C8 244F0002 */  addiu      $t7, $v0, 0x2
    /* 61CC 800055CC AE2F0000 */  sw         $t7, 0x0($s1)
    /* 61D0 800055D0 8E220000 */  lw         $v0, 0x0($s1)
    /* 61D4 800055D4 9058FFFF */  lbu        $t8, -0x1($v0)
    /* 61D8 800055D8 9048FFFE */  lbu        $t0, -0x2($v0)
    /* 61DC 800055DC 0018CA00 */  sll        $t9, $t8, 8
    /* 61E0 800055E0 10000004 */  b          .L800055F4
    /* 61E4 800055E4 03281825 */   or        $v1, $t9, $t0
  .L800055E8:
    /* 61E8 800055E8 0C0019B5 */  jal        func_800066D4
    /* 61EC 800055EC 00000000 */   nop
    /* 61F0 800055F0 00401825 */  or         $v1, $v0, $zero
  .L800055F4:
    /* 61F4 800055F4 02034804 */  sllv       $t1, $v1, $s0
    /* 61F8 800055F8 26100010 */  addiu      $s0, $s0, 0x10
    /* 61FC 800055FC 0213082B */  sltu       $at, $s0, $s3
    /* 6200 80005600 1420FFEC */  bnez       $at, .L800055B4
    /* 6204 80005604 02499025 */   or        $s2, $s2, $t1
  .L80005608:
    /* 6208 80005608 00135040 */  sll        $t2, $s3, 1
    /* 620C 8000560C 03CA5821 */  addu       $t3, $fp, $t2
    /* 6210 80005610 956C0000 */  lhu        $t4, 0x0($t3)
    /* 6214 80005614 8EAF0004 */  lw         $t7, 0x4($s5)
    /* 6218 80005618 01926824 */  and        $t5, $t4, $s2
    /* 621C 8000561C 000D70C0 */  sll        $t6, $t5, 3
    /* 6220 80005620 01CFA821 */  addu       $s5, $t6, $t7
    /* 6224 80005624 92B30000 */  lbu        $s3, 0x0($s5)
    /* 6228 80005628 2E610011 */  sltiu      $at, $s3, 0x11
    /* 622C 8000562C 1020FFD6 */  beqz       $at, .L80005588
    /* 6230 80005630 00000000 */   nop
  .L80005634:
    /* 6234 80005634 92A20001 */  lbu        $v0, 0x1($s5)
    /* 6238 80005638 24010010 */  addiu      $at, $zero, 0x10
    /* 623C 8000563C 00529006 */  srlv       $s2, $s2, $v0
    /* 6240 80005640 16610008 */  bne        $s3, $at, .L80005664
    /* 6244 80005644 02028023 */   subu      $s0, $s0, $v0
    /* 6248 80005648 96B80004 */  lhu        $t8, 0x4($s5)
    /* 624C 8000564C 8EF90000 */  lw         $t9, 0x0($s7)
    /* 6250 80005650 A3380000 */  sb         $t8, 0x0($t9)
    /* 6254 80005654 8EE80000 */  lw         $t0, 0x0($s7)
    /* 6258 80005658 25090001 */  addiu      $t1, $t0, 0x1
    /* 625C 8000565C 1000FFA6 */  b          .L800054F8
    /* 6260 80005660 AEE90000 */   sw        $t1, 0x0($s7)
  .L80005664:
    /* 6264 80005664 2401000F */  addiu      $at, $zero, 0xF
    /* 6268 80005668 126100AB */  beq        $s3, $at, .L80005918
    /* 626C 8000566C 00000000 */   nop
    /* 6270 80005670 0213082B */  sltu       $at, $s0, $s3
    /* 6274 80005674 10200016 */  beqz       $at, .L800056D0
    /* 6278 80005678 00000000 */   nop
  .L8000567C:
    /* 627C 8000567C 8E220000 */  lw         $v0, 0x0($s1)
    /* 6280 80005680 8E8A0000 */  lw         $t2, 0x0($s4)
    /* 6284 80005684 004A082B */  sltu       $at, $v0, $t2
    /* 6288 80005688 10200009 */  beqz       $at, .L800056B0
    /* 628C 8000568C 00000000 */   nop
    /* 6290 80005690 244B0002 */  addiu      $t3, $v0, 0x2
    /* 6294 80005694 AE2B0000 */  sw         $t3, 0x0($s1)
    /* 6298 80005698 8E220000 */  lw         $v0, 0x0($s1)
    /* 629C 8000569C 904CFFFF */  lbu        $t4, -0x1($v0)
    /* 62A0 800056A0 904EFFFE */  lbu        $t6, -0x2($v0)
    /* 62A4 800056A4 000C6A00 */  sll        $t5, $t4, 8
    /* 62A8 800056A8 10000004 */  b          .L800056BC
    /* 62AC 800056AC 01AE1825 */   or        $v1, $t5, $t6
  .L800056B0:
    /* 62B0 800056B0 0C0019B5 */  jal        func_800066D4
    /* 62B4 800056B4 00000000 */   nop
    /* 62B8 800056B8 00401825 */  or         $v1, $v0, $zero
  .L800056BC:
    /* 62BC 800056BC 02037804 */  sllv       $t7, $v1, $s0
    /* 62C0 800056C0 26100010 */  addiu      $s0, $s0, 0x10
    /* 62C4 800056C4 0213082B */  sltu       $at, $s0, $s3
    /* 62C8 800056C8 1420FFEC */  bnez       $at, .L8000567C
    /* 62CC 800056CC 024F9025 */   or        $s2, $s2, $t7
  .L800056D0:
    /* 62D0 800056D0 0013C040 */  sll        $t8, $s3, 1
    /* 62D4 800056D4 03D8C821 */  addu       $t9, $fp, $t8
    /* 62D8 800056D8 97280000 */  lhu        $t0, 0x0($t9)
    /* 62DC 800056DC 8FAB006C */  lw         $t3, 0x6C($sp)
    /* 62E0 800056E0 96AA0004 */  lhu        $t2, 0x4($s5)
    /* 62E4 800056E4 02138023 */  subu       $s0, $s0, $s3
    /* 62E8 800056E8 01124824 */  and        $t1, $t0, $s2
    /* 62EC 800056EC 020B082B */  sltu       $at, $s0, $t3
    /* 62F0 800056F0 02729006 */  srlv       $s2, $s2, $s3
    /* 62F4 800056F4 10200017 */  beqz       $at, .L80005754
    /* 62F8 800056F8 012AB021 */   addu      $s6, $t1, $t2
  .L800056FC:
    /* 62FC 800056FC 8E220000 */  lw         $v0, 0x0($s1)
    /* 6300 80005700 8E8C0000 */  lw         $t4, 0x0($s4)
    /* 6304 80005704 004C082B */  sltu       $at, $v0, $t4
    /* 6308 80005708 10200009 */  beqz       $at, .L80005730
    /* 630C 8000570C 00000000 */   nop
    /* 6310 80005710 244D0002 */  addiu      $t5, $v0, 0x2
    /* 6314 80005714 AE2D0000 */  sw         $t5, 0x0($s1)
    /* 6318 80005718 8E220000 */  lw         $v0, 0x0($s1)
    /* 631C 8000571C 904EFFFF */  lbu        $t6, -0x1($v0)
    /* 6320 80005720 9058FFFE */  lbu        $t8, -0x2($v0)
    /* 6324 80005724 000E7A00 */  sll        $t7, $t6, 8
    /* 6328 80005728 10000004 */  b          .L8000573C
    /* 632C 8000572C 01F81825 */   or        $v1, $t7, $t8
  .L80005730:
    /* 6330 80005730 0C0019B5 */  jal        func_800066D4
    /* 6334 80005734 00000000 */   nop
    /* 6338 80005738 00401825 */  or         $v1, $v0, $zero
  .L8000573C:
    /* 633C 8000573C 8FA8006C */  lw         $t0, 0x6C($sp)
    /* 6340 80005740 0203C804 */  sllv       $t9, $v1, $s0
    /* 6344 80005744 26100010 */  addiu      $s0, $s0, 0x10
    /* 6348 80005748 0208082B */  sltu       $at, $s0, $t0
    /* 634C 8000574C 1420FFEB */  bnez       $at, .L800056FC
    /* 6350 80005750 02599025 */   or        $s2, $s2, $t9
  .L80005754:
    /* 6354 80005754 8FA90044 */  lw         $t1, 0x44($sp)
    /* 6358 80005758 8FAC0064 */  lw         $t4, 0x64($sp)
    /* 635C 8000575C 02495024 */  and        $t2, $s2, $t1
    /* 6360 80005760 000A58C0 */  sll        $t3, $t2, 3
    /* 6364 80005764 016C1021 */  addu       $v0, $t3, $t4
    /* 6368 80005768 90530000 */  lbu        $s3, 0x0($v0)
    /* 636C 8000576C 0040A825 */  or         $s5, $v0, $zero
    /* 6370 80005770 2E610011 */  sltiu      $at, $s3, 0x11
    /* 6374 80005774 1420002C */  bnez       $at, .L80005828
    /* 6378 80005778 00000000 */   nop
  .L8000577C:
    /* 637C 8000577C 24010063 */  addiu      $at, $zero, 0x63
    /* 6380 80005780 16610003 */  bne        $s3, $at, .L80005790
    /* 6384 80005784 00000000 */   nop
    /* 6388 80005788 10000068 */  b          .L8000592C
    /* 638C 8000578C 24020001 */   addiu     $v0, $zero, 0x1
  .L80005790:
    /* 6390 80005790 92A20001 */  lbu        $v0, 0x1($s5)
    /* 6394 80005794 2673FFF0 */  addiu      $s3, $s3, -0x10
    /* 6398 80005798 02028023 */  subu       $s0, $s0, $v0
    /* 639C 8000579C 0213082B */  sltu       $at, $s0, $s3
    /* 63A0 800057A0 10200016 */  beqz       $at, .L800057FC
    /* 63A4 800057A4 00529006 */   srlv      $s2, $s2, $v0
  .L800057A8:
    /* 63A8 800057A8 8E220000 */  lw         $v0, 0x0($s1)
    /* 63AC 800057AC 8E8D0000 */  lw         $t5, 0x0($s4)
    /* 63B0 800057B0 004D082B */  sltu       $at, $v0, $t5
    /* 63B4 800057B4 10200009 */  beqz       $at, .L800057DC
    /* 63B8 800057B8 00000000 */   nop
    /* 63BC 800057BC 244E0002 */  addiu      $t6, $v0, 0x2
    /* 63C0 800057C0 AE2E0000 */  sw         $t6, 0x0($s1)
    /* 63C4 800057C4 8E220000 */  lw         $v0, 0x0($s1)
    /* 63C8 800057C8 904FFFFF */  lbu        $t7, -0x1($v0)
    /* 63CC 800057CC 9059FFFE */  lbu        $t9, -0x2($v0)
    /* 63D0 800057D0 000FC200 */  sll        $t8, $t7, 8
    /* 63D4 800057D4 10000004 */  b          .L800057E8
    /* 63D8 800057D8 03191825 */   or        $v1, $t8, $t9
  .L800057DC:
    /* 63DC 800057DC 0C0019B5 */  jal        func_800066D4
    /* 63E0 800057E0 00000000 */   nop
    /* 63E4 800057E4 00401825 */  or         $v1, $v0, $zero
  .L800057E8:
    /* 63E8 800057E8 02034004 */  sllv       $t0, $v1, $s0
    /* 63EC 800057EC 26100010 */  addiu      $s0, $s0, 0x10
    /* 63F0 800057F0 0213082B */  sltu       $at, $s0, $s3
    /* 63F4 800057F4 1420FFEC */  bnez       $at, .L800057A8
    /* 63F8 800057F8 02489025 */   or        $s2, $s2, $t0
  .L800057FC:
    /* 63FC 800057FC 00134840 */  sll        $t1, $s3, 1
    /* 6400 80005800 03C95021 */  addu       $t2, $fp, $t1
    /* 6404 80005804 954B0000 */  lhu        $t3, 0x0($t2)
    /* 6408 80005808 8EAE0004 */  lw         $t6, 0x4($s5)
    /* 640C 8000580C 01726024 */  and        $t4, $t3, $s2
    /* 6410 80005810 000C68C0 */  sll        $t5, $t4, 3
    /* 6414 80005814 01AEA821 */  addu       $s5, $t5, $t6
    /* 6418 80005818 92B30000 */  lbu        $s3, 0x0($s5)
    /* 641C 8000581C 2E610011 */  sltiu      $at, $s3, 0x11
    /* 6420 80005820 1020FFD6 */  beqz       $at, .L8000577C
    /* 6424 80005824 00000000 */   nop
  .L80005828:
    /* 6428 80005828 92A20001 */  lbu        $v0, 0x1($s5)
    /* 642C 8000582C 02028023 */  subu       $s0, $s0, $v0
    /* 6430 80005830 0213082B */  sltu       $at, $s0, $s3
    /* 6434 80005834 10200016 */  beqz       $at, .L80005890
    /* 6438 80005838 00529006 */   srlv      $s2, $s2, $v0
  .L8000583C:
    /* 643C 8000583C 8E220000 */  lw         $v0, 0x0($s1)
    /* 6440 80005840 8E8F0000 */  lw         $t7, 0x0($s4)
    /* 6444 80005844 004F082B */  sltu       $at, $v0, $t7
    /* 6448 80005848 10200009 */  beqz       $at, .L80005870
    /* 644C 8000584C 00000000 */   nop
    /* 6450 80005850 24580002 */  addiu      $t8, $v0, 0x2
    /* 6454 80005854 AE380000 */  sw         $t8, 0x0($s1)
    /* 6458 80005858 8E220000 */  lw         $v0, 0x0($s1)
    /* 645C 8000585C 9059FFFF */  lbu        $t9, -0x1($v0)
    /* 6460 80005860 9049FFFE */  lbu        $t1, -0x2($v0)
    /* 6464 80005864 00194200 */  sll        $t0, $t9, 8
    /* 6468 80005868 10000004 */  b          .L8000587C
    /* 646C 8000586C 01091825 */   or        $v1, $t0, $t1
  .L80005870:
    /* 6470 80005870 0C0019B5 */  jal        func_800066D4
    /* 6474 80005874 00000000 */   nop
    /* 6478 80005878 00401825 */  or         $v1, $v0, $zero
  .L8000587C:
    /* 647C 8000587C 02035004 */  sllv       $t2, $v1, $s0
    /* 6480 80005880 26100010 */  addiu      $s0, $s0, 0x10
    /* 6484 80005884 0213082B */  sltu       $at, $s0, $s3
    /* 6488 80005888 1420FFEC */  bnez       $at, .L8000583C
    /* 648C 8000588C 024A9025 */   or        $s2, $s2, $t2
  .L80005890:
    /* 6490 80005890 00136840 */  sll        $t5, $s3, 1
    /* 6494 80005894 03CD7021 */  addu       $t6, $fp, $t5
    /* 6498 80005898 95CF0000 */  lhu        $t7, 0x0($t6)
    /* 649C 8000589C 96AB0004 */  lhu        $t3, 0x4($s5)
    /* 64A0 800058A0 02138023 */  subu       $s0, $s0, $s3
    /* 64A4 800058A4 01F2C024 */  and        $t8, $t7, $s2
    /* 64A8 800058A8 000B6023 */  negu       $t4, $t3
    /* 64AC 800058AC 01981023 */  subu       $v0, $t4, $t8
    /* 64B0 800058B0 2841FFF8 */  slti       $at, $v0, -0x8
    /* 64B4 800058B4 10200009 */  beqz       $at, .L800058DC
    /* 64B8 800058B8 02729006 */   srlv      $s2, $s2, $s3
    /* 64BC 800058BC 8EE40000 */  lw         $a0, 0x0($s7)
    /* 64C0 800058C0 02C03025 */  or         $a2, $s6, $zero
    /* 64C4 800058C4 0C001F1A */  jal        func_80007C68
    /* 64C8 800058C8 00822821 */   addu      $a1, $a0, $v0
    /* 64CC 800058CC 8EF90000 */  lw         $t9, 0x0($s7)
    /* 64D0 800058D0 03364021 */  addu       $t0, $t9, $s6
    /* 64D4 800058D4 1000FF08 */  b          .L800054F8
    /* 64D8 800058D8 AEE80000 */   sw        $t0, 0x0($s7)
  .L800058DC:
    /* 64DC 800058DC 02C01825 */  or         $v1, $s6, $zero
    /* 64E0 800058E0 1060FF05 */  beqz       $v1, .L800054F8
    /* 64E4 800058E4 26D6FFFF */   addiu     $s6, $s6, -0x1
  .L800058E8:
    /* 64E8 800058E8 8EE40000 */  lw         $a0, 0x0($s7)
    /* 64EC 800058EC 02C01825 */  or         $v1, $s6, $zero
    /* 64F0 800058F0 26D6FFFF */  addiu      $s6, $s6, -0x1
    /* 64F4 800058F4 00824821 */  addu       $t1, $a0, $v0
    /* 64F8 800058F8 912A0000 */  lbu        $t2, 0x0($t1)
    /* 64FC 800058FC A08A0000 */  sb         $t2, 0x0($a0)
    /* 6500 80005900 8EEB0000 */  lw         $t3, 0x0($s7)
    /* 6504 80005904 256D0001 */  addiu      $t5, $t3, 0x1
    /* 6508 80005908 1460FFF7 */  bnez       $v1, .L800058E8
    /* 650C 8000590C AEED0000 */   sw        $t5, 0x0($s7)
    /* 6510 80005910 1000FEF9 */  b          .L800054F8
    /* 6514 80005914 00000000 */   nop
  .L80005918:
    /* 6518 80005918 3C018003 */  lui        $at, %hi(D_800354BC)
    /* 651C 8000591C AC3254BC */  sw         $s2, %lo(D_800354BC)($at)
    /* 6520 80005920 3C018003 */  lui        $at, %hi(D_800354C0)
    /* 6524 80005924 AC3054C0 */  sw         $s0, %lo(D_800354C0)($at)
    /* 6528 80005928 00001025 */  or         $v0, $zero, $zero
  .L8000592C:
    /* 652C 8000592C 8FBF003C */  lw         $ra, 0x3C($sp)
    /* 6530 80005930 8FB00018 */  lw         $s0, 0x18($sp)
    /* 6534 80005934 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 6538 80005938 8FB20020 */  lw         $s2, 0x20($sp)
    /* 653C 8000593C 8FB30024 */  lw         $s3, 0x24($sp)
    /* 6540 80005940 8FB40028 */  lw         $s4, 0x28($sp)
    /* 6544 80005944 8FB5002C */  lw         $s5, 0x2C($sp)
    /* 6548 80005948 8FB60030 */  lw         $s6, 0x30($sp)
    /* 654C 8000594C 8FB70034 */  lw         $s7, 0x34($sp)
    /* 6550 80005950 8FBE0038 */  lw         $fp, 0x38($sp)
    /* 6554 80005954 03E00008 */  jr         $ra
    /* 6558 80005958 27BD0060 */   addiu     $sp, $sp, 0x60
```

## Tips for This Category

