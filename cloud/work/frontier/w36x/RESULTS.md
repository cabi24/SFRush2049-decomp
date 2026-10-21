# Lane w36x (wave 36, mid-wave near-miss closer) — RESULTS

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36x (copied from base, master e68aea6e1).

| function | image | GC identity | bytes | state | best file |
|---|---|---|---|---|---|
| func_803A44FC | A | animText@carscr.c (w36b, confirmed) | 3668 | NONMATCH 2/917 (as1 xbb hoist into the loop header; mechanism traced, no source) | w36b/func_803A44FC/best.c (unchanged) |
| func_8039842C | A | updatePlayerScreen@playerscr.c (w36c, confirmed) | 1804 | NONMATCH 2/451 (spill home; extra cross-region web missing) | w36c group (unchanged); q-free MAT3 alternative 9842C/b3/v0000.c also 2/451 |
| func_803A9088 | A | drawDeletePlayer@playerscr.c (w36c, confirmed) | 3668 | NONMATCH 15/917 (spill homes; not worked beyond analysis) | installed body (unchanged) |
| func_803AD57C | A | animText@recordscr.c (w36f, confirmed) | 2092 | NONMATCH 497/523 strict, 1 as1 placement (same xbb family) | installed stats group (unchanged) |
| func_80391B00 | B | UpdateHUDExtraPostCamera@hudextra.c (w36j, confirmed) | 2380 | NONMATCH 407/595 (82 with an uninitialised-local holder, not delivered) | w36j best/ (unchanged); 91B00/b2/v0024.c = 82 with holder |
| func_803A9EEC | A | animText@playerscr.c (w36c, confirmed) | 1260 | NONMATCH 102/315 strict; EXACT listing oracle found (`.alias` after case 6's `b`) | w36c best_group.c (unchanged); oracle A9EEC/x1.s |

No deliverable match this lane: nothing in cloud/matches/, no group revision (no group improved). Bases used: w36c's
unlanded availability revision (9842C), installed runtime_a_profile_menu (A9EEC/A9088), installed stats group
(AD57C, w36f's revision is the installed one), w36j best/ (91B00), w36b best.c (A44FC).

## Log

### func_803A44FC (in progress, 2026-10-21): as1 xbb mechanism traced
- Reproduced w36b best.c standalone: 2/917 (`+0x094/+0x098` bne/sll swap in the animText loop header).
- Instrumented as1 (tools/as1_patch_w.py on top of w12b build_as1dbg.sh; AS1W=1 prints every xbb block weight
  decision). Rules read from as1.c func_429534/func_42aa0c/func_428758:
  weight(B) = min over LOWER-id preds P in the region of P.w-3 (B postdominates P), P.w+1 (P has an edge leaving
  the region), else P.w+3; blocks control-equivalent to an earlier block (dominated by it and postdominating it)
  inherit its weight; pred-less blocks (entry, a directive block after an unconditional jump, the .rdata jump table
  block after `j`) get the lowest ids and w=1. Weights are u16 (1-3 wraps to 65534). Dominance is one-pass, so a loop
  header is dominated only by itself. xbb then visits blocks in ascending weight and may pull instructions only from
  blocks LATER in that order, dominated by the current block, with weight difference < 6.
- Ours: header bb5 w=1 (= preheader 4 - 3), first inlined block bb6 w=4 -> candidate -> sll/addu hoisted. The
  oracle `b $32; .alias; $32:` gives the header a pred-less lower pred (w=1) -> header 65534, bb6 wraps to 1 ->
  no candidate -> retail header. So retail's header must have had a lower predecessor of weight 0..2 that it
  postdominates (or a preheader of weight 0..2).
- Consequence (proved on the listing): with retail's code layout (guard, fall-through preheader, header) the header's
  only lower predecessor is the preheader (weight guard+3 = 4), so the header always gets 1 and bb6 4 -> hoist. A
  pred-less block in front of the header needs an alias directive after an unconditional `b` (only `.alias/.noalias`
  make blocks: `.loc`, `.livereg`, `.set`, labels and `b` to the next label do not; e8/e10/e13-e17), and that `b`
  stays in the code (e1, e13, e16: `b 0x90`). Preheader reorders (p1-p3), bb6 reorders (r1-r5), `.set volatile`
  around header/bb6 loads (e18-e20), a label inside the preheader (e6) do not change the decision.
- as1 carries state across procedures: assembling AD57C's listing in front of A44FC's changes A44FC's prologue
  schedule (`sw a0,312(sp)` placement) but not the header (cat1.s). Standalone compile already matches the prologue.
- Source sweep b1 (162 variants: loop forms for/while/if+do-while, compiled-out reads before/inside/after the loop,
  arm order/braces): best stays 2 (40 variants at 2), nothing better.
- Status: 2/917, unchanged; mechanism understood but no natural ugen output found that gives the header a weight
  0..2 predecessor. Tools: tools/wt.sh (weight trace), tools/at.sh (assemble edited listing + diff), A44FC/e*.s.

### func_8039842C (in progress): spill-home residual narrowed
- Base: w36c's revision of dot_runtime_a_availability_refresh_20261007 (not yet landed by w36i), 2/451 reproduced.
- `q` is not needed once w36c's shaping line is present: MAT3's plain `updateButtons(playerIdx)` after the switch
  (no q, no unused locals, 9842C/b2/v0003) gives 14 words = frame 160 vs 192 only (+ the playerIdx home).
  With 6/7/8 unused s32 the frame grows but the reset-loop pointer stays one slot off (b3, b4).
- Spill-temp trace (9842C/pre_h6.txt, spill.sh): homes are handed out in itable order; ours: temp0 {0,10,14,56,71,141},
  temp1 {16 (first-loop &D_8014A118[i]), 53 (&D_803BACE0[playerIdx])}, temp2 {17,57,67,86,163}, temp3 {59}.
  Retail (frame 192, reset ptr 124, D_803BACE0 ptr 116) = cfe area 64 with 5 temps: the D_803BACE0 web must find
  temp1 occupied by a web it conflicts with, i.e. temp1 holds (besides 16) a web numbered before 53 that is live in
  the main loop but not in the first loop -- or the first-loop pointer web is the same web as a main-loop one.
- Tried without effect (all stay at 2 or worse): the four control helpers as real inlined functions in every subset
  (b2/b4, 8 bytes of frame per inlined call, homes shift), empty inlined updatePlayer (b1), MAT3 block `i` for the
  allReady and purge loops (b4), compiled-out reads of 10 candidate expressions at 4 positions (b5; inside the loop
  they break the per-path conversion), first loop indexed by playerIdx (b6: playerIdx then lives in s8 there).

### func_803AD57C (same as1 family as A44FC): traced, not closed
- Base = installed runtime_a_stats_menu_20261006 (w36f's revision landed in batch 2): AD57C 497/523 strict.
- Weight trace (AD57C/wt0.log): dispatch block [lw; beq 0] is control-equivalent to the entry (w=1), the `beq 1`
  block has w=4 -> xbb fills the lw stall with its `li at,1` and leaves the delay slot empty. With w36f's label
  oracle (AD57C/ec.s) the lw is alone in its block (stall-free), the [beq] block has only a delay-slot stall, and xbb
  moves `li at,1` into the delay slot = retail. So retail's as1 input had a block boundary between the load and the
  compare (a join there, as in the matched func_8039C140, whose `confirm_option:` label is a goto target right at a
  switch whose value arrives from two paths -- the only retail image-A/B site with the same kept-stall shape besides
  AD57C; tools/scanpat.py).
- Tried without effect: goto/loop wrappers around the switch (`goto L; L:`, do/while(0), for(;;){..break;},
  while(1), goto arms, default goto), gRecordState defined/static/u32 in the unit (volatile changes code),
  assembling AD57C after other procedures / in retail procedure order (no as1 cross-procedure effect here).

### The shared as1 cause behind A44FC, AD57C and A9EEC (one family)
All three residuals are the same as1 cross-block (xbb) decision: as1 pulls the first instruction(s) of a successor
block (weight +3) into a block that ends in a load-use stall; retail did not. A44FC: loop header <- first block of
the inlined animButtonText. AD57C: [lw gRecordState; beq 0] <- `li at,1` of the `beq 1` block. A9EEC: loop header
[lw; lw; bgeu 7] <- default block `addu s0,s2,1` (weight trace A9EEC/wt_v4.log: header bb5 w=1, default bb6 w=4,
MOVEFROM bb 6). In every case the target block has weight 1 because it is a region entry or control-equivalent
to one, and the candidate is its +3 successor. A one-pass as1 oracle that would give retail in all three: the
target's weight wrapping to 65534 (a weight-0..2 predecessor that it postdominates), or the candidate failing the
dominance test (as1's dominance is one-pass: a block with a higher-numbered predecessor is dominated only by itself).
- A9EEC: `case 6: drawPakDamaged(p); default: break;` (fall-through into an empty default, semantically identical)
  gives retail's dispatch block exactly (A9EEC/b1/v0001, `lw t8; sltiu; beqz; sll` = retail) but then case 6 shares
  the default's `p+1` and lays out 2 words shorter (107 strict vs w36c's 102). Retail case 6 ends `b latch` past the
  default block, so this is not the source; it does confirm the lever class (a second predecessor of the candidate).
- Not causes (tested): as1 -O2/-Olimit, the xbb window K (patched as1: K=1 is the stock value; K=0 is far worse),
  procedure order / preceding procedures in the same as1 run (label-renumbered concatenations, tools/catlst.py:
  AD57C after its retail predecessors, A44FC after carscr group + A3A6C/A3DA4/A4134/A4340), alias directives at the
  function entry. Note: as1 block flags carry stale low bits across procedures (cat1: A44FC after AD57C changes
  A44FC's prologue schedule), but no tested context sets the bits xbb tests (0x20 candidate skip, 0x40 target skip).

### func_803A9EEC: listing oracle found (exact), source form still open
- Base: w36c A9EEC/best_group.c (102/315 strict). Writing `default: break;` last (retail layout: default block after
  case 6, case 6 ends `b latch`) gives A9EEC/v4.s = retail layout + the as1 hoist of `addu s0,s2,1` (254 strict:
  one-word shift).
- ORACLE (A9EEC/x1.s, x2.s, x5.s): inserting a single `.alias $R,$sp` (or `.noalias`, any register) between case 6's
  `b $312` and the default label `$311:` makes the default block a join with a pred-less directive predecessor
  (weight 1-3 -> 65534), so it is no longer an xbb candidate of the dispatch block: the assembled listing then has
  ONLY relocation-artifact differences (12 positional, all unresolved `$$NN`/symbol relocs of a listing-assembled
  object; baseline v4.s gives 254). `.livereg`, `.loc`, `.set`, `.mask` there do nothing (x3,x4,x6,x7).
  So retail's ugen output had a noalias region still open at the end of case 6 (closed after its `b`), i.e. a
  memory access through an lda-rooted register after the last call of the inlined drawPakDamaged (ugen closes a
  region after the next jal, or after the block-ending `b` when no call follows: carscr/stats listings show the
  `b; .alias; label` shape only for an access in the last block). Retail's last block of case 6 is
  `lui v1; b latch; lh v1,gNumPlayers` -- only the PRE'd gNumPlayers reload.
- Source attempts that did not open such a region (A9EEC/b2): compiled-out reads at the end of drawPakDamaged
  (`playerTextColor[p]`, `gPlayerRects[..][p].x`, gNumPlayers, str, *str, str1[0], gText.single[29]), a block-local
  pointer for playerTextColor[p], gPlayerRects..x re-read at the last TB_CenterText.
- A9EEC/b1/v0001 (case 6 falls into `default: break;`) reproduces the dispatch but merges case 6's p+1 (107 strict).

### func_80391B00: holder sweep (w36j base)
- Reproduced w36j: best/ 407/595, best_holder/ 83/595 (builder `score.py group`, image B).
- 91B00/b2 (72 variants: 12 holder expressions x 6 positions): only reads of UNINITIALISED locals (e, tex, texIdx)
  work as ghosts (82-83); best 82 = `if (e) {}` in the reset loop after `coin = &stuntCoin[i][j]` (91B00/b2/v0024).
  Expression reads (gNumWindows, whichCam[i], p->lock, p->working, gStuntPlayer[i].active, coinColor, alpha) do not.
- 91B00/b1: indexing the reset loop with `k` (the search/prev-loop variable) instead of `j` gives the stunt-loop
  allocation without any holder (407 -> 309) but breaks the reset loop's own j*72 IV, i.e. retail's extra web is a
  variable/web that spans both the coin loops and the reset branch -- the same kind of cross-region web that
  9842C needs (there: sharing the first-loop index with the main loop puts the D_803BACE0 pointer at retail's 116,
  9842C/b6/v0002, but permutes the first loop's registers).
- Remaining with the holder: (u8)alpha temp home 140 vs 136, `lw v1,144(sp)` placement / v1 vs v0, one extra
  `sw v0,124(sp)` (w36j's analysis stands). Not delivered (artificial read, still 82).

## Tools written (cloud/work/frontier/w36x/tools/)
- `obatch.py` + `ob.sh VARDIR ovl_a|ovl_b FUNCS [--group lane/..json] [--show N]`: overlay variant batches on the
  builder (standalone or group mode), ranked, with the first N differing words per variant (vbatch has no overlay
  support).
- `as1_patch_w.py` (on top of w12b build_as1dbg.sh): AS1W=1 prints every xbb block-weight decision; AS1K=n overrides
  the xbb window constant. `wt.sh LABEL FILE.s` runs it; `kt.sh` assembles a listing with env overrides.
- `at.sh LABEL FILE.s NAME [IMG]`: assemble an edited listing in a snapshot and diff an overlay function.
- `catlst.py OUT A.s B.s ...`: concatenate ugen listings with per-file label renumbering (plain `cat` makes as0 fail
  on duplicate `$N` labels and silently leaves a stale object).
- `ofor.sh LABEL NAME SPEC [IMG]`: force.sh colour oracle with an overlay diff. `rdis.py IMG NAME`: retail overlay
  disassembly. `scanpat.py IMG`: finds kept-stall vs hoisted dispatch shapes in an image.

## Identities proven
None new (all six identities were already confirmed by w36b/w36c/w36f/w36j; re-checked against MAT3 bodies).

## Permission denials
none

Housekeeping: the 9842C batch variants (b1-b8) were pruned to the files cited above (manifest.tsv + score.txt kept;
regenerate the rest with 9842C/gen1.py, gen2.py, gen4.py, gen7.py; b3/b5/b6/b8 were generated inline, their
manifests record the options).
