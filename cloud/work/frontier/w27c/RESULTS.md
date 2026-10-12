# w27c — game results/menu pool (0x800FEE04–0x80105EA8), IDO 5.3 -O3

Status: final (wave 27). Builder scratch `watchman2:~/rush2049/scratch/frontier/w27c`; unit tag `w27c`.

| function | bytes | state | scorer line |
|---|---|---|---|
| func_80100B8C | 408 | **MATCH in group** (superseding credits_scroll_grp) + EQUAL in unit; devices disclosed | group: `func_80100B8C:` `MATCH` (score.py group cand/credits_scroll_grp); unit: `EQUAL func_80100B8C: 102 words (kept, c_v3.c)` / `locked bodies that differ in this unit: 0` |
| func_80100D5C | 252 | EQUAL in unit, provisional; regmap CONSISTENT | `EQUAL func_80100D5C: 63 words (internal, c_s1.c)` |
| func_80101904 | 1,152 | EQUAL in unit with its real caller 221C, provisional (221C/F30 unmatched); devices | `EQUAL func_80101904: 288 words (internal, c_s1.c)` |
| func_80101D84 | 1,176 | same; draft_check EQUIVALENT (stunt_end, 690 calls) | `EQUAL func_80101D84: 294 words (internal, c_s1.c)` |
| func_80102448 | 1,336 | draft, colour tie + 2 schedule spots | `FAIL func_80102448: 18 of 334 words differ` (us.sh `words 16 ops 8 norm 8 \| frame 376/376`) |
| func_80102980 | 1,456 | draft, structure right, colouring off | `FAIL func_80102980: 325 of 364 words differ; compiled body is 372 words, target 364` |
| func_80100E58 | 2,732 | draft, frame exact | `FAIL func_80100E58: 633 of 683 words differ; compiled body is 680 words, target 683` (`words 267 ops 28`) |
| func_8010221C | 548 | draft (Astra root.c fixed) | `FAIL func_8010221C: 96 of 137 words differ; compiled body is 133 words, target 137` (`words 52 ops 12`, frame 24/56) |
| highscore_entry_anim | 188 | NOT A FUNCTION: tail of unregistered func_80104704 (0x80104704–0x80104B14, frame 136, called from func_80105480 +0x2B0) | — |
| game_results_update/render/input/exit, func_80100564, func_80102F30, func_80103D28, func_80104B14, func_80105480, func_80105EA8 | — | not started (time) | — |

## func_80100B8C (408 B) — EQUAL in the unit
`python3 -m tools.conveyor.pipeline.blob_unit --tag w27c score func_80100B8C --with cloud/work/frontier/w27c/func_80100B8C/best.c --neighbours`
→ `EQUAL func_80100B8C: 102 words (kept, c_v3.c)` / `locked bodies that differ in this unit: 0`.
- Prior body: the m2c-era context copy in locked `credits_scroll_grp/gr3_d.c` (48/102 words off, frame 104/144).
  Fixes: `dispatch_handler(1)` only in the else arm; frame residual; `== 1U`; D_80118E20 defined.
- Devices (header): unused `char buf[40]` = exact frame residual (retail 144, without 104; 10 words at the top,
  inlined font_set areas at sp+60/84 already coincide). Ten unused `s32` scalars also give EQUAL (v/v5.c) if the
  array form is ruled a "pad array". `1U` shares the u32 1 web of the inlined `func_800B669C(1,1)`; defining
  `Pair D_80118E20;` gives the shared `lui at` for the two tail stores.
- Not standalone: slot_state_setup takes its argument in s2 (IPA), so `score.py fn` cannot match it. Integration:
  supersede `credits_scroll_grp` (same name, member list + func_80100B8C) replacing gr3_d.c's context copy with
  this body, and give D_80118E20 a definition (data-symbol change; check blob_group handles a defined 8-byte object
  at 0x80118E20). The unit run used `--with` (my definition replaced gr3_d.c's).

## func_80100D5C (252 B) — EQUAL in unit with stand-in callers (provisional)
Fill-rectangle leaf: pipe sync, SetPrimDepth(0x10,0), SetPrimColor from `u8 color[4]`, combine PRIM/PRIM,
FillRectangle(x, y, x+w, y+h); `D_8014A248 = -1`. Prototype `(s32 x, s32 w, s32 y, s32 h, u8 *color)`;
IPA registers x=t1, w=t2, y=t3, h=t4, color=a2. Mapping from retail call sites (func_80100E58 +0x18c..:
`addu t1,s8,s1` column x, `li t2,60` width, t3 = row*14+36, `li t4,14` height, `move a2,s0` = &rgba on stack;
the same t1,t2,(t3),t4,a2 set-up order at all 10 sites). Unit: `--internal func_80100D5C --keep standin_80100D5C`
(three stand-in call sites; one site gets it inlined). Becomes a match with its real callers (func_80100E58,
func_80101904, func_80101D84, func_80102448, func_80102980, func_80102F30).

## Results-table family: func_80101904 / func_80101D84 (+ func_80100D5C) — EQUAL in unit, provisional
Source: `fam/fam_best.c` (one file: func_80100D5C, func_80101904, func_80101D84, static helpers, and a kept
stand-in `standin_1904` that calls them twice so umerge does not inline them). Start: Astra's research packet
`dot_results_callbacks_101904_research_20261007` (first.c/second.c, 32/288 and 36/294 there).
Command: `blob_unit --tag w27c score func_80101904 func_80101D84 func_80100D5C --with fam/fam_best.c --internal
func_80101904 --internal func_80100D5C --internal func_80101D84 --keep standin_1904 --neighbours` →
`EQUAL func_80101904: 288 words` / `EQUAL func_80101D84: 294 words` / `EQUAL func_80100D5C: 63 words ...; stub tail:
6 words of deleted-procedure stubs follow (NOT as in the image)` / `locked bodies that differ in this unit: 0`.
Fixes over the packet: osJamMesg→osSendMesg (rename); func_80100D5C prototype (x, w, y, h, color);
case 2 stores r then g; the else-arm colour init on four lines (as1 order); frame layout solved (below).
Devices (all to disclose in headers):
- Font switches: retail keeps `move s0,v0` (the old font) at every site. 1904 and D84's pre-loop/end-of-cell
  sites are written out (`osRecvMesg; previous = slot_state_setup(n); osSendMesg;`) with a compiled-out
  `if (previous) {}` holder; D84's col-0 and header sites use the inlined static `font_set` (gfx_lock/gfx_unlock
  statics), whose 2x24 B inline areas are exactly D84's extra bottom-of-frame bytes. 1904 has no room for any
  inline area (bottom 32 B), so its sites cannot be the inlined helper.
- Frame: top of frame `s16 x; s32 previous; s16 y;` (x, y used for the column-0 label; 16 B above `text`), the
  function-level `row,column,total,other` group (4 words between `columns` and `color`; `other` unused in both),
  and one block-scoped `s32 i` (1904: the abs-sum loop index in the data block; D84: unused, in the column-0
  block) = the 8 B below `label`. Retail 360/392; each removal measured (`other` out: 352; no block var: 352/384).
- Buffers `char value[40]`, `label[212]`/`[204]` are Astra's sizes, fixed by the next live stack objects.
Packaging: static helpers leave 6 words of deleted stubs after func_80100D5C — make gfx_lock/gfx_unlock/font_set
non-static and internal (w17j). Becomes a match only with the real callers in one group: func_8010221C (draft
below) is the only caller of 1904/D84/E58, called once from func_80102F30 (kept, 3,560 B, unmatched).

### Prototype correction (applies to the whole family)
func_80100D5C is `(s32 x, s32 y, s32 w, s32 h, u8 *color)`: func_80102448 retail sets t3 (y) before t2 (w)
(`move t3,s6` / `li t2,60` order at +0x168), and ugen evaluates IPA register arguments in prototype order (w26d).
With this order 1904/D84/D5C stay EQUAL and 2448 drops from 37 to 16 rows. regmap_verify (rocky):
`VERDICT: CONSISTENT: the claimed registers are exactly the live-in set` for t1=x,t2=w,t3=y,t4=h,a2=color.

### func_80101D84 behaviour
`python3 -m tools.recomp.draft_check --remote rocky func_80101D84 cloud/work/frontier/w27c/fam/fam_best.c --scripts
reached --callee 's32 slot_state_setup(s32 font)=s2/s0,s1,s3'` → `VERDICT: EQUIVALENT over 690 calls / 1 scripts
(stunt_end 690)`. Without the `--callee` shim the draft crashed (o32 call of slot_state_setup); with a static
copy of slot_state_setup it diverged inside audio_frame_sync (v0 register parameter) — both harness gaps.
slot_state_setup clobbers s0, s1, s3 (no saves; s2 = argument), which is why callers reload s3 and spill s1.

### func_80102448 (16 words) — the per-player records table (13 rows x players+1 columns)
Rows: names header, two countdown_state.fixed rows (130, 213), then 10 lap rows (`strings[indices->records +
row - 3]`); cells `%d` of D_80152038 player stats (words[5], words[2], s16 laps[row-3] at +72). Row 3 gets an
8-px separator bar of width columns*60. Layout: rows above row 3 shift by 7. Residual (ctrace): webs w26 (x =
column*60) and w116 (160 - width/2) tie at save=125/numintf 27, the lower web number takes s0; retail has them
the other way round (x in s1). force.sh with `p1:w26=c15,p1:w116=c14` fixes those rows. Not found: a source
order that flips the tie (tried named left before/after x, decl swaps, compiled-out read, expression-only x).
Plus the `sll s0,s5,2`/`move s2,s8` delay-slot order in the row-label arm.

### func_80102980 (181 words) — the head-to-head matrix (players+2 rows x players+1 columns)
Diagonal cells (column+1 == row) use 64-intensity colours and dispatch_handler(4). Two name buffers (sp+208,
188 B; sp+64, 144 B). Residual: retail keeps the constant 1 in s8 (`beq v0,s8`, `move a0,s8`, `move a2,s8`) and
columns in s7, recomputing &color/&value with `addiu a2,sp,396` at each use; ours colours &color/&value
(w194/w181, type 1) and splits the 1-web (w201, save 21.65 < callee cost). Next: find why the 1-web is smaller
in ours (separate 1-webs w184/w189/w210 exist) — likely a type split between the s8 switch compare / s32 uses.

### func_80100E58 (267 words, frame exact) — the race results table (players+1 rows x 3/4 columns)
Columns = 3 when D_801543CA == 1 or (gameplay_mode == 3 && D_80152907[D_801543D4].flag == 0), else 4.
Times are `fcvt(value, "%d:%02d.%03d", (u32)(t/60.0f), (u32)t % 60, (u32)(t*1000.0f) % 1000)` from
D_80152908[p].best (when D_80144018[p] == D_80152734) and D_80144DA8[p] (when D_80144018[p] > 0), else
fixed[215]; column 3 = strings[D_80152906[D_8014AA16[p].car].lap + indices->h108]. Frame: six in-loop font
switches are the inlined font_set (24 B each), the column-1 'best' switch is written out (frame 528 exact).
Residual: head (retail reads D_801543CA through `addiu t8; lh t9,0(t8)` and stores rows first), the x0/x
colour swap as in 2448, loop-1 row kept in v1 (ours a0).

### func_8010221C (52 words) — results menu root (only caller of 1904/D84/E58; called once by func_80102F30)
Fixes over Astra's root.c: the selected-item global is D_801391E0 (`lui 0x8014; addiu -28192`), not D_801491E0;
x is s32 (u8 kept in s6, sign-extended at each call), y s16. Residual: uopt hoists &gameplay_mode into s8
(loop compare + final switch), which pushes x to the stack (frame 56 vs 24); retail reloads it with `lui` in the
item==0 arm. Tried volatile reads and a switch form, no change. The `move s0,v0` copies are not reproduced by the
holder here (they are in 1904/D84). Own .rodata: 7-entry jump table at 0x80124848.

## Integration notes
- **func_80100B8C**: install `cloud/work/frontier/w27c/credits_scroll_grp/` (same name, supersedes the locked group;
  every old member kept, func_80100B8C moved context→members, `"claims": ["func_80100B8C"]`). `score.py group` on the
  builder: credits_scroll MATCH, func_80100B8C MATCH (context bodies print their usual diffs). Data-symbol change:
  gr3_d.c now DEFINES `Pair D_80118E20;` (8 B at 0x80118E20) — check blob_group/blob_unit data handling (the unit
  run with `--with` placed it correctly, 0 locked bodies differing). Devices in the header: unused `char buf[40]`
  (exact frame residual 144 vs 104; ten unused s32 scalars also EQUAL if the array form is ruled a pad array) and
  `== 1U`. No unit_overrides needed (func_80100B8C was already kept).
- **Family (D5C/1904/D84)**: not installable until func_8010221C and func_80102F30 (kept, 3,560 B) match; then one
  group: F30 (keep), 221C, E58, 1904, D84, 2448, 2980, D5C (all internal). Make gfx_lock/gfx_unlock/font_set
  non-static internal helpers (static ones leave 6 deleted-stub words after D5C).
- **Inventory**: highscore_entry_anim (0x80104A58) is mid-function; the real head is 0x80104704 (after the
  caller-less `jr ra; nop` at 0x801046FC), already listed in cloud/work/unregistered-heads.md. Needs
  refresh-game-context before anyone drafts it.
- **Tree issue seen at the end of the run**: after HEAD moved to b4e8f39a, `asm/us/blob/symbols.json` sha256
  (89ee4b7a…) no longer matches `asm/us/blob/SHA256SUMS` (4cdb9e30…), so `score.targets()` (us.sh/udiff, tdis,
  score.py) refuses to run on the Pi; blob_unit still scores. Builder scratch keeps the older consistent asm.

## What generalises
- The results screens share one template: a fill-rect pass (func_80100D5C) then a text pass with font switches
  (osRecvMesg / slot_state_setup(n) in s2 / osSendMesg). Whether a switch is the inlined `font_set` static or
  written out is decided by the frame: each inlined font_set adds 24 B below the named locals.
- Frame accounting that worked: named function-level locals top-down (`x, previous, y` = 16 B above `text`; the
  4-word `row,column,total,other` group), then block-scoped locals (8 B), then inline areas, then spill homes.
- func_80100D5C argument order (x, y, w, h) came from ugen's prototype-order evaluation (t3 before t2).
- draft_check on a function that calls slot_state_setup needs
  `--callee 's32 slot_state_setup(s32 font)=s2/s0,s1,s3'`; without it the draft crashes.
- `x = column*60` against `160 - width/2`: the two webs tie, and the lower web number takes s0. Retail has them
  the other way round in all three tables (2448, 2980, E58). This is the next lever to find.
