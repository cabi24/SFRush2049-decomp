# __osPfsCheckPages

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000B918` |
| **Category** | `libultra/pfs` |
| **Status** | `TODO` |
| **Instructions** | ~182 |

## Description

build inode usage bitmap for integrity check

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
./work/libultra/pfs/__osPfsCheckPages/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pfs/__osPfsCheckPages

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pfs/__osPfsCheckPages --watch
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
# Source: BFE0.s
# Address: 0x8000B918

glabel func_8000B918
    /* C518 8000B918 27BDFE98 */  addiu      $sp, $sp, -0x168
    /* C51C 8000B91C AFB30024 */  sw         $s3, 0x24($sp)
    /* C520 8000B920 00809825 */  or         $s3, $a0, $zero
    /* C524 8000B924 AFB40028 */  sw         $s4, 0x28($sp)
    /* C528 8000B928 00A0A025 */  or         $s4, $a1, $zero
    /* C52C 8000B92C AFBF003C */  sw         $ra, 0x3C($sp)
    /* C530 8000B930 AFBE0038 */  sw         $fp, 0x38($sp)
    /* C534 8000B934 AFB70034 */  sw         $s7, 0x34($sp)
    /* C538 8000B938 AFB60030 */  sw         $s6, 0x30($sp)
    /* C53C 8000B93C AFB5002C */  sw         $s5, 0x2C($sp)
    /* C540 8000B940 AFB20020 */  sw         $s2, 0x20($sp)
    /* C544 8000B944 AFB1001C */  sw         $s1, 0x1C($sp)
    /* C548 8000B948 AFB00018 */  sw         $s0, 0x18($sp)
    /* C54C 8000B94C 24040100 */  addiu      $a0, $zero, 0x100
    /* C550 8000B950 00001025 */  or         $v0, $zero, $zero
    /* C554 8000B954 00A01825 */  or         $v1, $a1, $zero
  .L8000B958:
    /* C558 8000B958 24420004 */  addiu      $v0, $v0, 0x4
    /* C55C 8000B95C A0600102 */  sb         $zero, 0x102($v1)
    /* C560 8000B960 A0600103 */  sb         $zero, 0x103($v1)
    /* C564 8000B964 A0600104 */  sb         $zero, 0x104($v1)
    /* C568 8000B968 24630004 */  addiu      $v1, $v1, 0x4
    /* C56C 8000B96C 1444FFFA */  bne        $v0, $a0, .L8000B958
    /* C570 8000B970 A06000FD */   sb        $zero, 0xFD($v1)
    /* C574 8000B974 240E00FF */  addiu      $t6, $zero, 0xFF
    /* C578 8000B978 A28E0100 */  sb         $t6, 0x100($s4)
    /* C57C 8000B97C 926F0064 */  lbu        $t7, 0x64($s3)
    /* C580 8000B980 0000B825 */  or         $s7, $zero, $zero
    /* C584 8000B984 00008025 */  or         $s0, $zero, $zero
    /* C588 8000B988 19E0008C */  blez       $t7, .L8000BBBC
    /* C58C 8000B98C 241E0080 */   addiu     $fp, $zero, 0x80
    /* C590 8000B990 27B6015A */  addiu      $s6, $sp, 0x15A
    /* C594 8000B994 27B50058 */  addiu      $s5, $sp, 0x58
    /* C598 8000B998 27B20158 */  addiu      $s2, $sp, 0x158
  .L8000B99C:
    /* C59C 8000B99C 1A000003 */  blez       $s0, .L8000B9AC
    /* C5A0 8000B9A0 02602025 */   or        $a0, $s3, $zero
    /* C5A4 8000B9A4 10000002 */  b          .L8000B9B0
    /* C5A8 8000B9A8 24110001 */   addiu     $s1, $zero, 0x1
  .L8000B9AC:
    /* C5AC 8000B9AC 8E710060 */  lw         $s1, 0x60($s3)
  .L8000B9B0:
    /* C5B0 8000B9B0 02A02825 */  or         $a1, $s5, $zero
    /* C5B4 8000B9B4 00003025 */  or         $a2, $zero, $zero
    /* C5B8 8000B9B8 0C003CE9 */  jal        func_8000F3A4
    /* C5BC 8000B9BC 32E700FF */   andi      $a3, $s7, 0xFF
    /* C5C0 8000B9C0 10400005 */  beqz       $v0, .L8000B9D8
    /* C5C4 8000B9C4 24010003 */   addiu     $at, $zero, 0x3
    /* C5C8 8000B9C8 50410004 */  beql       $v0, $at, .L8000B9DC
    /* C5CC 8000B9CC 2A210080 */   slti      $at, $s1, 0x80
    /* C5D0 8000B9D0 1000007C */  b          .L8000BBC4
    /* C5D4 8000B9D4 8FBF003C */   lw        $ra, 0x3C($sp)
  .L8000B9D8:
    /* C5D8 8000B9D8 2A210080 */  slti       $at, $s1, 0x80
  .L8000B9DC:
    /* C5DC 8000B9DC 10200071 */  beqz       $at, .L8000BBA4
    /* C5E0 8000B9E0 02201025 */   or        $v0, $s1, $zero
    /* C5E4 8000B9E4 03D11823 */  subu       $v1, $fp, $s1
    /* C5E8 8000B9E8 30780001 */  andi       $t8, $v1, 0x1
    /* C5EC 8000B9EC 13000025 */  beqz       $t8, .L8000BA84
    /* C5F0 8000B9F0 0011C840 */   sll       $t9, $s1, 1
    /* C5F4 8000B9F4 02B94021 */  addu       $t0, $s5, $t9
    /* C5F8 8000B9F8 95010000 */  lhu        $at, 0x0($t0)
    /* C5FC 8000B9FC A6410000 */  sh         $at, 0x0($s2)
    /* C600 8000BA00 8E6C0060 */  lw         $t4, 0x60($s3)
    /* C604 8000BA04 97AB0158 */  lhu        $t3, 0x158($sp)
    /* C608 8000BA08 93AD0158 */  lbu        $t5, 0x158($sp)
    /* C60C 8000BA0C 016C082A */  slt        $at, $t3, $t4
    /* C610 8000BA10 5420001B */  bnel       $at, $zero, .L8000BA80
    /* C614 8000BA14 26220001 */   addiu     $v0, $s1, 0x1
    /* C618 8000BA18 120D0018 */  beq        $s0, $t5, .L8000BA7C
    /* C61C 8000BA1C 93AE0159 */   lbu       $t6, 0x159($sp)
    /* C620 8000BA20 31CF007F */  andi       $t7, $t6, 0x7F
    /* C624 8000BA24 05E10003 */  bgez       $t7, .L8000BA34
    /* C628 8000BA28 000FC083 */   sra       $t8, $t7, 2
    /* C62C 8000BA2C 25E10003 */  addiu      $at, $t7, 0x3
    /* C630 8000BA30 0001C083 */  sra        $t8, $at, 2
  .L8000BA34:
    /* C634 8000BA34 05A10004 */  bgez       $t5, .L8000BA48
    /* C638 8000BA38 31B90007 */   andi      $t9, $t5, 0x7
    /* C63C 8000BA3C 13200002 */  beqz       $t9, .L8000BA48
    /* C640 8000BA40 00000000 */   nop
    /* C644 8000BA44 2739FFF8 */  addiu      $t9, $t9, -0x8
  .L8000BA48:
    /* C648 8000BA48 00194940 */  sll        $t1, $t9, 5
    /* C64C 8000BA4C 03091821 */  addu       $v1, $t8, $t1
    /* C650 8000BA50 02831021 */  addu       $v0, $s4, $v1
    /* C654 8000BA54 90480101 */  lbu        $t0, 0x101($v0)
    /* C658 8000BA58 06010004 */  bgez       $s0, .L8000BA6C
    /* C65C 8000BA5C 320A0007 */   andi      $t2, $s0, 0x7
    /* C660 8000BA60 11400002 */  beqz       $t2, .L8000BA6C
    /* C664 8000BA64 00000000 */   nop
    /* C668 8000BA68 254AFFF8 */  addiu      $t2, $t2, -0x8
  .L8000BA6C:
    /* C66C 8000BA6C 240B0001 */  addiu      $t3, $zero, 0x1
    /* C670 8000BA70 014B6004 */  sllv       $t4, $t3, $t2
    /* C674 8000BA74 010C7025 */  or         $t6, $t0, $t4
    /* C678 8000BA78 A04E0101 */  sb         $t6, 0x101($v0)
  .L8000BA7C:
    /* C67C 8000BA7C 26220001 */  addiu      $v0, $s1, 0x1
  .L8000BA80:
    /* C680 8000BA80 105E0048 */  beq        $v0, $fp, .L8000BBA4
  .L8000BA84:
    /* C684 8000BA84 00027840 */   sll       $t7, $v0, 1
    /* C688 8000BA88 02AF2821 */  addu       $a1, $s5, $t7
    /* C68C 8000BA8C 24A40002 */  addiu      $a0, $a1, 0x2
  .L8000BA90:
    /* C690 8000BA90 94A10000 */  lhu        $at, 0x0($a1)
    /* C694 8000BA94 A6410000 */  sh         $at, 0x0($s2)
    /* C698 8000BA98 8E690060 */  lw         $t1, 0x60($s3)
    /* C69C 8000BA9C 97B80158 */  lhu        $t8, 0x158($sp)
    /* C6A0 8000BAA0 93AB0158 */  lbu        $t3, 0x158($sp)
    /* C6A4 8000BAA4 0309082A */  slt        $at, $t8, $t1
    /* C6A8 8000BAA8 5420001B */  bnel       $at, $zero, .L8000BB18
    /* C6AC 8000BAAC 94810000 */   lhu       $at, 0x0($a0)
    /* C6B0 8000BAB0 120B0018 */  beq        $s0, $t3, .L8000BB14
    /* C6B4 8000BAB4 93AA0159 */   lbu       $t2, 0x159($sp)
    /* C6B8 8000BAB8 3148007F */  andi       $t0, $t2, 0x7F
    /* C6BC 8000BABC 05010003 */  bgez       $t0, .L8000BACC
    /* C6C0 8000BAC0 00086083 */   sra       $t4, $t0, 2
    /* C6C4 8000BAC4 25010003 */  addiu      $at, $t0, 0x3
    /* C6C8 8000BAC8 00016083 */  sra        $t4, $at, 2
  .L8000BACC:
    /* C6CC 8000BACC 05610004 */  bgez       $t3, .L8000BAE0
    /* C6D0 8000BAD0 316E0007 */   andi      $t6, $t3, 0x7
    /* C6D4 8000BAD4 11C00002 */  beqz       $t6, .L8000BAE0
    /* C6D8 8000BAD8 00000000 */   nop
    /* C6DC 8000BADC 25CEFFF8 */  addiu      $t6, $t6, -0x8
  .L8000BAE0:
    /* C6E0 8000BAE0 000E7940 */  sll        $t7, $t6, 5
    /* C6E4 8000BAE4 018F1821 */  addu       $v1, $t4, $t7
    /* C6E8 8000BAE8 02831021 */  addu       $v0, $s4, $v1
    /* C6EC 8000BAEC 904D0101 */  lbu        $t5, 0x101($v0)
    /* C6F0 8000BAF0 06010004 */  bgez       $s0, .L8000BB04
    /* C6F4 8000BAF4 32190007 */   andi      $t9, $s0, 0x7
    /* C6F8 8000BAF8 13200002 */  beqz       $t9, .L8000BB04
    /* C6FC 8000BAFC 00000000 */   nop
    /* C700 8000BB00 2739FFF8 */  addiu      $t9, $t9, -0x8
  .L8000BB04:
    /* C704 8000BB04 24180001 */  addiu      $t8, $zero, 0x1
    /* C708 8000BB08 03384804 */  sllv       $t1, $t8, $t9
    /* C70C 8000BB0C 01A95025 */  or         $t2, $t5, $t1
    /* C710 8000BB10 A04A0101 */  sb         $t2, 0x101($v0)
  .L8000BB14:
    /* C714 8000BB14 94810000 */  lhu        $at, 0x0($a0)
  .L8000BB18:
    /* C718 8000BB18 24840004 */  addiu      $a0, $a0, 0x4
    /* C71C 8000BB1C A6410000 */  sh         $at, 0x0($s2)
    /* C720 8000BB20 8E6C0060 */  lw         $t4, 0x60($s3)
    /* C724 8000BB24 97AE0158 */  lhu        $t6, 0x158($sp)
    /* C728 8000BB28 93AF0158 */  lbu        $t7, 0x158($sp)
    /* C72C 8000BB2C 01CC082A */  slt        $at, $t6, $t4
    /* C730 8000BB30 1420001A */  bnez       $at, .L8000BB9C
    /* C734 8000BB34 00000000 */   nop
    /* C738 8000BB38 120F0018 */  beq        $s0, $t7, .L8000BB9C
    /* C73C 8000BB3C 93B80159 */   lbu       $t8, 0x159($sp)
    /* C740 8000BB40 3319007F */  andi       $t9, $t8, 0x7F
    /* C744 8000BB44 07210003 */  bgez       $t9, .L8000BB54
    /* C748 8000BB48 00196883 */   sra       $t5, $t9, 2
    /* C74C 8000BB4C 27210003 */  addiu      $at, $t9, 0x3
    /* C750 8000BB50 00016883 */  sra        $t5, $at, 2
  .L8000BB54:
    /* C754 8000BB54 05E10004 */  bgez       $t7, .L8000BB68
    /* C758 8000BB58 31E90007 */   andi      $t1, $t7, 0x7
    /* C75C 8000BB5C 11200002 */  beqz       $t1, .L8000BB68
    /* C760 8000BB60 00000000 */   nop
    /* C764 8000BB64 2529FFF8 */  addiu      $t1, $t1, -0x8
  .L8000BB68:
    /* C768 8000BB68 00095140 */  sll        $t2, $t1, 5
    /* C76C 8000BB6C 01AA1821 */  addu       $v1, $t5, $t2
    /* C770 8000BB70 02831021 */  addu       $v0, $s4, $v1
    /* C774 8000BB74 90480101 */  lbu        $t0, 0x101($v0)
    /* C778 8000BB78 06010004 */  bgez       $s0, .L8000BB8C
    /* C77C 8000BB7C 320B0007 */   andi      $t3, $s0, 0x7
    /* C780 8000BB80 11600002 */  beqz       $t3, .L8000BB8C
    /* C784 8000BB84 00000000 */   nop
    /* C788 8000BB88 256BFFF8 */  addiu      $t3, $t3, -0x8
  .L8000BB8C:
    /* C78C 8000BB8C 240E0001 */  addiu      $t6, $zero, 0x1
    /* C790 8000BB90 016E6004 */  sllv       $t4, $t6, $t3
    /* C794 8000BB94 010CC025 */  or         $t8, $t0, $t4
    /* C798 8000BB98 A0580101 */  sb         $t8, 0x101($v0)
  .L8000BB9C:
    /* C79C 8000BB9C 1496FFBC */  bne        $a0, $s6, .L8000BA90
    /* C7A0 8000BBA0 24A50004 */   addiu     $a1, $a1, 0x4
  .L8000BBA4:
    /* C7A4 8000BBA4 926F0064 */  lbu        $t7, 0x64($s3)
    /* C7A8 8000BBA8 26F70001 */  addiu      $s7, $s7, 0x1
    /* C7AC 8000BBAC 32F000FF */  andi       $s0, $s7, 0xFF
    /* C7B0 8000BBB0 020F082A */  slt        $at, $s0, $t7
    /* C7B4 8000BBB4 1420FF79 */  bnez       $at, .L8000B99C
    /* C7B8 8000BBB8 0200B825 */   or        $s7, $s0, $zero
  .L8000BBBC:
    /* C7BC 8000BBBC 00001025 */  or         $v0, $zero, $zero
    /* C7C0 8000BBC0 8FBF003C */  lw         $ra, 0x3C($sp)
  .L8000BBC4:
    /* C7C4 8000BBC4 8FB00018 */  lw         $s0, 0x18($sp)
    /* C7C8 8000BBC8 8FB1001C */  lw         $s1, 0x1C($sp)
    /* C7CC 8000BBCC 8FB20020 */  lw         $s2, 0x20($sp)
    /* C7D0 8000BBD0 8FB30024 */  lw         $s3, 0x24($sp)
    /* C7D4 8000BBD4 8FB40028 */  lw         $s4, 0x28($sp)
    /* C7D8 8000BBD8 8FB5002C */  lw         $s5, 0x2C($sp)
    /* C7DC 8000BBDC 8FB60030 */  lw         $s6, 0x30($sp)
    /* C7E0 8000BBE0 8FB70034 */  lw         $s7, 0x34($sp)
    /* C7E4 8000BBE4 8FBE0038 */  lw         $fp, 0x38($sp)
    /* C7E8 8000BBE8 03E00008 */  jr         $ra
    /* C7EC 8000BBEC 27BD0168 */   addiu     $sp, $sp, 0x168
```

## Tips for This Category

