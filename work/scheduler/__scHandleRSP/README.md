# __scHandleRSP

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80000950` |
| **Category** | `scheduler` |
| **Status** | `TODO` |
| **Instructions** | ~95 |

## Description

SP interrupt handler, finalizes RSP tasks

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `scheduler` category.

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
./work/scheduler/__scHandleRSP/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/scheduler/__scHandleRSP

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/scheduler/__scHandleRSP --watch
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
# Source: 1050.s
# Address: 0x80000950

glabel func_80000950
    /* 1550 80000950 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 1554 80000954 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1558 80000958 AFA40028 */  sw         $a0, 0x28($sp)
    /* 155C 8000095C AFA00020 */  sw         $zero, 0x20($sp)
    /* 1560 80000960 AFA0001C */  sw         $zero, 0x1C($sp)
    /* 1564 80000964 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 1568 80000968 8DCF0274 */  lw         $t7, 0x274($t6)
    /* 156C 8000096C 15E00001 */  bnez       $t7, .L80000974
    /* 1570 80000970 00000000 */   nop
  .L80000974:
    /* 1574 80000974 8FB80028 */  lw         $t8, 0x28($sp)
    /* 1578 80000978 8F190274 */  lw         $t9, 0x274($t8)
    /* 157C 8000097C AFB90024 */  sw         $t9, 0x24($sp)
    /* 1580 80000980 8FA80028 */  lw         $t0, 0x28($sp)
    /* 1584 80000984 AD000274 */  sw         $zero, 0x274($t0)
    /* 1588 80000988 8FA90024 */  lw         $t1, 0x24($sp)
    /* 158C 8000098C 8D2A0004 */  lw         $t2, 0x4($t1)
    /* 1590 80000990 314B0010 */  andi       $t3, $t2, 0x10
    /* 1594 80000994 1160001F */  beqz       $t3, .L80000A14
    /* 1598 80000998 00000000 */   nop
    /* 159C 8000099C 0C001DCC */  jal        func_80007730
    /* 15A0 800009A0 25240010 */   addiu     $a0, $t1, 0x10
    /* 15A4 800009A4 1040001B */  beqz       $v0, .L80000A14
    /* 15A8 800009A8 00000000 */   nop
    /* 15AC 800009AC 8FAC0024 */  lw         $t4, 0x24($sp)
    /* 15B0 800009B0 8D8D0004 */  lw         $t5, 0x4($t4)
    /* 15B4 800009B4 35AE0020 */  ori        $t6, $t5, 0x20
    /* 15B8 800009B8 AD8E0004 */  sw         $t6, 0x4($t4)
    /* 15BC 800009BC 8FAF0024 */  lw         $t7, 0x24($sp)
    /* 15C0 800009C0 24010003 */  addiu      $at, $zero, 0x3
    /* 15C4 800009C4 8DF80008 */  lw         $t8, 0x8($t7)
    /* 15C8 800009C8 33190007 */  andi       $t9, $t8, 0x7
    /* 15CC 800009CC 1721000F */  bne        $t9, $at, .L80000A0C
    /* 15D0 800009D0 00000000 */   nop
    /* 15D4 800009D4 8FA80028 */  lw         $t0, 0x28($sp)
    /* 15D8 800009D8 8FAB0024 */  lw         $t3, 0x24($sp)
    /* 15DC 800009DC 8D0A0268 */  lw         $t2, 0x268($t0)
    /* 15E0 800009E0 AD6A0000 */  sw         $t2, 0x0($t3)
    /* 15E4 800009E4 8FA90024 */  lw         $t1, 0x24($sp)
    /* 15E8 800009E8 8FAD0028 */  lw         $t5, 0x28($sp)
    /* 15EC 800009EC ADA90268 */  sw         $t1, 0x268($t5)
    /* 15F0 800009F0 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 15F4 800009F4 8DCC0270 */  lw         $t4, 0x270($t6)
    /* 15F8 800009F8 15800004 */  bnez       $t4, .L80000A0C
    /* 15FC 800009FC 00000000 */   nop
    /* 1600 80000A00 8FAF0024 */  lw         $t7, 0x24($sp)
    /* 1604 80000A04 8FB80028 */  lw         $t8, 0x28($sp)
    /* 1608 80000A08 AF0F0270 */  sw         $t7, 0x270($t8)
  .L80000A0C:
    /* 160C 80000A0C 10000015 */  b          .L80000A64
    /* 1610 80000A10 00000000 */   nop
  .L80000A14:
    /* 1614 80000A14 8FB90024 */  lw         $t9, 0x24($sp)
    /* 1618 80000A18 8F280008 */  lw         $t0, 0x8($t9)
    /* 161C 80000A1C 310A0040 */  andi       $t2, $t0, 0x40
    /* 1620 80000A20 11400003 */  beqz       $t2, .L80000A30
    /* 1624 80000A24 00000000 */   nop
    /* 1628 80000A28 10000006 */  b          .L80000A44
    /* 162C 80000A2C 00000000 */   nop
  .L80000A30:
    /* 1630 80000A30 8FAB0024 */  lw         $t3, 0x24($sp)
    /* 1634 80000A34 24010002 */  addiu      $at, $zero, 0x2
    /* 1638 80000A38 8D690010 */  lw         $t1, 0x10($t3)
    /* 163C 80000A3C 15210001 */  bne        $t1, $at, .L80000A44
    /* 1640 80000A40 00000000 */   nop
  .L80000A44:
    /* 1644 80000A44 8FAD0024 */  lw         $t5, 0x24($sp)
    /* 1648 80000A48 2401FFFD */  addiu      $at, $zero, -0x3
    /* 164C 80000A4C 8DAE0004 */  lw         $t6, 0x4($t5)
    /* 1650 80000A50 01C16024 */  and        $t4, $t6, $at
    /* 1654 80000A54 ADAC0004 */  sw         $t4, 0x4($t5)
    /* 1658 80000A58 8FA40028 */  lw         $a0, 0x28($sp)
    /* 165C 80000A5C 0C000315 */  jal        func_80000C54
    /* 1660 80000A60 8FA50024 */   lw        $a1, 0x24($sp)
  .L80000A64:
    /* 1664 80000A64 8FAF0028 */  lw         $t7, 0x28($sp)
    /* 1668 80000A68 8DF80274 */  lw         $t8, 0x274($t7)
    /* 166C 80000A6C 8DEA0278 */  lw         $t2, 0x278($t7)
    /* 1670 80000A70 2F190001 */  sltiu      $t9, $t8, 0x1
    /* 1674 80000A74 00194040 */  sll        $t0, $t9, 1
    /* 1678 80000A78 2D4B0001 */  sltiu      $t3, $t2, 0x1
    /* 167C 80000A7C 010B4825 */  or         $t1, $t0, $t3
    /* 1680 80000A80 AFA90018 */  sw         $t1, 0x18($sp)
    /* 1684 80000A84 8FA40028 */  lw         $a0, 0x28($sp)
    /* 1688 80000A88 27A50020 */  addiu      $a1, $sp, 0x20
    /* 168C 80000A8C 27A6001C */  addiu      $a2, $sp, 0x1C
    /* 1690 80000A90 0C0003FA */  jal        func_80000FE8
    /* 1694 80000A94 8FA70018 */   lw        $a3, 0x18($sp)
    /* 1698 80000A98 8FAE0018 */  lw         $t6, 0x18($sp)
    /* 169C 80000A9C 104E0005 */  beq        $v0, $t6, .L80000AB4
    /* 16A0 80000AA0 00000000 */   nop
    /* 16A4 80000AA4 8FA40028 */  lw         $a0, 0x28($sp)
    /* 16A8 80000AA8 8FA50020 */  lw         $a1, 0x20($sp)
    /* 16AC 80000AAC 0C000388 */  jal        func_80000E20
    /* 16B0 80000AB0 8FA6001C */   lw        $a2, 0x1C($sp)
  .L80000AB4:
    /* 16B4 80000AB4 10000001 */  b          .L80000ABC
    /* 16B8 80000AB8 00000000 */   nop
  .L80000ABC:
    /* 16BC 80000ABC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 16C0 80000AC0 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 16C4 80000AC4 03E00008 */  jr         $ra
    /* 16C8 80000AC8 00000000 */   nop
```

## Tips for This Category

