# w32d — control settings group / LoadTrack / SetupWaveModel / func_800E543C (wave 32)

Status: FINAL. Branch wave32 (main checkout); nothing committed, spliced or pushed. No permission denials.
Builder scratch watchman2:~/rush2049/scratch/frontier/w32d (toolkit reused from wtk), deleted at the end.
Flags `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3). Unit = `blob_unit --tag w32d` via `control_settings/run.sh FILE`.

**Landable: packet `groups/codex_control_settings_a4/` — 4 functions new to the cartridge, 2,684 B**
(control_cell_text 264 + control_cell_text3 820 + control_cell_text2 692 + func_800DD0C0 908) plus the two locked
members kept. `score.py group` MATCH for all six members, with and without `--claims`.

| Function | Bytes | Start (w31d) | Now | draft_check | Residual |
|---|---:|---|---|---|---|
| **control_cell_text2** (0x800DCDF4) | 692 | 17 rows | **group MATCH**, unit EQUAL | EQUIVALENT 90 calls (ghost_delete_fail) | — (no devices) |
| **control_cell_text3** (0x800DC99C) | 820 | 35 rows | **group MATCH**, unit EQUAL | EQUIVALENT 90 calls (pak_repair) | — (no devices) |
| **func_800DD0C0** | 908 | 22 rows | **group MATCH**, unit EQUAL | EQUIVALENT 90 calls (pak_nopak) | — (no devices) |
| **control_cell_text** (0x800DC88C) | 264 | EQUAL | **group MATCH**, unit EQUAL | (w28d EQUIVALENT) | — |
| func_800DCD58 / func_800DCCE0 | 156/112 | locked | group MATCH (verbatim) | — | — |
| control_settings | 2,552 | 634/638, rows 280, ops 82 | **636/638, rows 277, ops 56**, frame 1896/1896 (context in the packet) | EQUIVALENT 360 calls / 4 scripts | font-set return copies (3 sites), s0/s1 colour, temp ring, case order details |
| LoadTrack | 2,104 | 39/526 | unchanged (7 more ring hypotheses) | (w30d EQUIVALENT) | 1 code-free ring pop |
| SetupWaveModel / func_80097164 | 1,204 / 344 | 290/301 | unchanged (analysis only) | (w29d EQUIVALENT) | idx colour |
| func_800E543C | 700 | 180 rows | not attempted beyond reading (time) | (w28d EQUIVALENT) | — |

Bytes = words x 4 from the unit (66, 205, 173, 227 words).

## Integration notes (exact) — `groups/codex_control_settings_a4/`
Supersedes the locked group IN PLACE: same name; old members func_800DCD58, func_800DCCE0 kept with bodies verbatim
(diffed against src/blob/groups/codex_control_settings_a4/group.c at d1c53ab2: identical); old context
control_settings kept as context and `keep: [control_settings]` unchanged.
- members: func_800DCD58, func_800DCCE0, control_cell_text, control_cell_text2, control_cell_text3, func_800DD0C0
- claims: the same six.  context: control_settings, slot_state_setup, object_create, func_800B669C
- files: group.c, ctx_gr3_b.c (byte-identical copy of src/blob/groups/credits_scroll_grp/gr3_b.c; slot_state_setup's
  s2 convention — same precedent as func_8010B9C8; checked with cmp at d1c53ab2)
- unit_overrides force_internal += control_cell_text, control_cell_text2, control_cell_text3, func_800DD0C0 (IPA
  internal, only caller control_settings) and the non-member inlined helpers gfx_lock_DC, gfx_unlock_DC,
  font_set_DC, font_set1_DC, pair_set_DC (non-static, not kept, no stubs: retail has no stubs in 0x800DC88C..0x800DD4AC).
- No data-symbol changes. NOTE (rules): the members' only caller control_settings is a non-matching (EQUIVALENT)
  C draft in the same compile; its IPA conventions for all six callees equal retail's (the members are byte-exact
  in the group compile and in the unit). If the coordinator treats that as "stand-in caller" the packet is
  provisional — decision delegated to the coordinator, as briefed ("deliver whichever members MATCH with the rest
  as context").
Evidence (quoted):
```
score.py group cand/codex_control_settings_a4   (watchman2, 2026-10-14, also identical with --claims)
  func_800DCD58: MATCH / func_800DCCE0: MATCH / control_cell_text: MATCH / control_cell_text2: MATCH /
  control_cell_text3: MATCH / func_800DD0C0: MATCH
  Context (informational): control_settings 536/638 words differ; slot_state_setup 2/58; object_create 28/28; func_800B669C 5/5
blob_unit --tag w32d score control_settings control_cell_text control_cell_text3 control_cell_text2 func_800DD0C0
  func_800DCD58 func_800DCCE0 --with groups/codex_control_settings_a4/group.c --internal ... --neighbours
  EQUAL control_cell_text: 66 words / EQUAL control_cell_text3: 205 words / EQUAL control_cell_text2: 173 words /
  EQUAL func_800DD0C0: 227 words / EQUAL func_800DCD58: 39 words / EQUAL func_800DCCE0: 28 words
  locked bodies that differ in this unit: 0
draft_check --remote rocky control_settings group.c --scripts pak_nopak,pak_corrupt,pak_repair,ghost_delete_fail
  VERDICT: EQUIVALENT over 360 calls / 4 scripts (pak_nopak 90, pak_corrupt 90, pak_repair 90, ghost_delete_fail 90)
draft_check func_800DD0C0 --scripts pak_nopak: EQUIVALENT over 90 calls
draft_check control_cell_text3 --params sel1=s5,row=s6,text=sp+8,s1=sp+12,s2=sp+16,s3=sp+20,col=s7,sel2=s8: EQUIVALENT 90 (pak_repair)
draft_check control_cell_text2 --params sel=s5,row=s6,col=s7,a=sp+12,b=sp+16,text=s8: EQUIVALENT 90 (ghost_delete_fail)
regmap_verify control_cell_text3 's5=sel1,s6=row,s7=col,s8=sel2' --static-only: CONSISTENT
regmap_verify control_cell_text2 's5,s6,s7,s8=text' --static-only: REFUTED "s8 is claimed but the callee never
  reads it before writing it" (text2 passes `text` on as an outgoing stack argument; harness note for w32r/w32n)
```
The coordinator's w32r note (DD0C0 dialog divergence = harness clobber sets) is consistent: the drafts are EQUIVALENT.

## What closed the three callees (traced, one lever each)
1. **TB_SetJust takes ints** (GC `TB_SetJust(enum hJust, enum vJust)`, enums are 4 bytes): `pair_set_DC(s32, s32)`
   and `Pair {s32 first, second}` (D_80118E20 = the justification pair). The u32 version kept the `1` in its own web;
   s32 shares it with osRecvMesg's `li a2,1` (text3: retail `sw a2,0(v1)`). control_settings 280 -> 257 rows, text3 35 -> 24.
2. **text2/text3: separate `fh` and `x` locals instead of reusing parameters** (w28d reused `row`/`col`/`text`):
   the reuse swapped the IPA colours of sel1/row (s5/s6) and moved the cell pointer through v0. Then the **declaration
   order c, y, fh, x** places fh/y homes as retail (every named local takes a slot in declaration order: retail
   y at 48, fh at 44 in text3). text3 -> EQUAL, text2 -> EQUAL.
3. **func_800DD0C0: declarations x, y, fh, buf, big** (retail homes x 436, y 432, fh 428, buf 172) and **no cell
   pointer local**: with `c` the big-block `lb 1(s5)` got no `.noalias` against the `y` spill store, so as1 hoisted the
   lock arguments and used beqzl (uopt: a pointer to a global assigned in an earlier block gets no .noalias); indexing
   `D_80153FD8[row][col].f` directly fixes it, and the operand orders `x + w/2`, `y + (h - k)/2 + 20`, `y + h - fh*3`
   restore the temp ring (the expression form evaluates the opposite operand first). 22 -> 15 -> 2 -> EQUAL.

## control_settings (lever hour; best = `control_settings/best.c` = packet group.c)
Unit: `FAIL control_settings: 534 of 638 words differ; compiled body is 636 words, target 638` / `differing rows 277
(words); frame 1896/1896 | ops 56` (w31d: 634 body, rows 280, ops 82; the TB_SetJust fix alone: 633, rows 256, ops 59).
New this wave: (a) the TB_SetJust type (above); (b) the cell timeouts read the **volatile scheduler frame counter**
`D_8002E8E8.frameCount` (+0x27C; struct Sched as in matched func_800FD41C.c/display_settings.c, which writes the same
cell `deadline`): retail `lui; addiu; lw 636()` (body 633 -> 636, ops 59 -> 56; rows 256 -> 277 because the ring of
those 3 cases is now one off — the compare's operand load order: retail loads the cell timeout before the tick).
**Font-set frame contradiction measured** (small -O3 TUs on the builder, `ft/`): each umerge-inlined call costs exactly
8 B (buf[104]: 0/1/2/3 inlined `st()` -> frame 128/136/144/152; a 4-byte slack absorbs the first); a single-level
font_set (osRecvMesg + slot_state_setup + osSendMesg) is NOT inlined even at one site in a small TU (inlined in the
unit only at a single call site), the nested form costs 24 B/site. Retail frame below t9 (sp+128): `i` 124 + 24 B +
the two spill temps 92/88 => at most 3 inlined calls in the whole function, so four inlined font_set return copies are
impossible; the 3 remaining `move s0,v0` must come from something else (open: e.g. one inlined helper that contains
two font sets, or a caller-variable form).
**Lead (not best): `control_settings/lead_centerx.c`** — `i = (s16)(C.w / 2 + C.x); state_utility(i -
(object_manager_update(num, -1) >> 1), C.y, num);` (reusing the loop counter for the centre x; no frame cost) makes
retail's shape appear: the num buffer address is rematerialised (`addiu a0,sp,1816` x3, retail) and the centre x is
held in a callee-saved register across object_manager_update. Natural colours put i in s1 and the search-loop cell
pointer (p1 w155, save 20.5) in s0 (retail the reverse; i save 17.7 needs one more loop-weighted use). force.sh
`p1:w55=c14,p1:w155=c15` gives ops 46 (rows 323) — remaining: the three font-set copies, case-1 t1 buffer kept in s3,
case 5/7/8 load order, the 3 timeout cases' ring.
Devices (unchanged, disclosed in the file header): `if (row) {}` x2, `if (col) {}` in the search loop.

## LoadTrack (~25 min; stop rule: ~50 variants over w31d+w32d)
`start.c: FAIL LoadTrack: 39 of 526 words differ | words 40 ops 9 norm 9 | frame 168/168` (w31d best unchanged).
SCHED=1 ugt (lt0): ring allocations t6 at line 222 (gNumWindows), t7/t8 at 224 (second `D_80156994 ||
D_8014978C >= 6`), t9 at 238 (trackInited); retail t6 there, so one code-free allocation is missing in lines 225-237
(gRace tests + other.{loaded,ready,inited} stores). Tried (all 39/526 unless noted): `(... ) != 0` on the inner test
(147), `else { if (gRace == 4) }`, `D_8014978C > 5`, `(s8)`/`(s16)` casts on D_8014978C, `(s8)D_80156994`, `(u8)` (40).
GC (tracks.c LoadTrack) shows nothing between the gRace tests and the trackInited join (`.L800522D8`), and passes
`(short)track` to initTargetStructs. Next: an as1 narrowing on a *store* in that stretch (e.g. a store of a
computed s8 value), or the gRace value arriving through an inlined locked getter's return copy.

## SetupWaveModel (analysis only, ~15 min)
Retail j loop: three `lw X,0(s3)` loads of gRomObjDefPtrs[idx].ptr per iteration (s3 = &gRomObjDefPtrs[idx], CSE'd
from the prologue store, not PRE-hoisted), the bound through s7 = &gRomObjDefPtrs[n] (n at 68(sp)), `i*88` not
strength-reduced; GC's j loop additionally has RegisterObjName's result stored back and the HOODSHEEN/SHEEN/BRAKE
strstr arms, none of which exist on N64. No new source form for idx's depth-2 weight; w29d best.c and w31d notes stand.

## func_800E543C — not attempted this wave (time); w31d next step stands (code-free post-stunt occurrence of index).

## GC-derived names / layouts recorded this wave
- pair_set_DC / D_80118E20 = GC `TB_SetJust(hJust, vJust)` (TB_text.c): two 4-byte enum fields, set (1,1) before
  centred text, (0,3) after.
- control_settings cell `timeout` (+0x28) is compared with gScheduler (D_8002E8E8) frameCount (+0x27C), same as
  display_settings.c's `deadline` (not a GC derivation: from matched N64 siblings).
- font_set_DC = GC `HAL_SetFont(int fontID)` returning the old font (HAL_text.c) wrapped in the gfx lock.

## What generalises
- **Reusing a parameter as a local changes the IPA parameter colouring of internal functions** (text3: row reused
  as x swapped sel1/row between s5/s6); separate locals in the right declaration order fixed both colour and homes.
- **`.noalias` decides as1's cross-block hoist**: a cell pointer local assigned before an `if` block gives no
  `.noalias` against a stack spill inside it; indexing the global directly does (DD0C0).
- **Expression form vs pointer local flips operand evaluation order** — compensate with source operand order
  (`x + w/2`), not with the pointer.
- **Measured inline frame law**: each umerge-inlined call is an 8-byte, 8-aligned area (a 4-byte slack absorbs the
  first); a helper with 3-argument calls is not inlined at >1 site (and not at all in a small TU).
