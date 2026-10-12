# w26e — game small residuals (IDO 5.3), wave 26

Builder scratch: watchman2:~/rush2049/scratch/frontier/w26e (toolkit `--reuse wtk`; snapshots deleted at the end).
Nothing committed, spliced or pushed. No permission denials. Variants under `<fn>/v/` (ignored).
Unit scores: `TAG=w26e cloud/work/frontier/tools/trace/us.sh NAME FILE` (words/ops/norm rows, frame want/got).

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| audio_channel_priority | 468 | **strict MATCH** (standalone + unit), no shaping devices | -O3 | `audio_channel_priority: MATCH` / `EQUAL audio_channel_priority: 117 words (kept, c_audio_channel_priority.c)` / `locked bodies that differ in this unit: 0` |
| entity_process_main | 1424 | 2 words, unchanged | -O3 | `FAIL entity_process_main: 2 of 356 words differ \| words 4 ops 0 norm 0 \| frame 280/280` |
| audio_doppler_full | 952 | 39 words, unchanged | -O3 | `FAIL audio_doppler_full: 39 of 238 words differ \| words 45 ops 0 norm 0 \| frame 256/256` |
| world_effect_update | 456 | 12 words, unchanged | -O3 | `FAIL world_effect_update: 106 of 114 words differ; compiled body is 112 words, target 114 \| words 12 ops 11 norm 11 \| frame 24/24` |
| audio_frame_update | 600 | 27 rows (f2), unchanged | -O3 | `FAIL audio_frame_update: 25 of 150 words differ \| words 27 ops 8 norm 8 \| frame 40/40` |
| camera_play_script | 3520 | not worked (time) | -O3 | w18d: `words 27 ops 1 norm 1 \| frame 608/608` |
| camera_update, func_80096130, audio_channel_setup, differential_output, engine_sound_sync | | already locked on master | | — |

## Pool check (2026-10-12, master 46780d3e)

Five of the eleven pool names are already locked in `blob_matched.lock.json` and need no work:
camera_update (group camera_aspect_ratio, w16a), func_80096130 (group codex_heap_release_a25, w17i),
audio_channel_setup (single, w15), differential_output (group frontier_level_objects), engine_sound_sync
(group engine_sound_sync, w23). Their pool rows came from stale near-miss tables.

Open targets: entity_process_main, audio_channel_priority, audio_doppler_full, audio_frame_update,
world_effect_update, camera_play_script.

## audio_channel_priority (468 B): strict MATCH, no shaping devices

Deliverable: `cloud/matches/audio_channel_priority.c` (plain kept single, -O3; -O2 does not match).
```
builder: score.py fn cand/audio_channel_priority.c audio_channel_priority --flags "-g0 -O3 -mips2 -G 0 -non_shared"
audio_channel_priority:
  MATCH
python3 -m tools.conveyor.pipeline.blob_unit --tag w26e score audio_channel_priority --with cloud/matches/audio_channel_priority.c --neighbours
  EQUAL audio_channel_priority: 117 words (kept, c_audio_channel_priority.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal
```
Start: w4a best.c `11 of 117 words` (colour only; all webs phase 2 = web-number order, lowest free register).
Trace (ctrace): retail's order is outer v0, weights v1, row a0, base a1, handles a2, slot a3, id t0, index t1.
- Parameter used directly as the id (w12e stride.c): the param web is numbered at its first use (bb3), after
  slot -> t0. 24 words alone because the weights expression web (w28) was still after the byte-offset row (w3).
- `row` as an index 0..2 (`D_80151690[outer][row]`, `D_80150F88[outer].weight[row]`; uopt turns row*20 into
  the a0 IV with bound 60): n3, 14 words; the row variable web shrinks and slot takes v1.
- A compiled-out read of the weights expression in bb0 (`if (D_80150F88[outer].weight[row]) {}`, uninitialised
  outer/row): 2 words, only the frame (40 vs 32); as a dead store `weights = D_80150F88[outer].weight[row];` in
  bb0: EQUAL — proves the mechanism (first occurrence of the weights expression decides its web number).
- The natural form without devices: assign `weights` at the top of the row loop next to `handles` (uopt sinks it
  into the if-block): EQUAL. w4a had tried this with the byte-offset `row` (14 words); the index form is what
  makes it work.
Integration: `splice_singles.py audio_channel_priority`. No unit_overrides, no rodata (0.0f is `mtc1 zero`).

## entity_process_main (1424 B): 2 words unchanged (lever probes)

Base w20e best.c: `FAIL entity_process_main: 2 of 356 words differ | words 4 ops 0 norm 0 | frame 280/280`.
Ring reading (ugt.sh): ours `li t8; sllv t9; nor t6; lhu t7; and t8` (tree and(not(shl), load)); retail
`li t8; sllv t9; lhu t6; nor t7; and t8,t6,t7` = and(load, not(shl)) with the load evaluated between the shift
and the NOT. Levers at the clear arm:
- narrowing `~(1 << i) & (poly->flags & 0xFFFF)`: as1 deletes the andi (ops 0) but it is one extra pop in the
  wrong place: `17 of 356 words` (li t8, sllv t9, nor t6, lhu t7, [andi t8], and t9). `(poly->flags & 0xFFFF) &
  ~(1 << i)` 20; `~((1 << i) & 0xFFFF) & ...` 18 (andi kept); `(...) & 0xFFFF` on the result 17.
- empty block before or after the statement (`do {} while (0);`): 2 words, unchanged (no CSE to break).
Next hypothesis: retail's order needs the shift to be a node ugen evaluates before the load while the NOT is
evaluated after it, i.e. a shared (ugen-CSE) shift node: `1 << i` occurring twice in the block's ugen input. No
code-free spelling of a second occurrence found yet.

## audio_doppler_full (952 B): 39 words unchanged (lever probes)

Base w12g best.c (re-scored, now with the locked direct-return func_800B61A8): `FAIL audio_doppler_full: 39 of
238 words differ | words 45 ops 0 norm 0 | frame 256/256`. ugt.sh: retail allocates three GP temps between
`li t5,1` and the `sllv` of the create call's flags argument (ring +3 to the end of the function; the else block
is +3 only because ugen emits it after the create block). Probes on the create call: `& 0xFFFF` on the texture
load (as1 deletes the andi, but the load moves before the `1`: 142 words), `(u16)(...)` on flags (39), `& 0xFFFF`
inside/outside flags (144, andi kept), `(player & 0xFF)` shift (145), `(u8)1` (39), `4 & 0xFF`/`1 & 0xFF` (39),
`do {} while (0);` before the statement (39). Next hypothesis: the three allocations belong to the shift
operand's evaluation (as1 must delete them and leave `sllv t9,t5,s6`), so a form whose shift amount is a
copy/narrowing of `player` that as1 can prove redundant — or a ugen-CSE'd constant 1 shared between the shift
and the indexed argument.

## world_effect_update (456 B): 12 words unchanged

w19e best.c re-scored (12 words). Probes: `do {} while (0);` and `if (1) {}` as the first statement (retail keeps
the prologue above the head stores = a block boundary right after it): unchanged; `do {} while (0);` after the
msgq_ptr store (the second boundary): 16 words; both: 16. uopt drops these empty blocks before ugen here (no label
reaches the listing). w20d's ~40 forms and its listing oracle (a label / jump-to-next right after `.frame`) stand.

## audio_frame_update (600 B): unchanged

w17i f2.c re-scored: `25 of 150 words differ | words 27 ops 8 norm 8`. Probes: `do {} while (0);` before the loop
and before each of the six loop stores (a1-a6): all byte-identical to f2 (the empty block is removed inside the
loop, so it adds no live block). The w17i diagnosis stands: one more web, loop-live, ranked above `i`, shifts
i/24/car/anim/ctl one register down (v1 a0 a1 a2 a3 -> retail a0 a1 a2 a3 t0). New thought, untested: i is denied
v1 rather than v1 being taken (the cascade is identical either way); look for what forbids v1 to i (a precoloured
v1 occurrence in the loop's uopt blocks).

## camera_play_script (3520 B): not worked

Out of time after the five above. w18d's hypothesis stands (retail needs P(pts[ka]) >= P(w515) > P(pts[kb]); the
corner IV is not a bare argument copy). Note for the next lane: an empty block can only add live blocks, which lowers
save; it can make pts[kb] < w515 only if kb is live across it and ka is not, and w515 (tot 30) would still need
~9 live blocks to fall below pts[ka]'s 8, so the empty-block lever alone does not reach retail's order.

## Extra probes (entity_process_main)
`~(1 << i) & poly->flags & ~(1 << i)` 43 words (extra and); a compiled-out `if ((1 << i) == 0) {}` before the
statement makes the shift a coloured web (v1) and the load is then evaluated before the NOT (22 words): the
interleave appears once the shift is not part of the statement's tree, but in a register, not the ring.
`poly->flags & ~(1 << i) & 0xFFFF` 19, `(u16)(poly->flags & ~(1 << i))` 18.

## What generalises
- **Natural index form decides p2 web numbers** (audio_channel_priority): writing a byte-offset loop variable
  (`row += 20`) creates a variable web numbered at its first block, while an index `row` (0..2) that uopt
  strength-reduces leaves a tiny variable web and an IV expression web, so other webs move ahead of it. Try the plain
  `a[i][j][k]` form before tuning colour order on a strength-reduced loop.
- **A pointer local assigned one loop level too high is sunk by uopt into the block that uses it, but keeps its
  first-occurrence web number** (audio_channel_priority `weights`): the lever for "this expression web must be numbered
  earlier" in phase 2 without moving code. A dead store of the same expression in bb0 proved it (EQUAL) before the
  natural placement was found.
- A parameter used directly is numbered at its first use (stride.c: bb3), not at entry; a copy `id = param` at entry
  numbers the copy first.
- `do {} while (0);` inside a loop body or at a function head is removed by uopt in these three functions (no label,
  no extra live block): the wave-25 empty-block lever works only where the empty block survives to a real boundary.
