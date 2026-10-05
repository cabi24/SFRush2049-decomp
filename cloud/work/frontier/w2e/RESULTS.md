# Frontier wave 2 — w2e results

Agent `w2e` (two sessions: the first was cut off before this file; the second re-scored everything
below against the current lock, 759 locked, builder copy refreshed from `scratch/frontier/base`).
Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared` (`-O2` tried on each near-miss: far worse).

| # | Function | Bytes | State | Deliverable |
|---|---|---:|---|---|
| 1 | `world_gravity_apply` | 2,644 | **code identical, own rodata unverified** (5 literals, bits checked); EQUAL in unit | `cloud/matches/world_gravity_apply.c` |
| 2 | `func_800CB9D0` | 516 | **strict MATCH in a real group** (no stand-ins); EQUAL in unit | `groups/frontier_heap_move/` (claims `func_800CB9D0`) |
| 3 | `audio_output_setup` (+ stub `func_800BAF90`) | 148 (+8) | **strict MATCH in a real group**; EQUAL in unit with `--internal func_800BAF90` | `groups/frontier_heap_free_total/` |
| 4 | `func_800E2D18` | 488 | **strict MATCH** (single); EQUAL in unit | `cloud/matches/func_800E2D18.c` (re-scored, unchanged) |
| 5 | `sound_play_menu` | 332 | 14 / 83 words off | `sound_play_menu/best.c` |
| 6 | `func_800E7FA0` | 1,244 | provisional lane, 40 / 311 words off with stand-in caller | `func_800E7FA0/best.c`, `func_800E7FA0/grp/` |
| 7 | `init_state_continue` | 712 | provisional lane, 89 / 178 strict (50 aligned rows) with stand-in caller | `init_state_continue/best.c`, `init_state_continue/grp/` |

Spliceable now: 1–4 (3,796 bytes of game code). 6 and 7 can never be more than provisional until
their callers match (`func_800E847C`; `display_list_flush`, `countdown` — all unmatched).

All `blob_unit … --neighbours` runs for 1–4 report `locked bodies that differ in this unit: 0`.

---

## 1. world_gravity_apply — code identical (own rodata unverified)

```
sc.sh world_gravity_apply/best.c world_gravity_apply --flags "-g0 -O3 -mips2 -G 0 -non_shared"
world_gravity_apply:
  MATCH (10 section-relative relocations unverified: .rodata+0x0 at +0x48, .rodata+0x0 at +0x4c, .rodata+0x4 at +0x50, .rodata+0x4 at +0x68, .rodata+0x8 at +0xf4, .rodata+0x8 at +0xfc, .rodata+0xc at +0x3c4, .rodata+0xc at +0x3c8, .rodata+0x10 at +0x694, .rodata+0x10 at +0x6a4)

python3 -m tools.conveyor.pipeline.blob_unit --tag w2e score world_gravity_apply --with cloud/matches/world_gravity_apply.c --neighbours
  EQUAL world_gravity_apply: 661 words (kept, c_best.c)
  locked bodies that differ in this unit: 0
```

Own literals, in first-use order, against the image (`rd.py 80124574 5 f`): `10000.0f` = 0x461C4000,
`2500.0f` = 0x451C4000, `1e20f` = 0x60AD78EC, `40000.0f` = 0x471C4000 twice. Splice as a single
(`splice_singles.py world_gravity_apply`); the splice verifies the five words.

Semantics, shaping quirks and recovered types are in the file header: per-car nearest path point
search (main path successors, side sets, checkpoint segment, full wrap-around scan) and the
distance along the track at Car +0x100. Quirks: a second pointer `m2` to the same Model used only in
two loop conditions; element access written out (`D_801407F0.points[idx].pos[0]`), never through a
pointer temporary; `end = 5; j < end` in two loops; operand orders `gc->unkFE != gc->unkFC`,
`rel[0]*d[0] + rel[2]*d[2]`; `best` reused for the projection; declaration order for the frame.
Types: `PathPoint { s16 pos[3]; }`, `PathSet { u8 type; u8 pad[9]; u16 count; PathPoint *points; }`
(0x10), checkpoint lengths at +0x58 with 0x50 stride inside `D_80151CE8` (block layout still open).
No arcade ancestor (nearest relative: path_dist code in scp.c).

## 2. func_800CB9D0 — strict MATCH in the real heap group

```
grp.sh groups/frontier_heap_move        (score.py group)
...
func_800CB9D0:
  MATCH
(all 9 members and 6 context functions: MATCH)

python3 -m tools.conveyor.pipeline.blob_unit --tag w2e score func_800CB9D0 --with cloud/work/frontier/w2e/groups/frontier_heap_move/func_800CB9D0.c --neighbours
  EQUAL func_800CB9D0: 129 words (kept, c_func_800CB9D0.c)
  locked bodies that differ in this unit: 0
```

The group is the locked `src/blob/groups/codex_heap_release_a25` (group.c, alloc_at.c,
car_damage_visual.c copied unchanged from the current src) plus `func_800CB9D0.c`; claims only
`func_800CB9D0`. It cannot be a plain single: it calls the internal `audio_reverb_update` (address in
a1, tag in a2, s0/s1 unsaved — which is why this function saves s0/s1 without using them).
Integrate by replacing `src/blob/groups/codex_heap_release_a25` with this directory (drop "claims") or
by adding the file to that group; it scores alone with `score.py fn` as 116/129 (no IPA facts).

Semantics: heap compaction step — find the block holding `addr`, look for an earlier free block that
fits (or the free block immediately in front), move the data (memmove `func_800A47C0`), repoint the
owner handle, split off a free header for the remainder and free the old copy through
`audio_reverb_update`.

What closed it (from 25 words at the takeover):
- `n->size = b->size - used;` before `n->owner = 0; n->used = adjacent;` (as1 then hoists the b->size
  load over the stores): 25 -> 18.
- **No `src` variable**: `if (adjacent) old = n;` and `audio_reverb_update((u8 *)old + 32, 0)`. The freed
  address is then the CSE'd memmove source `old + 32`, spilled at 36(sp) and reloaded straight into a1.
  Every `src` spelling (before/after the memmove, typed u32/void*/Block*, ternary at the call, static
  release helpers like w2a's `release_handle`, dst/src locals) colours it a0 or t3 and costs a move
  (18–77 words). 18 -> 0.
- `s32 pad[7]` after the named locals for the 104-byte frame (which real locals sat there is unknown).

For w2a's `func_800CB748` hint: this function does *not* use the three-statement
lock/`audio_reverb_update`/unlock sequence as a helper (the lock is taken at the top and released on
two paths), so it says nothing about `release_handle`'s shape.

## 3. audio_output_setup — strict MATCH in a real group

```
grp.sh groups/frontier_heap_free_total
Members:
func_800BAF90:
  MATCH
audio_output_setup:
  MATCH

python3 -m tools.conveyor.pipeline.blob_unit --tag w2e score audio_output_setup func_800BAF90 --with cloud/work/frontier/w2e/groups/frontier_heap_free_total/group.c --internal func_800BAF90 --neighbours
  EQUAL audio_output_setup: 37 words (kept, c_group.c)
  EQUAL func_800BAF90: 2 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
```

Without `--internal func_800BAF90` the unit run fails (0/2: the stub is kept and not inlined), so when
this lands `func_800BAF90` must stay out of keep and the unit probably needs a `prefer_definition`
entry for `func_800BAF90` pointing at the group file (it is locked today as the empty stub), as for
`func_800A51D8`. Total free bytes of a heap under the heap lock; the worker is the deleted static at
the `jr ra; nop` stub 0x800BAF90 and must be defined *after* its caller (as1 line order decides where
`move a2,zero` lands; 2 words otherwise). As a single function (`audio_output_setup/v1.c`) it is 28/37.

## 4. func_800E2D18 — strict MATCH (re-scored, unchanged)

```
sc.sh cloud/matches/func_800E2D18.c func_800E2D18 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800E2D18:
  MATCH
blob_unit --tag w2e score func_800E2D18 --with cloud/matches/func_800E2D18.c --neighbours
  EQUAL func_800E2D18: 122 words (kept, c_func_800E2D18.c)
  locked bodies that differ in this unit: 0
```

Arcade `enginetorque()` tail (drivetra.c); quirks in the header (interp() as a real function inlined
three times, `(short)` divisor cast, `right *= …`). The file also defines the locked
`func_800E2CC0` with the same words.

## 5. sound_play_menu — 14 / 83 words

```
sc.sh sound_play_menu/best.c sound_play_menu --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  14/83 words differ (1 extra words (nonzero beyond target length))
blob_unit --tag w2e score sound_play_menu --with …/best.c
  FAIL sound_play_menu: 14 of 83 words differ; compiled body is 84 words, target 83
```

Allocate from the end of a game heap (mirror of `audio_helper`). Residual lane (workbench `diagnose`,
via the new `diag.sh`): pool colouring, webs a0->a1 x4 and a1->a2 x1 — retail colours the new block
`n` into a0 and hoists *both* osJamMesg zero arguments above the split test; here `n` takes a1 (plus a
`move a0,a1` copy, the extra word) and only a2's zero is hoisted. ~300 variants without movement
(listed in the header: all 120 declaration orders, link/return spellings, prototypes, lock/unlock and
worker helpers before/after the caller, result variables, `#line` probes, `-O2`).
Best next hypothesis: a construction where `n` is an existing variable (what closed func_800CB9D0),
or an instrumented-uopt colouring trace of `n`'s web to see what blocks a0.

## 6. func_800E7FA0 — provisional lane, 40 / 311 words

```
grp.sh func_800E7FA0/grp          (keep = standin_caller)
    +0x1e4  want 50920041 beql a0,s2,0x108              got 50910041 beql a0,s1,0x108
    +0x314  want 166a0003 bne s3,t2,0x10                got 164a0003 bne s2,t2,0x10
  40/311 words differ
```

Only caller `func_800E847C` (2,100 bytes, four other unmatched callees; the old codex group's version
of it is 521/525 off), so provisional at best. From 199 aligned rows at the takeover to 40 words; the
stand-in caller's shape does not matter (three shapes, identical). What moved it is in the header;
the general ones: `sample` is read before written on the default switch path, so it gets a home
loaded before and stored after the loop, and *the number of locals declared before it sets the frame*
(three -> 16 bytes); dropping the `a`/`b` float locals in favour of `fabsf(x) > fabsf(y)` re-coloured
the whole FP pool to retail's (f20/f22 for the samples, f2/f12/f14/f16 constants); a temp for the
interpolated level (retail compares the first threshold against the register and reloads for the
second). Residual: the pool order of car->level's induction pointer (retail t5) against the segment
pointer/hi (retail s0/s1) — swapped here, which renumbers s1..s5; and retail initialises `i`/`i*4`
before the `D_8013FECB` test. Types: Car 0x808 bytes (+0x4 params with f32 at +0x20, |float| pairs at
+0x4DC/+0x594, +0x480/+0x538, +0x484/+0x4E0, +0x598/+0x53C, u16 wheel[4] at +0x61C, u32 flags at +0x7D4,
f32 level[4] at +0x7F8); `Curve { s32 level[5]; f32 first, second; }` (0x1C) x4 at D_80120EEC; bit masks
D_80120EBC/ECC/EDC[4].

## 7. init_state_continue — provisional lane, 89 / 178 strict (50 aligned rows)

```
grp.sh init_state_continue/grp    (keep = standin_caller)
  89/178 words differ
gfull.sh init_state_continue/best.c init_state_continue standin_caller
want 178 words, got 178; differing rows 50
```

Frameless, writes s0–s8 unsaved; real callers `display_list_flush` and `countdown` are unmatched.
Previous best was 181 words / 57 rows (it kept the constant 6 in `ra` and built a frame). Moved: the
mode 4/5/6 test as a `switch` (the `||` chain merges the early 6 with the loop bound into one web,
coloured ra) and the player-index ternary inline in the subscript. Residual: operand order of the
case-6 compare, the retry-loop rotation (`while (1)`/`for (;;)`/goto give retail's bottom test but
shift the t-registers: 90 rows), frand-block temp numbering. Semantics and the slot record layout
(`Config` 8 bytes, `Player` 76 bytes) are in the header.

---

## Generalises

1. **Reuse an existing pointer instead of a new local for a value the compiler already CSE'd.**
   `if (adjacent) old = n; free(old + 32)` matched where every `src` local missed by 18+ words: the
   CSE'd memmove argument becomes the variable and is spilled/reloaded in its argument register.
2. **Read-before-write locals get a stack home, and frame size counts the locals declared before
   them.** In `func_800E7FA0` the frame was `4 × (position of sample in the declaration list)`, rounded up to 8:
   slots are assigned to every local declared before the first one that needs a home. Choosing which
   locals precede it fixes the frame without pad arrays.
3. **Named float temporaries can steal the low FP registers from loop constants.** Removing `a`/`b`
   (writing `fabsf(x) > fabsf(y)` and letting CSE share the loads) moved the samples to f20/f22 and the
   constants to f2–f16, exactly retail's pool.
4. A `||` chain against a constant that is also a loop bound can make uopt keep that constant in a
   register across the loop (here in `ra`, which adds a frame); a `switch` does not.
5. Tool: `cloud/work/frontier/w2e/diag.sh cand.c NAME [keep,…]` runs workbench `diagnose` without a
   target object: it compiles the candidate on the builder, copies the object and overwrites the
   function's words with the retail words, then diagnoses locally (`mips-linux-gnu-objdump`).
   `udiff.py NAME` gives the aligned diff of the last `blob_unit --tag w2e score` run.
