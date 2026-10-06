# Wave 9, agent w9f: four large game functions

Date 2026-10-06. Builder scratch `watchman2:~/rush2049/scratch/frontier/w9f` (2 threads; my copy of
`w1b/bscore.py` is set to 2 workers, the original uses 4). Nothing committed, spliced or pushed.

All four are strict `MATCH` from `tools/cloud/score.py` standalone at `-O3`, and `EQUAL` in the whole-program unit
(`blob_unit --neighbours`: 0 locked bodies differ). `-O2` was not probed: every prior draft was `-O3` and all four
closed at `-O3`.

| Function | Bytes | State | Flags | Started from | File |
|---|---:|---|---|---|---|
| `net_state_validate` | 2,728 | **MATCH** | `-g0 -O3 -mips2 -G 0 -non_shared` | w7d/w6b best.c, 9/682 | `cloud/matches/net_state_validate.c` |
| `world_physics_tick` | 1,564 | **MATCH** | same | w2h best.c, 305/391 (+3 words, 56 aligned rows) | `cloud/matches/world_physics_tick.c` |
| `net_session_update` | 3,568 | **MATCH** | same | bigfish base.c, 692/892 at -O3 | `cloud/matches/net_session_update.c` |
| `hud_render` | 2,420 | **MATCH**, own .rodata verified | same | w5b best.c, 377/605 | `cloud/matches/hud_render.c` |

Total 10,280 bytes of game code.

## Scorer commands and output (exact)

For each NAME, from the repo root on the Pi:

```
scp cloud/matches/NAME.c watchman2:rush2049/scratch/frontier/w9f/cand/NAME.c
ssh watchman2 'cd ~/rush2049/scratch/frontier/w9f && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"'
python3 -m tools.conveyor.pipeline.blob_unit --tag w9f score NAME --with cloud/matches/NAME.c --neighbours
```

```
net_state_validate:
  MATCH
  EQUAL net_state_validate: 682 words (kept, c_net_state_validate.c)
  locked bodies that differ in this unit: 0

world_physics_tick:
  MATCH
  EQUAL world_physics_tick: 391 words (kept, c_world_physics_tick.c)
  locked bodies that differ in this unit: 0

net_session_update:
  MATCH
  EQUAL net_session_update: 892 words (kept, c_net_session_update.c)
  locked bodies that differ in this unit: 0

hud_render:
  MATCH
    own .rodata verified at 0x80123FC0..0x80123FE8
  EQUAL hud_render: 605 words (kept, c_hud_render.c)
  locked bodies that differ in this unit: 0
```

## Integration notes (for the maintainer lane)

- `world_physics_tick.c` defines the locked `func_800B930C` (arcade `get_next_checkpoint`) verbatim, so umerge
  can inline it, and has a tentative definition `Car D_8014A250[6];` (shares one `lui at`; precedent
  `audio_occlusion`, which is spliced the same way).
- `net_session_update.c` defines the locked seeded LCG `func_800D50E4` verbatim and a `static` one-statement
  wrapper `irand`. Alternative `w9f/net_session_update/b.c` names the wrapper `func_800F34D0` (the caller-less stub
  directly before the function, very likely its remains); that form is EQUAL only with
  `--internal func_800F34D0` (`EQUAL func_800F34D0: 2 words (internal, c_b.c)`), so it would need a
  `prefer_definition` override. The static form needs nothing.
- `hud_render.c` defines the locked `func_8008B2B4` (libc rand) and `func_8008B2E4` (arcade `Random`) verbatim,
  plus two statics (`resource_set_frame`, `vadd`) that are fully inlined.
- `net_state_validate.c` is self-contained (callee `func_800B78A4` declared with its locked signature).

## Per function

### net_state_validate (9 -> 0)

Lane: the w6b/w7d three-way colour tie in the last loop (data4 vs the `D_80150F7C` address). w7d had shown that a
use of `data4` after the loop gives retail's colours but costs a home store and a stack slot, because `data4` is
undefined on the zero-trip path. Fix: write the loop as `i = 0; if (i < D_8014A108) { do { ... } while
(++i < D_8014A108); if (data4) {} }`. Inside the guard every path to the empty check has defined `data4`, so uopt
adds no store; the extended range lowers `data4`'s priority below the address. The check is required (without it:
12 words; inside the loop: 9). About 15 variants.

### world_physics_tick (56 rows/+3 words -> 0)

Arcade `init_cars()` (game/mdrive.c), as w2h found. Levers, in the order they were found (about 60 variants):
1. byte-clear loops: body on its own line (`for (...)` newline `((s8 *)m)[j] = 0;`). With header and body on one
   line as1 fills the delay slot with the pointer increment instead of the store (`.loc` layout matters to as1).
2. `owner == 6` (int, not `6U`): retail's `&D_80151CE8` in a0 (the inlined `func_800B930C`) and +1 word.
3. `gLink[i]` (D_80153E88) indexed directly, as the arcade does, instead of a `config` pointer local: ugen then has
   `.noalias` between car and link, and as1 lifts the three config-byte loads above the byte15 store (22 -> 3 rows).
4. `Car D_8014A250[6];` defined (shared `lui at` for model[0].byte9/byte10).
5. index as one `?:` (or a one-line `if (..) a; else b;`): order of the hoisted `li 6` and `lui`.
6. the helper is the real `func_800B930C`; an inline expression or macro is 15 rows worse (its parameter web is
   needed).

### net_session_update (692 -> 0)

Started from the bigfish natural rewrite (`cloud/work/bigfish/net_session_update/base.c`), which at -O3 already had
892 words and was much closer than w1b's draft (bscore mnem-missing 55 vs 85). Levers, about 150 variants:
1. random numbers: retail's eight sequences are the kept seeded LCG `func_800D50E4(&D_80154658)` (it has no jal
   callers anywhere: inlined) wrapped by `(u32)func_800D50E4(&seed) % (max + 1)` returning u16. The `(u32)` is
   what gives divu; without it every site is a signed divide (+27 words). Mirror bit `(u16)(rand & 1)`.
2. struct layout errors in the draft: entrant `skill[12]` is at 0x34 (b28 is 24 bytes, not 16), so the column copy
   wrote the wrong offsets; `D_80155148` rows are `u16[6]`, not `[38]`.
3. one `i` for all outer loops (retail's t0 + s3 = i+1 pair), the drone shuffle on `i`, the sort's inner loop
   `k < 6`, the final places one `for (i = 0; i < 6; i++)` loop (retail keeps i = 2 unfolded after the 2-peel).
4. results-decode entrant count is a second variable `m`, declared after `idx` so `idx` stays at sp+264;
   `u8 pad[8]` frame filler (the inlined helper's slots took the rest of bigfish's `pad[128]`).
5. last 19 words: the sort's unrolled body. Traced (`w7d` pretrace on the unit): the three values are coloured by
   uopt's second pass (p2), which goes in web-number order; `D_80154450[idx[i]].h2` (web 456) took v1 before
   `&idx[k]` (457) and `idx[k]` (458). CDX oracle `p2:w456=c4,p2:w457=c2,p2:w458=c3` -> 0 rows. Fix: the drone
   shuffle draws into `k` (`do { k = irand(cnt - 1); } while (i == k); idx[i] ^= idx[k]; ...`). Declaration
   order, operand order, casts and the sort's own variable names do not move it (720 + ~60 variants). Inferred:
   the earlier `idx[k]` gives those expressions lower bit numbers.

### hud_render (377 -> 0)

w5b's structure was right; the residuals were helpers, alias regions and one semantic swap. About 120 variants.
1. type-4 jitter is arcade `Random(max)` = `func_8008B2E4` on libc rand `func_8008B2B4` (both kept, inlined):
   `Random(0.3f) - 0.15f`, `Random(0.25f)`, `Random(0.15f) - 0.075f`. Its inlined locals explain 8 of w5b's
   unexplained frame-filler words (pad3 removed).
2. `if (type >= 6) +0.03 else +0.05`: w5b had the arms swapped (semantic bug, visible as `bnez`/`beqz`).
3. attached path: `position += velocity` is an inlined static `vadd(a, b, out)` (single call site): operand order
   of the adds; frame adjusted.
4. both `player = &D_80152818[car]` through `(Player *)(u32)`: stops `.noalias $18,$sp`, so as1 no longer lifts
   player loads above the stack-vector stores (retail does not).
5. the type-5 arm body in `do { ... } while (0)`: the model pointer's `.alias` closer was emitted after the arm's
   unconditional jump (a "poisoned" region, w1b item 2), which switched off as1's cross-block hoisting. Proven on
   the listing first (`asmt.sh`: moving only that closer before the `b` gives retail minus three words), then
   found the source form.
6. type 5 writes `D_8011744C[D_8014A250[car].model]` in each row-1 term (one CSE'd load) instead of a `scale`
   local: retail's `mul.s $f18,$f0,$fX` operand order. The stack-vector adds are `position[k] = position[k] + ...`.

## Struct layouts and global types recovered (new or corrected)

```c
/* net_session_update */
Entrant D_80154450[6] (0x4C): +0x00 u8 car; +0x01 u8 place; +0x02 u16 points; +0x04 u8 finish[24];
                              +0x1C u8 pts[24]; +0x34 s16 skill[12]
u16 D_80155148[][6]; u32 D_80154658 (LCG seed, func_800D50E4 state)
/* world_physics_tick */
Car D_8014A250[] (model, 0x808) is defined in the unit's sense (tentative definition)
Route D_80151CE8: +0x02 s16 first (lap loop index), +0x08 s16 count (number_checkpoints)
/* hud_render: free-puff growth is +0.03 for type >= 6, +0.05 otherwise */
```

## What generalises

1. **Kept library helpers are the hidden structure of big leaf functions.** Every "inlined rand" so far is one of
   the locked kept functions: `func_800D50E4(u32 *seed)` (seeded LCG), `func_8008B2B4` (libc rand),
   `func_8008B2E4` (arcade `Random(max)`), and `func_800B930C` (`get_next_checkpoint`). Grep `src/blob` for the
   constant or global (1103515245, `D_80151CE8`) and paste the locked definition into the candidate. Their
   inlined parameters/locals are also the "unexplained frame filler".
2. **A post-loop compiled-out use needs the variable defined on the zero-trip path**: guard the loop
   (`if (n > 0) do { } while`) and put the use inside the guard. No home store, no extra slot.
3. **`.loc` layout is an as1 input.** Loop body on the same line as the `for` vs its own line, and a two-line
   `if/else` vs `?:`/one line, changed delay-slot fills and hoisted-constant order with identical ugen code.
4. **Alias regions decide as1 scheduling in both directions.** Retail reorders and we do not: index the global
   array directly (arcade style) instead of through a pointer local. Retail does not reorder and we do: launder
   the pointer with `(T *)(u32)`. A region closed after an unconditional `b` kills cross-block hoisting:
   `do { ... } while (0)` around the arm body moves the closer before the jump. To find which closer retail lacks,
   edit the ugen listing (`o3s.sh` -> move one `.alias` -> `asmt.sh`) before searching source.
5. **uopt's p2 colouring is in web-number order**, so the register of a short expression can depend on whether
   the same expression (or its sub-expression, e.g. `idx[k]`) occurs earlier in the function. Variable identity
   across distant phases (which variable a shuffle draws into) is a lever; declaration order is not.
6. **Prior drafts can carry real bugs**: two wrong struct offsets (net_session_update) and swapped if-arms
   (hud_render) survived earlier waves because positional word counts hid them. Check offsets and branch sense
   against retail before register work.
7. The w7d trace tools work from a fresh agent copy (`w9f/tools/tr/`: `pretrace.sh`, `dec.sh`, `force.sh` with
   `p2:` force specs); the CDX oracle settled the last lane of net_session_update in one run.

## Files

| Path | What |
|---|---|
| `cloud/matches/{net_state_validate,world_physics_tick,net_session_update,hud_render}.c` | the matches |
| `net_state_validate/c1.c`, `world_physics_tick/{base,r4,f}.c`, `hud_render/h15.c` | intermediate sources |
| `net_session_update/s10.c` | 19-word version (before the `k` lever), `s10sw.c` its swapped-compare control |
| `net_session_update/a.c`, `b.c` | final static-helper form / stub-named form (needs `--internal func_800F34D0`) |
| `tools/` | w1b helpers retargeted to `w9f` (`bscore.py` at 2 threads), `tools/tr/` w7d trace scripts retargeted |

Trace snapshots (`runs/`) were deleted; regenerate with
`cloud/work/frontier/w9f/tools/tr/pretrace.sh NAME FILE.c LABEL` (needs `~/rush2049/scratch/frontier/w9f/uopt/uopt`,
a copy of w7d's instrumented uopt).

No permission denials occurred.
