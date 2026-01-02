# __osTlbFlush

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8000D840` |
| **Category** | `libultra/tlb` |
| **Status** | `TODO` |
| **Instructions** | ~17 |

## Description

flush all TLB entries

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
./work/libultra/tlb/__osTlbFlush/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/libultra/tlb/__osTlbFlush

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/libultra/tlb/__osTlbFlush --watch
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
# Source: E440.s
# Address: 0x8000D840

glabel func_8000D840
    /* E440 8000D840 40085000 */  mfc0       $t0, $10 /* handwritten instruction */
    /* E444 8000D844 2409001E */  addiu      $t1, $zero, 0x1E
    /* E448 8000D848 3C0A8000 */  lui        $t2, (0x80000000 >> 16)
    /* E44C 8000D84C 408A5000 */  mtc0       $t2, $10 /* handwritten instruction */
    /* E450 8000D850 40801000 */  mtc0       $zero, $2 /* handwritten instruction */
    /* E454 8000D854 40801800 */  mtc0       $zero, $3 /* handwritten instruction */
  .L8000D858:
    /* E458 8000D858 40890000 */  mtc0       $t1, $0 /* handwritten instruction */
    /* E45C 8000D85C 00000000 */  nop
    /* E460 8000D860 42000002 */  tlbwi /* handwritten instruction */
    /* E464 8000D864 00000000 */  nop
    /* E468 8000D868 00000000 */  nop
    /* E46C 8000D86C 2129FFFF */  addi       $t1, $t1, -0x1 /* handwritten instruction */
    /* E470 8000D870 0521FFF9 */  bgez       $t1, .L8000D858
    /* E474 8000D874 00000000 */   nop
    /* E478 8000D878 40885000 */  mtc0       $t0, $10 /* handwritten instruction */
    /* E47C 8000D87C 03E00008 */  jr         $ra
    /* E480 8000D880 00000000 */   nop
```

## Tips for This Category

