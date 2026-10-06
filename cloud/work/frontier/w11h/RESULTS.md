# w11h results (wave 11): small real callers that close provisional entries

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w11h` (fresh copy of `base`, then src/blob, include,
tools/cloud, asm/us/blob and blob_matched.lock.json synced from the Pi). Trace toolkit installed there with
`tools/trace/install.sh --reuse wtk`. No permission denials.

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| speed_set | 204 | **MATCH** (group `codex_vsync_a145`, internal) | `-g0 -O3 -mips2 -G 0 -non_shared` | group: `speed_set: MATCH`; unit: `EQUAL speed_set: 51 words (internal, c_group.c)` |
| speed_mode0_wrapper | 88 | **MATCH** (same group) | same | group: `speed_mode0_wrapper: MATCH`; unit: `EQUAL speed_mode0_wrapper: 22 words (kept, c_group.c)` |
| speed_mode1_wrapper | 88 | **MATCH** (same group) | same | group: `speed_mode1_wrapper: MATCH`; unit: `EQUAL speed_mode1_wrapper: 22 words (kept, c_group.c)` |
| continue_prompt | 92 | **MATCH**, own literal verified (same group) | same | group: `continue_prompt: MATCH` / `own .rodata verified at 0x80124280..0x80124284`; unit: `EQUAL continue_prompt: 23 words (kept, c_group.c)` |
| (vsync_wait, already locked) | 148 | MATCH, unchanged | same | unit: `EQUAL vsync_wait: 37 words (kept, c_group.c)` |
| func_800E7D0C | 196 | **MATCH**, own .data verified (group `audio_heap`) | same | group: `func_800E7D0C: MATCH` / `own .data verified at 0x80116488 (4 bytes, all zero; .data is laid out per translation unit)`; unit: `EQUAL func_800E7D0C: 49 words (kept, c_group.c)` |
| func_800E7D04 (locked stub) | 8 | MATCH, now a real deleted static (same group) | same | group `func_800E7D04: MATCH`; unit (`--internal func_800E7D04`): `EQUAL func_800E7D04: 2 words (internal, c_group.c)` |
| func_80097468 (locked stub) | 8 | MATCH, now the real `heap_or_default` (same group) | same | group `func_80097468: MATCH`; unit (`--internal func_80097468`): `EQUAL func_80097468: 2 words (internal, c_group.c)` |
| audio_channel_setup | 332 | 3 words off | same (unit) | `FAIL audio_channel_setup: 3 of 83 words differ` / `locked bodies that differ in this unit: 0` |
| object_bytes23_sum | 72 | 4 words off | same (unit) | `FAIL object_bytes23_sum: 4 of 18 words differ` |
| object_bytes_sum_global | 84 | 11 words off | same (unit) | `FAIL object_bytes_sum_global: 11 of 21 words differ` / `locked bodies that differ in this unit: 0` |
| func_80096130 | 264 | 21 words off (frame and structure exact; colouring tie, force-oracle confirmed) | same (unit) | `FAIL func_80096130: 21 of 66 words differ` / `locked bodies that differ in this unit: 0` |
| func_800D63EC | 324 | 70 words off (allocation strategy) | same (unit) | `FAIL func_800D63EC: 70 of 81 words differ; compiled body is 72 words, target 81` / `locked bodies that differ in this unit: 0` |
| func_800B66B0 / menu_input_process | 152 / 1,280 | not attempted: blocked | - | see below |
| func_800CBF2C | 276 | not attempted: blocked | - | see below |
| display_enable | 316 | not attempted (provisional at best) | - | see below |

## 1. Group `codex_vsync_a145` (supersedes the locked group of the same name) - closes provisional `speed_set`

Dir: `groups/codex_vsync_a145/` (`group.c`, `group.json`). Members: speed_set, vsync_wait, speed_mode0_wrapper,
speed_mode1_wrapper, continue_prompt (all five strict; vsync_wait is the locked group's only member and is kept).
Keep list unchanged from the locked group; speed_set is internal (as in the unit, no override needed).

```
tools/sg.sh groups/codex_vsync_a145          # = score.py group cand/codex_vsync_a145 on the builder
Members:
speed_set:
  MATCH
vsync_wait:
  MATCH
speed_mode0_wrapper:
  MATCH
speed_mode1_wrapper:
  MATCH
continue_prompt:
  MATCH
    own .rodata verified at 0x80124280..0x80124284

python3 -m tools.conveyor.pipeline.blob_unit --tag w11h --jobs 2 score speed_set speed_mode0_wrapper \
    speed_mode1_wrapper continue_prompt vsync_wait --with cloud/work/frontier/w11h/groups/codex_vsync_a145/group.c --neighbours
  EQUAL speed_set: 51 words (internal, c_group.c)
  EQUAL speed_mode0_wrapper: 22 words (kept, c_group.c)
  EQUAL speed_mode1_wrapper: 22 words (kept, c_group.c)
  EQUAL continue_prompt: 23 words (kept, c_group.c)
  EQUAL vsync_wait: 37 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 5/5 equal
```

Source = PR #138's `dot_speed_flags_c9210_20261006/group.c` (speed_set with s32 flag carriers; vsync_wait
unchanged). **What closed it: line layout only.** The three one-call wrappers were written on one line each; as1
then sank the IPA saves of s3/s0 (speed_set clobbers s0/s3 unsaved) below the argument moves (6/22, 6/22,
10/23). Written as normal multi-line functions, all three match. continue_prompt's `D_80124280` extern is now the
natural literal `0.05f` (retail word 0x3D4CCCCD, its own rodata; verified by the scorer).

Integration: `blob_group revert codex_vsync_a145`, copy this dir to `src/blob/groups/codex_vsync_a145/`
(drop "claims"), splice. Remove `speed_set` from `provisional.json`. No unit_overrides entries.

## 2. Group `audio_heap` (supersedes the locked group of the same name) - func_800E7D0C

Dir: `groups/audio_heap/`. Members: the five locked members (audio_helper, func_800E7B44, audio_dma_sync,
audio_task_complete, func_800E7C2C) plus func_800E7D0C, func_800E7D04, func_80097468. Keep list unchanged.

```
tools/sg.sh groups/audio_heap
Members:
audio_helper:
  MATCH
func_800E7B44:
  MATCH
audio_dma_sync:
  MATCH
audio_task_complete:
  MATCH
func_800E7C2C:
  MATCH
func_800E7D0C:
  MATCH
    own .data verified at 0x80116488 (4 bytes, all zero; .data is laid out per translation unit)
func_800E7D04:
  MATCH
func_80097468:
  MATCH

python3 -m tools.conveyor.pipeline.blob_unit --tag w11h --jobs 2 score func_800E7D0C func_800E7D04 func_80097468 \
    audio_helper func_800E7B44 audio_dma_sync audio_task_complete func_800E7C2C \
    --with cloud/work/frontier/w11h/groups/audio_heap/group.c --internal func_800E7D04 --internal func_80097468 --neighbours
  EQUAL func_800E7D0C: 49 words (kept, c_group.c)
  EQUAL func_800E7D04: 2 words (internal, c_group.c)
  EQUAL func_80097468: 2 words (internal, c_group.c)
  EQUAL audio_helper: 57 words (internal, c_group.c)
  EQUAL func_800E7B44: 58 words (internal, c_group.c)
  EQUAL audio_dma_sync: 31 words (kept, c_group.c)
  EQUAL audio_task_complete: 104 words (kept, c_group.c)
  EQUAL func_800E7C2C: 54 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 8/8 equal
```

What closed func_800E7D0C (w1a left it 15/49, "3 scheduling words + flag access"):
- the run-once flag is a **function-local static** `static s8 D_80116488 = 0;` (retail 0x80116488 is read and
  written only by this function, checked in `type_model/refs.json`). An extern, even `volatile`, gets one CSE'd
  address register; the local static gets retail's separate `lui t6`/`lui at`.
- the heap-pointer setup (align `D_8017A640` to 32, store to `D_801527C8`, `end = osMemSize | K0BASE`, flag the
  expansion pak when `end >= 0x80400001`) is the **deleted static `func_800E7D04`** (caller-less stub right before
  func_800E7D0C), inlined. Inline in the body, as1 schedules the `&D_801527C8` lui two words early (5/49);
  as the inlined helper it matches.
- `heap_or_default` (w1a's static) is renamed to its retail stub **`func_80097468`** and made non-static. Same
  words in every caller; as a named function its 2-word stub no longer lands unnamed directly after
  func_800E7D0C in the group object, which score.py counted as `1 extra words (nonzero beyond target length)`.

**Integration needs:**
1. `blob_splice revert func_800E7D04` and `blob_splice revert func_80097468` (both locked as `-O2` singles), then
   `blob_group revert audio_heap`, copy this dir to `src/blob/groups/audio_heap/` (drop "claims"), splice.
2. `src/blob/unit_overrides.json`: `force_internal` and `prefer_definition` (file
   `src/blob/groups/audio_heap/group.c`) for **func_800E7D04** and **func_80097468** (same pattern as
   func_80095CF4). Without `--internal func_800E7D04` the unit keeps the stub and func_800E7D0C does not inline it.
3. Own data: `.data` 0x80116488 is a 4-byte all-zero local static (the open owner decision on all-zero statics
   applies; the scorer reports it verified).

## 3. Near misses (best.c + notes.md in each dir)

- **audio_channel_setup** - 3/83 (`audio_channel_setup/`). Semantics and layouts in notes (sibling of the matched
  func_8010D85C: same Obj/Model/Ent records). Only the as1 address temp of `D_801427C0[f]` differs (retail
  `lui a2; addu a2; lhu a1,%lo(a2)`, ours uses a1). ~25 variants.
- **object_bytes23_sum / object_bytes_sum_global** - 4/18 and 11/21 (`object_bytes/`). Traced: the address web
  takes the lowest caller-saved register not killed by `sound_update_channel` (t2); retail's t4 means uopt believed
  sound_update_channel/func_80096288 kill t2 and t3 in retail. Points at the locked stand-in source of
  func_80096288 (`if(0){switch...} if(c){}`), not at these two bodies.
- **func_80096130** - 21/66 (`func_80096130/`). Frame (72) and homes exact after making the busy wait an inlined
  static. Remaining: one colouring tie (slot address web splits at totalsave == bestcost 3.0); forcing
  `p1:w5=c2,p1:w7=c6` leaves 9 rows from a single as1 placement of `sw zero,12(v1)`.
- **func_800D63EC** - 70/81 (`func_800D63EC/`). Retail keeps everything in caller-saved registers with
  save/restore around calls; ours gives the packet s0 (callee cost 4.2 < caller cost 6.2). Needs the source of a
  higher callee-saved cost; ~7 variants.

## 4. Not attempted

- **func_800B66B0 via menu_input_process:** `frontier show menu_input_process` lists `blockers: audio_doppler_calc`
  (unmatched, 1,124 bytes) and `preserved: a2, a3, t1..t5` across its callees, so its IPA context cannot be exact
  until audio_doppler_calc matches; PR #163's 319/320 is consistent with that. Only draft: the PR #163 group
  (`integrate-163:cloud/matches/dot_text_measure_b66b0_20261006/`, caller from codex_defaults_a16).
- **func_800CBF2C:** no w11a file exists for it, but `frontier show` lists it as a `unit` function with register
  parameters s0/s1/s3 from its caller and blockers draw_ui_element, drone_set_catchup and menu_back (all
  unmatched), so a strict result is not possible this wave.
- **display_enable:** unsaved s0-s4 (internal); its real caller playgame_state_change is unmatched, so the best
  possible result is provisional. Skipped for the strict work above.

## What generalises

1. **One-line function bodies change as1 scheduling.** `void f(...) { g(...); }` on one line made as1 sink the
   IPA callee-save stores below the argument moves; the same body on separate lines matched (3 functions).
2. **A run-once flag that retail loads and stores with separate `lui`s is a function-local static**, not an
   extern (volatile does not help: uopt still CSEs the address). Check `refs.json` that only one function uses it.
3. **A deleted static's stub can be a score.py artefact.** A static inlined everywhere still emits a `jr ra; nop`
   stub with no symbol; if ugen places it directly after a member, score.py reports `1 extra words (nonzero beyond
   target length)` although the unit is EQUAL. Name the static after its retail stub (non-static, internal via
   overrides) and the extra word goes away.
4. **Caller-saved register choice for values kept across an internal call follows the callee's believed kill
   set** (`available0` in the p1dec record). A t2-vs-t4 style residual means the callee's uopt summary differs,
   not the caller's source.
