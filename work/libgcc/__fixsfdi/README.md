# __fixsfdi

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80009A0C` |
| **Category** | `libgcc` |
| **Status** | `TODO` |
| **Instructions** | ~7 |

## Description

float to long long

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
./work/libgcc/__fixsfdi/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libgcc/__fixsfdi

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libgcc/__fixsfdi --watch
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
# Address: 0x80009A0C

glabel func_80009A0C
    /* A60C 80009A0C 46006109 */  trunc.l.s  $ft0, $fa0
    /* A610 80009A10 44222000 */  dmfc1      $v0, $ft0
    /* A614 80009A14 00000000 */  nop
    /* A618 80009A18 0002183C */  dsll32     $v1, $v0, 0
    /* A61C 80009A1C 0003183F */  dsra32     $v1, $v1, 0
    /* A620 80009A20 03E00008 */  jr         $ra
    /* A624 80009A24 0002103F */   dsra32    $v0, $v0, 0
```

## Tips for This Category

