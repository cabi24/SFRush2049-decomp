# __scMain

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000063C` |
| **Category** | `scheduler` |
| **Status** | `TODO` |
| **Instructions** | ~81 |

## Description

scheduler thread main loop (handles events 0x29A-0x29E)

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
./work/scheduler/__scMain/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/scheduler/__scMain

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/scheduler/__scMain --watch
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
# Source: 1050.s
# Address: 0x8000063C

glabel func_8000063C
    /* 123C 8000063C 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 1240 80000640 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1244 80000644 AFA40028 */  sw         $a0, 0x28($sp)
    /* 1248 80000648 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 124C 8000064C AFAE0020 */  sw         $t6, 0x20($sp)
  .L80000650:
    /* 1250 80000650 8FA40020 */  lw         $a0, 0x20($sp)
    /* 1254 80000654 27A50024 */  addiu      $a1, $sp, 0x24
    /* 1258 80000658 24060001 */  addiu      $a2, $zero, 0x1
    /* 125C 8000065C 0C001C9C */  jal        func_80007270
    /* 1260 80000660 24840040 */   addiu     $a0, $a0, 0x40
    /* 1264 80000664 8FAF0024 */  lw         $t7, 0x24($sp)
    /* 1268 80000668 25F8FD66 */  addiu      $t8, $t7, -0x29A
    /* 126C 8000066C 2F010005 */  sltiu      $at, $t8, 0x5
    /* 1270 80000670 1020003B */  beqz       $at, .L80000760
    /* 1274 80000674 00000000 */   nop
    /* 1278 80000678 0018C080 */  sll        $t8, $t8, 2
    /* 127C 8000067C 3C018003 */  lui        $at, %hi(jtbl_8002D490_main)
    /* 1280 80000680 00380821 */  addu       $at, $at, $t8
    /* 1284 80000684 8C38D490 */  lw         $t8, %lo(jtbl_8002D490_main)($at)
    /* 1288 80000688 03000008 */  jr         $t8
    /* 128C 8000068C 00000000 */   nop
    /* 1290 80000690 0C001CEC */  jal        func_800073B0
    /* 1294 80000694 00000000 */   nop
    /* 1298 80000698 3C018003 */  lui        $at, %hi(D_8002EB80)
    /* 129C 8000069C AC22EB80 */  sw         $v0, %lo(D_8002EB80)($at)
    /* 12A0 800006A0 AC23EB84 */  sw         $v1, %lo(D_8002EB84)($at)
    /* 12A4 800006A4 0C000220 */  jal        func_80000880
    /* 12A8 800006A8 8FA40020 */   lw        $a0, 0x20($sp)
    /* 12AC 800006AC 0C0001E0 */  jal        func_80000780
    /* 12B0 800006B0 8FA40020 */   lw        $a0, 0x20($sp)
    /* 12B4 800006B4 1000002A */  b          .L80000760
    /* 12B8 800006B8 00000000 */   nop
    /* 12BC 800006BC 0C000254 */  jal        func_80000950
    /* 12C0 800006C0 8FA40020 */   lw        $a0, 0x20($sp)
    /* 12C4 800006C4 10000026 */  b          .L80000760
    /* 12C8 800006C8 00000000 */   nop
    /* 12CC 800006CC 0C001CEC */  jal        func_800073B0
    /* 12D0 800006D0 00000000 */   nop
    /* 12D4 800006D4 3C088003 */  lui        $t0, %hi(D_8002E8E0)
    /* 12D8 800006D8 8D08E8E0 */  lw         $t0, %lo(D_8002E8E0)($t0)
    /* 12DC 800006DC 3C098003 */  lui        $t1, %hi(D_8002E8E4)
    /* 12E0 800006E0 8D29E8E4 */  lw         $t1, %lo(D_8002E8E4)($t1)
    /* 12E4 800006E4 00485023 */  subu       $t2, $v0, $t0
    /* 12E8 800006E8 0069082B */  sltu       $at, $v1, $t1
    /* 12EC 800006EC 01415023 */  subu       $t2, $t2, $at
    /* 12F0 800006F0 00695823 */  subu       $t3, $v1, $t1
    /* 12F4 800006F4 0160C825 */  or         $t9, $t3, $zero
    /* 12F8 800006F8 3C018012 */  lui        $at, %hi(D_80124F70)
    /* 12FC 800006FC AC394F70 */  sw         $t9, %lo(D_80124F70)($at)
    /* 1300 80000700 0C0002B3 */  jal        func_80000ACC
    /* 1304 80000704 8FA40020 */   lw        $a0, 0x20($sp)
    /* 1308 80000708 10000015 */  b          .L80000760
    /* 130C 8000070C 00000000 */   nop
    /* 1310 80000710 0C0001E0 */  jal        func_80000780
    /* 1314 80000714 8FA40020 */   lw        $a0, 0x20($sp)
    /* 1318 80000718 10000011 */  b          .L80000760
    /* 131C 8000071C 00000000 */   nop
    /* 1320 80000720 8FAC0020 */  lw         $t4, 0x20($sp)
    /* 1324 80000724 8D8D0260 */  lw         $t5, 0x260($t4)
    /* 1328 80000728 11A0000B */  beqz       $t5, .L80000758
    /* 132C 8000072C AFAD001C */   sw        $t5, 0x1C($sp)
  .L80000730:
    /* 1330 80000730 8FAE001C */  lw         $t6, 0x1C($sp)
    /* 1334 80000734 8FA50020 */  lw         $a1, 0x20($sp)
    /* 1338 80000738 24060001 */  addiu      $a2, $zero, 0x1
    /* 133C 8000073C 8DC40004 */  lw         $a0, 0x4($t6)
    /* 1340 80000740 0C001D10 */  jal        func_80007440
    /* 1344 80000744 24A50020 */   addiu     $a1, $a1, 0x20
    /* 1348 80000748 8FAF001C */  lw         $t7, 0x1C($sp)
    /* 134C 8000074C 8DF80000 */  lw         $t8, 0x0($t7)
    /* 1350 80000750 1700FFF7 */  bnez       $t8, .L80000730
    /* 1354 80000754 AFB8001C */   sw        $t8, 0x1C($sp)
  .L80000758:
    /* 1358 80000758 10000001 */  b          .L80000760
    /* 135C 8000075C 00000000 */   nop
  .L80000760:
    /* 1360 80000760 1000FFBB */  b          .L80000650
    /* 1364 80000764 00000000 */   nop
    /* 1368 80000768 10000001 */  b          .L80000770
    /* 136C 8000076C 00000000 */   nop
  .L80000770:
    /* 1370 80000770 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1374 80000774 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 1378 80000778 03E00008 */  jr         $ra
    /* 137C 8000077C 00000000 */   nop
```

## Tips for This Category

