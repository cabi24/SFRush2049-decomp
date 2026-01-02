# __osInsertTimer

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000E390` |
| **Category** | `libultra/timer` |
| **Status** | `TODO` |
| **Instructions** | ~10 |

## Description

insert timer into sorted timer queue

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/timer` category.

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
./work/libultra/timer/__osInsertTimer/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/timer/__osInsertTimer

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/timer/__osInsertTimer --watch
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
# Source: E9A0.s
# Address: 0x8000E390

glabel func_8000E390
    /* EF90 8000E390 3C038003 */  lui        $v1, %hi(D_8002C380)
    /* EF94 8000E394 2463C380 */  addiu      $v1, $v1, %lo(D_8002C380)
    /* EF98 8000E398 8C6E0000 */  lw         $t6, 0x0($v1)
    /* EF9C 8000E39C 55C00004 */  bnel       $t6, $zero, .L8000E3B0
    /* EFA0 8000E3A0 8C620008 */   lw        $v0, 0x8($v1)
    /* EFA4 8000E3A4 03E00008 */  jr         $ra
    /* EFA8 8000E3A8 00001025 */   or        $v0, $zero, $zero
    /* EFAC 8000E3AC 8C620008 */  lw         $v0, 0x8($v1)
  .L8000E3B0:
    /* EFB0 8000E3B0 03E00008 */  jr         $ra
    /* EFB4 8000E3B4 00000000 */   nop
```

## Tips for This Category

