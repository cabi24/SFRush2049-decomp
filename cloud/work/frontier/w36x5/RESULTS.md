# w36x5 (wave 36, second-pass game lane: waves.c convention cluster + audio thread) RESULTS

Status (updated after every function):

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| SetupWaveModel (0x80096CA8) | _SetupWaveModel@waves.c (conf) | 1204 | **group MATCH + unit EQUAL** (DC `modelIdx` + 1 disclosed empty-macro block) | groups/audio_frame_sync/waves.c |
| func_80097164 | _ReadWave@waves.c | 344 | **group MATCH + unit EQUAL**, no unused locals (inlined DMA helper = stub func_8009715C) | groups/audio_frame_sync/waves.c |
| func_8009715C | deleted inlined static: N64 DMA read (already-locked 8 B stub, gets its real body) | 8 | group MATCH + unit EQUAL | groups/audio_frame_sync/waves.c |
| fp_call_wrapper | handle-wave re-setup (DC func_8C065862; SetupWaveModel(idx, 0)) | 104 | **group MATCH + unit EQUAL** (was blocked only by SetupWaveModel) | groups/audio_frame_sync/waves.c |
| audio_frame_sync | LoadWave@waves.c | 520 | **group MATCH + unit EQUAL** (w36t's v7.c code unchanged) | groups/audio_frame_sync/loadwave.c |
| func_800972BC | deleted inlined _NewWaveInstance (already-locked 8 B stub) | 8 | group MATCH + unit EQUAL | groups/audio_frame_sync/loadwave.c |
| suspension_setup | ReplaceWave@waves.c | 240 | **group MATCH + unit EQUAL** (w36t's v1.c code unchanged) | groups/audio_frame_sync/replacewave.c |
| func_800A5014 | deleted inlined _ReplaceWaveInstance (already-locked 8 B stub) | 8 | group MATCH + unit EQUAL | groups/audio_frame_sync/replacewave.c |
| entity_ai_pathfind | N64 audio thread + MAT3 _ProcessCommand inlined | 2180 | 222 rows, ops 40, **frame 208/208 now exact** (MAT3 MyGetTime pair); residual = colouring/emission regime, written up | thread/best.c |
| func_80098FB8 | AudioMain (+ _PeriodicMaintenance/_ProcessPriorities inlined) | 756 | provisional: unit EQUAL with the thread (unchanged from w36t) | thread/best.c |

**Landable now: superseding group `groups/audio_frame_sync/` = 2,412 B of new game code (SetupWaveModel 1204 + func_80097164 344 + audio_frame_sync 520 + suspension_setup 240 + fp_call_wrapper 104), game +5 functions; three already-locked 8-byte stubs get their real helper bodies.** Nothing in cloud/matches (all are private-IPA group members). Provisional: func_80098FB8 756 (needs the thread). Not matched: entity_ai_pathfind 2180 (222 rows).

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36x5 (from base, master e68aea6e1). Probe scripts: tools/.

## Identities proven

| N64 | MAT3 / role | evidence |
|---|---|---|
| SetupWaveModel | waves.c _SetupWaveModel (Dreamcast func_8C06588C) | group MATCH |
| func_80097164 | waves.c _ReadWave | group MATCH |
| func_8009715C | deleted inlined static of waves.c (N64 only): DMA read (dst, rom, size) = osInvalICache, osInvalDCache, osPiStartDma(&mb,0,0,rom,dst,size,&D_80035410), osRecvMesg | group MATCH (its 8-byte stub + _ReadWave's frame/schedule) |
| fp_call_wrapper | handle-wave re-setup after a heap move: SetupWaveModel(idx, 0) (Dreamcast func_8C065862; no GC counterpart) | group MATCH |
| func_800972BC / func_800A5014 | deleted inlined _NewWaveInstance / _ReplaceWaveInstance (w36t's attribution, confirmed: their stubs carry the helper symbols in the matching group) | group MATCH |
| D_80140A04 | gMirrorWaves (s8): entity_render_mode (= MirrorWaveInstance) compares `mirrored` with (D_80140A04 != 0); SetupWaveModel passes `D_80140A04 ? 1 : 0` to entity_lod_select | bodies |
| D_80123564 / D_8011B5BC / D_8002E580 | per-wave storage kind (0 raw ROM DMA, 1 lzss, 2 inflate) / per-wave ROM address / gWaveSizes | _ReadWave |
| D_80035410 | PI DMA completion message queue (_ReadWave's DMA helper) | _ReadWave |
| brake_force_apply | N64 background wave-loader thread: osCreateMesgQueue(D_80156BB0 request queue, D_80156BD4, 1), osSetThreadPri(D_80034690, 3), loop {recv; _ReadWave(D_801569AC); osSendMesg(D_80035440 done queue)} | unit body identical except `.align 5` epilogue pad |
| gRomObjDefPtrs/gTexDefPtrs/gPalDefPtrs N64 layouts | tModelDef 0x58 (lodCount 0x16, lod[4] at 0x18, 16-byte lods: displayList +8, second relocated list +0xc); tTexture 0x24 (texBase 0x18); palette 0x18 (palBase 0x14); tBasicTargetPath 0x24 (trigPtr 0x18, pts 0x1c) | SetupWaveModel MATCH |
| _ProcessCommand MyGetTime pair | MAT3 audiomgr.c _ProcessCommand's two MyGetTime() calls survive on N64 as two empty inlined sites (thread frame 208 exact) | frame |

## Permission denials

None so far.

## SetupWaveModel: the convention lever is the Dreamcast `modelIdx` copy (2026-10-21)
Dreamcast `src/waves.c` func_8C06588C (= _SetupWaveModel, DC status `differs` 143 B, used as a structural lead only)
has `modelIdx=idx;` right after the WADptr select (exactly where retail N64 has `sw s8,68(sp)`) and indexes
gRomObjDefPtrs/gTexDefPtrs/gPalDefPtrs ONLY through modelIdx (gWaveInstance through idx). The w28d/w29d `n = idx`
was this variable, but only the i/j loop bounds used it. Measured (unit, `--internal SetupWaveModel --internal
func_80097164`, ctrace): uopt copy-propagates `modelIdx -> idx` everywhere except the loop back-edge tests, and every
propagated modelIdx occurrence counts toward idx's colouring gross, while direct idx occurrences inside PRE-deleted
address expressions do not. idx gross: base (idx refs) 15 -> DC style (all table refs via modelIdx) 340
(save 17.89 vs first 17.94: 55 rows, ops 0, length 301 exact, idx t0/first s8). Per site: j-loop body via idx 40
(so the body MUST use modelIdx), texdef or paldef loop via idx -11 each, prologue lookups via idx -3.
One more modelIdx gWaveInstance reference puts idx (341-344) above first (17.94) and below &ROD[modelIdx] (18.5):
idx -> s8, first -> t0 as retail: 31 rows, func_80097164 2 rows. Variants (all 31 rows): ready=1 via modelIdx (341),
lastPtr store (341), offset computation (341), one PTHD type test (342), both PTHD tests (344).
Remaining 31 rows = two register swaps: offset (w45 save 320/14 = 22.86) outranks i*88 (w121 131/6 = 21.83) -> s4/s5
swapped (retail i*88 s4, offset s5); idx*8 / TexDef address / IMAGptr s0<->s1.
Clean typed port `swm/swm.c` (MAT3/DC names, N64 offsets) reproduces it: SetupWaveModel 31 rows ops 0 frame 112/112,
func_80097164 2 rows. **Force oracle: the 31 rows are exactly two priority inversions** (force.sh on snapshot swm,
`p1:w45=c19,p1:w123=c18,p1:w221=c14,p1:w77=c15,p1:w187=c15,p1:w209=c18` -> `differing rows 0`):
(A) i*88 (w123, 131/div 6 = 21.83) must outrank offset (w45, 320/div 14 = 22.86) -> retail i*88 s4, offset s5 (and the
PalDef address then also lands in s4); (B) the gTexDefPtrs[idx] address (w221, 24/div 4 = 6.0) must outrank IMAGptr
(w77, 21/div 3 = 7.0) -> retail TexDef address s0, IMAGptr s1, idx*8 s1.
**SetupWaveModel unit EQUAL** (`swm/swm.c` = v/D2_dcpure.c): both inversions are one live-range length: a code-free
block boundary between the texdef and paldef loops (`do {} while (0);`, the locked func_800A79F4 precedent: an empty
debug-macro expansion) lengthens IMAGptr's and offset's live ranges (div 3->4, 14->15) without touching TexDef's or
i*88's. With it the pure Dreamcast form (gWaveInstance always through idx) is enough: idx 340/19 = 17.89 > first
323/19 = 17.0. `if (IMAGptr) {}` at the same place also gives EQUAL; holders inside either loop change the code.

**func_80097164 = _ReadWave unit EQUAL** (`swm/swm.c`): the caller-less locked stub func_8009715C between
SetupWaveModel and _ReadWave is a deleted inlined static, the N64 DMA read `(dst, rom, size)` with its own
`OSIoMesg mb` (osInvalICache, osInvalDCache, osPiStartDma, osRecvMesg on D_80035410). Defined before _ReadWave its body
has lower .loc lines, which gives as1's tie-break retail's `move a0,s0` before `lw s3` (the "known as1 row" of
w27e/w31d), and its inline site replaces w27e's three unused locals (frame 112 exact, no unused locals).
`blob_unit --tag w36x5 score SetupWaveModel func_80097164 func_8009715C fp_call_wrapper --internal SetupWaveModel
--internal func_80097164 --internal func_8009715C --with cloud/work/frontier/w36x5/swm/swm.c --neighbours`:
`EQUAL SetupWaveModel: 301 words (internal, c_swm.c)`, `EQUAL func_80097164: 86 words (internal, c_swm.c)`,
`EQUAL func_8009715C: 2 words (internal, c_swm.c)`, `EQUAL fp_call_wrapper: 26 words (kept, g_audio_frame_sync__group.c)`,
`locked bodies that differ in this unit: 0`.

## Group `audio_frame_sync` (supersedes the installed group IN PLACE) - READY TO INSTALL
New code bytes: SetupWaveModel 1204 + func_80097164 344 + audio_frame_sync 520 + suspension_setup 240 +
fp_call_wrapper 104 = **2,412 B** (func_8009715C/func_800972BC/func_800A5014 are already-locked 8-byte stubs that
get their real helper bodies; no new bytes).
- builder `score.py group cand/audio_frame_sync` (exit 0): func_800972C4, SetupWaveModel, func_8009715C,
  func_80097164, fp_call_wrapper, audio_frame_sync, func_800972BC, suspension_setup, func_800A5014: `MATCH`;
  context func_80097694/func_80096288/entity_render_mode `MATCH`, brake_force_apply `12/49 words differ (5 extra
  words ...)` (the `.align 5` endless-loop epilogue, see below), entity_lod_select `179/179` (unchanged m2c context).
- `score.py group --claims` exit 0, all 8 claims `MATCH`.
- unit: `blob_unit --tag w36x5 score SetupWaveModel func_80097164 func_8009715C fp_call_wrapper audio_frame_sync
  suspension_setup func_800972C4 func_800972BC func_800A5014 --internal SetupWaveModel --internal func_80097164
  --internal func_8009715C --internal func_800972BC --internal func_800A5014 --internal _BGLoadWave --with waves.c
  --with loadwave.c --with replacewave.c --neighbours` -> all 9 `EQUAL`, `locked bodies that differ in this unit: 0`.
Integration (exact):
- group name `audio_frame_sync` (same name; old member func_800972C4 kept, its text unchanged in group.c).
- members: func_800972C4, SetupWaveModel, func_8009715C, func_80097164, fp_call_wrapper, audio_frame_sync,
  func_800972BC, suspension_setup, func_800A5014; claims: all but func_800972C4.
- files: group.c (old group.c minus the context bodies of audio_frame_sync, fp_call_wrapper, SetupWaveModel,
  func_80097164, suspension_setup and the two `__standin_*` callers; its func_80096288 context replaced by the locked
  slot_sound body verbatim, with the dead `if (0) switch` inline blocker, so suspension_setup keeps its jal),
  waves.c (w36x5), loadwave.c / replacewave.c (w36t's code unchanged, helpers made non-static for stub symbols),
  entity_render_mode.c (byte-identical copy of src/blob/groups/slot_sound/entity_render_mode.c; context: its second
  call site keeps func_80096288 out of line in the group compile).
- keep: audio_frame_sync, brake_force_apply, fp_call_wrapper, suspension_setup, entity_lod_select,
  entity_render_mode (stand-ins removed). context: brake_force_apply, func_80097694, func_80096288, entity_lod_select,
  entity_render_mode.
- unit_overrides: force_internal SetupWaveModel, func_80097164, func_8009715C, func_800972BC, func_800A5014,
  _BGLoadWave; prefer_definition func_8009715C -> groups/audio_frame_sync/waves.c, func_800972BC ->
  groups/audio_frame_sync/loadwave.c, func_800A5014 -> groups/audio_frame_sync/replacewave.c (the locked stub singles
  src/blob/func_8009715C.c, func_800972BC.c, func_800A5014.c are superseded, as w36t_loadcars did for func_8009768C);
  prefer_definition audio_frame_sync -> groups/audio_frame_sync/loadwave.c (w36t_loadcars/loadwave.c and
  codex_state_audio_a146/group.c also define it as context).
- Disclosed devices: SetupWaveModel's empty `do {} while (0);` between the texdef and paldef loops (empty
  debug-macro expansion; func_800A79F4 precedent). `_BGLoadWave` is an invented helper name (no retail stub symbol
  found for it; w36t's frame evidence). No unused locals, no pad, no volatile.
- brake_force_apply (N64 background loader thread, 196 B, not claimed): body identical in the unit except the
  `.align 5` padding nops before its unreachable epilogue (3-4 nops depending on the object offset); claimable only
  with an `align` unit override like task_complete_signal. Not attempted further.

## Audio thread (entity_ai_pathfind + func_80098FB8): frame fixed, residual written up
Base: w36t thread/best.c (all reconstruction is w36t's). Unit
`blob_unit --tag w36x5 score entity_ai_pathfind func_80098FB8 --internal func_80098FB8 --with thread/best.c`:
`FAIL entity_ai_pathfind: 451 of 545 words differ; compiled body is 533 words, target 545` (us.sh: `words 222 ops 40
norm 40 | frame 208/208`), `EQUAL func_80098FB8: 189 words (internal, c_best.c)` (provisional, lands only with the thread).
- **Frame 192 -> 208 exact**: MAT3 _ProcessCommand brackets the switch with `timer = MyGetTime(); ... gAudioTime +=
  MyGetTime() - timer;`. Two inlined empty MyGetTime() sites (8 B each) give 208/208; the timer/gAudioTime arithmetic
  itself is not on N64 (with it: frame 216 and extra code). Placement of the two sites is code-neutral (6 variants).
- **The "148-block threshold" is not a block threshold.** Probe set (single-file -O3 group, `uopt -v`, case-0 arg
  reload check; scripts in the scratchpad pattern uv.sh/probe.sh): case 5 emptied = 147 blocks FWD; +2 holder ifs (151)
  still FWD; +1 call (148) NOFWD; +1 real if (149) NOFWD but +2 real ifs (151) FWD; -4 plain calls (151) NOFWD;
  expression-bit counts do not separate the classes either. Non-monotonic: not a size limit.
- **What the symptoms really are (traced):** uopt's PRE treats gSong identically in forwarding and non-forwarding
  builds (bit `ilod gSong` antloc/DELETE sets identical); the case-0 reload and the `&gSong` handling come from the
  colouring/emission of the `&gSong` address web (w208 `ldaS(gSong)`: save 1.16, nocs 43, split, best 342.5 - a
  loop-invariant address spanning the whole endless loop). Retail colours a case-0-local piece of it in v0
  (`addiu v0,v0,576; lw a0,0(v0); ...; sw v1,0(v0); move a0,v1`); the PA-style builds hoist it into s6; ours splits it
  with no coloured piece. Retail's separate `b end` blocks (cases 1, 2, 3, 4) are **globalcolor's inserted jump blocks**
  (uopt `f_put_in_jump_bb` is called only from `f_globalcolor`): live-range-split edges whose spill code vanished.
  Source-level `if (c) {} else {...}` / `f(gSong = x)` / file-static gSong forms compile identically (tested).
- Next: force-oracle the `&gSong` web pieces (force.sh on snapshot t2 is set up) and look for what makes a case-0-local
  piece colourable (f_split region growth from the case-0 blocks), rather than block-count hunting.

## What generalises (w36x5)
- **The Dreamcast decomp carries N64-era statements that MAT3 (GC) dropped.** DC _SetupWaveModel's `modelIdx = idx;`
  (all model-table indexing through the copy) was the whole idx-vs-first convention problem that blocked four
  functions for eight waves. When MAT3 and N64 disagree structurally, read the DC version of the function.
- **uopt copy propagation feeds colour priority**: a local copy `m = p` used in place of `p` makes every propagated use
  count toward `p`'s colouring gross (and uopt does not propagate into loop back-edge tests, which is the reloaded
  `lw t6,68(sp)` bound). Direct uses of `p` inside PRE-deleted address expressions count for nothing.
- **Two priority inversions from one live-range length**: a code-free block boundary (`do {} while (0);`) placed
  where two webs' live ranges differ changes their divisors (L29's `((n-2)>>2)+2`) and flips their order. Force first
  (force.sh proved the two swaps were the whole residual), then place the boundary where only the webs to demote are live.
- **A caller-less stub before a function is often its inlined helper, also for as1 ties**: defined before the caller,
  the helper's lower `.loc` lines fixed the "known as1 row" of _ReadWave that three waves left, and its inline site
  replaced three disclosed unused locals.
- **Group packaging of inlined helpers**: name each inlined static after its retail deleted stub and make it
  non-static, so the stub carries a symbol and no member gets "extra words"; a locked callee that must stay out of line
  needs its real inline blocker body AND a second call site in the group (single call sites are inlined regardless).
- **Retail `b end`-only blocks after an if are globalcolor's inserted jump blocks** (`f_put_in_jump_bb` is called only
  from `f_globalcolor`): they are live-range-split artifacts, not source `if/else` shapes. And the "148-block forwarding
  threshold" of w36t is not a block count (non-monotonic in probes): treat such regimes as colouring problems.
- MAT3 MyGetTime()/stats* timing calls survive on N64 as empty inlined sites: count them for frame residuals.
