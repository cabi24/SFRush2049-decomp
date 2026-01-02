# __osDispatchThread

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D11C` |
| **Category** | `libultra/thread` |
| **Status** | `TODO` |
| **Instructions** | ~97 |

## Description

dispatch next runnable thread

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/thread` category.

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
./work/libultra/thread/__osDispatchThread/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/thread/__osDispatchThread

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/thread/__osDispatchThread --watch
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
# Source: D580.s
# Address: 0x8000D11C

glabel func_8000D11C
    /* DD1C 8000D11C 3C048003 */  lui        $a0, %hi(D_8002C3D8)
    /* DD20 8000D120 0C003443 */  jal        func_8000D10C
    /* DD24 8000D124 2484C3D8 */   addiu     $a0, $a0, %lo(D_8002C3D8)
    /* DD28 8000D128 3C018003 */  lui        $at, %hi(D_8002C3E0)
    /* DD2C 8000D12C AC22C3E0 */  sw         $v0, %lo(D_8002C3E0)($at)
    /* DD30 8000D130 24080004 */  addiu      $t0, $zero, 0x4
    /* DD34 8000D134 A4480010 */  sh         $t0, 0x10($v0)
    /* DD38 8000D138 0040D025 */  or         $k0, $v0, $zero
    /* DD3C 8000D13C 3C088003 */  lui        $t0, %hi(D_8002C370)
    /* DD40 8000D140 8F5B0118 */  lw         $k1, 0x118($k0) /* handwritten instruction */
    /* DD44 8000D144 2508C370 */  addiu      $t0, $t0, %lo(D_8002C370)
    /* DD48 8000D148 8D080000 */  lw         $t0, 0x0($t0)
    /* DD4C 8000D14C 3C01FFFF */  lui        $at, (0xFFFF00FF >> 16)
    /* DD50 8000D150 3369FF00 */  andi       $t1, $k1, 0xFF00 /* handwritten instruction */
    /* DD54 8000D154 342100FF */  ori        $at, $at, (0xFFFF00FF & 0xFFFF)
    /* DD58 8000D158 3108FF00 */  andi       $t0, $t0, 0xFF00
    /* DD5C 8000D15C 01284824 */  and        $t1, $t1, $t0
    /* DD60 8000D160 0361D824 */  and        $k1, $k1, $at
    /* DD64 8000D164 0369D825 */  or         $k1, $k1, $t1
    /* DD68 8000D168 409B6000 */  mtc0       $k1, $12 /* handwritten instruction */
    /* DD6C 8000D16C DF5B0108 */  ld         $k1, 0x108($k0) /* handwritten instruction */
    /* DD70 8000D170 DF410020 */  ld         $at, 0x20($k0) /* handwritten instruction */
    /* DD74 8000D174 DF420028 */  ld         $v0, 0x28($k0) /* handwritten instruction */
    /* DD78 8000D178 03600013 */  mtlo       $k1
    /* DD7C 8000D17C DF5B0110 */  ld         $k1, 0x110($k0) /* handwritten instruction */
    /* DD80 8000D180 DF430030 */  ld         $v1, 0x30($k0) /* handwritten instruction */
    /* DD84 8000D184 DF440038 */  ld         $a0, 0x38($k0) /* handwritten instruction */
    /* DD88 8000D188 DF450040 */  ld         $a1, 0x40($k0) /* handwritten instruction */
    /* DD8C 8000D18C DF460048 */  ld         $a2, 0x48($k0) /* handwritten instruction */
    /* DD90 8000D190 DF470050 */  ld         $a3, 0x50($k0) /* handwritten instruction */
    /* DD94 8000D194 DF480058 */  ld         $t0, 0x58($k0) /* handwritten instruction */
    /* DD98 8000D198 DF490060 */  ld         $t1, 0x60($k0) /* handwritten instruction */
    /* DD9C 8000D19C DF4A0068 */  ld         $t2, 0x68($k0) /* handwritten instruction */
    /* DDA0 8000D1A0 DF4B0070 */  ld         $t3, 0x70($k0) /* handwritten instruction */
    /* DDA4 8000D1A4 DF4C0078 */  ld         $t4, 0x78($k0) /* handwritten instruction */
    /* DDA8 8000D1A8 DF4D0080 */  ld         $t5, 0x80($k0) /* handwritten instruction */
    /* DDAC 8000D1AC DF4E0088 */  ld         $t6, 0x88($k0) /* handwritten instruction */
    /* DDB0 8000D1B0 DF4F0090 */  ld         $t7, 0x90($k0) /* handwritten instruction */
    /* DDB4 8000D1B4 DF500098 */  ld         $s0, 0x98($k0) /* handwritten instruction */
    /* DDB8 8000D1B8 DF5100A0 */  ld         $s1, 0xA0($k0) /* handwritten instruction */
    /* DDBC 8000D1BC DF5200A8 */  ld         $s2, 0xA8($k0) /* handwritten instruction */
    /* DDC0 8000D1C0 DF5300B0 */  ld         $s3, 0xB0($k0) /* handwritten instruction */
    /* DDC4 8000D1C4 DF5400B8 */  ld         $s4, 0xB8($k0) /* handwritten instruction */
    /* DDC8 8000D1C8 DF5500C0 */  ld         $s5, 0xC0($k0) /* handwritten instruction */
    /* DDCC 8000D1CC DF5600C8 */  ld         $s6, 0xC8($k0) /* handwritten instruction */
    /* DDD0 8000D1D0 DF5700D0 */  ld         $s7, 0xD0($k0) /* handwritten instruction */
    /* DDD4 8000D1D4 DF5800D8 */  ld         $t8, 0xD8($k0) /* handwritten instruction */
    /* DDD8 8000D1D8 DF5900E0 */  ld         $t9, 0xE0($k0) /* handwritten instruction */
    /* DDDC 8000D1DC DF5C00E8 */  ld         $gp, 0xE8($k0) /* handwritten instruction */
    /* DDE0 8000D1E0 03600011 */  mthi       $k1
    /* DDE4 8000D1E4 DF5D00F0 */  ld         $sp, 0xF0($k0) /* handwritten instruction */
    /* DDE8 8000D1E8 DF5E00F8 */  ld         $fp, 0xF8($k0) /* handwritten instruction */
    /* DDEC 8000D1EC DF5F0100 */  ld         $ra, 0x100($k0) /* handwritten instruction */
    /* DDF0 8000D1F0 8F5B011C */  lw         $k1, 0x11C($k0) /* handwritten instruction */
    /* DDF4 8000D1F4 409B7000 */  mtc0       $k1, $14 /* handwritten instruction */
    /* DDF8 8000D1F8 8F5B0018 */  lw         $k1, 0x18($k0) /* handwritten instruction */
    /* DDFC 8000D1FC 13600013 */  beqz       $k1, .L8000D24C /* handwritten instruction */
    /* DE00 8000D200 00000000 */   nop
    /* DE04 8000D204 8F5B012C */  lw         $k1, 0x12C($k0) /* handwritten instruction */
    /* DE08 8000D208 44DBF800 */  ctc1       $k1, $31
    /* DE0C 8000D20C D7400130 */  ldc1       $fv0, 0x130($k0)
    /* DE10 8000D210 D7420138 */  ldc1       $fv1, 0x138($k0)
    /* DE14 8000D214 D7440140 */  ldc1       $ft0, 0x140($k0)
    /* DE18 8000D218 D7460148 */  ldc1       $ft1, 0x148($k0)
    /* DE1C 8000D21C D7480150 */  ldc1       $ft2, 0x150($k0)
    /* DE20 8000D220 D74A0158 */  ldc1       $ft3, 0x158($k0)
    /* DE24 8000D224 D74C0160 */  ldc1       $fa0, 0x160($k0)
    /* DE28 8000D228 D74E0168 */  ldc1       $fa1, 0x168($k0)
    /* DE2C 8000D22C D7500170 */  ldc1       $ft4, 0x170($k0)
    /* DE30 8000D230 D7520178 */  ldc1       $ft5, 0x178($k0)
    /* DE34 8000D234 D7540180 */  ldc1       $fs0, 0x180($k0)
    /* DE38 8000D238 D7560188 */  ldc1       $fs1, 0x188($k0)
    /* DE3C 8000D23C D7580190 */  ldc1       $fs2, 0x190($k0)
    /* DE40 8000D240 D75A0198 */  ldc1       $fs3, 0x198($k0)
    /* DE44 8000D244 D75C01A0 */  ldc1       $fs4, 0x1A0($k0)
    /* DE48 8000D248 D75E01A8 */  ldc1       $fs5, 0x1A8($k0)
  .L8000D24C:
    /* DE4C 8000D24C 8F5B0128 */  lw         $k1, 0x128($k0) /* handwritten instruction */
    /* DE50 8000D250 3C1A8003 */  lui        $k0, %hi(D_8002C370) /* handwritten instruction */
    /* DE54 8000D254 275AC370 */  addiu      $k0, $k0, %lo(D_8002C370) /* handwritten instruction */
    /* DE58 8000D258 8F5A0000 */  lw         $k0, 0x0($k0) /* handwritten instruction */
    /* DE5C 8000D25C 001AD402 */  srl        $k0, $k0, 16
    /* DE60 8000D260 037AD824 */  and        $k1, $k1, $k0
    /* DE64 8000D264 001BD840 */  sll        $k1, $k1, 1
    /* DE68 8000D268 3C1A8003 */  lui        $k0, %hi(D_8002D6D0) /* handwritten instruction */
    /* DE6C 8000D26C 275AD6D0 */  addiu      $k0, $k0, %lo(D_8002D6D0) /* handwritten instruction */
    /* DE70 8000D270 037AD821 */  addu       $k1, $k1, $k0
    /* DE74 8000D274 977B0000 */  lhu        $k1, 0x0($k1) /* handwritten instruction */
    /* DE78 8000D278 3C1AA430 */  lui        $k0, %hi(MI_INTR_MASK_REG) /* handwritten instruction */
    /* DE7C 8000D27C 275A000C */  addiu      $k0, $k0, %lo(MI_INTR_MASK_REG) /* handwritten instruction */
    /* DE80 8000D280 AF5B0000 */  sw         $k1, 0x0($k0) /* handwritten instruction */
    /* DE84 8000D284 00000000 */  nop
    /* DE88 8000D288 00000000 */  nop
    /* DE8C 8000D28C 00000000 */  nop
    /* DE90 8000D290 00000000 */  nop
    /* DE94 8000D294 42000018 */  eret /* handwritten instruction */
  alabel D_8000D298
    /* DE98 8000D298 0C003EE8 */  jal        func_8000FBA0
    /* DE9C 8000D29C 00002025 */   or        $a0, $zero, $zero
```

## Tips for This Category

