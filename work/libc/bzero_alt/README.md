# bzero_alt

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D2A0` |
| **Category** | `libc` |
| **Status** | `TODO` |
| **Instructions** | ~3 |

## Description

alternative bzero (zeroes memory)

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libc` category.

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
./work/libc/bzero_alt/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libc/bzero_alt

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libc/bzero_alt --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Standard C library function - should match common implementations

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: D580.s
# Address: 0x8000D2A0

glabel func_8000D2A0
    /* DEA0 8000D2A0 3C0EA404 */  lui        $t6, %hi(SP_STATUS_REG)
    /* DEA4 8000D2A4 03E00008 */  jr         $ra
    /* DEA8 8000D2A8 8DC20010 */   lw        $v0, %lo(SP_STATUS_REG)($t6)
```

## Tips for This Category

