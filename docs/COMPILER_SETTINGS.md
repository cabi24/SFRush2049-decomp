# Compiler Settings for Rush 2049 N64

## Confirmed Compiler

**IDO 5.3** (IRIX Development Option / SGI C Compiler)

The game was compiled using SGI's IDO compiler. Both IDO 5.3 and 7.1 have been tested and produce similar output for most functions.

## Base Flags

All code uses these common flags:
```
-g0 -mips2 -G 0 -non_shared
```

- `-g0`: No debug info
- `-mips2`: MIPS II instruction set (N64 R4300)
- `-G 0`: No global pointer optimization
- `-non_shared`: Static linking

## Mixed Optimization Levels

**Key Discovery**: The game uses DIFFERENT optimization levels for different source files!

### Functions Matching with -O2

| Function | File | Notes |
|----------|------|-------|
| `strlen` | libc | Perfect byte-for-byte match |
| `guMtxIdentF` | libultra/gu | Perfect byte-for-byte match |

Compile command:
```bash
cc -g0 -O2 -mips2 -G 0 -non_shared -c source.c
```

### Functions Matching with -O1

| Function | File | Notes |
|----------|------|-------|
| `osCreateMesgQueue` | libultra/os | Perfect match (registers t6/t7 pattern) |

Compile command:
```bash
cc -g0 -O1 -mips2 -G 0 -non_shared -c source.c
```

## Pattern Analysis

### -O2 Characteristics
- No stack frame for leaf functions
- Aggressive CSE (common subexpression elimination)
- Branch-likely instructions (`beql`, `bnel`)
- Loop optimizations

### -O1 Characteristics
- Creates stack frames even for simple functions
- No CSE - loads same address twice if written twice in source
- Arguments spilled to stack
- More straightforward register allocation

## Functions That Don't Match

Some functions appear to be hand-optimized or use non-standard compiler settings:

| Function | Issue | Likely Cause |
|----------|-------|--------------|
| `__isnan` | 8-byte stack frame, unusual pattern | Hand-optimized |
| `bzero` | 32-byte unrolled loop | Hand-optimized ASM |
| `memcpy` | No loop unrolling | Different -O level or hand-written |
| `strchr` | Different branch pattern | Possibly different compiler version |

## Build Strategy

For matching decompilation:

1. **libc functions** (string.c, etc.): Try `-O2` first
2. **libultra/os functions**: Try `-O1` first
3. **libultra/gu functions**: Try `-O2` first
4. **Game code**: Likely `-O2` based on branch-likely usage

## Test Results Summary

```
strlen:            -O2 PERFECT MATCH
osCreateMesgQueue: -O1 PERFECT MATCH
guMtxIdentF:       -O2 PERFECT MATCH
strchr:            -O2 close but different register allocation
memcpy:            -O2 adds loop unrolling (target doesn't have it)
__isnan:           Neither -O1 nor -O2 produce matching output
```

## Recommended Approach

When decompiling a new function:

1. Check which file/module it belongs to
2. Start with the optimization level used by similar functions
3. If no match, try the other level
4. If still no match, check if function might be hand-optimized

## Tooling

Use these commands to test:

```bash
# IDO 5.3 (default)
ssh watchman "tools/ido-static-recomp/build/out/cc -g0 -O2 -mips2 -G 0 -non_shared -c -o /tmp/test.o /tmp/test.c && mips-linux-gnu-objdump -d /tmp/test.o"

# IDO 7.1
ssh watchman "tools/ido7.1/cc -g0 -O2 -mips2 -G 0 -non_shared -c -o /tmp/test.o /tmp/test.c && mips-linux-gnu-objdump -d /tmp/test.o"
```

## Date

Last updated: 2026-01-02
