# __osSpSetStatus

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D640` |
| **Category** | `libultra/sp` |
| **Status** | `TODO` |
| **Instructions** | ~3 |

## Description

set RSP status register

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/sp` category.

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
./work/libultra/sp/__osSpSetStatus/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/sp/__osSpSetStatus

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/sp/__osSpSetStatus --watch
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
# Source: E240.s
# Address: 0x8000D640

glabel func_8000D640
    /* E240 8000D640 3C0EA404 */  lui        $t6, %hi(SP_STATUS_REG)
    /* E244 8000D644 03E00008 */  jr         $ra
    /* E248 8000D648 ADC40010 */   sw        $a0, %lo(SP_STATUS_REG)($t6)
```

## Tips for This Category

