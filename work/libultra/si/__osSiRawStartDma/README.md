# __osSiRawStartDma

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000E4B0` |
| **Category** | `libultra/si` |
| **Status** | `TODO` |
| **Instructions** | ~43 |

## Description

SI raw DMA to/from PIF

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/si` category.

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
./work/libultra/si/__osSiRawStartDma/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/si/__osSiRawStartDma

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/si/__osSiRawStartDma --watch
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
# Source: F0B0.s
# Address: 0x8000E4B0

glabel func_8000E4B0
    /* F0B0 8000E4B0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* F0B4 8000E4B4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* F0B8 8000E4B8 AFA5001C */  sw         $a1, 0x1C($sp)
    /* F0BC 8000E4BC 3C0EA480 */  lui        $t6, %hi(SI_STATUS_REG)
    /* F0C0 8000E4C0 8DCF0018 */  lw         $t7, %lo(SI_STATUS_REG)($t6)
    /* F0C4 8000E4C4 00803025 */  or         $a2, $a0, $zero
    /* F0C8 8000E4C8 24010001 */  addiu      $at, $zero, 0x1
    /* F0CC 8000E4CC 31F80003 */  andi       $t8, $t7, 0x3
    /* F0D0 8000E4D0 13000003 */  beqz       $t8, .L8000E4E0
    /* F0D4 8000E4D4 00000000 */   nop
    /* F0D8 8000E4D8 1000001C */  b          .L8000E54C
    /* F0DC 8000E4DC 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000E4E0:
    /* F0E0 8000E4E0 14C10005 */  bne        $a2, $at, .L8000E4F8
    /* F0E4 8000E4E4 8FA4001C */   lw        $a0, 0x1C($sp)
    /* F0E8 8000E4E8 24050040 */  addiu      $a1, $zero, 0x40
    /* F0EC 8000E4EC 0C001F28 */  jal        func_80007CA0
    /* F0F0 8000E4F0 AFA60018 */   sw        $a2, 0x18($sp)
    /* F0F4 8000E4F4 8FA60018 */  lw         $a2, 0x18($sp)
  .L8000E4F8:
    /* F0F8 8000E4F8 8FA4001C */  lw         $a0, 0x1C($sp)
    /* F0FC 8000E4FC 0C003570 */  jal        func_8000D5C0
    /* F100 8000E500 AFA60018 */   sw        $a2, 0x18($sp)
    /* F104 8000E504 8FA60018 */  lw         $a2, 0x18($sp)
    /* F108 8000E508 3C19A480 */  lui        $t9, %hi(SI_DRAM_ADDR_REG)
    /* F10C 8000E50C AF220000 */  sw         $v0, %lo(SI_DRAM_ADDR_REG)($t9)
    /* F110 8000E510 14C00006 */  bnez       $a2, .L8000E52C
    /* F114 8000E514 3C0A1FC0 */   lui       $t2, (0x1FC007C0 >> 16)
    /* F118 8000E518 3C081FC0 */  lui        $t0, (0x1FC007C0 >> 16)
    /* F11C 8000E51C 350807C0 */  ori        $t0, $t0, (0x1FC007C0 & 0xFFFF)
    /* F120 8000E520 3C09A480 */  lui        $t1, %hi(SI_PIF_ADDR_RD64B_REG)
    /* F124 8000E524 10000004 */  b          .L8000E538
    /* F128 8000E528 AD280004 */   sw        $t0, %lo(SI_PIF_ADDR_RD64B_REG)($t1)
  .L8000E52C:
    /* F12C 8000E52C 354A07C0 */  ori        $t2, $t2, (0x1FC007C0 & 0xFFFF)
    /* F130 8000E530 3C0BA480 */  lui        $t3, %hi(SI_PIF_ADDR_WR64B_REG)
    /* F134 8000E534 AD6A0010 */  sw         $t2, %lo(SI_PIF_ADDR_WR64B_REG)($t3)
  .L8000E538:
    /* F138 8000E538 14C00003 */  bnez       $a2, .L8000E548
    /* F13C 8000E53C 8FA4001C */   lw        $a0, 0x1C($sp)
    /* F140 8000E540 0C002138 */  jal        func_800084E0
    /* F144 8000E544 24050040 */   addiu     $a1, $zero, 0x40
  .L8000E548:
    /* F148 8000E548 00001025 */  or         $v0, $zero, $zero
  .L8000E54C:
    /* F14C 8000E54C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* F150 8000E550 27BD0018 */  addiu      $sp, $sp, 0x18
    /* F154 8000E554 03E00008 */  jr         $ra
    /* F158 8000E558 00000000 */   nop
```

## Tips for This Category

