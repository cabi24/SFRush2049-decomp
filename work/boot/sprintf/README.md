# sprintf

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80004990` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~22 |

## Description

sprintf wrapper (calls fcvt)

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
./work/boot/sprintf/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/sprintf

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/sprintf --watch
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
# Source: 34A0.s
# Address: 0x80004990

glabel func_80004990
    /* 5590 80004990 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 5594 80004994 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 5598 80004998 AFA40020 */  sw         $a0, 0x20($sp)
    /* 559C 8000499C AFA50024 */  sw         $a1, 0x24($sp)
    /* 55A0 800049A0 AFA60028 */  sw         $a2, 0x28($sp)
    /* 55A4 800049A4 AFA7002C */  sw         $a3, 0x2C($sp)
    /* 55A8 800049A8 27AE0024 */  addiu      $t6, $sp, 0x24
    /* 55AC 800049AC 25CF0004 */  addiu      $t7, $t6, 0x4
    /* 55B0 800049B0 AFAF0018 */  sw         $t7, 0x18($sp)
    /* 55B4 800049B4 8FA40020 */  lw         $a0, 0x20($sp)
    /* 55B8 800049B8 8FA50024 */  lw         $a1, 0x24($sp)
    /* 55BC 800049BC 0C000B34 */  jal        func_80002CD0
    /* 55C0 800049C0 8FA60018 */   lw        $a2, 0x18($sp)
    /* 55C4 800049C4 AFA2001C */  sw         $v0, 0x1C($sp)
    /* 55C8 800049C8 10000003 */  b          .L800049D8
    /* 55CC 800049CC 8FA2001C */   lw        $v0, 0x1C($sp)
    /* 55D0 800049D0 10000001 */  b          .L800049D8
    /* 55D4 800049D4 00000000 */   nop
  .L800049D8:
    /* 55D8 800049D8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 55DC 800049DC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 55E0 800049E0 03E00008 */  jr         $ra
    /* 55E4 800049E4 00000000 */   nop
```

## Tips for This Category

