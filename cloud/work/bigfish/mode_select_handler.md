# mode_select_handler (0x800DEF68), scout report

Feasibility: **LOW** (IPA callee with jump tables and no rodata available). Effort: 3+ days.

## Facts
- 744 words, `asm/us/blob/blob_800de454.s`. Not spliced. Frame 40, saves only `ra`.
- **IPA callee.** Reads `s1` at entry (`lb t6,1600(s1)`, `lh s6,1990(s1)`) and writes `s0..s8` without saving them; caller `func_800E05F0` (332 words) does `move s0,s3; move s1,s3` then `jal`, then reloads from its own stack. Also uses `$f20..$f30` without saving. Group: `func_800E05F0` + this + `func_800DED78`/`func_800DFBA0` at minimum (about 1,100+ words).
- **Two jump tables** (`jr t9` after `sltiu 7`, `jr t8` after `sltiu 6`; tables at `0x80124...`) in `.rodata`, which is not in the repo (`asm/us/blob` holds only text). The switch case targets cannot be read from here, so m2c fails ("Unable to determine jump table"), and the scorer cannot verify the table words (they would be reported as unverified relocations).
- Callees: `high_scores_display` x12 (the label is odd; more likely a `text_draw`-style call), `best_times_display` (56 words, already an IPA group entry in `cloud/work/ipa-groups/INDEX.md`, parameter in `s1`), `func_800DED78` x2, `scheduler_recv` x3. 19 globals, 263 FP ops, 7 multiplies, 4 loops.

## First pass
No seed: m2c aborts on the jump tables; a hand skeleton was not attempted because IPA plus missing rodata makes any measurement misleading.

## Approach
Skip. If revisited: recover the switch tables from case-body addresses (the 7 and 6 case entry points are visible as branch-target clusters after the `jr`), model with `switch`, and join `best_times_display` and `func_800E05F0` in one group.
