# Wave 10 — agent w10d

Assignment: func_800E0050 (1,440 B), camera_look_at_point, camera_process_input (1,020 B), camera_update
(2,876 B; it would close the provisional `camera_track_spline`). All flags `-g0 -O3 -mips2 -G 0 -non_shared`.
Builder scratch: `watchman2:~/rush2049/scratch/frontier/w10d`. No strict match this wave. Every function moved,
and camera_update's structure is now close to retail (13 opcode-aligned rows, 52 in the first cam_slot_set draft).

| Function | Bytes | State | Flags | Scorer output (whole-program unit, `--neighbours`: 0 locked bodies differ) |
|---|---:|---|---|---|
| `func_800E0050` | 1,440 | near-miss, **12 words**; provisional (stand-in dead caller) | -O3 | `FAIL func_800E0050: 12 of 360 words differ` (`EQUAL func_800E0048: 2 words`) |
| `camera_process_input` | 1,020 | near-miss, 15 words, all stack offsets; pad arrays | -O3 | `FAIL camera_process_input: 15 of 255 words differ` (was 23) |
| `camera_update` | 2,876 | near-miss: 255 words, 13 opcode rows; register colouring and frame | -O3 | `FAIL camera_update: 255 of 719 words differ; compiled body is 718 words, target 719` (was 551) |
| `camera_look_at_point` | 796 | near-miss: 5 opcode rows (one missing degenerate branch) | -O3 | `FAIL camera_look_at_point: 160 of 199 words differ; compiled body is 198 words, target 199` |
| `camera_free_look` (not assigned; camera_update's blocker) | 436 | near-miss, 24 words, register lane only | -O3 | `FAIL camera_free_look: 24 of 109 words differ` (was 35) |
| `camera_track_spline` (provisional) | 860 | EQUAL with these real callers, but the callers do not match, so it **stays provisional** | -O3 | `EQUAL camera_track_spline: 215 words (internal, c_best.c)` |

Commands, run from the repo root on the Pi:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10d score func_800E0050 func_800E0048 \
    --with cloud/work/frontier/w10d/func_800E0050/best.c --internal func_800E0050 --neighbours
  FAIL func_800E0050: 12 of 360 words differ
  EQUAL func_800E0048: 2 words (kept, c_best.c)
  locked bodies that differ in this unit: 0

python3 -m tools.conveyor.pipeline.blob_unit --tag w10d score camera_update camera_free_look camera_look_at_point \
    camera_track_spline camera_process_input --with cloud/work/frontier/w10d/camera_update/best.c \
    --internal cam_slot_set --neighbours
  FAIL camera_update: 255 of 719 words differ; compiled body is 718 words, target 719
  FAIL camera_free_look: 24 of 109 words differ
  FAIL camera_look_at_point: 160 of 199 words differ; compiled body is 198 words, target 199
  EQUAL camera_track_spline: 215 words (internal, c_best.c)
  FAIL camera_process_input: 15 of 255 words differ
  locked bodies that differ in this unit: 0
```
`camera_update/best.c` is a single cluster file. It holds every camera-unit function (the locked
aspect_ratio, fov_control and build_view_matrix unchanged) plus the inlined setter `cam_slot_set`.
`camera_process_input/best.c` is the same body in a standalone file.

`camera_track_spline` does **not** close: it is EQUAL with any caller, but its real caller camera_update does
not match.

Integration notes: nothing is spliceable. If func_800E0050 closes, func_800E0048 would need to be the
stand-in dead caller defined in its file. That replaces the locked empty `func_800E0048.c` body; the words are
identical (`jr ra; nop`). It would also need `--internal func_800E0050`, which means a unit_overrides entry,
because no caller is visible.

---

## func_800E0050 — 12 words (provisional)

The semantics are in the header of `func_800E0050/best.c`: a per-car engine-loop sound update, the sibling of
the locked func_800D5524. The arcade ancestor is not identified.

**How the body is called.** Retail has no reference to func_800E0050 at all: no jal, no j, no address
constant (checked over the whole image). Yet the body is internal. It reads its parameter from the caller's
home slot `72(sp)` without storing it, and it uses s0–s8 and f20–f30 without saving them. I tested several
caller set-ups:
- an internal function with no callers is deleted (2 words);
- one dead call site gets inlined away;
- a kept caller whose two call sites are dead (`if (0) { f(m); f(m); }`) reproduces exactly the
  memory-parameter read and the unsaved registers.

If that caller is the caller-less stub **func_800E0048** placed directly before it, the stub compiles to
retail's `jr ra; nop` (EQUAL), provided the caller has no parameter of its own. With a parameter it emits
`sw a0,0(sp)`. This is a hypothesis: compiled-out calls fit, but the caller is a stand-in. Hence provisional.

Closers, in order of effect (rows 360 → 12):
1. The volume message is an inlined static: `osRecvMesg / entity_transform_calc(h, -2, -2, -2, v) /
   osJamMesg`. The locked `entity_hierarchy_update`, which has the same text and 0 jal callers in retail,
   is not inlined by our umerge (kept). A static copy is.
2. Random is the locked arcade `func_8008B2E4` (Random(max), declared only): retail's result is in f0.
3. `set = &D_80140420[slot]` is hoisted before the loop and indexed `set->rec[i]`. Retail strength-reduces
   it, so the fields sit at 4/8/12(s1). A `rec` pointer local adds a `+4` bias.
4. `ref = &D_801141B0` is a local assigned before the loop (retail keeps it in s6).
5. The pitch scale is written inline, `pitch *= 0.85f + ((load + 200.0f) / 900.0f) * 0.15f`. With a `k`
   local the add operands come out reversed.

Residual:
- **8 words**: the float conversions of `t->b0` and `t->b1` tie at save 15 over 2 blocks (traced uopt, proc
  80, webs w76/w85). Retail colours b1 first (b0 → f12, b1 → f2). `force.sh "p1:w76=c26,p1:w85=c25"` gives
  exactly retail's words for these rows. Nine source variants tried, none moved it: reversed compares,
  nested and goto ladders, `(f32)` casts, float locals, a single expression per segment.
- **4 words**: as1 order in the mode-2 block (`addiu a0` for &D_80142728 in the bne delay slot; the position
  of the `lwc1 f20,68(sp)` reload).
- Frame: `f32 unused[2]` between load and slot gives retail's 72 bytes. The real cause is unknown.

Own rodata: 890.0f, .15f, .85f, .05f, .8f at 0x80124378..0x80124388. All five words were checked against the
image, and the scorer reports no relocation problem.

## camera_update — 551 → 255 words (13 opcode rows)

Starting point: `cloud/work/ipa-groups/camera_aspect_ratio/group.c` (551). Each fix below was found with the
traced allocator (`tools/ctrace.sh`, proc 589; `force.sh` to confirm before changing source):
1. **Flag updates are `sc->flags &= ~A; sc->flags |= B;`** at all four sites. The ipa-group form
   `sc->flags = v; *(u32 *)&sc->flags |= B` made B an unsigned constant web, so it was not shared. With plain
   signed `&=`/`|=`, IDO keeps **both** stores (as retail does). The 0x100000 constant is then shared with the
   `fl & 0x100000` test and hoisted into a register (retail `lui a2,0x10`). Earlier notes said the stores fold.
   They do not in this context.
2. **Mode updates are written the same way**: `ctl->mode &= ~4; ctl->mode |= 8;` (and `lc->mode` likewise),
   with tests on the expression and no `m` copy. This removes retail-absent `move` instructions and the
   v0/v1/a0 rotation.
3. `lk->flags & 0x100` / `lk->flags &= ~0x100` as expressions, not via `v`.
4. **The key loop uses a separate `s32 idx` tracking `ctl->idx`.** It is kept in v0 across the loop, as in
   retail. The loop does not use the k local; it indexes `sc->keys[idx]` directly, so the address web gets v1.
5. `} else if (na + 1 == sc->count) {`, not `sc->count == na + 1`. With the second form, `idx + 1` became a
   2-block CSE web in v1 and stole v1 from the mode web (w194). Retail computes it twice.
6. `ctl->t += *pdt;` (operand order of the add).
7. The second frame-time read is `*(f32 *)(s32)&D_8002EB94`. A second `(u32)` launder is CSE'd with the
   first, and that shared address gets spilled. Retail rematerialises both addresses.
8. The tb-section counter is `v = node->s04 + 1; node->s04 = v; if (v >= cam->s5A) {...}`, an int separate
   from the s16 `na`.
9. Inlined setter `cam_slot_set(slot, v)` at the four flag sites (retail's v0/v1 pair). The fifth site
   (`(&D_801427C0)[na]`, s50 update) is a direct store. Inlining it there costs 8 frame bytes that retail
   does not have. Note that retail loads that value **before** the 0x8000 test, into a0. A nested inline
   `cam_slot_put(v, cam, sc)` reproduces the a0 value and its ordering, but the frame is then 16 bytes off.

Residual: register colouring (k@block_99 and nx declined v1/a2 when forced; tb a0 vs retail a2; the s16 na
v1 vs a1), the knock-on t6–t9 ring and FP temp rotation, plus three small issues:
- the loop-top FP load order;
- `lw s3,244(sp)` vs `move a0,zero` for the build_view_matrix calls (retail sets s3 first);
- 1 missing word.

Frame: `f32 padA[9]` (36 bytes between cam at 244 and moved at 207) is a placeholder. The bottom area matches
only with exactly four inlined setter calls.

## camera_free_look — 35 → 24 words

Two fixes:
- `dist = d[0]*d[0] + d[1]*d[1] + d[2]*d[2]; if (D_80123E84 < dist)` gives retail's operand order and the
  late constant load. An inlined `magsq()` gives the same code but costs 8 frame bytes.
- The `m` pointer local must go (use `cam->m[0]`): each named local costs a slot, and that slot brings the
  frame to 96.

Residual: the plain-call argument preload. Retail hoists `a1 = k->rot` above the 0x20 test and leaves
`move a0,s0` for the delay slot; ours does the opposite. As a result idx gets a1 instead of a0 and the
const 68 / keys / k shift, and t/dur are swapped (f14/f2). Restructured if/goto forms are worse (45).

## camera_look_at_point — 5 opcode rows

Three fixes:
- Testing `D_80117530[cam->tbl].s20` and `cam->handle` as expressions instead of the `kind`/`handle`
  variables gives retail's constant-first compares (`bnel s1,a2`).
- `* 0.5f + .5f` gives two separate 0.5 constant webs (retail materialises 0.5 twice).
- Without the `id` local, the s20 value is used directly.

Residual:
- one missing **degenerate `bgez v1` (delay `move a1,a0`)** before the two abs selects. Tried: `?:` order,
  inline ternaries, if-forms, empty `if (b < 0) {}`, `abs()` intrinsic; none produce it;
- r/f in f0/f2 swapped;
- part 2's t/id/-1 register choice (t a0 vs v1, -1 a1 vs a0).

## camera_process_input — 23 → 15 words

Applying w4d's two fixes to the ipa-group body takes it to 15 words: the node allocation outside the flags
block, and the `cam_slot_set` inline. Everything left is stack offsets in the mode-2 block, which sit 8 bytes
high. Pads are still needed (`padT[11]`, `padA[2]`, `padB[1]`). Neither inlining the stub `func_800C15FC`
(mode-2 block as a helper) nor any pad combination gets both the outer and the inner offsets right (24
variants).

## Tools (this directory)

- `tools/run.sh cand.c FN… [-- extra]`: unit score plus the aligned diff summary.
- `tools/udiff.py FN [--all] [--nosp] [--ops]`: aligned diff vs retail. `--nosp` ignores stack offsets;
  `--ops` compares opcodes only, a good structure metric for large bodies.
- `tools/vrun.py BASE.c FN SPEC.py [args]`: batch variant runner.
- `tools/fnrep.py IN OUT FN old new…`: replaces text inside one function only.
- `ctrace.sh`/`pdiff.sh`/`force.sh`/`tr.sh`/`sum.sh`: w3a's traced uopt retargeted to `w10d`; `ctrace.sh`
  honours `EXTRA="--internal X"`.

Permission denials: none.

## What generalises

1. **Read-modify-write of flag words is `x &= ~A; x |= B;`.** In this context IDO keeps both stores, and the
   signed constants share webs with the bit tests (hoisted constant registers). `*(u32 *)&x |= B` workarounds
   give unsigned constant webs that do not share. Halfword mode fields follow the same rule.
2. **A full-body internal function with no references is a compiled-out callee.** Two or more dead call sites
   in a kept caller reproduce the unsaved callee-saved registers and the parameter read from the caller's
   home slot. A single dead site lets umerge inline it away. A caller-less stub next to it compiles to
   `jr ra; nop` when it is that caller and has no parameter.
3. **Comparison operand order changes CSE:** `na + 1 == count` vs `count == na + 1` decided whether
   `na + 1` became a coloured 2-block web (camera_update).
4. **Two launders of the same global with different integer casts** (`(u32)` vs `(s32)`) are not CSE'd. Use
   this when retail rematerialises an address that our build hoists and spills.
5. **Expression instead of variable gives constant-first compares** (`bnel s1,a2`, `beql a0,t0`). When
   retail's compare has the constant register first, test the field expression rather than a local copy.
6. When the traced allocator declines a forced colour, try forcing each earlier web of that colour to
   another register (`CDX_FORCE=pN:wM=c9`). The one that releases the colour is the interfering web. This
   identified w194 (`idx + 1`) and w160 (the loop index).
7. `--ops` (opcode-aligned) is the right progress metric for big functions: camera_update went from 52 to 13
   rows while strict words moved 551 → 255.
