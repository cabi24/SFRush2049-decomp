# Wave 3, agent w3c — results (2026-10-05)

Builder copy `~/rush2049/scratch/frontier/w3c` (from `base`); unit runs `--tag w3c`. Helper scripts in `tools/`
(`sc.sh` strict score, `full.sh`/`batch.sh` aligned diffs, `grp.sh` group score, `ubatch.sh` batch blob_unit,
`udiff.py` aligned diff of a function in `build/blob_unit/w3c/unit.o`, `pmap.sh` PrevMaxPath register map).
Nothing committed or spliced.

| Function | Bytes | State | Flags | Deliverable |
|---|---:|---|---|---|
| `PrevMaxPath` | 96 | **MATCH** (group member, internal) | -O3 | `groups/frontier_overlay_loader/` |
| `InitMaxPath` | 136 | **MATCH** (group member; own .rodata verified) | -O3 | same group |
| `sync_maxpath_to_checkpoint` | 136 | **MATCH** (group member; own .rodata verified) | -O3 | same group |
| `display_enable` | 316 | not reached; 36 aligned rows off with a stand-in caller (would be provisional at best: real caller `playgame_state_change` unmatched) | -O3 | `display_enable/best.c` |
| `func_800BFBE8` | 384 | **MATCH** | -O3 (also -O2) | `cloud/matches/func_800BFBE8.c` |
| `difficulty_select` | 412 | **MATCH** | -O3 only | `cloud/matches/difficulty_select.c` |
| `func_800B9740` | 408 | 61 aligned rows off (86/102 positional at best) | -O3 | `func_800B9740/best.c` |
| `func_800E1AA0` | 400 | **MATCH** (own .rodata verified) | -O3 (also -O2) | `cloud/matches/func_800E1AA0.c` |

Six strict (1,300 bytes). All six are also `EQUAL` in the whole-program unit with `--neighbours`
reporting 0 locked bodies broken.

---

## 1. Overlay-loader unit: PrevMaxPath + InitMaxPath + sync_maxpath_to_checkpoint — MATCH (group)

`groups/frontier_overlay_loader/` = `src/blob/groups/codex_heap_release_a25/*` **unchanged** (group.c, alloc_at.c,
car_damage_visual.c, func_800CB9D0.c, deflate_mem.c) + new `overlay_loader.c`. `group.json` adds the three to
`members`, `InitMaxPath`/`sync_maxpath_to_checkpoint` to `keep` (PrevMaxPath stays internal), `"claims"` = the three.
Integrate as a superseding group (`blob_group revert codex_heap_release_a25` first) or by adding `overlay_loader.c`
and the three members to the locked group.

```
cloud/work/frontier/w3c/tools/grp.sh cloud/work/frontier/w3c/groups/frontier_overlay_loader
  (score.py group, -g0 -O3 -mips2 -G 0 -non_shared)
Members: audio_reverb_update MATCH, audio_effect_process MATCH, synced_model_render MATCH, MP_TargetSpeed MATCH,
assign_default_paths MATCH, stat_race_end MATCH, NextMaxPath MATCH, menu_item_select MATCH, car_damage_visual MATCH,
func_800CB9D0 MATCH, car_angular_velocity_clamp MATCH,
PrevMaxPath:
  MATCH
InitMaxPath:
  MATCH
    own .rodata verified at 0x80123860..0x80123868
sync_maxpath_to_checkpoint:
  MATCH
    own .rodata verified at 0x80123850..0x80123858
Context: func_80095F8C, func_80095EF4, audio_buffer_sync, object_counter_decrement, object_counter_increment,
func_800A51D8 all MATCH
```
(each member line printed exactly `MATCH`; condensed onto lines here.)
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w3c score PrevMaxPath InitMaxPath sync_maxpath_to_checkpoint \
    --with cloud/work/frontier/w3c/groups/frontier_overlay_loader/overlay_loader.c --internal PrevMaxPath --neighbours
  EQUAL PrevMaxPath: 24 words (internal, c_overlay_loader.c)
  EQUAL InitMaxPath: 34 words (kept, c_overlay_loader.c)
  EQUAL sync_maxpath_to_checkpoint: 34 words (kept, c_overlay_loader.c)
  locked bodies that differ in this unit: 0
```

**Semantics.** `PrevMaxPath` = `load_overlay(vram, name, vramEnd, bssEnd, romStart, bssStart, &loaded, romData)`:
if not loaded, reserve `[vram, vramEnd)` in the game heap (`NextMaxPath` = alloc-at-address), mark the block
(`0x800962D4`), `inflate_decompress(romData, vram, 1)`, `bzero(bssStart, bssEnd - bssStart)`, set the flag.
`InitMaxPath` loads overlay "Extra" (flag `D_8011ED04`, data `*D_8002B024`), `sync_maxpath_to_checkpoint` loads
"Start" (flag `D_8011ED00`, data `*D_8002B020`); both at vram `0x8038A400..0x803BB380`. N64-only, no arcade ancestor.

**What closed it (from w2g's lead, 4/6/6 rows).** The open problem was the callee-saved colouring of the IPA register
parameters (retail s1 bssEnd, s2 bssStart, s3 loaded, s4 romData; lead had loaded, bssStart, romData, bssEnd).
Parameter order is inert (w2g: 24 orders). **Read count is the dial** (workbench lever 9): code-free
`if (x) {}` reads, placed after the guards, reorder the webs monotonically. Measured map (PrevMaxPath regs for
dst/bssEnd/bssStart/loaded/rom, via `tools/pmap.sh`):
- none: s0/s4/s2/s1/s3; `+2 bssEnd`: s0/s1/s3/s2/s4; `+2 bssEnd +1 bssStart`: s2/s0/s1/s3/s4;
- **`+1 dst +2 bssEnd +1 bssStart`: s0/s1/s2/s3/s4 = retail.** (`+2 dst` / `+3 dst` give the same.)
These reads are shaping quirks (stated in the file header); the original probably had something with the same
reference counts (debug asserts compiled to nothing are a guess, not evidence). `if (&name == 0) return;` (w2g)
is still what keeps `name` homed and stops umerge inlining PrevMaxPath.

## 2. display_enable — not reached (provisional at best)

Real caller `playgame_state_change` (2,544 bytes) is unmatched, so this can never be strict now. Findings:
- Retail inlines both "unload overlay" bodies (= locked `MP_TargetSpeed` / `assign_default_paths`) and
  `sync_maxpath_to_checkpoint`. In the unit umerge inlines `sync…` but **not** the kept `MP_TargetSpeed`/
  `assign_default_paths` (also not when redefined in the candidate, nor as a 2-call static helper): writing the
  unload bodies inline in `display_enable` gives the exact 79-word size and control flow.
- With a stand-in caller (`standin_caller`, `--block display_enable`, display_enable internal) the residual is
  allocation only: this build keeps `&D_801174C0`, `&D_8011ED04`, `&D_8011ED00` in s5/s3/s6 across the calls;
  retail re-materialises each with `lui` and uses only s0–s4 (the PrevMaxPath parameters) plus s2 for the queue.
  A kept display_enable stops the hoisting but then saves s0–s4 and loses s2 for the queue. Volatile flags, stand-in
  callers with 0–8 live values (before or after display_enable in the file) are inert.
- Best: `display_enable/best.c` (includes the matched loader): `blob_unit … score display_enable --with
  display_enable/best.c --internal PrevMaxPath --internal display_enable --keep standin_caller --block display_enable`
  → `FAIL display_enable: 67 of 79 words differ`, 36 aligned rows (`tools/udiff.py`).
- Next hypothesis: the free callee-saved set of an internal function may come from its real caller's IPA summary;
  retry once `playgame_state_change` has a body. Also try an `unload_overlay(s8 *flag)` helper that umerge does
  inline (a late-folded constant pointer would explain the per-use `lui`, as w2a found for a zero index).

## 3. func_800BFBE8 — MATCH

```
tools/sc.sh cloud/matches/func_800BFBE8.c func_800BFBE8          (-g0 -O3 -mips2 -G 0 -non_shared)
func_800BFBE8:
  MATCH
(-O2: MATCH)
```
Quaternion → rotation matrix. **Arcade ancestor: `CreateQuatMat` (`LIB/fmath.c`)**, pasted verbatim with `f`
literals; matched on the first compile. The earlier codex attempt (A163, 5/96) had flattened the arcade's block-scoped
`xx/yy/zz/xy/wz/…` locals; those blocks are what produce retail's 32-byte frame and stack round-trips.

## 4. difficulty_select — MATCH (-O3 only)

```
tools/sc.sh cloud/matches/difficulty_select.c difficulty_select   (-g0 -O3 -mips2 -G 0 -non_shared)
difficulty_select:
  MATCH
(-O2: 101/103 words differ — saves s0/s1)
```
Recursive path-distance helper (semantics in the header). From B117's native source (25/103 at
`-Wab,-r4300_mul`, which is not the game's flag set): **remove every named local** (each cost a frame slot; retail's
32-byte frame has none) and write the recursive term `delta + recursion - point`. 3 batches, ~10 variants.
Types recovered: `D_80151CE8[]` 80-byte section records (`+2` anchor/loop section in record 0, `+0x2E` total,
`+0x30 s16 first[]`, `+0x38 s16 second[]`); `D_801407F0` path-graph header `{u16 total; …; u8 nbranches @8;
Branch *branches @12}`, Branch 16 bytes `{u8 kind; …; s8 parent @4; u16 end @6; …; u16 count/delta @10;
Vertex *pts @12}` (the same record func_800B9740 walks); `D_8012E5E8[]` 8-byte `{u16 num; …}`.

## 5. func_800E1AA0 — MATCH (own rodata verified)

```
tools/sc.sh cloud/matches/func_800E1AA0.c func_800E1AA0           (-g0 -O3 -mips2 -G 0 -non_shared)
func_800E1AA0:
  MATCH
    own .rodata verified at 0x801243C0..0x801243C4
(-O2: identical output)
```
Car drag/brake term (header has the field semantics). `D_801243C0` = `0x463B8000` = `12000.0f` (=100·120),
written as a natural literal. Closers, from the C31 seed (58/100): (1) int `0` in `f72 < 0` only (the two product
tests keep `0.0f`) — separates the zero webs; (2) `scale = m->f1452; scale += m->f980 * 0.5f;` as two statements —
loads f1452 straight into f0 and fixes the entire FP ring (39 → 0 words). ~12 variants.

## 6. func_800B9740 — 61 aligned rows off (stopped at ~55 variants)

Vertex bounding box over the eligible vertices (min/max s16[3] at `D_801407D4`/`D_801407B4`, sentinels ±32767;
vertex `i` eligible if `i < D_801407F0.primary` or it lies inside a branch's point range with `kind == 1`). Body
and control flow are right; the residual is uopt's loop-invariant hoisting: this build hoists `&D_801409E8`,
the stride constant (`6` or `3`, a second copy) and `&D_801407F0` into s-registers (frame 16, s0–s2), where retail
hoists only `6` (s0, for `count * 6`), expands `i * 6` to shifts and re-loads `D_801409E8` with `lui` at each use
(frame 8). Established on the way:
- primary count and loop bound must be read in place (`i < D_801527A4`, `i < D_801407F0.primary_count`): caching
  either in a local gives the LFTR `bne` exit and costs ~30 rows; read in place gives retail's `slt`/`bnezl` loop.
- the branch scan in retail is `range[j]` indexing (j·16 counter compared with n·16, pointer bumped by 16): written
  that way the inner loop is exact, but the extra pressure then spreads to s3 (worse overall).
- inert: `s16 *` base with `[i*3]`, `(char *)` byte offsets, D_801409E8 as a field at `+0x1F8` of D_801407F0
  (0x801407F0+0x1F8 = 0x801409E8 — worth keeping in mind for the type model), unsigned/while/do-while loop forms,
  w2a's "late-folded zero index" lever (`base = 0`), `-O2` (86/102).
- Next hypothesis: a deleted static helper per vertex (e.g. `vertex_eligible(i)` and `extend_bounds(v)`), whose
  inlining happens before uopt's hoisting decisions; or the bounds loop written with explicit min/max pointers.

## What generalises

1. **Paste the arcade function first.** `func_800BFBE8` had eaten a codex batch at 5/96; the verbatim arcade
   `CreateQuatMat` (block-scoped locals intact, `f` literals) matched on the first compile.
2. **IPA register-parameter order is set by read counts, not parameter order.** Code-free `if (p) {}` reads in the
   callee reorder its callee-saved parameter registers monotonically; a 12-file grid (0–3 reads per parameter) found
   the retail order in one batch. Probably applies to every register-parameter callee with a permutation residual.
3. **No named locals when the frame is too big** (difficulty_select: 48 → 32 bytes, 25 → 0 words).
4. **Split `x = a + b` into `x = a; x += b;`** to put a field load directly into the destination register — fixed a
   whole FP ring in func_800E1AA0. And mixed int/float zero literals separate zero webs.
5. umerge does not inline a *kept* multi-call function into an internal caller in the unit even when retail did;
   writing the body inline reproduces retail's size and flow (display_enable).
