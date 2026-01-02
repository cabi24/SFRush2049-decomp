# dll_init

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000C090` |
| **Category** | `dll` |
| **Status** | `TODO` |
| **Instructions** | ~35 |

## Description

initialize timer queue (doubly-linked list)

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `dll` category.

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
./work/dll/dll_init/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/dll/dll_init

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/dll/dll_init --watch
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
# Source: CC50.s
# Address: 0x8000C090

glabel func_8000C090
    /* CC90 8000C090 3C018003 */  lui        $at, %hi(D_80037C54)
    /* CC94 8000C094 240E0000 */  addiu      $t6, $zero, 0x0
    /* CC98 8000C098 240F0000 */  addiu      $t7, $zero, 0x0
    /* CC9C 8000C09C AC2F7C54 */  sw         $t7, %lo(D_80037C54)($at)
    /* CCA0 8000C0A0 AC2E7C50 */  sw         $t6, %lo(D_80037C50)($at)
    /* CCA4 8000C0A4 3C018003 */  lui        $at, %hi(D_80037C58)
    /* CCA8 8000C0A8 3C188003 */  lui        $t8, %hi(D_8002C3F0)
    /* CCAC 8000C0AC 8F18C3F0 */  lw         $t8, %lo(D_8002C3F0)($t8)
    /* CCB0 8000C0B0 AC207C58 */  sw         $zero, %lo(D_80037C58)($at)
    /* CCB4 8000C0B4 3C018003 */  lui        $at, %hi(D_80037C5C)
    /* CCB8 8000C0B8 AC207C5C */  sw         $zero, %lo(D_80037C5C)($at)
    /* CCBC 8000C0BC 3C198003 */  lui        $t9, %hi(D_8002C3F0)
    /* CCC0 8000C0C0 AF180004 */  sw         $t8, 0x4($t8)
    /* CCC4 8000C0C4 8F39C3F0 */  lw         $t9, %lo(D_8002C3F0)($t9)
    /* CCC8 8000C0C8 3C098003 */  lui        $t1, %hi(D_8002C3F0)
    /* CCCC 8000C0CC 240A0000 */  addiu      $t2, $zero, 0x0
    /* CCD0 8000C0D0 8F280004 */  lw         $t0, 0x4($t9)
    /* CCD4 8000C0D4 240B0000 */  addiu      $t3, $zero, 0x0
    /* CCD8 8000C0D8 3C0C8003 */  lui        $t4, %hi(D_8002C3F0)
    /* CCDC 8000C0DC AF280000 */  sw         $t0, 0x0($t9)
    /* CCE0 8000C0E0 8D29C3F0 */  lw         $t1, %lo(D_8002C3F0)($t1)
    /* CCE4 8000C0E4 3C0D8003 */  lui        $t5, %hi(D_8002C3F0)
    /* CCE8 8000C0E8 3C188003 */  lui        $t8, %hi(D_8002C3F0)
    /* CCEC 8000C0EC AD2A0010 */  sw         $t2, 0x10($t1)
    /* CCF0 8000C0F0 AD2B0014 */  sw         $t3, 0x14($t1)
    /* CCF4 8000C0F4 8D8CC3F0 */  lw         $t4, %lo(D_8002C3F0)($t4)
    /* CCF8 8000C0F8 8D8E0010 */  lw         $t6, 0x10($t4)
    /* CCFC 8000C0FC 8D8F0014 */  lw         $t7, 0x14($t4)
    /* CD00 8000C100 AD8E0008 */  sw         $t6, 0x8($t4)
    /* CD04 8000C104 AD8F000C */  sw         $t7, 0xC($t4)
    /* CD08 8000C108 8DADC3F0 */  lw         $t5, %lo(D_8002C3F0)($t5)
    /* CD0C 8000C10C ADA00018 */  sw         $zero, 0x18($t5)
    /* CD10 8000C110 8F18C3F0 */  lw         $t8, %lo(D_8002C3F0)($t8)
    /* CD14 8000C114 03E00008 */  jr         $ra
    /* CD18 8000C118 AF00001C */   sw        $zero, 0x1C($t8)
```

## Tips for This Category

