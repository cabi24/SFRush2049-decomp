# __osPopThread

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D10C` |
| **Category** | `libultra/thread` |
| **Status** | `TODO` |
| **Instructions** | ~4 |

## Description

pop highest priority thread

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/thread` category.

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
./work/libultra/thread/__osPopThread/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/thread/__osPopThread

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/thread/__osPopThread --watch
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
# Source: D580.s
# Address: 0x8000D10C

glabel func_8000D10C
    /* DD0C 8000D10C 8C820000 */  lw         $v0, 0x0($a0)
    /* DD10 8000D110 8C590000 */  lw         $t9, 0x0($v0)
    /* DD14 8000D114 03E00008 */  jr         $ra
    /* DD18 8000D118 AC990000 */   sw        $t9, 0x0($a0)
```

## Tips for This Category

