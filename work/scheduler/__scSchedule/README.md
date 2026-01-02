# __scSchedule

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80000780` |
| **Category** | `scheduler` |
| **Status** | `TODO` |
| **Instructions** | ~64 |

## Description

drain command queue, append tasks to pending lists

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `scheduler` category.

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
./work/scheduler/__scSchedule/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/scheduler/__scSchedule

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/scheduler/__scSchedule --watch
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
# Source: 1050.s
# Address: 0x80000780

glabel func_80000780
    /* 1380 80000780 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 1384 80000784 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1388 80000788 AFA40028 */  sw         $a0, 0x28($sp)
    /* 138C 8000078C AFA0001C */  sw         $zero, 0x1C($sp)
    /* 1390 80000790 AFA00018 */  sw         $zero, 0x18($sp)
    /* 1394 80000794 8FA40028 */  lw         $a0, 0x28($sp)
    /* 1398 80000798 27A50024 */  addiu      $a1, $sp, 0x24
    /* 139C 8000079C 00003025 */  or         $a2, $zero, $zero
    /* 13A0 800007A0 0C001C9C */  jal        func_80007270
    /* 13A4 800007A4 24840078 */   addiu     $a0, $a0, 0x78
    /* 13A8 800007A8 2401FFFF */  addiu      $at, $zero, -0x1
    /* 13AC 800007AC 1041000C */  beq        $v0, $at, .L800007E0
    /* 13B0 800007B0 00000000 */   nop
  .L800007B4:
    /* 13B4 800007B4 8FA40028 */  lw         $a0, 0x28($sp)
    /* 13B8 800007B8 0C00035B */  jal        func_80000D6C
    /* 13BC 800007BC 8FA50024 */   lw        $a1, 0x24($sp)
    /* 13C0 800007C0 8FA40028 */  lw         $a0, 0x28($sp)
    /* 13C4 800007C4 27A50024 */  addiu      $a1, $sp, 0x24
    /* 13C8 800007C8 00003025 */  or         $a2, $zero, $zero
    /* 13CC 800007CC 0C001C9C */  jal        func_80007270
    /* 13D0 800007D0 24840078 */   addiu     $a0, $a0, 0x78
    /* 13D4 800007D4 2401FFFF */  addiu      $at, $zero, -0x1
    /* 13D8 800007D8 1441FFF6 */  bne        $v0, $at, .L800007B4
    /* 13DC 800007DC 00000000 */   nop
  .L800007E0:
    /* 13E0 800007E0 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 13E4 800007E4 8DCF0280 */  lw         $t7, 0x280($t6)
    /* 13E8 800007E8 11E0000B */  beqz       $t7, .L80000818
    /* 13EC 800007EC 00000000 */   nop
    /* 13F0 800007F0 8DD80264 */  lw         $t8, 0x264($t6)
    /* 13F4 800007F4 13000008 */  beqz       $t8, .L80000818
    /* 13F8 800007F8 00000000 */   nop
    /* 13FC 800007FC 8DD90274 */  lw         $t9, 0x274($t6)
    /* 1400 80000800 13200005 */  beqz       $t9, .L80000818
    /* 1404 80000804 00000000 */   nop
    /* 1408 80000808 0C0003E2 */  jal        func_80000F88
    /* 140C 8000080C 8FA40028 */   lw        $a0, 0x28($sp)
    /* 1410 80000810 10000015 */  b          .L80000868
    /* 1414 80000814 00000000 */   nop
  .L80000818:
    /* 1418 80000818 8FA80028 */  lw         $t0, 0x28($sp)
    /* 141C 8000081C 8D090274 */  lw         $t1, 0x274($t0)
    /* 1420 80000820 8D0C0278 */  lw         $t4, 0x278($t0)
    /* 1424 80000824 2D2A0001 */  sltiu      $t2, $t1, 0x1
    /* 1428 80000828 000A5840 */  sll        $t3, $t2, 1
    /* 142C 8000082C 2D8D0001 */  sltiu      $t5, $t4, 0x1
    /* 1430 80000830 016D7825 */  or         $t7, $t3, $t5
    /* 1434 80000834 AFAF0020 */  sw         $t7, 0x20($sp)
    /* 1438 80000838 8FA40028 */  lw         $a0, 0x28($sp)
    /* 143C 8000083C 27A5001C */  addiu      $a1, $sp, 0x1C
    /* 1440 80000840 27A60018 */  addiu      $a2, $sp, 0x18
    /* 1444 80000844 0C0003FA */  jal        func_80000FE8
    /* 1448 80000848 8FA70020 */   lw        $a3, 0x20($sp)
    /* 144C 8000084C 8FB80020 */  lw         $t8, 0x20($sp)
    /* 1450 80000850 10580005 */  beq        $v0, $t8, .L80000868
    /* 1454 80000854 00000000 */   nop
    /* 1458 80000858 8FA40028 */  lw         $a0, 0x28($sp)
    /* 145C 8000085C 8FA5001C */  lw         $a1, 0x1C($sp)
    /* 1460 80000860 0C000388 */  jal        func_80000E20
    /* 1464 80000864 8FA60018 */   lw        $a2, 0x18($sp)
  .L80000868:
    /* 1468 80000868 10000001 */  b          .L80000870
    /* 146C 8000086C 00000000 */   nop
  .L80000870:
    /* 1470 80000870 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1474 80000874 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 1478 80000878 03E00008 */  jr         $ra
    /* 147C 8000087C 00000000 */   nop
```

## Tips for This Category

