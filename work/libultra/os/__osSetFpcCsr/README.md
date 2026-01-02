# __osSetFpcCsr

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D790` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~4 |

## Description

set FP control/status register

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
./work/libultra/os/__osSetFpcCsr/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/__osSetFpcCsr

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/__osSetFpcCsr --watch
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
# Source: E390.s
# Address: 0x8000D790

glabel func_8000D790
    /* E390 8000D790 4442F800 */  cfc1       $v0, $31
    /* E394 8000D794 44C4F800 */  ctc1       $a0, $31
    /* E398 8000D798 03E00008 */  jr         $ra
    /* E39C 8000D79C 00000000 */   nop
```

## Tips for This Category

