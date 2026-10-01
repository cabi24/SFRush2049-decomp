# A13: real physics-response closure — NONMATCH

Frozen 2026-10-01. Empty claims, no new coverage. Current shared root locks confirm physics_response and audio_effect_apply unlocked; func_800B9338 is already accepted and stays context only. Three real bodies total432 retail words: physics230, actual sole caller174, real index helper28. Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`. No dummy formal, wrapper, pressure-only buffer, synthetic caller, register binding or added runtime read.

Final strict result physics **225/230 differing words**, emitted234, four extras, no unresolved/unverified relocations or errors. Caller170/174, emitted190, sixteen extras; accepted index helper remains MATCH28/28. Canonical --claims exits0 explicitly as work in progress because claims are empty, not as an acceptance verdict. Full results are in scores.json and score_claims.txt.

## Real closure and source repairs

Retail physics800B93A8 has a64-byte frame and saves only ra while writing s0–s6 and f20/f22/f24. Actual caller audio_effect_apply800BAAA0 has an80-byte frame and saves those registers, with calls at800BAADC,800BAAFC and800BAD1C. The delivered real caller makes IDO move preservation into the caller without a stand-in: physics uses a24-byte frame, saving ra, while the caller saves its real derived clobber set. This reproduces the whole-program mechanism, but not the retail allocation. Compiler additionally uses s7/s8/f26; caller frame128 and extra preservation differ from retail.

Physics uses fixed metadata base D_80151CE8, signed16 primary index at6, secondary at4, wrap index at2 and terminal index at8. Its second outer pass must reload the primary index from base+6, proven by delay slot800B9680; B15 seed instead retained the previous segment's secondary index. That is corrected. Post-segment termination reloads secondary from base+4; the final average uses the already loaded secondary carrier as retail does, rather than introducing a fresh global reload. Record metadata uses stride80 and point-start index at48. Geometry uses D_8012E5EC eight-byte records: signed16 XYZ at0/2/4 and unsigned speed byte6. Iteration counters remain32-bit until the genuine signed16 func_800B9338 interface, matching the retail full-width increment and explicit narrowing around its three calls.

Unsigned speed bytes are held in u32 and converted naturally to float, preserving the actual unsigned conversion mechanism. The seed's manually injected sign-test branch is removed: its cast already handles unsigned conversion, and byte values cannot be negative. sqrtf is correctly declared float before its intrinsic pragma. The real helper definition was extracted unchanged from accepted src/blob/func_800B9338.c and remains strict MATCH.

## Caller stream audit

Caller was freshly decompiled from its own authoritative retail instructions, then repaired against every packed-stream advance/store. Mode flag D_80114650 selects a genuine fast path: physics, audio_mixer_main(-1), physics_friction_apply, func_800B9740, physics. Normal path loads D_80153F20+812, copies the actual16-byte header, advances by16 then header byte8 times16, copies a two-byte count, and advances by2 plus header halfword0 times6. The seed wrongly used byte8 for this last advance; retail800BAB84 reads the halfword at header0.

A private typed header represents exact offsets: pointCount0, points4, sections8, recordCount10, records12, total16. The points pointer at4 is always assigned from D_801409E8 (retail800BABFC), while positive section count sets records12 from D_80153E80; otherwise records12 and D_80153E80 become zero. The initial seed incorrectly made the offset4 assignment conditional and cleared it in the zero-count arm. Source now preserves the two distinct fields and their actual sequencing. Pointer globals and stream carriers are typed as byte pointers; no shared declarations were edited.

For each16-byte section record, caller writes its packed substream pointer at12 and advances the stream by unsigned16 count at10 times6. It then loads four genuine eight-byte point descriptors into D_8012E5E8 plus section*8, writes unsigned counts to D_801527D0 plus section*2, writes record pointers to D_80153EF8 plus section*4 and descriptor+4, and advances by count*8. Retail empty signed16 counter loops are retained as actual source context; no fabricated loop was added. Normal path ends with audio_mixer_main(-1), physics_friction_apply, func_800B9740, physics.

## Bounded measurements and blocker

Fourteen compiling controls: nine combinations of authentic ABI keep choice, word-sized iteration carriers and fixed-base accesses; four natural unsigned-conversion/volatile-temporary controls; one typed stream/header repair. Baseline after primary-index repair emitted253 words; truthful word carriers reduce to240, natural unsigned conversion to234. Some alternatives reported unpaired HI16 and are recorded as failed verification, never claims. Volatile temporary controls preserve real vector stores but add loads/observable qualification and worsen emitted size; they are discarded. No volatile declaration is delivered.

Retail stores its genuine delta temporary at24/28/32 even before its inline square-root expression. Compiler removes those first-loop dead stores and retains the later deltas in additional floating registers across its real helper calls. Corresponding24-byte compiler frame differs from retail64. Fixing that with invented locals or clobbering helpers would be unjustified. No concrete new real inline helper was identified, so the packet stops after source/prototype repairs rather than a color/reflow sweep. Full raw object/disassembly remains ignored build/codex-A13 and private Rocky A scratch.

Reproduce:

```sh
rsync -a cloud/work/ipa-groups/codex_physics_a13/ Rocky:agents/A/wt/cloud/work/ipa-groups/codex_physics_a13/
ssh Rocky 'cd ~/agents/A/wt && python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_physics_a13 --claims'
ssh Rocky 'cd ~/agents/A/wt && python3 cloud/work/tools/amatch/builder.py group cloud/work/ipa-groups/codex_physics_a13 --json'
```

Frozen group.c SHA256: `6efc2714b46b4a034b7dc837fd015a445cb4401c0d441c8c34b571b22757874f`.
