# LEAN RESEARCH: countdown panel callback (80106874)

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Extracted GAME target: 712 bytes / 178 words.

This complete typed callback selects the countdown font, measures its text, creates a colored panel for each eligible local player, removes panels when they are no longer eligible, and updates the callback's hidden/state fields. It is reconstructed from the native accesses and generated seed, without asserting an arcade-source identity.

## Observed compilation

IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`; canonical automatic `-Wab,-r4300_mul`:

- Generated seed body standalone: **175/178 differing words**, **9 excess**, 748-byte ELF function.
- Typed complete callback with real font context: **101/178 differing words**, **1 excess**, 716-byte ELF function.
- Target: 712 bytes. Candidate target has no unresolved/unverified sites or scorer errors. It remains research, not a match.
- Typed body alone measured 158/178 with one excess; genuine context supplies the further improvement.

The baseline is frozen `cloud/work/registered-heads/seeds/func_80106874/group.c`, cut immediately before its `slot_state_setup` definition so neither that helper nor the fake stand-in is built. Its guessed scalar and byte-pointer fields are replaced with evidenced native types and strides. Player count is reloaded after calls, pointer globals remain pointers, and the inactive flag uses the native signed-byte access. Signed division-by-two is preserved for both width and height.

## Real layouts, prototypes and context

The Blit prefix has the actual hidden byte at +0x1A and callback-state word at +0x28. Player records have stride 0x4C and car selector +1; car records have stride 0x304 and signed flag +6. `D_80116198` is viewed as pairs of four-byte colors, and `D_80154350` as panel pointers. Opaque spans express observed object layouts, not local padding. The width helper uses the current accepted `s32 object_manager_update(u8 *, s16)` contract; the called argument is -1. The panel constructor and color-copy helpers preserve their real pointer arguments.

`slot_context.c` is the actual font selection body from frozen `src/blob/groups/credits_scroll_grp/gr3_b.c`, normalized as in PR #215. `replay_caller.c` is the complete actual replay setup from PR #212, unchanged. These real calls preserve the font helper out of line; no dummy caller, empty helper or artificial pressure is used. Both context functions remain unclaimed. `observed.json` records their residuals, including fourteen unverified replay owned-data sites and two owned-data mismatches with raw byte excerpts redacted. Full sound-update IPA context is still absent.

Assumptions are valid local-player counts, sufficient corresponding coordinate/color/panel table storage, in-range car indices, and valid panel/service pointers. Coordinate selection remains `(player_count-1)*4+player`; no runtime fallback outside the native valid domain is invented. Field names are descriptive rather than original-source names.

## Minimal replay

With the repository's IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_countdown_panels_20261006/replay.py --repo .
```

The replay reads the frozen scorer, baseline, protected targets and manifests from Git into temporary storage, then compiles and reports the measurements. This publication contains C, exact flags, minimal replay and observations only. No behavior/acceptance checks, locks, splice claims or ROM integration are added. Independent verification, acceptance and merging belong to the checker.
