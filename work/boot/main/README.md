# main

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800020F0` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~45 |

## Description

real main function (called from entrypoint)

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
./work/boot/main/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/main

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/main --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Check `reference/repos/rushtherock/game/drones.c`

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 2CF0.s
# Address: 0x800020F0

glabel func_800020F0
    /* 2CF0 800020F0 27BDFF98 */  addiu      $sp, $sp, -0x68
    /* 2CF4 800020F4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 2CF8 800020F8 AFA40068 */  sw         $a0, 0x68($sp)
    /* 2CFC 800020FC 0C001FA0 */  jal        func_80007E80
    /* 2D00 80002100 00000000 */   nop
    /* 2D04 80002104 3C0E00FF */  lui        $t6, (0xFFB000 >> 16)
    /* 2D08 80002108 35CEB000 */  ori        $t6, $t6, (0xFFB000 & 0xFFFF)
    /* 2D0C 8000210C AFAE0060 */  sw         $t6, 0x60($sp)
    /* 2D10 80002110 AFA00064 */  sw         $zero, 0x64($sp)
  .L80002114:
    /* 2D14 80002114 8FAF0064 */  lw         $t7, 0x64($sp)
    /* 2D18 80002118 27B90020 */  addiu      $t9, $sp, 0x20
    /* 2D1C 8000211C 8FA40060 */  lw         $a0, 0x60($sp)
    /* 2D20 80002120 000FC080 */  sll        $t8, $t7, 2
    /* 2D24 80002124 0C002074 */  jal        func_800081D0
    /* 2D28 80002128 03192821 */   addu      $a1, $t8, $t9
    /* 2D2C 8000212C 8FA80064 */  lw         $t0, 0x64($sp)
    /* 2D30 80002130 8FAA0060 */  lw         $t2, 0x60($sp)
    /* 2D34 80002134 25090001 */  addiu      $t1, $t0, 0x1
    /* 2D38 80002138 2D210010 */  sltiu      $at, $t1, 0x10
    /* 2D3C 8000213C 254B0004 */  addiu      $t3, $t2, 0x4
    /* 2D40 80002140 AFAB0060 */  sw         $t3, 0x60($sp)
    /* 2D44 80002144 1420FFF3 */  bnez       $at, .L80002114
    /* 2D48 80002148 AFA90064 */   sw        $t1, 0x64($sp)
    /* 2D4C 8000214C 3C0C8003 */  lui        $t4, %hi(D_8002F660)
    /* 2D50 80002150 258CF660 */  addiu      $t4, $t4, %lo(D_8002F660)
    /* 2D54 80002154 258D0190 */  addiu      $t5, $t4, 0x190
    /* 2D58 80002158 3C048003 */  lui        $a0, %hi(D_80034BA0)
    /* 2D5C 8000215C 3C068000 */  lui        $a2, %hi(func_800021A4)
    /* 2D60 80002160 240E0002 */  addiu      $t6, $zero, 0x2
    /* 2D64 80002164 AFAE0014 */  sw         $t6, 0x14($sp)
    /* 2D68 80002168 24C621A4 */  addiu      $a2, $a2, %lo(func_800021A4)
    /* 2D6C 8000216C 24844BA0 */  addiu      $a0, $a0, %lo(D_80034BA0)
    /* 2D70 80002170 AFAD0010 */  sw         $t5, 0x10($sp)
    /* 2D74 80002174 24050001 */  addiu      $a1, $zero, 0x1
    /* 2D78 80002178 0C001BCC */  jal        func_80006F30
    /* 2D7C 8000217C 8FA70068 */   lw        $a3, 0x68($sp)
    /* 2D80 80002180 3C048003 */  lui        $a0, %hi(D_80034BA0)
    /* 2D84 80002184 0C001C20 */  jal        func_80007080
    /* 2D88 80002188 24844BA0 */   addiu     $a0, $a0, %lo(D_80034BA0)
    /* 2D8C 8000218C 10000001 */  b          .L80002194
    /* 2D90 80002190 00000000 */   nop
  .L80002194:
    /* 2D94 80002194 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 2D98 80002198 27BD0068 */  addiu      $sp, $sp, 0x68
    /* 2D9C 8000219C 03E00008 */  jr         $ra
    /* 2DA0 800021A0 00000000 */   nop
```

## Tips for This Category

