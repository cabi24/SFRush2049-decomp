# Wave 3, lane w3b: closing provisional callees

Agent `w3b`, 2026-10-05. Builder scratch `~/rush2049/scratch/frontier/w3b` (copied from `base`), unit runs with
`--tag w3b`. Nothing committed, spliced or edited outside this directory.

| # | Function | Bytes | State | Closes | Where |
|---|---|---:|---|---|---|
| 2 | `func_800E681C` | 716 | **strict MATCH** in a real `-O3` group, no stand-ins | `func_800E6460` (239 w, provisional -> MATCH with its real caller) | `groups/control_input_E681C/` |
| 3 | `func_800F8EC8` | 1,024 | **strict MATCH** in a real `-O3` group, no stand-ins | `func_800D1AB0` (140 w, provisional -> MATCH with both real callers) | `groups/car_checkpoints_F8EC8/` |
| 1 | `audio_mixer_main` | 732 | 31/183 words (unchanged from w2b) | `audio_priority_find` stays provisional | `audio_mixer_main/best.c` |
| 4c | `sound_bank_unload` | 1,248 | 134/312 words (was 262/312), exact length and frame | `func_800B0EA0` stays provisional | `sound_bank_unload/best.c` |
| 4a | `menu_input_process` | 1,280 | not attempted: blocked | - | see below |
| 4b | `camera_update` | 2,876 | not attempted: blocked | - | see below |

New game coverage if both groups are integrated: `func_800E681C` 716 + `func_800E6460` 956 + `func_800F8EC8` 1,024 +
`func_800D1AB0` 560 = **3,256 bytes, 4 functions**. Both formerly provisional callees leave `provisional.json`.

---

## 1. func_800E681C + func_800E6460 (group `control_input_E681C`): MATCH

Flags `-g0 -O3 -mips2 -G 0 -non_shared`; `keep = [func_800E681C]`; members `func_800E627C` (locked, source
unchanged), `func_800E6460`, `func_800E681C`; context `func_800E6AE8` (a retail stub, defined for real here);
claims `func_800E681C`, `func_800E6460`.

```
$ ./grp.sh groups/control_input_E681C          # = score.py group cand/<dir> on the builder
Members:
func_800E627C:
  MATCH
func_800E6460:
  MATCH
func_800E681C:
  MATCH

Context (informational; excluded from exit status):
func_800E6AE8:
  MATCH
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w3b score func_800E681C func_800E6460 func_800E627C func_800E6AE8 \
    --with cloud/work/frontier/w3b/groups/control_input_E681C/group.c \
    --internal func_800E6460 --internal func_800E627C --internal func_800E6AE8 --keep func_800E681C --neighbours
  EQUAL func_800E681C: 179 words (kept, c_group.c)
  EQUAL func_800E6460: 239 words (internal, c_group.c)
  EQUAL func_800E627C: 121 words (internal, c_group.c)
  EQUAL func_800E6AE8: 2 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 4/4 equal; object build/blob_unit/w3b/unit.o (4.4s)
```

**What closed it (w1d left 16/179, one register permutation of three rematerialised invariants):** the gear
selection is its own function, a deleted static. `func_800E6AE8` and `func_800E6AF0` are caller-less stubs
directly after `func_800E681C`. Writing the gear code as `void func_800E6AE8(CarState *st, InputRecord *in)`
**defined after** `func_800E681C`, with early `return`s (including the `autoGear != 0` test) instead of w1d's
`goto pick` join, and calling it once from the loop, gives all 179 words and reproduces the stub. First try of the
hypothesis; the colouring falls out of the changed web-creation order. The button code as `func_800E6AF0` too:
18 words (wrong), so that stub belongs to something else.

**Integration note:** the unit keeps the locked single `src/blob/func_800E6AE8.c` (empty stub) by default. Without
`--internal func_800E6AE8` blob_unit compiles our definition as kept and `func_800E681C` is 106/179. So the
integrator needs a `prefer_definition` entry for `func_800E6AE8` pointing at the group file (as for
`func_800AFD54`/`func_800AFB30`, plan decision D3), and must make it non-kept. The group supersedes
`src/blob/groups/func_800E681C` (stand-in group): revert that first.

## 2. func_800F8EC8 + func_800D1AB0 (group `car_checkpoints_F8EC8`): MATCH

Flags `-g0 -O3 -mips2 -G 0 -non_shared`; files `func_800F8EC8.c` (new), `car_setup_confirm.c` and
`func_800D1AB0.c` (unchanged copies from `src/blob/groups/frontier_car_setup_confirm/`), `func_800B61A8.c`
(unchanged copy of the locked single); `keep = [car_setup_confirm, func_800F8EC8, func_800B61A8]`; claims
`func_800F8EC8`, `func_800D1AB0`. No own `.rodata` (3.0f is `lui`, 0.0f is `mtc1 zero`).

```
$ ./grp.sh groups/car_checkpoints_F8EC8
Members:
car_setup_confirm:
  MATCH
    own .rodata verified at 0x80124178..0x8012417C
func_800D1AB0:
  MATCH
func_800F8EC8:
  MATCH

Context (informational; excluded from exit status):
func_800B61A8:
  MATCH
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w3b score func_800F8EC8 func_800D1AB0 car_setup_confirm func_800B61A8 \
    --with …/car_checkpoints_F8EC8/func_800F8EC8.c --with …/car_setup_confirm.c --with …/func_800D1AB0.c --neighbours
  EQUAL func_800F8EC8: 256 words (kept, c_func_800F8EC8.c)
  EQUAL func_800D1AB0: 140 words (internal, c_func_800D1AB0.c)
  EQUAL car_setup_confirm: 221 words (kept, c_car_setup_confirm.c)
  EQUAL func_800B61A8: 21 words (kept, s_func_800B61A8.c)
  locked bodies that differ in this unit: 0
blob_unit score: 4/4 equal; object build/blob_unit/w3b/unit.o (3.6s)
```

The near_miss_B125 reconstruction (254/256 standalone, 232/256 in the unit, 233 words emitted) was semantically
right but structurally off; I rewrote it from the disassembly. Arcade ancestor: `CheckCPs()`
(`game/checkpoint.c:866`): the per-car checkpoint-plane test, then `find_maxpath_intervals` ->
`world_gravity_apply`, the place sort -> `func_800D1AB0`, the first-place timer, and `SOUND(S_LEADERLIGHT)` ->
`func_800B61A8(16, car, 1, 1)`. Path and levers, each measured in the unit:

| Step | Words | Change |
|---|---:|---|
| fresh arrays-and-pointers draft (`v1.c`) | 244 (255 emitted) | `diff[3]`/`move[3]` arrays, `volatile s16` count |
| `x_noarr.c` → `i1.c` | 244 → 58 | call the locked sound wrapper `func_800B61A8(16, D_8014A118, 1, 1)` instead of open-coding `if (D_8010FFC0) entity_flags_apply(…)`. umerge inlines it: argument set-up before the test, `li a3,1` (u8 parameter), the inverted `bnez …; b end`. It also took three single-use invariants (`&count`, `&D_80152218`, 952) out of s6-s8 |
| `if/else` side | (part of 58) | `if (plane < 0) side = -1; else side = 1;`: the empty-else `b` |
| `p_o1.c` | 57 | dot product in x, y, z order |
| `m_u32.c` | 28 | `m = (Model *)((u32)D_8014A250 + index * sizeof(Model))`. With `&D_8014A250[index]` ugen emits `.noalias $4,$sp` (it knows the pointer's base), and as1 hoists the `m->next_cp` reloads and the `m->side` store across the `diff[]` stack stores. Retail keeps them in source order |
| `k_a.c` | 14 | the crossing time goes through one named local assigned twice (`t = plane / (plane - last); t = D_8002EB94 * t;`): quotient and product in `$f0` |
| `fb1.c` / final | 0 | frame 168: `cent_dist` plus an unused `s16 i` above `diff[3]`, the scalars between `diff[3]` and `move[3]`, an unused `zvec[3]` below |

Layouts recovered: `CarData` (0x3B8, `D_80152818`): `time` +4, `pos[3]` +8, `blocked` +0xEE (s8), `distance`
+0x108, `crashed` +0x358 (s8). Model (0x808, `D_8014A250`): `moving` +0x6C4 (s16, -1 = racing), `next_cp` +0x7E4
(s16), `side` +0x7E9 (s8). `CheckPoint` (0x50, `D_80151CF4`): `pos[3]`, `normal[3]`, `s32 radius` +0x18.
`D_80153E88[]` 8-byte player records, `type` at +7. `D_80152218[][3]` last positions, `D_801527E8[]` last plane
value. Volatile in this function: `D_801543CA`, `D_80153FD2` (s16), `D_8002EB90`, `D_8002EB94` (f32).

## 3. audio_mixer_main: 31/183 (no movement)

`best.c` = w2b's source unchanged. About 45 more variants, all in the unit (`bu.sh … --internal audio_priority_find
--keep audio_mixer_main`). None moved the residual (retail computes `k + 1` three times; ours keeps one copy in an
s-register and moves it into `k`):
- unsigned spellings (`k + 1U` in the test, the arm or the increment), `1 + k`, `k - -1`, `k + 2 - 1`, cast count:
  31 (cfe/uopt canonicalise them all);
- `k == count - 1` and three other rearrangements: 59, extra `addiu -1`;
- two counters `k`/`n = k + 1` in parallel: 42-62 (uopt keeps both IVs);
- `next = k; next++` / `++next` / `(next = k + 1) == count` forms: 58 (one fewer word);
- `get_next_checkpoint` as an inlined helper (arcade `checkpoint.c:847`, the trackers are the checkpoint ring:
  `first` = lap-loop index, `count` = number of checkpoints), as the deleted static at the stub `func_800BA2B0`
  (s32/s16/u32 parameter, `cur++` or `cur + 1`, `==` or `>=`): 31 to 176. A helper that writes its parameter
  changes the whole function's colouring (`mode` moves into s8) and still CSEs `k + 1`;
- a `Tracker *` loop pointer: 61; register pressure (three more invariants across loop 2): `k + 1` still wins a
  register, so the residual is not a lost allocation contest. The CSE web exists in ours and not in retail.
Next hypothesis: none well supported. An instrumented uopt PRE trace (`decomp-workbench trace pre`) on retail vs
ours would show whether retail's three `k + 1` are different ucode expressions.

## 4. sound_bank_unload: 134/312 (from 262/312)

`best.c` (context for `func_800B0EA0`, which stays EQUAL internal):
```
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w3b score sound_bank_unload func_800B0EA0 func_800B0F60 \
    --with …/sound_bank_unload/best.c --internal func_800B0EA0 --internal func_800B0F60 --neighbours
  FAIL sound_bank_unload: 134 of 312 words differ      (emitted 312 = retail)
  EQUAL func_800B0EA0: 48 words (internal, c_best.c)
  EQUAL func_800B0F60: 2 words (internal, c_best.c)
  locked bodies that differ in this unit: 0
```
- The stub `func_800B0F60` (between the interpolator and `sound_bank_load`) is a deleted static
  `func_800B0F60(s32 id, Resource24 *d) { D_8012E700[id].palette = d; }`, inlined at both tail call sites. It
  gives the retail tail colouring (`a0`/`a1` ids, `a2` base, `a3` 68, `t0` descriptor).
- Retail unrolls the two brightening loops by 2 (ours by 4, 512 words emitted). An unused
  `u16 *p = &colors[i];` in the loop body is enough to get 2 (312 words). It is probably not the original: the real
  body is bigger in ucode by about one statement.
- `s32 pad[10]` after `handle` gives frame 192 with `name`/`colors`/`handle` at 160/156/154.
- Residual: (a) ugen temp-ring phase in the brightening body. The first divergence is the second `or` of
  element 1 (retail `t7`, ours `t8`), so the event sequence differs inside that statement. Six `GPACK`/operand-order
  spellings did not move it. (b) Colouring of the ramp variables (retail ratio t0, cursor a3, colors t3, third
  colour t4; ours a3, t0, t4, t3). All 27 declaration orders give the same result. (c) The descriptor spill slot is
  at 48 in retail and 44 in ours.

## 5. Not attempted: menu_input_process, camera_update

- `menu_input_process` (closes `func_800B66B0`): `frontier show` lists `blockers: audio_doppler_calc` (unmatched,
  1,124 bytes, register parameters s0/s2, itself blocked by the near-miss `func_80087110`). It also preserves
  a2/a3/t1-t5 across calls, so it cannot be matched until those callees are. The prior context reconstruction
  (`codex_defaults_a16`) is 320/280 words.
- `camera_update` (closes `camera_track_spline`): 2,876 bytes with blockers `camera_free_look`,
  `camera_look_at_point`, `entity_spawn_callback` (all unmatched) and 12 temp-ring wraps.

## What generalises

1. **A caller-less stub next to a near-miss is the first thing to try.** Two of three closings came from it
   (`func_800E6AE8`, `func_800B0F60`, and the locked `func_800B61A8` inlined into `func_800F8EC8`). Defining the
   helper for real changed register colouring, temp order and unroll factor where some 45 source variants had
   failed. The helper's position in the file follows the stub (defined after the caller when the stub follows it).
   In `blob_unit score`, pass `--internal STUB`, or the locked kept single wins and nothing inlines.
2. **Calling a locked small wrapper instead of open-coding it** (`func_800B61A8` = `if (!flag) return -1; if (a
   == -1) return -1; return entity_flags_apply(…)`) is a strong signal when retail sets the arguments up before a
   flag test and shows an inverted `bnez; b end` branch.
3. **An integer cast on an array address removes ugen's `.noalias` facts** (`(T *)((u32)base + i * sizeof(T))`).
   Use it when retail keeps loads and stores through that pointer in source order around stack stores but ours
   lets as1 reorder them. Inspect the `.noalias` lines in the pre-as1 listing (`o3s.sh`).
4. **A named local assigned twice** keeps an FP chain in `$f0` (`t = a / b; t = k * t;`).
5. **Frame:** named arrays and scalars take slots top-down in declaration order, and compiler temps sit at the
   bottom (36 bytes in `func_800F8EC8`). Unused declarations above, between and below the arrays place each array
   independently.

## Tools (this directory)
`bu.sh FILE NAME… [blob_unit args]` (unit score summary), `cmp.py FN [--full]` (aligned diff of the unit object
against retail with relocation names), `sc.sh`/`grp.sh`/`full.sh`/`o3s.sh` (w2b scripts retargeted to `w3b`).
