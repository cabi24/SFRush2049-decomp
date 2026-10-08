# Wave 14, lane w14s: assign_drones (1236 B, 0x800F4FEC)

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w14s` (base copy, src/blob, include, tools/cloud, asm/us/blob and
blob_matched.lock.json synced from the Pi; trace toolkit installed with `--reuse wtk`, not fidelity-checked).
Nothing spliced, committed, pushed, or written to cloud/matches. NOT a match.

| Function | Bytes | State | Best clean candidate | Unit output |
|---|---:|---|---|---|
| `assign_drones` | 1236 | 288 of 309 words differ; frame 136/136 (retail 136); NOT a match | `ad/assign_drones_best.c` (= `ad/v7.c`) | `us.sh`: `FAIL assign_drones: 288 of 309 words differ; compiled body is 310 words, target 309 \| words 281 ops 97 norm 97 \| frame 136/136` |

Flags: `-g0 -O3 -mips2 -G 0 -non_shared` (line 1 header of every candidate).

## Changes that mattered (vs w14i v3, 283/309 frame 136/144)
- Frame 144 -> 136: dropping the `cnt` local (`D_80144018[i]` indexed directly) and the `sb` local (`(u8) s8` at the call)
  gave frame 136/136. Diagnose named `drop-a-declared-local` for the frame before the change.
- `D_80144018` is a u8 array indexed by player `i` (count), not a scalar. `f40` is u16 (`lhu`). `(*tgt)->+8` gates the
  pass-1 tgt side store at slot k. The insertion loop is `k<5` with a store at k==4 and no shift (retail `swc1` in the delay slot).
- Key update as one expression, `(u32)((f32)*(u32*)(p+0x58) + cp->dist/528.0f)`, as in place_cars_in_order.
- Rec pointer walk `rp++` (retail s4 += 76) and `fq` float pointer (retail t2 walk).

## Diagnose (flags passed explicitly, per coordinator note)
`python3 -m tools.conveyor.pipeline.diagnose one assign_drones --source ad/v7.c --flags "-g0 -O3 -mips2 -G 0 -non_shared"`:
verdict `mixed(constant:4, structural:244, register:102)`, lever `declare-the-pair-later`, frame_delta 0, 289 words differ.

## Rounds (standalone bscore, strict and mnem-missing)
- b1 `ad/t1.c` (7 choice points, 128 variants): all strict 300 (ranking saturated; positional diffs shift with the frame).
- b2 `ad/t2.c` (7 choice points incl. key form, 128): best strict 288, mnem 82.
- b3 `ad/t4.c` (10 choice points on v7, 288 variants): 32 variants at mnem 60; 96 compile failures (shift-form spellings).
  Per-axis: `(u8) s8 & 0xFF` is worst (83-89); 60*s6 spelling is not separated.
- g1 wbgen climb on v7 (318 variants): best `v0281` strict 257 but it adds `k = p;` (pointer assigned to s32 k), a type-invalid
  shaping hack. NOT accepted; `ad/v8.c` kept only as evidence (unit 257/309, ops 93, frame 136/136).
- g2 wbgen climb on v8 (309 variants): best `v0209` strict 240 but it reuses `pass` as the count (`pass = D_80144018[i]`),
  which overwrites the pass loop variable. REJECTED as semantically wrong; `ad/v9.c` unit 240/309 kept as evidence only.
- Unit confirmation of v9 and v8 done only on the hack-free candidate (v7) for the frame; no other candidate is clean.

## Residual (open)
- Retail computes `60*s6` and `96*s6` inline with shifts (`sll 4; subu; sll 2`); ours hoists 60 into a register (`multu`),
  because our register pressure differs (retail uses `ra` as the s6 data register, spilled to 132 around the call;
  retail s8 is a stack word at 128, read as `lbu 131`).
- Retail regs: s3=5 and s8=D_80152818 are live constants; ours put 5 in s6 and never spill s8. This is the colour residual.
- Next hypothesis: force the s8 spill (128) and the `ra` s6 home with `tools/trace/force.sh` on the `p1` webs for s6/s8
  before further spelling changes; then re-run wbgen on the forced winner.

## What generalises
- Frame residuals in this family come from named locals that take a home slot (`cnt`, `sb`); removing them is a real lever
  (diagnose `drop-a-declared-local`).
- Standalone strict is saturated (positional shift) for big frame-wrong functions; rank by mnem-missing and fix the frame first.
- wbgen hill-climbs can pick semantically invalid edits (`k = p`, `pass = count`); always review the winner before accepting it.
