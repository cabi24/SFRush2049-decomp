# osDpIsBusy

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D740` |
| **Category** | `libultra/dp` |
| **Status** | `TODO` |
| **Instructions** | ~9 |

## Description

check if RDP is busy

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
./work/libultra/dp/osDpIsBusy/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/dp/osDpIsBusy

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/dp/osDpIsBusy --watch
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
# Source: E340.s
# Address: 0x8000D740

glabel func_8000D740
    /* E340 8000D740 3C0EA410 */  lui        $t6, %hi(DPC_STATUS_REG)
    /* E344 8000D744 8DC2000C */  lw         $v0, %lo(DPC_STATUS_REG)($t6)
    /* E348 8000D748 304F0100 */  andi       $t7, $v0, 0x100
    /* E34C 8000D74C 11E00003 */  beqz       $t7, .L8000D75C
    /* E350 8000D750 00001025 */   or        $v0, $zero, $zero
    /* E354 8000D754 03E00008 */  jr         $ra
    /* E358 8000D758 24020001 */   addiu     $v0, $zero, 0x1
  .L8000D75C:
    /* E35C 8000D75C 03E00008 */  jr         $ra
    /* E360 8000D760 00000000 */   nop
```

## Tips for This Category

