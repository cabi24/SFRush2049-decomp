# LEAN RESEARCH: real pause-menu panel helper and initializer

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. All observations use IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`, with canonical automatic `-Wab,-r4300_mul`.

## Observed result

- `func_8010E8B4` initializer: generated seed **85/85 differing words**, 300 bytes/frame 40; typed standalone **85/85**, 304 bytes/frame 56; actual four-file context **24/85**, **340 bytes**, frame **80**.
- Native initializer: 340 bytes, frame **96**. Candidate has no excess, unresolved/unverified sites or scorer errors, but remains nonmatching.
- Newly reconstructed `sound_loop_set` UI helper: **4/83 differing words**, **332 bytes**, frame **144**, all equal to native extent/frame; no excess or unresolved/unverified sites/errors. This is still research, not a full match.
- Existing genuine pause-menu caller `physics_sym`: 303/313, 1172 versus 1252 bytes, frame 80 versus 152; unclaimed.
- Existing real font helper `slot_state_setup`: 18/58, 232-byte function, two canonical next-symbol-span excess words; unclaimed.

The historical `codex_heads_A2` packet obtained 53/85 for E8B4 using an explicitly artificial volatile eight-word stack array. That shaping is not carried forward. The new 24/85 result uses the actual direct UI callee and its other real caller, without dummy calls, pressure arrays or padding locals. The remaining 16-byte initializer frame gap is not filled speculatively.

## New complete helper and genuine source context

The historical `sound_loop_set` name is misleading: native address **800B574C** formats/measures a UI label and creates or resizes its panel. Its native signature takes no arguments. The looping-audio implementation in `src/game/game.c` does not describe this target and is not used as a donor.

`panel_helper.c` reconstructs the entire native routine, using the genuine sibling `time_result_display` body from frozen `src/blob/groups/credits_scroll_grp/gr3_c.c` as the source-structure donor. The native function differs in flag/panel globals, format-string address and the fallback text slot. Those are all changed to the actual native references: D_8011AD6C, D_8011AD68, D_80122614, and countdown-object slot 53. The formatted path retains slot 233. Width, signed half-width, top/bottom coordinates and the real five/eight-argument panel operations are preserved.

The meaningful formatted-text buffer retains the sibling's 76-byte declaration. The target passes a buffer at stack +68 within a 144-byte frame, consistent with that storage, but its original source declaration is not recovered. The explicit assumption is that formatted text fits this buffer. No capacity sweep, unused local or extra storage was introduced to force a frame.

`callback.c` reconstructs the full initializer with the actual signed-byte flags, 16-byte player-setting records, signed-halfword y narrowing and three-word panel metrics. Its existing callback argument is unused by the native body.

`pause_caller.c` reuses the complete actual other caller from frozen `cloud/work/frontier/w12h/physics_sym/best.c`. Despite its historical name, this is a pause-menu input handler. Only that body and its actual menu-exit helper are retained; its separate animation implementation is left external. The old explicitly unused c/pad/e frame-shaping locals are removed, the unsupported volatile input-byte spelling is removed, and the no-argument audio_distance_atten interface follows the real current contract. This caller is nonmatching context, not newly recovered or accepted code.

`slot_context.c` is the existing real font-selection body from frozen `src/blob/groups/credits_scroll_grp/gr3_b.c`, with normalized interfaces as in PR #215. The two actual panel-helper call sites (initializer and pause handler) keep it out of line naturally. The group keeps those real roots and internalizes the two helpers; claims remain empty.

## Assumptions and replay

Valid player index, sufficient setting/text/panel storage, valid service objects and the bounded formatted string are assumptions. Native behavior is not replaced with fallback validation. Caller/global ownership and the remaining font IPA context are unresolved.

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_pause_panel_context_20261006/replay.py --repo .
```

The baseline is frozen `cloud/work/registered-heads/seeds/func_8010E8B4/group.c`, cut before its helper body so no generated stand-in is built. The replay reads frozen tools and protected targets/manifests into temporary storage, compiles and reports exact metadata. No behavioral, acceptance or integration checks are added. Only C, flags, minimal replay and observations are published; no ROM bytes, raw assembly, binaries, locks or splice claims. Independent verification, acceptance and merging belong to the checker.
