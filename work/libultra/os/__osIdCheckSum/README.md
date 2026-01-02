# __osIdCheckSum

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000EB74` |
| **Category** | `libultra/os` |
| **Status** | `TODO` |
| **Instructions** | ~63 |

## Description

calculate ID checksum (Adler-like)

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
./work/libultra/os/__osIdCheckSum/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/os/__osIdCheckSum

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/os/__osIdCheckSum --watch
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
# Address: 0x8000EB74

glabel func_8000EB74
    /* F774 8000EB74 A4C00000 */  sh         $zero, 0x0($a2)
    /* F778 8000EB78 A4A00000 */  sh         $zero, 0x0($a1)
    /* F77C 8000EB7C 94820000 */  lhu        $v0, 0x0($a0)
    /* F780 8000EB80 24870004 */  addiu      $a3, $a0, 0x4
    /* F784 8000EB84 24E90002 */  addiu      $t1, $a3, 0x2
    /* F788 8000EB88 0002C021 */  addu       $t8, $zero, $v0
    /* F78C 8000EB8C A4B80000 */  sh         $t8, 0x0($a1)
    /* F790 8000EB90 94D90000 */  lhu        $t9, 0x0($a2)
    /* F794 8000EB94 00406827 */  not        $t5, $v0
    /* F798 8000EB98 24EA0004 */  addiu      $t2, $a3, 0x4
    /* F79C 8000EB9C 032D7021 */  addu       $t6, $t9, $t5
    /* F7A0 8000EBA0 A4CE0000 */  sh         $t6, 0x0($a2)
    /* F7A4 8000EBA4 94AF0000 */  lhu        $t7, 0x0($a1)
    /* F7A8 8000EBA8 94820002 */  lhu        $v0, 0x2($a0)
    /* F7AC 8000EBAC 2404001C */  addiu      $a0, $zero, 0x1C
    /* F7B0 8000EBB0 24EB0006 */  addiu      $t3, $a3, 0x6
    /* F7B4 8000EBB4 01E2C021 */  addu       $t8, $t7, $v0
    /* F7B8 8000EBB8 A4B80000 */  sh         $t8, 0x0($a1)
    /* F7BC 8000EBBC 94D90000 */  lhu        $t9, 0x0($a2)
    /* F7C0 8000EBC0 00406827 */  not        $t5, $v0
    /* F7C4 8000EBC4 24030004 */  addiu      $v1, $zero, 0x4
    /* F7C8 8000EBC8 032D7021 */  addu       $t6, $t9, $t5
    /* F7CC 8000EBCC A4CE0000 */  sh         $t6, 0x0($a2)
  .L8000EBD0:
    /* F7D0 8000EBD0 94E20000 */  lhu        $v0, 0x0($a3)
    /* F7D4 8000EBD4 94AF0000 */  lhu        $t7, 0x0($a1)
    /* F7D8 8000EBD8 24630008 */  addiu      $v1, $v1, 0x8
    /* F7DC 8000EBDC 00406827 */  not        $t5, $v0
    /* F7E0 8000EBE0 01E2C021 */  addu       $t8, $t7, $v0
    /* F7E4 8000EBE4 A4B80000 */  sh         $t8, 0x0($a1)
    /* F7E8 8000EBE8 94D90000 */  lhu        $t9, 0x0($a2)
    /* F7EC 8000EBEC 24E70008 */  addiu      $a3, $a3, 0x8
    /* F7F0 8000EBF0 25290008 */  addiu      $t1, $t1, 0x8
    /* F7F4 8000EBF4 032D7021 */  addu       $t6, $t9, $t5
    /* F7F8 8000EBF8 A4CE0000 */  sh         $t6, 0x0($a2)
    /* F7FC 8000EBFC 94AF0000 */  lhu        $t7, 0x0($a1)
    /* F800 8000EC00 9522FFF8 */  lhu        $v0, -0x8($t1)
    /* F804 8000EC04 254A0008 */  addiu      $t2, $t2, 0x8
    /* F808 8000EC08 256B0008 */  addiu      $t3, $t3, 0x8
    /* F80C 8000EC0C 01E2C021 */  addu       $t8, $t7, $v0
    /* F810 8000EC10 A4B80000 */  sh         $t8, 0x0($a1)
    /* F814 8000EC14 94D90000 */  lhu        $t9, 0x0($a2)
    /* F818 8000EC18 00406827 */  not        $t5, $v0
    /* F81C 8000EC1C 032D7021 */  addu       $t6, $t9, $t5
    /* F820 8000EC20 A4CE0000 */  sh         $t6, 0x0($a2)
    /* F824 8000EC24 94AF0000 */  lhu        $t7, 0x0($a1)
    /* F828 8000EC28 9542FFF8 */  lhu        $v0, -0x8($t2)
    /* F82C 8000EC2C 01E2C021 */  addu       $t8, $t7, $v0
    /* F830 8000EC30 A4B80000 */  sh         $t8, 0x0($a1)
    /* F834 8000EC34 94D90000 */  lhu        $t9, 0x0($a2)
    /* F838 8000EC38 00406827 */  not        $t5, $v0
    /* F83C 8000EC3C 032D7021 */  addu       $t6, $t9, $t5
    /* F840 8000EC40 A4CE0000 */  sh         $t6, 0x0($a2)
    /* F844 8000EC44 94AF0000 */  lhu        $t7, 0x0($a1)
    /* F848 8000EC48 9562FFF8 */  lhu        $v0, -0x8($t3)
    /* F84C 8000EC4C 01E2C021 */  addu       $t8, $t7, $v0
    /* F850 8000EC50 A4B80000 */  sh         $t8, 0x0($a1)
    /* F854 8000EC54 94D90000 */  lhu        $t9, 0x0($a2)
    /* F858 8000EC58 00406827 */  not        $t5, $v0
    /* F85C 8000EC5C 032D7021 */  addu       $t6, $t9, $t5
    /* F860 8000EC60 1464FFDB */  bne        $v1, $a0, .L8000EBD0
    /* F864 8000EC64 A4CE0000 */   sh        $t6, 0x0($a2)
    /* F868 8000EC68 03E00008 */  jr         $ra
    /* F86C 8000EC6C 00001025 */   or        $v0, $zero, $zero
```

## Tips for This Category

