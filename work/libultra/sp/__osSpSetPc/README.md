# __osSpSetPc

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D650` |
| **Category** | `libultra/sp` |
| **Status** | `TODO` |
| **Instructions** | ~11 |

## Description

set RSP program counter (only if halted)

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
./work/libultra/sp/__osSpSetPc/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/sp/__osSpSetPc

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/sp/__osSpSetPc --watch
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
# Source: E250.s
# Address: 0x8000D650

glabel func_8000D650
    /* E250 8000D650 3C0EA404 */  lui        $t6, %hi(SP_STATUS_REG)
    /* E254 8000D654 8DC20010 */  lw         $v0, %lo(SP_STATUS_REG)($t6)
    /* E258 8000D658 3C18A408 */  lui        $t8, %hi(SP_PC)
    /* E25C 8000D65C 304F0001 */  andi       $t7, $v0, 0x1
    /* E260 8000D660 15E00003 */  bnez       $t7, .L8000D670
    /* E264 8000D664 00001025 */   or        $v0, $zero, $zero
    /* E268 8000D668 03E00008 */  jr         $ra
    /* E26C 8000D66C 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000D670:
    /* E270 8000D670 AF040000 */  sw         $a0, %lo(SP_PC)($t8)
    /* E274 8000D674 03E00008 */  jr         $ra
    /* E278 8000D678 00000000 */   nop
```

## Tips for This Category

