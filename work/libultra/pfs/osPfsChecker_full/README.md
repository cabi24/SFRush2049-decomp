# osPfsChecker_full

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000BD90` |
| **Category** | `libultra/pfs` |
| **Status** | `TODO` |
| **Instructions** | ~48 |

## Description

validate controller pak filesystem

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
./work/libultra/pfs/osPfsChecker_full/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pfs/osPfsChecker_full

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pfs/osPfsChecker_full --watch
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
# Source: C990.s
# Address: 0x8000BD90

glabel func_8000BD90
    /* C990 8000BD90 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* C994 8000BD94 AFBF0024 */  sw         $ra, 0x24($sp)
    /* C998 8000BD98 AFA40038 */  sw         $a0, 0x38($sp)
    /* C99C 8000BD9C AFB20020 */  sw         $s2, 0x20($sp)
    /* C9A0 8000BDA0 AFB1001C */  sw         $s1, 0x1C($sp)
    /* C9A4 8000BDA4 0C00312C */  jal        func_8000C4B0
    /* C9A8 8000BDA8 AFB00018 */   sw        $s0, 0x18($sp)
    /* C9AC 8000BDAC 8FAE0038 */  lw         $t6, 0x38($sp)
    /* C9B0 8000BDB0 00408025 */  or         $s0, $v0, $zero
    /* C9B4 8000BDB4 15C00003 */  bnez       $t6, .L8000BDC4
    /* C9B8 8000BDB8 00000000 */   nop
    /* C9BC 8000BDBC 10000003 */  b          .L8000BDCC
    /* C9C0 8000BDC0 24110004 */   addiu     $s1, $zero, 0x4
  .L8000BDC4:
    /* C9C4 8000BDC4 8FAF0038 */  lw         $t7, 0x38($sp)
    /* C9C8 8000BDC8 95F10010 */  lhu        $s1, 0x10($t7)
  .L8000BDCC:
    /* C9CC 8000BDCC 02209025 */  or         $s2, $s1, $zero
    /* C9D0 8000BDD0 24010002 */  addiu      $at, $zero, 0x2
    /* C9D4 8000BDD4 1241000F */  beq        $s2, $at, .L8000BE14
    /* C9D8 8000BDD8 24010004 */   addiu     $at, $zero, 0x4
    /* C9DC 8000BDDC 12410005 */  beq        $s2, $at, .L8000BDF4
    /* C9E0 8000BDE0 24010008 */   addiu     $at, $zero, 0x8
    /* C9E4 8000BDE4 1241000B */  beq        $s2, $at, .L8000BE14
    /* C9E8 8000BDE8 00000000 */   nop
    /* C9EC 8000BDEC 10000010 */  b          .L8000BE30
    /* C9F0 8000BDF0 00000000 */   nop
  .L8000BDF4:
    /* C9F4 8000BDF4 3C198003 */  lui        $t9, %hi(D_8002C3E0)
    /* C9F8 8000BDF8 8F39C3E0 */  lw         $t9, %lo(D_8002C3E0)($t9)
    /* C9FC 8000BDFC 24180001 */  addiu      $t8, $zero, 0x1
    /* CA00 8000BE00 00002025 */  or         $a0, $zero, $zero
    /* CA04 8000BE04 0C0033F1 */  jal        func_8000CFC4
    /* CA08 8000BE08 A7380010 */   sh        $t8, 0x10($t9)
    /* CA0C 8000BE0C 10000008 */  b          .L8000BE30
    /* CA10 8000BE10 00000000 */   nop
  .L8000BE14:
    /* CA14 8000BE14 8FA90038 */  lw         $t1, 0x38($sp)
    /* CA18 8000BE18 24080001 */  addiu      $t0, $zero, 0x1
    /* CA1C 8000BE1C A5280010 */  sh         $t0, 0x10($t1)
    /* CA20 8000BE20 8FAA0038 */  lw         $t2, 0x38($sp)
    /* CA24 8000BE24 8D440008 */  lw         $a0, 0x8($t2)
    /* CA28 8000BE28 0C003014 */  jal        func_8000C050
    /* CA2C 8000BE2C 01402825 */   or        $a1, $t2, $zero
  .L8000BE30:
    /* CA30 8000BE30 0C003148 */  jal        func_8000C520
    /* CA34 8000BE34 02002025 */   or        $a0, $s0, $zero
    /* CA38 8000BE38 8FBF0024 */  lw         $ra, 0x24($sp)
    /* CA3C 8000BE3C 8FB00018 */  lw         $s0, 0x18($sp)
    /* CA40 8000BE40 8FB1001C */  lw         $s1, 0x1C($sp)
    /* CA44 8000BE44 8FB20020 */  lw         $s2, 0x20($sp)
    /* CA48 8000BE48 03E00008 */  jr         $ra
    /* CA4C 8000BE4C 27BD0038 */   addiu     $sp, $sp, 0x38
```

## Tips for This Category

