# guMtxL2F

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800092E0` |
| **Category** | `libultra/gu` |
| **Status** | `TODO` |
| **Instructions** | ~45 |

## Description

convert N64 fixed-point to float matrix

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/gu` category.

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
./work/libultra/gu/guMtxL2F/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/gu/guMtxL2F

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/gu/guMtxL2F --watch
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
# Source: 9DE0.s
# Address: 0x800092E0

glabel func_800092E0
    /* 9EE0 800092E0 3C013780 */  lui        $at, (0x37800000 >> 16)
    /* 9EE4 800092E4 44810000 */  mtc1       $at, $fv0
    /* 9EE8 800092E8 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* 9EEC 800092EC 00A01025 */  or         $v0, $a1, $zero
    /* 9EF0 800092F0 24A30020 */  addiu      $v1, $a1, 0x20
    /* 9EF4 800092F4 00003025 */  or         $a2, $zero, $zero
    /* 9EF8 800092F8 00804025 */  or         $t0, $a0, $zero
    /* 9EFC 800092FC 240C0004 */  addiu      $t4, $zero, 0x4
    /* 9F00 80009300 240B0002 */  addiu      $t3, $zero, 0x2
    /* 9F04 80009304 3C0AFFFF */  lui        $t2, (0xFFFF0000 >> 16)
  .L80009308:
    /* 9F08 80009308 00002025 */  or         $a0, $zero, $zero
    /* 9F0C 8000930C 01004825 */  or         $t1, $t0, $zero
  .L80009310:
    /* 9F10 80009310 8C6E0000 */  lw         $t6, 0x0($v1)
    /* 9F14 80009314 8C590000 */  lw         $t9, 0x0($v0)
    /* 9F18 80009318 24840001 */  addiu      $a0, $a0, 0x1
    /* 9F1C 8000931C 000E7C02 */  srl        $t7, $t6, 16
    /* 9F20 80009320 31F8FFFF */  andi       $t8, $t7, 0xFFFF
    /* 9F24 80009324 032A6824 */  and        $t5, $t9, $t2
    /* 9F28 80009328 030D7025 */  or         $t6, $t8, $t5
    /* 9F2C 8000932C AFAE0004 */  sw         $t6, 0x4($sp)
    /* 9F30 80009330 8C580000 */  lw         $t8, 0x0($v0)
    /* 9F34 80009334 8FA50004 */  lw         $a1, 0x4($sp)
    /* 9F38 80009338 8C6F0000 */  lw         $t7, 0x0($v1)
    /* 9F3C 8000933C 00186C00 */  sll        $t5, $t8, 16
    /* 9F40 80009340 44852000 */  mtc1       $a1, $ft0
    /* 9F44 80009344 01AA7024 */  and        $t6, $t5, $t2
    /* 9F48 80009348 31F9FFFF */  andi       $t9, $t7, 0xFFFF
    /* 9F4C 8000934C 032E3825 */  or         $a3, $t9, $t6
    /* 9F50 80009350 468021A0 */  cvt.s.w    $ft1, $ft0
    /* 9F54 80009354 44875000 */  mtc1       $a3, $ft3
    /* 9F58 80009358 AFA70000 */  sw         $a3, 0x0($sp)
    /* 9F5C 8000935C 25290008 */  addiu      $t1, $t1, 0x8
    /* 9F60 80009360 24420004 */  addiu      $v0, $v0, 0x4
    /* 9F64 80009364 46805420 */  cvt.s.w    $ft4, $ft3
    /* 9F68 80009368 46003202 */  mul.s      $ft2, $ft1, $fv0
    /* 9F6C 8000936C 24630004 */  addiu      $v1, $v1, 0x4
    /* 9F70 80009370 46008482 */  mul.s      $ft5, $ft4, $fv0
    /* 9F74 80009374 E528FFF8 */  swc1       $ft2, -0x8($t1)
    /* 9F78 80009378 148BFFE5 */  bne        $a0, $t3, .L80009310
    /* 9F7C 8000937C E532FFFC */   swc1      $ft5, -0x4($t1)
    /* 9F80 80009380 24C60001 */  addiu      $a2, $a2, 0x1
    /* 9F84 80009384 14CCFFE0 */  bne        $a2, $t4, .L80009308
    /* 9F88 80009388 25080010 */   addiu     $t0, $t0, 0x10
    /* 9F8C 8000938C 03E00008 */  jr         $ra
    /* 9F90 80009390 27BD0010 */   addiu     $sp, $sp, 0x10
```

## Tips for This Category

