# viEnableAccum

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800013C0` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~7 |

## Description

enable time accumulation

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `boot` category.

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
./work/boot/viEnableAccum/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/viEnableAccum

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/viEnableAccum --watch
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
# Source: 1050.s
# Address: 0x800013C0

glabel func_800013C0
    /* 1FC0 800013C0 240E0001 */  addiu      $t6, $zero, 0x1
    /* 1FC4 800013C4 3C018003 */  lui        $at, %hi(D_8002AFB0)
    /* 1FC8 800013C8 AC2EAFB0 */  sw         $t6, %lo(D_8002AFB0)($at)
    /* 1FCC 800013CC 03E00008 */  jr         $ra
    /* 1FD0 800013D0 00000000 */   nop
    /* 1FD4 800013D4 03E00008 */  jr         $ra
    /* 1FD8 800013D8 00000000 */   nop
```

## Tips for This Category

