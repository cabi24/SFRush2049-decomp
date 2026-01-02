# __osPiRawStartDma

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80008630` |
| **Category** | `libultra/pi` |
| **Status** | `TODO` |
| **Instructions** | ~47 |

## Description

PI raw DMA request via message queue

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/pi` category.

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
./work/libultra/pi/__osPiRawStartDma/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pi/__osPiRawStartDma

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pi/__osPiRawStartDma --watch
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
# Source: 9230.s
# Address: 0x80008630

glabel func_80008630
    /* 9230 80008630 3C0E8003 */  lui        $t6, %hi(D_8002C380)
    /* 9234 80008634 8DCEC380 */  lw         $t6, %lo(D_8002C380)($t6)
    /* 9238 80008638 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 923C 8000863C AFB00018 */  sw         $s0, 0x18($sp)
    /* 9240 80008640 00808025 */  or         $s0, $a0, $zero
    /* 9244 80008644 15C00003 */  bnez       $t6, .L80008654
    /* 9248 80008648 AFBF001C */   sw        $ra, 0x1C($sp)
    /* 924C 8000864C 10000022 */  b          .L800086D8
    /* 9250 80008650 2402FFFF */   addiu     $v0, $zero, -0x1
  .L80008654:
    /* 9254 80008654 14C00004 */  bnez       $a2, .L80008668
    /* 9258 80008658 2418000C */   addiu     $t8, $zero, 0xC
    /* 925C 8000865C 240F000B */  addiu      $t7, $zero, 0xB
    /* 9260 80008660 10000002 */  b          .L8000866C
    /* 9264 80008664 A60F0000 */   sh        $t7, 0x0($s0)
  .L80008668:
    /* 9268 80008668 A6180000 */  sh         $t8, 0x0($s0)
  .L8000866C:
    /* 926C 8000866C A2050002 */  sb         $a1, 0x2($s0)
    /* 9270 80008670 8FB90038 */  lw         $t9, 0x38($sp)
    /* 9274 80008674 24010001 */  addiu      $at, $zero, 0x1
    /* 9278 80008678 AE190004 */  sw         $t9, 0x4($s0)
    /* 927C 8000867C 8FA80030 */  lw         $t0, 0x30($sp)
    /* 9280 80008680 AE07000C */  sw         $a3, 0xC($s0)
    /* 9284 80008684 AE080008 */  sw         $t0, 0x8($s0)
    /* 9288 80008688 8FA90034 */  lw         $t1, 0x34($sp)
    /* 928C 8000868C AE000014 */  sw         $zero, 0x14($s0)
    /* 9290 80008690 14A10009 */  bne        $a1, $at, .L800086B8
    /* 9294 80008694 AE090010 */   sw        $t1, 0x10($s0)
    /* 9298 80008698 0C0038E4 */  jal        func_8000E390
    /* 929C 8000869C 00000000 */   nop
    /* 92A0 800086A0 00402025 */  or         $a0, $v0, $zero
    /* 92A4 800086A4 02002825 */  or         $a1, $s0, $zero
    /* 92A8 800086A8 0C001D10 */  jal        func_80007440
    /* 92AC 800086AC 00003025 */   or        $a2, $zero, $zero
    /* 92B0 800086B0 10000008 */  b          .L800086D4
    /* 92B4 800086B4 00401825 */   or        $v1, $v0, $zero
  .L800086B8:
    /* 92B8 800086B8 0C0038E4 */  jal        func_8000E390
    /* 92BC 800086BC 00000000 */   nop
    /* 92C0 800086C0 00402025 */  or         $a0, $v0, $zero
    /* 92C4 800086C4 02002825 */  or         $a1, $s0, $zero
    /* 92C8 800086C8 0C001D78 */  jal        func_800075E0
    /* 92CC 800086CC 00003025 */   or        $a2, $zero, $zero
    /* 92D0 800086D0 00401825 */  or         $v1, $v0, $zero
  .L800086D4:
    /* 92D4 800086D4 00601025 */  or         $v0, $v1, $zero
  .L800086D8:
    /* 92D8 800086D8 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 92DC 800086DC 8FB00018 */  lw         $s0, 0x18($sp)
    /* 92E0 800086E0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 92E4 800086E4 03E00008 */  jr         $ra
    /* 92E8 800086E8 00000000 */   nop
```

## Tips for This Category

