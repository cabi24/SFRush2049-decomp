# camera_aspect_ratio

**3 of 6 members MATCH** (263 of 1,290 member words), rescored 2026-09-30 on master d0891f3:

```
python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/camera_aspect_ratio --as1=-r4300_mul
```

| function | role | target words | result |
|---|---|---|---|
| `camera_aspect_ratio` | member | 50 | **MATCH** |
| `camera_fov_control` | member | 60 | **MATCH** |
| `camera_build_view_matrix` | member (former closure gap) | 153 | **MATCH** |
| `camera_free_look` | member | 109 | 103 differ (emits 108) |
| `camera_look_at_point` | member | 199 | 182 differ (emits 191) |
| `camera_update` | member, in `keep` | 719 | 667 differ (emits 720, 1 extra) |
| `camera_process_input` | context, in `keep` | 255 | 251 differ (emits 253) |
| `camera_track_spline` | context | 215 | 206 differ (emits 206) |

Differences are word-position based: one extra or missing instruction shifts
everything after it, so "N differ" overstates how far the structure is.
Not spliced (not in `src/blob/groups`).

## Closure

No gap left. `camera_build_view_matrix`, `camera_process_input` and
`camera_track_spline` (missed by the V2 table: `camera_update` calls it with
the camera in `$a3`) are in the unit with hand-written bodies.
`camera_process_input` has no direct caller, so it is in `keep`.

## Techniques that worked

- Typed structs (`Camera`, `CamCtl`, `CamScene`, `CamKey`, `CamTbl`) instead of
  `M2C_FIELD`. `fabsf`/`sqrtf` declared as intrinsics.
- `camera_target_track` prototype is `(void*, s32, f32, f32, f32, f32, s32, s32, s32, s32)`:
  the ROM passes floats in `$a2/$a3`. `D_8002EB94` is `f32`, `D_80117530` a `CamTbl[]`.
- The `if (D_8010FFC0 == 0) -1; else if (id == -1) -1; else camera_target_track(...)`
  tail of `camera_aspect_ratio`/`camera_fov_control` is an inlined
  `static __inline camera_track_entry`. Its argument order decides delay-slot
  filling: `(cam, f2C, id, s28)` matches, the other five orders do not.
- `camera_build_view_matrix`: `idx` is `s32` (an `s16` adds `sll/sra`); the
  scale product is `scale * m`, not `m * scale`.
- Stack layout: unused arrays reserve frame space, and a later declaration gets
  the lower address. `camera_free_look` needs `f32 out[4]` before `f32 d[3]`
  plus extra scalar home slots.

## Remaining blockers

- `camera_free_look`: structure and frame (96) right; IDO puts `ctl` in `$s2`
  and `sc` in `$s3` (ROM: `sc`=`$s2`, `ctl`=`$s3`, first uses via `$v0`), lays the
  "plain" block after the interpolation block, and `dur` is `f2` (ROM `f14`).
- `camera_look_at_point`: conversion order (`f14` truncated before `f10`) and
  `-1` compared as `beql a0,t0`; register naming differs after that.
- `camera_track_spline`: the IPA parameter lands in `$a2`, ROM `$a3`. IDO takes
  the first caller-saved register the body does not use, so the body must leave
  `$a3` free and use `$a0/$a1/$a2` for `idx/ctl/key`.
- `camera_process_input`: matches to about word 60 and again from the second
  loop; frame 184 vs 248 (about 16 more scalar home slots). The ROM keeps a
  dead store of `D_8011750C` (`sw t6,200(sp)`) that IDO drops unless the local
  is `volatile`, which then moves `move s7,a0`.
- `camera_update` (719 words) is still the m2c seed plus fixes; a typed rewrite
  (frame right, about 20 hunks) is the largest item. Call convention known:
  `s0`/`s1`/`s3` are loaded with the camera right before each call.
