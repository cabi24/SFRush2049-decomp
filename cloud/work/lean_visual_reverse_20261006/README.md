# buffer_swap: native reversal-flag branch research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `buffer_swap`, `[0x800924F4,0x80092B80)`,
1,676 bytes / 419 words. No matching claim.

The frozen complete B137 real-context source scores 394/419 words off. A plain
if/else assigning the consumed reversal flag to 1 or 0, instead of its boolean
comparison expression, improves this to **373/419 differing words**, with zero
nonzero excess words, unresolved symbols, unverified relocations or errors.
This follows the actual native branch before sign/position setup, instead of
IDO's shorter SLTU normalization. No runtime meaning is changed for the tested
source domain.

The body remains broadly nonmatching. Its natural frame is 184 bytes versus
native 240. No padding, unused local, dummy read, extra formal, artificial
volatile or alternate optimizer flag is added to fill that gap. Normalizing
the later unrelated flags with additional branches introduced excess words
and is not submitted. The scoped real sound-handle locals and all other target
operations are unchanged from the prior B137 source.

The complete callback retains cleanup, timed six-step grow/shrink/reversal,
actual sound queue operations, basis copy/rotation, single-player effect spawn,
visibility/quality controls and object XOR/clear flags. Visual ancestry and the
real tire-module context are documented in the base's near_miss_B137 notes;
this target's timed motion is N64-specific, with no exact arcade donor asserted.

`group.json` supplies the full real B109 and visual-context bodies. The two
visibility helpers remain locally matching context and receive no new credit.
The existing tire/texture callbacks are copied unchanged as actual callers;
in particular 91874 is neither refined nor newly claimed. Context scores and
inlined/deleted bodies must not be treated as production replacements.

```sh
python3 tools/cloud/score.py group cloud/work/lean_visual_reverse_20261006
```

The entire submitted group uses IDO 5.3 and exact
`-g0 -O3 -mips2 -G 0 -non_shared`, with canonical assembler `-r4300_mul`.
Historical first-line helper recipes are provenance, not different effective
flags. Only native O32 layouts, valid callback/model/car indices and normal
finite game values are assumed. No target, scorer, accepted-lock or production
source is changed. Local research only; independent checking, image integration
and accepted coverage remain separate.
