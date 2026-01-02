# osRecvMesg

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007270` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~78 |

## Description

receive message from queue

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
./work/libultra/os/osRecvMesg/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osRecvMesg

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osRecvMesg --watch
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
# Source: 7C80.s
# Address: 0x80007270

glabel func_80007270
    /* 7E70 80007270 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 7E74 80007274 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 7E78 80007278 AFA40028 */  sw         $a0, 0x28($sp)
    /* 7E7C 8000727C AFA5002C */  sw         $a1, 0x2C($sp)
    /* 7E80 80007280 AFA60030 */  sw         $a2, 0x30($sp)
    /* 7E84 80007284 AFB10018 */  sw         $s1, 0x18($sp)
    /* 7E88 80007288 0C00312C */  jal        func_8000C4B0
    /* 7E8C 8000728C AFB00014 */   sw        $s0, 0x14($sp)
    /* 7E90 80007290 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 7E94 80007294 00408025 */  or         $s0, $v0, $zero
    /* 7E98 80007298 8DCF0008 */  lw         $t7, 0x8($t6)
    /* 7E9C 8000729C 15E00012 */  bnez       $t7, .L800072E8
    /* 7EA0 800072A0 00000000 */   nop
  .L800072A4:
    /* 7EA4 800072A4 8FB80030 */  lw         $t8, 0x30($sp)
    /* 7EA8 800072A8 17000005 */  bnez       $t8, .L800072C0
    /* 7EAC 800072AC 00000000 */   nop
    /* 7EB0 800072B0 0C003148 */  jal        func_8000C520
    /* 7EB4 800072B4 02002025 */   or        $a0, $s0, $zero
    /* 7EB8 800072B8 10000036 */  b          .L80007394
    /* 7EBC 800072BC 2402FFFF */   addiu     $v0, $zero, -0x1
  .L800072C0:
    /* 7EC0 800072C0 3C088003 */  lui        $t0, %hi(D_8002C3E0)
    /* 7EC4 800072C4 8D08C3E0 */  lw         $t0, %lo(D_8002C3E0)($t0)
    /* 7EC8 800072C8 24190008 */  addiu      $t9, $zero, 0x8
    /* 7ECC 800072CC A5190010 */  sh         $t9, 0x10($t0)
    /* 7ED0 800072D0 0C0033F1 */  jal        func_8000CFC4
    /* 7ED4 800072D4 8FA40028 */   lw        $a0, 0x28($sp)
    /* 7ED8 800072D8 8FA90028 */  lw         $t1, 0x28($sp)
    /* 7EDC 800072DC 8D2A0008 */  lw         $t2, 0x8($t1)
    /* 7EE0 800072E0 1140FFF0 */  beqz       $t2, .L800072A4
    /* 7EE4 800072E4 00000000 */   nop
  .L800072E8:
    /* 7EE8 800072E8 8FAB002C */  lw         $t3, 0x2C($sp)
    /* 7EEC 800072EC 11600008 */  beqz       $t3, .L80007310
    /* 7EF0 800072F0 00000000 */   nop
    /* 7EF4 800072F4 8FAC0028 */  lw         $t4, 0x28($sp)
    /* 7EF8 800072F8 8D8E000C */  lw         $t6, 0xC($t4)
    /* 7EFC 800072FC 8D8D0014 */  lw         $t5, 0x14($t4)
    /* 7F00 80007300 000E7880 */  sll        $t7, $t6, 2
    /* 7F04 80007304 01AFC021 */  addu       $t8, $t5, $t7
    /* 7F08 80007308 8F190000 */  lw         $t9, 0x0($t8)
    /* 7F0C 8000730C AD790000 */  sw         $t9, 0x0($t3)
  .L80007310:
    /* 7F10 80007310 8FA80028 */  lw         $t0, 0x28($sp)
    /* 7F14 80007314 8D09000C */  lw         $t1, 0xC($t0)
    /* 7F18 80007318 8D0C0010 */  lw         $t4, 0x10($t0)
    /* 7F1C 8000731C 252A0001 */  addiu      $t2, $t1, 0x1
    /* 7F20 80007320 014C001A */  div        $zero, $t2, $t4
    /* 7F24 80007324 00007010 */  mfhi       $t6
    /* 7F28 80007328 AD0E000C */  sw         $t6, 0xC($t0)
    /* 7F2C 8000732C 8FAD0028 */  lw         $t5, 0x28($sp)
    /* 7F30 80007330 15800002 */  bnez       $t4, .L8000733C
    /* 7F34 80007334 00000000 */   nop
    /* 7F38 80007338 0007000D */  break      7
  .L8000733C:
    /* 7F3C 8000733C 2401FFFF */  addiu      $at, $zero, -0x1
    /* 7F40 80007340 15810004 */  bne        $t4, $at, .L80007354
    /* 7F44 80007344 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 7F48 80007348 15410002 */  bne        $t2, $at, .L80007354
    /* 7F4C 8000734C 00000000 */   nop
    /* 7F50 80007350 0006000D */  break      6
  .L80007354:
    /* 7F54 80007354 8DAF0008 */  lw         $t7, 0x8($t5)
    /* 7F58 80007358 25F8FFFF */  addiu      $t8, $t7, -0x1
    /* 7F5C 8000735C ADB80008 */  sw         $t8, 0x8($t5)
    /* 7F60 80007360 8FB90028 */  lw         $t9, 0x28($sp)
    /* 7F64 80007364 8F2B0004 */  lw         $t3, 0x4($t9)
    /* 7F68 80007368 8D690000 */  lw         $t1, 0x0($t3)
    /* 7F6C 8000736C 11200006 */  beqz       $t1, .L80007388
    /* 7F70 80007370 00000000 */   nop
    /* 7F74 80007374 0C003443 */  jal        func_8000D10C
    /* 7F78 80007378 27240004 */   addiu     $a0, $t9, 0x4
    /* 7F7C 8000737C 00408825 */  or         $s1, $v0, $zero
    /* 7F80 80007380 0C001C20 */  jal        func_80007080
    /* 7F84 80007384 02202025 */   or        $a0, $s1, $zero
  .L80007388:
    /* 7F88 80007388 0C003148 */  jal        func_8000C520
    /* 7F8C 8000738C 02002025 */   or        $a0, $s0, $zero
    /* 7F90 80007390 00001025 */  or         $v0, $zero, $zero
  .L80007394:
    /* 7F94 80007394 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 7F98 80007398 8FB00014 */  lw         $s0, 0x14($sp)
    /* 7F9C 8000739C 8FB10018 */  lw         $s1, 0x18($sp)
    /* 7FA0 800073A0 03E00008 */  jr         $ra
    /* 7FA4 800073A4 27BD0028 */   addiu     $sp, $sp, 0x28
```

## Tips for This Category

