# __osSiRelAccess

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000E5F4` |
| **Category** | `libultra/si` |
| **Status** | `TODO` |
| **Instructions** | ~11 |

## Description

release SI mutex

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/si` category.

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
./work/libultra/si/__osSiRelAccess/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/si/__osSiRelAccess

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/si/__osSiRelAccess --watch
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
# Source: F160.s
# Address: 0x8000E5F4

glabel func_8000E5F4
    /* F1F4 8000E5F4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* F1F8 8000E5F8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* F1FC 8000E5FC 3C048003 */  lui        $a0, %hi(__osSiMesg)
    /* F200 8000E600 24847C98 */  addiu      $a0, $a0, %lo(__osSiMesg)
    /* F204 8000E604 00002825 */  or         $a1, $zero, $zero
    /* F208 8000E608 0C001D78 */  jal        func_800075E0
    /* F20C 8000E60C 00003025 */   or        $a2, $zero, $zero
    /* F210 8000E610 8FBF0014 */  lw         $ra, 0x14($sp)
    /* F214 8000E614 27BD0018 */  addiu      $sp, $sp, 0x18
    /* F218 8000E618 03E00008 */  jr         $ra
    /* F21C 8000E61C 00000000 */   nop
```

## Tips for This Category

