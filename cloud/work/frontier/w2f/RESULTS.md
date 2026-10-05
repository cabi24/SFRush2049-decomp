# Frontier wave 2, agent w2f (2026-10-04/05)

Scorer: `tools/cloud/score.py` in the builder copy `watchman2:~/rush2049/scratch/frontier/w2f`
(`IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a.../ido`). `sc.sh SRC NAME [args]` copies the source to
`cand/NAME.c` and runs `python3 tools/cloud/score.py fn cand/NAME.c NAME [args]`; `grp.sh DIR` runs
`python3 tools/cloud/score.py group cand/<copy of DIR>`. Whole-program checks:
`python3 -m tools.conveyor.pipeline.blob_unit --tag w2f score ...` from the repo root. Nothing was committed,
spliced or locked. All results were re-scored together at the end against the lock at 751 functions.

| # | function | bytes | state | flags | where |
|---|---|---:|---|---|---|
| 1 | func_8008E26C | 300 | **not matched: 34/75 words** (was 68) | -O3 | `func_8008E26C/best.c`, `NOTES.md` |
| 2 | exhaust_smoke_effect | 452 | code identical, own-rodata unverified (3 literals) | **-O3** (-O2: 80 words) | `exhaust_smoke_effect/best.c` |
| 3 | func_800D2FA8 | 1,160 | **strict MATCH** (standalone; `group` label is a false positive) | **-O3** (-O2: 278 words) | `cloud/matches/func_800D2FA8.c` |
| 4 | car_damage_visual | 680 | **strict MATCH** in a real group, no stand-ins | -O3 | `groups/heap_compactor/` |
| 5 | physics_friction_apply | 504 | **strict MATCH** in a real group, no stand-ins | -O3 | `groups/path_graph_links/` |
| 6 | menu_options_screen | 2,356 | code identical, own-rodata unverified (4 literals) | **-O3** (-O2: 582 words) | `menu_options_screen/best.c` |
| 7 | func_800BA61C | 424 | code identical, own-rodata unverified (1 literal) | -O3 (same words at -O2) | `func_800BA61C/best.c` |

Strict: 3 functions, 2,344 bytes. Code identical: 3 functions, 3,232 bytes. Open: 1 function, 300 bytes.

Whole-program unit, all six together (`--internal func_800A51D8 --neighbours`):
```
  EQUAL func_800D2FA8: 290 words (kept, c_func_800D2FA8.c)
  EQUAL exhaust_smoke_effect: 113 words (kept, c_exhaust_smoke_effect.c)
  EQUAL func_800BA61C: 106 words (kept, c_func_800BA61C.c)
  EQUAL menu_options_screen: 589 words (kept, c_menu_options_screen.c)
  EQUAL car_damage_visual: 170 words (kept, c_car_damage_visual.c)
  EQUAL physics_friction_apply: 126 words (kept, c_path_graph_links.c)
  locked bodies that differ in this unit: 0
blob_unit score: 6/6 equal; object build/blob_unit/w2f/unit.o (4.7s)
```

## Strict matches

### func_800D2FA8 — `cloud/matches/func_800D2FA8.c`
`score.py fn cand/func_800D2FA8.c func_800D2FA8 --flags "-g0 -O3 -mips2 -G 0 -non_shared"` -> `func_800D2FA8:  MATCH`.
Track path graph search (N64 code): visited list `s32 D_80124F88[]`, loop nodes mapped proportionally, else
distances both ways round and `split_time_display` for the remainder. Earlier state: 264/290
(`src/blob/groups/func_800D2FA8`, context only). What closed it, in order:
- one scratch variable `v` holds the `next` link, then the span length (`v = prevPos - nextPos + 1`), with the
  fields re-read in source (named copies of `nextPos`/`prevPos` let uopt forward the length into temps): 264 -> 5;
- operand orders `nextPos + pos * v / numPoints` and `*outPos + start - numPoints`; `dist1` updated **before**
  `start1 = start2` (the old source used the new `start1`, which was also wrong semantically): 5 -> 1;
- the last test written `start2 - start1 > start1 + numPoints - start2`: 1 -> 0;
- two plain unused locals replace the old `volatile s32 padv[2]` (same 112-byte frame).
It matches alone: no group needed. Types: `PathNode` (16 bytes), `PathGraph D_801407F0`, `Section D_80151CE8[]`
(0x50; `s16 last` at +2, `s16 start` at +0x2E) as in the existing group source.

### car_damage_visual — `groups/heap_compactor/` (claims: `car_damage_visual`)
`score.py group cand/<heap_compactor>` -> all nine members `MATCH` (`car_damage_visual` plus the eight already
locked), all six context functions `MATCH` including `func_800A51D8`.
Heap compaction under the heap queue lock. Earlier state: 85/170 (`cloud/work/ipa-groups/codex_heap_compactor_a153`).
- **`func_800A51D8` is a deleted static**: the `jr ra; nop` stub directly in front is
  `Heap *get(Heap *h) { if (h) return h; return D_801527C8; }`, inlined at the top. With it the first 46 words
  (value in `v0`, copy to `s7`, the reload of `heap->first` for the second loop) match at once.
- scan loop with `next` set on both paths; no variable for the payload address; `size` assigned, then the
  memmove argument re-reads `block->size`; remainder size stored before its other fields; final `if` polarity.
- `audio_reverb_update` is internal (arguments in `a1/a2`), so the body needs the real release module:
  `group.c` and `alloc_at.c` are unchanged copies of the locked `src/blob/groups/codex_heap_release_a25/`.
- **Integration:** this extends (supersedes) `codex_heap_release_a25`; `func_800A51D8` must stay out of `keep`,
  and `src/blob/unit_overrides.json` needs a `prefer_definition` entry for it pointing at
  `car_damage_visual.c` (its locked definition is `(void) {}`). In `blob_unit score` I passed
  `--internal func_800A51D8`.
- Layout: block header 0x20 (`magic 0xFEDCBA98`, `next`, `prev`, `u32 size`, `u32 *ref`, `s8 used`, `s8 tag`,
  `u8 lock`); heap `+8 first`, `+0xC last`; `Heap *D_801527C8`.

### physics_friction_apply — `groups/path_graph_links/` (claims: `physics_friction_apply`)
`score.py group cand/<path_graph_links>` -> `func_800B98D8`, `func_800B9B64`, `minimap_render`,
`physics_friction_apply`, `physics_velocity_clamp`: all `MATCH`.
Earlier state: 21/126 as context in `src/blob/groups/func_800B9B64`. One change: the last-point argument is
written index first, `numPoints - 1 + points` (`&points[numPoints - 1]` and `points + numPoints - 1` both
rotate three temps). The group is the locked `func_800B9B64` group text with that line changed and **both
stand-in callers removed**: `physics_friction_apply` is the real caller of `func_800B9B64` (twice) and of
`physics_velocity_clamp`. It supersedes `src/blob/groups/func_800B9B64` (revert, splice, delete the old dir).

## Code identical, own-rodata unverified (spliceable; the splice verifies the literal bytes)
Each: `score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"`.

- **exhaust_smoke_effect** -> `MATCH (6 section-relative relocations unverified: .rodata+0x0 at +0x9c, .rodata+0x0 at +0xa0, .rodata+0x4 at +0xa8, .rodata+0x4 at +0xc0, .rodata+0x8 at +0x88, .rodata+0x8 at +0x144)`.
  Viewport projection set-up into the 72-byte record `D_8017A510[idx]` (fov in radians, tan, 0.5/tan, aspect).
  Compiled `.rodata` = `3C8EFA35 3C8EFA35 3FC90FDB`, equal to retail 0x80123BC8. Closers: halvings written
  `/ 2.0f` (not shared; `* 0.5f` shares one constant in `$f20`); the two reciprocals spelled `0.5f /` and
  `.5f /`; `* 2` (int) for mul.s by 2.0; `fy = fx = 1.5707964f`. `func_800A557C` = tanf, `func_8008C720` = atanf.
- **menu_options_screen** -> `MATCH (8 section-relative relocations unverified: .rodata+0x0 at +0x2ec, .rodata+0x0 at +0x2f0, .rodata+0x4 at +0x3b4, .rodata+0x4 at +0x3cc, .rodata+0x8 at +0x5ec, .rodata+0x8 at +0x5f0, .rodata+0xc at +0x660, .rodata+0xc at +0x664)`.
  N64 descendant of arcade `setFBCollisionForce` (`reference/repos/rushtherock/game/collision.c`; proven by
  structure and by its callee `menu_load_options` = `ForceApart`). First draft from the arcade body: 418/589,
  aligned 250; literal and operand-order fixes took it to 0. Compiled `.rodata` = `3EAAAAAA 3EAAAAAA 469C4000
  469C4000`, equal to retail 0x80124108 (`0.3333333f`, not 1/3 = 3EAAAAAB). Closers are listed in the file
  header; the ones that generalise are under "Generalises" below. Layout recovered (model): `BODYR[4][3]` +0xF4,
  `CENTERFORCE` +0x124, f32[3] +0x220, f32 +0x3F0, mass +0x5C4, f32 +0x63C, s8 +0x640, s16 +0x6C4, `RWV` +0x788,
  `RWR` +0x794, `UV` +0x7A0, s16 slot +0x7C6, s8 +0x7CC; car (`player_array`, 0x3B8): s8 +0x358, +0x359,
  +0x35B, +0x35F, +0x384, +0x385, f32 +0x3AC; `s8 D_8012E67C[]`, `s8 D_8017A634`; external
  `func_8038D3A4(Car *, Car *, s32)`.
- **func_800BA61C** -> `MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x34, .rodata+0x0 at +0x40)`.
  Path point at which a checkpoint-like record is passed (else the nearest point). Body from
  `cloud/work/s20261004/C` (which matched with `extern f32 D_80123E00` and two never-used locals but did not
  claim it). Here with the natural literal `1e20f` (compiled `60AD78EC` = retail 0x80123E00). Both unused
  locals are needed (without the 4-byte one 14 words, without the s16 one 4 words); stated in the header.

## Not matched: func_8008E26C (34/75 strict, 20 aligned; was 68/27)
Full write-up in `func_8008E26C/NOTES.md`. Command: `bscore.py cand/<dir> func_8008E26C --flags "-g0 -O3 -mips2 -G 0 -non_shared" --keep func_8008E26C`
-> `aligned-missing  20  strict  34  size +0`.
Found: the index is an address-taken local filled through the out-parameter of an inlined static (that is
what produces the dead `sw v1,32(sp)` and the four parameter copies), the function returns `s32`, and the
saved value is a word local assigned `(s16)i`. Residual lane (workbench `diagnose`: structure-mismatch, pool
lane, one extra coloured web): retail keeps `i*68` in `a3` from the loop exit and adds the array base after
the two `if`s. About 1,000 variants in total; I went well past the stop rule on this one because 41
functions wait on it. Next hypothesis is in the notes (the helper is generic over base/element size or
returns the entry and the index separately).

## Generalises
1. **Operand order of commutative float ops is decided by the statement form, not by how the operands are
   written.** Measured on probes and used in three places in `menu_options_screen`:
   a plain `a + b` of two loads usually comes out as `(b, a)` (not always: `m1->mass + m2->mass` kept its
   order); a constant always goes right, whichever side it is written on;
   a compound assignment `x op= e` keeps `(x, e)` and still does so after `x`'s constant is propagated. So a
   retail `add.s`/`mul.s` with the constant on the **left** means a variable was assigned the constant and
   then compound-updated (`time = 160; time += v * 7.0f;`), and `p->f = p->f + q` vs `p->f += q` are
   different code.
2. **uopt shares a float constant only between identical spellings in comparable positions.** `0.5f` vs `.5f`,
   `/ 2.0f` vs `* 0.5f`, int `0` vs `0.0f`, `* 2` vs `* 2.0f` are all distinct. A retail function that loads
   the same constant twice into two temps has two spellings; one that keeps it in `$f20` has one.
3. **A stack store before a call that is never reloaded** (`cloud/work/frontier/w2f/deadspill.py` lists 72
   functions with one, 16 locked) has two sources: an inlined function's local or return value that the
   caller ignores (`game_timer_display`, `checkpoint_hit`, ...: `sw v0,N(sp)` after a `jal`), or an
   address-taken local written through an inlined function's out-parameter (`func_8008E26C`).
4. **Deleted statics again:** `func_800A51D8` is the heap getter. A stub next to an unmatched function was
   worth checking before anything else; the residual it explained looked like an ordinary "value in v0 then
   copied" register difference.
5. **Frontier labels:** `func_800D2FA8` (`group`, `sets_register_params`) matches alone. `car_damage_visual`
   and `physics_friction_apply` (`group`, `ring`) needed nothing ring-specific once the real module was in
   the unit.
6. **Locked groups move under you:** `codex_heap_release_a25` gained two members while I worked; a group
   that extends a locked one should be rebuilt from the locked files just before hand-over (done here).
7. `blob_unit score` rejects two `--with` files with the same basename (`best.c`); copy them to distinct
   names. Its `EQUAL` held for all three natural-literal sources.

Tools left here: `udiff.py` (aligned diff of a function in the last `blob_unit` object), `uq.sh`/`ubatch.sh`
(unit score + diff, one or many), `bscore.py` with `--feat REGEX` (flags per variant whether a retail
instruction is present), `deadspill.py`.
