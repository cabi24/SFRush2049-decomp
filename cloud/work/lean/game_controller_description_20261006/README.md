# LEAN RESEARCH: controller description helper (800DD0C0)

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Native extent: 908 bytes / 227 words, frame 440. This is a first complete native-backed C body, using its genuine existing controller caller.

## Observed compilation

IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`; automatic `-Wab,-r4300_mul`:

- Same complete C standalone: **225/227 differing words**, **21 excess**, 996-byte function, frame 432.
- With the actual controller/font context: **194/227**, **zero excess**, **904-byte function**, frame **432**.

The target has no unresolved/unverified sites or scorer errors. It remains four bytes short and eight frame bytes short, so this is research, not a match. The buffer capacity was fixed from the evidence below and never varied to improve scoring. Context observations are retained and claims remain empty.

## Meaningful scratch buffer: explicit hypothesis

The native routine copies and concatenates real text into scratch beginning at stack +172. Subsequent scalar slots occur at +428/+432/+436 in its 440-byte frame. The candidate therefore uses one **fixed 256-byte string buffer as an inferred source hypothesis**. This is not a recovered original declaration or a proven runtime length bound. No larger/smaller capacities were swept and no unrelated locals were added to fill the frame gap.

The actual accepted interfaces are `func_800BE6A4(u8 *,u8 *)` and `func_800BE4F0(u8 *,u8 *)`. Their source establishes encoded copy/concatenation: ordinary terminated bytes or 255-prefixed two-byte text; concatenation can widen a plain destination. They are called unchanged and remain external. The explicit capacity assumption is that the resulting encoded text, including terminator, fits the buffer. This packet does not prove that all runtime resources satisfy it or claim behavioral safety beyond that assumption.

## Complete native body and real context

The helper snapshots whether the state-mask condition selects the local branch, selects font/color, copies label 104 and appends label 103 or 102, then renders the description inside the actual menu rectangle. Margin is forty or twenty pixels according to the snapshotted branch. The optional two-choice section measures line height and reloads the signed selection byte at each native helper boundary, rather than caching it across renderer calls. It preserves actual global option-label lookups, restores alignment and finally reselects font 11.

The table uses 88-byte rows/two 44-byte entries, selection byte +1, and signed-halfword geometry +24/+26/+28/+30. Text-root pointer fields and the bank's +0x32 option index are the actual native layout. Valid indices/pointers, usable geometry, service state, intended coordinate arithmetic and the encoded-text bound are assumptions. Names describe observed fields rather than original declarations.

The complete `control_settings` caller is read unchanged from frozen `cloud/work/ipa-groups/codex_control_settings_a4/group.c`, including its real existing helper bodies. Path/hash are recorded in `observed.json`; it remains nonmatching context. `slot_context.c` is the real credits-scroll font body normalized as in PR #215. `countdown_caller.c` is the unchanged actual font caller from corrected PR #220. The returning wrapper follows frozen `cloud/work/s20261004/E/src/helper.h`. No fabricated calls, formals, pressure operations or padding locals are introduced.

## Minimal replay

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_controller_description_20261006/replay.py --repo .
```

The replay reads frozen tools, actual caller and protected target/manifests into temporary storage, compiles the two controls and reports exact metadata. No behavior/acceptance checks, live-source edits, locks, splice claims or ROM integration are added. Independent validation, capacity resolution, acceptance and merging belong to the checker.
