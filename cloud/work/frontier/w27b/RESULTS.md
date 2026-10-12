# w27b — game race/world/attract pool (wave 27)

Branch wave27 @ 8a54d6d2. Builder scratch watchman2:~/rush2049/scratch/frontier/w27b. Status: finished (see table; untouched pool members at the end).

Prior work found before starting: near_miss_B73 (stat_race_start drafts), near_miss_B136 (attract_or_transition
drafts), near_miss_B93 (func_800E543C), near_miss_B47/B109 (lap_complete notes). Everything else in the pool: no
prior draft.

| function | bytes | state | best score |
|---|---|---|---|
| world_trigger_check | 216 | **group MATCH** (groups/credits_scroll_grp) | unit EQUAL; `score.py group` MATCH |
| slot_state_setup (not in pool; sole blocker of world_trigger_check + 7 others) | 232 | **group MATCH** (groups/credits_scroll_grp) | unit EQUAL; `score.py group` MATCH |
| world_object_destroy | 636 | **MATCH** (cloud/matches/world_object_destroy.c) | `score.py fn` MATCH (own .rodata verified); unit EQUAL, 0 locked differ; draft_check EQUIVALENT 45 calls (attract) |
| func_800EB90C | 2,000 | **MATCH** (cloud/matches/func_800EB90C.c) — 6 disclosed unused locals + volatile, coordinator to review | `score.py fn` MATCH (own .rodata verified); unit EQUAL, 0 locked differ |
| func_800EB028 | 1,640 | draft, register/schedule; draft_check EQUIVALENT 33,257 calls / 6 scripts | unit 163/410 (rows 175/21/21) |
| countdown_handler | 496 | draft, shaping (address CSE, loop shape, frame -8) | unit --internal 103/124 (rows 80/15/15) |
| lap_complete | 1,232 | draft, shaping (y narrowing, address CSE, frame -8) | unit 288/308 (rows 111/34/34) |
| game_late_init | 820 | draft, conventions/frame unknown; draft_check DIVERGES at call #1 | unit 184/205 (rows 69/30/30) |
| world_velocity_integrate | 712 | draft (callee convention) | unit 141/178 (words 78 ops 44 norm 44) |
| stat_race_start | 1,068 | **MATCH** (cloud/matches/stat_race_start.c) | `score.py fn` MATCH; unit EQUAL, 0 locked differ |

## stat_race_start — MATCH (-O3, no devices)
- `score.py fn cloud/matches/stat_race_start.c stat_race_start --flags "-g0 -O3 -mips2 -G 0 -non_shared"` → `MATCH`
- `blob_unit --tag w27b score stat_race_start --with cloud/matches/stat_race_start.c --neighbours` →
  `EQUAL stat_race_start: 267 words (kept, c_stat_race_start.c)` / `locked bodies that differ in this unit: 0`
- From near_miss_B73 (130/267, all sp offsets + one schedule). Two levers: (1) declaration order — retail's 8-byte gap
  between values[] (108) and ranks[] (76) is total_weights/total_values declared between them, and `i, j` before
  `session` puts session's spill home at 64 (130 → 3 words); (2) `for (total_weights = 0, i = 0; …)` — the separate
  `total_weights = 0;` statement put `move a0,zero` ahead of the strength-reduced pointer setup (3 → 0).
- Integration: plain single (splice_singles). Does not need -Wab,-r4300_mul.

## world_trigger_check + slot_state_setup — group MATCH (supersedes credits_scroll_grp)
- world_trigger_check (0x800EDCE8): osRecvMesg; func_800B6748(...) (resets the 7 slot/colour outputs, t0–t2 IPA args);
  slot_state_setup(D_80149DA0) (s2 IPA arg); `D_80149B88 = 0; if (D_8002AFC4 > 220) D_80149B88 |= 0x8000;`;
  clear D_8011474C/D_8011473C; osSendMesg. Only lever: `|= 0x8000` (plain `= 0x8000` gives li t7 for t8, 2 words).
  The draft in codex_defaults_a16's context (`= 0x8000`) is the 2-word form.
- slot_state_setup (0x800B4200, not in my pool, picked up because it is the sole unmatched blocker of
  world_trigger_check, checkpoint_hit, dust_cloud_effect, func_800D9058, func_80107EDC, func_80109F54, object_create,
  skid_mark_render): the locked context copy in credits_scroll_grp/gr3_b.c was 16 words off in the unit, colour-only
  (`force.sh base slot_state_setup p1:w31=c15,p1:w33=c14,p1:w34=c14` → 0 rows). w31 (&D_80149DA0, save 1.67) beat
  w33/w34 (&D_80149780/&D_801497A4, 1.5) to s0. **Device (disclosed):** a compiled-out `do {} while (0);` between the
  two lookup blocks adds a live block to w31 only and flips the tie (`if (selection) {}` there, or `do {} while (0);`
  right after `D_80149DA0 = selection;`, also work; the explicit-assignment rewrite and a block before
  sound_update_channel do not).
- Unit: `blob_unit --tag w27b score slot_state_setup world_trigger_check --with w27b/slot_state_setup/slot_state_setup.c
  --with w27b/world_trigger_check/world_trigger_check.c --neighbours` →
  `EQUAL slot_state_setup: 58 words (internal, c_slot_state_setup.c)` / `EQUAL world_trigger_check: 54 words (kept,
  c_world_trigger_check.c)` / `locked bodies that differ in this unit: 0`.
- `score.py fn` cannot score either (register-parameter callee/callee-saved conventions; world_trigger_check alone is 49/54).
- Group: `groups/credits_scroll_grp/` (SAME name as the locked group; members credits_scroll + slot_state_setup +
  world_trigger_check; claims all three). `score.py group cand/credits_scroll_grp` (with and without `--claims`):
  `credits_scroll: MATCH`, `slot_state_setup: MATCH`, `world_trigger_check: MATCH`. Changes vs the locked group:
  gr3_b.c slot_state_setup gets the `do {} while (0);`; new gr3_w.c = world_trigger_check + byte-identical context copy
  of func_800B6748 (locked member of codex_defaults_a16) + a kept stand-in caller `__standin_func_800B6748` (a lone
  internal caller lets umerge inline it); gr3.c's context copy of object_byte9_set gets blob_unit's 8-store dead
  `if (0)` inline-blocker block (it is an `inline_blockers` entry in the unit; without it umerge inlined it into
  slot_state_setup). slot_state_setup moved from context to members; func_800B6748 + the stand-in added to
  context/keep. No unit_overrides change needed (slot_state_setup is already internal in the unit).
- After landing, codex_defaults_a16's context copy of world_trigger_check (`= 0x8000`) is stale but harmless (context).

## world_object_destroy — MATCH (-O3), no devices
- `score.py fn cloud/matches/world_object_destroy.c world_object_destroy` → `MATCH` / `own .rodata verified at 0x80124558..0x8012455C`
- `blob_unit --tag w27b score world_object_destroy --with cloud/matches/world_object_destroy.c --neighbours` →
  `EQUAL world_object_destroy: 159 words (kept, c_world_object_destroy.c)` / `locked bodies that differ in this unit: 0`
- draft_check: `VERDICT: EQUIVALENT over 45 calls / 1 scripts (attract 45); never called in: attract_pak`
- Respawn-point picker (see header). No prior draft. Path to match: frame 144 = three inlined statics (dist2d(x, z),
  and the locked kept rand/Random = func_8008B2B4/func_8008B2E4 pasted verbatim — func_800B23E0's precedent; a
  custom `frand` left rand's result in v0, 3 words); `(s16)(speed >> 6) + 4` (as1 narrowing deletion pops t6/t7);
  the point walk on one source line (as1 line tie-break, 2 words); `if (dist > 200.0f) continue;` numbers the 200
  web before -999 (FP f14/f16 swap, 5 words); `Respawn *e` declared before the arrays (frame offsets).
- Integration: single, but the helpers must not leave deleted-procedure stubs: with `static` helpers the unit
  says `stub tail: 6 words of deleted-procedure stubs follow (NOT as in the image)`. Delivered with non-static
  wod_rand / wod_random / wod_dist2d: add unit_overrides `force_internal` for those three names. Verified:
  `score.py fn` → `MATCH` (own .rodata verified); `blob_unit … --internal wod_rand --internal wod_random --internal
  wod_dist2d --neighbours` → `EQUAL world_object_destroy: 159 words` / `locked bodies that differ in this unit: 0`.

## world_velocity_integrate — draft, callee-convention residual
- best.c: unit `FAIL world_velocity_integrate: 141 of 178 words differ` (rows words 78 ops 44 norm 44), frame 96/96.
  Started from codex_defaults_a16's context draft (unit 161/178, rows 139/90): pos++ hoisted before the switch,
  `slot_state_setup(*p++)` forms, case 6 passes `&D_80149DD8[pos]` (retail s2) not p.
- Residual class: callee convention. audio_doppler_calc (unmatched, blocked by Astra's func_80087110) is absent
  from the unit; retail passes it s2 = &D_80149DD8[pos], s0 = len (IPA) and it clobbers s1–s8, hence retail's
  pos/len spills (92/88) and the per-call rematerialisation of D_80149DD0/&D_80149DD8 in a1/t0. Not finishable
  before audio_doppler_calc.

## func_800EB90C — MATCH with disclosed devices (arcade camera.c CheckCameraView)
- `score.py fn cloud/matches/func_800EB90C.c func_800EB90C` → `MATCH` / `own .rodata verified at 0x8012455C..0x80124570`
- `blob_unit --tag w27b score func_800EB90C --with w27b/func_800EB90C/best.c --neighbours` →
  `EQUAL func_800EB90C: 500 words (kept, c_best.c)` / `locked bodies that differ in this unit: 0`
- **Devices (owner rules allow; coordinator please review before CI):** (1) six unused locals named after the arcade
  CheckCameraView locals (new_view3, block, delta, extra_lag, edge, view3_lim): exact frame residual — retail frame
  200, without them 176 and 5 words off (`v/nounused.c`: `FAIL func_800EB90C: 5 of 500 words differ … frame 200/176`),
  5 of them gives 192; (2) `extern volatile s16 D_801543CA` (retail reloads the car count through a hoisted address
  every bump-loop iteration; non-volatile = 459 words).
- Natural levers found: index-form bump loop (strength-reduced as retail), arcade Random/rand pasted from the locked
  kept func_8008B2E4/func_8008B2B4, inlined `node_camera(s16)` (retail's a0 narrowing), 7 used locals declared before
  i (home 168), `= 0` vs `0.0f` zero webs, `pl++` in the for header and the two suspension resets on one line (as1
  line tie-breaks), `Random(1.0f) + 2.5` double add, `switch ((u32)Random(11.0f))`.
- If the coordinator rejects the unused locals: best form without them is `v/nounused.c` (5 words: i home slot).

## func_800EB028 — draft (arcade camera.c setcamview's view switch)
- best.c: unit `FAIL func_800EB028: 163 of 410 words differ` (rows words 175 ops 21 norm 21), frame 96/96;
  standalone score similar. draft_check: `VERDICT: EQUIVALENT over 33257 calls / 6 scripts (stunt_end 9189,
  attract_pak 8298, attract 7688, battle 4026, backward 2028, ghost_race 2028)` — logic right, compiler shaping left.
- Residual class: register/schedule (see header): retail keeps pos_out[1] in $f0 in cases 0/1, FP ring one pair
  ahead in case 1, case 2/3/8/10 copy loads-first. Levers that worked: Model layout (body_force 328, center_force 352),
  `for (j = 0; j != 4; j += 2)` (x/z only; retail's bne end test), i/j declared first, math_utility after the
  pos_in block in case 1.

## draft_check verdicts (harness gaps)
- world_object_destroy: EQUIVALENT 45 calls (attract). func_800EB028: EQUIVALENT 33,257 calls / 6 scripts.
- slot_state_setup (`--params selection=s2`): `DIVERGES (6 of 6 scripts)`, first at frame 3 call #8, effect
  D_80149B60/B70 — harness gap: its callee sound_update_channel takes `force` in t0 (IPA) and is not in
  tools/recomp ipa_callees.json, so the standalone draft passes it in a0. The body is byte-EQUAL in the unit.
- world_trigger_check, world_velocity_integrate: `ERROR` (replaced host exits -11) — same gap: func_800B6748 takes
  t0–t2 and slot_state_setup s2 (IPA), not in ipa_callees.json. Both callers' bodies are unit-EQUAL / draft.

## countdown_handler — draft (internal; unit --internal)
- best.c: `FAIL countdown_handler: 103 of 124 words differ` (rows words 80 ops 15 norm 15), frame 344/336.
- Found: it uses credits_scroll_grp gr3_c.c's inlined `font_set` (lock + slot_state_setup + unlock) — the same
  static inline explains lap_complete's/race_finish's duplicated lock sequences; one variable carries count,
  font_set result and timer (retail s0); text[256] declared before num[8].
- Left: &D_801146F8 CSE'd into s1 (retail rematerialises), ready-scan loop branch shape (bnezl/b vs beqzl; for/while/
  goto/else-break forms all compile the same), frame 8 short. draft_check not run (calls slot_state_setup via s2: harness gap).

## lap_complete — draft
- best.c: unit `FAIL lap_complete: 288 of 308 words differ` (rows words 111 ops 34 norm 34), frame 184/176.
- Hand-formatted race clock drawn with a shadow pass (header). Same font_set static; func_800B669C (locked Pair
  setter) inlined. Left: y is loaded lw + sll/sra in retail (ours lh), &D_80151AD0 in s5 (retail rematerialises),
  frame 8 short. race_finish (same layout, stub neighbour func_80107AEC) not drafted.

## game_late_init — draft
- best.c: unit `FAIL game_late_init: 184 of 205 words differ` (rows words 69 ops 30 norm 30), frame 128/56.
- draft_check: `VERDICT: DIVERGES (6 of 6 scripts)`, first at frame 0 call #1 (effect) — expected: the audio_reverb_update
  call convention (retail passes the pointer in a1, 0 in a2 and leaves a0 unset) and func_800DE45C's argument are
  unknown. Buffer symbols are D_800586D0 / D_80069090 (not 0x80058AD0). Saves unused s0/s1 → an unmatched internal
  callee (func_800DE45C, w27d) clobbers them; 72 bytes of frame unexplained.

## Not reached / not drafted (notes for the next wave)
- race_finish (1,000): same layout as lap_complete (font_set static, hand-formatted digits); start from lap_complete/best.c.
- RaceStateMachine_Update (2,148): 7 unmatched callees incl. Astra's func_800D91A0 and the IPA unit partner
  func_800D6160 — provisional at best; skipped.
- attract_or_transition (2,076): unblocked now (wheel_render_full landed by w27a, world_trigger_check here); prior
  drafts cloud/work/near_miss_B136 score 492/519 in the unit (gfx/dispatch forms); not reworked.
- attract_mode/demo/video_handler (IPA register-parameter unit, no script reaches them): not started.
- func_800E543C (700): near_miss_B93 drafts are 162/175 in the unit (rows 223/30/30), frame 72/56. Untested lever:
  the caller-less `jr ra; nop` stub func_800E5434 right before it is a deleted inlined static (frame +16 = its
  inline area); `action` is re-read from its stack home (lh 74(sp)) three times, so it is probably passed into
  that helper. func_800E56F8 / func_800E6AF8 (IPA s4 unit): not started.
