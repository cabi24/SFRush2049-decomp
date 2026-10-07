# LEAN RESEARCH: two-choice controller text helper (800DCDF4)

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Native `attract_video_handler`: 692 bytes / 173 words, frame 48. This is a first complete native-backed body for its actual rectangle heading and two-choice UI behavior.

## Observed excess/extent improvement

IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`; automatic `-Wab,-r4300_mul`:

- Same complete C standalone: **172/173 differing words**, **17 nonzero excess words**, 760-byte function, frame 144.
- With genuine controller/font context: **172/173**, **2 excess**, **700 bytes/frame 64**.

The word-difference count does not improve. The concrete improvement is fifteen fewer excess words and a much closer complete extent. It remains eight bytes oversized, sixteen frame bytes oversized and nonmatching. Neither target observation has unresolved/unverified sites or scorer errors. Context bodies are unclaimed and their scores are included.

## Complete native behavior and actual argument evidence

The helper selects the actual font/color state, renders its heading in the rectangle's height-minus-forty area with a twenty-pixel inset, then reloads geometry and draws the two choices using the measured line height. The selection flag chooses the font, color and one-pixel upward shift. Alignment is reset at the end.

The existing controller source declares six formals. All three actual native caller sites pass selection, menu, item and heading through private saved registers, **and also store two label words in outgoing stack slots +12/+16**. The complete native callee never reads those two words; instead, it loads one option from global label slot 236 and the other via the bank's +0x32 unsigned index. The candidate therefore retains the evidenced six-formal interface and unchanged caller, marks the final two arguments passed-but-unused, and preserves those global label reads. It does not invent consumption, manufacture extra arguments, or remove real caller work. This is call-site/interface evidence, not proof of original parameter names.

The rectangle table uses 88-byte rows with two 44-byte entries, signed-halfword geometry at +24/+26/+28/+30. Text-root pointer fields are +4/+12/+16 at D_8017A4E0. All narrowings and signed half-width/half-height arithmetic follow native data use. Valid menu/item indices, text tables/pointers, geometry and service state remain assumptions. Opaque spans are observed global object storage, not stack padding.

## Genuine context and minimal replay

The full `control_settings` caller is read unchanged from frozen `cloud/work/ipa-groups/codex_control_settings_a4/group.c`; path/hash are recorded. It is prior complete nonmatching context, not new reconstruction. Existing helper bodies in that file remain unclaimed.

`slot_context.c` is the actual credits-scroll font body normalized as in PR #215. `countdown_caller.c` is the unchanged real callback from corrected PR #220. The returning gfx/font wrapper follows frozen `cloud/work/s20261004/E/src/helper.h`. Full font IPA and the remaining frame/word mismatches remain unresolved.

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_two_choice_text_20261006/replay.py --repo .
```

The replay reads immutable tools, caller and protected target/manifests into temporary storage, compiles and reports the controls. No fake call sites, pressure locals, padding, behavior/acceptance checks, live-source edits, locks, splice claims or ROM integration are added. Independent verification, acceptance and merging belong to the checker.
