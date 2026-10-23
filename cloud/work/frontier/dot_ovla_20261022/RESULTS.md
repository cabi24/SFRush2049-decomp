# Overlay-A bounded continuation, 2026-10-22

Base: `dd4cf01d3`. **NONMATCH research only: zero new functions, zero new bytes.**
No installed source, target, target checksum, scorer, lock, or receipt was changed.
The held A820/E5DC functions and the user's excluded controlscr functions were not
worked on. No reference sources or binary/assembly dumps are included.

## A9088 / drawDeletePlayer: 15 to 10 differing words

The retained candidate is `A9088_body.c`, a continuation of the existing
`w39b/A9088/best_body.c` reconstruction. It makes two source changes:

1. After `cx = screenPos[0]`, assign the same value to the already-present `s16 w`.
2. Consume `w`, rather than implicitly narrowing `cx`, in the first
   `sound_pitch_diff_halved` call.

That call already takes an `s16`. The assignment consumes the same initialized
value; `w` is assigned again before its later width uses. No call, capacity
change, unused local, padding, volatile access, or forced register is added.
This is disclosed source shaping, not a claim about original variable spelling.

The complete native extent is still **917 words / 3,668 bytes**, with the same
**272-byte frame**. Stock scoring reports:

```text
Baseline func_803A9088:
  15/917 words differ (8 section-relative relocations unverified: .rodata+0x0 at +0x648, .rodata+0x0 at +0x64c, .rodata+0x4 at +0x650, .rodata+0x4 at +0x654, .rodata+0x8 at +0x660, .rodata+0x8 at +0x664, .rodata+0xc at +0xa3c, .rodata+0xc at +0xa44)
Candidate func_803A9088:
  10/917 words differ (8 section-relative relocations unverified: .rodata+0x0 at +0x648, .rodata+0x0 at +0x64c, .rodata+0x4 at +0x650, .rodata+0x4 at +0x654, .rodata+0x8 at +0x660, .rodata+0x8 at +0x664, .rodata+0xc at +0xa3c, .rodata+0xc at +0xa44)
```

The five corrected words are at offsets 0x124, 0x280, 0x550, 0x96c and 0x9f4:
the narrow-center-x spill home changes from stack offset 72 to native offset 56.
The ten remaining differences are only stack homes:

- Button-loop pointer: 52 instead of 48, at 0x6f0, 0x6f4, 0x73c, 0x75c,
  0x790 and 0xe08.
- Selected-profile address: 48 instead of 52, at 0xd3c, 0xd48, 0xd54 and 0xd58.

### Compiler evidence and unsuccessful continuation

Observation-only tracing used the cached official IDO 5.3 recompiler's uopt
instrumentation documented in `cloud/work/frontier/tools/trace/README.md`.
For both the baseline and the candidate, the traced `.opt` output was compared
byte-for-byte with the stock `.opt` output and was identical. Acceptance scoring
used the stock compiler separately, without trace patches or force controls.

The CFE local area stays 196 bytes. Uopt still allocates eight four-byte temp
homes, finishing at 228 bytes. In the baseline, the narrow-cx expression is
numbered before narrow-y and takes temp 0. In the candidate, narrow-y takes
temp 0 first; narrow-cx is encountered later and takes temp 4. Thus the gain
comes from expression order, not growing a frame or changing a buffer.

Targeted probes for the remaining pointer-home pair did not improve ten words:

- A selected-profile read before the main loop preserves 10 but changes hidden
  expression-home assignments. A function-entry initializer gives 28 and moves
  the earlier homes too. Neither is retained.
- Explicit early byte-scaled label indices and alternate slot-address arithmetic
  regress to 83 or more differences.
- Equivalent narrow-cx copies via the other existing short also give 10;
  swapping x/y initialization gives 14 and copying the initial y gives 15.
- Empty branch holders alter control flow or regress; none is retained.

The remaining task is to recover the authentic pointer-expression lifetime or
ordering that gives the native 48/52 homes without disturbing the earlier homes.
The old blanket claim that this necessarily needs an additional storage slot is
not established by these measurements.

### Packet boundaries and proof limits

`make_packet.py` replaces only the A9088 definition in a scratch copy of the
installed `runtime_a_profile_menu_20261006` group. It copies the manifest
unchanged: A9088 remains informational NONMATCH **context**, not a member or
claim. Every byte of the source prefix and suffix is retained.

Independent replay also found all other compiled function slices unchanged after
relocation. The full object text has the same size and differs in exactly the
five improved A9088 words. The inherited member score lines are unchanged:

```text
func_80395E24: MATCH
func_803A87D4: MATCH
func_803A8B74: MATCH
func_803A7B58: MATCH (8 section-relative relocations unverified)
```

The last line is the scorer's word-match description, **not** a fully verified
match in this environment. Both canonical group CLI runs exit **1**, because
that inherited member's owned data cannot be verified. There is no strict
group-score-zero result. The other context is unchanged: A9EEC is 281/315;
96FA0 is 717/718 plus 39 extra words; 96EE8/96F44 word-match. Those existing
nonmatches are not new claims.

This cloud checkout has no image-A owned-data region. The eight unverified
A9088 references cover four literal locations; they cannot be proven merely
from the code targets. No image or ROM hash gate was run. This candidate cannot
be promoted to a match even if its ten instruction differences are later fixed
until the owned-data and normal installation gates are satisfied.

### Reproduce with the canonical scorer

Use the normal `CloudHandoff.md` toolchain environment, including `IDO_DIR` if
the stock compiler is in a shared cache. From the repository root:

```sh
python3 tools/cloud/score.py group \
  src/ovl_a/groups/runtime_a_profile_menu_20261006 --targets asm/us/ovl_a
python3 cloud/work/frontier/dot_ovla_20261022/make_packet.py \
  build/dot_ovla_replay
python3 tools/cloud/score.py group build/dot_ovla_replay --targets asm/us/ovl_a
```

The output directory must be new; the generator will not overwrite existing
work. Read the A9088 **context score**, not just the group exit code. No
`--allow-unverified` option is used.

## A400 / updateWarningScreen: no retained improvement

The donor-backed `w39b/A400/dc2la.c` remains the better structural baseline.
Canonical command:

```sh
python3 tools/cloud/score.py fn cloud/work/frontier/w39b/A400/dc2la.c \
  func_8038A400 --flags '-g0 -O3 -mips2 -G 0 -non_shared' --targets asm/us/ovl_a
```

It still reports **118/139 words differ**, with 24 unverified section-relative
references and a body two words shorter than the native 556-byte extent.
The prior aligned diagnostic's ten non-relocation rows are not a match score.
The controller-scan conditional/break lowering is still the principal structural
residual; group compilation produces the same result as standalone compilation.

A postincrement/decrement scan form reaches 32/139 by restoring body length,
but changes additional loop computations and register choices. That lower
positional count is **not** retained as an improved reconstruction. Bounded
switch, nested-break, empty-block, pointer/loop and goto forms did not recover
the native branch topology. No new helper is introduced: the established
warningscr layout provides no corresponding deleted-inline stub.

The initial pre-guidance A44FC scheduling diagnostics are preserved only in
ignored scratch storage; they produced no improvement or source change.
