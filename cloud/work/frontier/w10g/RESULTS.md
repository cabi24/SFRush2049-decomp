# Wave 10, lane w10g: results

All unit scores: `python3 -m tools.conveyor.pipeline.blob_unit --tag w10g score ... --neighbours` from the
repo root on the Pi (2026-10-06, tree at 29ffa0e4 plus local edits). Builder scratch:
`watchman2:~/rush2049/scratch/frontier/w10g`. No permission denials.

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| `func_800DC628` | 248 | **strict MATCH** (single) | `-O3` | `score.py fn`: `MATCH` / `own .data verified at 0x801170F8 (4 bytes ...)`; unit: `EQUAL func_800DC628: 62 words (kept, c_func_800DC628.c)`, `locked bodies that differ in this unit: 0` |
| `func_8008B640` | 92 | **strict MATCH** (group) | `-O3` | `score.py group cand/g_8008B640 --claims`: `func_8008B640: MATCH`; unit: `EQUAL func_8008B640: 23 words (internal, c_group.c)` |
| `physics_velocity_integrate_a` | 712 | **strict MATCH** (group) | `-O3` | `physics_velocity_integrate_a: MATCH`; unit: `EQUAL physics_velocity_integrate_a: 178 words (internal, c_group.c)`, all 6 old members EQUAL, `locked bodies that differ in this unit: 0` |
| `best_times_display` | 224 | provisional | `-O3` | unit: `EQUAL best_times_display: 56 words (internal, c_best.c)`, 0 locked differ, with the real-caller draft `func_800D5E64` (129/146 off) plus one stand-in for `mode_select_handler` |
| `mode_select_input` | 152 | provisional | `-O3` | unit: `EQUAL mode_select_input: 38 words (internal, c_best.c)`, 0 locked differ, with the drafts of its real callers `func_800DFBA0` (293/298 off) and `func_800E05F0` |
| `menu_back` | 132 | 3 words off | `-O3` | unit: `FAIL menu_back: 3 of 33 words differ` (internal, with the locked group's context draft of `func_800CBF2C`) |

## func_800DC628: strict, `cloud/matches/func_800DC628.c`

This function opens the bit-packing buffer that `func_800DC1AC` and `func_800DC57C` use. On the first call it builds the
reverse lookup of a 32-symbol alphabet. It then sets the byte count `(bits+extra+7)>>3`, rejects more than 32 bytes or
more than 32 extra bits, clears the buffer and resets the cursor.
- **Closer:** the one-shot flag at `0x801170F8` is a **function-local `static s32 = 1`**. Declared as `extern s32`, IDO
  shares one `lui/addiu` base between the flag's load and its store, which leaves the function 10 words off. As a local
  static, the load and the store each get their own `lui`, as in retail. The scorer verifies the own `.data` word.
  Nothing else references `0x801170F8` (game_globals: lw 1, sw 1).
- **Integration:** run `splice_singles.py func_800DC628`. The splice places and verifies the 4-byte own `.data` word.

## func_8008B640 + physics_velocity_integrate_a: group `cloud/work/frontier/w10g/groups/func_8008B640/`

- **Supersedes the locked group `src/blob/groups/func_8008B640`.** The members list keeps all six old members
  (`model_bounds_calc`, `physics_velocity_integrate_b`..`_f`) and adds the two claims. Steps: `blob_group revert
  func_8008B640`, copy the new dir over it (drop `"claims"`), then `blob_group splice func_8008B640` and the usual checks.
  The new group deletes `__standin_func_8008B640` and removes it from `keep`: `_a` now calls `func_8008B640` twice for
  real. `__standin_model_bounds_calc` is unchanged.
- **Closer 1, a deleted inlined static:** `static f32 *model_mat(s16 idx) { return TBL[idx].m; }` is used for the
  `D_8012E708` 0x44-byte slot pointer in `func_8008B640` and at both `func_8008B32C(m, m, t/100)` sites in `_a`.
  This alone settles the old "IPA parameter register" blocker:
  - the index goes to `a0` (it was `a2`);
  - the pointer goes to `a1` and is then copied to `s0`/`a0`;
  - the address is formed as `lui v0; addu v0; lw a1,%lo(v0)`.

  `func_8008B640` went from 9 words off to EQUAL and `_a` from 64 to 5. Two other forms are worse: a helper that returns
  `&TBL[idx]` (19 words off) and a helper that takes an `s32` and casts (130 words off).
- **Closer 2, an inlined locked helper:** the LCG block is arcade `Random(4.0f) + 2.0f`. It uses the static copies of the
  kept `func_8008B2B4`/`func_8008B2E4` (rand/Random), as in `func_800B23E0`. The division result in `$f0` was the sign.
  Calling the extern `func_8008B2E4(4.0f)` also gives EQUAL in the unit.
- **Side note:** `stub tail: 2 words of deleted-procedure stubs follow (NOT as in the image)` appears on `_b` only
  because the old locked group's `__standin_func_8008B640` is still in the unit during scoring. It goes away once the old
  group is superseded.

## best_times_display: provisional, `best_times_display/best.c` (whole harness)

This function resets one player's 5x24-byte best-times slot, calls `scheduler_recv` on the handle, sets it to -1 and
zeroes four per-player globals.
- **Closer:** one 5-iteration loop `for (i = 0; i < 5; i++) D_80140808[idx].e[i]... = 0` that indexes the global
  directly. IDO unrolls it by 4 with a one-iteration remainder, which is the retail `li a0,1; a0*24`. The old stop note
  ("unfolded induction variable") was this unroll remainder. Using a pointer local costs 6 words.
- **Why it stays provisional:** both real callers are unmatched. `func_800D5E64` is a draft, 129/146 words off, and
  `mode_select_handler` is a 2,976-byte function stood in by `__standin_msh`. With only one caller the unit inlines it,
  so the second caller is needed. The `s2` parameter came out right with these callers.

## mode_select_input: provisional, `mode_select_input/best.c`

The source is the A168/A169 helper from `cloud/work/ipa-groups/dot_layer_update_20261005/candidate.c`. I removed the
locked context definitions (`func_80091BA8`, `entity_transform_calc`, `client_sync`, `scheduler_recv`). Leaving them
in broke the locked body `func_800F0674` in the unit.
- **It needs the `__inline` definition of the locked `entity_hierarchy_update`.** With only a prototype it is 14/38 off
  (24-word body, call not inlined).
- Its real callers `func_800DFBA0` and `func_800E05F0` are unmatched drafts, so it cannot land until they do.
- **Integration needs:** a `prefer_definition` (or equivalent) override for the `__inline entity_hierarchy_update`
  definition.

## menu_back: 3 words off, `menu_back/best.c`

- **Residual:** an `as1` delay-slot pick at the `format_string_parse(m->buffer, n)` call.
  - Retail: `move a1,v0; sw count; jal; lw a0,76(s0)`.
  - Ours: `sw count; lw a0; jal; move a1,v0`.
- **Unit listing (ugen `-l` on the unit stage):** we emit `lw $4` before `move $5,$2`, so in retail `n` was copied to
  `a1` before the `m->buffer` reload.
- **Tried, about 17 variants, none moved:**
  - temporary or extra-local forms of `n`/count, `register`, `u32 n`;
  - assignment-in-expression, early return;
  - unprototyped `format_string_parse` and `inflate_entry_alt`.
- **Next hypothesis:** `n` is homed in `a1`, as uopt does for a value whose only later use is outgoing argument 2. Look
  for a source shape where the `n/3` divide reads the return value but the argument reads a copy, possibly an inlined
  helper around `inflate_entry_alt`.
- **Its only caller `func_800CBF2C` is itself unmatched** (57/69 off, context in `menu_cc040_20261004`). Even an exact
  body would be provisional.

## Tools added (lane dir)
- `sc.sh NAME file.c [flags]`: runs `score.py fn` in the builder scratch copy.
- `uvar.py BASE.c FUNC variants.py [blob_unit opts]`: replaces FUNC's definition with each variant and unit-scores it.
- `ulist.sh FUNC`: pre-`as1` listing of FUNC from the last `--tag w10g` unit build. It reruns `ugen -l` on the stage's
  `opt`, which is the only way to see unit-context register choices.
- `udis.sh FUNC`: objdump of FUNC from `build/blob_unit/w10g/unit.o`.

## What generalises
- **A temp of `v0` with the destination elsewhere** (`lui v0; addu v0; lw a1,%lo(v0)`) means an inlined accessor's
  result. `as1` itself always uses the destination register as the temp.
- **`$f0` mid-expression** marks an inlined float-returning helper. Here it was Random.
- **An extern `s32` flag that is read and then cleared,** whose load and store each get their own `lui` in retail, is a
  function-local static.
- **A fully unrolled loop whose induction variable starts at an unfolded `li 1`** is a 5-iteration loop with a
  1-iteration remainder, not a peeled first element.
