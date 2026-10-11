# w20h RESULTS (wave 20)

Targets: entity_render_setup (start w17d best.c, 68 rows, frame 184 exact) and camera_play_script (start w17a/w18d
best.c, 27 rows). Builder scratch `watchman2:~/rush2049/scratch/frontier/w20h` (toolkit reused from wtk).

| Function | Bytes | State | Flags | Deliverable |
|---|---:|---|---|---|
| entity_render_setup | 2048 | near-miss; all four residual causes traced, three proven by assembler oracle; blocked on a tooling question (defined-data alignment in the unit) | `-g0 -O3 -mips2 -G 0 -non_shared` (unit) | `w20h/entity_render_setup/best.c` |
| camera_play_script | 3520 | unchanged: 27 rows, frame 608/608 (best.c = w17a/w18d best.c); two untried hypotheses measured inert | `-g0 -O3 -mips2 -G 0 -non_shared` (unit) | `w20h/camera_play_script/best.c` (= w17a) |

## entity_render_setup (0x8008C884)

### Scores (current tree, `TAG=w20h`)

```
$ cloud/work/frontier/tools/trace/us.sh entity_render_setup cloud/work/frontier/w20h/entity_render_setup/base.c   # = w17d best.c
base.c: FAIL entity_render_setup: 420 of 512 words differ; compiled body is 518 words, target 512; +0x354: .rodata+0xd | words 68 ops 26 norm 26 | frame 184/184
$ cloud/work/frontier/tools/trace/us.sh entity_render_setup cloud/work/frontier/w20h/entity_render_setup/best.c
best.c: FAIL entity_render_setup: 417 of 512 words differ; compiled body is 514 words, target 512; +0x354: .rodata+0xd | words 75 ops 20 norm 20 | frame 184/184
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w20h --jobs 2 score entity_render_setup --with cloud/work/frontier/w20h/entity_render_setup/v/d2.c --neighbours   # d2.c = best.c body
  FAIL entity_render_setup: 417 of 512 words differ; compiled body is 514 words, target 512; +0x354: .rodata+0xd0: retail words encode 0x39041821, outside the image; ... (+20 more); .data+0x48 is placed at different image addresses: ...
  locked bodies that differ in this unit: 0
```
best.c's "words 75" is higher than base's 68 only because us.sh counts the new `.data`-relative relocation rows
(`lui at,0x0` / `lwc1 $f8,72(at)` against retail's `0x8012`/`-21104`): `unverified 40` vs 26. Structurally it is
closer: 514 vs 518 words, ops/norm 20 vs 26. The headline "417 of 512" is the positional count of a misaligned body.

### Cause 1 (draw loop, 4 words + ~8 rows): `lui at` sharing needs a DEFINED symbol at a 16-aligned .data offset

Retail's 4x-unrolled draw loop uses three `lui at,0x8012` for the eight D_8011AD90 loads, grouped
{x0,y0,x1} {y1,x2,y2} {x3,y3}; ours (extern) has one `lui at` per load (+4 words). Found:
- `extern` -> no sharing at all. A tentative (common) definition -> sharing in 8-byte chunks (516 words). An
  initialised definition (retail bytes, best.c) at `.data+0x48` -> sharing {0,4},{12,16},{24,28},{36,40} relative
  to the symbol = 16-byte chunks of the section offset 72..120 (514 words).
- **Rule (proven):** as1 keeps one `lui at` for loads of one defined symbol only while the addresses stay in the
  same 16-byte-aligned chunk of the section (the section is 16-aligned, so the hi half cannot change inside a
  chunk). Oracle: the d1/d2 ugen listing with `D_8011AD90` defined in a `.data` block placed BEFORE `.text`
  (as0 is one-pass; a definition after the text is still "undefined" when the loads are parsed) and `.space N`
  in front of it, reassembled with `tools/trace/asm.sh`:
  `N=0: 513 words, 52 rows; N=80: 513/52; N=8: 514/62; N=72: 514/62; N=4: 513/59` (52 includes 26 own-rodata
  relocation rows). With N = 0 mod 16 the draw loop is exact (rows +0x530..+0x6bc vanish).
  Files: `entity_render_setup/oracle/listing_d1_data_at0.s` / `_at8.s`.
- So retail had D_8011AD90 defined, at a section offset = 0 mod 16 (its final address 0x8011AD90 is too).
  `music_control` (unmatched) also takes `&D_8011AD90` (`addiu s3,s3,-21104`) and `D_8011AD8C`, so this is
  shared global data, not a function-local static.
- **Blocker (tooling, for the coordinator):** in the current unit D_8011AD90's offset is fixed by every earlier
  file's `.data` (unit order: D_801161D0, D_80116DB8, D_801170F8, `$$3575` (48 B), `$$3781`, static_init_done,
  D_80116488 = 72 bytes before it). No natural source in this file can move it to 80 or 64 (an unreferenced extra
  definition is dropped by uld; defining the neighbouring retail objects D_8011AD88/8C would be other functions'
  data). Options: (a) a blob_unit data pad analogous to the text `align` pads (pad the unit `.data` so an
  `align`-listed data symbol sits at its image address mod 16), or (b) accept defined globals ordered/padded by
  the coordinator. Any later `.data` added to an earlier unit file would shift it again, so (a) is the robust one.
  Note for integration: blob_unit's own-data check already verifies `.data` references (it reports
  `.data+0x48 is placed at different image addresses` while the body is misaligned).

### Cause 2 (world loop, ~15 rows): ucode operand order of the `pos + prod` adds

Oracle `listing_all_edits.s` (see `oracle/edits.diff`): rewriting only the world-loop FP registers to retail's
(`l.s $f4,0($16); mul.s $f10,$f4,$f14; l.s $f6,8($2); add.s $f8,$f6,$f10` ...) makes the whole loop exact, the
`s0` bump schedule included. Retail = our ring state with ugen evaluating `record->pos.x` FIRST, i.e. retail's ucode
is `ADD(ilod pos, MPY)` where ours is `ADD(MPY, ilod pos)` (simulated through the 4-entry $f4..$f10 ring for x, y, z).
- Source order is inert (`v/w1..w4`, `z1..z3` with `* 1.0f` identities: all 75/20).
- **Rule (probe-proven):** uopt orders a commutative add's operands by creation order, the NEWER operand first.
  With a pos expression that is new in the loop (`D_80152818[effect->player].pos.x`, probes `v/q1.c`, `v/q2.c`):
  source `prod + pos` -> `l.s pos` first; `pos + prod` -> product first. Ours is product-first for every spelling
  because `record->pos.x` was already created by the delta statements (`delta.x = record->pos.x - cam.x`).
- So in retail the loop's pos loads are newer than the products: either the delta block reads the car position
  through a different expression (not `ILOD(record)+8`), or the loop does. Tested: direct global indexing in delta
  (`v/r1.c`, `r2.c`: address recomputed, 353 rows), a laundered `record` in delta or loop (`v/x2`, `x4`: inert, the
  `(u32)` cast makes no ucode), a product through a reused variable (`v/y1..y3`: right order but the product
  becomes a coloured $f2 web). Not found yet.

### Cause 3 (palette base, 4 rows + 1 word) and cause 4 (`&delta`/`&quad`, 3 rows): uopt LR start blocks

Both are address live ranges that start one block earlier in retail. Proven by listing edits (asm.sh):
- moving `la $3, D_8011B558` from the else arm (`$505`) into the mode-test block before `bne $11,2,$505` gives
  retail's `lui v1 / bne / addiu v1` and the body length 512 (one `lui v1` vs `li at,2` order row left, a line tie);
- moving `addu $4,$sp,100` above `bne $9,$6,$515` gives retail's a0 at +0x2d4; retail also has `addu $5,$sp,128`
  there (a1 directly, not `move $5,$8`).
With all edits (`listing_all_edits.s`): `want 512 words, got 512; differing rows 31 (words)` = 26 own-rodata
relocation rows + 5 (the a1 pair and the lui-v1/li-at tie). as1 does not hoist from the fall-through or the target
on its own (the `.noalias $3,$sp` after `$505` is not what blocks it: removing it gives 41). Defining the palette
symbols (`v/p1.c`, `p2.c`) changes nothing in uopt (norm 20). w17d's pretrace (two contexts for the palette
address: ixa operand in the sub-arms, ilod base in the else arm) stands; the source form is still open.

### Next
1. Coordinator decision on Cause 1 (data alignment pad); without it no source can match.
2. Cause 2: find the delta/loop spelling where the loop's `pos` load is a fresh itable entry (watch the add order in
   the listing: `tools/trace/as1t.sh LABEL entity_render_setup` then grep `.loc 7040 194`).
3. Causes 3/4: one address-LR lever probably fixes both (an earlier occurrence in the branch block).

Files: `entity_render_setup/best.c` (= `v/d2.c` with header), `base.c` (= w17d best.c), `oracle/` (listings +
`edits.diff`), `v/` (variants, ignored).

## camera_play_script (0x800C5644)

Start: w17a/w18d best.c, re-scored in the current unit, unchanged:
```
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w20h --jobs 2 score camera_play_script --with cloud/work/frontier/w20h/camera_play_script/best.c --neighbours
  FAIL camera_play_script: 793 of 880 words differ; compiled body is 879 words, target 880; +0x208: .rodata+0x498: retail words encode 0x01284282, outside the image
       umerge inlined into it: effect_cleanup, func_800B61A8
  locked bodies that differ in this unit: 0
$ TAG=w20h cloud/work/frontier/tools/trace/us.sh camera_play_script .../best.c
best.c: FAIL camera_play_script: 793 of 880 words differ; compiled body is 879 words, target 880; +0x208: .rodata+0x49 | words 27 ops 1 norm 1 | frame 608/608
```
Already tried by w15h/w16c/w17a/w18d (from their RESULTS): effect_cleanup inlining, merge order, `goto end` loop
weights, frame/named locals, func_800B61A8 `u8` parameter, `u` denominators, k ternary, `n &= 0xF` split, zero
spellings, `conv = 1` placement, `old` in the branch, `i = idx[0]`, `c1` removal, `ez` before `ex`, product
through `u`; corner IV: `&pts[k][0]`, `(f32 *)&pts[k]`, `pts[0] + k*3`, `pts[e]`, `pts[ka]`, k=0 before the call,
do-while, `ka/kb/e/i/type` holders, `k != 4`, `k <= 3`, extra `pts[ka]` reads, laundered argument (oracle only),
stub search (none). w18d's oracle: items 1+2 are exactly two decisions (IV not a bare argument copy + table pointer
ranked at or above it; pts[ka] needs ~2x pts[kb]'s weight); item 3 is temp-home order.

New this wave (both inert, stopped well inside the 2-hour limit):
1. **Defined index tables** (the entity_render_setup finding: retail's whole-program unit defines all data).
   `s8 D_801174C4[8] = {0,1,3,2,0,1,3,2}` and `s8 D_801174CC[16] = {0,1,2,3,0,1,2,3,1,2,3,0,4,5,7,6}` (retail
   bytes) (`v/g1.c`): `words 33 ops 1 norm 1` -- the extra 6 rows are only `.data`-relative relocation displays;
   every register, spill slot and the IV colour are unchanged. uopt does not treat defined and extern data
   differently here (same result as the palette tables in entity_render_setup); it matters to as1 only.
2. **Corner argument through a reused, otherwise-live pointer** (`v = (PV *)pts[k]; func_800A61B0(d, (f32 *)v,
   mtx);`, `v/c1.c`, the w17c "reused local stops substitution" idea) and `*(pts + k)` (`v/c2.c`): both 27 rows,
   byte-identical to best.c. `v` is still copy-propagated inside the loop, so the argument stays the bare IV.

Not delivered: nothing changed. Best next hypothesis unchanged from w18d (a source form that is not a bare copy
of the IV into a1 while ranking the D_801174C4 pointer at or above the IV). For item 4 (`ex * (hz - pz)` order),
note the commutative-operand rule found in entity_render_setup: uopt puts the NEWER of two non-variable operands
first; a plain variable goes second. So retail's `ex`-first order means `ex` reached that multiply as an expression
created after `hz - pz` (consistent with w17a's `ex * 1.0f` identity fixing it).

Permission denials: none.
## What generalises

- **as1 shares `lui at` only for DEFINED data, and only inside one 16-byte-aligned chunk of the section.** Extern
  symbols get one `lui at` per access; a tentative (common) definition shares in 8-byte chunks; an initialised
  definition shares while `section offset + addend` stays in the same 16-byte chunk. Retail's whole game was one
  unit with every data symbol defined, so any retail `lui at` sharing pattern on a global table also tells you the
  table's section offset mod 16. Test with `asm.sh` by putting a `.data` block with `.space N` BEFORE `.text` in
  the listing (as0 is one-pass; a definition after the text does not count, which is why w13f found asm.sh "loses"
  the sharing).
- **uopt orders a commutative add's two non-variable operands newest-first** (creation order of the itable
  entries), and ugen evaluates the first operand first, which sets the FP temp ring. Source order is irrelevant
  once both operands exist; an operand created by an earlier statement (e.g. `record->pos.x` in a delta
  computation) always goes second.
- uopt itself does not care whether a data symbol is defined (palette and index tables: no change in colouring,
  LR placement or spill homes); the definition matters to as1 only.

Time: entity_render_setup ~2.5 h (trace + oracles), camera_play_script ~20 min. Permission denials: none.
