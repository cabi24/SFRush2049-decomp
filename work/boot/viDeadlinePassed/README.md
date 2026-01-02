# viDeadlinePassed

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800015BC` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~8 |

## Description

check if deadline passed

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
./work/boot/viDeadlinePassed/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/viDeadlinePassed

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/viDeadlinePassed --watch
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
# Source: 1050.s
# Address: 0x800015BC

glabel func_800015BC
    /* 21BC 800015BC 3C0E8003 */  lui        $t6, %hi(D_8002EB9C)
    /* 21C0 800015C0 3C0F8003 */  lui        $t7, %hi(D_8002EB64)
    /* 21C4 800015C4 8DEFEB64 */  lw         $t7, %lo(D_8002EB64)($t7)
    /* 21C8 800015C8 8DCEEB9C */  lw         $t6, %lo(D_8002EB9C)($t6)
    /* 21CC 800015CC 01CF1023 */  subu       $v0, $t6, $t7
    /* 21D0 800015D0 28580001 */  slti       $t8, $v0, 0x1
    /* 21D4 800015D4 03E00008 */  jr         $ra
    /* 21D8 800015D8 03001025 */   or        $v0, $t8, $zero
```

## Tips for This Category

