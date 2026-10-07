# LEAN RESEARCH: three-choice rectangle-text helper (800DC99C)

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Native `attract_demo_handler` extent: 820 bytes / 205 words, frame 56. This is a first complete native-backed C body for a controller-menu heading plus three choices, with the real existing caller.

## Observed O3 control

IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`; automatic `-Wab,-r4300_mul`:

- Same complete C standalone: **204/205 differing words**, 24 nonzero excess words, 920-byte function, frame 168.
- With actual controller/font context: **196/205**, **6 excess**, **844 bytes/frame 64**.

The group is still oversized and nonmatching. It has no unresolved or unverified target sites, but **the canonical scorer reports an unpaired R_MIPS_HI16 for D_80118E20 at .text+0x430**. This remains unmodified in `observed.json`; no verified relocation pass is claimed. Context bodies remain unclaimed and their observations are included.

## Complete behavior and genuine interface

The native body sets centered alignment, selects font 11 and renders a heading inside the actual menu rectangle with forty vertical pixels removed and a twenty-pixel inset. It then measures font height, reloads rectangle geometry, and draws three choices below the heading. The first and second flags select the larger highlighted font/color and one-pixel upward shift. The third is highlighted only when both flags are clear. Alignment state is restored after all three lines.

The 88-byte row / 44-byte entry table at D_80153FD8 contains signed-halfword x/y/width/height at +24/+26/+28/+30. Width/height halving uses signed division and drawing arguments narrow at the real call sites. Height-minus-forty stays full width for center arithmetic before narrowing its outgoing dimension.

The exact existing caller declaration has eight formals: two signed-byte selection flags, menu/item indices, and four word-sized text values. The candidate preserves that declaration and all actual caller arguments unchanged. Those final words are converted to pointers only at the actual text APIs, reflecting native 32-bit text-address use; original source pointer type spelling is not asserted. No extra formals or unused inputs are invented. Valid indices, rectangle geometry, text addresses and intended coordinate arithmetic are assumptions.

## Real context and minimal replay

The complete `control_settings` caller is read unchanged from frozen `cloud/work/ipa-groups/codex_control_settings_a4/group.c`. Its exact path/hash is recorded. Its actual call supplies every target argument; no stand-in is generated. The existing helper bodies in that file are also retained without new claims.

`slot_context.c` is the real credits-scroll font helper normalized as in PR #215. `countdown_caller.c` is the unchanged actual callback from corrected PR #220. The returning gfx/font wrapper follows frozen `cloud/work/s20261004/E/src/helper.h`. Full font/sound-update IPA, the frame difference and the relocation diagnostic remain unresolved.

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_three_choice_text_20261006/replay.py --repo .
```

The replay reads immutable tools, caller and protected target/manifests into temporary storage, compiles standalone/group controls and reports metadata. No padding locals, artificial pressure, behavior/acceptance checks, live-source edits, locks, splice claims or ROM integration are added. Independent verification, acceptance and merging belong to the checker.
