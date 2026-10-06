# w12g results (wave 12): the func_800B61A8 spelling question, then its dependants

Flags are `-g0 -O3 -mips2 -G 0 -non_shared` throughout. All scoring was done in the whole-program unit
(`blob_unit --tag w12g … --neighbours`). Nothing was committed, spliced or pushed, and nothing under
`src/` was edited. There were no permission denials.

| Function | Bytes | State | Exact unit output |
|---|---:|---|---|
| func_800B61A8 (direct-return spelling) | 84 | **EQUAL in the unit, 0 locked bodies differ**; replacement for a locked source | `EQUAL func_800B61A8: 21 words (kept, c_func_800B61A8.c)` / `EQUAL func_800F8EC8: 256 words (kept, g_frontier_car_checkpoints__func_800F8EC8.c)` / `locked bodies that differ in this unit: 0` |
| audio_doppler_full | 952 | near-miss, **improved 119 → 39** | `FAIL audio_doppler_full: 39 of 238 words differ` / `locked bodies that differ in this unit: 0` (run with `--with func_800B61A8.c`) |
| race_countdown_display | 1,120 | not taken (see below); rescored only | `FAIL race_countdown_display: 34 of 280 words differ; compiled body is 279 words, target 280` (direct-return form canonical) vs `72 of 280` (locked new_var form) |

## 1. func_800B61A8: the direct-return spelling is safe to land

Deliverable: `cloud/work/frontier/w12g/func_800B61A8.c`. It is a byte-for-byte copy of `src/blob/func_800B61A8.c`
except for the function body (and a header comment above it):

```c
s32 func_800B61A8(s32 arg0, s32 arg1, s32 arg2, unsigned char arg3)
{
  if (D_8010FFC0 == 0) { return -1; }
  if (arg0 == (-1)) { return -1; }
  return entity_flags_apply(arg0, arg1, arg2, arg3);
}
```

Evidence:
```
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w12g score func_800B61A8 func_800F8EC8 car_setup_confirm func_800D1AB0 \
      --with cloud/work/frontier/w12g/func_800B61A8.c --neighbours
  EQUAL func_800B61A8: 21 words (kept, c_func_800B61A8.c)
  EQUAL func_800F8EC8: 256 words (kept, g_frontier_car_checkpoints__func_800F8EC8.c)
  EQUAL car_setup_confirm: 221 words (kept, g_frontier_car_checkpoints__car_setup_confirm.c)
  EQUAL func_800D1AB0: 140 words (internal, g_frontier_car_checkpoints__func_800D1AB0.c)
  locked bodies that differ in this unit: 0
blob_unit score: 4/4 equal
```
`--neighbours` compares every locked body in the lock (`run.compare(sorted(run.lock))`), so 0 means
nothing in the unit moved.

**Which locked bodies inline it.** The unit's `umerge.log` has exactly one `inlining func_800B61A8`, under
`func_800F8EC8` (group `frontier_car_checkpoints`). No other staged source calls it. The other locked callers
in `cloud/matches/` (players_frame_update, players_race_update, save_load_data, func_800A464C, func_800B9F60,
func_800DD45C, func_8008A644, audio_effect_remove, func_800A8F38) only carry the shared prototype. Retail has
no `jal func_800B61A8` anywhere, so every real caller inlines it. Only func_800F8EC8 is locked today, and it
stays EQUAL.

Standalone and group builds (builder scratch `w12g`, `tools/cloud/score.py`):
- `score.py fn cand/func_800B61A8.c func_800B61A8`: `MATCH` with `-O2` and with `-O3` (the locked form also matches both).
- `score.py group` on a copy of `src/blob/groups/frontier_car_checkpoints` with its `func_800B61A8.c` context
  replaced by the new file gives car_setup_confirm `MATCH`, func_800D1AB0 `MATCH` and func_800F8EC8 `MATCH`
  (context func_800B61A8 `MATCH`). These are the same results as the unchanged group.

### Integration note (func_800B61A8)
- Replace `src/blob/func_800B61A8.c` with `cloud/work/frontier/w12g/func_800B61A8.c` and re-splice the single
  through the normal `blob_splice` → `blob_rom` path. The lock entry's source hash changes, but the bytes do not.
- For consistency, also replace the context copy `src/blob/groups/frontier_car_checkpoints/func_800B61A8.c`,
  which group.json describes as an "unchanged copy of src/blob/func_800B61A8.c". The group verifies with
  either copy.
- No unit_overrides entry is needed. With this form canonical, candidates for race_countdown_display and
  audio_doppler_full no longer need their own `__inline` definition: a plain prototype plus the unit's
  definition gives the same bytes as w11d's in-file `__inline` (119 and 34). With the locked `new_var` form
  the same candidates score 125 and 72.
- `cloud/matches/func_800B61A8.c` already uses this spelling (with `-O2`).

## 2. audio_doppler_full: 119 → 39 / 238 (`audio_doppler_full/best.c`)

Command: `cloud/work/frontier/w12g/tools/sc.sh audio_doppler_full cloud/work/frontier/w12g/audio_doppler_full/best.c`
(= `blob_unit --tag w12g score audio_doppler_full --with w12g/func_800B61A8.c --with best.c --neighbours`):
`FAIL audio_doppler_full: 39 of 238 words differ   locked bodies that differ in this unit: 0`.
With the locked new_var form it scores 48.

What closed 80 words:
1. **The callee-saved address webs (traced and forced).** ctrace identified the webs in w11d's best: w154 is
   the texture (u16 at 0x80154398, two `&` uses plus three loads, tot 50 → s7), w156 is quad (tot 30 → s8),
   and w157 is colour `&D_801140F4` (tot 30, split). The forced oracle `p1:w156=c21,p1:w157=c22` reproduces
   retail's s-registers (the `p1:w154=s` split force is silently declined, as documented).
   Source that reaches the same colouring: every texture address web must be ≤ 20, so that quad and colour
   (tied at 30) take s7 and s8 in first-appearance order. `&` and any load in one web make ≥ 30, and that web
   wins the tie. So the texture needs three spellings: `&D_80154398` (2 address arguments),
   `*(u16 *)((char *)D_80154368 + 0x30)` (create-call load) and `D_80154394[2]` (two draw-call loads).
   All four assignments of the three spellings give 41.
   - The `if (D_801140F4[0]) {}` lever does not work here. Its dead load adds 10 to colour's tot (30 → 40),
     so colour beats quad and the two swap (50 words).
   - A struct `{u8 color[4]; f32 verts[4][3];}` at 0x801140F4 (colour and vertex template are adjacent)
     produces two walking pointers in the loop (189 words). Refuted.
2. **Frame layout.** Two scalar locals declared between `quad` and `name` put name at sp+176 (retail) with
   frame 256 (41 → 39). Any two of player/i/size/p work.

**Residual (all 39 words): the ugen temp ring is +3 from the create call to the end.** Retail has
`li t5,1; sllv t9,t5,s6; ori t0; li t1` where ours has `t5; t6; t7; t8`. The traced ugen shows that allocation
is pure next-free (29 GP allocations, no ADD/REMOVE after the prologue), so retail made three more GP
allocations between `li 1` and the shift, and ugen or as1 removed their instructions. Tried without movement
on this residual: flags spelled 6 ways (s32/u32 prototype params, unprototyped, `(u16)` cast, swapped
operands, split constant), `1U`, the flags value in a local, the create call through a `p` local, and three
inlined static helper shapes (frame +8, ring unchanged). Best next hypothesis: find a source construct whose
ugen output is `op $T; move $argreg,$T` three times (as1 copy-propagates the move away, as in the line-79
`(s8)` cast), probably in the create call's register arguments. Diff the traced ugen listing
(`SCHED=1 ugt.sh`) of a candidate against that pattern.

## 3. race_countdown_display: not taken

w12b's directory (`d5e64/`, `comp/`, `tools/`) has no notes on race_countdown_display. It neither claims nor
releases the function, so following the assignment I did not attack it. Rescore only (w11d best, with its
in-file `__inline` replaced by a prototype in `race_countdown_display/noinl.c`):
`FAIL race_countdown_display: 34 of 280 words differ; compiled body is 279 words, target 280` when the
direct-return func_800B61A8 is canonical, and `72 of 280` with the locked form. Its residual is still w11d's
as1 cross-block hoist.

## What generalises
1. **Address-web ties: a dead-load lever adds to tot.** `if (G[0]) {}` adds 10 per occurrence to G's address
   web. It is not a pure first-appearance lever: it reorders a tie only when one extra use is wanted.
2. **The webdetail `raw14` of a type-1 (address) web is the referenced object's size** (2 = u16, 0x30 = a
   48-byte local, 0x7fffffff = unknown-size `extern T x[]`), and `raw10` is the offset (sp-relative for
   locals). Use this to tell which ctrace web is which global.
3. **Two declared scalars between two arrays move the lower array by 8 without changing the frame.**
   Declaration order sets the slots even for locals that live in registers.
4. A temp ring offset that holds to the end of a function and starts at one statement means a different
   number of GP allocations in that statement, because ugen allocation is pure next-free.

## Files
- `func_800B61A8.c`: deliverable (replacement for the locked source)
- `audio_doppler_full/best.c` (39), `w11d_best.c`, `noinl.c` and the variants (`a*`/`b*` lever, `c*`/`d*` struct,
  `e*` spellings, `o*` declaration order, `r*`/`q*`/`h*` ring attempts)
- `race_countdown_display/noinl.c`: w11d best without the in-file inline
- `tools/sc.sh NAME FILE…`: unit score with the direct-return func_800B61A8 in place
- Builder scratch: `watchman2:~/rush2049/scratch/frontier/w12g` (toolkit installed via `--reuse wtk`)
