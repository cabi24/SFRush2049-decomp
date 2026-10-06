# LEAN RESEARCH: countdown text callback (80106B3C)

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Extracted GAME extent: 600 bytes.

The callback shows the countdown string for active local players. It chooses font 11 or 12 according to the available viewport width, selects each player's color and position, and restores the text alignment settings. This is N64-specific reconstruction from the frozen target and tracked generated seed; no arcade-source identity is asserted.

## Observed compilation

IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`; canonical scorer adds `-Wab,-r4300_mul`.

- Extracted generated-seed body, standalone: **127/150 differing words**, 0 excess; ELF function 568 bytes.
- Typed complete callback with genuine font context: **105/150 differing words**, **1 excess**; ELF function 604 bytes.
- Target has no unresolved/unverified relocation sites or scorer errors in this candidate. This remains research, not a match.
- The typed callback alone measured 111/150 with 2 excess. Context therefore contributes a further 6-word reduction.

The baseline is `cloud/work/registered-heads/seeds/func_80106B3C/group.c` at the base, cut immediately before the `slot_state_setup` definition. This removes both its helper body and fake stand-in caller; no fake caller is built in this experiment. The baseline's guessed source contains real reconstruction errors: it narrows the state mask before comparison, uses a byte instead of a word color lookup, misses player-count reloads, and clears the wrong alignment global. The candidate repairs these according to the native access widths and call ordering. The target itself compares the full `0x400000` mask with the signed hidden byte, then truncates the store; that unusual behavior is deliberately retained.

## Genuine context and assumptions

`slot_context.c` contains the existing real font-selection body from `src/blob/groups/credits_scroll_grp/gr3_b.c`, with the same normalized prototypes used in PR #215. Its second actual caller, `replay_caller.c`, is the complete `replay_save_prompt` reconstruction published in PR #212, unchanged (SHA-256 recorded in `observed.json`). The local gfx-lock/unlock/font-set wrappers are the established source abstraction seen in the credits-scroll context, not stand-in call sites.

Both context functions remain unclaimed. Their scores and limitations are preserved in `observed.json`: the replay caller has 14 unverified owned-data sites and two owned-data mismatches; raw byte excerpts are redacted. `slot_state_setup` still differs by 18/58 words. In particular, the sound-update helper's full IPA context remains absent.

The callback's Blit layout is a prefix view using the real +0x1A hidden byte and +0x28 callback state. Player input records have native stride 0x4C with the car selector at +1; car state records have stride 0x304 with the signed flag at +6. Opaque array spans represent evidenced object offsets/strides, not stack padding. The coordinate table is viewed as pairs at `D_80115FA8`, indexed by `(player_count-1)*4+player`; valid local-player counts and in-range car selectors remain runtime assumptions. Names for previously unidentified fields are descriptive rather than recovered original declarations.

## Minimal replay

With the repository's IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_countdown_callback_20261006/replay.py --repo .
```

The script reads the frozen scorer, protected target/manifests and baseline from Git into a temporary directory, builds the two measurements, and prints score metadata. The packet contains only C, group configuration, replay code and notes. It does not alter live source, locks, splice claims, or ROM integration. Independent verification, acceptance and merging are left to the checker.
