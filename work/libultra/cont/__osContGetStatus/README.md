# __osContGetStatus

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800095AC` |
| **Category** | `libultra/cont` |
| **Status** | `TODO` |
| **Instructions** | ~43 |

## Description

get controller status

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
./work/libultra/cont/__osContGetStatus/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/cont/__osContGetStatus

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/cont/__osContGetStatus --watch
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
# Address: 0x800095AC

glabel func_800095AC
    /* A1AC 800095AC 3C078003 */  lui        $a3, %hi(D_80037AE1)
    /* A1B0 800095B0 24E77AE1 */  addiu      $a3, $a3, %lo(D_80037AE1)
    /* A1B4 800095B4 90EE0000 */  lbu        $t6, 0x0($a3)
    /* A1B8 800095B8 3C038003 */  lui        $v1, %hi(D_80037AA0)
    /* A1BC 800095BC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* A1C0 800095C0 00001025 */  or         $v0, $zero, $zero
    /* A1C4 800095C4 24637AA0 */  addiu      $v1, $v1, %lo(D_80037AA0)
    /* A1C8 800095C8 19C00020 */  blez       $t6, .L8000964C
    /* A1CC 800095CC 00003025 */   or        $a2, $zero, $zero
    /* A1D0 800095D0 27A8000C */  addiu      $t0, $sp, 0xC
  .L800095D4:
    /* A1D4 800095D4 88610000 */  lwl        $at, 0x0($v1)
    /* A1D8 800095D8 98610003 */  lwr        $at, 0x3($v1)
    /* A1DC 800095DC AD010000 */  sw         $at, 0x0($t0)
    /* A1E0 800095E0 88780004 */  lwl        $t8, 0x4($v1)
    /* A1E4 800095E4 98780007 */  lwr        $t8, 0x7($v1)
    /* A1E8 800095E8 AD180004 */  sw         $t8, 0x4($t0)
    /* A1EC 800095EC 93B9000E */  lbu        $t9, 0xE($sp)
    /* A1F0 800095F0 332900C0 */  andi       $t1, $t9, 0xC0
    /* A1F4 800095F4 00095103 */  sra        $t2, $t1, 4
    /* A1F8 800095F8 314B00FF */  andi       $t3, $t2, 0xFF
    /* A1FC 800095FC 1560000D */  bnez       $t3, .L80009634
    /* A200 80009600 A0AA0003 */   sb        $t2, 0x3($a1)
    /* A204 80009604 93AC0011 */  lbu        $t4, 0x11($sp)
    /* A208 80009608 93AE0010 */  lbu        $t6, 0x10($sp)
    /* A20C 8000960C 24190001 */  addiu      $t9, $zero, 0x1
    /* A210 80009610 000C6A00 */  sll        $t5, $t4, 8
    /* A214 80009614 01AE7825 */  or         $t7, $t5, $t6
    /* A218 80009618 A4AF0000 */  sh         $t7, 0x0($a1)
    /* A21C 8000961C 93B80012 */  lbu        $t8, 0x12($sp)
    /* A220 80009620 00D94804 */  sllv       $t1, $t9, $a2
    /* A224 80009624 00491025 */  or         $v0, $v0, $t1
    /* A228 80009628 304A00FF */  andi       $t2, $v0, 0xFF
    /* A22C 8000962C 01401025 */  or         $v0, $t2, $zero
    /* A230 80009630 A0B80002 */  sb         $t8, 0x2($a1)
  .L80009634:
    /* A234 80009634 90EB0000 */  lbu        $t3, 0x0($a3)
    /* A238 80009638 24C60001 */  addiu      $a2, $a2, 0x1
    /* A23C 8000963C 24630008 */  addiu      $v1, $v1, 0x8
    /* A240 80009640 00CB082A */  slt        $at, $a2, $t3
    /* A244 80009644 1420FFE3 */  bnez       $at, .L800095D4
    /* A248 80009648 24A50004 */   addiu     $a1, $a1, 0x4
  .L8000964C:
    /* A24C 8000964C A0820000 */  sb         $v0, 0x0($a0)
    /* A250 80009650 03E00008 */  jr         $ra
    /* A254 80009654 27BD0018 */   addiu     $sp, $sp, 0x18
```

## Tips for This Category

