# __floatundisf

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x80009BC8` |
| **Category** | `libgcc` |
| **Status** | `TODO` |
| **Instructions** | ~12 |

## Description

unsigned long long to float

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libgcc` category.

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
./work/libgcc/__floatundisf/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libgcc/__floatundisf

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libgcc/__floatundisf --watch
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
# Source: A5F0.s
# Address: 0x80009BC8

glabel func_80009BC8
    /* A7C8 80009BC8 AFA40000 */  sw         $a0, 0x0($sp)
    /* A7CC 80009BCC AFA50004 */  sw         $a1, 0x4($sp)
    /* A7D0 80009BD0 DFAE0000 */  ld         $t6, 0x0($sp)
    /* A7D4 80009BD4 44AE2000 */  dmtc1      $t6, $ft0
    /* A7D8 80009BD8 05C10005 */  bgez       $t6, .L80009BF0
    /* A7DC 80009BDC 46A02020 */   cvt.s.l   $fv0, $ft0
    /* A7E0 80009BE0 3C014F80 */  lui        $at, (0x4F800000 >> 16)
    /* A7E4 80009BE4 44813000 */  mtc1       $at, $ft1
    /* A7E8 80009BE8 00000000 */  nop
    /* A7EC 80009BEC 46060000 */  add.s      $fv0, $fv0, $ft1
  .L80009BF0:
    /* A7F0 80009BF0 03E00008 */  jr         $ra
    /* A7F4 80009BF4 00000000 */   nop
```

## Tips for This Category

