# osPiInit

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000DBC0` |
| **Category** | `libultra/pi` |
| **Status** | `TODO` |
| **Instructions** | ~20 |

## Description

peripheral interface initialization

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/pi` category.

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
./work/libultra/pi/osPiInit/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pi/osPiInit

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pi/osPiInit --watch
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
# Source: E7C0.s
# Address: 0x8000DBC0

glabel func_8000DBC0
    /* E7C0 8000DBC0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E7C4 8000DBC4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E7C8 8000DBC8 240E0001 */  addiu      $t6, $zero, 0x1
    /* E7CC 8000DBCC 3C018003 */  lui        $at, %hi(D_8002C4A0)
    /* E7D0 8000DBD0 3C048003 */  lui        $a0, %hi(D_80037C78)
    /* E7D4 8000DBD4 3C058003 */  lui        $a1, %hi(D_80037C70)
    /* E7D8 8000DBD8 AC2EC4A0 */  sw         $t6, %lo(D_8002C4A0)($at)
    /* E7DC 8000DBDC 24A57C70 */  addiu      $a1, $a1, %lo(D_80037C70)
    /* E7E0 8000DBE0 24847C78 */  addiu      $a0, $a0, %lo(D_80037C78)
    /* E7E4 8000DBE4 0C001A80 */  jal        func_80006A00
    /* E7E8 8000DBE8 24060001 */   addiu     $a2, $zero, 0x1
    /* E7EC 8000DBEC 3C048003 */  lui        $a0, %hi(D_80037C78)
    /* E7F0 8000DBF0 24847C78 */  addiu      $a0, $a0, %lo(D_80037C78)
    /* E7F4 8000DBF4 00002825 */  or         $a1, $zero, $zero
    /* E7F8 8000DBF8 0C001D78 */  jal        func_800075E0
    /* E7FC 8000DBFC 00003025 */   or        $a2, $zero, $zero
    /* E800 8000DC00 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E804 8000DC04 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E808 8000DC08 03E00008 */  jr         $ra
    /* E80C 8000DC0C 00000000 */   nop
```

## Tips for This Category

