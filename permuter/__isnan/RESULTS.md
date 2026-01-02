# __isnan Function Matching Results

## Summary

**Target**: `__isnan` function at ROM address 0x80002C60
**Best Score Achieved**: ~400-500 (estimated based on differences)
**Previous Permuter Best**: 665

## IDO 7.1 Results

Tested IDO 7.1 with `-g0 -mips2 -O2` as requested:

| Flags | Stack | Store Offset | Branch |
|-------|-------|--------------|--------|
| -O2 | none | 0 | bne (with goto) |
| -O1 | 8 bytes | 8 | bne (with goto) |
| -O0 | 8 bytes | 8 | beq |

**Best IDO 7.1 result**: `-O1` gives 8-byte frame AND `bne`, but stores at offset 8 (target stores at offset 0).

The target's unusual pattern (8-byte frame + store at offset 0) is not achievable with IDO 5.3 or 7.1.

## Key Findings

### What We Got Right
1. **`bne` instruction** - Using `goto` pattern produces correct `bne` instead of `beql`
2. **sdc1/ldc1 double ops** - Correct double load/store instructions
3. **c.eq.d comparison** - Correct floating point comparison
4. **bc1f branch** - Correct FP branch instruction
5. **andi 0x800F mask** - Correct masking operation
6. **mtc1 zero loads** - Correct FP register initialization

### Remaining Differences
1. **Missing stack allocation** (`addiu sp,sp,-8`)
   - Target allocates 8 bytes, our code uses caller's stack space
   - Tried: volatile, unions, explicit arrays - all give 16 bytes, not 8

2. **Pointer register** (`addiu v1,sp,0`)
   - IDO creates a pointer in v1 instead of using sp directly
   - This is an IDO 5.3 codegen quirk

3. **Register allocation** (v0 vs t6)
   - Target uses t6 for hi word, we get v0
   - Difficult to control without inline asm

4. **Redundant branches in target**
   - Target has multiple `b .L80002CC8` instructions
   - Suggests older compiler or different optimization settings

## Best C Code

```c
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef double f64;

s32 __isnan(f64 x) {
    u32 hi, exp;
    hi = ((u32 *)&x)[0];
    exp = (hi << 1) >> 21;
    if (exp != 0x7FF) goto ret_zero;
    ((u16 *)&x)[0] &= 0x800F;
    if (x == 0.0) return 1;
ret_zero:
    return 0;
}
```

## Compiler Flags Tested

| Flags | Stack | Branch | Notes |
|-------|-------|--------|-------|
| -O2 -mips2 | none | beql | Original attempt |
| -O1 -mips2 | 8 bytes | beq | Stack offset wrong (8 not 0) |
| -O0 -mips2 | 8 bytes | beq | Too much code |
| -O2 + goto | none | bne | **Best branch** |
| -O2 + volatile | 16 bytes | bne | Stack too big |

## Conclusion

The target assembly appears to be from:
- A different IDO version (possibly older)
- Different stack alignment settings
- Or possibly hand-written/modified assembly

The 8-byte stack frame with offset 0 storage is unusual for IDO 5.3 which prefers 16-byte alignment for doubles.

## Recommendations

1. **Mark as NON_MATCHING** - Use current best with `#ifdef NON_MATCHING`
2. **Consider asm stub** - If exact match required, use inline assembly
3. **Check other libm functions** - See if similar patterns exist elsewhere
