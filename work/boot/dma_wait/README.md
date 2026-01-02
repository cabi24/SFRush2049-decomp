# dma_wait

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80002598` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~37 |

## Description

wait for DMA completion

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
./work/boot/dma_wait/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/dma_wait

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/dma_wait --watch
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
# Source: 3140.s
# Address: 0x80002598

glabel func_80002598
    /* 3198 80002598 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 319C 8000259C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 31A0 800025A0 AFA40020 */  sw         $a0, 0x20($sp)
    /* 31A4 800025A4 3C0E8003 */  lui        $t6, %hi(D_8002B030)
    /* 31A8 800025A8 81CEB030 */  lb         $t6, %lo(D_8002B030)($t6)
    /* 31AC 800025AC 15C00003 */  bnez       $t6, .L800025BC
    /* 31B0 800025B0 00000000 */   nop
    /* 31B4 800025B4 0C000950 */  jal        func_80002540
    /* 31B8 800025B8 00000000 */   nop
  .L800025BC:
    /* 31BC 800025BC 8FAF0020 */  lw         $t7, 0x20($sp)
    /* 31C0 800025C0 11E00008 */  beqz       $t7, .L800025E4
    /* 31C4 800025C4 00000000 */   nop
    /* 31C8 800025C8 3C048003 */  lui        $a0, %hi(D_8002F190)
    /* 31CC 800025CC 2484F190 */  addiu      $a0, $a0, %lo(D_8002F190)
    /* 31D0 800025D0 27A5001C */  addiu      $a1, $sp, 0x1C
    /* 31D4 800025D4 0C001C9C */  jal        func_80007270
    /* 31D8 800025D8 24060001 */   addiu     $a2, $zero, 0x1
    /* 31DC 800025DC 1000000B */  b          .L8000260C
    /* 31E0 800025E0 00000000 */   nop
  .L800025E4:
    /* 31E4 800025E4 3C048003 */  lui        $a0, %hi(D_8002F190)
    /* 31E8 800025E8 2484F190 */  addiu      $a0, $a0, %lo(D_8002F190)
    /* 31EC 800025EC 27A5001C */  addiu      $a1, $sp, 0x1C
    /* 31F0 800025F0 0C001C9C */  jal        func_80007270
    /* 31F4 800025F4 00003025 */   or        $a2, $zero, $zero
    /* 31F8 800025F8 2401FFFF */  addiu      $at, $zero, -0x1
    /* 31FC 800025FC 14410003 */  bne        $v0, $at, .L8000260C
    /* 3200 80002600 00000000 */   nop
    /* 3204 80002604 10000005 */  b          .L8000261C
    /* 3208 80002608 00001025 */   or        $v0, $zero, $zero
  .L8000260C:
    /* 320C 8000260C 10000003 */  b          .L8000261C
    /* 3210 80002610 24020001 */   addiu     $v0, $zero, 0x1
    /* 3214 80002614 10000001 */  b          .L8000261C
    /* 3218 80002618 00000000 */   nop
  .L8000261C:
    /* 321C 8000261C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 3220 80002620 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 3224 80002624 03E00008 */  jr         $ra
    /* 3228 80002628 00000000 */   nop
```

## Tips for This Category

