# osJamMesg

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800075E0` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~83 |

## Description

insert message at front of queue

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
./work/libultra/os/osJamMesg/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osJamMesg

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osJamMesg --watch
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
# Source: 81E0.s
# Address: 0x800075E0

glabel func_800075E0
    /* 81E0 800075E0 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* 81E4 800075E4 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 81E8 800075E8 AFA40038 */  sw         $a0, 0x38($sp)
    /* 81EC 800075EC AFA5003C */  sw         $a1, 0x3C($sp)
    /* 81F0 800075F0 AFA60040 */  sw         $a2, 0x40($sp)
    /* 81F4 800075F4 AFB20020 */  sw         $s2, 0x20($sp)
    /* 81F8 800075F8 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 81FC 800075FC 0C00312C */  jal        func_8000C4B0
    /* 8200 80007600 AFB00018 */   sw        $s0, 0x18($sp)
    /* 8204 80007604 8FAE0038 */  lw         $t6, 0x38($sp)
    /* 8208 80007608 00408025 */  or         $s0, $v0, $zero
    /* 820C 8000760C 8DCF0008 */  lw         $t7, 0x8($t6)
    /* 8210 80007610 8DD80010 */  lw         $t8, 0x10($t6)
    /* 8214 80007614 01F8082A */  slt        $at, $t7, $t8
    /* 8218 80007618 14200018 */  bnez       $at, .L8000767C
    /* 821C 8000761C 00000000 */   nop
  .L80007620:
    /* 8220 80007620 8FB90040 */  lw         $t9, 0x40($sp)
    /* 8224 80007624 24010001 */  addiu      $at, $zero, 0x1
    /* 8228 80007628 1721000A */  bne        $t9, $at, .L80007654
    /* 822C 8000762C 00000000 */   nop
    /* 8230 80007630 3C098003 */  lui        $t1, %hi(D_8002C3E0)
    /* 8234 80007634 8D29C3E0 */  lw         $t1, %lo(D_8002C3E0)($t1)
    /* 8238 80007638 24080008 */  addiu      $t0, $zero, 0x8
    /* 823C 8000763C A5280010 */  sh         $t0, 0x10($t1)
    /* 8240 80007640 8FA40038 */  lw         $a0, 0x38($sp)
    /* 8244 80007644 0C0033F1 */  jal        func_8000CFC4
    /* 8248 80007648 24840004 */   addiu     $a0, $a0, 0x4
    /* 824C 8000764C 10000005 */  b          .L80007664
    /* 8250 80007650 00000000 */   nop
  .L80007654:
    /* 8254 80007654 0C003148 */  jal        func_8000C520
    /* 8258 80007658 02002025 */   or        $a0, $s0, $zero
    /* 825C 8000765C 1000002D */  b          .L80007714
    /* 8260 80007660 2402FFFF */   addiu     $v0, $zero, -0x1
  .L80007664:
    /* 8264 80007664 8FAA0038 */  lw         $t2, 0x38($sp)
    /* 8268 80007668 8D4B0008 */  lw         $t3, 0x8($t2)
    /* 826C 8000766C 8D4C0010 */  lw         $t4, 0x10($t2)
    /* 8270 80007670 016C082A */  slt        $at, $t3, $t4
    /* 8274 80007674 1020FFEA */  beqz       $at, .L80007620
    /* 8278 80007678 00000000 */   nop
  .L8000767C:
    /* 827C 8000767C 8FAD0038 */  lw         $t5, 0x38($sp)
    /* 8280 80007680 8FA8003C */  lw         $t0, 0x3C($sp)
    /* 8284 80007684 8DAE000C */  lw         $t6, 0xC($t5)
    /* 8288 80007688 8DAF0008 */  lw         $t7, 0x8($t5)
    /* 828C 8000768C 8DB90010 */  lw         $t9, 0x10($t5)
    /* 8290 80007690 8DA90014 */  lw         $t1, 0x14($t5)
    /* 8294 80007694 01CFC021 */  addu       $t8, $t6, $t7
    /* 8298 80007698 0319001A */  div        $zero, $t8, $t9
    /* 829C 8000769C 00008810 */  mfhi       $s1
    /* 82A0 800076A0 00115080 */  sll        $t2, $s1, 2
    /* 82A4 800076A4 012A5821 */  addu       $t3, $t1, $t2
    /* 82A8 800076A8 AD680000 */  sw         $t0, 0x0($t3)
    /* 82AC 800076AC 8FAC0038 */  lw         $t4, 0x38($sp)
    /* 82B0 800076B0 17200002 */  bnez       $t9, .L800076BC
    /* 82B4 800076B4 00000000 */   nop
    /* 82B8 800076B8 0007000D */  break      7
  .L800076BC:
    /* 82BC 800076BC 2401FFFF */  addiu      $at, $zero, -0x1
    /* 82C0 800076C0 17210004 */  bne        $t9, $at, .L800076D4
    /* 82C4 800076C4 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 82C8 800076C8 17010002 */  bne        $t8, $at, .L800076D4
    /* 82CC 800076CC 00000000 */   nop
    /* 82D0 800076D0 0006000D */  break      6
  .L800076D4:
    /* 82D4 800076D4 8D8E0008 */  lw         $t6, 0x8($t4)
    /* 82D8 800076D8 25CF0001 */  addiu      $t7, $t6, 0x1
    /* 82DC 800076DC AD8F0008 */  sw         $t7, 0x8($t4)
    /* 82E0 800076E0 8FB80038 */  lw         $t8, 0x38($sp)
    /* 82E4 800076E4 8F190000 */  lw         $t9, 0x0($t8)
    /* 82E8 800076E8 8F2D0000 */  lw         $t5, 0x0($t9)
    /* 82EC 800076EC 11A00006 */  beqz       $t5, .L80007708
    /* 82F0 800076F0 00000000 */   nop
    /* 82F4 800076F4 0C003443 */  jal        func_8000D10C
    /* 82F8 800076F8 03002025 */   or        $a0, $t8, $zero
    /* 82FC 800076FC 00409025 */  or         $s2, $v0, $zero
    /* 8300 80007700 0C001C20 */  jal        func_80007080
    /* 8304 80007704 02402025 */   or        $a0, $s2, $zero
  .L80007708:
    /* 8308 80007708 0C003148 */  jal        func_8000C520
    /* 830C 8000770C 02002025 */   or        $a0, $s0, $zero
    /* 8310 80007710 00001025 */  or         $v0, $zero, $zero
  .L80007714:
    /* 8314 80007714 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 8318 80007718 8FB00018 */  lw         $s0, 0x18($sp)
    /* 831C 8000771C 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 8320 80007720 8FB20020 */  lw         $s2, 0x20($sp)
    /* 8324 80007724 03E00008 */  jr         $ra
    /* 8328 80007728 27BD0038 */   addiu     $sp, $sp, 0x38
```

## Tips for This Category

