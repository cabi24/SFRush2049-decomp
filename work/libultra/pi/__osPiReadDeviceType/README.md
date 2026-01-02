# __osPiReadDeviceType

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80008128` |
| **Category** | `libultra/pi` |
| **Status** | `TODO` |
| **Instructions** | ~39 |

## Description

save PI BSD DOM1/DOM2 timing registers to globals

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
./work/libultra/pi/__osPiReadDeviceType/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/pi/__osPiReadDeviceType

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/pi/__osPiReadDeviceType --watch
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
# Source: 8A80.s
# Address: 0x80008128

glabel func_80008128
    /* 8D28 80008128 240E0007 */  addiu      $t6, $zero, 0x7
    /* 8D2C 8000812C 3C018003 */  lui        $at, %hi(D_800367E4)
    /* 8D30 80008130 A02E67E4 */  sb         $t6, %lo(D_800367E4)($at)
    /* 8D34 80008134 3C0FA460 */  lui        $t7, %hi(PI_BSD_DOM1_LAT_REG)
    /* 8D38 80008138 8DF80014 */  lw         $t8, %lo(PI_BSD_DOM1_LAT_REG)($t7)
    /* 8D3C 8000813C 3C018003 */  lui        $at, %hi(D_800367E5)
    /* 8D40 80008140 3C19A460 */  lui        $t9, %hi(PI_BSD_DOM1_PWD_REG)
    /* 8D44 80008144 A03867E5 */  sb         $t8, %lo(D_800367E5)($at)
    /* 8D48 80008148 8F280018 */  lw         $t0, %lo(PI_BSD_DOM1_PWD_REG)($t9)
    /* 8D4C 8000814C 3C018003 */  lui        $at, %hi(D_800367E8)
    /* 8D50 80008150 3C09A460 */  lui        $t1, %hi(PI_BSD_DOM1_PGS_REG)
    /* 8D54 80008154 A02867E8 */  sb         $t0, %lo(D_800367E8)($at)
    /* 8D58 80008158 8D2A001C */  lw         $t2, %lo(PI_BSD_DOM1_PGS_REG)($t1)
    /* 8D5C 8000815C 3C018003 */  lui        $at, %hi(D_800367E6)
    /* 8D60 80008160 3C0BA460 */  lui        $t3, %hi(PI_BSD_DOM1_RLS_REG)
    /* 8D64 80008164 A02A67E6 */  sb         $t2, %lo(D_800367E6)($at)
    /* 8D68 80008168 8D6C0020 */  lw         $t4, %lo(PI_BSD_DOM1_RLS_REG)($t3)
    /* 8D6C 8000816C 3C018003 */  lui        $at, %hi(D_800367E7)
    /* 8D70 80008170 240D0007 */  addiu      $t5, $zero, 0x7
    /* 8D74 80008174 A02C67E7 */  sb         $t4, %lo(D_800367E7)($at)
    /* 8D78 80008178 3C018003 */  lui        $at, %hi(D_8003685C)
    /* 8D7C 8000817C A02D685C */  sb         $t5, %lo(D_8003685C)($at)
    /* 8D80 80008180 3C0EA460 */  lui        $t6, %hi(PI_BSD_DOM2_LAT_REG)
    /* 8D84 80008184 8DCF0024 */  lw         $t7, %lo(PI_BSD_DOM2_LAT_REG)($t6)
    /* 8D88 80008188 3C018003 */  lui        $at, %hi(D_8003685D)
    /* 8D8C 8000818C 3C18A460 */  lui        $t8, %hi(PI_BSD_DOM2_LWD_REG)
    /* 8D90 80008190 A02F685D */  sb         $t7, %lo(D_8003685D)($at)
    /* 8D94 80008194 8F190028 */  lw         $t9, %lo(PI_BSD_DOM2_LWD_REG)($t8)
    /* 8D98 80008198 3C018003 */  lui        $at, %hi(D_80036860)
    /* 8D9C 8000819C 3C08A460 */  lui        $t0, %hi(PI_BSD_DOM2_PGS_REG)
    /* 8DA0 800081A0 A0396860 */  sb         $t9, %lo(D_80036860)($at)
    /* 8DA4 800081A4 8D09002C */  lw         $t1, %lo(PI_BSD_DOM2_PGS_REG)($t0)
    /* 8DA8 800081A8 3C018003 */  lui        $at, %hi(D_8003685E)
    /* 8DAC 800081AC 3C0AA460 */  lui        $t2, %hi(PI_BSD_DOM2_RLS_REG)
    /* 8DB0 800081B0 A029685E */  sb         $t1, %lo(D_8003685E)($at)
    /* 8DB4 800081B4 8D4B0030 */  lw         $t3, %lo(PI_BSD_DOM2_RLS_REG)($t2)
    /* 8DB8 800081B8 3C018003 */  lui        $at, %hi(D_8003685F)
    /* 8DBC 800081BC 03E00008 */  jr         $ra
    /* 8DC0 800081C0 A02B685F */   sb        $t3, %lo(D_8003685F)($at)
```

## Tips for This Category

