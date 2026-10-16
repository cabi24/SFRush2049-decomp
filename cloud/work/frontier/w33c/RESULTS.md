# w33c — collision / path / blast (GC-driven), wave 33

Branch `wave33` main checkout. Builder scratch `watchman2:~/rush2049/scratch/frontier/w33c`.
Flags `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3) unless stated. Permission denials: none so far.

## Table (in progress)

| Function | Bytes | Start | Now | Verdict | Residual |
|---|---:|---|---|---|---|
| entity_iterate | 224 | 15/56 (w33p) | **MATCH** standalone + unit EQUAL | `cloud/matches/entity_iterate.c` | none |
| func_800B9740 | 408 | 83/102 (w14r) | **MATCH** standalone + unit EQUAL (first draft) | `cloud/matches/func_800B9740.c` | none |
| audio_effect_apply + physics_response | 696 + 920 | no fresh score (w17h ctx stand-in); physics_response provisional | **MATCH (group)**: unit EQUAL 2/2, `score.py group` MATCH/MATCH with and without `--claims` | `groups/frontier_read_path_data/` | none |
| func_80090308 | 1,128 | 210/282, rows 198, ops 87 (natural) | **180/282, rows 152, ops 16**, frame 168/272 | `blast/best_effects.c` (draft) | rand4 seed reload, IV s2/s3, as1 extra block, frame 104 B |
| entity_physics_update | 712 | 21/178 colour | 21/178 (not worked further) | — | state web save 2.5 vs 2.0 (w23d) |
| save_write_data | 1,200 | 149/300 | 149/300 (frame map only) | — | frame 248/352 |
| camera_look_at_point | 796 | 38/199 (w11c, group context) | 38/199 (GC single `ratio`: identical code) | — | f0/f2 colour + temp ring one step (w11c) |


## 1. entity_iterate (GC CameraCollision) — MATCH

Start: w33p `entity_iterate/best.c` (15/56, frame exact). ugt.sh (SCHED=1) showed ugen evaluating the
`ipos[k]` reload BEFORE `temp[k]*0.25f` in each post-call statement, while retail's ring order (temp→$f10,
product→$f16, ipos→$f18, sum→$f4) shows the product evaluated first: the add's operand order inside uopt is the
whole residual (as1 order and FP colours follow from it). GC CameraCollision's `fmadds f0, f2(0.25), f1(temp), f0(ipos)`
reads as `ipos[k] = ipos[k] + 0.25f * temp[k]`; that spelling is EQUAL (the constant-first product changes uopt's
node order; `+=`, `temp*0.25f + ipos`, a pointer copy and a named `s` local all stayed 15/56).
```
us.sh: e.c: EQUAL entity_iterate: 56 words (kept, c_e.c) ... frame 88/88
blob_unit --tag w33c score entity_iterate --with cloud/matches/entity_iterate.c --neighbours
  EQUAL entity_iterate: 56 words (kept, c_entity_iterate.c); stub tail: 2 words of deleted-procedure stubs follow (NOT as in the image)
  locked bodies that differ in this unit: 0
  blob_unit score: 1/1 equal
builder: score.py fn cand/entity_iterate.c entity_iterate --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  entity_iterate:
    MATCH
```
Integration: single, `splice_singles.py entity_iterate` from `cloud/matches/entity_iterate.c`. No overrides, no
groups superseded, no data symbols. The "stub tail" note is the known unit-ordering artefact (w13f/w16d; the file
defines no statics). Rename lead: entity_iterate = GC `CameraCollision` (input_deadzone_apply =
`ShotToWorldCollision`, func_8008E0B8 = `NormalVector`, per PR #379 RESEARCH item 1). Shaping devices: none.
Lesson: GC `fmadds a,b,c` (= a*b + c) gives the N64 add's operand order directly: write `c + a * b` with GC's
multiply operand order — a constant-first product is a different uopt node from `temp * 0.25f`.

## 2. func_80090308 (GC StartNewBlast) — 210 → 180 words, ops 87 → 16 (draft)

GC StartNewBlast (reference/gc/disasm/visuals.c/StartNewBlast.s) evaluates each Random inside its store statement
(`scaleInc = .05f + Random(.005f)`, `yawInc = .1f - Random(.2f)`, ...), sets `clr.a = 128` after the third Random,
then ht/yaw/pitch = 0, alpha = 128, alphaInc = (int)Random(10) + 17 (N64 +25), SetColor. Porting that statement
order (no a/b/c temporaries; direct `D_80154660[car].particles[i]` indexing; `p` only for the timer and SetColor
handle, = retail s5) took ops 87 → 28 at once (v/g1.c); `color.c.a = extra.alpha` read-back 28 → 26 (g4); w25d's
inlined 5-argument colour setter for the extra block gives retail's shared 255/95 constants: ops 16 (g8 =
`blast/best_effects.c`). Same command as w33p:
```
blob_unit --tag w33c score func_80090308 entity_physics_update save_write_data --with blast/v/g8.c
  --with <PR>/blast/baseline_start_blast.c --internal func_80090308 --neighbours
  FAIL func_80090308: 180 of 282 words differ; compiled body is 281 words, target 282; +0x3ac: .rodata+0x184: ...
  FAIL entity_physics_update: 21 of 178 words differ
  FAIL save_write_data: 149 of 300 words differ; +0x1f8: .rodata+0x470: ...
  locked bodies that differ in this unit: 0
us.sh: g8.c ... | words 152 ops 16 norm 16 | frame 272/168
```
Remaining ops: (1) the rand4 seed reload (retail `lw t6,0(s1)` after rand3's store; stores through `p` of the zero
fields / alpha / spin_z do not produce it: g10–g13 identical 177 words, ops 16 — `p` is copy-propagated); (2) the
two loop IVs (table pointer and matrix pointer) take s3/s2 instead of retail's s2/s3; (3) as1 order of the
extra-block colour bytes. A named `Color *cp` for every colour store (g2) also gives retail's `li s8,128` sharing but
stores through s7 (ops 42). Signed byte types for the colour/alpha (g5–g7) are inert.

Frame (unchanged problem, now quantified for the GC locals): GC StartNewBlast's locals are v, i, clr only. Ours with
node, i, p, color = frame 168 (node -4, color -12). Retail 272: node -4, color -60 (13 words between), and the area
below color is ~60 B larger than ours. `f32 mat[12]` after node (f1.c) gives node -4 / color -60 ... frame 216, still 56 B short.
Under the "every inlined call reserves >= 8 B" law the 56 B are ~7 more inline sites than ours, or unused locals
(N64-only, as StartCone's five). Not resolved.

## 3. save_write_data (GC StartBlast) — frame map only

Retail 352: save area to 136; named slots (sp+) node 348 (-4), m 340 (-12), two unknown words at 344/336/332, then the
inlined on-car helper (GC StartNewBlastRing): s16 param 330, node 324, [12 words], blast 272?, color 268 (-84);
then 116 B with no reference (152..267); spill homes 144/148 at the bottom. GC StartBlast has locals v, mat
(pointer) and params data/linkToCar/scale/playSound; the N64 sound helper (play_at = EmitGameSound wrapper) is
inlined. No source change tried (no lever with a prediction; stop rule applied: 5 prior lanes).

## 4. func_800B9740 (GC CalcResPathBounds) — MATCH, first draft

Typed/indexed port of GC CalcResPathBounds exactly as RESEARCH item 5 predicted: s16 extrema with chained
initialisers, `int i, j, validPt`, `D_801407F0.alt[j]` indexed alternate ranges (16-B records: u8 type +0, u16 numPts
+0xA, point pointer +0xC; header u16 numPrimary +0, u8 numAlt +8, alt pointer +0xC), no early break, and one ternary
store per axis inside a `j < 3` loop. EQUAL on the first compile (w14k/w14r's break + if-form plateaued at 83).
```
us.sh: p1.c: EQUAL func_800B9740: 102 words (kept, c_p1.c) | words 0 ops 0 norm 0 | frame 8/8
blob_unit --tag w33c score func_800B9740 --with cloud/matches/func_800B9740.c --neighbours
  EQUAL func_800B9740: 102 words (kept, c_func_800B9740.c)
  locked bodies that differ in this unit: 0
  blob_unit score: 1/1 equal
builder: score.py fn cand/func_800B9740.c func_800B9740 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  func_800B9740:
    MATCH
```
Integration: single, `splice_singles.py func_800B9740`. No overrides, groups or data-symbol changes. Rename lead:
func_800B9740 = GC `CalcResPathBounds`; data: D_801407D4 = gPathMin, D_801407B4 = gPathMax, D_801407F0 = gResPath,
D_801409E8 = gResPathPtList, D_801527A4 = gNumResPathPts (GC sda names from the disassembly). It was the
`sole_blocker_for: audio_effect_apply`.

## 5. audio_effect_apply (GC ReadPathData) + physics_response — MATCH as group `frontier_read_path_data`

With func_800B9740 matched and audio_mixer_main locked (w33i), audio_effect_apply had no blocker left. Fresh draft from
the GC ReadPathData body (the w17h ctx.c m2c stand-in was not used): GC statement order, s16 i/p/j counters, the
GC byte-swap loops kept as empty-body loops (N64 data is big-endian; retail still runs the s16 counters), types
as func_800B9740's (`ResPath` header: u16 numPrimary @0, Vertex *pts @4, u8 numAlt @8, AltPath *alt @0xC;
`DronePath` {u16 num; pts} 8 B). First compile 34 aligned rows (ops 4): retail forwards gAltPaths' value from the
if/else into the following loop. GC's else arm is a CHAIN (`stw r0,gAltPaths; stw r0,0xc(r3)` from one `li r0,0`):
`D_801407F0.alt = D_80153E80 = 0;` → EQUAL (b1; `D_80153E80 = 0; D_801407F0.alt = D_80153E80;` also EQUAL, b3;
`if (numAlt)` instead of `> 0` costs 1 word, b2). No devices in audio_effect_apply.
```
blob_unit --tag w33c score audio_effect_apply physics_response func_800B9740 --with groups/frontier_read_path_data/audio_effect_apply.c
  --with groups/frontier_read_path_data/physics_response.c --with cloud/matches/func_800B9740.c --internal physics_response --neighbours
  EQUAL audio_effect_apply: 174 words (kept, c_audio_effect_apply.c)
  EQUAL physics_response: 230 words (internal, c_physics_response.c)
  EQUAL func_800B9740: 102 words (kept, c_func_800B9740.c)
  locked bodies that differ in this unit: 0
  blob_unit score: 3/3 equal
builder: score.py group cand/frontier_read_path_data --claims   -> physics_response: MATCH (own .rodata verified at
  0x80123DF4..0x80123DF8) | audio_effect_apply: MATCH | context func_800B930C: MATCH | rc=0
         score.py group cand/frontier_read_path_data            -> same, rc=0
```
**Integration notes (`groups/frontier_read_path_data/`):** NEW group, members physics_response + audio_effect_apply
(claims both), context func_800B930C (defined verbatim in physics_response.c, kept, as world_physics_tick.c and
frontier_race_countdown_display do), keep audio_effect_apply + func_800B930C. `physics_response.c` is byte-identical
to w17h's provisional file (two disclosed `do {} while (0);` devices there, w17h RESULTS). Remove the
`physics_response` entry from `cloud/work/frontier/provisional.json`. Until the lock derives it, the unit default
keeps physics_response (default run: `FAIL physics_response: 229 of 230 words differ; compiled body is 247 words`), so
unit_overrides `force_internal` += `physics_response` (as frontier_route_cross needed for audio_priority_find).
Land cloud/matches/func_800B9740.c (single) in the same batch (it is a callee; not required for the group compile).
No data-symbol changes. Rename leads: audio_effect_apply = ReadPathData, physics_response = CalcCheckpointTimes,
physics_friction_apply = SyncAltPaths (GC call order in ReadPathData: CalcCheckpointTimes, SyncCheckpointsToPaths(-1),
SyncAltPaths, CalcResPathBounds, CalcCheckpointTimes). Data: D_80153F20 = gPathWave, D_80153E80 = gAltPaths,
D_8012E5E8 = gDronePaths, D_801527D0 = gNumPathPts, D_80153EF8 = gPathPtList, D_80114650 = gRestartGame.

## 6. entity_physics_update (GC AnimateBlast) — not worked further

GC AnimateBlast is the same control flow as the PR's natural draft (kill block under `!op`, redundant `slot >= 0`
re-test, count==3 StartNewBlast). No new lever from GC: the residual is still w23d's state-web priority (save 2.5 vs
func_80090770's inlined params 2.0); w23d's six empty gotos remain the only EQUAL form (device; RESEARCH says not a
proposed fix). It also cannot land before func_80090308 (its internal callee with an IPA convention) matches.

## 7. camera_look_at_point (GC _PlayMoveSound) — 38/199 unchanged

Scored in the locked camera_aspect_ratio group.c copy (`us.sh camera_look_at_point v/X.c` with
`UARGS="--internal func_800C15FC"`): base 38/199 (ops 0, frame 48/48). GC locals (dPtr, one `ratio`, maxVel,
curVel): one variable for w11c's f/r is byte-identical (c4, 38); `abs()` is not an IDO builtin here (c1: calls, 145);
ternary abs without the `tt` device loses the degenerate `bgez` (c2, 197 words) and one `.5f` spelling merges retail's
two 0.5 constants (c2/c3). Residual unchanged: f0/f2 colour of the ratio copy + the t/FP temp ring one step behind
from +0x174/+0x208 (w11c). Stopped (no named lever left from GC).

## What generalises

- **GC `fmadds a,b,c` fixes the N64 add operand order**: write `c + a * b` with GC's multiply operand order; the
  constant-first product is a different uopt node, and the reload of `c` then comes second (entity_iterate 15 → 0).
- **GC chained stores mark N64 chained assignments**: two stores from one `li r0,0` (`stw gAltPaths; stw alt`) =
  `x.alt = g = 0;` — the chain makes uopt forward g's value into the next loop (audio_effect_apply 34 rows → 0).
- **GC statement order around inlined Random calls is the N64 order**: computing each Random inside its store
  statement (not into a/b/c temporaries first) took func_80090308 from 87 to 28 opcode rows in one step.
- **Typed GC ports close plateaued sweeps**: func_800B9740 had 1,252 variants at 83/102 (w14k/w14r); the direct
  port of CalcResPathBounds (no break, ternary per axis, indexed alternate ranges) was EQUAL on the first compile.
- Sibling GC bodies (ReadPathData / CalcResPathBounds / SyncCheckpointsToPaths) share one header layout: reuse the
  `ResPath`/`AltPath` types across the path family.

## Files

- `cloud/matches/entity_iterate.c`, `cloud/matches/func_800B9740.c` (standalone MATCH singles).
- `cloud/work/frontier/w33c/groups/frontier_read_path_data/` (group.json, physics_response.c, audio_effect_apply.c).
- `cloud/work/frontier/w33c/blast/best_effects.c` (func_80090308 draft 180/282 + entity_physics_update natural).
- Variants under `*/v/` (ignored).

Permission denials: none.
