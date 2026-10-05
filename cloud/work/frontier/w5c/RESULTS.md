# Frontier wave 5 — agent w5c results

Builder scratch `~/rush2049/scratch/frontier/w5c` (copied from `base` 2026-10-05; `uopt`/`as1` are symlinks to the
w4a traced builds). Unit tag `w5c`. Nothing committed or spliced. All flags `-g0 -O3 -mips2 -G 0 -non_shared`.
Helper scripts: `tools/` (w4a tools retagged; new `u.sh`/`ub.sh` = unit score + aligned diff with extra args,
`ctracex.sh` = ctrace with `XARGS` such as `--block`, `rd.py ADDR N` = retail words/floats from `build/game_code.bin`).

| Function | Bytes | Deps | State | Deliverable |
|---|---:|---:|---|---|
| `func_800B5898` | 168 | 14 | **strict MATCH**, own .rodata verified, unit EQUAL | `cloud/matches/func_800B5898.c` |
| `func_800B5940` (bonus) | 168 | 10 | **strict MATCH**, own .rodata verified, unit EQUAL | `cloud/matches/func_800B5940.c` |
| `func_80090F44` (bonus) | 168 | 18 | **strict MATCH**, own .rodata verified, unit EQUAL | `cloud/matches/func_80090F44.c` |
| `func_8009EA68` (bonus) | 168 | 18 | **strict MATCH**, own .rodata verified, unit EQUAL | `cloud/matches/func_8009EA68.c` |
| `func_80090E9C` (bonus) | 168 | 33 | **strict MATCH**, own .rodata verified, unit EQUAL | `cloud/matches/func_80090E9C.c` |
| `gfx_setup_fc` (bonus) | 168 | 0 | **strict MATCH**, own .rodata verified, unit EQUAL | `cloud/matches/gfx_setup_fc.c` |
| `func_800E92C8` | 788 | 10 | **strict MATCH (group)**, own .rodata verified, unit EQUAL (internal) | `groups/func_800E92C8/` |
| `func_800EA108` | 468 | 7 | **strict MATCH (group)**, own .rodata verified, unit EQUAL | `groups/func_800E92C8/` |
| `func_800EA2DC` | 280 | — | re-proven strict MATCH in the new group (already locked) | `groups/func_800E92C8/` |
| `func_800E95DC` | 1,684 | 8 | **2 instructions off** (one as1 delay-slot fill), context in the group | `func_800E95DC/` |
| `func_800E9E2C` | 732 | 7 | **strict MATCH**, own .rodata verified, unit EQUAL | `cloud/matches/func_800E9E2C.c` |
| `music_tempo_set` | 980 | 5 | 130 aligned rows off (register colouring of one param piece) | `music_tempo_set/best.c` |
| `entity_anim_texture` | 636 | 0 | 94 aligned rows off | `entity_anim_texture/best.c` |
| `anim_state_update` | 1,840 | 0 | 258 aligned rows off | `anim_state_update/best.c` |

Spliceable now: 7 singles (1,740 bytes) + group members `func_800E92C8` and `func_800EA108` (1,256 bytes) =
**2,996 bytes, 9 new functions**. The five bonus functions are the rest of the `func_800B5898` family (same 42-word
shape, all six callers of `sinf`+`cosf` with that size); none was in any w5 directory.

The 8 dependants of `func_800E95DC` and the 1 KB `drone_throttle_calc` (sole blocker was `func_800B5898`) are
now unblocked or one step closer.

---

## func_800B5898 family — strict MATCH (6 functions)

Arcade `LIB/fmath.c` unit-vector rotations, pasted: `WPitchUV` (`func_800B5898`), `WRollUV` (`func_800B5940`),
`PitchUV` (`func_80090F44`), `RollUV` (`func_8009EA68`), and two yaw variants whose N64 sign is the reverse of the
arcade (`ut = a*cos + b*sin; b = b*cos - a*sin`): `func_80090E9C` (columns, YawUV) and `gfx_setup_fc` (rows,
WYawUV; the historical label is wrong). Epsilon is `0.001f` for the W* row forms and `0.0001f` for
`func_80090F44`/`func_8009EA68`/`func_80090E9C`; float literals (retail compares with `c.lt.s`).
Only quirk: the operand order of the sum, written as in the arcade (`uv[i][2]*cost + uv[i][1]*sint`), selects the
`mul.s` order (the reversed spelling was 2 words off).

```
cloud/work/frontier/w5c/tools/sc.sh cloud/matches/func_800B5898.c func_800B5898 --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
func_800B5898:
  MATCH
    own .rodata verified at 0x80123DB4..0x80123DBC
func_800B5940:   MATCH     own .rodata verified at 0x80123DBC..0x80123DC4
func_80090F44:   MATCH     own .rodata verified at 0x801239D8..0x801239E0
func_8009EA68:   MATCH     own .rodata verified at 0x80123B04..0x80123B0C
func_80090E9C:   MATCH     own .rodata verified at 0x801239D0..0x801239D8
gfx_setup_fc:    MATCH     own .rodata verified at 0x80123B0C..0x80123B14
```
(same command per function). `blob_unit --tag w5c score NAME --with cloud/matches/NAME.c --neighbours`: EQUAL,
"locked bodies that differ in this unit: 0" for all six.

## func_800E92C8 unit — group: E92C8 + EA108 strict, E95DC 2 instructions

Group dir `groups/func_800E92C8/` (`group.json` with `claims`, `group.c`). It **supersedes**
`src/blob/groups/func_800E92C8` (the cloud Lanes A seed: members `func_800EA2DC`, `__standin_func_800E92C8`
kept): `blob_group revert func_800E92C8`, install this one, then the checks. The stand-in is gone; the real
callers E95DC/EA108/EA2DC are all present. `func_800E92C8` is internal (defined, not kept).

```
cloud/work/frontier/w5c/tools/grp.sh cloud/work/frontier/w5c/groups/func_800E92C8      # score.py group
Members:
func_800E92C8:
  MATCH
    own .rodata verified at 0x801244C8..0x801244D0
func_800EA108:
  MATCH
    own .rodata verified at 0x801244E8..0x801244EC
func_800EA2DC:
  MATCH
Context: func_800E95DC  (2 instructions; positional count 112/421 because of the one-word shift)

python3 -m tools.conveyor.pipeline.blob_unit --tag w5c score func_800E92C8 func_800EA108 func_800EA2DC \
    --with cloud/work/frontier/w5c/groups/func_800E92C8/group.c --neighbours
  EQUAL func_800E92C8: 197 words (internal, c_group.c)
  EQUAL func_800EA108: 117 words (kept, c_group.c)
  EQUAL func_800EA2DC: 70 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
```

Semantics (full header in `group.c`):
- `func_800E92C8(Cr *car, f32 ab, f32 AB, f32 *camoff)` = arcade `game/camera.c` `update_rear_camera(ab, AB,
  camoff)` with the car record added first. The arcade body pasted almost verbatim matched; N64 changes:
  `fixed_cam` is `car->mode == 8`, the death-cam view is mode 4 (`vec = {V[1], V[0], 0}`), the abrupt-movement
  timer is gone, the `fabs` lands on `vec[2]` and `res[1]` (Y-up), elastic factor per slot (`D_80152720`).
- `func_800EA108` = arcade `DeathCam` (`update_rear_camera(car, 30, 16, res)`, elasticity `.13f` in
  `D_80152708[slot]`). Arcade loop expressions verbatim; matched first compile.
- `func_800E95DC(s16 mode, Cr *car, f32 *pos, s32 uvs)` = arcade `steady_move_cam` rebuilt for per-player legs:
  `D_8012E690[pl]` cam_pos, `D_8012E6C8[pl]` leg, `D_8012E6E8[pl]` leg time, `D_801108C8[leg]` leg durations
  (1.4, 1.0), `V3 delta[4]` a *stack* array indexed by player (retail sp+184+pl*12), leg 0 aims at
  `RWR - uvs[2]*40, +5` (first overwritten `delta` set is kept by retail), leg 2 calls `func_800E9234` (init_view3)
  and re-picks the view.

Quirks that closed things (all in source):
- **Call-site argument order is the callee's parameter order** — `(car, ab, AB, camoff)`, not the arcade's
  `(…, camoff)` last: retail sets up `s0, f22, f24, s4` in that order in every caller. This removed the last
  E95DC case-body residual and EA2DC still matches with it.
- E92C8: `0.0f` for the vec/elastic zeros but int `0` for `magvel = 0`; int `1` in `magvel > 1` and the
  interpolate lower bound, `1.0f` in `1.0f - x` (two different 1.0 webs, f30 vs a fresh `lui`); natural
  `vx*vx + vz*vz` (with `dr_vel[]` array access the operand order flips — see "What generalises");
  a dead `char buf[8]` (arcade has `char buf[60]`) for the 160-byte frame (sizes 1..8 all work).
- E95DC: `(diff) * (t / dur)` with the scale written inline (CSE'd) gives retail's `mul.s k, diff` order;
  direct array `D_8012E6C8[pl]` (a `s16 *count` pointer forces reloads after the delta stores);
  `res[i] += pos[i]` (or `pos + res`) instead of `res[i] = res[i] + pos[i]` keeps `res[0]` forwarded in a
  register (135 → 18 rows); `if (…) v = func_800CDE38(…); else v = 2;` with `s32 v` (retail has the
  then-arm `b` to the join and no byte mask); int `0` zeros in the tail and in case 2 (one shared `f0` zero
  materialised at the end of each case).

**E95DC residual (2 instructions, lane: as1 delay-slot fill).** Dispatch `beq a0,1 → case 1`: retail fills the
slot with case 1's `move s0,t3` (taken from the target, branch retargeted); ours fills it with the fall-through
`li at,2`. ugen listings of both cases are identical and the per-block as1 schedule is identical (traced as1,
`ulist.sh`); case 0's equivalent hoist matches. Tried without movement: `default:` first/last, case order,
`(s32)` switch value, a `leg` local (s16/s32), if-chain (much worse), call on the `case` line. Best next
hypothesis: something gives the `li at,2; beql` block a second predecessor (a label) in retail so as1 cannot
take it, e.g. a different switch spelling; or find as1's slot-filler rule for `beq` with a single-predecessor
target. Best source: `func_800E95DC/best_full_group.c` (whole group) / `best_body.c`; sweep in
`func_800E95DC/sweep/` (`es.sh BODY LABEL` rebuilds the group with a new E95DC body).

## func_800E9E2C — strict MATCH

Arcade `circle_camera_around_car`: offset `{cos(ang)*15, 10, sin(ang)*15}`, one turn per 6 s, elastic factor
cleared, then `func_800E8D50` (N64 UpdateCarObj). N64 adds a per-car state byte `D_80110680[car]`: state 1
latches the clock into `D_80110668[car]` after 10 s and becomes state 2, which freezes the angle. `IRQTIME` is
`(u32)(D_801543CC * 1000.0f)` (the float→unsigned sequences); angle `(f32)(IRQTIME % 6000) * (1.0f/6000) *
6.2831855f` (two literal pairs = two expressions). Matched on the first compile.
```
cloud/work/frontier/w5c/tools/sc.sh cloud/matches/func_800E9E2C.c func_800E9E2C --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
func_800E9E2C:
  MATCH
    own .rodata verified at 0x801244D8..0x801244E8
```

## music_tempo_set — 130 aligned rows (from 242 for the B99 source)

`music_tempo_set(s16 player, u8 mode, int apply)`: under the `D_80034840` thread stop/start (historical labels
`osPfsChecker_full`/`osStartThread`) mask the hulk/visibility bits; colour the car's root/body/wheel resources;
pick the wheel object; show root, hide body/extras/wheels. Found:
- **`func_80092B80` must be blocked** in the unit (`--block func_80092B80`): otherwise umerge inlines it and the
  body is 12 words different. Landing needs a unit blocker entry for it.
- Both wheel loops index `D_80139320[player].wheels[i]` directly (retail computes a second slot pointer in s1),
  the rest goes through `slot`.
- Frame 88 needs 16 bytes of unused locals (`char buf[16]`; a compiled-out debug buffer is the likely source).
- Residual is register colouring: retail colours the middle live-range piece of `player` (after the
  `func_80092B80` call) **t2**; ours colours it a3. Forcing it (`force.sh mtb music_tempo_set 407 "p1:w162=c9"`)
  makes ugen's temp pool match (retail never uses t0–t2 as temps because uopt coloured t2): the top 90 rows align.
  Natural lever not found (tried `root`/`body` temporaries, `char buf[12/16/20]`).
Command: `cloud/work/frontier/w5c/tools/ub.sh music_tempo_set/best.c music_tempo_set "--block func_80092B80"` →
`FAIL music_tempo_set: … 246 words` / 130 aligned rows.

## entity_anim_texture — 94 aligned rows; anim_state_update — 258 aligned rows

Both live in `codex_visual_tire_module_20261002` (internal `model_bounds_calc`/`matrix_scale_apply` with register
parameters; scored in the unit, where the locked group supplies them). Recovered from retail:
- entity: `slot = record->player` is read before the `mode == 0` test (delay slot); `hulk = (flags & 0x10) != 0`
  is computed before `lucent`; `model_bounds_calc(!hulk && !player->disabled, record)`. Residual: uopt colours the
  slot web a2 (retail v1) and the side offset; frame 80 vs 72.
- anim_state_update (arcade `AnimateTire`): `lucent` uses `player_array[slot].appearance` before `car` is assigned
  (the address is recomputed); `controller` is `s32`; `D_8014A250[slot].visual_code[tire]` (not `m->`); `index` is
  `s32`; the tire angular velocity is re-read in the `tire == 0` block (no `vel` local); the
  `scale != 1.0f → func_8008B32C` tail is duplicated in both `tire >= 2` arms; rand is inlined. Residual: broad
  register colouring and a 16-byte frame gap (retail 224).
Neither has dependants; stopped for breadth.

## What generalises

1. **Commutative operand order is decided by expression shape, not source order.** `x * y` with one side a
   named variable and the other an expression emits `expr, var` whichever way it is written; two expressions
   come out reversed (`(diff) * (k-expr)` → `mul.s k, diff`), and a CSE'd inline `k` keeps that order. Array
   element vs struct field also flips it (`dr_vel[0]*dr_vel[0] + dr_vel[2]*dr_vel[2]` needed the reverse spelling;
   with `vx`/`vz` fields the natural spelling matched). Probe with the `o3s_remote.sh` listing on a 5-line test
   file before sweeping the real function.
2. **`a += b` and `a = a + b` are different ucode for array elements**: `res[i] += pos[i]` let uopt keep the
   stored value in a register for the next statement (E95DC 135 → 18 rows).
3. **Callee parameter order shows in every caller's argument set-up order** — confirmed again for an IPA
   register-parameter callee (s0/f22/f24/s4): reorder the parameters, not the call sites.
4. **ugen's temp pool excludes every register uopt coloured anywhere in the procedure.** If retail's temps start
   at t3 (t0–t2 never used as temps), a uopt web somewhere was coloured t2; `force.sh` confirms it in one run.
5. **A callee that umerge inlines in the unit but retail calls by `jal` needs `--block`** when scoring
   (music_tempo_set/func_80092B80) — check for a missing `jal` first when a body is shorter than retail.
6. The 42-word `sinf`+`cosf` shape is a family: grep `score.targets()` for the same call pair and size to find
   siblings after the first match (5 bonus matches here).
