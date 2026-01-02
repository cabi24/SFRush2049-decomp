# huft_alloc

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80004D6C` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~11 |

## Description

allocate Huffman table memory

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `inflate` category.

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
./work/inflate/huft_alloc/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/huft_alloc

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/huft_alloc --watch
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
# Source: 5610.s
# Address: 0x80004D6C

glabel func_80004D6C
    /* 596C 80004D6C 3C028003 */  lui        $v0, %hi(D_800354C8)
    /* 5970 80004D70 244254C8 */  addiu      $v0, $v0, %lo(D_800354C8)
    /* 5974 80004D74 8C4E0000 */  lw         $t6, 0x0($v0)
    /* 5978 80004D78 3C198003 */  lui        $t9, %hi(D_800354C4)
    /* 597C 80004D7C 01C47821 */  addu       $t7, $t6, $a0
    /* 5980 80004D80 AC4F0000 */  sw         $t7, 0x0($v0)
    /* 5984 80004D84 8C430000 */  lw         $v1, 0x0($v0)
    /* 5988 80004D88 8F3954C4 */  lw         $t9, %lo(D_800354C4)($t9)
    /* 598C 80004D8C 0064C023 */  subu       $t8, $v1, $a0
    /* 5990 80004D90 03E00008 */  jr         $ra
    /* 5994 80004D94 03191021 */   addu      $v0, $t8, $t9
```

## Tips for This Category

