# w38b RESULTS (wave 38, overlay images A and B: deleted-inline stubs, then near-misses)

Branch wave37 at fc6281e8b. Builder scratch `watchman2:~/rush2049/scratch/frontier/w38b` (copied from `base`; its
src/ovl_*, asm/us/ovl_* and ovl lock are identical to this checkout). Lane dir only; no git operations; no permission
denials.

Headline (Part 1): **33 stubs claimed** in 14 group packets, every old member still strict MATCH: image A +31 stubs
(127 -> 158/193 functions, +248 B), image B +2 stubs (41 -> 43/49, +16 B). Several revisions also *remove* disclosed
shaping (unused frame slots, carriers, one compiled-out holder): the 8-byte frame area umerge reserves for each inlined
call is what those devices were standing in for.

## Method (stubs)
- The images are laid out in the post-order of the IPA call graph (w36n): callees visited from the last call site to
  the first, each function once; an inlined helper leaves its 8-byte deleted procedure at its slot. Refinement found
  here: game-called roots are processed first; the rest of each file (the blit callbacks) is laid out by a forward
  scan in SOURCE order with a post-order DFS from every procedure not yet emitted. That explains recordscr
  (drawSelect before drawView's subtree), playerscr (drawSelect .. drawPakDamaged in file order), circuitscr
  (drawReview first) and pakscr (_GetPakStatusStr's stub before _AnimText's subtree, none before _DrawPickPak).
  With MAT3's call lists and file order (`tools/mat3fns.py FILE.c`) every claimed stub position is predicted.
- `tools/objsyms.py` (builder side) compiles a packet exactly as `score.py group` does and prints the .text symbols in
  object order with each name's retail extent. Every claimed stub below is emitted at its retail slot relative to its
  neighbours (transcripts in `scores/<group>.txt`, section "object order"); the one exception is pakscr's
  _GetPakStatusStr, whose retail slot comes from the source-order scan that a one-root group compile does not reproduce.
- Helper = MAT3 static, non-static and off `keep`, named after its stub, defined in MAT3 file order. Bodies were moved
  verbatim out of the caller where the installed source had hand-inlined them.
- `tools/ownv.sh PACKET ovl_a|ovl_b` fetches the compiled object and runs `score.compare` locally with own data
  verified against the retail image (build/ovl/<x>/image.bin, sha256 = extents.json image_sha256). All new revisions:
  every member strict MATCH with own data verified, except F79C, FFD8, 915E8, 94DD8, whose own .rodata lies in two
  retail regions (strings/floats) - identical result on the installed copies (the splice's window proof handles it).

## 1. Stub table

State: **MATCH** = claimed in a packet, scorer MATCH; **caller** = identified, lands only with its unmatched caller;
**tried** = identified, helper form tried, caller no longer matches (not delivered); **unid** = not identified.

| stub | helper identity | evidence | group (packet) | state |
|---|---|---|---|---|
| A 8038A62C | setCamera@trackscr | post-order of updateTrackScreen (its last call); helper already defined by w36d, inlined into member DFEC | dot_runtime_a_c93c_closure_20261012 | MATCH |
| A 8038AEA8 | loadTrackScreen | buildTrackMap's callee; helper defined (w36e) | c93c | MATCH |
| A 8038B930 | clearTrackMaps | MAT3 _UninitScreen calls uninitButtons, clearTrackMap, clearTrackMaps; body moved out of B940 | c93c | MATCH |
| A 8038B938 | uninitButtons@trackscr | same; body moved out of B940 | c93c | MATCH |
| A 8038C934 | BlowFog | _InitScreen (DFEC) callee; helper defined (w36d) | c93c | MATCH |
| A 8038CCE4..8038CD0C (6) | unloadPathData, loadPathData, buildRaceTrackMaps, buildObstacleTrackMaps, buildBattleTrackMaps, buildStuntTrackMaps | post-order of buildTrackMaps; helpers already defined in c93c and emitted at exactly these slots | c93c (inlined into context CD14) | caller (func_8038CD14) |
| A 8038F734 | uninitButtons@startscr | post-order of updateStartScreen (CheckForEgg F648 first, then uninitScreen's subtree); former static renamed | w35d_start_screen | MATCH |
| A 8038F73C | uninitScreen@startscr | same | w35d_start_screen | MATCH |
| A 8038FFD0 | initButtons@startscr | initScreen's first callee (InitEgg is the locked single FF90, not a stub) | w35d_start_screen | MATCH |
| A 80390BB8 | CreateSelectCar | post-order of UpdateSelectCar (w36e) | dot_runtime_a_joint_transforms_20261006 | caller (func_80390DCC) |
| A 803915E0 | updateViewButtons | helper recordscr_updateViewButtons renamed | dot_runtime_a_callers_deep_20261011 | MATCH |
| A 803932D4 | controlView | case 1 of updateRecordScreen (94058) moved into a helper | callers_deep | MATCH |
| A 803932DC | uninitButtons@recordscr | loop of uninitRecordScreen (932E4) moved into a helper | callers_deep | MATCH |
| A 803949C4 | uninitButtons@playerscr (signed char p) | loop of uninitScreen (949CC) moved into a helper | dot_runtime_a_menu_cleanup_20261007 | MATCH |
| A 80395E14, 80395E1C | controlPakDamaged, controlPakFull (w36c listed them the other way round) | the helper form of 9842C (`v/ps/all_u0_s1_top`) emits all four control stubs at exactly 95E14/95E1C/96ED8/96EE0 | availability_refresh (root 9842C context) | caller (func_8039842C) |
| A 803968B8 | claimPlayer | helper named func_80396EE0 in the installed group is emitted at 968B8 (after controlDeletePlayer); renamed | dot_runtime_a_availability_refresh_20261007 | MATCH |
| A 80396ED8, 80396EE0 | controlControls, controlWait | w36c + post-order | availability_refresh | caller (func_8039842C) |
| A 80397FF4 | calcSelectMenuMaxSize | helper already defined, inlined into initScreen 97FFC | availability_refresh | MATCH |
| A 80398B38 | _BuildFileDelList | _SetState case 1 moved into a helper | dot_runtime_a_menu_state_20261006 | MATCH |
| A 80398BE0 | _ControlDamaged | updatePakScreen case 4 | menu_state | MATCH |
| A 80398BE8 | _ControlRumblePak | case 3 | menu_state | MATCH |
| A 80398C34 | _ControlFileDelVerify | case 2 (after _FindFile 98BF0 in post-order) | menu_state | MATCH |
| A 80399384 | uninitButtons@modescr | helper modescr_uninitButtons renamed | dot_runtime_a_seven_option_group_20261007 | MATCH |
| A 8039938C | uninitScreen@modescr | helper modescr_uninitScreen renamed | seven_option_group | MATCH |
| A 803998B8 | initButtons@modescr | button set-up of initScreen 998C0 moved into a helper | dot_runtime_a_seven_option_init_20261007 | MATCH |
| A 8039A440 | ? (first slot of the circuitscr subtree, before menuItemValid) | not in MAT3's call graph | text_helpers | unid |
| A 8039B004 | uninitButtons@circuitscr | loop of uninitScreen B00C moved into a helper | dot_runtime_a_text_helpers_20261006 | MATCH |
| A 8039B558 | controlShowTrophy | the former C140_ANIMATION_ARM macro of C140 | text_helpers | MATCH |
| A 8039BA20 | getPlayerStr | name copy in calcProgressSize BB80 | text_helpers | MATCH |
| A 8039D044 | clearCarComponents (int p) | MAT3 _UninitScreen order; body moved out of 9D05C | dot_cleanup_colortrace_20261011 | MATCH |
| A 8039D04C | uninitButtons@carscr (signed char p) | same | dot_cleanup_colortrace | MATCH |
| A 8039D054 | releaseGridMap (empty on N64, like createGridMap EF00) | same; called under `gNumPlayers == 1 && gRace != 2` | dot_cleanup_colortrace | MATCH |
| A 803A3D9C | ? (between the slider callbacks A3A6C / A3DA4; correlation lead "drawOptions@carscr") | MAT3 animNewSlider has no in-file callee | - | unid |
| A 803A44EC, 803A44F4 | drawOptions (empty), animButtonText @carscr (in that order: our object emits drawOptions first) | w36b; `A44FC/g` object order | (A44FC standalone) | caller (func_803A44FC) |
| A 803A6F14 | _GetPakStatusStr (int pak, char *str) | source-order scan reaches it (line 224) before _AnimText (251); inlined into _DrawPickPak 7620, whose four unused slots drop to one (MAT3 statStr[24]) | dot_runtime_a_menu_renderers_20261011 | MATCH |
| A 803A6F1C | _DrawDamaged | _AnimText case 4 (visited first) moved into a helper | menu_renderers | MATCH |
| A 803A6F24 | _DrawRumblePak | case 3 | menu_renderers | MATCH |
| A 803A8B6C | drawControls | w36c; only caller animText A9EEC | runtime_a_profile_menu (A9EEC context) | caller (func_803A9EEC) |
| A 803A9EDC, 803A9EE4 | drawPakFull, drawPakDamaged | w36c | runtime_a_profile_menu | caller (func_803A9EEC) |
| A 803ACA8C | drawStuntTotals | helper recordscr_drawStuntTotals is emitted at this slot; its only caller is drawView | runtime_a_stats_menu (AD57C context) | caller (func_803AD57C) |
| A 803AD574 | drawView | helper recordscr_drawView, inlined into animText AD57C | runtime_a_stats_menu | caller (func_803AD57C) |
| A 803AE62C, 803AE634 | probably AnimOptText, animSlider (GC-stripped trackscr functions that sit between showTrackSelect and animNewSlider in the DWARF/link order) | source-order scan; no reference in either image, so dead (an unreferenced procedure also compiles to `jr ra; nop`) | dot_runtime_a_settings_bar_20261012 | identified (dead), not claimed |
| A 803AECC4, 803AECCC, 803AECD4 | probably AnimZ, animFan (GC blit callbacks the N64 tables no longer reference) + one more (drawOptions is a candidate, but its source position is after animText) | source-order scan between animArrows (AE940) and animText (AECE4) | dot_runtime_a_settings_values_20261012 | identified (dead), not claimed |
| A 803AECDC | animButtonText@trackscr | the whole body of the animText callback AECE4; the inlined call's 8-byte area is exactly the unaccessed native x/y homes (sp+208..215) | dot_runtime_a_settings_values_20261012 | MATCH |
| B 8038A400 | findDamageSlot | worlddam.c; the locked single's static, renamed | NEW w38b_worlddam (supersedes single func_8038A408) | MATCH |
| B 8038CB10, 8038CB18 | _NewStreakPoly, _AnimateStreakPolys | post-order of UpdateStreaks | dot_runtime_b_trail_20261012 | tried (8CB20 48/333: FP constant/s-register colouring, 40 variants) |
| B 8038FCD8 | AnimateCase | post-order of ShotUpdate (w36n strong) | w36h_shots | tried (8FCE0 47/764: frame +8, every home +8; 16 variants) |
| B 80391640, 80391648 | animStuntCoins, animBattleCoins | post-order of UpdateHUDExtraPostCamera (w36n) | dot_runtime_b_status_scene_20261012 | caller (func_80391B00) |
| B 803925C8 | ClearBattlePoints | hudextra.c; the locked single's static, renamed | NEW w38b_hud_show (supersedes single func_803925D0) | MATCH |

Totals: MATCH 33 (A 31, B 2); caller 20 (A 18, B 2); tried 3 (B); identified-dead not claimed A 5 (AE62C, AE634,
AECC4, AECCC, AECD4); unid A 2 (A440 before circuitscr's subtree; A3D9C between the carscr slider callbacks A3A6C and
A3DA4). Claiming the dead ones needs an owner call: their bodies would be unverifiable MAT3 ports (stub bytes only).

## 2. Integration notes (one per packet; all in `cloud/work/frontier/w38b/groups/`)

How w36i installed overlay groups: `python3 -m tools.conveyor.pipeline.ovl_splice group --targets asm/us/ovl_a
--claims all cloud/work/frontier/<lane>/groups/<group>` (same name supersedes in place; it refuses if a member is
locked outside the group or if an old member would be dropped), then the overlay gates (`ovl_layout build`,
`ovl_splice check`, `ovl_rom rom`: image byte-identical, ROM SHA-1 exact) and the test suites. Each packet's
group.json has `claims` = every member (old + new), `context`/`keep`/`flags` unchanged unless stated. No
`force_internal`, `prefer_definition` or `extern_data` overrides are needed anywhere (overlay groups have none).
Scorer transcripts: `scores/<group>.txt` (plain, `--claims`, object order).

| packet (= installed group it supersedes, same name) | old members (all still MATCH) | new claims | notes |
|---|---|---|---|
| dot_runtime_a_c93c_closure_20261012 | C93C BBB4 DFEC ACB0 B834 A634 AE50 B388 B940 AEB0 B3EC C5EC (12) | A62C AEA8 C934 B930 B938 | context E5DC/CD14/A820 unchanged; keep [E5DC]. `--claims` exit 0 |
| w35d_start_screen | F79C 80390418 FFD8 | F734 F73C FFD0 | context F744; keep [80390418] |
| dot_runtime_a_callers_deep_20261011 | 92F28 93004 930B8 931C0 932E4 94058 923B0 915E8 9338C 93B3C 93B34 | 915E0 932D4 932DC | 94058's disclosed unused residual_slot0/1 and `mode` removed (now no shaping); keep [94058] |
| dot_runtime_a_availability_refresh_20261007 | 94564 94834 94B00 97AD8 97FFC 968C0 95E8C 94DD8 96FA0 | 97FF4 968B8 | helper renamed func_80396EE0 -> func_803968B8; context unchanged (9842C 2/451 as before) |
| dot_runtime_a_menu_cleanup_20261007 | 949CC | 949C4 | context unchanged |
| dot_runtime_a_seven_option_group_20261007 | 99394 A6B50 993EC 99D10 | 99384 9938C | eligibility.c changes (renames only). The capacity_evidence receipt already accepts a superseded production (its superseded copy exists), production keeps 99394/A6B50 |
| dot_runtime_a_seven_option_init_20261007 | 998C0 | 998B8 | adds `allow_unverified: true` (998C0's own .rodata; the installed json lacked it, so `--claims` exited 1 there too); own data verified locally |
| dot_runtime_a_text_helpers_20261006 | 17 members (A448 A4D4 B00C B120 B214 B560 B908 B988 BA28 BAE0 BB80 BE48 C140 A5658 A5948 A5938 A5940) | B004 B558 BA20 | C140 loses its macro + 2 carriers; BB80 keeps 4 of its 8 disclosed unused homes (exact residual now 4) |
| dot_runtime_a_menu_state_20261006 | 98B40 98C3C 98E70 990D0 | 98B38 98BE0 98BE8 98C34 | 990D0's 4 disclosed unused slots removed (MAT3 local order); adds `allow_unverified: true` (990D0 jump table) |
| dot_runtime_a_menu_renderers_20261011 | 6F2C 7180 7620 7918 | 6F14 6F1C 6F24 | 7918's 3 unused slots removed, 7620 keeps 1 of 4 (disclosed); adds `allow_unverified: true`. **Breaks `tests/cloud/test_runtime_a_writer_bounds.py`**: its verify.py (lines 257-263) requires production group.c to equal its packet (comments stripped). Needs the handoff section 5 superseded-copy fallback (byte copy of the current production group.c in cloud/work/frontier/superseded/dot_runtime_a_menu_renderers_20261011/, verify.py compares with it when production differs and requires production members to include the packet's, re-pin verify.py in verification.json) - an owner patch |
| dot_runtime_a_settings_values_20261012 | AECE4 | AECDC | AECE4 = animText calling the helper; the native slot carrier is gone; adds `allow_unverified: true` (AECE4's own .rodata; verified locally). The settings_neighbor receipt uses its own copy, not production |
| dot_cleanup_colortrace_20261011 | 14 (D300 D05C D494 ED20 E200 E3BC FF04 D6A4 EF08 A0FD8 A1EAC EF00 A0498 E444) | D044 D04C D054 | D05C's disclosed `if (player) {}` holder removed |
| NEW w38b_worlddam (image B) | (supersedes the locked single func_8038A408) | 8A400, 8A408 | First `ovl_splice revert` the single func_8038A408 (src/ovl_b/func_8038A408.c), then install the group; keep [8A408]; `allow_unverified: true` |
| NEW w38b_hud_show (image B) | (supersedes the locked single func_803925D0) | 925C8, 925D0 | revert the single func_803925D0 first; keep [925D0] |

Receipts: grep found no other test or receipt that reads the production files of these groups (the availability,
a5658/bb80/c140, menu_state_root, product_pre and renderer_residual receipts use their own copies or pre-install
snapshots). Run the full suites anyway; the menu_renderers/writer_bounds case is the one known failure.

## 3. Near-misses (Part 2)

| function | state | scorer (exact) | best file | tried this lane | next hypothesis |
|---|---|---|---|---|---|
| A func_8039842C updatePlayerScreen | NONMATCH 2/451 (unchanged; installed context form) | w36c/f3 form: `2/451 words differ` (sw/lw v0 at 120 vs 124). Helper form (four MAT3 control helpers inlined, the four retail stubs 95E14/95E1C/96ED8/96EE0): `10/451 words differ` (q, no unused) / `22/451` (no q, cfe area 64 = retail, frame 184) | `v/ps/all_u0_s1_top/group.c`, `v/ps/all_u0_s1_none/group.c` | 4 control helpers, q forms, unused counts, block-local placement, first-loop index (i/p/playerIdx), compiled-out holders (80+ variants; spill/pretrace traces) | see the 9842C section below: retail = cfe area 64 + 5-6 temp homes; its temp1 holds a web numbered between 17 and 53 that is live in the main loop |
| A func_80390DCC UpdateSelectCar | NONMATCH 33/425 (unchanged) | `score.py group` (w36e group_best, 90DCC context): `33/425 words differ` - every word a temp-home offset (+4) | w36e/func_80390DCC/group_best (copy `9DCC/best0`) | unused-local count 8..13 (11 or fewer: frame 224; 12 = 13 = 33 words); spill trace | retail = cfe area 116 (ours 120) + 7 temp homes, the 7th never referenced (ours 6 homes). Needs one more coloured expression web that takes a home without spill code, then 11 unused locals. CreateSelectCar is still a plain-named helper: name it func_80390BB8 when 90DCC lands |
| A func_803A9EEC animText@playerscr | NONMATCH 102/315 (unchanged) | `ctx A9EEC=102/315` (w36c best_group.c) | w36c/A9EEC/best_group.c | listing oracles on w36x's v4.s (`A9EEC/*.s`, `tools/ocmp.py`): besides w36x's `.alias $19,$sp`, a `.noalias $3,$sp` / `.alias $3,$sp` pair around case 6's final `lh $3, D_8014A108` also gives `12/315` (only the listing's unresolved `$$NN` relocations). A `la; lh 0($3)` form does not: as1 does not fold the `addiu` (55/315, one extra word) | any alias directive between case 6's `b` and the default label is enough; find the C that leaves a noalias region open at the end of case 6 |
| A func_803A44FC animText@carscr | NONMATCH 2/917 (unchanged) | `score.py fn ... --targets asm/us/ovl_a`: `2/917 words differ` (+0x094/+0x098 bne/sll) | w36b/func_803A44FC/best.c | rescored only | (post-order fixes the stub names: A44EC = drawOptions, A44F4 = animButtonText - reversed call order) |
| A func_803AD57C animText@recordscr | NONMATCH 497/523 (unchanged) | `ctx AD57C=497/523` (installed stats group) | installed runtime_a_stats_menu_20261006 | rescored only | helpers recordscr_drawView / recordscr_drawStuntTotals are emitted exactly at AD574 / ACA8C: rename and claim them when AD57C lands |
| A func_803A9088 drawDeletePlayer | NONMATCH 15/917 (unchanged) | `ctx A9088=15/917` | installed runtime_a_profile_menu_20261006 | rescored, compared with MAT3 | the installed body is native-led (`>= 2` where MAT3 has `> 1`, different locals); same class as 9842C/90DCC (one more conflicting/spilled web in retail); a MAT3-shaped port is untried |
| A func_8038A400 updateWarningScreen | NONMATCH (w36e: 50/139) | not rescored | w36e/func_8038A400/var/n.c | read only | layout evidence: nothing precedes A400 (image base) and warningscr's callback AF744 has no stub before it, so warningscr has **no** inlined static: w36e's `anyAttached` helper cannot be the original form |
| A func_8038CD14 buildTrackMaps | NONMATCH (installed context 1085/1206; w36d best 1005/1206) | `ctx 8038CD14=1085/1206` | w36d/func_8038CD14/best_unit.c | none | its six helpers are already named after their stubs in c93c |
| B func_80391B00 UpdateHUDExtraPostCamera | NONMATCH 407/595 (83 with w36j's holder) | `best0 ... 91B00=407/595`, `holder0 ... 91B00=83/595` | w36j/func_80391B00/best, best_holder | coin helpers non-static (func_80391640/48): no change (407 / 83) | w36j's "one more interfering expression web" again |
| B func_8038CB20 UpdateStreaks (stubs 8CB10/8CB18) | MATCH installed; helper form 48/333 | `m2_nonec0x0u0 ... 8CB20=48/333` | `v/st/` | 40 variants: _NewStreakPoly/_AnimateStreakPolys helpers, holder, continue form, xyz carriers, unused locals | FP constant registers ($f30/$f26) and s6/s5 colour; the native body's holder/unused slots do not carry over |
| B func_8038FCE0 ShotUpdate (stub 8FCD8) | MATCH installed; helper form 47/764 | `k1beforec0 ... 8FCE0=47/764` | `v/sh/` | 16 variants: AnimateCase helper with/without idx carrier, block-scoped locals, placement | `spill.sh`: cfe area 88 (installed) -> 96 with the helper. A two-local AnimateCase (MAT3: casing, next) gives area 88 = retail but loses retail's `lw; sll; sra; jal; move` argument path (the s32 `idx` carrier; volatile/cast/identity forms give 729-734 words). The inline area rounds the helper's locals to 8 bytes, so retail's root has 8 bytes fewer locals than the installed body (block-scoping hp/nexth/sizep/off does not shrink it) |

### func_8039842C: what the helper form establishes (new)
- The four retail stubs before/inside playerscr (95E14, 95E1C, 96ED8, 96EE0) are controlPakDamaged/controlPakFull and
  controlControls/controlWait inlined into updatePlayerScreen. Written as real helpers (bodies = the N64 case bodies),
  the root needs no unused locals: with `q` dropped the cfe locals area is 64 bytes (`spill.sh`: `AREA f_readnxtinst
  area=64`), exactly retail's (retail homes: s8 register home 188 = frame-4, reset-loop pointer 124 = -68 = temp0,
  &D_803BACE0[playerIdx] 116 = -76 = temp2). The seven unused locals and q of the installed form were standing in for
  the four inlined-call areas (8 bytes each).
- Remaining: our compile has 4 temp homes (frame 184), retail 5-6 (frame 192 = 104 + 64 + 24). Homes are given in
  expression-bit order (pretrace bits): 0 gSkipToGameState, 10 gNumPlayers, 14 &gJustPlayers[i] -> temp0;
  16 &gPlayers[i] (first loop) -> temp1; 17 gPlayers[i].save -> temp2; 53 &D_803BACE0[playerIdx] (split) conflicts only
  with web 10, so it lands in temp1 (retail temp2). Retail therefore has one more coloured expression web, numbered
  between 17 and 53, live in the main loop (so it conflicts with 10 and 53 but not with 16) - it takes temp1 and pushes
  53 to temp2; the extra home(s) also give the 192 frame. Compiled-out reads inside the loop (6 expressions x 2
  positions) restructure the loop instead (73-425 words).

## 4. What generalises
1. **Image A/B stubs are predictable**: game-called roots first, post-order of the call graph with callees from the
   last call site to the first (switch cases from the last case backwards: pakscr _AnimText case 4 first); then the
   rest of each file in SOURCE order, DFS from each procedure not yet emitted (this is why drawSelect precedes
   drawView's subtree in recordscr, why playerscr's draw* functions appear in file order, and why pakscr has
   _GetPakStatusStr's stub before _AnimText's subtree). `tools/objsyms.py` shows where our compile emits each helper's
   stub; with MAT3's call lists and file order that is the evidence for a claim. 33 stubs landed this way.
2. **Unexplained frame slots in native-led bodies are inlined calls.** umerge reserves 8 bytes per inlined call at the
   top of the callee's local area (even for an empty `void f(void)`), and the callee's locals are rounded up to 8.
   Writing the MAT3 static as a real helper removed disclosed unused slots/carriers in nine installed members (94058,
   9D05C holder, 990D0, 7918, 7620 3 of 4, 998C0 carriers, C140 carriers, BB80 half of its homes, AECE4's x/y homes
   and slot carrier) with every word still MATCH. Before adding unused locals to a function, check whether a
   caller-less stub sits next to it; unaccessed homes at the TOP of a local area are the classic sign.
3. **Unreferenced functions also become `jr ra; nop`** (probe: an unreferenced non-kept function in a group compiles to
   an 8-byte stub with its symbol; a static one without). In a single-file group they are emitted after the roots.
   trackscr's AE62C/AE634/AECC4/AECCC fit GC functions the N64 build no longer references (AnimOptText, animSlider,
   AnimZ, animFan) at their source-order positions; they are not claimed (their bodies cannot be verified).
4. Observation (not a rule yet): image A's units follow alphabetical source-file order: game-called roots are laid out in reverse file order
   (warningscr, trackscr, titlescr, startscr, selectcarload, recordscr, playerscr, pakscr, modescr, licensescr,
   circuitscr, cheatscr, carscr), then the data-referenced callbacks in forward order (carscr, cheatscr, circuitscr,
   A6A28 = licensescr's callback, modescr, pakscr, playerscr, recordscr, startscr, titlescr, trackscr, warningscr).
   The address-taken func_8039A24C (pakscr _CancelFunc, passed to ShowPakError) sits between modescr and licensescr.
5. `cfe` area arithmetic (`spill.sh` `AREA f_readnxtinst`) decides frame residuals precisely: 9842C's helper form
   reaches retail's area 64 exactly; 90DCC needs 116 + 7 homes; ShotUpdate needs 88. The three open spill-home
   near-misses (9842C, 90DCC, A9088) all need one more coloured expression web than the reconstruction has.
6. A volatile view stops IDO narrowing `(s16)u32_field` into `lh`, but retail's `lw; sll; sra; move` needs a real s32
   variable (the extra `move`).

## 5. Files and tools (lane dir)
- `groups/<group>/`: the 14 packets (sources + group.json with claims); `base/`: byte copies of every installed overlay
  group at fc6281e8b; `scores/<group>.txt`: scorer transcripts (plain, `--claims`, object order).
- `tools/objsyms.py` (builder side: compile a group like score.py, list .text symbols in object order with retail
  extents), `tools/full.sh PACKET ovl_x` (score plain + --claims + object order), `tools/gb.sh ovl_x DIR...` (batch
  summary), `tools/ownv.sh PACKET ovl_x` (local score.compare with own data verified against build/ovl/<x>/image.bin),
  `tools/ocmp.py OBJ NAME` (local compare of a listing-assembled object), `tools/mat3fns.py FILE.c [-a]` (MAT3 call
  lists in source order), `tools/mat3body.py FILE.c NAME...`, `tools/pkt.py`, `tools/allowun.py`.
- Variant trees: `v/rec` (recordscr), `v/pc` (playerscr uninit), `v/mi` (modescr init), `v/ci` (circuitscr),
  `v/pk`/`v/pr` (pakscr), `v/cu` (carscr), `v/tv` (trackscr animText), `v/ps` (9842C), `v/st` (streaks), `v/sh`
  (shots), `v/dead` (unreferenced-function probe), `9DCC/`, `91B00/`, `A9EEC/` (listing oracles), `A44FC/`.
- Builder scratch `~/rush2049/scratch/frontier/w38b` (toolkit reused from wtk; snapshots st_*).

## Permission denials
None.
