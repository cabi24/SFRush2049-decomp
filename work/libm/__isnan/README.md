# __isnan

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80002C60` |
| **Category** | `libm` |
| **Status** | `TODO` |
| **Instructions** | ~28 |

## Description

No description available.

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libm` category.

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
./work/libm/__isnan/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libm/__isnan

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libm/__isnan --watch
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
# Source: 34A0.s
# Address: 0x80002C60

glabel func_80002C60
    /* 3860 80002C60 27BDFFF8 */  addiu      $sp, $sp, -0x8
    /* 3864 80002C64 F7AC0000 */  sdc1       $fa0, 0x0($sp)
    /* 3868 80002C68 8FAE0000 */  lw         $t6, 0x0($sp)
    /* 386C 80002C6C 240107FF */  addiu      $at, $zero, 0x7FF
    /* 3870 80002C70 000E7840 */  sll        $t7, $t6, 1
    /* 3874 80002C74 000FC542 */  srl        $t8, $t7, 21
    /* 3878 80002C78 1701000F */  bne        $t8, $at, .L80002CB8
    /* 387C 80002C7C 00000000 */   nop
    /* 3880 80002C80 97B90000 */  lhu        $t9, 0x0($sp)
    /* 3884 80002C84 3328800F */  andi       $t0, $t9, 0x800F
    /* 3888 80002C88 A7A80000 */  sh         $t0, 0x0($sp)
    /* 388C 80002C8C D7A40000 */  ldc1       $ft0, 0x0($sp)
    /* 3890 80002C90 44803800 */  mtc1       $zero, $ft1f
    /* 3894 80002C94 44803000 */  mtc1       $zero, $ft1
    /* 3898 80002C98 00001025 */  or         $v0, $zero, $zero
    /* 389C 80002C9C 46262032 */  c.eq.d     $ft0, $ft1
    /* 38A0 80002CA0 00000000 */  nop
    /* 38A4 80002CA4 45000002 */  bc1f       .L80002CB0
    /* 38A8 80002CA8 00000000 */   nop
    /* 38AC 80002CAC 24020001 */  addiu      $v0, $zero, 0x1
  .L80002CB0:
    /* 38B0 80002CB0 10000005 */  b          .L80002CC8
    /* 38B4 80002CB4 00000000 */   nop
  .L80002CB8:
    /* 38B8 80002CB8 10000003 */  b          .L80002CC8
    /* 38BC 80002CBC 00001025 */   or        $v0, $zero, $zero
    /* 38C0 80002CC0 10000001 */  b          .L80002CC8
    /* 38C4 80002CC4 00000000 */   nop
  .L80002CC8:
    /* 38C8 80002CC8 03E00008 */  jr         $ra
    /* 38CC 80002CCC 27BD0008 */   addiu     $sp, $sp, 0x8
```

## Tips for This Category

