# inflate_flush_window

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80004D50` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~7 |

## Description

flush output window to destination

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
./work/inflate/inflate_flush_window/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/inflate_flush_window

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/inflate_flush_window --watch
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
# Address: 0x80004D50

glabel func_80004D50
    /* 5950 80004D50 3C018003 */  lui        $at, %hi(D_800354C4)
    /* 5954 80004D54 AC2454C4 */  sw         $a0, %lo(D_800354C4)($at)
    /* 5958 80004D58 3C018003 */  lui        $at, %hi(D_800354CC)
    /* 595C 80004D5C AC2554CC */  sw         $a1, %lo(D_800354CC)($at)
    /* 5960 80004D60 3C018003 */  lui        $at, %hi(D_800354C8)
    /* 5964 80004D64 03E00008 */  jr         $ra
    /* 5968 80004D68 AC2054C8 */   sw        $zero, %lo(D_800354C8)($at)
```

## Tips for This Category

