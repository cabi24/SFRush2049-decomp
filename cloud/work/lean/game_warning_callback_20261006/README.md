# LEAN RESEARCH: per-player warning callback (80108AB0)

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Extracted GAME target: 760 bytes / 190 words.

This complete typed callback updates each player's warning flag/timer and draws the warning text with an offset shadow after two seconds outside the accepted condition. It preserves the early mode-5 return, state/alignment setup, native short-circuit condition, font choice and final rendering resets. This is native-backed reconstruction; descriptive field names are not asserted to be recovered original names.

## Observed compilation

IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`; canonical automatic `-Wab,-r4300_mul`:

- Extracted generated-seed baseline: **187/190 differing words**, **5 excess**, 780-byte function, frame 112, two unverified owned-data sites.
- Complete typed callback standalone: **182/190 differing words**, **zero excess**, **760-byte function**, frame **80**. No unresolved/unverified sites or scorer errors.
- Genuine three-file font-context control: 183/190, six excess, 784-byte function, frame 160.
- Native: 760-byte function and **200-byte frame**. Correct standalone byte length is not a match. The frame and private font calling convention remain unresolved.

The standalone result is the retained improvement; genuine context did not improve it. All three measurements are reproduced and included in `observed.json`. No padding locals are added to fill any frame gap.

## Source corrections and actual interfaces

Baseline: frozen `cloud/work/registered-heads/seeds/func_80108AB0/group.c`, cut before its helper definition to remove the generated helper and fake stand-in. The seed incorrectly rewrites the advancing player pointer to one global byte, gives `func_800ED66C` an integer/two-argument interface, and converts an integer bit-pattern to a large float for the final render reset. The candidate uses the actual player-relative signed byte, true single-float helper prototypes from accepted `src/blob/func_800ED66C.c` and `src/blob/render_helper.c`, and the real `-1.0f` reset values.

The player record is an evidenced 952-byte layout with signed flag at +0xEF and warning metric at +0x364. The field called `direction` is a descriptive interpretation only. The timer and warning arrays advance by four and one bytes respectively. Coordinate pairs use `(player_count-1)*4+player`, and text comes from countdown-object pointer slot 26. The `!(elapsed < 2.0f)` expression retains the native comparison form rather than silently converting it to `>=`. The existing callback parameter is unused by the native body; no synthetic argument is introduced. All opaque spans express global object layout, not unused stack storage.

`slot_context.c` is the real font helper from the frozen credits-scroll context; `countdown_caller.c` is the complete actual countdown text callback from corrected PR #220, unchanged. The group exists only as a genuine-context control, with empty claims. Context scores and identities are included; neither helper nor caller is newly claimed. Full sound-update context and the native 200-byte source-frame declaration remain unknown.

Assumptions: valid local-player count and matching array extents, in-range coordinate selection, valid text and rendering state. The packet does not invent a frame buffer, async clock contract or fallback behavior for invalid runtime state.

## Minimal replay

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_warning_callback_20261006/replay.py --repo .
```

The replay reads the frozen scorer, baseline and protected target/manifests from Git into temporary storage, compiles and reports metadata. No behavior or acceptance checks, locks, splice claims or image/ROM integration are included. Independent verification, acceptance and merging remain with the checker.
