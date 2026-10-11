# w20f RESULTS (wave 20, game fresh singles/units)

Status: done (session end).

| Function | Bytes | State | Flags | Scorer line |
|---|---:|---|---|---|
| func_800D11BC | 140 | **MATCH** -> `cloud/matches/func_800D11BC.c` (disclosed `volatile` shaping) | -g0 -O3 -mips2 -G 0 -non_shared | `score.py fn`: `MATCH` / `own .rodata verified at 0x8012416C..0x80124174`; unit: `EQUAL func_800D11BC: 35 words`, `locked bodies that differ in this unit: 0` |
| audio_fade_control unit (8 fns) | 1904 | 6/8 EQUAL in the unit; audio_fade_control 4 words, gfx_setup_e700 9 words; not installable yet | same | see section below |
| wheel_setup_initial | 124 | near-miss: only an unused s4 save + 8 B frame | same | `FAIL wheel_setup_initial: 27 of 31 words differ; compiled body is 29 words, target 31` |
| ghost_race_setup | 440 | unchanged (w8b best 16/110); finding: release path = inlined func_800960CC | same | `FAIL ghost_race_setup: 16 of 110 words differ` |
| credits_screen | 296 | skipped: internal, its only caller func_800D7634 (1804 B) is unmatched -> provisional at best (w14d) | | |


Targets chosen (fewest prior attempts): audio_fade_control unit (8 fns / 1904 B; prior: only the codex
ipa-groups a31/a139/a158/b113, no frontier lane), then func_800D11BC, credits_screen, wheel_setup_initial,
ghost_race_setup, sound_position_update.

## audio_fade_control unit

Findings so far (unit, `blob_unit --tag w20f ... --internal audio_fade_control --internal func_800956BC`):
- audio_fade_control is internal (all callers in this file): IPA gives it node in s4 and leaves s0-s3 to callers.
- func_800956BC (locked single, 20 words) is called only by audio_bus_route / sound_priority_set /
  audio_stream_control / audio_buffer_manage; retail keeps it internal (callers keep a2 live across the call).
  Its body is unchanged when internal ("locked bodies that differ in this unit: 0").
- Inlined helper `node_alloc()` {head of free list D_80146138, busy=0, unlink} = the 8-byte frame area at 52(sp)
  in every caller; `list_append(list,node)` = func_80091FBC(list,node,list->tail).
- Entry index written `D_80144C48[handle & 255]` (no `offset` local) -> frames 64/72 exact.
- Store order: kind, entry, then the three -2.0f fields ascending, then the updated field.
- audio_state_save: an inlined `entry_alloc(Entry **out)` helper (`*out = 0;` first) explains `entry` living
  only in memory at 76(sp) (address taken) and the found flag sharing v0 with i.

State after ~2.5 h (stop rule reached): **6 of 8 EQUAL in the unit, 2 near-misses; not installable yet**
(audio_fade_control is the internal callee every member calls, so the group needs it matched).
Best: `cloud/work/frontier/w20f/audio_fade_control/best.c`. Command (Pi, repo root):
`python3 -m tools.conveyor.pipeline.blob_unit --tag w20f score audio_fade_control gfx_setup_e700 audio_bus_route
audio_timing_sync sound_priority_set audio_stream_control audio_buffer_manage audio_state_save --with
cloud/work/frontier/w20f/audio_fade_control/best.c --internal audio_fade_control --internal func_800956BC --neighbours`
```
  FAIL audio_fade_control: 4 of 53 words differ        (+1 extra word = node_alloc's stub, packaging only)
  FAIL gfx_setup_e700: 9 of 32 words differ             (udiff: 4 rows)
  EQUAL audio_bus_route / audio_timing_sync / sound_priority_set / audio_stream_control /
        audio_buffer_manage / audio_state_save
  locked bodies that differ in this unit: 0
```
Levers that closed the six (each traced, not swept):
- `--internal audio_fade_control` (IPA s4 parameter, s0-s3 left to callers): 44 -> 22 rows, callers' frames.
- inlined `node_alloc()` helper: callers' 8-byte inline area (frame 56 -> 64).
- `D_80144C48[handle & 255]` written three times instead of an `offset` local: frame 72 -> 64 (timing_sync EQUAL).
- `--internal func_800956BC` (only these four callers in retail; a2 survives the call): 32 -> 12 words.
- store order kind, entry, the three `-2.0f` fields ascending, then the updated field: the four float setters EQUAL.
- audio_state_save: `entry_alloc(Entry **out)` with `*out = 0;` first -> `entry` memory-only at 76(sp) and the
  found flag in v0 shared with i; the entry's `state16` field is `u32` (its `-1` is then a different constant
  from the `resource == -1` compare, so no shared -1 web): 93 -> 0.

Residuals (traced):
- audio_fade_control: retail `li a0,2; ... bne a0,v1` = the constant 2 is a COLOURED constant web (a0), hoisted by
  as1 into block 0; ours compares with `li at,2` (macro). pretrace: our constant 2 has a LIVE-RANGE (block 1) but
  save=0 (one use), so it is never a p1 candidate. A second 2-use proves the mechanism (`li s5,2; bne s5,v1` when
  the second use crosses a call), so retail has a second use of 2 that ends before the first argument set-up and
  emits no code. Not found: switch forms, nested ifs, goto, kind local (u8/s32/u32), unsigned/cast constants,
  bitfield kind, helper predicates (24+ forms). Also entry must be loaded in block 3 without bnel (f4 form does
  that), which follows once 2 sits in a0.
- gfx_setup_e700: retail lacks `.noalias $4,$sp` (listing oracle: deleting it fixes the lw placement, 4 -> 3 rows)
  and as1 renames the `la` high part into a2 (`lui a2; addiu a0,a2`). Per w13b the first access's ucode root must
  be a variable (isvar), not `ldaS`; every single-block form copy-propagates it. Line layout, statement order,
  helper with a List* parameter: no change.
Integration (when finished): group with members audio_fade_control + 7 callers + func_800956BC (supersede the
locked single; `--revert-single func_800956BC`) + the stubs the helpers are renamed to (func_800953B4/BC/C4,
func_800954A0); helpers non-static, not on keep. func_80095CF4/CFC belong to codex_heap_release_a25, leave them.

## func_800D11BC (140 B) -- MATCH -> cloud/matches/func_800D11BC.c

Arcade syminit() (drivsym.c): initialize / road / positions / inlined mcommunication rpm line
(`(s16)(m->engangvel * 9.549305f * .9f)`), plus an N64 identity-quaternion init at 0x720 and a byte flag at 0x730.
- `score.py fn` (watchman2, -g0 -O3 -mips2 -G 0 -non_shared): `MATCH` / `own .rodata verified at 0x8012416C..0x80124174`
- `blob_unit --tag w20f score func_800D11BC --with cloud/matches/func_800D11BC.c --neighbours`:
  `EQUAL func_800D11BC: 35 words (kept, c_func_800D11BC.c)` / `locked bodies that differ in this unit: 0`
- Shaping device (disclosed in the header): the quat[4] and the flag byte are `volatile`. Traced: the only
  residual of the plain form (7 words) was as1 hoisting `li t6,1; sb; move a0,s0` into the mtc1 latency slots;
  retail keeps the byte store after the float stores. Volatile floats alone: 5 words; both: EQUAL.
  Labels / `do {} while (0)` / `if (m) {}` / goto / one-line layouts / helper inlining did not change it.
- Integration: plain game single (splice_singles.py func_800D11BC). No overrides.

## wheel_setup_initial (124 B) -- near-miss, only the s4 save / frame 48 vs 40

Release every slot of D_80156D38[64] (20-byte records) owned by `resource` via func_80096130 (locked, kept, plain
ABI: saves s0/s1 only). Callers: state_change_preprocess (constants 55..60), billboard_render.
Best `wheel_setup_initial/best.c` (w4d's loop + one compiled-out `if (resource) {}` holder, which swaps the
resource / 64 colours to retail's s2 / s3):
`FAIL wheel_setup_initial: 27 of 31 words differ; compiled body is 29 words, target 31` (udiff 14 rows: only the
prologue/epilogue offsets and the missing `sw/lw s4`). Retail saves s4 without using it and has 8 bytes of
local area at 16..23 (ours 4): one more web live across the call that emits no code, plus one more slot.
Tried (no s4): index form, `&D[i]` pointer, pointer-bound loop, `n = 64`, u8 parameter (adds andi), nested ifs,
inlined predicate / release helpers (defined after, stub func_800A4B40 follows the function), holders on i,
slot->allocation, resource (before/after the call), alloc local. Next hypothesis: an inlined helper whose
parameter copy or local survives across the call (the stub func_800A4B40 right after the function is the
candidate), or a dead second induction variable kept only by a compiled-out use.

## ghost_race_setup (440 B) -- not improved; one structural finding

Start: w8b best re-scored in the current unit: `FAIL ghost_race_setup: 16 of 110 words differ` (14 rows), unchanged.
Finding: the release path `osRecvMesg / audio_reverb_update(state->allocation, 0) / osJamMesg` is the inlined
heap-free helper func_800960CC (defined for real in codex_heap_release_a25/wheel_params_set.c, force_internal +
prefer_definition already in unit_overrides). Calling `func_800960CC(state->allocation)` reproduces retail's
`lw s0,76(s2)` (the helper's parameter copy in s0 -- w8b residual 3, which w8b could only get with a named local)
but as1 then hoists the osRecvMesg argument moves above the `beqz` (`ghost_race_setup/g1_heapfree.c`:
`FAIL ghost_race_setup: 54 of 110 words differ; compiled body is 111 words, target 110`, 21 rows). The remaining
cause is the same as w8b's residual 2: retail's ready path branches to a count reload shared with the release
path (`b 0x158`), ours duplicates the reload into the ready path. Inner-join holders / restructured if-else /
continue / swapped arms: worse (30-51 rows). Next: combine the func_800960CC call with a form that gives
uopt one inner-join block for the D_8014A108 reload (w8b's listing edit e2.py shows the target shape).
