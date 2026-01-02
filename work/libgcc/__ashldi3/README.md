# __ashldi3

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D994` |
| **Category** | `libgcc` |
| **Status** | `TODO` |
| **Instructions** | ~11 |

## Description

arithmetic left shift

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
./work/libgcc/__ashldi3/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libgcc/__ashldi3

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libgcc/__ashldi3 --watch
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
# Address: 0x8000D994

glabel func_8000D994
    /* E594 8000D994 AFA40000 */  sw         $a0, 0x0($sp)
    /* E598 8000D998 AFA50004 */  sw         $a1, 0x4($sp)
    /* E59C 8000D99C AFA60008 */  sw         $a2, 0x8($sp)
    /* E5A0 8000D9A0 AFA7000C */  sw         $a3, 0xC($sp)
    /* E5A4 8000D9A4 DFAF0008 */  ld         $t7, 0x8($sp)
    /* E5A8 8000D9A8 DFAE0000 */  ld         $t6, 0x0($sp)
    /* E5AC 8000D9AC 01EE1014 */  dsllv      $v0, $t6, $t7
    /* E5B0 8000D9B0 0002183C */  dsll32     $v1, $v0, 0
    /* E5B4 8000D9B4 0003183F */  dsra32     $v1, $v1, 0
    /* E5B8 8000D9B8 03E00008 */  jr         $ra
    /* E5BC 8000D9BC 0002103F */   dsra32    $v0, $v0, 0
```

## Tips for This Category

