# w26c RESULTS (wave 26, game, IDO 5.3)

No MATCH: nothing for cloud/matches, no group dir, no unit_overrides, no data-symbol changes. Work bases only.
Builder scratch `watchman2:~/rush2049/scratch/frontier/w26c` (toolkit reused from wtk; uopt_ulr/uopt_or copied from
w25d; snapshots deleted at the end). Permission denials: none.

| Function | Bytes | Before -> after | State | Scorer line (Pi unit, current tree, `--tag w26c`) |
|---|---:|---|---|---|
| entity_render_setup | 2048 | 8 ops rows -> 8 | near-miss; two-LR mechanism proven (oracle, alias symbol + holder: 6 ops, body 512); no one-symbol spelling exists | `best.c: FAIL entity_render_setup: 420 of 512 words differ; compiled body is 513 words, target 512; +0x354: .rodata+0x1 \| words 52 ops 8 norm 8 \| frame 184/184` |
| func_80090308 | 1128 | 210 true / 198 rows / ops 87 -> **202 / 195 / 59** | near-miss, improved (disclosed device + unused-local hypothesis) | `FAIL func_80090308: 202 of 282 words differ; +0x19c: own .rodata+0x118 (3ba3d70a) differs from the image at 0x80123220` / `locked bodies that differ in this unit: 0` |
| save_write_data | 1200 | 223 true / 153 rows / ops 33 / body 303 -> **149 / 120 / 30 / body 300** | near-miss, improved (inlined blast_init) | `FAIL save_write_data: 149 of 300 words differ; +0x1f8: .rodata+0x340: retail words encode 0x3bff8014, outside the imag[e]` |
| entity_physics_update | 712 | 21 words, unchanged | not worked (time) | `FAIL entity_physics_update: 21 of 178 words differ` |

Group command: `python3 -m tools.conveyor.pipeline.blob_unit --tag w26c --jobs 2 score func_80090308
entity_physics_update save_write_data --with cloud/work/frontier/w26c/f308/best_effects_w26c.c --with
cloud/work/frontier/w26c/f308/swd_best.c --internal func_80090308 --neighbours` -> the three FAIL lines above and
`locked bodies that differ in this unit: 0`. Shortcuts: `f308/q.sh FN FILE` (rows), `f308/qs.sh SWD.c [EFFECTS.c]`.

## entity_render_setup (0x8008C884, 2048 B) — 8 ops rows, unchanged; w25d's two-LR hypothesis PROVEN as mechanism, no natural spelling

Re-score (Pi unit, current tree): `best.c: FAIL entity_render_setup: 420 of 512 words differ; compiled body is 513
words, target 512; +0x354: .rodata+0x1 | words 52 ops 8 norm 8 | frame 184/184` (best.c = w25d/w24d best).

### Two candidates, no CDX_FORCE (oracle `v/y3.c`)
Sub-arms read the palette through a second extern name `PalB` (stand-in for "a symbol resolving to 0x8011B558"),
plus w25d's code-free block-17 occurrence `if (D_8011B558) {}`. pretrace/ctrace (snapshot y3), no force:
```
  lda      ldaS(7210+0)           ctx=0 LIVE-RANGE     blocks 17
  lda      ldaS(7210+0)           ctx=3 LIVE-RANGE     blocks 30,30,30
  lda      ldaS(7211+0)           ctx=0 LIVE-RANGE     blocks 21,21,21,22,22,22      (PalB, sub-arms)
 375  ldaS(7210+0)   p1:w375 save=1.50 nocs=2 color->v1     (else-arm LR {17,30} = retail {17,28}, coloured whole)
 377  ldaS(7211+0)   p1:w377 save=2.00 nocs=2 color->v1     (sub-arm LR, reloaded in each sub-arm)
```
`us.sh`: `y3.c: FAIL entity_render_setup: 20 of 512 words differ; +0x1a0: unresolved symbol PalB ... | words 50 ops 6
norm 6 | frame 184/184` — body 512 words; ops rows left: block 17 as1 order (`lui v1` before `li at,2`, retail
the reverse) and the four &delta/&quad rows (cause 4). This is exactly w23e's ADD17+double-split oracle state, reached
WITHOUT forcing: retail's palette is two register candidates, the else-arm one containing block 17.

### Why there is no natural spelling with one symbol (measured)
- uopt's lda key is `ldaS(symbol+offset)`. A block-scope redeclaration `extern Color4 D_8011B558[];` or `[8]` in the
  else arm (x1, x2) gives the SAME key (cfe reuses the external's symbol number); a type-changing block-scope extern
  (`extern u8 D_8011B558[][4]`, x3) is a cfe error ("redeclaration").
- Offsets in the key change the emitted `%lo` (p1/h1 of w23e: `pal[4]` copy-propagates to `ldaS(+16)`, folded
  `lbu -19096`), so retail's `addiu -19112` + `lbu 16(v1)` pins the else-arm key to base offset 0.
- So the second key needs a second name for 0x8011B558. That is a data-symbol hypothesis, not an acceptable device by
  itself, and the block-17 occurrence is still the compiled-out holder. Not delivered.

- y3 is kept as `ers/y3_twokeys_oracle.c` (oracle only: `PalB` is an unresolved stand-in name).
- Remaining after y3: as1 order in block 17 (retail `li at,2` before `lui v1`; w23e's forced oracle left the same
  row) and cause 4 (&delta/&quad, 4 rows). Stopped (stop rule; ~1 h here on top of w23e/w24d/w25d).
- **Best next hypothesis:** a second data name for 0x8011B558 in the original (e.g. the car-colour table and the
  default/entry-4 colour declared as two externs that the linker placed at one address, or a struct view), PLUS a
  natural block-17 occurrence; or accept that the palette residual needs a disclosed alias + holder. Ask the
  coordinator whether a second extern name for an existing address is an acceptable data-symbol hypothesis.

## func_80090308 + save_write_data — mat[12] tested exactly as asked; both improved

### The shared-idiom test (`f32 mat[12]`)
- func_80090308: adding `f32 mat[12];` alone (function level after node, v/m1; block-scoped, v/m2; before color,
  v/m3) changes ONLY the frame and sp offsets: every variant `words 198 ops 87` like the base, frame 160 -> 208.
- Placement that reproduces retail's named layout (v/m4): function level `Node *node; f32 mat[12]; s32 i; Color color;`,
  `p`, `a`, `b`, `c` block-scoped in the loop, unused `vehicle` dropped: node -4, color -60 (retail 268/212 in 272).
  Code identical (ops 87); frame 200 vs 272, so 72 B (18 words) of inline/temp area are still missing.
- save_write_data: `f32 mat[12]` alone (v/s1, between node and color) also changes only offsets (ops 33) and gives
  retail's node-to-color distance (56 B, retail 324 -> 268).
- So the mirror holds: both functions have `node, <12 words>, <1 word>, color` (func_80090308: i; save_write_data:
  the blast pointer). As an UNUSED LOCAL it is disclosed and not yet the exact frame residual (frames 200/272 and
  248/352), so it cannot be delivered as a match device yet.

### save_write_data: the on-car block is an inlined sibling of func_80090308 (new)
- `static void blast_init(s16 slot)` (locals node, mat[12], b, color) inlined into save_write_data (v/s2): the
  s16 parameter home becomes retail's halfword slot 330, node/color keep the 56 B distance, true 223 -> 218,
  ops 33 -> 29, rows 153 -> 127.
- `D_8012E700[b->handle].color.word = color.word` (v/s3, w24e lesson 4): body 303 -> 300 = retail, true 149, rows
  120, ops 30. mat[12] removed (v/s4): identical code, frame 200 -> so code-neutral.
- Retail's own top area: node2 -4, ? -8, m -12, ? -16, ? -20, then the inline param at -22: two named words more
  than ours (node, m, sound). Remaining ops rows: as1 placement of spill reloads (node `lw t0,324(sp)`, `where`
  reloads) and the sound-constant copy (`li a0,69; move v0,a0`).

### func_80090308: rand1 before the stores (new)
- Retail's store order: size, [seed-1 store], zero80/60/68, alpha, m00, [seed 2], m11, m22, [seed 3], stretch,
  [seed 4], spin_y, spin_z. Moving `a = rand(0.005f)` ahead of the diagonal chain and zero stores (v/e3) lowers the
  structural rows but makes &color (ldaM -60) span one block less: save 2.0 ties with 0x42000/128 (ctrace m4 vs r4:
  w209 type1 -60 colour s7; 128 split), ops 73.
- DEVICE `do {} while (0);` right before rand1 (v/e2, = best_effects_w26c.c): &color is split again, s7/s8 =
  0x42000/128 as retail, ops 59, true 202 (was 210). Other positions: before b/c rand 64 ops (v/d1-d3), before
  func_8008E06C 75 (v/d4). A split chain (`m00` before rand2, `m11/m22` after, v/r1/r4) is worse than the chain.
- The w25d colour setter on top (v/c1e): ops 57, frame 224, true 202 (unchanged), body 281; not taken.
- Seed reload before rand4: still absent. In retail stretch is stored through the IV (s0), not through p (s5), so
  the killing store is not p->stretch; uopt disambiguates the s0 stores (no reloads for rand2/rand3).

### Integration (when it matches)
As w23d: group of func_80090308 + entity_physics_update + save_write_data with context copies of func_8008E06C,
func_80090770, func_8008B2E4.c and func_80090284; `force_internal func_80090308`. blast_init is a new inlined
static (no stub seen in retail; check for a caller-less `jr ra; nop` before packaging).

## What generalises
- **uopt keys lda candidates by (symbol number, offset)**; a block-scope `extern` redeclaration reuses the external's
  symbol number (same key), and a type-changing one is a cfe error. Two register candidates for one address need two
  names. With two names, w25d's code-free holder gives the "{17,28} coloured whole + reloaded sub-arms" shape
  without any forced split (`ers/y3_twokeys_oracle.c`).
- **An unused local array moves only sp offsets** (measured on two functions, 0 ops change): a cheap test for
  frame-layout hypotheses before they are needed as an exact residual.
- **Sibling functions share local templates**: a block of another function that has the same named-slot pattern
  (node, 12 words, 1 word, color) is probably an inlined static from the same template; inlining it moves the
  parameter home to retail's slot and fixes neighbouring code (save_write_data 223 -> 149).
- Reordering statements changes the block count of long-lived address webs (&color) and can flip a save tie;
  the empty-block lever restores it (one more case of the w25c mechanism).
