# w33b — screen pairs + options (wave 33), IDO 5.3 -O3

Status: FINAL (wave 33). Branch `wave33` at 58425072b (src/blob + lock synced to the builder at start). Unit tag `w33b`;
builder scratch `watchman2:~/rush2049/scratch/frontier/w33b` (deleted at the end). func_8010E8B4 not worked (stop rule).

**New coverage ready to install: 184 B** (func_800DA0BC, superseding packet of frontier_optionscr_buttons).

| function | bytes | start | end | residual |
|---|---:|---|---|---|
| func_800DA0BC (GC uninitScreen@optionscr.c) | 184 | provisional (stand-in caller, .loc device) | **MATCH in group** (superseding `frontier_optionscr_buttons`, no device) | none |
| controls_animText (func_80100564) | 1,576 | 253 rows | **155 rows**, frame 360 exact, EQUIVALENT 1,949 calls | p*2 web takes s8 over const 10; gNumWindows load PRE'd onto continue edges |
| controls_animButtonText (0x800FFDE8) | 1,916 | 234 rows, frame 520/544 | **227 rows**, frame 544 exact | end SetJust stores duplicated per `goto end` edge (retail: shared end, &D_80118E20 in t0) |
| func_8010F218 (video_animText) | 2,444 | 529 rows, 536/611 w | **324 rows**, 518/611 w, frame 384/456 | uopt does not fully unroll the 3-block loop |
| func_8010EA08 (video_animButtonText) | 2,056 | 287 rows | 287 (not reworked) | FlashTextColors argument copies (w32b) |

## 1. func_800DA0BC — MATCH (superseding packet of frontier_optionscr_buttons)

GC counterpart uninitScreen@optionscr.c, with GC uninitButtons@optionscr.c (32-entry buttonInfo loop:
`if (buttonInfo[i].objnum != -1) { ZOID_RemoveObject(objnum); objnum = -1; } buttonsInited = 0;`) written in place.
N64 mapping: gGameState & 0x7C03FFFE = state_word_a; RemovePanes(0) = sound_handles_clear(0); gTitlePane =
D_80116DA4; blit = D_80116D98 / RemoveBlit = sound_stop; removeBG = ambient_sounds_clear; buttonInfo = D_8011650C
(the group's `Button[]`, objnum = `.handle` at +4); ZOID_RemoveObject = entity_spawn_callback(s16,0,0);
buttonsInited = D_80116D0C; inited = D_80116D94 (s8).
- With the real (now locked) caller, w14ob's body is EQUAL in the unit, but it still needs its one-line
  `slot = &D_8011650C; do {` .loc device (two-line form: 2 words swapped, `v/a1.c`). The GC form — an indexed
  `for (i = 0; i < 32; i++)` over `D_8011650C[i].handle` — is EQUAL with NO device, either written in place (`v/b2.c`,
  chosen) or as an inlined non-static helper `uninitButtons_da` (`v/b1.c`, also EQUAL).
- Unit (Pi): `blob_unit --tag w33b score func_800DA0BC func_800DB1E0 func_800DA2C0 --with
  cloud/work/frontier/w33b/groups/frontier_optionscr_buttons/buttons.c --internal func_800DA0BC --internal func_800DA2C0
  --internal ZOID_SetColor_da --internal ZOID_SetColorP_da --neighbours`
  ```
  EQUAL func_800DA0BC: 46 words (internal, c_buttons.c)
  EQUAL func_800DB1E0: 350 words (kept, c_buttons.c)
  EQUAL func_800DA2C0: 579 words (internal, c_buttons.c); own zero-initialised data (addresses consistent, inside game .bss, no overlaps; nothing to compare)
  locked bodies that differ in this unit: 0
  blob_unit score: 3/3 equal
  ```
- Builder `score.py group cand/frontier_optionscr_buttons` (and `--claims`; `groups/score_group.log`):
  `func_800DB1E0: MATCH` (`own .rodata verified at 0x80124248..0x80124280`), `func_800DA0BC: MATCH`,
  `func_800DA2C0: NOT VERIFIED (32 section-relative relocations unverified ...)` + own .bss 0x8015273C/0x80152740 —
  identical to the locked group's known two-region line (w32b §2), rc=1 only for it.
- Splice-proof replay (`tools/gproof.py`, read-only copy of w32b's; `groups/proof_buttons.log`): `group_bodies OK:
  func_800DA0BC, func_800DA2C0, func_800DB1E0`; 46 / 579 / 350 words, `0 differ from the image`; `3/3 members
  image-identical`.
- **Integration**: SUPERSEDES locked group `frontier_optionscr_buttons` (same name, all old members kept, the two old
  members' code byte-identical). Packet `cloud/work/frontier/w33b/groups/frontier_optionscr_buttons/` (buttons.c = old
  file + 34 lines: declarations and the func_800DA0BC body inserted before ZOID_SetColorP_da; header paragraph).
  members/claims func_800DB1E0, func_800DA2C0, func_800DA0BC; keep unchanged (func_800DB1E0). unit_overrides
  `force_internal` += func_800DA0BC (reason: group member, sole caller func_800DB1E0). Retire the provisional.json
  entry for func_800DA0BC (w14ob). No data-symbol changes, no reverts (func_800DA0BC was never locked; its locked
  stub neighbour func_800DA0B4 untouched). Shaping devices: none new.

## 2. Controls pair — improved, not closed (`controls/best.c`; uargs `controls/uargs.txt`)

Unit (Pi, `us.sh`): `FAIL controls_animText: 363 of 394 words differ; compiled body is 407 words, target 394 | words 155
ops 47 norm 47 | frame 360/360`; `FAIL controls_animButtonText: 428 of 479 words differ; compiled body is 489 words,
target 479 | words 227 ops 63 norm 63 | frame 544/544`. draft_check (`controls/dc.log`): `VERDICT: EQUIVALENT over 1949
calls / 2 scripts (controls_frontend 1110, pause_menus 839)`.
- **Traced cause of w32b's "p coloured s8 / p+1 PRE"** (pretrace bit 55 `add.J(p,1)`: antloc 17 (sprintf), 70 (tail);
  DELETE 70, INSERT 5,11,14,77): the sprintf's `p + 1` and the loop increment are one expression, so PRE moves the
  increment onto every `continue` edge (a spill temp at 144) and p's web ends at the sprintf; p (save 12.83) then
  takes s8 and the constant 10 (10.53) is split. Diagnostic: with the sprintf argument replaced by a constant, p
  stays in memory at 352 as retail (175 rows). Spellings uopt canonicalises to the same expression (no change, 253):
  `1 + p`, `p - -1`, `(p + 2) - 1`, `p + (s8)1`, `(s32)(p + 1U)`, `p + 1 + 0`, `(p | 0) + 1`, a `ct_next(n)` helper
  (copy-propagated). Spellings that break it: `(p * 1) + 1`, `(p << 0) + 1` (no-code identities, 171 rows), `-~p`
  (174; different code). Chosen: `(p * 1) + 1` (DISCLOSED device; no natural form found).
- **Frame**: with the PRE temp gone the frame fell to 352; GC's TB_SetJust (an inlined non-static `ct_SetJust(h, v)`)
  at animText's two sites gives 360 exact and replaces w32b's void ct_SetFont wrapper (155 vs 156 rows with both).
  animButtonText: SetJust helper at 3 of its 5 sites (first, second, end; other 3-site sets tie at 227-229) -> 544
  exact (520 before); 5 sites -> 560.
- Residual animText (ctrace `g1`): the hoisted `p*2` web (mpy, bits 113; save 120/11 = 10.91) takes s8 over the
  constant 10 (200/19 = 10.53, tied with ldaS 570 and 772); retail spills p*2 (136) and colours 10. Operand-order
  variants (h1-h5) do not move it. Continue edges also get the gNumWindows load PRE-inserted (bit 3: INSERT
  5,11,14,27,30,65,68); retail loads it in the tail only. Force oracle `p1:w113=s,p1:w267=c22` was declined (s8 taken).
  Next lever: lower p*2's weight (one fewer outer-level use) or find what kills gNumWindows on retail's continue paths.
- Residual animButtonText: the end label's SetJust(0, 3) stores are duplicated onto each `goto end` edge; retail
  keeps one shared end whose base &D_80118E20 sits in t0 (set on each incoming edge), also used by the first site.
- Variants: `controls/v/` (a*: holder, b*/e*: p+1 spellings, c*: helpers, f*/g*: SetJust sites, h*: operand order,
  k*/m*: animButtonText SetJust/SetFont site sets).

## 3. Video-options pair — F218 improved (`videoscr/best.c`; uargs `videoscr/uargs.txt`)

Unit: `FAIL func_8010F218: 423 of 611 words differ; compiled body is 518 words, target 611 | words 324 ops 113 norm 113
| frame 456/384` (w32b: 529 rows, 536 words); `FAIL func_8010EA08: 484 of 514 words differ; compiled body is 511 words,
target 514 | words 287 ops 86 norm 86 | frame 312/312` (unchanged).
- w32b's diagnosis "uopt does not unroll the 32-bar loop" was wrong in the current unit: `for (c = 0; c < 256; c += 8)`
  IS unrolled by 4, but with one colour IV and `c+8/c+16/c+24` offsets and a `!= 256` test. Retail's four colour IVs
  (s0/s1/s2/t4, stride 32, exit `t4 != 280`) come from a counter loop whose colour is a multiplication:
  `for (c = 0; c < 32; c++) PrimColor(c * 8, ...)` (strength reduction after unrolling gives one IV per copy).
  Counter loops whose variable feeds coordinates or a named `c = i * 8` are NOT unrolled (e7/e8/b1), nor loops on x.
- The 3-block section: GC drawColorBars writes white/black pairs between the colour blocks (videoscr.c, 11
  DC_SendFillRect calls); retail is a fully unrolled 3-trip loop (constant colours, x chain s0 -> t4 = s0+32 -> s0 =
  t4+48). A branch-free body (`gDPWords(..., 0xFA000000, (0xFF000000 >> (i * 8)) | 0xFF)`, x += 48 / x += 32) gives
  retail's per-copy instruction pattern, but uopt keeps the loop. Diagnostics (`v/z*`, not candidates): it is NOT body
  size — a 2-command body with trip count 3 stays a loop (z2), the same body with trip count 4 is fully unrolled
  (z2t4, z2m); the 6-command body stays a loop at 3, 4 and 8 (z4, z8: with the shift colour). So this uopt fully
  unrolls a 4-trip loop but not a 3-trip one, while retail has exactly three straight copies with a runtime x chain.
  Ternary colours (branches) are worse (g1t). Next lever: a source shape whose known trip count uopt treats as
  unroll-by-4 remainder copies (e.g. a 4-trip loop whose 4th copy is dead/folded away), or straight-line copies with
  an x that is not constant-propagated.
- A GC-style inlined `DC_SendFillRect(x0, y0, x1, y1, r, g, b, a)` helper is NOT inlined by umerge (8 args; b2/b3).
- draft_check (`videoscr/dc.log`, `--unit func_8010F218,func_8010EA08 --scripts options_menus,pause_menus`): `VERDICT:
  EQUIVALENT over 569 calls / 2 scripts (options_menus 330, pause_menus 239)`; `unit members (outermost calls compared):
  func_8010F218 569, func_8010EA08 0` (EA08 only reached inside F218, as in wave 32).
- Variants `videoscr/v/` (b*: GC helper / counter loops, d*: branch-free 3-loop in the old shape, e*/f*: 32-bar loop
  spellings — f3 is the closing form, g*: 3-loop bodies, t1/z*: diagnostics only).

## What generalises
- **A provisional body whose real caller is now locked can need no device at all**: rewritten in the GC form
  (indexed `for` over the struct array instead of w14ob's pointer `do`-loop) func_800DA0BC is EQUAL in the unit and in
  the group without the one-line `.loc` device.
- **An expression shared by a call argument and a loop increment (`f(p + 1)` ... `p++`) is PRE'd onto every
  `continue` edge**; that shortens the counter's web and can promote it into a callee-saved register retail leaves in
  memory. uopt canonicalises `1 + p`, `p - -1`, `p + 1U`, `(p | 0) + 1` and inlined-helper copies to the same
  expression; only a no-code identity on the variable (`p * 1`, `p << 0`) separates them. Check pretrace for
  INSERT on continue blocks when retail recomputes `x + 1` at a shared tail.
- **A removed spill temp frees 8 B of frame** that must then come from another inline area (here GC TB_SetJust).
- **uopt unroll shapes**: a loop is unrolled by 4 when its counter is the loop variable with a known trip count
  divisible by 4; per-copy strength-reduced IVs (retail's four colour registers, exit `!= 280`) appear only when the
  body uses a multiple of the counter (`c * 8`), not when the counter steps by 8. Loops on a variable used after the
  loop, `i` counters feeding coordinates, and a 3-trip loop are not unrolled here.

## Permission denials
None.
