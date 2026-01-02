# dma_signal

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000262C` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~13 |

## Description

signal DMA complete

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
./work/boot/dma_signal/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/dma_signal

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/dma_signal --watch
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
# Source: 3140.s
# Address: 0x8000262C

glabel func_8000262C
    /* 322C 8000262C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 3230 80002630 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 3234 80002634 3C048003 */  lui        $a0, %hi(D_8002F190)
    /* 3238 80002638 2484F190 */  addiu      $a0, $a0, %lo(D_8002F190)
    /* 323C 8000263C 00002825 */  or         $a1, $zero, $zero
    /* 3240 80002640 0C001D78 */  jal        func_800075E0
    /* 3244 80002644 00003025 */   or        $a2, $zero, $zero
    /* 3248 80002648 10000001 */  b          .L80002650
    /* 324C 8000264C 00000000 */   nop
  .L80002650:
    /* 3250 80002650 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 3254 80002654 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 3258 80002658 03E00008 */  jr         $ra
    /* 325C 8000265C 00000000 */   nop
```

## Tips for This Category

