# viGetTimeToDeadline

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80001578` |
| **Category** | `boot` |
| **Status** | `TODO` |
| **Instructions** | ~17 |

## Description

get float time until deadline

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `boot` category.

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
./work/boot/viGetTimeToDeadline/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/boot/viGetTimeToDeadline

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/boot/viGetTimeToDeadline --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

No specific reference available.

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 1050.s
# Address: 0x80001578

glabel func_80001578
    /* 2178 80001578 3C0E8003 */  lui        $t6, %hi(D_8002EB9C)
    /* 217C 8000157C 3C0F8003 */  lui        $t7, %hi(D_8002EB64)
    /* 2180 80001580 8DEFEB64 */  lw         $t7, %lo(D_8002EB64)($t7)
    /* 2184 80001584 8DCEEB9C */  lw         $t6, %lo(D_8002EB9C)($t6)
    /* 2188 80001588 3C018003 */  lui        $at, %hi(D_8002AFB8)
    /* 218C 8000158C C428AFB8 */  lwc1       $ft2, %lo(D_8002AFB8)($at)
    /* 2190 80001590 01CFC023 */  subu       $t8, $t6, $t7
    /* 2194 80001594 44982000 */  mtc1       $t8, $ft0
    /* 2198 80001598 00000000 */  nop
    /* 219C 8000159C 468021A0 */  cvt.s.w    $ft1, $ft0
    /* 21A0 800015A0 46083002 */  mul.s      $fv0, $ft1, $ft2
    /* 21A4 800015A4 03E00008 */  jr         $ra
    /* 21A8 800015A8 00000000 */   nop
    /* 21AC 800015AC 03E00008 */  jr         $ra
    /* 21B0 800015B0 00000000 */   nop
    /* 21B4 800015B4 03E00008 */  jr         $ra
    /* 21B8 800015B8 00000000 */   nop
```

## Tips for This Category

