# osContGetQuery

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800097AC` |
| **Category** | `libultra/cont` |
| **Status** | `TODO` |
| **Instructions** | ~9 |

## Description

get controller query results (wrapper for __osContGetInitData)

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/cont` category.

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
./work/libultra/cont/osContGetQuery/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/cont/osContGetQuery

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/cont/osContGetQuery --watch
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
# Source: A330.s
# Address: 0x800097AC

glabel func_800097AC
    /* A3AC 800097AC 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* A3B0 800097B0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* A3B4 800097B4 00802825 */  or         $a1, $a0, $zero
    /* A3B8 800097B8 0C00256B */  jal        func_800095AC
    /* A3BC 800097BC 27A4001F */   addiu     $a0, $sp, 0x1F
    /* A3C0 800097C0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* A3C4 800097C4 27BD0020 */  addiu      $sp, $sp, 0x20
    /* A3C8 800097C8 03E00008 */  jr         $ra
    /* A3CC 800097CC 00000000 */   nop
```

## Tips for This Category

