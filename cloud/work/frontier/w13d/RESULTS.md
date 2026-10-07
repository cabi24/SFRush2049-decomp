# w13d results (wave 13): small traced residuals

Builder scratch `watchman2:~/rush2049/scratch/frontier/w13d` (fresh copy of `base`; src/blob, include, tools/cloud,
asm/us/blob and `blob_matched.lock.json` rsynced from the Pi; trace toolkit installed with `--reuse wtk`). At most
2 cores. Nothing was committed, spliced or pushed, and nothing was edited outside this directory. There were no
permission denials. Unit scores come from `blob_unit --tag w13d --jobs 2 score ... --neighbours` (wrapper `vr.sh`),
run on the Pi against the current tree.

| Function | Bytes | State | Flags | Scorer output (exact) | File |
|---|---:|---|---|---|---|
| differential_output | 648 | **EQUAL in the whole-program unit, and group MATCH** (was 13 words), as a superseding `frontier_level_objects` group | `-g0 -O3 -mips2 -G 0 -non_shared` | unit: `EQUAL differential_output: 162 words (kept, c_differential_output.c)` / `locked bodies that differ in this unit: 0` / `blob_unit score: 7/7 equal`; builder `score.py group`: all 6 members `MATCH` | `groups/frontier_level_objects/` |
| func_800AC660 (stub) | 8 | re-attributed: differential_output's node-flags getter. EQUAL. | same | `EQUAL func_800AC660: 2 words (internal, c_differential_output.c)` | same group |
| func_80096130 | 264 | 3 words (unchanged) | unit | `FAIL func_80096130: 3 of 66 words differ` / `locked bodies that differ in this unit: 0` | best stays `../w12e/func_80096130/best.c` |
| audio_channel_setup | 332 | 3 words (unchanged) | unit | `FAIL audio_channel_setup: 3 of 83 words differ` / `locked bodies that differ in this unit: 0` | best stays `../w12e/audio_channel_setup/best.c` |
| entity_process_main | 1424 | 2 words (unchanged; 2 more rows are unverified own rodata) | unit | `FAIL entity_process_main: 2 of 356 words differ` / `locked bodies that differ in this unit: 0` | best stays `../w11a/entity_process_main/best.c` |

New whole-program coverage: differential_output, **648 bytes**. It is not a `cloud/matches` single: it is EQUAL only
as part of the group, because it needs the re-attributed stub.

## differential_output: EQUAL (13 → 0)

Group dir `groups/frontier_level_objects/` supersedes the locked group of the same name and keeps all five old
members:
- `group.c`: a copy of the locked file with three changes (see "Integration").
- `differential_output.c`: the new claim, plus `func_800AC660`.
- `func_8008AE64.c`: a context copy of the locked kept entity getter, used only by `score.py group`.

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w13d --jobs 2 score differential_output transmission_ratio_get \
  engine_torque_calc engine_sound_update func_800AB7D0 func_800AC660 func_8008AE64 \
  --with $G/group.c --with $G/differential_output.c --with $G/func_8008AE64.c --internal func_800AC660 --neighbours
  EQUAL differential_output: 162 words (kept, c_differential_output.c)
  EQUAL transmission_ratio_get: 452 words (kept, c_group.c)
  EQUAL engine_torque_calc: 224 words (internal, c_group.c)
  EQUAL engine_sound_update: 357 words (kept, c_group.c)
  EQUAL func_800AB7D0: 2 words (internal, c_group.c)
  EQUAL func_800AC660: 2 words (internal, c_differential_output.c)
  EQUAL func_8008AE64: 10 words (kept, c_func_8008AE64.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/7 equal; object build/blob_unit/w13d/unit.o (6.0s)
```
(`G=cloud/work/frontier/w13d/groups/frontier_level_objects`.) Builder `score.py group cand/flo` (a copy of the
dir): engine_sound_update, engine_torque_calc, transmission_ratio_get (`own .rodata verified at
0x80121D40..0x80121D4C`), func_800AB7D0, func_800AC660 and differential_output are all `MATCH`. Context
func_8008AE64 is also `MATCH`.

**How the last 13 words closed.** All of them were colouring. In w12d's best.c the webs `parent` and the constant
`-1` had swapped s3 and s4. Fixing that exposed a second swap, `&D_80149D94` against `&D_80149D90` (s8/s7). Each
step below is traced with `ctrace.sh` (`dout/`):
1. **Four end-of-function `if(parent){}` reads.** These lower parent's priority to 54/13 = 4.15, below the
   constant -1 at 30/7 = 4.29, so -1 takes s3 and parent takes s4.
   - Three reads are not enough (`y2`: 13 rows).
   - The reads raise the per-procedure callee-saved cost to about 12–13. That splits the `&D_80149D94` web
     (tot 10), and the frame drops to 72.
2. **`if(D_80149D94){}` inside the first loop.** This raises that web to tot 20, so it stays coloured. It now ties
   with `&D_80149D90` at 20/7, and the tie goes to the lower web number (D94), which gives s7/s8 swapped (6 rows).
3. **First loop written as `if(current) { if(D_80149D94){} do {...} while(current); }`.** The preheader read adds
   one block to the D94 web, so it scores 20/8 = 2.5 against D90's 2.857, and D90 takes s7 while D94 takes s8.
   - With the read on its own line, 2 rows remain. They are as1 scheduling of the preheader `la` sequence: the
     read's `.loc` line splits the `lui/addiu` interleave.
   - Putting the read on the same source line as `do {` gives EQUAL (`x3`).
   - Ablation (`y1`–`y7`): the end-of-function D94 reads from the intermediate drafts are not needed. The final
     file has 4 parent reads, 1 in-loop D94 read and 1 preheader D94 read.
All of these are code-free compiled-out reads, and the file header discloses them (owner rule: shaping devices
disclosed). The guarded do-while is ordinary C.

### func_800AC660 attribution: settled (handoff §6 item 3)

- **It is differential_output's getter.** retail's `lw v0,64(s0)` … `jal; ori a3,v0,0xe00` needs an inlined getter
  of `node->flags`. The stub `func_800AC660` sits at 0x800AC660, which is exactly differential_output's end
  (0x800AC3D8 + 648).
- **There is no non-static body of a `node->flags` getter anywhere in retail.** A word search for
  `jr ra` / `lw v0,64(a0)` found nothing, so the getter must be a deleted static, and this is its stub.
- **transmission_ratio_get's inlined entity getter is not a stub. It is the kept function func_8008AE64**
  (0x8008AE64, locked single, `return D_8012E700[id].flags`, retail words
  `sll/sra/sll/addu/sll/lui v0,0x8013/addu/sw a0/jr ra/lw v0,-6400(v0)`). umerge inlines it across files,
  just as model_data_load and model_transform_setup show the same `D_8012E700[(s16)a].flags` read pattern.
- **Checks against the unit:**
  - transmission_ratio_get calling func_8008AE64 instead of func_800AC660 is EQUAL, with the same bytes.
  - A plain `D_8012E700[h].flags | 0x20` gives `FAIL transmission_ratio_get: 188 of 452`, so it does need an
    inlined getter.
  - A shared `u32 *` getter for both callers gives `FAIL transmission_ratio_get: 102 of 452` (refuted).
- **Conclusion:** w11e's attribution was a naming hypothesis and is replaced. The two getters are different
  functions: one is a deleted static (stub 0x800AC660), the other is a kept external.

### Integration (for the integrator; I edited nothing outside this lane)

1. **Supersede the locked group `src/blob/groups/frontier_level_objects/`** with `groups/frontier_level_objects/`
   (same name). The old members engine_sound_update, engine_torque_calc, transmission_ratio_get, func_800AB7D0 and
   func_800AC660 are all kept; differential_output is new.
   - `"claims": ["differential_output", "func_800AC660"]`.
   - `keep` adds differential_output; it also lists func_8008AE64, for the group scorer only.
   - `context: ["func_8008AE64"]`.
2. **`group.c` diff from the locked file:**
   - `u32 func_800AC660(s16 id);` becomes `s32 func_8008AE64(s16 id);`. This is the locked source's real
     signature (`s32 func_8008AE64(s16 arg0)`), not a false prototype.
   - The call becomes `func_8008AE64(node->handle) | 0x20`.
   - The `func_800AC660` definition is deleted.
   - The header comments are updated.
   - transmission_ratio_get's bytes are unchanged (EQUAL).
3. **`unit_overrides.json`:** the `func_800AC660` prefer_definition entry (file
   `src/blob/groups/frontier_level_objects/group.c`) must point at the group's `differential_output.c`, and the
   function stays internal. Its reason text should change to "node-flags getter of differential_output (w13d)".
   No new entries are needed for func_8008AE64: it is already kept and locked, and the unit inlines it.
4. **`func_8008AE64.c` in the group dir is context only.** It is a copy of the locked expression, needed so
   `score.py group` can inline it. Do not claim or splice it from here; the locked single stays authoritative.
   Drop it if the group path can take locked singles as context some other way.
5. There is no own rodata in differential_output. transmission_ratio_get's string literals are unchanged.

## func_80096130: 3 words (no movement, ~20 variants in `f130/`)

- The residual is the spill home of the `index*8` CSE web: retail puts it at 32(sp), we put it at 36(sp).
- `spill.sh` gives the numbering:
  - bit5 is slot, temp0 at 36.
  - bit7 is the `&D_8002EB70` address web, temp1 at 32.
  - The `index*8` web reuses temp0 because it shares no block with slot.
- For retail's 32, `index*8` must conflict with slot (share a block) but not with bit7. d5 gets that; its `.alias`
  then lands after the first `jal memset`, and as1 sinks the store. The `.alias $3,$sp` is emitted after the next
  call or at the end of the block, not right after the last use (d5 listing).
- New this wave:
  - `off` assigned before the store: copy-propagated (b1–b4).
  - Compiled-out `if (off) {}`: off becomes a variable web. The home is right (32), but the reloads go through v0
    and the `move v0,t7` disappears (a1, a5: 19–30 rows).
  - Compiled-out read of slot after the boundary: does not add to CONFL (a4).
  - Volatile store: as1 still sinks it (c1).
  - Inlined `clear3` helpers: frame 80 (e1–e3).
- Next hypothesis: retail's `index*8` occurrence is in the store's block with the off variable web still a dead
  copy (`move v0,t7`). Look for a construct that keeps a code-generating occurrence of `index*8` before the block
  boundary without making `off` a reloaded variable.

## audio_channel_setup: 3 words (no movement, 12 variants in `acs/`)

- `ctrace`: one tex web (w51, a1). Retail needs a loaded expression web in a2 plus a copy into a1, which as1 then
  coalesces (w12e mechanism).
- None of these moved it:
  - store/assignment order;
  - chained assignment;
  - double reads of `D_801427C0[f]`;
  - compiled-out reads of tex, id or the expression;
  - `s32 tex`.
  - They either stay at 3 rows or break the colouring (8–23 rows).
- This is not a colour-only residual (forcing tex to a2 would drop the a1 copy), so `force.sh` cannot prove it.

## entity_process_main: 2 words (6 variants in `epm/`)

- Spellings of the mask that gave no movement: `^ -1` / `-1 ^` (39–42 rows, an extra word) and a `(u32)` cast on
  the load (unchanged).
- New lead: `poly->flags = ~((1 << i) | ~poly->flags);` (`epm/c4.c`, 12 rows, ops 1) gives retail's evaluation
  order shift, load, not, which no and/not spelling gives. ugen then emits `nor(load)` + `nor` where retail has
  `nor(shift)` + `and`. So the order is reachable when the NOT is applied after the load is evaluated; look for a
  spelling with that order whose outer operation is an AND.

## What generalises

1. **An inlined getter in a locked group may be a kept external, not a stub.** When a caller needs an inlined
   getter, search retail for a full small body before assigning it a caller-less stub (here func_8008AE64:
   `return D_8012E700[id].flags`). umerge inlines kept functions across files with the same effect on frame and
   registers. A stub sitting directly after its only possible inliner is strong positional evidence.
2. **Breaking constant-web ties with a preheader read.** Loop-invariant address webs that tie on save (same tot,
   same nocs) can be separated by a compiled-out read in the loop preheader: it adds one block (nocs+1) to that web
   only. Write the loop as `if (p) { read; do {...} while (p); }`. Keep the read on the same source line as
   `do {`, otherwise its `.loc` splits as1's `lui/addiu` interleave of the hoisted constants.
3. **End-of-function dead reads lower a web's save towards 1.** Reads add +1 tot and, when they are in distinct
   blocks, +1 nocs. Several of them raise the callee-saved cost; compensate the lowest coloured constant web with
   one in-loop read (+10 tot).

## Files

- `groups/frontier_level_objects/` — the superseding group (`group.json`, `group.c`, `differential_output.c`,
  `func_8008AE64.c`).
- `dout/` — differential_output variant history. `base.c` is w12d's best; `t*`/`u*`/`v*`/`w*`/`x*`/`y*` are the
  dead-read steps; `x3`/`y1` are EQUAL.
- `flo_test/` — attribution experiments: `group_plain.c` (no getter), `group_ptr.c` (shared pointer getter),
  `group_ae64.c` (func_8008AE64).
- `f130/`, `acs/`, `epm/` — variants for the three unchanged functions. `vr.sh` is the unit-score wrapper.
