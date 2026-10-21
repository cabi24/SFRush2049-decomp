# w35e results (MAT3-donor matching: image B StartWeapon / newWorldDamage, image A animText+drawView)

Builder scratch: watchman2 `~/rush2049/scratch/frontier/w35e` (copy of base; src/, include/, tools/cloud, tools/conveyor,
asm/us/{blob,blob_data,ovl_a,ovl_b} and the three lock files re-synced from the Pi at session start).
Nothing outside this lane dir and `cloud/matches/ovl_b/` was written.

## Summary

| function | image | GC identity | bytes | state | flags | scorer line |
|---|---|---|---|---|---|---|
| func_8038C910 | B | `StartWeapon` (weapons.c, GC 0x8005C7C0): CONFIRMED | 276 | **MATCH** (standalone) | `-g0 -O3 -mips2 -G 0 -non_shared` | `func_8038C910:` / `  MATCH` |
| func_8038A408 | B | `newWorldDamage` + inlined static `findDamageSlot` (worlddam.c): CONFIRMED | 1220 | provisional, **4/305 words**, frame 248 = retail only with 10 disclosed unused words | `-g0 -O3 -mips2 -G 0 -non_shared` | `4/305 words differ` |
| func_803AD57C | A | `animText` (recordscr.c, GC 0x8008451C) with `drawView` + `drawStuntTotals` inlined (correlation said drawView): REFINED | 2092 | near-match, group: strict `497/523` (one extra dispatch word shifts everything), 35 words in aligned runs, frame 312 = retail | `-g0 -O3 -mips2 -G 0 -non_shared` (group) | `497/523 words differ (1 extra words ...)` |

Deliverables: `cloud/matches/ovl_b/func_8038C910.c` (strict MATCH); `func_8038A408/best.c`;
`func_803AD57C/best/{group.c,group.json,root_function.c}` (full group dir, members untouched, all 11 members MATCH).

## 1. image B func_8038C910 = GC StartWeapon  -- MATCH

- PR #364 called it a "callback initializer" (13/69 words). It is MAT3 `StartWeapon(slot)`:
  `v = &gGameCar[slot].visuals[N]; if (v->objnum < 0) { v->index = 0; v->func = AnimateWeapon; v->timeStamp = gSeconds;
  v->slot = slot; v->objnum = gCarParts[slot].weap = InitCarObj(gObjList[531], 15, gCarParts[slot].frame, 0x2000, slot, -1, 1);
  ZOID_UpdateObject(v->objnum, weaponOffsets[1][model[slot].body_type], 0); }`.
  N64 callees: func_800AF5E0 (locked label save_slot_valid, `docs/external/gc/correlation.md` already lists it as
  `InitCarObj`), func_8008D6FC = ZOID_UpdateObject, func_8038AA8C = AnimateWeapon (w35f's lane).
- Score (the brief's command, builder): `python3 tools/cloud/score.py fn cand/func_8038C910.c func_8038C910 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"`
  -> `func_8038C910:` / `  MATCH` (the same file at `-O2`: 63/69 words differ; first typed port, first scorer run).
- Why #364 stalled at 13/69: its `*(f32 *)(address_word)D_8002EB90` round trips imitate MAT3's `extern volatile float gSeconds`.
  A plain `extern volatile f32 D_8002EB90;` gives retail's `lui/addiu/lwc1 0(reg)` pair and schedule; the rest is the straight
  typed port (chained `v->objnum = parts.weap = InitCarObj(...)`, MAT3 prototype `(s32, s16, s32, u32, s16, s16, s32)`,
  `void (*func)(tVisual *, s16)` in the visual).
- N64 adaptations: weapon visual = visuals[20] (car+0x2F0; 21 visuals at +0x110, GC 22), tCarParts 0x40 B with weap at +0x18,
  model[] stride 0x808 with body_type at +8, weaponOffsets `[8][13][3]` at 0x803943A4, gObjList = u16 array at 0x8014254C
  (index 531 -> 0x80142972). Also the types the w35f lane needs for AnimateWeapon: tVisual {next, s16 index, s16 objnum, s16 slot,
  u32 data, f32 timeStamp, func} (0x18).

## 2. image B func_8038A408 = GC newWorldDamage  -- 4 words + frame residual

- Identity: worlddam.c `newWorldDamage(float *pos, int slot)` (GC 0x8005E9DC) with the static `findDamageSlot()` (GC 0x8005E880) inlined by
  umerge; the caller-less `jr ra; nop` at B:8038A400 is its deleted-procedure stub. N64 callees: camera_trigger_check (func_800C4200) =
  RoadPosition, func_800A78BC = ZOID_NewPoly, func_8008C074 = ZOID_UpdatePoly, func_8008C874 = ZOID_HidePoly. The N64 body has no
  trailing ZOID_SetPolyFlags. Ring D_80399A70 {s8 full; u16 oldest; poly *polys[25]} (0x68), D_80399AD8 damTexture, D_80394888 dummyVtx[4][3],
  D_803948B8 damColor[5], model[].airdist[4] at +0x5EC (stride 0x808).
- Direct typed port, with the Dreamcast reconstruction's EDGE_CHECK shape (`reference/repos/dreamcast/src/worlddam.c`: ONE function-level
  `float delta[3]` + `edge_length2`; the single shared array is why only the LAST edge keeps its three stores in retail: four block-local arrays
  keep all twelve, tested) and the house `fabsf` intrinsic: first draft 55/305 words, every difference a stack offset (frame 208 vs 248).
- Frame: retail Udef is 188 B (3 temps, 248 total); this source's is 148 B. Ten or eleven unused words are needed (nine give 240). They fit
  exactly between the caller's function-level locals and the inlined callee's locals (declared after `m`, or first in findDamageSlot): in either
  place the result is the same 4 words. `best.c` declares ten named unused `s32` (disclosed in the header with the arithmetic). Their owner is
  unknown: candidates are block-scope locals of the original macro (GC's EDGE_CHECK declares `float dx, dy, dz;` per edge, invisible to DWARF)
  but no dead-scalar arrangement tried (per-edge 1/2/3 floats or ints + remainder; 288 combinations) changes anything but the frame.
- The 4 remaining words (+0x310..+0x324): as1's second (cross-block hoist) pass pulls `lui v1`/`addiu v1,v1,-26000` (ring address) out of the
  successor block; retail leaves the `addiu` last in the block (after `mul.s f4`), ours places it right after the `lui`, ahead of the delta
  stores. Traced in `build/trace/w35e/as1r_pb.func_8038A408.log`: the newly-ready `addiu` goes to the front of as1's ready list and wins the
  zero-priority tie. Independent of: pad shape/placement, ~500 source variants (comparison direction, operand order of the squared sum,
  inline vs named `edge_length2`, static findDamageSlot order/linkage, explicit `&xyz[0]`), and of where `la` sits in the ugen listing.
- Standalone 'edge as a static function' is not what retail does (4 call sites: umerge refuses, and the unit would need 4 stubs).

## 3. image A func_803AD57C = GC animText (drawView, drawStuntTotals inlined)  -- near-match

- Identity. The correlation's `drawView` (GC 0x80081C78, 0x568 B) is the dominant inlined body, but the function is the root callback
  `animText` (GC 0x8008451C): `HAL_SetTextDepth(0.0f); switch (gRecordState) { case 0: drawSelect(); case 1: drawView(); }
  HAL_SetTextDepth(-1.0f); return 1;`. drawSelect stays the call func_803AA3D8, drawView is inlined by umerge (stub func_803AD574, 8 B,
  right before it), and its kViewStuntTotals arm inlines drawStuntTotals (GC 0x80082B30); the other 11 arms are `jal`s to the already
  matched stats-menu members. N64 labels: render_helper = HAL_SetTextDepth, func_800B669C = TB_SetJust, brake_light_update = ZOID_ProjectWindow,
  func_800F6928 = HAL_SetMenuTextDepth, dispatch_handler = TB_SetTextColor, object_create = HAL_SetFont, music_fade = TB_DrawEllipseText,
  object_bytes_sum_global = TB_GetFontLineHeight, func_800ED66C = HAL_SetTextTranslucency, func_800BEA3C = TB_FlashTextColors (two 4-byte
  structs by value), state_utility = TB_DrawText, sound_pitch_diff_calc = TB_RightText, fcvt_wrapper = TB_Sprintf, crowd_cheer_play =
  ChangePaneSize, countdown_state = gText (single +4, tab +12, taba +16), D_803B5F28 = buttonInfo[] (0x40 stride, position at +0x30).
- Method: `score.py group` only (alone the function is 505/523: it needs the stats-menu group's IPA callees). Base = the existing
  `src/ovl_a/groups/runtime_a_stats_menu_20261006` (348/523, frame 320 vs 312); only func_803AD57C replaced, `drawView` made a NON-static function
  not on `keep` (as the brief says, so its deleted stub does not become "extra words" of func_803AD0D4); the 11 member bodies untouched and
  all still MATCH.
- Measured steps (difflib non-equal-run words; strict score is position-aligned and hides this): previous best 348 (strict) ->
  1. `buttonInfo[i + 11].field` indexing (retail's `712(s2)`/`764(s2)`/`s2 += 64`; the prior `slot++` pointer was wrong)            181 -> 159
  2. GC local lists and order for drawView / drawStuntTotals (the prior draft had `str[40]` highest; GC order puts it lowest)      frame 312
  3. a function-level `track` for `gRecordTrack > 12 ? gRecordTrack - 6 : gRecordTrack / 2` (without it cfe shares ONE temp slot between the
     switch scrutinee, both `?:` and that expression -> one merged web, scrutinee loses v0)                                       159 -> 155
  4. `str = NAME(..); TB_DrawText(x, y, str)` for both label draws and `str = ... ? ... : ...; TB_DrawEllipseText(cx, y, 140, str)` (retail
     evaluates the table lookup before the argument narrowing; one variable web a3)                                               155 -> 37
  5. real structure instead of hand inlining: `static`-style functions inlined by umerge. drawView as a real function gives, with NO invented
     locals: the 2 top words (per-inlined-call reservation, 8 B), `&str1` hoisted into s7 for the loop (retail), x/y/text homes at 188/192/144;
     `track` is no longer needed. Hand-inlining drawStuntTotals as well (it is a nested umerge inline, +6 words of overhead where retail has 4)
     and keeping GC's block-scope `Color4 c1, c2;` in BOTH arms of the flash-colour if/else (dead: the N64 call passes flashColors[0/1] by value,
     and as live struct locals they cost +19 words of stores) gives exactly retail's 4 mid words: frame 312, screenPos 216, str1 220.   37 -> 35
- Frame accounting (312): FP saves 32..79, int saves 80..119, temps 120..135, Udef 136..312. Natural locals + the dead GC pair; nothing else
  invented (the earlier `pad_top[1]` and `track` were artefacts of hand-inlining).
- Remaining differences (all measured; not claimed colour-only): (a) as1 hoists the state dispatch's `li at,1` into the `lw v0` stall and leaves a
  `nop` in the `beqz` delay slot (+1 word and a one-word shift of everything after: strict 497/523); retail keeps `li at,1` in the delay slot as in matched
  func_8039C140 / entity_spawn_init. 14 toy variants of the same switch (cases 0/1, 3 cases, 4 cases, short/unsigned scrutinee, with/without
  default, if-chain) all hoist, `.loc` markers and an explicit `li $1,1` in the ugen listing do not change it; the matched retail cases have register-
  based loads or extra hoistable instructions from the taken target. (b) loop head: `addiu s0,s2,752` / `lui at,0x4f80` 1-2 slots earlier than
  retail. (c) the last `lw a2,0(t7)` is the delay-slot filler in ours but is scheduled before the arg moves in retail (zero-priority tie in as1's list,
  `aftercycles 0`). (d) spill temps: webs 70/168 land in temp3, 158/164 in temp1 where retail uses temp2/temp0: sp offsets 120/128 vs 124/132 (8 words).
  (e) the jump-table address is own .rodata (unverified relocation, not a difference).
- 384 mechanical variants of the loop (operand order, comparison direction, continue/else, indexing form, conversion casts) and 48 more on the
  button indexing form: all stay at 35.

## What generalises (MAT3 -> N64 IDO matching)

1. Take MAT3's declarations literally, including `volatile` (gSeconds: IDO then emits `lui/addiu/lwc1 0(reg)`), parameter and local TYPES (s16 params
   with int arguments), local declaration ORDER, and block-scope locals that DWARF omits (tRGBA c1/c2, the EDGE_CHECK floats): block locals are
   allocated after function-level ones and show up as "unexplained" frame words.
2. Write a once-called static as a real static (or non-static, not on keep) function and let umerge inline it. Hand-inlining changes the frame
   (inlined call = 8 B reservation + the callee's locals appended below the caller's), loop-invariant address hoisting (`&local` into a callee-saved
   register only appears with the real inline), temp-slot numbering and evaluation order. Nested inlines add their own overhead (6 words measured).
3. cfe gives switch scrutinees and `?:` results the same stack temp slot, so they become ONE variable web; a named variable in the source separates
   them (retail's scrutinee in v0, `?:` temps elsewhere).
4. `str = expr; call(x, y, str)` and `call(x, y, expr)` differ in evaluation order after cvtl narrowing (arguments typed `s16` with `int` variables):
   retail's shapes were reached only by assigning.
5. Frame gaps of 40 B in a function that inlines a static are not necessarily missing inlined callees: check umerge `-v` output, the stub list
   (`jr ra; nop` functions in asm/us/ovl_*), and block-scope locals first.
6. as1's second (cross-block hoist) pass is deterministic but source-opaque: the same `li at,1`/lw-stall pattern and `la` placement are reproduced
   by 14 toys; if the residual is only these holes after the structure is right, report it rather than keep varying statements.

## Tools written (this lane, all under `tools/`)

`odis.py IMG FN` (overlay disassembly), `sc.sh IMG file FN [flags]` (builder score), `dump.sh`/`cmp.py` (compiled disassembly),
`gd.sh`/`gdiff.py` (full aligned diff for a group or single file), `gsc.sh` (group summary), `gb.sh`/`gbatch.py` (rank root-function variants
inside a group), `vb.sh`/`bscore_o.py` (vbatch for overlay targets), `gdp.sh`/`gdump.py` (sp-offset census), `um.py` (umerge -v inlining log),
`asmo.sh`/`oscore.py`/`forceo.sh` (assemble an edited ugen listing / forced-colour object against an overlay target), `mkgrp.py`.
No permission denials occurred.
