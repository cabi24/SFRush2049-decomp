# camera_aspect_ratio

**3 of 6 members MATCH** (263 words), scored strictly with `-r4300_mul`
([../../R4300_MUL.md](../../R4300_MUL.md)):

```
python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/camera_aspect_ratio --as1=-r4300_mul
```

| function | role | words | result |
|---|---|---|---|
| `camera_aspect_ratio` | member | 50 | **MATCH** |
| `camera_fov_control` | member | 60 | **MATCH** |
| `camera_build_view_matrix` | member (was a closure gap) | 153 | **MATCH** |
| `camera_free_look` | member | 109 | 103 differ (emits 108) |
| `camera_look_at_point` | member | 199 | 182 differ (emits 191) |
| `camera_update` | member | 719 | 686 differ (emits 675) |
| `camera_process_input` | context (closure gap) | 255 | 251 differ (emits 254) |
| `camera_track_spline` | context (closure gap, not in the V2 table) | 215 | 206 differ (emits 206) |

Differences are word-position based: one extra or missing instruction
shifts everything after it, so "N differ" overstates how far the structure is.

## What was done

- **Closure gaps closed.** `camera_build_view_matrix` and
  `camera_process_input` (V2 table) plus `camera_track_spline` (the V2 table
  missed it: `camera_update` calls it with the camera in `$a3`) are now in the
  unit with hand-written bodies. `camera_process_input` is a root (no direct
  caller), so it is in `keep`; the other two are internal.
- **Bodies rewritten with typed structs** (`Camera`, `CamCtl`, `CamScene`,
  `CamKey`, `CamTbl` at the top of `group.c`) instead of `M2C_FIELD`.
- **Seed defects fixed.**
  - `camera_target_track`: the m2c prototype had `s32` where the ROM passes
    floats in `$a2/$a3`; it is `(void*, s32, f32, f32, f32, f32, s32, s32, s32, s32)`.
  - `fabsf`/`sqrtf` declared as intrinsics at the top of the file.
  - `D_8002EB94` is `f32` (frame time), `D_80117530` a `CamTbl[]`.
  - `camera_update` no longer passes unset `$a0/$a1` to `camera_track_spline`;
    its argument is the camera.
- **Why the three matches match** (reusable):
  - The `if (D_8010FFC0 == 0) -1; else if (id == -1) -1; else camera_target_track(...)`
    tail in `camera_aspect_ratio`/`camera_fov_control` is an **inlined helper**
    (`static __inline camera_track_entry`). Its argument order decides the
    delay-slot filling: `(cam, f2C, id, s28)` matches, the other five orders do not.
  - `camera_build_view_matrix` matched once `idx` was an `s32` (an `s16` adds
    `sll/sra`) and the scale product was written `scale * m`, not `m * scale`.
- Stack layout tricks that worked: unused arrays reserve frame space, and a
  later declaration gets the lower address. `camera_free_look` needed
  `f32 out[4]` declared before `f32 d[3]`, and the extra scalar home slots.

## Remaining blockers

- `camera_free_look`: right structure and frame (96), but IDO puts `ctl` in
  `$s2` and `sc` in `$s3` (ROM: `sc`=`$s2`, `ctl`=`$s3` with the first uses
  through `$v0`), and it lays the "plain" block after the interpolation block.
  `dur` is `f14` in the ROM and `f2` here.
- `camera_look_at_point`: needs the conversion order (`f14` truncated before
  `f10`) and `-1` compared as `beql a0,t0`; register naming differs after that.
- `camera_track_spline`: the IPA parameter lands in `$a2` here, `$a3` in the
  ROM. IDO picks the first caller-saved register the body does not use, so the
  body must leave `$a3` free and use `$a0/$a1/$a2` for `idx/ctl/key`.
- `camera_process_input`: structure matches to about word 60 and again from
  the second loop; the frame is 184 against 248 (about 16 more scalar home
  slots), and the ROM keeps a dead store of `D_8011750C` (`sw t6,200(sp)`)
  that IDO drops unless the local is `volatile` (which then moves `move s7,a0`).
- `camera_update` (719 words) is still the m2c seed plus fixes; a typed
  rewrite is the largest remaining item. Its members call convention is known
  now: `s0`/`s1`/`s3` are loaded with the camera right before each call.

## Relocations

Seed-level for `camera_update`; the rewritten functions use symbol names that
`score.py` resolves through `symbols.json`.
