# w14o results (wave 14): three near-misses, mass variant testing

Builder scratch `watchman2:~/rush2049/scratch/frontier/w14o` (copy of `base`; src/blob, include, tools/cloud, asm/us/blob
and blob_matched.lock.json synced from the Pi; trace toolkit installed with `--reuse wtk`). Two cores at most. Nothing
committed, spliced, pushed or edited outside this lane. No permission denials.

Result: **no function matched.** All three are unchanged from their input best (no strict improvement in any round).

## Baselines (unit, confirmed this session)

```
blob_unit --tag w14o --jobs 2 score entity_process_main --with cloud/work/frontier/w11a/entity_process_main/best.c --neighbours
  FAIL entity_process_main: 2 of 356 words differ
  locked bodies that differ in this unit: 0
blob_unit --tag w14o --jobs 2 score audio_channel_setup --with cloud/work/frontier/w12e/audio_channel_setup/best.c --neighbours
  FAIL audio_channel_setup: 3 of 83 words differ
  locked bodies that differ in this unit: 0
blob_unit --tag w14o --jobs 2 score func_80096130 --with cloud/work/frontier/w12e/func_80096130/best.c --neighbours
  FAIL func_80096130: 3 of 66 words differ
  locked bodies that differ in this unit: 0
```

`diagnose one` (standalone): entity_process_main `mixed(constant:2, structural:2, schedule:2, register:2)`, lever
none-known. audio_channel_setup `mixed(structural:1, register:3)`, lever none-known. func_80096130 standalone is not
meaningful (66 of 66 words differ, the group's own callers are missing), so only unit scores are used for it.

## Rounds

| Function | Round | Template / file | Variants | Best strict (standalone bscore, or unit) | Notes |
|---|---|---|---:|---|---|
| entity_process_main | wbgen g1 (hill-climb from best) | `w14o/entity_process_main/g1/` | 2400 | 2 (control 2) | 15 ties at 2; no strict win, so climbing stopped |
| entity_process_main | vgen b1 | `t1.c`, `b1/` | 128 | 2 | clear-arm spellings, decl order, `d1*0.25f`, dead read |
| entity_process_main | vgen b2 | `t2.c`, `b2/` | 300 | 2 | mask spellings, `i` init/compare forms, hide arm via `poly` |
| audio_channel_setup | vgen b1 | `t1.c`, `b1/` | 128 | 3 (control 3) | decl order, `tex` type, load spellings, compiled-out reads |
| audio_channel_setup | vgen b2 | `t2.c`, `b2/` | 300 | 3 | index casts, `f` / `id` order, `obj->timer` and `frame` spellings |
| audio_channel_setup | vgen b3 | `t3.c`, `b3/` | 64 | 3 | `kind` as pointer arithmetic, `if (...) != 0` forms, `if (m)`/`if (id)` reads |
| func_80096130 | vgen b1, unit-scored | `t1.c`, `b1/`, `unit_b1.txt` | 128 | 3 (control 3; 4 variants tie at 3) | slot/off decl, `if (slot)`/`if (0)` reads, memset ordering, `index << 3` |

Control lines: every template's v0000 was checked equal to the best file (diff empty). An earlier draft of the func_80096130
and entity templates had a non-original option 0, which I found and fixed before any reported number. The discarded
sweep is not evidence.

## Residuals and what they are

- **entity_process_main (2 words):** the known as1/ugen order (`shl, not, load` vs retail `shl, load, not`). Every
  spelling of the clear arm (`&=`, `~((1<<i)|~flags)`, `(u16)` cast, `flags & ~(1<<i)`) and the decl/compare/hide-arm
  choices reproduce 2 rows and no other row moved. The w13d lead (`~((1<<i)|~flags)`) was one of the b1 options; it did not
  reach below 2. Diagnose's lever is none-known.
- **audio_channel_setup (3 words):** the copy of the loaded `D_801427C0[f]` web into a1 (w12e mechanism). None of 492
  variants moved it. Choice points that changed the body (not the copy) gave 8–23 rows (w13d) or no movement.
- **func_80096130 (3 words):** the spill home of the `index*8` web (36(sp) vs retail 32(sp)). Unit round b1 with 128
  variants: 4 tie at 3 (including the control), and none is EQUAL. The best rows left are the w13d/w12e residual.

## Not done (honest scope)

- func_80096130 has had one unit round (128 variants), not the three the brief asks for. Its variants are scored in the
  whole-program unit (`unitsweep.sh`, one blob_unit run per file, about 15 s each), so each round costs about 30 min. The
  standalone bscore is not a usable ranking for it (66 rows on the control).
- No `force.sh` oracle was run on audio_channel_setup or func_80096130. The w12e/w13d traces say the audio residual is not
  colour-only; the spill residual of func_80096130 has not been forced.
- The entity_process_main hill-climb stopped at round g1 (no strict improvement). The three vgen rounds for the same
  function were all flat at 2.

## Files

- `entity_process_main/{best.c,t1.c,t2.c,g1/,b1/,b2/,score.txt}`: the template, rounds and the g1 run log (`g1.run.txt`).
- `audio_channel_setup/{best.c,t1.c,t2.c,t3.c,b1/,b2/,b3/,score.txt}`: the templates and rounds.
- `func_80096130/{best.c,t1.c,b1/,unit_b1.txt,ctl/}`: the template and unit results for round b1.
- `unitsweep.sh`: unit-scores every `v*.c` in a directory with blob_unit (tag w14o).
