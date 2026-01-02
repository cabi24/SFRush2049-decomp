# __osPackReadData

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800098E0` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~50 |

## Description

pack read-buttons command into PIF buffer

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
./work/libultra/os/__osPackReadData/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/__osPackReadData

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/__osPackReadData --watch
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
# Source: A330.s
# Address: 0x800098E0

glabel func_800098E0
    /* A4E0 800098E0 3C058003 */  lui        $a1, %hi(D_80037AA0)
    /* A4E4 800098E4 24A57AA0 */  addiu      $a1, $a1, %lo(D_80037AA0)
    /* A4E8 800098E8 3C048003 */  lui        $a0, %hi(D_80037AA0)
    /* A4EC 800098EC 3C038003 */  lui        $v1, %hi(D_80037ADC)
    /* A4F0 800098F0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* A4F4 800098F4 00A01025 */  or         $v0, $a1, $zero
    /* A4F8 800098F8 24637ADC */  addiu      $v1, $v1, %lo(D_80037ADC)
    /* A4FC 800098FC 24847AA0 */  addiu      $a0, $a0, %lo(D_80037AA0)
  .L80009900:
    /* A500 80009900 24840004 */  addiu      $a0, $a0, 0x4
    /* A504 80009904 0083082B */  sltu       $at, $a0, $v1
    /* A508 80009908 1420FFFD */  bnez       $at, .L80009900
    /* A50C 8000990C AC80FFFC */   sw        $zero, -0x4($a0)
    /* A510 80009910 3C048003 */  lui        $a0, %hi(D_80037AE1)
    /* A514 80009914 24847AE1 */  addiu      $a0, $a0, %lo(D_80037AE1)
    /* A518 80009918 908C0000 */  lbu        $t4, 0x0($a0)
    /* A51C 8000991C 240E0001 */  addiu      $t6, $zero, 0x1
    /* A520 80009920 240F00FF */  addiu      $t7, $zero, 0xFF
    /* A524 80009924 24180001 */  addiu      $t8, $zero, 0x1
    /* A528 80009928 24190004 */  addiu      $t9, $zero, 0x4
    /* A52C 8000992C 24080001 */  addiu      $t0, $zero, 0x1
    /* A530 80009930 3409FFFF */  ori        $t1, $zero, 0xFFFF
    /* A534 80009934 240AFFFF */  addiu      $t2, $zero, -0x1
    /* A538 80009938 240BFFFF */  addiu      $t3, $zero, -0x1
    /* A53C 8000993C ACAE003C */  sw         $t6, 0x3C($a1)
    /* A540 80009940 A3AF000C */  sb         $t7, 0xC($sp)
    /* A544 80009944 A3B8000D */  sb         $t8, 0xD($sp)
    /* A548 80009948 A3B9000E */  sb         $t9, 0xE($sp)
    /* A54C 8000994C A3A8000F */  sb         $t0, 0xF($sp)
    /* A550 80009950 A7A90010 */  sh         $t1, 0x10($sp)
    /* A554 80009954 A3AA0012 */  sb         $t2, 0x12($sp)
    /* A558 80009958 A3AB0013 */  sb         $t3, 0x13($sp)
    /* A55C 8000995C 1980000E */  blez       $t4, .L80009998
    /* A560 80009960 00001825 */   or        $v1, $zero, $zero
    /* A564 80009964 27A5000C */  addiu      $a1, $sp, 0xC
    /* A568 80009968 8CA10000 */  lw         $at, 0x0($a1)
  .L8000996C:
    /* A56C 8000996C 24630001 */  addiu      $v1, $v1, 0x1
    /* A570 80009970 24420008 */  addiu      $v0, $v0, 0x8
    /* A574 80009974 A841FFF8 */  swl        $at, -0x8($v0)
    /* A578 80009978 B841FFFB */  swr        $at, -0x5($v0)
    /* A57C 8000997C 8CAE0004 */  lw         $t6, 0x4($a1)
    /* A580 80009980 A84EFFFC */  swl        $t6, -0x4($v0)
    /* A584 80009984 B84EFFFF */  swr        $t6, -0x1($v0)
    /* A588 80009988 908F0000 */  lbu        $t7, 0x0($a0)
    /* A58C 8000998C 006F082A */  slt        $at, $v1, $t7
    /* A590 80009990 5420FFF6 */  bnel       $at, $zero, .L8000996C
    /* A594 80009994 8CA10000 */   lw        $at, 0x0($a1)
  .L80009998:
    /* A598 80009998 241800FE */  addiu      $t8, $zero, 0xFE
    /* A59C 8000999C A0580000 */  sb         $t8, 0x0($v0)
    /* A5A0 800099A0 03E00008 */  jr         $ra
    /* A5A4 800099A4 27BD0018 */   addiu     $sp, $sp, 0x18
```

## Tips for This Category

