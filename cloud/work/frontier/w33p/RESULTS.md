# w33p — scoring Astra's draft PR #379 (`gc_informed_n64_unscored_20261015`)

Worktree lane, branch `w33p` at origin/master `c22612328`. PR branch `pr379` head `b244417bb` (merge-base = c22612328;
27 files, all under `cloud/work/frontier/gc_informed_n64_unscored_20261015/`). The PR files were extracted with
`git archive pr379 <dir> | tar -x` into this worktree and left **uncommitted** (Astra's content; only this lane dir
is committed). build/ inputs copied from the main checkout (blob, ovl, us, m2c_asm, *.json, game_code.*,
blob_unit/cache), baserom symlinked. Builder scratch `watchman2:~/rush2049/scratch/frontier/w33p` (base copy + rsync,
`IDO_DIR` = toolkit 796ae99a IDO 5.3; unit remote dir `--remote-dir rush2049/scratch/frontier/w33p/unit`), deleted
at the end. Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3; group/fn scorer adds `-r4300_mul`).
Permission denials: none.

## Table

| Function | Bytes | File (PR path) | Historical (cited) | Score now: words / aligned rows / ops / frame | Verdict | Residual |
|---|---:|---|---|---|---|---|
| audio_mixer_main | 732 | routes/baseline_w6b.c | 31/183 (w6b) | **31/183** (unit) | reproduces w6b | `k + 1` CSE (w6b) |
| audio_mixer_main | 732 | routes/distance_boundary_control.c | — | 37/183 | worse | — |
| audio_mixer_main | 732 | routes/signed_above.c | — | 64/183, body 184 | worse | — |
| audio_mixer_main | 732 | routes/signed_below.c | — | 64/183, body 184 | worse | — |
| audio_mixer_main | 732 | routes/unsigned_above.c | — | **6/183**, frame 72 vs 80 | big step | frame only (8 B; a0 home + 3 `pos` reads shift) |
| audio_mixer_main | 732 | routes/unsigned_below.c | — | **6/183**, frame 72 vs 80 | same as above | frame only |
| audio_mixer_main | 732 | routes/negative_successor_plus2.c | (negative) | 8/183 | negative control behaves (not EQUAL) | — |
| audio_mixer_main + audio_priority_find | 732+432 | unsigned_above.c, helper KEPT (`negative_kept_helper`) | (negative) | 110/183 + 63/108 | negative control behaves | — |
| audio_priority_find | 432 | (all seven route files, internal) | provisional EQUAL (w2b) | **EQUAL 108 words** in every run | stays EQUAL | — |
| **audio_mixer_main + audio_priority_find** | **1,164** | w33p `groups/frontier_route_cross/group.c` = unsigned_above.c + 2 unused s32 locals | 31/183 | **unit EQUAL 2/2; `score.py group` MATCH/MATCH with and without `--claims`** | **MATCH (group)**, device disclosed | none |
| entity_iterate | 224 | entity/original-seed.c | never scored | 29/56, frame 80 vs 88 | complete draft | frame + homes |
| entity_iterate | 224 | entity/candidate.c | never scored | **15/56**; diagnose aligned 17, opcodes 4 (two of them are the jal relocation rows), frame 88/88; draft_check **EQUIVALENT** (28,011 calls / 6 scripts) | near-miss, logic right | post-call FP schedule + colour (as1 order: retail loads `ipos[2]` after the `ipos[0]` store) |
| save_write_data | 1,200 | blast/baseline_start_blast.c (+ effects.c) | 149/300, frame 248 vs 352 (w26c) | **149/300, rows 120, ops 30, frame 352/248** | reproduces w26c | frame (104 B) + colour |
| save_write_data | 1,200 | blast/start_blast.c (no `mat[12]`) | — | **149/300, rows 120, ops 30, frame 352/200** | no change in words; frame further from retail | frame now 152 B short |
| func_80090308 | 1,128 | blast/effects.c (internal) | 202/282 with w26c device source; 210 natural | **210/282 (+1 extra word), rows 198, ops 87, frame 272/160** | = w26c's "210 true" for the natural file | structure (ops 87) + frame + own .rodata unverified |
| entity_physics_update | 712 | blast/effects.c | 21/178 | **21/178, rows 23, ops 0, frame 112/112** | unchanged | colour only (a1 vs v0 web) |
| (blast group, packet `blast_group` spec) | 3,040 | effects.c + start_blast.c / baseline + 4 ctx copies | — | `score.py group`: 210/282, 21/178, 149/300 (both variants); context func_80090770 / func_8008B2E4 MATCH, func_8008E06C / func_80090284 MISMATCH (1–2 extra words beyond target, informational) | spec usable; same numbers as unit | as above |
| func_8038C910 (ovl_b, Astra) | 276 | c910/prior-candidate.c (= PR #364 candidate.c, byte-identical) | 13/69 | **13/69**; diagnose aligned 24, opcodes 10, frame 64/64 | reproduces #364 | outgoing-arg setup order +0x60..+0xA0 |
| func_8038C910 (ovl_b, Astra) | 276 | c910/candidate.c | — | **58/69**, 68 insns vs 69, frame 56 vs 64; draft_check `ovl_b:` **EQUIVALENT** (8 calls / 4 battle scripts) | regression in bytes, logic right | structure + frame |

## 1. audio_mixer_main + audio_priority_find — MATCH as a group (the result of this lane)

Unit command (Pi), each route file:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w33p --remote-dir rush2049/scratch/frontier/w33p/unit --jobs 2 \
  score audio_mixer_main audio_priority_find --internal audio_priority_find --keep audio_mixer_main --with FILE --neighbours
baseline_w6b:             FAIL audio_mixer_main: 31 of 183 words differ | EQUAL audio_priority_find: 108 words (internal)
distance_boundary_control FAIL 37 of 183 | signed_above/below FAIL 64 of 183; compiled body is 184 words
unsigned_above/below:     FAIL audio_mixer_main: 6 of 183 words differ | EQUAL audio_priority_find: 108 words (internal)
negative_successor_plus2: FAIL 8 of 183 | negative_kept_helper (--keep both): FAIL 110 of 183, FAIL audio_priority_find 63 of 108
every run: locked bodies that differ in this unit: 0
```
GC's unsigned `GetNextCheckpoint(unsigned int)` inlined through a static `FindPathDistances(void)` removes the w6b
`k + 1` residual entirely (that was the whole 31-word mechanism). What is left in unsigned_above is the frame only:
retail `addiu sp,-80`, `sw a0,80(sp)`, `lh 82(sp)` x3; ours 72 / 72 / 74. Retail's sp map: saves s0–s8/ra at 24..60,
`pos[3]` stored at 64/66/68, 8 B above `pos` (the slot the PR removed when it deleted w6b's `s32 pad[2]`).

Lever (disclosed, ~20 min): va/vb (helpers non-static + `--internal`) 6/183 unchanged; one unused `s32` frame 80 but
`pos` at 68 → 3/183; **two unused `s32` locals → EQUAL** (`audio_mixer_main/v/vd.c`, ignored dir; the delivered copy is
`groups/frontier_route_cross/group.c`); `pad[2]` restored (diagnostic only, not deliverable) also EQUAL. Frame arithmetic
for the owner's unused-locals rule: retail frame 80; without them 72; 2 slots (8 B); disclosed in the file header.
```
unit, group.c: EQUAL audio_mixer_main: 183 words (kept, c_group.c) | EQUAL audio_priority_find: 108 words (internal, c_group.c)
               locked bodies that differ in this unit: 0 | blob_unit score: 2/2 equal
builder (~/rush2049/scratch/frontier/w33p): python3 tools/cloud/score.py group cand/frontier_route_cross --claims
  audio_priority_find: MATCH | audio_mixer_main: MATCH | rc=0      (same without --claims: MATCH / MATCH, rc=0)
```
**Packaging notes (nothing installed):** `groups/frontier_route_cross/` (group.json claims + members both, keep
`audio_mixer_main`). New group; no locked group is superseded (audio_priority_find was only ever provisional:
`cloud/work/frontier/provisional.json`, evidence w2b `groups/frontier_route_cross`, never installed) — remove that
provisional entry on install. Unit default derivation without flags KEEPS audio_priority_find (it is not yet locked):
`group_unit_default.log` FAIL 107/183 + 63/108, so until the lock derives it, unit_overrides `force_internal` +=
`audio_priority_find` (the gc_lane_* helpers are static and were inlined, `umerge inlined into it:
gc_lane_FindPathDistances`). Names to settle before install: the helper names are the PR's private `gc_lane_*`
spellings; GC names are FindPathDistances (static) and GetNextCheckpoint — but the PR identifies locked
`func_800B930C` as N64's GetNextCheckpoint (a different `index+1 == count` body), so the inlined successor needs its own
name. Rename leads for a batch: audio_mixer_main = GC SyncCheckpointsToPaths, audio_priority_find =
FindAltPathIntersect. The two unused locals need the coordinator's yes under the standing rule. Knock-on: w17h said
audio_effect_apply (ReadPathData) is blocked by audio_mixer_main and func_800B9740 — one of the two is now closed.

## 2. entity_iterate — 15/56, EQUIVALENT, best.c saved

`entity_iterate/best.c` = PR `entity/candidate.c` unchanged (seed 29/56 frame 80 vs 88; the named `surfaceptr` local
fixes the frame and the homes). `python3 tools/workbench.py diagnose` (target assembled from `asm/us/blob/blob_800c46d0.s`):
`verdict=structure-mismatch aligned_total=17 words=15 opcodes=4 frame=-88/-88`, prefix-exact to row 34; everything
from `lwc1 temp[0]` after the normalise call differs: retail loads ipos[0], ipos[1], stores ipos[0], then loads
ipos[2]; ours loads all three ipos words before the first store, and the FP temps are coloured differently.
`python3 -m tools.recomp.draft_check --remote rocky entity_iterate candidate.c` → `VERDICT: EQUIVALENT over 28011 calls / 6 scripts`.
Lever variants (`entity_iterate/v/`, ignored; ~25 min): product-first `ipos[k] = t*0.25f + ipos[k]` (v1), `/ 4.0f`
(v6), `0.25f * t` (v7), all three adds on one line (v4), subtractions and adds on one line each (v5): all
byte-identical 15/56 (uopt canonicalises); loop forms v2 34/56 (63 words), v3 52/56. Next: as1 trace (`as1t`) of the
post-call block — the ready-list order of the three ipos loads vs the first store is the whole residual.

## 3–4. Blast trio (save_write_data / func_80090308 / entity_physics_update)

```
blob_unit ... score func_80090308 entity_physics_update save_write_data --with P/effects.c --with P/{baseline_start_blast,start_blast}.c --internal func_80090308 --neighbours
both: FAIL func_80090308: 210 of 282 words differ; +0x1a8: .rodata+0x17c: retail words encode 0x005439b8, outside the image (+4 more)
      FAIL entity_physics_update: 21 of 178 words differ
      FAIL save_write_data: 149 of 300 words differ; +0x1f8: .rodata+0x470: retail words encode 0x3bff8014, outside the image
      locked bodies that differ in this unit: 0
udiff (rows/ops/frame): baseline save_write_data 120/30 frame 352/248; no_array 120/30 frame 352/200;
      func_80090308 198/87 frame 272/160 (got 283 words); entity_physics_update 23/0 frame 112/112
```
Removing `mat[12]` changes nothing but the frame, which moves AWAY from retail (200 vs 352; the array was 48 of the
missing bytes). The PR's `blast_group` spec works as a `score.py group` dir (4 ctx copies hash-checked against the
packet's `blast_context`: all OK) and gives the same three numbers. Not worth a lever hour (no near-miss).

## 5. func_8038C910 (ovl_b, Astra-owned — scored only, nothing packaged)

`score.py fn cand/c910/X.c func_8038C910 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"` (overlay
byte scorer exists): prior-candidate `13/69 words differ`; candidate `58/69 words differ` (68 insns, frame 56 vs 64,
diagnose aligned 65, opcodes 48). `draft_check ovl_b:func_8038C910 candidate.c` → EQUIVALENT over 8 calls / 4 scripts.
The embedded `visuals[22]` form is behaviourally right but compiles to a different address computation
(`addiu v1,v0,752` base then small offsets in retail vs `lh 758(v1)` large offsets in ours) and loses a frame slot.

## PR hygiene and cited numbers

- Diff is 27 text files, 4,451 insertions, all inside the PR dir; no binaries (`git diff --numstat` has no `-` rows),
  no absolute/private paths, credentials, e-mail addresses, `.word`/glabel dumps or ROM payload; c910
  `native_reference.py` carries only addresses and fixture constants, `inputs.json`/`packet.json` only SHA-256 hashes.
  `materialize.py` writes only into a fresh `--out` dir. Mergeable as research. Remarks for Astra: the README's "Windows,
  no compiler" boundary is now stale for the scored items; packet `fresh_score: null` would need updating if merged
  after this.
- Cited historical numbers all check out against lane RESULTS: w6b 31/183 (reproduced today); w26c func_80090308 202/282
  with `best_effects_w26c.c` (natural file 210 = w26c's "210 true", reproduced), entity_physics_update 21/178
  (reproduced), save_write_data 149/300 frame 248 vs 352 (reproduced); w12a input_deadzone_apply 122/895; w14r
  func_800B9740 83/102; w17h audio_effect_apply no fresh score (physics_response provisional 920 B); w18d
  camera_play_script 793/880, 27 aligned rows, frame 608; camera_look_at_point 38/199 is w11c; #364 C910 13/69
  (reproduced). Population totals 12,996 B (game) / 8,392 B (ovl_b) add up.
