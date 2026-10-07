# Wave 13: lane w13c (camera_update)

Assignment: camera_update (2,876 B), 17 residual words, starting from `w12f/camera_update/best.c`.
Flags `-g0 -O3 -mips2 -G 0 -non_shared`. Builder scratch `watchman2:~/rush2049/scratch/frontier/w13c`
(fresh copy of `base`; synced src/blob, include, tools/cloud, asm/us/blob and blob_matched.lock.json;
trace toolkit installed with `--reuse wtk`).

**No new match. camera_update is unchanged at 17/719.** Nothing to splice and no group to supersede.
The w12f group (`w12f/groups/camera_aspect_ratio/`, `"claims": []`) is still the work base.

| Function | Bytes | State | Flags | Exact scorer output |
|---|---:|---|---|---|
| `camera_update` | 2,876 | near-miss, 17 words (unchanged) | -O3 | `FAIL camera_update: 17 of 719 words differ` |
| `camera_free_look` | 436 | code identical, provisional (caller unmatched) | -O3 | `EQUAL camera_free_look: 109 words (internal, c_group_w12f.c)` |
| `camera_track_spline` | 860 | code identical, provisional (caller unmatched) | -O3 | `EQUAL camera_track_spline: 215 words (internal, c_group_w12f.c)` |

Unit confirmation, run from the repo root on the Pi. `tmp/group_w12f.c` is a verbatim copy of the w12f group.c.
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w13c score camera_process_input func_800C15FC camera_update \
  camera_free_look camera_look_at_point camera_track_spline camera_aspect_ratio camera_fov_control \
  camera_build_view_matrix --with cloud/work/frontier/w13c/tmp/group_w12f.c --internal func_800C15FC --neighbours
  EQUAL camera_process_input: 255 words (kept, c_group_w12f.c)
  EQUAL func_800C15FC: 2 words (internal, c_group_w12f.c)
  FAIL camera_update: 17 of 719 words differ
  EQUAL camera_free_look: 109 words (internal, c_group_w12f.c)
  FAIL camera_look_at_point: 38 of 199 words differ
  EQUAL camera_track_spline: 215 words (internal, c_group_w12f.c)
  EQUAL camera_aspect_ratio: 50 words (internal, c_group_w12f.c)
  EQUAL camera_fov_control: 60 words (internal, c_group_w12f.c)
  EQUAL camera_build_view_matrix: 153 words (internal, c_group_w12f.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/9 equal
```

## What was established (oracle evidence, snapshot `b0` = w12f best.c)

Proc ordinal 598. 48 p1 decisions and no p2. Callee-saved cost 38.

- **Webs identified.** w277 = `na` (s16 at -26), w286 = `m` (u16 at -28), w242 = `tb` (-24),
  w344 = `tt` (-76). Forcing `p1:w277=c4,p1:w242=c5,p1:w286=c3,p1:w344=c27` leaves exactly the 5 compare
  rows. `p1:w344=c27` alone leaves 13 rows. Single forces do not fix the 8-word family:
  - `w277=c4` alone moves tb to a0 and m to v1.
  - `w242=c5` alone fixes only the tb rows.

  So the family needs v1 to be unavailable to all three of na, m and tb, and m must take a0 before na
  does.
- **Forbidden masks** (p1dec):

  | Web | forbidden0 | Colours forbidden |
  |---|---|---|
  | na | `0x40000a00` | v0, s6, s8 |
  | m | `0x60000a00` | adds v1, from na |
  | tb | `0x70000a00` | adds a0 |
  | tt | `0xc0` | f0 (`f`), f2 (`fr`) |

  Caller-saved costs are all 0 for these webs, so cost cannot steer them: retail needs v1 forbidden to
  na, m and tb, and f12 forbidden to tt.
- **No existing web can be the holder.** I forced every int p1 web to v1 (`p1:wN=c2`, 39 webs) and every
  FP web except tt to f12 (`p1:wN=c26`, w358, w184 and w187). None lowered the row count below 17, and many
  raised it or were declined. So retail's v1 and f12 holders are webs that do not exist in our ucode:
  either a web whose code is entirely folded, or a hard-register pin
  (workbench law L58: `forbidden0` is seeded from hard-register conflicts as well as neighbours).
- **Retail context.** At the four other setter sites, retail (and ours) colour the inlined setter's
  `slot` = v0 and `v` = v1. At the s50 site the retail `v`/m is a0, so v1 is blocked exactly there. No
  retail instruction touches v1 between 0x800C1124 and 0x800C1430, or f12 anywhere in the function.
- **Compare rows (5): mechanism found.**
  - Pre-uopt ucode is in source order: site 1 is `equ(idx+1, count)` and site 2 is `equ(count, idx+1)`.
  - uopt rewrites site 1 to `equ(idx, count + -1)` and swaps site 2 to `equ(idx+1, count)`.
  - The ordering is consistent with "the operand node created first goes left": `add(idx,1)` is created
    at site 1 before `ilod count`.
  - ugen then evaluates idx+1 first at both sites. Retail evaluated count first, so retail's ucode had
    count on the left at both sites.
  - Writing site 1 count-first makes the compare's idx+1 CSE with the else-arm `ctl->idx = idx + 1`
    (w12f's 496-row result). Every alternative spelling of the store either adds a load (`ctl->idx += 1`,
    `++`: 720 words) or CSEs.

  What is needed is a site-1 form in which `sc->count` is created before any `idx + 1` and the store's
  idx+1 is not a syntactic twin of the compare's. None was found.

## Variants tried (about 55, all in `v/`, scored with `tools/s3n.sh`)

| Dir | Idea | Best result |
|---|---|---|
| `d1/` | compiled-out `if (x) {}` for every local at the end of block_99 | memory vars: 17 (no change); register vars add code |
| `t/` | un-merge idx (separate flags or s14/counter variable); tb as direct indexing | colours unchanged (frame 256); tb direct: 732 words |
| `c/` | 8 site-1 compare and store spellings | 17 at best (`count - 1 == idx`); others 363–639 rows |
| `s/` | `na`/`m` expression forms, store `cam->s50 = s58 + idx`, operand order | 17 at best |
| `f/` | fraction as a loop-invariant `tt / f`, `tt /= f` | 17; the `tt/f` web takes f2, and tt stays f12 |
| `m/` | no `m` (argument direct), m inside the else | 34 rows or worse |
| `r/` | `return` instead of `goto block_99`; hoisting tb above the calls | 165+ rows |
| `x/` | compiled-out expression reads (`cam->s58`, `node->s04`, `tb->s14`, `cam->slot`, `sc->flags`) after the s50 store | 17 at best |

## Best next hypotheses

1. **Hard-register pin (L58).** Look for a source shape that puts na/m/tb, and separately tt, live across
   something that pins v1/f12 at zero instructions. Instrumenting uopt to print the initial
   hard-conflict vector per web (not in the toolkit today) would answer "pin or web" directly. This is
   the cheapest next step.
2. **Folded-copy holder.** An expression web coloured v1 (or f12) whose only instructions are a def plus
   a copy that as1 folds, like the `idx+1` → nx (`addu a0; move a2,a0`) web that already exists here.
   A candidate is something CSE'd between the s04 increment and the s50 store. No natural spelling has
   been found.
3. **Compares.** Inspect uopt's commutative-operand ordering (node creation order) with `udn.py` on
   `opt.o`. A source in which `sc->count` is entered before the loop's first `idx + 1` (for example, a
   count read earlier in the loop) may flip both sites without creating the CSE.

## Files

- `best.c`: unchanged copy of w12f's best. `base2.c`: w12f's cluster without the body. `retail.s`:
  retail disassembly.
- `tools/s3run.sh`, `tools/s3n.sh`: splice a body and unit-score it (s3n also prints nosp rows).
  `tools/dead.py`: dead-read generator. `fnset.py`/`fnget.py` are copied from w12f.
- `v/*/`: every variant above.
- `tmp/group_w12f.c`: a verbatim copy of the w12f group.c, used for the unit confirmation.
- Traces (Pi): `build/trace/w13c/b0.camera_update.cdx`, `pre_b0.txt` (PRE/expression bits),
  `b0_m.txt`/`b0_u.txt` (pre-/post-uopt ucode dumps), `ugt_b0.txt` (ugen trace and listing).

## Integration notes

Nothing to integrate. No overrides, no superseded groups. When camera_update closes, follow w12f's notes:
- add camera_update, camera_free_look and camera_track_spline to the group's claims and members;
- switch `D_80123E8C` to `0.0425f`;
- keep `prefer_definition` func_800C15FC.

Permission denials: none.

## What generalises

- **Prove a hidden holder with a brute-force colour sweep.** Force every existing web onto the missing
  colour (`p1:wN=c2` for each N, about 4 s per run). If none closes the rows, the holder is not in the
  current ucode. Stop hunting colour levers and look for a missing web or a hard pin.
- **uopt reorders commutative compare operands, apparently by node creation order.** Source operand
  order is therefore not what reaches ugen. The pre-/post-uopt ucode dumps (`udn.py` on `merged` and
  `opt.o`) show it directly.
