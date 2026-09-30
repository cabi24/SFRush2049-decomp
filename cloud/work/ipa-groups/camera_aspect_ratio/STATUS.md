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
| `camera_free_look` | member | 109 | 35 differ (emits 109; was 103 differ, emits 108) |
| `camera_look_at_point` | member | 199 | 163 differ (emits 196; was 182 differ, emits 191) |
| `camera_update` | member, in `keep` | 719 | 551 differ (emits 719; was 667 differ, emits 720) |
| `camera_process_input` | context, in `keep` | 255 | 23 differ (emits 255 + 3 pad; was 251 differ, emits 253) |
| `camera_track_spline` | context | 215 | 182 differ (emits 212; was 206 differ, emits 206) |

Rescored 2026-09-30 (Round 3, pass 4) with `zbuild.py --as1=-r4300_mul`; context
functions with `score.compare` on the same object (zbuild only lists members).

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
- Stack layout: the first declaration gets the highest address; unused `f32`
  arrays reserve frame space (an unused `s32` array or scalar does not).
  Spill/home slots of named scalars sit below the named locals of the same
  scope, in declaration order, and inner-scope arrays sit below those.
- **Pass 4 (register and web shape):**
  - Writing `sc = cam->ctl->scene;` (the whole expression, with `ctl = cam->ctl;`
    kept as a second statement) instead of `sc = ctl->scene;` makes the ROM's
    `lw v0,108(a0)` / `move a3,v0` / `lw a2,0(v0)` shape: the load goes to a
    scratch register and the named variable is a copy. This fixed the head of
    `camera_process_input` and `camera_free_look` and shifted `camera_track_spline`
    into the ROM's shape.
  - Drop a named pointer local when the ROM recomputes the address: in the
    `camera_process_input` key loop, `if (s->keys[i].flags & ..) { k = &s->keys[i]; ..}`
    gives the ROM's two `addu` (the flag test and the body use separate
    address temps); a single `k = &s->keys[i]` before the test does not.
  - Keep an IDO-dropped dead store with an array, not `volatile`: `s32 unused[1];
    unused[0] = D_8011750C;` keeps the store and leaves `move s7,a0` where the
    ROM has it (a `volatile` local moved it to the prologue).
  - `camera_track_spline`: `for (p = v; p < &v[3]; p++)` (pointer compare,
    `sltu`) gives the ROM's rotated two-copy loop; `next = idx + 1` belongs in
    the `else` block only (ROM fills the delay slot with it); the `b < a` test
    with two identical arms (`d = a * t + c` in both) reproduces the ROM's
    redundant `c.lt.s`; `v[3]` and `w[3]` must be the first locals so no scalar
    homes sit above them (frame 128, `v` at 116).
  - `camera_free_look`: `dur = k->dur` after the `t` block gives ROM's
    `mov.s $f14,$f0`; the goto target `plain:` goes inside the `dur == 0` arm
    (`if (k->flags & 0x20) goto plain; ... if (dur == 0.0f) { plain: ... } else {...}`);
    an `idx` local (`idx = ctl->idx; k = &sc->keys[idx]; next = idx + 1`) fixes
    the `a0/t0/t1` assignment; sum of squares is `d[0]*d[0] + d[1]*d[1] + d[2]*d[2]`.
  - `camera_look_at_point`: convert `ctl->f14` before `ctl->f10`; `x = a < 0 ? -a : a`,
    `y = b < 0 ? -b : b` as new variables; `if (b == 0) b = a != 0 ? a : 1;`.
  - `camera_update`: the flag-word local `fl` is not a variable in the ROM
    (all tests read `sc->flags`, which uopt keeps in one register until a call);
    the `done` flag and the loop counter `i` are the same variable (both land
    in `$s7`, which gave `sc` = `$s8`); the second read of `D_8002EB94` must be
    the plain global while the first goes through `(f32 *)(u32)&D_8002EB94`,
    otherwise the address is hoisted and spilled (+2 words, wrong offsets).
    Frame 248 needs `f32 padC[2]` after `padB[6]`.
- Declaration order does not change register assignment in these functions
  (720 orderings of `camera_track_spline`'s webs and 250 of `camera_free_look`'s
  gave identical code); it only moves stack slots. Register order follows web
  priorities, which change with statement shape (see above), not with names.

## Remaining blockers

- `camera_update` (551 differ, 719 words, frame and layout right; register-blind
  the structure is 683/719): `sc->flags` lands in `$v1` (`m` in `$v0`) where
  the ROM has `$a0` (`m` in `$v1`), which rotates every later `t6..t9` scratch
  register. `m`/`v`/`na` types, declaration order, `sc = cam->ctl->scene`
  variants, expression-versus-local for `m`, and three write forms of the
  `sc->flags = v; sc->flags |= C` pair were all tried (`*(u32 *)&sc->flags = v`
  drops the first store; `|=` and `= v | C` fold both stores). ROM schedules
  the first `sw` before the `or`; we emit `or` first (4 sites, about 10 words).
- `camera_free_look` (35 differ): `t` is `$f14` and `dur` `$f2` where the ROM
  has `t` `$f2`, `dur` `$f14`; declaring `dur = k->dur` before the `t` block
  swaps them but loses the `mov.s`, and everything below shifts one word.
  The final distance test loads `D_80123E84` late in the ROM (`c.lt.s $f8,$f0`),
  we load it first.
- `camera_look_at_point` (196 of 199 words, 163 differ): ROM has a degenerate
  `bgez v1,+8` (delay `move a1,a0`) before the two `abs` blocks, we do not; `r`
  is `$f2`/`f` `$f0` in the ROM, the reverse here (tests on `f` instead of `r`
  and clamp forms do not change it); constant `-1` sits in `$a0` (ours `$a1`),
  `kind` in `$a2` (ours `$a1`).
- `camera_track_spline` (182 differ, structure 195/215): IPA puts the parameter
  in `$a2` here and `$a3` in the ROM. The ROM's `idx`/`ctl`/`k` are `$a0/$a1/$a2`;
  ours are `$a3/$a0/$a1` (the parameter web is fixed before the body, and
  the body then skips `$a2`). The ROM computes `k = &keys[idx]` (`multu`) before the
  first branch, we sink it below the early-return test. `k->dur * 2.0f` is a real
  `mul.s` by a hoisted `2.0` in the ROM; IDO always emits `add.s` here (no source
  form found; `/ dur / 2.0f` reaches the ROM size but is a different computation).
  Adding a 3-argument call, a 3-parameter prototype for `func_800C0294`, or an extra
  live local does not move the parameter.
- `camera_process_input` (23 differ, the closest): remaining differences are the
  `$v0/$v1` versus `$t6..$t9` choice for the `cam->slot`/`D_80142A7A` pair
  (ROM `lh v0,14(s7); lhu v1,..`; ours `t6/t9`) and the `t`-register rotation after it.
