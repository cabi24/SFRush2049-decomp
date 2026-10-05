# Frontier wave 4 — agent w4d results (2026-10-05)

Builder scratch `watchman2:~/rush2049/scratch/frontier/w4d` (copied from `base`), unit tag `w4d` (and `w4d2`
for one parallel run). Nothing committed or spliced. Helpers in `tools/` (w4d copies of the w3a scripts:
`sc.sh`, `grp.sh`, `us.sh`, `ctrace.sh NAME cand LABEL PROC|- [blob_unit args]`, `pdiff.sh`, `force.sh`,
and `var.py`, a replacement-spec variant runner that blob_unit-scores each variant).

| Function | Bytes | State | Flags | Deliverable |
|---|---:|---|---|---|
| `collision_sound_play` | 216 | **strict MATCH** (+ callee prototype fix) | -O3 (also -O2) | `cloud/matches/collision_sound_play.c`, `collision_sound_play/func_800B24EC.c` |
| `camera_process_input` | 1,020 | near-miss, 63/255 in the unit | -O3 | `camera_process_input/best.c` |
| `func_80096130` | 264 | near-miss, 15/66 in the unit (caller `wheel_setup_initial` 28/31) | -O3 | `func_80096130/best.c` |
| `tournament_trophy_award` | 388 | **strict MATCH (group)** | -O3 | `groups/championship_standings/` (claims it) |
| `func_8010C2E4` | 356 | **strict MATCH** | -O3 (also -O2) | `cloud/matches/func_8010C2E4.c` |
| `sfx_volume_set` | 256 | **strict MATCH** | -O3 (also -O2) | `cloud/matches/sfx_volume_set.c` |
| `func_800CCB40` | 396 | strict MATCH + own .rodata verified, **held for owner decision** (disputed device) | -O3 | `func_800CCB40/best.c` |
| `func_800C1A00` | 352 | **strict MATCH** | -O3 (also -O2) | `cloud/matches/func_800C1A00.c` |

Strict: 5 functions (1,568 bytes) + 1 held (396 bytes). Near-miss: 2.

All scorer commands run from the repo root on the Pi (`T=cloud/work/frontier/w4d/tools`); `sc.sh` wraps
`score.py fn ... --flags "-g0 -O3 -mips2 -G 0 -non_shared"` in the w4d scratch, `grp.sh` wraps `score.py group`.

Whole-unit check of everything together:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4d score collision_sound_play func_800B24EC camera_shake_start \
  func_8010C2E4 sfx_volume_set func_800C1A00 --with cloud/work/frontier/w4d/collision_sound_play/func_800B24EC.c \
  --with cloud/matches/collision_sound_play.c --with cloud/matches/func_8010C2E4.c \
  --with cloud/matches/sfx_volume_set.c --with cloud/matches/func_800C1A00.c --neighbours
  EQUAL collision_sound_play: 54 words (kept, c_collision_sound_play.c)
  EQUAL func_800B24EC: 91 words (kept, c_func_800B24EC.c)
  EQUAL camera_shake_start: 186 words (kept, s_camera_shake_start.c)
  EQUAL func_8010C2E4: 89 words (kept, c_func_8010C2E4.c)
  EQUAL sfx_volume_set: 64 words (kept, c_sfx_volume_set.c)
  EQUAL func_800C1A00: 88 words (kept, c_func_800C1A00.c)
  locked bodies that differ in this unit: 0
blob_unit score: 6/6 equal; object build/blob_unit/w4d/unit.o (9.0s)
```

---

## collision_sound_play — strict MATCH; the five-argument question is resolved

**The arcade ancestor settles the prototype.** `collision_sound_play` is the N64 `InitBlit`
(`reference/repos/rushtherock/LIB/blit.c`): `ti = MBOX_FindTexture_Err(blit->Name, &blit->TexIndex, MBOX_WARN)`.
The arcade MathBox library binary `MB/zmb.a` (`mb_model.o`, objdump of the archive member) shows the real
contract:

- `MBOX_FindTexture_Sub(name, &index, lo, hi, err)` — **five formals**; the fifth is loaded from `104(sp)`
  (the caller's 5th-argument slot) and only compared against 1 (`MBOX_WARN`) and 2 (`MBOX_FATAL`) to choose a
  not-found message.
- `MBOX_FindTexture_Err(name, idx, err)` = `Sub(name, idx, 0, MBOX_NumTexTables - 1, err)` (stores `a2` to `16(sp)`).
- `MBOX_FindTexture(name, idx)` = `Sub(name, idx, 0, n - 1, 0)` (stores `zero` to `16(sp)`).

So `func_800B24EC` **is `MBOX_FindTexture_Sub` with five formals**; on N64 the messages are compiled out, so
`err` is never read and the callee's bytes are identical with or without it. The two wrappers are ~1 statement
and umerge inlined them, which is why callers set up `0, count-1, 1` (collision, `camera_shake_start`) or
`..., 0` (`sfx_position_3d`) themselves. `MBOX_WARN = 1`, `MBOX_NOERR = 0` (`MB/mb_util.h`).

Deliverables:
- `cloud/matches/collision_sound_play.c` — `InitBlit` with a full five-formal prototype
  `TexDef *func_800B24EC(char *, s16 *, s8, s8, s32 err)`, passing `MBOX_WARN`. No unprototyped declaration.
- `cloud/work/frontier/w4d/collision_sound_play/func_800B24EC.c` — **proposed replacement for
  `src/blob/func_800B24EC.c`** (and `cloud/matches/func_800B24EC.c`): same body plus the fifth formal `s32 err`
  and the arcade's (compiled-out) `if (err == MBOX_WARN) ... else if (err == MBOX_FATAL) ...` message branch,
  header naming the ancestor. Byte-identical. `camera_shake_start` (locked) already declares the 5-argument form.

```
$T/sc.sh cloud/matches/collision_sound_play.c collision_sound_play          -> collision_sound_play:  MATCH
FLAGS="-g0 -O2 ..." $T/sc.sh ... collision_sound_play                        -> MATCH
$T/sc.sh cloud/work/frontier/w4d/collision_sound_play/func_800B24EC.c func_800B24EC -> func_800B24EC:  MATCH
blob_unit ... score collision_sound_play func_800B24EC camera_shake_start (with both files) --neighbours
  -> 3/3 EQUAL, locked bodies that differ: 0
blob_unit ... score collision_sound_play func_800B24EC --with cloud/matches/collision_sound_play.c --neighbours
  -> 2/2 EQUAL (locked 4-formal callee), 0 differ
```
Integration: splice `collision_sound_play` as a single; replace the callee's source with the 5-formal file
(bytes unchanged) so the caller/callee declarations agree. This unblocks `func_800EF5B0` (sole blocker) and
46 dependents.

Recovered layout (N64 `Blit`): `0 name`, `4 image` (`D_80110664` when name is NULL), `8 TexDef *info`,
`0xC s16 texIndex`, `0x12 u16 state`, `0x14/0x16 u16 width/height`, `0x18 u8 alpha`, `0x1C..0x24` five s16
crop fields (arcade Top/Bot/Left/Right/color). `TexDef` 36 bytes: `name[16]`, `u16 width, height` at 0x10.

## tournament_trophy_award — strict MATCH as a group member

Password encoder: appends the checksum bits after the payload in the bit buffer `D_8012E618`, then emits one
character of the alphabet `D_80116FE4` per 5 bits into `out`, NUL-terminated. Register-preserving callee
`func_800DC120` (keeps `t4` across the call), so it needs the group.

Group `cloud/work/frontier/w4d/groups/championship_standings/` **supersedes** `src/blob/groups/championship_standings`:
same members plus `tournament_trophy_award` (claims: `tournament_trophy_award`); the old
`__standin_func_800DC120` stand-in is **removed** (the two real callers keep `func_800DC120` out of line).
```
$T/grp.sh cloud/work/frontier/w4d/groups/championship_standings
Members:
func_800DC120:  MATCH
championship_standings:  MATCH
tournament_trophy_award:  MATCH
Context (informational; excluded from exit status):
func_800DC1AC:  MATCH
blob_unit --tag w4d score tournament_trophy_award championship_standings func_800DC120 --with .../group.c --neighbours
  EQUAL tournament_trophy_award: 97 words (kept) / championship_standings: 108 (kept) / func_800DC120: 35 (internal)
  locked bodies that differ in this unit: 0
```
What closed it from 40/97 (old w2c context copy): `i` is the bit position in **both** loops (one variable =
one register: retail shares a1); the first loop's counter `j` is its own variable (sharing `n` let uopt skip
re-zeroing `n` on the empty-loop path); `j++, i++, sum >>= 1` in the for-header in that order (temp ring);
`i = 0;` stated before an empty for-init.

## func_8010C2E4 — strict MATCH (the frontier's group/ring label is a false positive)

Callback (no direct callers, four arguments, last two unused and homed). For player `*player` (records
`D_8014A250[]`, **0x808** bytes in this use — the retail index arithmetic is `<<8, +, <<3`), if byte 0x640 is 0
and s16 0x6C4 is negative, it reads the 24-byte polygon records `D_801497F8[]` under the four wheel polygon
indices (0x5A0..0x5A6); if any has flag 0x20 and 5-bit id (bits 11–15) equal to the camera scene id, sets scene
flag bit `24 + player` (plus 0x200 when no player bit is set yet or 0x4000 is set), else clears it. Returns 0.
```
$T/sc.sh cloud/matches/func_8010C2E4.c func_8010C2E4   -> func_8010C2E4:  MATCH   (also MATCH at -O2)
blob_unit ... score func_8010C2E4 --with cloud/matches/func_8010C2E4.c --neighbours -> EQUAL, 0 differ
```
Closed with the traced allocator: from 13 rows off, `ctrace` showed `ctl` (save 2.0) coloured `a0` before the
camera parameter (save 0.67), pushing the parameter to `a2` and homing `a0`. `force.sh "p2:w20=c4,p2:w26=c5"`
(ctl→a1, sc→a2) reproduced retail exactly; the natural source with that priority is an empty
`if (cam == NULL) { DEBUG_PRINT(...); }` (compiled-out message) before `ctl = cam->ctl`. Also needed: `idx` as a
variable (kept in v0 for the shift) and `id = sc->id; id <<= 11;`.

## sfx_volume_set — strict MATCH

Car-part object lookup: for each part suffix `D_8011B42C[j]` ("FRAME1", "HOOD", "SHEEN") formats
`"%s%s"` (`.data` 0x801233B8) with the car prefix `D_80110D3C[car]` ("CAR1"…) and stores
`string_copy_format(name, model, model, MBOX_WARN)` — i.e. `MBOX_FindObject_Sub` — into `D_801427C0[slot*3 + j]`.
**Arcade ancestor:** the car-part loop of `InitDynamicObjs` (`game/visuals.c`), whose declaration list
(`S32 i, j, k, ...; char partName[20];`) gave the frame.
```
$T/sc.sh cloud/matches/sfx_volume_set.c sfx_volume_set   -> sfx_volume_set:  MATCH   (also MATCH at -O2)
blob_unit ... --neighbours -> EQUAL, 0 differ
```
Quirks (in the header): unused `i`; `char partName[20]` (16 gives the old 88-byte frame; ≤16 also flips an
`as1` delay-slot choice); `k` declared after the buffer; `++j, ++k` in the for-header; `(s32)model` call
arguments (an int web for the model, narrowed at each call). A deleted-helper experiment
(`func_800B2820(name, (s32)model)`, the neighbouring stub) also fixed registers but added 8 bytes of frame; the
cast alone does it without the helper, so the stub `func_800B2820` is **not** shown to belong here. The format
string is referenced by its data name (a literal is placed in `.rodata` by the scorer's compile, retail has it
in `.data`), following `menu_save_options`.

## func_800C1A00 — strict MATCH

Walks the cameras registered by `camera_process_input` (`D_8013C300[0..D_8013F1DC-1]`); for the first whose
scene id equals `id`, writes the current key's direction (`keys[ctl->idx].dir`) times `ctl->f10`, negated while
`ctl->mode & 8`; zero otherwise. Plain typed C (types from `camera_aspect_ratio`), first structurally right
draft was 6 words off only because the rate field is `f10` (0x10), not `t` (0x4).
```
$T/sc.sh cloud/matches/func_800C1A00.c func_800C1A00   -> func_800C1A00:  MATCH   (also MATCH at -O2)
blob_unit ... --neighbours -> EQUAL, 0 differ
```

## func_800CCB40 — strict MATCH, own .rodata verified, HELD (not in cloud/matches)

Option-default table for the settings initialiser `func_800CCCCC` (selectors 0..40). Case 9 = PAL console.
```
$T/sc.sh cloud/work/frontier/w4d/func_800CCB40/best.c func_800CCB40
func_800CCB40:
  MATCH
    own .rodata verified at 0x80124020..0x801240C4
blob_unit ... --neighbours -> EQUAL func_800CCB40: 99 words, 0 differ
```
The bytes require `|| <compile-time 0>` on case 9. I wrote it as a compiled-out development switch
(`return D_80000300 == OS_TV_PAL || DEBUG_FORCE_PAL;`, `#define DEBUG_FORCE_PAL 0`). The equivalent bare spelling
`(D_80000300==0) || 0` was explicitly **rejected** by an earlier coordinator
(`cloud/work/ipa-groups/codex_switch_a84/STATUS.md`) as an unsupported device. Wave 3 treats compiled-out
debug/config code as source structure, but no evidence for this particular switch exists, so it is held for the
owner. Case order and table: from the A84 packet (unchanged).

## camera_process_input — near-miss, 63/255 in the unit (was 248/255 with the locked-group context copy)

`best.c` = context prelude (copied from `src/blob/groups/camera_aspect_ratio/group.c`) + body; score with
`blob_unit --tag w4d score camera_process_input --with best.c --internal func_800C15FC --internal cam_slot_set`
→ `FAIL camera_process_input: 63 of 255 words differ`. No arcade ancestor found (N64-specific).

Structural fixes found (in order of effect):
1. **The node allocation (`func_80090284` …) is outside the flags block**, not inside it (retail's flag-test
   branches go to the `jal func_80090284`). The old context copy had it nested.
2. `sc->flags &= ~0x100000; sc->flags |= 0x200400;` and the tests on `sc->flags` directly (not a cached `fl`):
   retail keeps both stores.
3. `sc = cam->ctl->scene` (whole expression) and, in the scene loop, `if (s->keys[i].flags & …) { k = &s->keys[i]; … }`.
4. The `D_8012E714[slot]` store is an inlined two-parameter setter (`cam_slot_set(cam->slot, D_80142A7A)`):
   that alone gives retail's `v0/v1` pair and fixes the temp ring for the rest of the function.
5. Moving the gameplay-mode-2 block into an inlined function named after the caller-less stub
   `func_800C15FC` (scored with `--internal func_800C15FC`) gives the same words as inline code — not proven.

Residual lanes: (a) **frame layout** — retail frame 248 with `unused` at 200, `tbl` 196, `sc` home 192, loop
spill 160, `v` 148, `delta` 136, `mat` 100; the current source reaches it only with pad arrays (`f32 pad[9]`,
`f32 padI[3]`), which are not acceptable source — the real cause (missing locals/inlined helpers) is unknown;
(b) **the camera parameter's live range is split in retail** (`a0` until the first call, `move s7,a0` later,
the mode-2 block's first read uses `a0`): ours colours it `s7` from entry (`ctrace` proc 531: w4 → s7,
save 5.23 over 13 blocks). Empty-`if` dead reads on cam/ctl/sc changed nothing.
Best next hypothesis: find the source of the 44 bytes above `unused` (an inlined helper's array or the
original declaration list) and the split, with `ctrace` on each frame variant; check whether the stub
`func_800C15FC` belongs to `camera_update` instead.

## func_80096130 — near-miss, 15/66 in the unit (wheel_setup_initial 28/31)

Texture-table slot release: if `D_80156D38[index].allocation` is set, wait for `D_8002EB70 == 0`, release the
allocation under the heap message queue (`osRecvMesg; audio_reverb_update(allocation, 0); osJamMesg` — the
same body as the locked `audio_effect_process`), clear it, and zero entry `index` of three 8-byte tables
(`D_801161F4`, `D_80151AE8` = the MBOX texture tables `{base, count}` that `func_800B24EC` searches, and
`D_80138670`). Its only caller `wheel_setup_initial(resource)` releases every slot of a resource group.
```
blob_unit --tag w4d score func_80096130 wheel_setup_initial --with cloud/work/frontier/w4d/func_80096130/best.c \
   --internal heap_release_locked --internal slot_tables_clear --internal wait_idle
  FAIL func_80096130: 15 of 66 words differ
  FAIL wheel_setup_initial: 28 of 31 words differ; compiled body is 29 words, target 31
```
From the A122 source (24/66): writing the three steps as inlined helpers (wait, locked release, table clear)
gives retail's 72-byte frame (20/66); an empty `if (slot == NULL) {}` (dead read, a compiled-out check) moves the
post-call reload of `slot` into `v1` as retail (15/66). Residual: `allocation`'s stack slot at 64 vs retail 56
(two more words above it in retail) and the `sw zero,12(v1)` that `as1` now schedules after the first memset's
argument set-up. `wheel_setup_initial` still lacks retail's unused `s4` save (frame 40 vs 48), i.e. something in
its inlined or IPA closure is still missing. ~35 variants; stopped per the rule.

---

## What generalises

- **Arcade library binaries are evidence.** `reference/repos/rushtherock/MB/zmb.a` is the MathBox library as
  a MIPS archive (`ar x`, `mips-linux-gnu-objdump -d`). It has `MBOX_FindTexture_Sub/_Err`,
  `MBOX_FindObject_Sub/_Err` and friends with real arities: N64 `func_800B24EC` = `MBOX_FindTexture_Sub`
  (5 formals), `string_copy_format` = `MBOX_FindObject_Sub(name, lo, hi, err)`. Callers that pass a "phantom"
  constant (0/1/2) in an argument slot the callee never reads are passing an `err` level to a compiled-out message.
- **An empty `if (param == NULL) {}` (compiled-out message) raises a parameter's colouring priority** so it keeps
  its incoming register (closed `func_8010C2E4`; moved `slot` into `v1` in `func_80096130`). Find it with
  `ctrace` + `force.sh` first: if forcing the retail colours reproduces retail, look for a dead read on the
  web that lost.
- **One variable reused for the same role across loops** (bit position `i` in `tournament_trophy_award`) is the
  "one variable = one register" rule in another form; conversely a counter shared with an output index can let
  uopt drop a re-initialisation.
- `(s32)` on narrow arguments to a prototyped narrow parameter creates an int web for the value and moves the
  narrowing to each call (sfx_volume_set).
- The w3a uopt tracer runs fine from another agent's scratch via a symlink (`~/…/w4d/uopt -> ../w3a/uopt`);
  `pdiff` needs two variants whose colouring differs to reveal the procedure ordinal.
