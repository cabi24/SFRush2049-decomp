# osPfsReAllocate

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000BE50` |
| **Category** | `libultra/pfs` |
| **Status** | `TODO` |
| **Instructions** | ~6 |

## Description

reallocate controller pak file pages

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/pfs` category.

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
./work/libultra/pfs/osPfsReAllocate/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pfs/osPfsReAllocate

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pfs/osPfsReAllocate --watch
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
# Source: C990.s
# Address: 0x8000BE50

glabel func_8000BE50
    /* CA50 8000BE50 14800003 */  bnez       $a0, .L8000BE60
    /* CA54 8000BE54 00000000 */   nop
    /* CA58 8000BE58 3C048003 */  lui        $a0, %hi(D_8002C3E0)
    /* CA5C 8000BE5C 8C84C3E0 */  lw         $a0, %lo(D_8002C3E0)($a0)
  .L8000BE60:
    /* CA60 8000BE60 03E00008 */  jr         $ra
    /* CA64 8000BE64 8C820014 */   lw        $v0, 0x14($a0)
```

## Tips for This Category

