# __scExec

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80000E20` |
| **Category** | `scheduler` |
| **Status** | `TODO` |
| **Instructions** | ~90 |

## Description

launch next task to RSP/RDP

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
./work/scheduler/__scExec/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/scheduler/__scExec

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/scheduler/__scExec --watch
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
# Address: 0x80000E20

glabel func_80000E20
    /* 1A20 80000E20 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1A24 80000E24 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1A28 80000E28 AFA40020 */  sw         $a0, 0x20($sp)
    /* 1A2C 80000E2C AFA50024 */  sw         $a1, 0x24($sp)
    /* 1A30 80000E30 AFA60028 */  sw         $a2, 0x28($sp)
    /* 1A34 80000E34 8FAE0020 */  lw         $t6, 0x20($sp)
    /* 1A38 80000E38 8DCF0274 */  lw         $t7, 0x274($t6)
    /* 1A3C 80000E3C 11E00001 */  beqz       $t7, .L80000E44
    /* 1A40 80000E40 00000000 */   nop
  .L80000E44:
    /* 1A44 80000E44 8FB80024 */  lw         $t8, 0x24($sp)
    /* 1A48 80000E48 13000032 */  beqz       $t8, .L80000F14
    /* 1A4C 80000E4C 00000000 */   nop
    /* 1A50 80000E50 8FB90024 */  lw         $t9, 0x24($sp)
    /* 1A54 80000E54 24010002 */  addiu      $at, $zero, 0x2
    /* 1A58 80000E58 8F280010 */  lw         $t0, 0x10($t9)
    /* 1A5C 80000E5C 1501000A */  bne        $t0, $at, .L80000E88
    /* 1A60 80000E60 00000000 */   nop
    /* 1A64 80000E64 0C001CEC */  jal        func_800073B0
    /* 1A68 80000E68 00000000 */   nop
    /* 1A6C 80000E6C 3C018003 */  lui        $at, %hi(D_8002EB78)
    /* 1A70 80000E70 AC22EB78 */  sw         $v0, %lo(D_8002EB78)($at)
    /* 1A74 80000E74 AC23EB7C */  sw         $v1, %lo(D_8002EB7C)($at)
    /* 1A78 80000E78 8FA90024 */  lw         $t1, 0x24($sp)
    /* 1A7C 80000E7C 3C018003 */  lui        $at, %hi(D_8002AFA0)
    /* 1A80 80000E80 1000000D */  b          .L80000EB8
    /* 1A84 80000E84 AC29AFA0 */   sw        $t1, %lo(D_8002AFA0)($at)
  .L80000E88:
    /* 1A88 80000E88 8FAA0024 */  lw         $t2, 0x24($sp)
    /* 1A8C 80000E8C 8D4B0004 */  lw         $t3, 0x4($t2)
    /* 1A90 80000E90 316C0020 */  andi       $t4, $t3, 0x20
    /* 1A94 80000E94 15800008 */  bnez       $t4, .L80000EB8
    /* 1A98 80000E98 00000000 */   nop
    /* 1A9C 80000E9C 3C0E8003 */  lui        $t6, %hi(D_8002EB80)
    /* 1AA0 80000EA0 8DCEEB80 */  lw         $t6, %lo(D_8002EB80)($t6)
    /* 1AA4 80000EA4 3C0F8003 */  lui        $t7, %hi(D_8002EB84)
    /* 1AA8 80000EA8 8DEFEB84 */  lw         $t7, %lo(D_8002EB84)($t7)
    /* 1AAC 80000EAC 3C018003 */  lui        $at, %hi(D_8002E8E0)
    /* 1AB0 80000EB0 AC2EE8E0 */  sw         $t6, %lo(D_8002E8E0)($at)
    /* 1AB4 80000EB4 AC2FE8E4 */  sw         $t7, %lo(D_8002E8E4)($at)
  .L80000EB8:
    /* 1AB8 80000EB8 0C001E04 */  jal        func_80007810
    /* 1ABC 80000EBC 00000000 */   nop
    /* 1AC0 80000EC0 8FAD0024 */  lw         $t5, 0x24($sp)
    /* 1AC4 80000EC4 2401FFCF */  addiu      $at, $zero, -0x31
    /* 1AC8 80000EC8 8DB80004 */  lw         $t8, 0x4($t5)
    /* 1ACC 80000ECC 0301C824 */  and        $t9, $t8, $at
    /* 1AD0 80000ED0 ADB90004 */  sw         $t9, 0x4($t5)
    /* 1AD4 80000ED4 8FA40024 */  lw         $a0, 0x24($sp)
    /* 1AD8 80000ED8 0C001E53 */  jal        func_8000794C
    /* 1ADC 80000EDC 24840010 */   addiu     $a0, $a0, 0x10
    /* 1AE0 80000EE0 8FA40024 */  lw         $a0, 0x24($sp)
    /* 1AE4 80000EE4 0C001EAD */  jal        func_80007AB4
    /* 1AE8 80000EE8 24840010 */   addiu     $a0, $a0, 0x10
    /* 1AEC 80000EEC 8FA80024 */  lw         $t0, 0x24($sp)
    /* 1AF0 80000EF0 8FA90020 */  lw         $t1, 0x20($sp)
    /* 1AF4 80000EF4 AD280274 */  sw         $t0, 0x274($t1)
    /* 1AF8 80000EF8 8FAA0024 */  lw         $t2, 0x24($sp)
    /* 1AFC 80000EFC 8FAB0028 */  lw         $t3, 0x28($sp)
    /* 1B00 80000F00 154B0004 */  bne        $t2, $t3, .L80000F14
    /* 1B04 80000F04 00000000 */   nop
    /* 1B08 80000F08 8FAC0028 */  lw         $t4, 0x28($sp)
    /* 1B0C 80000F0C 8FAE0020 */  lw         $t6, 0x20($sp)
    /* 1B10 80000F10 ADCC0278 */  sw         $t4, 0x278($t6)
  .L80000F14:
    /* 1B14 80000F14 8FAF0028 */  lw         $t7, 0x28($sp)
    /* 1B18 80000F18 11E00015 */  beqz       $t7, .L80000F70
    /* 1B1C 80000F1C 00000000 */   nop
    /* 1B20 80000F20 8FB80024 */  lw         $t8, 0x24($sp)
    /* 1B24 80000F24 11F80012 */  beq        $t7, $t8, .L80000F70
    /* 1B28 80000F28 00000000 */   nop
    /* 1B2C 80000F2C 8FB90028 */  lw         $t9, 0x28($sp)
    /* 1B30 80000F30 8F2D0038 */  lw         $t5, 0x38($t9)
    /* 1B34 80000F34 15A00001 */  bnez       $t5, .L80000F3C
    /* 1B38 80000F38 00000000 */   nop
  .L80000F3C:
    /* 1B3C 80000F3C 8FA80028 */  lw         $t0, 0x28($sp)
    /* 1B40 80000F40 8D09003C */  lw         $t1, 0x3C($t0)
    /* 1B44 80000F44 8D040038 */  lw         $a0, 0x38($t0)
    /* 1B48 80000F48 8D260000 */  lw         $a2, 0x0($t1)
    /* 1B4C 80000F4C 0C001EC0 */  jal        func_80007B00
    /* 1B50 80000F50 8D270004 */   lw        $a3, 0x4($t1)
    /* 1B54 80000F54 AFA2001C */  sw         $v0, 0x1C($sp)
    /* 1B58 80000F58 8FAA001C */  lw         $t2, 0x1C($sp)
    /* 1B5C 80000F5C 11400001 */  beqz       $t2, .L80000F64
    /* 1B60 80000F60 00000000 */   nop
  .L80000F64:
    /* 1B64 80000F64 8FAB0028 */  lw         $t3, 0x28($sp)
    /* 1B68 80000F68 8FAC0020 */  lw         $t4, 0x20($sp)
    /* 1B6C 80000F6C AD8B0278 */  sw         $t3, 0x278($t4)
  .L80000F70:
    /* 1B70 80000F70 10000001 */  b          .L80000F78
    /* 1B74 80000F74 00000000 */   nop
  .L80000F78:
    /* 1B78 80000F78 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1B7C 80000F7C 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 1B80 80000F80 03E00008 */  jr         $ra
    /* 1B84 80000F84 00000000 */   nop
```

## Tips for This Category

