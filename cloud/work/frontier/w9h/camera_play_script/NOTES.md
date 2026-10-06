# camera_play_script (0x800C5644, 880 words): not matched, first structural draft

The name is a historical label. This is not a camera function. It tests the car against one track polygon and
recurses to settle the best hit:

- `car` (a0, `$s3`) is the 0x808-byte car/MODELDAT record, the same `CCar` that `func_800C36A0` takes.
  Fields used here are `B628[4][3]` (corner positions), `C676[4][3]` (previous corners), `mat[9]` at 748
  (it reads `mat[3..5]`), `b1600`, `f6C4` (s16, compared with -1), `b1741`, and `pidx` (s16 at 1990, the
  player_array index).
- `poly` (a1) is the polygon record (`Poly`: type & 0xF, cnt & 0xF, body[0x12] for `func_800AD650`, `off`
  for `func_800AD5D0`). This is the same record that `func_800C3AD0` and `input_process_controller` take.
- `col` (a2, `$s2` for the whole function) is the best-hit record (`CCol`: poly, m[9], idx, edge, k).

The flow: decode the vertex indices, then decode vertex 0 (the 5.5-bit fixed point at 1/32, as in DECODE).
Build the polygon matrix and move the 4 corners into polygon space (`pts[0..3]`). Add `pts[4..7]` = corners +
up*scale, where scale is -3 for type 5/6 and otherwise -4.5 or -0.5 against a -0.707f dot product. Then, for
each of the 8 edges in the table D_801174CC[e]/[e+8], take an edge that crosses the plane, clip it against the
polygon outline (vertices converted lazily into `pv[]`, a 4x-unrolled loop), and take the smallest parameter.
Types 4, 7 and 3 are triggers: 4 calls the overlay `func_803914B4` in modes 4 and 6 and sets b1600; 7 calls
`func_800C54F0(idx, 1)`; 3 does the same and returns, playing SOUND(21,0,1,2) when D_80153E88[idx].kind == 6.
Any other type keeps the deepest hit, and then the result is merged into `col`. `func_800C36A0(car, col)`
applies a stored hit (IPA: car in `$s1`, col in `$s2`), and the function calls itself.

## State

`camera_play_script/body.c` is the draft. `best.c` is the same draft with the unit prelude, ready for
`blob_unit --with`. It compiles in the whole-program unit, and the SOUND wrapper func_800B61A8 is inlined there
as in retail:

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w9h --jobs 2 score camera_play_script --with cloud/work/frontier/w9h/camera_play_script/best.c
  FAIL camera_play_script: 844 of 880 words differ; compiled body is 884 words, target 880; ...
cps/u.sh camera_play_script/body.c   ->  want 880 words, got 884; differing rows 1103
```

The prologue, the vertex decode, the corner loop, the scale selection and the whole 4x-unrolled conversion loop
are structurally identical (same opcodes; only the registers differ). The 4 extra words and most of the
remaining rows come from register allocation:

1. **Callee-saved assignment.** Retail uses all of `$s0`-`$s8` for long-lived webs. In priority order those are
   `$s0` n, `$s1` &pv, `$s2` col, `$s3` car, `$s4` e, `$s5` edge-table IV, `$s6` 12, `$s7` &player_array and
   `$s8` 952. Retail also keeps the constant -1 in `$t5` and spills ka/kb (`$t3`/`$t4`) around calls at 272/268.
   In ours (traced with `cps/tr.sh`, proc 3) the col, -1, 952 and player_array webs are *split*, with
   save ~0.7-1.2. Without 952 in a register, `&player_array[idx]` becomes shifts instead of `multu s8`. With
   -1 not in a register, `car->f6C4 == -1` uses `li at,-1`.
2. **FP pressure.** Retail keeps dx/dz (240/236), da/db (252/248) and dy (148) in memory and stores the
   scale into dz's home (236) in the -4.5/-0.5 path. The scale is therefore the same C variable as dz; the
   draft does this. 1/32 is in `$f28`, and the edge parameter t is in `$f30`.
3. Frame: 632 against 608. The named-local layout is reconstructed from the retail homes: two scalars above
   `idx[16]` (568), one between idx and org (552), then d 540, pts[8][3] 444, pv[16][2] 316, mtx 280, conv 276,
   ka 272, kb 268, k 264, best 256, da 252, db 248, dx 240, dz 236, bestk 220, beste 216, hx 204, hz 200 and
   old 188. The uopt spill temps are at 152/148. Ours has the same named layout. The extra 24 bytes are extra
   uopt spill temps from lane 1.

Tried: loop forms (for and do-while), u32 n/k (needed: retail has `sltiu`/`sltu`), the fabsf intrinsic (needed),
and pointer or index forms for the corner transform. Nothing moved lane 1.

Next hypothesis: the split webs have tiny savings (tot ~20 over nocs ~18), so retail must give them more weight.
More references at loop depth, or shorter ranges, are likely to do that: for example `p0`/`p1` kept as pointers
across the whole clip, `pc = &player_array[car->pidx]` computed once before the type dispatch, or the type
dispatch written as one `switch`. Use `cps/tr.sh BODY LABEL 3` to trace and `../force.sh`-style CDX_FORCE to
test a target colouring before searching.

## Partners

`func_800C36A0` is locked (src/blob/groups/func_800AD4C8, with stand-in callers). Its real callers are this
function and `camera_victory` (unmatched, 1,468 B, L2). The draft calls it as `func_800C36A0(car, col)`, and in
both the unit and the cps group (`cps/g_c`) func_800C36A0 still scores MATCH next to it. `func_800AD650` is
internal with IPA-swapped parameter registers, so the call is written `func_800AD650(poly->body, mtx)` (in, out).
Even an exact body here would need the unit (or a group with func_800C36A0 and its stand-ins) to prove, because
func_800C36A0's interface depends on its callers.
