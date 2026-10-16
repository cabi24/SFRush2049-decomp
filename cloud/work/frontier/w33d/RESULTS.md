# w33d — control_settings / SetupWaveModel + func_80097164 / func_800E543C / CreatePlayer / pak_delete_file_retry (wave 33)

Status: FINAL. Branch wave33 (main checkout); nothing committed, spliced or pushed.
Builder scratch watchman2:~/rush2049/scratch/frontier/w33d. Flags `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3).

| Function | Bytes | Start | Now | Residual |
|---|---:|---|---|---|
| **control_settings** | 2,552 | 636/638, rows 277, ops 56 (w32d) | **group MATCH** (7/7 members, with and without `--claims`), unit EQUAL | — (one disclosed `if (row) {}`) |
| SetupWaveModel / func_80097164 | 1,204 / 344 | 290/301, rows 233 / 10/86 | unchanged (1 lever: parameter order, no effect) | IPA colour: first takes s8, idx splits |
| func_800E543C | 700 | 180 rows | not worked (blocked: unit_blockers func_800C4F68, stunt_combo_display unmatched) | — |
| CreatePlayer | 1,088 | lead 256/272, rows 120, ops 43 | unchanged (1 lever: inlined name/extension helper — not inlined) | i colour (a3 vs s0), attempt copy in a3 |
| pak_delete_file_retry | 784 | best 98 rows; lead_q3 105 | unchanged (1 h: callback local / helper / goto loop) | callback colour (s0 vs retail t0), pfs split |

Permission denials: none so far.


## control_settings — CLOSED (group packet `groups/codex_control_settings_a4/`)
Evidence:
```
blob_unit --tag w33d score control_settings control_cell_text control_cell_text3 control_cell_text2 func_800DD0C0
  func_800DCD58 func_800DCCE0 --with <best.c, helpers *_DC renamed *_DQ> --internal <4 callees + all helpers> --neighbours
  EQUAL control_settings: 638 words (kept, c_w33d_cand.c)
  EQUAL control_cell_text: 66 words / EQUAL control_cell_text3: 205 words / EQUAL control_cell_text2: 173 words /
  EQUAL func_800DD0C0: 227 words / EQUAL func_800DCD58: 39 words / EQUAL func_800DCCE0: 28 words
  locked bodies that differ in this unit: 0
  blob_unit score: 7/7 equal
  udiff: want 638 words, got 638; differing rows 2 (words); frame 1896/1896; unverified 2 unresolved 0
    (the 2 rows are the jump-table `lui at/lw t6,17100(at)` relocation pair: own .rodata, unverified in the unit)
score.py group cand/codex_control_settings_a4 (watchman2; identical with --claims, rc=0)
  func_800DCD58: MATCH / func_800DCCE0: MATCH / control_cell_text: MATCH / control_cell_text2: MATCH /
  control_cell_text3: MATCH / func_800DD0C0: MATCH / control_settings: MATCH
  Context (informational): slot_state_setup 2/58 words differ; object_create 28/28; func_800B669C 5/5
```
Run script: `control_settings/run.sh FILE` (renames `_DC` helpers to `_DQ` on the fly so the candidate does not collide
with the locked group.c's helper copies; `--internal` for every helper).

### What closed it (traced; one lever at a time, rows 277 -> 167 -> 131 -> 111 -> 19 ops -> 6 ops -> 0 ops -> EQUAL)
1. **The frame is GC-style inlined dialog helpers, each with `u8 str[256]`.** The retail buffers at sp+1520/1192/920/
   656/392/128 had sizes 296/328/272/264/264/264 in every earlier draft; they are 256-byte helper buffers plus the
   helpers' inline areas. The merged-ucode Mmt dump (`control_settings/lay.sh`, w12h udump) shows per-variable
   offsets; with helpers (row, col) for the case-1/3/4/15/6/9 text, cs_wait11/12/13 for the timeout cases and
   cs_ctl10(Cell *) for the case-10 input handling, every retail buffer offset and the col home (1892) land exactly.
   cs_ctl10 with (row, col) params instead of `Cell *` broke the colouring (ops 138).
2. **pair_set_DC without the `Pair *p` local** (w30d's animPlace lesson): the pointer local costs 8 B of inline area.
3. **The header's two `move s0,v0` copies + TB_CenterText.** A return copy of an inlined font set needs a helper that
   returns a local AND contains a nested inlined call (measured in small TUs: `fs` keeps it, `sfo`/`fsC`/`return f()`
   do not); except that a single-level helper inlined at ONE site also keeps it (fsS_DC). Header = fsS_DC (8 B) +
   fsL_DC (gfx_lock nested, 16 B) + TB_CenterText_DC (GC `s16 TB_CenterText`; gives retail's centre x in s0 and the
   s16 return copy `move a0,t8`). A single-level helper used at two sites is not inlined; mixed shapes need the
   one-line `old = ...; osSendMesg(...)` layout so as1's line-number ties schedule `addiu a0` as retail.
4. **control_cell_text2/text3 parameter order** = (row, col, text, a, b, sel) / (row, col, text, s1, s2, s3, sel1, sel2):
   retail loads sel/sel1 last because ugen evaluates IPA register arguments in prototype order. The callee bodies
   are byte-identical under the new order (their IPA colours come from the callee allocation). rows 161 -> 121, ops 19 -> 6.
5. `(D_8017A4E0.strings + D_8017A4E0.lang[n])[1]` for the second string of a pair (retail CSEs the row address).
6. Timeout cases: `if (C.timeout < D_8002E8E8.frameCount) C.done = 1;` on ONE line (as1 delay-slot tie: retail fills
   the beqz slot with `li t0,1`). ops 6 -> 0.
7. Temp ring: `C.x + C.w / 2` (header) and `C.x + C.w / 2, C.y + (C.h - 20) / 2 + 20` (case 1) — operand order.
8. Of w32d's three compiled-out reads only one `if (row) {}` is still needed (removing it: ops 36).

### Integration notes (exact)
- Group `codex_control_settings_a4` superseded IN PLACE (same name). members = the 6 old members (func_800DCD58,
  func_800DCCE0 verbatim; control_cell_text, control_cell_text2, control_cell_text3, func_800DD0C0 byte-identical)
  + control_settings (moved from context to member; still on `keep`). claims = all seven. context: slot_state_setup,
  object_create, func_800B669C (unchanged); files group.c + ctx_gr3_b.c (cmp-identical to
  src/blob/groups/credits_scroll_grp/gr3_b.c and to the locked packet's copy).
- Source changes the integrator should know: control_cell_text2/text3 prototypes are reordered (bodies identical);
  pair_set_DC writes the global directly (callers identical).
- unit_overrides force_internal: keep the existing entries (control_cell_text/2/3, func_800DD0C0, gfx_lock_DC,
  gfx_unlock_DC, font_set_DC, font_set1_DC, pair_set_DC) and ADD the new non-static inlined helpers: fsS_DC, fsL_DC,
  TB_CenterText_DC, cs_draw1_DC, cs_draw3_DC, cs_drawT0_DC, cs_drawT1_DC, cs_drawT2_DC, cs_drawT3_DC, cs_wait11_DC,
  cs_wait12_DC, cs_wait13_DC, cs_ctl10_DC (none kept, so no deleted-procedure stubs; control_settings' range
  0x800DD4AC..0x800DDEA4 has no stubs).
- control_settings' own .rodata: the 15-entry jump table (0x801242CC, `lui at,0x8012; lw t6,17100(at)`) — the w31
  jump-table window rule in blob_group applies.
- The four wave-32 members lose their "provisional on the caller form" caveat: their only caller now MATCHes.
- No data-symbol changes. GC names used: TB_CenterText (TB_text.c), the per-dialog draw helpers mirror GC playerscr.c
  drawPakFull/drawPakDamaged/drawFormatMC (`unsigned char str1[256]`); names cs_* are lane labels, not GC names.

## SetupWaveModel + func_80097164 (~30 min; no new best — w29d best / w31d unit_best.c stand)
Unit (`--internal SetupWaveModel --internal func_80097164`, v/start.c = w31d `unit_best.c`): `FAIL SetupWaveModel: 290 of
301 words differ; compiled body is 294 words` rows 233, ops 35, frame 112/112; func_80097164 10/86, rows 7.
ctrace (`swm0`): first w11 save 17.94 nocs 18 tot 323 -> s8; idx param piece w203 tot 100 = best 100 -> split; idx
whole web w3 save 0.79 -> split. Retail's nine s-registers are j*16, j, i, &gRomObjDefPtrs[idx], i*88, offset, WADptr,
&wad/&gRomObjDefPtrs[n], idx; `first` is not among them (t0, spilled to its home 116(sp) around calls), so in retail
first's priority falls below all nine. Lever tested: parameter order `SetupWaveModel(first, idx)` (web numbering):
no change (290/301, rows 233). GC `_SetupWaveModel` locals include pointer locals ptTexDef/ptRomObjDef/ptPalDef/
ptBasicTargetPath that the N64 frame (112, every home matched by the w28d declaration list) has no room for.
Next hypothesis: the three in-loop `(first ? WADptr : offset)` selects are where first's weight (300 of 323) comes
from; a form where the select is not a use of `first` at depth 2 (e.g. the selected base computed once per i
iteration into a variable) would drop first below the nine — but retail's inner loop does test t0 three times per
j iteration, so the select must stay in the loop; the open question is why its weight does not count in retail.

## func_800E543C — not worked
frontier show: `unit_blockers: func_800C4F68, stunt_combo_display` (both unmatched), so even a unit match would be
provisional; w28d's three disclosed `do {} while (0);` devices stand. Skipped by priority.

## CreatePlayer (~30 min; no new best — w32c lead_f2.c stands, 256/272, rows 120, ops 43, frame 224/224)
New facts: the two caller-less stubs before CreatePlayer are, by GC save.c order, `_GetUniqueExtension`
(0x800CD0F4) and `_Pow` (0x800CD0FC); retail's `move a0,s7` (name) and `move a3,s2` (attempt) right before the name
loop look like the parameter copies of an inlined helper holding the name loop and the sprintf. Lever tested
(`v/g1.c`): `GetUniqueExtension_CP(file, name, attempt, ext)` holding the name loop + digit code — NOT inlined by
umerge (jal, frame 168): too large. ctrace (`cp0`): i = w6 save 103 -> a3 (cost 10 vs s0 18.6); retail keeps a3 for
the attempt copy, which forbids a3 to i. Next: find a smaller helper boundary that umerge inlines (e.g. only the
name-copy loop with (name, attempt) params) and measure its Mmt cost against the exact 224 frame.

## pak_delete_file_retry — the one-hour `lead_q3.c` lead (no strict improvement; best.c = w31c's)
Unit (own run.sh): best.c `165 of 196`, rows 98, ops 12, frame 232/232; lead_q3.c 123/196, rows 105, ops 21.
ctrace on w31c p5 (no pfs local): callback constant w41 save 10 tot 20 > callee cost 10.5 -> s0; pfs split.
Tested: (a) `cb = func_8008A6A4;` local before the loop (p5_a: frame 232 exact, rows 98 = best, callback still s0,
and the lock message slot moves 8 B below retail: 188 vs 196 — so a cb local is not retail's 8 B); (b) the error
report as an inlined helper taking the callback (p5_b/q3_b: frame +16/+24, worse); (c) the retry loop written with
`goto` (identical code). Stop: callee-saved colour (callback in s0 vs retail t0) with pfs split; next is a force
oracle of the callback split on p5 and a source form for pfs' in-loop weight.

## What generalises
- **A frame-layout mismatch can be a missing HELPER structure, not a missing variable.** Odd array sizes (296/328/272)
  that fit retail offsets were really 256-byte locals of inlined per-case helpers plus their inline areas. Read the
  per-variable Mmt layout from the merged ucode (`control_settings/lay.sh` = blob_unit + w12h `udump.py` + `mmt.py`):
  it gives every variable's frame-top offset, so a layout hypothesis is checked in one run.
- **Inline-area law refined (merged-ucode measurements):** an inlined call reserves its parameters and locals
  (8-aligned blocks), a nested void call ~4-8 B more; a return copy (`move sX,v0`) survives only for a helper that
  returns a local AND contains a nested inlined call, or for a single-level helper inlined at exactly ONE site
  (a single-level helper used at two sites is not inlined at all).
- **IPA callee parameter order is visible in the caller's schedule**: ugen sets IPA register arguments in prototype
  order, so a load retail emits late belongs to a later parameter — and reordering an internal callee's parameters
  left its body byte-identical (control_cell_text2/text3).
- **as1 line-number ties**: one-line statements (`if (c) x = 1;`, `old = f(); osSendMesg(...);`) and defining a
  helper after its caller move delay-slot and ready-list choices; test them before any colour work.
- **`(base + i)[1]` vs `base[i + 1]`**: only the first lets uopt CSE the row address between `[i]` and `[i + 1]`.

Permission denials: none. Builder scratch watchman2:~/rush2049/scratch/frontier/w33d deleted at the end.
