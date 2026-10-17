# Audio-options teardown: exact-function research closure

Base `58425072b49e95f0fbe5c4c239988fb4885f7c5c`, 2026-10-17.
Research only. Empty claims, no complete-group acceptance, no image or coverage credit.

## Result and boundary

The native `credits_screen` at 0x800D6698 is the audio-options teardown,
corresponding to MAT3 `audioscr.c::uninitScreen`. The complete native function
matches strictly: 74/74 words, 296 bytes, a 24-byte frame, no extra words,
unresolved references, owned-data refusals, or comparison errors. Its genuine
native caller `func_800D7634` remains exact. The retained song-command helper
and both renderer removal wrappers also match their own native bodies.

All 19 known native bodies are explicit members, with no hidden context:

- Seventeen strict function comparisons are accepted, including the new cleanup.
- `func_800D6914`: zero code-word differences, but twelve unverified references
  and an owned-data refusal. Its strings and floats imply incompatible image
  bases for one object section. This is not an accepted match.
- `func_800D6160`: 65/76 differing words. This inherited body remains nonmatching.

Those two failures prevent installation or an accepted complete-group claim.
The 296 bytes are an exact function candidate inside this research closure,
not newly accepted image bytes. The existing locked groups are untouched.
`verification.json` and every mutation receipt preserve all 19 outcomes.

## Authentic source and native adaptation

The saved MAT3 audio-screen source is recorded Matching at 12/12 functions,
6,772 bytes. Its source SHA-256 is
`decd753ae39fa5fbdcbf9a0c41f8a07cd79bba921528c998c5d2c7762e0f0ae5`.
This is a retrieved historical reconstruction, not a fresh MAT3 build or
independent proof of N64 compiler boundaries. Original DWARF gives no locals
for `uninitScreen` (0x80066254, 124 bytes; DIE 0x13E6E1) and only
`int buttonIdx` for `uninitButtons` (0x80067094, 116 bytes; DIE 0x13F143).

The root preserves unconditional `AudKillSong`, the initialized guard,
the twelve-button cleanup, masked pane removal, pointer-valued blit removal,
background removal, and the two final flag stores. Button records are 64
bytes, with the signed-int object identifier at +4; both original types and
the native code support the referenced layout. The renderer argument is
signed-short even though the stored identifier is an int. Unused record
fields describe existing global layout; they are not local frame fillers.

The original renderer chain is `ZOID_RemoveObject(s16)` to
`ZOID_RemoveObjectTree(s16, int)` to `_RemoveObjectTree(s16, int, int)`.
Here these are the repository's legacy names `sound_call_minimal`,
`sound_call_simple`, and `entity_spawn_callback`. The first two are emitted
and checked native members. `sound_handles_clear`, `sound_stop`, and
`ambient_sounds_clear` are the repository's legacy names for RemovePanes,
RemoveBlit, and removeBG; their bodies are not copied into this closure.

The song helper keeps the authentic single command-pointer local. It uses
the native N64 lock, 24-byte slot allocator, command-byte store, unlock, and
queued submission. The MAT3 synchronous backend and its `gNoAudio` guard
are not transplanted. The source-only `ui_ProcessCommand` boundary models
that native queue submission; it does not claim a recovered original N64
function name or a separate surviving entry. Likewise `ui_uninitButtons`
is an evidence-backed fully inlined source boundary, not a new native symbol.

Restoring the two real source boundaries removes the old four-word scheduling
residual without adding root locals or storage. The native allocator,
lock/unlock bodies, and genuine caller remain in the closure. All slot-return
prototypes use the same Slot18 pointer type; queues, pane/blit globals,
button records, and speed-set prototypes are consistent across translation
units. The generated, unused declarations around sync_entry_register were
removed; its native body and exact result remain. Ordinary multiline song
source also preserves the retained native helper's own scheduling.

## Inherited limitations

This is deliberately not a new source-fidelity certificate for the whole
older audio_options_menu reconstruction. The retained source still discloses:

- The compiled-out block ending and unused `padB` in func_800D7634.
- Unused frame locals in func_800D6914, whose data ownership already refuses.
- The hypothesized constant-one func_800D690C and the two post-message copies.
- The allocator's inherited volatile-table scheduling treatment.

None is newly introduced or concealed by this result. No padding, unused
local, forced register, fake caller, address cast, or unsupported helper was
added to the new cleanup. The small physical-line scheduling sensitivity in
the existing audio writers is preserved and disclosed, not treated as proof
of the original source's spelling.

## Replay

Run `python3 cloud/work/frontier/mat3_ui_audio_cleanup_20261017/verify.py`
with the repository's existing IDO toolchain configured. The verifier pins all
source inputs, the native target digests, and the complete member receipt;
its explicit exceptions remain active under `python3 -O`. Both replay outputs
are byte-identical. Three fast unittest guard checks pass; rerun them with
`python3 -m unittest discover -s tests/cloud -p test_mat3_audio_cleanup_research.py`.

Four current-source mutations are rejected: wrong button count, wrong audio
command, wrong state mask, and wrong button-field offset. Every mutation
recompiles and preserves a complete 19-member receipt. The target must pass
the strict scorer, not a masked link-only or shifted-alignment comparison.

The entire stock compiler object and its uopt/ugen/as1 traced object are
byte-identical; `fidelity.json` records their shared fingerprint and trace settings. Only stock comparisons establish the result. Objects, ROM
bytes, raw assembly, and trace dumps are not included. No canonical source
installation, full integration build, ROM gate, or CI watcher is claimed.
