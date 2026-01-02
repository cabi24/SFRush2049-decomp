# guMtxIdent

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000941C` |
| **Category** | `libultra/gu` |
| **Status** | `TODO` |
| **Instructions** | ~12 |

## Description

create N64 identity matrix (fixed-point)

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
./work/libultra/gu/guMtxIdent/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/gu/guMtxIdent

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/gu/guMtxIdent --watch
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
# Source: 9DE0.s
# Address: 0x8000941C

glabel func_8000941C
    /* A01C 8000941C 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* A020 80009420 AFBF0014 */  sw         $ra, 0x14($sp)
    /* A024 80009424 AFA40058 */  sw         $a0, 0x58($sp)
    /* A028 80009428 0C0024E5 */  jal        func_80009394
    /* A02C 8000942C 27A40018 */   addiu     $a0, $sp, 0x18
    /* A030 80009430 27A40018 */  addiu      $a0, $sp, 0x18
    /* A034 80009434 0C002478 */  jal        func_800091E0
    /* A038 80009438 8FA50058 */   lw        $a1, 0x58($sp)
    /* A03C 8000943C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* A040 80009440 27BD0058 */  addiu      $sp, $sp, 0x58
    /* A044 80009444 03E00008 */  jr         $ra
    /* A048 80009448 00000000 */   nop
```

## Tips for This Category

