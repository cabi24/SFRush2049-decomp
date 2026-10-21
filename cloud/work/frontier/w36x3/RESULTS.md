# w36x3 RESULTS (wave 36, second-pass game near-misses)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36x3 (from base, master e68aea6e1).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_800E847C | update_game_data@mdrive.c (w36u confirmed) | 2100 | **MATCH (group, own .rodata verified) + unit EQUAL** | groups/w36x3_update_game_data/ |
| func_800E7FA0 | _UpdateTireAppearance@mdrive.c (w36u confirmed) | 1244 | **MATCH (group) + unit EQUAL** | groups/w36x3_update_game_data/ |
| particles_update | animCoins (+animCoinsText folded)@hud.c (w36u confirmed) | 1464 | **MATCH (group, own .data/.rodata verified) + unit EQUAL** (disclosed frame shaping) | groups/w36x3_coins/ |
| explosion_effect | animSpeedText@hud.c (w36o confirmed) | 1460 | 37 aligned rows (w36o 73-77), unit words 37 ops 8, frame 288/288, 366/365 words | explosion_effect/best.c |
| func_800F7F3C | FindWinner@gamerecords.c (w36o confirmed) | 1396 | **MATCH** (standalone `score.py fn`) + unit EQUAL | cloud/matches/func_800F7F3C.c |
| physics_float_calc | WorldCullObjects@world.c (w36u confirmed) | 1176 | 39 aligned rows (from 86), same size, frame 136/136 | physics_float_calc/best.c |

## Notes

### func_800E847C + func_800E7FA0 (update_game_data + _UpdateTireAppearance) -- MATCH, new group `w36x3_update_game_data`
- Scorer (builder, `score.py group cand/groups/w36x3_update_game_data`, also with `--claims`, exit 0):
  `func_800E847C:` / `  MATCH` / `    own .rodata verified at 0x801244B0..0x801244C0` / `func_800E7FA0:` / `  MATCH`.
- Unit: `blob_unit --tag w36x3 score func_800E847C func_800E7FA0 --with cloud/work/frontier/w36x3/groups/w36x3_update_game_data/update_game_data.c --internal func_800E7FA0 --neighbours`
  -> `EQUAL func_800E847C: 525 words (kept, c_update_game_data.c)` / `EQUAL func_800E7FA0: 311 words (internal, c_update_game_data.c)` /
  `locked bodies that differ in this unit: 0` / `blob_unit score: 2/2 equal`.
- Integration: new group `w36x3_update_game_data`: members = claims = [func_800E847C, func_800E7FA0], context [], keep
  [func_800E847C], file update_game_data.c. func_800E7FA0 must stay internal (unit_overrides force_internal / not on keep);
  provisional.json's func_800E7FA0 note (w2e) can be retired. Standalone `score.py fn` cannot match (private IPA callee).
- Base: w36u's best.c (caller complete). Callee fixes (35/37 rows -> 0):
  1. FP: retail keeps the skid intensity in $f0 for the store and the compare. The assignment-expression form
     `if ((m->skidIntensity[side] = ...) > thr)` puts the value in cfe's float temp, which is the SAME temp variable as the
     fabsf `?:` temps in the switch (one variable = one web; its web touches the fabsf intrinsic's varR $f0, so $f0 is
     forbidden -> $f18). A named `f32 intensity` gives its own web -> $f0 (37 -> 35 rows).
  2. INT colour (the "t3 holder" w36u could not find): traced with CDX_DETAIL_WEB interference lists and the p2 colour
     rule read from uopt.c (L4726d8: with the allcallersave flag, p2 takes the LOWEST available colour that is already
     USED in the procedure, else the lowest available). With the MAT3 web set, retail's forceLow=t4 / IV=t5 / ptr=s0 /
     ftnext=s1 / forceTable[4]=t3 / loop-1 IV=t3 is unreachable: forceTable[4] (bb41-42) shares no block with forceLow
     (bb48), the bracket pointer (bb43-48) or forceTable[index+1]. Retail needs forceTable[4]'s web to be live through the
     search loop: a VARIABLE `maxForce = skidData->forceTable[4]` used by the clamp and read again after forceLow
     (`if (maxForce) {}`, compiled out) -> t3, interfering with forceLow/IV/ptr/ftnext -> they shift to t4/t5/s0/s1 and the
     constants to s2-s5 (35 -> 7 rows).
  3. maxForce took t2 and index t3 (maxForce numbered first). `index = 0;` before `maxForce = ...` gives index the lower
     web number -> index t2, maxForce t3 (7 -> 0). (`index = 0;` before the `if (force >= scale * ...)` also gives 0.)
- Force oracle first: forcing the callee's retail colours (force.sh on the group snapshot) gave callee 0 rows and the
  caller 8 rows, all own-rodata relocations -- that proved the caller only needed the callee's s0-s5 clobber set.

### particles_update (animCoins + animCoinsText) -- MATCH, new group `w36x3_coins`
- Scorer (builder, `score.py group cand/groups/w36x3_coins`, also `--claims`, exit 0): `particles_update:` / `  MATCH` /
  `    own .data verified at 0x801161BC (8 bytes; .data is laid out per translation unit)` /
  `    own .rodata verified at 0x80123DD8..0x80123DE0`. (Context, informational: slot_state_setup MATCH,
  sound_update_channel `122/122 words differ` -- its t0 convention comes from callers outside this group; excluded
  from exit status, as in w36o_animgear.)
- Unit (group file with gJust turned extern = what the extern_data override does):
  `blob_unit --tag w36x3 score particles_update --with particles_update/coins_unit.c --internal _GetSemaphore_Coins
  --internal _ReleaseSemaphore_Coins --internal HAL_SetFont_Coins --internal TB_SetJust_Coins --internal
  HAL_SetTextDepth_Coins --neighbours` -> `EQUAL particles_update: 366 words (kept, c_coins_unit.c)` /
  `locked bodies that differ in this unit: 0` / `1/1 equal`. With the tentative definition the unit refuses
  (`D_80118E20: multiply defined`, gr3_d.c defines it) -- same as w36o_countdown.
- Integration: new group `w36x3_coins` (members = claims = [particles_update]; context slot_state_setup,
  sound_update_channel via byte-identical copies ctx_gr3_b.c/ctx_gr3.c of credits_scroll_grp gr3_b.c/gr3.c; keep in
  group.json). unit_overrides: force_internal += _GetSemaphore_Coins, _ReleaseSemaphore_Coins, HAL_SetFont_Coins,
  TB_SetJust_Coins, HAL_SetTextDepth_Coins; extern_data += {name D_80118E20, file src/blob/groups/w36x3_coins/coins.c}
  (same reason as w36o_countdown). Standalone `score.py fn` cannot match (slot_state_setup s2 convention).
- Path from w36u's lead (unit 262/366): `extern volatile u8 D_80140BDC` (gNumModels) -> 10 words; MAT3 operand order
  of x/y (`table + j * spacing`, `table + (n==1 ? 12 : 8)`) -> code identical; strings as data symbols
  D_80120E0C/D_80120E1C/D_80120E2C (precedent func_80108DA8.c's D_80120E34 = the next string): literals put own
  .rodata in two regions and the names[] initializer (.data) points into own .rodata, which neither the unit
  (`an address stored there has no image address`) nor blob_group (`jump table entry names .rodata`) can place --
  with the symbols both proofs pass and the audio_animText relocation-only neighbour difference disappears;
  static font/just helpers made 16 locked slot_* bodies differ in the unit, the w36o non-static shape fixes it.
- Frame (disclosed in the header): retail named area 128 (spill.sh AREA f_readnxtinst); natural body 104. +8 = MAT3
  animCoins DWARF locals `f32 xoffset, yoffset` (unused on N64) in the text block (function scope would move names[]
  from 232 to 224); +16 = one extra inlined wrapper level at two call sites (measured: any 2-site wrapper level =
  +16: HAL_SetTextDepth, HAL_SetFont or TB_SetJust; all give identical code). Written as HAL_SetTextDepth_Coins.

### func_800F7F3C (FindWinner) -- standalone MATCH
- `score.py fn cand/func_800F7F3C.c func_800F7F3C --flags "-g0 -O3 -mips2 -G 0 -non_shared"` -> `func_800F7F3C:` / `  MATCH`.
- Unit: `blob_unit --tag w36x3 score func_800F7F3C --with cloud/work/frontier/w36x3/FindWinner/func_800F7F3C.c --neighbours`
  -> `EQUAL func_800F7F3C: 349 words (kept, c_func_800F7F3C.c)` / `locked bodies that differ in this unit: 0` /
  `1/1 equal`. (A first attempt was refused while the integrator's working-tree unit_overrides.json listed D_80152818 twice.)
- Deliverable: cloud/matches/func_800F7F3C.c (standalone strict MATCH, no own data).
- Path (from w11d best, 137 aligned rows / 117 words): two mechanisms, both traced:
  1. Colour: retail's stunt-arm sort loop needs t4 forbidden for the end pointer (it gets t5, &gStuntPlayer s0, 120 s1)
     but retail shows no t4 there. A single compiled-out read (`if (a) {}`, `if (b) {}`, `if (key) {}`) colours a VISIBLE
     web into t4 (b1/b2 batches: 106-126 rows). Two holders of the same register-only expression (`if (ia + ib) {}` after
     ib and at the end of the loop body) form one PRE web whose every occurrence is compiled out: coloured t4, live across
     the body, invisible -> whole stunt sort part exact (b3: 114 rows, all later rows ring-phase only).
  2. Ring phase: everything after differed only in t6-t9 rotation. ugt.sh FREELIST trace: ours allocates t6 at the tie
     preheader where retail allocates t7 -> retail pops one more ring temp per sort loop. `a & 0xFF` on a swap store is a
     redundant mask: one ring pop, no instruction (L65 phantom pop). One per sort loop (4) -> 0 rows (b6: every combination
     of `a & 0xFF`/`b & 0xFF` per loop matches; holders still required).

### physics_float_calc (WorldCullObjects) -- 39 rows (w36u 86), not matched
- `full.py physics_float_calc/best.c physics_float_calc -O3` (one-file compile) -> `want 294 words, got 294; differing rows 39;
  unverified 2`.
- Found: (1) w36u's gTrackNum was the wrong address: retail reads `lui a1,0x8015; lb a1,-26740(a1)` = D_8014978C
  (w36y: gTrackNum = D_8014978C, proven); (2) MAT3's `ptr[w]` form is retail's (t2 ptr through the switch, t0 = copy,
  dead w*4 and i*68 IVs) once ptr is eligible for colouring: ptr's save is 0 (19 case defs, one preheader use) so the L55
  gate strikes it; two compiled-out `if (ptr) {}` reads in the loop (top and end) make it a register web: 252 -> 40 rows;
  (3) `w` declared between mask and ptr puts ptr's home at 100 (retail); (4) the dead debug call can be guarded by
  `ptr_no_clip` (0 on N64) instead of a `dbg` local.
- Residual (39 rows): the inline area is 8 B larger than retail (dv homes 52/56/60 vs 60/64/68; area.sh: each inlined
  pointer parameter costs 8 B of named area, `d` 4 B); the `(ptr_no_clip ? ... : 0)` cfe temp (save 15) and the dead `w`
  web are coloured v0 before mask, so mask/i are v1/a1 (retail v0/v1); the && spelling removes the temp (mask/i right)
  but reorders the preheader IV inserts. Batches c1-c9, d1 (~380 variants) in the lane dir.

### explosion_effect (animSpeedText) -- 37 rows (w36o 73-77), not matched
- Group (gr3 context, tools/rscore.py / tools/gdiff_ee.sh): 37 aligned rows, 366/365 words. Unit (best.c with gJust extern =
  explosion_effect/u27.c, helpers --internal): `FAIL explosion_effect: 282 of 365 words differ; compiled body is 366 words,
  target 365 ... | words 37 ops 8 norm 8 | frame 288/288`.
- Lever: `if (y) {}` at the top of the slot loop (batch h1, 81 holder variants: expressions slot/x/y/digitW/... at 5 places,
  singles and pairs) -> slot a1, slot*8 a0, model[slot] IV fully in memory (80(sp)) = retail; w36o's split-oracle finding
  (slot's piece running into the HAL_SetFont arms) is fixed by it.
- Residual: &gGameCar's split piece (decided 2x) spans the call-argument blocks: forbidden v0..a3, so t0 (retail v0);
  constants 1/10 t1/a3 (retail a3/a2); as1 order of the arm arguments; +1 word. force.sh declines all three retail colours
  (interference) -> split extent, not a tie. Second-holder batch h2 (62 variants) found nothing below 37.

## Identities proven
- gTrackNum is D_8014978C (w36y proven; retail physics_float_calc `lb a1,-26740(a1)`): w36u's physics_float_calc draft used
  D_80149B8C, a wrong address (not a rename -- the load would read another byte).
- particles_update's translation unit on the N64 is the NumCoinsFound one: func_800B78A4 = CountBits (gamerecords.c),
  func_800B78F0 = NumCoinsFound (gamerecords.c), then particles_update = animCoins.
- D_80120E0C / D_80120E1C / D_80120E2C = "GOLDCOING_COIN" / "SILVERCOINS_COI" / "%d/8" (animCoins/animCoinsText strings);
  0x801161BC = animCoins' playerCoinName initializer.
- func_800F7F3C = FindWinner@gamerecords.c (w36o identity) confirmed by MATCH.
- (confirmed by MATCH) func_800E847C = update_game_data@mdrive.c, func_800E7FA0 = _UpdateTireAppearance@mdrive.c (w36u's identities).

## What generalises
- **An invisible colour holder = one expression read only by two compiled-out `if`s.** When retail's colouring needs a
  register forbidden by a web that never appears in the code, `if (x + y) {}` at the start and end of the region gives a
  PRE web with no surviving occurrence that still takes a colour (FindWinner). A single holder colours a visible web.
- **A residual that is only t6-t9 rotation after an exact region = a missing ring pop.** Trace with `SCHED=1 ugt.sh` and
  find the first ALLOC that differs; a redundant `& 0xFF` on a byte store (phantom pop) or a missing one moves the phase.
- **Strings referenced from an initialised local array: use the retail data symbols.** A string literal in
  `char *names[] = {"..."}` makes the .data initializer point at own .rodata; the unit and blob_group cannot place
  such a pointer (and IDO's string .rodata and float literals sit in two retail regions). `extern char D_8012xxxx[]`
  (precedent func_80108DA8.c) gives MATCH with own .data/.rodata verified.
- **Any two-site inlined wrapper level costs +16 named area** (8 per site); the bytes cannot tell which helper
  carries it. Measure with spill.sh's `AREA f_readnxtinst` in a one-file group (`snap.sh L FILE KEEP` + `spill.sh`):
  the group-mode named area equals the unit's.
- **p2 colour selection prefers USED colours** (uopt.c L4726d8, allcallersave procedures such as private IPA callees): a p2
  web takes the lowest available colour that some earlier web already holds, else the lowest available. So "retail web
  got t4 although t3 looks free" means either an interfering t3 web or a used t4 web; enumerate orders with the
  interference lists (`tk.sh ... cdx NAME CDX_DETAIL_WEB=<web>` prints `intf ... other=` neighbours) before writing variants.
- **A value live longer than its uses = a named variable read again by compiled-out code.** When retail's colouring needs a
  web to interfere with webs it never meets, make the value a named local and add a code-free read (`if (v) {}`) where the
  live range must end (here after the bracket search). Position matters: the read before forceLow had no effect.
- **cfe reuses one float temp for every `?:`/assignment-expression value in a function**: an assignment-in-condition
  shares the fabsf `?:` temps' web and inherits their $f0 conflict; a named local separates it.

## Permission denials
none
