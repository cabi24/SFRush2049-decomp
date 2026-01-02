# guMtxIdentF

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80009394` |
| **Category** | `libultra/gu` |
| **Status** | `MATCHING` |
| **Instructions** | ~34 |

## Description

create 4x4 identity matrix (float)

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
./work/libultra/gu/guMtxIdentF/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/gu/guMtxIdentF

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/gu/guMtxIdentF --watch
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
# Address: 0x80009394

glabel func_80009394
    /* 9F94 80009394 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 9F98 80009398 00801825 */  or         $v1, $a0, $zero
    /* 9F9C 8000939C 44810000 */  mtc1       $at, $fv0
    /* 9FA0 800093A0 44801000 */  mtc1       $zero, $fv1
    /* 9FA4 800093A4 24040001 */  addiu      $a0, $zero, 0x1
    /* 9FA8 800093A8 00001025 */  or         $v0, $zero, $zero
    /* 9FAC 800093AC 24070004 */  addiu      $a3, $zero, 0x4
    /* 9FB0 800093B0 24060003 */  addiu      $a2, $zero, 0x3
    /* 9FB4 800093B4 24050002 */  addiu      $a1, $zero, 0x2
  .L800093B8:
    /* 9FB8 800093B8 54400004 */  bnel       $v0, $zero, .L800093CC
    /* 9FBC 800093BC E4620000 */   swc1      $fv1, 0x0($v1)
    /* 9FC0 800093C0 10000002 */  b          .L800093CC
    /* 9FC4 800093C4 E4600000 */   swc1      $fv0, 0x0($v1)
    /* 9FC8 800093C8 E4620000 */  swc1       $fv1, 0x0($v1)
  .L800093CC:
    /* 9FCC 800093CC 54440004 */  bnel       $v0, $a0, .L800093E0
    /* 9FD0 800093D0 E4620004 */   swc1      $fv1, 0x4($v1)
    /* 9FD4 800093D4 10000002 */  b          .L800093E0
    /* 9FD8 800093D8 E4600004 */   swc1      $fv0, 0x4($v1)
    /* 9FDC 800093DC E4620004 */  swc1       $fv1, 0x4($v1)
  .L800093E0:
    /* 9FE0 800093E0 54450004 */  bnel       $v0, $a1, .L800093F4
    /* 9FE4 800093E4 E4620008 */   swc1      $fv1, 0x8($v1)
    /* 9FE8 800093E8 10000002 */  b          .L800093F4
    /* 9FEC 800093EC E4600008 */   swc1      $fv0, 0x8($v1)
    /* 9FF0 800093F0 E4620008 */  swc1       $fv1, 0x8($v1)
  .L800093F4:
    /* 9FF4 800093F4 54460004 */  bnel       $v0, $a2, .L80009408
    /* 9FF8 800093F8 E462000C */   swc1      $fv1, 0xC($v1)
    /* 9FFC 800093FC 10000002 */  b          .L80009408
    /* A000 80009400 E460000C */   swc1      $fv0, 0xC($v1)
    /* A004 80009404 E462000C */  swc1       $fv1, 0xC($v1)
  .L80009408:
    /* A008 80009408 24420001 */  addiu      $v0, $v0, 0x1
    /* A00C 8000940C 1447FFEA */  bne        $v0, $a3, .L800093B8
    /* A010 80009410 24630010 */   addiu     $v1, $v1, 0x10
    /* A014 80009414 03E00008 */  jr         $ra
    /* A018 80009418 00000000 */   nop
```

## Tips for This Category

