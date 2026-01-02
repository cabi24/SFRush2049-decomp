# guLookAtF

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80008EB0` |
| **Category** | `libultra/gu` |
| **Status** | `TODO` |
| **Instructions** | ~174 |

## Description

look-at view matrix (float)

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
./work/libultra/gu/guLookAtF/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/gu/guLookAtF

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/gu/guLookAtF --watch
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
# Source: 9AB0.s
# Address: 0x80008EB0

glabel func_80008EB0
    /* 9AB0 80008EB0 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* 9AB4 80008EB4 AFBF004C */  sw         $ra, 0x4C($sp)
    /* 9AB8 80008EB8 AFB00048 */  sw         $s0, 0x48($sp)
    /* 9ABC 80008EBC 00808025 */  or         $s0, $a0, $zero
    /* 9AC0 80008EC0 F7BE0040 */  sdc1       $fs5, 0x40($sp)
    /* 9AC4 80008EC4 F7BC0038 */  sdc1       $fs4, 0x38($sp)
    /* 9AC8 80008EC8 F7BA0030 */  sdc1       $fs3, 0x30($sp)
    /* 9ACC 80008ECC F7B80028 */  sdc1       $fs2, 0x28($sp)
    /* 9AD0 80008ED0 F7B60020 */  sdc1       $fs1, 0x20($sp)
    /* 9AD4 80008ED4 F7B40018 */  sdc1       $fs0, 0x18($sp)
    /* 9AD8 80008ED8 AFA50054 */  sw         $a1, 0x54($sp)
    /* 9ADC 80008EDC AFA60058 */  sw         $a2, 0x58($sp)
    /* 9AE0 80008EE0 0C0024E5 */  jal        func_80009394
    /* 9AE4 80008EE4 AFA7005C */   sw        $a3, 0x5C($sp)
    /* 9AE8 80008EE8 C7A40060 */  lwc1       $ft0, 0x60($sp)
    /* 9AEC 80008EEC C7A60054 */  lwc1       $ft1, 0x54($sp)
    /* 9AF0 80008EF0 C7A80064 */  lwc1       $ft2, 0x64($sp)
    /* 9AF4 80008EF4 C7AA0058 */  lwc1       $ft3, 0x58($sp)
    /* 9AF8 80008EF8 46062001 */  sub.s      $fv0, $ft0, $ft1
    /* 9AFC 80008EFC C7A6005C */  lwc1       $ft1, 0x5C($sp)
    /* 9B00 80008F00 C7A40068 */  lwc1       $ft0, 0x68($sp)
    /* 9B04 80008F04 460A4581 */  sub.s      $fs1, $ft2, $ft3
    /* 9B08 80008F08 46000202 */  mul.s      $ft2, $fv0, $fv0
    /* 9B0C 80008F0C 46000506 */  mov.s      $fs0, $fv0
    /* 9B10 80008F10 46062601 */  sub.s      $fs2, $ft0, $ft1
    /* 9B14 80008F14 4616B282 */  mul.s      $ft3, $fs1, $fs1
    /* 9B18 80008F18 460A4100 */  add.s      $ft0, $ft2, $ft3
    /* 9B1C 80008F1C 4618C182 */  mul.s      $ft1, $fs2, $fs2
    /* 9B20 80008F20 0C0038F0 */  jal        func_8000E3C0
    /* 9B24 80008F24 46062300 */   add.s     $fa0, $ft0, $ft1
    /* 9B28 80008F28 3C01BFF0 */  lui        $at, (0xBFF00000 >> 16)
    /* 9B2C 80008F2C 44814800 */  mtc1       $at, $ft2f
    /* 9B30 80008F30 44804000 */  mtc1       $zero, $ft2
    /* 9B34 80008F34 460002A1 */  cvt.d.s    $ft3, $fv0
    /* 9B38 80008F38 C7A60070 */  lwc1       $ft1, 0x70($sp)
    /* 9B3C 80008F3C 462A4103 */  div.d      $ft0, $ft2, $ft3
    /* 9B40 80008F40 C7AA0074 */  lwc1       $ft3, 0x74($sp)
    /* 9B44 80008F44 462020A0 */  cvt.s.d    $fv1, $ft0
    /* 9B48 80008F48 4602A502 */  mul.s      $fs0, $fs0, $fv1
    /* 9B4C 80008F4C 00000000 */  nop
    /* 9B50 80008F50 4602B582 */  mul.s      $fs1, $fs1, $fv1
    /* 9B54 80008F54 00000000 */  nop
    /* 9B58 80008F58 4602C602 */  mul.s      $fs2, $fs2, $fv1
    /* 9B5C 80008F5C 00000000 */  nop
    /* 9B60 80008F60 46183202 */  mul.s      $ft2, $ft1, $fs2
    /* 9B64 80008F64 00000000 */  nop
    /* 9B68 80008F68 46165102 */  mul.s      $ft0, $ft3, $fs1
    /* 9B6C 80008F6C 46044681 */  sub.s      $fs3, $ft2, $ft0
    /* 9B70 80008F70 46145202 */  mul.s      $ft2, $ft3, $fs0
    /* 9B74 80008F74 C7A4006C */  lwc1       $ft0, 0x6C($sp)
    /* 9B78 80008F78 46182282 */  mul.s      $ft3, $ft0, $fs2
    /* 9B7C 80008F7C 460A4701 */  sub.s      $fs4, $ft2, $ft3
    /* 9B80 80008F80 46162202 */  mul.s      $ft2, $ft0, $fs1
    /* 9B84 80008F84 00000000 */  nop
    /* 9B88 80008F88 46143282 */  mul.s      $ft3, $ft1, $fs0
    /* 9B8C 80008F8C 460A4781 */  sub.s      $fs5, $ft2, $ft3
    /* 9B90 80008F90 461AD102 */  mul.s      $ft0, $fs3, $fs3
    /* 9B94 80008F94 00000000 */  nop
    /* 9B98 80008F98 461CE182 */  mul.s      $ft1, $fs4, $fs4
    /* 9B9C 80008F9C 46062200 */  add.s      $ft2, $ft0, $ft1
    /* 9BA0 80008FA0 461EF282 */  mul.s      $ft3, $fs5, $fs5
    /* 9BA4 80008FA4 0C0038F0 */  jal        func_8000E3C0
    /* 9BA8 80008FA8 460A4300 */   add.s     $fa0, $ft2, $ft3
    /* 9BAC 80008FAC 3C013FF0 */  lui        $at, (0x3FF00000 >> 16)
    /* 9BB0 80008FB0 44812800 */  mtc1       $at, $ft0f
    /* 9BB4 80008FB4 44802000 */  mtc1       $zero, $ft0
    /* 9BB8 80008FB8 460001A1 */  cvt.d.s    $ft1, $fv0
    /* 9BBC 80008FBC 46262203 */  div.d      $ft2, $ft0, $ft1
    /* 9BC0 80008FC0 462040A0 */  cvt.s.d    $fv1, $ft2
    /* 9BC4 80008FC4 4602D682 */  mul.s      $fs3, $fs3, $fv1
    /* 9BC8 80008FC8 00000000 */  nop
    /* 9BCC 80008FCC 4602E702 */  mul.s      $fs4, $fs4, $fv1
    /* 9BD0 80008FD0 00000000 */  nop
    /* 9BD4 80008FD4 4602F782 */  mul.s      $fs5, $fs5, $fv1
    /* 9BD8 80008FD8 00000000 */  nop
    /* 9BDC 80008FDC 461EB282 */  mul.s      $ft3, $fs1, $fs5
    /* 9BE0 80008FE0 00000000 */  nop
    /* 9BE4 80008FE4 461CC102 */  mul.s      $ft0, $fs2, $fs4
    /* 9BE8 80008FE8 00000000 */  nop
    /* 9BEC 80008FEC 461AC182 */  mul.s      $ft1, $fs2, $fs3
    /* 9BF0 80008FF0 00000000 */  nop
    /* 9BF4 80008FF4 461EA202 */  mul.s      $ft2, $fs0, $fs5
    /* 9BF8 80008FF8 46045381 */  sub.s      $fa1, $ft3, $ft0
    /* 9BFC 80008FFC 461CA282 */  mul.s      $ft3, $fs0, $fs4
    /* 9C00 80009000 00000000 */  nop
    /* 9C04 80009004 461AB102 */  mul.s      $ft0, $fs1, $fs3
    /* 9C08 80009008 E7AE006C */  swc1       $fa1, 0x6C($sp)
    /* 9C0C 8000900C 46083401 */  sub.s      $ft4, $ft1, $ft2
    /* 9C10 80009010 460E7182 */  mul.s      $ft1, $fa1, $fa1
    /* 9C14 80009014 00000000 */  nop
    /* 9C18 80009018 46108202 */  mul.s      $ft2, $ft4, $ft4
    /* 9C1C 8000901C E7B00070 */  swc1       $ft4, 0x70($sp)
    /* 9C20 80009020 46045481 */  sub.s      $ft5, $ft3, $ft0
    /* 9C24 80009024 46129102 */  mul.s      $ft0, $ft5, $ft5
    /* 9C28 80009028 46083280 */  add.s      $ft3, $ft1, $ft2
    /* 9C2C 8000902C E7B20074 */  swc1       $ft5, 0x74($sp)
    /* 9C30 80009030 0C0038F0 */  jal        func_8000E3C0
    /* 9C34 80009034 46045300 */   add.s     $fa0, $ft3, $ft0
    /* 9C38 80009038 3C013FF0 */  lui        $at, (0x3FF00000 >> 16)
    /* 9C3C 8000903C 44813800 */  mtc1       $at, $ft1f
    /* 9C40 80009040 44803000 */  mtc1       $zero, $ft1
    /* 9C44 80009044 46000221 */  cvt.d.s    $ft2, $fv0
    /* 9C48 80009048 C7AE006C */  lwc1       $fa1, 0x6C($sp)
    /* 9C4C 8000904C 46283283 */  div.d      $ft3, $ft1, $ft2
    /* 9C50 80009050 C7AC0070 */  lwc1       $fa0, 0x70($sp)
    /* 9C54 80009054 C7A40074 */  lwc1       $ft0, 0x74($sp)
    /* 9C58 80009058 3C013F80 */  lui        $at, (0x3F800000 >> 16)
    /* 9C5C 8000905C 462050A0 */  cvt.s.d    $fv1, $ft3
    /* 9C60 80009060 46027382 */  mul.s      $fa1, $fa1, $fv1
    /* 9C64 80009064 00000000 */  nop
    /* 9C68 80009068 46026302 */  mul.s      $fa0, $fa0, $fv1
    /* 9C6C 8000906C 00000000 */  nop
    /* 9C70 80009070 46022182 */  mul.s      $ft1, $ft0, $fv1
    /* 9C74 80009074 E7A60074 */  swc1       $ft1, 0x74($sp)
    /* 9C78 80009078 E61A0000 */  swc1       $fs3, 0x0($s0)
    /* 9C7C 8000907C E61C0010 */  swc1       $fs4, 0x10($s0)
    /* 9C80 80009080 E61E0020 */  swc1       $fs5, 0x20($s0)
    /* 9C84 80009084 C7A80054 */  lwc1       $ft2, 0x54($sp)
    /* 9C88 80009088 C7A40058 */  lwc1       $ft0, 0x58($sp)
    /* 9C8C 8000908C 461A4282 */  mul.s      $ft3, $ft2, $fs3
    /* 9C90 80009090 00000000 */  nop
    /* 9C94 80009094 461C2182 */  mul.s      $ft1, $ft0, $fs4
    /* 9C98 80009098 C7A4005C */  lwc1       $ft0, 0x5C($sp)
    /* 9C9C 8000909C E60E0004 */  swc1       $fa1, 0x4($s0)
    /* 9CA0 800090A0 E60C0014 */  swc1       $fa0, 0x14($s0)
    /* 9CA4 800090A4 46065200 */  add.s      $ft2, $ft3, $ft1
    /* 9CA8 800090A8 461E2282 */  mul.s      $ft3, $ft0, $fs5
    /* 9CAC 800090AC 460A4180 */  add.s      $ft1, $ft2, $ft3
    /* 9CB0 800090B0 46003107 */  neg.s      $ft0, $ft1
    /* 9CB4 800090B4 E6040030 */  swc1       $ft0, 0x30($s0)
    /* 9CB8 800090B8 C7A00074 */  lwc1       $fv0, 0x74($sp)
    /* 9CBC 800090BC E7AC0070 */  swc1       $fa0, 0x70($sp)
    /* 9CC0 800090C0 E7AE006C */  swc1       $fa1, 0x6C($sp)
    /* 9CC4 800090C4 C7AE0054 */  lwc1       $fa1, 0x54($sp)
    /* 9CC8 800090C8 C7AC0058 */  lwc1       $fa0, 0x58($sp)
    /* 9CCC 800090CC C7A2005C */  lwc1       $fv1, 0x5C($sp)
    /* 9CD0 800090D0 E6000024 */  swc1       $fv0, 0x24($s0)
    /* 9CD4 800090D4 C7A8006C */  lwc1       $ft2, 0x6C($sp)
    /* 9CD8 800090D8 C7A60070 */  lwc1       $ft1, 0x70($sp)
    /* 9CDC 800090DC E6140008 */  swc1       $fs0, 0x8($s0)
    /* 9CE0 800090E0 46087282 */  mul.s      $ft3, $fa1, $ft2
    /* 9CE4 800090E4 E6160018 */  swc1       $fs1, 0x18($s0)
    /* 9CE8 800090E8 E6180028 */  swc1       $fs2, 0x28($s0)
    /* 9CEC 800090EC 46066102 */  mul.s      $ft0, $fa0, $ft1
    /* 9CF0 800090F0 46045200 */  add.s      $ft2, $ft3, $ft0
    /* 9CF4 800090F4 46001182 */  mul.s      $ft1, $fv1, $fv0
    /* 9CF8 800090F8 44800000 */  mtc1       $zero, $fv0
    /* 9CFC 800090FC 00000000 */  nop
    /* 9D00 80009100 E600000C */  swc1       $fv0, 0xC($s0)
    /* 9D04 80009104 E600001C */  swc1       $fv0, 0x1C($s0)
    /* 9D08 80009108 E600002C */  swc1       $fv0, 0x2C($s0)
    /* 9D0C 8000910C 46064280 */  add.s      $ft3, $ft2, $ft1
    /* 9D10 80009110 46147202 */  mul.s      $ft2, $fa1, $fs0
    /* 9D14 80009114 00000000 */  nop
    /* 9D18 80009118 46166182 */  mul.s      $ft1, $fa0, $fs1
    /* 9D1C 8000911C 46005107 */  neg.s      $ft0, $ft3
    /* 9D20 80009120 E6040034 */  swc1       $ft0, 0x34($s0)
    /* 9D24 80009124 46181102 */  mul.s      $ft0, $fv1, $fs2
    /* 9D28 80009128 46064280 */  add.s      $ft3, $ft2, $ft1
    /* 9D2C 8000912C 46045200 */  add.s      $ft2, $ft3, $ft0
    /* 9D30 80009130 44815000 */  mtc1       $at, $ft3
    /* 9D34 80009134 46004187 */  neg.s      $ft1, $ft2
    /* 9D38 80009138 E60A003C */  swc1       $ft3, 0x3C($s0)
    /* 9D3C 8000913C E6060038 */  swc1       $ft1, 0x38($s0)
    /* 9D40 80009140 8FBF004C */  lw         $ra, 0x4C($sp)
    /* 9D44 80009144 8FB00048 */  lw         $s0, 0x48($sp)
    /* 9D48 80009148 D7BE0040 */  ldc1       $fs5, 0x40($sp)
    /* 9D4C 8000914C D7BC0038 */  ldc1       $fs4, 0x38($sp)
    /* 9D50 80009150 D7BA0030 */  ldc1       $fs3, 0x30($sp)
    /* 9D54 80009154 D7B80028 */  ldc1       $fs2, 0x28($sp)
    /* 9D58 80009158 D7B60020 */  ldc1       $fs1, 0x20($sp)
    /* 9D5C 8000915C D7B40018 */  ldc1       $fs0, 0x18($sp)
    /* 9D60 80009160 03E00008 */  jr         $ra
    /* 9D64 80009164 27BD0050 */   addiu     $sp, $sp, 0x50
```

## Tips for This Category

