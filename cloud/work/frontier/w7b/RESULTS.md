# w7b — frontier wave 7, batch b (2026-10-05)

Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared`. Builder scratch `~/rush2049/scratch/frontier/w7b`
(unit tags `w7b`, and `w7bdot` for the func_80109A60 fork). Tools: `tools/` (w6a copies retargeted to w7b;
`full.py` defaults to `-O3` here, the w6a copy defaults to `-O2`!). Nothing committed or spliced.

| Function | Bytes | State | Where |
|---|---:|---|---|
| func_800D5374 | 328 | **group MATCH** (+ object_activate re-sourced), unit 4/4 EQUAL | `groups/frontier_main_menu_input/` |
| func_8010E4E4 | 432 | **strict MATCH**, own .rodata verified, unit EQUAL | `cloud/matches/func_8010E4E4.c` |
| func_800A1A60 | 340 | **strict MATCH**, unit EQUAL | `cloud/matches/func_800A1A60.c` |
| audio_channel_setup | 332 | 3/83 words (one ugen/as0 temp register) | `audio_channel_setup/best.c` |
| func_800E7710 (bonus sibling) | 248 | 7 aligned rows | `func_800E7710/best.c` |
| check_mpath_save (bonus sibling, 9 deps) | 276 | 6 aligned rows | `check_mpath_save/best.c` |
| func_8010C448 | 320 | 9/80 words here; **already strict-matched by another agent** (`cloud/matches/func_8010C448.c`, untracked, arcade targets.c OverlapTarget) — use that one | `func_8010C448/best.c` (superseded) |
| func_8010D3C0 | 704 | 23 aligned rows (as1 hoists the default case) | `func_8010D3C0/best.c` |
| sound_position_update | 444 | 27 aligned rows in the unit (15 with forced colours) | `sound_position_update/best.c` |
| func_80109A60 | 1,268 | 35 aligned rows (`--mnem`), colouring | `func_80109A60/` (fork; NOTES.md) |

---

## func_800D5374 — group MATCH (supersedes `frontier_main_menu_input`)

`groups/frontier_main_menu_input/` = `group.json` (`claims`: func_800D5374, object_activate) +
`main_menu_input.c` (unchanged copy of the locked group file) + `object_activate.c` (new: object_activate
and func_800D5374).
```
ssh watchman2 '... python3 tools/cloud/score.py group cand/g_mmi'
Members:
main_menu_input:
  MATCH
func_800D52CC:
  MATCH
object_activate:
  MATCH
func_800D5374:
  MATCH
python3 -m tools.conveyor.pipeline.blob_unit --tag w7b score func_800D5374 object_activate main_menu_input func_800D52CC \
    --internal func_800D52CC --with cloud/work/frontier/w7b/groups/frontier_main_menu_input/object_activate.c
  EQUAL func_800D5374: 82 words (kept, c_object_activate.c)
  EQUAL object_activate: 38 words (kept, c_object_activate.c)
  EQUAL main_menu_input: 56 words (kept, g_frontier_main_menu_input__main_menu_input.c)
  EQUAL func_800D52CC: 2 words (internal, g_frontier_main_menu_input__main_menu_input.c)
blob_unit score: 4/4 equal
```
Semantics: for each active player record (D_8014A118, 76 bytes, count `active_player_count` D_8014A108)
activate the pending node at +0x44 (object_activate: under the D_80142728 lock, unlink from list D_80146188
if byte +9, insert at the head of list D_80146170, set byte +8) and clear the slot to -1.
How it was found (each step measured):
- The body is object_activate inlined. object_activate (0x800D52D4) is the kept function just before it,
  so it is a **kept `__inline` function in the same file** (w6c mechanism): its inlined statements carry
  the call's `.loc`. A static helper or a hand-inlined body leaves 3 rows of as1 order (`sll` of the loop
  end in the load-delay slot instead of the bne delay slot); proved by editing only the `.loc` of the
  skip block in the ugen listing (`asm/l39.s`, `l40.s`: 0 rows; `l36.s`: 3 rows).
- **Early `return` for the -1 node, not `if (node != -1) {...}`.** The return (or a `continue`/`goto next`
  in a hand-inlined body) gives the skip path its own block; only then does uopt's test replacement turn
  `i < D_8014A108` into retail's `p < D_8014A118 + count*76`, recomputed on both paths (the 5-instruction
  `*76` shift chain). With the if-block form `i` stays live and needs a ninth s-register (≈44 rows); ~30
  loop spellings (s16/u16/u32 counters, pointer loops, local bounds, do-while with guards) never get there.
- The slot field is an int (`= -1`) and the node a pointer: two -1 constant webs (s4, s5) as in retail.
- func_800D52CC must be the group's internal function (the empty debug check); scored standalone it is
  3 rows off.

Integration: `blob_group revert frontier_main_menu_input`, splice this group, then `blob_unit check`.
The locked object_activate source changes (if-block form → `__inline` + early return; both compile to the
same object_activate words).

## func_8010E4E4 — strict MATCH

```
score.py fn cloud/matches/func_8010E4E4.c func_8010E4E4 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_8010E4E4:
  MATCH
    own .rodata verified at 0x801249CC..0x801249D0
blob_unit --tag w7b score func_8010E4E4 --with cloud/matches/func_8010E4E4.c
  EQUAL func_8010E4E4: 108 words (kept, c_func_8010E4E4.c)
```
`-O2`: 103/108. Falling-debris callback `(Obj *, s16 mode)`, sibling of func_8010D85C: gravity
D_80121DDC={0,-0.25,0} × 0.15 into V and position, `rotateuv(W*dt, UV)` (sound_position_set), timer
countdown, then removal. Shaping: removal block = inlined static helper taking `&D_80117530[m->type]`
(retail forms the kind pointer and loads +0x12 from it); unused `f32 a[6]` for the 104-byte frame
(INFERRED filler; same as func_8010D85C's `buf[16]`); `v = p->V` (retail hoists p+12).

## func_800A1A60 — strict MATCH

```
score.py fn cloud/matches/func_800A1A60.c func_800A1A60 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800A1A60:
  MATCH
blob_unit --tag w7b score func_800A1A60 --with cloud/matches/func_800A1A60.c
  EQUAL func_800A1A60: 85 words (kept, c_func_800A1A60.c)
```
Controller-Pak file check (osPfsFileState under the pak lock, then compare the 32 state bytes with the
remembered copy). Starting point B82 was 47/85 with a 88-byte frame. Closed by **three inlined static
helpers** — `pak_enabled(pak)` (return value in v0: retail's `lb v0; bnez; nop; b; move v0,zero`),
`pak_lock()` (calls `pak_queue_init()`; owns `OSMesg message` → sp+44) and `pak_unlock()` — which give
exactly the 112-byte frame with no padding locals; a compiled-out `if (error) {}` (keeps `error` in v1 and
spills it to its home sp+96 across osJamMesg, as retail); `D_8011194C = 1; osCreateMesgQueue(...)` on one
line (as1 puts `li t3,1` in the bnez delay slot). The same helpers are visible in check_mpath_save,
track_collision_setup, MaxPathZeroControls, func_800E73D8, func_800E7710 (all test D_8011194C).

## audio_channel_setup — 3/83 words

```
score.py fn audio_channel_setup/best.c audio_channel_setup --flags "-g0 -O3 -mips2 -G 0 -non_shared"
    +0x0fc  want 3c068014 lui a2,0x8014                 got 3c058014 lui a1,0x8014
    +0x118  want 00ca3021 addu a2,a2,t2                 got 00aa2821 addu a1,a1,t2
    +0x11c  want 94c527c0 lhu a1,10176(a2)              got 94a527c0 lhu a1,10176(a1)
  3/83 words differ
blob_unit (same 3 words)
```
CTF flag animation callback (obj->fn set by func_8010D85C): every 0.0625 s advance the frame, at the end
remove (kind flag 0x1000) or stop the sound (0x2000) then remove, else wrap; set the resource object
(`resource_set_object`, the w6b inlined setter: index v1, value a1) to D_801427C0[base+frame].
`remove:` label inside the `mode == 0` block (retail's shared removal call is at the top).
Residual lane: the as0 macro temp for `lhu a1, D_801427C0(t2)` is a2 in retail, a1 (= destination) in
ours; uopt colours are identical. `la`+`lhu 0()` in the listing gives 7 rows, so retail's listing has
the same macro with another register free/blocked. Tried: setter signatures (s16/s32/u16 index, s32
value, pointer value, table+frame params, nested `model_set_frame`), pointer temp, `*(D+f)`. Next: look
at as0's macro temp rule (when is the destination not reused as the address temp?).

## func_800E7710 — 7 rows (bonus sibling of func_800A1A60, L1, 1 dep)

`osCreateMesgQueue(&D_80035458, D_80150F18, 8); D_80150F58 = 5; osSetEventMesgAlt(5, &D_80035458,
&D_80150F58); pak_lock(); __osContBuildPacket(...) [= osContInit]; pak_unlock(); clear 4 rumble bytes.`
Residual: retail stores `D_80150F58 = 5` via `lui at` and builds the argument address separately in a2;
ours CSEs one address web. volatile/struct/array/integer-cast spellings all CSE. Next: two different
symbols at the same address (e.g. a struct member store vs a typed message pointer).

## check_mpath_save — 6 rows (bonus sibling, 9 dependents)

Clear rumble flag D_8011EAE4, pak_lock, for each of 4 paks with byte +6 set: clear +125,
osMotorStart(&D_80035458, &pak->pfs, i) [0x8000A194, = osMotorInit by signature], and if
"osMotorInit"(&pak->pfs, 0) [0x80009F20] returns 0 clear +124; pak_unlock. Everything matches except
the skip: retail's ugen listing has `bne t8,0,L; b inc; L:` (proved: `asm/skip.s`, that one listing edit
gives 0 rows); ours `beq t8,0,inc`. `continue`, early-return helper (static and kept `__inline`),
`if/else continue`, `goto`, `switch`, pointer IV — all give the single beq. Next: find which source makes
ugen emit a jump-around (a then-block that uopt keeps as a separate block).

## func_8010C448 — 9/80 words (superseded: another agent's `cloud/matches/func_8010C448.c` MATCHes)

That source explains both of my residuals: `s16 slot = *slotp` is a named s16 local, which always gets a
stack home (the 8 bytes my `pad[2]` stood in for), and `gap = dsq - radius*radius; if (dist) *dist = gap;
if (gap > 0.0f)` keeps the unfolded compare. Arcade ancestor: game/targets.c OverlapTarget.

`best.c` (= `sw3/r_after_dist.c`). `(s16 *id, f32 *pos, f32 *radius, f32 *out)`: is the car within
radius+3.5 (xz) and −2 < dy < 18 of pos; *out gets dist² − r². Found: `18.0f`; `d[0]*d[0] + d[2]*d[2]`
(operand order); `r += 3.5f` after the dist line; `dist -= r * r` (a `dist - r*r > 0` compare is folded by
uopt to `r*r < dist`). Residual: FP temps of the final compare (retail r² f6, 0.0 f8, difference f10 as a
fresh temp; ours f8/f6 and the subtraction into dist's f2) — retail neither folds the compare nor writes
dist. `f32 pad[2]` (first local) is an artificial 8-byte frame filler (retail frame 32). Tried ~45
spellings (helper for the vectors, `excess` local, int zero, `!(x <= 0)`, `(dist -= r*r) > 0`).

## func_8010D3C0 — 23 aligned rows (72/176 positional)

Pickup effect callback (B130's eleven-case source, cleaned: natural literals `0.666667f`, `0.0333333f`,
`0.05f`; func_80391490 called with no arguments — a0 still holds the effect). Found: `vehicle->speed =
vehicle->boost = bias + 3.0f`; `speed > max ? speed : max`; kind/amount assignment order kind-first with
each case on separate lines (as1 delay-slot pick); case 354 order state, boost, field936, color; case 359
order 916, 920, 924. Residual: as1 hoists the default case's `li a0,1; lui v1` into the switch head;
retail does not. Listing experiments: setting `.livereg` at the return to include v1 and a0
(`0x1800FF0E`) reproduces retail (8 rows = relocations only) — so in retail a0/v1 are live on the
jump-table paths that return. No C form found (end-of-function compiled-out uses, default placements).
`if(amount) {}` ×2 in best.c is a colour-priority lever (artificial).

## sound_position_update — 27 aligned rows (unit)

`best.c` + the kept func_80095198 (`sum < clock` timer helper) defined in the same file (it is inlined
in retail). Found: `if (s->state == 2) {} else if (s->state == 1) return;` gives retail's
branch-to-next; `c->sound->refs++`. Residual: colouring (s→a3, state→v1, v→a2, const 2→t0, const 1→t1
in retail); `force.sh s0 sound_position_update 139 "p1:w0=c6,p1:w6=c3,p1:w2=c5,p1:w82=c7"` gives 15
rows; the rest is the constant 1 not being a register candidate in ours (`li at,1` twice).

## func_80109A60 — 35 aligned rows (fork; full notes in `func_80109A60/NOTES.md`)

Arcade `hud.c:AnimateDot` with `Hidden` defined in the file and inlined; arcade declaration order gives
the 40-byte frame. Residual: colouring (blt in t0 and spilled; slot memory-resident needs one more block
in its range — the `if (flash) {}` in best.c is artificial).

---

## What generalises

1. **A kept function can be `__inline` *and* inlined into its file neighbour** (w6c), and that matters
   beyond wrappers: object_activate (38 words) is inlined into func_800D5374. Signs: the body of the
   neighbouring kept function appears verbatim in the caller, and the stub between them.
2. **Uopt's loop test replacement needs a block on the skip path.** `for (i…) { if (x == -1) return/continue
   /goto next; …; next: a[i].f = -1; }` gives retail's pointer compare against `base + count*stride`
   recomputed per path; `if (x != -1) {…}` keeps `i` (and costs an s-register). Look for this whenever
   retail's loop exit is `sltu sN, v0` with the `*stride` shift chain recomputed in two places.
3. **as1 tie-breaks follow `.loc`**: equal-or-later line for an instruction moves it later. Editing just
   the `.loc` of one block in the ugen listing (asmt.sh) is a one-run test.
4. **Inlined static helpers explain frames without padding** (w5b/w6b again): func_800A1A60's 112-byte
   frame is pak_enabled + pak_lock(→pak_queue_init) + pak_unlock; func_8010E4E4's removal helper. An
   inlined callee's return value lands in v0 (so `lb v0; bnez; nop` rather than `bnezl`).
5. **A compiled-out `if (v) {}` right after an assignment keeps v register-allocated** and spilled to its
   own home across the next call (retail `jal; sw v0,96(sp)` … `lw v1,96(sp)`); without it ugen reloads
   the memory variable into a fresh temp (t4).
6. **ugen listing edits answer "which structure does retail's ugen have"**: `bne; b` vs `beq`
   (check_mpath_save), `.livereg` at the return (func_8010D3C0), `la`+`lhu 0()` vs the lhu macro
   (audio_channel_setup). Do this before writing C variants.
7. **Named s16/s8 locals always take a stack home** (fork finding on func_80109A60, confirmed by the
   other agent's func_8010C448): an unexplained 8-byte frame difference is often an s16 copy of a parameter
   (`s16 slot = *slotp;`), not a pad.
8. Same-shape sibling families found: (a) the pak-lock family (D_8011194C: func_800A1A60 ✓, check_mpath_save,
   func_800E7710, track_collision_setup, MaxPathZeroControls, func_800E73D8); (b) the active-player table
   loop with test replacement (func_800D5374 ✓, func_800D6530 L2 — waits on func_800D63EC);
   (c) the `(Obj *, s16 mode)` object callbacks reading D_801170FC (func_8010E4E4 ✓, audio_channel_setup,
   func_8010D680 (w6d near-miss: its 80-byte frame and `sw a3,76(sp)` look like func_8010E4E4's inlined
   removal helper), func_80106874, func_801084D4, func_80109F54, func_8010C974).
