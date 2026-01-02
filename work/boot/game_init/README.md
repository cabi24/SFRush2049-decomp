# game_init

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80002238` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~177 |

## Description

main game initialization, creates threads

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `boot` category.

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
./work/boot/game_init/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/game_init

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/game_init --watch
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
# Source: 2CF0.s
# Address: 0x80002238

glabel func_80002238
    /* 2E38 80002238 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* 2E3C 8000223C AFBF0024 */  sw         $ra, 0x24($sp)
    /* 2E40 80002240 AFA40050 */  sw         $a0, 0x50($sp)
    /* 2E44 80002244 AFB00020 */  sw         $s0, 0x20($sp)
    /* 2E48 80002248 AFA0002C */  sw         $zero, 0x2C($sp)
    /* 2E4C 8000224C 3C040100 */  lui        $a0, (0x1000E00 >> 16)
    /* 2E50 80002250 0C00127C */  jal        func_800049F0
    /* 2E54 80002254 34840E00 */   ori       $a0, $a0, (0x1000E00 & 0xFFFF)
    /* 2E58 80002258 3C048003 */  lui        $a0, %hi(D_80035410)
    /* 2E5C 8000225C 3C058003 */  lui        $a1, %hi(D_8002F140)
    /* 2E60 80002260 24A5F140 */  addiu      $a1, $a1, %lo(D_8002F140)
    /* 2E64 80002264 24845410 */  addiu      $a0, $a0, %lo(D_80035410)
    /* 2E68 80002268 0C001A80 */  jal        func_80006A00
    /* 2E6C 8000226C 24060008 */   addiu     $a2, $zero, 0x8
    /* 2E70 80002270 3C048003 */  lui        $a0, %hi(D_80035428)
    /* 2E74 80002274 3C058003 */  lui        $a1, %hi(D_8002F160)
    /* 2E78 80002278 24A5F160 */  addiu      $a1, $a1, %lo(D_8002F160)
    /* 2E7C 8000227C 24845428 */  addiu      $a0, $a0, %lo(D_80035428)
    /* 2E80 80002280 0C001A80 */  jal        func_80006A00
    /* 2E84 80002284 24060008 */   addiu     $a2, $zero, 0x8
    /* 2E88 80002288 3C048003 */  lui        $a0, %hi(D_8002ECF8)
    /* 2E8C 8000228C 3C058003 */  lui        $a1, %hi(D_8002ED10)
    /* 2E90 80002290 24A5ED10 */  addiu      $a1, $a1, %lo(D_8002ED10)
    /* 2E94 80002294 2484ECF8 */  addiu      $a0, $a0, %lo(D_8002ECF8)
    /* 2E98 80002298 0C001A80 */  jal        func_80006A00
    /* 2E9C 8000229C 2406003C */   addiu     $a2, $zero, 0x3C
    /* 2EA0 800022A0 3C048003 */  lui        $a0, %hi(D_8002ECC0)
    /* 2EA4 800022A4 3C058003 */  lui        $a1, %hi(D_8002ECD8)
    /* 2EA8 800022A8 24A5ECD8 */  addiu      $a1, $a1, %lo(D_8002ECD8)
    /* 2EAC 800022AC 2484ECC0 */  addiu      $a0, $a0, %lo(D_8002ECC0)
    /* 2EB0 800022B0 0C001A80 */  jal        func_80006A00
    /* 2EB4 800022B4 24060008 */   addiu     $a2, $zero, 0x8
    /* 2EB8 800022B8 0C00077F */  jal        func_80001DFC
    /* 2EBC 800022BC 00000000 */   nop
    /* 2EC0 800022C0 3C058003 */  lui        $a1, %hi(D_80033EA0)
    /* 2EC4 800022C4 24A53EA0 */  addiu      $a1, $a1, %lo(D_80033EA0)
    /* 2EC8 800022C8 00408025 */  or         $s0, $v0, $zero
    /* 2ECC 800022CC 3C048003 */  lui        $a0, %hi(D_8002E8E8)
    /* 2ED0 800022D0 240E0001 */  addiu      $t6, $zero, 0x1
    /* 2ED4 800022D4 AFAE0010 */  sw         $t6, 0x10($sp)
    /* 2ED8 800022D8 2484E8E8 */  addiu      $a0, $a0, %lo(D_8002E8E8)
    /* 2EDC 800022DC 02003825 */  or         $a3, $s0, $zero
    /* 2EE0 800022E0 24A50640 */  addiu      $a1, $a1, 0x640
    /* 2EE4 800022E4 0C000114 */  jal        func_80000450
    /* 2EE8 800022E8 2406000C */   addiu     $a2, $zero, 0xC
    /* 2EEC 800022EC 3C0F8008 */  lui        $t7, %hi(D_80086A50)
    /* 2EF0 800022F0 3C188011 */  lui        $t8, %hi(D_8010FD80)
    /* 2EF4 800022F4 2718FD80 */  addiu      $t8, $t8, %lo(D_8010FD80)
    /* 2EF8 800022F8 25EF6A50 */  addiu      $t7, $t7, %lo(D_80086A50)
    /* 2EFC 800022FC 01E02025 */  or         $a0, $t7, $zero
    /* 2F00 80002300 0C002118 */  jal        func_80008460
    /* 2F04 80002304 030F2823 */   subu      $a1, $t8, $t7
    /* 2F08 80002308 3C198011 */  lui        $t9, %hi(D_8010FD80)
    /* 2F0C 8000230C 3C088012 */  lui        $t0, %hi(D_801249F0)
    /* 2F10 80002310 250849F0 */  addiu      $t0, $t0, %lo(D_801249F0)
    /* 2F14 80002314 2739FD80 */  addiu      $t9, $t9, %lo(D_8010FD80)
    /* 2F18 80002318 03202025 */  or         $a0, $t9, $zero
    /* 2F1C 8000231C 0C002138 */  jal        func_800084E0
    /* 2F20 80002320 01192823 */   subu      $a1, $t0, $t9
    /* 2F24 80002324 3C0400B1 */  lui        $a0, %hi(D_B0CB10)
    /* 2F28 80002328 3C058008 */  lui        $a1, %hi(D_80086A50)
    /* 2F2C 8000232C 24A56A50 */  addiu      $a1, $a1, %lo(D_80086A50)
    /* 2F30 80002330 2484CB10 */  addiu      $a0, $a0, %lo(D_B0CB10)
    /* 2F34 80002334 0C001A05 */  jal        func_80006814
    /* 2F38 80002338 00003025 */   or        $a2, $zero, $zero
    /* 2F3C 8000233C 3C098012 */  lui        $t1, %hi(D_801249F0)
    /* 2F40 80002340 3C0A8018 */  lui        $t2, %hi(D_8017A640)
    /* 2F44 80002344 254AA640 */  addiu      $t2, $t2, %lo(D_8017A640)
    /* 2F48 80002348 252949F0 */  addiu      $t1, $t1, %lo(D_801249F0)
    /* 2F4C 8000234C 01202025 */  or         $a0, $t1, $zero
    /* 2F50 80002350 0C002164 */  jal        func_80008590
    /* 2F54 80002354 01492823 */   subu      $a1, $t2, $t1
    /* 2F58 80002358 3C048003 */  lui        $a0, %hi(D_8002E8E8)
    /* 2F5C 8000235C 3C068003 */  lui        $a2, %hi(D_8002ECF8)
    /* 2F60 80002360 24C6ECF8 */  addiu      $a2, $a2, %lo(D_8002ECF8)
    /* 2F64 80002364 2484E8E8 */  addiu      $a0, $a0, %lo(D_8002E8E8)
    /* 2F68 80002368 0C000175 */  jal        func_800005D4
    /* 2F6C 8000236C 27A50030 */   addiu     $a1, $sp, 0x30
    /* 2F70 80002370 3C0B8003 */  lui        $t3, %hi(D_8002F7F0)
    /* 2F74 80002374 256BF7F0 */  addiu      $t3, $t3, %lo(D_8002F7F0)
    /* 2F78 80002378 256C0960 */  addiu      $t4, $t3, 0x960
    /* 2F7C 8000237C 3C048003 */  lui        $a0, %hi(D_80034690)
    /* 2F80 80002380 3C06800B */  lui        $a2, %hi(D_800AC75C)
    /* 2F84 80002384 240D0003 */  addiu      $t5, $zero, 0x3
    /* 2F88 80002388 AFAD0014 */  sw         $t5, 0x14($sp)
    /* 2F8C 8000238C 24C6C75C */  addiu      $a2, $a2, %lo(D_800AC75C)
    /* 2F90 80002390 24844690 */  addiu      $a0, $a0, %lo(D_80034690)
    /* 2F94 80002394 AFAC0010 */  sw         $t4, 0x10($sp)
    /* 2F98 80002398 24050008 */  addiu      $a1, $zero, 0x8
    /* 2F9C 8000239C 0C001BCC */  jal        func_80006F30
    /* 2FA0 800023A0 00003825 */   or        $a3, $zero, $zero
    /* 2FA4 800023A4 3C048003 */  lui        $a0, %hi(D_80035440)
    /* 2FA8 800023A8 3C058003 */  lui        $a1, %hi(D_8002F180)
    /* 2FAC 800023AC 24A5F180 */  addiu      $a1, $a1, %lo(D_8002F180)
    /* 2FB0 800023B0 24845440 */  addiu      $a0, $a0, %lo(D_80035440)
    /* 2FB4 800023B4 0C001A80 */  jal        func_80006A00
    /* 2FB8 800023B8 24060001 */   addiu     $a2, $zero, 0x1
    /* 2FBC 800023BC 3C048003 */  lui        $a0, %hi(D_80035440)
    /* 2FC0 800023C0 24845440 */  addiu      $a0, $a0, %lo(D_80035440)
    /* 2FC4 800023C4 00002825 */  or         $a1, $zero, $zero
    /* 2FC8 800023C8 0C001D78 */  jal        func_800075E0
    /* 2FCC 800023CC 24060001 */   addiu     $a2, $zero, 0x1
    /* 2FD0 800023D0 3C048003 */  lui        $a0, %hi(D_80034690)
    /* 2FD4 800023D4 0C001C20 */  jal        func_80007080
    /* 2FD8 800023D8 24844690 */   addiu     $a0, $a0, %lo(D_80034690)
    /* 2FDC 800023DC 3C0E8003 */  lui        $t6, %hi(D_80031D70)
    /* 2FE0 800023E0 25CE1D70 */  addiu      $t6, $t6, %lo(D_80031D70)
    /* 2FE4 800023E4 25D80960 */  addiu      $t8, $t6, 0x960
    /* 2FE8 800023E8 3C048003 */  lui        $a0, %hi(D_80034840)
    /* 2FEC 800023EC 3C06800E */  lui        $a2, %hi(D_800E7808)
    /* 2FF0 800023F0 240F0007 */  addiu      $t7, $zero, 0x7
    /* 2FF4 800023F4 AFAF0014 */  sw         $t7, 0x14($sp)
    /* 2FF8 800023F8 24C67808 */  addiu      $a2, $a2, %lo(D_800E7808)
    /* 2FFC 800023FC 24844840 */  addiu      $a0, $a0, %lo(D_80034840)
    /* 3000 80002400 AFB80010 */  sw         $t8, 0x10($sp)
    /* 3004 80002404 24050005 */  addiu      $a1, $zero, 0x5
    /* 3008 80002408 0C001BCC */  jal        func_80006F30
    /* 300C 8000240C 00003825 */   or        $a3, $zero, $zero
    /* 3010 80002410 3C048003 */  lui        $a0, %hi(D_80034840)
    /* 3014 80002414 0C001C20 */  jal        func_80007080
    /* 3018 80002418 24844840 */   addiu     $a0, $a0, %lo(D_80034840)
    /* 301C 8000241C 0C03BA9F */  jal        func_800EEA7C
    /* 3020 80002420 00000000 */   nop
    /* 3024 80002424 0C02924D */  jal        func_800A4934
    /* 3028 80002428 00000000 */   nop
    /* 302C 8000242C 3C088003 */  lui        $t0, %hi(D_80030AB0)
    /* 3030 80002430 25080AB0 */  addiu      $t0, $t0, %lo(D_80030AB0)
    /* 3034 80002434 251912C0 */  addiu      $t9, $t0, 0x12C0
    /* 3038 80002438 3C048003 */  lui        $a0, %hi(D_800344E0)
    /* 303C 8000243C 3C068000 */  lui        $a2, %hi(func_800024FC)
    /* 3040 80002440 240A0005 */  addiu      $t2, $zero, 0x5
    /* 3044 80002444 AFAA0014 */  sw         $t2, 0x14($sp)
    /* 3048 80002448 24C624FC */  addiu      $a2, $a2, %lo(func_800024FC)
    /* 304C 8000244C 248444E0 */  addiu      $a0, $a0, %lo(D_800344E0)
    /* 3050 80002450 AFB90010 */  sw         $t9, 0x10($sp)
    /* 3054 80002454 24050007 */  addiu      $a1, $zero, 0x7
    /* 3058 80002458 0C001BCC */  jal        func_80006F30
    /* 305C 8000245C 8FA70050 */   lw        $a3, 0x50($sp)
    /* 3060 80002460 24090ABF */  addiu      $t1, $zero, 0xABF
    /* 3064 80002464 3C018014 */  lui        $at, %hi(D_80142D90)
    /* 3068 80002468 A4292D90 */  sh         $t1, %lo(D_80142D90)($at)
    /* 306C 8000246C 3C0B8003 */  lui        $t3, %hi(D_8002B028)
    /* 3070 80002470 816BB028 */  lb         $t3, %lo(D_8002B028)($t3)
    /* 3074 80002474 15600004 */  bnez       $t3, .L80002488
    /* 3078 80002478 00000000 */   nop
    /* 307C 8000247C 3C048003 */  lui        $a0, %hi(D_800344E0)
    /* 3080 80002480 0C001C20 */  jal        func_80007080
    /* 3084 80002484 248444E0 */   addiu     $a0, $a0, %lo(D_800344E0)
  .L80002488:
    /* 3088 80002488 3C048003 */  lui        $a0, %hi(D_8002ECF8)
    /* 308C 8000248C 2484ECF8 */  addiu      $a0, $a0, %lo(D_8002ECF8)
    /* 3090 80002490 27A5002C */  addiu      $a1, $sp, 0x2C
    /* 3094 80002494 0C001C9C */  jal        func_80007270
    /* 3098 80002498 24060001 */   addiu     $a2, $zero, 0x1
    /* 309C 8000249C 8FAC002C */  lw         $t4, 0x2C($sp)
    /* 30A0 800024A0 24010ABE */  addiu      $at, $zero, 0xABE
    /* 30A4 800024A4 858D0000 */  lh         $t5, 0x0($t4)
    /* 30A8 800024A8 15A10003 */  bne        $t5, $at, .L800024B8
    /* 30AC 800024AC 00000000 */   nop
    /* 30B0 800024B0 10000003 */  b          .L800024C0
    /* 30B4 800024B4 00000000 */   nop
  .L800024B8:
    /* 30B8 800024B8 1000FFF3 */  b          .L80002488
    /* 30BC 800024BC 00000000 */   nop
  .L800024C0:
    /* 30C0 800024C0 3C018003 */  lui        $at, %hi(D_80035472)
    /* 30C4 800024C4 A0205472 */  sb         $zero, %lo(D_80035472)($at)
    /* 30C8 800024C8 0C029232 */  jal        func_800A48C8
    /* 30CC 800024CC 00000000 */   nop
  .L800024D0:
    /* 30D0 800024D0 0C03B977 */  jal        func_800EE5DC
    /* 30D4 800024D4 00000000 */   nop
    /* 30D8 800024D8 1000FFFD */  b          .L800024D0
    /* 30DC 800024DC 00000000 */   nop
    /* 30E0 800024E0 10000001 */  b          .L800024E8
    /* 30E4 800024E4 00000000 */   nop
  .L800024E8:
    /* 30E8 800024E8 8FBF0024 */  lw         $ra, 0x24($sp)
    /* 30EC 800024EC 8FB00020 */  lw         $s0, 0x20($sp)
    /* 30F0 800024F0 27BD0050 */  addiu      $sp, $sp, 0x50
    /* 30F4 800024F4 03E00008 */  jr         $ra
    /* 30F8 800024F8 00000000 */   nop
```

## Tips for This Category

