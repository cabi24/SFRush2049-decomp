# LEAN RESEARCH: menu rectangle-text helper (800DC88C)

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. The target historically named `attract_mode_handler` is 264 bytes / 66 words, native frame 40. This is a first complete C body for its actual menu-rectangle text behavior, with its genuine existing controller caller.

## Observed result

IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`; canonical automatic `-Wab,-r4300_mul`:

- Same complete C standalone: **65/66 differing words**, **8 excess**, 296-byte function, frame 56.
- With the actual caller/font context: **10/66**, **zero excess**, **native 264-byte extent and 40-byte frame**.
- No target unresolved/unverified sites or scorer errors. This is research, not a match.

The full existing `control_settings` caller remains nonmatching (627/638, two unverified owned-data sites). Its two existing helper bodies `func_800DCD58` and `func_800DCCE0` still compile at 0/39 and 0/28 respectively; these are informational existing matches, not new claims. Other context scores are included. Claims remain empty.

## Actual interface and table

The native helper reads private s5/s6/s7 inputs. Their meaning comes from the actual existing caller and seven real call sites: `void attract_mode_handler(s32 menu, s32 item, void *text)`. The helper selects font 11, sets the color mode, centers the text within the selected rectangle, and restores alignment state.

The table at D_80153FD8 has 88-byte outer rows and two 44-byte entries. Each entry contains signed-halfword x/y/width/height at +24/+26/+28/+30. Opaque spans represent observed record layout, not stack padding. The alignment-pair view at D_80118E20 is also used by existing native-backed HUD sources. The real `camera_auto_follow` text-box interface uses six signed-halfword arguments plus the text pointer, preserving signed division-by-two and narrowing at the actual call.

Assumptions are valid menu/item indexes, sufficient table storage and a valid text object. Names describe use rather than original recovered struct names. No dummy formals, caller fixtures, pressure locals, unsupported volatile or stack filler are introduced.

## Genuine context and replay

The complete caller is read unchanged from frozen `cloud/work/ipa-groups/codex_control_settings_a4/group.c`, including its real existing helper bodies. Its path and SHA-256 are recorded in `observed.json`. This is prior complete source, not newly reconstructed work. `slot_context.c` is the actual credits-scroll font helper normalized as in PR #215. `countdown_caller.c` is the unchanged actual font caller from corrected PR #220. The returning gfx/font wrapper follows frozen `cloud/work/s20261004/E/src/helper.h`.

The group keeps `control_settings` and the actual countdown callback, leaving this target and the font helper internal so their private ABI can be inferred from real code. Full sound-update context remains unresolved. The frozen caller is materialized by the replay rather than duplicated as thousands of header lines in this packet.

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_rectangle_text_context_20261006/replay.py --repo .
```

The replay reads immutable tools, caller and protected target/manifests into temporary storage, builds the standalone and genuine-context controls and reports metadata. No behavior/acceptance checks, protected-source edits, locks, splice claims or ROM integration are added. Independent verification, acceptance and merging belong to the checker.
