# strlen

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007C40` |
| **Category** | `libc` |
| **Status** | `MATCHING` |
| **Instructions** | ~10 |

## Description

get string length

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libc` category.

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
./work/libc/strlen/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libc/strlen

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libc/strlen --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Standard C library function - should match common implementations

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 8800.s
# Address: 0x80007C40

glabel func_80007C40
    /* 8840 80007C40 908E0000 */  lbu        $t6, 0x0($a0)
    /* 8844 80007C44 00801825 */  or         $v1, $a0, $zero
    /* 8848 80007C48 11C00005 */  beqz       $t6, .L80007C60
    /* 884C 80007C4C 00000000 */   nop
    /* 8850 80007C50 906F0001 */  lbu        $t7, 0x1($v1)
  .L80007C54:
    /* 8854 80007C54 24630001 */  addiu      $v1, $v1, 0x1
    /* 8858 80007C58 55E0FFFE */  bnel       $t7, $zero, .L80007C54
    /* 885C 80007C5C 906F0001 */   lbu       $t7, 0x1($v1)
  .L80007C60:
    /* 8860 80007C60 03E00008 */  jr         $ra
    /* 8864 80007C64 00641023 */   subu      $v0, $v1, $a0
```

## Tips for This Category

