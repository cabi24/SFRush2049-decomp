# __osTlbInit

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D890` |
| **Category** | `libultra/tlb` |
| **Status** | `TODO` |
| **Instructions** | ~22 |

## Description

initialize TLB for 0xC0000000 region

## Compiler Settings

```bash
-g0 -O1 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `libultra/tlb` category.

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
./work/libultra/tlb/__osTlbInit/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/tlb/__osTlbInit

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/tlb/__osTlbInit --watch
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
# Source: E490.s
# Address: 0x8000D890

glabel func_8000D890
    /* E490 8000D890 40085000 */  mfc0       $t0, $10 /* handwritten instruction */
    /* E494 8000D894 2409001F */  addiu      $t1, $zero, 0x1F
    /* E498 8000D898 40890000 */  mtc0       $t1, $0 /* handwritten instruction */
    /* E49C 8000D89C 40802800 */  mtc0       $zero, $5 /* handwritten instruction */
    /* E4A0 8000D8A0 240A0017 */  addiu      $t2, $zero, 0x17
    /* E4A4 8000D8A4 3C09C000 */  lui        $t1, (0xC0000000 >> 16)
    /* E4A8 8000D8A8 40895000 */  mtc0       $t1, $10 /* handwritten instruction */
    /* E4AC 8000D8AC 3C098000 */  lui        $t1, (0x80000000 >> 16)
    /* E4B0 8000D8B0 00095982 */  srl        $t3, $t1, 6
    /* E4B4 8000D8B4 016A5825 */  or         $t3, $t3, $t2
    /* E4B8 8000D8B8 408B1000 */  mtc0       $t3, $2 /* handwritten instruction */
    /* E4BC 8000D8BC 24090001 */  addiu      $t1, $zero, 0x1
    /* E4C0 8000D8C0 40891800 */  mtc0       $t1, $3 /* handwritten instruction */
    /* E4C4 8000D8C4 00000000 */  nop
    /* E4C8 8000D8C8 42000002 */  tlbwi /* handwritten instruction */
    /* E4CC 8000D8CC 00000000 */  nop
    /* E4D0 8000D8D0 00000000 */  nop
    /* E4D4 8000D8D4 00000000 */  nop
    /* E4D8 8000D8D8 00000000 */  nop
    /* E4DC 8000D8DC 40885000 */  mtc0       $t0, $10 /* handwritten instruction */
    /* E4E0 8000D8E0 03E00008 */  jr         $ra
    /* E4E4 8000D8E4 00000000 */   nop
```

## Tips for This Category

