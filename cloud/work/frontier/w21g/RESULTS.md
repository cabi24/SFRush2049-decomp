# w21g RESULTS (wave 21, game fresh singles/groups)

| Function | Bytes | State | Flags | Scorer line |
|---|---:|---|---|---|
| func_800D63EC | 324 | **MATCH** single, `cloud/matches/func_800D63EC.c` (1 disclosed compiled-out read) | -O3 | `score.py fn`: `MATCH`; unit `EQUAL func_800D63EC: 81 words`, 0 locked differ |
| func_800D6530 | 352 | **MATCH** in superseding group `groups/codex_vsync_a145/` (no shaping) | -O3 | `score.py group`: all 6 members MATCH (own .rodata 0x801241AC verified); unit `EQUAL func_800D6530: 88 words`, 0 locked differ |
| func_800B3704 | 228 | **MATCH** single, `cloud/matches/func_800B3704.c` (arcade NewBlit; 1 disclosed compiled-out read of the 4th param) | -O3 | `score.py fn`: `MATCH`; unit `EQUAL func_800B3704: 57 words`, 0 locked differ |
| func_800EC190 | 224 | 2/56 words (from 18); `func_800EC190/best.c` | -O3 | `2/56 words differ` + `own .data verified at 0x801163F4` |
| func_8008AD6C | 164 | 29/41 (from 34); `func_8008AD6C/best.c` | -O3 | unit `FAIL … 29 of 41` |
| func_800A1BB4 | 184 | 35/46, structure fixed but early return; `func_800A1BB4/best.c` | -O3 | unit `FAIL … 35 of 46; compiled body is 44 words` |
| func_800A7480 | 136 | not improved (24/34 rows) | -O3 | |
| wheel_setup_initial | 124 | not improved (27/31, frame 40 vs 48) | -O3 | |

New game coverage: 3 functions, 904 B (func_800D63EC 324, func_800D6530 352, func_800B3704 228).

### Integration
1. `splice_singles.py func_800D63EC func_800B3704` (plain singles, no overrides, no data-symbol changes).
2. Then `install_group.py cloud/work/frontier/w21g/groups/codex_vsync_a145 codex_vsync_a145` (same name supersedes the
   locked group; group.c is the locked file byte-identical; new member func_800D6530 added to members/keep/claims; no
   unit_overrides). func_800D6530 calls func_800D63EC, which must be locked first (it is not in the group, see below).
3. `func_800EC190` (when it lands): D_801163F4 becomes a function-local static (own .data, 4 zero bytes at
   0x801163F4); `extern s16 D_801163F4;` lines elsewhere are generated declarations only (no other function uses it).

### What generalises
- Re-score old sweep directories, not just best.c: w7c's own `sweeps/sw1` already held unit-EQUAL func_800D63EC
  variants its RESULTS never reported.
- A data symbol referenced by exactly one function and lying in .data with zero bytes is a function-local static:
  uopt then promotes it like a local (no coloured address web). Word-scan all targets for the address (lui/imm pair)
  before tuning colours of a "global".
- Arcade struct field types are constant-web keys: U32 vs S32 fields make two `-1` constants (two `li`), one shared
  register otherwise.
- If every caller passes N+1 arguments the function has N+1 parameters even when the body never reads the last one;
  the unused parameter's register (a3) can be the "unknown interferer" that pushes a parameter copy from a3 to t0
  (a compiled-out read keeps it live).
- `forbidden0` in a ctrace raw `p2dec` line shows precoloured interference (return value v0, parameter a0) before
  any colouring: an in-arm `return 2;` can forbid v0 to the main value web; one final return fixes it.
- An in-loop `break` that retail compiles as `b` to a post-loop test is an inlined helper's `return p`.

## func_800D63EC (324 B) — strict MATCH, `cloud/matches/func_800D63EC.c`
Found by re-scoring old drafts: w7c's own sweep `w7c/func_800D63EC/sweeps/sw1/` already contained unit-EQUAL
variants (`P0_h.c`, `P0-P1_*`) that w7c's RESULTS never reported (it recorded the 10-word best.c). Picked the one
with the fewest shaping devices (P0_h: a single compiled-out `if (D_80146170.head) {}` after the inlined lock).
```
blob_unit --tag w21g score func_800D63EC --with cloud/matches/func_800D63EC.c --neighbours
  EQUAL func_800D63EC: 81 words (kept, c_func_800D63EC.c)
  locked bodies that differ in this unit: 0
score.py fn cand/func_800D63EC.c func_800D63EC --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800D63EC:
  MATCH
```
Integration: plain single (splice_singles). No overrides. Unblocks func_800D6530 (its L2 caller).

## func_800D6530 (352 B, L2 caller of func_800D63EC) — MATCH in superseding group `groups/codex_vsync_a145/`
First draft from the w11h-era ipa-group seed `cloud/work/ipa-groups/codex_speed_records_a150` (83/88 off there, pointer
loop). Retail's pointer loop with a recomputed end (count in a0, reloaded after the call) is IDO's test replacement
of an index loop `for (i = 0; i < D_8014A108; i++) D_8014A118[i]...` (sibling idiom: audio_channel_reset,
assign_drones). Literal 0.1f is own .rodata 0x801241AC (0x3DCCCCCD). speed_set prototype uses the locked
`(f32, f32, s32, s32)` signature. No shaping devices.
```
blob_unit --tag w21g score func_800D6530 --with groups/codex_vsync_a145/func_800D6530.c --neighbours
  EQUAL func_800D6530: 88 words (kept, c_func_800D6530.c)
  locked bodies that differ in this unit: 0
score.py group cand/codex_vsync_a145     (no --claims)
speed_set: MATCH / vsync_wait: MATCH / speed_mode0_wrapper: MATCH / speed_mode1_wrapper: MATCH /
continue_prompt: MATCH (own .rodata verified at 0x80124280..0x80124284) /
func_800D6530: MATCH (own .rodata verified at 0x801241AC..0x801241B0)
```
Group = locked `src/blob/groups/codex_vsync_a145/group.c` byte-identical + new `func_800D6530.c`; same name, all
five old members kept, func_800D6530 added to members/keep/claims. No unit_overrides. func_800D63EC is NOT in the
group (prototype only): with its file as `context` the group compile placed its four inlined static helpers'
deleted-procedure stubs after speed_set ("speed_set: MISMATCH (4 extra words)"). Land func_800D63EC first as a
single (its body is called, not inlined: unit EQUAL either way).
Note (retail structure, for later naming): object_action_clear (locked, 0x800D63B8) is the free-list pop that
func_800D63EC inlines, and func_800D63B0 is an empty locked stub; the static `packet_alloc` in func_800D63EC.c is a
copy of that body.

## func_800EC190 (224 B) — in progress, 2/56 words (from 18 / 8 rows, w8c)
Traced cause of w8c's residual: `D_801163F4` is referenced by NO other function (word scan of every game target),
and its retail bytes are a zero s16 in .data → it is a **function-local static** (`static s16 D_801163F4 = 0;`).
uopt then promotes it like a local: no coloured address web (retail `lui v0; lh v0,25588(v0)` / `lui at; sh`),
written `D_801163F4++; if (D_801163F4 >= 6) D_801163F4 = 0; D_8014A118 = D_801163F4; ...` (no `v` local).
`&D_8014A118` then needs a second use to be coloured into v1 (save 0 → 0.5): compiled-out `if (D_8014A118) {}`
(non-volatile D_8014A118). Left: `li t1,2` vs our `li t0,2` (ugen temp ring one temp short).
score.py fn: `2/56 words differ` + `own .data verified at 0x801163F4 (4 bytes, all zero ...)`.
Best: `func_800EC190/best.c` (= v/u2.c). ugen trace (`SCHED=1 ugt.sh`): our tail allocates t8, t9 (line of the
increment) then t0 for the 2; retail's t1 means one more ugen temp allocated after the loop with no visible code.
Tried without change (all 2 words): holder spellings `if (D_8014A118) {}` / `== 0` / `!= D_801163F4` at 8 tail
positions, increment spellings (`++`, `+= 1`, `= +1`, pre-increment in the test, `> 5`), u8/s16/s8 store types and
casts, volatile pointer locals; holders before or inside the loop are much worse (48–53). Volatile D_8014A118 without
holder gets `li t1,2` right but the address uncoloured (3 words). Next hypothesis: a different second use of
&D_8014A118 that costs a ugen temp but no instruction (e.g. a store of a converted value), or a loop-exit temp.

## func_800A7480 (136 B) — not improved (w14z best 24/34 rows in the current unit; w9d's 17 calls a function retail does not)
pretrace: our residual's v1 is uopt web w43 = the CSE'd `(u16)` pixel expression (save 3, 2 uses) coloured v1;
retail keeps it in ugen temps (`andi t2,…,0xffff` used by both `sll` and `or`), i.e. retail's equivalent web is
not coloured. Tried (no change, 24–28): px as s32 `& 0xffff`, inline expression twice, comma form, `px | px<<16`,
inlined helpers `set_fill(u16)` / `rgba16()`; un-laundered pointer or direct `D_8017A510[index].f` is much worse
(33: as1 reorders the stores). Also open: retail forms index*72 after the base `lui/addiu` (ours before).

## func_800B3704 (228 B) — strict MATCH, `cloud/matches/func_800B3704.c`
Arcade NewBlit (LIB/blit.c), fresh from the arcade (prior: only the B53 near-miss 43/57 with m2c pointer casts).
Levers: arcade blit.h types (AnimID U32 vs AnimDTA S32 → two separate -1 constants: 40 → 14 words); the 14 left
were colour-only (`force.sh n2 func_800B3704 "p1:w14=c7"` → 0 rows: name's copy in t0, ours a3). Every caller
passes a fourth argument, so the function has four parameters; a compiled-out `if (flags) {}` keeps the a3
parameter web live over `name` (DISCLOSED shaping device).
```
blob_unit --tag w21g score func_800B3704 --with cloud/matches/func_800B3704.c --neighbours
  EQUAL func_800B3704: 57 words (kept, c_best.c)
  locked bodies that differ in this unit: 0
score.py fn cand/func_800B3704.c func_800B3704 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800B3704:
  MATCH
```
Integration: plain single, no overrides.

## func_8008AD6C (164 B) — 29/41 (28 rows), from 34; not matched
Traced (ctrace raw p2dec): the word web `w` had `forbidden0=0x50000000` (v0, a0) before any colouring — the
precoloured return value v0 of the in-arm `return 2;` overlaps w. Writing the 0x05 arm and the 0x06/0x07 arm as
`if … else if …` with ONE final `return 2;` (`best.c` = v/t1.c) gives retail's w → v0 and x → v1 (w14a's "force
declined" was exactly this interference). Left: retail keeps hi (v1, reusing x) and lo (a1) as coloured
values and the 0xFFFF0000 mask in a2 (`lui a2` at entry and again in arm 2); ours propagates hi/lo into ugen
temps. Inlined helper with (w, hi, lo) parameters: 32 (params propagated too). Next: find why retail's hi/lo
are webs (a CSE: each of `w & 0xFF`/`w & 0xFF00` used twice in the source? an IDO-unrolled 1–2 word loop?).

## func_800A1BB4 (184 B) — 35/46 strict but structure fixed except the early return (31 rows, 44 vs 46 words)
`best.c` (= v/i1.c): the loop is an inlined helper `find_enabled(index)` whose in-loop `return p` becomes retail's
`b` to the post-loop test (`bnez v1`) — w14a's "break path `beqz; nop; b exit`" residual is gone. Left: retail
returns early with its own `jr ra` (`bnez t7,cont; jr ra; nop`), ours branches to the shared final `jr ra`
(2 words short), and the registers are one web off (retail rec v0, p v1, next a1, data a2). Every spelling of
the early return tried (plain, braces, else, goto, nested, `return` at the end, rec pointer local) compiles
identically; returning `rec` is worse. Next: find what makes uopt keep a separate return block (an inlined
helper returning early in a *void* caller? a second, compiled-out check?).

## wheel_setup_initial (124 B) — not improved (w20f 27/31)
Re-scored w20f's variants in the current unit (a1/p1/p2/q2a all frame 40 vs 48) and six new loop/helper forms
(`release_matching(base, 64, resource)` inlined, `&D[i]`, `D[i].f`, do-while, helper getter): all frame 40.
ctrace (q2a): only four webs (slot s0, i s1, resource s2, constant 64 s3); retail needs a fifth coloured web that
emits no code (s4 save) and one more 4-byte slot (16..19). Callers pass one argument, so the w21g four-parameter
lever (func_800B3704) does not apply. Next: a coloured expression web (type 4, gets a spill-temp slot) with no
remaining code — e.g. an LFTR end pointer or hoisted address that is later dead.

### Combined unit check (all three new bodies together)
```
blob_unit --tag w21g score func_800D63EC func_800D6530 func_800B3704 --with cloud/matches/func_800D63EC.c \
  --with cloud/work/frontier/w21g/groups/codex_vsync_a145/func_800D6530.c --with cloud/matches/func_800B3704.c --neighbours
  EQUAL func_800D63EC: 81 words (kept, c_func_800D63EC.c)
  EQUAL func_800D6530: 88 words (kept, c_func_800D6530.c)
  EQUAL func_800B3704: 57 words (kept, c_func_800B3704.c)
  locked bodies that differ in this unit: 0
blob_unit score: 3/3 equal
```
No permission denials this session. Builder scratch: watchman2:~/rush2049/scratch/frontier/w21g (toolkit reused from wtk).
