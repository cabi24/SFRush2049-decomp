# osDpWait

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007B80` |
| **Category** | `libultra/dp` |
| **Status** | `TODO` |
| **Instructions** | ~8 |

## Description

wait for RDP ready

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/dp` category.

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
./work/libultra/dp/osDpWait/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/dp/osDpWait

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/dp/osDpWait --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Check `reference/repos/rushtherock/game/drones.c`

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 8700.s
# Address: 0x80007B80

glabel func_80007B80
    /* 8780 80007B80 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 8784 80007B84 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8788 80007B88 0C003590 */  jal        func_8000D640
    /* 878C 80007B8C 24040400 */   addiu     $a0, $zero, 0x400
    /* 8790 80007B90 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 8794 80007B94 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 8798 80007B98 03E00008 */  jr         $ra
    /* 879C 80007B9C 00000000 */   nop
```

## Tips for This Category

