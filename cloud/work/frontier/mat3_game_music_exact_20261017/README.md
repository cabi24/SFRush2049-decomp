# _StartGameMusic: complete source-backed N64 candidate

`func_800D6160` at **0x800D6160** strictly reproduces all **76 words / 304
bytes**, including its 24-byte frame and owned 0.05f literal. Only this root
adds candidate credit. The narrow genuine group has thirteen strict members;
three additional locked callable leaves are independently strict at their
existing compiler flags. Every body has its complete native ELF extent.
No canonical integration, runtime or image/ROM result is claimed.

## Source mechanism

Original MAT3 DWARF identifies audio.c `_StartGameMusic(int resume)`, 272 bytes,
with the sole automatic local `int song` and a function-static `int[19]` track
song table. Its retained C uses conditional table selection, Random and the
music-volume wrapper. The source was reported 100% fuzzy by the donor lookup;
no fresh strict MAT3 acceptance receipt was supplied. Provenance:

- audio.c SHA256 e94e4a4f159c7d773c61d35069f33fbcc1477e36f9697f01d84a5923315ba3d9
- original ELF SHA256 d4a06e4516ab9a75e6de780beee10ea07b8358016e4964a8baf9e8eaa282908c

The N64 twelve-song range, retry when random selection yields song five,
nonempty resume wrapper, native external table contents and queued backend are
retained. MAT3 uses eighteen songs and an empty ResumeGameMusic; those platform
differences are not transferred. The native option view covers all 41 bytes and
the track table declares its nineteen int entries. There is no added padding
local, forced register, fabricated caller, instruction edit or empty guard in
the new root.

The old expansion assigned the table value before testing it. Restoring the
donor's conditional selection, adapted to N64's required retry loop, removes
the branch/allocation discrepancy. A control compiles the assign-first form:
it remains semantically equal in the finite model but loses strict matching.
The genuine Random calls the existing global rand body rather than a private
static copy. A second causal control restores that identical private RNG body;
the root remains exact but D6530 acquires a deleted-helper excess word. The
positive group retains both real global functions and has no unclaimed gaps.

## Complete boundary

The thirteen-member group includes the root, both actual music wrappers, the
entire existing volume group (including D6530), song writer, slot allocator,
Random and rand. The twelve older members contribute zero new bytes. The
additional locked leaves are D63EC (324 bytes), linked-list insertion 91FBC
(352 bytes), and removal 9211C (348 bytes). The direct/tail-call graph is checked
for all sixteen functions; only osRecvMesg and osSendMesg remain SDK boundaries.
There are no indirect calls. Every group member is explicitly claimed, and the
three leaf claims are explicit in closure.json. Preserved code totals 2,592 bytes.

D63EC's existing source has four genuine static helper bodies, leaving 32 bytes
of pre-entry return stubs. These are separately identified, outside its complete
324-byte function extent, and receive no credit. Its inherited compiled-out
empty-list read is disclosed unchanged. The allocator's inherited volatile
slot table and the volume wrappers' original source-line scheduling sensitivity
also remain unchanged; this packet does not authenticate those older source
choices as original N64 syntax. Existing field-access views use the established
native offsets and aligned storage. The new root introduces none of these devices.

The larger UI audio-initializer closure was independently replayed with this
root and Random: 44/49 bodies strict, with five inherited failures still explicit.
The new owned literal shifts one failing member's section diagnostics without
changing its failure boundary. That broader screen is not included or credited
here. The narrow packet preserves every member of the affected volume group
and every callable game dependency instead of hiding a failed required body.

## Proof and finite semantics

The verifier binds packet sources, compiler stages, scorer/ownership verifier,
locked provenance and all sixteen protected targets. It checks complete objects,
all extents and function inventories, the call graph, owned data, and twelve
IDO-emitted layout facts. Three wrong-layout controls are rejected by reading
emitted constants. Leaf source arguments are relative `source.c`, making the
whole-object receipts portable across checkout paths. Observation-only traced
uopt reproduces the full stock group object. Raw traces/binaries stay in ignored
build scratch.

The host differential executes the extracted actual root, Random/rand,
music-volume wrapper, volume/song writers and slot allocator. An independent
field/event model checks **86,184 fixtures**: nineteen valid tracks, signed
volume/option extremes, zero and nonzero resume values, nine RNG seeds (including
one and two consecutive rejected draws), and three pool occupancy states with
room for both commands. It compares the entire 128-slot pool, final RNG state
and ordered queue events. Queue operations are deterministic captures; no real
scheduler, SDK or concurrent mutation is modeled. The track-song table is a
controlled fixture, not a claim about every native table value.

The inherited rand C uses signed arithmetic. Host compilation explicitly uses
`-fwrapv` and `-fno-strict-aliasing` with fatal UBSan; the independent model uses
uint32_t wrap. This establishes the target's 32-bit wrap contract, not arbitrary
portable-C signed-overflow behavior. Native correspondence is established by the
separate strict instruction/data proof. Exhausted pools, invalid indices,
arbitrary callbacks, general floating-point behavior and full-game runtime are
outside the finite harness.

Four compiled semantic negatives alter the excluded song, random range, resume
time and song-loop argument. Each loses strict matching and fails the host model
with the exact differential reason. Three focused tests include full replay,
25 optimized integrity refusals and a relocated-packet replay. Python `-O` cannot
remove verification guards.

With the existing IDO/MIPS toolchain configured, run:

```
python cloud/work/frontier/mat3_game_music_exact_20261017/verify.py
python -O cloud/work/frontier/mat3_game_music_exact_20261017/verify.py
python -m pytest -q tests/cloud/test_mat3_game_music_exact_20261017.py
```

Base: 58425072b49e95f0fbe5c4c239988fb4885f7c5c. Independent review is required
before publication or candidate counting. No integration, merge or CI watcher.

## Compiler availability follow-up

Compiler fingerprints now use the scorer's normal availability check before
reading stage files. This keeps the full hash binding while reporting the
standard missing-IDO diagnostic when tools are absent. It changes no C, recipe,
object, comparison row or semantic result. A focused diagnostic test verifies
that path; ordinary local missing-tool skips remain failures when
REQUIRE_TOOLCHAIN=1.
