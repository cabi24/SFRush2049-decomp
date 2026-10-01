# Workbench pilot, agent B (C2): 5 functions

Tool: vendored n64-decomp-workbench `diagnose` (via `~/agents/wb/loop.sh`) plus `guide`. Scoring: strict `score.py`
(`fn` for singles, `group` for the IPA pair). All flags `-g0 -O2 -mips2 -G 0 -non_shared` (the group uses the
group build, `-O3` whole-program with `uld -kp`). Scratch: `~/agents/B/scratch/` on Rocky. Run counts are approximate
(scripted batches of source variants count one run each).

Setup note: the loop's strict line printed "no target section .text.func_8010E828" because my private `~/agents/B/wt`
had an older `asm/us/blob/` than the Pi checkout (newly registered 0x8010xxxx regions). I rsynced the Pi's
`asm/us/blob/` into `~/agents/B/wt/asm/us/blob/` (my private copy only) and the strict scorer then worked.
`loop.sh` should refresh or warn about a stale `WT`.

## func_800D4D84 -- MATCH (flags `-g0 -O2 -mips2 -G 0 -non_shared`)

- Start: seed 16/30 words (verdict structure-mismatch, 2 alignment gaps, 5 opcode mismatches: the m2c store order was
  a permutation of the real one). Not the "very low" number the brief expected on the strict count.
- Lever (none from diagnose, my own): restore natural field order of the nine copies (0x734 .. 0x754 ascending): 16 -> 6.
  That is the same 6 the hand notes had.
- diagnose on the 6: `verdict=schedule`, `playbook=g0-schedule-probe`, `routing=permuter-first`, lanes identical on all four
  classes (pool, temp, fp-pool, fp-temp), hunk = three `lwc1` of 0x22C/0x230/0x234 issued in a different order, plus the
  matching store order. Correct diagnosis (pure as1 ordering). Its suggested levers 3 (-g0) and 4 (flag parity) were
  vacuous here (already -g0, flags right); lever 23 (preprocess) is vacuous for a macro-free body.
- Lever 33 (fold statements onto one physical line so they tie on line number; `guide 33`, `guide 25`): I scripted all 256
  fold masks of the nine statements. 128 masks stay at 6, masks with the 0x750 statement joined to the previous one reach 4,
  and mask 192 (the last two copies, 0x750 and 0x754, on the same physical line as the 0x74C copy) gives 0 = MATCH.
- Final: MATCH. Moved it most: lever 33 (line-number ties; 6 -> 0). Source: `cloud/matches/func_800D4D84.c`, re-scored from
  the repo copy as bare MATCH.
- Verdict on the tool: diagnosis right, lever list pointed at the right mechanism family (as1 line-number scheduling) but the
  headline lever it printed first (3) was vacuous; the guide's lever 33 is the one that solved it and diagnose did not name
  it for this case (it names 3, 4, 23, 24). Useful in that it ruled out register/frame causes in one screen; the fold search
  was mine. The earlier hand notes (15000 statement-order permutations) never tried the physical layout, which the PLAYBOOK
  even says to "keep multi-line", so this lever is worth adding to the playbook.

## func_8010E828 -- MATCH (flags `-g0 -O2 -mips2 -G 0 -non_shared`)

- Start: seed compiles; diagnose 1 word differs (`li a0,1` vs `li a1,1` at the first `jal`).
- Diagnosis printed: `register-permutation`, `owning_pass=uopt-globalcolor`, and
  `lever: unreachable (uopt-coalescing-tie-break)`, `reachability=permuter-target`, "no HAND lever found".
  This was WRONG for this case: the residual was not a colouring tie, it was a missing argument. The target does
  `jal entity_transform_apply` with `a0` untouched and `a1=1`; the m2c seed called `entity_transform_apply((void*)1)`.
- Fix (my own reading of the disassembly, the tool's web "a1->a0 x1" was the clue): first call
  `entity_transform_apply(arg0, 1)` like the second one. 1 word -> 0. Strict score 0 differing; diagnose
  `instruction-words-identical`, `exact`.
- Final: MATCH. One run on the seed, one on the fix. Source: `cloud/matches/func_8010E828.c`.
- Verdict on the tool: misleading. It told me the case was unreachable by hand and should go to the permuter; a one-word
  look at the call setup (copy vs immediate on an argument register of a call whose callee shape is known) solves it.
  Suggestion: when the single differing site is an argument-register `li` immediately before a `jal`, say "check the
  argument list / arity of the call" before concluding colouring.

## func_80096BBC -- MATCH (flags `-g0 -O2 -mips2 -G 0 -non_shared`)

- Start: seed 11/27 words (diagnose: structure-mismatch/mixed, 1 instruction-count gap, register 11). The brief's "v1/v0
  reversed" was not what the lanes showed for this seed (pool lane equal until slot 6; the loaded element value took the
  pool register `a1` instead of a ring temp, and the temp ring was one pop off).
- Natural typed-struct rewrite (as in the hand notes): 17/27, because `off = 0` hoists above the `lw v1,0(a0)`
  (extra `move`) and the offset ends up in a ring temp, not `v1`. diagnose called this structure (3 structural words).
- Tried ablations of the m2c seed: the `goto dummy_label` and the dead `temp_v0_2 = arg0[2] + var_v1;` before the
  `if` are both load-bearing (removing the label: 11 -> 17; removing the dead read of the uninitialised `var_v1`: 11 -> 18).
  Folding `temp_t2` into `+= arg2`: pool lane becomes identical to the target (7/7), 11 -> 11 words but the residual is now only
  the temp ring. This is where `diagnose` helped: lanes said pool identical, temp lane diverges at slot 4.
- Lever 14/15/16 (phantom pop) and named-temp variants (q1-q6, masks, `(t==-1)!=0`): no change, 11 words. The shared lane
  showed the real reason: the 4(v1) value in the target is a ugen temp (`t9`) but ours was uopt-coloured `t0` ("shared" lane)
  because the named `temp_t9` (and the label) made it live across a block boundary.
- Moving `var_v1 = 0;` into the `if` body made the dummy label unnecessary (pool still identical), then stating the
  loop guard as a re-read of the just-stored field (`arg0->4 = hdr->4; if (arg0->4 > 0)`), so no named value spans the
  branch: 11 -> 0 = MATCH.
- Final: MATCH. Moved it most: making the guard value block-local (class-crossing site: coloured `t0` -> ugen temp `t9`).
  The `shared` lane in the report found that; the guide's own sentence "chase the class-crossing sites, not the phase" is
  the right instruction and diagnose's lane display is what makes it actionable.
- Source: `cloud/matches/func_80096BBC.c`. It still needs the dead read `temp_v0_2 = ... + var_v1` (uninitialised var), so it is
  a compile-affecting quirk and not obviously the original source; say so when splicing.
- Verdict on the tool: correct and useful (lane view located the problem, three guide levers did not apply but the "class
  crossing" law did). Verdict label (structure-buckets/"cfe-spelling") for the natural rewrite was less useful.

## func_800B7438 -- MATCH as a two-function IPA group (not a single-function match)

- Start: seed 16/24 words, structure-mismatch. The seed was the wrong shape (m2c `s32 *` return with goto loop, byte flag read
  as `s32`). Diagnose said constant/structural mix; correct that it was not a colouring problem, but it had no way to say
  "callee-ABI".
- Hand notes were right: single-function source spills `a0` around the call; the target stores `a0` after the `jal` because
  `func_800B73E4` is compiled in the same IPA group and clobbers only `v0,v1,at,t6,t7`. No single-TU `-O2` source can do it.
- Group (`cloud/work/ipa-groups/func_800B7438/`): `func_800B73E4` + `func_800B7438`, flags `-O3`, keep
  `func_800B7438` and a stand-in `__standin_input_init_flag_get` that gives the callee a second call site (prevents inlining).
  Steps and word counts, each with `zbuild`/`gc` on the group: first source 8 words differ in B7438 (it moved `a0` to `a2`
  because the callee as I wrote it used `a0`/`a1`); the callee needed to colour its flag address in `v0` and merge `lui at` for the
  two leading stores: define the array in the TU (`s32 D_801551E8[10];`) and write the init as the plain loop
  `for (i = 0; i < 10; i++)` (so IDO's own 4x unroll peels the first two stores, not an explicit pair; explicit pair stores hoist a base
  register to `a1`): B73E4 4 words differ (loop store order [0..3] vs target [1],[2],[3],[0]).
- Lever 33 (line layout) fixed the callee: writing `for (...)` and the body on two physical lines flipped as1's
  choice of the delay-slot store: B73E4 4 -> 0 (`for(...) stmt;` on one line is 4, two lines 0, brace forms 0, `do/while` worse).
  B7438: end pointer `&D_80155210` as the loop bound instead of `&arr[10]` (2 words left: the two `addiu` for start/end in the wrong
  order), `do/while` guard form (17 -> 2), then `for (p = arr; ; ) { if (*p == 0) { *p = a; break; } p++; if (p == &D_80155210) break; }` -> 0.
- Final: `score.py group` reports both `func_800B73E4` and `func_800B7438` as MATCH (45/45 words with the stand-in in `zbuild`).
  The stand-in cannot be spliced; the real second caller of `func_800B73E4` must be in the ROM group. B7438's callee-saved
  behaviour confirms the IPA diagnosis in the hand notes.
- Verdict on the tool: for the single function its `diagnose` cannot see the IPA cause (it compares one object), so it
  would have sent a hand-matcher into register levers; `diagnose` on the group objects would have been the right use. For the
  callee, lever 33 was the winning lever (not named in diagnose's output either; the report said `structure`).

## func_800D18D8 -- NOT matched (best 8/41 strict words)

- Start: seed 10/41 (8 after dropping the dead `sp1C`). diagnose: `allocation-mismatch`, `playbook=pool-position`, 4 relocation sites name a
  different symbol, with the caution "words=8 is a floor" (it correctly warns about the masking that hid the earlier false zero).
  Lanes: pool lane identical (a0 a0 a1 a1 v1 a0 a2 a2 ...); the temp lane starts at slot 0 with `t9` where target has `t6`
  (the symbol-to-register assignment of the two address webs `a0`/`a2` is what differs, and the temp ring follows).
  A second hunk: spill slot `28(sp)` in the target vs `24(sp)` ours (`constant` class, 2 words).
- Levers tried: expression order of the shifted/masked operands, comparison direction, `>>` spelling (a2-a5, c1-c6): no change
  (8); levers 7/9/39 dead reads of `D_801460F8` (b1-b4, d1-d6): worse (19-25), the dead read emits loads or takes `v1`;
  `register`, extra declared locals, `volatile`/address-taken pads (e, f, g, w): frame grows to 40 or no change; swapped extern
  declaration order: no change; physical line folds of all five statements (16 masks): no change; flags -O3/-mips1/-r4300_mul: no change (8/13);
  lever 39 three-reference statements (`(D_801460F8 | D_801460F8) + 1` etc.): no change.
- Frame: target spill of `a1` is at `28(sp)`, ours `24(sp)`. By L53/L73 ours is the uopt-temp region below an (empty) declared
  region; the target keeps it at the top. No source construct moved it without growing the frame.
- Final: 8/41, same as the hand notes. The two remaining mismatches (which global gets `a0` vs `a2`, and the spill slot) look like
  they come from the original having a different declaration/definition structure for the three globals.
- Verdict on the tool: correct diagnosis (pool position, not schedule), and the relocation caution is valuable. The
  pool-position levers (7-9, 39) did not apply: here the competing webs are address-load webs and the dead reads
  materialise real loads. I would call the guide's pool levers unproven for address webs.

## Summary

| function | start | final | what moved it | tool verdict |
|---|---|---|---|---|
| func_800D4D84 | 16/30 (6 after natural order) | MATCH | lever 33 fold (last two stores on one line) | right class, wrong first lever |
| func_8010E828 | 1 word | MATCH | missing `arg0` argument on first call | misleading ("unreachable / permuter") |
| func_80096BBC | 11/27 | MATCH | block-local guard value (class-crossing temp) | correct, lane view decisive |
| func_800B7438 | 16/24 | MATCH as IPA group with func_800B73E4 | IPA callee + lever 33 layout on the callee | cannot see IPA from one object |
| func_800D18D8 | 10/41 | 8/41 not matched | none | correct but no workable lever |

`diagnose` helped most where the lanes separated the residual into a mechanism (func_80096BBC: pool identical, one
class-crossing temp; func_800D4D84: all four register lanes identical, so pure scheduling). It did not help, and once misled,
where a semantic difference hides as a register diff (func_8010E828: an unreachable verdict for a missing argument) or where the cause
is outside the compared object (callee ABI of func_800B7438). New or under-weighted in the guide: (1) lever 33 (fold statements onto one
physical line) solved two different functions here (a store-order tie in a straight-line copy, and the delay-slot store choice in a
callee's unrolled loop) and `diagnose` named neither time; it should list it for any `schedule` residual with identical lanes. (2) A
dead read of an uninitialised variable before the block (`temp = ... + var_v1;` with `var_v1` unset) changes the pool lane
for a later web (needed by func_80096BBC): it is a pool-position lever that is not in the guide as such. (3) Keeping a value block-local
(re-reading the stored field in the condition rather than a named local) turns a uopt-coloured shared register into a ugen
ring temp; levers 14-16 rotate the ring but this "class-crossing" move is described only in prose.
