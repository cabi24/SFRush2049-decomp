# __scTaskReady

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80000BA4` |
| **Category** | `scheduler` |
| **Status** | `TODO` |
| **Instructions** | ~44 |

## Description

validate task timing before scheduling

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
./work/scheduler/__scTaskReady/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/scheduler/__scTaskReady

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/scheduler/__scTaskReady --watch
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
# Address: 0x80000BA4

glabel func_80000BA4
    /* 17A4 80000BA4 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 17A8 80000BA8 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 17AC 80000BAC AFA40030 */  sw         $a0, 0x30($sp)
    /* 17B0 80000BB0 AFA50034 */  sw         $a1, 0x34($sp)
    /* 17B4 80000BB4 AFB00018 */  sw         $s0, 0x18($sp)
    /* 17B8 80000BB8 8FAE0034 */  lw         $t6, 0x34($sp)
    /* 17BC 80000BBC 11C0001C */  beqz       $t6, .L80000C30
    /* 17C0 80000BC0 00000000 */   nop
    /* 17C4 80000BC4 0C001DE4 */  jal        func_80007790
    /* 17C8 80000BC8 00000000 */   nop
    /* 17CC 80000BCC 00408025 */  or         $s0, $v0, $zero
    /* 17D0 80000BD0 0C001DF4 */  jal        func_800077D0
    /* 17D4 80000BD4 AFB0002C */   sw        $s0, 0x2C($sp)
    /* 17D8 80000BD8 AFA20028 */  sw         $v0, 0x28($sp)
    /* 17DC 80000BDC 8FAF0028 */  lw         $t7, 0x28($sp)
    /* 17E0 80000BE0 11F00003 */  beq        $t7, $s0, .L80000BF0
    /* 17E4 80000BE4 00000000 */   nop
    /* 17E8 80000BE8 10000015 */  b          .L80000C40
    /* 17EC 80000BEC 00001025 */   or        $v0, $zero, $zero
  .L80000BF0:
    /* 17F0 80000BF0 3C188003 */  lui        $t8, %hi(D_8002AFA4)
    /* 17F4 80000BF4 8F18AFA4 */  lw         $t8, %lo(D_8002AFA4)($t8)
    /* 17F8 80000BF8 1300000B */  beqz       $t8, .L80000C28
    /* 17FC 80000BFC 00000000 */   nop
    /* 1800 80000C00 8FB90030 */  lw         $t9, 0x30($sp)
    /* 1804 80000C04 3C098003 */  lui        $t1, %hi(D_8002AFA8)
    /* 1808 80000C08 8D29AFA8 */  lw         $t1, %lo(D_8002AFA8)($t1)
    /* 180C 80000C0C 8F28027C */  lw         $t0, 0x27C($t9)
    /* 1810 80000C10 01095023 */  subu       $t2, $t0, $t1
    /* 1814 80000C14 2D410002 */  sltiu      $at, $t2, 0x2
    /* 1818 80000C18 10200003 */  beqz       $at, .L80000C28
    /* 181C 80000C1C 00000000 */   nop
    /* 1820 80000C20 10000007 */  b          .L80000C40
    /* 1824 80000C24 00001025 */   or        $v0, $zero, $zero
  .L80000C28:
    /* 1828 80000C28 10000005 */  b          .L80000C40
    /* 182C 80000C2C 8FA20034 */   lw        $v0, 0x34($sp)
  .L80000C30:
    /* 1830 80000C30 10000003 */  b          .L80000C40
    /* 1834 80000C34 00001025 */   or        $v0, $zero, $zero
    /* 1838 80000C38 10000001 */  b          .L80000C40
    /* 183C 80000C3C 00000000 */   nop
  .L80000C40:
    /* 1840 80000C40 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 1844 80000C44 8FB00018 */  lw         $s0, 0x18($sp)
    /* 1848 80000C48 27BD0030 */  addiu      $sp, $sp, 0x30
    /* 184C 80000C4C 03E00008 */  jr         $ra
    /* 1850 80000C50 00000000 */   nop
```

## Tips for This Category

