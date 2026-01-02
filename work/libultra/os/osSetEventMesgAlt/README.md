# osSetEventMesgAlt

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80006E10` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~45 |

## Description

alternative event message setter

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
./work/libultra/os/osSetEventMesgAlt/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/osSetEventMesgAlt

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/osSetEventMesgAlt --watch
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
# Source: 7A10.s
# Address: 0x80006E10

glabel func_80006E10
    /* 7A10 80006E10 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* 7A14 80006E14 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 7A18 80006E18 AFA40028 */  sw         $a0, 0x28($sp)
    /* 7A1C 80006E1C AFA5002C */  sw         $a1, 0x2C($sp)
    /* 7A20 80006E20 AFA60030 */  sw         $a2, 0x30($sp)
    /* 7A24 80006E24 0C00312C */  jal        func_8000C4B0
    /* 7A28 80006E28 AFB00018 */   sw        $s0, 0x18($sp)
    /* 7A2C 80006E2C 8FAE0028 */  lw         $t6, 0x28($sp)
    /* 7A30 80006E30 3C188003 */  lui        $t8, %hi(D_80036710)
    /* 7A34 80006E34 8FA8002C */  lw         $t0, 0x2C($sp)
    /* 7A38 80006E38 27186710 */  addiu      $t8, $t8, %lo(D_80036710)
    /* 7A3C 80006E3C 000E78C0 */  sll        $t7, $t6, 3
    /* 7A40 80006E40 01F8C821 */  addu       $t9, $t7, $t8
    /* 7A44 80006E44 AFB90020 */  sw         $t9, 0x20($sp)
    /* 7A48 80006E48 AF280000 */  sw         $t0, 0x0($t9)
    /* 7A4C 80006E4C 8FAA0020 */  lw         $t2, 0x20($sp)
    /* 7A50 80006E50 8FA90030 */  lw         $t1, 0x30($sp)
    /* 7A54 80006E54 2401000E */  addiu      $at, $zero, 0xE
    /* 7A58 80006E58 00408025 */  or         $s0, $v0, $zero
    /* 7A5C 80006E5C AD490004 */  sw         $t1, 0x4($t2)
    /* 7A60 80006E60 8FAB0028 */  lw         $t3, 0x28($sp)
    /* 7A64 80006E64 15610010 */  bne        $t3, $at, .L80006EA8
    /* 7A68 80006E68 00000000 */   nop
    /* 7A6C 80006E6C 3C0C8003 */  lui        $t4, %hi(D_8002C36C)
    /* 7A70 80006E70 8D8CC36C */  lw         $t4, %lo(D_8002C36C)($t4)
    /* 7A74 80006E74 11800009 */  beqz       $t4, .L80006E9C
    /* 7A78 80006E78 00000000 */   nop
    /* 7A7C 80006E7C 3C0D8003 */  lui        $t5, %hi(D_8002C350)
    /* 7A80 80006E80 8DADC350 */  lw         $t5, %lo(D_8002C350)($t5)
    /* 7A84 80006E84 15A00005 */  bnez       $t5, .L80006E9C
    /* 7A88 80006E88 00000000 */   nop
    /* 7A8C 80006E8C 8FA4002C */  lw         $a0, 0x2C($sp)
    /* 7A90 80006E90 8FA50030 */  lw         $a1, 0x30($sp)
    /* 7A94 80006E94 0C001D78 */  jal        func_800075E0
    /* 7A98 80006E98 00003025 */   or        $a2, $zero, $zero
  .L80006E9C:
    /* 7A9C 80006E9C 240E0001 */  addiu      $t6, $zero, 0x1
    /* 7AA0 80006EA0 3C018003 */  lui        $at, %hi(D_8002C350)
    /* 7AA4 80006EA4 AC2EC350 */  sw         $t6, %lo(D_8002C350)($at)
  .L80006EA8:
    /* 7AA8 80006EA8 0C003148 */  jal        func_8000C520
    /* 7AAC 80006EAC 02002025 */   or        $a0, $s0, $zero
    /* 7AB0 80006EB0 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 7AB4 80006EB4 8FB00018 */  lw         $s0, 0x18($sp)
    /* 7AB8 80006EB8 27BD0028 */  addiu      $sp, $sp, 0x28
    /* 7ABC 80006EBC 03E00008 */  jr         $ra
    /* 7AC0 80006EC0 00000000 */   nop
```

## Tips for This Category

