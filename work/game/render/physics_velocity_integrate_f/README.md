# physics_velocity_integrate_f

## Quick Info

| Property | Value |
|----------|-------|
| **Address** | `0x8008BD9C` |
| **Category** | `game/render` |
| **Status** | `TODO` |
| **Instructions** | ~0 |

## Description

physics velocity integration variant

## Compiler Settings

```bash
-g0 -O2 -mips2 -G 0 -non_shared
```

These flags were determined by matching other functions in the `game/render` category.

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
./work/game/render/physics_velocity_integrate_f/compile.sh

# Or use asmdiff for side-by-side view:
python3 tools/asmdiff.py work/game/render/physics_velocity_integrate_f

# Watch mode (auto-refresh on save):
python3 tools/asmdiff.py work/game/render/physics_velocity_integrate_f --watch
```

### Step 4: Update Status

When done, update the `STATUS` file:
- `MATCHING` - Byte-for-byte match achieved
- `CLOSE` - Compiles, minor differences
- `WIP` - Still working on it

## Reference Materials

Check `reference/repos/rushtherock/game/collision.c`

### Useful Resources

- Symbol table: `symbol_addrs.us.txt`
- Original assembly: `asm/us/*.s`
- Arcade source: `reference/repos/rushtherock/`

## Target Assembly

```mips
# Target assembly not found
# Address: 0x8008BD9C
# Search in asm/us/*.s for this address

```

## Tips for This Category

