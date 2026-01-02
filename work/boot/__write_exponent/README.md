# __write_exponent

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80004878` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~70 |

## Description

write exponent string (e+XX or e-XX)

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
./work/boot/__write_exponent/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/__write_exponent

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/__write_exponent --watch
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
# Address: 0x80004878

glabel func_80004878
    /* 5478 80004878 27BDFEC8 */  addiu      $sp, $sp, -0x138
    /* 547C 8000487C A0860000 */  sb         $a2, 0x0($a0)
    /* 5480 80004880 24840001 */  addiu      $a0, $a0, 0x1
    /* 5484 80004884 04A10006 */  bgez       $a1, .L800048A0
    /* 5488 80004888 00000000 */   nop
    /* 548C 8000488C 00052823 */  negu       $a1, $a1
    /* 5490 80004890 240E002D */  addiu      $t6, $zero, 0x2D
    /* 5494 80004894 A08E0000 */  sb         $t6, 0x0($a0)
    /* 5498 80004898 10000004 */  b          .L800048AC
    /* 549C 8000489C 24840001 */   addiu     $a0, $a0, 0x1
  .L800048A0:
    /* 54A0 800048A0 240F002B */  addiu      $t7, $zero, 0x2B
    /* 54A4 800048A4 A08F0000 */  sb         $t7, 0x0($a0)
    /* 54A8 800048A8 24840001 */  addiu      $a0, $a0, 0x1
  .L800048AC:
    /* 54AC 800048AC 27B80000 */  addiu      $t8, $sp, 0x0
    /* 54B0 800048B0 27190134 */  addiu      $t9, $t8, 0x134
    /* 54B4 800048B4 AFB90134 */  sw         $t9, 0x134($sp)
    /* 54B8 800048B8 28A1000A */  slti       $at, $a1, 0xA
    /* 54BC 800048BC 14200028 */  bnez       $at, .L80004960
    /* 54C0 800048C0 00000000 */   nop
  .L800048C4:
    /* 54C4 800048C4 2401000A */  addiu      $at, $zero, 0xA
    /* 54C8 800048C8 00A1001A */  div        $zero, $a1, $at
    /* 54CC 800048CC 8FA80134 */  lw         $t0, 0x134($sp)
    /* 54D0 800048D0 00005010 */  mfhi       $t2
    /* 54D4 800048D4 254B0030 */  addiu      $t3, $t2, 0x30
    /* 54D8 800048D8 2509FFFF */  addiu      $t1, $t0, -0x1
    /* 54DC 800048DC AFA90134 */  sw         $t1, 0x134($sp)
    /* 54E0 800048E0 A12B0000 */  sb         $t3, 0x0($t1)
    /* 54E4 800048E4 2401000A */  addiu      $at, $zero, 0xA
    /* 54E8 800048E8 00A1001A */  div        $zero, $a1, $at
    /* 54EC 800048EC 00006012 */  mflo       $t4
    /* 54F0 800048F0 01802825 */  or         $a1, $t4, $zero
    /* 54F4 800048F4 28A1000A */  slti       $at, $a1, 0xA
    /* 54F8 800048F8 1020FFF2 */  beqz       $at, .L800048C4
    /* 54FC 800048FC 00000000 */   nop
    /* 5500 80004900 8FAD0134 */  lw         $t5, 0x134($sp)
    /* 5504 80004904 24AF0030 */  addiu      $t7, $a1, 0x30
    /* 5508 80004908 25AEFFFF */  addiu      $t6, $t5, -0x1
    /* 550C 8000490C AFAE0134 */  sw         $t6, 0x134($sp)
    /* 5510 80004910 A1CF0000 */  sb         $t7, 0x0($t6)
    /* 5514 80004914 8FB80134 */  lw         $t8, 0x134($sp)
    /* 5518 80004918 27B90000 */  addiu      $t9, $sp, 0x0
    /* 551C 8000491C 27280134 */  addiu      $t0, $t9, 0x134
    /* 5520 80004920 0308082B */  sltu       $at, $t8, $t0
    /* 5524 80004924 1020000C */  beqz       $at, .L80004958
    /* 5528 80004928 00000000 */   nop
  .L8000492C:
    /* 552C 8000492C 8FAA0134 */  lw         $t2, 0x134($sp)
    /* 5530 80004930 27AD0000 */  addiu      $t5, $sp, 0x0
    /* 5534 80004934 25AF0134 */  addiu      $t7, $t5, 0x134
    /* 5538 80004938 914B0000 */  lbu        $t3, 0x0($t2)
    /* 553C 8000493C 24840001 */  addiu      $a0, $a0, 0x1
    /* 5540 80004940 A08BFFFF */  sb         $t3, -0x1($a0)
    /* 5544 80004944 8FA90134 */  lw         $t1, 0x134($sp)
    /* 5548 80004948 252C0001 */  addiu      $t4, $t1, 0x1
    /* 554C 8000494C 018F082B */  sltu       $at, $t4, $t7
    /* 5550 80004950 1420FFF6 */  bnez       $at, .L8000492C
    /* 5554 80004954 AFAC0134 */   sw        $t4, 0x134($sp)
  .L80004958:
    /* 5558 80004958 10000007 */  b          .L80004978
    /* 555C 8000495C 00000000 */   nop
  .L80004960:
    /* 5560 80004960 240E0030 */  addiu      $t6, $zero, 0x30
    /* 5564 80004964 A08E0000 */  sb         $t6, 0x0($a0)
    /* 5568 80004968 24840001 */  addiu      $a0, $a0, 0x1
    /* 556C 8000496C 24B90030 */  addiu      $t9, $a1, 0x30
    /* 5570 80004970 A0990000 */  sb         $t9, 0x0($a0)
    /* 5574 80004974 24840001 */  addiu      $a0, $a0, 0x1
  .L80004978:
    /* 5578 80004978 10000003 */  b          .L80004988
    /* 557C 8000497C 00801025 */   or        $v0, $a0, $zero
    /* 5580 80004980 10000001 */  b          .L80004988
    /* 5584 80004984 00000000 */   nop
  .L80004988:
    /* 5588 80004988 03E00008 */  jr         $ra
    /* 558C 8000498C 27BD0138 */   addiu     $sp, $sp, 0x138
```

## Tips for This Category

