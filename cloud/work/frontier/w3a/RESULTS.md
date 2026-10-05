# Frontier wave 3 — agent w3a (hub lane) results

Builder scratch `~/rush2049/scratch/frontier/w3a` (copied from `base` on 2026-10-05; `tools/cloud/owndata.py`
refreshed from the local repo so the scorer verifies own data). Unit tag `w3a`. Nothing committed or spliced.

| Function | Addr | Bytes | Deps | State | Deliverable |
|---|---|---:|---:|---|---|
| `object_manager_update` | 0x800B3FA4 | 532 | 89 | **strict MATCH (group)**, unit EQUAL | `groups/object_manager_update/` |
| `func_8008E26C` | 0x8008E26C | 300 | 41 | **strict MATCH** (single), unit EQUAL | `cloud/matches/func_8008E26C.c` |
| `string_copy_format` | 0x80092E2C | 436 | 30 | **strict MATCH, own .rodata verified**, unit EQUAL | `cloud/matches/string_copy_format.c` |

All three at `-g0 -O3 -mips2 -G 0 -non_shared`. Spliceable now: 1,268 bytes (dependant counts 89, 41 and 30 from `frontier show`; they overlap).
None uses a fake read, a fake extern literal or a stand-in.

## The tool that closed all three: instrumented uopt colouring trace

The previous agents stopped on register residuals they could only probe blind. I built the workbench's
globalcolor-instrumented uopt from the recompiled `uopt.c` already on the builder
(`~/rush2049/scratch/ci/tools/ido-static-recomp/build/uopt.c`), recipe in `tools/build_uopt.sh`
(`workbench instrument-uopt --profile globalcolor --allow-unverified-source`, gcc on watchman2, ~15 s).
**Fidelity check:** with tracing off, its output on the whole game unit's `merged` ucode is byte-identical to the
toolkit uopt (`cmp opt.stock opt.tr` → identical; also identical to the stage's own `opt`).

Workflow (all scripts in `tools/`):
- `ctrace.sh NAME cand.c LABEL` — scores in the unit (tag w3a), snapshots the stage's `merged`/`st` on the builder
  as `st_LABEL`, traces every procedure. `pdiff.sh L1 L2` on two snapshots of different candidates prints the
  procedure ordinal that changed (ordinals: object_manager_update = 407, func_8008E26C = 80,
  string_copy_format = 111 in the current unit; they shift when the unit changes).
- `ctrace.sh NAME cand.c LABEL PROC` — one line per colouring decision: web, `save`, `nocs`, `totalsave`, final
  register, web type and frame offset (`raw10` of a type-3 web is the local's frame offset).
- `force.sh LABEL NAME PROC "p1:wN=cM,…"` — re-runs uopt with `CDX_FORCE`, then ugen/as1, and diffs the function
  against retail (answers "would this colouring be the match?").
- `sum.sh`, `tr.sh` (builder side), `us.sh`/`ub.sh` (unit score + aligned diff / batch), `sc.sh`, `grp.sh`.

---

## object_manager_update — strict MATCH (group)

Pixel width of a text string in the current font (full semantics in the file header). Internal callee
`sound_update_channel` preserves t1/t4 by IPA, so it matches only with the real sound closure.

```
cloud/work/frontier/w3a/tools/grp.sh groups/object_manager_update      # score.py group on the builder
Members:
object_manager_update:
  MATCH
object_byte9_set:
  MATCH
func_800F68A4:
  MATCH
camera_shake_update:
  MATCH
Context (informational): sound_update_channel 122/122, func_80096288 3/4, slot_value_get 13/15,
  object_bytes23_sum 4/18, object_bytes_sum_global 11/21 differ; mode_byte2_set, mode_byte_set,
  object_type_byte2_get, object_type_byte3_get MATCH  (identical to the locked codex_sound_channel_extra group)

python3 -m tools.conveyor.pipeline.blob_unit --tag w3a score object_manager_update \
    --with cloud/work/frontier/w3a/groups/object_manager_update/object_manager_update.c --neighbours
  EQUAL object_manager_update: 133 words (kept, c_object_manager_update.c)
  locked bodies that differ in this unit: 0
```
Group = locked `src/blob/groups/codex_sound_channel_extra` (`group.c` verbatim) plus `object_manager_update.c`;
claims only `object_manager_update`. **Integration:** it supersedes `codex_sound_channel_extra` (same members +
the new one); alternatively add `object_manager_update.c` to that group's files/members/keep.

**What closed it.** Trace of the no-dead-read source (16 words, `object_manager_update/no_dead_read_16w.c`): the
width web (w3) had `save = 102/10 = 10.2`, the cached `s[pos]` byte web (w38) `10.5`, so the byte was coloured first
and took a1. w2c's fake `if (width + D_80149B70 + pos + (s32)str) {}` only raised width's total. The natural lever is
the divisor: `save = totalsave / nocs` and nocs counts the blocks of the live range. Writing `width = 0;` **after**
the wide/narrow `if/else` (instead of next to `pos = 0`) drops two blocks → 102/8, width is coloured before the byte
and takes a1. Everything else (li order in the space arm, t2/t3/t0 webs) was already right without the fake read;
the fake read was what reversed the two `li -1`. Other quirks (str reused as the counter, per-arm `prev = …`,
uninitialised `ch` read in the monospace arm) are as w2c found them.

## func_8008E26C — strict MATCH

Scene-record allocator (first free slot by id 0xFFFF, else append; count/high-water; fill; link under parent).
```
cloud/work/frontier/w3a/tools/sc.sh cloud/matches/func_8008E26C.c func_8008E26C --flags '-g0 -O3 -mips2 -G 0 -non_shared'
func_8008E26C:
  MATCH
python3 -m tools.conveyor.pipeline.blob_unit --tag w3a score func_8008E26C --with cloud/matches/func_8008E26C.c --neighbours
  EQUAL func_8008E26C: 75 words (kept, c_func_8008E26C.c)
  locked bodies that differ in this unit: 0
```
**Route (evidence in `func_8008E26C/`).** w2f's address-taken-index-through-a-static design was a dead end: with the
offset hoisted (`static_offset_48w.c`) the trace shows the caller's index as its own web (copy at the `*p = i`
block interferes with the loop index, so it can never share v1: `CDX_FORCE` to v1 is declined); a single
address-taken index (`aliased_index_62w_loop_init.c`) gets every register right but strength reduction leaves a
`sll t7,zero,6` initialiser, because promotion of an aliased local happens after SR. Dropping the address-taking:
- a plain single function with a **byte-offset local** `off = i * sizeof(Ent)` right after the loop and
  `e = (Ent *)((u8 *)D_8012E700 + off)` after the two count updates reproduces retail's i*68 in a3 at both loop
  exits and the late base add (`plain_offset_10w.c`, only frame/store off);
- the `sw v1,32(sp)` that is never reloaded is i's **home store for a use after the call that generates no
  code**: a compiled-out range check `if (i < 0 || i >= D_80156990) DEBUG_PRINT(...)` after `render_mode_select`.
  A check against a constant does not work (uopt's test replacement rewrites `i` onto `off`, 6 words);
- `return (s16)i` (the narrowed argument is the CSE'd value spilled at sp+24), and declaration order `e, i, off`
  for i's home at sp+32 (`deadcheck_wrong_slot_1w.c` is the same with i at sp+36).

## string_copy_format — strict MATCH, own .rodata verified

```
cloud/work/frontier/w3a/tools/sc.sh cloud/matches/string_copy_format.c string_copy_format --flags '-g0 -O3 -mips2 -G 0 -non_shared'
string_copy_format:
  MATCH
    own .rodata verified at 0x80120E68..0x80120E74
python3 -m tools.conveyor.pipeline.blob_unit --tag w3a score string_copy_format --with cloud/matches/string_copy_format.c --neighbours
  EQUAL string_copy_format: 109 words (kept, c_string_copy_format.c)
  locked bodies that differ in this unit: 0
```
Trace of w2h's best: the `name` copy web had `forbidden0=0x1c010000` (a0-a2 and s1 only) and a3/t0/s0… all at cost
0.1, so lowest index a3 won. Retail's t0 needs a3 forbidden, i.e. the fourth parameter's web alive from entry. The
fourth parameter (callers pass 1) is a **"complain" flag read only by a compiled-out diagnostic in the not-found
branch**: `if (complain) DEBUG_PRINT((…));` before `return -1`. That also explains its home store that is never
reloaded. With it the *plain* source (no `p = name; name = key.name` shuffle, no `pad`) matches. Placing the dead
read elsewhere (entry, loop, final return) fails (35-95 words); `s8`/`u8`/`s32` all match, `s16` breaks callers.

---

## What generalises

1. **Instrumented uopt works on our whole-program unit** and turns register residuals from searches into readings:
   one trace says which web lost, by how much (`save`), and which registers were forbidden. Three hub functions
   that had absorbed ~900 blind variants between them closed in a few dozen compiles each.
2. **`save = totalsave / nocs`, and nocs counts live-range blocks.** Moving an initialisation past an `if/else`
   (or otherwise shortening a range) raises a web's priority without touching its uses — the natural lever behind
   many "fake dead read raises priority" results.
3. **Compiled-out debug code is real source structure.** `if (cond) DEBUG_PRINT((…));` with an empty macro keeps
   the condition's variables live into uopt's allocator and then emits nothing. Signatures: a parameter or local
   that is homed/stored but never reloaded (`sw a3,172(sp)`, `sw v1,32(sp)`), a parameter "never read", or a
   register forbidden for no visible reason. Try the check where a diagnostic would naturally sit (not-found
   branch, after a link/allocation) before reaching for a fake dead read. Prefer conditions against variables:
   uopt's linear-function test replacement rewrites a constant compare onto an induction-derived web.
4. **Address-taken locals are a poor explanation for a dead stack store** when the function has a loop on that
   variable: promotion of aliased locals runs after strength reduction and leaves a visible initialiser artefact.
