# __osSpDeviceBusy

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D710` |
| **Category** | `libultra/sp` |
| **Status** | `TODO` |
| **Instructions** | ~9 |

## Description

check if RSP is busy (bits 0x1C)

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/sp` category.

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
./work/libultra/sp/__osSpDeviceBusy/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/sp/__osSpDeviceBusy

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/sp/__osSpDeviceBusy --watch
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
# Source: E310.s
# Address: 0x8000D710

glabel func_8000D710
    /* E310 8000D710 3C0EA404 */  lui        $t6, %hi(SP_STATUS_REG)
    /* E314 8000D714 8DC20010 */  lw         $v0, %lo(SP_STATUS_REG)($t6)
    /* E318 8000D718 304F001C */  andi       $t7, $v0, 0x1C
    /* E31C 8000D71C 11E00003 */  beqz       $t7, .L8000D72C
    /* E320 8000D720 00001025 */   or        $v0, $zero, $zero
    /* E324 8000D724 03E00008 */  jr         $ra
    /* E328 8000D728 24020001 */   addiu     $v0, $zero, 0x1
  .L8000D72C:
    /* E32C 8000D72C 03E00008 */  jr         $ra
    /* E330 8000D730 00000000 */   nop
```

## Tips for This Category

