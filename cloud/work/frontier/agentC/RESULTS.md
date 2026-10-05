# Frontier batch, agent C (2026-10-04)

Scorer: `tools/cloud/score.py` in the builder copy `watchman2:~/rush2049/scratch/frontier/agentC`
(`IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a.../ido`). `sc.sh SRC NAME [args]` in this directory copies a
source over and runs `python3 tools/cloud/score.py fn cand/NAME.c NAME [args]`; the commands below are that call.
Nothing was committed, spliced or locked.

| # | function | bytes | result | flags |
|---|---|---:|---|---|
| 1 | audio_channel_alloc | 676 | **strict MATCH** | -O2 |
| 2 | camera_blend_between | 496 | text identical, `MATCH (2 section-relative relocations unverified)` — not strict | -O2 |
| 3 | menu_load_options | 364 | **strict MATCH** | **-O3** (90/91 differ at -O2) |
| 4 | func_800DE860 | 844 | text identical, `MATCH (8 section-relative relocations unverified)` — not strict | -O2 |
| 5 | random_int | 284 | **strict MATCH** | -O2 (also -O3) |
| 6 | func_800EC270 | 136 | text identical, `MATCH (6 section-relative relocations unverified)` — not strict | -O2 |
| 7 | func_800D1AB0 | 560 | 137/140 alone (+4 words); strict MATCH only inside a stand-in -O3 group — not claimable | -O3 group |

## Strict matches (cloud/matches/)

- `score.py fn cand/audio_channel_alloc.c audio_channel_alloc` -> `audio_channel_alloc:  MATCH`
  Source `cloud/matches/audio_channel_alloc.c`. No quirks; only the declaration order of the locals was chosen
  (`i, next, end, dist, d[3]`; four orders match). The earlier 161/169 attempt (near_miss_B29) was m2c-shaped;
  a typed `PathPoint points[]` with `points[next].pos[k] - points[i].pos[k]` and a plain `for` matched first try
  apart from stack offsets.
- `score.py fn cand/menu_load_options.c menu_load_options --flags '-g0 -O3 -mips2 -G 0 -non_shared'` ->
  `menu_load_options:  MATCH`. At the default -O2 the same source gives `90/91 words differ (1 extra words ...)`.
  Source `cloud/matches/menu_load_options.c` (arcade ForceApart). Quirk: unused local `f32 mag` supplies 8 bytes of
  the 80-byte frame (arcade has an unused-on-N64 `force` scalar there). Float literals are referenced as
  `extern f32 D_801240FC/D_80124100/D_80124104` so the scorer can resolve them.
- `score.py fn cand/random_int.c random_int` -> `random_int:  MATCH`
  Source `cloud/matches/random_int.c`. No quirks. Literals referenced as `extern f32 D_801247FC/D_80124800/D_80124804`;
  with inline literals (`random_int/literal.c`) the scorer prints
  `MATCH (6 section-relative relocations unverified: .rodata+0x0 at +0x1c, ...)`.

## Text-identical but unverified by the scorer (best.c in each directory)

These print `MATCH (N section-relative relocations unverified ...)` (exit status non-zero without
`--allow-unverified`). Every text word equals retail; the unverified words are references to the function's own
`.rodata`/`.data`, which the scorer masks. They are leads, not matches, until the image gate places that data.

- **camera_blend_between** — `score.py fn cand/camera_blend_between.c camera_blend_between` ->
  `MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x154, .rodata+0x0 at +0x158)`.
  The literal is 0.0025f; retail word at the referenced 0x80123E74 is 0x3B23D70A = 0.0025f.
  Cannot be made strict with an extern: `extern f32 D_80123E74` is address-hoisted into s6 (121/124 differ).
  Quirk: final `val = 1;` (int literal) keeps that 1.0 out of the hoisted 1.0f web (`1.0f` there: 9 words off).
- **func_800DE860** — `score.py fn cand/func_800DE860.c func_800DE860` ->
  `MATCH (8 section-relative relocations unverified: .rodata+0x0 at +0x230, .rodata+0x0 at +0x238, .rodata+0x4 at +0x240, .rodata+0x4 at +0x248, .rodata+0x8 at +0x2fc, .rodata+0x8 at +0x300, .rodata+0xc at +0x308, .rodata+0xc at +0x30c)`.
  Literal order 0.1f, 1500.0f, 0.97f, 0.03f = retail 0x80124310/14/18/1C (3DCCCCCD, 44BB8000, 3F7851EC, 3CF5C28F).
  This closes the float-allocation residual recorded in `cloud/work/ipa-groups/render_large_objects/STATUS.md`
  (~500 variants): (a) K1..K4 as literals, K1/K2 through locals `k1`/`k2` (arcade `max_catchup`/`cuzone`), K3/K4
  inline — extern floats are loop-hoisted, rodata literals are not; (b) arcade literal types from
  `set_catchup` (rushtherock/game/model.c): `k1 + 1.0f` but `d * k1 / k2 + 1` and `(1 - s) * 0.5f + 1` with int 1,
  which yields the two separate 1.0 webs (f14, f20). Found by a 648-file product over the spellings of the four
  1.0 sites x four 0.5 spellings x locals/inline; two files match (`* 0.5f` and `/ 2.0f`).
  With all four as externs: 214 words, 62 aligned rows differ (frame 32, f22/f24 saved).
- **func_800EC270** — `score.py fn cand/func_800EC270.c func_800EC270` ->
  `MATCH (6 section-relative relocations unverified: .data+0x0 at +0x2c, .data+0x0 at +0x38, .data+0x0 at +0x3c, .data+0x0 at +0x4c, .data+0x0 at +0x58, .data+0x0 at +0x60)`.
  The round-robin counter at 0x801161D0 must be a function-local `static s32 x = 0;` (retail word there is 0, in
  .data). Every extern/file-scope form tried (plain, volatile, array, struct, defined in unit, file `static`,
  local temp, `++`/`+= 1`/chained forms, -O1, -O3; ~25 variants) gets its address hoisted (`lui/addiu` + `0(reg)`),
  7-26 words off; micro-tests confirm IDO always hoists a read-modify-write of a file-scope global and never of a
  function static.

## Not matched

- **func_800D1AB0** (best: `func_800D1AB0/best.c`, group: `func_800D1AB0/standin_group/`).
  `score.py fn cand/func_800D1AB0.c func_800D1AB0 --flags '-g0 -O3 -mips2 -G 0 -non_shared'` ->
  `137/140 words differ (4 extra words (nonzero beyond target length))`; instruction-aligned it is 10 rows:
  `sw/lw s0,s1` (4 extra words) and the resulting frame 88 vs 72 with shifted `sp` offsets. Same at -O2.
  Residual class: **frame / calling convention (IPA)**. Retail uses s0 and s1 and never saves them, which only
  whole-program -O3 produces. `score.py group cand/<standin_group>` -> `Members: func_800D1AB0: MATCH` with two
  stand-in callers, so the body is right, but a stand-in group cannot be spliced.
  Tried: arcade CheckCPs shape (index local, `dist_tab[index] > dist_tab[place[high_index]]`) took the group from
  124 -> 69 differing words; `s32 high_index` and a 192-file sweep of array sizes / local order closed the rest
  (5 layouts match). Declaration order of scalars alone changes nothing (61 shuffles identical).
  Next hypothesis: build the real group {func_800D1AB0, car_setup_confirm, func_800F8EC8}; the 254/256
  func_800F8EC8 near-miss (near_miss_B125: "frame 144 vs 168, additional native callee-save registers") is very
  likely the same convention seen from the caller side.

## What made the mid-size ones hard

1. **Wrong flags, not wrong source.** menu_load_options needs -O3; at -O2 it is 90/91 off however it is written
   (-O2 puts `car` in s0; -O3 keeps it in t0 and spills around the call). About 65 -O2 variants were wasted
   before a flag probe. Probe -O2 and -O3 on the first structurally right draft.
2. **The frontier tool misses one IPA signature.** func_800D1AB0 has no register params and nothing in
   `preserved`, but it clobbers s0/s1 without saving them. "Writes a callee-saved register that the prologue
   does not save" should mark a function as an IPA member.
3. **Own-rodata / own-data references cannot score strict.** Float literals that are not `lui`-encodable
   (0.0025f, 0.1f, 1500.0f, 0.97f, 0.03f, 250000.0f ...) and function statics are section-relative. Replacing a
   literal by `extern f32 D_xxxxxxxx` works only where the load is not inside a loop (random_int,
   menu_load_options); inside a loop the extern is hoisted and the code changes (camera_blend_between,
   func_800DE860). Three of seven functions end as "MATCH (N unverified)" for this reason alone; the scorer (or
   the splice gate) needs a way to check a local .rodata/.data word against the retail word at the address the
   target instruction encodes.
4. **Literal type is part of the source.** Arcade code writes `+ 1` and `1.0` interchangeably; int-typed
   literals become separate constant webs. This was the whole 500-variant residual of func_800DE860 and the last
   9 words of camera_blend_between.
5. **Missing struct types / callee prototypes were not the problem.** Local ad-hoc structs with the right
   strides were enough each time; the arcade source (collision.c ForceApart, model.c set_catchup, scp.c
   CheckCPs) gave the statement shapes for four of the seven. Historical names were wrong for all five named
   functions (see the header comment in each source).

Helper scripts here (`sc.sh`, `batch.sh` + `bscore.py`, `full.sh` + `full.py`, `grp.sh`, `dump.sh`, `peek.py`)
only drive the builder copy; `bscore.py`/`full.py` give instruction-aligned counts/diffs for a directory of
variants or one file, also in -O3 group mode (`--keep a,b`).
