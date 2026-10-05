# Palette interpolation match and bounded steering research

The sole new match is `func_800B0EA0`: **192 complete function bytes**, strict
0/48 words different with exact ELF size and full relocated bytes. Its complete
real palette caller supplies the genuine three-call O3 context. No source-image,
compression or ROM gate was run; no cartridge coverage is claimed.

The full palette caller `sound_bank_unload` remains **NONMATCH**: 262/312 words
different, 192 nonzero excess words and 2,048 emitted ELF bytes for a 1,248-byte
native target. Steering remains **NONMATCH**: 106/232 words different, exact
928-byte ELF extent and 104-byte frame. The caller reconstructions receive no
match credit. Thirteen accepted heap context functions and four accepted steering
context functions remain unchanged and strict, with no duplicate match credit.

## Evidence and checks

- Both packet receipts freshly reproduce byte-for-byte with the pinned toolchain.
- Six focused tests pass with no skips. Normal compiler CI replays both packets,
  including all 20 steering controls and all three palette controls, and refuses
  a false palette-parent claim and an actual alpha-bit source mutation.
- The existing changed-submission selector compiles the palette group and checks
  its sole helper claim. The unmatched parent stays explicitly reported context.
- Host C tests and ASan/UBSan pass 983,040 interpolation and 4,098 full palette
  cases, including copy boundaries, guard words, service ordering and publications.
  Service stubs do not establish those services' behavior or N64 execution.
- Independent palette review verifies the genuine caller sites, exact GNU ELF
  extent, all 192 target bytes, and 10,075 native-interpreter/host comparisons.
  Independent steering review checks the complete native root, all four actual
  caller sites, all controls and the four unchanged accepted context bodies.
- Protected-path guard and changed-submission checks pass; all 383 static locks
  remain intact. No protected source, context, target, scorer or flags changed.

**Final local aggregate: 1,926 passed, 17 failed, 41 skipped,
9 deselected, 25 subtests passed.** The failure multiset exactly
matches a fresh unchanged-`cf10b339` replay: 15 existing Conveyor local-section
relocation uncertainties and two existing Cloud descriptive-flag-header subtests.
No unrelated failure was fixed or waived. Environment/private-input skips remain;
this is not a full-suite pass.

The new worktree's initial collection/setup errors were resolved by restoring
exact pinned submodule archives and the identical ignored IDO tool directory.
The final aggregate above ran after that local setup recovery, with no source or
compiler change. Leak detection is disabled for the local ptrace environment.

See `verification.json` for source hashes, narrow claims, final aggregate counts
and failure IDs. Individual packets retain their full sanitized receipts and
independent reviews. No ROM bytes, raw assembly, generated binaries, credentials,
or unrelated private data are included. Independent checking owns image/ROM
acceptance and merging; this batch is for draft review only.
