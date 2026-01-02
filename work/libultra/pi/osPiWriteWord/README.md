# osPiWriteWord

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D7F0` |
| **Category** | `libultra/pi` |
| **Status** | `TODO` |
| **Instructions** | ~18 |

## Description

write word to cartridge space

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
./work/libultra/pi/osPiWriteWord/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pi/osPiWriteWord

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pi/osPiWriteWord --watch
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
# Source: E3F0.s
# Address: 0x8000D7F0

glabel func_8000D7F0
    /* E3F0 8000D7F0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E3F4 8000D7F4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E3F8 8000D7F8 AFA40018 */  sw         $a0, 0x18($sp)
    /* E3FC 8000D7FC 0C003F5C */  jal        func_8000FD70
    /* E400 8000D800 AFA5001C */   sw        $a1, 0x1C($sp)
    /* E404 8000D804 10400003 */  beqz       $v0, .L8000D814
    /* E408 8000D808 8FAE001C */   lw        $t6, 0x1C($sp)
    /* E40C 8000D80C 10000006 */  b          .L8000D828
    /* E410 8000D810 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000D814:
    /* E414 8000D814 8FAF0018 */  lw         $t7, 0x18($sp)
    /* E418 8000D818 3C01A000 */  lui        $at, %hi(D_A0000000)
    /* E41C 8000D81C 00001025 */  or         $v0, $zero, $zero
    /* E420 8000D820 01E1C025 */  or         $t8, $t7, $at
    /* E424 8000D824 AF0E0000 */  sw         $t6, %lo(D_A0000000)($t8)
  .L8000D828:
    /* E428 8000D828 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E42C 8000D82C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E430 8000D830 03E00008 */  jr         $ra
    /* E434 8000D834 00000000 */   nop
```

## Tips for This Category

