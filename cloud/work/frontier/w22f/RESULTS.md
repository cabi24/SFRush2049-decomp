# w22f RESULTS (wave 22)

Targets: entity_render_setup (from w20h best.c, 12 rows with data_align), then fresh singles.
Builder scratch `watchman2:~/rush2049/scratch/frontier/w22f` (toolkit reused from wtk).

| Function | Bytes | State | Flags | Scorer line |
|---|---:|---|---|---|
| entity_render_setup | 2048 | near-miss, 12 -> 8 ops rows (cause 2 fixed) | -O3 unit | `us.sh`: `words 52 ops 8 norm 8 \| frame 184/184`; unit `FAIL ... 420 of 512 ... body 513`, `locked bodies that differ in this unit: 0` |
| func_800EC190 | 224 | unchanged, 2 words (ugen temp t0 vs t1) | -O3 unit | `FAIL func_800EC190: 2 of 56 words differ \| words 6 ops 0` |
| controller_poll | 928 | near-miss, 120 -> 92 words (ops 30) | -O3 unit | `FAIL controller_poll: 214 of 232 words differ; compiled body is 230 words` `words 92 ops 30 \| frame 88/88` |

No MATCH delivered; no cloud/matches files, no groups. No integration needed (best.c files are work bases).

## entity_render_setup (0x8008C884, 2048 B)

Start: w20h best.c re-scored with the landed data_align (`TAG=w22f us.sh`): `words 65 ops 12 norm 12 | frame 184/184`.

### Cause 2 (world loop) — FIXED: `f32 pos[3]` array field + `prod + pos[k]` order
`best.c` declares CAR_DATA's position as the arcade `F32 pos[3]` (modeldat.h:413) and writes the world loop as
`quad[i].x = quad[i].x * angle + record->pos[0];` (y/z alike). Result: `words 52 ops 8 norm 8 | frame 184/184`;
the world loop (+0x6bc..+0x720) is exact; the 52 word rows are 39 own-data relocation displays + the cause-3 rows.
Trace (uopt output ucode via the workbench parser, `build/trace/w22f/u/dump.py`): ours was `ADD(MPY, ILOD(r2))`.
w20h's "newest operand first" is incomplete: an ILOD whose base is a plain variable (`record->pos.x` = ILOD(var,8))
always goes second (like a plain variable in a compare), whatever the source order or creation order (p2s: a fresh
`pos = get_pos(record)` pointer still went second). An ILOD of an *array element* (`record->pos[0]`, also
`(&record->pos)[0].x`, `((f32 *)record)[2]`) is an expression operand; then source order decides and
`prod + pos[0]` puts the load first, as retail. Delta may use the same `pos[k]` form (it does in best.c).

### Cause 3 (palette base, `&delta`/`&quad`) — open, tracing
Both are coloured-constant live ranges whose RLDA sits one block later in ours (ugen listing: `$505: la $3,D_8011B558`
at the head of the else arm; `bne $9,$6,$515` then `addu $8,$sp,128` / `addu $4,$sp,100` in the call block).
Traced with w16d's split-instrumented uopt (`bin/uopt_spl`, `SPL=1 SPLF=1`, copied into this scratch):
- palette = sym 376, livbb {19,19,19,20,20,20,28,28,28} (both mode-2 arms and the else arm, one candidate, v1);
  no livbb for the mode-test block 17, so its else-arm entry load lands at the head of 28. Retail needs 17 in the LR.
- `&delta` = sym 381, livbb {30,52}: never a colouring candidate in ours (no p1 decision). `&quad` = sym 382 (w382,
  t0) livbb {30,50,58,60,63x4}; forcing it to a1 is declined (spans calls). Retail's a0/a1 before the delta-block
  `bne` are therefore a different decision (an a0/a1 piece that includes block 29), not w382's colour.
Inert (all `ops 8`): inlined `vsub(pos, cam, d)` helper for delta (frame +16), `f32 *d` pointer stores (frame +8),
`(&delta)->x`, `f32 delta[3]` / `f32 quad[4][3]` arrays, `do {} while (0);` boundaries before/after the mode
if-else, inside the else arm and before the delta test; `goto world` restructure (35 ops); 3-byte RGB struct copy
in the else arm (10 ops, different copy code).
Best next hypothesis: find what puts block 17 (resp. 29) into the constant's livbb list (an occurrence of the
same address in that block, e.g. a compiled-out read of `D_8011B558[...]` / `&delta` there, or a source shape
where the else arm shares a block with the test); prove it first by patching formlivbb (add bb17 to sym 376).

Unit (`blob_unit --tag w22f score entity_render_setup --with cloud/work/frontier/w22f/entity_render_setup/best.c --neighbours`):
```
  FAIL entity_render_setup: 420 of 512 words differ; compiled body is 513 words, target 512; +0x354: .rodata+0xd0: ...
  locked bodies that differ in this unit: 0
us.sh: best.c: ... | words 52 ops 8 norm 8 | frame 184/184
```
Time: ~2.5 h. Stopped per the stop rule after the cause-2 fix.

## func_800EC190 (224 B) — not improved, 2/56 words (w21g best.c re-scored, `li t1,2` vs `li t0,2`), ~1 h
`us.sh`: `base.c: FAIL func_800EC190: 2 of 56 words differ | words 6 ops 0 norm 0 | frame 0/0` (the other 4 word rows
are `.data` relocation displays of the function-local static).
ugen trace (`SCHED=1 ugt.sh ec0`): allocation order over the whole function is t6 t7 (line 21) t8 t9 (27) t0 t1 t2
t3 t4 t5 (loop arms) t6 t7 (loop increment) t8 t9 (the static's s16 increment: `sll $24; move; sra $25; move`, as1
folds the moves) t0 (`li $8,2`). The ring is a FIFO of freed temps, so retail's t1 needs exactly one extra
allocation after the increment's t9 and before the `2`, with no surviving code (an earlier extra one would rename
the loop's temps). `2` is not a colour candidate (w46, save 0, no-colour). Inert (all 2 words): placements of
`D_80150F14 = 2` (worse: 5-29 rows), zeroing forms (`?:`, `x - x`, `(s16)0`, `owner & 0`), assignment-as-condition
holders (`if ((D_8014A118 = D_801163F4)) {}` replaces w21g's holder at equal score), volatile casts on each of the
three stores (worse or equal). Next: an operation in lines 41-45 whose ugen result is moved into a coloured register
(as1 then deletes the move), e.g. a CVT that survives uopt for the D_80150C04/D_8014A118 stores.

## controller_poll (0x800C95DC, 928 B) — near-miss 92 words (from w1g's 120), ~45 min
Fresh pick (last touched wave 1). w1g's best.c is a body for the controller_poll group prelude; `controller_poll/mk.sh
BODY OUT` prepends lines 1..3720 of `src/blob/groups/controller_poll/group.c` (the tentative `D_80149B64`/`D_80149B74`
made extern, since the locked group.c defines them in the unit). Re-scored: `words 120 ops 30 | frame 88/88`.
- Lever: audio_occlusion's chained zero store (w8b) `D_8015694C = D_80149784 = D_80156944 = 0;` -> `words 92 ops 30`
  (top block now retail's: &D_80156944/&D_80149784 in registers, D_8015694C via `lui at`).
- ctrace (`cp1`): the queue address &D_801497A8 is w225 (tot 1 > best 0.2, coloured s0 because s0 is already paid
  for); retail rematerialises it at all three calls. &D_8015694C is w231 (tot 20, s8 for the whole function); retail
  splits it and uses a v0 piece inside the `if (connected)` block. `connected` lands in ra (c13) in ours, t4 in retail.
- `*(s32 *)(u32)&D_8015694C |= D_80156998[i];` in the loop (laundering, a shaping device) gives `words 81 ops 22`
  (`v/q1.c`); it frees the D_8015694C web but s8 then takes the loop bound 4 and the queue stays in s0.
- Inert: laundering the queue address at the recv call; `D_8015694C = D_8015694C | x`.
Next: find why retail's queue address web loses (tot <= best): fewer weighted uses or the calls in a form that does
not share one address web; then re-check the array-pointer colour order (retail gStick in s0, D_80149B10 in t5).

## Shared notes / what generalises
- **uopt commutative-operand order (refines w20h):** an ILOD whose base is a plain variable (`p->f`) is a "leaf"
  operand and always goes second in an ADD, whatever the source or creation order; an array-element load
  (`p->arr[k]`, `(&p->v)[0].x`, `((f32 *)p)[k]`) is an expression operand and then source order decides
  (`prod + p->arr[k]` puts the load first). Arcade `F32 pos[3]` fields are therefore not interchangeable with Vec3
  structs in matching source. Tool: `w22f/tools/udump_proc.py OPT.p BLOCK L0 L1` prints uopt's output ucode for a
  procedure (block number = the `M nnnn` in pretrace listings; opt.p from `pretrace.sh`'s snapshot).
- ugen's temp ring is a FIFO; `SCHED=1 ugt.sh` shows each ALLOC/FREE, so a one-register shift in a tail localises the
  missing (code-free) allocation to the statements between two allocations.
- w16d's `uopt_spl` (SPL/SPLF) works on other lanes' snapshots (`cp ~/rush2049/scratch/frontier/w16d/bin/uopt_spl`).
Permission denials: none. Builder: 2 jobs, own scratch only.
