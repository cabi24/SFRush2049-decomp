# bzero

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80008590` |
| **Category** | `libc` |
| **Status** | `TODO` |
| **Instructions** | ~39 |

## Description

optimized zero-fill

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libc` category.

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
./work/libc/bzero/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libc/bzero

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libc/bzero --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Standard C library function - should match common implementations

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Source: 9190.s
# Address: 0x80008590

glabel func_80008590
    /* 9190 80008590 28A1000C */  slti       $at, $a1, 0xC
    /* 9194 80008594 1420001D */  bnez       $at, .L8000860C
    /* 9198 80008598 00041823 */   negu      $v1, $a0
    /* 919C 8000859C 30630003 */  andi       $v1, $v1, 0x3
    /* 91A0 800085A0 10600003 */  beqz       $v1, .L800085B0
    /* 91A4 800085A4 00A32823 */   subu      $a1, $a1, $v1
    /* 91A8 800085A8 A8800000 */  swl        $zero, 0x0($a0)
    /* 91AC 800085AC 00832021 */  addu       $a0, $a0, $v1
  .L800085B0:
    /* 91B0 800085B0 2401FFE0 */  addiu      $at, $zero, -0x20
    /* 91B4 800085B4 00A13824 */  and        $a3, $a1, $at
    /* 91B8 800085B8 10E0000C */  beqz       $a3, .L800085EC
    /* 91BC 800085BC 00A72823 */   subu      $a1, $a1, $a3
    /* 91C0 800085C0 00E43821 */  addu       $a3, $a3, $a0
  .L800085C4:
    /* 91C4 800085C4 24840020 */  addiu      $a0, $a0, 0x20
    /* 91C8 800085C8 AC80FFE0 */  sw         $zero, -0x20($a0)
    /* 91CC 800085CC AC80FFE4 */  sw         $zero, -0x1C($a0)
    /* 91D0 800085D0 AC80FFE8 */  sw         $zero, -0x18($a0)
    /* 91D4 800085D4 AC80FFEC */  sw         $zero, -0x14($a0)
    /* 91D8 800085D8 AC80FFF0 */  sw         $zero, -0x10($a0)
    /* 91DC 800085DC AC80FFF4 */  sw         $zero, -0xC($a0)
    /* 91E0 800085E0 AC80FFF8 */  sw         $zero, -0x8($a0)
    /* 91E4 800085E4 1487FFF7 */  bne        $a0, $a3, .L800085C4
    /* 91E8 800085E8 AC80FFFC */   sw        $zero, -0x4($a0)
  .L800085EC:
    /* 91EC 800085EC 2401FFFC */  addiu      $at, $zero, -0x4
    /* 91F0 800085F0 00A13824 */  and        $a3, $a1, $at
    /* 91F4 800085F4 10E00005 */  beqz       $a3, .L8000860C
    /* 91F8 800085F8 00A72823 */   subu      $a1, $a1, $a3
    /* 91FC 800085FC 00E43821 */  addu       $a3, $a3, $a0
  .L80008600:
    /* 9200 80008600 24840004 */  addiu      $a0, $a0, 0x4
    /* 9204 80008604 1487FFFE */  bne        $a0, $a3, .L80008600
    /* 9208 80008608 AC80FFFC */   sw        $zero, -0x4($a0)
  .L8000860C:
    /* 920C 8000860C 18A00005 */  blez       $a1, .L80008624
    /* 9210 80008610 00000000 */   nop
    /* 9214 80008614 00A42821 */  addu       $a1, $a1, $a0
  .L80008618:
    /* 9218 80008618 24840001 */  addiu      $a0, $a0, 0x1
    /* 921C 8000861C 1485FFFE */  bne        $a0, $a1, .L80008618
    /* 9220 80008620 A080FFFF */   sb        $zero, -0x1($a0)
  .L80008624:
    /* 9224 80008624 03E00008 */  jr         $ra
    /* 9228 80008628 00000000 */   nop
```

## Tips for This Category

