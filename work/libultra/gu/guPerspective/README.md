# guPerspective

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80008E50` |
| **Category** | `libultra/gu` |
| **Status** | `TODO` |
| **Instructions** | ~22 |

## Description

perspective projection matrix (fixed-point)

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/gu` category.

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
./work/libultra/gu/guPerspective/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/gu/guPerspective

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/gu/guPerspective --watch
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
# Source: 9820.s
# Address: 0x80008E50

glabel func_80008E50
    /* 9A50 80008E50 27BDFF98 */  addiu      $sp, $sp, -0x68
    /* 9A54 80008E54 44866000 */  mtc1       $a2, $fa0
    /* 9A58 80008E58 44877000 */  mtc1       $a3, $fa1
    /* 9A5C 80008E5C C7A40078 */  lwc1       $ft0, 0x78($sp)
    /* 9A60 80008E60 C7A6007C */  lwc1       $ft1, 0x7C($sp)
    /* 9A64 80008E64 C7A80080 */  lwc1       $ft2, 0x80($sp)
    /* 9A68 80008E68 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 9A6C 80008E6C AFA40068 */  sw         $a0, 0x68($sp)
    /* 9A70 80008E70 44066000 */  mfc1       $a2, $fa0
    /* 9A74 80008E74 44077000 */  mfc1       $a3, $fa1
    /* 9A78 80008E78 27A40028 */  addiu      $a0, $sp, 0x28
    /* 9A7C 80008E7C E7A40010 */  swc1       $ft0, 0x10($sp)
    /* 9A80 80008E80 E7A60014 */  swc1       $ft1, 0x14($sp)
    /* 9A84 80008E84 0C002308 */  jal        func_80008C20
    /* 9A88 80008E88 E7A80018 */   swc1      $ft2, 0x18($sp)
    /* 9A8C 80008E8C 27A40028 */  addiu      $a0, $sp, 0x28
    /* 9A90 80008E90 0C002478 */  jal        func_800091E0
    /* 9A94 80008E94 8FA50068 */   lw        $a1, 0x68($sp)
    /* 9A98 80008E98 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 9A9C 80008E9C 27BD0068 */  addiu      $sp, $sp, 0x68
    /* 9AA0 80008EA0 03E00008 */  jr         $ra
    /* 9AA4 80008EA4 00000000 */   nop
```

## Tips for This Category

