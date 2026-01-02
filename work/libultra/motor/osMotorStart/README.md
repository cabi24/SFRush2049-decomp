# osMotorStart

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000A194` |
| **Category** | `libultra/motor` |
| **Status** | `TODO` |
| **Instructions** | ~87 |

## Description

start rumble pak motor (writes 0x01 to 0xC000)

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/motor` category.

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
./work/libultra/motor/osMotorStart/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/motor/osMotorStart

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/motor/osMotorStart --watch
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
# Source: AB20.s
# Address: 0x8000A194

glabel func_8000A194
    /* AD94 8000A194 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* AD98 8000A198 AFBF001C */  sw         $ra, 0x1C($sp)
    /* AD9C 8000A19C AFB00018 */  sw         $s0, 0x18($sp)
    /* ADA0 8000A1A0 AFA40048 */  sw         $a0, 0x48($sp)
    /* ADA4 8000A1A4 AFA60050 */  sw         $a2, 0x50($sp)
    /* ADA8 8000A1A8 ACA40004 */  sw         $a0, 0x4($a1)
    /* ADAC 8000A1AC 8FAF0050 */  lw         $t7, 0x50($sp)
    /* ADB0 8000A1B0 241800FF */  addiu      $t8, $zero, 0xFF
    /* ADB4 8000A1B4 00A08025 */  or         $s0, $a1, $zero
    /* ADB8 8000A1B8 A0B80065 */  sb         $t8, 0x65($a1)
    /* ADBC 8000A1BC ACA00000 */  sw         $zero, 0x0($a1)
    /* ADC0 8000A1C0 00A02025 */  or         $a0, $a1, $zero
    /* ADC4 8000A1C4 ACAF0008 */  sw         $t7, 0x8($a1)
    /* ADC8 8000A1C8 0C003A14 */  jal        func_8000E850
    /* ADCC 8000A1CC 240500FE */   addiu     $a1, $zero, 0xFE
    /* ADD0 8000A1D0 24010002 */  addiu      $at, $zero, 0x2
    /* ADD4 8000A1D4 14410005 */  bne        $v0, $at, .L8000A1EC
    /* ADD8 8000A1D8 00401825 */   or        $v1, $v0, $zero
    /* ADDC 8000A1DC 02002025 */  or         $a0, $s0, $zero
    /* ADE0 8000A1E0 0C003A14 */  jal        func_8000E850
    /* ADE4 8000A1E4 24050080 */   addiu     $a1, $zero, 0x80
    /* ADE8 8000A1E8 00401825 */  or         $v1, $v0, $zero
  .L8000A1EC:
    /* ADEC 8000A1EC 10400003 */  beqz       $v0, .L8000A1FC
    /* ADF0 8000A1F0 8FA40048 */   lw        $a0, 0x48($sp)
    /* ADF4 8000A1F4 10000039 */  b          .L8000A2DC
    /* ADF8 8000A1F8 00601025 */   or        $v0, $v1, $zero
  .L8000A1FC:
    /* ADFC 8000A1FC 8FA50050 */  lw         $a1, 0x50($sp)
    /* AE00 8000A200 24060400 */  addiu      $a2, $zero, 0x400
    /* AE04 8000A204 0C003A34 */  jal        func_8000E8D0
    /* AE08 8000A208 27A70024 */   addiu     $a3, $sp, 0x24
    /* AE0C 8000A20C 24010002 */  addiu      $at, $zero, 0x2
    /* AE10 8000A210 14410002 */  bne        $v0, $at, .L8000A21C
    /* AE14 8000A214 00401825 */   or        $v1, $v0, $zero
    /* AE18 8000A218 24030004 */  addiu      $v1, $zero, 0x4
  .L8000A21C:
    /* AE1C 8000A21C 10600003 */  beqz       $v1, .L8000A22C
    /* AE20 8000A220 93B90043 */   lbu       $t9, 0x43($sp)
    /* AE24 8000A224 1000002D */  b          .L8000A2DC
    /* AE28 8000A228 00601025 */   or        $v0, $v1, $zero
  .L8000A22C:
    /* AE2C 8000A22C 240100FE */  addiu      $at, $zero, 0xFE
    /* AE30 8000A230 17210003 */  bne        $t9, $at, .L8000A240
    /* AE34 8000A234 02002025 */   or        $a0, $s0, $zero
    /* AE38 8000A238 10000028 */  b          .L8000A2DC
    /* AE3C 8000A23C 2402000B */   addiu     $v0, $zero, 0xB
  .L8000A240:
    /* AE40 8000A240 0C003A14 */  jal        func_8000E850
    /* AE44 8000A244 24050080 */   addiu     $a1, $zero, 0x80
    /* AE48 8000A248 24010002 */  addiu      $at, $zero, 0x2
    /* AE4C 8000A24C 14410002 */  bne        $v0, $at, .L8000A258
    /* AE50 8000A250 00401825 */   or        $v1, $v0, $zero
    /* AE54 8000A254 24030004 */  addiu      $v1, $zero, 0x4
  .L8000A258:
    /* AE58 8000A258 10600003 */  beqz       $v1, .L8000A268
    /* AE5C 8000A25C 8FA40048 */   lw        $a0, 0x48($sp)
    /* AE60 8000A260 1000001E */  b          .L8000A2DC
    /* AE64 8000A264 00601025 */   or        $v0, $v1, $zero
  .L8000A268:
    /* AE68 8000A268 8FA50050 */  lw         $a1, 0x50($sp)
    /* AE6C 8000A26C 24060400 */  addiu      $a2, $zero, 0x400
    /* AE70 8000A270 0C003A34 */  jal        func_8000E8D0
    /* AE74 8000A274 27A70024 */   addiu     $a3, $sp, 0x24
    /* AE78 8000A278 24010002 */  addiu      $at, $zero, 0x2
    /* AE7C 8000A27C 14410002 */  bne        $v0, $at, .L8000A288
    /* AE80 8000A280 00401825 */   or        $v1, $v0, $zero
    /* AE84 8000A284 24030004 */  addiu      $v1, $zero, 0x4
  .L8000A288:
    /* AE88 8000A288 10600003 */  beqz       $v1, .L8000A298
    /* AE8C 8000A28C 93A80043 */   lbu       $t0, 0x43($sp)
    /* AE90 8000A290 10000012 */  b          .L8000A2DC
    /* AE94 8000A294 00601025 */   or        $v0, $v1, $zero
  .L8000A298:
    /* AE98 8000A298 24010080 */  addiu      $at, $zero, 0x80
    /* AE9C 8000A29C 51010004 */  beql       $t0, $at, .L8000A2B0
    /* AEA0 8000A2A0 8E090000 */   lw        $t1, 0x0($s0)
    /* AEA4 8000A2A4 1000000D */  b          .L8000A2DC
    /* AEA8 8000A2A8 2402000B */   addiu     $v0, $zero, 0xB
    /* AEAC 8000A2AC 8E090000 */  lw         $t1, 0x0($s0)
  .L8000A2B0:
    /* AEB0 8000A2B0 8FA40050 */  lw         $a0, 0x50($sp)
    /* AEB4 8000A2B4 3C0D8003 */  lui        $t5, %hi(D_80037B30)
    /* AEB8 8000A2B8 312A0008 */  andi       $t2, $t1, 0x8
    /* AEBC 8000A2BC 15400004 */  bnez       $t2, .L8000A2D0
    /* AEC0 8000A2C0 00046180 */   sll       $t4, $a0, 6
    /* AEC4 8000A2C4 25AD7B30 */  addiu      $t5, $t5, %lo(D_80037B30)
    /* AEC8 8000A2C8 0C002822 */  jal        func_8000A088
    /* AECC 8000A2CC 018D2821 */   addu      $a1, $t4, $t5
  .L8000A2D0:
    /* AED0 8000A2D0 240E0008 */  addiu      $t6, $zero, 0x8
    /* AED4 8000A2D4 AE0E0000 */  sw         $t6, 0x0($s0)
    /* AED8 8000A2D8 00001025 */  or         $v0, $zero, $zero
  .L8000A2DC:
    /* AEDC 8000A2DC 8FBF001C */  lw         $ra, 0x1C($sp)
    /* AEE0 8000A2E0 8FB00018 */  lw         $s0, 0x18($sp)
    /* AEE4 8000A2E4 27BD0048 */  addiu      $sp, $sp, 0x48
    /* AEE8 8000A2E8 03E00008 */  jr         $ra
    /* AEEC 8000A2EC 00000000 */   nop
```

## Tips for This Category

