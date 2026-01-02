# osContStartQuery

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80009730` |
| **Category** | `libultra/cont` |
| **Status** | `TODO` |
| **Instructions** | ~31 |

## Description

start controller status query

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
./work/libultra/cont/osContStartQuery/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/cont/osContStartQuery

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/cont/osContStartQuery --watch
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
# Address: 0x80009730

glabel func_80009730
    /* A330 80009730 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* A334 80009734 AFBF0014 */  sw         $ra, 0x14($sp)
    /* A338 80009738 0C00396C */  jal        func_8000E5B0
    /* A33C 8000973C AFA40020 */   sw        $a0, 0x20($sp)
    /* A340 80009740 3C0E8003 */  lui        $t6, %hi(D_80037AE0)
    /* A344 80009744 91CE7AE0 */  lbu        $t6, %lo(D_80037AE0)($t6)
    /* A348 80009748 11C0000B */  beqz       $t6, .L80009778
    /* A34C 8000974C 00000000 */   nop
    /* A350 80009750 0C002596 */  jal        func_80009658
    /* A354 80009754 00002025 */   or        $a0, $zero, $zero
    /* A358 80009758 3C058003 */  lui        $a1, %hi(D_80037AA0)
    /* A35C 8000975C 24A57AA0 */  addiu      $a1, $a1, %lo(D_80037AA0)
    /* A360 80009760 0C00392C */  jal        func_8000E4B0
    /* A364 80009764 24040001 */   addiu     $a0, $zero, 0x1
    /* A368 80009768 8FA40020 */  lw         $a0, 0x20($sp)
    /* A36C 8000976C 00002825 */  or         $a1, $zero, $zero
    /* A370 80009770 0C001C9C */  jal        func_80007270
    /* A374 80009774 24060001 */   addiu     $a2, $zero, 0x1
  .L80009778:
    /* A378 80009778 3C058003 */  lui        $a1, %hi(D_80037AA0)
    /* A37C 8000977C 24A57AA0 */  addiu      $a1, $a1, %lo(D_80037AA0)
    /* A380 80009780 0C00392C */  jal        func_8000E4B0
    /* A384 80009784 00002025 */   or        $a0, $zero, $zero
    /* A388 80009788 3C018003 */  lui        $at, %hi(D_80037AE0)
    /* A38C 8000978C AFA2001C */  sw         $v0, 0x1C($sp)
    /* A390 80009790 0C00397D */  jal        func_8000E5F4
    /* A394 80009794 A0207AE0 */   sb        $zero, %lo(D_80037AE0)($at)
    /* A398 80009798 8FBF0014 */  lw         $ra, 0x14($sp)
    /* A39C 8000979C 8FA2001C */  lw         $v0, 0x1C($sp)
    /* A3A0 800097A0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* A3A4 800097A4 03E00008 */  jr         $ra
    /* A3A8 800097A8 00000000 */   nop
```

## Tips for This Category

