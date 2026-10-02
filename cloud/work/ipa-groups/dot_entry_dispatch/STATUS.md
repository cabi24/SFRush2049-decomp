# Complete conditional entry dispatcher: strict O3 match

Base: `cabi24/SFRush2049-decomp` master
`2f1f30c508495db08d61170c4a9b2409461ae326` (2026-10-02).

## Result

`func_800D3430`, `0x800D3430..0x800D348C`, is a complete **23-word / 92-byte**
strict match using the unchanged canonical cloud scorer. It is the only function
in this translation unit and the only member/keep/claim. No helper implementation
or caller context is supplied.

Literal recipe: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`, through the
standard whole-program group compiler pipeline. The emitted ELF function size is
92 bytes. Ordinary zero object alignment is not part of the claim. No masks,
unresolved or unverified relocations, relocation errors, or nonzero excess words
are allowed by this result.

## Source and provenance

The source is the faithful candidate previously preserved under
`cloud/work/tiny_A24/func_800D3430.c`, reformatted into readable multiline control
flow and aligned to the existing signed 32-bit callee interface. That historical
source was an unclaimed
11-of-23-word O2 nonmatch. A control compiling the original single-line source
with the same O3 group pipeline also matches: **the build recipe is decisive**,
not the formatting. The final signed-interface control also matches in full. The
historical candidate and its research records are unchanged, including the
corresponding archive in draft PR #9 at commit
`433611408270ede7fe6acdb027f1b82179209511`.

The function has five genuine inputs. Its scalar and output-pointer signedness
matches the existing `func_800D2FA8` reconstruction; both direct callers guard
negative indices before calling. The native wrapper alone establishes widths
and register placement rather than signedness, and exact byte identity is the
matching evidence. When the fifth input is nonzero and the
entry at the first-input index has a nonzero unsigned byte at offset zero, it
writes the index and second input through the two output pointers and returns
one. Otherwise it returns the result of `func_800D2FA8` with all five inputs and
a sixth zero argument. The entry pointer comes from `D_801407FC` and the record
stride is 16 bytes. The byte array in `Entry` describes that actual stride;
it is not stack padding. Names remain conservative because the subsystem's
semantic purpose is not established by this wrapper alone.

No invented runtime operation, argument, helper, caller, assembly, volatile
access, artificial stack local, target change, or scorer change is used. There
is no new matching credit for the fallback callee.

## Reproduce

With the project-pinned IDO 5.3 static-recomp v1.2 installed:

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_entry_dispatch
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_entry_dispatch --claims
```

Expected output:

```text
Members:
func_800D3430:
  MATCH
```

The approved compiler archive is pinned by `tools/cloud/setup.sh` to SHA-256
`ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
The installed compiler is reused from the prior verified contribution; a fresh
standalone `sound_handles_clear` and three-member `resource_slot_clear` group
both pass before this work. Independent clean-directory verification records
all source/spec/compiler/scorer and compared-byte hashes in `verification.json`.

## Limits and integration

This is a draft source contribution, **not accepted cartridge coverage**. The
`claims` field requests strict changed-submission CI rescoring only. Accepted C,
locks, coverage totals, build wiring, protected retail targets and the scorer are
unchanged. The target is absent from accepted locks and other current claims at
publication preflight. Open draft PR #10 covers the distinct `func_80098710`.

No original ROM, extracted full game image, or derived `build/blob_layout.json`
is available here. Source-image splice, compressed-stream identity, full-ROM
SHA-1, and `make test` have not run. Read-only source-hash checks do not substitute
for those gates. Live LAN coordinator ownership is inaccessible and must be
rechecked by the integrator before the normal image/ROM/lock promotion gates.

## Additional validation

- Cloud/scorer suite: **840 passed, 27 subtests passed**, exit 0
  (228.27 seconds): `python -m pytest tests/cloud tests/conveyor/test_cloud_score.py`
- CI-style repository suite: **1299 passed, 41 skipped, 9 deselected**, exit 0
  (58.30 seconds):
  `python -m pytest tests/conveyor -q -o addopts='' -m 'not node_required'`
- `make check-matched`: all **160 static locked functions** intact
- Existing blob/group source-hash checks: **zero problems**, via read-only
  `blob_splice.check()` / `blob_group.check()`; this is not an image gate
- Host `cc -std=c89 -pedantic -Wall -Wextra -fsyntax-only group.c`: exit 0
- Fresh compiler sanity controls pass, and all 23 protected manifest files verify
- Changed-submission strict scorer and protected-path guard: both pass
