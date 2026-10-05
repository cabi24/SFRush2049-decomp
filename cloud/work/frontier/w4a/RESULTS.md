# Frontier wave 4 — agent w4a (near-miss lane) results

Builder scratch `~/rush2049/scratch/frontier/w4a` (copied from `base` on 2026-10-05), unit tag `w4a`.
All flags `-g0 -O3 -mips2 -G 0 -non_shared`. Nothing committed or spliced.

| # | Function | Bytes | Was | State now | Deliverable |
|---|---|---:|---|---|---|
| 1 | `func_800E8D50` | 448 | 1/112 | **strict MATCH**, own .rodata verified; unit EQUAL | `cloud/matches/func_800E8D50.c` |
| 2 | `func_800EC914` | 596 | 3/149 | **strict MATCH** (also -O2); unit EQUAL | `cloud/matches/func_800EC914.c` |
| 3a | `model_data_load` | 372 | 3/93 | **strict MATCH**; unit EQUAL | `cloud/matches/model_data_load.c` |
| 3b | `model_transform_setup` | 404 | 8/101 | **strict MATCH**; unit EQUAL | `cloud/matches/model_transform_setup.c` |
| 4 | `func_80087110` | 1,780 | 4/445 | 4/445 (unchanged); mechanism now read from the as1 trace | `func_80087110/best.c` (= w1c) |
| 5 | `entity_spawn_callback` | 416 | 9/104 | **strict MATCH** (also -O2) with one QUIRK (volatile list head); unit EQUAL | `cloud/matches/entity_spawn_callback.c` |
| 6 | `func_800CB748` | 600 | 9/150 (unit) | **strict MATCH in a real group** (`--claims`); unit EQUAL | `groups/heap_release_score_insert/` |
| 7 | `audio_channel_priority` | 468 | 11/117 | 11/117 (unchanged); mechanism identified (phase-2 web order) | `audio_channel_priority/best.c` (= w2h) |
| 8 | `sound_play_menu` | 332 | 14/83 | 14/83 (unchanged); mechanism identified | `sound_play_menu/best.c` (= w2e) |
| 9a | `func_80091B00` | 168 | 11/42 (stand-in group) | **strict MATCH in a group with four real locked callers** (`--claims`); unit EQUAL (internal) | `groups/slot18_alloc/` |
| 9b | `physics_response` | 920 | 13/230 provisional | unchanged (provisional; caller unmatched); trace reading below | `physics_response/best.c` (= w2a) |

Strict: 7 functions, 3,104 bytes (448+596+372+404+416+600+168). All seven together in one unit run:

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4a score func_800E8D50 func_800EC914 model_data_load \
  model_transform_setup entity_spawn_callback func_800CB748 func_80091B00 --with cloud/matches/func_800E8D50.c \
  --with cloud/matches/func_800EC914.c --with cloud/matches/model_data_load.c --with cloud/matches/model_transform_setup.c \
  --with cloud/matches/entity_spawn_callback.c --with cloud/work/frontier/w4a/groups/heap_release_score_insert/func_800CB748.c \
  --with cloud/work/frontier/w4a/groups/slot18_alloc/func_80091B00.c --neighbours
  EQUAL func_800E8D50: 112 words (kept, c_func_800E8D50.c)
  EQUAL func_800EC914: 149 words (kept, c_func_800EC914.c)
  EQUAL model_data_load: 93 words (kept, c_model_data_load.c)
  EQUAL model_transform_setup: 101 words (kept, c_model_transform_setup.c)
  EQUAL entity_spawn_callback: 104 words (kept, c_entity_spawn_callback.c)
  EQUAL func_800CB748: 150 words (kept, c_func_800CB748.c)
  EQUAL func_80091B00: 42 words (internal, c_func_80091B00.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/7 equal
```
(Unrelated, pre-existing: a single `--with` run that names `func_800F0674` reports a `.bss` overlap for it
whatever the candidate is, including a non-matching one; it is not caused by these files.)

## Tools (all in `tools/`, builder side under the w4a scratch)

- `build_uopt.sh`, `ctrace.sh`, `pdiff.sh`, `force.sh`, `sum.sh`, `tr.sh`, `us.sh` — w3a's instrumented-uopt kit
  re-pointed at w4a.
- **`gtrace.sh GROUPDIR LABEL [PROC]`** (new): the same colouring trace for an `-O3` *group* build (score.py's
  cc/uld/usplit/umerge steps), for functions the unit cannot hold (provisional, caller unmatched).
- **Traced as1** (new): the toolkit's recompiled `as1` aborts on `-R` because `wrapper_printf` is unimplemented.
  `patch_as1_printf.py` adds a general printf to `libc_impl.c`; the rebuilt `~/rush2049/scratch/frontier/w4a/as1/as1`
  prints the scheduler's DAG (`Node N: inst …, lineno …` + `aftercycles`) and every `Picking node` decision.
  **Fidelity:** its object is `cmp`-identical to the stock as1 on the single listings and on the whole unit `gen`.
  `asr_remote.sh FILE.s LOG` (listing → traced as1), **`ulist.sh NAME CAND.c LABEL`** (unit score, then ugen `-l`
  listing of NAME into `ul_LABEL/fn.s` and the whole-unit as1 trace into `ul_LABEL/as1r.log`).
- `o3s.sh`/`asmt.sh` (w1b/w2g pre-as1 listing and "assemble an edited listing" tools) re-pointed at w4a, and
  `batch.sh NAME FLAGS FILES…` (standalone batch scoring).

---

## 1. func_800E8D50 — strict MATCH

```
sc.sh cloud/matches/func_800E8D50.c func_800E8D50 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800E8D50:
  MATCH
    own .rodata verified at 0x801244C0..0x801244C4
blob_unit --tag w4a score func_800E8D50 --with cloud/matches/func_800E8D50.c   ->  EQUAL func_800E8D50: 112 words
```
**Arcade ancestor found: `game/camera.c UpdateCarObj()`** (follow camera; `elasticity = .6 * elastic_factor`,
`fcam.mat3.pos[i] = (fcam.mat3.pos[i]*elasticity + (carpos[i]+res[i])*(1.0-elasticity))`, `rpos = carpos - campos`,
`LookInDir`). Pasting that expression closed it: the elasticity is re-read from the global `D_80152708[slot]` in
both factors and the sum is `(carpos[i]+res[i])`. The named `inverse`/`weight` locals of every earlier attempt
were what made uopt order the `mul.s` operands sum-first; 110+ operand-order variants could not reach it.
Literal `0.6f` = 0x3F19999A (verified by the scorer). N64 extras: modes 3/4/5 of `D_8002EB98` scale by
.75/.6/.5; optional matrix forwarded to `func_800E8CB8` first.

## 2. func_800EC914 — strict MATCH

```
sc.sh cloud/matches/func_800EC914.c func_800EC914 --flags "-g0 -O3 …"   ->  func_800EC914:  MATCH   (-O2: MATCH)
blob_unit … --with cloud/matches/func_800EC914.c   ->  EQUAL func_800EC914: 149 words
```
Arcade `munge_gLink_data()` (w1h). Residual lane was as1, and it was **memory ordering, not line/list order**:
on the ugen listing, deleting the single directive `.noalias $13,$12` (num_humans vs num_drones store) gives
retail's schedule (found by editing the listing: la placements ×120, `.loc` ×125 — none moved; alias edits did).
The source form: **`D_80152768` (num_drones) and `D_80153FD2` (num_humans) are `volatile`** (both `sh` get
`.set volatile`, so as1 keeps their order). `D_80153FD2` is already `volatile` in two matched files
(`func_80105B74`, `func_800F8EC8`), which corroborates it; only one volatile of the two does nothing.

## 3. model_data_load + model_transform_setup — both strict MATCH (same fix)

```
sc.sh cloud/matches/model_data_load.c model_data_load …        ->  model_data_load:  MATCH
sc.sh cloud/matches/model_transform_setup.c model_transform_setup …  ->  model_transform_setup:  MATCH
blob_unit: EQUAL model_data_load: 93 words / EQUAL model_transform_setup: 101 words
```
Trace (model_data_load, proc 22): the loop child web (`c`, w33) took a3 because it shares a basic block with the
incoming index `a` (a0). **A compiled-out diagnostic after the flag update** — `if (a < 0) DEBUG_PRINT((…));` —
ends the block, the child is loaded straight into a0, and the `move a0,a3` disappears. In the Show twin the same
statement in **both** child-walking arms (EACHCHILD and RECURSIVE) also fixes the constant swap: before it the
0x80000000 web (save 5.5) beat the `1` web (5.0) to v1; `force.sh … "p1:w50=c2,p1:w53=c5"` proved that swap was the
whole remaining residual, and the second check (EACHCHILD arm) is the natural source of it. Applying the same two
checks to the Hide twin keeps it matching, so both are written identically. The condition is not recoverable: any
test of `a` or `t` gives the same code (`a == -1`, `v == 0` and flag tests do not).

## 4. func_80087110 — 4/445 (unchanged), mechanism now known

As1 trace of the listing (traced as1, identical object): the stretched x-flip arm's fall-through block is scheduled
by `aftercycles` (critical path); `lui v1` (the `la D_80149438`) has 34, the `texture_edge = s + right` `addu` 30,
so `lui` is first and goes into the `beqz` delay slot. Retail puts the `addu` there. In the other arms the rule is
visible in retail itself: when the branch target **reads v1** (the both-flags arm's target `beq $3`), `lui v1` is
ineligible for the slot and as1 hoists the `addu` (seen as a re-scheduled `[addu, beq]` block in the trace's
second pass). So in retail, as1 believed v1 was live at 0x8008768c. Not the cause (all tested on the listing):
`.loc` lines (125 combos), `la`/pair placement (28), `.noalias`/`.alias` edits, alias of the display-list pointers.
Forcing a read of `$3` at the target gives `beqzl` with the target's first instruction instead — another fill path.
**Next:** find what in retail makes `$3` live at the `D_8012E608 & 8` arm (a use of the `flags & 4` value, or of the
display-list address register, in that arm or after the chain); the trace tool now answers each try in one run.

## 5. entity_spawn_callback — strict MATCH, with one QUIRK

```
sc.sh cloud/matches/entity_spawn_callback.c entity_spawn_callback …  ->  MATCH   (-O2: MATCH)
blob_unit … -> EQUAL entity_spawn_callback: 104 words
```
Trace (proc 87): w1a's `if (0) {}` adds one block to `idx`'s live range (save 1.2 over 5 blocks → 1.0 over 6), so idx
is split to its home slot as in retail; written as `if (SCENE_DEBUG) { }`. The remaining 9 words were the list head
`&D_8015B254` in v0 (`lui; addiu; lh 0(v0) … sh 0(v0)`) and the else-block reload of idx: **`volatile s16
D_8015B254`** gives both. QUIRK, stated in the file header: the locked `render_mode_select` matches with a plain
`extern s16 D_8015B254` and is 1 word off with volatile (with a head temp), so this declaration is not proven; no
non-volatile spelling was found (w1a ~80 variants; here operand order, `*&`, compiled-out reads of the head).
`if (freeSiblings) ;` at the top works as well as `if (0)`.

## 6. func_800CB748 — strict MATCH in a real group

```
score.py group cand/<groups/heap_release_score_insert> --claims
Members:
func_800CB748:
  MATCH
blob_unit … --with groups/heap_release_score_insert/func_800CB748.c --neighbours
  EQUAL func_800CB748: 150 words (kept, c_func_800CB748.c)   locked bodies that differ in this unit: 0
```
As1 trace of the unit: the residual block ties on `aftercycles`, and **as1 breaks ties by source line**: the inlined
release helper's statements carried line 62 (helper defined above the caller), the caller's handle spill
`sw v0,40(sp)` line 88, so the spill sank into the `jal` slot. **Defining `static release_handle()` after the caller**
(forward declaration first) gives retail's order: EQUAL. The two unused locals (`j`, `k`, w2a's pads) are still
needed for the 72-byte frame. Group = locked `src/blob/groups/codex_heap_release_a25` (all files unchanged) + this
file (internal callee `audio_reverb_update`). Without `--claims` the context member `audio_effect_process` reads
`MISMATCH (1 extra words)`: in the group object the deleted static leaves an unnamed `jr ra; nop` after it; the same
happens with w2a's helper-first source, and in the unit nothing differs. **Integrate** by adding the file to
`codex_heap_release_a25` (members/files/keep) or replacing that group with this directory (drop `claims`).

## 7. audio_channel_priority — 11/117 (unchanged)

Trace (unit proc 37): every web is coloured in **phase 2** (no pressure), i.e. in web-number order, each taking the
lowest free register; web numbers follow first occurrence by block. Retail's colours imply the order
outer, weights, row, base, handles, slot, id, index. Using the parameter directly as `id` (w2h `stride.c`) already
puts id after slot (its first use is the compare block), but weights (an expression web, first occurrence in the
if-block) sits after id. Computing `weights` at the top of the row loop (`acp/w/rowloop.c`, 14 words) moves it before
slot but still after row. **Next:** a source in which the weights web is created between outer and row (bb0/bb1)
without moving its code out of the if-block.

## 8. sound_play_menu — 14/83 (unchanged)

Trace (unit proc 555): the new block `n` is two webs: a type-4 expression web (`b + bs - size`, w56) used by the
`n->next` link updates, coloured a1, and the variable `n` (w52) from the join on, a0; hence the `move a0,a1` and the
lost `move a1,zero` hoist. Tested without movement: 15 spellings of the split test and of `n`, 4 link-update forms,
compiled-out reads of total/largest after the loop. **Next:** a form in which `n`'s definition is not copy-propagated
into the link blocks (e.g. `n` an existing variable, as w2e suggested), traced with `ctrace.sh … 555`.

## 9a. func_80091B00 — strict MATCH in a group with real callers

```
score.py group cand/<groups/slot18_alloc> --claims
Members:
func_80091B00:
  MATCH
Context: object_type7_create, object_type1_create, func_800D6348, sync_entry_register: MATCH
blob_unit … --with groups/slot18_alloc/func_80091B00.c --neighbours
  EQUAL func_80091B00: 42 words (internal, c_func_80091B00.c)   locked bodies that differ: 0
```
In the unit the four-wide ring was already right; the residual was as1's order inside each unrolled copy. On the
listing, marking the `sb` and the `sh` `.set volatile` gives retail exactly; in source **`extern volatile Slot18
D_80142DD8[128]`** (or the two fields volatile). The group holds only real, locked callers (copied unchanged from
`src/blob`), so this is not provisional. **Integrate:** three locked groups (`codex_entity_helpers_a10`,
`codex_transform_b109`, `entity_lookup`) carry a non-volatile *context* copy of `func_80091B00`; when landing, make
the unit prefer this definition (`prefer_definition`) and run `blob_unit check`.

## 9b. physics_response — 13/230, provisional (unchanged)

`gtrace.sh` on w2a's group: with `cur = next` before the `next == last` test (retail's branch shape) `cur` (w22,
save 132 → 110, nocs 5 → 6) starts to interfere with w186 (the hoisted inner-loop load, v1, save 4700) and drops to a1.
Retail has `cur` in v1, so in retail that hoisted value does not overlap `cur`'s extended range. Still provisional
until `audio_effect_apply` matches.

## Struct layouts and global types recovered

- `D_80152708[]`/`D_80152720[]`: per-slot camera elasticity / elastic factor (f32); `D_80150B70[]` 152-byte slot
  camera records (`Mat3 @96`, `pos[3] @132`); `D_8014A914[car].status @0` = resurrect moving_state (s16).
- `D_80152768` num_drones and `D_80153FD2` num_humans: `volatile s16`.
- `D_8015B254` scene list head: `volatile s16` in entity_spawn_callback (quirk, see §5).
- `D_80142DD8[128]`: 24-byte slots {s16 id @0; s8 @2; s8 used @3}, `volatile` (allocator `func_80091B00`).
- Object record `D_8012E700[]` (0x44): flags@0, id@0x14, child@0x16, sibling@0x18 (unchanged).

## What generalises

1. **Paste the arcade expression before searching operand orders.** `func_800E8D50` was one `mul.s` operand off
   after 110 variants; the arcade statement verbatim (global re-read in both factors, no named `inverse`) matched first try.
2. **as1 residuals can be decided by memory facts, not only lines.** Edit the ugen listing (`asmt.sh`): if deleting
   a `.noalias` or adding `.set volatile` around two stores gives retail, the source has `volatile` globals.
   Two of this wave's closes (`func_800EC914`, `func_80091B00`) were that, plus `entity_spawn_callback`'s head.
3. **as1 tie-break is the source line.** When two ready nodes tie on `aftercycles`, the lower line wins; an inlined
   static's statements carry the helper's own line numbers, so *where the helper is defined* (above or below its
   caller) is a scheduling lever (`func_800CB748`). The traced as1 (`ulist.sh`) shows this in one run.
4. **as1 delay-slot fill:** the first scheduled instruction of the fall-through block goes into the slot unless its
   destination is live at the branch target; then as1 hoists another instruction (re-scheduled block in the trace).
5. **Phase-2 colouring (no register pressure) is web-number order, lowest free colour.** Then "save" is irrelevant;
   what matters is the block where each web first occurs (audio_channel_priority).
6. **Compiled-out checks split blocks.** `if (x) DEBUG_PRINT(...)` between two statements ends a basic block, which
   separates live ranges that uopt would otherwise treat as interfering (model_data_load/transform_setup).
