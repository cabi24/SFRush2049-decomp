# Wave 14, lane w14c: graphics_chunk, place_cars_in_order, assign_drones

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w14c` (synced from the Pi). Nothing spliced, committed or pushed.
Shared unit check: `python3 -m tools.conveyor.pipeline.blob_unit --tag w14c score graphics_chunk --with cloud/work/frontier/w14c/gc/vp.c --neighbours`.

| Function | Bytes | State | Flags | Source |
|---|---:|---|---|---|
| `graphics_chunk` | 528 | MATCH (standalone) and EQUAL in the whole-program unit | `-g0 -O3 -mips2 -G 0 -non_shared` | `cloud/matches/graphics_chunk.c` (= `gc/vp.c`) |
| `place_cars_in_order` | 544 | 76 of 136 words differ (best) | same | `pc/v18.c` (= `pc/place_cars_in_order_best.c`) |
| `assign_drones` | 1236 | not started (reconnaissance only) | - | none |

## graphics_chunk scorer output
```
python3 tools/cloud/score.py fn cand/gc/graphics_chunk.c graphics_chunk --flags '-g0 -O3 -mips2 -G 0 -non_shared'
graphics_chunk:
  MATCH
blob_unit score ... graphics_chunk: EQUAL graphics_chunk: 132 words (kept, c_vp.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal
```
The prior m2c draft in `cloud/work/near-miss/graphics_chunk/base.c` was rebuilt from the retail asm. Three things decided the match:
- The `abs()` of the `D_80149428` column is written as `if (v<0) v=-v; else v=v;` in the `v1` form (`bltz`/`negu`/`move`).
- `a2->w8 += D_80149428[i*4+k]` is a compound assignment (the explicit `(u16)(w8 + x)` spelling gave 26 words of diff).
- Local declaration order `s32 t2; s32 i;` puts the t2 spill at sp+68 (the explicit `(u16)` store form was not the cause).

Globals: `D_8014A118[]` is a 0x4C-stride record array (`idx` at +1, `tgt` at +0x48, `blk` at target +0x2C).
`D_80149428` is an s8 4x4 table, `D_80151578` a 12-byte Acc array. Own-rodata: none.

## place_cars_in_order (best 76/136 standalone, 74/136 in unit, not a match)

Update (w14c, second pass). Best candidate is now `pc/v26.c` (copy `pc/place_cars_in_order_best.c`): the sort-key
float sum is `q = dist/528.0f; f = q + (f32)(u32)key`. Unit check (`tools/trace/us.sh`): `FAIL ... 74 of 136 words differ;
compiled body is 134 words, target 136 | frame 72/72`.
Force/colour evidence: `ctrace.sh place_cars_in_order b18 pc/v18.c` shows the unit assigns w77 (game_car base) to s1 and
w78 (952) to s2, the same as retail. Colour is therefore not the residual, so `force.sh` would be a no-op there (not run).
`udiff.py --norm` shows a structural residual: retail reads `game_car.dist` twice (once in the unsigned-fix path,
`bgezl t0 ... lwc1 $f10,264(a0)`), and our body is 2 words short. Shape variants (`pc/v27.c`, `v28.c`, `v29.c`) moved it
the wrong way (frame 80 or 132 words off), so the source shape is still open.
Shared scaffolding: the record walk (`D_8014A118` stride 76, `tgt` via `D_80146150[idx]`, `blk+0x2C`, `96*s6` block
pointers, the key at `+0x58`, the `func_800CD8EC((void**)tgt, (u8)s8)` call) is shared with `graphics_chunk` and with
`assign_drones`. The per-pass bodies differ (graphics_chunk uses `D_80151578` with stride 12 and `+0x60C`), so
they are probably not one source file; they look like one family (DoDrones).

Not the arcade `place_cars_in_order` (that one sorts cars by `place`). The retail function is the same two-pass record walk as `graphics_chunk`: it
recomputes a sort key `+0x58` for each car, `(s32)(u32)((f32)(u32)key + game_car[b0].dist / 528.0f)`, for pass 0 (`blk + 96*s6 + 140`) and pass 1 (`D_80150F88 + 96*s6`), then calls `func_800CD8EC(tgt, (u8)s7)`.
`s6 = D_8014978C (+6 if D_80152570)`, `s7 = D_8014978C (+19 if D_80152570)`.
The frame (72) and the prologue match. Remaining diff is register colour: retail keeps the game_car base in `s1` and 952 in `s2`, and computes `96*s6` in `a1` before the null check.
Tried, no movement below 76: `96*s6` spellings and hoisting, `(u8)s7` hoisting, `s7` init forms, `game_car` address spellings, pass branch order.
The typed `Car` struct (`D_80152818[b0].dist`) gave the best result (80 to 76).
Next hypothesis: force the colour of `s1`/`s2` with a second use of the game_car base; test with `tools/trace/force.sh` before further spelling changes.

## assign_drones (first draft, 295/309 words differ, not a match)

Built from the retail asm (`ad_retail.s`, `ad.s` with `.L` labels) through `tools/mips_to_c/m2c.py -t mips-ido-c -f assign_drones ad.s` (`ad_m2c.c`).
The m2c output has the same outer scaffold as above, then:
- a 5-slot sorted insert of `car+0xF0` into a float array at `p+0x2C` (unrolled by IDO, four elements per step), with shifts into `D_80151690 + s6*0x3C` and `D_80151AC0` when pass == 1;
- the `+0x46`, `+0x54`, `+0x4E` (`+= rec->f40`), `+0x44` counters, a `D_80149A78[(i<<5)+k]` accumulation into `+0x40`;
- the key update at `+0x58`, then `func_800CD8EC(tgt, (u8)s8)` with `s8 = D_8014978C (+19 if D_80152570)`.
Draft `ad/assign_drones_draft.c` (= `ad/v2.c`): `295 of 309 words differ` (standalone, scorer). Two variants only.
Not iterated beyond that. The unrolled insert and the pass-1 side arrays are not yet reconstructed. The frame and prologue differ.
Next step: derive the insert section per element from the retail asm, then iterate on the frame (136) first.

