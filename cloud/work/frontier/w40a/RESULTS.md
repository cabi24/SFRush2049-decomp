# w40a RESULTS (wave 40: N64 audio manager + thread/platform code)

Branch wave37 @ dd4cf01d3. Builder scratch: watchman2:~/rush2049/scratch/frontier/w40a (copied from base; trace
toolkit reused from wtk). No git operations; everything is in this lane dir.

Matched this lane: 6 functions in 3 groups, 4,652 B of game code (func_80098FB8 756 + entity_ai_pathfind 2180 +
render_thread_entry 268 + sound_bank_unload 1248 + func_800B0EA0 192 + func_800B0F60 8); 4,644 B are new coverage
(func_800B0F60's stub is already locked as a single and is re-claimed by the group). Nothing standalone, so
nothing in cloud/matches. Joint unit check of all three packets: `scores/combined_unit_align2.txt` (11/11 EQUAL,
`locked bodies that differ in this unit: 0`).

| function | identity | bytes | state | scorer (exact, builder unless noted) | best file | tried | next hypothesis |
|---|---|---:|---|---|---|---|---|
| func_80098FB8 | _PeriodicMaintenance (+ _ProcessPriorities inlined) | 756 | **MATCH** (engine_sound_sync superseded in place) | `score.py group cand/g/engine_sound_sync --claims` -> `func_80098FB8: MATCH`, rc=0; unit `EQUAL func_80098FB8: 189 words (internal)` | groups/engine_sound_sync/audiomgr_thread.c | see audio sections | - |
| entity_ai_pathfind | N64 audio-manager thread (+ MAT3 _ProcessCommand in place) | 2180 | **MATCH** (same group) | same run -> `entity_ai_pathfind: MATCH` / `own .rodata verified at 0x80123A8C..0x80123ABC`; unit `EQUAL ...: 545 words (kept)` | groups/engine_sound_sync/audiomgr_thread.c | dead-return inlines, f_split DFS (`goto skip; skip:`), holders | - |
| render_thread_entry | ModelThread@model.c (N64 thread form) | 268 | **MATCH** (new group w40a_modelthread) | `score.py group cand/g/w40a_modelthread --claims` -> `render_thread_entry: MATCH`, rc=0; unit `EQUAL ...: 67 words (kept)` (needs `align` in the joint unit) | groups/w40a_modelthread/modelthread.c | compiled-out two-compare test, layout context files | - |
| world_collision_response | N64 EndGraphics | 540 | not matched: structure exact (135/135 words, every op-row difference is colour), 88 rows | `gd.sh wcr/g world_collision_response` -> `want 135 words, got 135; differing rows 88` | wcr/best/gfx8_volatile.c | 4 sweeps (wcr/b3-b7, ~80 variants) + ctrace/force oracles; cursor a3 reached only with 2 dummy reads | second use of -0x9CC0 (coloured v1 web), then rspboot v0 / &D_8015B250 t1 |
| sound_bank_unload | N64 SetupCarColor (palette builder, visuals.c) | 1248 | **MATCH** (new group w40a_carcolor) | `score.py group cand/g/w40a_carcolor --claims` -> `sound_bank_unload: MATCH`, rc=0; unit `EQUAL sound_bank_unload: 312 words (kept, c_carcolor.c)` | groups/w40a_carcolor/carcolor.c | ~25 batches (sbu/b*, sbu/c*), ctrace/spill/pretrace | - |
| func_800B0EA0 | RGBA5551 interpolator (SetupCarColor helper) | 192 | **MATCH** (same group, internal member) | same run -> `func_800B0EA0: MATCH`; unit `EQUAL ...: 48 words (internal)` | groups/w40a_carcolor/carcolor.c | parameter order (left, right, ratio) | - |
| func_800B0F60 | MakeRamps (deleted after inlining; 2-word stub) | 8 | **MATCH** (same group; supersedes the locked -O2 single) | same run -> `func_800B0F60: MATCH`; unit `EQUAL ...: 2 words (internal)` | groups/w40a_carcolor/carcolor.c | - | - |

## Audio pair: MATCH (group engine_sound_sync, superseded in place)
Scorer (builder, scratch w40a, `IDO_DIR=.../796ae99a.../ido`), transcripts in `scores/engine_sound_sync.txt`:
- `python3 tools/cloud/score.py group cand/g/engine_sound_sync` -> members `engine_sound_sync: MATCH`,
  `audio_effect_setup: MATCH`, `func_80098ADC: MATCH`, `func_800988D8: MATCH`, `func_80098FB8: MATCH`,
  `entity_ai_pathfind: MATCH` / `own .rodata verified at 0x80123A8C..0x80123ABC`; exit=0.
- `python3 tools/cloud/score.py group cand/g/engine_sound_sync --claims` -> `func_80098FB8: MATCH`,
  `entity_ai_pathfind: MATCH` / `own .rodata verified at 0x80123A8C..0x80123ABC`; exit=0.
- Unit (Pi, `scores/engine_sound_sync_unit.txt`): `python3 -m tools.conveyor.pipeline.blob_unit --tag w40a score
  entity_ai_pathfind func_80098FB8 engine_sound_sync audio_effect_setup func_800988D8 func_80098ADC func_800986DC
  --internal func_80098FB8 --with .../audiomgr_thread.c --with .../audio_effect_setup_b132.c --with
  .../func_800986DC.c --neighbours` -> `EQUAL entity_ai_pathfind: 545 words (kept, c_audiomgr_thread.c)`, `EQUAL
  func_80098FB8: 189 words (internal, c_audiomgr_thread.c)`, `EQUAL engine_sound_sync: 309 words (...); stub tail: 4
  words of deleted-procedure stubs follow (NOT as in the image)`, `EQUAL audio_effect_setup`, `EQUAL func_800988D8`,
  `EQUAL func_80098ADC`, `EQUAL func_800986DC`, `locked bodies that differ in this unit: 0`. No `align` override needed
  (the unit run without one is EQUAL too). The stub-tail note is unit-only: blob_group slices members from the group
  object, where engine_sound_sync has no extra words (scorer MATCH above).

## render_thread_entry = ModelThread: MATCH (new group w40a_modelthread)
- `score.py group cand/g/w40a_modelthread` -> `render_thread_entry: MATCH`, exit=0; `--claims` -> same, exit=0
  (`scores/w40a_modelthread.txt`; context func_800E7710 MATCH, func_800E6AF0 "3 extra words" informational).
- Unit: `python3 -m tools.conveyor.pipeline.blob_unit --tag w40a score render_thread_entry --with
  .../w40a_modelthread/modelthread.c --neighbours` -> `EQUAL render_thread_entry: 67 words (kept, c_modelthread.c); stub
  tail: 2 words of deleted-procedure stubs follow (NOT as in the image)`, `locked bodies that differ in this unit: 0`.
- Joint unit run with the other packets (`scores/combined_unit.txt`): `FAIL render_thread_entry: 9 of 67 words differ;
  compiled body is 74 words` (the `.align 5` epilogue pad moves with the unit layout); with an in-memory `align` entry
  (`tools/unit_extra.py --align render_thread_entry`, `scores/combined_unit_align.txt`) it is EQUAL again. The install
  needs that `align` override (same mechanism as dma_wait_complete / brake_force_apply).
- Lever (one line): in the drain loop, `if (((OSScMsg *)msg)->type == 1 || ((OSScMsg *)msg)->type == 4) {}`
  (compiled out; any second constant, an s16 `type` local, or `*(s16 *)msg` give the same code). It makes the constant
  1 a register candidate inside the drain loop (retail `li s1,1` / `move a2,s1`) and the type load a coloured web (the
  never-used s2 save) whose code ugen/as1 drop (retail's `lhu t7`: the ring pop of the deleted msg load). This is
  w39c's "late-removed test inside the drain loop" hypothesis; a single `== 1` test gives the s1 constant but not s2
  (29-30 rows), the second compare supplies the second web.

## world_collision_response = N64 EndGraphics: not matched (structure exact, colouring open)
Harness `wcr/h0` (one-file -O3 group). Earlier rounds: w36w3 draft 92 rows, sweep wcr/b3 best 72 rows (byte-difference
data_size, colour luck). This round (wcr/b4-b7):
- **Scheduler fields are volatile** (as in w39c's matched mainloop): `extern volatile Sched D_8002E8E8` (frameCount at
  +636) and `extern volatile s32 D_8002EB98` give retail's `lui t5; addiu t5; lw t6,636(t5)` / `lui t7; addiu t7;
  sw t6,0(t7)` tail and the exact length (135 words). Every retail access to D_8002EB98 is of this form (5 sites).
- **data_size is a Gfx pointer difference times 8**: `(u32)((Gfx *)D_801497C8 - (Gfx *)(D_8015B250 + 0x9CC0)) *
  sizeof(Gfx)` gives retail's `subu; addu -0x9CC0; sra 3; sll 3`. With both: `wcr/best/gfx8_volatile.c`, 88 aligned rows,
  ops rows 11, all of them register/order (`gd.sh`: want 135 words, got 135).
- Colouring (ctrace snapshot wg8): the DL-cursor address web (&D_801497C8, w77, tot 7 / nocs 2 = 3.5) outranks the
  three GBI `_g` temporaries (3.0 each, coalesced away but holding a0/a2/a3), so it takes a1; retail's a3 means it
  was coloured after them. Oracle `force.sh wg8 ... p1:w77=c6`: the first 33 words match. Diagnostic: two compiled-out
  `if (D_801497C8) {}` reads after world_velocity_integrate lift its nocs to 3 (8/3 = 2.67) and give a3 naturally
  (`wcr/best/gfx8_volatile_2reads_diag.c`, 77 rows) -- a device, not adopted.
- Still open after that: &D_8015B250 should be t1 (ours t0; forcing it works), rspboot (D_80027DD0) should be v0 (force
  declined: v0 is forbidden for its web in ours), and the constant -0x9CC0 should be a coloured web in v1. In ours it is
  a register candidate (`makelivranges`: `const -40128 ctx=1 LIVE-RANGE blocks 2`) with one use and save 0, so it is
  never coloured: retail's source uses it twice or in a heavier context. Next: find the second -0x9CC0 use (the task
  size/pointer pair written through a local or a helper), then rspboot's v0.

## sound_bank_unload = N64 SetupCarColor: MATCH (new group w40a_carcolor)
Group `groups/w40a_carcolor/`: `carcolor.c` (listed first) + byte copies `ctx_*.c` of all 12 codex_heap_release_a25 files
(cmp-verified). Members = claims: sound_bank_unload, func_800B0EA0, func_800B0F60; keep = heap keep + sound_bank_unload;
context = heap members + heap context.
- Builder (`scores/w40a_carcolor.txt`, `scores/w40a_carcolor_claims.txt`): `python3 tools/cloud/score.py group
  cand/g/w40a_carcolor` -> `sound_bank_unload: MATCH`, `func_800B0EA0: MATCH`, `func_800B0F60: MATCH`, all 27 context
  functions MATCH, rc=0; `--claims` -> the same three MATCH, rc=0. No own .data/.rodata (1.25f is an immediate).
- Unit (Pi, `scores/w40a_carcolor_unit.txt`): `python3 -m tools.conveyor.pipeline.blob_unit --tag w40a score
  sound_bank_unload func_800B0EA0 func_800B0F60 --internal func_800B0EA0 --internal func_800B0F60 --internal
  SetModelPalette --with cloud/work/frontier/w40a/groups/w40a_carcolor/carcolor.c --neighbours` -> `EQUAL
  sound_bank_unload: 312 words (kept, c_carcolor.c)`, `EQUAL func_800B0EA0: 48 words (internal)`, `EQUAL
  func_800B0F60: 2 words (internal)`, `locked bodies that differ in this unit: 0`.
- Not standalone (needs the heap group's internal audio_reverb_update for the IPA a1/a2 call and s0/s1 saves).
Harness and tools: `sbu/g1/` (same context, file under test `visuals.c`), `sbu/gen.py`, `sbu/gen2.py`, `sbu/ct.sh`
(colouring probe), `sbu/ring_sim.py`. Path from w11d's 135/312 words (rows = aligned-diff rows in the harness):
1. Heap-group context: frame 160 -> 184; the free path exact (osRecvMesg / internal audio_reverb_update(a1=colors,
   a2=0) / osSendMesg, unused s0/s1 saves for the callee).
2. umerge never inlines a 2-site brightening helper (w40b's knapsack, S/B = 65/6 > 3; even a one-statement body fails).
3. Unroll x2 needs a bigger loop body: separate r/g/b channel statements -> 199 rows, frame 192.
4. Brightening temp ring: `(u16)(r | g | b) | 1`. ugen emits the u16 mask (one hidden ring pop) that as1 deletes; an
   LRF-queue simulation of ugen's temp list predicted exactly this -> 59 rows.
5. Ramp loops in index form with the body on its own line (retail's `sh v0,-2(a3)` in the bnez slot) -> 34 rows.
6. func_800B0EA0 takes (left, right, ratio): the callee is byte-identical for every order (IPA keeps ratio in a0), but
   the caller's argument order puts `move a0,t0` in the jal delay slot as retail -> 28 rows.
7. Colouring (ctrace): retail has first t1, second t2, colors t3, third t4, so colors' priority is in [3.0, 4.0). A
   brightening helper gives colors totalsave 76 (the inlined parameter's in-loop uses count, 31 per path); brightening
   in place gives 14 (w11d's t4). Ramps as the single-site helper (its stub is retail's func_800B0F60, right after
   func_800B0EA0) give 44/11 = 4.0, a tie lost on web number; the helper returning its palette pointer adds the one
   occurrence that moves nocs to 12 (45/12 = 3.75) -> 5 rows.
8. second/third colour loads on one line (as1 line tie: third load first, as retail) -> 3 rows.
9. Descriptor spill temp: inline areas start 8-aligned in source order, so with two 8-byte SetTexPal sites last the
   area always ends 8-aligned (144: temp at sp+44). The heap lock/unlock helpers called in place (no DeallocateMemory
   wrapper: 8 bytes each) plus a `Model68 *m` local in the model-palette setter end the area at 140, temp at sp+48,
   frame 192 -> 0 rows, 312 words. The same lever also removes the need for unused caller locals.
10. File order: with carcolor.c last in `files`, a context file's static stub followed sound_bank_unload (1 extra
   word); listed first, none.

## What generalises
1. **Retail `b end`-only blocks can be dead inlined value returns** (int-returning static called as a statement): uopt
   keeps the `str $2`, ugen emits `move v0,...`, as1 deletes it; each inline site costs 8 frame bytes.
2. **f_split carves the first piece from the first unit in the live range's unit list (DFS of f_formlivbb).** An
   unreachable `goto L; L:` before the first case changes the DFS order at zero cost.
3. **Compiled-out two-compare tests (`if (a == 1 || a == 4) {}`) create coloured webs that later vanish** (model thread).
4. **IPA parameter registers do not follow source parameter order**, but the caller's argument evaluation order does:
   a callee that matches for every parameter order can still fix the caller's jal delay slot (func_800B0EA0 is
   (left, right, ratio)). Line numbers of inlined code drive the same as1 ties.
5. **Colour priority is arithmetic: save = totalsave / nocs, nocs = ((n - 2) >> 2) + 2.** A helper's inlined parameter
   makes its in-loop uses count toward the caller variable's totalsave (31 per loop path here); code written in place
   does not. Choose helper boundaries by the priority they produce, and look for one natural extra occurrence when two
   webs tie (a helper returning its buffer pointer). The dummy-read diagnostic (`if (x) {}` k times) measures n.
6. **Hidden ugen ops move the temp ring.** A u16 cast before `| 1` makes ugen emit a mask that as1 deletes; an
   LRF-queue simulation of ugen's free list (`sbu/ring_sim.py`) finds which hidden op reproduces retail's temp names.
7. **Inline areas start 8-aligned, in source order, params first then locals; spill temps follow.** A spill slot 4 bytes
   off means the last inline area must end 4 mod 8 (a local in the last inlined helper, one nested helper level less).
8. **Volatile globals show as `lui; addiu; lw/sw 0(reg)`** (D_8002E8E8 / D_8002EB98, as in w39c's mainloop).
9. **A Gfx pointer difference times 8 is retail's `sra 3; sll 3`** for display-list sizes.
10. **Group file order matters for extra words:** a context file emitted right after the claimed function can leave its
    static stub there; list the claimed file first.
11. **Endless-loop thread entries need the unit `align` override** even when a solo unit run is EQUAL by luck.

## Integration notes
1. **Group `engine_sound_sync` (supersede in place)** -- dir `groups/engine_sound_sync/`, files engine_sound_sync.c,
   audio_effect_setup_b132.c (installed copy minus its func_80098FB8 NONMATCH context body), codex_transform_b109.c,
   timeout.c (unchanged), audiomgr_thread.c (new), func_800986DC.c (byte copy of the locked single; context, kept,
   listed last for the thread's `.align 5` pad). members: engine_sound_sync, audio_effect_setup, func_80098ADC,
   func_800988D8, func_80098FB8, entity_ai_pathfind; claims: func_80098FB8, entity_ai_pathfind. The old members are all
   still MATCH. Unit overrides: none required (func_80098FB8 becomes internal by derivation; the existing force_internal
   entries for audio_effect_setup/func_80098ADC stay). Optional safety: `align` entry for entity_ai_pathfind (endless
   audio-manager loop; EQUAL with and without it in every run). Stub singles to revert: none. Caveat: func_800986DC has
   three byte-identical definitions (single, dot_record_score context, this copy).
2. **Group `w40a_modelthread` (new)** -- dir `groups/w40a_modelthread/`, files [func_800E7710.c, modelthread.c,
   func_800E6AF0.c] (the outer two are byte copies of locked singles, context/kept, layout only). members = claims =
   [render_thread_entry]. Unit overrides: `align` += render_thread_entry ("position dependent: ModelThread ends in an
   endless loop; as1 pads the unreachable epilogue to a 32-byte boundary of .text, 67 words only at its image offset
   modulo 32"). Stub singles to revert: none. Caveat: one disclosed compiled-out test (header of modelthread.c).
3. **Group `w40a_carcolor` (new)** -- dir `groups/w40a_carcolor/`, files [carcolor.c, ctx_*.c x12] (the ctx files are
   cmp-identical copies of every codex_heap_release_a25 file). members = claims = [sound_bank_unload, func_800B0EA0,
   func_800B0F60]; keep = codex_heap_release_a25 keep + sound_bank_unload; context = that group's members + context.
   Unit overrides: force_internal += SetModelPalette ("non-kept helper inlined at both model-palette stores of
   sound_bank_unload; only its stub remains"); func_800B0EA0 and func_800B0F60 are members not on keep (the unit runs
   used --internal for them; add force_internal entries if derivation does not make them internal). Stub single to
   revert: `src/blob/func_800B0F60.c` and its blob_matched.lock.json entry (the locked -O2 `void func_800B0F60(void) {}`
   is superseded by the group's deleted MakeRamps, which produces the same 2 words). Caveats: not standalone (needs
   the heap group's internal audio_reverb_update); SetModelPalette leaves a named non-retail stub in the group object
   (not claimed); no unused locals, no compiled-out tests (the levers are listed in carcolor.c's header).
4. Joint unit check of all three: `scores/combined_unit_align2.txt` (align for render_thread_entry and
   entity_ai_pathfind): 11/11 EQUAL, `locked bodies that differ in this unit: 0`.

## Audio thread: how it was found (2026-10-22)
Harness: `ap/h0/` = single -O3 group (engine_sound_sync group files + aes.c (audio_effect_setup_b132.c minus its
func_80098FB8 context) + thread.c), keep = engine_sound_sync keep + entity_ai_pathfind; `gb.sh`/`tools/gbatch.py` batch
scores variants (aligned rows, ops rows, regime signature F/L = forwarded song arg, reg holding &gSong, count of `b`).
Same residual as the unit (219 vs 222 rows; only the `.align` pad differs).
1. **Retail's separate `b end` blocks are dead inlined value returns, not globalcolor jump blocks.** f_put_in_jump_bb
   is never reached in this unit (instrumented uopt, `bin/uopt.jb`). A block holding only a discarded call to an inlined
   function that returns a value (`static int DBG(void) { return 0; }`) survives uopt's jump threading (uopt keeps the
   `str $2`), ugen emits `move v0,zero`, as1 deletes it: the block becomes `b end; nop`. Sites in retail: case 1 when
   gSong < 0, case 2 after `startTime = -1.0f`, case 3 when sysId == -1, end of case 4, end of cases 7/8/9 (the empty-list
   `beqz` targets the loop-exit block), and a `default:` after case 11 (case 11's `b end` to the next instruction).
2. **Case 0's forwarding (`move a0,v1`, &gSong in v0) is f_split region order** (instrumented f_split + f_addadjacents,
   `jbpatch/splitpatch/adjpatch`): the &gSong web spans the loop and is split; f_split carves the first piece from the
   first unit with uses in the lr's unit list, whose order is the DFS order of f_formlivbb (fall-through successor of the
   switch dispatch first, then the successor list = cases 11..1). With case 0 first, its piece grows 9->12->160 (case-0
   join, then the switch end with ~20 predecessors) and is never coloured. Oracles: dropping block 160 (or 12) from the
   lr, or moving case 1's units first (`SPLORDER`), give retail's case 0 exactly. Source lever: an unreachable
   `goto skip; skip:` before `case 0:` (zero frame cost) removes case 0 as the dispatch's fall-through, the DFS then
   visits case 1 before case 0, case 1's piece takes block 160 and case 0's piece {9,10,11,12} is coloured v0.
3. Frame accounting: bare thread 136; each inlined call site +8. With GetAudioTime() inlined (its $f0 return is in
   retail) and 8 dead-return sites the frame is 208 = retail. _PlayFxTag written in place through the `tag` local gives
   retail's `move s4,s3` with no frame cost; the switch is written directly in the thread (no _ProcessCommand helper).

## Permission denials
None.
