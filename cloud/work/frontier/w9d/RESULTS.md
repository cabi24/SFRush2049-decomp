# w9d results (wave 9, 2026-10-06)

Builder scratch `~/rush2049/scratch/frontier/w9d` (copied from `base`), unit tag `w9d`. Nothing committed, spliced
or pushed; nothing under `src/`, `asm/`, `tools/`, `include/`, lock files or other agents' dirs touched.
`func_80087110`, `stat_race_update`, `func_800FE5B0` not touched. No permission denials affected the work (one
`rm` of my own probe files with a `$W` glob was refused by the safety check; redone with literal paths).

Tools (`tools/`): w3a's traced-uopt scripts retargeted to w9d (`ctrace.sh`, `force.sh`, `pdiff.sh`, `sum.sh`,
`tr.sh`, `udiff.py`), `sc.sh`/`full.sh`/`batch.sh` (score.py fn, aligned diff, batch on the builder, 1 thread),
`bu.sh` (batch `blob_unit --tag w9d score`), `o3s.sh` (pre-as1 ugen listing).

## Table

All flags `-g0 -O3 -mips2 -G 0 -non_shared`. Scorer = `IDO_DIR=… python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"` in the w9d scratch unless noted.

| Function | Bytes | State | Exact scorer output | Deliverable |
|---|---:|---|---|---|
| `sound_play_menu` | 332 | **MATCH** | `sound_play_menu:` / `MATCH`; unit: `EQUAL sound_play_menu: 83 words (kept)` | `cloud/matches/sound_play_menu.c` |
| `func_800F7EB0` | 140 | **MATCH** (own rodata verified) | `MATCH` / `own .rodata verified at 0x80124618..0x8012461C`; unit `EQUAL … 35 words (kept)` | `cloud/matches/func_800F7EB0.c` |
| `game_timer_pause` | 164 | **MATCH** | `MATCH`; unit `EQUAL game_timer_pause: 41 words (kept)` (also EQUAL with the non-static stub form and `--internal func_800FE79C`) | `cloud/matches/game_timer_pause.c` |
| `audio_channel_reset` | 164 | **MATCH** | `MATCH`; unit `EQUAL audio_channel_reset: 41 words (kept)` | `cloud/matches/audio_channel_reset.c` |
| `save_slot_valid` | 176 | **MATCH** | `MATCH`; unit `EQUAL save_slot_valid: 44 words (kept)` | `cloud/matches/save_slot_valid.c` |
| `func_800D2C10` | 196 | **MATCH** (own rodata verified) | `MATCH` / `own .rodata verified at 0x8012417C..0x80124180`; unit `EQUAL … 49 words (kept)` | `cloud/matches/func_800D2C10.c` |
| `func_8008ABE4` | 144 | **MATCH in a real group** (internal) | `score.py group cand/groups/dma_queue_next --claims` → `Members:` / `func_8008ABE4:` / `MATCH`; unit `blob_unit score func_8008ABE4 --internal func_8008ABE4 --with …` → `EQUAL func_8008ABE4: 36 words (internal)` | `groups/dma_queue_next/` (claims `func_8008ABE4`) |
| `arb_rate_set` | 312 | 25/78 (unchanged) | unit: `FAIL arb_rate_set: 25 of 78 words differ` | `arb_rate_set/best.c` (= w4b) |
| `brake_light_update` | 448 | 74/112 (unchanged; 27 rows with forced colouring) | unit: `FAIL brake_light_update: 74 of 112 words differ` | `brake_light_update/best.c` (= w2a) |
| `entity_lod_select` | 716 | 36/179 (unchanged) | unit: `FAIL entity_lod_select: 36 of 179 words differ` | `entity_lod_select/best.c` (= agentA) |
| `physics_float_calc` | 1176 | not attempted beyond reading w1e's notes | — | — |
| `func_800A7480` | 136 | 17/34 | `17/34 words differ` | `func_800A7480/best.c` |
| `func_800E762C` | 228 | 26/57 | `26/57 words differ` | `func_800E762C/best.c` |
| `func_8010FBE0` | 128 | 30/32 strict, 3 aligned-missing (diagnostic form only) | `30/32 words differ` | `func_8010FBE0/best.c` |
| `func_800AC9BC` | 224 | 32/56 | `32/56 words differ` | `func_800AC9BC/best.c` |

Strict matches: 7 functions, 1,116 bytes (6 singles + 1 group member). None are provisional: every one is
standalone or has only real, locked partners.

## Per function

### sound_play_menu — MATCH (assigned)
Heap allocate-from-the-end (mirror of `audio_helper`). Residual of w2e/w4a (14/83, ~300 variants) was a
**copy-propagation split**: uopt replaced uses of `n = b + bs - size` inside the two link blocks with the
expression (web w56, a1) and kept `n` as a variable only from the join (w52, a0) → `move a0,a1` and a lost
hoist of `move a1,zero`. Diagnosed with `ctrace` (proc 615) + `force` (variant with `n` from `b->size` forced to
`n=a0/load=v1` gave retail exactly). Fix: a dead reassignment `bs = b->size;` right after computing `n` kills
the availability of `n`'s defining expression, so it is not propagated (`bs -= size;` there also matches).

### func_800F7EB0 — MATCH (extra)
Reset loop (count D_8014A108; byte array cleared, float array set to 5999.999f; 3x5 bytes = -1). Earlier
research (`cloud/work/dot_array_reset`, 4/35 words, ~1,800 layouts) spelled the function's **own rodata
literal** as `extern float D_80124618`. Writing `5999.999f` (0x45BB7FFE) closes it; the scorer now verifies own
rodata. Also MATCH at -O2.

### game_timer_pause — MATCH (extra)
Event lookup by handle under the D_80142728 lock, set flag +0x0E. The lookup is the deleted inlined static
`func_800FE79C` (caller-less stub right before it), same shape as `func_80091BA8` (entity_lookup group). The
match file has it `static`; for landing, the non-static form named `func_800FE79C` is also EQUAL with
`--internal func_800FE79C` (then a `prefer_definition` override may be needed, per plan §0).

### audio_channel_reset — MATCH (extra)
All controllers of active input records present (D_80144030[port].present, +6) → store in spr->unk1A and
re-apply pad config if changed. Written inline it was 15/41, a pure colouring permutation (force
`v=v0, ptr=v1, slot=a0` gave 0 rows). The flag read through an **inlined static helper returning s8**
(`controller_present(u8 port)`) changes the web priorities to retail's. No stub for the helper is known.

### save_slot_valid — MATCH (extra)
Scene-record creation via func_8008E26C with owner from func_800A7D6C / D_8011418C / table D_803B9AA0.
Correct callee prototype (`s32` first argument, from src/blob/func_8008E26C.c) plus a **flat** index
`D_803B9AA0[row / 13 * 12 + col]` (2-D `[][12]` gives the same instructions, 8 words of register naming). -O3 only.

### func_800D2C10 — MATCH (extra)
Nearest point in D_8012E5E8[set]. Quirks (in header): goto loop (for/while/do with s32 counter unroll at -O3,
s16 counter adds narrowing), no `PSet *` local (v0/v1 swap), uninitialised `best` lives on the stack as retail,
and an unused `s32 pad0[4]` **filler** for the 32-byte frame (best at sp+6). Own rodata 1e20f verified.

### func_8008ABE4 — MATCH in real group (extra)
Start the next queued PI DMA (callee at 0x80008630 must be named `__osPiRawStartDma`, the blob.ld name; an
`osPiStartDma` name links to 0xDCD0). Internal: four-wide t6–t9 ring; matches only with its callers. Group
`groups/dma_queue_next/`: member func_8008ABE4 + unchanged locked sources `struct_init_and_call.c` and
`task_complete_signal.c` (copied from `src/blob/groups/codex_task_complete_signal/group.c`, with
func_8008AD04). Context: struct_init_and_call MATCH, func_8008AD04 MATCH, task_complete_signal differs only by
the known position-dependent epilogue padding (unit_overrides `align` entry). Earlier codex attempt stopped at
6/36 with a named record pointer; reading fields straight from `reqs[next - 1]` removes it.

### arb_rate_set — 25/78 (assigned, unchanged)
Two residuals, both colouring: (1) record address (w3) and `&D_8011EA30` (w62) swapped v0/v1, and retail's
post-call record pointer is a different register (v0) from the pre-call one (v1) — forcing w3=v1 keeps v1
after the call too, so retail's spill/reload is a split web; (2) the inlined `func_800A5560` u16 parameter
(w43, frame −6) is a coloured web (a0) in ours, absent in retail. Diagnostic: with an argument free of loads
(`GPACK(index,index,index,1)`) the parameter is copy-propagated and w43 disappears; with field loads in the
argument (as retail's `sll …,zero,…` proves) it stays. Tried: macro twice/direct `(u16)` casts (frame drops to
48, worse), pointer local (31), inlined `set_color`/`set_fill` statics (35–41, folded or not inlined), calling
an inlined func_800A7480 (not inlined). **Next:** trace why uopt refuses to copy-propagate an inlined
parameter whose argument contains forwarded loads; func_800A7480 is the minimal reproduction of the same
mechanism.

### brake_light_update — 74/112 (assigned, unchanged)
Trace (proc 339): retail keeps `r[2]` (w22) and `scr[0]` (w38) in memory; ours colours both (save 0.5 each).
`force p1:w22=s,p1:w38=s` takes it to 27 rows (front identical; remaining: `scr[1]` load CSE across the two
compare blocks into f0, and as1 order). Tried without movement: volatile on the clamp read (78), ternary clamp,
volatile scr (79), one local struct holding d/r/scr (74, IDO promotes struct members as it does array
elements), loop vout copy (not unrolled, 100). **Next:** find what makes the r/scr elements non-candidates in
retail (an escaping address, e.g. a callee taking `&scr`/`r` again, or the callers if the function is internal).

### entity_lod_select — 36/179 (assigned, unchanged)
Trace (proc 182): forcing `w19=v1` (op's loop-head web) and `w26=a0` (`op & 0xC0`) fixes the 5-word loop-head
allocation; the 31-word temp-ring phase residual remains (ugen, as agentA found). Not pursued further (agentA
spent ~900 variants).

### physics_float_calc — not attempted
Read w1e's notes (800 variants; hypothesis: whole body is the static `func_8009EBB8` with a debug parameter).
Breadth-first: time went to fresh singles instead.

### func_800A7480 — 17/34 (extra; sibling of arb_rate_set)
Sets range/colour for view record `index` and inlines `func_800A5560(GPACK_RGBA5551(r,g,b,1))`. Found:
`s8` red (one more ugen narrowing temp moves the index temps to retail's t4/t5/t6); a pointer local laundered
through `(u32)` removes ugen's `.noalias $2,$sp`, which puts the stores before the alpha load as in retail
(33 → 17). Residual: the GPACK result is a coloured expression web (v1) in ours; retail has none and never uses
t1. 324-way type sweep and helper forms did not move it.

### func_800E762C — 26/57 (extra)
Per-model rescale over the six 0x808-byte MODELDAT records. Needed: goto loop (else unrolled), condition
inverted (`!= 2 || i == 0 || unk718 == 0.0f` first), volatile D_80143FF4, named `scale` local. Residual:
FP colouring (f → f12 and the unk718 load → f2 in retail; forced `p2:w14=c26,p2:w27=c25` fixes it), retail
materialises `&D_80143FF4` in a3 (a uopt web, ours uses a t-temp), and the FP temp ring.

### func_8010FBE0 — near-miss (extra)
Copies an OSTask into the OSScTask at D_80155238 and jams it into the scheduler queues. Retail stores each
field with its own `lui at` (no base register); with one struct symbol uopt makes `&D_80155238` a web (a3).
Only separate field symbols (diagnostic, not a source claim) reach 3 aligned rows. Volatile, defined,
pointer, array and order forms: no change.

### func_800AC9BC — 32/56 (extra)
Quadtree descent: node 0x14 bytes {u8 leafMask @3; s16 x0,x1,y0,y1 @4..10; u16 child[4] @12}, node array
pointer D_80124EEC; quadrant q = (x >= midx) + 2*(y < midy); return the node whose bit q is clear (q to
*quad), or NULL when a child index leads back to the root. Needed: do/while with `n != D_80124EEC`, found
block after the loop via `goto found`. Residual: q takes a1 (x moved to t0) where retail gives q a2 (y moved),
and retail's signed `/2` idiom clobbers the sum register (`bgezl v0; addiu v0,v0,1; sra t6,v0,1`) where ours
uses `at`; the latter suggests the sums are dying temps whose quotient is not a coloured variable.

## What generalises

1. **Copy propagation is a lever in both directions.** A defining expression copy-propagated into some uses
   splits a variable into an expression web plus a variable web (sound_play_menu); a dead redefinition of an
   operand right after the definition stops it. Conversely an inlined parameter whose argument contains loads
   is *not* propagated and becomes its own coloured web (arb_rate_set, func_800A7480).
2. **Old near-misses that spell a function's own float literal as `extern f32 D_8012xxxx` should be retried
   with the literal**: the scorer now verifies own rodata, and the extern changes as1 scheduling
   (func_800F7EB0 closed from 4/35 instantly).
3. **A pure colour permutation in a small loop can be web priority, not interference**: an inlined static
   helper returning the value (audio_channel_reset) or removing a named pointer local (func_800D2C10) reorders
   uopt's colouring. `force.sh` first to prove the residual is colour-only, then try helper/local forms.
4. **`.noalias reg,$sp` is visible in the schedule**: when retail keeps stack-argument loads/homes behind
   stores through a record pointer, the pointer is not a known global address in retail; `(u32)` laundering of
   a pointer local reproduces that (func_800A7480; same effect noted in display_settings).
5. **Internal leaf functions with locked callers are real matches, not provisional**: `frontier show` said
   `single` with no callers listed for func_8008ABE4, but its two callers are locked; `blob_unit score
   --internal NAME` found it EQUAL at once. Worth sweeping other `single` near-misses with `--internal`.
6. Callee names must be the blob.ld names at the target address (`__osPiRawStartDma` at 0x80008630), not the
   SDK name the arguments suggest.

## Struct layouts / globals recovered

- D_80110270: `Event *` table (0x3C-byte events, handle @8, flag @0x0E), index `h & D_80146104`.
- D_8014A118[]: 0x4C-byte input records, controller port u8 @+1; count D_8014A108 (s16).
- D_80144030[]: 0x304-byte controller/pak records, `s8 present` @+6.
- D_8012E5E8[]: point sets `{u16 n; P3 *pts}`; P3 = 4 × s16.
- D_80153F10: DMA request list `{u16 count @2; u16 next @4; DmaReq *reqs @8}`, DmaReq = `{size, devAddr, vAddr}`.
- D_80155238: OSScTask (next, state, flags, framebuffer, OSTask list @0x10, msgQ @0x50, msg @0x54).
- MODELDAT (0x808): s32 @0x710, f32 @0x714, f32 @0x718 (func_800E762C).
- View record D_8017A510 (0x48): range_start/end s16 @0x40/0x42, r,g,b,a u8 @0x44..0x47.
