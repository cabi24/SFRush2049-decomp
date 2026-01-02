# osViGetCurrentFramebuffer

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007790` |
| **Category** | `libultra/vi` |
| **Status** | `TODO` |
| **Instructions** | ~15 |

## Description

get current framebuffer

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/vi` category.

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
./work/libultra/vi/osViGetCurrentFramebuffer/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/vi/osViGetCurrentFramebuffer

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/vi/osViGetCurrentFramebuffer --watch
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
# Address: 0x80007790

glabel func_80007790
    /* 8390 80007790 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 8394 80007794 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8398 80007798 0C00312C */  jal        func_8000C4B0
    /* 839C 8000779C 00000000 */   nop
    /* 83A0 800077A0 3C0E8003 */  lui        $t6, %hi(D_8002C460)
    /* 83A4 800077A4 8DCEC460 */  lw         $t6, %lo(D_8002C460)($t6)
    /* 83A8 800077A8 00402025 */  or         $a0, $v0, $zero
    /* 83AC 800077AC 8DCF0004 */  lw         $t7, 0x4($t6)
    /* 83B0 800077B0 0C003148 */  jal        func_8000C520
    /* 83B4 800077B4 AFAF0018 */   sw        $t7, 0x18($sp)
    /* 83B8 800077B8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 83BC 800077BC 8FA20018 */  lw         $v0, 0x18($sp)
    /* 83C0 800077C0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 83C4 800077C4 03E00008 */  jr         $ra
    /* 83C8 800077C8 00000000 */   nop
```

## Tips for This Category

