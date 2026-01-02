# osSpTaskYielded

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007730` |
| **Category** | `libultra/sp` |
| **Status** | `TODO` |
| **Instructions** | ~24 |

## Description

check if SP task yielded

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/sp` category.

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
./work/libultra/sp/osSpTaskYielded/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/sp/osSpTaskYielded

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/sp/osSpTaskYielded --watch
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
# Source: 8330.s
# Address: 0x80007730

glabel func_80007730
    /* 8330 80007730 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 8334 80007734 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8338 80007738 0C0034A8 */  jal        func_8000D2A0
    /* 833C 8000773C AFA40018 */   sw        $a0, 0x18($sp)
    /* 8340 80007740 304E0100 */  andi       $t6, $v0, 0x100
    /* 8344 80007744 11C00003 */  beqz       $t6, .L80007754
    /* 8348 80007748 8FA40018 */   lw        $a0, 0x18($sp)
    /* 834C 8000774C 10000002 */  b          .L80007758
    /* 8350 80007750 24030001 */   addiu     $v1, $zero, 0x1
  .L80007754:
    /* 8354 80007754 00001825 */  or         $v1, $zero, $zero
  .L80007758:
    /* 8358 80007758 304F0080 */  andi       $t7, $v0, 0x80
    /* 835C 8000775C 51E00008 */  beql       $t7, $zero, .L80007780
    /* 8360 80007760 8FBF0014 */   lw        $ra, 0x14($sp)
    /* 8364 80007764 8C980004 */  lw         $t8, 0x4($a0)
    /* 8368 80007768 2401FFFD */  addiu      $at, $zero, -0x3
    /* 836C 8000776C 0303C825 */  or         $t9, $t8, $v1
    /* 8370 80007770 AC990004 */  sw         $t9, 0x4($a0)
    /* 8374 80007774 03214824 */  and        $t1, $t9, $at
    /* 8378 80007778 AC890004 */  sw         $t1, 0x4($a0)
    /* 837C 8000777C 8FBF0014 */  lw         $ra, 0x14($sp)
  .L80007780:
    /* 8380 80007780 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 8384 80007784 00601025 */  or         $v0, $v1, $zero
    /* 8388 80007788 03E00008 */  jr         $ra
    /* 838C 8000778C 00000000 */   nop
```

## Tips for This Category

