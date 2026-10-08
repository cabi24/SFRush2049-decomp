# Voice-unblock production contract

This source-only adaptation removes the reproduced production-TU compile refusal
for `func_8001F954` without changing its 124 native bytes. It earns **zero new
matching bytes and zero accepted bytes**: PR #145 already supplied the standalone
match. Production relocking, promotion and cartridge verification remain with the
independent integration owner.

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.

## What changes

The new [matching group](../../../matches/voice_unblock_production/group.json)
has one source and one claim, `func_8001F954`. No caller, inlined helper or
deleted-static stub is required in the unit. Every helper remains a declaration
of its genuine existing interface. The O2 historical source is read from BASE,
so its verified body and recipe remain reproducible even when integration
updates the live legacy header. This adaptation changes no lock or legacy body.

The adapted source repeats the base TU's exact `SequenceNode` and `VoiceState`
declarations, allowing the ordinary promotion declaration deduplicator to reuse
the accepted records. The function body changes only two field names:
`identifier` becomes `identifier60` at +0x60; `valueBD` becomes `activeBD` at
+0xBD. The VoiceState stride remains 0x1A0. Its behavior and indexed-expression
topology are unchanged.

There is also a declaration-scope issue in the default whole-file deduplicator:
an anonymous-parameter `func_8001F6EC(VoiceState *)` declaration already occurs
*after* this slot, so it would be dropped even though not yet visible. The new
source gives that existing formal the natural name `state`. Its compatible
prototype remains before the new body; the later accepted declaration stays
unchanged. No deduplicator, gate, header or accepted-body edit is needed.

The verifier reproduces both failures in scratch TUs: the historical conflicting
VoiceState, then the later-only prototype failure after fixing only the type.
The adapted source passes the normal default whole-file deduplication path.

## Fresh evidence

- Direct IDO compile under exactly `-g0 -O3 -mips2 -G 0 -non_shared`: strict
  31/31 native words; 124-byte ELF function and four zero alignment bytes.
- Standard scorer O3 group pipeline: the same strict result and sole claim.
  The receipt records the actual delegated backend invocations, including
  `umerge/uopt/as1 -Olimit 5000`, backend `-mips2 -EB -g0 -O3`,
  `uopt/ugen/as1 -G 0` and `as1 -r4300_mul`. The direct exact-header
  compile above is separate and adds no implicit scorer erratum flag. The
  command observer passes every stock scorer invocation through unchanged.
- New source under the actual O2 production recipe and the historical BASE O2 source:
  the same complete native result. The exact O3 claim is fresh evidence, not a
  relabeling of the historical O2 receipt.
- Independent GNU linking and readelf agree on full text, address, function
  extent, relocations, padding and absence of owned data for all standalone
  variants and the new group.
- Real asm-processor compilation of the complete base and scratch-adapted
  `lib_1f5b0` TUs: all 18 native slots remain strict-equal. All 14 pre-existing C
  bodies remain textually unchanged. The target becomes C; the other three
  passthroughs remain unchanged assembly. Their preserved bytes earn no C credit.
- The actual existing caller `8001C7F4` and accepted helper `80014AF0` are loaded
  from the base, tied to their real production bodies and freshly compiled with
  their accepted headers. Both still match and independently GNU-link exactly.
- IDO checks pointer width, SequenceNode size, VoiceState stride and all named
  native record offsets. The host test does not assume host pointer width equals
  the N64 ABI.
- The base's bounded instruction interpreter runs 2,112 native and 2,112 freshly
  linked cases with all 31 instructions covered. The actual adapted C runs 2,112
  additional C89 cases under ASan/UBSan/bounds and strict aliasing. Side-effecting
  helper hooks check call order, arguments and the complete voice array. Three
  C-body mutants and three instruction mutants are rejected.

The target is MusyX `synthvoice.c:voiceUnblock`, an N64 middleware reconstruction,
with no arcade equivalent. The inherited donor provenance is
[AxioDL/musyx 78d2e16](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthvoice.c#L733),
[CC0](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE).
The native record contract remains authoritative.

## Portable replay

Run from any directory, supplying an absolute verifier path if needed:

```sh
python3 cloud/work/boot_tail_promotion/voice_unblock_contract/verify.py --check
python3 -m pytest tests/cloud/test_dot_voice_unblock_contract.py -q
python3 tools/cloud/score.py group cloud/matches/voice_unblock_production \
  --claims --targets asm/us/boot_tail
```

Production TU, headers, historical source, asm-processor and original boundary
proof are materialized using `git show BASE:path`. BASE must be available in Git
history. The current scorer verifies the current native target manifest normally;
this packet does not pin that manifest or the scorer. The receipt binds only the
new source, group declaration, verifier, C behavioral proof source `host_test.c`,
native target words and proof invariants. Editable pytest wrappers are not
hashed. It includes no live lock assertions or production-file hash pins.
Observed backend commands are retained as recipe metadata, not compared as a
pin on the evolving scorer; fresh replay comparisons use the proof invariants.
Later integration changes therefore do not invalidate this packet's base-context
proof. Tests and the verifier skip cleanly without pinned IDO or the
MIPS GNU linker; the full replay also needs GNU MIPS as/objcopy/objdump/readelf and
a host C compiler with sanitizers.

The current production segment uses O2. Both flagsets are separately proven;
nothing here changes the segment flags or treats an O3 source-header claim as a
production lock. The maintainer must select this adapted source, refresh its
normal context and O2 score-zero evidence, then run the unchanged real promotion
transaction and full-ROM gate. No override or force-promotion is proposed.

## Limits and review

The whole-TU check is compilation and complete relocated native equality in a
scratch copy. No accepted source, protected target, lock, context gate or ROM
input is written. Private cartridge, full image/compression, gameplay/hardware
and hosted CI gates were unavailable and are not claimed. Bounded behavior covers
valid slots 0..31 and the FFFFFFFF sentinel with explicit helper-boundary hooks;
it does not execute the two nonmatching helper implementations or establish
arbitrary-index safety.

The focused tests include source/type mutants, receipt invariants, optimized
Python refusal, missing-toolchain skips and a full foreign-working-directory
replay. Full current-master `tests/conveyor tests/cloud` runs, with and without
IDO, are coordinated separately before publication. Independent source/evidence
review is required before opening the draft PR. Merging belongs to the user's
independent checker.
