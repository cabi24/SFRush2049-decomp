# Entity texture callback: consumed side offset

Research target: `entity_anim_texture`, 636 bytes / 159 words.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

The original complete visual-context callback scores 148/159 differing words.
The archived `cloud/work/frontier/w5c/entity_anim_texture/best.c` scores
122/159 in the same real context. This packet scores **116/159**, with no
nonzero excess words, unresolved symbols, unverified relocations or errors.
The native 80-byte frame is reproduced, but body register/scheduling and
instruction-count differences remain. This is not a match or coverage claim.

The archived source restores the actual early player-index capture and
separate appearance booleans. The new source carries the selected side as its
consumed 0-or-24-byte native displacement, then uses that displacement both
for the side-record lookup and direction test. An explicit appearance-flag
branch retains the native boolean meaning. No data read, call, effect, dummy
local, padding, volatile, assembly or optimizer override is introduced.

The context function bodies are the frozen base's real tire donor, visibility
wrappers and accepted rand from
`cloud/work/module_campaign_20261002/ai/visual_tire/`. The original types.h is
expanded verbatim into the two dependent C files for the group compiler
(which copies declared C files only); context bodies are unchanged. The callbacks are the
actual two roots; the helpers remain internal. Both `model_bounds_calc`
(18 words) and `matrix_scale_apply` (25 words) still compare exactly here.
Those are context checks only, not new claims. Tire callback remains 429/460
off and the inlined rand body is unclaimed. Nonmatching context must not
replace its production owners. The tire donor cites arcade AnimateTire;
no exact arcade ancestor for this texture callback has been established.

Run `python3 tools/cloud/score.py group cloud/work/lean_entity_texture_20261007`.
Use IDO 5.3 and exact `-g0 -O3 -mips2 -G 0 -non_shared`; the canonical scorer
adds assembler `-r4300_mul`. Native O32 layouts and valid model/player indices
are required. Scorer, targets, locks and production source are unchanged.
Only local comparison is claimed; independent checking and integration remain.
