# Wave 9 — agent w9b

Assignment: the `AdjustSpeed` group (0x800A2680, 784 B) and the `camera_target_track` group (0x800AED64, 636 B).
All flags `-g0 -O3 -mips2 -G 0 -non_shared`. Builder scratch: `watchman2:~/rush2049/scratch/frontier/w9b`.

| Function | State | Flags | Scorer output |
|---|---|---|---|
| `camera_target_track` | **strict MATCH** (group, real partners, no stand-ins); unit EQUAL, 0 locked neighbours broken | `-O3` | `score.py group cand/camera_target_track` → `camera_target_track:\n  MATCH` (members func_80092278, entity_flags_apply, high_scores_display also `MATCH`) |
| `AdjustSpeed` | near-miss: 92 aligned rows, all callee-saved register colouring; frame and all named stack slots equal | `-O3` | `blob_unit --tag w9b score AdjustSpeed --with …/AdjustSpeed/best.c` → `FAIL AdjustSpeed: 165 of 196 words differ; compiled body is 195 words, target 196` |

## camera_target_track — strict MATCH (group extension)

Deliverable: `groups/camera_target_track/` (`group.json` with `"claims": ["camera_target_track"]`, `group.c`).
It is the locked group `src/blob/groups/frontier_list_alloc_sound` plus camera_target_track, with
`Node68.p20` changed from `u32` to `s32`. The locked members' words do not change (they score MATCH here, and the
unit run reports 0 locked bodies differing).

```
ssh watchman2 'cd ~/rush2049/scratch/frontier/w9b && IDO_DIR=…/ido python3 tools/cloud/score.py group cand/camera_target_track'
Members:
func_80092278:
  MATCH
entity_flags_apply:
  MATCH
high_scores_display:
  MATCH
camera_target_track:
  MATCH
Context (informational; excluded from exit status):
func_8009211C:
  MATCH
func_80091FBC:
  MATCH

python3 -m tools.conveyor.pipeline.blob_unit --tag w9b score camera_target_track --with cloud/work/frontier/w9b/groups/camera_target_track/group.c --neighbours
  EQUAL camera_target_track: 159 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
```

It is kept (ABI arguments, homed float arguments), so its unmatched callers (camera_look_at_point, func_800E0050)
cannot change its words. This is **not** provisional.

**What closed it.** I started from w6d/w7d's source (12 rows; the constant `1` coloured a0 where retail has t1).
Changing only `Cam.queued` (+9) from `u8` to `signed char` gives EQUAL. Trace (`tools/ctrace.sh`, ordinal 119): the
s32 `1` web for p20/state (w101, dtype 6) lived only in block 21, and the u8 `1` webs were a separate dtype-8 web.
With a signed byte, the `queued = 1` store joins the s32 web. That web is then live out of the block that ends at
the first `osJamMesg`, so it interferes with the a0–a2 argument set-up and with the result in t0, and gets t1.
After the calls the allocator rematerialises `li 1` for queued, as retail does. The u8 `live` keeps its own local
`li 1` in t8. w7d's analysis ("v0–a3 and t0 forbidden") was right. The extra interference comes from a later use
of the same-typed constant, not from the func_80092278 call.

Quirks, also in the header comment: `Node68.p20` is `s32`; `Cam.queued` is `signed char`; `char buf[4]` is an
unused local that supplies 4 frame bytes (result home at sp+32; w6d's finding, still needed: without it the build
is 2 words off).

Semantics, from w6d: a positioned variant of high_scores_display. Under the D_80142728 lock it pops an emitter
from D_801461B0, stores pos/volume/clamped x,y/1.0f, allocates a command and a node, links them, posts the command,
re-locks, inserts the emitter into D_801461E8 unless it is active, marks it queued, and returns the node tag.

## AdjustSpeed — near-miss, colouring lane (best: `AdjustSpeed/best.c`)

Semantics (header of `best.c`): a Controller Pak file delete request. Under the pak lock it calls 0x8000A700
(label `osPfsRename`, but it takes 5 args, so it is presumably `osPfsDeleteFile`) on the port's 0x28-byte file
record. On error it maps the status (func_800A1E94), unlocks, and calls the error hook `D_80144008(port, status, 0,
0, &retry, &state, func_8008A6A4)` with `D_8011EAE8` = port. Then it checks the node is still pending, re-locks,
and retries while `retry` is set. On success it frees blocks, calls `request->done`, clears the file state, frees
the request buffer under the heap lock, moves the node between lists and unlocks.

Recovered layout: `D_80144030[]` is 0x304-byte port records: `+0x0C` pfs, `+0x74` status, `+0x78` free-block
count, file records at `+0x8C + idx*0x28` (`+0` state, `+4` game_code, `+8` u16 company, `+0xA` ext_name,
`+0xE` game_name). The request has `+0` next link, `+8` done callback, `+0x10/+0x11` port/index bytes, and `+0x48`
buffer. `D_80144D60[]` is 16-byte lists with the head at +8.

What is right:
- Inlining is reproduced in the whole-program unit (`umerge inlined into it: audio_effect_process,
  func_8008A704, sync_release_video`). The lock is inlined twice and each copy has its own message slot (196/172),
  as in retail. The unlock is `sync_release_video()`, which costs the same as a direct static.
- The port address is not CSE'd with the file address only when the port fields are written as
  `D_80144030[channel].x` inside the loop. With no `port` variable, uopt hoists them separately, as retail does.
- `node == current` operand order and the `for (; cur; cur = …) if (node == cur) break;` loop shape.
- Frame 232 and every named slot (retry 215, request 208, msg 196/172) equal retail. Four scalars must be declared
  before `retry`/`status`/`request`. That leaves 24 bytes, i.e. three inlined calls placed after the second lock.
  `best.c` supplies them as three empty `func_800A2678()` calls at the end: **a hypothesis** (the adjacent
  caller-less stubs func_800A2670/func_800A2678 suggest deleted statics), and it fixes only the frame.

Residual (traced uopt, `tools/ctrace.sh`, ordinal 169): retail assigns list s0, channel s1, node s2, queue s3, port
s4, file s5, &D_8011EAE8 s6, &D_8011194C s7 and pfs s8. The callback constant `func_8008A6A4` loses every
callee-saved register: it is held in t0 across func_800A1E94 (IPA: t0 preserved) and spilled at sp+96 around
osJamMesg. Ours colours the callback constant (w38, save 10/2) first, so it gets s0, and pfs is split and spilled.
Forcing the nine s-registers to the retail set leaves 56 rows, because the pfs web is declined s8 (an ext_name web
takes it). So the residual is the relative priority of the callback constant (must fall below pfs) and of
&D_8011EAE8 (must fall below file). Tried with no gain: a callback local assigned at the top or in the error block,
a `pfs` local, a `port` local, pointer-arithmetic indexing, `while`/`do`/goto loop forms, and declaration orders.
About 25 variants in all.

Best next hypothesis: the callback constant has low priority in retail because its web spans more blocks. That
could happen if the error report were an inlined static taking the callback as a parameter and called from more
than one place, or if `func_8008A6A4` were also used elsewhere in the function, e.g. a compiled-out debug use. Try
the error-hook call as a static helper, and look at the sibling `drone_set_catchup` (0x800A2990, unmatched), which
has the same error path with the callback constant in s0.

## Provisional entries (`speed_set`, `camera_track_spline`)

Neither result supplies a real caller that a provisional entry was waiting on, so nothing was re-proved:
- `speed_set` waits on speed_mode0_wrapper, speed_mode1_wrapper and continue_prompt. AdjustSpeed calls none of
  them (its callers are draw_ui_element, drone_set_catchup and track_process_main).
- `camera_track_spline` waits on camera_update. camera_target_track was one of camera_update's `unit_blockers`
  (through camera_look_at_point). With this match, camera_update's unit blocker list shrinks to camera_free_look.
  camera_look_at_point's and func_800E0050's only remaining blocker was camera_target_track.

## Tools (this directory)

- `run.sh FN cand.c` runs a unit score plus a frame/row summary. `udiff.py` gives an aligned diff of the unit
  object (copy of w2c's).
- `tools/ctrace.sh`, `pdiff.sh`, `force.sh`, `tr.sh` and `sum.sh` are w3a's traced-uopt scripts retargeted to tag
  `w9b`. The instrumented uopt binary was copied into my builder scratch.
- I did not run workbench `diagnose`: it needs a target object, and the traced allocator already names the lane.

## What generalises

1. **A constant web's colour can be set by a later store of the same constant, through the field's signedness.**
   IDO keys constant webs by dtype. A `1` stored to a `signed char` field joins the s32 `1` web, while a `1` stored
   to a `u8` field forms its own unsigned web. Joining makes the web live across intervening calls, which changes
   which registers it may take (camera_target_track: a0 → t1). When retail's constant colour needs an
   unexplained forbidden set, look for a later same-value store and check the signedness of that field.
2. **Inlined calls each reserve 8 frame bytes in call order.** Even an empty inlined function does this. Measuring
   which named slots shift tells you where in the source the missing inlined calls sit (AdjustSpeed: after the
   second lock).
3. Recomputed (non-CSE'd) record addresses in retail mean the fields are written as `array[i].field` inside a
   loop and hoisted separately, not through a pointer local.
