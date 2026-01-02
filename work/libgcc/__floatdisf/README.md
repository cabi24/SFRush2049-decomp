# __floatdisf

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80009B7C` |
| **Category** | `libgcc` |
| **Status** | `TODO` |
| **Instructions** | ~6 |

## Description

long long to float

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
./work/libgcc/__floatdisf/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libgcc/__floatdisf

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libgcc/__floatdisf --watch
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
# Address: 0x80009B7C

glabel func_80009B7C
    /* A77C 80009B7C AFA40000 */  sw         $a0, 0x0($sp)
    /* A780 80009B80 AFA50004 */  sw         $a1, 0x4($sp)
    /* A784 80009B84 DFAE0000 */  ld         $t6, 0x0($sp)
    /* A788 80009B88 44AE2000 */  dmtc1      $t6, $ft0
    /* A78C 80009B8C 03E00008 */  jr         $ra
    /* A790 80009B90 46A02020 */   cvt.s.l   $fv0, $ft0
```

## Tips for This Category

