# osGetActiveQueue

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000C660` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~3 |

## Description

get active thread queue

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/os` category.

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
./work/libultra/os/osGetActiveQueue/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osGetActiveQueue

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osGetActiveQueue --watch
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
# Source: D260.s
# Address: 0x8000C660

glabel func_8000C660
    /* D260 8000C660 3C028003 */  lui        $v0, %hi(D_8002C460)
    /* D264 8000C664 03E00008 */  jr         $ra
    /* D268 8000C668 8C42C460 */   lw        $v0, %lo(D_8002C460)($v0)
```

## Tips for This Category

