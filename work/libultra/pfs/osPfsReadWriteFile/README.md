# osPfsReadWriteFile

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000A970` |
| **Category** | `libultra/pfs` |
| **Status** | `TODO` |
| **Instructions** | ~205 |

## Description

read/write controller pak file

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/pfs` category.

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
./work/libultra/pfs/osPfsReadWriteFile/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pfs/osPfsReadWriteFile

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pfs/osPfsReadWriteFile --watch
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
# Source: B570.s
# Address: 0x8000A970

glabel func_8000A970
    /* B570 8000A970 27BDFD68 */  addiu      $sp, $sp, -0x298
    /* B574 8000A974 AFB70040 */  sw         $s7, 0x40($sp)
    /* B578 8000A978 AFB6003C */  sw         $s6, 0x3C($sp)
    /* B57C 8000A97C AFB50038 */  sw         $s5, 0x38($sp)
    /* B580 8000A980 AFB30030 */  sw         $s3, 0x30($sp)
    /* B584 8000A984 AFA5029C */  sw         $a1, 0x29C($sp)
    /* B588 8000A988 30AEFFFF */  andi       $t6, $a1, 0xFFFF
    /* B58C 8000A98C 01C02825 */  or         $a1, $t6, $zero
    /* B590 8000A990 00809825 */  or         $s3, $a0, $zero
    /* B594 8000A994 AFBF0044 */  sw         $ra, 0x44($sp)
    /* B598 8000A998 AFB40034 */  sw         $s4, 0x34($sp)
    /* B59C 8000A99C AFB2002C */  sw         $s2, 0x2C($sp)
    /* B5A0 8000A9A0 AFB10028 */  sw         $s1, 0x28($sp)
    /* B5A4 8000A9A4 AFB00024 */  sw         $s0, 0x24($sp)
    /* B5A8 8000A9A8 AFA702A4 */  sw         $a3, 0x2A4($sp)
    /* B5AC 8000A9AC 0000B025 */  or         $s6, $zero, $zero
    /* B5B0 8000A9B0 0000B825 */  or         $s7, $zero, $zero
    /* B5B4 8000A9B4 11C00003 */  beqz       $t6, .L8000A9C4
    /* B5B8 8000A9B8 0000A825 */   or        $s5, $zero, $zero
    /* B5BC 8000A9BC 14C00003 */  bnez       $a2, .L8000A9CC
    /* B5C0 8000A9C0 8FB102AC */   lw        $s1, 0x2AC($sp)
  .L8000A9C4:
    /* B5C4 8000A9C4 100000AC */  b          .L8000AC78
    /* B5C8 8000A9C8 24020005 */   addiu     $v0, $zero, 0x5
  .L8000A9CC:
    /* B5CC 8000A9CC 263000FF */  addiu      $s0, $s1, 0xFF
    /* B5D0 8000A9D0 8FB802A8 */  lw         $t8, 0x2A8($sp)
    /* B5D4 8000A9D4 8FB902B0 */  lw         $t9, 0x2B0($sp)
    /* B5D8 8000A9D8 06010003 */  bgez       $s0, .L8000A9E8
    /* B5DC 8000A9DC 00107A03 */   sra       $t7, $s0, 8
    /* B5E0 8000A9E0 260100FF */  addiu      $at, $s0, 0xFF
    /* B5E4 8000A9E4 00017A03 */  sra        $t7, $at, 8
  .L8000A9E8:
    /* B5E8 8000A9E8 01E08025 */  or         $s0, $t7, $zero
    /* B5EC 8000A9EC 01E0A025 */  or         $s4, $t7, $zero
    /* B5F0 8000A9F0 02602025 */  or         $a0, $s3, $zero
    /* B5F4 8000A9F4 8FA702A4 */  lw         $a3, 0x2A4($sp)
    /* B5F8 8000A9F8 A7A5029E */  sh         $a1, 0x29E($sp)
    /* B5FC 8000A9FC AFA602A0 */  sw         $a2, 0x2A0($sp)
    /* B600 8000AA00 AFB80010 */  sw         $t8, 0x10($sp)
    /* B604 8000AA04 0C0028D4 */  jal        func_8000A350
    /* B608 8000AA08 AFB90014 */   sw        $t9, 0x14($sp)
    /* B60C 8000AA0C 10400005 */  beqz       $v0, .L8000AA24
    /* B610 8000AA10 24010005 */   addiu     $at, $zero, 0x5
    /* B614 8000AA14 50410004 */  beql       $v0, $at, .L8000AA28
    /* B618 8000AA18 8FA802B0 */   lw        $t0, 0x2B0($sp)
    /* B61C 8000AA1C 10000097 */  b          .L8000AC7C
    /* B620 8000AA20 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000AA24:
    /* B624 8000AA24 8FA802B0 */  lw         $t0, 0x2B0($sp)
  .L8000AA28:
    /* B628 8000AA28 2401FFFF */  addiu      $at, $zero, -0x1
    /* B62C 8000AA2C 02602025 */  or         $a0, $s3, $zero
    /* B630 8000AA30 8D090000 */  lw         $t1, 0x0($t0)
    /* B634 8000AA34 11210003 */  beq        $t1, $at, .L8000AA44
    /* B638 8000AA38 00000000 */   nop
    /* B63C 8000AA3C 1000008E */  b          .L8000AC78
    /* B640 8000AA40 24020009 */   addiu     $v0, $zero, 0x9
  .L8000AA44:
    /* B644 8000AA44 0C002C90 */  jal        func_8000B240
    /* B648 8000AA48 27A50054 */   addiu     $a1, $sp, 0x54
    /* B64C 8000AA4C 8FAA0054 */  lw         $t2, 0x54($sp)
    /* B650 8000AA50 0151082A */  slt        $at, $t2, $s1
    /* B654 8000AA54 10200003 */  beqz       $at, .L8000AA64
    /* B658 8000AA58 00000000 */   nop
    /* B65C 8000AA5C 10000086 */  b          .L8000AC78
    /* B660 8000AA60 24020007 */   addiu     $v0, $zero, 0x7
  .L8000AA64:
    /* B664 8000AA64 16000003 */  bnez       $s0, .L8000AA74
    /* B668 8000AA68 02602025 */   or        $a0, $s3, $zero
    /* B66C 8000AA6C 10000082 */  b          .L8000AC78
    /* B670 8000AA70 24020005 */   addiu     $v0, $zero, 0x5
  .L8000AA74:
    /* B674 8000AA74 8FAB02B0 */  lw         $t3, 0x2B0($sp)
    /* B678 8000AA78 00002825 */  or         $a1, $zero, $zero
    /* B67C 8000AA7C 00003025 */  or         $a2, $zero, $zero
    /* B680 8000AA80 00003825 */  or         $a3, $zero, $zero
    /* B684 8000AA84 AFA00010 */  sw         $zero, 0x10($sp)
    /* B688 8000AA88 0C0028D4 */  jal        func_8000A350
    /* B68C 8000AA8C AFAB0014 */   sw        $t3, 0x14($sp)
    /* B690 8000AA90 10400005 */  beqz       $v0, .L8000AAA8
    /* B694 8000AA94 24010005 */   addiu     $at, $zero, 0x5
    /* B698 8000AA98 50410004 */  beql       $v0, $at, .L8000AAAC
    /* B69C 8000AA9C 8FAC02B0 */   lw        $t4, 0x2B0($sp)
    /* B6A0 8000AAA0 10000076 */  b          .L8000AC7C
    /* B6A4 8000AAA4 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000AAA8:
    /* B6A8 8000AAA8 8FAC02B0 */  lw         $t4, 0x2B0($sp)
  .L8000AAAC:
    /* B6AC 8000AAAC 2401FFFF */  addiu      $at, $zero, -0x1
    /* B6B0 8000AAB0 8D8D0000 */  lw         $t5, 0x0($t4)
    /* B6B4 8000AAB4 55A10004 */  bnel       $t5, $at, .L8000AAC8
    /* B6B8 8000AAB8 926E0064 */   lbu       $t6, 0x64($s3)
    /* B6BC 8000AABC 1000006E */  b          .L8000AC78
    /* B6C0 8000AAC0 24020008 */   addiu     $v0, $zero, 0x8
    /* B6C4 8000AAC4 926E0064 */  lbu        $t6, 0x64($s3)
  .L8000AAC8:
    /* B6C8 8000AAC8 00008025 */  or         $s0, $zero, $zero
    /* B6CC 8000AACC 27B20180 */  addiu      $s2, $sp, 0x180
    /* B6D0 8000AAD0 19C00049 */  blez       $t6, .L8000ABF8
    /* B6D4 8000AAD4 27B10080 */   addiu     $s1, $sp, 0x80
  .L8000AAD8:
    /* B6D8 8000AAD8 02602025 */  or         $a0, $s3, $zero
    /* B6DC 8000AADC 02402825 */  or         $a1, $s2, $zero
    /* B6E0 8000AAE0 00003025 */  or         $a2, $zero, $zero
    /* B6E4 8000AAE4 0C003CE9 */  jal        func_8000F3A4
    /* B6E8 8000AAE8 320700FF */   andi      $a3, $s0, 0xFF
    /* B6EC 8000AAEC 10400003 */  beqz       $v0, .L8000AAFC
    /* B6F0 8000AAF0 02602025 */   or        $a0, $s3, $zero
    /* B6F4 8000AAF4 10000061 */  b          .L8000AC7C
    /* B6F8 8000AAF8 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000AAFC:
    /* B6FC 8000AAFC 27AF0290 */  addiu      $t7, $sp, 0x290
    /* B700 8000AB00 27B8028C */  addiu      $t8, $sp, 0x28C
    /* B704 8000AB04 AFB80018 */  sw         $t8, 0x18($sp)
    /* B708 8000AB08 AFAF0014 */  sw         $t7, 0x14($sp)
    /* B70C 8000AB0C 02402825 */  or         $a1, $s2, $zero
    /* B710 8000AB10 02803025 */  or         $a2, $s4, $zero
    /* B714 8000AB14 27A70294 */  addiu      $a3, $sp, 0x294
    /* B718 8000AB18 0C002B29 */  jal        func_8000ACA4
    /* B71C 8000AB1C AFB00010 */   sw        $s0, 0x10($sp)
    /* B720 8000AB20 10400003 */  beqz       $v0, .L8000AB30
    /* B724 8000AB24 8FA30294 */   lw        $v1, 0x294($sp)
    /* B728 8000AB28 10000054 */  b          .L8000AC7C
    /* B72C 8000AB2C 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000AB30:
    /* B730 8000AB30 2401FFFF */  addiu      $at, $zero, -0x1
    /* B734 8000AB34 5061002B */  beql       $v1, $at, .L8000ABE4
    /* B738 8000AB38 926B0064 */   lbu       $t3, 0x64($s3)
    /* B73C 8000AB3C 16A00004 */  bnez       $s5, .L8000AB50
    /* B740 8000AB40 0016C840 */   sll       $t9, $s6, 1
    /* B744 8000AB44 A3A30051 */  sb         $v1, 0x51($sp)
    /* B748 8000AB48 1000000D */  b          .L8000AB80
    /* B74C 8000AB4C A3B00050 */   sb        $s0, 0x50($sp)
  .L8000AB50:
    /* B750 8000AB50 02391021 */  addu       $v0, $s1, $t9
    /* B754 8000AB54 A0500000 */  sb         $s0, 0x0($v0)
    /* B758 8000AB58 A0430001 */  sb         $v1, 0x1($v0)
    /* B75C 8000AB5C 02602025 */  or         $a0, $s3, $zero
    /* B760 8000AB60 02202825 */  or         $a1, $s1, $zero
    /* B764 8000AB64 24060001 */  addiu      $a2, $zero, 0x1
    /* B768 8000AB68 0C003CE9 */  jal        func_8000F3A4
    /* B76C 8000AB6C 32E700FF */   andi      $a3, $s7, 0xFF
    /* B770 8000AB70 50400004 */  beql       $v0, $zero, .L8000AB84
    /* B774 8000AB74 8FA80290 */   lw        $t0, 0x290($sp)
    /* B778 8000AB78 10000040 */  b          .L8000AC7C
    /* B77C 8000AB7C 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000AB80:
    /* B780 8000AB80 8FA80290 */  lw         $t0, 0x290($sp)
  .L8000AB84:
    /* B784 8000AB84 02402025 */  or         $a0, $s2, $zero
    /* B788 8000AB88 02202825 */  or         $a1, $s1, $zero
    /* B78C 8000AB8C 0114082A */  slt        $at, $t0, $s4
    /* B790 8000AB90 5020000A */  beql       $at, $zero, .L8000ABBC
    /* B794 8000AB94 0000A025 */   or        $s4, $zero, $zero
    /* B798 8000AB98 0C0034AC */  jal        func_8000D2B0
    /* B79C 8000AB9C 24060100 */   addiu     $a2, $zero, 0x100
    /* B7A0 8000ABA0 8FA90290 */  lw         $t1, 0x290($sp)
    /* B7A4 8000ABA4 8FB6028C */  lw         $s6, 0x28C($sp)
    /* B7A8 8000ABA8 321700FF */  andi       $s7, $s0, 0xFF
    /* B7AC 8000ABAC 26B50001 */  addiu      $s5, $s5, 0x1
    /* B7B0 8000ABB0 1000000B */  b          .L8000ABE0
    /* B7B4 8000ABB4 0289A023 */   subu      $s4, $s4, $t1
    /* B7B8 8000ABB8 0000A025 */  or         $s4, $zero, $zero
  .L8000ABBC:
    /* B7BC 8000ABBC 02602025 */  or         $a0, $s3, $zero
    /* B7C0 8000ABC0 02402825 */  or         $a1, $s2, $zero
    /* B7C4 8000ABC4 24060001 */  addiu      $a2, $zero, 0x1
    /* B7C8 8000ABC8 0C003CE9 */  jal        func_8000F3A4
    /* B7CC 8000ABCC 320700FF */   andi      $a3, $s0, 0xFF
    /* B7D0 8000ABD0 10400009 */  beqz       $v0, .L8000ABF8
    /* B7D4 8000ABD4 00000000 */   nop
    /* B7D8 8000ABD8 10000028 */  b          .L8000AC7C
    /* B7DC 8000ABDC 8FBF0044 */   lw        $ra, 0x44($sp)
  .L8000ABE0:
    /* B7E0 8000ABE0 926B0064 */  lbu        $t3, 0x64($s3)
  .L8000ABE4:
    /* B7E4 8000ABE4 26100001 */  addiu      $s0, $s0, 0x1
    /* B7E8 8000ABE8 320A00FF */  andi       $t2, $s0, 0xFF
    /* B7EC 8000ABEC 014B082A */  slt        $at, $t2, $t3
    /* B7F0 8000ABF0 1420FFB9 */  bnez       $at, .L8000AAD8
    /* B7F4 8000ABF4 01408025 */   or        $s0, $t2, $zero
  .L8000ABF8:
    /* B7F8 8000ABF8 1E800004 */  bgtz       $s4, .L8000AC0C
    /* B7FC 8000ABFC 8FA30294 */   lw        $v1, 0x294($sp)
    /* B800 8000AC00 2401FFFF */  addiu      $at, $zero, -0x1
    /* B804 8000AC04 14610003 */  bne        $v1, $at, .L8000AC14
    /* B808 8000AC08 97AC0050 */   lhu       $t4, 0x50($sp)
  .L8000AC0C:
    /* B80C 8000AC0C 1000001A */  b          .L8000AC78
    /* B810 8000AC10 24020003 */   addiu     $v0, $zero, 0x3
  .L8000AC14:
    /* B814 8000AC14 97AD029E */  lhu        $t5, 0x29E($sp)
    /* B818 8000AC18 8FAE02A0 */  lw         $t6, 0x2A0($sp)
    /* B81C 8000AC1C A7AC0066 */  sh         $t4, 0x66($sp)
    /* B820 8000AC20 A7A0006A */  sh         $zero, 0x6A($sp)
    /* B824 8000AC24 8FA402A4 */  lw         $a0, 0x2A4($sp)
    /* B828 8000AC28 27A50070 */  addiu      $a1, $sp, 0x70
    /* B82C 8000AC2C 24060010 */  addiu      $a2, $zero, 0x10
    /* B830 8000AC30 A7AD0064 */  sh         $t5, 0x64($sp)
    /* B834 8000AC34 0C0034AC */  jal        func_8000D2B0
    /* B838 8000AC38 AFAE0060 */   sw        $t6, 0x60($sp)
    /* B83C 8000AC3C 8FA402A8 */  lw         $a0, 0x2A8($sp)
    /* B840 8000AC40 27A5006C */  addiu      $a1, $sp, 0x6C
    /* B844 8000AC44 0C0034AC */  jal        func_8000D2B0
    /* B848 8000AC48 24060004 */   addiu     $a2, $zero, 0x4
    /* B84C 8000AC4C 8FAF02B0 */  lw         $t7, 0x2B0($sp)
    /* B850 8000AC50 8E79005C */  lw         $t9, 0x5C($s3)
    /* B854 8000AC54 8E640004 */  lw         $a0, 0x4($s3)
    /* B858 8000AC58 8DF80000 */  lw         $t8, 0x0($t7)
    /* B85C 8000AC5C 8E650008 */  lw         $a1, 0x8($s3)
    /* B860 8000AC60 AFA00010 */  sw         $zero, 0x10($sp)
    /* B864 8000AC64 03193021 */  addu       $a2, $t8, $t9
    /* B868 8000AC68 30C8FFFF */  andi       $t0, $a2, 0xFFFF
    /* B86C 8000AC6C 01003025 */  or         $a2, $t0, $zero
    /* B870 8000AC70 0C003DA0 */  jal        func_8000F680
    /* B874 8000AC74 27A70060 */   addiu     $a3, $sp, 0x60
  .L8000AC78:
    /* B878 8000AC78 8FBF0044 */  lw         $ra, 0x44($sp)
  .L8000AC7C:
    /* B87C 8000AC7C 8FB00024 */  lw         $s0, 0x24($sp)
    /* B880 8000AC80 8FB10028 */  lw         $s1, 0x28($sp)
    /* B884 8000AC84 8FB2002C */  lw         $s2, 0x2C($sp)
    /* B888 8000AC88 8FB30030 */  lw         $s3, 0x30($sp)
    /* B88C 8000AC8C 8FB40034 */  lw         $s4, 0x34($sp)
    /* B890 8000AC90 8FB50038 */  lw         $s5, 0x38($sp)
    /* B894 8000AC94 8FB6003C */  lw         $s6, 0x3C($sp)
    /* B898 8000AC98 8FB70040 */  lw         $s7, 0x40($sp)
    /* B89C 8000AC9C 03E00008 */  jr         $ra
    /* B8A0 8000ACA0 27BD0298 */   addiu     $sp, $sp, 0x298
```

## Tips for This Category

