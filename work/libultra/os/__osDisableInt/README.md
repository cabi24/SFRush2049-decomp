# __osDisableInt

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000C4B0` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~28 |

## Description

disable interrupts, return previous state

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/os` category.

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
./work/libultra/os/__osDisableInt/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/__osDisableInt

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/__osDisableInt --watch
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
# Source: D0B0.s
# Address: 0x8000C4B0

glabel func_8000C4B0
    /* D0B0 8000C4B0 3C0A8003 */  lui        $t2, %hi(D_8002C370)
    /* D0B4 8000C4B4 254AC370 */  addiu      $t2, $t2, %lo(D_8002C370)
    /* D0B8 8000C4B8 8D4B0000 */  lw         $t3, 0x0($t2)
    /* D0BC 8000C4BC 316BFF00 */  andi       $t3, $t3, 0xFF00
    /* D0C0 8000C4C0 40086000 */  mfc0       $t0, $12 /* handwritten instruction */
    /* D0C4 8000C4C4 2401FFFE */  addiu      $at, $zero, -0x2
    /* D0C8 8000C4C8 01014824 */  and        $t1, $t0, $at
    /* D0CC 8000C4CC 40896000 */  mtc0       $t1, $12 /* handwritten instruction */
    /* D0D0 8000C4D0 31020001 */  andi       $v0, $t0, 0x1
    /* D0D4 8000C4D4 8D480000 */  lw         $t0, 0x0($t2)
    /* D0D8 8000C4D8 3108FF00 */  andi       $t0, $t0, 0xFF00
    /* D0DC 8000C4DC 110B000E */  beq        $t0, $t3, .L8000C518
    /* D0E0 8000C4E0 3C0A8003 */   lui       $t2, %hi(D_8002C3E0)
    /* D0E4 8000C4E4 254AC3E0 */  addiu      $t2, $t2, %lo(D_8002C3E0)
    /* D0E8 8000C4E8 8D490118 */  lw         $t1, 0x118($t2)
    /* D0EC 8000C4EC 312AFF00 */  andi       $t2, $t1, 0xFF00
    /* D0F0 8000C4F0 01485024 */  and        $t2, $t2, $t0
    /* D0F4 8000C4F4 3C01FFFF */  lui        $at, (0xFFFF00FF >> 16)
    /* D0F8 8000C4F8 342100FF */  ori        $at, $at, (0xFFFF00FF & 0xFFFF)
    /* D0FC 8000C4FC 01214824 */  and        $t1, $t1, $at
    /* D100 8000C500 012A4825 */  or         $t1, $t1, $t2
    /* D104 8000C504 2401FFFE */  addiu      $at, $zero, -0x2
    /* D108 8000C508 01214824 */  and        $t1, $t1, $at
    /* D10C 8000C50C 40896000 */  mtc0       $t1, $12 /* handwritten instruction */
    /* D110 8000C510 00000000 */  nop
    /* D114 8000C514 00000000 */  nop
  .L8000C518:
    /* D118 8000C518 03E00008 */  jr         $ra
    /* D11C 8000C51C 00000000 */   nop
```

## Tips for This Category

