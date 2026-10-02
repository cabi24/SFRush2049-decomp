# Complete record-score helper: strict isolated match

Base: `cabi24/SFRush2049-decomp` master
`2f1f30c508495db08d61170c4a9b2409461ae326` (2026-10-02).

## Result

- New member: `func_80098710`, `0x80098710..0x800987E8`, **54 words / 216 bytes**.
- Genuine context: the existing 13-word / 52-byte `func_800986DC` from
  `src/blob/func_800986DC.c`. It receives **zero new matching credit**.
- Both return bare `MATCH` with the unchanged canonical `tools/cloud/score.py`.
  No masks, unresolved/unverified relocations, errors, or nonzero excess words.
  The compiled unit has one ordinary zero alignment word after the new body;
  it is not part of the 216-byte claim.
- Literal recipe: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`, using the
  standard whole-program group pipeline. Both genuine functions stay in `keep`.
- No stand-ins, fabricated arguments, volatile workarounds, artificial stack
  padding, additional runtime operations, target changes, or scorer changes.

## Source and semantics

The minimal record view preserves the native signed word at +0x10, signed byte
at +0x19, unsigned byte at +0x1B, and floats at +0x20/+0x24. It computes a base
integer score from the byte and `(1 - scale) * 80`, adds eight when the flag is
set, otherwise conditionally adds the genuine wrapped-time ratio. Both original
float-to-integer conversions and the 14400.0 / 0.25 constants remain intact.

The protected complete caller is `func_80098FB8`, with two direct calls that
store the score at record+0x1C before ordered-tree insertion. An audio-priority
interpretation is plausible but unproved; names remain conservative. No direct
Rush The Rock arcade equivalent was identified. Historical `world_collision`
work-directory labels describe a suffix and are not authoritative boundaries.

The first ordinary standalone reconstruction was a nonmatch. Capturing the
unsigned byte in the real signed integer accumulator removed IDO's unsigned
conversion sequence. Compiling the genuine wrapped-time helper as inline
context recovered the native float registers. Compound addition and a normal
`if / else if` with one final return recovered operand order and branch/return
scheduling. The final source is a normal full C implementation.

## Reproduce

From the repository root, with the pinned compiler installed:

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_record_score
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_record_score --claims
```

The project setup verified IDO 5.3 static-recomp v1.2 archive SHA-256
`ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
Both prescribed setup controls passed before work: standalone
`sound_handles_clear` and the three-member `resource_slot_clear` group.
The changed-submission checker also passed for this new group. An independent
clean-directory rebuild reproduced every full resolved word of both functions;
`verification.json` records the strict counts and hashes. All 23 protected
manifest entries validate, and all tracked paths remain unchanged from HEAD.

## Additional checks

- `pytest tests/cloud tests/conveyor/test_cloud_score.py`: **837 passed,
  3 skipped, 27 subtests passed**, exit 0 (213.78 seconds).
- Initial stdlib-only unittest discovery ran 269 cases but had one import error
  because pytest was not yet installed. After installing pytest in an isolated
  workspace virtual environment, the complete applicable pytest run above passed.
- CI-style repository tests: **1299 passed, 41 skipped, 9 deselected**, exit 0
  (56.99 seconds). Command: `pytest tests/conveyor -q -o addopts='' -m 'not node_required'`.
  Pinned submodules and local test dependencies were installed, and
  workspace-local MIPS binutils put on PATH.
- Host `cc -std=c89 -pedantic -Wall -Wextra -fsyntax-only group.c`: exit 0.
- New-file patch applies cleanly with `git apply --check` in an empty directory.
- Remote HEAD remained the exact base commit at final conflict recheck.

## Limits and integration

This is a draft source contribution, not accepted cartridge coverage. The
group.json `claims` field requests strict CI rescoring only; it does not add
accepted locks or cartridge coverage. No ROM was
provided, so no source-image splice, compressed-stream identity, full-ROM SHA-1,
`make test`, or whole-repository integration claim is made. The owner must run
the ordinary independent splice/image/ROM/lock gates before promotion.

The target remains absent from current master accepted locks. Publication
preflight inspected the actual branch of research PR #9 and found an earlier
frozen O2 candidate in `cloud/work/tiny_A30/func_80098710.c`: its recorded score
is 44/54 differing words, with empty claims. The new typed/helper reconstruction
was produced independently before that archived source was found. The archive
is preserved unchanged. This group is the newly verified strict match.

Live LAN coordinator queue/ownership state was inaccessible and is not claimed
to have been checked. Recheck it before integration. No external coordinator
claim/lock, accepted source, coverage total, or protected file was changed.

Publication preflight also passes all 160 static locked functions and the
existing blob/group source-hash checks (zero problems). The blob_splice CLI
cannot run without `build/blob_layout.json`; its read-only `check()` function
was called directly for source-hash integrity only, not as a substitute for
the still-pending image and full-ROM gates.
