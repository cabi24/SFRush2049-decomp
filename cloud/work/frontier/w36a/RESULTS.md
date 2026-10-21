# w36a results: image A carscr near-misses (group dot_cleanup_colortrace_20261011)

Lane dir `cloud/work/frontier/w36a/`, builder scratch `~/rush2049/scratch/frontier/w36a` (copied from base, master e68aea6e1).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_8039EF08 | showCarSelect@carscr.c (w35b, confirmed) | 4092 | **MATCH in the group** (claimed) | groups/dot_cleanup_colortrace_20261011/group.c, func_8039EF08/body.c |
| func_803A1EAC | updateCarScreen@carscr.c (w35c, confirmed) | 7104 | **MATCH in the group** (claimed) | groups/dot_cleanup_colortrace_20261011/group.c, func_803A1EAC/body.c |
| func_803A0FD8 | _InitScreen@carscr.c (w35b, confirmed) | 3796 | **MATCH in the group** (claimed) | groups/dot_cleanup_colortrace_20261011/group.c, func_803A0FD8/body.c |
| func_8039EF00 | createGridMap@carscr.c, empty in N64 (w36a, see "Identities proven") | 8 | **MATCH in the group** (claimed; bonus, droppable) | same group |

All four are group-only (private IPA ABI of this group); nothing goes to cloud/matches. Flags `-g0 -O3 -mips2 -G 0 -non_shared`.

## Deliverable (current)

`groups/dot_cleanup_colortrace_20261011/{group.c,group.json}`: superseding revision of the installed group (same name), all 8 old
members byte-identical (source text and bytes) and MATCH, `claims: [func_8039EF08, func_803A0FD8, func_803A1EAC, func_8039EF00]`
(all four also in members), context func_803A0498 (unchanged, w36b's MATCH body drops in: see "Merge test") and func_8039E444
(w35b stand-in, NEW context entry), keep [func_803A1EAC], allow_unverified true. func_8039EF00 is the first 8-byte stub anyone
claims in image A; if the splice treats stubs specially, drop it from `claims` (keep it in the file: EF08 needs it inlined).
Full transcripts: `score_group_final.txt` (`--claims`, exit 0) and `score_group_final_all.txt` (all members).

```
$ python3 tools/cloud/score.py group cand/dot_cleanup_colortrace_20261011 --targets asm/us/ovl_a --claims
Members:
func_8039EF08:
  MATCH (66 section-relative relocations unverified: .rodata+0x60 at +0x28, .rodata+0x60 at +0x2c, ...
func_803A0FD8:
  MATCH (14 section-relative relocations unverified: .rodata+0x10c at +0x384, .rodata+0x10c at +0x388, ...
func_803A1EAC:
  MATCH (12 section-relative relocations unverified: .rodata+0x12c at +0x8b4, .rodata+0x12c at +0x8bc, ...
func_8039EF00:
  MATCH
Context (informational; excluded from exit status):
func_8039D300/D05C/D494/ED20/E200/E3BC: MATCH; func_8039FF04, func_8039D6A4: MATCH (own data unverified, as before)
func_803A0498:  154/720 words differ
func_8039E444:  556/567 words differ (8 extra words ...)
```
Own .rodata of EF08: `tools/rocheck.py groups/dot_cleanup_colortrace_20261011 func_8039EF08` -> `rodata sites ok=33 bad=0 skipped=0`
(literal values at the retail addresses of decompressed image A); A0FD8: `rodata sites ok=7 bad=0 skipped=0`; A1EAC: 5/5 float
sites ok, and the one rocheck "MISMATCH" (.rodata+0x12c) is the 19-entry switch jump table, whose words are .text relocations:
all 19 entries equal retail's case addresses relative to the function start (checked against image A at 0x803B9740).

## func_803A0FD8 = _InitScreen: closed (12 -> 0 words)

Two causes, both traced:
1. 4 words, as1 delay slot of the unrolled `D_80110E78[p+1][i] = paint` loop: with `for (...) stmt;` on ONE line as1 fills the
   `bne` delay slot with `addiu v0,v0,4`; with the body on the next line (any of 4 line splits tried) it moves the first
   `sb a1,13(v0)` there with its displacement rewritten to 9, as in retail. Same lever as EF08: as1 breaks ties by source line.
2. 8 words (7 after 1), spill-home order (`tools/trace/spill.sh`): f_spilltemps gives every coloured expression web a 4-byte home,
   in EXPRESSION-BIT order, first fit by block conflict. Ours: home0 (188) = {gNumPlayers load, p+1, ...}, home1 (184) =
   {first-loop &D_803B28B8[p], &gNumCarsActive[p] (bit 29)}, home3 (176) = &gPlayers[p] (bit 93, the strength-reduced s6
   pointer, saved around the initButtons call). Retail has s6 at 188 and gNumCarsActive at 180. Bit numbers follow first
   appearance in uopt's expression table; a dead `pl = &D_8014A118[p];` as the first statement of the FIRST player loop
   (compiles to nothing) gives &gPlayers[p] a bit before the second loop's webs, so it is homed first (188, no block conflict
   with the gNumPlayers web) and p+1 then lands in home1, which pushes &gNumCarsActive[p] to home2 (180). Result: MATCH.
   (At the end of the first loop: also MATCH; at the top of the second loop: 2 words.) Disclosed in the header as a code-free
   shaping statement; the original probably set a player pointer at the top of each player loop.
   Tried first (no): MAT3 locals only (120 words), extra locals (frame), `pl++` induction variable (244+ words: s6 init moves
   before the loop test, frame 200).

## func_8039EF08 = showCarSelect: closed (2 -> 0 words)

The w35b residual (`lui at` / `addiu t9` swapped at +0x2ac/+0x2b4) is an as1 cross-block-pull tie broken by SOURCE LINE NUMBER.
Trace (`tools/trace/as1t.sh`): the block after `angle -= 1.5707964f` (8 own nodes) is scheduled with stall cycles at t=3,4 and
t=6..8; as1 then pulls `lui t9; addiu t9` (`&gFrameSeconds` of the `angle > 1.5707964f` then-arm, line N) and the compare's
`lui at` (li.s 1.5707964f) into those holes. In the w35b source the `if (...) angle += CAR_MIN(...)` is ONE line, so `lui at` and
`addiu t9` tie on aftercycles and line, and `addiu t9` wins. With the then-statement on the next line the compare's `lui at` has
the lower line and goes first, exactly as in retail. One edit:
```
                if (D_803B9BC8[p][i].angle > 1.5707964f)
                    D_803B9BC8[p][i].angle += CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, 3.1415927f - D_803B9BC8[p][i].angle);
```
Then (w36a, second pass) the two unused locals `xx1, xx2` that w35b needed for the 248-byte frame are GONE: MAT3 calls
`createGridMap()` at the top of the `gNumPlayers == 1 && gRace != 2` block, and image A has a caller-less `jr ra; nop` at
0x8039EF00 right before showCarSelect. Defining `void func_8039EF00(void) {}` (non-static, not kept, so umerge inlines it and
still emits its body with a symbol) and calling it there (with `show` block-local, as in MAT3) reserves exactly the 8 frame
bytes; EF08 and func_8039EF00 both MATCH. As a `static` the deleted stub lands after func_8039ED20 without a symbol and ED20
reports `1 extra words` (the w17j packaging trap), so it stays non-static.

Caveat for integration: EF08's code depends on the E444 context stand-in's convention (register parameter p in $s6, every
callee-saved register clobbered). If w36b replaces func_8039E444 with a real body, rescore EF08 in the merged group.

## func_803A1EAC = updateCarScreen: closed (28 -> 0 words)

Start: w35c's MAT3 port (28/1776, only the two ghost-compaction tail loops differ: retail hoists `lui v1; addiu v1` = &gGhosts
in front of each tail loop, ours rematerialises it inside). The lever is in the ghost-file loop at the top of the function:
```
        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
            ghost = *ghosth;
            nextGhost = ghost->next;
            if (ghost->file && !func_800A1A3C((*ghost->file)->controller)) continue;
            if (ghosth == D_80152698[i]) break;
        }
```
instead of MAT3's single `if ((!ghost->file || PS_PakInserted(...)) && ghosth == gGhosts[i]) break;`. The first loop's code is
byte-identical either way; the difference is the order of uopt's flow-graph chain. Traced with an instrumented uopt
(w35c's `tools/patch_fsplit.py` + a w36a print in `f_formlivbb`, built in `~/rush2049/scratch/frontier/w36a/dbg`):
- `f_makelivranges` appends live units in the order of the flow-graph chain (0x1001c8f8 list). That chain is a DFS that follows
  each block's FIRST successor and visits second-successor edges last (block 2, the else-arm of the first `if`, is the last block).
- `f_split` (lda web &gGhosts, nocs 96, totalsave 180 < callee cost 2224) takes as seed the first live unit with uses that is not in
  an all-clobbering block. With the single condition the compare block bb19 is reached by first-successor edges and leads the list
  (13:0, 19:1, 387:1, 393:1, cases 318..314): seed bb19, the first-loop piece (nocs 6) and the remainder (nocs 91, save -0.44) are
  both unprofitable, so the tail is rematerialised everywhere.
- With the `continue` form the compare block hangs off a second-successor edge and moves to the END of the list (13:0, 388, 394,
  319, 320, 317, 318, 315, 316, 19): seed = first tail loop, piece {388, 394} (nocs 3, totalsave 19) is coloured v1 (exactly
  retail's tail), the remainder keeps a positive save and splits on harmlessly (w806 ... w840).
Also tried before finding it (no effect, 56 rows): compare operand order, `while`/`if (..) do..while` loop forms, nested `if`,
tail loops on `p`; `goto found` and a code-free `if (gGhosts[i]) {}` holder broke the function.

## Merge test with w36b (read-only use of their file)

w36b matched func_803A0498 (setGhosts) in their copy of the group. My group with their body swapped in
(`python3 tools/rep.py groups/dot_cleanup_colortrace_20261011 g/merge_b2 func_803A0498=../w36b/func_803A0498/best.c`, test dir
`g/merge_b2/`): all 13 members MATCH (the 8 old ones, EF08, A0FD8, A1EAC, EF00, A0498); only func_8039E444 (stand-in) differs.
So the two revisions compose without any head change. When func_8039E444 gets a real body, rescore EF08 (its caller).

## Shared-declaration changes (for merging with w36b)

Relative to the INSTALLED head (src/ovl_a/groups/dot_cleanup_colortrace_20261011/group.c), all inherited from w35b + w35c (mk.py):
- `extern s8 D_80110E78[][13], D_8011103C[][13], D_80111080[][13], D_8011119C[][13], D_80111230[][13], D_8011128C[][13];` and
  `extern s8 D_80111604[][13], D_80111648[][13], D_8011168C[][13], D_8011157C[][13], D_801115C0[][13];` (were `[13]`; rows p+1,
  row 0 = defaults; w35b's form, which w35c's A1EAC body also uses);
- `extern u32 D_801174BC, D_801174B4;` (was s32);
- `extern void resource_type_select(u32);` (was s32);
- `extern u32 object_manager_update(u8 *, s16);` (was s32);
- `PreparedCar.color` is `s8 color[3]` (was u8);
- `func_8039E444` added to context (w35b stand-in).
No w36a-specific head change. w36a's edits are inside the function bodies (line breaks in EF08/A0FD8, the dead first-loop `pl`
assignment in A0FD8, the `continue` in A1EAC, the createGridMap call + block-local `show` in EF08) plus ONE new definition,
`void func_8039EF00(void) {}`, placed just before func_8039EF08 (after EF08's extern block).

## Identities proven

- func_8039EF00 (image A, 8 bytes, `jr ra; nop`) = createGridMap@carscr.c, EMPTY in the N64 build. Evidence: MAT3 showCarSelect calls
  createGridMap() immediately before showGridMap (= func_8039ED20); the N64 stub sits directly before showCarSelect (func_8039EF08,
  its only caller in the IDO procedure order); inlining an empty function at that call reproduces showCarSelect's retail frame
  (248) exactly and both functions match in the group.
- Confirmed by matching: func_8039EF08 = showCarSelect, func_803A0FD8 = _InitScreen, func_803A1EAC = updateCarScreen (carscr.c).

## What generalises

1. **as1 breaks ties by source line.** Both as1 decisions here (EF08: which free instruction the cross-block pull puts in a stall
   slot; A0FD8: which store fills an unrolled loop's branch delay slot) went the retail way once a statement was split over two
   lines (`if (c)` / then-statement; `for (...)` / body). One-line MAT3-style statements tie on line number. w36b found the same
   for setGhosts' shift loops. Try line breaks first on any small as1-order residual.
2. **Spill-temp homes follow expression-bit order** (first appearance in uopt's expression table, first fit by block conflict):
   a home that is "one slot off" for a strength-reduced pointer can be fixed by an earlier (even dead) reference to the same
   expression. Trace with `tools/trace/spill.sh` + `pretrace.sh` (bit numbers) before adding locals.
3. **uopt's live-unit lists follow the flow-graph chain**, a DFS that takes first successors first and second-successor edges
   last; `f_split` seeds at the first eligible unit. Whether a condition is written as `a && b` or `if (!a) continue; if (b)`
   reorders that chain without changing the code of the loop itself, and decides which piece of a split constant/address web gets
   a register elsewhere in the function (A1EAC tail, 28 words). Instrumented trace: `tools/patch_fsplit.py` (w35c) plus a
   formlivbb print (`~/rush2049/scratch/frontier/w36a/dbg/src/uopt.dbg2.c`).
4. **A caller-less `jr ra; nop` next to a function is an empty inlined helper that reserves 8 frame bytes**: it replaced two
   unused locals (EF08). Check the stubs next to any function whose frame needs filler words.

## Tools and files (lane)

- `tools/rep.py BASE OUT FN=body.c ... [--sub OLD@@NEW]` (replace function bodies in a group, promote to members), `tools/rg.sh DIR
  [--claims]` (score a group on the builder), `tools/sf.sh DIR FN..` (summary lines), `tools/ad.sh DIR FN` (aligned diff via
  `adiff.py`, w35c), `tools/hb.sh` / `tools/hv.py` (A1EAC variant batches), `tools/rocheck.py` (w35b, float literals vs image A).
- `tools/patch_fsplit.py` (w35c, lda.txt path pointed at the w36a scratch) + `tools/patch_formlivbb.py` (w36a): instrumented uopt,
  built at `~/rush2049/scratch/frontier/w36a/dbg/bin/uopt`; run `ctrace.sh` with `SCR=rush2049/scratch/frontier/w36a/dbg`.
- Trace outputs went to the toolkit's default `build/trace/w36a/` (git-ignored build dir).
- `a0/`, `ef/`, `hx/` variant sources; `g/` scored test groups (`g/merge_b2` = merge with w36b's setGhosts).

## Permission denials

(none)
