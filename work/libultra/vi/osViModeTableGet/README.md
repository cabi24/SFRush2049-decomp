# osViModeTableGet

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007840` |
| **Category** | `libultra/vi` |
| **Status** | `TODO` |
| **Instructions** | ~67 |

## Description

get VI mode from table

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/vi` category.

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
./work/libultra/vi/osViModeTableGet/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/vi/osViModeTableGet

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/vi/osViModeTableGet --watch
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
# Source: 8440.s
# Address: 0x80007840

glabel func_80007840
    /* 8440 80007840 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 8444 80007844 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8448 80007848 3C058003 */  lui        $a1, %hi(D_80036790)
    /* 844C 8000784C 24A56790 */  addiu      $a1, $a1, %lo(D_80036790)
    /* 8450 80007850 0C0034AC */  jal        func_8000D2B0
    /* 8454 80007854 24060040 */   addiu     $a2, $zero, 0x40
    /* 8458 80007858 3C048003 */  lui        $a0, %hi(D_800367A0)
    /* 845C 8000785C 8C8467A0 */  lw         $a0, %lo(D_800367A0)($a0)
    /* 8460 80007860 10800005 */  beqz       $a0, .L80007878
    /* 8464 80007864 00000000 */   nop
    /* 8468 80007868 0C003570 */  jal        func_8000D5C0
    /* 846C 8000786C 00000000 */   nop
    /* 8470 80007870 3C018003 */  lui        $at, %hi(D_800367A0)
    /* 8474 80007874 AC2267A0 */  sw         $v0, %lo(D_800367A0)($at)
  .L80007878:
    /* 8478 80007878 3C048003 */  lui        $a0, %hi(D_800367A8)
    /* 847C 8000787C 8C8467A8 */  lw         $a0, %lo(D_800367A8)($a0)
    /* 8480 80007880 10800005 */  beqz       $a0, .L80007898
    /* 8484 80007884 00000000 */   nop
    /* 8488 80007888 0C003570 */  jal        func_8000D5C0
    /* 848C 8000788C 00000000 */   nop
    /* 8490 80007890 3C018003 */  lui        $at, %hi(D_800367A8)
    /* 8494 80007894 AC2267A8 */  sw         $v0, %lo(D_800367A8)($at)
  .L80007898:
    /* 8498 80007898 3C048003 */  lui        $a0, %hi(D_800367B0)
    /* 849C 8000789C 8C8467B0 */  lw         $a0, %lo(D_800367B0)($a0)
    /* 84A0 800078A0 10800005 */  beqz       $a0, .L800078B8
    /* 84A4 800078A4 00000000 */   nop
    /* 84A8 800078A8 0C003570 */  jal        func_8000D5C0
    /* 84AC 800078AC 00000000 */   nop
    /* 84B0 800078B0 3C018003 */  lui        $at, %hi(D_800367B0)
    /* 84B4 800078B4 AC2267B0 */  sw         $v0, %lo(D_800367B0)($at)
  .L800078B8:
    /* 84B8 800078B8 3C048003 */  lui        $a0, %hi(D_800367B8)
    /* 84BC 800078BC 8C8467B8 */  lw         $a0, %lo(D_800367B8)($a0)
    /* 84C0 800078C0 10800005 */  beqz       $a0, .L800078D8
    /* 84C4 800078C4 00000000 */   nop
    /* 84C8 800078C8 0C003570 */  jal        func_8000D5C0
    /* 84CC 800078CC 00000000 */   nop
    /* 84D0 800078D0 3C018003 */  lui        $at, %hi(D_800367B8)
    /* 84D4 800078D4 AC2267B8 */  sw         $v0, %lo(D_800367B8)($at)
  .L800078D8:
    /* 84D8 800078D8 3C048003 */  lui        $a0, %hi(D_800367BC)
    /* 84DC 800078DC 8C8467BC */  lw         $a0, %lo(D_800367BC)($a0)
    /* 84E0 800078E0 10800005 */  beqz       $a0, .L800078F8
    /* 84E4 800078E4 00000000 */   nop
    /* 84E8 800078E8 0C003570 */  jal        func_8000D5C0
    /* 84EC 800078EC 00000000 */   nop
    /* 84F0 800078F0 3C018003 */  lui        $at, %hi(D_800367BC)
    /* 84F4 800078F4 AC2267BC */  sw         $v0, %lo(D_800367BC)($at)
  .L800078F8:
    /* 84F8 800078F8 3C048003 */  lui        $a0, %hi(D_800367C0)
    /* 84FC 800078FC 8C8467C0 */  lw         $a0, %lo(D_800367C0)($a0)
    /* 8500 80007900 10800005 */  beqz       $a0, .L80007918
    /* 8504 80007904 00000000 */   nop
    /* 8508 80007908 0C003570 */  jal        func_8000D5C0
    /* 850C 8000790C 00000000 */   nop
    /* 8510 80007910 3C018003 */  lui        $at, %hi(D_800367C0)
    /* 8514 80007914 AC2267C0 */  sw         $v0, %lo(D_800367C0)($at)
  .L80007918:
    /* 8518 80007918 3C048003 */  lui        $a0, %hi(D_800367C8)
    /* 851C 8000791C 8C8467C8 */  lw         $a0, %lo(D_800367C8)($a0)
    /* 8520 80007920 50800006 */  beql       $a0, $zero, .L8000793C
    /* 8524 80007924 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 8528 80007928 0C003570 */  jal        func_8000D5C0
    /* 852C 8000792C 00000000 */   nop
    /* 8530 80007930 3C018003 */  lui        $at, %hi(D_800367C8)
    /* 8534 80007934 AC2267C8 */  sw         $v0, %lo(D_800367C8)($at)
    /* 8538 80007938 8FBF0014 */  lw         $ra, 0x14($sp)
  .L8000793C:
    /* 853C 8000793C 3C028003 */  lui        $v0, %hi(D_80036790)
    /* 8540 80007940 24426790 */  addiu      $v0, $v0, %lo(D_80036790)
    /* 8544 80007944 03E00008 */  jr         $ra
    /* 8548 80007948 27BD0018 */   addiu     $sp, $sp, 0x18
```

## Tips for This Category

