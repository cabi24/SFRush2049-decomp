# w10f — cheap re-sweep of old near-misses (wave 10)

Strict: **6 functions, 1,528 bytes**, all singles, all kept, no overrides needed, nothing superseded.
All scored on watchman2 (`~/rush2049/scratch/frontier/w10f`, `tools/sc.sh` = `score.py fn … --flags "-g0 -O3 -mips2 -G 0 -non_shared"`)
and together in the whole-program unit:

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10f score func_800956BC func_80092BF4 entity_state_check \
  func_80108DA8 camera_collision_avoid func_800A4CB8 --with cloud/matches/func_800956BC.c --with cloud/matches/func_80092BF4.c \
  --with cloud/matches/entity_state_check.c --with cloud/matches/func_80108DA8.c \
  --with cloud/matches/camera_collision_avoid.c --with cloud/matches/func_800A4CB8.c --neighbours
  EQUAL func_800956BC: 20 words (kept, c_func_800956BC.c)
  EQUAL func_80092BF4: 25 words (kept, c_func_80092BF4.c)
  EQUAL entity_state_check: 30 words (kept, c_entity_state_check.c)
  EQUAL func_80108DA8: 102 words (kept, c_func_80108DA8.c)
  EQUAL camera_collision_avoid: 103 words (kept, c_camera_collision_avoid.c)
  EQUAL func_800A4CB8: 102 words (kept, c_func_800A4CB8.c)
  locked bodies that differ in this unit: 0
blob_unit score: 6/6 equal; object build/blob_unit/w10f/unit.o (5.1s)
```

| Function | Bytes | State | Flags | Scorer output | Source |
|---|---:|---|---|---|---|
| func_800956BC | 80 | **MATCH** | -O3 (also -O2) | `func_800956BC: MATCH` | `cloud/matches/func_800956BC.c` |
| func_80092BF4 | 100 | **MATCH** | -O3 (also -O2) | `func_80092BF4: MATCH` | `cloud/matches/func_80092BF4.c` |
| entity_state_check | 120 | **MATCH** | -O3 (also -O2) | `entity_state_check: MATCH` | `cloud/matches/entity_state_check.c` |
| func_80108DA8 | 408 | **MATCH** | -O3 (also -O2) | `func_80108DA8: MATCH` | `cloud/matches/func_80108DA8.c` |
| camera_collision_avoid | 412 | **MATCH** | -O3 only | `camera_collision_avoid: MATCH` | `cloud/matches/camera_collision_avoid.c` |
| func_800A4CB8 | 408 | **MATCH** | -O3 only | `func_800A4CB8: MATCH` | `cloud/matches/func_800A4CB8.c` |
| func_800E7A98 | 172 | 13 rows in unit (was 20) | -O3 unit | `want 43 words, got 42; differing rows 13` | `func_800E7A98/best.c` |
| world_effect_update | 456 | 49 rows in unit (was 51) | -O3 unit | `want 114 words, got 114; differing rows 49` | `world_effect_update/best.c` |
| camera_update_a | 128 | 12 rows (s0/s1 swap) | -O3 | `want 32 words, got 32; differing rows 12` | `camera_update_a/best.c` |
| func_800A1BB4 | 184 | 24 rows unit / 26 standalone | -O3 | `want 46 words, got 42; differing rows 24` | `func_800A1BB4/best.c` |
| func_8008AD6C | 164 | 34 rows | -O3 | `want 41 words, got 41; differing rows 34` | `func_8008AD6C/best.c` |
| sound_stop | 160 | unchanged (w5a 12/40) | | stub-helper try 26 rows | `sound_stop/a.c` |
| audio_channel_priority | 468 | unchanged (11 rows) | | stub-helper try reproduces the same 11 rows | `audio_channel_priority/b.c` |
| func_8010D680 | 476 | unchanged (22 rows) | | K&R `short on` no change | `func_8010D680/b.c` |

Not attempted (deeply worked, ~100s–2,400 variants each, no wave-9 technique applies): func_800EC190, func_800A79F4,
race_position_update, func_800B9740. func_800CCB40 is a strict match already **held for an owner decision** (w4d) — left alone.

## Per function

### func_800956BC — MATCH
List search over `D_80146160` (head = word 3): first node with key (+0xC) == arg and kind byte (+8) == 3.
Quirk: the list header is `volatile` (retail `lui/addiu` + `lw 12(reg)`). 3 compiles.

### func_80092BF4 — MATCH
`D_8012E700[r->w14].v3C = *v1; …v40 = *v2` with `r = &D_80139320[idx]`. Written in the style of the locked sibling
func_80092FE0: two separate `short` locals for the slot index (two sign-extensions, and `li 68; multu` instead of a
shift expansion of `*68`), second store through a `ModelSlot *` local (puts the address in a3). 8 compiles.

### entity_state_check — MATCH
Handle validation against the 24-byte table `*D_80144C48` (slot = low byte of id): 0 for -1 or stale, 1 if byte +1 is
clear, else static `func_800201D0(slot.handle) != -1`. Quirk: the call argument goes through `e = D_80144C48 + slot`
(retail recomputes offset+base into v0). Nested `if (id != -1) { … }` form. 10 compiles.

### func_80108DA8 — MATCH
Found by rescoring old drafts in the unit: `cloud/work/heads_B13/func_80108DA8_structmode.c` was 36 rows, all from one
cause — `D_801543CA` must be `volatile s16` (retail `la; lh 0(reg)`, same as the store in func_800EC190). Then rewritten
with typed structs (node fields, 0x3B8-byte player record `D_801528F0[i].state` at +0x17, `Pos D_80116028[][4]`
indexed `[count - 1][index]`). Semantics in the file header.

### camera_collision_avoid — MATCH
Found by the old-draft sweep: `cloud/work/near_miss_B57/camera_collision_avoid_native.c` was code-identical except a
16-byte-smaller frame (14 rows, every `sp` offset). An unused `f32 pad[4]` declared after `point` gives the 120-byte
frame (an unused `Vec3` before or after does not). The frontier lanes' later best (w2h/w4b, 31 rows, "FP ring") had
diverged from this simpler body.

### func_800A4CB8 — MATCH (w3d's 98/102 near-miss)
w3d's best.c plus one compiled-out `if (count == 0) {}` right after the first `list_init`. The retail residual was
`count` living in a caller-saved register (`move a2,a0 … sw a2,48(sp)`) instead of only its home slot; the empty
check makes it a coloured web (technique found on func_800E7A98 below). Placement matters: before the first
list_init 20 rows, after `D_801460F4 = count` 18, later 40–52, right after the first list_init 0. The stub
`func_800A4E50` (w3d's "Next") is not involved (three helper splits 53/53/79 rows).

### func_800E7A98 — 20 → 13 rows (unit; callee audio_reverb_update is internal, so unit only)
New: the list head is read into a `next` variable in **both** arms of the heap select (`if (heap) { h = heap; next = head; }
else { next = head; h = next; }`, loop `while (next) { p = next; next = p->next; if (next == h) {…} }`), and a compiled-out
`if (heap == NULL) {}` before the lock makes `heap` a coloured web in a3 spilled to its home across osRecvMesg, exactly as
retail (`move a3,a0 … sw a3,32(sp) … lw a3,32(sp)`). Residual: `h` coalesces with the internal callee's a1 argument
(retail keeps h in v0 and copies `move a1,v0` at the join), so `next` gets v0 instead of a0. Next: something that keeps h
live/interfering with a1 without emitting code.

### world_effect_update — 51 → 49 rows (unit)
Chained `D_80156BE0[0].buffer = D_80156CEC = align(D_80156CEC)` stores and `volatile D_801497C8` (retail stores and
reloads it through `la` before copying to D_801497F4). Open: retail's prologue stays at entry while ours is scheduled
below the first stores; all three alignment loads precede the stores in retail. The stub `func_800EE8AC` as an inlined
wait/initialiser helper made no difference.

### camera_update_a — 12 rows (recursive tree walk with callback and counter)
Trace (`tools/ctrace.sh`, proc 226): `n` (a1) save 25.5 → s0, `count` (a2) save 20 → s1; retail is the reverse.
Forcing `p1:w7=c14,p1:w0=c15` leaves 2 rows (retail's entry `beqz` tests s1 after the moves; ours tests a1). Tried
for/while/do-while, K&R order, pointer copies of n and count, increment spellings, child-null guard: no movement.

### func_800A1BB4 — 24 rows unit
Typed rewrite (772-byte record with 40-byte sub-array at +128, model link list `D_80144D68[i].head`); structure right
except retail's early `jr ra` after the active test, an unfilled `beqz; b exit` inner test, and t6–t9 temps in the loop.

### func_8008AD6C — 34 rows (display-list byte-swap callback, returns 2)
Retail keeps the op/hi/lo values in v1/a1 and the word in v0 (uopt webs); every spelling tried (helper, loop, u8/u16,
split assignments) leaves them as ugen temps.

### Stub/inlined-helper probes that did not help
sound_stop (stub func_800B3584 as the remove-one helper: 26 rows), audio_channel_priority (stub func_80094A4C as the
shift-down helper: same 11 rows), func_800A4CB8 (stub func_800A4E50, three splits, no gain — closed instead by a compiled-out check, above).

## Integration notes
- All six are plain kept singles: `PYTHONPATH=. python3 cloud/work/frontier/tools/splice_singles.py func_800956BC
  func_80092BF4 entity_state_check func_80108DA8 camera_collision_avoid func_800A4CB8` (flags on line 1). No unit_overrides entries,
  no group superseded, no own rodata (camera_collision_avoid's `0.0f` is `mtc1 zero`).
- func_800A4CB8.c also defines the locked kept `func_800A370C` (List clear) as context, as src/blob/hud_speed_display.c
  already does; if the single splice path rejects the second definition, make it `extern` + keep the static list_init
  (not tested) or splice through blob_group with func_800A370C as context.
- camera_collision_avoid and func_800A4CB8 are -O3 only; the others also match at -O2 (line 1 says -O3).
- func_80108DA8 declares `D_801528F0` (record base) and `D_80116028`; both resolved in strict scoring.

## What generalises
1. **Rescore old pre-frontier drafts in the unit before writing new source.** Two of six matches came from files in
   `cloud/work/heads_B13/` and `cloud/work/near_miss_B57/` that were 1 cause away (one `volatile`, one frame pad) and
   had been superseded by worse later drafts. A batch standalone score of every old definition file takes minutes
   (`$S/sweep/run.sh` pattern: copy files to the builder, loop `score.py fn` at -O3/-O2).
2. **`li N; multu` for a struct-array index** (instead of shift expansion) comes from indexing with a `short` local
   whose value is read once per use (sibling func_80092FE0 style).
3. **A compiled-out `if (param == NULL) {}` before a call** turns a parameter that lived only in its home slot into a
   coloured caller-saved web spilled to its home — retail's `move aN,a0 … sw aN,home` prologue shape. Closed
   func_800A4CB8 (52 → 0 rows) and took func_800E7A98 from 20 to 13. Its position also moves the schedule: sweep it
   over every statement boundary.
4. A pure frame-size residual (all `sp` offsets shifted, code identical) closed with an unused local array sized to the
   difference; a same-size struct local did not work.

## Permission denials
None.
