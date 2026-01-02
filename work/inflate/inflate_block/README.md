# inflate_block

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800064D4` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~105 |

## Description

decode one deflate block

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
./work/inflate/inflate_block/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/inflate_block

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/inflate_block --watch
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
# Address: 0x800064D4

glabel func_800064D4
    /* 70D4 800064D4 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 70D8 800064D8 AFB00014 */  sw         $s0, 0x14($sp)
    /* 70DC 800064DC 3C108003 */  lui        $s0, %hi(D_800354C0)
    /* 70E0 800064E0 8E1054C0 */  lw         $s0, %lo(D_800354C0)($s0)
    /* 70E4 800064E4 AFB2001C */  sw         $s2, 0x1C($sp)
    /* 70E8 800064E8 3C128003 */  lui        $s2, %hi(D_800354BC)
    /* 70EC 800064EC 8E5254BC */  lw         $s2, %lo(D_800354BC)($s2)
    /* 70F0 800064F0 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 70F4 800064F4 AFB30020 */  sw         $s3, 0x20($sp)
    /* 70F8 800064F8 AFB10018 */  sw         $s1, 0x18($sp)
    /* 70FC 800064FC 16000019 */  bnez       $s0, .L80006564
    /* 7100 80006500 AFA40028 */   sw        $a0, 0x28($sp)
    /* 7104 80006504 3C138003 */  lui        $s3, %hi(D_800354B4)
    /* 7108 80006508 3C118003 */  lui        $s1, %hi(D_800354B0)
    /* 710C 8000650C 263154B0 */  addiu      $s1, $s1, %lo(D_800354B0)
    /* 7110 80006510 267354B4 */  addiu      $s3, $s3, %lo(D_800354B4)
  .L80006514:
    /* 7114 80006514 8E220000 */  lw         $v0, 0x0($s1)
    /* 7118 80006518 8E6E0000 */  lw         $t6, 0x0($s3)
    /* 711C 8000651C 004E082B */  sltu       $at, $v0, $t6
    /* 7120 80006520 10200009 */  beqz       $at, .L80006548
    /* 7124 80006524 00000000 */   nop
    /* 7128 80006528 244F0002 */  addiu      $t7, $v0, 0x2
    /* 712C 8000652C AE2F0000 */  sw         $t7, 0x0($s1)
    /* 7130 80006530 8E220000 */  lw         $v0, 0x0($s1)
    /* 7134 80006534 9058FFFF */  lbu        $t8, -0x1($v0)
    /* 7138 80006538 9048FFFE */  lbu        $t0, -0x2($v0)
    /* 713C 8000653C 0018CA00 */  sll        $t9, $t8, 8
    /* 7140 80006540 10000004 */  b          .L80006554
    /* 7144 80006544 03281825 */   or        $v1, $t9, $t0
  .L80006548:
    /* 7148 80006548 0C0019B5 */  jal        func_800066D4
    /* 714C 8000654C 00000000 */   nop
    /* 7150 80006550 00401825 */  or         $v1, $v0, $zero
  .L80006554:
    /* 7154 80006554 02034804 */  sllv       $t1, $v1, $s0
    /* 7158 80006558 26100010 */  addiu      $s0, $s0, 0x10
    /* 715C 8000655C 1200FFED */  beqz       $s0, .L80006514
    /* 7160 80006560 02499025 */   or        $s2, $s2, $t1
  .L80006564:
    /* 7164 80006564 8FAB0028 */  lw         $t3, 0x28($sp)
    /* 7168 80006568 2610FFFF */  addiu      $s0, $s0, -0x1
    /* 716C 8000656C 2E010002 */  sltiu      $at, $s0, 0x2
    /* 7170 80006570 3C118003 */  lui        $s1, %hi(D_800354B0)
    /* 7174 80006574 3C138003 */  lui        $s3, %hi(D_800354B4)
    /* 7178 80006578 324A0001 */  andi       $t2, $s2, 0x1
    /* 717C 8000657C 00126042 */  srl        $t4, $s2, 1
    /* 7180 80006580 01809025 */  or         $s2, $t4, $zero
    /* 7184 80006584 267354B4 */  addiu      $s3, $s3, %lo(D_800354B4)
    /* 7188 80006588 263154B0 */  addiu      $s1, $s1, %lo(D_800354B0)
    /* 718C 8000658C 10200016 */  beqz       $at, .L800065E8
    /* 7190 80006590 AD6A0000 */   sw        $t2, 0x0($t3)
  .L80006594:
    /* 7194 80006594 8E220000 */  lw         $v0, 0x0($s1)
    /* 7198 80006598 8E6D0000 */  lw         $t5, 0x0($s3)
    /* 719C 8000659C 004D082B */  sltu       $at, $v0, $t5
    /* 71A0 800065A0 10200009 */  beqz       $at, .L800065C8
    /* 71A4 800065A4 00000000 */   nop
    /* 71A8 800065A8 244E0002 */  addiu      $t6, $v0, 0x2
    /* 71AC 800065AC AE2E0000 */  sw         $t6, 0x0($s1)
    /* 71B0 800065B0 8E220000 */  lw         $v0, 0x0($s1)
    /* 71B4 800065B4 904FFFFF */  lbu        $t7, -0x1($v0)
    /* 71B8 800065B8 9059FFFE */  lbu        $t9, -0x2($v0)
    /* 71BC 800065BC 000FC200 */  sll        $t8, $t7, 8
    /* 71C0 800065C0 10000004 */  b          .L800065D4
    /* 71C4 800065C4 03191825 */   or        $v1, $t8, $t9
  .L800065C8:
    /* 71C8 800065C8 0C0019B5 */  jal        func_800066D4
    /* 71CC 800065CC 00000000 */   nop
    /* 71D0 800065D0 00401825 */  or         $v1, $v0, $zero
  .L800065D4:
    /* 71D4 800065D4 02034004 */  sllv       $t0, $v1, $s0
    /* 71D8 800065D8 26100010 */  addiu      $s0, $s0, 0x10
    /* 71DC 800065DC 2E010002 */  sltiu      $at, $s0, 0x2
    /* 71E0 800065E0 1420FFEC */  bnez       $at, .L80006594
    /* 71E4 800065E4 02489025 */   or        $s2, $s2, $t0
  .L800065E8:
    /* 71E8 800065E8 32420003 */  andi       $v0, $s2, 0x3
    /* 71EC 800065EC 00124882 */  srl        $t1, $s2, 2
    /* 71F0 800065F0 01209025 */  or         $s2, $t1, $zero
    /* 71F4 800065F4 3C018003 */  lui        $at, %hi(D_800354BC)
    /* 71F8 800065F8 AC3254BC */  sw         $s2, %lo(D_800354BC)($at)
    /* 71FC 800065FC 3C018003 */  lui        $at, %hi(D_800354C0)
    /* 7200 80006600 2610FFFE */  addiu      $s0, $s0, -0x2
    /* 7204 80006604 AC3054C0 */  sw         $s0, %lo(D_800354C0)($at)
    /* 7208 80006608 24010002 */  addiu      $at, $zero, 0x2
    /* 720C 8000660C 14410005 */  bne        $v0, $at, .L80006624
    /* 7210 80006610 00000000 */   nop
    /* 7214 80006614 0C001751 */  jal        func_80005D44
    /* 7218 80006618 00000000 */   nop
    /* 721C 8000661C 1000000F */  b          .L8000665C
    /* 7220 80006620 00000000 */   nop
  .L80006624:
    /* 7224 80006624 14400005 */  bnez       $v0, .L8000663C
    /* 7228 80006628 00000000 */   nop
    /* 722C 8000662C 0C001657 */  jal        func_8000595C
    /* 7230 80006630 00000000 */   nop
    /* 7234 80006634 10000009 */  b          .L8000665C
    /* 7238 80006638 00000000 */   nop
  .L8000663C:
    /* 723C 8000663C 24010001 */  addiu      $at, $zero, 0x1
    /* 7240 80006640 14410005 */  bne        $v0, $at, .L80006658
    /* 7244 80006644 00000000 */   nop
    /* 7248 80006648 0C0016DF */  jal        func_80005B7C
    /* 724C 8000664C 00000000 */   nop
    /* 7250 80006650 10000002 */  b          .L8000665C
    /* 7254 80006654 00000000 */   nop
  .L80006658:
    /* 7258 80006658 24020001 */  addiu      $v0, $zero, 0x1
  .L8000665C:
    /* 725C 8000665C 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 7260 80006660 8FB00014 */  lw         $s0, 0x14($sp)
    /* 7264 80006664 8FB10018 */  lw         $s1, 0x18($sp)
    /* 7268 80006668 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 726C 8000666C 8FB30020 */  lw         $s3, 0x20($sp)
    /* 7270 80006670 03E00008 */  jr         $ra
    /* 7274 80006674 27BD0028 */   addiu     $sp, $sp, 0x28
```

## Tips for This Category

