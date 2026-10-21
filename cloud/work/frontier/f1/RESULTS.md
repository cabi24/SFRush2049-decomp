# Lane f1 (fast 20-minute dig, 2026-10-21)

| function | GC identity | bytes | state | deliverable |
|---|---|---|---|---|
| menu_back | _DecodeGhost@save.c | 132 | **group MATCH + unit EQUAL** (internal, IPA-private in func_800CBF2C's unit) | groups/menu_save_options/ (supersedes in place) |
| ghost_race_setup | GhostEnd@ghost.c | 440 | 3 words (unchanged; 8 line-split/order variants tried) | ../w36w2/ghost_race_setup/best.c |
| func_8010E8B4 | animPanes@videoscr.c | 340 | 3 words (unchanged; 21 variants tried) | ../w32b/func_8010E8B4/best.c |

## menu_back: MATCH
- Lever: compiled-out `do {} while (0);` between `m->count = n / 3;` and `format_string_parse(m->buffer, n);`
  (disclosed shaping). Retail then emits `move a1,v0` early and puts `lw a0,76(s0)` in the jal delay slot.
  Decl-order (MAT3 `size` first), block-local n, line merges, trailing do-while: all still 3 words.
- `python3 -m tools.conveyor.pipeline.blob_unit --tag f1 score menu_back --neighbours --with cloud/work/frontier/f1/menu_back/m6.c`
  -> `EQUAL menu_back: 33 words (internal, c_m6.c)`, `locked bodies that differ in this unit: 0`.
- Group `groups/menu_save_options/` = installed src/blob/groups/menu_save_options with ONE change in ctx_slot.c (the
  do-while line) and group.json: menu_back context -> members, `claims: [menu_back]`; every other file byte-identical.
  Builder `score.py group cand_msog` -> exit 0, all 26 members MATCH (24 old + hud_setup + menu_back); context
  CreatePlayer 246/272 informational, as before. Not standalone (IPA-private s1/s0), so no cloud/matches file.

## ghost_race_setup: still 3 words
- Variants l1-l6 (merge/split the ghost copy, slot clear, modelrun store and header load onto lines; separate
  `ghost = ...` assignment): all `FAIL 3 of 110`. l7 (`do {} while (0);` before the header load): 21. l8 (header
  load before modelrun store): 74. Residual unchanged (as1 copy propagation; see w36w2 notes).

## func_8010E8B4: still 3 words
- Residual: retail `sll t0,v0,1; addiu t0,10; sll t8,t0,16; sra t9,t8,16` (h coloured t0, narrowed y a ring temp
  t9 used for a1 and a3); ours narrows into t0 (y is the coloured web, h propagated into it).
- Tried (unit, all from w32b best): line merges/splits of the y/h statements and the InitPane call (e2,e3,e5,e6: 3);
  `do {} while (0);` after `y = h` (e1: 8) / before it (e4: 43); `y = h = ...` (g3: 3); `h = lineh(); y = h*2+10`
  (g4: 3); compiled-out `if (h) {}` (g1: 43) / `if (y) {}` (g2: 19); declaration orders (h1,h3: 3; h2: 6);
  block-local y (h4: 6); inlined helpers defined after the caller: `line_h()*2+10` (f3/f4: 6, frame 80 without u0,
  same 3 core words), helper returning the whole expression (f1/f2: 40), s16 helper (f5: 23).
- Frame note: removing w32b's disclosed unused `u0` gives frame 80 (retail 96) with the helper form; the missing
  16 B are still unexplained.

Files: menu_back/best.c (= m6.c), groups/menu_save_options/, ghost/*.c, e8b4/*.c (variants).
