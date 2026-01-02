# huft_build

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80004D98` |
| **Category** | `inflate` |
| **Status** | `TODO` |
| **Instructions** | ~435 |

## Description

build Huffman decoding tables

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `inflate` category.

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
./work/inflate/huft_build/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/inflate/huft_build

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/inflate/huft_build --watch
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
# Source: 5610.s
# Address: 0x80004D98

glabel func_80004D98
    /* 5998 80004D98 27BDFE90 */  addiu      $sp, $sp, -0x170
    /* 599C 80004D9C AFB30024 */  sw         $s3, 0x24($sp)
    /* 59A0 80004DA0 AFB20020 */  sw         $s2, 0x20($sp)
    /* 59A4 80004DA4 00A09025 */  or         $s2, $a1, $zero
    /* 59A8 80004DA8 00809825 */  or         $s3, $a0, $zero
    /* 59AC 80004DAC AFBF003C */  sw         $ra, 0x3C($sp)
    /* 59B0 80004DB0 AFA60178 */  sw         $a2, 0x178($sp)
    /* 59B4 80004DB4 24060044 */  addiu      $a2, $zero, 0x44
    /* 59B8 80004DB8 27A40128 */  addiu      $a0, $sp, 0x128
    /* 59BC 80004DBC 00002825 */  or         $a1, $zero, $zero
    /* 59C0 80004DC0 AFBE0038 */  sw         $fp, 0x38($sp)
    /* 59C4 80004DC4 AFB70034 */  sw         $s7, 0x34($sp)
    /* 59C8 80004DC8 AFB60030 */  sw         $s6, 0x30($sp)
    /* 59CC 80004DCC AFB5002C */  sw         $s5, 0x2C($sp)
    /* 59D0 80004DD0 AFB40028 */  sw         $s4, 0x28($sp)
    /* 59D4 80004DD4 AFB1001C */  sw         $s1, 0x1C($sp)
    /* 59D8 80004DD8 AFB00018 */  sw         $s0, 0x18($sp)
    /* 59DC 80004DDC 0C0009E4 */  jal        func_80002790
    /* 59E0 80004DE0 AFA7017C */   sw        $a3, 0x17C($sp)
    /* 59E4 80004DE4 32420003 */  andi       $v0, $s2, 0x3
    /* 59E8 80004DE8 00021023 */  negu       $v0, $v0
    /* 59EC 80004DEC 00401825 */  or         $v1, $v0, $zero
    /* 59F0 80004DF0 27AD0128 */  addiu      $t5, $sp, 0x128
    /* 59F4 80004DF4 02605825 */  or         $t3, $s3, $zero
    /* 59F8 80004DF8 1060000D */  beqz       $v1, .L80004E30
    /* 59FC 80004DFC 0240A025 */   or        $s4, $s2, $zero
    /* 5A00 80004E00 00521821 */  addu       $v1, $v0, $s2
  .L80004E04:
    /* 5A04 80004E04 8D6E0000 */  lw         $t6, 0x0($t3)
    /* 5A08 80004E08 2694FFFF */  addiu      $s4, $s4, -0x1
    /* 5A0C 80004E0C 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5A10 80004E10 000E7880 */  sll        $t7, $t6, 2
    /* 5A14 80004E14 01AF1021 */  addu       $v0, $t5, $t7
    /* 5A18 80004E18 8C580000 */  lw         $t8, 0x0($v0)
    /* 5A1C 80004E1C 27190001 */  addiu      $t9, $t8, 0x1
    /* 5A20 80004E20 1474FFF8 */  bne        $v1, $s4, .L80004E04
    /* 5A24 80004E24 AC590000 */   sw        $t9, 0x0($v0)
    /* 5A28 80004E28 1280001F */  beqz       $s4, .L80004EA8
    /* 5A2C 80004E2C 00000000 */   nop
  .L80004E30:
    /* 5A30 80004E30 8D6E0000 */  lw         $t6, 0x0($t3)
    /* 5A34 80004E34 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5A38 80004E38 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5A3C 80004E3C 000E7880 */  sll        $t7, $t6, 2
    /* 5A40 80004E40 01AF1021 */  addu       $v0, $t5, $t7
    /* 5A44 80004E44 8C580000 */  lw         $t8, 0x0($v0)
    /* 5A48 80004E48 2694FFFC */  addiu      $s4, $s4, -0x4
    /* 5A4C 80004E4C 27190001 */  addiu      $t9, $t8, 0x1
    /* 5A50 80004E50 AC590000 */  sw         $t9, 0x0($v0)
    /* 5A54 80004E54 8D6EFFFC */  lw         $t6, -0x4($t3)
    /* 5A58 80004E58 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5A5C 80004E5C 000E7880 */  sll        $t7, $t6, 2
    /* 5A60 80004E60 01AF1021 */  addu       $v0, $t5, $t7
    /* 5A64 80004E64 8C580000 */  lw         $t8, 0x0($v0)
    /* 5A68 80004E68 27190001 */  addiu      $t9, $t8, 0x1
    /* 5A6C 80004E6C AC590000 */  sw         $t9, 0x0($v0)
    /* 5A70 80004E70 8D6EFFFC */  lw         $t6, -0x4($t3)
    /* 5A74 80004E74 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5A78 80004E78 000E7880 */  sll        $t7, $t6, 2
    /* 5A7C 80004E7C 01AF1021 */  addu       $v0, $t5, $t7
    /* 5A80 80004E80 8C580000 */  lw         $t8, 0x0($v0)
    /* 5A84 80004E84 27190001 */  addiu      $t9, $t8, 0x1
    /* 5A88 80004E88 AC590000 */  sw         $t9, 0x0($v0)
    /* 5A8C 80004E8C 8D6EFFFC */  lw         $t6, -0x4($t3)
    /* 5A90 80004E90 000E7880 */  sll        $t7, $t6, 2
    /* 5A94 80004E94 01AF1021 */  addu       $v0, $t5, $t7
    /* 5A98 80004E98 8C580000 */  lw         $t8, 0x0($v0)
    /* 5A9C 80004E9C 27190001 */  addiu      $t9, $t8, 0x1
    /* 5AA0 80004EA0 1680FFE3 */  bnez       $s4, .L80004E30
    /* 5AA4 80004EA4 AC590000 */   sw        $t9, 0x0($v0)
  .L80004EA8:
    /* 5AA8 80004EA8 8FAE0128 */  lw         $t6, 0x128($sp)
    /* 5AAC 80004EAC 164E0007 */  bne        $s2, $t6, .L80004ECC
    /* 5AB0 80004EB0 00000000 */   nop
    /* 5AB4 80004EB4 8FA70184 */  lw         $a3, 0x184($sp)
    /* 5AB8 80004EB8 8FA40188 */  lw         $a0, 0x188($sp)
    /* 5ABC 80004EBC 00001025 */  or         $v0, $zero, $zero
    /* 5AC0 80004EC0 ACE00000 */  sw         $zero, 0x0($a3)
    /* 5AC4 80004EC4 1000015B */  b          .L80005434
    /* 5AC8 80004EC8 AC800000 */   sw        $zero, 0x0($a0)
  .L80004ECC:
    /* 5ACC 80004ECC 8FA40188 */  lw         $a0, 0x188($sp)
    /* 5AD0 80004ED0 24100001 */  addiu      $s0, $zero, 0x1
    /* 5AD4 80004ED4 27A3012C */  addiu      $v1, $sp, 0x12C
    /* 5AD8 80004ED8 24020011 */  addiu      $v0, $zero, 0x11
    /* 5ADC 80004EDC 8C950000 */  lw         $s5, 0x0($a0)
  .L80004EE0:
    /* 5AE0 80004EE0 8C6F0000 */  lw         $t7, 0x0($v1)
    /* 5AE4 80004EE4 15E00004 */  bnez       $t7, .L80004EF8
    /* 5AE8 80004EE8 00000000 */   nop
    /* 5AEC 80004EEC 26100001 */  addiu      $s0, $s0, 0x1
    /* 5AF0 80004EF0 1602FFFB */  bne        $s0, $v0, .L80004EE0
    /* 5AF4 80004EF4 24630004 */   addiu     $v1, $v1, 0x4
  .L80004EF8:
    /* 5AF8 80004EF8 02B0082B */  sltu       $at, $s5, $s0
    /* 5AFC 80004EFC 10200002 */  beqz       $at, .L80004F08
    /* 5B00 80004F00 0200F025 */   or        $fp, $s0, $zero
    /* 5B04 80004F04 0200A825 */  or         $s5, $s0, $zero
  .L80004F08:
    /* 5B08 80004F08 24140010 */  addiu      $s4, $zero, 0x10
    /* 5B0C 80004F0C 27A20168 */  addiu      $v0, $sp, 0x168
  .L80004F10:
    /* 5B10 80004F10 8C580000 */  lw         $t8, 0x0($v0)
    /* 5B14 80004F14 17000004 */  bnez       $t8, .L80004F28
    /* 5B18 80004F18 00000000 */   nop
    /* 5B1C 80004F1C 2694FFFF */  addiu      $s4, $s4, -0x1
    /* 5B20 80004F20 1680FFFB */  bnez       $s4, .L80004F10
    /* 5B24 80004F24 2442FFFC */   addiu     $v0, $v0, -0x4
  .L80004F28:
    /* 5B28 80004F28 0295082B */  sltu       $at, $s4, $s5
    /* 5B2C 80004F2C 10200002 */  beqz       $at, .L80004F38
    /* 5B30 80004F30 02806025 */   or        $t4, $s4, $zero
    /* 5B34 80004F34 0280A825 */  or         $s5, $s4, $zero
  .L80004F38:
    /* 5B38 80004F38 24190001 */  addiu      $t9, $zero, 0x1
    /* 5B3C 80004F3C 0214082B */  sltu       $at, $s0, $s4
    /* 5B40 80004F40 02198804 */  sllv       $s1, $t9, $s0
    /* 5B44 80004F44 1020000D */  beqz       $at, .L80004F7C
    /* 5B48 80004F48 AC950000 */   sw        $s5, 0x0($a0)
  .L80004F4C:
    /* 5B4C 80004F4C 8C6E0000 */  lw         $t6, 0x0($v1)
    /* 5B50 80004F50 022E8823 */  subu       $s1, $s1, $t6
    /* 5B54 80004F54 06210003 */  bgez       $s1, .L80004F64
    /* 5B58 80004F58 00000000 */   nop
    /* 5B5C 80004F5C 10000135 */  b          .L80005434
    /* 5B60 80004F60 24020002 */   addiu     $v0, $zero, 0x2
  .L80004F64:
    /* 5B64 80004F64 26100001 */  addiu      $s0, $s0, 0x1
    /* 5B68 80004F68 0214082B */  sltu       $at, $s0, $s4
    /* 5B6C 80004F6C 00117840 */  sll        $t7, $s1, 1
    /* 5B70 80004F70 01E08825 */  or         $s1, $t7, $zero
    /* 5B74 80004F74 1420FFF5 */  bnez       $at, .L80004F4C
    /* 5B78 80004F78 24630004 */   addiu     $v1, $v1, 0x4
  .L80004F7C:
    /* 5B7C 80004F7C 8C430000 */  lw         $v1, 0x0($v0)
    /* 5B80 80004F80 02238823 */  subu       $s1, $s1, $v1
    /* 5B84 80004F84 06210003 */  bgez       $s1, .L80004F94
    /* 5B88 80004F88 00000000 */   nop
    /* 5B8C 80004F8C 10000129 */  b          .L80005434
    /* 5B90 80004F90 24020002 */   addiu     $v0, $zero, 0x2
  .L80004F94:
    /* 5B94 80004F94 0071C021 */  addu       $t8, $v1, $s1
    /* 5B98 80004F98 AC580000 */  sw         $t8, 0x0($v0)
    /* 5B9C 80004F9C 2694FFFF */  addiu      $s4, $s4, -0x1
    /* 5BA0 80004FA0 AFB20174 */  sw         $s2, 0x174($sp)
    /* 5BA4 80004FA4 AFA00074 */  sw         $zero, 0x74($sp)
    /* 5BA8 80004FA8 00008025 */  or         $s0, $zero, $zero
    /* 5BAC 80004FAC 27AB012C */  addiu      $t3, $sp, 0x12C
    /* 5BB0 80004FB0 27A60078 */  addiu      $a2, $sp, 0x78
    /* 5BB4 80004FB4 1280002B */  beqz       $s4, .L80005064
    /* 5BB8 80004FB8 2442FFFC */   addiu     $v0, $v0, -0x4
    /* 5BBC 80004FBC 32850003 */  andi       $a1, $s4, 0x3
    /* 5BC0 80004FC0 00052823 */  negu       $a1, $a1
    /* 5BC4 80004FC4 00A01825 */  or         $v1, $a1, $zero
    /* 5BC8 80004FC8 1060000F */  beqz       $v1, .L80005008
    /* 5BCC 80004FCC AFB20174 */   sw        $s2, 0x174($sp)
    /* 5BD0 80004FD0 00B41821 */  addu       $v1, $a1, $s4
    /* 5BD4 80004FD4 0003C880 */  sll        $t9, $v1, 2
    /* 5BD8 80004FD8 27AE0128 */  addiu      $t6, $sp, 0x128
    /* 5BDC 80004FDC 032E2021 */  addu       $a0, $t9, $t6
  .L80004FE0:
    /* 5BE0 80004FE0 8D6F0000 */  lw         $t7, 0x0($t3)
    /* 5BE4 80004FE4 2442FFFC */  addiu      $v0, $v0, -0x4
    /* 5BE8 80004FE8 24C60004 */  addiu      $a2, $a2, 0x4
    /* 5BEC 80004FEC 020F8021 */  addu       $s0, $s0, $t7
    /* 5BF0 80004FF0 ACD0FFFC */  sw         $s0, -0x4($a2)
    /* 5BF4 80004FF4 1482FFFA */  bne        $a0, $v0, .L80004FE0
    /* 5BF8 80004FF8 256B0004 */   addiu     $t3, $t3, 0x4
    /* 5BFC 80004FFC 27B80128 */  addiu      $t8, $sp, 0x128
    /* 5C00 80005000 10580018 */  beq        $v0, $t8, .L80005064
    /* 5C04 80005004 00000000 */   nop
  .L80005008:
    /* 5C08 80005008 27A30128 */  addiu      $v1, $sp, 0x128
  .L8000500C:
    /* 5C0C 8000500C 8D790000 */  lw         $t9, 0x0($t3)
    /* 5C10 80005010 24C60004 */  addiu      $a2, $a2, 0x4
    /* 5C14 80005014 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5C18 80005018 02198021 */  addu       $s0, $s0, $t9
    /* 5C1C 8000501C ACD0FFFC */  sw         $s0, -0x4($a2)
    /* 5C20 80005020 8D6E0000 */  lw         $t6, 0x0($t3)
    /* 5C24 80005024 24C60004 */  addiu      $a2, $a2, 0x4
    /* 5C28 80005028 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5C2C 8000502C 020E8021 */  addu       $s0, $s0, $t6
    /* 5C30 80005030 ACD0FFFC */  sw         $s0, -0x4($a2)
    /* 5C34 80005034 8D6F0000 */  lw         $t7, 0x0($t3)
    /* 5C38 80005038 24C60004 */  addiu      $a2, $a2, 0x4
    /* 5C3C 8000503C 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5C40 80005040 020F8021 */  addu       $s0, $s0, $t7
    /* 5C44 80005044 ACD0FFFC */  sw         $s0, -0x4($a2)
    /* 5C48 80005048 8D780000 */  lw         $t8, 0x0($t3)
    /* 5C4C 8000504C 2442FFF0 */  addiu      $v0, $v0, -0x10
    /* 5C50 80005050 24C60004 */  addiu      $a2, $a2, 0x4
    /* 5C54 80005054 02188021 */  addu       $s0, $s0, $t8
    /* 5C58 80005058 ACD0FFFC */  sw         $s0, -0x4($a2)
    /* 5C5C 8000505C 1443FFEB */  bne        $v0, $v1, .L8000500C
    /* 5C60 80005060 256B0004 */   addiu     $t3, $t3, 0x4
  .L80005064:
    /* 5C64 80005064 8FB20174 */  lw         $s2, 0x174($sp)
    /* 5C68 80005068 24040480 */  addiu      $a0, $zero, 0x480
    /* 5C6C 8000506C 0C00135B */  jal        func_80004D6C
    /* 5C70 80005070 AFAC0120 */   sw        $t4, 0x120($sp)
    /* 5C74 80005074 8FAC0120 */  lw         $t4, 0x120($sp)
    /* 5C78 80005078 27AD0128 */  addiu      $t5, $sp, 0x128
    /* 5C7C 8000507C AFA200B8 */  sw         $v0, 0xB8($sp)
    /* 5C80 80005080 02605825 */  or         $t3, $s3, $zero
    /* 5C84 80005084 0000A025 */  or         $s4, $zero, $zero
    /* 5C88 80005088 27A90070 */  addiu      $t1, $sp, 0x70
    /* 5C8C 8000508C 8FA40174 */  lw         $a0, 0x174($sp)
  .L80005090:
    /* 5C90 80005090 8D700000 */  lw         $s0, 0x0($t3)
    /* 5C94 80005094 AFB10068 */  sw         $s1, 0x68($sp)
    /* 5C98 80005098 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5C9C 8000509C 1200000A */  beqz       $s0, .L800050C8
    /* 5CA0 800050A0 00000000 */   nop
    /* 5CA4 800050A4 0010C880 */  sll        $t9, $s0, 2
    /* 5CA8 800050A8 01391821 */  addu       $v1, $t1, $t9
    /* 5CAC 800050AC 8C6E0000 */  lw         $t6, 0x0($v1)
    /* 5CB0 800050B0 000E7880 */  sll        $t7, $t6, 2
    /* 5CB4 800050B4 004FC021 */  addu       $t8, $v0, $t7
    /* 5CB8 800050B8 AF140000 */  sw         $s4, 0x0($t8)
    /* 5CBC 800050BC 8C790000 */  lw         $t9, 0x0($v1)
    /* 5CC0 800050C0 272E0001 */  addiu      $t6, $t9, 0x1
    /* 5CC4 800050C4 AC6E0000 */  sw         $t6, 0x0($v1)
  .L800050C8:
    /* 5CC8 800050C8 26940001 */  addiu      $s4, $s4, 0x1
    /* 5CCC 800050CC 0284082B */  sltu       $at, $s4, $a0
    /* 5CD0 800050D0 1420FFEF */  bnez       $at, .L80005090
    /* 5CD4 800050D4 8FB10068 */   lw        $s1, 0x68($sp)
    /* 5CD8 800050D8 019E082A */  slt        $at, $t4, $fp
    /* 5CDC 800050DC 0000A025 */  or         $s4, $zero, $zero
    /* 5CE0 800050E0 AFA00070 */  sw         $zero, 0x70($sp)
    /* 5CE4 800050E4 00405825 */  or         $t3, $v0, $zero
    /* 5CE8 800050E8 2413FFFF */  addiu      $s3, $zero, -0x1
    /* 5CEC 800050EC 00159023 */  negu       $s2, $s5
    /* 5CF0 800050F0 AFA000BC */  sw         $zero, 0xBC($sp)
    /* 5CF4 800050F4 00003025 */  or         $a2, $zero, $zero
    /* 5CF8 800050F8 142000C7 */  bnez       $at, .L80005418
    /* 5CFC 800050FC 00008825 */   or        $s1, $zero, $zero
    /* 5D00 80005100 001E7880 */  sll        $t7, $fp, 2
    /* 5D04 80005104 01AFC021 */  addu       $t8, $t5, $t7
    /* 5D08 80005108 AFB80050 */  sw         $t8, 0x50($sp)
    /* 5D0C 8000510C 27B700FC */  addiu      $s7, $sp, 0xFC
    /* 5D10 80005110 8FA70184 */  lw         $a3, 0x184($sp)
  .L80005114:
    /* 5D14 80005114 8FB90050 */  lw         $t9, 0x50($sp)
    /* 5D18 80005118 8F280000 */  lw         $t0, 0x0($t9)
    /* 5D1C 8000511C 01002825 */  or         $a1, $t0, $zero
    /* 5D20 80005120 10A000B6 */  beqz       $a1, .L800053FC
    /* 5D24 80005124 2508FFFF */   addiu     $t0, $t0, -0x1
  .L80005128:
    /* 5D28 80005128 02551821 */  addu       $v1, $s2, $s5
    /* 5D2C 8000512C 007E082A */  slt        $at, $v1, $fp
    /* 5D30 80005130 10200052 */  beqz       $at, .L8000527C
    /* 5D34 80005134 00000000 */   nop
    /* 5D38 80005138 25160001 */  addiu      $s6, $t0, 0x1
  .L8000513C:
    /* 5D3C 8000513C 00609025 */  or         $s2, $v1, $zero
    /* 5D40 80005140 01921023 */  subu       $v0, $t4, $s2
    /* 5D44 80005144 02A2082B */  sltu       $at, $s5, $v0
    /* 5D48 80005148 00408825 */  or         $s1, $v0, $zero
    /* 5D4C 8000514C 10200002 */  beqz       $at, .L80005158
    /* 5D50 80005150 26730001 */   addiu     $s3, $s3, 0x1
    /* 5D54 80005154 02A08825 */  or         $s1, $s5, $zero
  .L80005158:
    /* 5D58 80005158 03D21023 */  subu       $v0, $fp, $s2
    /* 5D5C 8000515C 240E0001 */  addiu      $t6, $zero, 0x1
    /* 5D60 80005160 004E2004 */  sllv       $a0, $t6, $v0
    /* 5D64 80005164 02C4082B */  sltu       $at, $s6, $a0
    /* 5D68 80005168 10200013 */  beqz       $at, .L800051B8
    /* 5D6C 8000516C 00408025 */   or        $s0, $v0, $zero
    /* 5D70 80005170 24500001 */  addiu      $s0, $v0, 0x1
    /* 5D74 80005174 0211082B */  sltu       $at, $s0, $s1
    /* 5D78 80005178 00881823 */  subu       $v1, $a0, $t0
    /* 5D7C 8000517C 001E7880 */  sll        $t7, $fp, 2
    /* 5D80 80005180 01AF3021 */  addu       $a2, $t5, $t7
    /* 5D84 80005184 1020000C */  beqz       $at, .L800051B8
    /* 5D88 80005188 2463FFFF */   addiu     $v1, $v1, -0x1
  .L8000518C:
    /* 5D8C 8000518C 8CC40004 */  lw         $a0, 0x4($a2)
    /* 5D90 80005190 00031040 */  sll        $v0, $v1, 1
    /* 5D94 80005194 00402825 */  or         $a1, $v0, $zero
    /* 5D98 80005198 0085082B */  sltu       $at, $a0, $a1
    /* 5D9C 8000519C 00401825 */  or         $v1, $v0, $zero
    /* 5DA0 800051A0 10200005 */  beqz       $at, .L800051B8
    /* 5DA4 800051A4 24C60004 */   addiu     $a2, $a2, 0x4
    /* 5DA8 800051A8 26100001 */  addiu      $s0, $s0, 0x1
    /* 5DAC 800051AC 0211082B */  sltu       $at, $s0, $s1
    /* 5DB0 800051B0 1420FFF6 */  bnez       $at, .L8000518C
    /* 5DB4 800051B4 00641823 */   subu      $v1, $v1, $a0
  .L800051B8:
    /* 5DB8 800051B8 24180001 */  addiu      $t8, $zero, 0x1
    /* 5DBC 800051BC 02181004 */  sllv       $v0, $t8, $s0
    /* 5DC0 800051C0 000220C0 */  sll        $a0, $v0, 3
    /* 5DC4 800051C4 24840008 */  addiu      $a0, $a0, 0x8
    /* 5DC8 800051C8 00408825 */  or         $s1, $v0, $zero
    /* 5DCC 800051CC AFA70184 */  sw         $a3, 0x184($sp)
    /* 5DD0 800051D0 AFA8016C */  sw         $t0, 0x16C($sp)
    /* 5DD4 800051D4 AFAB0108 */  sw         $t3, 0x108($sp)
    /* 5DD8 800051D8 0C00135B */  jal        func_80004D6C
    /* 5DDC 800051DC AFAC0120 */   sw        $t4, 0x120($sp)
    /* 5DE0 800051E0 8FA70184 */  lw         $a3, 0x184($sp)
    /* 5DE4 800051E4 8FA8016C */  lw         $t0, 0x16C($sp)
    /* 5DE8 800051E8 27A90070 */  addiu      $t1, $sp, 0x70
    /* 5DEC 800051EC 8FAB0108 */  lw         $t3, 0x108($sp)
    /* 5DF0 800051F0 8FAC0120 */  lw         $t4, 0x120($sp)
    /* 5DF4 800051F4 27AD0128 */  addiu      $t5, $sp, 0x128
    /* 5DF8 800051F8 14400003 */  bnez       $v0, .L80005208
    /* 5DFC 800051FC 00403025 */   or        $a2, $v0, $zero
    /* 5E00 80005200 1000008C */  b          .L80005434
    /* 5E04 80005204 24020003 */   addiu     $v0, $zero, 0x3
  .L80005208:
    /* 5E08 80005208 24430008 */  addiu      $v1, $v0, 0x8
    /* 5E0C 8000520C ACE30000 */  sw         $v1, 0x0($a3)
    /* 5E10 80005210 00132080 */  sll        $a0, $s3, 2
    /* 5E14 80005214 27B900BC */  addiu      $t9, $sp, 0xBC
    /* 5E18 80005218 00992821 */  addu       $a1, $a0, $t9
    /* 5E1C 8000521C AC400004 */  sw         $zero, 0x4($v0)
    /* 5E20 80005220 00603025 */  or         $a2, $v1, $zero
    /* 5E24 80005224 ACA60000 */  sw         $a2, 0x0($a1)
    /* 5E28 80005228 12600010 */  beqz       $s3, .L8000526C
    /* 5E2C 8000522C 24470004 */   addiu     $a3, $v0, 0x4
    /* 5E30 80005230 01247021 */  addu       $t6, $t1, $a0
    /* 5E34 80005234 ADD40000 */  sw         $s4, 0x0($t6)
    /* 5E38 80005238 260F0010 */  addiu      $t7, $s0, 0x10
    /* 5E3C 8000523C A3AF00FC */  sb         $t7, 0xFC($sp)
    /* 5E40 80005240 AFA60100 */  sw         $a2, 0x100($sp)
    /* 5E44 80005244 A3B500FD */  sb         $s5, 0xFD($sp)
    /* 5E48 80005248 8CB8FFFC */  lw         $t8, -0x4($a1)
    /* 5E4C 8000524C 0255C823 */  subu       $t9, $s2, $s5
    /* 5E50 80005250 8EE10000 */  lw         $at, 0x0($s7)
    /* 5E54 80005254 03347006 */  srlv       $t6, $s4, $t9
    /* 5E58 80005258 000E78C0 */  sll        $t7, $t6, 3
    /* 5E5C 8000525C 030FC821 */  addu       $t9, $t8, $t7
    /* 5E60 80005260 AF210000 */  sw         $at, 0x0($t9)
    /* 5E64 80005264 8EEF0004 */  lw         $t7, 0x4($s7)
    /* 5E68 80005268 AF2F0004 */  sw         $t7, 0x4($t9)
  .L8000526C:
    /* 5E6C 8000526C 02551821 */  addu       $v1, $s2, $s5
    /* 5E70 80005270 007E082A */  slt        $at, $v1, $fp
    /* 5E74 80005274 1420FFB1 */  bnez       $at, .L8000513C
    /* 5E78 80005278 00000000 */   nop
  .L8000527C:
    /* 5E7C 8000527C 0013C080 */  sll        $t8, $s3, 2
    /* 5E80 80005280 01381821 */  addu       $v1, $t1, $t8
    /* 5E84 80005284 27CE001F */  addiu      $t6, $fp, 0x1F
    /* 5E88 80005288 24190001 */  addiu      $t9, $zero, 0x1
    /* 5E8C 8000528C 8FB80174 */  lw         $t8, 0x174($sp)
    /* 5E90 80005290 01D95004 */  sllv       $t2, $t9, $t6
    /* 5E94 80005294 8FAE00B8 */  lw         $t6, 0xB8($sp)
    /* 5E98 80005298 240F0001 */  addiu      $t7, $zero, 0x1
    /* 5E9C 8000529C 024FF804 */  sllv       $ra, $t7, $s2
    /* 5EA0 800052A0 0018C880 */  sll        $t9, $t8, 2
    /* 5EA4 800052A4 032E7821 */  addu       $t7, $t9, $t6
    /* 5EA8 800052A8 016F082B */  sltu       $at, $t3, $t7
    /* 5EAC 800052AC 03D22823 */  subu       $a1, $fp, $s2
    /* 5EB0 800052B0 A3A500FD */  sb         $a1, 0xFD($sp)
    /* 5EB4 800052B4 14200004 */  bnez       $at, .L800052C8
    /* 5EB8 800052B8 27FFFFFF */   addiu     $ra, $ra, -0x1
    /* 5EBC 800052BC 24180063 */  addiu      $t8, $zero, 0x63
    /* 5EC0 800052C0 10000025 */  b          .L80005358
    /* 5EC4 800052C4 A3B800FC */   sb        $t8, 0xFC($sp)
  .L800052C8:
    /* 5EC8 800052C8 8D640000 */  lw         $a0, 0x0($t3)
    /* 5ECC 800052CC 8FB90178 */  lw         $t9, 0x178($sp)
    /* 5ED0 800052D0 0099082B */  sltu       $at, $a0, $t9
    /* 5ED4 800052D4 1020000D */  beqz       $at, .L8000530C
    /* 5ED8 800052D8 00000000 */   nop
    /* 5EDC 800052DC 2C810100 */  sltiu      $at, $a0, 0x100
    /* 5EE0 800052E0 10200004 */  beqz       $at, .L800052F4
    /* 5EE4 800052E4 00000000 */   nop
    /* 5EE8 800052E8 240E0010 */  addiu      $t6, $zero, 0x10
    /* 5EEC 800052EC 10000003 */  b          .L800052FC
    /* 5EF0 800052F0 A3AE00FC */   sb        $t6, 0xFC($sp)
  .L800052F4:
    /* 5EF4 800052F4 240F000F */  addiu      $t7, $zero, 0xF
    /* 5EF8 800052F8 A3AF00FC */  sb         $t7, 0xFC($sp)
  .L800052FC:
    /* 5EFC 800052FC 8D780000 */  lw         $t8, 0x0($t3)
    /* 5F00 80005300 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5F04 80005304 10000014 */  b          .L80005358
    /* 5F08 80005308 A7B80100 */   sh        $t8, 0x100($sp)
  .L8000530C:
    /* 5F0C 8000530C 8FA20178 */  lw         $v0, 0x178($sp)
    /* 5F10 80005310 8FAE0180 */  lw         $t6, 0x180($sp)
    /* 5F14 80005314 00047840 */  sll        $t7, $a0, 1
    /* 5F18 80005318 0002C840 */  sll        $t9, $v0, 1
    /* 5F1C 8000531C 03201025 */  or         $v0, $t9, $zero
    /* 5F20 80005320 0002C823 */  negu       $t9, $v0
    /* 5F24 80005324 01CFC021 */  addu       $t8, $t6, $t7
    /* 5F28 80005328 03197021 */  addu       $t6, $t8, $t9
    /* 5F2C 8000532C 95CF0000 */  lhu        $t7, 0x0($t6)
    /* 5F30 80005330 8FB8017C */  lw         $t8, 0x17C($sp)
    /* 5F34 80005334 256B0004 */  addiu      $t3, $t3, 0x4
    /* 5F38 80005338 A3AF00FC */  sb         $t7, 0xFC($sp)
    /* 5F3C 8000533C 8D79FFFC */  lw         $t9, -0x4($t3)
    /* 5F40 80005340 00197040 */  sll        $t6, $t9, 1
    /* 5F44 80005344 0002C823 */  negu       $t9, $v0
    /* 5F48 80005348 030E7821 */  addu       $t7, $t8, $t6
    /* 5F4C 8000534C 01F9C021 */  addu       $t8, $t7, $t9
    /* 5F50 80005350 970E0000 */  lhu        $t6, 0x0($t8)
    /* 5F54 80005354 A7AE0100 */  sh         $t6, 0x100($sp)
  .L80005358:
    /* 5F58 80005358 02541006 */  srlv       $v0, $s4, $s2
    /* 5F5C 8000535C 0051082B */  sltu       $at, $v0, $s1
    /* 5F60 80005360 1020000C */  beqz       $at, .L80005394
    /* 5F64 80005364 00408025 */   or        $s0, $v0, $zero
    /* 5F68 80005368 240F0001 */  addiu      $t7, $zero, 0x1
    /* 5F6C 8000536C 00AF1004 */  sllv       $v0, $t7, $a1
  .L80005370:
    /* 5F70 80005370 8EE10000 */  lw         $at, 0x0($s7)
    /* 5F74 80005374 0010C8C0 */  sll        $t9, $s0, 3
    /* 5F78 80005378 00D9C021 */  addu       $t8, $a2, $t9
    /* 5F7C 8000537C AF010000 */  sw         $at, 0x0($t8)
    /* 5F80 80005380 8EEF0004 */  lw         $t7, 0x4($s7)
    /* 5F84 80005384 02028021 */  addu       $s0, $s0, $v0
    /* 5F88 80005388 0211082B */  sltu       $at, $s0, $s1
    /* 5F8C 8000538C 1420FFF8 */  bnez       $at, .L80005370
    /* 5F90 80005390 AF0F0004 */   sw        $t7, 0x4($t8)
  .L80005394:
    /* 5F94 80005394 028AC824 */  and        $t9, $s4, $t2
    /* 5F98 80005398 01408025 */  or         $s0, $t2, $zero
    /* 5F9C 8000539C 13200007 */  beqz       $t9, .L800053BC
    /* 5FA0 800053A0 8C620000 */   lw        $v0, 0x0($v1)
  .L800053A4:
    /* 5FA4 800053A4 0290A026 */  xor        $s4, $s4, $s0
    /* 5FA8 800053A8 00107042 */  srl        $t6, $s0, 1
    /* 5FAC 800053AC 01C08025 */  or         $s0, $t6, $zero
    /* 5FB0 800053B0 0290C024 */  and        $t8, $s4, $s0
    /* 5FB4 800053B4 1700FFFB */  bnez       $t8, .L800053A4
    /* 5FB8 800053B8 00000000 */   nop
  .L800053BC:
    /* 5FBC 800053BC 0290A026 */  xor        $s4, $s4, $s0
    /* 5FC0 800053C0 029F7824 */  and        $t7, $s4, $ra
    /* 5FC4 800053C4 11E2000A */  beq        $t7, $v0, .L800053F0
    /* 5FC8 800053C8 00000000 */   nop
  .L800053CC:
    /* 5FCC 800053CC 02559023 */  subu       $s2, $s2, $s5
    /* 5FD0 800053D0 24190001 */  addiu      $t9, $zero, 0x1
    /* 5FD4 800053D4 02597004 */  sllv       $t6, $t9, $s2
    /* 5FD8 800053D8 8C79FFFC */  lw         $t9, -0x4($v1)
    /* 5FDC 800053DC 25D8FFFF */  addiu      $t8, $t6, -0x1
    /* 5FE0 800053E0 02987824 */  and        $t7, $s4, $t8
    /* 5FE4 800053E4 2673FFFF */  addiu      $s3, $s3, -0x1
    /* 5FE8 800053E8 15F9FFF8 */  bne        $t7, $t9, .L800053CC
    /* 5FEC 800053EC 2463FFFC */   addiu     $v1, $v1, -0x4
  .L800053F0:
    /* 5FF0 800053F0 01002825 */  or         $a1, $t0, $zero
    /* 5FF4 800053F4 14A0FF4C */  bnez       $a1, .L80005128
    /* 5FF8 800053F8 2508FFFF */   addiu     $t0, $t0, -0x1
  .L800053FC:
    /* 5FFC 800053FC 8FAE0050 */  lw         $t6, 0x50($sp)
    /* 6000 80005400 27DE0001 */  addiu      $fp, $fp, 0x1
    /* 6004 80005404 019E082A */  slt        $at, $t4, $fp
    /* 6008 80005408 25D80004 */  addiu      $t8, $t6, 0x4
    /* 600C 8000540C 1020FF41 */  beqz       $at, .L80005114
    /* 6010 80005410 AFB80050 */   sw        $t8, 0x50($sp)
    /* 6014 80005414 AFA70184 */  sw         $a3, 0x184($sp)
  .L80005418:
    /* 6018 80005418 8FA20068 */  lw         $v0, 0x68($sp)
    /* 601C 8000541C 0002782B */  sltu       $t7, $zero, $v0
    /* 6020 80005420 01E01025 */  or         $v0, $t7, $zero
    /* 6024 80005424 10400003 */  beqz       $v0, .L80005434
    /* 6028 80005428 00000000 */   nop
    /* 602C 8000542C 39820001 */  xori       $v0, $t4, 0x1
    /* 6030 80005430 0002102B */  sltu       $v0, $zero, $v0
  .L80005434:
    /* 6034 80005434 8FBF003C */  lw         $ra, 0x3C($sp)
    /* 6038 80005438 8FB00018 */  lw         $s0, 0x18($sp)
    /* 603C 8000543C 8FB1001C */  lw         $s1, 0x1C($sp)
    /* 6040 80005440 8FB20020 */  lw         $s2, 0x20($sp)
    /* 6044 80005444 8FB30024 */  lw         $s3, 0x24($sp)
    /* 6048 80005448 8FB40028 */  lw         $s4, 0x28($sp)
    /* 604C 8000544C 8FB5002C */  lw         $s5, 0x2C($sp)
    /* 6050 80005450 8FB60030 */  lw         $s6, 0x30($sp)
    /* 6054 80005454 8FB70034 */  lw         $s7, 0x34($sp)
    /* 6058 80005458 8FBE0038 */  lw         $fp, 0x38($sp)
    /* 605C 8000545C 03E00008 */  jr         $ra
    /* 6060 80005460 27BD0170 */   addiu     $sp, $sp, 0x170
```

## Tips for This Category

