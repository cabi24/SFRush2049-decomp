# Graphics and row-rotation matching batch

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.

This bounded batch supplies two strict source/object matches totaling **552 bytes**
and preserves three separately labeled nonmatching research results. It does not
splice production source or claim linked-image or cartridge coverage.

## New strict matches

- [`sound_init`](../dot_graphics_init_match/README.md): complete **400-byte**
  graphics initializer, **100/100 full relocated words equal**, all **37**
  relocation sites resolved. It uses the genuine unchanged `gfx_modes` sources
  and the genuine unchanged `object_render` storage owner. The ordinary
  no-argument ABI is preserved.
- [`func_800C40E8`](../frontier/dot_matrix_row02/README.md): complete **152-byte**
  row-0/2 matrix rotation, **38/38 full words equal**, ordinary O3 and O2 builds.
  The native three-input ABI needs no artificial caller or register-pressure
  parameter. Its two actual callers were identified but are not claimed.

Five accepted strict graphics functions and both accepted pad-context functions
remain exact at their real ELF extents. The graphics mode helper retains its
**two pre-existing unverified local-table placements**. Its masked body and table
sections are unchanged. The batch makes no strict whole-group claim for that
helper and gives no new credit to existing matches.

## Bounded research, not matches

- [`func_800A150C`](../encoded_string_mapping/STATUS.md): exact **312-byte**
  extent, **1/78** differing words, all four relocations resolved. The remaining
  instruction reverses the operands of a symmetric equality branch. The source
  preserves the original zero-limit quirk. No match is claimed.
- [`audio_channel_reset`](../dot_pad_channel_reset_20261005/RESULTS.md): exact
  **164-byte** extent, **15/41** differing words in genuine pad context. This
  supersedes the earlier historical 168-byte candidate's misleading extent.
- [`state_update_global`](../dot_state_update_context_20261005/RESULTS.md): exact
  **112-byte** extent, **3/28** differing words. The actual accepted pad callees
  do not improve the known allocation residual. This is useful negative evidence,
  not a reason to add artificial pressure or repeat blind syntax searches.

## Verification

The batch runs all four packet verifiers and the matrix verifier from the final
source tree. All five fresh sanitized receipts reproduce their saved versions
byte-for-byte. The strict graphics `--claims` command passes, and the normal
changed-submission checker scores the matrix source as MATCH.

**21 focused tests pass** without skips. Host checks include 5,142 graphics
initialization cases plus the full-state no-op; 4,102 finite matrix cases;
1,015 encoded-string cases; 1,192 pad cases with repeat calls; and 64 state
callback cases. The graphics harness additionally passes ASan/UBSan with leak
detection disabled because the local ptrace executor does not support it.
Host tests are semantic evidence; complete native object comparison establishes
matching.

The graphics packet lies outside the generic changed-submission directory.
`test_recompile_genuine_group_and_accepted_baselines` explicitly recompiles its
real source group, all accepted baseline recipes, and both causal controls in
the normal cloud test suite. It compares full records, including unresolved-table
limits. The existing `REQUIRE_TOOLCHAIN=1` CI policy makes a missing compiler a
failure rather than a silent passing skip. The matrix submission is also covered
by the existing changed-submission check and its own independent replay test.

Protected-path checks and `make check-matched` pass: **383 static locks intact**.
No target, context, symbol, scorer, compiler flag, production source, or lock is
changed. Independent source reviews pass for both matching packets and all three
research targets. Packet receipts and the batch verification record bind the
reviewed source hashes.

The full local aggregate result and its comparison with the unchanged baseline
are recorded in `verification.json`. The known baseline is not repaired or waived:
15 Conveyor strictness failures concern unresolved local-section placements in
existing accepted singles; two Cloud builder subtests concern existing descriptive
flag headers. Exact remote CI status is reported in the pull request body after
publication; local checks are not presented as a remote CI pass.

## Reproduce

Initialize the repository's pinned submodules and IDO toolchain, then run:

```sh
python cloud/work/dot_graphics_init_match/verify.py --output build/graphics.json
python tools/cloud/score.py group build/dot_graphics_init_match/final --claims
python cloud/work/frontier/dot_matrix_row02/verify.py
python cloud/work/encoded_string_mapping/verify.py --build-dir build/encoded --output build/encoded.json
python cloud/work/dot_pad_channel_reset_20261005/verify.py
python cloud/work/dot_state_update_context_20261005/verify.py
REQUIRE_TOOLCHAIN=1 python -m pytest tests/conveyor tests/cloud -q -rs -m 'not node_required'
```

No ROM bytes, raw assembly, generated binaries, credentials, or unrelated data
are included. No remote builder is used. The independent checker retains the
image/splice/compression/full-ROM hash gates and the merge decision.
