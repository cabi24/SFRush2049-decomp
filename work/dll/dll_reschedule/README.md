# dll_reschedule

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000C294` |
| **Category** | `dll` |
| **Status** | `TODO` |
| **Instructions** | ~29 |

## Description

reschedule timer interrupt (__osSetTimerIntr)

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
./work/dll/dll_reschedule/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/dll/dll_reschedule

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/dll/dll_reschedule --watch
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
# Address: 0x8000C294

glabel func_8000C294
    /* CE94 8000C294 27BDFFD8 */  addiu      $sp, $sp, -0x28
    /* CE98 8000C298 AFBF0014 */  sw         $ra, 0x14($sp)
    /* CE9C 8000C29C AFA40028 */  sw         $a0, 0x28($sp)
    /* CEA0 8000C2A0 0C00312C */  jal        func_8000C4B0
    /* CEA4 8000C2A4 AFA5002C */   sw        $a1, 0x2C($sp)
    /* CEA8 8000C2A8 0C00325C */  jal        func_8000C970
    /* CEAC 8000C2AC AFA2001C */   sw        $v0, 0x1C($sp)
    /* CEB0 8000C2B0 3C018003 */  lui        $at, %hi(D_80037C60)
    /* CEB4 8000C2B4 AC227C60 */  sw         $v0, %lo(D_80037C60)($at)
    /* CEB8 8000C2B8 3C0E8003 */  lui        $t6, %hi(D_80037C60)
    /* CEBC 8000C2BC 8DCE7C60 */  lw         $t6, %lo(D_80037C60)($t6)
    /* CEC0 8000C2C0 8FA9002C */  lw         $t1, 0x2C($sp)
    /* CEC4 8000C2C4 8FA80028 */  lw         $t0, 0x28($sp)
    /* CEC8 8000C2C8 01C0C825 */  or         $t9, $t6, $zero
    /* CECC 8000C2CC 03295821 */  addu       $t3, $t9, $t1
    /* CED0 8000C2D0 24180000 */  addiu      $t8, $zero, 0x0
    /* CED4 8000C2D4 0169082B */  sltu       $at, $t3, $t1
    /* CED8 8000C2D8 00385021 */  addu       $t2, $at, $t8
    /* CEDC 8000C2DC 01485021 */  addu       $t2, $t2, $t0
    /* CEE0 8000C2E0 AFAA0020 */  sw         $t2, 0x20($sp)
    /* CEE4 8000C2E4 AFAB0024 */  sw         $t3, 0x24($sp)
    /* CEE8 8000C2E8 0C003EE4 */  jal        func_8000FB90
    /* CEEC 8000C2EC 01602025 */   or        $a0, $t3, $zero
    /* CEF0 8000C2F0 0C003148 */  jal        func_8000C520
    /* CEF4 8000C2F4 8FA4001C */   lw        $a0, 0x1C($sp)
    /* CEF8 8000C2F8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* CEFC 8000C2FC 27BD0028 */  addiu      $sp, $sp, 0x28
    /* CF00 8000C300 03E00008 */  jr         $ra
    /* CF04 8000C304 00000000 */   nop
```

## Tips for This Category

