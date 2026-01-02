# __floatundidf

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80009B94` |
| **Category** | `libgcc` |
| **Status** | `TODO` |
| **Instructions** | ~13 |

## Description

unsigned long long to double

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
./work/libgcc/__floatundidf/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libgcc/__floatundidf

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libgcc/__floatundidf --watch
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
# Source: A5F0.s
# Address: 0x80009B94

glabel func_80009B94
    /* A794 80009B94 AFA40000 */  sw         $a0, 0x0($sp)
    /* A798 80009B98 AFA50004 */  sw         $a1, 0x4($sp)
    /* A79C 80009B9C DFAE0000 */  ld         $t6, 0x0($sp)
    /* A7A0 80009BA0 44AE2000 */  dmtc1      $t6, $ft0
    /* A7A4 80009BA4 05C10006 */  bgez       $t6, .L80009BC0
    /* A7A8 80009BA8 46A02021 */   cvt.d.l   $fv0, $ft0
    /* A7AC 80009BAC 3C0141F0 */  lui        $at, (0x41F00000 >> 16)
    /* A7B0 80009BB0 44813800 */  mtc1       $at, $ft1f
    /* A7B4 80009BB4 44803000 */  mtc1       $zero, $ft1
    /* A7B8 80009BB8 00000000 */  nop
    /* A7BC 80009BBC 46260000 */  add.d      $fv0, $fv0, $ft1
  .L80009BC0:
    /* A7C0 80009BC0 03E00008 */  jr         $ra
    /* A7C4 80009BC4 00000000 */   nop
```

## Tips for This Category

