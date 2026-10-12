# w25d RESULTS (wave 25, game near-misses, IDO 5.3)

No MATCH: nothing for cloud/matches, no group dir, no unit_overrides, no data-symbol changes. Work bases only.

| Function | Bytes | Before -> after | State | Scorer line (Pi unit, current tree, `--tag w25d`) |
|---|---:|---|---|---|
| entity_render_setup | 2048 | 8 ops rows -> 8 | near-miss, stopped; SPL trace done, h_d route refuted, natural block-17 occurrence found | `us.sh`: `best.c: FAIL entity_render_setup: 420 of 512 words differ; compiled body is 513 words, target 512; +0x354: .rodata+0x1 \| words 52 ops 8 norm 8 \| frame 184/184` |
| func_80090308 | 1128 | 198 rows / ops 87 -> unchanged best (198/87); c1 lever gives ops 85 but 214 words (frame 184 vs 272) | near-miss, stopped | `FAIL func_80090308: 210 of 282 words differ; +0x1a8: .rodata+0x118: re[tail ...]` / `locked bodies that differ in this unit: 0` \| words 198 fr 272/160 ops 87 |
| entity_physics_update | 712 | 21 words (w24e) -> unchanged; measures 23 aligned word rows, ops 0 | colour-only, stopped; helper route refuted | same command: `FAIL entity_physics_update: ...` \| words 23 fr 112/112 ops 0 norm 0 |
| save_write_data | 1200 | 223/300 -> unchanged | frame 352 vs 192 analysed only | `FAIL save_write_data: 223 of 300 words differ; compiled body is 303 wo[rds ...]` \| words 153 fr 352/192 ops 33 |

Command for the group rows: `cloud/work/frontier/w25d/f308/q.sh FN FILE` = `blob_unit --tag w25d --jobs 2 score
func_80090308 entity_physics_update save_write_data --with FILE --with f308/swd.c --internal func_80090308 --neighbours`
(+ udiff row counts); every run printed `locked bodies that differ in this unit: 0` except where noted.

Targets: entity_render_setup (8 rows; start w24d best.c, SPL=1 trace on h_d), func_80090308 group +
entity_physics_update + save_write_data (start w24e best_effects.c / swd.c).
Builder scratch `watchman2:~/rush2049/scratch/frontier/w25d` (toolkit reused from wtk; oracle uopts copied from
w23e/w24d/w24e scratch bins).

## entity_render_setup (0x8008C884, 2048 B) — unchanged at 8 ops rows (in progress notes)

Start: w24d best.c (= w22f), re-scored: `best.c: FAIL entity_render_setup: 420 of 512 words differ; compiled body is 513
words, target 512; +0x354: .rodata+0x1 | words 52 ops 8 norm 8 | frame 184/184`.

### SPL=1 trace on h_d (the requested next step)
`tools/oracle.sh hd "SPL=1 CDX_PROC=100 CDX_FORCE=p1:w367=s" uopt_ulr` (uopt_ulr from w24d's scratch):
`[SPL] list lr=367: 0[o1] 19[o3] 20[o3] 28[o3]` / `[SPL] split lr=367 sym=367 start bb=0`, then the BFS visits and
ADDS every block 1,2,3,5,4,8,9,17,10,18,28,11,12,19,20,15,13,14,16 (regsleft 18 throughout, no reject). So with the
default-colour copy through the palette, the LR is live from block 0, the split starts at the block-0 occurrence (first
livbb with occurrences in list order) and the first piece is the whole LR again. **The h_d route cannot yield the
{17,28} piece**: whatever puts a palette occurrence in block 0 makes block 0 the split start. Dead end.

### A code-free occurrence in block 17 (new, natural form of w23e's "ADD 17")
`if (D_8011B558) {}` right before `if (vehicle->mode == 2)` (v/k5.c; also `if (D_8011B558[4].a) {}`, k4) compiles to
nothing but leaves an lda occurrence in block 17: `[SPL] list lr=375: 17[o1] 21[o3] 22[o3] 30[o3]` (blocks renumbered
+2), one LR coloured v1 with the load in block 17 — exactly w23e's ADD-17 oracle state: `k5.c: ... | words 54 ops 10 norm
10 | frame 184/184` (sub-arms lose their reloads because the gap block is filled after colouring). `if (D_8011B558[4].r) {}`
(k1) is not compiled out (PRE keeps the load: 13 ops); `&D_8011B558[4]` (k6) and a bare `D_8011B558[4].r;` (k3) are inert.
Forcing the split (`force.sh k5 ... p1:w375=s`) starts at 17 (the o1 block) and again takes everything: 9 ops.

### Why the natural split cannot happen in this shape
p1dec for the palette LR: `save=3.0 nocs=3 totalsave=9.0 bestcost=0.000000 bestreg=v1 regsleft=21`. bestcost is 0
because v1 is a free caller-saved register and no call lies inside the LR, so `totalsave <= bestcost` never holds; and
f_split starts at the first livbb WITH occurrences, so an o1 block 17 is always the start. w23e's oracle worked because
its ADD livbb had o0. Conclusion: retail's sub-arm reloads are very probably NOT split pieces of the else-arm LR; the
sub-arms and the {17,28} else-arm LR must be different register candidates in retail (different lda keys), with the
else-arm LR containing 17 and coloured whole.
k4 (ctx3 holder `.a`) gives the same merge as k5 (ctx0 holder): the sub-arms lose their reloads in both, so the
ilod-offset/ixa contexts do NOT keep separate LRs once block 17 holds an occurrence — w24d's "two candidates by
context" only holds while the occurrences are disjoint. Stopped here (stop rule: w23e 2.4 h + w24d 1.5 h + ~1 h here).
**Best next hypothesis:** retail's else-arm LR {17,28} and the sub-arm loads are different lda keys (a different
symbol/offset for one of them, e.g. the sub-arms through a separately declared palette view, or the else arm through a
symbol whose lda base differs but whose reloc resolves to 0x8011B558), coloured whole; or a gap-fill exception for
block 18. Test with `[SPL] list lr=` showing two LRs, without CDX_FORCE.

## func_80090308 group (+ entity_physics_update, save_write_data) — ~1.5 h, stopped (stop rule: 4 rounds, no strict gain)

Start: w24e `best_effects.c` (198/87, = w24e x4) and `swd.c`, copied to `f308/`.

### fx_ok helper, rebuilt properly (w23d's h1/h2 were self-recursive)
`f308/mkh.py` writes 20 variants: five helper bodies (A `return a && (b||c);`, B if/return 1/0, C three early
returns, D nested if, E `r` variable) x defined before/after x used at all three sites / only in entity_physics_update.
entity_physics_update: **every form is worse than the inline condition** (A 40-65 words, B/D 34, C 38, E 32; inline
form 23 rows ops 0). func_80090308 with A at its two sites: 185 rows but body 265 words, ops 119 and **1 locked body
differing (entity_spawn_init)** — not an improvement. The fx_ok-helper hypothesis for the 6 missing blocks is refuted.

### Pointer-parameter colour setter (w24e lesson 2, applied)
`static void color_set(Color *c,u8 r,u8 g,u8 b,u8 a)` inlined for the extra-block colour (`f308/c1_colorset.c`):
reproduces retail's constant sharing exactly — `li a0,255` used for the three byte stores and the 95 shared between
`extra.alpha` and `color.a` (ours a1, retail t7) — ops 87 -> **85**, but words 198 -> 214 because the frame moves
160 -> 184 (24 B inline area). A 3-byte setter + separate `color.a=95` (c2) gives 255 shared, 95 not (197/86,
frame 176); using it for the 244/205/20 init too (c3/c4) is worse (227/228). Source position between the scale
stores (c5) is inert (as1 reorders). So the extra-block colour IS written through an inlined pointer-taking helper
(5-arg form), consistent with the frame needing more inline area.

### Seed reload before rand4 (open) and loop store order
- `do {} while (0);` before rand1 / before the stretch stores / before the timer (d1-d3): 196 rows, no reload — a
  block boundary between rand3 and rand4 is NOT the cause (uopt forwards the seed across blocks).
- An inlined `particle_spin(Particle *q)` holding a,b,c and the three stores, called with `p` or with
  `&D_80154660[car].particles[i]`, with or without the timer (r1-r4): code identical to c1 (ops 85) or worse; frame
  +16. No reload either: an inlined pointer parameter is copy-propagated, so its stores do not kill the seed.

### Frame: a shared pattern with save_write_data (new)
save_write_data's retail sp references: named slots at 340, 330, 324 and 268 (frame 352): **56 B (14 words) between
324 and 268**, like func_80090308's 52 B (13 words) between node (268) and color (212). Both functions share callees
(func_80090284, math_utility, func_8008E26C, entity_spawn_callback) and the Node/particle setup. Best next hypothesis:
both declare the same 12-word local (an arcade `MATRIX` = `F32 uvs[3][3]; F32 pos[3]`, 48 B) plus one/two words,
never referenced in the final code (copy-propagated or dead), i.e. one shared idiom from a common ancestor (a macro
or a block-scoped temp matrix). It would be acceptable only as the exact frame residual of otherwise matching
bodies; test by adding `f32 mat[12];` (block-scoped) to both and checking that ONLY the frame/sp offsets change.

### entity_physics_update
Unchanged: the state web (save 2.5) still beats func_80090770's inlined parameters (2.0). With the helper route
refuted, the remaining natural candidate for 6 more live blocks is the inlined callee structure itself; no new lever.

## Files
- `entity_render_setup/best.c` (= w24d best, unchanged), `entity_render_setup/k5_holder17.c` (natural block-17
  occurrence, 10 ops), `entity_render_setup/v/` (k1-k6, h_d).
- `f308/best_effects.c`, `f308/swd.c` (= w24e), `f308/c1_colorset.c` (colour-setter lever), `f308/mkh.py`
  (helper generator), `f308/us3.sh`, `f308/q.sh`, `f308/v/` (all variants).
- `tools/oracle.sh` (uopt_ulr/uopt_spl oracle driver, w25d scratch), `tools/oracle_or.sh` (w17f uopt_or).

## What generalises
- **f_split starts at the first livbb WITH occurrences** (list order), and with free caller-saved registers
  (bestcost 0) a constant LR without a call inside is never split naturally. A retail pattern "load in a branch
  block for one arm, reloads in the other arms" is then two candidates, not one split LR.
- **`if (GLOBAL_ARRAY) {}` is a code-free lda occurrence**: it puts an address constant's LR into a chosen block at no
  code cost (`if (D_8011B558[4].r) {}` is NOT compiled out: PRE keeps the load). Useful as an ADD-oracle stand-in.
- **An inlined `T *` setter reproduces retail constant sharing for byte stores into a local** (`li a0,255` x3, a
  constant shared with a global store): confirmed w24e lesson 2 in source, cost = the helper's inline area.
- An inlined helper's pointer parameter is copy-propagated: stores through it do not kill unrelated globals.
- A fx_ok-style condition helper materialises or reshapes branches; for a repeated multi-term test, the inline form
  stays best (5 bodies x 4 placements measured).

Permission denials: none. Builder: own scratch only (`~/rush2049/scratch/frontier/w25d`), serial unit runs, <= 2 jobs.
