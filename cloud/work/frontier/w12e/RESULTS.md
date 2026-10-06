# w12e results (wave 12): small near-misses

Builder scratch `watchman2:~/rush2049/scratch/frontier/w12e` (fresh copy of `base`; src/blob, include, tools/cloud,
asm/us/blob and blob_matched.lock.json rsynced from the Pi). Trace toolkit installed with
`tools/trace/install.sh … --reuse wtk` (uopt/ugen/as1 OK). At most 2 cores. Nothing committed, spliced or pushed;
nothing edited outside this directory and `cloud/matches/sound_stop.c`. No permission denials.
Unit scores: `python3 -m tools.conveyor.pipeline.blob_unit --tag w12e --jobs 2 score NAME --with FILE --neighbours`
(wrapper `vr.sh`).

| Function | Bytes | State | Flags | Scorer output (exact) | File |
|---|---:|---|---|---|---|
| sound_stop | 160 | **strict MATCH** (was 12/40, ~150 earlier variants) | `-g0 -O3 -mips2 -G 0 -non_shared` (also MATCH at -O2) | builder `score.py fn cand/sound_stop.c sound_stop`: `sound_stop:` / `MATCH`; unit: `EQUAL sound_stop: 40 words (kept, c_sound_stop.c)` / `locked bodies that differ in this unit: 0` / `blob_unit score: 1/1 equal` | `cloud/matches/sound_stop.c` |
| func_80096130 | 264 | 3 words off (was 21); residual proven to be one ugen `.alias` directive | unit, -O3 | `FAIL func_80096130: 3 of 66 words differ` / `locked bodies that differ in this unit: 0` | `func_80096130/best.c` |
| audio_channel_setup | 332 | 3 words off (unchanged); mechanism identified (as1 move coalescing) | unit | `FAIL audio_channel_setup: 3 of 83 words differ` / `locked bodies that differ in this unit: 0` | `audio_channel_setup/best.c` (= w11h) |
| object_bytes23_sum | 72 | 4 words off (unchanged); w11h's func_80096288 hypothesis refuted by retail evidence | unit | `FAIL object_bytes23_sum: 4 of 18 words differ` | `object_bytes/best.c` |
| object_bytes_sum_global | 84 | 8 words off (was 11) | unit | `FAIL object_bytes_sum_global: 8 of 21 words differ` / `locked bodies that differ in this unit: 0` | `object_bytes/best_sum_global_v2.c` |
| func_800E7A98 | 172 | 14 words off (unchanged) | unit | `FAIL func_800E7A98: 14 of 43 words differ` / `locked bodies that differ in this unit: 0` | `func_800E7A98/best.c` |
| audio_channel_priority | 468 | 11 words off (unchanged); colour-only proven from w4a `stride.c` (6 forced colours, 0 rows) | unit | best stays w4a/w2h `best.c` 11/117; `stride.c` 24/117 | `audio_channel_priority/stride.c` |
| func_8010D680 | 476 | not attempted beyond a check (w10f b.c: 22 rows, 2 unverified rodata relocs) | - | - | - |

## sound_stop: strict MATCH

```
builder: score.py fn cand/sound_stop.c sound_stop --flags "-g0 -O3 -mips2 -G 0 -non_shared"   ->  sound_stop:  MATCH
         (same file, -O2)                                                                       ->  sound_stop:  MATCH
python3 -m tools.conveyor.pipeline.blob_unit --tag w12e --jobs 2 score sound_stop --with cloud/matches/sound_stop.c --neighbours
  EQUAL sound_stop: 40 words (kept, c_sound_stop.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w12e/unit.o (5.8s)
```
Semantics: release a linked list of voices: find each in the active pool `D_80149450[0..D_80149788)`, set byte 22 of
its 32-byte `pad_config[voice->slot]` record to 2, swap-remove it (count--, pool[i] = pool[count], pool[count] =
voice). Shaping, from the trace (`ctrace.sh`, all webs phase 2 = web-number order, lowest free colour):
- a compiled-out `if (i) {}` right after the search loop puts the `i*4` shift first (exit path and the `beql` delay
  slot), before the slot load;
- the slot is a named local `s` (register web, `lhu v0`);
- w5a's compiled-out `if (D_80149450[i] != voice) {}` must go: it made `&D_80149450` the first address web, so
  phase 2 handed a3/t0/t1/t2 to pool/pad/2/count instead of retail's pad/2/count/pool.
Integration: plain kept single, `splice_singles.py sound_stop` (flags on line 1). No unit_overrides, no rodata.
Several locked callers (UpdateActiveObjects, sound_control, …) react to sound_stop's shape (a static-helper split
breaks 8 of them); this file leaves all of them EQUAL. The caller-less stub `func_800B3584` stays as locked.

## func_80096130: 21 -> 3 words; the last residual is one `.alias` directive

- `if (slot) {}` placed right before `slot->allocation = 0` lifts the slot web's totalsave above bestcost (no more
  split): slot = v1, allocation = a3, reloads into v1, exactly retail's colours (`f130/d5.c`, 11 words).
- d5's remaining rows: as1 sinks `sw zero,12(v1)` into the first memset's delay slot. In the ugen listing
  `.noalias $3,$sp` is in force until `.alias $3,$sp`, which ugen emits at the end of the block holding the slot
  web's last use (after `jal memset`). **Oracle:** the d5 listing with one `.alias $3,$sp` inserted before the
  post-osJamMesg reload reassembles to `differing rows 0` (`asm.sh d5 d5_alias.s` -> `want 66 words, got 66;
  differing rows 0`; listing in `func_80096130/d5_alias_oracle.s`).
- A block boundary right after the store (`if (0) {}`, best.c / alt_g3.c) gets that `.alias` placement and fixes the
  order, but then the `index*8` CSE web shares no block with the slot web, so `f_spilltemps` reuses the slot's home:
  `sw/lw t7,36(sp)` instead of retail's 32(sp) (3 words; `spill.sh` shows `HOME bit=23 home=0 reused=1`). With the
  slot in the memset block (d5) the homes are right (CONFL 21 vs 5, temp 1 = sp+32) but the alias is late.
- best.c replaces w11h's `dma_wait` static with a named local `off = index * 8` (frame 72 either way).
- ~45 variants (`f130/`). Next hypothesis: a source where the slot web's last block ends at the store while the
  idx*8 web still has an occurrence in a block that the slot web touches (e.g. the first memset's address computed
  in the store's block before a boundary; `off` was copy-propagated past the boundary).

## audio_channel_setup: 3 words, mechanism identified

Retail `lui a2; addu a2,a2,t2; lhu a1,%lo(a2)` (temp a2, value a1). All matched instances of this pattern
(func_8008B640, func_800F43B8, physics_velocity_integrate_a) come from as1 copy coalescing: ugen emits
`lw $X, sym($i)` followed by `move $Y, $X`; as1 renames the loaded value to $Y, deletes the move and keeps $X as the
macro temp (verified on func_8008B640 and func_800F43B8 listings: ugen `lw $2,D_8012E708($24)` + `move $5,$2`, final
`lui v0 … lw a1,-6392(v0)`). In every matched case $Y is an outgoing argument register. So retail had the tex value
in a2 (coloured web) copied into a1. Not found: inlined getters/setters (`tex_get`, `ent_set`), `mode` reuse, copies,
compiled-out ifs, CSE double reads (~20 variants, `acs/`); all either propagate the copy away or move colours.

## object_bytes23_sum / object_bytes_sum_global

- **w11h's hypothesis is refuted by retail.** uopt's caller-saved cost (`f_cupcosts`) charges a register only if the
  callee's used-register byte array (proc+36, filled during its colouring and merged from callees) marks it. Retail
  `state_utility` keeps x/y in **t2/t3 across `jal sound_update_channel`** (and t1), and `func_800BB9B0` keeps t1
  across `func_80096288`. So in retail sound_update_channel/func_80096288 do not kill t2/t3; a different
  func_80096288 body cannot be the lever. The t4 must come from interference inside these two functions (two webs
  live across the second call coloured t2/t3 that emit no code), or something not yet identified.
- uopt processing order is call-graph order regardless of file order (moving the two bodies before
  sound_update_channel in one file: ordinals unchanged, same rows).
- object_bytes_sum_global 11 -> 8: retail's `lb v0` / `lbu v1` are coloured webs, so the source has locals:
  `best_sum_global_v2.c` (`a = byte3_get(); b = byte2_get(); g = D_80149B60; return g + b + a;`); remaining: v0/v1
  swapped (g/b) + the t4/t2 address register. Note `credits_scroll` (locked) inlines object_bytes_sum_global; some
  spellings break it (`--neighbours` shows it).
- Extra named locals cost frame (24 -> 32/40), so codeless extra webs must be unnamed. ~15 variants (`ob/`).

## func_800E7A98: 14 words (unchanged)

Retail also inlines the heap lock/unlock statics (`func_80095CF4`/`func_80095CFC`, as func_800960CC's callers do);
using them gives the same 14 words (`best.c`). An inlined `heap_unlink(h)` helper (`e7a/a5.c`, 15 words) loads the
list head on both paths as retail but colours h straight into a1. ~7 variants.

## audio_channel_priority: colour-only, needs weights' web number < row's

`ctrace.sh` on w4a's `stride.c` (parameter used directly as id; 24 words): all webs phase 2. Forcing
`p2:w3=c3,p2:w9=c4,p2:w10=c5,p2:w12=c6,p2:w15=c7,p2:w28=c2` gives `differing rows 0`. In phase 2 the only change
needed is that the weights expression web (w28) be numbered before the row variable (w3), so that it takes v1 and
row falls to a0; then every other web lands on retail's register by lowest-free. Dead `weights = …` stores and
compiled-out reads of `weights` create a separate uncoloured variable web (copy propagation keeps the real uses on
the expression web). ~6 variants.

## What generalises

1. **as1 copy coalescing explains "macro temp ≠ destination".** `lui X; addu X,X,i; lw Y,%lo(X)` with X≠Y, X≠at
   means ugen loaded into X and had a `move Y,X`; as1 renamed. In matched code Y is always an outgoing argument.
2. **ugen `.noalias`/`.alias` pairs bracket each coloured address web**, emitted at the end of the block of its last
   use; as1 reorders stores through that register past `$sp` accesses while `.noalias` holds. Use `asm.sh` with a
   hand-inserted `.alias` to prove such residuals (func_80096130: 0 rows).
3. **Compiled-out uses of an index (`if (i) {}`) reorder ugen's post-loop evaluation** (sound_stop), while a
   compiled-out read of a global array creates that array's address web early and permutes all phase-2 address webs.
4. **A caller-saved register held across a call in any retail caller proves the callee does not kill it**
   (`f_cupcosts` reads the callee's used-register array). Check other callers before blaming a callee's body.
