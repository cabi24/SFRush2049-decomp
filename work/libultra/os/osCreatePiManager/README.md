# osCreatePiManager

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80008210` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~92 |

## Description

create PI DMA manager thread and queues

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
./work/libultra/os/osCreatePiManager/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osCreatePiManager

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osCreatePiManager --watch
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
# Source: 8E10.s
# Address: 0x80008210

glabel func_80008210
    /* 8E10 80008210 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 8E14 80008214 AFB00020 */  sw         $s0, 0x20($sp)
    /* 8E18 80008218 3C108003 */  lui        $s0, %hi(D_8002C380)
    /* 8E1C 8000821C 2610C380 */  addiu      $s0, $s0, %lo(D_8002C380)
    /* 8E20 80008220 8E0E0000 */  lw         $t6, 0x0($s0)
    /* 8E24 80008224 AFBF0024 */  sw         $ra, 0x24($sp)
    /* 8E28 80008228 AFA40030 */  sw         $a0, 0x30($sp)
    /* 8E2C 8000822C AFA50034 */  sw         $a1, 0x34($sp)
    /* 8E30 80008230 15C0004E */  bnez       $t6, .L8000836C
    /* 8E34 80008234 AFA60038 */   sw        $a2, 0x38($sp)
    /* 8E38 80008238 00A02025 */  or         $a0, $a1, $zero
    /* 8E3C 8000823C 00C02825 */  or         $a1, $a2, $zero
    /* 8E40 80008240 0C001A80 */  jal        func_80006A00
    /* 8E44 80008244 00E03025 */   or        $a2, $a3, $zero
    /* 8E48 80008248 3C048003 */  lui        $a0, %hi(D_80037A80)
    /* 8E4C 8000824C 3C058003 */  lui        $a1, %hi(D_80037A98)
    /* 8E50 80008250 24A57A98 */  addiu      $a1, $a1, %lo(D_80037A98)
    /* 8E54 80008254 24847A80 */  addiu      $a0, $a0, %lo(D_80037A80)
    /* 8E58 80008258 0C001A80 */  jal        func_80006A00
    /* 8E5C 8000825C 24060001 */   addiu     $a2, $zero, 0x1
    /* 8E60 80008260 3C0F8003 */  lui        $t7, %hi(D_8002C4A0)
    /* 8E64 80008264 8DEFC4A0 */  lw         $t7, %lo(D_8002C4A0)($t7)
    /* 8E68 80008268 15E00003 */  bnez       $t7, .L80008278
    /* 8E6C 8000826C 00000000 */   nop
    /* 8E70 80008270 0C0036F0 */  jal        func_8000DBC0
    /* 8E74 80008274 00000000 */   nop
  .L80008278:
    /* 8E78 80008278 3C058003 */  lui        $a1, %hi(D_80037A80)
    /* 8E7C 8000827C 3C062222 */  lui        $a2, (0x22222222 >> 16)
    /* 8E80 80008280 34C62222 */  ori        $a2, $a2, (0x22222222 & 0xFFFF)
    /* 8E84 80008284 24A57A80 */  addiu      $a1, $a1, %lo(D_80037A80)
    /* 8E88 80008288 0C001B84 */  jal        func_80006E10
    /* 8E8C 8000828C 24040008 */   addiu     $a0, $zero, 0x8
    /* 8E90 80008290 2418FFFF */  addiu      $t8, $zero, -0x1
    /* 8E94 80008294 AFB80028 */  sw         $t8, 0x28($sp)
    /* 8E98 80008298 0C003124 */  jal        func_8000C490
    /* 8E9C 8000829C 00002025 */   or        $a0, $zero, $zero
    /* 8EA0 800082A0 8FB90030 */  lw         $t9, 0x30($sp)
    /* 8EA4 800082A4 00002025 */  or         $a0, $zero, $zero
    /* 8EA8 800082A8 0059082A */  slt        $at, $v0, $t9
    /* 8EAC 800082AC 10200003 */  beqz       $at, .L800082BC
    /* 8EB0 800082B0 03202825 */   or        $a1, $t9, $zero
    /* 8EB4 800082B4 0C0020E0 */  jal        func_80008380
    /* 8EB8 800082B8 AFA20028 */   sw        $v0, 0x28($sp)
  .L800082BC:
    /* 8EBC 800082BC 0C00312C */  jal        func_8000C4B0
    /* 8EC0 800082C0 00000000 */   nop
    /* 8EC4 800082C4 8FA90034 */  lw         $t1, 0x34($sp)
    /* 8EC8 800082C8 8FAF0030 */  lw         $t7, 0x30($sp)
    /* 8ECC 800082CC 3C048003 */  lui        $a0, %hi(D_800368D0)
    /* 8ED0 800082D0 3C0A8003 */  lui        $t2, %hi(D_80037A80)
    /* 8ED4 800082D4 3C0B8003 */  lui        $t3, %hi(D_80037C78)
    /* 8ED8 800082D8 3C0C8001 */  lui        $t4, %hi(func_8000DCD0)
    /* 8EDC 800082DC 3C0D8001 */  lui        $t5, %hi(func_8000DDA0)
    /* 8EE0 800082E0 3C0E8003 */  lui        $t6, %hi(D_80037A80)
    /* 8EE4 800082E4 248468D0 */  addiu      $a0, $a0, %lo(D_800368D0)
    /* 8EE8 800082E8 24080001 */  addiu      $t0, $zero, 0x1
    /* 8EEC 800082EC 254A7A80 */  addiu      $t2, $t2, %lo(D_80037A80)
    /* 8EF0 800082F0 256B7C78 */  addiu      $t3, $t3, %lo(D_80037C78)
    /* 8EF4 800082F4 258CDCD0 */  addiu      $t4, $t4, %lo(func_8000DCD0)
    /* 8EF8 800082F8 25ADDDA0 */  addiu      $t5, $t5, %lo(func_8000DDA0)
    /* 8EFC 800082FC 25CE7A80 */  addiu      $t6, $t6, %lo(D_80037A80)
    /* 8F00 80008300 3C068001 */  lui        $a2, %hi(func_8000DF80)
    /* 8F04 80008304 AFA2002C */  sw         $v0, 0x2C($sp)
    /* 8F08 80008308 AE080000 */  sw         $t0, 0x0($s0)
    /* 8F0C 8000830C AE040004 */  sw         $a0, 0x4($s0)
    /* 8F10 80008310 AE0A000C */  sw         $t2, 0xC($s0)
    /* 8F14 80008314 AE0B0010 */  sw         $t3, 0x10($s0)
    /* 8F18 80008318 AE0C0014 */  sw         $t4, 0x14($s0)
    /* 8F1C 8000831C AE0D0018 */  sw         $t5, 0x18($s0)
    /* 8F20 80008320 24C6DF80 */  addiu      $a2, $a2, %lo(func_8000DF80)
    /* 8F24 80008324 AFAE0010 */  sw         $t6, 0x10($sp)
    /* 8F28 80008328 00002825 */  or         $a1, $zero, $zero
    /* 8F2C 8000832C 02003825 */  or         $a3, $s0, $zero
    /* 8F30 80008330 AE090008 */  sw         $t1, 0x8($s0)
    /* 8F34 80008334 0C001BCC */  jal        func_80006F30
    /* 8F38 80008338 AFAF0014 */   sw        $t7, 0x14($sp)
    /* 8F3C 8000833C 3C048003 */  lui        $a0, %hi(D_800368D0)
    /* 8F40 80008340 0C001C20 */  jal        func_80007080
    /* 8F44 80008344 248468D0 */   addiu     $a0, $a0, %lo(D_800368D0)
    /* 8F48 80008348 0C003148 */  jal        func_8000C520
    /* 8F4C 8000834C 8FA4002C */   lw        $a0, 0x2C($sp)
    /* 8F50 80008350 8FB80028 */  lw         $t8, 0x28($sp)
    /* 8F54 80008354 2401FFFF */  addiu      $at, $zero, -0x1
    /* 8F58 80008358 00002025 */  or         $a0, $zero, $zero
    /* 8F5C 8000835C 53010004 */  beql       $t8, $at, .L80008370
    /* 8F60 80008360 8FBF0024 */   lw        $ra, 0x24($sp)
    /* 8F64 80008364 0C0020E0 */  jal        func_80008380
    /* 8F68 80008368 03002825 */   or        $a1, $t8, $zero
  .L8000836C:
    /* 8F6C 8000836C 8FBF0024 */  lw         $ra, 0x24($sp)
  .L80008370:
    /* 8F70 80008370 8FB00020 */  lw         $s0, 0x20($sp)
    /* 8F74 80008374 27BD0030 */  addiu      $sp, $sp, 0x30
    /* 8F78 80008378 03E00008 */  jr         $ra
    /* 8F7C 8000837C 00000000 */   nop
```

## Tips for This Category

