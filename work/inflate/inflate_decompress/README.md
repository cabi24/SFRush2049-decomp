# inflate_decompress

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800026C0` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~26 |

## Description

inflate decompress with DMA

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
./work/inflate/inflate_decompress/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/inflate_decompress

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/inflate_decompress --watch
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
# Address: 0x800026C0

glabel func_800026C0
    /* 32C0 800026C0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 32C4 800026C4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 32C8 800026C8 AFA40020 */  sw         $a0, 0x20($sp)
    /* 32CC 800026CC AFA50024 */  sw         $a1, 0x24($sp)
    /* 32D0 800026D0 AFA60028 */  sw         $a2, 0x28($sp)
    /* 32D4 800026D4 0C000966 */  jal        func_80002598
    /* 32D8 800026D8 24040001 */   addiu     $a0, $zero, 0x1
    /* 32DC 800026DC 14400003 */  bnez       $v0, .L800026EC
    /* 32E0 800026E0 00000000 */   nop
    /* 32E4 800026E4 1000000C */  b          .L80002718
    /* 32E8 800026E8 00001025 */   or        $v0, $zero, $zero
  .L800026EC:
    /* 32EC 800026EC 8FA40020 */  lw         $a0, 0x20($sp)
    /* 32F0 800026F0 8FA50024 */  lw         $a1, 0x24($sp)
    /* 32F4 800026F4 0C001A05 */  jal        func_80006814
    /* 32F8 800026F8 8FA60028 */   lw        $a2, 0x28($sp)
    /* 32FC 800026FC AFA2001C */  sw         $v0, 0x1C($sp)
    /* 3300 80002700 0C00098B */  jal        func_8000262C
    /* 3304 80002704 00000000 */   nop
    /* 3308 80002708 10000003 */  b          .L80002718
    /* 330C 8000270C 8FA2001C */   lw        $v0, 0x1C($sp)
    /* 3310 80002710 10000001 */  b          .L80002718
    /* 3314 80002714 00000000 */   nop
  .L80002718:
    /* 3318 80002718 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 331C 8000271C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 3320 80002720 03E00008 */  jr         $ra
    /* 3324 80002724 00000000 */   nop
```

## Tips for This Category

