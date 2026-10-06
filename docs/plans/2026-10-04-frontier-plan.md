# Frontier plan: matching the game image in call-graph order

> **Current status and how to run the next wave:** see [2026-10-06 frontier handoff](2026-10-06-frontier-handoff.md) (after wave 12).

**Written:** 2026-10-04, on `master` at `abc0f256` plus the uncommitted files listed in [Appendix B](#appendix-b-what-exists-uncommitted).
**For:** whoever continues this work (written to be followed step by step by an agent that has not seen the session that produced it).
**Scope:** the compressed game image only (540 unmatched functions, 457,224 bytes). Static cartridge code is out of scope.
**Relation to other plans:** this replaces the *target selection* and *readiness* parts of [PROJECT_PLAN.md](../../PROJECT_PLAN.md) and [dot_handoff.md](../../dot_handoff.md). Their acceptance gates (image hash, exact compressed stream, full-ROM SHA-1) are unchanged and still mandatory. Where this plan and an older one disagree about what to work on next, this one wins; where they disagree about what counts as accepted, the older gates win.

Every number below was measured on 2026-10-04 and has a script or command behind it. Re-measure before relying on one: `python3 -m tools.conveyor.pipeline.frontier report` recomputes from the current lock.

---

## 0. Status at the end of 2026-10-04 (read this first)

Sections 1–5 are the plan as first written; this section says what has since been done and what changed. Where they disagree, this section is newer.

**Coverage:** game code 737 / 1,216 functions, 137,844 / 647,072 bytes (21.30%), up from 676 / 103,784 (16.04%) at the start of the day. ROM SHA-1 exact (`blob_rom rom`), lock checks clean, shadow gate 737 of 737, `pytest tests/conveyor tests/cloud` green. **Nothing is committed** — the first action for whoever continues is to ask the owner for a commit of this ROM-exact state.

| Workstream | State | What exists |
|---|---|---|
| A. Land wave 0 | done | 14 spliced from wave 0, then 47 more from wave 1 (61 functions, 34,060 bytes in the day) |
| B. Own rodata | done for the splice paths; scorer patch waits on the owner | `tools/cloud/owndata.py`; `blob_splice.link_function` verifies and places own `.rodata`/`.data`; `cloud/work/frontier/rodata/score_py.patch` (hook-protected file, owner applies); `asm/us/blob_data/` (untracked, owner decides) |
| C. Whole-program unit | phases 1, 2, 4 done; 3 and 5 open | `python3 -m tools.conveyor.pipeline.blob_unit check` (≈4 s) and `… score NAME --with cand.c`; overrides in `src/blob/unit_overrides.json`. Not yet called from the splice path: **run `blob_unit check` by hand after every splice** |
| D. Matching loop | wave 1 done (8 agents, 47 assigned functions) | results under `cloud/work/frontier/w1{a..h}/RESULTS.md`; shared brief `cloud/work/frontier/WAVE_BRIEF.md` |
| E. Type model | step 1 proposed | `include/game_globals.h` (generated proposal, not included by anything), `tools/conveyor/pipeline/shared_decls.py`, `cloud/work/frontier/type_model/decl_conflicts.md`, `names_audit.md` |
| F. Tools | done | `frontier` gained `assign`, `stubs`, `calibrate`, provisional set, temp-ring detector; `cloud/work/frontier/tools/splice_singles.py` |

**Group splice path fixed (same day):** `blob_group` now falls back to per-reference own-data placement and verification (via `owndata.verify`, one member at a time) when a group's members are not address-adjacent; details and the reviewer checklist are in `cloud/work/frontier/grouprodata/README.md`. The two groups that waited on it are spliced (`frontier_skid_marks`, and `camera_scene_manager` extended by four members). One assertion was deliberately weakened: a wrong jump table in an unspliced *context* function no longer refuses the members.

### Wave 3 (2026-10-05) — read before the next wave

- **Coverage after wave 3:** game 796/1,216 functions, 188,316 bytes (29.10%); static 379/668, 69,680 bytes (43.35%, after promoting 52 boot-tail functions from PRs #82/#83). Combined 31.94%.
- **Traced register allocator.** The hub lane built an instrumented IDO `uopt` (byte-identical output with tracing off) that prints why each variable got or lost a register: `cloud/work/frontier/w3a/tools/` (`build_uopt.sh`, `ctrace.sh`, `pdiff.sh`, `force.sh`). It closed all three hubs from natural source after earlier agents had stopped on blind variants. **Use it on every allocation residual before trying variants.** Priority = savings / live blocks: shortening a live range (moving an initialisation past an `if/else`) is the natural form of most "dead read" fixes.
- **Compiled-out debug code is source structure.** `if (cond) DEBUG_PRINT(...)` with an empty macro keeps `cond`'s variables alive and emits nothing. Signs: a parameter never read, a stack store never reloaded, a register barred for no visible reason.
- **Paste the arcade function first** (it matched `func_800BFBE8` on the first compile); **caller-less stubs are deleted helpers** (closed `func_800E681C`, `func_800F8EC8`, `sound_bank_unload` progress) — pass `--internal STUB` to `blob_unit score` and add a `prefer_definition` override when landing.
- **Game `.bss` is owned** (`tools/cloud/owndata.py`, design in `cloud/work/frontier/bss/README.md`): function-local statics splice; `blob_unit check` is the only cross-function overlap check.
- **Integration tools:** `cloud/work/frontier/tools/splice_singles.py` and `install_group.py` (supersedes and restores on failure). Never push before `pytest tests/conveyor tests/cloud` shows 0 failures; CI rescores every changed `cloud/matches/*.c` standalone (keep group-only members out of it; bare `/* flags: ... */` on line 1).

### How to integrate a batch (the procedure that worked all day)

```bash
# singles: flags are read from line 1 of each file; refused ones are reported and left out
PYTHONPATH=. python3 cloud/work/frontier/tools/splice_singles.py NAME [NAME=path/to/best.c …]
# groups: copy group.json + its files to src/blob/groups/<name>/ (drop "claims"), then
python3 -m tools.conveyor.pipeline.blob_group splice <name>
# every time, in this order
python3 -m tools.conveyor.pipeline.blob_group check && python3 -m tools.conveyor.pipeline.blob_splice check
python3 -m tools.conveyor.pipeline.blob_unit check        # shadow gate: catches what the image gate cannot
python3 -m tools.conveyor.pipeline.blob_rom rom           # must end MAKE=0 TEST=0
make progress
```

- Code-identical results with own literals (`MATCH (N section-relative relocations unverified …)` from the unpatched scorer) can be spliced directly: the splice verifies the literal bytes against the image. It caught a wrong literal on the first day (`9.5493f` where retail is `30/3.14159`), so trust a refusal.
- A single-function file may define an inlined helper; the helper may come first (`-O3` emits it first anyway).
- When a new group supersedes an old one, `blob_group revert <old>` (or `blob_splice revert NAME` for a single) first, splice the new group, delete the old directory, run the checks. Keep a copy of `blob_matched.lock.json` before starting.
- A function that fails only in `blob_unit check` after passing the image gate means **a locked callee's source is wrong** (it happened with `func_8008B3F4`: fake extern literal, inlined by its caller). Fix the callee's source and re-splice it; do not add an override.
- If members of a new group are emptied or inlined away in `blob_unit check` although the group itself matched, a caller-less stub the group defines for real (a deleted static) is being resolved to its old locked empty definition: add a `prefer_definition` entry pointing at the group file (`func_800AFD54`, `func_800AFB30`).
- A function whose size changes by a word in the unit and that ends in an endless loop needs an `align` entry in `src/blob/unit_overrides.json` (`task_complete_signal`, `audio_queue_process`).
- Research packets pin lock state in tests (`tests/conveyor/test_dot_d08_scout.py`): when a splice makes one fail on `accepted_lock_present`, update that field in the packet's `verification.json` and nothing else.

### What wave 1 changed in the method (fold into every brief)

1. **Hit rate.** Of 47 assigned functions: 28 strict, 8 code-identical (all spliceable now), 9 provisional or in groups waiting on a caller, and a short list of true near-misses. Frontier `single` functions around 0.5–1 KB match in one pass far more often than not.
2. **Flags.** Everything is `-O3`. About half also match at `-O2`; none needed `-O2`.
3. **Source form beats search.** The recurring closers, roughly in order of yield: the arcade function's *declaration list and literal spellings* (unused locals, int `0`/`1` vs `0.0f`/`1.0f`, `.025f` vs `0.025f` are distinct constants); one C variable = one register (split or reuse variables to move registers); typed struct fields instead of cast macros; SDK GBI macros; `?:` vs a named local for frame slots; statement and line layout for `as1` delay slots; `volatile` where retail shows `lui; addiu; l? 0(reg)` on a single read.
4. **Deleted statics are the commonest hidden structure.** A caller-less `jr ra; nop` stub next to a function (`frontier stubs`, `frontier show`) is an inlined function. Writing it as a real non-static function named after the stub, left out of `keep`, reproduces the stub at the retail address. An 8-byte-too-small frame or an "extra move" residual usually means one is missing. This is direct evidence for D3.
5. **A matched callee is not settled until its callers match.** Three locked sources were replaced because callers proved them wrong (wrong signature, hand-built display-list words, fake extern literal). Argument set-up order at a call site is the callee's source parameter order.
6. **Large functions (1.4–3.6 KB)** reach a structurally right body in minutes; the rest is register colouring and `as1` scheduling. Use the pre-`as1` listing (`cloud/work/frontier/w1b/o3s.sh`, `w1c/klist.sh`), the opcode-aligned metric (`w1b/bscore.py`; positional word counts mislead above ~300 words) and a choice-point generator (`w1b/net_state_validate/gen.py`) rather than hand variants.
7. **Tool caveats.** `agentC/full.py` crashes on `nop` runs (use the copies in `w1a`/`w1b`/`w1f`, which pass `-z`). The frontier's `group` label has false positives (`entity_spawn_callback` is plain ABI) and its `single` label has false negatives for FP callee-saved registers (`$f20`–`$f30` unsaved, e.g. `func_800E3430`): extend `unsaved_callee_writes` to FP registers.

### Open near-misses (each has `best.c` and notes; do not restart from scratch)

| Function | Bytes | Residual | Where | Next step recorded |
|---|---:|---|---|---|
| `func_80087110` | 1,780 | 4 words, one `as1` delay-slot pick | `w1c` | instrumented `as1`; ~600 layout variants exhausted |
| `func_800EC914` | 596 | 3 words, `as1` order of two `la` | `w1h` | none well supported |
| `model_data_load` | 372 | 3 words | `agentB` | entry pointer as one web across modes 2 and 3 |
| `entity_spawn_callback` | 416 | 9 words | `w1a` | run workbench `diagnose` first (not yet done) |
| `func_800E681C` | 716 | 16 words, one register permutation | `w1d` | rewrite the `goto pick` join; closes provisional `func_800E6460` |
| `entity_process_main` | 1,424 | 20 words | `w1b` | one more variable live across the tail (arcade `xlu`) |
| `net_state_validate` | 2,728 | 27 words | `w1b` | extend `gen.py` with section-4 choice points |
| `entity_lod_select` | 716 | 36 words | `agentA` | emptied statement before the `op == 1 ||` chain |
| `func_800F7F3C` | 1,396 | 117 words | `w1c` | needs a uopt web trace |
| `net_session_update` | 3,568 | ~85 unaligned | `w1b` | choice-point generator as for `net_state_validate` |
| `camera_scene_manager` | 2,456 | 452 words | `w1g` | three more inlined siblings (stubs `func_800C2418/2420/2428`) |
| `controller_poll` | 928 | 225 words | `w1g` | retail never keeps the queue address in a register |
| `physics_float_calc` | 1,176 | 210 words | `w1e` | whole body is a static called once from a thin wrapper |
| `func_80091B00` | 168 | 11 words in a stand-in group | `agentB` | ring plus `as1` reorder |

Provisional results (proven with stand-ins or waiting on a caller; never coverage) are in `cloud/work/frontier/provisional.json`: 9 functions, 7,164 bytes.

### Next wave

```bash
python3 -m tools.conveyor.pipeline.frontier report
python3 -m tools.conveyor.pipeline.frontier assign --agents 8 --per 7 --skip-in-flight   # disjoint batches
```

Give each agent `cloud/work/frontier/WAVE_BRIEF.md`, a batch and a name; integrate with the procedure above as results arrive. Before launching, refresh the builder base copy (Appendix A) so agents see the current `src/blob`. Callee-ready now: 181 functions, 120,676 bytes (104 `single`, 51 `group`, 1 `unit`; the rest are provisional or need a caller).

### Owner decisions still open

D1 (provisional tier — the tool support exists, the policy is yours), D2 (`-O3` as the recorded flagset), D3 (the 159 stubs — wave 1 gives direct evidence they are deleted inlined functions), plus four from workstream B: apply `score_py.patch`; track `asm/us/blob_data/` (86 KB of retail data as text; a 4.5 KB rodata-only variant covers literals and jump tables but not local statics); whether an all-zero local static counts as verified; add `tools/cloud/owndata.py` and `asm/us/blob_data/` to the protection hook. And from E: the 16 open type conflicts in `decl_conflicts.md`, and `gstate` bound to two addresses.

---

## 1. The model in one page

1. **The game image is one whole-program IDO 5.3 `-O3` unit.** Tested: all 676 locked bodies compile together as one 622-file `uld -kp` unit and 675 are byte-identical (676 with files linked in descending address order). All 543 "standalone `-O2`" bodies also match alone at `-O3`. Evidence: [wholeprog/README.md](../../cloud/work/frontier/wholeprog/README.md).
2. **Most functions are *kept* (externally visible) and do not react to their neighbours; about 100 of the 676 are *internal*** (non-ABI register parameters, unsaved callee-saved registers, a four-wide `t6`–`t9` temp ring, callers that keep values across the call). An internal function only reproduces with its real callers in the unit. Internal is equivalent to C `static` (tested on all 48 single-file groups).
3. **`umerge` inlines** any callee of about two statements at every call site, and any internal callee with one call site. An inlined callee leaves no `jal`, so the call graph cannot see it. A deleted internal procedure leaves a `jr ra; nop` stub: retail has 159 caller-less stubs, currently locked as empty functions; most are probably the remains of inlined statics (inferred from the zlib range, where it is exact).
4. **`.rodata` is per function, in function order** (float literals and jump tables, 4,480 bytes at `0x80123870`, one owner per word). **`.data` is per translation unit** in a different link order, with 317 shared objects. Evidence: [type_model/REPORT.md](../../cloud/work/frontier/type_model/REPORT.md) §1.
5. **So the order of work is bottom-up over the call graph**, and the unit of proof is "this body, compiled in the whole-program unit, equals the retail words". The frontier tool produces that order.

What this means in practice, from the first wave (20 frontier functions, three agents, about 25–60 minutes each):

| Outcome | Functions | Bytes | Cause |
|---|---:|---:|---|
| Strict `MATCH` | 11 | 3,664 | Natural typed C; 3 of them only at `-O3` |
| Code identical, own-literal references unverified | 3 | 1,476 | Scorer cannot check a function's own `.rodata`/`.data` words |
| `MATCH` only with stand-in callers in an `-O3` group | 4 | 1,084 | Internal function; real callers are unmatched |
| Near-miss | 2 | 1,088 | `model_data_load` 3 words, `entity_lod_select` 36 words |

Missing struct types were **not** what stopped any of them. Flags, literal ownership and whole-program context were.

---

## 2. Rules that do not bend

- Only **strict `MATCH`** from `tools/cloud/score.py` is matching evidence. `MATCH (N … unverified)`, a relocation-blind zero, an aligned-diff zero or a workbench verdict is a lead.
- Cartridge coverage changes only through `blob_splice`/`blob_group` → `blob_rom rom` with the image gate and the built ROM's SHA-1. Use the [promotion skill](../../.claude/skills/promote-match/SKILL.md).
- Never edit `asm/us/blob/**`, `*.lock.json`, `us.sha1`, `src/blob/blob.ld` or `tools/cloud/score.py` to make something pass. Changing the scorer's *capability* (workstream B) is a reviewed code change with tests, including a test that a wrong literal fails.
- A body proven with **stand-in callers is not spliceable** and is not coverage. Record it as provisional (see D1).
- Names in this repo are historical labels. `MP_TargetSpeed` is a message-queue lock wrapper; `gMainGameStruct` is an `OSMesgQueue`; `entity_name_copy` is `bsearch`. Never infer semantics, a struct or an arcade donor from a name. Anchor from code (§7).
- Say which quirks a match depends on (unused local, `volatile`, dead read, literal type) in the source header comment.
- IDO runs only on the builder. Never work in `watchman2:~/rush2049/repo`, `/tmp/blobsplice` or `/tmp/blobgroup` by hand; use a scratch copy ([Appendix A](#appendix-a-builder-scratch-setup)). The box is shared: stay under about 4 cores per agent.
- Commit and push only when the repository owner asks.

---

## 3. Workstreams, in order

A and B are small and unblock everything else. C is the structural change. D is the matching loop and runs continuously once A lands. E and F run alongside D.

### A. Land wave 0 — DONE 2026-10-04

**Result:** all eleven spliced; image gate and built-ROM SHA-1 exact (`blob_rom rom`: `MAKE=0 TEST=0`). Game code 676 → 687 functions, 103,784 → 107,448 bytes (16.61%). Not committed.
Two things came up that matter for later batches:
- `blob_splice.link_function` placed a 16-aligned input section above the target address, so a **recursive** function's self-`jal` resolved 8 bytes late (`display_list_traverse`). Fixed with an explicit output address plus `SUBALIGN(4)`; all 687 bodies re-pass the image gate.
- The `blob_splice` CLI cannot take a flagset; `-O3` singles were spliced by calling `blob_splice.splice(conn, names, source_for, flagsets={name: flags})`. `Input_ApplyPadConfig` went through `blob_group` as `src/blob/groups/frontier_pad_config` (member) with the already-locked `Input_InitPadHandlers` as context.

The original steps are kept below as the procedure for later batches.

Eleven strict matches sit unspliced in `cloud/matches/`:

| Function | Bytes | Flags | Note |
|---|---:|---|---|
| `audio_channel_alloc` | 676 | `-O2` | |
| `func_8008A148` | 580 | `-O2` | GBI macros on the global `Gfx *` |
| `display_list_traverse` | 468 | `-O2` | under 109 unmatched dependents |
| `menu_load_options` | 364 | **`-O3`** | unused local supplies 8 frame bytes |
| `entity_name_copy` | 296 | `-O2` | libc `bsearch`; one unused local |
| `random_int` | 284 | `-O2` | |
| `func_8008C768` | 268 | **`-O3`** | `atan2f`; int `0` vs `0.0f` mix matters |
| `entity_cull_check` | 256 | `-O2` | one code-free shaping assignment |
| `Input_ApplyPadConfig` | 192 | **`-O3`** | needs `Input_InitPadHandlers` in the same file (inlined) |
| `func_800947F0` | 152 | `-O2` | address-form read, two `volatile f32` |
| `func_80096C28` | 128 | `-O2` | |

Steps:

1. Re-score each one in a fresh builder scratch copy (Appendix A). Each must print `MATCH`. The first line of each file gives its flags.
2. Splice through `blob_splice` (singles) with the flagset that scored. Read `blob_splice.splice` first: confirm how `--from` picks the flagset, because three of these need `-O3`. `Input_ApplyPadConfig` carries a second function definition in its file; if the single path rejects that, splice it as the real two-member group in `cloud/work/frontier/agentB/Input_ApplyPadConfig/group/`.
3. `python3 -m tools.conveyor.pipeline.blob_rom rom`, then `make progress`. Expect game functions 676 → 687 and game bytes +3,664.
4. `python3 -m tools.conveyor.pipeline.frontier scan` and note what moved to layer 1.

**Done when:** image gate and ROM SHA-1 pass with the eleven locked, or each one that failed has a recorded reason. Do not batch-fix a gate failure by editing sources that already scored; diagnose the link difference (absolute data symbols are the usual cause — see "Codex acceptance findings" in [cloud/PLAYBOOK.md](../../cloud/PLAYBOOK.md)).

### B. Let a function own its rodata

**Problem.** `score.py` reports a reference to the object's own `.rodata`/`.data` as unverified, and `blob_splice.link_function` never places the object's `.rodata`. The workaround is spelling literals as `extern f32 D_8012xxxx`, which only works when the load is outside a loop (inside a loop the extern's address is hoisted and the code changes). The group path already solves this (`blob_group._local_data_bases`, `_jump_table_windows`).

**Blocked on it today:** `camera_blend_between`, `func_800DE860`, `func_800EC270` (code identical now); every unmatched function with a jump table that is not in a group (29 functions, 45,276 bytes); and natural literals in 132 float-only owners (163,052 bytes).

**Work.**

1. In `score.py`, for each section-relative `HI16`/`LO16` pair into the object's own `.rodata`/`.lit4`/`.lit8`: take the address the *retail* words encode at that site, read the retail bytes there, and compare with the object's bytes at the relocation's section offset. Equal → verified. For jump tables, compare every entry after mapping label offsets to the function's image address (port `_jump_table_windows`).
2. Own `.data` (the `func_800EC270` function-local `static`): same check against the retail `.data` bytes at the encoded address. Report it as a distinct class (`own .data verified at 0x…`), because `.data` is TU-owned and the address is evidence about TU layout worth keeping.
3. Mirror the same placement and verification in `blob_splice.link_function`, so a single-path splice of a function with literals passes the image gate without fake externs.
4. Tests in `tests/conveyor/test_cloud_score.py`: a correct literal verifies; a wrong literal value fails; a wrong jump-table entry fails; a literal at the wrong address fails. The existing "wrong global / wrong addend / wrong callee" tests must still fail.

`cloud/work/frontier/type_model/rodata_owners.json` has the per-function rodata windows and is a useful cross-check, not the authority (the retail words at each site are).

**Done when:** the three blocked functions print strict `MATCH` from natural-literal source, the negative tests fail as designed, and all 676+ locked bodies still score as before.

### C. One whole-program build as the shadow gate, then the canonical build

**Why.** It is the real recipe; it removes the stand-in problem structurally; and it makes "does adding this body break a neighbour" a 5-second check.

**Tested facts to build on** ([wholeprog/README.md](../../cloud/work/frontier/wholeprog/README.md)): the unit builds in about 5 s (`cc -j` over 622 files 4.5 s, `uld`…`as1` 0.3 s). It needs a manifest that no single rule derives:

- the internal set (about 100 functions; the union of the 70 groups' keep decisions gets 658/676),
- 9 inline blockers (tiny functions retail still calls by `jal`; a dead `if (0)` block) → 669,
- 3 forced keeps → 672,
- one definition per function (72 names are defined in more than one file; `func_800BF01C` is locked as `(void) {}` but its callers need the one-parameter version) → 675,
- descending-address link order → 676.

**Phases.**

1. **Manifest.** Generate `src/blob/unit.json` (or similar) from the lock and group specs: files in link order, internal set, blockers, forced keeps. Start from `cloud/work/frontier/wholeprog/results/` (`best_keep.txt`, `block_ab_groups.txt`) and `wp.py`. Keep it generated; hand-edits go in a small overrides file with a reason per line.
2. **Shadow gate.** A command (suggested: `python3 -m tools.conveyor.pipeline.blob_unit check`) that builds the unit on the builder and reports per-function equality against the image. Wire it to run after every splice. It fails if any locked body stops matching in the unit. It does not yet feed the ROM.
3. **Resolve duplicates at source.** The 57 locked bodies that groups redefine as context: each group should reference the one real definition. Fix `func_800BF01C`.
4. **Fast per-function scoring in unit context.** `score.py` on the whole unit takes about 42 s; add a mode that scores named members only.
5. **Promote.** Feed the unit object to the existing per-slice relocation and image gate (`blob_group.relocate` already does per-slice placement), prove the image hash and ROM SHA-1, then retire per-group builds. Untested so far: data-section order in a merged unit, and whether the unit object splices to the exact image hash.

**Done when:** (phase 2) the shadow gate is green on the current lock and runs in the splice path; (phase 5) the ROM is built from the unit with the same SHA-1.

**What C does not fix by itself:** an internal function whose callers are unmatched still has nothing real to be compiled with. See D1 and workstream D step 3.

### D. The matching loop

Run this continuously. Fan out by giving each agent a disjoint list from `frontier next`, its own builder scratch copy, and the procedure below. Seven functions per agent per pass worked well.

```bash
python3 -m tools.conveyor.pipeline.frontier next --limit 60        # ready work, best first
python3 -m tools.conveyor.pipeline.frontier hubs                   # what most code waits on
python3 -m tools.conveyor.pipeline.frontier show NAME              # callees, blockers, unit, prior source
```

`single` in the tool means *no whole-program signature detected*, not *will match alone*: in wave 0, 5 of 14 `single` functions needed `-O3` or group context. Hence step 2.

**Per function:**

1. **Look before writing.** `frontier show NAME` (prior source paths under `evidence`), `grep -r NAME cloud/work docs dot_handoff.md` for earlier attempts and their stop notes, `python3 cloud/work/tools/tdis.py NAME` for the retail disassembly. Find the arcade ancestor by behaviour and constants, not by the function's name.
2. **Classify from the disassembly (2 minutes, saves hours):**
   - reads `t0`–`t5`/`s*`/`f16+` on entry without writing them → register-parameter callee (`unit`);
   - writes `s0`–`s7`/`s8` it never saves → internal (`frontier show` lists `unsaved_callee_regs`);
   - temps cycle only `t6`→`t9` and wrap, while nothing reaches `t5` → internal member of a group with its callers;
   - several loads into `ra, t5, t4, …` in descending order followed by all the stores, at the top or mid-body → an **inlined callee**: find the matched neighbour that has no `jal` callers and put its definition in the same file;
   - a `jr ra; nop` stub next door with no callers → probably a deleted static that is inlined into this function or a neighbour.
3. **If it is internal:** build a real group with the callers `frontier show` lists under `unit`. If those callers are themselves unmatched, write the body anyway, prove it with stand-in callers, and record it as **provisional** (D1) so its dependents are not held up. It closes for real when the callers land.
4. **Write natural, typed C.** Typed struct arrays, plain `for` loops, SDK GBI macros (`gDPLoadTLUT`, `gDPSetPrimColor`, …) rather than hand-built words, literals written as literals with the arcade source's literal types (`1` vs `1.0f` form different constant webs — that was the whole residual in two wave-0 functions). Delete m2c spill locals.
5. **Probe flags on the first structurally right draft:** score at `-O3` and at `-O2`. Default to `-O3`.
6. **Diagnose before searching.** On a near-miss run `python3 tools/workbench.py diagnose …` ([external notes](../external/README.md)) and read which lane differs. Then apply the matching lever from [cloud/PLAYBOOK.md](../../cloud/PLAYBOOK.md). Scripted variant searches are cheap (0.2 s per compile-and-score) but only after the residual has a name.
7. **Stop rule.** About 50 variants with no movement on the same residual: stop, write the residual, the lane, what was tried and the single best next hypothesis in the work directory, and take the next function. Two wave-0 functions absorbed 600–900 variants without moving; that time was better spent elsewhere.
8. **Hand back.** Strict `MATCH` → `cloud/matches/NAME.c` (first line `/* flags: … */`, header comment with real semantics, arcade ancestor if proven, quirks) and one line with the exact scorer command and output in the pass's `RESULTS.md`. Non-match → `cloud/work/frontier/<pass>/NAME/best.c` plus the residual note.
9. **Integrate in batches** (one maintainer lane): re-score, splice, `blob_rom rom`, `frontier scan`, re-rank. Batch independent functions to amortise the ROM build, keep the per-function audit.

**Ordering within the frontier.** Take `next` from the top (own bytes plus the bytes it alone unblocks), but pull forward anything in `hubs` that is ready: the render/entity cluster at `0x80096xxx` and the audio/sound pair sit under about 110 unmatched functions.

**Open items from wave 0** (each has a `best.c` and notes under `cloud/work/frontier/agent{A,B,C}/`):

| Function | Bytes | State | Next step |
|---|---:|---|---|
| `camera_blend_between`, `func_800DE860`, `func_800EC270` | 1,476 | code identical | closes with workstream B |
| `func_800B66B0` | 152 | matches with stand-ins | real group with its only caller `menu_input_process` |
| `func_80099B30` | 204 | matches with stand-ins | real group with `render_display_list` and `particle_system`; it is the last unmatched callee of the 10 KB `render_display_list` |
| `func_800D1AB0` | 560 | matches with stand-ins | real group with `car_setup_confirm` and `func_800F8EC8` |
| `func_80091B00` | 168 | 11/42 in a stand-in group | ring plus an `as1` reorder (`sb; li -1; sh`); `cc -S` already has retail order |
| `model_data_load` | 372 | 3/93 | make the entry pointer one web across modes 2 and 3 |
| `entity_lod_select` | 716 | 36/179 at `-O3` | one more emptied statement between `op2 = …` and the `op == 1 ||` chain; also needs its jump table owned (B) |

### E. Shared type and data model

Do this alongside D, not before it. Wave 0 matched mid-size functions with ad-hoc structs; the shared model pays off as functions get larger and as sources move into one unit (C makes conflicting declarations a build error instead of a private matter).

Measured starting point ([type_model/REPORT.md](../../cloud/work/frontier/type_model/REPORT.md)): 185 hand-written struct names with 261 bodies (37 names conflict); 2,315 declared `D_` addresses, 61 with two or more conflicting hand types; every single-function source embeds its own copy of a generated prelude.

Order:

1. **Scalars first.** Two scalar blocks (`0x8014A0F8..0x8014A110`, `0x80117498..0x801174E4`) touch 46% of unmatched bytes. Seed a shared header mechanically from the 2,185 generated defaults plus the 266 hand overrides (`type_model/decl_overrides.json`); resolve the 61 conflicts by hand against access widths.
2. **Car array and `MODELDAT` together.** `0x80152818` (0x3B8 × 6, 101 unmatched functions, 139 KB) and `0x8014A250` (0x808 × 6, 76 functions, 105 KB; this is the N64 `MODELDAT`, not "track data" — inferred). They are used by the same functions. Anchor from `func_800E4B58` (the real maxpath code, identified by float literals unique to arcade `maxpath.c`). **Arcade offsets cannot be transcribed:** `CAR_DATA.mpath` is at +0x314 on N64 against +0x374 in the arcade; `MPCTL` keeps its float order shifted by −4, with `mpi` moved and narrowed to `S16`. Build each sub-block from N64 access evidence (`type_model/bases.json` has per-offset access widths) and take an arcade field name only where a code anchor proves it.
3. **N64-specific records in cover order:** the pointer block at `0x8017A4E0`, the 0x44-byte records at `0x8012E700` (wave 0 recovered `u32 flags` at 0, `s16 child` at 0x16, `s16 sibling` at 0x18), input records at `0x8014A114` (0x4C), `0x80151FC8` (0x78), player slots at `0x80144030` (0x304 × 4).
4. **Acceptance for any shared type:** every locked body that includes it still scores strict `MATCH` (the shadow gate in C does this in one build). A type that changes a locked body's code is wrong or incomplete, however plausible.
5. **Last:** BSS ownership and per-TU `.data`. Nothing owns game storage today (`blob.ld` PROVIDEs 4,466 absolute symbols; the group linker refuses `.bss`). TU boundaries can be read from the 67 `.data` clusters. Not needed for matching code.

### F. Tool upkeep

Small items, each independently useful:

- **Frontier: provisional set.** A file of functions proven with stand-ins; `frontier` treats them as satisfied for layering and reports them separately (never as matched).
- **Frontier: more whole-program signatures.** The four-wide `t6`–`t9` ring and the inlined-callee load pattern from D step 2; mark both as `group`. The unsaved-callee-saved detector (added 2026-10-04) already moved 38 unmatched functions out of `single`, with no false positive among the 543 standalone locks.
- **Frontier: caller-less stubs.** List the 159 `jr ra; nop` bodies with their address neighbours, so step 2's "deleted static" check is a lookup.
- **IPA scan:** fold the same signatures into `ipa.py scan` so the farm stops spending `-O2` permuter time on internal functions.
- **`score.py` default flags:** probe `-O3` and `-O2` and report both for a new function.
- **Stale statements to correct** in older docs when next touched: `PROJECT_PLAN.md` lists `func_800D1248` as off limits ([dot_response](../../dot_response) lifted that), says there are zero ready packets, and gives the pre-boot-tail static numbers; `docs/COMPILER_SETTINGS.md` describes game code as mixed `-O2`/`-O3` and the whole-program build as a hypothesis.

---

## 4. Decisions for the repository owner

These change policy, so they are the owner's, not the implementer's. The plan works without them, more slowly.

- **D1. A provisional tier for stand-in proofs.** Proposed: a body that scores strict `MATCH` in an `-O3` group whose only non-real members are stand-in *callers* is recorded as provisional — not spliced, not coverage, but counted as satisfied by the frontier so its callers can be worked. It becomes a real claim when its callers are in the unit. Without this, an internal callee and its unmatched callers block each other.
- **D2. Make `-O3` the recorded flagset for game code.** All 543 standalone locks match at `-O3`; moving them is a no-op on bytes and makes the lock agree with the real recipe. Proposed as part of C phase 5, not before.
- **D3. Reinterpret the 159 caller-less stubs.** If they are deleted statics, the honest source is the static function defined where it is inlined, not an empty function. That changes what "matched" means for 159 lock entries and should be decided before C phase 3 rewrites sources.
- **D4. Authorise splicing wave 0** (workstream A). Nothing from this session has touched the lock.

---

## 5. Expected shape of progress (not a forecast)

- Layer 1 today: 195 functions, 88,768 bytes. Ready by recipe after the detector fix: 140 `single` (63,608), 38 `group` (16,368), 5 `unit` (2,984).
- Layers: 13, no cycles. Layer 2 is 100 functions / 96,372 bytes; the six largest functions sit in layers 2–7 and should not be attempted before their callees are real.
- Wave 0 yield: 11 of 20 strict in one pass, 14 of 20 once B lands, 18 of 20 once their real callers exist. Do not extrapolate a rate from 20 functions chosen from the top of the list.
- Report game and static coverage separately, from `make progress`, after a ROM gate.

---

## Appendix A. Builder scratch setup

```bash
# once per pass, from the repo root on the Pi (refreshes the shared base copy)
ssh watchman2 'mkdir -p ~/rush2049/scratch/frontier/base'
rsync -a --delete --exclude='__pycache__/' --relative \
  tools/cloud tools/conveyor asm/us/blob src/blob include cloud/work/tools \
  cloud/PLAYBOOK.md blob_matched.lock.json \
  watchman2:rush2049/scratch/frontier/base/

# once per agent
ssh watchman2 'cp -r ~/rush2049/scratch/frontier/base ~/rush2049/scratch/frontier/<agent>'

# score a candidate (about 0.2 s)
scp cand.c watchman2:rush2049/scratch/frontier/<agent>/cand/NAME.c
ssh watchman2 'cd ~/rush2049/scratch/frontier/<agent> && \
  IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido \
  python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"'

# an -O3 group directory (group.json + sources)
…  python3 tools/cloud/score.py group cand/<groupdir>
```

The toolkit hash is `blob_splice.TOOLKIT`; check it has not changed. Scratch copies from 2026-10-04 (`agentA`, `agentB`, `agentC`, `wholeprog`, `base`) are still on the builder and can be deleted once their results are integrated. `wholeprog/reproduce.sh` reruns the whole-program matrix in about 12 minutes.

## Appendix B. What exists, uncommitted

| Path | What |
|---|---|
| `tools/conveyor/pipeline/frontier.py`, `tests/conveyor/test_frontier.py` | the frontier tool and its 9 tests |
| `tools/conveyor/README.md`, `docs/README.md` | tool section and index row |
| `cloud/matches/*.c` (11 new files, table in §3 A) | wave-0 strict matches, unspliced |
| `cloud/work/frontier/agent{A,B,C}/` | per-function results, best sources, stand-in groups, helper scripts |
| `cloud/work/frontier/wholeprog/` | whole-program experiment: README, scripts, keep and blocker lists |
| `cloud/work/frontier/type_model/` | data/type survey: REPORT, scripts, JSON (`bases.json`, `rodata_owners.json`, `decl_overrides.json`, …) |
| `build/frontier.json` | generated, git-ignored |

## Appendix C. Evidence index

| Claim | Where |
|---|---|
| 675/676 in one unit; manifest ingredients; build time; stubs; internal ≡ `static` | [wholeprog/README.md](../../cloud/work/frontier/wholeprog/README.md) |
| Section split, rodata ownership, global bases, arcade offsets, source duplication | [type_model/REPORT.md](../../cloud/work/frontier/type_model/REPORT.md) |
| Wave-0 scorer commands and outputs | `cloud/work/frontier/agent{A,B,C}/RESULTS.md` |
| Whole-program build mechanism (`uld -kp`), group splicing | [010 spikes](../../specs/010-ipa-call-groups/research/s2-s5-spikes.md), `tools/conveyor/pipeline/blob_group.py` |
| IDO levers and quirks | [cloud/PLAYBOOK.md](../../cloud/PLAYBOOK.md), [external notes](../external/README.md) |
