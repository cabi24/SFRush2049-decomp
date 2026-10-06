# Wave 9, agent w9h: camera cluster

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w9h` (a copy of base, rsynced from the current tree on
2026-10-06). All scoring used `-g0 -O3 -mips2 -G 0 -non_shared`, at most 2 cores. Nothing was committed or
spliced, and no protected path was edited. No tool call was denied.

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| `camera_scene_manager` | 2,456 | **strict MATCH** in a real -O3 group, with no stand-ins; own .rodata verified; unit EQUAL | `-g0 -O3 -mips2 -G 0 -non_shared` | `camera_scene_manager:` / `MATCH` / `own .rodata verified at 0x80123F38..0x80123F54` |
| `camera_play_script` | 3,520 | first structural draft, 844/880 words differ (884 compiled) | `-O3` (unit) | `FAIL camera_play_script: 844 of 880 words differ; compiled body is 884 words, target 880` |

## camera_scene_manager: strict MATCH (group)

Deliverable: `groups/camera_scene_manager/` (`group.json` has `"claims": ["camera_scene_manager"]`). The group
has the same seven members as the locked `src/blob/groups/camera_scene_manager` (their sources are unchanged),
plus camera_scene_manager. It also defines the three inlined trackers `func_800C2418/2420/2428`, which are not
kept. Body with header comment: `camera_scene_manager/best.c`.

```
cloud/work/frontier/w9h/grp.sh groups/camera_scene_manager          # score.py group on the builder
  func_800C2944 MATCH, func_800C26C4 MATCH, func_800C2430 MATCH,
  func_800C1B60 MATCH (own .rodata verified at 0x80123E94..0x80123EE4),
  func_800C2004 MATCH (0x80123EE4..0x80123EE8), func_800C220C MATCH (0x80123EE8..0x80123EEC), func_800C3578 MATCH
camera_scene_manager:
  MATCH
    own .rodata verified at 0x80123F38..0x80123F54
(--claims mode: Members: camera_scene_manager: MATCH, own .rodata verified at 0x80123F38..0x80123F54)

python3 -m tools.conveyor.pipeline.blob_unit --tag w9h --jobs 2 score camera_scene_manager func_800C2418 \
    func_800C2420 func_800C2428 --with cloud/work/frontier/w9h/groups/camera_scene_manager/group.c \
    --internal func_800C2418 --internal func_800C2420 --internal func_800C2428 --neighbours
  EQUAL camera_scene_manager: 614 words (kept, c_group.c)
  EQUAL func_800C2418: 2 words (internal, c_group.c)
  EQUAL func_800C2420: 2 words (internal, c_group.c)
  EQUAL func_800C2428: 2 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
```

**Integration notes.** This group supersedes the locked group of the same name: same members plus
camera_scene_manager. Replace `src/blob/groups/camera_scene_manager/` with this directory, dropping `"claims"`.
The three stubs at 0x800C2418/2420/2428 are locked today as empty singles (`src/blob/func_800C24{18,20,28}.c`).
In the unit they must be internal and resolved to the group's real definitions: add `prefer_definition` (and
`force_internal`) entries in `src/blob/unit_overrides.json`, as for `func_800AFD54`. Otherwise the shadow gate
resolves them to the empty bodies and they are not inlined.

**Semantics.** This is the per-frame stunt/trick driver over all players: idle timer, wheel-contact counters,
axis-flip detection, eight per-axis trackers, and the landing bank (`func_800C1B60(i, 11)`). It has nothing to do
with the camera. No arcade ancestor was found (the stunt system is N64-only).

**What closed it** (starting from w1g's 452/614 words): steps 1-4 brought it to 16 rows, step 5 to 0.
1. **Three inlined siblings.** func_800C2418/2420/2428 are written as real functions with one call site each,
   between `func_800C2430` and `func_800C220C` in the call order. This reproduces the stubs. Write them **without**
   a cached `fl` local (the matched siblings have one): with `p->flags` read directly, uopt's CSE produces
   retail's reloads.
2. **Index-form loops.** `ply = &D_801569B8[i]; car = &player_array[i];` at the top of the body, and the first loop
   as `for (i…) if (D_801569B8[i].flags & 0x1F7)`. uopt then strength-reduces both loops into one `$s2` IV,
   and the preheader order matches.
3. **Stunt record without a pointer local.** `D_80152038[i].f60 / .count` instead of `Stunt *st`. With the
   local, the store through `st` kills the tail's PRE web for D_80157238. That moved the v0/v1/a1 colouring of
   the *whole* body (traced: the D_80157238 web in the inlined trackers lost v1). This single change took the
   function from 126 rows to 19.
4. **No `fl` local in the driver.** The flip pairs are `ply->flags &= ~A; ply->flags |= B;` (two stores, no
   cast needed). The 0x80 tracker needs its sum in a named float assigned inside the test
   (`3.1415927f <= (s = p->a30 + (p->a28 + p->a2C))`). The tail test is written `ply->t38 != -1.0f` (operand
   order of `c.eq.s`).
5. `*(f32 *)(u32)&D_8016139C += *pdt;`: the address-laundered `+=` stores through the hoisted address register,
   as the loads do.
   Literals are natural (2.69f, 0.15f, 0.4712389f x2 copies, 3.1415927f, 5.0f, 0.25f); the bits were checked
   against retail 0x80123F38..0x80123F50.

Struct layouts (`Ply` 0x7C, `PCar` 0x3B8, `HudRec` 0x808, `Stunt` 0x78) are as in the locked group.

## camera_play_script: not matched (draft and notes)

`camera_play_script/NOTES.md` has the semantics (car-against-polygon edge clip with recursive best-hit merge),
the recovered frame layout, residual lanes and the next hypothesis. The draft is `camera_play_script/body.c`
(`best.c` is the same with the unit prelude). The structure is right up to and including the 4x-unrolled vertex
conversion. The rest is register allocation: retail fills all of `$s0`-`$s8`, while ours splits the col, -1,
952 and player_array webs. The FP pressure differs too. About 4 variants; this was stopped for breadth, not
because it was stuck.

## What generalises

1. **A pointer local into a global array can poison a PRE web far away.** A store through `Stunt *st` (even
   with `st = &D_80152038[i]`) is treated as aliasing every global, so it kills `D_80157238`'s availability.
   Indexing the array directly (`D_80152038[i].f60 = …`) lets uopt keep the web. That changed the colouring
   order of the whole function. When retail keeps a global in one register across a store, check whether the
   store is through a named pointer local.
2. **Shared strength-reduction IVs:** two loops that index the same array by `i` share one SR temp, and so
   one callee-saved register. A pointer-walking first loop (m2c's shape) gets its own register.
3. **The traced uopt works on plain -O3 groups,** not only on the unit. `gtr.sh`/`gtr2.sh` (builder side) run
   cc -j / uld / usplit / umerge and the instrumented uopt on a group. `tr.sh BODY LABEL PROC` prints the decisions.
   `force.sh LABEL PROC "p1:wN=cM"` re-runs uopt+ugen+as1 with a forced colour and diffs. Colour codes: int
   1=v0 2=v1 3=a0 4=a1 5=a2 …, float 24=f0 25=f2 26=f12 27=f14 … 30=f20 33=f26. Forcing one web
   (w87 = v1) showed that the whole 452-word residual was one priority inversion before any source search started.
4. **Batch scoring large functions:** compare words, not disassembly text. One early extra word shifts every
   absolute branch target in the text. `bfull.py` reports "miss" = retail words not in the LCS.
5. An inlined callee's cached `fl = p->flags` local, copied from its matched siblings, made things worse here.
   Sibling style is a starting point, not evidence.
