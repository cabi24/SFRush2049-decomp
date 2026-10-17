# ProcessSequences: genuine flow, one-word residual

Research target `func_800988D8` / `_ProcessSequences`, native 0x800988D8,
380 bytes. **Empty claims; zero new candidate or image bytes.**

The current complete `engine_sound_sync` group starts at 80/95 differing
words plus one nonzero excess word. The native-adapted MAT3 flow, its three
pointer locals, and the actual native clock predicate improve this to 21/95.
An explicitly used signed state local reduces that to **1/95**, with the
correct 380-byte extent and 24-byte frame. The only difference reverses the
source operands of a BNE at offset 0xB0. The replay checks that exact relation
without changing the compiler output or accepting the mismatch.

## Source evidence and platform boundary

The independently reviewed historical MAT3 audiomgr export records 58/58
functions matching. Its `_ProcessSequences` at 0x800A44D8 is 340 bytes;
DWARF records pointers `s`, `next`, and `t`. Source SHA256:
`a35815624388be97020f2961fa878fa0319b7ed4e4a6fa5496654e89c504b972`.
That is donor provenance, not a fresh MAT3 build or an N64 match.

The supplied sequence layout is 60 bytes, and its effect tag is 68 bytes.
The donor preserves the successor before callbacks, tests cancellation and
clock expiry, frees a stopped effect, scans four ids with a byte cursor, and
launches the next effect with an unchecked successful-tag lookup. The native
clock helper `func_80095198` has the genuine 14400-second rollover correction;
the GameCube helper does not. The native correction is retained.

The added `s32 state = t->state` is a meaningful native reconstruction choice,
not an original local claimed from DWARF. The donor-only causal control leaves
it out. There are no new unused locals, padding, artificial volatile objects,
register directives, modified Ucode, synthetic callers, or oracle values.
The root still uses the inherited integer-return lookup declaration and an
explicit pointer cast in native O32 context. The host model uses a pointer
return; it does not truncate host pointers or claim cross-platform ABI proof.

## Complete context and limits

`replay.py` materializes all three canonical engine-sound group files, replaces
only the sequence body, and adds the unchanged locked native timeout helper.
All 16 previously strict surrounding functions are required to remain strict,
with exact ELF extents. All 20 emitted functions are accounted for. The kept
maintenance owner remains 189/189 plus 25 excess words; `func_80091B00` is
11/42, and the inlined/deleted `audio_pitch_adjust` boundary is 18/18.
These are failures, not implied accepted closure.

Existing engine-sound shaping (two compiled-out comparisons and one unused
local) remains untouched context. The packet makes no new credit claim for
that inherited source, the old helpers, or either unmatched owner.

A separate complete real `entity_ai_pathfind` audio-thread context control
makes maintenance internal and restores `audio_pitch_adjust`, but the sequence
still differs by one word. That reconstructed thread remains broadly unmatched,
with an explicit jump-table own-data failure. The older thread source's queue
unlock spelling is corrected from `osJamMesg` to the observed native
`osSendMesg` destination for this control. The canonical maintenance body
already has the correct four SendMesg calls. No thread or maintenance match is
claimed. Fresh private typed BST-chain checks were also neutral; those exploratory
files are not prerequisites or acceptance evidence for this packet.

## Checks

Run from the repository root with stock IDO 5.3 and the documented toolchain:

```
python3 cloud/work/frontier/dot_mat3_sequences_research_20261017/replay.py
python3 -O cloud/work/frontier/dot_mat3_sequences_research_20261017/replay.py
pytest -q tests/cloud/test_mat3_sequences_research.py
```

The replay binds the sources, recipe, scorer, own-data verifier, every actual
IDO stage executable, and protected targets. Repeat executable and relocation
sections must agree. Native IDO-emitted layouts are checked independently;
a wrong cursor-width layout must be rejected. Historical, donor-only, wrong
state, wrong id bound, and lost-successor controls are compiled. The three
semantic mutations must fail with the precise differential exit and message.

The fatal-UBSan host comparison covers 32,768 finite three-node list fixtures
against the native-adapted donor flow. It compares sequence/tag state and
ordered service traces, including callbacks that mutate the current link.
Initial cursors are restricted to 0..3; valid list/tag storage, finite clocks,
and successful controlled playback/tag lookup are assumed. This does not prove
scheduler behavior, arbitrary aliasing, allocation failure, corrupted cursors,
concurrency, or execution of the native image. In particular, native cursor 4
would access the following word before the limit check; this harness does not
claim ISO-C validity for that out-of-array access. No ROM/image integration,
canonical source edits, or CI monitoring is performed.

## Toolchain availability correction

The compiler executable hashes now pass through the shared `score.ido` availability check. With missing IDO, local focused tests use the repository's toolchain-skip policy; `REQUIRE_TOOLCHAIN=1` still fails closed. Previously those two receipt/guard tests raised `FileNotFoundError` before the shared policy could handle the absent compiler. A focused missing-compiler diagnostic test protects this behavior. This changes no C, recipe, target, comparison, extent, model, claim or credit.
