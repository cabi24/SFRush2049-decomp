# render_large_objects (0x800F93A0)

| item | value |
|---|---|
| words | **1413** (blob_800f8d9c.s) |
| spliced? | No (only prototypes in groups' group.c). |
| ABI or IPA | Entry is **ABI** (void, saves ra, s0-s8, f20-f30, frame 472). **But** it calls `func_800F92C8` (52w) 12 times, and that callee reads **$f16, $f18, $f20 before writing them**: IPA float params (f12,f14 ABI + f16,f18,f20). So it needs an IPA group: `{render_large_objects, func_800F92C8, func_800DE860}`. |
| closure | Closed and small: `func_800F92C8` caller = only render_large_objects; `func_800DE860` (211w, reads f14 ABI, saves f20 itself) caller = only render_large_objects. Total **1676 words**. Its own caller `render_viewport_init` (231w) does not depend on IPA state (no args, callee saves everything) so it need not be in the group. Not in any existing `ipa-groups` dir; `closure.py` would list it as its own tiny group. |
| callees | `func_800DE860` x1, `func_800F92C8` x12 (nothing else; no libc, no libultra) |
| globals | 19 lui targets: car table `0x8014A250` (stride 0x808; fields +0x7C6 order s16, +0x7CA s16, +0x7CC s8 kind 1/2, +0x7E6 s16, +0x7EC/+0x7F0 f32), record table `0x80152818` (stride 0x3B8; +0xE E place s8, +8 pos f32[3], +0x100 f32, +0x356 s16), `0x80152744` (num cars s8), `0x801174B4` flags, `0x801543CC` timer, ~40 f32 literals in rodata 0x8012463C-0x801247E8 |
| shape | **Rubber-banding / catch-up tuning per car** (arcade-flavoured: rank cars, count human vs drone, pairwise squared distance table, then per car choose a speed/accel scale from `1 - k*(a*f22+b)*f28` forms and clamp toward the previous value with a rate limit). 538 FP instructions, only 43% repetition (the 12 interpolation blocks are the repetitive part). Logic heavy, not table code. |

`func_800F92C8` itself is a clamped linear map: `f(x, lo, hi, out_lo, out_hi)` (12 words per case, `abs.s` + `div.s`); it is a good 52-word standalone warm-up (needs the group compile for f16-f20).

## First pass
- m2c seed (`seeds/render_large_objects.m2c_raw.c`, 433 lines) does **not** compile as generated: undeclared `spXXX` arrays (the m2c stack arrays `sp178/188/194/1A0/1AC` are five `s16[6]` tables), int-as-pointer casts, `M2C_ERROR` "read from unset register $t2/$a1/$t0" (m2c lost the loop variable and the car pointer; they are ordinary locals) and it mis-types the `func_800F92C8` args (real: 5 floats).
- Hand head (`seeds/render_large_objects_head.c`): typed structs and plain loops for the sort/count/pairwise-distance part, compiled at -O2: 532 words for roughly the first 35-40% of the function. Alignment against the target was inconclusive (opcode-shape 168 of the 532 words) because my partial function does not reproduce the same control flow, so **no trustworthy closeness number** for this function. Treat it as unmeasured.

## Feasibility: MEDIUM
For: closed 3-function group; entry is ABI; small callee count; the algorithm is readable from m2c and probably has an arcade twin (arcade catch-up code, see `dynamic_difficulty`/`catchup_logic` INDEX rows); float code has strict scheduling but few surprises once expression order is right; existing IPA group tooling (`zbuild.py`, `score.py group`) applies directly.
Against: 1413 words of mixed integer/float logic with 12 near-identical but not identical float blocks (each has different literal order; the literals are separate rodata words whose load order must match); float constant pool order is only checkable with `--allow-unverified`; the pairwise-distance loop with a 6x6 `spBC` table and packed `s16` arrays has precise stack layout requirements.

## Recommended approach
1. Write `func_800F92C8` and `func_800DE860` first as a 2-function group (with a `__standin_render_large_objects` caller to keep IPA honest, see resource_slot_clear example); confirm `func_800F92C8` gets f16/f18/f20.
2. Hand-write from the m2c body with real structs (Car, Rec), five `s16[6]` locals in the order sp178..sp1AC, `f32 d2[6][6]` at spBC.
3. Match section by section with `zbuild.py --as1=-r4300_mul` (it prints emitted size per function).
4. Do the 12 float blocks last, as one helper-free block each in source order of the asm.

## Effort
2-4 days. 1 day gets the two small callees and the integer half. Payoff 1676 words for 3 functions; highest match probability per word of the five.

## Hail mary result (2026-09-30)
Group dir `cloud/work/ipa-groups/render_large_objects/` (see STATUS.md there):
`func_800F92C8` strict MATCH (52/52, args are a,b,x,outA,outB), `func_800DE860`
structure reproduced (203 vs 211 words; loop var must be `s32`, `bd` local, test on
`D_80150F14 == 1`) but prologue/FP hoisting differ, `render_large_objects` untouched.
