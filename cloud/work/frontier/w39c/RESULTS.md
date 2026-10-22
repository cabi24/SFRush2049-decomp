# w39c RESULTS (wave 39, matching the w39a MAT3/DC identifications)

Branch wave37 @ 6e5265bc4. Builder scratch: watchman2:~/rush2049/scratch/frontier/w39c (copied from base, then rsync of the
current src/, include/, tools/cloud, tools/conveyor and lock files at the start; after the coordinator's note also
asm/ and src/ again: blob_matched.lock.json 3742133e, ovl_matched.lock.json 43174aa7, asm/us/blob/SHA256SUMS e2ba9b0a
identical to the local repo). Every group below was re-scored after that sync with the same verdicts (all MATCH,
`--claims` rc=0). blob_unit runs on the Pi against the current local repo. Trace toolkit reused from wtk.

## Summary

| function | identity | bytes | state | flags | best file |
|---|---|---:|---|---|---|
| world_effect_update | InitGraphics (gfx/Init) | 456 | **MATCH** (group w39c_initgraphics, unit EQUAL; no shaping) | -O3 group | groups/w39c_initgraphics/gfx.c |
| func_800EE8AC | N64 _InitGfx, deleted-procedure stub | 8 | **MATCH** (same group; replaces the locked stub single) | -O3 group | groups/w39c_initgraphics/initgfx.c |
| sound_position_update | _UpdateFx@audiomgr.c | 444 | **MATCH** (group w39c_updatefx, unit EQUAL; own .rodata verified; 1 disclosed code-free line) | -O3 group | groups/w39c_updatefx/updatefx.c |
| render_thread_entry | ModelThread@model.c (N64 thread) | 268 | not matched: 30-33 rows, colour only (two s-register webs retail has and we cannot produce) | -O3 | rte/best.c (= w27a) |
| brake_force_apply | BGLoadWaveMain@waves.c | 196 | **MATCH** (audio_frame_sync superseded in place; unit EQUAL with an `align` override; no shaping) | -O3 group | groups/audio_frame_sync/group.c |
| world_collision_response | N64 EndGraphics | 540 | not matched: ~70 rows group / 84 words unit (w36w3 draft still best) | -O3 | wcr/best.c (= w36w3) |
| world_trigger_activate | N64 main-thread message step | 464 | **MATCH** (group w39c_mainloop, unit EQUAL; own .data verified; no unused locals, no holders) | -O3 group | groups/w39c_mainloop/mainloop.c |
| sound_bank_unload | SetupCarColor@visuals.c (N64 palette body) | 1248 | not matched: 135/312 unit words (w11d best unchanged); frame/stub structure analysed | -O3 | sbu/best.c (= w11d) |
| func_800B0EA0 | RGBA5551 blend (SetupCarColor helper) | 192 | **provisional**: score.py group MATCH as an internal callee with a stand-in caller | -O3 group | blend/g2/b.c |
| func_80098FB8 + entity_ai_pathfind | _PeriodicMaintenance / audio thread | 756+2180 | not attempted beyond review: w36x5 state stands (func_80098FB8 unit-EQUAL provisional with the thread; thread 222 rows); func_80098FB8 also calls Astra's camera_transform | - | ../w36x5/thread/best.c |
| display_enable + playgame_state_change | LoadFrontEndOverlay / updateGameState | 316+2544 | not attempted beyond review (w36p ops-2 draft needs umerge-budget blockers that break PrintScores) | - | ../w36p/playgame_state_change/best.c |

Matched this lane: 5 functions + 1 stub claim = 1,568 B of game code (456 + 8 + 444 + 196 + 464), all group claims;
nothing standalone, so nothing in cloud/matches. Provisional: func_800B0EA0 (192 B).

## Scorer commands and verbatim outputs (builder, after the sync)
All from `~/rush2049/scratch/frontier/w39c` with `IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a.../ido`:
- `python3 tools/cloud/score.py group cand/g/w39c_initgraphics --claims` -> `world_effect_update:` `MATCH`,
  `func_800EE8AC:` `MATCH`; rc=0.
- `python3 tools/cloud/score.py group cand/g/w39c_updatefx --claims` -> `sound_position_update:` `MATCH` /
  `own .rodata verified at 0x80123A64..0x80123A6C`; rc=0 (context func_80095198, func_80091B00 MATCH).
- `python3 tools/cloud/score.py group cand/g/audio_frame_sync` -> func_800972C4, SetupWaveModel, func_8009715C,
  func_80097164, fp_call_wrapper, audio_frame_sync, func_800972BC, suspension_setup, func_800A5014, brake_force_apply all
  `MATCH`; `--claims` rc=0, plain rc=0 (context entity_lod_select differs as in the installed group; informational).
- `python3 tools/cloud/score.py group cand/g/w39c_mainloop --claims` -> `world_trigger_activate:` `MATCH` /
  `own .data verified at 0x801161CC (4 bytes, all zero; .data is laid out per translation unit)`; rc=0.
- `python3 tools/cloud/score.py group cand/g/g2` (blend/g2, provisional) -> `func_800B0EA0:` `MATCH`.
Unit (Pi, `python3 -m tools.conveyor.pipeline.blob_unit --tag w39c score ...`): see each section; every claimed
function `EQUAL` with `locked bodies that differ in this unit: 0`.

## Details

### world_effect_update = InitGraphics@gfx.c (456 B) + func_800EE8AC = _InitGfx (8 B stub): MATCH
- Key: _InitGfx is in a SECOND source file. umerge's cross-file inline puts as1 block boundaries where the inlined
  file's code starts and ends; that is exactly w19e's two-label asm.sh oracle (prologue alone in the entry block;
  the D_801497C8 store and its reload for D_801497F4 in different blocks). One-line form from initsystem.c: 35 rows
  (worse); w19e multi-line single file: 12 words; two files, `D_801497F4 = D_801497C8;` in InitGraphics after
  the _InitGfx call: MATCH, no shaping, no volatile.
- `score.py group cand/g/w39c_initgraphics --claims` -> `world_effect_update: MATCH`, `func_800EE8AC: MATCH`, rc=0.
- Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w39c score world_effect_update func_800EE8AC
  game_late_init --with .../gfx.c --with .../initgfx.c --internal func_800EE8AC --neighbours` ->
  `EQUAL world_effect_update: 114 words (kept, c_gfx.c)`, `EQUAL func_800EE8AC: 2 words (internal, c_initgfx.c)`,
  `EQUAL game_late_init`, `locked bodies that differ in this unit: 0`. (The unit's "stub tail" note is the
  pre-existing HAL_InitText_Init / DeallocateMemory_Init stubs of codex_heap_release_a25 placed before game_late_init.)
- Group object order (objsyms): func_800EE8AC stub at +0, world_effect_update at +8 = retail 0x800EE8AC / 0x800EE8B4.

### sound_position_update = _UpdateFx@audiomgr.c (444 B): MATCH
- Group: updatefx.c (member) + timeout.c (byte copy of src/blob/func_80095198.c = AudioTimeout, kept, inlined
  cross-file as in retail) + getcmd.c (byte copy of frontier_slot18_alloc/func_80091B00.c = _GetCmd, internal; its
  IPA convention keeps stag in a2 across the jal, frontier "preserved: a2").
- `score.py group cand/g/w39c_updatefx --claims` -> `sound_position_update: MATCH` / `own .rodata verified at
  0x80123A64..0x80123A6C`, rc=0. Unit: `blob_unit --tag w39c score sound_position_update func_80095198 func_80091B00
  --with .../updatefx.c --neighbours` -> `EQUAL sound_position_update: 111 words (kept, c_updatefx.c)`, `locked bodies
  that differ in this unit: 0`.
- Path: MAT3 text 72 rows -> `tag->` instead of `stag->tag->` in the conditions (state CSE) 36 -> branch-keeping
  line 29 -> `gMaxSfx * 0.75f` 19 (colour only: force.sh p1:w5=c6,w95=c7,w2=c5 -> 4 rows = the rodata relocations)
  -> no `tag` local at all (every access `stag->tag->`) 0.
- Shaping (disclosed in the header): `{ s32 t = elNum == 2; if (stag->tag->state != 2) { if (t) {} if (t) {} } }`
  gives retail's leading `beq t0,v1,next; nop` (a branch to the next instruction). Mechanism (ugen listing, trace
  ugt.sh): uopt sinks the computation of t into the guarded block; ugen drops the empty `if (t)` branches but the
  outer branch now targets a label that is not the next one; as1 deletes the dead `seq`. Only 4 of the
  retail image's game functions have such a branch (scan of asm/us/blob): this one, func_80096288 (installed with the
  same device family), entity_ai_pathfind, differential_output.

### render_thread_entry = ModelThread@model.c (268 B): not matched
- Group harness rte/g1 (single file, keep render_thread_entry): w27a best -> `want 67 words, got 64; differing rows 33`
  (the 2-3 word length gap is the position-dependent `.align 5` epilogue pad; needs an `align` unit override as
  task_complete_signal / audio_queue_process).
- ctrace (snapshot rte0): &msg w17 save 27.5 (s0), -1 w18 25.0 (s1), &queue w12 22.2 (s2), &modelrun w14 2.5 (s3);
  the constant 1 is no candidate. Retail: s0 &msg, **s1 = 1**, **s2 saved, never used**, s3 -1, s4 &queue,
  s5 &modelrun, and the modelrun load uses t7 (one phantom ring pop before it). Weight arithmetic: for 1 to rank
  above -1 (tot 100 = one use in the drain loop at weight 100), 1 needs a use inside the drain loop (tot 110 ties
  &msg's 27.5 and loses on web number), and the s2 web X too. So the retail drain loop held code using 1 and X that
  was deleted after colouring (as1 deletes dead set/move code: seen in sound_position_update and world_effect_update).
- Tried (no gain): `== 1` on ControllerUpdate / modelrun (1 becomes s4, 32 rows), dead `t` computations in the drain
  loop (while-form changes the loop shape; block-scope t costs a frame slot), `volatile` modelrun, a skipped-frame
  counter `n++` in the drain loop (DCE'd).
- Next hypothesis: a dead per-message test inside the drain loop (e.g. `*(s16 *)msg == OS_SC_RETRACE_MSG (1)`) whose
  compare is removed late; find the form with the t_8 mechanism (computation sunk into a block whose empty `if`s
  ugen drops) but without a block-scope local.
- Round 2 (vbatch rte/b1, 192 variants with a 24-byte pad function so the `.align 5` epilogue lands right):
  loop forms, extra n/t locals with dead uses in the drain loop, volatile modelrun, three ControllerUpdate forms; no
  variant colours the constant 1 or adds a sixth s-register (best 30 rows, all colour).

### brake_force_apply = BGLoadWaveMain@waves.c (196 B): MATCH (position-dependent)
- The body was already code-identical as audio_frame_sync context (w36x5): the only residual is the `.align 5` pad of
  the unreachable epilogue after the endless loop, i.e. the function's offset modulo 32 in its object. Retail offset in
  the game .text (base 0x80086A50) is 0x25D0C = 12 mod 32; in the installed group object it is 0xED8 = 24 mod 32
  (`12/49 words differ (5 extra words)`).
- Fix for the group: one NEW context file, wheel_torque_apply.c = byte copy of the locked single
  src/blob/wheel_torque_apply.c (retail 0x800AC668, the function two before it), kept. It is emitted first (with an
  8-byte unnamed stub of its inlined static), so brake_force_apply moves to 0xF6C = 12 mod 32. Body rewritten from
  the m2c `goto loop_1` form to a `for (;;)` thread loop with `OSMesg msg` (same code).
- `score.py group cand/g/g3` (= groups/audio_frame_sync): every member MATCH (func_800972C4, SetupWaveModel,
  func_8009715C, func_80097164, fp_call_wrapper, audio_frame_sync, func_800972BC, suspension_setup, func_800A5014,
  brake_force_apply); `--claims` rc=0 and plain rc=0. Context entity_lod_select differs as before (pre-existing,
  informational).
- Unit: `python3 cloud/work/frontier/w39c/tools/unit_extra.py --align brake_force_apply -- --tag w39c score
  brake_force_apply func_80097164 --with cloud/work/frontier/w39c/bfa/bgl_unit.c --neighbours` -> `EQUAL
  brake_force_apply: 49 words`, `locked bodies that differ in this unit: 0`. Without the override, with the group's
  group.c: `FAIL brake_force_apply: 12 of 49 words differ; compiled body is 50 words` (padding only).
  unit_extra.py is a lane wrapper that adds override entries in memory (src/blob/unit_overrides.json untouched).

### world_collision_response (540 B): parked
- The frontier "preserved: a1, a2" is a heuristic artifact of ipa.analyze: retail sets a3 (&D_801497C8) before
  `jal world_velocity_integrate` with a0, so a1/a2 count as "passed through". No IPA input.
- ctrace (snapshot wcr1, w36w3 draft): &D_801497C8 is web 77 -> a1 (forbidden v0/v1/a0 only); retail gives it a3,
  so in retail a1 and a2 are taken by webs that interfere with it, and rspboot (v0) / -0x9CC0 (v1, hoisted lui/ori)
  are webs; ours keeps both as ring temps. Not resolved. (The caller world_trigger_activate matched anyway: no value
  is live across its jal to this function.)
- Round 2: in the unit, world_velocity_integrate (HAL_FinishText) and audio_start have world_collision_response as
  their only caller; made internal they are always inlined (even with 300 dead stores), so retail kept them external
  and IPA clobber sets cannot explain a3. vbatch wcr/b1 (128 variants: volatile cursor/frame byte, dl local, statement
  order, ternary flags, EndDL/SetupTask helpers) never beats the w36w3 draft (87 strict in a one-file group).

### func_800B0EA0 (192 B): provisional, 0 rows with a stand-in caller
- `s32 func_800B0EA0(s32 t, s32 a, s32 b)`: `if (t == 0) return a; if (t >= 255) return b; inv = 255 - t;`
  then `c = ch(0xF800); c |= ch(0x7C0); c |= ch(0x3E); return c | 1;` with ch(m) = `(((a & m) * inv + (b & m) * t)
  >> 8) & m`. u16 parameters add callee-side narrowing (48/48 differ); s32 is right.
- Kept (external) it is 39 rows off: ugen's temp ring runs t6..t9 then t0..t5 (10 temps). Retail wraps t6..t9 only,
  because the function is IPA-internal: its caller keeps values in a3/t0-t4 across the three ramp calls (frontier
  "preserved: a0, a3, t0..t4" on sound_bank_unload), so the internal callee's ugen pool excludes them. With the
  function NOT kept and a kept stand-in caller (blend/g2: two ramp loops), `gd.sh blend/g2 func_800B0EA0` ->
  `want 48 words, got 48; differing rows 0`; `score.py group cand/g/g2` -> `func_800B0EA0:` `MATCH`. Provisional until
  SetupCarColor (sound_bank_unload) is matched; then it joins that group as an internal member.

### sound_bank_unload = SetupCarColor (1248 B): not matched (135/312 unit words, w11d best unchanged)
- Re-scored w11d best in the current unit: `FAIL sound_bank_unload: 135 of 312 words differ | words 199 ops 24 norm 24 |
  frame 192/192` (`--internal func_800B0EA0 --internal func_800B0F60`).
- The two unused s0/s1 saves are explained: DeallocateMemory's internal audio_reverb_update (IPA a1/a2) leaves s0/s1 to
  its callers (wheel_params_set.c header). The ~100-byte middle of the frame is inline reserves + locals, not w3b's
  `s32 pad[10]`: DeallocateMemory as an inlined helper (with func_80095CF4/func_80095CFC nested) adds 16 B (sbu/v1:
  168 without pad, 208 with), retail needs 24 B more.
- Layout says the stub func_800B0F60 is the deleted helper whose call sites lie between the MBOX call and the ramps,
  i.e. a brighten helper, not w3b's texture-def store. Written as a range helper (v2) or per-element helper (v3) it is
  never inlined: umerge's per-caller budget is exhausted (also with the TexDef stores and DeallocateMemory written
  in place, sbu/g3, g3b, g3c), so our reconstruction is bigger in u-code than retail's.
- Ops residual (24 rows) is the ramp-loop colouring: retail ratio t0 / cursor a3 / colour t1, ours swapped.
- Next: shrink the u-code of the body (DC-era statement shapes, fewer named temporaries) until umerge inlines the
  brighten helper at both sites, then the frame and the unroll factor should follow without w3b's devices.

### world_trigger_activate (464 B): MATCH
- Base: w36w3 draft (25 unit words; 32 rows in a one-file group). vbatch b1 (400 of 18432 over 13 axes: volatile per
  global, static/extern counters, x, holder, compare orders, helper) -> D_80114730 must NOT be volatile (address webs
  in v1 per case; v0204 mnem 0). b2 (576, exhaustive over 8 axes: helpers for case 4 / 2750 / 2, statics, x) -> 20,
  frame exact with the case-4 and case-2750 bodies as inlined helpers.
- Last residual: the retrace counter's loaded value is a v1 web in retail. A separate `n` local takes v0 (force.sh
  wta3 p1:w40=c2 -> 4 rows = only the unverified own-.data relocation rows), and reusing the framebuffer scratch
  variable (`cur = D_80151AE0; D_80151AE0 = cur + 1;`) gives v1. The 4-byte local above msg (w36w3's unused `x`) is
  the named switch value `s16 type`. The `if (cur) {}` holder is no longer needed.
- `score.py group cand/g/w39c_mainloop --claims` -> `world_trigger_activate: MATCH` / `own .data verified at
  0x801161CC (4 bytes, all zero ...)`, rc=0. Unit: `blob_unit --tag w39c score world_trigger_activate --with
  .../mainloop.c --internal MainPreNMI --internal MainEndFrame --neighbours` -> `EQUAL world_trigger_activate: 116 words
  (kept, c_mainloop.c)`, `locked bodies that differ in this unit: 0`.


## What generalises
1. **Cross-file inlining is an as1 lever.** A callee inlined by umerge from ANOTHER source file leaves as1 block
   boundaries where its code starts and ends (prologue alone in the entry block; a store and its reload kept apart).
   w19e had proved world_effect_update's residual was two as1 labels but no single-file form made them; putting
   _InitGfx in a second file of the group did, with no shaping. Try it whenever an asm.sh label oracle closes a residual.
2. **A deleted helper's stub sits where post-order puts it** (w38b rule holds in the game image): _InitGfx's stub is
   the caller-less func_800EE8AC right before InitGraphics; SetupCarColor's callee order (func_800B0EA0, stub
   func_800B0F60, MBOX_FindPalette_WORK) places that stub's call sites between the MBOX call and the ramps.
3. **Branch-to-next (`beq x,y,next; nop`) = a guarded block whose content died after uopt.** Only 4 game functions have
   one. Mechanism (ugt.sh): uopt sinks a computation into the guarded block, ugen drops the empty inner ifs, the outer
   branch then targets a non-adjacent label and as1 deletes the dead set. Reproduced with
   `{ s32 t = elNum == 2; if (stag->tag->state != 2) { if (t) {} if (t) {} } }` (disclosed).
4. **IPA-internal leaf callees get a restricted ugen pool.** func_800B0EA0 kept: ring t6..t9 then t0..t5 (39 rows);
   internal with a caller: t6..t9 only (0 rows), because the caller keeps values in a3/t0-t4 across the calls. A leaf
   whose retail ring never leaves t6-t9 while ours spills into t0+ is an internal callee: score it in a group.
5. **Register reuse of one scratch local** decides v0 vs v1: world_trigger_activate's retrace counter reload is v1 only
   when it goes through the variable that held the framebuffer (`cur`); a fresh local takes v0 (force.sh proved it the
   only residual). A 4-byte "unused" local above the others can be a NAMED switch value (`s16 type = *(s16 *)msg`).
6. **vbatch exhaustive sweeps are seconds** (576 group compiles in < 10 s): sweep volatile-per-global, static/extern,
   helper-vs-inline axes exhaustively, then trace the last residual. That took world_trigger_activate 32 -> 0.
7. **Prefer no `volatile` on counters**: D_80114730 volatile kept a hoisted address web across switch cases.
8. **Position-dependent `.align 5` epilogues** (thread entries with endless loops) can be fixed in a group by context:
   a byte-copied locked neighbour emitted before the member puts it at its retail offset mod 32 (brake_force_apply).
9. Single-call-site internal functions are inlined by umerge regardless of a dead `if (0)` blocker (tested with 300
   dead stores on world_collision_response and on world_velocity_integrate/audio_start): a callee that retail keeps out
   of line with one caller was kept (external), not internal.
10. umerge's per-caller budget is real: SetupCarColor never inlines a brighten helper even with every other helper
    removed, so if retail inlined one, retail's caller was smaller in u-code than our reconstruction.

## Integration notes (per MATCH)
- **w39c_initgraphics (NEW group)**: files gfx.c (world_effect_update, kept) + initgfx.c (func_800EE8AC = _InitGfx,
  non-static, NOT kept: cross-file inlined, emits the 8-byte deleted-procedure stub with its symbol). claims
  [world_effect_update, func_800EE8AC]. Supersedes the locked standalone stub single src/blob/func_800EE8AC.c (-O2):
  remove that single / `prefer_definition func_800EE8AC -> groups/w39c_initgraphics/initgfx.c`, and unit
  `force_internal func_800EE8AC`. Caveats: world_effect_update is a kept, unreferenced global (no caller anywhere);
  game_late_init's installed initsystem.c keeps its own one-line _InitGfx copy (still EQUAL in the unit with this
  group: `EQUAL game_late_init`).
- **w39c_updatefx (NEW group)**: updatefx.c (sound_position_update, kept: its address is taken by the unmatched
  func_80098FB8) + context timeout.c (byte copy of src/blob/func_80095198.c, kept) + getcmd.c (byte copy of
  src/blob/groups/frontier_slot18_alloc/func_80091B00.c, internal). claims [sound_position_update]. No overrides needed
  (unit EQUAL as kept with no flags). Own .rodata (0.2f, 0.1f) verified. Disclosed: one code-free shaping line.
- **audio_frame_sync (superseded IN PLACE)**: every old member unchanged and MATCH; brake_force_apply moved from context
  to member (body in group.c rewritten from the m2c goto form to `for (;;)`, same code); NEW context file
  wheel_torque_apply.c (byte copy of the locked single src/blob/wheel_torque_apply.c, on keep) only to position
  brake_force_apply at its retail offset modulo 32. claims [brake_force_apply]. Unit: needs
  `align brake_force_apply` in unit_overrides.json (as audio_queue_process / task_complete_signal); without it the unit
  shows `12 of 49 words differ` (padding only). Other files byte-identical to the installed group.
- **w39c_mainloop (NEW group)**: mainloop.c (world_trigger_activate, kept: called from the static game_init) with two
  inlined non-static helpers MainPreNMI / MainEndFrame (invented names, not kept; unit `force_internal MainPreNMI,
  MainEndFrame`). claims [world_trigger_activate]. D_801161CC is a function-local static (own .data verified).
  Caveat: its callee world_collision_response is unmatched retail asm; no value is live across that call, so the
  callee's eventual body cannot change this one.
- Provisional only (not for installation): blend/g2 (func_800B0EA0 with a stand-in caller `__standin_SetupCarColor`).

## Tools (lane)
`sc.sh` (score.py fn), `gs.sh` (score.py group), `gd.sh` (group compile + udiff aligned diff), `tc.sh` (one-file
cc + objdump), `tools/vrun.py` (one-edit variants of a group), `tools/objsyms.py` (group object layout, blob symbols),
`tools/gobj.py` (builder-side group compile), `tools/unit_extra.py` (blob_unit with extra override entries added in
memory, e.g. `--align NAME`; src/blob/unit_overrides.json is not touched). vbatch templates: wta/t1.c, wta/t2.c,
rte/t1.c, wcr/t1.c.

## Permission denials
None.
