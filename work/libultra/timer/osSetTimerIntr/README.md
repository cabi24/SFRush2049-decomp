# osSetTimerIntr

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80006ED0` |
| **Category** | `libultra/timer` |
| **Status** | `TODO` |
| **Instructions** | ~23 |

## Description

set timer interrupt

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/timer` category.

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
./work/libultra/timer/osSetTimerIntr/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/timer/osSetTimerIntr

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/timer/osSetTimerIntr --watch
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
# Source: 7AD0.s
# Address: 0x80006ED0

glabel func_80006ED0
    /* 7AD0 80006ED0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 7AD4 80006ED4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 7AD8 80006ED8 AFA40018 */  sw         $a0, 0x18($sp)
    /* 7ADC 80006EDC AFA5001C */  sw         $a1, 0x1C($sp)
    /* 7AE0 80006EE0 0C00312C */  jal        func_8000C4B0
    /* 7AE4 80006EE4 AFA60020 */   sw        $a2, 0x20($sp)
    /* 7AE8 80006EE8 3C038003 */  lui        $v1, %hi(D_8002C464)
    /* 7AEC 80006EEC 2463C464 */  addiu      $v1, $v1, %lo(D_8002C464)
    /* 7AF0 80006EF0 8C6F0000 */  lw         $t7, 0x0($v1)
    /* 7AF4 80006EF4 8FAE0018 */  lw         $t6, 0x18($sp)
    /* 7AF8 80006EF8 00402025 */  or         $a0, $v0, $zero
    /* 7AFC 80006EFC ADEE0010 */  sw         $t6, 0x10($t7)
    /* 7B00 80006F00 8C790000 */  lw         $t9, 0x0($v1)
    /* 7B04 80006F04 8FB8001C */  lw         $t8, 0x1C($sp)
    /* 7B08 80006F08 AF380014 */  sw         $t8, 0x14($t9)
    /* 7B0C 80006F0C 8C690000 */  lw         $t1, 0x0($v1)
    /* 7B10 80006F10 8FA80020 */  lw         $t0, 0x20($sp)
    /* 7B14 80006F14 0C003148 */  jal        func_8000C520
    /* 7B18 80006F18 A5280002 */   sh        $t0, 0x2($t1)
    /* 7B1C 80006F1C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 7B20 80006F20 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 7B24 80006F24 03E00008 */  jr         $ra
    /* 7B28 80006F28 00000000 */   nop
```

## Tips for This Category

