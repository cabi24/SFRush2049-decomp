# Compiler Settings for Rush 2049 N64

The initial experiments below were recorded on 2026-01-02. Use per-target
recorded flagsets when available; subsystem defaults are starting points.
See [builder operations](BUILDING.md#watchman2-builder) for the current
host and toolchain paths, and the [matching skill](../.claude/skills/match-function/SKILL.md)
for the workflow.

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

## Interprocedural register allocation in game code (hypothesis, 2026-09-28)

Evidence gathered 2026-09-28; status is **hypothesis with strong evidence**,
not a confirmed build recipe.

**Observation.** 99 extracted game functions read `$t0`-`$t3` (or `$s` registers)
on entry without ever writing them, and their callers load those registers
right before the `jal`. Example: every caller of `audio_helper` (0x80097384)
does `move $t0,$zero; move $t1,$zero` before calling it, and `audio_helper`
stores both registers. O32, which IDO `-O2` uses, passes arguments 5-8 on the
stack. A separate but related pattern: callers keep using `$a0`-`$a3`/`$t*`
after a `jal` to a callee that is known not to clobber them
(`audio_buffer_sync` reuses `$a1` across `jal func_80095F8C`).

**Mechanism reproduced.** IDO `-O3` applies interprocedural register allocation
to a static function whose call sites are all visible. In a synthetic test the
callee took its parameters in `$s2`/`$s4`/`$s6`, which its callers set, and read
them without writing them. The same mechanism explains the dead-looking
`jal sound_update_channel; move $t0,$zero` wrappers (`object_type_byte2/3_get`,
`mode_byte_set`, ...). The value is the callee's `force` argument, passed in
`$t0`. `camera_update_c` does the same with a float argument in `$f16`.

**Scope.** IPA callees plus every function that calls one: 243 of the 912
gate-passed functions, about 39% of their instructions (an upper bound; some
call sites use standard registers and already matched at `-O2`). This group
includes most of the 229 `partial_decomp` m2c seeds (the "Read from unset
register" errors) and the `saved_reg_s1`/`saved_reg_s3` histogram blockers.

**Why they cannot match today.** A function is compiled alone at `-O2`, so IDO
never sees the callee's register usage. Compiling caller and callee together
per file at `-O3` was not enough: with external callees, per-file `-O3` dropped
the calls entirely. The original build probably merged ucode across files
(whole-program `-O3`). Reproducing it means compiling a group of functions to
ucode and linking it with `uld` before optimization, with the real source for
every member of the call group.

**Seed-side partial fix.** `tools/m2c_patches/experimental/0003-ipa-preserved-caller-save-regs.patch`
makes m2c fall back to a register's pre-call value instead of `M2C_ERROR`.
It turns 38 of the 229 partial seeds into compiling seeds, but those seeds
score far off at `-O2` because their callers still depend on IPA. The patch is
not in the active patch set on purpose: it would spend node time on seeds
that cannot match at `-O2`.

## Build Strategy

For matching decompilation:

1. **libc functions** (string.c, etc.): Try `-O2` first
2. **libultra/os functions**: Try `-O1` first
3. **libultra/gu functions**: Try `-O2` first
4. **Game code**: Check recorded evidence; matches exist under different flagsets.
   Do not assume `-O2` from branch-likely usage alone.

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

IDO runs on the x86 builder (`watchman2`); its recompiled binaries require
4 KB pages and do not work on the Pi's 16 KB page configuration. Prefer Conveyor
compile/score jobs so source, toolkit, target object, and flags remain attributable.
For a standalone probe, first place the candidate at `/tmp/test.c` on the builder:

```bash
ssh watchman2 'cd ~/rush2049/repo && tools/ido-static-recomp/build/out/cc -g0 -O2 -mips2 -G 0 -non_shared -c -o /tmp/test.o /tmp/test.c'
ssh watchman2 'mips-linux-gnu-objdump -d /tmp/test.o'
```

IDO 7.1 was available on the retired `watchman`; verify availability before
using historical paths. Do not overwrite the builder's x86 IDO with the Pi's
aarch64 build during sync.

## C89 source compatibility

The initial port required these changes, moved here from `CLAUDE.md`:

- Declare locals at the start of a block, before statements.
- Replace compound literals with compatible initialization or static constants.
- Keep GNU inline assembly out of the IDO C path; use the project's assembly
  passthrough mechanism and mark temporary non-matching scaffolding appropriately.
- Resolve duplicate struct/typedef definitions in shared headers.
- Use the project's math declarations rather than incompatible host `<math.h>`.

## Date

Compiler experiments: 2026-01-02. Guidance reorganized: 2026-09-28.
