# __udivdi3

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D958` |
| **Category** | `libgcc` |
| **Status** | `TODO` |
| **Instructions** | ~15 |

## Description

unsigned divide

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
./work/libgcc/__udivdi3/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libgcc/__udivdi3

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libgcc/__udivdi3 --watch
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
# Address: 0x8000D958

glabel func_8000D958
    /* E558 8000D958 AFA40000 */  sw         $a0, 0x0($sp)
    /* E55C 8000D95C AFA50004 */  sw         $a1, 0x4($sp)
    /* E560 8000D960 AFA60008 */  sw         $a2, 0x8($sp)
    /* E564 8000D964 AFA7000C */  sw         $a3, 0xC($sp)
    /* E568 8000D968 DFAF0008 */  ld         $t7, 0x8($sp)
    /* E56C 8000D96C DFAE0000 */  ld         $t6, 0x0($sp)
    /* E570 8000D970 01CF001F */  ddivu      $zero, $t6, $t7
    /* E574 8000D974 15E00002 */  bnez       $t7, .L8000D980
    /* E578 8000D978 00000000 */   nop
    /* E57C 8000D97C 0007000D */  break      7
  .L8000D980:
    /* E580 8000D980 00001012 */  mflo       $v0
    /* E584 8000D984 0002183C */  dsll32     $v1, $v0, 0
    /* E588 8000D988 0003183F */  dsra32     $v1, $v1, 0
    /* E58C 8000D98C 03E00008 */  jr         $ra
    /* E590 8000D990 0002103F */   dsra32    $v0, $v0, 0
```

## Tips for This Category

