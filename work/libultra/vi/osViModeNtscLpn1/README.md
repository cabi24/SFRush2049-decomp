# osViModeNtscLpn1

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80007AB4` |
| **Category** | `libultra/vi` |
| **Status** | `TODO` |
| **Instructions** | ~16 |

## Description

get NTSC LPN1 VI mode

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
./work/libultra/vi/osViModeNtscLpn1/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/vi/osViModeNtscLpn1

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/vi/osViModeNtscLpn1 --watch
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
# Source: 8440.s
# Address: 0x80007AB4

glabel func_80007AB4
    /* 86B4 80007AB4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 86B8 80007AB8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 86BC 80007ABC 0C0035C4 */  jal        func_8000D710
    /* 86C0 80007AC0 AFA40018 */   sw        $a0, 0x18($sp)
    /* 86C4 80007AC4 10400005 */  beqz       $v0, .L80007ADC
    /* 86C8 80007AC8 00000000 */   nop
  .L80007ACC:
    /* 86CC 80007ACC 0C0035C4 */  jal        func_8000D710
    /* 86D0 80007AD0 00000000 */   nop
    /* 86D4 80007AD4 1440FFFD */  bnez       $v0, .L80007ACC
    /* 86D8 80007AD8 00000000 */   nop
  .L80007ADC:
    /* 86DC 80007ADC 0C003590 */  jal        func_8000D640
    /* 86E0 80007AE0 24040125 */   addiu     $a0, $zero, 0x125
    /* 86E4 80007AE4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 86E8 80007AE8 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 86EC 80007AEC 03E00008 */  jr         $ra
    /* 86F0 80007AF0 00000000 */   nop
```

## Tips for This Category

