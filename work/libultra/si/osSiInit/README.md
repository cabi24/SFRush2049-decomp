# osSiInit

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000E560` |
| **Category** | `libultra/si` |
| **Status** | `TODO` |
| **Instructions** | ~20 |

## Description

serial interface initialization

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/si` category.

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
./work/libultra/si/osSiInit/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/si/osSiInit

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/si/osSiInit --watch
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
# Source: F160.s
# Address: 0x8000E560

glabel func_8000E560
    /* F160 8000E560 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* F164 8000E564 AFBF0014 */  sw         $ra, 0x14($sp)
    /* F168 8000E568 240E0001 */  addiu      $t6, $zero, 0x1
    /* F16C 8000E56C 3C018003 */  lui        $at, %hi(__osSiInitialized)
    /* F170 8000E570 3C048003 */  lui        $a0, %hi(__osSiMesg)
    /* F174 8000E574 3C058003 */  lui        $a1, %hi(__osSiMesgQueue)
    /* F178 8000E578 AC2EC4B0 */  sw         $t6, %lo(__osSiInitialized)($at)
    /* F17C 8000E57C 24A57C90 */  addiu      $a1, $a1, %lo(__osSiMesgQueue)
    /* F180 8000E580 24847C98 */  addiu      $a0, $a0, %lo(__osSiMesg)
    /* F184 8000E584 0C001A80 */  jal        func_80006A00
    /* F188 8000E588 24060001 */   addiu     $a2, $zero, 0x1
    /* F18C 8000E58C 3C048003 */  lui        $a0, %hi(__osSiMesg)
    /* F190 8000E590 24847C98 */  addiu      $a0, $a0, %lo(__osSiMesg)
    /* F194 8000E594 00002825 */  or         $a1, $zero, $zero
    /* F198 8000E598 0C001D78 */  jal        func_800075E0
    /* F19C 8000E59C 00003025 */   or        $a2, $zero, $zero
    /* F1A0 8000E5A0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* F1A4 8000E5A4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* F1A8 8000E5A8 03E00008 */  jr         $ra
    /* F1AC 8000E5AC 00000000 */   nop
```

## Tips for This Category

