# __divdi3

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D9FC` |
| **Category** | `libgcc` |
| **Status** | `TODO` |
| **Instructions** | ~23 |

## Description

signed divide

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libgcc` category.

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
./work/libgcc/__divdi3/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libgcc/__divdi3

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libgcc/__divdi3 --watch
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
# Source: E4F0.s
# Address: 0x8000D9FC

glabel func_8000D9FC
    /* E5FC 8000D9FC AFA40000 */  sw         $a0, 0x0($sp)
    /* E600 8000DA00 AFA50004 */  sw         $a1, 0x4($sp)
    /* E604 8000DA04 AFA60008 */  sw         $a2, 0x8($sp)
    /* E608 8000DA08 AFA7000C */  sw         $a3, 0xC($sp)
    /* E60C 8000DA0C DFAF0008 */  ld         $t7, 0x8($sp)
    /* E610 8000DA10 DFAE0000 */  ld         $t6, 0x0($sp)
    /* E614 8000DA14 01CF001E */  ddiv       $zero, $t6, $t7
    /* E618 8000DA18 00000000 */  nop
    /* E61C 8000DA1C 15E00002 */  bnez       $t7, .L8000DA28
    /* E620 8000DA20 00000000 */   nop
    /* E624 8000DA24 0007000D */  break      7
  .L8000DA28:
    /* E628 8000DA28 6401FFFF */  daddiu     $at, $zero, -0x1
    /* E62C 8000DA2C 15E10005 */  bne        $t7, $at, .L8000DA44
    /* E630 8000DA30 64010001 */   daddiu    $at, $zero, 0x1
    /* E634 8000DA34 00010FFC */  dsll32     $at, $at, 31
    /* E638 8000DA38 15C10002 */  bne        $t6, $at, .L8000DA44
    /* E63C 8000DA3C 00000000 */   nop
    /* E640 8000DA40 0006000D */  break      6
  .L8000DA44:
    /* E644 8000DA44 00001012 */  mflo       $v0
    /* E648 8000DA48 0002183C */  dsll32     $v1, $v0, 0
    /* E64C 8000DA4C 0003183F */  dsra32     $v1, $v1, 0
    /* E650 8000DA50 03E00008 */  jr         $ra
    /* E654 8000DA54 0002103F */   dsra32    $v0, $v0, 0
```

## Tips for This Category

