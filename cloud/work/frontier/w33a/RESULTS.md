# w33a — high-score unit + hud (wave 33), IDO 5.3 -O3

Status: FINAL (wave 33). **DrawUnlocked EQUAL in the unit (588/629 -> 0); packet `groups/highscore_anim` ready (DrawName +
DrawGhost + DrawUnlocked, 5,960 B, provisional on the caller form — highscore_animText stays an EQUIVALENT draft,
coordinator decision).** animResults 562 -> 150/627 (ops 41 -> 15, length and frame exact, EQUIVALENT 9,241 calls).
highscore_animText frame and func_80100E58 not advanced (one lever each, written up). Unit tag `w33a`; builder scratch
`watchman2:~/rush2049/scratch/frontier/w33a` (deleted at the end; `score.py group` / `gproof.py` re-runs need a fresh scratch copy).

| function | bytes | before (w32a) | after (w33a) | verdict | residual |
|---|---|---|---|---|---|
| DrawUnlocked | 2,516 | 588/629, ops 48 | `EQUAL DrawUnlocked: 629 words (internal, c_best.c)`; group MATCH | packet member | none (device `if (p) {}`) |
| DrawName | 1,040 | EQUAL (unit) | EQUAL; group MATCH | packet member | none |
| DrawGhost | 2,404 | EQUAL (unit) | EQUAL; group NOT VERIFIED two-region .rodata, splice proof image-identical | packet member | none |
| highscore_animText | 1,780 | 27/445, frame 584/704 | 27/445, frame 584/696 with K&R DCC/DS (`hs/v/r1.c`; best.c keeps 704) | EQUIVALENT 2,473 calls/4 scripts | frame only: nested helper areas (§1) |
| animResults | 2,508 | 562/627, 358 rows, ops 41 | `150 of 627 words differ` (228 rows, ops 15), frame 400/400, length exact | EQUIVALENT 9,241 calls/3 scripts | as1/temp-ring + constant hoist (§3) |
| func_80100E58 | 2,732 | 534/683, 180 rows | unchanged (`f0e58/best.c` = w32a copy) | — | bb45 `columns` split (§4) |
Start points: w32a `hs/best.c` (renamed to the w32n names: highscore_animText, DrawName, DrawGhost, DrawUnlocked;
copied as `hs/v/w32a_best.c`; the current file is `hs/best.c`, unit flags `hs/uargs_hs.txt`), w32a `func_80105EA8/best.c` (animResults),
`func_80100E58/best.c`.

Baseline reproduced on `wave33` (Pi): `blob_unit --tag w33a score highscore_animText DrawName DrawGhost DrawUnlocked
--with cloud/work/frontier/w33a/hs/best.c $(cat cloud/work/frontier/w33a/hs/uargs_hs.txt) --neighbours`
```
  FAIL highscore_animText: 27 of 445 words differ
  EQUAL DrawName: 260 words (internal, c_best.c)
  EQUAL DrawGhost: 601 words (internal, c_best.c)
  FAIL DrawUnlocked: 588 of 629 words differ
  locked bodies that differ in this unit: 0
```

## 1. highscore_animText frame (584 vs 704) — lever hour(s) spent, NOT closed; three new facts

Tools: `anchors.sh FILE [UARGS]` (repo-relative FILE; prints retail/our frame-top-relative anchors), `tools/mmt.py
MERGED [CONSTS]` (dumps every Mmt slot of the merged ucode with its source lines — the per-site layout umerge
actually built; fetch `unit/w33a/stage/merged` from the builder), `tools/syn.sh`/`synk.sh` (synthetic -O3 / -O3 -K
compiles on the builder; `hs/syn/*.c` are the tests quoted below).

### Fact A (new law): umerge's per-site area is `align8(caller_size + callee_parm_size)` + callee Mmt, and an
ANSI `(void)` definition has an 8-byte parm size while a K&R `()` definition has 0
From the recompiled umerge (`tools/ido-static-recomp/build/umerge.c` f_inline_procedure @L40804c: `base =
align_to_double(caller+56 + callee+48); caller+56 = callee+56 + base`) and synthetic tests (`hs/syn/m1.c` vs `m2.c`):
`static void mid(void){char m[32];…}` inlined into a 64-B frame puts m at -104; `static void mid(){…}` puts it at -96.
K&R zero-parameter helpers with no locals cost 0 B per inlined site (q3/r3: 3 sites, frame unchanged, even with a used
return value except for one 4-B vreg); a helper with one local or one parameter costs 8 B per site (s3, t3); ANSI
void(void) helpers 8 B per site (c3). Applied to the draft: `void DrawCircuitCode()` / `void DrawScores()` (K&R)
`v/r1.c`: frame 704 -> 696 (`27 of 445 words differ | words 33 ops 2`), no code change. Static vs non-static, `__inline`,
K&R/`register` params, definition order: no effect (k2, k3, r4, r5, o2, p1–p3).

### Fact B: retail's high-score file has exactly four deleted-static stubs, in umerge postorder
`jr ra; nop` at 0x80103D18 (two), 0x801046FC (one, between DrawUnlocked and DrawName) and 0x80105478 (one, between
DrawGhost and highscore_animText). Our `umerge -v` log (`unit/w33a/stage/umerge.log`) shows umerge emits a caller
after its callees, visiting animText's callees in REVERSE call order (DS, DG, DN, DCC, DU); retail's emission order is
FORWARD (DU, [stub = DCC], DN, DG, [stub = DS], animText). So retail: two statics first used by DrawUnlocked (its font
and centre helpers), DrawCircuitCode and DrawScores are statics inlined into animText, and NO other file-static helper
is first used by DrawName, DrawGhost or DrawScores (no stub before DrawName except DCC's, none before DrawGhost):
hs_SetFont/hs_RightText/hs_SetJust/gh_* cannot be file statics in retail (macros, or extern functions from another
file such as GC's hal.c/TB_text.c). The forward order suggests retail's case bodies or call order differ from ours.

### Fact C: retail's top layout = [p][4 B][DrawCircuitCode cfe locals 80 B][DrawScores cfe locals 360 B][36 B]
`v/k4.c` (DCC/DS bodies as case blocks, one function-level `s32 old;` after p, font sites as a macro) puts all three
anchors exactly at retail's: `[88, 192, 448]` (str[64] T-88, sprintf buffer str[96] T-192, label[256] T-448; frame
648/584, 416 rows — the macro loses every `move s0,v0`, so it is a layout probe only). So DrawScores' locals are
exactly w30a's `i, numWid, str[96], label[256]` (label is NOT an ellipse-helper local: no stub for such a helper),
DrawCircuitCode's are GC's `x, y, i, length, str[64]`, and NEITHER block contains any inline area or vreg: all the
4 + 7 helper sites inside them cost 0 B in retail, while retail keeps their artifacts (`move s0,v0` = the inlined font
helper's `old` local surviving as a dead copy, which in every synthetic test needs a helper local, i.e. an area;
the CT `sll a0; sra t8,a0; move a0,t8` return copy, lost when written out: `v/r3.c` 390 rows). Retail's remaining
Mmt below the label is 36 B (T-448..T-484; spill temps 84..96), with the times-loop `old` at T-460.
Contradiction (open): no source shape found that keeps a helper local's dead copy at zero area. Mechanisms excluded
by test: umerge processing order (definition order, static, K&R, kept/internal, cross-file, fn-pointer root,
switch arms, shared helper — all postorder with nested areas included, `hs/syn/n1,nb,y1,z*`); per-callee area
sharing (never happens: w6, 3 sites = 3 areas).
Best next hypotheses: (1) the dead `old` copies come from a non-area mechanism we have not modelled — test whether a
helper returning through a K&R zero-parameter inline (`static s32 fs(){…}` with the font number in a file-static or
the s2 IPA register) can keep a dead `move s0,v0`; (2) retail's helpers were inlined into DCC/DS in a later umerge
pass after DCC/DS were already merged into animText (stale callee size: their areas would overlap the following
blocks and never enlarge the frame) — would need an `-inline_to`/`-space_time`-style budget difference, i.e. not
reproducible in the unit. Tested: re-running umerge on the unit's `split` with `-space_time 0/10`, `-inline_to 0/1000`
changes how many calls are inlined (214..2093) but never drops DCC/DS's nested areas (animText Mmt 0x250/0x274/0x870).
Other tests: `__inline` kept helpers (k3), all-static helpers (r5), definition order (o2), block-form DCC/DS (k1: frame
688, anchors 4 B high) — no layout that matches.

## 2. DrawUnlocked (2,516 B) — EQUAL in the unit, MATCH in the group compile
Source: `hs/best.c` (= `groups/highscore_anim/highscore.c`; the whole high-score unit, unit flags `hs/uargs_hs.txt`).
Levers (one at a time, `du/v/`):
1. GC type fact (wave-33 lesson): GC DrawUnlocked's locals are `int x, y, i, p; char str[64]; int stuffCount` — the
   draft's `s16 x` was the entire 48-op residual: `x1.c` `FAIL DrawUnlocked: 68 of 629 words differ | words 67 ops 0`.
2. Colour: ctrace (`dux1`): &str (type-1 web w297, save 42.857) beat p (w9, save 42.25) for s5. DISCLOSED SHAPING
   DEVICE `if (p) {}` at the head of the player loop (`x4.c`; same at the loop end `x5.c`; after `stuffCount = 3` or
   after the `continue` test it perturbs other webs): `4 of 629 words differ | words 3 ops 0`.
3. as1 tie (wave-32 lesson, CompressGhost): `du_gfx_unlock` DEFINED AFTER `du_font_set` (prototype first), so the
   unlock's osSendMesg argument lines come after `old = slot_state_setup(font)` and `move s0,v0` is scheduled first
   (`y1.c` = `hs/best.c`). Moving du_font_set after the caller (`x2/x8`) or one-line helpers (`x9`) do nothing.
Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w33a --jobs 2 score highscore_animText DrawName DrawGhost
DrawUnlocked --with cloud/work/frontier/w33a/hs/best.c $(cat cloud/work/frontier/w33a/hs/uargs_hs.txt) --neighbours`
```
  FAIL highscore_animText: 27 of 445 words differ
  EQUAL DrawName: 260 words (internal, c_best.c)
  EQUAL DrawGhost: 601 words (internal, c_best.c)
  EQUAL DrawUnlocked: 629 words (internal, c_best.c)
  locked bodies that differ in this unit: 0
```

## Packet `groups/highscore_anim` (NEW group; coordinator decision needed: provisional on the caller form)
claims = members = DrawName, DrawGhost, DrawUnlocked (1,040 + 2,404 + 2,516 = 5,960 B); context =
highscore_animText (keep; EQUIVALENT draft, 27/445 frame-only), slot_state_setup/object_create/func_800B669C via
`ctx_gr3_b.c` (byte-identical copy of locked `frontier_results_table/ctx_gr3_b.c`). Precedent: wave 32
codex_control_settings_a4 (members installed with control_settings, an EQUIVALENT draft, as kept context).
`score.py group cand/groups/highscore_anim` (builder, with and without `--claims`, rc=0):
```
DrawName:
  MATCH
DrawGhost:
  NOT VERIFIED (6 section-relative relocations unverified: .rodata+0x4 at +0x1ec, .rodata+0x4 at +0x1f0, .rodata+0x24 at +0x640, .rodata+0x24 at +0x648, .rodata+0x14 at +0x6c4, .rodata+0x14 at +0x6cc; own .rodata: this function's references disagree on the section's image address (0x80120650 for +0x4/+0x14, 0x8012485C for +0x24): the literals are not laid out as in retail)
DrawUnlocked:
  MATCH
```
(DrawGhost: the known two-region own-.rodata case — literals at 0x80120650, jump table at 0x8012485C.)
Splice-proof replay (`tools/gproof.py`, read-only `blob_group.group_bodies`):
`group_bodies OK: DrawGhost, DrawName, DrawUnlocked` / `DrawGhost: 601 words, 0 differ from the image` /
`DrawName: 260 words, 0 differ from the image` / `DrawUnlocked: 629 words, 0 differ from the image` /
`3/3 members image-identical`.
Integration: `install_group.py cloud/work/frontier/w33a/groups/highscore_anim highscore_anim` (no group superseded, no
locked stub given a body, no unit_overrides needed beyond the group's own keep list; helpers du_*, hs_*, gh_*,
DrawCircuitCode, DrawScores are non-static, internal, not on keep). Record "provisional on the caller form
(highscore_animText)". When highscore_animText closes, add it as a member of this same group.
Shaping devices (header of highscore.c): non-static inlined helpers (hs_SetFont/gh_SetFont void wrappers, hs_SetJust,
gh_font_set), the unused `s16 gFreePages` in DrawGhost (exact frame residual 568 -> 576, one slot), the compiled-out
`do {} while (0);` in DrawGhost's status-1/2 arm, NEW the `if (p) {}` holder in DrawUnlocked and the
define-after-use order of du_gfx_unlock.

draft_check (rocky) on the whole unit file: `python3 -m tools.recomp.draft_check --remote rocky --unit
highscore_animText,DrawName,DrawGhost,DrawUnlocked cloud/work/frontier/w33a/hs/best.c --params DrawName:p=s5 --scripts
ghost_pak,unlock,player_finish,race_finish` -> `VERDICT: EQUIVALENT over 2473 calls / 4 scripts (ghost_pak 1144, unlock
308, player_finish 863, race_finish 158)` (`hs/dc.args` carries `--unit ... --params DrawName:p=s5` for `--all-lanes`).

## 3. animResults (2,508 B) — 562 -> 150/627, ops 41 -> 15, frame and length exact
`ar/best.c` (unit flags `ar/uargs.txt` = `--internal ar_font_set --internal ar_SetJust --internal ar_SetFont`; `ar/dc.args`
`--params arg0=a0`). Three source levers, no new devices (header):
1. `playerIdx << 4` for the pane colour argument (`p5.c`): uopt no longer PRE-hoists p*16 above the font-height call into
   an s-register web, so x is coloured s8 again: `324 of 627 words differ | words 240 ops 17`, frame 392 (w31a's `<< 4`
   test predates the current unit — re-score old variants). `(u32)playerIdx * 16` and `16 * playerIdx` do nothing.
2. The per-player pane half-height in its own named local `h` (after `y`), not the reused `min` (`h1.c`): frame
   400/400 (retail's dead `move v1,t9` is h).
3. Time converted once (`t2.c`): `min = (u32)time; min = min / 60 % 100; sec = (u32)time % 60;` in both time blocks
   (retail's `move v0,t0` is the first assignment; GC also converts once): body 627/627,
   `FAIL animResults: 150 of 627 words differ` (`words 228 ops 15 norm 15 | frame 400/400`), `locked bodies ... 0`.
Remaining ops: gNumLaps `lh` order in the counting loop; the first ar_SetJust stores vs osRecvMesg arguments (defining
ar_SetJust first, `q1.c`, does nothing); retail hoists the two round-mode `li 1` constants (t0, v1) to the mode-5 head;
mfhi/ctc1 order in the time blocks. draft_check: `VERDICT: EQUIVALENT over 9241 calls / 3 scripts (race_finish 3218,
player_finish 2633, two_player_finish 3390)` (`--params arg0=a0`). The own-.rodata line (`+0x504: own .rodata+0x40
... differs from the image`) is the scorer's base check over strings + float literals; the strings are in retail order.

## 4. func_80100E58 (drawRaceStandings) — not advanced
`f0e58/best.c` = w32a's rebased copy (unit flags `f0e58/uargs.txt`): `534 of 683 words differ; compiled body is 682
words` (words 180 ops 9, frame 528/528). One lever each: `s32 y` (GC has no y; `v/y1.c`) no change; `volatile columns`
(`vol1.c`: 325 rows, frame 520) and `volatile rows, columns, row` (`vol2.c`: 504 rows) worse — retail's memory-resident
row/rows/columns are a live-range split, not volatile. GC drawRaceStandings locals are `str (u8 *), str1[40], rows,
row, col, c[4], time` — the draft's extra N64 locals (x, previous, y, text, columns, total, other, label[236]) are frame
exact; next: w32a's interferer listing of piece 299 at bb45.

## Integration notes
- **highscore_anim** (NEW group, coordinator decision on "provisional on the caller form"): `install_group.py
  cloud/work/frontier/w33a/groups/highscore_anim highscore_anim`; claims/members DrawName, DrawGhost, DrawUnlocked;
  context highscore_animText (keep) + ctx_gr3_b.c. No group superseded, no locked stub given a body, no data-symbol
  changes, no unit_overrides beyond the group json. DrawGhost's NOT VERIFIED line is the accepted two-region own-.rodata
  case (splice proof replayed image-identical). Disclosed devices listed above.
- animResults / func_80100E58 / highscore_animText: drafts only (not coverage).

## What generalises
- **K&R `()` definitions cost no parm area when umerge inlines them; ANSI `(void)` costs 8 B per site** (umerge
  f_inline_procedure: `base = align8(caller_size + callee_parm_size)`). A frame residual of 8 B per inlined
  no-parameter call is a prototype-style lever, not a missing local.
- **GC local types first**: DrawUnlocked's whole 48-op residual was `s16 x` vs GC's `int x` (588 -> 68 words).
- **A deleted-static stub census gives retail's umerge postorder** (callees emitted before callers, one stub per inlined
  file static): it tells which helpers were file statics and which were macros/extern.
- **Re-score old "tried" variants** in the current unit: `playerIdx << 4` failed in w31a and now removes animResults'
  PRE web (358 -> 240 rows).
- `tools/mmt.py` (merged-ucode Mmt dump per source line) shows exactly where umerge put every inlined area/vreg.

## Permission denials
None.
