# osPiReleaseAccess

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000DC54` |
| **Category** | `libultra/pi` |
| **Status** | `TODO` |
| **Instructions** | ~11 |

## Description

release PI mutex

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/pi` category.

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
./work/libultra/pi/osPiReleaseAccess/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pi/osPiReleaseAccess

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pi/osPiReleaseAccess --watch
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
# Source: E7C0.s
# Address: 0x8000DC54

glabel func_8000DC54
    /* E854 8000DC54 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E858 8000DC58 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E85C 8000DC5C 3C048003 */  lui        $a0, %hi(D_80037C78)
    /* E860 8000DC60 24847C78 */  addiu      $a0, $a0, %lo(D_80037C78)
    /* E864 8000DC64 00002825 */  or         $a1, $zero, $zero
    /* E868 8000DC68 0C001D78 */  jal        func_800075E0
    /* E86C 8000DC6C 00003025 */   or        $a2, $zero, $zero
    /* E870 8000DC70 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E874 8000DC74 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E878 8000DC78 03E00008 */  jr         $ra
    /* E87C 8000DC7C 00000000 */   nop
```

## Tips for This Category

