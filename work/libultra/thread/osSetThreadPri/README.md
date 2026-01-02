# osSetThreadPri

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80006D40` |
| **Category** | `libultra/thread` |
| **Status** | `TODO` |
| **Instructions** | ~22 |

## Description

set thread priority

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/thread` category.

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
./work/libultra/thread/osSetThreadPri/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/thread/osSetThreadPri

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/thread/osSetThreadPri --watch
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
# Source: 7940.s
# Address: 0x80006D40

glabel func_80006D40
    /* 7940 80006D40 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 7944 80006D44 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 7948 80006D48 0C00312C */  jal        func_8000C4B0
    /* 794C 80006D4C AFA40018 */   sw        $a0, 0x18($sp)
    /* 7950 80006D50 3C058003 */  lui        $a1, %hi(D_8002C464)
    /* 7954 80006D54 24A5C464 */  addiu      $a1, $a1, %lo(D_8002C464)
    /* 7958 80006D58 8CAF0000 */  lw         $t7, 0x0($a1)
    /* 795C 80006D5C 8FAE0018 */  lw         $t6, 0x18($sp)
    /* 7960 80006D60 24180001 */  addiu      $t8, $zero, 0x1
    /* 7964 80006D64 00402025 */  or         $a0, $v0, $zero
    /* 7968 80006D68 ADEE0008 */  sw         $t6, 0x8($t7)
    /* 796C 80006D6C 8CB90000 */  lw         $t9, 0x0($a1)
    /* 7970 80006D70 A7380000 */  sh         $t8, 0x0($t9)
    /* 7974 80006D74 8CA30000 */  lw         $v1, 0x0($a1)
    /* 7978 80006D78 8C680008 */  lw         $t0, 0x8($v1)
    /* 797C 80006D7C 8D090004 */  lw         $t1, 0x4($t0)
    /* 7980 80006D80 0C003148 */  jal        func_8000C520
    /* 7984 80006D84 AC69000C */   sw        $t1, 0xC($v1)
    /* 7988 80006D88 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 798C 80006D8C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 7990 80006D90 03E00008 */  jr         $ra
    /* 7994 80006D94 00000000 */   nop
```

## Tips for This Category

