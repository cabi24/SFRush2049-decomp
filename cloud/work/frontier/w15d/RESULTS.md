# w15d results

Flags: `-g0 -O3 -mips2 -G 0 -non_shared`. Unit scores via `tools/trace/us.sh` (TAG=w15d, whole-program blob_unit).
Builder scratch `~/rush2049/scratch/frontier/w15d` (toolkit installed with `--reuse wtk`).

## func_800A79F4 (240 B)
- Start: w14z best.c = unit `FAIL func_800A79F4: 4 of 60 words differ | words 4 ops 0 norm 0 | frame 0/0`.
  Residual: the two ugen ring temps for `w-1`/`h-1` (t6/t7) swapped, and the hm1/wm1 store order.
- Trace (ugt.sh): ugen evaluates `w-1` first (t6) because the source writes wm1 before hm1. Retail has `h-1` in t6,
  so retail's ugen evaluated hm1 first. But swapping the statements (swap.c, 7 words) renumbers the param webs:
  h is now the first-appearing web and takes t1 (phase 2 colours in web-number order).
- Retail therefore needs: w's web numbered before h's AND `hm1 = h - 1` evaluated before `wm1 = w - 1`.
- a2.c (= swap.c + `if (w) {}` before hm1): w is web 53 -> t1 again, but h (web 56) gets v0
  (its range no longer interferes with v0's web: nocs 2 -> 1). force.sh `p2:w56=c9` (h -> t2) gives
  `differing rows 0` => a2's residual is colour-only (one web).
- pretrace.sh (uopt listing): the store block is TWO uopt blocks (12, 13); block 13 holds the return. A web live
  in block 13 cannot take v0 (the return value). In best.c w and h are both in blocks 12+13 (nocs=2, t1/t2);
  in a2 h is only in block 12 (nocs=1) and takes v0. Where uopt splits the long store block depends on the
  statement mix (not a fixed count), so source order alone (b5: 360 permutations of w/h/hm1/wm1/state/f15) gets 2.
- Lever: an explicit block end. `s->w = w; s->h = h;` (w numbered first, both live into the last block),
  then a compiled-out block split, then `hm1, wm1, state, f15` (h-1 evaluated first -> t6; as1 keeps that store
  order). b7: 7 of 54 variants strict 0; b8: `do {} while (0);`, `if (h <= 0) {}`, `if (s == 0) {}`,
  `if (i < 0) {}`, `if (1) {}` all 0; an unused label does not split (20).
- **MATCH** -> `cloud/matches/func_800A79F4.c` (shaping device disclosed: empty `do {} while (0);`).
  - `score.py fn cand_A79F4.c func_800A79F4 --flags "-g0 -O3 -mips2 -G 0 -non_shared"` -> `MATCH`
  - `blob_unit --tag w15d score func_800A79F4 --with cloud/matches/func_800A79F4.c --neighbours` ->
    `EQUAL func_800A79F4: 60 words (kept, c_func_800A79F4.c)` / `locked bodies that differ in this unit: 0` /
    `blob_unit score: 1/1 equal`
  - Integration: plain single (no overrides, no group). It is the sole blocker of func_800B3704.

## func_800E7A98 (172 B) — MATCH (unit + group; not standalone)
- Re-scored drafts in the unit: w11a/w12e best `FAIL func_800E7A98: 14 of 43 words differ | words 16 ops 1`.
  (w14q b1/v0026 now scores 28 words / 42-word body in the current unit.)
- Lever (from the locked sibling src/blob/sound_play_menu.c): the default-heap choice is the static helper
  `heap_or_default(heap)` (`if (heap != 0) return heap; return D_801527C8;`), inlined by umerge. Its inlined
  return value is the v0 web; `h = heap_or_default(heap)` is the separate web that starts at the join and is
  coloured a1 (retail's `move a1,v0`). Hand-written copies were always copy-propagated (w11a).
  h2.c (helper + for loop): 5 words; the only residual was the parameter web (retail `move a3,a0` / `sw a3` /
  `lw a3`; ours `sw a0,32(sp)` / `lw v1`). Adding w11a's compiled-out `if (heap == NULL) {}` before the lock
  (h5.c) or `if (heap) {}` after it (h6.c) -> EQUAL.
- **Shaping device (disclosed):** compiled-out `if (heap == NULL) {}` before osRecvMesg. Best form without it:
  h2.c, 5 words (param web uncoloured).
- Deliverable: group `cloud/work/frontier/w15d/groups/func_800E7A98/` (claims [func_800E7A98]; files
  func_800E7A98.c + heap_release_context.c = unchanged copy of the locked codex_heap_release_a25/group.c as
  context; keep = func_800E7A98 + that group's kept functions; audio_reverb_update internal, so it takes a1/a2).
  Supersedes nothing; no unit_overrides. Not in cloud/matches (standalone, audio_reverb_update is external).
  - `blob_unit --tag w15d score func_800E7A98 --with cloud/work/frontier/w15d/groups/func_800E7A98/func_800E7A98.c --neighbours`
    -> `EQUAL func_800E7A98: 43 words (kept, c_func_800E7A98.c)` / `locked bodies that differ in this unit: 0` /
    `blob_unit score: 1/1 equal`
  - `score.py group cand/g7a98` -> `func_800E7A98: MATCH`; all context functions MATCH except one context body
    reported `MISMATCH (1 extra words (nonzero beyond target length))` (audio_reverb_update with this file order;
    synced_model_render with the other order) - a layout artifact of the trimmed context set; the locked
    codex_heap_release_a25 group itself scores all MATCH. Integrator: if install_group objects to it, add the
    group's remaining files (alloc_at.c, ...) as context, or fold func_800E7A98 into codex_heap_release_a25
    (same name, all old members).

## func_800AC9BC (224 B) — MATCH (standalone and unit)
- Start: w14h best.c `FAIL func_800AC9BC: 25 of 56 words differ | words 25 ops 19 norm 19` (unit, current tree).
- Semantics re-derived: it is the arcade `downleaf()` (game/stree.c) - quadrant from the bounds midpoints,
  descend while the leaf-mask bit for q is set, child index 0 = no node (return 0), else store q, return node.
- Steps (each traced against the ugen listing, ugt.sh):
  1. Recursive tail call (r1.c): prologue/frame/param homes exact; uopt turns it into the loop but cfe re-truncates
     the s16 x/y on every recursive call (extra sll/sra). Written as a loop instead (l1.c): 18 words, body length right.
  2. Retail's halving is `bgezl s; addiu s,s,1; sra` (in-place, a source-level branch turned branch-likely by
     as1), found nowhere else in the game; `/ 2` gives as1's `bgez; sra; addiu at; sra`. Written out as
     `if (s < 0) s++; s >>= 1;` (b1, 288 variants): 8 words, then midh inline in the compare (b2): 5 words, 0 ops.
  3. Remaining 5 words = ring temps t6/t7 swapped: ugen emitted `midv >> 1` before `midh >> 1`. Both shifts as
     statements after both sums (`midh >>= 1; midv >>= 1;`) puts midh >> 1 first (b4/v0004): EQUAL.
- **MATCH** -> `cloud/matches/func_800AC9BC.c`. No shaping devices.
  - `score.py fn cand_AC9BC.c func_800AC9BC --flags "-g0 -O3 -mips2 -G 0 -non_shared"` -> `MATCH`
  - `blob_unit --tag w15d score func_800AC9BC --with cloud/matches/func_800AC9BC.c --neighbours` ->
    `EQUAL func_800AC9BC: 56 words (kept, c_func_800AC9BC.c)` / `locked bodies that differ in this unit: 0` /
    `blob_unit score: 1/1 equal`
  - Integration: plain single (splice_singles.py), no overrides.

## func_8010FBE0 (128 B) — NOT matched, 6 word rows (from w14za's 7), cause traced
- Best `func_8010FBE0/best.c` (= s2.c): `FAIL func_8010FBE0: 30 of 32 words differ | words 6 ops 6 norm 6 | frame 24/24`
  (unit; the headline count is positional, the aligned rows are 6). Retail statement order: `D_80155238.next = 0;`
  first (struct field, via `lui at`), then `D_80155288 = &D_80152750; D_8015528C = 0; D_80155240 = 2;`, memcpy,
  two osJamMesg. w14za best2.c re-scored: `16 of 32 ... | words 7 ops 5`.
- Only residual: retail shares ONE `lui at` between the stores to 0x80155288 / 0x8015528C (+0x50/+0x54 of
  the OSScTask D_80155238). Traced (listing edits with asm.sh, force.sh):
  - as1 does not share `lui at` for two stores to an extern symbol (even `sym` / `sym+4` of the same symbol,
    with or without the .loc between): e1.s/e2.s edits, no change.
  - It does share when the symbol is DEFINED in the unit (precedent: locked func_800B2BDC.c defines
    `Node D_80138880[100];` and its unrolled stores share `lui at`).
  - With `OSScTask D_80155238;` defined (s7.c, all three stores through the struct) uopt makes D_80155238's
    address a coloured web (p1 w6 a3, tot 3.0 > best 2.1). Oracle: `force.sh s7 func_8010FBE0 "p1:w6=s"` ->
    `differing rows 0`. So retail = defined OSScTask + address web NOT coloured.
  - Not found: a source form that keeps tot <= 2.1 (next/msgQ/msg are three uses in the first block plus the
    `&D_80155238` jam argument after the call). Tried: next via cast / separate symbol, arg via `.next`,
    a separate `ScReply` pair symbol (2 uses -> tot 1 > best 0, coloured v0), volatile struct (12 rows), all-struct
    (s8, 15). Best next hypothesis: the field stores come from an inlined static task-init helper taking the task
    pointer (its parameter would be a variable web with different cost), or a different split of which fields go
    through D_80155238. Note defining a data symbol in the function file is an integration question (precedent above).
- Stopped by the stop rule (traced; 3 rounds of confirmation variants without strict improvement past 6).

## What generalises
1. **uopt splits a long straight-line block** (func_800A79F4: the last statements form their own block). A web
   live in the block holding the return cannot take v0. A compiled-out block end (`do {} while (0);`,
   `if (x) {}`) places that boundary; an unused label does not.
2. **Re-derive from the arcade and the locked siblings before sweeping.** func_800AC9BC is arcade downleaf();
   func_800E7A98's missing web was the inlined `heap_or_default` helper already in locked sound_play_menu.c.
3. **An inlined static's return value is a separate web** that copy propagation does not remove: it is the
   lever when retail keeps a real copy (`move a1,v0`) that every hand-written copy loses.
4. **`bgezl s; addiu s,s,1; sra` is a written-out signed halving** (`if (s < 0) s++; s >>= 1;`), not `/ 2`
   (as1 expands `/ 2` through at). Statement order of the shifts sets ugen's ring temps.
5. **as1 shares `lui at` between stores only for symbols defined in the unit.** Extern-only symbols get one
   `lui at` per store. Defining the data symbol (func_800B2BDC precedent) is the lever when retail shares.
6. Do not run two `us.sh`/blob_unit jobs with the same TAG at once: they share the stage dir (a background
   batch was stopped for this; the three matches were re-confirmed afterwards in one run:
   `blob_unit score: 3/3 equal`, `locked bodies that differ in this unit: 0`).

## Summary
| function | bytes | state | deliverable |
|---|---|---|---|
| func_800A79F4 | 240 | MATCH (standalone + unit); disclosed `do {} while (0);` block split | cloud/matches/func_800A79F4.c |
| func_800E7A98 | 172 | EQUAL in unit / group MATCH; disclosed compiled-out `if (heap == NULL) {}` | groups/func_800E7A98/ |
| func_800AC9BC | 224 | MATCH (standalone + unit); no shaping | cloud/matches/func_800AC9BC.c |
| func_8010FBE0 | 128 | 6 rows; colour/as1 cause traced (oracle 0 rows) | func_8010FBE0/best.c |
