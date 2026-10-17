# Sound-system initialization: hud_setup, 1,048 bytes

Strict cloud match at **0x800C8B8C**, despite the historical HUD label. Base: master `58425072b49e95f0fbe5c4c239988fb4885f7c5c` (remote rechecked 2026-10-17). The function initializes the N64 sound pools, lists, queues and audio thread.

## Deliverable

`groups/menu_save_options/` extends the existing group with `hud_setup.c`. **All 25 members strictly MATCH**: the new 1,048-byte body and all 24 prior members. All seven original C files are byte-identical. Existing context and keep decisions are preserved; all 25 members appear in `claims`. Existing unmatched context is unchanged and not claimed. No canonical source, target, lock, override, or tooling file is edited.

Independent read-only review freshly replayed all 25 matches, checked prior source preservation and data placement, and reproduced the key negative controls. This is a cloud-verified candidate; whole-unit/image/ROM integration remains with the independent checker.

## Source evidence and disclosures

The starting source is the N64 w27a draft (32/262 differing words), not an authenticated MAT3 sound-init donor. MAT3 work supplied the useful general lesson to inspect source type and variable identity rather than copy PPC register assumptions.

1. `static s8 D_80110274 = 0` is function-local. This reproduces the native once-flag addressing and owns the existing initialized-data address. The logical flag is **one byte**; the scorer verifies a **four-byte zero-data window at 0x80110274**. No new image address, alias or synthetic storage is introduced. An uninitialized static is rejected because this address is outside game BSS; a nonzero initializer is rejected against the committed data artifact.
2. `func_800C8738((u32)D_80110264)` explicitly converts the count at its bit-scan use, while retaining the real helper's existing `s32` parameter declaration. No load, use, call or storage is added. The conversion changes IDO's parameter-coalescing/allocation decisions, producing the native count in v0 and the argument copy into a0. Without it, 24 words differ. The compiler observation is gated on complete stock/instrumented object equality; no forcing is used.
3. The inherited inferred nested list-initialization wrapper is non-static/internal (`sound_list_init`). It performs real list-flag stores and calls the genuine locked `func_800A370C` from this group. Its original identity remains unknown; it is not relabeled as the different native `func_800A44E8`. A **named, unclaimed 8-byte empty-return residue** remains and is not spliced. Making this wrapper static leaves hud_setup exact but introduces a nonzero tail after the locked list-clear body, so the complete-group check is essential.
4. Allocator and count-guard declarations are corrected to their locked source contracts. The bit-scan prototype is unchanged. There are no unused locals, fake calls, volatile additions, artificial reads or compiler/oracle patches in the source.

An additional small genuine-callee observation included unchanged `func_800C8738` and `func_80091FBC` bodies; both remained exact, as did hud_setup. Those extra copies are unnecessary in the delivered group and are not new claims.

## Fresh verification

From the repository root, with stock IDO 5.3 and MIPS binutils configured:

    python3 tools/cloud/score.py group cloud/work/frontier/mat3_sound_init_match_20261017/groups/menu_save_options
    python3 cloud/work/frontier/mat3_sound_init_match_20261017/replay.py
    python3 -m pytest -q tests/cloud/test_mat3_sound_init_match.py

The single test validates source and target hashes, then performs the optional toolchain replay. Six fresh group compiles take about three seconds here. `verification.json` binds sources and target words by hash, not mutable tools or lock files.

Controls, with the 24 prior members preserved unless stated:

- Remove count conversion: 24/262 words differ.
- Make once flag external: 8/262 words differ.
- Remove static initializer: own BSS placement rejected.
- Change initializer to one: own initialized data rejected.
- Make list helper static: new body still exact, but func_800A370C fails the extra-word rule; only 23/24 old members remain exact.

`observe.py --uopt PATH --work build/sound-init-trace` optionally reproduces the filtered allocation evidence. Raw compiler traces and objects stay in ignored build output; none are included here.

## Checker integration

Supersede the existing **menu_save_options** group in place, retaining every previous member/file. Add the new kept `hud_setup` body, preserve the existing definition preference for `func_800A370C`, and make `sound_list_init` internal. The named residue is not a new target or claimed function. Preserve previous own-data placements; install the new verified initialized-data window at its existing address. Replay the whole-unit and image gates before coverage promotion. No merge or CI watcher is requested.

## Verification hardening, 2026-10-17

The C sources, group recipe, flags, claims, native words and comparison results
are unchanged. Replay and compiler-observation guards now use explicit runtime
exceptions so optimized Python cannot remove them. Protocol 3 seals every
claimed member and all seven inherited context rows for all six builds, verifies
native/ELF extents and diagnostics, and records each complete object hash.

The stock/traced candidate and no-count-conversion objects must match byte for
byte. Their observation receipts bind the complete source/verifier inputs and
both object hashes and exact variant-source hashes; nonempty allocation decisions
are required. The normal replay seals the complete observation-file digest and
checks those bindings against fresh objects. The one-byte initialized flag, four-byte checked data window and
unclaimed eight-byte helper residue remain explicit. Zero image-byte credit is
now an explicit field. The inherited context remains unclaimed, including its
nonmatching bodies.

Focused tests include optimized-interpreter refusal controls and replay after
moving the packet to a different path. Run the same replay and observe commands
with `python3 -O` to exercise the optimized-interpreter path. No source or
matching-credit expansion accompanies this verification-only change.
