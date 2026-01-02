# osContStartReadData2

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x800097D0` |
| **Category** | `libultra/cont` |
| **Status** | `TODO` |
| **Instructions** | ~33 |

## Description

alternative start controller read

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/cont` category.

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
./work/libultra/cont/osContStartReadData2/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/cont/osContStartReadData2

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/cont/osContStartReadData2 --watch
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
# Source: A330.s
# Address: 0x800097D0

glabel func_800097D0
    /* A3D0 800097D0 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* A3D4 800097D4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* A3D8 800097D8 0C00396C */  jal        func_8000E5B0
    /* A3DC 800097DC AFA40020 */   sw        $a0, 0x20($sp)
    /* A3E0 800097E0 3C0E8003 */  lui        $t6, %hi(D_80037AE0)
    /* A3E4 800097E4 91CE7AE0 */  lbu        $t6, %lo(D_80037AE0)($t6)
    /* A3E8 800097E8 24010001 */  addiu      $at, $zero, 0x1
    /* A3EC 800097EC 11C1000B */  beq        $t6, $at, .L8000981C
    /* A3F0 800097F0 00000000 */   nop
    /* A3F4 800097F4 0C002638 */  jal        func_800098E0
    /* A3F8 800097F8 00000000 */   nop
    /* A3FC 800097FC 3C058003 */  lui        $a1, %hi(D_80037AA0)
    /* A400 80009800 24A57AA0 */  addiu      $a1, $a1, %lo(D_80037AA0)
    /* A404 80009804 0C00392C */  jal        func_8000E4B0
    /* A408 80009808 24040001 */   addiu     $a0, $zero, 0x1
    /* A40C 8000980C 8FA40020 */  lw         $a0, 0x20($sp)
    /* A410 80009810 00002825 */  or         $a1, $zero, $zero
    /* A414 80009814 0C001C9C */  jal        func_80007270
    /* A418 80009818 24060001 */   addiu     $a2, $zero, 0x1
  .L8000981C:
    /* A41C 8000981C 3C058003 */  lui        $a1, %hi(D_80037AA0)
    /* A420 80009820 24A57AA0 */  addiu      $a1, $a1, %lo(D_80037AA0)
    /* A424 80009824 0C00392C */  jal        func_8000E4B0
    /* A428 80009828 00002025 */   or        $a0, $zero, $zero
    /* A42C 8000982C 240F0001 */  addiu      $t7, $zero, 0x1
    /* A430 80009830 3C018003 */  lui        $at, %hi(D_80037AE0)
    /* A434 80009834 AFA2001C */  sw         $v0, 0x1C($sp)
    /* A438 80009838 0C00397D */  jal        func_8000E5F4
    /* A43C 8000983C A02F7AE0 */   sb        $t7, %lo(D_80037AE0)($at)
    /* A440 80009840 8FBF0014 */  lw         $ra, 0x14($sp)
    /* A444 80009844 8FA2001C */  lw         $v0, 0x1C($sp)
    /* A448 80009848 27BD0020 */  addiu      $sp, $sp, 0x20
    /* A44C 8000984C 03E00008 */  jr         $ra
    /* A450 80009850 00000000 */   nop
```

## Tips for This Category

