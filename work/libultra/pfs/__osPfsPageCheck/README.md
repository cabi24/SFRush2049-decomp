# __osPfsPageCheck

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000BBF0` |
| **Category** | `libultra/pfs` |
| **Status** | `TODO` |
| **Instructions** | ~103 |

## Description

validate inode references, detect duplicates

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
./work/libultra/pfs/__osPfsPageCheck/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pfs/__osPfsPageCheck

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pfs/__osPfsPageCheck --watch
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
# Address: 0x8000BBF0

glabel func_8000BBF0
    /* C7F0 8000BBF0 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* C7F4 8000BBF4 AFA5003C */  sw         $a1, 0x3C($sp)
    /* C7F8 8000BBF8 93B8003C */  lbu        $t8, 0x3C($sp)
    /* C7FC 8000BBFC AFB40028 */  sw         $s4, 0x28($sp)
    /* C800 8000BC00 93AE003D */  lbu        $t6, 0x3D($sp)
    /* C804 8000BC04 0080A025 */  or         $s4, $a0, $zero
    /* C808 8000BC08 AFBF0034 */  sw         $ra, 0x34($sp)
    /* C80C 8000BC0C AFB60030 */  sw         $s6, 0x30($sp)
    /* C810 8000BC10 AFB5002C */  sw         $s5, 0x2C($sp)
    /* C814 8000BC14 AFB30024 */  sw         $s3, 0x24($sp)
    /* C818 8000BC18 AFB20020 */  sw         $s2, 0x20($sp)
    /* C81C 8000BC1C AFB1001C */  sw         $s1, 0x1C($sp)
    /* C820 8000BC20 AFB00018 */  sw         $s0, 0x18($sp)
    /* C824 8000BC24 92890064 */  lbu        $t1, 0x64($s4)
    /* C828 8000BC28 00C09825 */  or         $s3, $a2, $zero
    /* C82C 8000BC2C 00008025 */  or         $s0, $zero, $zero
    /* C830 8000BC30 05C10003 */  bgez       $t6, .L8000BC40
    /* C834 8000BC34 000E7883 */   sra       $t7, $t6, 2
    /* C838 8000BC38 25C10003 */  addiu      $at, $t6, 0x3
    /* C83C 8000BC3C 00017883 */  sra        $t7, $at, 2
  .L8000BC40:
    /* C840 8000BC40 07010004 */  bgez       $t8, .L8000BC54
    /* C844 8000BC44 33190007 */   andi      $t9, $t8, 0x7
    /* C848 8000BC48 13200002 */  beqz       $t9, .L8000BC54
    /* C84C 8000BC4C 00000000 */   nop
    /* C850 8000BC50 2739FFF8 */  addiu      $t9, $t9, -0x8
  .L8000BC54:
    /* C854 8000BC54 00194140 */  sll        $t0, $t9, 5
    /* C858 8000BC58 01E8B021 */  addu       $s6, $t7, $t0
    /* C85C 8000BC5C 19200040 */  blez       $t1, .L8000BD60
    /* C860 8000BC60 00009025 */   or        $s2, $zero, $zero
    /* C864 8000BC64 00001025 */  or         $v0, $zero, $zero
    /* C868 8000BC68 24150003 */  addiu      $s5, $zero, 0x3
  .L8000BC6C:
    /* C86C 8000BC6C 18400003 */  blez       $v0, .L8000BC7C
    /* C870 8000BC70 93AA003C */   lbu       $t2, 0x3C($sp)
    /* C874 8000BC74 10000002 */  b          .L8000BC80
    /* C878 8000BC78 24110001 */   addiu     $s1, $zero, 0x1
  .L8000BC7C:
    /* C87C 8000BC7C 8E910060 */  lw         $s1, 0x60($s4)
  .L8000BC80:
    /* C880 8000BC80 1142000C */  beq        $t2, $v0, .L8000BCB4
    /* C884 8000BC84 02765821 */   addu      $t3, $s3, $s6
    /* C888 8000BC88 916C0101 */  lbu        $t4, 0x101($t3)
    /* C88C 8000BC8C 04410004 */  bgez       $v0, .L8000BCA0
    /* C890 8000BC90 304D0007 */   andi      $t5, $v0, 0x7
    /* C894 8000BC94 11A00002 */  beqz       $t5, .L8000BCA0
    /* C898 8000BC98 00000000 */   nop
    /* C89C 8000BC9C 25ADFFF8 */  addiu      $t5, $t5, -0x8
  .L8000BCA0:
    /* C8A0 8000BCA0 240E0001 */  addiu      $t6, $zero, 0x1
    /* C8A4 8000BCA4 01AEC004 */  sllv       $t8, $t6, $t5
    /* C8A8 8000BCA8 0198C824 */  and        $t9, $t4, $t8
    /* C8AC 8000BCAC 53200027 */  beql       $t9, $zero, .L8000BD4C
    /* C8B0 8000BCB0 928B0064 */   lbu       $t3, 0x64($s4)
  .L8000BCB4:
    /* C8B4 8000BCB4 926F0100 */  lbu        $t7, 0x100($s3)
    /* C8B8 8000BCB8 02802025 */  or         $a0, $s4, $zero
    /* C8BC 8000BCBC 02602825 */  or         $a1, $s3, $zero
    /* C8C0 8000BCC0 11E2000A */  beq        $t7, $v0, .L8000BCEC
    /* C8C4 8000BCC4 00003025 */   or        $a2, $zero, $zero
    /* C8C8 8000BCC8 0C003CE9 */  jal        func_8000F3A4
    /* C8CC 8000BCCC 324700FF */   andi      $a3, $s2, 0xFF
    /* C8D0 8000BCD0 50400006 */  beql       $v0, $zero, .L8000BCEC
    /* C8D4 8000BCD4 A2720100 */   sb        $s2, 0x100($s3)
    /* C8D8 8000BCD8 50550004 */  beql       $v0, $s5, .L8000BCEC
    /* C8DC 8000BCDC A2720100 */   sb        $s2, 0x100($s3)
    /* C8E0 8000BCE0 10000021 */  b          .L8000BD68
    /* C8E4 8000BCE4 8FBF0034 */   lw        $ra, 0x34($sp)
    /* C8E8 8000BCE8 A2720100 */  sb         $s2, 0x100($s3)
  .L8000BCEC:
    /* C8EC 8000BCEC 2A010002 */  slti       $at, $s0, 0x2
    /* C8F0 8000BCF0 10200010 */  beqz       $at, .L8000BD34
    /* C8F4 8000BCF4 2A210080 */   slti      $at, $s1, 0x80
    /* C8F8 8000BCF8 1020000E */  beqz       $at, .L8000BD34
    /* C8FC 8000BCFC 00111040 */   sll       $v0, $s1, 1
    /* C900 8000BD00 02621821 */  addu       $v1, $s3, $v0
    /* C904 8000BD04 97A8003C */  lhu        $t0, 0x3C($sp)
  .L8000BD08:
    /* C908 8000BD08 94690000 */  lhu        $t1, 0x0($v1)
    /* C90C 8000BD0C 24420002 */  addiu      $v0, $v0, 0x2
    /* C910 8000BD10 55090003 */  bnel       $t0, $t1, .L8000BD20
    /* C914 8000BD14 2A010002 */   slti      $at, $s0, 0x2
    /* C918 8000BD18 26100001 */  addiu      $s0, $s0, 0x1
    /* C91C 8000BD1C 2A010002 */  slti       $at, $s0, 0x2
  .L8000BD20:
    /* C920 8000BD20 10200004 */  beqz       $at, .L8000BD34
    /* C924 8000BD24 24630002 */   addiu     $v1, $v1, 0x2
    /* C928 8000BD28 28410100 */  slti       $at, $v0, 0x100
    /* C92C 8000BD2C 5420FFF6 */  bnel       $at, $zero, .L8000BD08
    /* C930 8000BD30 97A8003C */   lhu       $t0, 0x3C($sp)
  .L8000BD34:
    /* C934 8000BD34 2A010002 */  slti       $at, $s0, 0x2
    /* C938 8000BD38 54200004 */  bnel       $at, $zero, .L8000BD4C
    /* C93C 8000BD3C 928B0064 */   lbu       $t3, 0x64($s4)
    /* C940 8000BD40 10000008 */  b          .L8000BD64
    /* C944 8000BD44 24020002 */   addiu     $v0, $zero, 0x2
    /* C948 8000BD48 928B0064 */  lbu        $t3, 0x64($s4)
  .L8000BD4C:
    /* C94C 8000BD4C 26520001 */  addiu      $s2, $s2, 0x1
    /* C950 8000BD50 324200FF */  andi       $v0, $s2, 0xFF
    /* C954 8000BD54 004B082A */  slt        $at, $v0, $t3
    /* C958 8000BD58 1420FFC4 */  bnez       $at, .L8000BC6C
    /* C95C 8000BD5C 00409025 */   or        $s2, $v0, $zero
  .L8000BD60:
    /* C960 8000BD60 02001025 */  or         $v0, $s0, $zero
  .L8000BD64:
    /* C964 8000BD64 8FBF0034 */  lw         $ra, 0x34($sp)
  .L8000BD68:
    /* C968 8000BD68 8FB00018 */  lw         $s0, 0x18($sp)
    /* C96C 8000BD6C 8FB1001C */  lw         $s1, 0x1C($sp)
    /* C970 8000BD70 8FB20020 */  lw         $s2, 0x20($sp)
    /* C974 8000BD74 8FB30024 */  lw         $s3, 0x24($sp)
    /* C978 8000BD78 8FB40028 */  lw         $s4, 0x28($sp)
    /* C97C 8000BD7C 8FB5002C */  lw         $s5, 0x2C($sp)
    /* C980 8000BD80 8FB60030 */  lw         $s6, 0x30($sp)
    /* C984 8000BD84 03E00008 */  jr         $ra
    /* C988 8000BD88 27BD0038 */   addiu     $sp, $sp, 0x38
```

## Tips for This Category

