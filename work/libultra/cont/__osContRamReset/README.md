# __osContRamReset

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80009658` |
| **Category** | `libultra/cont` |
| **Status** | `TODO` |
| **Instructions** | ~53 |

## Description

reset controller pak RAM

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/cont` category.

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
./work/libultra/cont/__osContRamReset/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/cont/__osContRamReset

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/cont/__osContRamReset --watch
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
# Source: A050.s
# Address: 0x80009658

glabel func_80009658
    /* A258 80009658 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* A25C 8000965C AFA40018 */  sw         $a0, 0x18($sp)
    /* A260 80009660 308E00FF */  andi       $t6, $a0, 0xFF
    /* A264 80009664 3C038003 */  lui        $v1, %hi(D_80037AA0)
    /* A268 80009668 3C028003 */  lui        $v0, %hi(D_80037ADC)
    /* A26C 8000966C 01C02025 */  or         $a0, $t6, $zero
    /* A270 80009670 24427ADC */  addiu      $v0, $v0, %lo(D_80037ADC)
    /* A274 80009674 24637AA0 */  addiu      $v1, $v1, %lo(D_80037AA0)
  .L80009678:
    /* A278 80009678 24630004 */  addiu      $v1, $v1, 0x4
    /* A27C 8000967C 0062082B */  sltu       $at, $v1, $v0
    /* A280 80009680 1420FFFD */  bnez       $at, .L80009678
    /* A284 80009684 AC60FFFC */   sw        $zero, -0x4($v1)
    /* A288 80009688 3C058003 */  lui        $a1, %hi(D_80037AA0)
    /* A28C 8000968C 24A37AA0 */  addiu      $v1, $a1, %lo(D_80037AA0)
    /* A290 80009690 240F0001 */  addiu      $t7, $zero, 0x1
    /* A294 80009694 AC6F003C */  sw         $t7, 0x3C($v1)
    /* A298 80009698 3C068003 */  lui        $a2, %hi(D_80037AE1)
    /* A29C 8000969C 24C67AE1 */  addiu      $a2, $a2, %lo(D_80037AE1)
    /* A2A0 800096A0 90CD0000 */  lbu        $t5, 0x0($a2)
    /* A2A4 800096A4 241800FF */  addiu      $t8, $zero, 0xFF
    /* A2A8 800096A8 24190001 */  addiu      $t9, $zero, 0x1
    /* A2AC 800096AC 24080003 */  addiu      $t0, $zero, 0x3
    /* A2B0 800096B0 240900FF */  addiu      $t1, $zero, 0xFF
    /* A2B4 800096B4 240A00FF */  addiu      $t2, $zero, 0xFF
    /* A2B8 800096B8 240B00FF */  addiu      $t3, $zero, 0xFF
    /* A2BC 800096BC 240C00FF */  addiu      $t4, $zero, 0xFF
    /* A2C0 800096C0 A3B8000C */  sb         $t8, 0xC($sp)
    /* A2C4 800096C4 A3B9000D */  sb         $t9, 0xD($sp)
    /* A2C8 800096C8 A3A8000E */  sb         $t0, 0xE($sp)
    /* A2CC 800096CC A3A4000F */  sb         $a0, 0xF($sp)
    /* A2D0 800096D0 A3A90010 */  sb         $t1, 0x10($sp)
    /* A2D4 800096D4 A3AA0011 */  sb         $t2, 0x11($sp)
    /* A2D8 800096D8 A3AB0012 */  sb         $t3, 0x12($sp)
    /* A2DC 800096DC A3AC0013 */  sb         $t4, 0x13($sp)
    /* A2E0 800096E0 19A0000E */  blez       $t5, .L8000971C
    /* A2E4 800096E4 00001025 */   or        $v0, $zero, $zero
    /* A2E8 800096E8 27A4000C */  addiu      $a0, $sp, 0xC
    /* A2EC 800096EC 8C810000 */  lw         $at, 0x0($a0)
  .L800096F0:
    /* A2F0 800096F0 24420001 */  addiu      $v0, $v0, 0x1
    /* A2F4 800096F4 24630008 */  addiu      $v1, $v1, 0x8
    /* A2F8 800096F8 A861FFF8 */  swl        $at, -0x8($v1)
    /* A2FC 800096FC B861FFFB */  swr        $at, -0x5($v1)
    /* A300 80009700 8C8F0004 */  lw         $t7, 0x4($a0)
    /* A304 80009704 A86FFFFC */  swl        $t7, -0x4($v1)
    /* A308 80009708 B86FFFFF */  swr        $t7, -0x1($v1)
    /* A30C 8000970C 90D80000 */  lbu        $t8, 0x0($a2)
    /* A310 80009710 0058082A */  slt        $at, $v0, $t8
    /* A314 80009714 5420FFF6 */  bnel       $at, $zero, .L800096F0
    /* A318 80009718 8C810000 */   lw        $at, 0x0($a0)
  .L8000971C:
    /* A31C 8000971C 241900FE */  addiu      $t9, $zero, 0xFE
    /* A320 80009720 A0790000 */  sb         $t9, 0x0($v1)
    /* A324 80009724 03E00008 */  jr         $ra
    /* A328 80009728 27BD0018 */   addiu     $sp, $sp, 0x18
```

## Tips for This Category

