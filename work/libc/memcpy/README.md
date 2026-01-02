# memcpy

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007C68` |
| **Category** | `libc` |
| **Status** | `TODO` |
| **Instructions** | ~11 |

## Description

copy memory

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
./work/libc/memcpy/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libc/memcpy

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libc/memcpy --watch
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
# Address: 0x80007C68

glabel func_80007C68
    /* 8868 80007C68 00801025 */  or         $v0, $a0, $zero
    /* 886C 80007C6C 10C00007 */  beqz       $a2, .L80007C8C
    /* 8870 80007C70 00A01825 */   or        $v1, $a1, $zero
  .L80007C74:
    /* 8874 80007C74 906E0000 */  lbu        $t6, 0x0($v1)
    /* 8878 80007C78 24C6FFFF */  addiu      $a2, $a2, -0x1
    /* 887C 80007C7C 24420001 */  addiu      $v0, $v0, 0x1
    /* 8880 80007C80 24630001 */  addiu      $v1, $v1, 0x1
    /* 8884 80007C84 14C0FFFB */  bnez       $a2, .L80007C74
    /* 8888 80007C88 A04EFFFF */   sb        $t6, -0x1($v0)
  .L80007C8C:
    /* 888C 80007C8C 03E00008 */  jr         $ra
    /* 8890 80007C90 00801025 */   or        $v0, $a0, $zero
```

## Tips for This Category

