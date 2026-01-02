# osAiSetFrequency

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000BF00` |
| **Category** | `libultra/ai` |
| **Status** | `TODO` |
| **Instructions** | ~81 |

## Description

set AI sample rate

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/ai` category.

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
./work/libultra/ai/osAiSetFrequency/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/ai/osAiSetFrequency

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/ai/osAiSetFrequency --watch
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
# Source: CA70.s
# Address: 0x8000BF00

glabel func_8000BF00
    /* CB00 8000BF00 3C058003 */  lui        $a1, %hi(D_8002C368)
    /* CB04 8000BF04 24A5C368 */  addiu      $a1, $a1, %lo(D_8002C368)
    /* CB08 8000BF08 8CAE0000 */  lw         $t6, 0x0($a1)
    /* CB0C 8000BF0C 44844000 */  mtc1       $a0, $ft2
    /* CB10 8000BF10 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* CB14 8000BF14 448E2000 */  mtc1       $t6, $ft0
    /* CB18 8000BF18 468042A0 */  cvt.s.w    $ft3, $ft2
    /* CB1C 8000BF1C 04810004 */  bgez       $a0, .L8000BF30
    /* CB20 8000BF20 468021A0 */   cvt.s.w   $ft1, $ft0
    /* CB24 8000BF24 44818000 */  mtc1       $at, $ft4
    /* CB28 8000BF28 00000000 */  nop
    /* CB2C 8000BF2C 46105280 */  add.s      $ft3, $ft3, $ft4
  .L8000BF30:
    /* CB30 8000BF30 460A3483 */  div.s      $ft5, $ft1, $ft3
    /* CB34 8000BF34 3C013F00 */  lui        $at, (0x3F000000 >> 16)
    /* CB38 8000BF38 44812000 */  mtc1       $at, $ft0
    /* CB3C 8000BF3C 24030001 */  addiu      $v1, $zero, 0x1
    /* CB40 8000BF40 3C014F00 */  lui        $at, (0x4F000000 >> 16)
    /* CB44 8000BF44 3C08A450 */  lui        $t0, %hi(AI_DACRATE_REG)
    /* CB48 8000BF48 3C0AA450 */  lui        $t2, %hi(AI_BITRATE_REG)
    /* CB4C 8000BF4C 46049000 */  add.s      $fv0, $ft5, $ft0
    /* CB50 8000BF50 444FF800 */  cfc1       $t7, $31
    /* CB54 8000BF54 44C3F800 */  ctc1       $v1, $31
    /* CB58 8000BF58 00000000 */  nop
    /* CB5C 8000BF5C 46000224 */  cvt.w.s    $ft2, $fv0
    /* CB60 8000BF60 4443F800 */  cfc1       $v1, $31
    /* CB64 8000BF64 00000000 */  nop
    /* CB68 8000BF68 30630078 */  andi       $v1, $v1, 0x78
    /* CB6C 8000BF6C 50600013 */  beql       $v1, $zero, .L8000BFBC
    /* CB70 8000BF70 44034000 */   mfc1      $v1, $ft2
    /* CB74 8000BF74 44814000 */  mtc1       $at, $ft2
    /* CB78 8000BF78 24030001 */  addiu      $v1, $zero, 0x1
    /* CB7C 8000BF7C 46080201 */  sub.s      $ft2, $fv0, $ft2
    /* CB80 8000BF80 44C3F800 */  ctc1       $v1, $31
    /* CB84 8000BF84 00000000 */  nop
    /* CB88 8000BF88 46004224 */  cvt.w.s    $ft2, $ft2
    /* CB8C 8000BF8C 4443F800 */  cfc1       $v1, $31
    /* CB90 8000BF90 00000000 */  nop
    /* CB94 8000BF94 30630078 */  andi       $v1, $v1, 0x78
    /* CB98 8000BF98 14600005 */  bnez       $v1, .L8000BFB0
    /* CB9C 8000BF9C 00000000 */   nop
    /* CBA0 8000BFA0 44034000 */  mfc1       $v1, $ft2
    /* CBA4 8000BFA4 3C018000 */  lui        $at, 0x8000
    /* CBA8 8000BFA8 10000007 */  b          .L8000BFC8
    /* CBAC 8000BFAC 00611825 */   or        $v1, $v1, $at
  .L8000BFB0:
    /* CBB0 8000BFB0 10000005 */  b          .L8000BFC8
    /* CBB4 8000BFB4 2403FFFF */   addiu     $v1, $zero, -0x1
    /* CBB8 8000BFB8 44034000 */  mfc1       $v1, $ft2
  .L8000BFBC:
    /* CBBC 8000BFBC 00000000 */  nop
    /* CBC0 8000BFC0 0460FFFB */  bltz       $v1, .L8000BFB0
    /* CBC4 8000BFC4 00000000 */   nop
  .L8000BFC8:
    /* CBC8 8000BFC8 44CFF800 */  ctc1       $t7, $31
    /* CBCC 8000BFCC 2C610084 */  sltiu      $at, $v1, 0x84
    /* CBD0 8000BFD0 10200003 */  beqz       $at, .L8000BFE0
    /* CBD4 8000BFD4 2479FFFF */   addiu     $t9, $v1, -0x1
    /* CBD8 8000BFD8 03E00008 */  jr         $ra
    /* CBDC 8000BFDC 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000BFE0:
    /* CBE0 8000BFE0 24010042 */  addiu      $at, $zero, 0x42
    /* CBE4 8000BFE4 0061001B */  divu       $zero, $v1, $at
    /* CBE8 8000BFE8 00001012 */  mflo       $v0
    /* CBEC 8000BFEC 305800FF */  andi       $t8, $v0, 0xFF
    /* CBF0 8000BFF0 2B010011 */  slti       $at, $t8, 0x11
    /* CBF4 8000BFF4 14200002 */  bnez       $at, .L8000C000
    /* CBF8 8000BFF8 304400FF */   andi      $a0, $v0, 0xFF
    /* CBFC 8000BFFC 24040010 */  addiu      $a0, $zero, 0x10
  .L8000C000:
    /* CC00 8000C000 AD190010 */  sw         $t9, %lo(AI_DACRATE_REG)($t0)
    /* CC04 8000C004 2489FFFF */  addiu      $t1, $a0, -0x1
    /* CC08 8000C008 AD490014 */  sw         $t1, %lo(AI_BITRATE_REG)($t2)
    /* CC0C 8000C00C 8CAB0000 */  lw         $t3, 0x0($a1)
    /* CC10 8000C010 0163001A */  div        $zero, $t3, $v1
    /* CC14 8000C014 00001012 */  mflo       $v0
    /* CC18 8000C018 14600002 */  bnez       $v1, .L8000C024
    /* CC1C 8000C01C 00000000 */   nop
    /* CC20 8000C020 0007000D */  break      7
  .L8000C024:
    /* CC24 8000C024 2401FFFF */  addiu      $at, $zero, -0x1
    /* CC28 8000C028 14610004 */  bne        $v1, $at, .L8000C03C
    /* CC2C 8000C02C 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* CC30 8000C030 15610002 */  bne        $t3, $at, .L8000C03C
    /* CC34 8000C034 00000000 */   nop
    /* CC38 8000C038 0006000D */  break      6
  .L8000C03C:
    /* CC3C 8000C03C 03E00008 */  jr         $ra
    /* CC40 8000C040 00000000 */   nop
```

## Tips for This Category

