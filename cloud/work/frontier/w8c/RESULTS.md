# w8c results (frontier wave 8, agent w8c, 2026-10-05)

Builder scratch `watchman2:~/rush2049/scratch/frontier/w8c`; unit runs `--tag w8c`. Nothing committed, spliced
or written outside `cloud/matches/` and this directory.

Strict command used for singles (via `tools/sc.sh FILE NAME --flags …`):
```
ssh watchman2 'cd ~/rush2049/scratch/frontier/w8c && IDO_DIR=…/ido python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"'
```
Unit command: `python3 -m tools.conveyor.pipeline.blob_unit --tag w8c score NAME… --with FILE… [--internal …] --neighbours`.

## Table

| Function | Bytes | State | Flags | Where |
|---|---:|---|---|---|
| car_lights_render | 284 | **MATCH** (also -O2); unit EQUAL | -O3 | `cloud/matches/car_lights_render.c` |
| func_800D2054 | 212 | **MATCH**; unit EQUAL | -O3 | `cloud/matches/func_800D2054.c` |
| car_stats_display | 308 | **MATCH** (-O3 only); unit EQUAL | -O3 | `cloud/matches/car_stats_display.c` |
| wheel_params_set | 232 | **group MATCH** (`score.py group … --claims` exit 0); unit EQUAL | -O3 group | `groups/heap_wheel/` |
| physics_collision_test | 240 | **MATCH** (also -O2); unit EQUAL | -O3 | `cloud/matches/physics_collision_test.c` |
| func_800B23E0 | 268 | **MATCH**; unit EQUAL | -O3 | `cloud/matches/func_800B23E0.c` |
| func_800E7D0C | 196 | 5/49 words off (group context) | -O3 group | `func_800E7D0C/best.c` (body) |
| func_800EC190 | 224 | 8 aligned rows / 18 positional words off | -O3 (same at -O2) | `func_800EC190/best.c` |
| *sibling* func_800C885C | 188 | **group MATCH**; unit EQUAL | -O3 group | `groups/heap_wheel/func_800C885C.c` |
| *sibling* func_800B0580 | 152 | **MATCH**; unit EQUAL | -O3 | `cloud/matches/func_800B0580.c` |
| *sibling* func_8008B2E4 | 72 | **MATCH**; unit EQUAL | -O3 | `cloud/matches/func_8008B2E4.c` |
| *sibling* func_800E7A98 | 172 | 25 rows off in unit | -O3 unit | `func_800E7A98/best.c` |

New strict coverage claimed: 6 assigned + 3 siblings = 9 functions, 1,956 bytes (wheel_params_set and func_800C885C
through the group).

Combined unit check of all nine new bodies plus the three stub helpers (`--with` all files, `--internal
func_80095CF4 func_80095CFC func_800960CC --neighbours`): **12/12 EQUAL**, but one locked body then fails:
`func_800F0674: .bss+0x1fe8 at 0x80156940 and .bss+0x2068 at 0x80156948 overlap in the image` (its own local
statics). It fails only with all three of func_800D2054, car_stats_display and func_800C885C present (dropping any
one of them clears it; none defines data). This is a unit `.bss` layout/size-inference artefact, not code; the
integrator should run `blob_unit check` after landing and look at func_800F0674's `.bss` object sizing if it
recurs.

## Per function

### car_lights_render — MATCH
Screen-to-world unprojection: `(sx-cx)*depth/(scale_x*proj_x)`, `(cy-sy)*depth/(scale_y*proj_y)`, `depth`,
negated by the two mirror flags (D_80140A04 x; D_80151AD8 x and y), rotated with func_8009E820 and offset by the
camera position. 72-byte viewport rows `D_8017A510[]` (scale_x +28, scale_y +32, proj_x +36, proj_y +40,
center_x +44, center_y +48). Camera = `f32 mat[9]; f32 pos[3]`.
- Closer: `v = D_8017A510; v += view;` — the typed `&D_8017A510[view]` gives `.noalias v,$sp` and lets as1 hoist the
  y-row loads above the `in[0]` stack store (6 rows off). `world` declared before `in`; sums written
  `world[i] + cam->pos[i]`.
```
car_lights_render:
  MATCH
```
(-O2 also MATCH). Unit: `EQUAL car_lights_render: 71 words`.

### func_800D2054 — MATCH
Lap split recorder: per-player u8 lap counter `D_80144018[]`, eight-float rows `D_80149A78[][8]`, best lap
`D_80144DA8[]`, guards `D_801174B4 & 8`, mode `D_8014A110 != 1`, `player < D_8014A108` (s16).
- Closer: spell every access `D_80149A78[player][D_80144018[player]]` (no local count, no slot pointer). Any local
  copy makes uopt unroll the subtraction loop by 4 (76 words); with the global spelling the stores into
  D_80149A78 provably miss D_80144018 and the bound is hoisted only after the unroll decision (retail's
  `move t0,a1` copy + rolled loop).
```
func_800D2054:
  MATCH
```

### car_stats_display — MATCH
Timed-event scheduler (variadic): `u32 f(int type, f32 delay, int func, int data, int nargs, ...)` under the
D_80142728 lock: node from the free pool (func_800D18D8), start = elapsed clock, up to 4 int varargs (rest -1),
link -1, insert at head of D_80149860 (func_80091FBC), active=1, return handle.
- va_arg must be the align-then-advance form `((T*)(ap = (char*)(((int)ap + 3) & -4) + sizeof(T)))[-1]`
  (the IDO `+2*4-1` form gives `lw -4(a1)`).
- The start time is the kept getter **func_80095CE8 = `return D_80152748;`, which has no jal callers anywhere in
  retail** (umerge inlines it at every site). Unit EQUAL with `e->start = func_80095CE8();`
  (`car_stats_display/unit_call.c`); standalone a static getter (f0 return web) is used.
- Declaration order `e, ap, i, handle` (24-permutation sweep, 2 of 24 match).

### wheel_params_set (+ func_800C885C) — group MATCH
`groups/heap_wheel/` = locked `codex_heap_release_a25` (all six files unchanged) + `wheel_params_set.c` +
`func_800C885C.c`; claims `wheel_params_set`, `func_800C885C`. `score.py group cand/groups/heap_wheel --claims`:
every member and context function MATCH, exit 0 (`wheel_params_set: MATCH`, `func_800C885C: MATCH`,
`func_80095CF4: MATCH`, `func_80095CFC: MATCH`, `func_800960CC: MATCH`).
- Classification: audio_reverb_update is internal (IPA a1/a2 parameters, leaves s0/s1 to callers — hence the
  unused `sw s0/s1` here). Cannot match alone.
- The old 4-word residual (frame 40/48 vs 64, homes 56/32) was **three deleted helpers**: lock
  `func_80095CF4` (osRecvMesg on D_80152770), unlock `func_80095CFC` (osJamMesg), and free `func_800960CC`
  (lock; audio_reverb_update(addr, 0); unlock) — the stubs right after func_80095CE8 and between
  audio_reverb_update and audio_effect_process. Defined for real (non-static, internal) they are inlined by
  umerge, leave their retail jr-ra stubs, and each inlined free costs 24 bytes of frame.
- Same-shape sibling found by searching callers of audio_reverb_update: **func_800C885C** (two guarded frees of
  D_8011025C/D_80110260, homes 56/32) — matched first try with the same helper.
- Landing: `prefer_definition` entries for func_80095CF4, func_80095CFC, func_800960CC → `wheel_params_set.c`
  (they are locked today as empty singles). Unit: `blob_unit … score wheel_params_set func_800C885C func_80095CF4
  func_80095CFC func_800960CC --internal (three stubs) --neighbours` → 5/5 EQUAL, 0 locked bodies differ.
- func_800C8918 (628 B, five frees with homes 192/168/80/56/32) is the next sibling: same helper.

### physics_collision_test (+ func_800B0580) — MATCH
Pool reset: pool D_8013F1E0 = 100 × 88 bytes at D_8013C378, flag 0, free list; zero D_801392D8[6]; when
D_80156994 or D_8014978C >= 6, six calls func_800B24EC(D_80117480[i], &D_8013F380[i], 0, (s8)(D_80140BDC-1), 1)
(D_80140BDC u8).
- The previous 5/60 → 4 words residual was the store order of the pool fields. **Real source: a call of the kept
  `struct_fields_init(pool, mem, size, count, flag)` (= pool_init + pool_linked_list_init)**, which umerge inlines;
  the inlined statements carry the call's `.loc`, so as1 orders the four stores as one line. Unit EQUAL with
  `struct_fields_init(&D_8013F1E0, D_8013C378, 88, 100, 0)` (`physics_collision_test/unit_call.c`). Standalone: a
  one-line multi-statement macro (a static helper keeps its own lines: 12 rows; a kept `__inline` copy also
  matches).
- Sibling **func_800B0580** (other pool_linked_list_init caller): drain D_80155220's active list
  (`while (pool.head) { n = pool.head; func_8008D0C0(n->data); func_800AFA84(&pool, n); }`), re-init 100 × 36 at
  D_80155B30 flag 1, memset D_80155290 2208. MATCH standalone; unit EQUAL with the struct_fields_init call
  (`func_800B0580/b.c`).

### func_800B23E0 (+ func_8008B2E4) — MATCH
Arcade ancestor **LIB/fmath.c `Random(F32 max)`**: `rannum = ((F32)(rand() & 0x07FFF)*max) / 32767.0;` (N64 divisor
32768.0f). func_8008B2E4 is that function; rand is the locked kept func_8008B2B4. The redundant `& 0x07FFF`
(rand already masks) is what moved the temps to t0/t1 and the seed address to v1 (3–4 words off without it).
func_800B23E0 = `do bit = Random(32.0f); while (!(D_80123418[index] & (1 << bit))); return bit;` (u8). Mask read
in the condition (a named mask local: 14 words off). Unit EQUAL with plain calls (`func_800B23E0/v/a.c` +
`rand_arc_unit.c`); standalone uses static copies. The previous 13/67 had neither the helper nor the double mask.

### func_800E7D0C — 5/49 words off (group context, not claimed)
Heap init (audio_heap group context). Body: `func_800E7D0C/best.c`; build with
`tools/gvar.py src/blob/groups/audio_heap func_800E7D0C OUT best.c` and `extern s8 D_80116488;` (not volatile).
- Fixed: the flag address CSE (15 → 5) by making the once-init a deleted helper taking the flag **by pointer**:
  `void func_800E7D04(s8 *flag) { if (*flag == 0) { *flag = 1; osCreateMesgQueue(...); osJamMesg(...); } }`
  called as `func_800E7D04(&D_80116488)` (stub func_800E7D04 sits right before). `volatile` does not stop the
  CSE (tested in group and unit). Helper by value / without the test: 15.
- Residual (lane: as1 schedule): `lui v1` for `&D_801527C8` (coloured v1, save 0.5) is emitted before
  `addiu v0,v0,31; li at,-32` instead of after. Tried: one-line/assignment-chain spellings, `-32`/int casts, block
  scopes, compiled-out ifs between the statements (17), store order, inline heap-setup helper (h4: 10).
  Next: edit the `.loc` of the hoisted `la` in the ugen listing to learn which line retail had.
- Traced report: `runs/e7a/report.txt`.

### func_800EC190 — 8 aligned rows (18/56 positional)
Slot setup: D_8014978C = owner (s8 param), volatile D_801543CA = 6 (retail `la; sh 0(t9)`), six 8-byte records
D_80153E88 (owner +0, ordinal +5, flags +6 = 176, index +7) with the dead `i >= 6` arm, then wrap-at-6
counter D_801163F4 (s16) stored to D_8014A118, D_80150C04, D_801163F4 and D_80150F14 = 2.
- Closed: loop registers (i must be web-coloured before n: `for (i = 0, n = 0; …)` — uopt colours in **web
  number order**, first-appearance order, not priority), player-arm statement order (24-perm sweep: index,
  ordinal, owner, flags).
- Residual (lane: address colouring): retail colours `&D_8014A118` (v1, hoisted before the wrap test) and not
  `&D_801163F4`; ours the reverse (w44 save 0.5). Oracle `gforce.sh ec3 func_800EC190 "p2:w44=n"` → 6 rows (rest
  is `&D_8014A118` needing a register). Tried: volatile on either, chained stores, promoted-global `++` forms,
  static next/set helpers, store permutations (96), `> 5`. Next: find a second use of `&D_8014A118` (a store
  that dies, or an inlined setter taking the address) that gives it save > 0.
- Trace: `runs/ec3/report.txt`.

### func_800E7A98 (sibling lead) — 25 rows in unit
Lock (inlined func_80095CF4), default heap D_801527C8 when NULL, unlink from the heap list, audio_reverb_update(h,
1), unlock. Residual: retail keeps the heap in a new variable (v0) and reads the list head in both arms; the
`li a2,1` of the tag is a separate constant from osRecvMesg's. `func_800E7A98/best.c` (variant d).

## What generalises

1. **Kept 1–3 statement functions are inlined across files in the unit, and their statements take the call's
   `.loc`.** physics_collision_test/func_800B0580 (struct_fields_init), car_stats_display (func_80095CE8 getter),
   func_800B23E0 (Random → rand). Signs: a locked kept function with **no jal callers in retail**
   (`grep -c NAME> all.dis` = 0); stores from several statements scheduled as if on one line. Standalone, a
   one-line macro (or a kept `__inline` copy) reproduces the single `.loc`; a static helper does not.
2. **Paste the arcade function literally, including redundancies.** `rand() & 0x07FFF` re-masks an already masked
   value; the dead mask costs a temp and moves the whole ring (func_8008B2E4/B23E0, 13 words closed).
3. **Deleted helpers recovered from stubs explain 24-byte frame steps**: lock/unlock/free stubs around the heap
   module (func_80095CF4, func_80095CFC, func_800960CC). One helper instance per call = one home 24 bytes apart.
4. **Passing a global by pointer to an inlined helper stops address CSE** (func_800E7D0C's flag: `lui; lb lo()`
   and `lui at; sb lo(at)` instead of a shared `la`); `volatile` does not.
5. **Loop bounds through a global the loop provably does not write stay rolled**; a local copy of the same bound
   gets unrolled by 4 (func_800D2054).
6. **uopt colours webs in web-number (first appearance) order**: when two loop variables have swapped v0/v1,
   initialise them in the other order (`for (i = 0, n = 0; …)`).
7. `.noalias reg,$sp` again (w6d): `p = ARRAY; p += i;` instead of `&ARRAY[i]` (car_lights_render).
8. Same-shape sibling searches that paid off: callers of the internal free (func_800C885C), callers of
   pool_linked_list_init (func_800B0580), the RNG constant 0x41C64E6D (func_8008B2E4). Remaining RNG users
   (`grep 0x4e6d`): func_800D6160 (unit, Random(12.0f) with signed trunc), hud_render, mode_select_handler, …

## Tools added (cloud/work/frontier/w8c/tools)
- `batch.sh NAME FLAGS files…` — 2-way parallel aligned-diff summary on the builder.
- `gvar.py SRCGROUP FUNC OUTDIR bodies…` + `gbatch.sh FUNC OUTDIR` — replace one function's body in a locked
  group (moved from context to members) and score each variant's line.
- `udiff.py NAME` — aligned diff of NAME from the last `blob_unit --tag w8c` object (lo16 relocation fields show
  as 0 on our side; ignore those rows).
- Others copied from w7d with paths changed (`sc.sh`, `fd.sh`, `gt.sh`, `gforce.sh`, …).

## Types / globals recovered
- `D_8017A510`: 72-byte viewport rows (scale +28/+32, proj +36/+40, centre +44/+48). Camera: `f32 mat[9], pos[3]`.
- Event node (D_80149808 free pool / D_80149860 active list): handle +8, busy u8 +12, active u8 +13, u8 +14,
  start f32 +16, delay f32 +20, u8 +24, args[4] +28, data +44, func +48, type u8 +52, link +56.
- Pool (`struct_fields_init`): flag u8 +0, count +4, size +8, mem +12, head +16. D_8013F1E0 (100 × 88),
  D_80155220 (100 × 36).
- Lap tables: D_80144018 u8 counter[], D_80149A78 f32[][8], D_80144DA8 best[].
- D_80153E88: 8-byte slots (owner s8 +0, ordinal +5, flags +6, index +7); D_801543CA s16 slot count (volatile).
- D_80123418: u32 bit masks indexed by a u8.
