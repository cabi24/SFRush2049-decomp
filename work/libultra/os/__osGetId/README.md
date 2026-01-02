# __osGetId

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000F124` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~107 |

## Description

top-level ID retrieval during pak init

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
./work/libultra/os/__osGetId/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/__osGetId

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/__osGetId --watch
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
# Source: F700.s
# Address: 0x8000F124

glabel func_8000F124
    /* FD24 8000F124 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* FD28 8000F128 AFBF001C */  sw         $ra, 0x1C($sp)
    /* FD2C 8000F12C AFB10018 */  sw         $s1, 0x18($sp)
    /* FD30 8000F130 AFB00014 */  sw         $s0, 0x14($sp)
    /* FD34 8000F134 908E0065 */  lbu        $t6, 0x65($a0)
    /* FD38 8000F138 00808025 */  or         $s0, $a0, $zero
    /* FD3C 8000F13C 51C00008 */  beql       $t6, $zero, .L8000F160
    /* FD40 8000F140 8E040004 */   lw        $a0, 0x4($s0)
    /* FD44 8000F144 0C003A14 */  jal        func_8000E850
    /* FD48 8000F148 00002825 */   or        $a1, $zero, $zero
    /* FD4C 8000F14C 50400004 */  beql       $v0, $zero, .L8000F160
    /* FD50 8000F150 8E040004 */   lw        $a0, 0x4($s0)
    /* FD54 8000F154 1000005A */  b          .L8000F2C0
    /* FD58 8000F158 8FBF001C */   lw        $ra, 0x1C($sp)
    /* FD5C 8000F15C 8E040004 */  lw         $a0, 0x4($s0)
  .L8000F160:
    /* FD60 8000F160 8E050008 */  lw         $a1, 0x8($s0)
    /* FD64 8000F164 24060001 */  addiu      $a2, $zero, 0x1
    /* FD68 8000F168 0C003A34 */  jal        func_8000E8D0
    /* FD6C 8000F16C 27A7004C */   addiu     $a3, $sp, 0x4C
    /* FD70 8000F170 10400003 */  beqz       $v0, .L8000F180
    /* FD74 8000F174 27A4004C */   addiu     $a0, $sp, 0x4C
    /* FD78 8000F178 10000051 */  b          .L8000F2C0
    /* FD7C 8000F17C 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8000F180:
    /* FD80 8000F180 27A5006E */  addiu      $a1, $sp, 0x6E
    /* FD84 8000F184 0C003ADD */  jal        func_8000EB74
    /* FD88 8000F188 27A6006C */   addiu     $a2, $sp, 0x6C
    /* FD8C 8000F18C 97AF006E */  lhu        $t7, 0x6E($sp)
    /* FD90 8000F190 97B80068 */  lhu        $t8, 0x68($sp)
    /* FD94 8000F194 27A5004C */  addiu      $a1, $sp, 0x4C
    /* FD98 8000F198 00A08825 */  or         $s1, $a1, $zero
    /* FD9C 8000F19C 15F80004 */  bne        $t7, $t8, .L8000F1B0
    /* FDA0 8000F1A0 97B9006C */   lhu       $t9, 0x6C($sp)
    /* FDA4 8000F1A4 97A9006A */  lhu        $t1, 0x6A($sp)
    /* FDA8 8000F1A8 53290015 */  beql       $t9, $t1, .L8000F200
    /* FDAC 8000F1AC 962A0018 */   lhu       $t2, 0x18($s1)
  .L8000F1B0:
    /* FDB0 8000F1B0 0C003BF0 */  jal        func_8000EFC0
    /* FDB4 8000F1B4 02002025 */   or        $a0, $s0, $zero
    /* FDB8 8000F1B8 2401000A */  addiu      $at, $zero, 0xA
    /* FDBC 8000F1BC 1441000B */  bne        $v0, $at, .L8000F1EC
    /* FDC0 8000F1C0 00401825 */   or        $v1, $v0, $zero
    /* FDC4 8000F1C4 02002025 */  or         $a0, $s0, $zero
    /* FDC8 8000F1C8 27A5004C */  addiu      $a1, $sp, 0x4C
    /* FDCC 8000F1CC 0C003B1C */  jal        func_8000EC70
    /* FDD0 8000F1D0 27A60028 */   addiu     $a2, $sp, 0x28
    /* FDD4 8000F1D4 10400003 */  beqz       $v0, .L8000F1E4
    /* FDD8 8000F1D8 00000000 */   nop
    /* FDDC 8000F1DC 10000038 */  b          .L8000F2C0
    /* FDE0 8000F1E0 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8000F1E4:
    /* FDE4 8000F1E4 10000005 */  b          .L8000F1FC
    /* FDE8 8000F1E8 27B10028 */   addiu     $s1, $sp, 0x28
  .L8000F1EC:
    /* FDEC 8000F1EC 50400004 */  beql       $v0, $zero, .L8000F200
    /* FDF0 8000F1F0 962A0018 */   lhu       $t2, 0x18($s1)
    /* FDF4 8000F1F4 10000031 */  b          .L8000F2BC
    /* FDF8 8000F1F8 00601025 */   or        $v0, $v1, $zero
  .L8000F1FC:
    /* FDFC 8000F1FC 962A0018 */  lhu        $t2, 0x18($s1)
  .L8000F200:
    /* FE00 8000F200 02002025 */  or         $a0, $s0, $zero
    /* FE04 8000F204 02202825 */  or         $a1, $s1, $zero
    /* FE08 8000F208 314B0001 */  andi       $t3, $t2, 0x1
    /* FE0C 8000F20C 5560000D */  bnel       $t3, $zero, .L8000F244
    /* FE10 8000F210 02202025 */   or        $a0, $s1, $zero
    /* FE14 8000F214 0C003B1C */  jal        func_8000EC70
    /* FE18 8000F218 27A60028 */   addiu     $a2, $sp, 0x28
    /* FE1C 8000F21C 10400003 */  beqz       $v0, .L8000F22C
    /* FE20 8000F220 97AC0040 */   lhu       $t4, 0x40($sp)
    /* FE24 8000F224 10000026 */  b          .L8000F2C0
    /* FE28 8000F228 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8000F22C:
    /* FE2C 8000F22C 318D0001 */  andi       $t5, $t4, 0x1
    /* FE30 8000F230 15A00003 */  bnez       $t5, .L8000F240
    /* FE34 8000F234 27B10028 */   addiu     $s1, $sp, 0x28
    /* FE38 8000F238 10000020 */  b          .L8000F2BC
    /* FE3C 8000F23C 2402000B */   addiu     $v0, $zero, 0xB
  .L8000F240:
    /* FE40 8000F240 02202025 */  or         $a0, $s1, $zero
  .L8000F244:
    /* FE44 8000F244 2605000C */  addiu      $a1, $s0, 0xC
    /* FE48 8000F248 0C0034AC */  jal        func_8000D2B0
    /* FE4C 8000F24C 24060020 */   addiu     $a2, $zero, 0x20
    /* FE50 8000F250 922E001B */  lbu        $t6, 0x1B($s1)
    /* FE54 8000F254 24090010 */  addiu      $t1, $zero, 0x10
    /* FE58 8000F258 240A0008 */  addiu      $t2, $zero, 0x8
    /* FE5C 8000F25C AE0E004C */  sw         $t6, 0x4C($s0)
    /* FE60 8000F260 922F001A */  lbu        $t7, 0x1A($s1)
    /* FE64 8000F264 AE090050 */  sw         $t1, 0x50($s0)
    /* FE68 8000F268 AE0A0054 */  sw         $t2, 0x54($s0)
    /* FE6C 8000F26C 31E200FF */  andi       $v0, $t7, 0xFF
    /* FE70 8000F270 000218C0 */  sll        $v1, $v0, 3
    /* FE74 8000F274 0002C040 */  sll        $t8, $v0, 1
    /* FE78 8000F278 24680008 */  addiu      $t0, $v1, 0x8
    /* FE7C 8000F27C 27190003 */  addiu      $t9, $t8, 0x3
    /* FE80 8000F280 01035821 */  addu       $t3, $t0, $v1
    /* FE84 8000F284 AE190060 */  sw         $t9, 0x60($s0)
    /* FE88 8000F288 AE080058 */  sw         $t0, 0x58($s0)
    /* FE8C 8000F28C AE0B005C */  sw         $t3, 0x5C($s0)
    /* FE90 8000F290 8E040004 */  lw         $a0, 0x4($s0)
    /* FE94 8000F294 8E050008 */  lw         $a1, 0x8($s0)
    /* FE98 8000F298 24060007 */  addiu      $a2, $zero, 0x7
    /* FE9C 8000F29C 2607002C */  addiu      $a3, $s0, 0x2C
    /* FEA0 8000F2A0 0C003A34 */  jal        func_8000E8D0
    /* FEA4 8000F2A4 A20F0064 */   sb        $t7, 0x64($s0)
    /* FEA8 8000F2A8 50400004 */  beql       $v0, $zero, .L8000F2BC
    /* FEAC 8000F2AC 00001025 */   or        $v0, $zero, $zero
    /* FEB0 8000F2B0 10000003 */  b          .L8000F2C0
    /* FEB4 8000F2B4 8FBF001C */   lw        $ra, 0x1C($sp)
    /* FEB8 8000F2B8 00001025 */  or         $v0, $zero, $zero
  .L8000F2BC:
    /* FEBC 8000F2BC 8FBF001C */  lw         $ra, 0x1C($sp)
  .L8000F2C0:
    /* FEC0 8000F2C0 8FB00014 */  lw         $s0, 0x14($sp)
    /* FEC4 8000F2C4 8FB10018 */  lw         $s1, 0x18($sp)
    /* FEC8 8000F2C8 03E00008 */  jr         $ra
    /* FECC 8000F2CC 27BD0070 */   addiu     $sp, $sp, 0x70
```

## Tips for This Category

