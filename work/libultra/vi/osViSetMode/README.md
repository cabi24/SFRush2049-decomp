# osViSetMode

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007590` |
| **Category** | `libultra/vi` |
| **Status** | `TODO` |
| **Instructions** | ~19 |

## Description

set VI mode

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
./work/libultra/vi/osViSetMode/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/vi/osViSetMode

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/vi/osViSetMode --watch
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
# Source: 8040.s
# Address: 0x80007590

glabel func_80007590
    /* 8190 80007590 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 8194 80007594 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 8198 80007598 0C00312C */  jal        func_8000C4B0
    /* 819C 8000759C AFA40018 */   sw        $a0, 0x18($sp)
    /* 81A0 800075A0 3C058003 */  lui        $a1, %hi(D_8002C464)
    /* 81A4 800075A4 24A5C464 */  addiu      $a1, $a1, %lo(D_8002C464)
    /* 81A8 800075A8 8CAF0000 */  lw         $t7, 0x0($a1)
    /* 81AC 800075AC 8FAE0018 */  lw         $t6, 0x18($sp)
    /* 81B0 800075B0 00402025 */  or         $a0, $v0, $zero
    /* 81B4 800075B4 ADEE0004 */  sw         $t6, 0x4($t7)
    /* 81B8 800075B8 8CA30000 */  lw         $v1, 0x0($a1)
    /* 81BC 800075BC 94780000 */  lhu        $t8, 0x0($v1)
    /* 81C0 800075C0 37190010 */  ori        $t9, $t8, 0x10
    /* 81C4 800075C4 0C003148 */  jal        func_8000C520
    /* 81C8 800075C8 A4790000 */   sh        $t9, 0x0($v1)
    /* 81CC 800075CC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 81D0 800075D0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 81D4 800075D4 03E00008 */  jr         $ra
    /* 81D8 800075D8 00000000 */   nop
```

## Tips for This Category

