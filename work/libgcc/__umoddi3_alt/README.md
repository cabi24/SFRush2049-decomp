# __umoddi3_alt

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D9C0` |
| **Category** | `libgcc` |
| **Status** | `TODO` |
| **Instructions** | ~15 |

## Description

unsigned 64-bit modulo (duplicate)

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
./work/libgcc/__umoddi3_alt/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libgcc/__umoddi3_alt

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libgcc/__umoddi3_alt --watch
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
# Address: 0x8000D9C0

glabel func_8000D9C0
    /* E5C0 8000D9C0 AFA40000 */  sw         $a0, 0x0($sp)
    /* E5C4 8000D9C4 AFA50004 */  sw         $a1, 0x4($sp)
    /* E5C8 8000D9C8 AFA60008 */  sw         $a2, 0x8($sp)
    /* E5CC 8000D9CC AFA7000C */  sw         $a3, 0xC($sp)
    /* E5D0 8000D9D0 DFAF0008 */  ld         $t7, 0x8($sp)
    /* E5D4 8000D9D4 DFAE0000 */  ld         $t6, 0x0($sp)
    /* E5D8 8000D9D8 01CF001F */  ddivu      $zero, $t6, $t7
    /* E5DC 8000D9DC 15E00002 */  bnez       $t7, .L8000D9E8
    /* E5E0 8000D9E0 00000000 */   nop
    /* E5E4 8000D9E4 0007000D */  break      7
  .L8000D9E8:
    /* E5E8 8000D9E8 00001010 */  mfhi       $v0
    /* E5EC 8000D9EC 0002183C */  dsll32     $v1, $v0, 0
    /* E5F0 8000D9F0 0003183F */  dsra32     $v1, $v1, 0
    /* E5F4 8000D9F4 03E00008 */  jr         $ra
    /* E5F8 8000D9F8 0002103F */   dsra32    $v0, $v0, 0
```

## Tips for This Category

