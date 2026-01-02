# apply_display_mode

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80001E58` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~11 |

## Description

apply current display mode settings

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
./work/boot/apply_display_mode/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/apply_display_mode

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/apply_display_mode --watch
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
# Source: 21F0.s
# Address: 0x80001E58

glabel func_80001E58
    /* 2A58 80001E58 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 2A5C 80001E5C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 2A60 80001E60 3C048003 */  lui        $a0, %hi(gViewportStruct)
    /* 2A64 80001E64 0C001B50 */  jal        func_80006D40
    /* 2A68 80001E68 8C84EBFC */   lw        $a0, %lo(gViewportStruct)($a0)
    /* 2A6C 80001E6C 10000001 */  b          .L80001E74
    /* 2A70 80001E70 00000000 */   nop
  .L80001E74:
    /* 2A74 80001E74 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 2A78 80001E78 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 2A7C 80001E7C 03E00008 */  jr         $ra
    /* 2A80 80001E80 00000000 */   nop
```

## Tips for This Category

