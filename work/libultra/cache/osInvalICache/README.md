# osInvalICache

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007810` |
| **Category** | `libultra/cache` |
| **Status** | `TODO` |
| **Instructions** | ~10 |

## Description

invalidate I-cache

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/cache` category.

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
./work/libultra/cache/osInvalICache/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/cache/osInvalICache

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/cache/osInvalICache --watch
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
# Source: 8410.s
# Address: 0x80007810

glabel func_80007810
    /* 8410 80007810 3C088000 */  lui        $t0, 0x8000
    /* 8414 80007814 240A2000 */  addiu      $t2, $zero, 0x2000
    /* 8418 80007818 010A4821 */  addu       $t1, $t0, $t2
    /* 841C 8000781C 2529FFF0 */  addiu      $t1, $t1, -0x10
  .L80007820:
    /* 8420 80007820 BD010000 */  cache      0x01, 0x0($t0) /* handwritten instruction */
    /* 8424 80007824 0109082B */  sltu       $at, $t0, $t1
    /* 8428 80007828 1420FFFD */  bnez       $at, .L80007820
    /* 842C 8000782C 25080010 */   addiu     $t0, $t0, (0x80000010 & 0xFFFF)
    /* 8430 80007830 03E00008 */  jr         $ra
    /* 8434 80007834 00000000 */   nop
```

## Tips for This Category

