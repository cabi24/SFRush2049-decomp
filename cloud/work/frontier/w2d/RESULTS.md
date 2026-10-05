# Frontier wave 2 — w2d results

Agent `w2d`. Two sessions: the first agent was cut off before writing this file; the second took over on
2026-10-05, refreshed the builder copy from `~/rush2049/scratch/frontier/base` (it had 581 `src/blob` files; base has 594),
re-scored everything below against the current lock, and continued.

Builder: `watchman2:~/rush2049/scratch/frontier/w2d`. `sc.sh`, `grp.sh` and the rest are thin wrappers around the
brief's commands: `sc.sh F NAME --flags …` = `score.py fn cand/NAME.c NAME --flags …`; `grp.sh DIR` =
`score.py group cand/<DIR>`. Unit checks use `python3 -m tools.conveyor.pipeline.blob_unit --tag w2d score … --with …`
on the Pi.

| # | Function | Bytes | State | Deliverable |
|---|---|---:|---|---|
| 1 | `func_800B24EC` | 364 | **MATCH** (-O3, also -O2) | `cloud/matches/func_800B24EC.c` |
| 2 | `AdjustSpeed` | 784 | 164/196 words differ in the unit (96 aligned rows); register-priority residual | `AdjustSpeed/best.c` (+ `k.c`) |
| 3 | `menu_save_options` | 768 | **MATCH** in a real group (with `func_800A44E8`, `func_800A370C`) | `groups/menu_save_options/` |
| 4 | `reverb_setup` | 528 | code identical; own jump tables unverified by the scorer. Group also strictly matches 3 stubs | `groups/reverb_setup/` |
| 5 | `func_800D169C` | 556 | code identical; own float `1e20` unverified (bits checked by hand). Group also strictly matches stub `func_800D18C8` | `groups/func_800D169C/` |
| 6 | `func_800BE4F0` | 436 | **MATCH** (-O3, also -O2) | `cloud/matches/func_800BE4F0.c` |

Four of the six can be spliced now (1, 6, and groups 3–5: the splice checks the own-rodata bytes for 4 and 5).
AdjustSpeed is still open.

---

## 1. func_800B24EC — MATCH

```
sc.sh cloud/matches/func_800B24EC.c func_800B24EC --flags '-g0 -O3 -mips2 -G 0 -non_shared'
func_800B24EC:
  MATCH
blob_unit --tag w2d score func_800B24EC --with cloud/matches/func_800B24EC.c
  EQUAL func_800B24EC: 91 words (kept, c_func_800B24EC.c)
blob_unit score: 1/1 equal
```
Looks up a name across a range of sorted 36-byte-record tables (`D_80151AE8[i] = {base, count}`). `entity_name_copy`
is libc `bsearch`, `func_800A473C` is `strcpy`, `pointer_compare_thunk` is the comparator. Quirks: `volatile u8 D_80140BDC`
(read address-form twice), `i` declared before the key buffer (buffer at sp+64), `continue` rather than a nested if.
It unblocks `camera_shake_start`, `collision_sound_play` and `physics_collision_test` (it is their only blocker).

## 6. func_800BE4F0 — MATCH

```
sc.sh cloud/matches/func_800BE4F0.c func_800BE4F0 --flags '-g0 -O3 -mips2 -G 0 -non_shared'
func_800BE4F0:
  MATCH
(same with -O2: MATCH)
blob_unit --tag w2d score func_800BE4F0 --with cloud/matches/func_800BE4F0.c
  EQUAL func_800BE4F0: 109 words (kept, c_func_800BE4F0.c)
```
This is `strcat` for the game's two string encodings. A string is either plain bytes, or "wide": a leading 255, then
2-byte characters ending in a 0,0 pair. The function after it, `func_800BE6A4`, is the matching `strcpy`. Plain + plain
is an ordinary strcat. Otherwise a plain destination is first converted in place (copied to `u8 buf[256]`, then written
back as 255 followed by 0,c pairs) and the source is appended as pairs.

How it closed (about 150 variants across both sessions; the first session's ~90 stayed at 84+ words off):
- Retail keeps the return value in `v1`, separate from `destination`, and resets it (`move v1,a0`) after the buffer
  copy. Getting this right needs a separate `ret` local that is also the copy pointer, then `ret = destination;`. On its
  own IDO copy-propagates `ret` back into `destination` and the body drops to 108 words. The fix is
  `*destination++ = 255; out = destination;`: modifying `destination` after the reset blocks the propagation
  (109 words, 10 off).
- The last 5 words were the register for the reloads of `*source` (retail `t1`, ours `a0`). Writing the loop tests as
  truth tests (`while (*in)`, `while (*out)`) instead of `!= 0` fixed it. The rule that carries over: for a `u8`
  load, `x != 0` and `x` build different expression webs.

## 3. menu_save_options — MATCH (group)

```
grp.sh groups/menu_save_options          (score.py group, flags -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul)
Members:
func_800CD058:  MATCH
func_800CCEFC:  MATCH
func_800CCCCC:  MATCH
menu_save_options:  MATCH
func_800A44E8:  MATCH
func_800A370C:  MATCH
Context (informational): func_800CD104 253/272 differ, draw_number MATCH, draw_speedometer 417/426 differ
blob_unit --tag w2d score menu_save_options func_800CCCCC func_800A44E8 func_800A370C func_800CD058 func_800CCEFC \
    --with groups/menu_save_options/save.c
  EQUAL all 6 (func_800CCCCC/func_800CD058/func_800CCEFC from the locked menu_cc_20261004 group.c)
blob_unit score: 6/6 equal
```
The group replaces `src/blob/groups/menu_cc_20261004`. Its `group.c` is byte-identical to that group's file. It also
replaces the single locks of `func_800A44E8` and `func_800A370C`. Claims: `menu_save_options`, `func_800A44E8`,
`func_800A370C`. The function initialises the save-slot table: it binds 4 slots, resets each through the internal
`func_800CCCCC` (handle in `s5`) and names it with sprintf; it initialises two list heads; and it allocates
`(D_80156994 ? 12 : 1) * 3` 44-byte records plus a pointer index. Shaping facts:
- `func_800A44E8` is inlined twice and itself inlines `func_800A370C`. That nesting gives the 160-byte frame and the
  retail store order.
- `D_80117428` and `D_80117424` must be `u8`.

The `context` entries still fail as before; they are informational.

## 4. reverb_setup — code identical, own rodata unverified (group)

```
grp.sh groups/reverb_setup
func_800B4AE8:  MATCH
func_800B4AF0:  MATCH
func_800B4AF8:  MATCH
func_800B4B00:  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x24, .rodata+0x0 at +0x2c)
reverb_setup:   MATCH (4 section-relative relocations unverified: .rodata+0x2c at +0xb4, .rodata+0x2c at +0xbc, .rodata+0x74 at +0x144, .rodata+0x74 at +0x14c)
blob_unit --tag w2d score reverb_setup func_800B4B00 func_800B4AE8 func_800B4AF0 func_800B4AF8 \
    --internal func_800B4B00 --internal func_800B4AE8 --internal func_800B4AF0 --internal func_800B4AF8 \
    --with groups/reverb_setup/reverb.c
  EQUAL reverb_setup: 132 words; EQUAL func_800B4B00: 37 words (internal); EQUAL the three stubs (internal)
blob_unit score: 5/5 equal
```
This is an option-byte getter for a save slot. The page `D_8014978C` selects a field block. The three caller-less stubs
`func_800B4AE8/AF0/AF8` are deleted getters inlined here; which stub is which getter is an assumption. `func_800B4B00`
gets its real 3-argument signature (internal; the page goes to the a2 home slot). This replaces
`src/blob/groups/codex_switch_a26` and the empty single locks of the three stubs. The jump tables are at
`0x80123CE4` (func_800B4B00) and `0x80123D10` / `0x80123D58` (reverb_setup), and the splice verifies them.
`audio_bus_mix` (0x800B4818) is the matching setter and has the same three-stub pattern at 0x800B4720..30.

## 5. func_800D169C — code identical, own literal unverified (group)

```
grp.sh groups/func_800D169C
func_800D169C:  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x18, .rodata+0x0 at +0x30)
func_800D18C8:  MATCH
blob_unit --tag w2d score func_800D169C func_800D18C8 --internal func_800D18C8 --with groups/func_800D169C/path.c
  EQUAL func_800D169C: 139 words (kept); EQUAL func_800D18C8: 2 words (internal)
blob_unit score: 2/2 equal
```
(Without `--internal func_800D18C8` the unit compiles the stub as a 9-word body: the stub must stay internal.)
The function finds the path point nearest car 0, steps back to the even segment start, and raises the high-water mark
`D_8014AA14` through the inlined `func_800D18C8`. A related routine is the closest-point scan in arcade
`game/resurrect.c` (`c_dist = 1e20`). Own literal: `1e20f` = `0x60AD78EC`, which equals the retail word at
`0x80124174` (checked). Integrator notes:
- `path.c` **defines** `Car D_80152818[6]` rather than declaring it `extern`. That is what makes as1 share the
  `lui` (precedent: `D_801551E8` in `codex_func_800B7438`). Check that the splice handles a data definition in a
  group file the same way it did for that precedent.
- `func_800D18C8`'s empty single lock is superseded.

## 2. AdjustSpeed — open, register-allocation residual

The best result in the unit is `AdjustSpeed/best.c`:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w2d score AdjustSpeed --internal func_800960CC \
    --internal func_800A2670 --internal func_800A2678 --with cloud/work/frontier/w2d/AdjustSpeed/best.c
  FAIL AdjustSpeed: 164 of 196 words differ; compiled body is 195 words, target 196
       umerge inlined into it: audio_effect_process, func_8008A704, func_800A2670, func_800A2678, sync_release_video
  EQUAL func_800A2670 / func_800A2678 (internal)
```
`AdjustSpeed/k.c`, which adds a `pfs` local, is 196 words long and scores `123 of 196 words differ`.

What it is: rename a controller-pak file with a retry loop (`osPfsRename`). On error it calls `func_800A1E94` and the
`D_80144008` error callback (`&retry`, `&status`, `func_8008A6A4`), then checks that the node is still queued in
`D_80144D60[port]`. On success it calls `osPfsFreeBlocks`, `done(node)`, `audio_effect_process(buffer)` and moves the
node to `D_801460E0`. Structure, frame (232), stack slots and the inline set are all right. Retail has stubs at
0x800A2670 and 0x800A2678; best.c reads them as the `osPfsFreeBlocks` wrapper and the move-node helper, which is an
assumption.

**Residual lane: callee-saved allocation priority.** The function has 10 candidate webs for 9 callee-saved registers.

| Version | Who loses (no register) |
|---|---|
| retail | the `&func_8008A6A4` callback constant, held in `t0` and spilled to 96(sp); `game_name`/`ext_name` pointers spilled to 84/88 |
| best.c | the `controller->pfs` web, spilled to 80(sp) |
| k.c | `&D_8011194C`, rematerialised with `lui at` |

Retail assignment: s0 list, s1 port, s2 node, s3 queue, s4 controller, s5 file, s6 &D_8011EAE8, s7 &D_8011194C, s8 pfs.

Tried this session (about 25 variants, on top of the first session's ~60):
- an inlined error-handler helper taking the callback (and the list) as parameters. The frame grows by 16–24 bytes, so
  it is not that;
- `cb` as a named local at the top, before the loop, or on the error path;
- `&func_8008A6A4`;
- dead reads of `D_8011194C` (top, in the loop, 1–4 copies);
- `if (pfs) {}` ×0/1/2;
- truth tests vs `== 0` in every comparison (best and k, up to 2 toggles each).

None moved the loser. The previous session tried `if (node) {}` ladders (10–50), comma-expression error calls, and
helpers `func_800A266C` (x1–x3) taking the callback.

Best next hypothesis: run `workbench diagnose` with a retail target `.o`, then work lever 19 (callee-saved tie-break,
"force the smallest causal web set"). The goal is to make the callback web lose to `pfs`. One untested idea: the
retail callback web may be longer because the original code passes the callback through a variable that is live
across the whole retry loop (assigned before `for(;;)`), which IDO then splits. A `cb` assigned once before the loop
did not move it, but a `cb` that is also used after the loop has not been tried.

## Files

- `cloud/matches/func_800B24EC.c`, `cloud/matches/func_800BE4F0.c` — strict matches.
- `groups/menu_save_options/`, `groups/reverb_setup/`, `groups/func_800D169C/` — group dirs with `"claims"`.
- `func_800BE4F0/best.c` (= the match), `func_800BE4F0/shape_wrong_semantics.c` and `reset_present.c` (intermediate
  steps, kept for the record).
- `AdjustSpeed/best.c`, `AdjustSpeed/k.c` — open.
- Helper scripts in this directory: `sc.sh`, `grp.sh`, `full.sh`, `batch.sh`, `us.sh`, `usa.sh`, `usb.sh`, `um.sh`,
  `bscore.py`, `full.py`, `objdiff.py`.

## What generalises

1. **`x != 0` vs `x` on a `u8` load changes which register the reload gets.** Try it whenever the residual is a
   single reloaded value colouring differently.
2. **Return-value copies.** If retail keeps the returned pointer in its own register (`move v1,a0` at entry and again
   later), the source has a separate `ret` that IDO could not copy-propagate. The parameter must be modified somewhere
   after the last `ret = param`.
3. **A deleted-static helper that takes parameters costs frame bytes** (16–24 per inline). If the frame already
   matches, there is no extra helper.
