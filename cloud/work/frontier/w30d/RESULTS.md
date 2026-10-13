# w30d — control settings / race end (animPlace) / model parser / track loading (wave 30)

Status: FINAL. Branch wave30 (main checkout); nothing committed, spliced or pushed. No permission denials.
Builder scratch watchman2:~/rush2049/scratch/frontier/w30d (toolkit reused from wtk) deleted at the end.
Flags everywhere `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3); unit = `blob_unit --tag w30d` (trace toolkit `us.sh`).

**Landable: animPlace (1,000 B)** via the superseding packet `groups/credits_scroll_grp/`.

| Function | Bytes | Before | After | Verdict | Residual class |
|---|---:|---|---|---|---|
| **animPlace** (race_finish) | 1,000 | 40/250 rows 40, ops 6, 1 device (w29c) | **group MATCH** (unit EQUAL, `score.py group` MATCH) | — (w29c body EQUIVALENT 18,271 calls) | — (1 disclosed device `if (i) {}`) |
| control_settings | 2,552 | 603/638, rows 663, ops 164, body 608, frame 1904/1896 | 623/638, rows 283, ops 135, body 629, frame 1872/1896 (2 oracle devices) | never called by any script | colour (row needs +2 weight), font_set inline shape, DD0C0 stack args |
| control_cell_text / text2 / text3 | 264/692/820 | EQUAL / rows 17 / rows 35 | unchanged (real-caller draft) | never called | as w28d |
| func_800E543C | 700 | rows 180, ops 24 (w28d, 3 devices) | unchanged | w28d EQUIVALENT 122 calls | index live range (traced, not solved) |
| SetupWaveModel (func_80096CA8) | 1,204 | rows 233 (w29d best) | unchanged | w29d EQUIVALENT 282 calls | idx vs first priority (traced, not solved) |
| func_80097164 unit | 2,504 | — | not attempted (blocked on SetupWaveModel) | — | — |
| LoadTrack (func_800BB9B0) | 2,104 | 40/526, rows 41, frame exact WITH 2 unused locals | 39/526, rows 40, frame exact, **no unused locals** | **EQUIVALENT 41 calls / race, attract** | WorldBase address form, temp ring |
| track_texture_load, props_render | | | not attempted (time) | | |

### Integration notes (exact)
- `groups/credits_scroll_grp/` supersedes the locked group IN PLACE: same name, all 8 old members, all old files
  byte-identical (copied from src/blob/groups/credits_scroll_grp at wave30 1b34f797), + `gr3_7af4.c` (animPlace).
  group.json: members += animPlace, keep += animPlace, files += gr3_7af4.c, claims [animPlace], provenance appended.
  Helpers gfx_lock_7AF4 / gfx_unlock_7AF4 / font_set_7AF4 / pair_set_7AF4 are non-static, not members, not keep
  (as *_761C; leave no stubs). unit_overrides: none expected (as lap_complete); if the shadow unit reports them,
  force_internal the four helper names. No data-symbol changes. Locked stub func_80107AEC untouched.
- Nothing else is installable. control_settings is NOT packaged (not matching); when it matches, the packet is
  codex_control_settings_a4 superseded with members func_800DCD58, func_800DCCE0 (kept bodies) + control_settings +
  control_cell_text/2/3 (and func_800DD0C0, whose IPA stack-argument convention control_settings depends on).

## control_settings (lever hour done) — `control_settings/best.c`, run with `control_settings/run.sh FILE.c`
`control_settings: want 638 words, got 629; differing rows 283 (words); frame 1896/1872 | ops 135` (was rows 663,
ops 164, body 608). control_cell_text EQUAL (66), text2 rows 17, text3 rows 35 (unchanged; real-caller draft).
**Why row/col miss s3/s4 (traced, ctrace raw p1dec/p1cost):** the procedure's base callee-saved cost is 32.0
(= nblocks/4, 128 blocks; w29c's FONT_SET `do {} while (0)` added 8 blocks -> 34.0). col's WHOLE web: totalsave 55
vs s4 cost 56.2 (32 + 24.2 = the save/reload penalty around the IPA handler calls that clobber s4 — exactly
retail's `sw s4,1892 / lw s4,1892` pairs) -> split; its loop piece then takes v0. row: 25 vs s3 36.4 (32 + 4.4,
slot_state_setup clobbers s3 — retail's `sw s3,1896 / lw s3,1896`) -> split. So retail colours both whole webs
and pays those penalties; retail's row/col need more weight (or ~8 fewer blocks for row, 5 for col).
Oracle evidence: `force.sh p1:w8=c17,p1:w196=c18` -> 436 rows; compiled-out `if (col) {}` in the search loop
(+10, loop weight) -> col in s4 as retail (tot 65 > 56.7); `if (row) {}` there -> row 35 vs 36.9 (still 2 short).
In best.c both holders are present as DISCLOSED devices (they are an oracle for the weight, not a claim).
Inlined `cell(row, col)` helpers in the loop add frame (+8 B/site) but no weight (params copy-propagated).
Frame: direct pair stores give 1872 (retail 1896 = +24 = one nested font_set site or three 8-B inline sites);
the inlined pair_set costs 16 B/site (1904); nested font_set at all 4 sites 1968; one-piece wrapper not inlined.
Other residuals: retail's four `move s0,v0` (inlined font_set return copies) and &num rematerialised (ours colours
&num in s0); func_800DD0C0 (unmatched, IPA) takes (row, col) in the caller's 0(sp)/4(sp): needs DD0C0 as C.
Next: find the natural source of the loop weight for row/col (candidate: the arcade/N64 original iterates
`D_80153FD8[row][col]` with an index expression that survives SR), then the 24 B inline area; package only then.
Not packaged (not matching).

## animPlace (old label race_finish) — **group MATCH** via `groups/credits_scroll_grp/` (READY TO INSTALL)
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w30d score animPlace --with cloud/work/frontier/w30d/animPlace/v/r11.c --neighbours
  EQUAL animPlace: 250 words (kept, c_r11.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal
ssh watchman2 '... python3 tools/cloud/score.py group cand/credits_scroll_grp'   (no --claims)
  credits_scroll / func_80100B8C / slot_state_setup / world_trigger_check / func_8010A53C / func_80107EDC /
  animLapTime / animElapsedTimerText / animPlace: MATCH   (context entries informational, unchanged)
```
Path 40 rows -> 0: (1) spcensus on the y homes: retail (s16)(y+1)/(s16)y at 76/72, ours 72/68; spill.sh showed
our homes slot0 = D_80151AD0 load + &player_array[i] (shared), slot1 = &str[k], slots 2/3 = the y webs => one 4-B
item too many in the merged area: the `Pair *p` local of the inlined pair_set. pair_set writing the global
directly at the SECOND site -> homes 72/76 as retail (rows still 40). (2) `x = placePos.x; y = ...; str[0] = ...;
x -= w;` (split assignment): the placePos address web is now numbered before y (save tie 30/30 -> address v0,
y v1 as retail) and the loads schedule y, place, x, w (rows 2: `li v0,1`/`li t7,3` order at the first pair site).
(3) pair_set direct at BOTH sites (as animElapsedTimerText's pair_set_761C) -> EQUAL, frame 280 exact.
Devices (disclosed in the file header): compiled-out `if (i) {}` at the end of the outer loop body (w29c's;
without it 87 rows). The split `x -= w` is a statement form, not a device.
Packet: `groups/credits_scroll_grp/` = the current locked group (src/blob/groups/credits_scroll_grp, all files
byte-identical) + `gr3_7af4.c`; group.json members += animPlace, keep += animPlace, files += gr3_7af4.c,
claims [animPlace]. Helpers gfx_lock_7AF4/gfx_unlock_7AF4/font_set_7AF4/pair_set_7AF4 non-static, not on keep (as
*_761C; force_internal them only if the shadow unit complains). No data-symbol changes. Locked stub func_80107AEC
(just before animPlace) untouched. `score.py fn` standalone not applicable (slot_state_setup s2 convention).

## func_800E543C (lever hour, no new best; w28d/func_800E543C/best.c stands: rows 180, ops 24, frame 72/72)
pretrace (index = varP(+6), bit 1): ours colours index in a2 with nocs 5 — its live range ends in the head
blocks because every later occurrence sits inside PRE'd address expressions (&player_array[index] t0/home 36,
&D_8014A250[index] a3/home 32, &D_80153E88[index] v1/home 28). Retail's index web (t0) is live ACROSS
func_800C4F68 (saved `sh t0,78`, reloaded `lh t0,78`) and into the stunt block (saved before
stunt_combo_display, never reloaded) — so retail has a real use of index after the stunt call in the same
block that later disappears, or the stunt block contains an index occurrence that is not PRE-deleted.
Tried (no index spill appears in ours): `stunt_combo_display(veh(index))` with an inlined getter (rows 147 but
frame 88 and arm order inverted — not an improvement), `if (index) {}` after the stunt call (removed by uopt,
rows 180). Next: pretrace retail-shaped hypotheses for a post-stunt index use that survives to the colourer
(e.g. `player_array[index].clear = 0` written through a differently-typed index expression, or the arm-order
of `if (action == 0)` reversed so the C4F68 arm is the join predecessor).

## SetupWaveModel (old func_80096CA8; lever ~30 min, no new best; w29d best.c stands: unit 290/301, rows 233, ops 35)
Re-measured in the current unit (`--internal SetupWaveModel --internal func_80097164`, s0 = w29d best.c):
`FAIL SetupWaveModel: 290 of 301 words differ ... | words 233 ops 35 norm 35 | frame 112/112`. ctrace: first (w11)
save 17.94, nocs 18 -> s8; idx (w0/w206) save 1.0 / 8.9 -> v1 piece + split. Retail's idx (s8) is used only at
entry, `sw s8,68(sp)` (n), `sll s1,s8,3` and `move a0,s8` at the end — no loop uses survive in retail either, so
its colouring-time weight must come from loop occurrences that PRE later deletes (exactly what the w29d holders
emulate). Every idx occurrence the draft has in the loops sits inside &D_801161F4[idx], which PRE hoists before
the colourer counts it. GC `_SetupWaveModel(int idx, int first, int loadTextures)` (waves.c) adds an in-loop
`gWaveInstance[idx].waveNum` test (HOODSHEEN/SHEEN/BRAKE, GC-only) — the N64 has no such test in the loop, but
it is the kind of occurrence that would carry the weight. GC local `waveNum` (int) exists; the N64 reloads
`gWaveInstance[idx].type` at both PTHD tests (no local). Not pursued further (stop rule; w29d already spent an hour).
func_80097164 unit: not attempted (blocked on this convention).

## LoadTrack (still func_800BB9B0 in the tree; w30n renames it) — `LoadTrack/best.c`, 41 -> 40 rows, devices -2
`FAIL func_800BB9B0: 39 of 526 words differ | words 40 ops 9 norm 9 | frame 168/168`
(`UARGS="--internal w29_FindWorld --internal w29_NewWorld --internal w30_Clear" us.sh func_800BB9B0 best.c`).
Lever 1 (8 B of inline area below FindWorld's whdr) SOLVED: the N64-only `D_80110680[0] = 0; D_80110680[1] = 0;`
(GC LoadTrack has nothing between InitVisuals and InitCheckpointData; retail forms the address in v0 = a pointer)
is an inlined helper `w30_Clear(s8 *p)` (non-static, --internal; its 8-B area is below FindWorld's in call
order): whdr at 48/wname at 52 as retail AND w29d's two unused GC locals swapTG/swapT are no longer needed (frame
168 exact without them). Variants: no-parameter helper -> not inlined (call stays); `s8 *p` local -> frame 176.
Helper identity is a hypothesis (disclosed in the header); no unused locals left.
Lever 2 (WorldBase) NOT solved: retail `lui at; sw` + `lui a0; lw a0` (reload of the just-stored global through two
separate %hi), ours `extern volatile` gives one coloured address (v1). Non-volatile forwards the value (rows 47);
`volatile u32 []` element form and a `do {} while (0)` between store and reload change nothing. Also left: the
temp ring is one register behind from the trackInited load (+0x3e8, as1-hoisted from the +0x6ac block: retail t6,
ours t9) although all code before it matches -> a code-free temp pop in the "other" wave block differs (ugt.sh next).
draft_check (best.c, `--scripts race,attract`): `VERDICT: EQUIVALENT over 41 calls / 2 scripts (race 6, attract 35)`.
Integration when it matches: single (o32); w29_FindWorld/w29_NewWorld/w30_Clear non-static, force_internal.
ugt.sh (l6): our last ring allocation before the trackInited test is t8 at the second `D_80156994 || D_8014978C >= 6`
test; retail's next temp is t6, ours t9 -> retail has exactly one more code-free ring allocation between that test
and `if (trackInited)`. `switch (D_8014A110)` instead of the if/else-if: worse (62 rows); `!= 0` spelling: no change.

## GC-derived names / layouts recorded this wave
- animPlace = GC hud.c `animPlace` (as w29c): func_800CF604 = AreWeSolid, D_80115CA8 = placePos, D_80116168 =
  playerColor, D_8016137C = gShowPlace; statement form `x = placePos.x; ... x -= digitW` (N64 retail evidence).
- LoadTrack: GC tracks.c LoadTrack / world.c NewWorld (store WorldBase, NewWorldObject(woff, parent), InitSTree) —
  GC passes the value, N64 reloads WorldBase. The N64-only `D_80110680[0..1] = 0` has no GC statement (helper
  `w30_Clear(s8 *)` is a placeholder name).
- SetupWaveModel: GC signature `static void _SetupWaveModel(int idx, int first, int loadTextures)`, GC local
  `waveNum`; the N64 drops loadTextures and the HOODSHEEN/SHEEN/BRAKE in-loop block.

## What generalises
- **An inlined helper's `Pair *p` local is a 4-B item in the merged area that shifts every spill home below it**:
  spcensus + spill.sh (slot order of the coloured expression webs) located it in animPlace; dropping it closed the
  homes without touching the frame.
- **Split assignment `x = a.x; ...; x -= w;` renumbers the address web** (the shared &placePos[..][i] then appears
  first, before y): a v0/v1 save tie flipped and the load order followed — 40 rows to 2.
- **A frame residual "8 B of inline area below helper X" is an inlined helper LATER in call order**: LoadTrack's
  N64-only two-byte clear as `helper(s8 *p)` (a parameter, not a local: a local made it 16 B; no parameter: not
  inlined) replaced two disclosed unused locals.
- **control_settings' callee-saved threshold is nblocks/4 (32.0 = 128 blocks) plus per-register IPA clobber
  penalties** (save/reload around the slot_state_setup and handler calls). A web is coloured whole only if its
  totalsave beats base + penalty; retail's sw/lw pairs around those calls ARE the penalty. Each `do {} while (0)`
  in a macro costs 2 blocks (w27s) — 0.5 of the threshold here.
