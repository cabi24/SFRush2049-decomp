# Matching-wave brief (frontier plan, workstream D)

Read first: `CLAUDE.md`, `docs/plans/2026-10-04-frontier-plan.md` (§1 model, §2 rules, §3 D per-function
procedure — follow that procedure), and the quirks list in `cloud/PLAYBOOK.md`.

## Goal
Strict `MATCH` from `tools/cloud/score.py` for as many of your assigned game functions as you can, as
IDO 5.3 / C89 source. Names are historical labels, not semantics.

## Facts you need
- The game is one whole-program **-O3** unit. Default flags: `-g0 -O3 -mips2 -G 0 -non_shared`; also try `-O2`
  on the first structurally right draft and keep whichever is closer.
- `python3 -m tools.conveyor.pipeline.frontier show NAME` gives callees, callers that must be in the unit, IPA
  register facts and paths of prior source. `grep -r NAME cloud/work docs dot_handoff.md` finds older attempts
  and their stop notes; start from the best prior source.
- `python3 cloud/work/tools/tdis.py NAME` disassembles the retail function. Matched house style: `src/blob/*.c`
  and `cloud/matches/*.c`. Arcade source for semantics: `reference/repos/rushtherock/` (find the ancestor by
  behaviour and constants, never by name).
- Classify from the disassembly before writing (plan §3 D step 2): register-parameter callee, unsaved
  callee-saved registers, four-wide `t6`-`t9` ring, inlined callee (descending `ra,t5,t4…` loads then stores).
  Any of these means it cannot match alone: build a real `-O3` group (`group.json` + sources; examples in
  `src/blob/groups/*/` and `cloud/work/frontier/agentB/Input_ApplyPadConfig/group/`) with the real partner
  functions. Already-matched partners' sources are in `src/blob/` — reuse them unchanged as context/members.
  If the real partners are unmatched, prove the body with stand-in callers and report it as **provisional**
  (not a match, not spliceable).
- A function's own float literals / jump tables / local statics score as `MATCH (N section-relative
  relocations unverified …)` in the (unpatched) scorer. Write natural literals, never `extern f32 D_8012xxxx`,
  and report it as "code identical, own-rodata unverified": the splice verifies the bytes against the image, so
  the integrator can land it. Check each literal's bits against the retail word yourself (the splice refused a
  `9.5493f` that should have been `9.549305f`).
- To test a function inside the real whole-program unit (an internal function with its real callers, or a
  caller that inlines a locked callee): from the repo root on the Pi,
  `python3 -m tools.conveyor.pipeline.blob_unit --tag <you> score NAME [NAME…] --with path/cand.c`
  (3–5 s; `--internal FN`, `--keep FN`, `--block FN`, `--neighbours`). Always pass your own `--tag`.
- Read §0 "What wave 1 changed in the method" in the plan before starting: arcade declaration lists and literal
  spellings, deleted-static stubs (`frontier stubs`, `frontier show NAME`), one variable = one register, and
  the large-function tools in `cloud/work/frontier/w1b/` (`o3s.sh`, `bscore.py`, `gen.py`).
- Literal types matter (`1` vs `1.0f`), declaration order sets stack slots, each named local costs a slot,
  use SDK GBI macros for display-list words.

## Wave 9 techniques (try these first; each closed a large function)
- **Inlined locked helpers:** big leaf functions often inline already-locked kept functions (rand
  `func_8008B2B4`/`func_8008B2E4`, LCG `func_800D50E4`, `func_800B930C` get_next_checkpoint). Paste the
  locked definition verbatim into the candidate; their inlined locals also explain "frame filler" words.
- **Caller-less `jr ra; nop` stubs right before a function are deleted inlined statics.** Define them (static,
  or non-static with `--internal NAME`) and size the frame: each inlined call reserves ~8 bytes in call order.
- **Index global arrays directly** (`D_80152038[i].f`) instead of through a pointer local: a store through a
  pointer local aliases every global and kills PRE webs. Conversely launder a pointer via `(T *)(u32)p` when
  retail keeps loads behind stores.
- **Retry `extern f32 D_8012xxxx` near-misses with natural literals** — the scorer now verifies own rodata.
- **`single` near-misses: try `blob_unit score NAME --internal NAME`** — the label can be wrong when callers
  are locked.
- **Constant sharing follows signedness:** a `1` stored to a signed byte shares the s32 `1` web; `u8` does not.
- **Tied colouring priorities resolve by first appearance in source**; splitting `x = a; x += b;` moves it.
- Check older drafts' global addresses against the retail disassembly before tuning (w9e found a wrong one).

## Shared tracing toolkit
`cloud/work/frontier/tools/trace/` (read its README.md): `install.sh <your builder scratch> [--reuse ~/rush2049/scratch/frontier/wtk]`
(SCRATCH FIRST, then `--reuse`; the other order builds a toolkit into `~/--reuse`, three wave-15 lanes did this) once, then
`us.sh` (unit score + rows), `udiff.py --ops/--norm`, `ctrace.sh`/`sum.sh` (colouring decisions, numintf,
callee-saved cost), `force.sh` (forced colouring oracle), `spill.sh`/`spcensus.sh` (spill-temp/frame layout),
`ugt.sh` (ugen order), `as1t.sh` (scheduler). Set `TAG=<lane>`. Run `force.sh` with the suspected webs before
writing variants: if forcing reaches 0 rows the residual is colour-only.

## Wave 12 rules
- Unused local declarations are acceptable ONLY when they are the exact frame residual (every
  function-level named local takes a slot, used or not) and are disclosed in the header. Pad arrays that
  stand in for unknown structure elsewhere are not.
- A group that gives a locked stub a real body must list that stub in "members" (and say which
  prefer_definition/force_internal overrides it needs). Superseding groups keep the SAME name and every
  old member.
- `cloud/matches/NAME.c` is ONLY for functions that `score.py fn` matches standalone (CI rescores every changed
  file there standalone). A function that is EQUAL only in the whole-program unit or in a group goes in your
  lane's `groups/<name>/` dir with the context files it needs (see w12l), never in cloud/matches.
- Prove a residual is colour-only with `tools/trace/force.sh` before writing variants. For a colour tie,
  try the compiled-out `if (x) {}` priority lever (w11a) and `t = a; t -= b;` splits (w11c) first.

## Wave 14: mass variant testing (vbatch + workbench generators)
- `cloud/work/frontier/tools/vbatch/` (README.md): `vgen.py` expands a template with choice points
  (`/*@{*/a/*@| b @}*/`) into hundreds of variants, and `wbgen.py` writes every mechanical one-edit neighbour
  (commutative swaps, copy removal, operand hoists) using the vendored workbench's sweep generators.
  `vbatch.sh` scores a whole directory on the builder in seconds and ranks by strict count.
- Diagnose first: `python3 -m tools.conveyor.pipeline.diagnose one NAME --source BEST.c --flags "-g0 -O3 -mips2
  -G 0 -non_shared"` (without --flags or a `/* flags: */` header it compiles at -O2), then
  `python3 tools/workbench.py guide`. Hundreds of variants are cheap. Plan at least 3 rounds and 300
  variants per function, and stop after 3 rounds with no strict improvement.
- **Named float locals change the FP registers.** place_cars_in_order's last 8 words: a named `f32 f`
  for a sum later converted to u32 is a coloured web (it got $f0). Written as one expression, the sum and
  the conversion stay in ugen's temp ring ($f18/$f4, as in retail). If retail's FP registers are all ring
  temps ($f4–$f18) but yours puts one in $f0/$f2 or a callee-saved $f20+, inline the named local.
- Pad arrays (`u8 pad[20]`) to fix a frame are still not acceptable (Wave 12 rules), even when a sweep finds
  them load-bearing. Report the frame residual instead.

## Wave 15: trace, don't sweep (7 of 8 matching lanes closed something)
- Mass-variant sweeps plateaued on a dozen functions in wave 14. Wave 15's matches came from tracing one cause
  and writing the one lever: run `diagnose`, then `force.sh` (colour-only?), then `ctrace.sh`/`sum.sh` to see WHY
  the colouring differs. Use vbatch only to confirm a hypothesis across neighbours.
- **A caller-less `jr ra; nop` stub next to an unmatched function is probably its inlined helper** (w15b: the getter
  at func_800B59E8 fixed the "registers one up" residual that w12h could only fix with implicit int; w15d:
  `heap_or_default` from locked sound_play_menu.c is the real copy retail keeps in func_800E7A98).
- **An inlined static's return value is a separate register value that is not optimised away** — the lever when
  retail keeps a copy (`move a1,v0`) that hand-written copies lose (w15d).
- **A compiled-out read can be a "holder"**: `lk = sc->link; if (lk) {}` right after a load gives an existing variable
  a code-free extra definition that blocks v1 for the neighbours (w15a, 17 -> 5 words). The FP version needs no
  variable: `if (sc->keys[idx].dur != 0.0f) {}` next to the block. Placement matters (same block).
- **A local kept in an argument register is often the reused parameter** (w15a `flag`).
- **Unexplained callee saves mean missing context**: if a caller saves more s-registers (or $f20/$f22) than its body
  uses, an unmatched internal callee clobbers them. Put a genuine reconstruction in the group as context (w15f
  billboard_render; the locked audio_frame_sync group did the same).
- **Loop weights decide callee-saved registers**: a `return` inside a loop leaves blocks at weight 1, a `goto end`
  gives weight 10 (w15h). The callee-save threshold is `clamp(blocks/4, 4, 60)` (workbench law L56).
- **Float literal spelling splits uopt constants**: `0.5f` and `.5f` are different constants (w15e). `x = x + E`
  and `x += E` compile with opposite commutative operand order; check the word diff, `--norm` hides it.
- **The compiler splits long straight-line blocks on its own**; a compiled-out block end (`do {} while (0);`,
  `if (x) {}`) moves the boundary, an unused label does not (w15d func_800A79F4).
- **Stores to two offsets of one symbol share one `lui at` only when the symbol is defined in the unit** (w15d).
- **uopt canonicalises compare operand order by operand kind** (plain variable vs memory load), not spelling (w15a).
- **Group packaging**: a match proven only in the whole-program unit does not install as a bare group — the group
  compile lacks the unit's IPA context. Add the locked callees whose conventions matter (internal float-arg callees,
  umerge-inlined helpers) as `context` files, byte-identical copies, and extend `keep` (w15i, mode-select cluster).
  Test `score.py group <dir>` on the builder before claiming a group lands.
- **Re-score old drafts in the current unit**: changes since wave 12 made func_80096130's levers unnecessary (w15c).
- **Write RESULTS.md early**: three wave-13/14 lanes lost their reports (one of them the 8.4 KB mode-select cluster)
  when their session ended.
- Concurrent `us.sh`/`blob_unit` runs on one tag share a stage directory and collide; serialise them.

## Wave 16: owner decisions and lane rules (2026-10-11)
- Unused locals: no numeric cap; the standing rule stands (exact frame residual only, each disclosed in the header
  with the frame arithmetic: retail frame, frame without them, slot count).
- `score.py fn` prints NOT VERIFIED for a function whose own `.rodata` sits in two retail regions (owndata's
  one-base rule). Expected, not a blocker: the group install (`blob_group`) carries the w15j fail-closed proof.
  Report the code score and the owndata line verbatim; the coordinator installs via blob_group.
- Implicit int stays an open decision (deliver to the lane dir, say so, give the best form without it). Look
  first for the wave-15 lever: a caller-less `jr ra; nop` stub next to the function is probably its inlined getter.
- Keep off Astra's functions: func_80087110, stat_race_update, func_800FE5B0, camera_transform.
- **Wave 16 lessons** (5 of 8 lanes landed something; game +9.3 KB, static +4.2 KB):
  - uopt orders `==`/`!=` operands by kind: a plain variable goes second, an expression first. A no-code identity
    (`x * 1`, `x ^ 0`, `x >> 0`) on the variable side flips the order; source order then decides (w16a camera_update).
  - A store through a pointer kills every pointer load in uopt: a value retail keeps across such a store is a
    variable or register expression, never a pointer load (w16a).
  - uopt's live-range split start block follows source-visible block order: which arm falls through (`if (c) {call}`
    vs `if (!c) continue;`) decides which call block is split off first and whether a long-lived pointer is ever
    coloured (w16d camera_victory, `car` in s4). A private instrumented uopt with skip/reject oracles
    (`w16d/tools/patch_spl.py`, `w16b/tools/splitpatch.py`) proves the lever before you search for the source form.
  - An exact benefit = cost tie in `ctrace` is broken by single-precision rounding of `total/blocks * blocks`:
    1-4 more live blocks in the loop (a `conv = 1;` after it) flip it with no new uses (w16c).
  - `0.f` and `0.0f` are different constants to uopt: a second spelling stops zero keeping two register pieces (w16c).
  - A one-line xor swap (macro) gives as1 a different byte-store order than the same swap on three lines (w16h).
  - Matched siblings supply idioms: camera_trigger_check's `cnt = *q++` closed camera_victory's last 3 words (w16d).
  - Re-measure old drafts in the current unit and check they compile (w16e: w15c's variants had a duplicated line).
  - Static side: the libgcc-named boot functions are ultralib `libc/ll.c`/`llcvt.c` verbatim at `-O1 -mips3 -32`;
    `score.py fn` cannot score static targets, use `lock add` (pool) and `promote_batch.py` (w16g/w16i).
- **Wave 17 lessons** (5 of 9 matching lanes landed something; game +9.5 KB):
  - Float-literal spelling is a constant-web key: `0`, `0.0f` and `0.f` are three constants; the spelling decides
    which uses share a register, what is hoisted into $f20–$f30 and what is rematerialised. Follow the arcade's mix
    (w17b, from 503/567 to EQUAL). Loop-constant hoisting ties go to the lowest web number.
  - Measure a blamed callee convention with a correct-convention stand-in before rebuilding the callee (w17b).
  - A value in a register that ends before a call while the variable stays live is a temporary copied into a
    memory variable; a reused local assigned again later stops uopt substituting it (w17c `lucent`).
  - Two register-argument copies emitted swapped → the callee's locked prototype has the parameters in the wrong
    order; check the arcade signature (w17c model_bounds_calc = CheckVisible(v, on)).
  - If retail keeps a one-use load in an argument register it is a variable; find its slot by checking which declared
    local the retail frame proves fake (w17a `i = idx[0]`, `c1`). One "variable" in two retail registers is two
    variables or an expression.
  - A bare entry store of a parameter with no sign extension is a `s16` parameter read once by a statement that
    compiles to nothing (`op;`) (w17d). Two paths sharing an unexplained register share a stack slot: one reused
    variable. `(T *)(u32)ptr` laundering removes the no-alias hint and restores load-after-store order.
  - A branch where a pointer is dead (early `return`) ends uopt's not-the-stack marker for its register and changes
    later reordering (w17i func_80096130). `&a[i]`, `a + i` and `a[i].f` are three different expressions.
  - A frame 16 B short per inlined call site means two missing nested inline levels: look for caller-less stubs next
    to the callee (w17h slot-queue lock/unlock). Check `umerge.log` before adding dummy locals.
  - Packaging: an inlined `static` helper's deleted-procedure stub has no symbol and may land right after a stub
    member, failing the splice with "extra words". Make the helper a non-static function that is not on `keep`
    (w17j). `score.py group` WITHOUT `--claims` must show MATCH for every member before claiming a group installs.
  - Priority ties: uopt scores register candidates with blocks counted in groups of four, so exact ties are common
    and go to the lower web number; 2–4 live blocks or one weighted use flips them (w17f, w16c). Oracles:
    `w17f/tools/prio_patch.py`, `w17i/boostpatch.py`, `w16d/tools/patch_spl.py`.
  - Static side: the Rush boot tail is IDO 7.1 (w17g); the telltale is a `u8`/`u16` argument narrowed in its own
    register (`andi a0,a0,0xFFFF` after `sw a0`), which 5.3 never does.
- Write RESULTS.md within the first 20 minutes and keep variants under `<lane>/<fn>/v/`.

## Permission denials
If any tool call is denied by a permission/safety check, do NOT retry it in another form (different paths,
globs, quoting, tools or hosts). Record it in RESULTS.md and continue without it.

## Builder (IDO runs only there)
```
ssh watchman2 'cp -r ~/rush2049/scratch/frontier/base ~/rush2049/scratch/frontier/<you>'      # once
scp cand.c watchman2:rush2049/scratch/frontier/<you>/cand/NAME.c
ssh watchman2 'cd ~/rush2049/scratch/frontier/<you> && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"'
…  python3 tools/cloud/score.py group cand/<groupdir>          # -O3 group directory
```
Work ONLY inside your own scratch copy on the builder. Never touch `~/rush2049/repo`, `/tmp/blobsplice`,
`/tmp/blobgroup`; never run make or the conveyor there. At most 2 cores; the box is shared and other agents
are running. Helper scripts (aligned diffs, batch scoring) are in `cloud/work/frontier/w1f/` and `w1b/` (the older `agentC/full.py` crashes on runs of `nop`).

## Discipline
- Run `python3 tools/workbench.py diagnose` (docs/external/README.md) on a near-miss before searching.
- Stop a function only after 3 batch rounds (300+ variants) with no strict improvement (Wave 14); then
  write it up and move on.
- Only what the scorer printed counts. Quote it exactly.

## Deliverables (local repo only)
Do not commit, splice, or edit `src/blob`, `asm/`, `*.lock.json`, `tools/`, `include/`, or other agents' dirs.
- Strict MATCH: `cloud/matches/NAME.c` (line 1 `/* flags: … */`; header comment with real semantics, arcade
  ancestor if proven, and any shaping quirk). For a group match: the group dir at
  `cloud/work/frontier/<you>/groups/<group>/` with `"claims"` listing the strict members.
- Otherwise: `cloud/work/frontier/<you>/NAME/best.c` + notes.
- `cloud/work/frontier/<you>/RESULTS.md`: per function — state (MATCH / code-identical-unverified /
  provisional / N words off), flags, exact scorer command and output, residual lane, what was tried, best next
  hypothesis, struct layouts and global types recovered.
Final message: the table, plus anything that generalises.

## Behavioural check (recomp oracle, waves 22-23)
A non-matching body can be checked for *behaviour* against retail: the recompiled game (rocky, private
ROM) runs a fixed input script with your C in place of the function and compares every CALL of it
(arguments, return, the words it changed) and every frame's display lists and rdram with a stock build.
This never changes coverage: a function replaced in the port stays unmatched. Use it to decide "correct,
not matched", or to find where a near-miss draft is wrong. Details: `tools/recomp/README.md` "Phase 2"/"Phase 3".
1. Add an entry to `tools/recomp/replacements.json` (one per body; `mode` `ido` = compiled by the project's
   IDO with the file's `/* flags: */` header, exact N64 semantics; leave it at that) and a set naming it:
   `"draft_NAME": {"function": "NAME", "population": "game", "source": "cloud/work/frontier/<you>/NAME/best.c",
   "mode": "ido", "state": "draft", "note": "..."}`, `"sets": {..., "draft_NAME": ["draft_NAME"]}`.
   The source must define NAME; anything else it defines and NAME needs comes along (static helpers); every
   other symbol binds to the inventory's address, libultra by the game-side legacy names (unknown names fail).
   Function-local statics bind to their retail addresses when the code around them is retail, or by a
   `D_XXXXXXXX` name, or by `"statics": {"name": "0x8015xxxx"}` in the entry (manifest `statics_private`
   lists any that stayed private). Optional: probes = `"mode": "capture"` entries on callees (with a
   `prototype`, or `capture_args` register names for non-o32 callees) in the same set.
2. Ask the coordinator (or, on rocky in `~/rush2049/recomp/<lane>`): build BOTH sides from one tree,
   `gen_config.py ... --out build/cap-SET --capture-set SET` (stock, capture hooks) and
   `gen_config.py ... --out build/repl-SET --replace-set SET` (add `--probe-callees` to both to record every
   retail callee), N64Recomp + `cmake`/`ninja` each, then
   `python3 tools/recomp/oracle.py run --stock-host build/cap-SET/host/rush2049_recomp --rom-stock build/cap-SET/rom/rush2049.recomp.z64 --repl-host build/repl-SET/host/rush2049_recomp --rom-repl build/repl-SET/rom/rush2049.recomp.z64 --out runs/SET --script SCRIPT --datasyms build/cap-SET/rush2049.datasyms.toml`.
   Scripts (`tools/recomp/scripts/`): `race` (default), `race_track2`, `two_player`, `audio_options`; pick
   one that reaches your function (NOT EXERCISED otherwise).
3. Reading a verdict. First line, verbatim, one of:
   - `EQUIVALENT over 1800 frames (...)` + `per-call: all N captured calls identical (arguments, returns,
     effects)` and the call count of the replaced function: behaviourally identical on that script.
   - `NOT EXERCISED` (a replaced function had 0 calls: no verdict; try another script).
   - `FIRST DIFFERING CALL: seq S = FUNC call K, frame F: WHAT` then `inside: OUTER call J` when FUNC is a
     probe/callee running under your function, then the differing fields: `args` (prototype labels, e.g.
     `f22: stock 3F800000, replaced 00000000`), `return`, `effect` (the words that call changed, stock
     old->new vs replaced old->new, with data symbols), `sequence` (one side made a call the other did not).
     Call K of FUNC is where behaviour first departs: everything before it was identical. The frame
     verdict follows (`frames: DIFFERENT at frame ...` or `no judged frame difference`).
   - `DIFFERENT at frame F` with `per-call: all N ... identical`: a difference outside the captured effects
     (writes to the stack frames of callers, or by other threads); report it.
4. A difference at a call into a function with IPA register parameters (frontier `register_params`, e.g.
   entity_transform_calc takes $f22-$f28, audio_effect_setup takes s0) is the standalone compile, not your
   logic: build an oracle source with static copies of those callees (w23f `engine_sound_sync/oracle.c`).
5. Report state "correct, not matched" only on EQUIVALENT with identical calls, in RESULTS.md and the CLAIMS
   line note: `correct, not matched (oracle EQUIVALENT 1800 frames, N calls identical, script S;
   runs/<SET>/verdict.json, replacements.json <id>)`. Limits to state with it: the scripts used; functions
   a script does not reach get no verdict; thread stacks below the replaced function's frame are not judged.

## 2026-10-21 lessons (z12d18: func_80012D18, IDO 7.1, 578 -> 187 rows in one session)

Method that worked on a far near-miss (674/787 words, 6 words short, five lanes stuck on it):
1. **Align by opcode first** (`difflib` on the mnemonic sequences of `t.rows`/`m.rows`; `cloud/work/frontier/z12d18/rows.py`
   prints ranges). It separates structural differences (inserted/deleted instructions) from allocation noise; fix
   structure before touching registers.
2. **Read the target's shape at each misaligned block** and name the C construct that emits it:
   - `mfc1; sll 16; sra 16; sh; sll 16` = the value of a short assignment reused: `x = (s->f = expr) << 16`.
   - `bgez r; sra; addiu at,r,31; sra` = signed `/ 32` of an int-promoted value (a `>> 5` in the source is a plain
     `sra`); two branches of one function may be written differently.
   - a branch into another switch's case body with that body's registers preloaded in the delay slot = a `goto`
     into a shared tail (IDO does not cross-jump identical tails).
   - the first instruction of a block sitting in the previous branch's delay slot = that statement comes first.
   - a field reloaded after a store to a stack array: the store killed it; computing the dependent value before the
     store removes the reload (`offset = pos - V->loop_start;` before `address[0] = V->loop_start;`).
3. **Frame by usage profile**: list every `N(sp)` the target and ours touch; a constant shift is a declared-local
   difference. `spill71.sh` gives the cfe area and homes; a never-used probe local tests a layout hypothesis in one
   compile (disclose any that stays). A named local replaces a cfe temp (the temp for an assignment expression's
   value is 2 bytes), so swapping forms moves the area.
4. **7.1 colouring is by priority = totalsave/nocs**, highest first; `ctrace71.sh` shows it. **Confirm the payoff with
   `force71.sh` before searching** (one compile tells you how many rows the swap is worth). A second definition
   (`x = a; x += b;`, same instructions) raises a variable web's totalsave and moves it ahead: that fixed a frames/excess
   a1/a2 swap. uopt copy-propagates single-definition locals into expressions in every form tried (own statement,
   int, register, two-step, initialised), so changing the *other* web's priority is the lever.
5. Harness: `cloud/work/frontier/z12d18/variants.py BASE SPEC...` applies named edit sets (`a+b`) and scores each with
   `obj71.sh`; keep every tried edit in its `EDITS` table so the next lane does not repeat it.

## 2026-10-21 lessons, part 2 (z12d18 closed: func_80012D18 187 -> 0 rows, IDO 7.1)

1. **A web that is itself a direct call argument is pinned to that argument's register.** In 7.1 `f_cupcosts`, a
   live unit whose second flag byte is 6 (the expression is passed as-is as an outgoing argument) costs every
   register in mask `0x1001c7d0` at the ~1e20 ineligible sentinel; only the argument register stays eligible.
   Symptom: retail keeps the value in v0 and does `move aN,v0` in the jal delay slot, ours loads straight into aN.
   Lever: make the argument a different expression from the shared web (one cast around the whole address
   expression, or `(int)x` for the argument). `XENV="CUPLOG=1" RAW=1 ctrace71.sh` prints the unit flags.
2. **Ties in p1 go to the lower web number, and split pieces get new high numbers.** A split piece loses every tie
   against an original web, so lowering the competitor's save below the piece's (not just to equal) is the lever.
   Find what inflates the competitor: here an `int` conversion web of a u16 local also covered two end-of-function
   uses; making `mode` unsigned put those uses in the unsigned conversion web with their neighbours.
3. **When opcodes match 100% and only t6-t9 names differ, it is the ugen ring.** Under 7.1 at -O2 the ring here was
   only t6 t7 t8 t9 (t0-t5 were coloured somewhere) and strictly FIFO; check it event by event with `ugt71.sh`
   (`DKWB-FREELIST` lines carry the source line). A block that pops and frees all four temps in order fixes the
   queue, so work the target's allocations backward from it: if no starting queue fits, the target spends an extra
   pop. The extra pop is law L65's phantom: a redundant mask that folds into a u16/u8 store (`x & 3 & 0xFFFF`,
   `(unsigned short)(a * b) + c` stored to a short) emits nothing but pops one temp. Where the pop lands matters:
   masking a whole sum popped one step too late (5 rows); casting the product popped where the target does (0).
   Truncating casts that cfe drops, or that narrow a load (`(unsigned short)arr[1]` became `lhu 98(sp)`), are not
   phantoms.
4. Read the workbench compiler laws (`third_party/n64-decomp-workbench/docs/compiler-laws/ido-5.3.md`, L1, L64-L78)
   before theorising about a ring residual: most of them carry over to 7.1's ugen.

## 2026-10-22 lessons (w38b: overlay deleted-inline stubs, 30 landed)

1. **Every caller-less `jr ra; nop` is a deleted inlined static, and its position is predictable.** Game-called roots
   are laid out first in post-order (callees from the last call site to the first; switch cases from the last case
   backwards), then the rest of the file in SOURCE order, depth-first from each procedure not yet emitted. With MAT3's
   call lists and file order this predicted every claimed stub; `w38b/tools/objsyms.py` shows where our compile puts
   each helper's stub. Claim it by writing the helper non-static, non-kept, named after the stub, in the caller's group.
2. **Unexplained frame slots in native-led bodies are inlined calls.** umerge reserves 8 bytes per inlined call at the
   top of the callee's local area (even for an empty `void f(void)`), and rounds the callee's locals up to 8. Before
   adding unused locals, look for a caller-less stub next to the function: writing the MAT3 static as a real helper
   removed disclosed slots, carriers and a holder from nine installed members with every word still MATCH.
3. Unreferenced non-kept functions in a group also compile to an 8-byte stub (with a symbol; a static one without).

## 2026-10-22 lessons, part 2 (w38a: 8 game functions, MAT3 typed ports)

1. **A save tie is decided by a zero-footprint block.** When ctrace shows equal-save webs and retail colours another
   one, an empty `else ;` (or `if (0) {}`, `do {} while (0)`, `if (x) {}`) placed where only one web is live adds a
   uopt block to its range and drops its save below the tie (HAL_DrawText: 14 rows -> 0). Prove it first with
   `force.sh LABEL NAME p1:wN=s`.
2. **Mask phase: sweep the whole subset, the answer may be "all".** Every computed byte stored into HAL_DrawText's byte
   buffer is `& 255`; 15 of 16 subsets per block differ. A redundant mask is one ring pop (L65).
3. **Rewrite old drafts as MAT3 typed ports against the real locked callees** (LoadTrack, calcMenuRect, HAL_FinishText,
   HAL_DrawText closed fast once their partners had matched). MAT3 indexed loops beat pointer walks; Dreamcast text keeps
   N64-era empty statements; helpers defined after their caller settle as1 line ties.
4. **Globals referenced by one function are often its function-local statics** (own .bss, address-verified by
   score.py and blob_group's per-reference path).
