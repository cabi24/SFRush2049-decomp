# w31b RESULTS (wave 31, menus, IDO 5.3 -O3)

Status: FINAL (wave 31). Builder scratch `watchman2:~/rush2049/scratch/frontier/w31b`; unit tag `w31b`.

| Function | Bytes | Before -> after | draft_check | Residual class | Deliverable |
|---|---:|---|---|---|---|
| func_8010AEAC + func_8010A8D0 + func_8010A7A4 (GC optionscr animText/animButtonText/printOption) | 3,452 | packaged into credits_scroll_grp: 13/13 code-identical (AEAC NOT VERIFIED own .rodata), unit 3/3 EQUAL; **blob_group proof still REFUSES** (same text as w30i) | EQUIVALENT (w28c) | packaging/tools: needs the jump-table-aware independent-regions rule (demonstrated: 13/13 and 3/3 image-identical) | packet `w31b/groups/credits_scroll_grp/` + `w31b/tools/indep_tables_proof.py` |
| func_8010E8B4 (GC pausescr initScreen arm) | 340 | unit 3/85 -> 3/85 (14 more variants) | EQUIVALENT (w29b) | narrowing register + `y + D2` operand order (as1/ugen) | `w31b/func_8010E8B4/best.c` (= w30b best); no physics_sym packet |
| func_800DA2C0 (GC optionscr updateButtons) | 2,316 | 489/579 ops 78 frame 152/160 WITH pads -> **202/579, words 88, ops 5, frame 152/120, NO pads** | **EQUIVALENT** 961 calls (unit pair, options_menus) | colour (s6/s7/s8 permutation) + one stack reload + frame 32 B | `w31b/func_800DA2C0/best.c` (pair file) + `dc.args` |
| func_800DB1E0 (GC updateOptionScreen) | 1,400 | unit EQUAL (unchanged) with the new DA2C0 internal | **EQUIVALENT** (same run) | blocked on DA2C0 | same pair file |
| func_8010EA08 + func_8010F218 (GC videoscr animButtonText/animText) | 2,056 + 2,444 | not drafted (time) | — | — | call map below |

Matched bytes this lane: 0 new (the 3,452-B unit is code-identical but uninstallable until blob_group gets the rule in §1).

## 1. func_8010AEAC / func_8010A8D0 / func_8010A7A4 repackaged into credits_scroll_grp — packet ready, STILL REFUSED by blob_group without a tools change

Packet `w31b/groups/credits_scroll_grp/` (supersedes in place): the installed group's 12 .c files byte-identical (cmp),
`gr3_aeac.c` = byte-identical copy of `w30b/groups/func_8010AEAC/faeac.c`; members = the 10 installed members (the
installed group has 10, not 11) + func_8010AEAC, func_8010A8D0, func_8010A7A4 (13); claims = the three; keep += func_8010AEAC;
context and flags unchanged (`-Wab,-r4300_mul`); provenance appended.

- Builder `score.py group cand/groups/credits_scroll_grp` (rc=1, only AEAC's owndata line): credits_scroll, func_80100B8C,
  slot_state_setup, world_trigger_check, func_8010A53C, func_80107EDC, animLapTime, animElapsedTimerText, animPlace,
  func_80108AB0 `MATCH` (func_80108AB0 `own .bss placed at 0x8013F2F8`); `func_8010A8D0: MATCH` (`own .rodata verified at
  0x801248D8..0x8012491C`); `func_8010A7A4: MATCH` (`own .rodata verified at 0x801248D4..0x801248D8`);
  `func_8010AEAC: NOT VERIFIED (4 section-relative relocations unverified: .rodata+0x0 at +0xd0, .rodata+0x0 at +0xe8,
  .rodata+0x5c at +0x208, .rodata+0x5c at +0x210; own .rodata: this function's references disagree on the section's image
  address (0x80121018 for +0x0, 0x801248C0 for +0x5c): the literals are not laid out as in retail)`. `--claims`: same three lines, rc=1.
- Unit: `blob_unit --tag w31b score func_8010AEAC func_8010A8D0 func_8010A7A4 --with .../credits_scroll_grp/gr3_aeac.c
  --internal func_8010A7A4 --internal func_8010A8D0 --internal gfx_lock_AEAC --internal gfx_unlock_AEAC --internal
  font_set_AEAC --internal TB_FlashTextColors_AEAC --internal TB_SetJust_AEAC --internal TB_CenterText_AEAC --neighbours`
  -> `EQUAL func_8010AEAC: 413 words (kept, c_gr3_aeac.c)`, `EQUAL func_8010A8D0: 375 words (internal, c_gr3_aeac.c)`,
  `EQUAL func_8010A7A4: 75 words (internal, c_gr3_aeac.c)`, `locked bodies that differ in this unit: 0`, `3/3 equal`.
- **The splice proof itself** (read-only replay: object built with `blob_group.builder_script` stages in my builder
  scratch, then `blob_group.group_bodies` run locally on it — the exact code `install_group`/`blob_group splice` runs; control:
  the installed credits_scroll_grp through the same harness -> `group_bodies OK`, all 10 members 0 word diffs):
  `GroupError: func_8010AEAC: own .rodata: this function's references disagree on the section's image address (0x80121018
  for +0x0, 0x801248C0 for +0x5c) ...; independent regions refused: .rodata: relocated words (a jump table) need the one-base
  or jump-table proof [per-reference placement tried because the whole-section placement was refused: .rodata: unmapped data
  between jump table entries]` — the SAME refusal as w30i's standalone install.

### Why no choice of siblings can fix it (retail .rodata order)
- `0x801248C0` is not a literal of AEAC: it is the section base owndata derives from the jump-table reference
  (+0x5c -> table at **0x8012491C**, 14 entries, 0x8012491C..0x80124954). AEAC owns exactly two retail windows: the string
  "CONTROLLER %d %s" at 0x80121018..0x8012102C and that jump table. func_80108AB0's literals (0x801248C4..CC) are NOT between
  AEAC's data; the literal run 0x801248B4..0x80124958 is simply in TEXT order: func_80106D94 B4..C0, func_80108AB0 C4..C8,
  func_80108F40 CC..D0, func_8010A7A4 D4, func_8010A8D0 D8..E4 (+ its table to 0x8012491C), func_8010AEAC table 91C..954,
  func_8010C974 954, func_8010D3C0 958.
- The STRING region is in SOURCE-FILE order instead: 0x80121004 time_result_display (optionscr _TitlePane), 0x80121018
  func_8010AEAC, 0x8012102C..38 replay_save_prompt (optionscr initScreen), 0x80121048..58 func_800DA2C0 (updateButtons), then
  0x80121060.. control_settings (controlscr.c), 0x801210B0.. voice_stop, 0x801210CC.. func_8010B5D0, 0x801210E8 stunt_combo_display.
  So AEAC's two windows are 0x3904 bytes apart in retail, in two different orderings: no group object can put them at one
  base (it would need every string after 0x80121018 and every literal/table before 0x8012491C as members, byte-exact).
  The "same source file" siblings, for the strings, are optionscr.c's (time_result_display, replay_save_prompt,
  func_800DA2C0, func_800DB1E0), not hud.c's; for the literals there is no file grouping at all.
- `_independent_regions` (tools/conveyor/pipeline/blob_group.py) refuses ANY .rodata section that carries relocations
  (`if elf.relocations(index): raise ... "relocated words (a jump table) need the one-base or jump-table proof"`), and
  `_jump_table_windows` needs the section to be table entries only. AEAC needs both: a string window and a table window.

### The tools change that lands it (demonstrated, not applied)
`w31b/tools/indep_tables_proof.py GROUP_DIR OBJ_DIR` monkeypatches `_independent_regions` in memory only: a window may
hold relocated words when every one is an R_MIPS_32 entry into THIS function's own text, and the window equals the
image after each entry becomes `function image address + (target - function start)`; all other rules are kept
(whole-extent compare, padding, access width, one opaque run, no shared image bytes). Results:
- credits_scroll_grp packet: `[func_8010AEAC] .rodata+0x0..+0x14 == retail 0x80121018..0x8012102C`,
  `[func_8010AEAC] .rodata+0x5c..+0x94 == retail 0x8012491C..0x80124954 (14 table entries relocated)`, `13/13 members image-identical`.
- w30b's standalone group `func_8010AEAC` (unchanged): same two windows, `3/3 members image-identical`.
So once blob_group gets this rule (a tools/ change: coordinator/integration lane), EITHER packet installs; the smaller
w30b standalone group is enough. The credits_scroll_grp repackaging buys nothing for the proof.

Integration (either packet): unit_overrides force_internal += func_8010A7A4, func_8010A8D0, gfx_lock_AEAC, gfx_unlock_AEAC,
font_set_AEAC, TB_FlashTextColors_AEAC, TB_SetJust_AEAC, TB_CenterText_AEAC (w30b: without the first two the unit is 0/3).
Data: gr3_aeac.c declares D_801146BC as `Color D_801146BC[2]` (GC flashColors[2]); no definition. No symbols renamed.
GC names (w30b, optionscr.c / TB_text.c): animText, animButtonText, printOption(x, y, item, name, value), TB_FlashTextColors.

## 2. func_8010E8B4 (= GC pausescr.c initScreen arm, NOT optionscr) — no change, 3/85
Identity note: E8B4 sets D_8011AD38 (= physics_sym's `inited`) and D_8011AD68 (the pane), and calls TitlePane =
the PAUSE screen's _TitlePane (physics_sym group) — so it is pausescr.c's initScreen arm; GC optionscr initScreen
has the same shape (`y = TB_GetFontLineHeight()*2 + 10` with `signed short y`, then InitPane(.., y-4, .., y + .. + 4)).
Tried (unit, `us.sh --internal TitlePane`, all on w30b best.c): `(s16)h` at both uses and no y (3), `y = h = ...` (3),
holder `if (h) {}` after the call (45, body 87) / before it (19), `(s32)y + D2` (3), `(y + 0)` (3), `(y ^ 0)` (3),
`(y + 4) + D2` (3), `D2 + y + 4` (3), `4 + y + D2` (3), `y + (D2 + 4)` (3), `y - -D2 + 4` (40), `(s16)(y + D2) + 4` (41),
`y = h; h = y;` (9). Every form that keeps one s16 value gives the same 3 words: no source-level identity flips
the `addu a3,t7,t0` operand order (so it is not uopt's operand-kind rule — w30b's listing proof already showed the
y-first order needs y in its own coloured register before as1). Next lever (unchanged): a source where the
unnarrowed h stays live past y's definition WITHOUT an extra use (e.g. y and h both live into an inlined helper with
parameters (s32 h, s16 y)), checked with ugt.sh on the narrowing sequence. physics_sym packet NOT delivered (E8B4
not matched); the `inline_blockers` TitlePane entry stays.

## 3. func_800DA2C0 + func_800DB1E0 (GC optionscr updateButtons + updateOptionScreen) — pair draft, pads removed
`w31b/func_800DA2C0/best.c` defines both (DB1E0 unchanged from w30b). Unit:
`blob_unit --tag w31b2 score func_800DB1E0 func_800DA2C0 --with cloud/work/frontier/w31b/func_800DA2C0/best.c
--internal func_800DA2C0 --neighbours` -> `EQUAL func_800DB1E0: 350 words (kept, c_best.c)`, `FAIL func_800DA2C0: 202 of
579 words differ; +0x8f4: .bss+0x4: ...` (the .bss line is a shifted-row artefact of the 32-B frame difference;
us.sh: `words 88 ops 5 norm 5 | frame 152/120`), `locked bodies that differ in this unit: 0`.
Path (us.sh `--internal func_800DA2C0`, variants in `w31b/func_800DA2C0/v/`):
| step | change | words / ops / frame |
|---|---|---|
| a0 | w30b pair (w10e pads) | 462 / 78 / 160 |
| a1 | rewrite on the func_800B59F0 twin, GC local order, no pads, speeds through globals | 455 / 78 / 128 |
| a2 | **GC cursorSpeed$472/scrollSpeed$473 as function-local `static f32` (D_8015273C/D_80152740)** | 415 / 54 / 120 |
| a6 | sel_y computed before the -160/225 stores (GC order) | 346 / 38 / 120 |
| b1 | second spelling of 255 in the alpha ramp (`* 255`): 12.566371f hoisted to $f30 as retail | 243 / 28 / 120 |
| c1 | `[i+14].alpha = [i].alpha`; test `[i].alpha == 0` | 229 / 26 / 120 |
| d3 | DEVICE `if (yi) {}` after `yi -= 40.0f` (ctrace: yi web 1.50 lost s8 to the &D_80116D10 address web 1.55) | **88 / 5 / 120** |
Also tried: D_80116D10 defined in the file / `volatile` (no change), holder at other places (d1/d2 108, f1 111, f2 184,
f3 111, e3 114), chained or by-union D_8012E700 stores (e1 = d3; e2 162).
Retail per-use reloads: D_8015273C/D_80152740 = the static lever (one load, stores at each arm exit, `lui at` per
use); D_80116D10 = per-use `lui/lw` once yi takes s8 (no address register left).
Residual: (1) yi in s6 / &B[i].pos s7 / &validOption[i] s8 vs retail s8/s6/s7 (the holder raises yi above the two
pointer webs; a weaker extra use is the next test); (2) the colour word is reloaded from the stack for the second
D_8012E700 store (retail `lw v0,96(sp)` once; alpha in v1); (3) frame 120 vs 152: every named slot keeps retail's
relative layout (old 124, flag 119, texIdx 102, color 96 in retail = ours -32), retail has 32 B more below `color`
with no stack traffic (8 split-web homes or block locals) — not padded. `u0` is one unused named local inside the
block (disclosed). Devices: `if (yi) {}`; statics named by their retail addresses (`static f32 D_8015273C`,
`static f32 D_80152740`).
draft_check: `python3 -m tools.recomp.draft_check --remote rocky --unit func_800DB1E0,func_800DA2C0
cloud/work/frontier/w31b/func_800DA2C0/best.c --scripts options_menus` -> `VERDICT: EQUIVALENT over 961 calls / 1 scripts
(options_menus 961)`, unit members func_800DB1E0 961, func_800DA2C0 0 (inside the unit), register shims for
func_800DA0BC/replay_save_prompt/time_result_display; the two `static f32` bound by their D_ names (no --statics needed).
Integration when it matches: pair group in the physics_sym style (members func_800DB1E0 kept + func_800DA2C0
internal: force_internal func_800DA2C0); own .bss: the two statics at 0x8015273C/0x80152740 (drop any extern-data
definition of them; none exists in src/blob). func_800D816C/func_800D91A0 untouched.

## 4. func_8010EA08 + func_8010F218 (GC videoscr.c animButtonText / animText) — not drafted
Call-sequence alignment (N64 jal order vs GC bl order) gives the callee identities for the draft:
render_helper = HAL_SetTextDepth; osRecvMesg/slot_state_setup/osSendMesg = HAL_SetFont (the lock/font_set helper
family, s2 = 13 font); dispatch_handler = TB_SetTextColor; brake_light_update = ZOID_ProjectWindow;
object_manager_update + `x - (w>>1)` = TB_CenterText (inlined, TB_GetTextWidth); state_utility = TB_DrawText;
func_800ED66C = HAL_SetTextTranslucency; fcvt_wrapper = TB_Sprintf ("%s%d"); camera_zoom_fov = TB_GetTextNumLines;
object_bytes_sum_global = TB_GetFontLineHeight; ambient_sound_set = InitPane; camera_auto_follow =
TB_DrawTextScrollBox; sound_handles_array_clear = RemovePane. EA08 follows GC animButtonText except that N64 drops
one TB_DrawText after the second sprintf pair; F218 follows GC animText minus drawColorBars (F218 calls EA08 first;
TB_RightText inlined as object_manager_update). EA08's entry `sh 160` x2 store 0x801569B0/0x80156988 (GC
width$636/height$637); EA08 is F218's internal callee (unsaved s0-s8). Next: draft from GC animButtonText/animText
with `--statics`.

## Permission denials
None.
