# osSetIntMask

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80006DA0` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~25 |

## Description

set interrupt mask

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
./work/libultra/os/osSetIntMask/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osSetIntMask

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osSetIntMask --watch
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
# Source: 79A0.s
# Address: 0x80006DA0

glabel func_80006DA0
    /* 79A0 80006DA0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 79A4 80006DA4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 79A8 80006DA8 0C00312C */  jal        func_8000C4B0
    /* 79AC 80006DAC AFA40018 */   sw        $a0, 0x18($sp)
    /* 79B0 80006DB0 93AE001B */  lbu        $t6, 0x1B($sp)
    /* 79B4 80006DB4 00402025 */  or         $a0, $v0, $zero
    /* 79B8 80006DB8 3C028003 */  lui        $v0, %hi(D_8002C464)
    /* 79BC 80006DBC 11C00007 */  beqz       $t6, .L80006DDC
    /* 79C0 80006DC0 00000000 */   nop
    /* 79C4 80006DC4 3C028003 */  lui        $v0, %hi(D_8002C464)
    /* 79C8 80006DC8 8C42C464 */  lw         $v0, %lo(D_8002C464)($v0)
    /* 79CC 80006DCC 944F0000 */  lhu        $t7, 0x0($v0)
    /* 79D0 80006DD0 35F80020 */  ori        $t8, $t7, 0x20
    /* 79D4 80006DD4 10000005 */  b          .L80006DEC
    /* 79D8 80006DD8 A4580000 */   sh        $t8, 0x0($v0)
  .L80006DDC:
    /* 79DC 80006DDC 8C42C464 */  lw         $v0, %lo(D_8002C464)($v0)
    /* 79E0 80006DE0 94590000 */  lhu        $t9, 0x0($v0)
    /* 79E4 80006DE4 3328FFDF */  andi       $t0, $t9, 0xFFDF
    /* 79E8 80006DE8 A4480000 */  sh         $t0, 0x0($v0)
  .L80006DEC:
    /* 79EC 80006DEC 0C003148 */  jal        func_8000C520
    /* 79F0 80006DF0 00000000 */   nop
    /* 79F4 80006DF4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 79F8 80006DF8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 79FC 80006DFC 03E00008 */  jr         $ra
    /* 7A00 80006E00 00000000 */   nop
```

## Tips for This Category

