# Complete run decoder: func_800ADCE0

Draft source contribution only. **120 native bytes / 30 instructions**, from
0x800ADCE0 through 0x800ADD57, strictly match the protected target. No cartridge
coverage is added, and no accepted source, lock, target or build wiring changes.

## Source and behavior

The four-input leaf expands big-endian packed words into halfwords. Each word's
upper three bits encode run length minus one; its lower thirteen bits encode
the initial value. The remaining output count falls by the entire run length
once, before its outputs. Values increment after every write. Each current
value equal to the full-word marker fills the output from the front; all
others fill it from the back. Comparison is repeated for every incremented
value, rather than once per packed word.

The complete native body and all four direct call-site argument setups were
reviewed. `input_deadzone_apply` and `camera_trigger_check` pass marker -1;
`camera_victory` passes zero; `entity_update` passes an unsigned halfword.
Each caller loads the count from one byte and passes the remaining input bytes
and a halfword-aligned stack output span. No direct arcade equivalent is
established: the primary arcade reference tree is absent. Existing names are
retained without asserting their historical semantic labels are correct.

The source retains native pointer movement, including forming
`back = front + count - 1` before the count guard and postdecrementing the back
cursor after a write. C execution therefore requires a surrounding array that
contains every formed cursor, including before the selected output span and
for nonpositive counts. This is **not** a claim of arbitrary pointer safety,
malformed-input validation, or a proof of every caller's output capacity. Host
tests use an interior output span with valid surrounding guard elements.

## Reconstruction and provenance

The frozen `cloud/work/tiny_A24/func_800ADCE0.c` reproduces a 29/30-word O2
nonmatch with one nonzero excess instruction. Its count decrement is inside
the inner loop, contrary to the target's once-per-run count reduction.
`hand_partial_A/func_800ADCE0.partial.c` already documents the correct count
reduction but remains a historical nonmatch. Both artifacts stay unchanged.

The final natural loop reuses its actual decoded value as it is masked and
incremented, instead of keeping a separate encoded-word carrier. It matches
through the ordinary whole-program O3 pipeline as the only defined function.
The unsigned full-word marker preserves the exact native comparison order.
There is no compiled helper/caller context, fake operation or argument,
volatile access, uninitialized local, stack-padding object, forced register,
assembly injection, changed target, mask, or compiler modification.

## Reproduce

`python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_run_decode --claims`

```text
Members:
func_800ADCE0:
  MATCH
```

The approved pinned IDO 5.3 static-recomp v1.2 installation was reused.
`verification.json` records source/spec/compiler hashes and sanitized proof.

## Verification

- Canonical strict scorer and independent clean-directory replay: 30/30 full
  words exact; no unresolved/unverified relocations, masks, or errors
- Additional direct comparison of all 30 raw compiled words proves equality
  without relocation processing; this function has no text relocations
- ELF symbol size 120 bytes; two trailing zero alignment words are excluded
- Separate reviewer copied and compiled the final source/spec, confirming the
  full body, four-input ABI and pointer-domain limitation
- 302,147 ASan/UBSan host semantic cases pass: all 65,536 packed words with
  four marker cases, 10,000 valid multirun streams with four markers each,
  and three nonpositive counts in the valid pointer domain
- Complete output/guard arrays and input bytes compared with an independent
  index-based reference; C89 syntax/warnings pass
- Cloud/scorer tests: 840 passed, 27 subtests passed, exit 0
- CI-style repository tests: 1,299 passed, 41 skipped, 9 deselected, exit 0
- All 160 static locks intact; blob/group source hashes have zero problems;
  all 23 protected manifest files verify
- Base master: `2f1f30c508495db08d61170c4a9b2409461ae326`; existing accepted
  names/aliases and matching PRs #10–16 excluded

Host tests do not execute the N64 binary. Leak detection is disabled because
sandbox ptrace does not support it; the harness uses no dynamic allocation.

## Integration gates still required

The original ROM, complete extracted image and derived `build/blob_layout.json`
are unavailable here. Source-image splicing, compressed-stream byte identity,
full-ROM SHA-1 and `make test` have **not run**. The integrator must recheck live
LAN ownership and run the normal image, ROM and lock gates before acceptance.
This PR is a draft source contribution; its local `claims` field only requests
strict changed-submission CI rescoring.
