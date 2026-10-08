# w14l RESULTS (mass variant testing on w14f's two ready singles)

Builder scratch: `~/rush2049/scratch/frontier/w14l` (base copy; src/blob, include, tools/cloud, asm/us/blob,
blob_matched.lock.json synced; trace toolkit `--reuse ~/rush2049/scratch/frontier/wtk`). Tag `w14l`.
Nothing here is a MATCH. No group, claims, overrides, cloud/matches entries, and no splice.

## Results table

| Function | Bytes | State | Flags | Best scorer output (verbatim, whole-program unit) |
|---|---|---|---|---|
| `entity_lod_select` | 716 | not matched; residual is register colour only (2 forced colours give ops 0 and words 2 = jump-table reloc) | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL entity_lod_select: 5 of 179 words differ` (`entity_lod_select/best.c` = r2 v0220; words 7, ops 0, norm 0) |
| `physics_float_calc` | 1176 | not matched; no improvement this wave (best unchanged = w14f `q_i_first.c`) | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL physics_float_calc: 180 of 294 words differ; compiled body is 288 words, target 294` (`physics_float_calc/base.c`) |

Unit score of `entity_lod_select/best.c` is from `us.sh` (`blob_unit --tag w14l score`) on the Pi, one run. The
standalone bscore ranking gave 5 for the same file.

## Batch log (vbatch, bscore standalone; `strict` = differing words)

Templates and variant dirs are under `entity_lod_select/` and `physics_float_calc/`.

entity_lod_select (base at round 1 = w14f `best.c`, 36 strict):
- R1 `t1.c` (12 choice points: decl order, empty `if (first)` / `if (cmd[0]..)`, `0x40 == (op&0xC0)`, val operand order, `was = first`, `type` / `idx` casts, `!` / `== 0` mask test, `nx` form), 300 variants (`b1/`). Best three: `strict 7` (v0210), `14` (v0083), `14` (v0085). Best v0210 choices: `if (first) {}`, `0x40 == (op & 0xC0)`, `val` order flipped, `was = first`, `nx` as `if (op != 0xDE && depth >= 0)`.
- R2 `t2.c` (16 choice points; `op2` placement, `while` forms, `(u16)` casts, `1u <<`, `depth = depth - 1`), 400 variants (`b2/`). Best three: `strict 5` (v0220), `7` (v0000 = R2 base), `42` (v0287). v0220 = `if (op2) {}`, `!!first`, `1u << idx`, `depth = depth - 1`, `nx` nested form. Unit: 5 of 179 (above).
- R3 `t3.c` (23 choice points, `w1` direct vs cached, `(u32) base` / `seg` forms, `was` removed with `if (first)`), 400 random (`b3/`): best `5` (v0000), rest 107 and worse. OFAT around t3 (`b3o/`, 35 variants): all 5 or worse. Pairwise (`b3p/`, 543, top 400 printed): best 5. No improvement.
- R4 `t4.c` (+ `stk` indexing forms `*(stk+depth)`, `(u32) stk` cast, `stk[1]` copy), OFAT + pairwise (`b4p/`, 721, top 600 printed): best 5. No improvement.
- R5 `t5.c` (+ compiled-out `if (stk) {}`-style reads at the loop head and after `stk[0]=dl`), 500 random (`b5/`): best 5, rest 109 or worse. No improvement.

Stop rule: R3, R4 and R5 gave no improvement over 5, so the function is stopped on the stop rule.

physics_float_calc (base = w14f `q_i_first.c`, standalone 186 = unit 180):
- R1 `t1.c` (10 choice points: decl order, `i = 0` / `n` order, `obj` form, `if` condition forms, `mask++`/`bit = 1` order, `bit` shift form, `obj->flags` form, `dbg` block removal, empty `if (n|bit|i) {}`), 300 variants (`b1/`): best `186` (base), then 334+ for structural breaks. No improvement.
- OFAT + pairwise (`b1p/`, 87): best 186 (base). No improvement.

## Oracle (forcing), entity_lod_select

- Snapshot `r2v220` (`ctrace.sh`, `force.sh`): single-web forces (`force_r2.txt`, 12 webs x c1/c2/c3): best 4 rows (`p1:w32=c3`).
- Pairs (`force_pairs.txt`, 55 pairs): `p1:w25=c2,p1:w32=c3` gives `differing rows 2`; the diff is the own-rodata jump table only
  (`lui at,0x8012`/`lw t7,14960(at)` vs `lui at,0x0`/`lw t7,172(at)`), so the rest is colour-only.
  Colour meaning: w32 is a pointer-typed web (`type=4`, `bb=-1`, probably the `stk` base) and retail keeps it in a0.
  The forced pair is not a source lever yet. Source variants that changed `stk` indexing (R4) did not move w32.
- Next lever (untried): find the source expression for w32 (`type=4`, global-level, 2 uses) and w25 (`type=3`, bb=5, `0xffffffe7`)
  from `webdetail` with a source-line map. Try a pointer-local for `stk`, or give `dl` a local alias.

## physics_float_calc

- Snapshot `pf0` colouring: w158 (`v0`, save 10.2) and w167 (`v1`, save 8.2) are the loop-2 webs; forcing w167=c1
  is declined (`forbidden=0x5002000000000000`), forcing w158=c1/c2 gives a worse positional count (214 rows).
  The base has 288 words against 294 retail, and 27 ops rows, so the residual is not colour-only. Colour work here
  does not reach the body length first; the loop-2 structure (288 vs 294) comes first.
- Retail loop-2 (`sll t5,v0,1`, `addiu v1,v1,1`) shows the swap, but a source form that fixes the swap alone
  was not found in ~400 variants.

## Integration notes

- No overrides, no superseded groups, no claims, nothing for cloud/matches. Neither function is ready.
- `entity_lod_select/best.c` (r2 v0220) carries the w14f decl pads (`pad0`, `pad1`, `pad[20]`) as before; the
  choice `if (op2) {}` is a compiled-out read (disclosed here, a shaping device, not semantics). Fold the choices in before
  any promotion, and recheck the unit with `blob_unit --tag w14l score entity_lod_select --neighbours`.
- Files: `entity_lod_select/best.c`, `entity_lod_select/t1.c`..`t5.c` (templates), `entity_lod_select/b*/` (variants and
  `score.txt`), `entity_lod_select/ofat.py`, `ofat2.py`, `force_r2.txt`, `force_pairs.txt`; `physics_float_calc/base.c`,
  `physics_float_calc/t1.c`, `physics_float_calc/b1/`, `physics_float_calc/b1p/`.

## Generalises

- The vbatch standalone `strict` count and the unit `words` count can differ by a few words (physics base: 186 vs 180).
  Compare variants only within one scorer.
- A single forced pair (`w25=c2`, `w32=c3`) can show that a residual is colour-only when no single force does.
  Sweep pairs among the webs with the highest `save` after the first force round.
- Random sampling of 20+ choice points is poor (most samples are 100+ words off). One-factor and pairwise sweeps around
  the best base are better once the base is fixed.

## Follow-up (coordinator request): entity_lod_select only

### Web mapping (what the residual is)

- `workbench diagnose` (target = retail words from `asm/us/blob/blob_800966d8.s` assembled to ELF, `diag/target_entity_lod_select.o`;
  candidate = unit build of `best.c`, `diag/cand_r2v220_unit.o`; objdump = `mips-linux-gnu-objdump`, Pi):
  `verdict=structure-mismatch aligned_total=9 words=6 ... insns=179`, but the structure is a count artefact: the
  target is 179 words plus 16-byte .text padding (the instruction-count warning). Aligned classes: register 5,
  constant 2 (the jump-table `lui`/`lw` relocations), structural 2 (the same two relocation sites).
  FIRST divergence: pool slot 6 at aligned row 27, `and v1,t8,t5` (target) vs `and v0,t8,t5` (ours).
  - workbench **w1** (v1 -> v0, rows 27-28): the `cmd[0] & 0xFF000000` temp that feeds `srl ...,24` to make `op`.
  - workbench **w2** (a0 -> v1, rows 30, 31, 34): the `op & 0xC0` temp used by the first `if` (`andi a0,t6,0xc0`).
  - The workbench names these with its own web labels. They are not uopt web numbers.
- Mapping uopt webs to these: `p1:w25=c2` (bb 5, type 3, `save 20`) and `p1:w32=c3` (`save 15`) is the forced pair that
  gives the retail code (2 rows, rodata relocation only). The trace records give no source line for w25 or w32 (`line=2`
  for most webs, `line=-1` for w32). w32's raw words (`0x1021db48`, `0x1025aba0`) look like table addresses, so
  **the earlier `stk`-base guess for w32 is not supported**. The mapping I can defend is: w25 = the and-temp (w1), and
  w32 = the andi-temp (w2), by the forced result.
- Guide: `python3 tools/workbench.py guide` shows levers 7-13 (dead-read to take a pool slot, read count as priority, split a
  chain with `if (!x);`, mask into a parameter), and lever 9 (read count) is the one that targets priority between
  tied saves. Law L67 says operand order at a comparison is a readout, not a lever.

### Batch rounds 6-8 (all standalone bscore, `strict` = differing words)

| Round | Template | Variants | Best three `strict` | Note |
|---|---|---|---|---|
| R6 | `t6.c` (choices on `cmd = stk[depth]` forms, `op` and-spelling, `(op&0xC0)` operand order, `if (stk) {}` / `if (depth) {}` at stk[0] and loop head, `if (stk) {}` before `if (seg<0)`) | 324 (`b6/`) | 5, 5, 5 | `op = cmd[0] >> 24` (no and) and `(cmd[0]>>24)&0xFF` break it (178/179). `if (stk) {}` placements are neutral. `if (depth) {}` breaks it. |
| R7 | `t7.c` (`pad0` as the and-temp (`pad0 = cmd[0] & 0xFF000000`) and `pad1` as the andi temp) | 600 random (`b7/`) + 81 one-factor/pair (`b7p/`) | 5, 5, 5 | pad0 spelling with `op = pad0 >> 24`: unit 67 words (`v0027`, `v0001`: `FAIL entity_lod_select: 67 of 179 words differ`). A second read of the and-temp costs structure, not colour. |
| R8 | `t8.c` (`stk` declared first / last / before `cmd`, `stk[0]=dl` with `if (stk) {}` before/after, `stk[depth] += 2` vs `2 + stk[depth]`) | 126 one-factor/pair (`b8p/`) | 5 (40 variants), then 27 and 179 | No `stk` declaration or placement lever moves the and-temp colour. |

Stop rule: R6, R7 and R8 gave no improvement over 5 (the same floor from R2), so the function is stopped.
The ten `strict 5` results in R6-R8 are all the same floor: the two rodata relocation words and the two colour
differences (w1 v1/v0, w2 a0/v1).

### Unit confirmation

- No strict 0 was reached in any round, so no unit confirmation of a match was needed.
- Unit verdict kept at the last run: `FAIL entity_lod_select: 5 of 179 words differ | words 7 ops 0 norm 0 | frame 232/232`
  (`us.sh` on `entity_lod_select/best.c`, one run this session).
- Not a match. No `cloud/matches/entity_lod_select.c` written, and no group, claims or overrides. The `strict 5` file is
  `entity_lod_select/best.c` for integration notes only.

### Round 9 (the next lever, now tried)

- `t9.c` = `t8.c` plus a compiled-out `if (cmd[0] & 0xFF000000) {}` (or `if (depth) {}`) before the loop, moving the and-temp's first appearance. OFAT + pairs (`b9p/`, 160 variants): best three `strict 5`, 5, 5 (same floor). No improvement; the function is stopped.

### Next lever (untested beyond R9)

- The residual is priority at a tie: w25 and w97/w103/w133 have `save=20`, and ties go to the lower web number
  (first appearance). The and-temp is web 25, so a statement that makes the and-temp appear before the others should move
  it. The only tested spellings (`pad0` split) change the frame and structure (67 words), so the next try is a split
  that does not add a store: put `(cmd[0] & 0xFF000000)` in a compiled-out `if (...) {}` before the loop, so its first
  appearance moves, and check the frame stays 232.
- Also untried: `dl` copied into `stk[0]` through `(u32 *) dl` and `stk` declared as an array of `s32`-sized slots.
