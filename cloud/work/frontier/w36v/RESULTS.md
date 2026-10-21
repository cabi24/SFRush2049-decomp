# w36v RESULTS (wave 36, save/input/camera, game population)

Status table (updated after every function):

| function | addr | bytes | GC identity | state | best file |
|---|---|---|---|---|---|
| PS_CreateFile | 0x800A2D4C | 1752 | paksysdc.c PS_CreateFile (N64 osPfs body; GC body is PS2-memcard, structure from PR #410 + retail) | PROVISIONAL group MATCH (score.py group: PS_CreateFile MATCH, no_catchup MATCH; unit EQUAL 2/2, 0 locked differ) — needs PS_PakServer (w36p) as no_catchup's 2nd caller | groups/w36v_ps_createfile/ |
| input_process_controller | 0x800AD734 | 1452 | stree.c CylinderInSurf (proven: GC body = installed context logic, sole caller ShotToWorldCollision) | MATCH (group): already EQUAL in the unit as unclaimed context; superseded group claims it | groups/frontier_shot_world_collision/ |
| menu_input_process | 0x800B6BEC | 1280 | HAL_text.c HAL_DrawText (proven: 800-byte buffer, opcodes 0..6, callees) | draft only; partners _DrawText (audio_doppler_calc, w36t, blocked by func_80087110) and _TextLen (func_800B66B0, provisional) are IPA-internal | menu_input_process/v/h1.c |
| CreatePlayer | 0x800CD104 | 1088 | save.c CreatePlayer (w28b) | not matched; prior best unchanged (w32c lead 256/272 words, 120 rows; installed context 241/272) | w32c/CreatePlayer/lead_f2.c |
| camera_auto_follow | 0x800BE078 | 1084 | TB_text.c TB_DrawTextScrollBox (confirmed: callees, structure) | MATCH: standalone score.py fn MATCH + unit EQUAL 271/271, 0 locked differ | cloud/matches/camera_auto_follow.c |
| drone_set_catchup | 0x800A2990 | 852 | paksysdc.c PS_ReadPartialFile (fileh, offset, numBytes) (N64 osPfsReadWriteFile body) | not matched: unit 114 rows (frame 208/216); fileh-colour residual | drone_set_catchup/best.c |
| camera_look_at_point | 0x800BF45C | 796 | targetpaths.c _PlayMoveSound (proven: callee roles, moveSnd 53->1, 83->18, 140->97) | not matched: best = installed context 38/199 (ops 0, colour + ugen ring); GC port 66 words | camera_look_at_point/v/l1.c, b1/b1_m1.c |
| pak_delete_file_retry | 0x800A2680 | 784 | paksysdc.c PS_DeleteFile (N64 osPfs body) | not matched: unit 63 rows (prior 98), frame 224/232 | pak_delete_file_retry/best.c |

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36v (copied from base, master e68aea6e1).

## Identities proven
- PS_CreateFile = paksysdc.c PS_CreateFile (N64 osPfs version); no_catchup = one-parameter paksysdc.c helper
  `InsertLLE(&gPS_Files[file->pak], fileh, head)` (IPA: fileh in a1, no a0; callers PS_CreateFile, PS_PakServer).
- drone_set_catchup = paksysdc.c PS_ReadPartialFile(fileh, offset, numBytes) (osPfsReadWriteFile read with retry,
  error 9 -> PS_DeleteFile; sets files[n].flag2 = numBytes != size).
- pak_delete_file_retry = paksysdc.c PS_DeleteFile (osPfsDeleteFile with retry; RemoveFile callback; handle back to
  the free list D_801460E0).
- camera_auto_follow = TB_text.c TB_DrawTextScrollBox; object_bytes_sum_global = TB_GetFontLineHeight; camera_zoom_fov
  = TB_GetTextNumLines; camera_shake_update = HAL_GetCharWidth; object_manager_update = HAL_GetTextWidth;
  func_800BDEB0 = HAL_GetFontSpacing; func_800B70EC = HAL_SetTextPos; D_80118E20 = gJust {h, v}.
- menu_input_process = HAL_text.c HAL_DrawText; func_800B66B0 = _TextLen; audio_doppler_calc = _DrawText (IPA text s2,
  length s0); func_800B6748 = _SetDefaults; D_80114744 gDepth, D_8011473C gFinished, D_80114748 gTrans, D_8011474C
  gBufferInited, D_801461D0 text semaphore queue, D_80149B48 gFontColor, D_80149B60 gLeading, D_80149B70 gSpacing,
  D_80149D92 gFontXpos, D_80149D9E gFontYpos, D_80149DA0 gFontIndex, D_80149DD0 gBufLen, D_80149DD8 gBuffer[800],
  D_80153F60/D_80153F80/D_80153FD0/D_80154180/D_80154184/D_80154194/D_8015419C = HAL_DrawText's statics lastFontColor/
  lastTrans/lastFontXpos/lastFontYpos/lastFontIndex/lastLeading/lastSpacing.
- camera_look_at_point = targetpaths.c _PlayMoveSound (Camera = tTarget: sound 0x60, sndState 0x64, infoIdx 0x10, pos
  0x38, data 0x6C); camera_aspect_ratio = _PlayStartSound, camera_fov_control = _PlayStopSound, camera_clip_planes =
  AudUpdateEmitter, leaderboard_update = AudEmitterPlaying, results_screen_update = AudStopEmitter, camera_target_track
  = EmitGameSound, D_80117530 = targetInfo (0x30: startSnd 0x1C, moveSnd 0x20, stopSnd 0x24, replace 0x28,
  fadeDist 0x2C), D_801141B0 = unitmatrix[3].

## Permission denials

(none so far)

## PS_CreateFile (0x800A2D4C, 1752 B) — EQUAL in the unit given its partners (provisional: stand-in caller)

Deliverable: `groups/w36v_ps_createfile/` (group.c + group.json, claims empty, provisional_claims PS_CreateFile +
no_catchup). Builder `score.py group cand/groups/w36v_ps_createfile`: `PS_CreateFile: MATCH`, `no_catchup: MATCH`,
context `func_800A1E94: MATCH`, `w36v_standin_caller: NOT VERIFIED (no target section ...)`.
Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w36v score PS_CreateFile no_catchup --with
cloud/work/frontier/w36v/groups/w36v_ps_createfile/group.c --internal func_800A1E94 --internal no_catchup --internal
paksys_queue_init --internal paksys_lock --internal paksys_unlock --internal pfs_get_node --keep w36v_standin_caller
--neighbours` -> `EQUAL PS_CreateFile: 438 words (kept, c_group.c)`, `EQUAL no_catchup: 14 words (internal,
c_group.c)`, `locked bodies that differ in this unit: 0`.
Integration needs: force_internal func_800A1E94 (verified harmless: 0 locked bodies differ) and no_catchup (its locked
single `no_catchup(s32 arg0, s32 *arg1)` superseded by the one-parameter body; same 14 words); the stand-in replaced
by PS_PakServer (track_render_process, w36p). Standalone `score.py fn` cannot match (no IPA: a0 load + t0 spill).

`blob_unit --tag w36v score PS_CreateFile no_catchup --with cloud/work/frontier/w36v/PS_CreateFile/v/v21b.c
 --internal func_800A1E94 --internal no_catchup --keep w36v_standin_caller` ->
`EQUAL PS_CreateFile: 438 words (kept, c_v21b.c)` / `EQUAL no_catchup: 14 words (internal, c_v21b.c)`;
with `--neighbours`: `locked bodies that differ in this unit: 0`.
Levers found (in order): frame/locals = retail declaration order (error, file, game_name[16], ext_name[4], retry,
status, node, request, state) with NO `pak` pointer local (every access `D_80144030[port].x`, volatile array);
success path inside `if (error) {...} else break;` (not `goto ok`/`break` after a test: PRE otherwise inserts the
`&D_80144030[port]` computation on a separate exit-edge block); node allocation = inlined static helper (retail stub
0x800A2D44) written `node = D_801460E0.head; func_8009211C(&D_801460E0, D_801460E0.head); return node;` (loads into
a1 and copies to s3 like retail); IPA: func_800A1E94 must be INTERNAL (t0 survives the call) and no_catchup must be a
ONE-parameter internal function `no_catchup(PakNode *node)` (IPA passes node in a1, no a0: retail's two callers never
set a0). no_catchup is inlined by umerge when it has one call site, so it needs its second retail caller
(track_render_process = PS_PakServer, lane w36p, unmatched) — here a stand-in. Not spliceable until that caller is in
the unit (or accepted as unmatched context).

## camera_auto_follow (0x800BE078, 1084 B) = MAT3 TB_text.c TB_DrawTextScrollBox — standalone MATCH

`score.py fn cand/camera_auto_follow.c camera_auto_follow --flags "-g0 -O3 -mips2 -G 0 -non_shared"` ->
`camera_auto_follow:` / `MATCH`. Deliverable `cloud/matches/camera_auto_follow.c`.
Unit: `blob_unit --tag w36v score camera_auto_follow --with cloud/matches/camera_auto_follow.c --neighbours` ->
`EQUAL camera_auto_follow: 271 words (kept, c_camera_auto_follow.c)`, `locked bodies that differ in this unit: 0`.
(The file also defines the locked getters func_800BDEB0 / func_800B70EC verbatim, as camera_zoom_fov.c does.)
Identity: callees object_bytes_sum_global = TB_GetFontLineHeight, camera_zoom_fov = TB_GetTextNumLines,
camera_shake_update = HAL_GetCharWidth, object_manager_update = HAL_GetTextWidth, menu_input_process = HAL_DrawText;
func_800BDEB0 = HAL_GetFontSpacing, func_800B70EC = HAL_SetTextPos (both inlined, defined in the file verbatim).
Levers: first typed port had frame/homes exact and 20 ops rows; gJust as a struct {h, v} (folded lui/lw instead of a
hoisted address), spacing via the inlined getter (v0), the justified x passed directly to HAL_SetTextPos as a nested
conditional with `s32 tw` the first function-scope local, and `str = (u8 *)s + charSize`.

## input_process_controller (0x800AD734, 1452 B) = MAT3 stree.c CylinderInSurf — MATCH as a group member (no new C)

The installed `frontier_shot_world_collision` group (PR #394, wave 34) carries a complete input_process_controller body as
CONTEXT (w1g/w12a reconstruction with the reordered parameters). Its only retail caller input_deadzone_apply
(ShotToWorldCollision) became a member in wave 34, so the body now compiles with its real caller:
- current unit (no --with): `blob_unit --tag w36v score input_process_controller ...` ->
  `EQUAL input_process_controller: 363 words (internal, g_frontier_shot_world_collision__group.c)`.
- builder `score.py group cand/groups/frontier_shot_world_collision` -> Members: `input_deadzone_apply: MATCH`,
  `input_process_controller: MATCH`; with `--claims`: `input_process_controller: MATCH`. Context unchanged (func_800AC9BC
  52/56 as before: inherited, unclaimed).
Deliverable: `groups/frontier_shot_world_collision/` = byte copy of the installed group, group.json changed only:
input_process_controller moved from context to members, `claims: [input_process_controller]`. Superseded IN PLACE (same
name, old member input_deadzone_apply unchanged). Note: group.c's comment above input_process_controller still says
"Real caller input_deadzone_apply is unmatched -> two stand-in call sites -> PROVISIONAL" — stale since wave 34 (left
unchanged so the file hash is the installed one; the stand-ins are prototypes only). The unit_overrides
prefer_definition input_process_controller entry stays as is. cloud/work/frontier/provisional.json's wave-1
`input_process_controller` entry ("its own caller input_deadzone_apply is a stand-in") can be retired.

## camera_look_at_point (0x800BF45C, 796 B) = MAT3 targetpaths.c _PlayMoveSound — not matched (colour/ring residual)

Context body installed in group camera_aspect_ratio (w12f/w16a; disclosed `tt` holder for the empty `bgez v1`
branch): current unit `FAIL camera_look_at_point: 38 of 199 words differ`, ops 0 (pure registers).
- Direct GC port (`v/l1.c`, static inline path_abs, single `ratio`): 66 words / 8 ops; macro-form abs
  (`b1/b1_m1.c`) 66 words / 3 ops (maxVel in v1, curVel in a0 as retail) but lacks retail's empty `bgez v1`
  (198 words). Intrinsic abs: worse. So the installed context stays the best base.
- force.sh on the context (`p1:w58=c25,p1:w60=c24`: computed ratio $f2, merged value $f0) -> 28 rows: the FP clamp
  colour is a two-web priority swap; the remaining 28 rows are ugen temp-ring offsets (FP constants 0.75/0.25 in
  $f8/$f6/$f4 vs ours $f4/$f8/$f10; int ring one register later in the else branch from +0x208), i.e. retail emitted
  one more int temp and a different FP temp order earlier — consistent with a different source form of the abs/ratio
  block than the `tt` holder.

## menu_input_process (0x800B6BEC, 1280 B) = MAT3 HAL_text.c HAL_DrawText — draft only (partner-blocked)

Identity proven from the words: gDepth >= 0 -> `_DrawText(text, _TextLen(text, numChars))`; gFinished -> strcpy/strncpy
into `str[256]` (sp+112); semaphore = osRecvMesg/osSendMesg on D_801461D0; `_SetDefaults` of the seven statics; the
size sum (2,2,2,5,5,5,+len+2) against 800; opcodes 0..6 written into gBuffer. Draft `menu_input_process/v/h1.c` (direct
GC typed port; N64 differences: spacing/position written without the GC halving, lastTrans stored before the bytes,
lastFontColor stored as one word). Standalone it is 320 words / frame 320 vs 376 / 117 ops — the residual is IPA:
retail passes `_DrawText`'s text in s2 and length in s0 and saves s0-s8 because the internal callee _DrawText
(audio_doppler_calc) uses callee-saved registers unsaved. Neither partner is in the unit: _DrawText is lane w36t's and
calls Astra's func_80087110 (off limits); _TextLen (func_800B66B0) is provisional (tiny_A26, closes only with this
caller). Not attempted further: needs a real _DrawText (or a register-faithful stand-in) first. The installed context
copy of HAL_DrawText (codex_defaults_a16, m2c-style) scores `FAIL menu_input_process: 280 of 320 words differ` today.

## pak_delete_file_retry (0x800A2680, 784 B) = paksysdc.c PS_DeleteFile — 63 rows (was 98), not matched

Unit: `blob_unit --tag w36v score pak_delete_file_retry --with cloud/work/frontier/w36v/pak_delete_file_retry/best.c
--internal func_800A1E94 --internal paksys_queue_init --internal paksys_lock --internal paksys_unlock --internal
pdf_heap_lock --internal pdf_heap_unlock --internal pdf_buffer_free --internal pdf_free_blocks --internal pdf_release`
-> `FAIL pak_delete_file_retry: 154 of 196 words differ; compiled body is 195 words, target 196`; udiff rows 63, ops 10,
frame 232/224. s0 list, s1 pak, s2 fileh now as retail (func_800A1E94 internal). Tried (no gain): 48 loop-shape
variants (b1, ubatch: best = base), extra inlined helpers for the delete call / list search / state (u2a-c), no state
pointer (u3/u4: frame exact 232 but 80 rows: the state address CSEs with the record address), state macros over psfile
fields (u6/u7, worse). Spill-temp trace (spill.sh): ours 6 coloured expression webs (rec, pfs, game_name, ext,
callback, list), retail 8 (retail order puts the callback web 4th, before game_name/ext). ctrace: ext tot 11 > callee
cost 10.75 takes s8 where retail colours pfs. Next: find the source form that makes `&...files[fileNo].state` an
expression web computed as ((pak*772 + fileNo*40) + 140) + D (u1's arithmetic) without a pointer local.

## drone_set_catchup (0x800A2990, 852 B) = paksysdc.c PS_ReadPartialFile — not matched

Unit (same --internal list): `FAIL drone_set_catchup: 136 of 213 words differ; compiled body is 212 words, target 213`;
114 rows, frame 208/216 (b2/e1.c: 124 words / 111 rows, ops 24). Retail keeps `fileh` in its home (sw a0,216(sp) at
entry, reloads for PS_DeleteFile and once into v1 before the handle search); ours colours it (ctrace w2 save 1.5 x 8 =
tot 12 > cost 11.2 -> s7), adding the s7 save (frame -8). Search loop as an inlined helper (b3/f1-3), search-then-if
(b2/e1-3): no fix. Named locals in retail order: psfile, pak, cur, error, fileNo, retry, status (homes exact relative
to the frame top).

## CreatePlayer (0x800CD104, 1088 B) — no improvement this wave

Re-scored in today's unit: w32c lead_f2 `FAIL CreatePlayer: 256 of 272 words differ; compiled body is 271 words`
(120 rows, ops 43, frame 224/224); installed context 241/272. New evidence: retail's `move a0,s7` (name copy walked by
a0++) and `move a3,s2` (attempt copy for sprintf) are set BEFORE the name loop, the signature of an inlined helper
taking (file, name, attempt) — but such a helper (memset + copy + filter + digits; `CreatePlayer/b2/h1-h3.c`, internal,
also with a non-colliding name) is NOT inlined by umerge even with a single call site (too big), so retail cannot have
it in that form. ctrace of the lead: i = w6 tot 1031, coloured a3 (caller-saved cost 10 for the memset crossing) where
retail has s0; constants 10/4/48 hoisted as retail but '.' (46) takes s6 where retail hoists '=' into a1 and '*' into
a2. Stub 0x800CD0F4's body is still unknown.

## Integration notes (for the coordinator)

1. `cloud/matches/camera_auto_follow.c` — standalone MATCH + unit EQUAL (0 locked differ). Also defines the locked
   getters func_800BDEB0 (HAL_GetFontSpacing) and func_800B70EC (HAL_SetTextPos) verbatim (camera_zoom_fov.c
   precedent); only camera_auto_follow is new. 1,084 B.
2. `groups/frontier_shot_world_collision/` — supersedes the installed group IN PLACE: sources byte-identical, group.json
   moves input_process_controller from context to members + `claims: [input_process_controller]`. score.py group MATCH
   (with/without --claims); unit EQUAL already today. 1,452 B. Retire provisional.json `input_process_controller`.
3. `groups/w36v_ps_createfile/` — PROVISIONAL (claims empty; provisional_claims PS_CreateFile, no_catchup). Needs:
   force_internal func_800A1E94 (verified 0 locked bodies differ), force_internal + prefer_definition no_catchup (new
   one-parameter body, same 14 words as the locked single), and a real second call site of no_catchup in place of
   `w36v_standin_caller`: track_render_process = PS_PakServer (lane w36p; Astra's PR #410 has a genuine reconstruction,
   747/800 words, that already calls `no_catchup(node)` with one parameter — usable as unmatched context if accepted,
   cf. input_process_controller's precedent). Then PS_CreateFile (1,752 B) installs.

## What generalises

- Check the unit for already-EQUAL unclaimed context before writing C: input_process_controller became a free match
  once its caller was installed (wave 34). `blob_unit score NAME` with no --with is the 20-second test.
- A record-pointer local (`pak = &D_80144030[port]`) changes the spill-temp/itable order and PRE placement; direct
  `D_80144030[port].x` indexing (volatile array) gave PS_CreateFile's exact frame and all homes.
- `if (error) {...} else break;` vs `if (!error) goto ok;`: the latter gives PRE a separate exit-edge block (a
  hoisted address computed on the edge and an inverted branch); a rotated `while (call() != 0)` duplicates the call.
- IPA unused/odd parameters: a callee whose retail callers never set a0 but pass a1 is a ONE-parameter internal
  function (IPA moved its only parameter to a1), not a two-parameter function with an unused first argument
  (which IPA passes on the stack). umerge always inlines an internal function with one call site (size does not
  matter), and inlines a small one with two; a big single-call helper is NOT inlined (CreatePlayer test).
- An inlined getter's `node = head; f(&list, head);` (head read twice) makes the load the argument register value and
  the variable a copy (retail's `lw a1; move s3,a1`).
- A block-scoped local takes no frame slot; a function-scope one does (camera_auto_follow's `tw`).
- Passing a nested conditional straight into an inlined setter colours the two arms differently from an if/else into
  a local (camera_auto_follow); `(u8 *)s + charSize` vs `(s + charSize)` flips addu operands where swapping the
  source operands does not.
