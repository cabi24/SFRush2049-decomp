# guOrthoF

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80008A60` |
| **Category** | `libultra/gu` |
| **Status** | `TODO` |
| **Instructions** | ~85 |

## Description

orthographic projection matrix (float)

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/gu` category.

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
./work/libultra/gu/guOrthoF/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/gu/guOrthoF

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/gu/guOrthoF --watch
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
# Source: 9660.s
# Address: 0x80008A60

glabel func_80008A60
    /* 9660 80008A60 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 9664 80008A64 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 9668 80008A68 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 966C 80008A6C AFA60020 */  sw         $a2, 0x20($sp)
    /* 9670 80008A70 AFA70024 */  sw         $a3, 0x24($sp)
    /* 9674 80008A74 0C0024E5 */  jal        func_80009394
    /* 9678 80008A78 AFA40018 */   sw        $a0, 0x18($sp)
    /* 967C 80008A7C C7A40020 */  lwc1       $ft0, 0x20($sp)
    /* 9680 80008A80 C7A6001C */  lwc1       $ft1, 0x1C($sp)
    /* 9684 80008A84 3C014000 */  lui        $at, (0x40000000 >> 16)
    /* 9688 80008A88 44814000 */  mtc1       $at, $ft2
    /* 968C 80008A8C 46062001 */  sub.s      $fv0, $ft0, $ft1
    /* 9690 80008A90 8FA20018 */  lw         $v0, 0x18($sp)
    /* 9694 80008A94 C7AE0028 */  lwc1       $fa1, 0x28($sp)
    /* 9698 80008A98 C7B00030 */  lwc1       $ft4, 0x30($sp)
    /* 969C 80008A9C 46004283 */  div.s      $ft3, $ft2, $fv0
    /* 96A0 80008AA0 C7B2002C */  lwc1       $ft5, 0x2C($sp)
    /* 96A4 80008AA4 44813000 */  mtc1       $at, $ft1
    /* 96A8 80008AA8 3C01C000 */  lui        $at, (0xC0000000 >> 16)
    /* 96AC 80008AAC 46128301 */  sub.s      $fa0, $ft4, $ft5
    /* 96B0 80008AB0 00001825 */  or         $v1, $zero, $zero
    /* 96B4 80008AB4 24040004 */  addiu      $a0, $zero, 0x4
    /* 96B8 80008AB8 E44A0000 */  swc1       $ft3, 0x0($v0)
    /* 96BC 80008ABC C7A40024 */  lwc1       $ft0, 0x24($sp)
    /* 96C0 80008AC0 44815000 */  mtc1       $at, $ft3
    /* 96C4 80008AC4 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 96C8 80008AC8 46047081 */  sub.s      $fv1, $fa1, $ft0
    /* 96CC 80008ACC 460C5103 */  div.s      $ft0, $ft3, $fa0
    /* 96D0 80008AD0 46023203 */  div.s      $ft2, $ft1, $fv1
    /* 96D4 80008AD4 E4440028 */  swc1       $ft0, 0x28($v0)
    /* 96D8 80008AD8 E4480014 */  swc1       $ft2, 0x14($v0)
    /* 96DC 80008ADC C7A8001C */  lwc1       $ft2, 0x1C($sp)
    /* 96E0 80008AE0 C7A60020 */  lwc1       $ft1, 0x20($sp)
    /* 96E4 80008AE4 46083280 */  add.s      $ft3, $ft1, $ft2
    /* 96E8 80008AE8 46005107 */  neg.s      $ft0, $ft3
    /* 96EC 80008AEC 46002183 */  div.s      $ft1, $ft0, $fv0
    /* 96F0 80008AF0 E4460030 */  swc1       $ft1, 0x30($v0)
    /* 96F4 80008AF4 C7A80024 */  lwc1       $ft2, 0x24($sp)
    /* 96F8 80008AF8 46087280 */  add.s      $ft3, $fa1, $ft2
    /* 96FC 80008AFC 46128200 */  add.s      $ft2, $ft4, $ft5
    /* 9700 80008B00 46005107 */  neg.s      $ft0, $ft3
    /* 9704 80008B04 46004287 */  neg.s      $ft3, $ft2
    /* 9708 80008B08 46022183 */  div.s      $ft1, $ft0, $fv1
    /* 970C 80008B0C 460C5103 */  div.s      $ft0, $ft3, $fa0
    /* 9710 80008B10 E4460034 */  swc1       $ft1, 0x34($v0)
    /* 9714 80008B14 44813000 */  mtc1       $at, $ft1
    /* 9718 80008B18 00000000 */  nop
    /* 971C 80008B1C E446003C */  swc1       $ft1, 0x3C($v0)
    /* 9720 80008B20 E4440038 */  swc1       $ft0, 0x38($v0)
    /* 9724 80008B24 C7A00034 */  lwc1       $fv0, 0x34($sp)
    /* 9728 80008B28 C4480000 */  lwc1       $ft2, 0x0($v0)
    /* 972C 80008B2C 24630001 */  addiu      $v1, $v1, 0x1
    /* 9730 80008B30 C4520004 */  lwc1       $ft5, 0x4($v0)
    /* 9734 80008B34 46004302 */  mul.s      $fa0, $ft2, $fv0
    /* 9738 80008B38 C44E0008 */  lwc1       $fa1, 0x8($v0)
    /* 973C 80008B3C 10640010 */  beq        $v1, $a0, .L80008B80
    /* 9740 80008B40 C450000C */   lwc1      $ft4, 0xC($v0)
  .L80008B44:
    /* 9744 80008B44 46009282 */  mul.s      $ft3, $ft5, $fv0
    /* 9748 80008B48 C4480010 */  lwc1       $ft2, 0x10($v0)
    /* 974C 80008B4C C4520014 */  lwc1       $ft5, 0x14($v0)
    /* 9750 80008B50 46007182 */  mul.s      $ft1, $fa1, $fv0
    /* 9754 80008B54 C44E0018 */  lwc1       $fa1, 0x18($v0)
    /* 9758 80008B58 24630001 */  addiu      $v1, $v1, 0x1
    /* 975C 80008B5C 46008102 */  mul.s      $ft0, $ft4, $fv0
    /* 9760 80008B60 C450001C */  lwc1       $ft4, 0x1C($v0)
    /* 9764 80008B64 E44C0000 */  swc1       $fa0, 0x0($v0)
    /* 9768 80008B68 46004302 */  mul.s      $fa0, $ft2, $fv0
    /* 976C 80008B6C E44A0004 */  swc1       $ft3, 0x4($v0)
    /* 9770 80008B70 E4460008 */  swc1       $ft1, 0x8($v0)
    /* 9774 80008B74 24420010 */  addiu      $v0, $v0, 0x10
    /* 9778 80008B78 1464FFF2 */  bne        $v1, $a0, .L80008B44
    /* 977C 80008B7C E444FFFC */   swc1      $ft0, -0x4($v0)
  .L80008B80:
    /* 9780 80008B80 46009282 */  mul.s      $ft3, $ft5, $fv0
    /* 9784 80008B84 24420010 */  addiu      $v0, $v0, 0x10
    /* 9788 80008B88 E44CFFF0 */  swc1       $fa0, -0x10($v0)
    /* 978C 80008B8C 46007182 */  mul.s      $ft1, $fa1, $fv0
    /* 9790 80008B90 00000000 */  nop
    /* 9794 80008B94 46008102 */  mul.s      $ft0, $ft4, $fv0
    /* 9798 80008B98 E44AFFF4 */  swc1       $ft3, -0xC($v0)
    /* 979C 80008B9C E446FFF8 */  swc1       $ft1, -0x8($v0)
    /* 97A0 80008BA0 E444FFFC */  swc1       $ft0, -0x4($v0)
    /* 97A4 80008BA4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 97A8 80008BA8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 97AC 80008BAC 03E00008 */  jr         $ra
    /* 97B0 80008BB0 00000000 */   nop
```

## Tips for This Category

