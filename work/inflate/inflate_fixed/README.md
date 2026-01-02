# inflate_fixed

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80005B7C` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~114 |

## Description

handle fixed Huffman block (type 1)

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `inflate` category.

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
./work/inflate/inflate_fixed/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/inflate_fixed

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/inflate_fixed --watch
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
# Source: 5610.s
# Address: 0x80005B7C

glabel func_80005B7C
    /* 677C 80005B7C 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* 6780 80005B80 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 6784 80005B84 0C00135B */  jal        func_80004D6C
    /* 6788 80005B88 24040480 */   addiu     $a0, $zero, 0x480
    /* 678C 80005B8C AFA20030 */  sw         $v0, 0x30($sp)
    /* 6790 80005B90 00002025 */  or         $a0, $zero, $zero
    /* 6794 80005B94 00401825 */  or         $v1, $v0, $zero
    /* 6798 80005B98 24060090 */  addiu      $a2, $zero, 0x90
    /* 679C 80005B9C 24050008 */  addiu      $a1, $zero, 0x8
  .L80005BA0:
    /* 67A0 80005BA0 24840004 */  addiu      $a0, $a0, 0x4
    /* 67A4 80005BA4 AC650000 */  sw         $a1, 0x0($v1)
    /* 67A8 80005BA8 AC650004 */  sw         $a1, 0x4($v1)
    /* 67AC 80005BAC AC650008 */  sw         $a1, 0x8($v1)
    /* 67B0 80005BB0 AC65000C */  sw         $a1, 0xC($v1)
    /* 67B4 80005BB4 1486FFFA */  bne        $a0, $a2, .L80005BA0
    /* 67B8 80005BB8 24630010 */   addiu     $v1, $v1, 0x10
    /* 67BC 80005BBC 28810100 */  slti       $at, $a0, 0x100
    /* 67C0 80005BC0 10200007 */  beqz       $at, .L80005BE0
    /* 67C4 80005BC4 00000000 */   nop
    /* 67C8 80005BC8 24060009 */  addiu      $a2, $zero, 0x9
  .L80005BCC:
    /* 67CC 80005BCC 24840001 */  addiu      $a0, $a0, 0x1
    /* 67D0 80005BD0 28810100 */  slti       $at, $a0, 0x100
    /* 67D4 80005BD4 AC660000 */  sw         $a2, 0x0($v1)
    /* 67D8 80005BD8 1420FFFC */  bnez       $at, .L80005BCC
    /* 67DC 80005BDC 24630004 */   addiu     $v1, $v1, 0x4
  .L80005BE0:
    /* 67E0 80005BE0 28810118 */  slti       $at, $a0, 0x118
    /* 67E4 80005BE4 10200007 */  beqz       $at, .L80005C04
    /* 67E8 80005BE8 00000000 */   nop
    /* 67EC 80005BEC 24060007 */  addiu      $a2, $zero, 0x7
  .L80005BF0:
    /* 67F0 80005BF0 24840001 */  addiu      $a0, $a0, 0x1
    /* 67F4 80005BF4 28810118 */  slti       $at, $a0, 0x118
    /* 67F8 80005BF8 AC660000 */  sw         $a2, 0x0($v1)
    /* 67FC 80005BFC 1420FFFC */  bnez       $at, .L80005BF0
    /* 6800 80005C00 24630004 */   addiu     $v1, $v1, 0x4
  .L80005C04:
    /* 6804 80005C04 28810120 */  slti       $at, $a0, 0x120
    /* 6808 80005C08 10200006 */  beqz       $at, .L80005C24
    /* 680C 80005C0C 00000000 */   nop
  .L80005C10:
    /* 6810 80005C10 24840001 */  addiu      $a0, $a0, 0x1
    /* 6814 80005C14 28810120 */  slti       $at, $a0, 0x120
    /* 6818 80005C18 AC650000 */  sw         $a1, 0x0($v1)
    /* 681C 80005C1C 1420FFFC */  bnez       $at, .L80005C10
    /* 6820 80005C20 24630004 */   addiu     $v1, $v1, 0x4
  .L80005C24:
    /* 6824 80005C24 3C0F8003 */  lui        $t7, %hi(D_8002B0CC)
    /* 6828 80005C28 25EFB0CC */  addiu      $t7, $t7, %lo(D_8002B0CC)
    /* 682C 80005C2C 240E0007 */  addiu      $t6, $zero, 0x7
    /* 6830 80005C30 3C078003 */  lui        $a3, %hi(D_8002B08C)
    /* 6834 80005C34 27B80040 */  addiu      $t8, $sp, 0x40
    /* 6838 80005C38 27B90038 */  addiu      $t9, $sp, 0x38
    /* 683C 80005C3C AFB90018 */  sw         $t9, 0x18($sp)
    /* 6840 80005C40 AFB80014 */  sw         $t8, 0x14($sp)
    /* 6844 80005C44 24E7B08C */  addiu      $a3, $a3, %lo(D_8002B08C)
    /* 6848 80005C48 AFAE0038 */  sw         $t6, 0x38($sp)
    /* 684C 80005C4C AFAF0010 */  sw         $t7, 0x10($sp)
    /* 6850 80005C50 8FA40030 */  lw         $a0, 0x30($sp)
    /* 6854 80005C54 24050120 */  addiu      $a1, $zero, 0x120
    /* 6858 80005C58 0C001366 */  jal        func_80004D98
    /* 685C 80005C5C 24060101 */   addiu     $a2, $zero, 0x101
    /* 6860 80005C60 10400005 */  beqz       $v0, .L80005C78
    /* 6864 80005C64 00402025 */   or        $a0, $v0, $zero
    /* 6868 80005C68 3C018003 */  lui        $at, %hi(D_800354C8)
    /* 686C 80005C6C AC2054C8 */  sw         $zero, %lo(D_800354C8)($at)
    /* 6870 80005C70 10000030 */  b          .L80005D34
    /* 6874 80005C74 00801025 */   or        $v0, $a0, $zero
  .L80005C78:
    /* 6878 80005C78 00002025 */  or         $a0, $zero, $zero
    /* 687C 80005C7C 8FA20030 */  lw         $v0, 0x30($sp)
    /* 6880 80005C80 24030005 */  addiu      $v1, $zero, 0x5
  .L80005C84:
    /* 6884 80005C84 24840001 */  addiu      $a0, $a0, 0x1
    /* 6888 80005C88 2881001E */  slti       $at, $a0, 0x1E
    /* 688C 80005C8C AC430000 */  sw         $v1, 0x0($v0)
    /* 6890 80005C90 1420FFFC */  bnez       $at, .L80005C84
    /* 6894 80005C94 24420004 */   addiu     $v0, $v0, 0x4
    /* 6898 80005C98 3C098003 */  lui        $t1, %hi(D_8002B148)
    /* 689C 80005C9C 2529B148 */  addiu      $t1, $t1, %lo(D_8002B148)
    /* 68A0 80005CA0 24080005 */  addiu      $t0, $zero, 0x5
    /* 68A4 80005CA4 3C078003 */  lui        $a3, %hi(D_8002B10C)
    /* 68A8 80005CA8 27AA003C */  addiu      $t2, $sp, 0x3C
    /* 68AC 80005CAC 27AB0034 */  addiu      $t3, $sp, 0x34
    /* 68B0 80005CB0 AFAB0018 */  sw         $t3, 0x18($sp)
    /* 68B4 80005CB4 AFAA0014 */  sw         $t2, 0x14($sp)
    /* 68B8 80005CB8 24E7B10C */  addiu      $a3, $a3, %lo(D_8002B10C)
    /* 68BC 80005CBC AFA80034 */  sw         $t0, 0x34($sp)
    /* 68C0 80005CC0 AFA90010 */  sw         $t1, 0x10($sp)
    /* 68C4 80005CC4 8FA40030 */  lw         $a0, 0x30($sp)
    /* 68C8 80005CC8 2405001E */  addiu      $a1, $zero, 0x1E
    /* 68CC 80005CCC 0C001366 */  jal        func_80004D98
    /* 68D0 80005CD0 00003025 */   or        $a2, $zero, $zero
    /* 68D4 80005CD4 28410002 */  slti       $at, $v0, 0x2
    /* 68D8 80005CD8 14200005 */  bnez       $at, .L80005CF0
    /* 68DC 80005CDC 00402025 */   or        $a0, $v0, $zero
    /* 68E0 80005CE0 3C018003 */  lui        $at, %hi(D_800354C8)
    /* 68E4 80005CE4 AC2054C8 */  sw         $zero, %lo(D_800354C8)($at)
    /* 68E8 80005CE8 10000012 */  b          .L80005D34
    /* 68EC 80005CEC 00801025 */   or        $v0, $a0, $zero
  .L80005CF0:
    /* 68F0 80005CF0 8FA40040 */  lw         $a0, 0x40($sp)
    /* 68F4 80005CF4 8FA5003C */  lw         $a1, 0x3C($sp)
    /* 68F8 80005CF8 8FA60038 */  lw         $a2, 0x38($sp)
    /* 68FC 80005CFC 0C001519 */  jal        func_80005464
    /* 6900 80005D00 8FA70034 */   lw        $a3, 0x34($sp)
    /* 6904 80005D04 10400008 */  beqz       $v0, .L80005D28
    /* 6908 80005D08 00000000 */   nop
    /* 690C 80005D0C 04410003 */  bgez       $v0, .L80005D1C
    /* 6910 80005D10 00000000 */   nop
    /* 6914 80005D14 10000002 */  b          .L80005D20
    /* 6918 80005D18 00401825 */   or        $v1, $v0, $zero
  .L80005D1C:
    /* 691C 80005D1C 24030001 */  addiu      $v1, $zero, 0x1
  .L80005D20:
    /* 6920 80005D20 10000004 */  b          .L80005D34
    /* 6924 80005D24 00601025 */   or        $v0, $v1, $zero
  .L80005D28:
    /* 6928 80005D28 3C018003 */  lui        $at, %hi(D_800354C8)
    /* 692C 80005D2C AC2054C8 */  sw         $zero, %lo(D_800354C8)($at)
    /* 6930 80005D30 00001025 */  or         $v0, $zero, $zero
  .L80005D34:
    /* 6934 80005D34 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 6938 80005D38 27BD0048 */  addiu      $sp, $sp, 0x48
    /* 693C 80005D3C 03E00008 */  jr         $ra
    /* 6940 80005D40 00000000 */   nop
```

## Tips for This Category

