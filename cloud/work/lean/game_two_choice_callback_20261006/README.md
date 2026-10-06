# LEAN RESEARCH: two-choice menu callback (8010B9C8)

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Native target: 700 bytes / 175 words, frame 152.

This complete typed body sets the actual hidden condition, draws a centered heading and two choices, switches their selection colors, and restores rendering mode. Native coordinate values replace the generated seed's uninitialized stack-alias temporaries; no filler stack storage is invented.

## Observed O3 result

IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`, automatic `-Wab,-r4300_mul`:

- One-cast repaired seed control: **171/175 differing words**, 524 bytes/frame 88, no excess, two unverified owned-data sites.
- Complete typed body standalone: 175/175, 652 bytes/frame 104, no excess or unresolved/unverified sites/errors.
- Complete typed body with real font context: **167/175**, **680 bytes/frame 120**, no excess or unresolved/unverified sites/errors.

The retained real-context candidate is a modest word-score improvement and a much more complete body. It remains twenty bytes short, with an unresolved 32-byte frame gap. It is research, not a match.

The control is frozen `cloud/work/registered-heads/seeds/func_8010B9C8/group.c`, cut before its helper definition to discard generated stand-ins, with only a `void **` cast before the integer-address dereference. **That control still contains undefined stack-alias reads and is not a semantic reference.** Its score is provided transparently as generated-seed codegen, not behavior evidence. The candidate contains none of those undefined aliases.

## Native reconstruction and actual context

The native stack stores x and y as words, then reads their low signed halfwords on the big-endian target. The seed incorrectly separated those reads into uninitialized locals. The candidate uses the actual x/y values, with the same signed-halfword narrowing at the draw sites. Header width is deliberately queried again before positioning choices. Quarter-width spacing uses signed division; centering uses the actual unsigned half-width shift. Global center and text-table references are reread at their native call boundaries. The final renderer reset is the real `-1.0f`, not an integer bit-pattern converted to float.

Layout views: Blit hidden byte +0x1A; signed-halfword coordinate pair at D_80117280/+2; text bank/string pointers at D_8017A4E0+12/+16; unsigned choice index at bank+0x30; heading pointer in countdown-object slot 216. These are observed offsets, not asserted original struct names. Valid string indices, array storage, service pointers and representable intended coordinate arithmetic are assumptions.

`slot_context.c` is the existing real credits-scroll font helper normalized as in PR #215. `countdown_caller.c` is the unchanged complete actual caller from corrected PR #220. The genuine returning font wrapper and real additional call sites provide the private helper context; no artificial calls or volatile pressure are used. All context scores are included and claims remain empty. Full font/sound-update IPA context and the original larger frame declaration remain unresolved.

## Minimal replay

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_two_choice_callback_20261006/replay.py --repo .
```

The replay reads frozen tools, baseline and protected target/manifests into temporary storage, applies the disclosed baseline cast and reports compiled controls. No behavior/acceptance checks, live-source edits, locks, splice claims or ROM integration are added. Independent validation, acceptance and merging belong to the checker.
