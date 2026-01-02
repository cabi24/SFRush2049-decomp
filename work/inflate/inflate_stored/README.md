# inflate_stored

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000595C` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~136 |

## Description

handle uncompressed block (type 0)

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
./work/inflate/inflate_stored/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/inflate_stored

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/inflate_stored --watch
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
# Address: 0x8000595C

glabel func_8000595C
    /* 655C 8000595C 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 6560 80005960 AFB00014 */  sw         $s0, 0x14($sp)
    /* 6564 80005964 3C108003 */  lui        $s0, %hi(D_800354C0)
    /* 6568 80005968 8E1054C0 */  lw         $s0, %lo(D_800354C0)($s0)
    /* 656C 8000596C AFB2001C */  sw         $s2, 0x1C($sp)
    /* 6570 80005970 3C128003 */  lui        $s2, %hi(D_800354BC)
    /* 6574 80005974 32020007 */  andi       $v0, $s0, 0x7
    /* 6578 80005978 8E5254BC */  lw         $s2, %lo(D_800354BC)($s2)
    /* 657C 8000597C 02028023 */  subu       $s0, $s0, $v0
    /* 6580 80005980 2E010010 */  sltiu      $at, $s0, 0x10
    /* 6584 80005984 AFBF002C */  sw         $ra, 0x2C($sp)
    /* 6588 80005988 AFB50028 */  sw         $s5, 0x28($sp)
    /* 658C 8000598C AFB40024 */  sw         $s4, 0x24($sp)
    /* 6590 80005990 AFB30020 */  sw         $s3, 0x20($sp)
    /* 6594 80005994 AFB10018 */  sw         $s1, 0x18($sp)
    /* 6598 80005998 1020001A */  beqz       $at, .L80005A04
    /* 659C 8000599C 00529006 */   srlv      $s2, $s2, $v0
    /* 65A0 800059A0 3C138003 */  lui        $s3, %hi(D_800354B4)
    /* 65A4 800059A4 3C118003 */  lui        $s1, %hi(D_800354B0)
    /* 65A8 800059A8 263154B0 */  addiu      $s1, $s1, %lo(D_800354B0)
    /* 65AC 800059AC 267354B4 */  addiu      $s3, $s3, %lo(D_800354B4)
  .L800059B0:
    /* 65B0 800059B0 8E220000 */  lw         $v0, 0x0($s1)
    /* 65B4 800059B4 8E6E0000 */  lw         $t6, 0x0($s3)
    /* 65B8 800059B8 004E082B */  sltu       $at, $v0, $t6
    /* 65BC 800059BC 10200009 */  beqz       $at, .L800059E4
    /* 65C0 800059C0 00000000 */   nop
    /* 65C4 800059C4 244F0002 */  addiu      $t7, $v0, 0x2
    /* 65C8 800059C8 AE2F0000 */  sw         $t7, 0x0($s1)
    /* 65CC 800059CC 8E220000 */  lw         $v0, 0x0($s1)
    /* 65D0 800059D0 9058FFFF */  lbu        $t8, -0x1($v0)
    /* 65D4 800059D4 9048FFFE */  lbu        $t0, -0x2($v0)
    /* 65D8 800059D8 0018CA00 */  sll        $t9, $t8, 8
    /* 65DC 800059DC 10000004 */  b          .L800059F0
    /* 65E0 800059E0 03281825 */   or        $v1, $t9, $t0
  .L800059E4:
    /* 65E4 800059E4 0C0019B5 */  jal        func_800066D4
    /* 65E8 800059E8 00000000 */   nop
    /* 65EC 800059EC 00401825 */  or         $v1, $v0, $zero
  .L800059F0:
    /* 65F0 800059F0 02034804 */  sllv       $t1, $v1, $s0
    /* 65F4 800059F4 26100010 */  addiu      $s0, $s0, 0x10
    /* 65F8 800059F8 2E010010 */  sltiu      $at, $s0, 0x10
    /* 65FC 800059FC 1420FFEC */  bnez       $at, .L800059B0
    /* 6600 80005A00 02499025 */   or        $s2, $s2, $t1
  .L80005A04:
    /* 6604 80005A04 2610FFF0 */  addiu      $s0, $s0, -0x10
    /* 6608 80005A08 2E010010 */  sltiu      $at, $s0, 0x10
    /* 660C 80005A0C 3C118003 */  lui        $s1, %hi(D_800354B0)
    /* 6610 80005A10 3C138003 */  lui        $s3, %hi(D_800354B4)
    /* 6614 80005A14 3255FFFF */  andi       $s5, $s2, 0xFFFF
    /* 6618 80005A18 00125402 */  srl        $t2, $s2, 16
    /* 661C 80005A1C 01409025 */  or         $s2, $t2, $zero
    /* 6620 80005A20 267354B4 */  addiu      $s3, $s3, %lo(D_800354B4)
    /* 6624 80005A24 10200016 */  beqz       $at, .L80005A80
    /* 6628 80005A28 263154B0 */   addiu     $s1, $s1, %lo(D_800354B0)
  .L80005A2C:
    /* 662C 80005A2C 8E220000 */  lw         $v0, 0x0($s1)
    /* 6630 80005A30 8E6B0000 */  lw         $t3, 0x0($s3)
    /* 6634 80005A34 004B082B */  sltu       $at, $v0, $t3
    /* 6638 80005A38 10200009 */  beqz       $at, .L80005A60
    /* 663C 80005A3C 00000000 */   nop
    /* 6640 80005A40 244C0002 */  addiu      $t4, $v0, 0x2
    /* 6644 80005A44 AE2C0000 */  sw         $t4, 0x0($s1)
    /* 6648 80005A48 8E220000 */  lw         $v0, 0x0($s1)
    /* 664C 80005A4C 904DFFFF */  lbu        $t5, -0x1($v0)
    /* 6650 80005A50 904FFFFE */  lbu        $t7, -0x2($v0)
    /* 6654 80005A54 000D7200 */  sll        $t6, $t5, 8
    /* 6658 80005A58 10000004 */  b          .L80005A6C
    /* 665C 80005A5C 01CF1825 */   or        $v1, $t6, $t7
  .L80005A60:
    /* 6660 80005A60 0C0019B5 */  jal        func_800066D4
    /* 6664 80005A64 00000000 */   nop
    /* 6668 80005A68 00401825 */  or         $v1, $v0, $zero
  .L80005A6C:
    /* 666C 80005A6C 0203C004 */  sllv       $t8, $v1, $s0
    /* 6670 80005A70 26100010 */  addiu      $s0, $s0, 0x10
    /* 6674 80005A74 2E010010 */  sltiu      $at, $s0, 0x10
    /* 6678 80005A78 1420FFEC */  bnez       $at, .L80005A2C
    /* 667C 80005A7C 02589025 */   or        $s2, $s2, $t8
  .L80005A80:
    /* 6680 80005A80 0240C827 */  not        $t9, $s2
    /* 6684 80005A84 3328FFFF */  andi       $t0, $t9, 0xFFFF
    /* 6688 80005A88 12A80003 */  beq        $s5, $t0, .L80005A98
    /* 668C 80005A8C 00000000 */   nop
    /* 6690 80005A90 10000031 */  b          .L80005B58
    /* 6694 80005A94 24020001 */   addiu     $v0, $zero, 0x1
  .L80005A98:
    /* 6698 80005A98 00124C02 */  srl        $t1, $s2, 16
    /* 669C 80005A9C 02A01825 */  or         $v1, $s5, $zero
    /* 66A0 80005AA0 26B5FFFF */  addiu      $s5, $s5, -0x1
    /* 66A4 80005AA4 01209025 */  or         $s2, $t1, $zero
    /* 66A8 80005AA8 10600026 */  beqz       $v1, .L80005B44
    /* 66AC 80005AAC 2610FFF0 */   addiu     $s0, $s0, -0x10
    /* 66B0 80005AB0 3C148003 */  lui        $s4, %hi(D_800354B8)
    /* 66B4 80005AB4 269454B8 */  addiu      $s4, $s4, %lo(D_800354B8)
  .L80005AB8:
    /* 66B8 80005AB8 2E010008 */  sltiu      $at, $s0, 0x8
    /* 66BC 80005ABC 10200016 */  beqz       $at, .L80005B18
    /* 66C0 80005AC0 00000000 */   nop
  .L80005AC4:
    /* 66C4 80005AC4 8E220000 */  lw         $v0, 0x0($s1)
    /* 66C8 80005AC8 8E6A0000 */  lw         $t2, 0x0($s3)
    /* 66CC 80005ACC 004A082B */  sltu       $at, $v0, $t2
    /* 66D0 80005AD0 10200009 */  beqz       $at, .L80005AF8
    /* 66D4 80005AD4 00000000 */   nop
    /* 66D8 80005AD8 244B0002 */  addiu      $t3, $v0, 0x2
    /* 66DC 80005ADC AE2B0000 */  sw         $t3, 0x0($s1)
    /* 66E0 80005AE0 8E220000 */  lw         $v0, 0x0($s1)
    /* 66E4 80005AE4 904CFFFF */  lbu        $t4, -0x1($v0)
    /* 66E8 80005AE8 904EFFFE */  lbu        $t6, -0x2($v0)
    /* 66EC 80005AEC 000C6A00 */  sll        $t5, $t4, 8
    /* 66F0 80005AF0 10000004 */  b          .L80005B04
    /* 66F4 80005AF4 01AE1825 */   or        $v1, $t5, $t6
  .L80005AF8:
    /* 66F8 80005AF8 0C0019B5 */  jal        func_800066D4
    /* 66FC 80005AFC 00000000 */   nop
    /* 6700 80005B00 00401825 */  or         $v1, $v0, $zero
  .L80005B04:
    /* 6704 80005B04 02037804 */  sllv       $t7, $v1, $s0
    /* 6708 80005B08 26100010 */  addiu      $s0, $s0, 0x10
    /* 670C 80005B0C 2E010008 */  sltiu      $at, $s0, 0x8
    /* 6710 80005B10 1420FFEC */  bnez       $at, .L80005AC4
    /* 6714 80005B14 024F9025 */   or        $s2, $s2, $t7
  .L80005B18:
    /* 6718 80005B18 8E980000 */  lw         $t8, 0x0($s4)
    /* 671C 80005B1C 00124A02 */  srl        $t1, $s2, 8
    /* 6720 80005B20 02A01825 */  or         $v1, $s5, $zero
    /* 6724 80005B24 A3120000 */  sb         $s2, 0x0($t8)
    /* 6728 80005B28 8E990000 */  lw         $t9, 0x0($s4)
    /* 672C 80005B2C 01209025 */  or         $s2, $t1, $zero
    /* 6730 80005B30 26B5FFFF */  addiu      $s5, $s5, -0x1
    /* 6734 80005B34 27280001 */  addiu      $t0, $t9, 0x1
    /* 6738 80005B38 AE880000 */  sw         $t0, 0x0($s4)
    /* 673C 80005B3C 1460FFDE */  bnez       $v1, .L80005AB8
    /* 6740 80005B40 2610FFF8 */   addiu     $s0, $s0, -0x8
  .L80005B44:
    /* 6744 80005B44 3C018003 */  lui        $at, %hi(D_800354BC)
    /* 6748 80005B48 AC3254BC */  sw         $s2, %lo(D_800354BC)($at)
    /* 674C 80005B4C 3C018003 */  lui        $at, %hi(D_800354C0)
    /* 6750 80005B50 AC3054C0 */  sw         $s0, %lo(D_800354C0)($at)
    /* 6754 80005B54 00001025 */  or         $v0, $zero, $zero
  .L80005B58:
    /* 6758 80005B58 8FBF002C */  lw         $ra, 0x2C($sp)
    /* 675C 80005B5C 8FB00014 */  lw         $s0, 0x14($sp)
    /* 6760 80005B60 8FB10018 */  lw         $s1, 0x18($sp)
    /* 6764 80005B64 8FB2001C */  lw         $s2, 0x1C($sp)
    /* 6768 80005B68 8FB30020 */  lw         $s3, 0x20($sp)
    /* 676C 80005B6C 8FB40024 */  lw         $s4, 0x24($sp)
    /* 6770 80005B70 8FB50028 */  lw         $s5, 0x28($sp)
    /* 6774 80005B74 03E00008 */  jr         $ra
    /* 6778 80005B78 27BD0030 */   addiu     $sp, $sp, 0x30
```

## Tips for This Category

