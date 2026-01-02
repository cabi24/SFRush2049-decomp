# osGetTime

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800073B0` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~33 |

## Description

get system time

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
./work/libultra/os/osGetTime/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osGetTime

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osGetTime --watch
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
# Source: 7FB0.s
# Address: 0x800073B0

glabel func_800073B0
    /* 7FB0 800073B0 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* 7FB4 800073B4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 7FB8 800073B8 0C00312C */  jal        func_8000C4B0
    /* 7FBC 800073BC AFB00018 */   sw        $s0, 0x18($sp)
    /* 7FC0 800073C0 0C00325C */  jal        func_8000C970
    /* 7FC4 800073C4 00408025 */   or        $s0, $v0, $zero
    /* 7FC8 800073C8 AFA20034 */  sw         $v0, 0x34($sp)
    /* 7FCC 800073CC 3C0F8003 */  lui        $t7, %hi(D_80037C58)
    /* 7FD0 800073D0 8DEF7C58 */  lw         $t7, %lo(D_80037C58)($t7)
    /* 7FD4 800073D4 8FAE0034 */  lw         $t6, 0x34($sp)
    /* 7FD8 800073D8 3C088003 */  lui        $t0, %hi(D_80037C50)
    /* 7FDC 800073DC 3C098003 */  lui        $t1, %hi(D_80037C54)
    /* 7FE0 800073E0 8D297C54 */  lw         $t1, %lo(D_80037C54)($t1)
    /* 7FE4 800073E4 8D087C50 */  lw         $t0, %lo(D_80037C50)($t0)
    /* 7FE8 800073E8 01CFC023 */  subu       $t8, $t6, $t7
    /* 7FEC 800073EC AFB80030 */  sw         $t8, 0x30($sp)
    /* 7FF0 800073F0 02002025 */  or         $a0, $s0, $zero
    /* 7FF4 800073F4 AFA9002C */  sw         $t1, 0x2C($sp)
    /* 7FF8 800073F8 0C003148 */  jal        func_8000C520
    /* 7FFC 800073FC AFA80028 */   sw        $t0, 0x28($sp)
    /* 8000 80007400 8FB90030 */  lw         $t9, 0x30($sp)
    /* 8004 80007404 8FAD002C */  lw         $t5, 0x2C($sp)
    /* 8008 80007408 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 800C 8000740C 03205825 */  or         $t3, $t9, $zero
    /* 8010 80007410 016D1821 */  addu       $v1, $t3, $t5
    /* 8014 80007414 8FAC0028 */  lw         $t4, 0x28($sp)
    /* 8018 80007418 240A0000 */  addiu      $t2, $zero, 0x0
    /* 801C 8000741C 006D082B */  sltu       $at, $v1, $t5
    /* 8020 80007420 002A1021 */  addu       $v0, $at, $t2
    /* 8024 80007424 8FB00018 */  lw         $s0, 0x18($sp)
    /* 8028 80007428 27BD0038 */  addiu      $sp, $sp, 0x38
    /* 802C 8000742C 03E00008 */  jr         $ra
    /* 8030 80007430 004C1021 */   addu      $v0, $v0, $t4
```

## Tips for This Category

