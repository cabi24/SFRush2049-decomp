# MaxPathControls genuine-boundary restoration: NONMATCH

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800E4B58`, `[0x800E4B58, 0x800E5434)`, 2,268 bytes / 567 words.
Research only. No matches, helper-stub identities, coverage, or ROM gates claimed.

## Reproduce

With the pinned IDO 5.3 toolchain in `IDO_DIR`, from the repository root:

    python3 cloud/work/frontier/dot_maxpath_controls_20261006/repro.py

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared`; canonical group compilation
includes `as1 -r4300_mul`.

Observed output:

    archived baseline: 528/567 words differ; emitted=533 words; frame=216; extra_nonzero=0; unresolved=0; unverified=0; errors=0
    donor candidate NONMATCH: 524/567 words differ; emitted=538 words; frame=304; extra_nonzero=0; unresolved=0; unverified=20; errors=0

The candidate now has the native 304-byte frame, and emits five more instructions,
reducing the length deficit from 34 words to 29 words.
Aligned opcode-missing rows improve 79 to 65, and relocated word-missing rows
371 to 342 (SequenceMatcher diagnostics, never a matching gate). Twenty own-data
relocation words remain unverified because the body is still broadly misaligned.
The four-word canonical improvement is not a relocated-byte equality claim.

## What changed

The arcade ancestor is `MaxPathControls` in
[game/maxpath.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/maxpath.c),
revision 3.48, copyright 1996 Atari Corporation. It calls the real operations
`MP_FindInterval`, `MP_TargetSpeed`, `MP_TargetSteerPos`, `avoid_areas`,
`AdjustSpeed`, and `AdjustSteer`. The old A7 reconstruction flattened most of
those bodies and their local scopes into the parent.

This candidate restores five genuine helper boundaries, keeping `avoid_areas`
at the native E398C call. The actual helpers are internal and called by their
real parent; no pressure callers or synthetic clobber stubs are used. The helper
names deliberately do not assign the nearby eight-byte retail stubs to guessed
identities. The exact boundaries retained by the original N64 compiler remain a
source-history hypothesis.

AdjustSpeed is rewritten from the actual donor operation, adapted to the native
sixteen-bit vehicle slot, floating throttle/brake range, difficulty-dependent
caps, and the N64 constants. Its helpers keep the actual two-argument contracts.
The unused flag inside MP_FindInterval is in the original donor; other unused
arcade locals are omitted. No array-size, padding, volatile, or fake-prototype
control is used. The root keeps only its real speed and navigation variables.
Own float literals replace this parent's historical extern-literal placeholders.

## Context and open work

`repro.py` uses the existing `codex_path_search_a7/group.c` as the genuine
E398C/E451C/E4300 context, replacing only E4B58 with the complete candidate and
its helper definitions. E4B58 is the kept ABI root; the other three functions
remain internal. Existing field/prototype hypotheses and the parent's external
`state_utility` dependency are inherited, not re-certified. The reproduction has
no dependency on the separate E398C draft PR.

The baseline already saves the same native register set; the improvement comes
from restoring source scopes, locals, and arithmetic structure. E451C still has
its old incomplete frame/allocation, placing the parent's car/navigation values
in s4/s5 rather than native s5/s6. This is the next genuine-source context to
repair. Reordering helper definitions and changing the speed-delta comparison
spelling were inert. The parent is not ready for a register-only sweep.

This is a source/compile research handoff. No independent review, behavior
harness, full tests, CI wait, production edits, or ROM acceptance was performed.
