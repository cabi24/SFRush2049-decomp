# r5_e scratch notes (cloud round 5): precursors of drone_ai_update / entity_tick_main

Callees of the pair and their state (game image, `asm/us/blob`):

| function | words | ABI/IPA | state |
|---|---|---|---|
| `math_utility` | 19 | ABI | already in `src/blob/` (a 3x3 f32 copy; not IPA) |
| `func_80092B80` | 49 | ABI | already in `src/blob/` |
| `func_80092FE0` | 47 | ABI | **MATCH** -> `cloud/matches/func_80092FE0.c` (`-O2`) |
| `func_8008B32C` | 29 | ABI | `v2.c`: 4/29 words differ (3x3 scale; only the delay-slot of the exit branch differs) |
| `model_data_load` | 93 | ABI (recursive) | `m1.c`: shape and frame right (48), 80 differ strict; `idx` lands in `a3` (3 extra `move`s) instead of `a0` |
| `model_transform_setup` | 101 | ABI (recursive) | `mts1.c`: same shape as `model_data_load`; note the retail has a duplicate `mode == 0` test |
| `string_copy_format` | 109 | ABI | `u1.c`: frame 160 (a 92-byte buffer), `lwl/lwr` literal copy right; register order and `move t0,a0` differ |
| `matrix_scale_apply` | 25 | IPA (reads `s0`,`t0`) | not attempted; callers are all IPA |

Helpers: `run.sh FILE FN [flags]` (strict score), `nr.sh` (aligned via bigfish/near.py), `odis.py`
(objdump of a compiled file), `gnear.py` (group aligned score and diff hunks).
`mdl_hdr.h` is the ModelSlot struct used by the `mdl*`/`m*`/`h*`/`mts1` experiments.

Tricks that paid off here:
- `func_80092FE0`: one named local per loaded field (`short a = r->a; ... ; T[a].v = *val;` with the
  load and store in the same statement) reproduced both registers and load placement.
- `model_data_load`: `T[idx].flags = t | v` where `t` reads `T[(short)idx]` and the store uses plain
  `idx` reproduces the retail sext-then-raw index pattern and the register order.
