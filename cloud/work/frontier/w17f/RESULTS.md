# w17f RESULTS: entity_spawn_init (0x8008EA10, 5544 B, 1386 words, -O3)

| function | bytes | state | flags | scorer output (Pi, `blob_unit --tag w17f score entity_spawn_init --with <file> --neighbours`) |
|---|---:|---|---|---|
| entity_spawn_init | 5544 | NONMATCH, 61 words (unchanged); frame 728 = retail | -g0 -O3 -mips2 -G 0 -non_shared | `entity_spawn_init/best.c`: `FAIL entity_spawn_init: 61 of 1386 words differ` / `umerge inlined into it: func_8008B2E4` / `locked bodies that differ in this unit: 0` |

Nothing to integrate: no match, no group, no cloud/matches file, no unit_overrides, no data-symbol changes.
`entity_spawn_init/best.c` is w16b's best.c unchanged (no source beat 61). `entity_spawn_init/base_alias.c`
(653 strict alone) is the recommended base for the next attempt (see 2). Stop rule reached (~3 h of trace work).

## 1. Oracle proof: what the function top needs (diagnostic, not match evidence)
New tool `tools/prio_patch.py`: a p1 priority-scale oracle for the traced uopt (`PRIO=web:mul` multiplies a web's
selection priority, so it is decided earlier/later with unchanged uses) plus `SAVELOG=1` (prints f_compute_save's
inputs per live range: each livbb's bb/uses/defs/weight/flags, the block-vector card, nocs, tot, save, and the
block vector). Built on top of w16d `patch_spl.py` + w16b `splitpatch.py` into builder
`~/rush2049/scratch/frontier/w17f/bin/uopt_or` (build steps: `python3 patch_spl.py src2/uopt.trace.c src2/u.c &&
python3 splitpatch.py src2/u.c && python3 prio_patch.py src2/u.c`, gcc as w16b). `tools/oracle.sh LABEL "ENV"` runs it
on a snapshot and diffs; `tools/sv.sh FILE LABEL` builds a variant and prints the model / constant-5 save lines.

| snapshot | oracle | rows (words) |
|---|---|---|
| s7 (= w16b probe_v7: `if (&surface) {}`, surface initialiser) | none | 600 |
| s7 | `PRIO=10:0.999` (model web loses its tie with the constant-5 web) | **29** |
| base_alias (surface aliased by `*(s16 *)&surface == 8`, no extra statement) | `PRIO=10:0.999` | **29** |
| best.c (surface not aliased) | `PRIO=10:0.745` | 558 (surface's top piece takes t1 first, model gets t2) |

The 29 rows are only the two spill homes (model 96 vs retail 92, type copy 88 vs 84: 8 rows) and the
relocation-blind `.rodata` rows that the scorer resolves. So the whole function top (≈45 of the 61 words) is
exactly two decisions: (a) `surface` never gets a register (address-taken; `volatile` is wrong, see 3), and (b) the
model address web (`ixa(D_8014A250, slot*2056)`) is decided **after** the constant-5 web. Any later position works
(0.999 down to 0.62 all give 29). With (b) the model's top piece grows into blocks 43/45/49 and is coloured t1,
as w16b predicted.

## 2. Why the model wins the tie, and the arithmetic of the fix
f_compute_save (decoded from uopt.trace.c, `L46efec`):
`tot = sum over livbbs (uses+defs)*weight - (store/load costs)`; `n = livbbs + card(block vector +0xc)`;
`nocs = n < 3 ? n : 2 + ((n-2) >> 2)`; `save = tot / nocs`; ties go to the lower web number (model 10 < constants).

| web | livbbs (bb:u d/weight) | card | n | nocs | tot | save |
|---|---|---:|---:|---:|---:|---:|
| model (s7) | 0:1 1/1, 181,132,124: 1 0/10 | 184 | 188 | 48 | 32 | 0.6667 |
| model (base_alias) | same | 182 | 186 | 48 | 32 | 0.6667 |
| const 5 | 18: 1 0/1 (f21, -1), 55,193,75: 1 0/10 | 171 | 175 | 45 | 30 | 0.6667 |

The constant's live blocks are a subset of the model's; the model has 12 extra blocks at the top (before the first
`type == 5` test). The tie flips (5 first) if any of: shared blocks -2..-4 (base_alias: model 184..182 -> 47,
5 173..171 -> 44: 0.681 vs 0.682); top-only blocks +4 on base_alias (+2 on s7); or 5's tot +1 **without** changing
5's own split. Measured:
- `if (type == 5) {}` before `do {` gives 5 tot 31 (save 0.689) and flips the order, but 5's own pre-loop piece is
  then coloured (`li a2,5`, 1385 words): wrong.
- `goto L; L:` adds exactly one top-only block (+1 card); 3 of them break the inline budget. `if (x) {}`, `switch
  (type) {}`, `do {} while (0)`, if/else/ternary/`return`->`break`/switch-`break` respellings, `type <= 4`,
  `type > 5`: no block change (cfe/uopt canonicalise) or break the budget.
- **Inline budget:** best.c + `if (&surface) {}` is at the umerge limit (one more statement drops one of the 19
  func_8008B2E4 sites, 1380 words). `base_alias.c` aliases surface through a use (`*(s16 *)&surface == 8`) instead
  of a statement: same code as s7, model card 182 (the `if` had added 2 top blocks), and room for about two goto
  labels.

## 3. Other findings
- `volatile s16 surface` + the oracle: 50 rows; volatile forbids hoisting the loop's `lh 666(sp)` into the `bne`
  delay slot (retail does), so retail's surface is address-taken, not volatile.
- Spill homes (the remaining 8 rows) are a separate structure question. Homes go to multi-block expression webs
  in bit order with first-fit reuse (`spill.sh`). best.c: car h0 (-628), model h1, D_801392D8 addr h2, its load h3,
  type copy reuses h3. Retail needs model at h2 and the type copy at h4. Two consistent explanations:
  (A) one more early multi-block expression web X that conflicts with car, model, the D_801392D8 address and the
  type copy; or (B) `car` is a non-propagated variable (no spill home) and the named area is 632 (8 B more fillers),
  giving model h0 = 92, D_801392D8 addr h1, load h2, type reusing h2 = 84. Dead early initialisers
  (`u32 *state = &D_801392D8[slot]` etc., 10 candidates) move the model home to 92 but keep the type copy at 88,
  so (A) is not any single existing expression; (B) is untested because I found no source form that stops uopt
  copy-propagating `car`.
- The model web in best.c is decided by the tie at save 0.6667; anything that changes the top block count must keep
  the constant 5's pieces unchanged.

## Best next hypothesis
Start from `base_alias.c` (surface aliased through a use, budget headroom). Find a code-neutral form that removes
2-4 blocks from the shared pre-loop/loop region (model and constant 5 both live), or adds 4 top-only blocks within
the budget. Test with `tools/sv.sh` (model nocs/5 nocs) and confirm with `tools/oracle.sh` (must reach 29 rows
without PRIO). Then do the spill homes (hypothesis B first: make `car` a real variable and add 8 B of named area).
Disclose the aliasing device either way; no natural reason for `&surface` was found.

## Notes
- Builder scratch: watchman2 `~/rush2049/scratch/frontier/w17f` (toolkit installed with `--reuse wtk`; snapshots
  st_b0, st_s7, st_x1, st_x2 and per-variant st_*). Variants are in `entity_spawn_init/v/`.
- No permission denials. Nothing written outside the lane dir and the builder scratch.
