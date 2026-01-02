# lzss_decompress

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80002660` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~24 |

## Description

LZSS decompress with DMA

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
./work/inflate/lzss_decompress/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/lzss_decompress

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/lzss_decompress --watch
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
# Address: 0x80002660

glabel func_80002660
    /* 3260 80002660 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 3264 80002664 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 3268 80002668 AFA40020 */  sw         $a0, 0x20($sp)
    /* 326C 8000266C AFA50024 */  sw         $a1, 0x24($sp)
    /* 3270 80002670 0C000966 */  jal        func_80002598
    /* 3274 80002674 24040001 */   addiu     $a0, $zero, 0x1
    /* 3278 80002678 14400003 */  bnez       $v0, .L80002688
    /* 327C 8000267C 00000000 */   nop
    /* 3280 80002680 1000000B */  b          .L800026B0
    /* 3284 80002684 00001025 */   or        $v0, $zero, $zero
  .L80002688:
    /* 3288 80002688 8FA40020 */  lw         $a0, 0x20($sp)
    /* 328C 8000268C 0C0012BF */  jal        func_80004AFC
    /* 3290 80002690 8FA50024 */   lw        $a1, 0x24($sp)
    /* 3294 80002694 AFA2001C */  sw         $v0, 0x1C($sp)
    /* 3298 80002698 0C00098B */  jal        func_8000262C
    /* 329C 8000269C 00000000 */   nop
    /* 32A0 800026A0 10000003 */  b          .L800026B0
    /* 32A4 800026A4 8FA2001C */   lw        $v0, 0x1C($sp)
    /* 32A8 800026A8 10000001 */  b          .L800026B0
    /* 32AC 800026AC 00000000 */   nop
  .L800026B0:
    /* 32B0 800026B0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 32B4 800026B4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 32B8 800026B8 03E00008 */  jr         $ra
    /* 32BC 800026BC 00000000 */   nop
```

## Tips for This Category

