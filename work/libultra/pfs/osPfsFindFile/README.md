# osPfsFindFile

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000A8D8` |
| **Category** | `libultra/pfs` |
| **Status** | `TODO` |
| **Instructions** | ~36 |

## Description

find file in controller pak

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
./work/libultra/pfs/osPfsFindFile/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pfs/osPfsFindFile

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pfs/osPfsFindFile --watch
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
# Source: B300.s
# Address: 0x8000A8D8

glabel func_8000A8D8
    /* B4D8 8000A8D8 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* B4DC 8000A8DC 30E200FF */  andi       $v0, $a3, 0xFF
    /* B4E0 8000A8E0 30CE00FF */  andi       $t6, $a2, 0xFF
    /* B4E4 8000A8E4 0002C200 */  sll        $t8, $v0, 8
    /* B4E8 8000A8E8 AFA60018 */  sw         $a2, 0x18($sp)
    /* B4EC 8000A8EC AFA7001C */  sw         $a3, 0x1C($sp)
    /* B4F0 8000A8F0 030EC821 */  addu       $t9, $t8, $t6
    /* B4F4 8000A8F4 A7B9000C */  sh         $t9, 0xC($sp)
    /* B4F8 8000A8F8 24070003 */  addiu      $a3, $zero, 0x3
    /* B4FC 8000A8FC 27A6000C */  addiu      $a2, $sp, 0xC
    /* B500 8000A900 27A30008 */  addiu      $v1, $sp, 0x8
    /* B504 8000A904 94C10000 */  lhu        $at, 0x0($a2)
  .L8000A908:
    /* B508 8000A908 A4610000 */  sh         $at, 0x0($v1)
    /* B50C 8000A90C 93AB000D */  lbu        $t3, 0xD($sp)
    /* B510 8000A910 000B6040 */  sll        $t4, $t3, 1
    /* B514 8000A914 00AC6821 */  addu       $t5, $a1, $t4
    /* B518 8000A918 95A10000 */  lhu        $at, 0x0($t5)
    /* B51C 8000A91C A4C10000 */  sh         $at, 0x0($a2)
    /* B520 8000A920 93B90009 */  lbu        $t9, 0x9($sp)
    /* B524 8000A924 00194840 */  sll        $t1, $t9, 1
    /* B528 8000A928 00A94021 */  addu       $t0, $a1, $t1
    /* B52C 8000A92C A5070000 */  sh         $a3, 0x0($t0)
    /* B530 8000A930 8C8B0060 */  lw         $t3, 0x60($a0)
    /* B534 8000A934 97AA000C */  lhu        $t2, 0xC($sp)
    /* B538 8000A938 93AC000C */  lbu        $t4, 0xC($sp)
    /* B53C 8000A93C 014B082A */  slt        $at, $t2, $t3
    /* B540 8000A940 54200004 */  bnel       $at, $zero, .L8000A954
    /* B544 8000A944 8FAF0020 */   lw        $t7, 0x20($sp)
    /* B548 8000A948 504CFFEF */  beql       $v0, $t4, .L8000A908
    /* B54C 8000A94C 94C10000 */   lhu       $at, 0x0($a2)
    /* B550 8000A950 8FAF0020 */  lw         $t7, 0x20($sp)
  .L8000A954:
    /* B554 8000A954 94C10000 */  lhu        $at, 0x0($a2)
    /* B558 8000A958 27BD0010 */  addiu      $sp, $sp, 0x10
    /* B55C 8000A95C 00001025 */  or         $v0, $zero, $zero
    /* B560 8000A960 03E00008 */  jr         $ra
    /* B564 8000A964 A5E10000 */   sh        $at, 0x0($t7)
```

## Tips for This Category

