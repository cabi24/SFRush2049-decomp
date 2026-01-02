# osPiReadIo

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D7A0` |
| **Category** | `libultra/pi` |
| **Status** | `TODO` |
| **Instructions** | ~19 |

## Description

read from physical I/O address

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
./work/libultra/pi/osPiReadIo/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pi/osPiReadIo

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pi/osPiReadIo --watch
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
# Source: E390.s
# Address: 0x8000D7A0

glabel func_8000D7A0
    /* E3A0 8000D7A0 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E3A4 8000D7A4 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E3A8 8000D7A8 AFA40018 */  sw         $a0, 0x18($sp)
    /* E3AC 8000D7AC 0C003F5C */  jal        func_8000FD70
    /* E3B0 8000D7B0 AFA5001C */   sw        $a1, 0x1C($sp)
    /* E3B4 8000D7B4 10400003 */  beqz       $v0, .L8000D7C4
    /* E3B8 8000D7B8 8FAE0018 */   lw        $t6, 0x18($sp)
    /* E3BC 8000D7BC 10000007 */  b          .L8000D7DC
    /* E3C0 8000D7C0 2402FFFF */   addiu     $v0, $zero, -0x1
  .L8000D7C4:
    /* E3C4 8000D7C4 3C01A000 */  lui        $at, %hi(D_A0000000)
    /* E3C8 8000D7C8 01C17825 */  or         $t7, $t6, $at
    /* E3CC 8000D7CC 8DF80000 */  lw         $t8, %lo(D_A0000000)($t7)
    /* E3D0 8000D7D0 8FB9001C */  lw         $t9, 0x1C($sp)
    /* E3D4 8000D7D4 00001025 */  or         $v0, $zero, $zero
    /* E3D8 8000D7D8 AF380000 */  sw         $t8, 0x0($t9)
  .L8000D7DC:
    /* E3DC 8000D7DC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E3E0 8000D7E0 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E3E4 8000D7E4 03E00008 */  jr         $ra
    /* E3E8 8000D7E8 00000000 */   nop
```

## Tips for This Category

