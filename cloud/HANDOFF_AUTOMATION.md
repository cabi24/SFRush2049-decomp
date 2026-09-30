# Handoff: deterministic matching toolkit (cloud sessions, 2026-09-30)

Written at the end of the cloud session that produced `cloud/PLAYBOOK.md`, the round 2-5 matches and this
toolkit. Read [AUTOMATION_DESIGN.md](work/tools/AUTOMATION_DESIGN.md) for the module spec and
[PLAYBOOK.md](PLAYBOOK.md) for the human-side techniques. This file says what exists, what it measured, how to run
it on local compute, and what to do next.

## 1. What this is, in one paragraph

Nearly every match in this session came from a cheap, strict, parallel loop (compile, compare to the retail words,
mutate, repeat) plus a small set of recurring code-shape fixes. That loop is now deterministic, stdlib-only Python
under `cloud/work/tools/amatch/` (engine) and `cloud/work/tools/ipakit/` (static IPA analysis). LLMs are needed only
for what the machine cannot finish, and `autopilot.py` hands them a compact residual instead of the whole problem.
No protected path is touched; nothing is written to the repo unless you pass `--write`.

## 2. Quick start (from the repo root; needs `bash tools/cloud/setup.sh` once for IDO)

```bash
# what is worth doing, ranked (about 5 s, no IDO needed)
python3 cloud/work/tools/amatch/triage.py --top 40 --class abi --json

# let the machine work the list (dry run: outputs under build/autopilot/ only)
python3 cloud/work/tools/amatch/autopilot.py --top 40 --class abi --budget 600 --jobs 8
#   resumable (state.json); add --write to put verified matches in cloud/matches/ and group claims;
#   read build/autopilot/report.md and worklist.json (the only things an LLM should read)

# one function by hand
python3 cloud/work/tools/amatch/search.py FILE.c FUNC --budget 3000 --jobs 8 --seed 1
python3 cloud/work/tools/amatch/report.py build/amatch_runs/FUNC        # residual for an LLM
python3 cloud/work/tools/amatch/builder.py fn FILE.c FUNC --hunks       # score one candidate
python3 cloud/work/tools/amatch/mutate.py FILE.c FUNC --list            # what the catalog can do here
python3 cloud/work/tools/amatch/probe.py GROUP_DIR --member FN          # IPA allocation experiments
python3 cloud/work/tools/amatch/autopilot.py --selftest                 # 5 known-solvable functions
python3 cloud/work/tools/amatch/autopilot.py --bench                    # corpus auto-match rate

# static IPA analysis (no IDO)
python3 cloud/work/tools/ipakit/deps.py ...    classify | edges | closure | roots | validate
python3 cloud/work/tools/ipakit/analyze.py NAME --sites
python3 cloud/work/tools/ipakit/sigs.py NAME --evidence
python3 cloud/work/tools/ipakit/groupgen.py SEED... --mode chain --plan   # skeleton group dir
python3 cloud/work/tools/ipakit/heads.py                                   # head audit
```

Tests: `python3 -m unittest discover -s tests/cloud` (253 tests, about 2 minutes with IDO; IDO-dependent tests skip
without it). Helper tools from earlier rounds are still in `cloud/work/tools/` (`zbuild.py`, `extscore.py`,
`tdis.py`, `callers.py`, `closure.py`, `set_claims.py`, `gen_index.py`).

## 3. What each part does and how well it works (measured)

| piece | result |
|---|---|
| `amatch/builder.py`, `aligned.py` | In-process compile + score, agrees exactly with `score.py` on 12 match files and 3 groups. About 198 scores/s on 4 cores (about 50/s/core), versus about 3/s calling `score.py` per candidate. Content-hash cache in `build/amatch_cache/`. Bit-parallel aligned LCS: 13 ms for a 2,559-word function. |
| `amatch/mutate.py` | 27 original + 16 added families (43), stdlib tokenizer/parser, edits are text splices so layout survives. 26,056 mutations over 252 repo files, 0 compile failures in 6,000 sampled IDO compiles. Reproduces 16/16 exact-kind before/after pairs from git history. Semantic equivalence is not proven (risky ones are flagged). |
| `amatch/search.py` | Seeded, parallel best-first + beam + annealing over mutation sequences (plus cross-product of the best singles, steepest ascent, `-O1/-O2/-O3` as a dimension); rejects candidates that regress a matched member in group mode; re-verifies every hit with the real `score.py`. Corpus solve rate at budget 1000: 10/59 before the improvement pass, 20/59 after (see 3a). |
| `amatch/corpus.py` | 124 functions mined from git history, 91 with a usable start->goal pair (59 singles, 32 group members), with edit-class labels (`corpus_data/STATS.md`). |
| `amatch/autopilot.py`, `triage.py` | Ranking + resumable batch driver + residual report + LLM worklist. Selftest 5/5. Corpus bench: 16/59 (27%) auto-matched, median 26 evals / 2.3 s. Real worklist: 1 of top-8 ABI items matched; 4 of the other 7 were `register_only`/`near_miss`. |
| `amatch/probe.py` | Matrix of controlled IPA experiments. Rediscovered: (a) the IPA parameter register follows the callee's USED parameter count (2 used -> t0, 3 -> t1, 4 -> t2, 5 -> t3; declared-but-unused leave it at t0); (b) a caller's live-across value takes the first register the callee does not use (stub using a0-a3 -> t0); (c) `func_800AD650` must not be in `keep`. |
| `ipakit/*` | MIPS decoder checked against objdump on 400 functions; real liveness across calls; callee clobber sets; dependency edges; head audit (52 heads in the opaque runs, 22 more than the old hand list). Whole game analyses in about 3 s. |
| `ipakit/sigs.py` | Parameter inference from home-slot stores: arity 97.4%, int/float order 452/453, relaxed class order 96%, return class 96% over 465 functions with known source. IPA register ORDER is validated on very few cases. |
| `ipakit/groupgen.py` | Skeleton group generator. On 5 real groups: 19/26 members recovered, 0 extras, keep flags 19/19, prototype arity 19/19. 44 of 46 IPA-leaf seeds compile after stub fallback (68% of bodies real m2c); none match out of the box. |

### 3a. Improvement pass: development vs held-out (read this before trusting the catalog)

`corpus_data/SPLIT.json` (by `sha1(name)` parity, made by `corpus.py split`; `bench --half dev|held`) separates 32
development functions from 27 held-out ones. Budget 1000 per function, same harness before and after:

| half | before | after |
|---|---|---|
| development (32) | 7 | 15 |
| held-out (27) | 3 | 5 |
| all (59) | 10 | 20 |

The held-out gain is NOT clean: one of the two extra held-out solves (`func_800B1F30`) was read before the cast family
was designed, and the other (`func_8009002C`) fell to `goto_branch`, added after looking at it. **The clean held-out
gain is 0 (3/27 before and after).** The new families fit the edit classes that recur in the development half; the
remaining held-out near-misses need other edits. Plan on the catalog generalising poorly until more real misses are
mined. No budget-3000 run finished (killed at wrap-up); no 3000 baseline exists.

Added families (`mutate.py`): `while_fold`, `cond_merge`, `loop_guard`, `goto_branch`, `call_arg`, `local_reload`,
`dead_purge`, `hoist_local` (float literals only), `cast_simplify`, `ret_type`, `proto_form`, `param_unused`,
`view_cast`, `m2c_field`, `compound_assign`, `deref_index`; `param_type` also tries `u16`/`s8`. `search.py` derives
`D_xxxxxxxx` hints from the target's address pairs (used by `call_arg`). Search changes: within 8 words of the goal it
ranks strict diff first (aligned-exact leads further away); matched paths are minimised before verification; the result
JSON carries `best_flags`, `full_path`, `min_evals`.

Still needs an LLM (search cannot find it): goto-loop to indexed array loop (`UpdateActiveObjects`, `func_800DD45C`),
struct typing of externs (`func_8008A38C`), if-chain to `switch` (`func_800CDDE8`, `func_800CDE38`), pointer-induction
loops and heavy restructures (`players_race_update`, `track_collision`, `func_800B4DA4`), four or more coordinated edits
where none improves alone (`car_select_handler`), and most pairs whose start is 15+ words off.

**Honest summary of the reach.** The machine reliably finishes the easy tail: one-token-class fixes on an already
close function (loop form, parameter type, operand order, statement order). It does not do the big step of rewriting
m2c output into natural C: 69 of 91 corpus pairs were large rewrites (over half the tokens changed), and that is
where the LLM earned its keep this session. Expect roughly 15-30% of ABI singles to finish unaided; the rest must
come back as a residual.

## 4. The static-analysis findings that matter for "IPA-bound" functions

- The detector flags about 364 functions. The old bounded discovery groups about 114 of them. With the
  register-specific minimal closure, about 284 have a group of at most 1,000 words (`deps.py closure --mode direct`).
  That is the concrete tooling gain: about 2.5x more groupable targets.
- Closure from evidence alone cannot be the whole group: stand-in callees and ABI context change allocation without
  leaving a register trace. Evidence-minimal closure vs real locked groups: mean precision 0.88 / recall 0.73
  (`direct`), 0.67 / 0.87 (`full`); whole group contained in 6/18 (`direct`) to 11/18 (`full`). Use it to SEED groups,
  then let `probe.py` find the remaining knobs.
- The fully transitive closure is unusable (median 41,033 words); `chain` and `full` blow up through free-s* chains.
- Heads: `cloud/work/unregistered-heads-full.md` (52 heads, sizes, frames, callers, pointer tables) supersedes
  `unregistered-heads.md`. `highscore_entry_anim` is a tail of `func_80104704`.

## 5. Known hard problems (machines did not crack these; do not just add budget)

1. **Empty-function stubs at -O3** (`func_80096288`, `func_800BF01C`): IDO deletes an empty `if`, inlines the stub, or
   passes arguments through the stack. Blocks several groups (`display_list_alloc`, `slot_deactivate`,
   `results_screen_update`, `leaderboard_update`, `camera_clip_planes`, `mode_byte_set` without stand-ins).
2. **Dead `move s0,v0` after `slot_state_setup`** (all three tail groups): about 50 source forms tried, no movement.
3. **`func_8008B640`** index in `$a2` instead of `$a0`: 448 probe variants, no fix. Rule found: the index register is
   the first register above the member's outgoing call-argument registers. Leads: how `physics_velocity_integrate_a`
   sets up the call; putting the real `vector_normalize_length` (95 words) into the same unit.
4. **`func_8008705C` / `func_800878E0` with the real `func_80086A50`**: retail callers behave as if the callee clobbered
   a2/a3 but its words never touch them; probably needs a larger closure (`object_render`, `sound_init`).
5. **Group compile hardcodes `-O3` after the front end and `group.json` has one flags string**, so a per-unit
   -O2/-O3 mix cannot be expressed. If the original was built that way, this is a root cause worth a maintainer test.
6. **Whole-function alignment tail** (`.align 5` after an infinite loop) makes a code-correct function one nop off when
   compiled alone (`task_complete_signal`); three dummy predecessor functions reproduce it. Splice in place to check.

## 6. Conventions to keep (so local and cloud work stay mergeable)

- Claim only what scores strict MATCH (`group.json` `"claims"`); de-duplicate against `src/blob/` and `cloud/matches/`.
  `cloud/work/tools/set_claims.py` recomputes claims by rescoring.
- Stand-in-dependent matches get empty claims (the splicer refuses calls into stand-ins).
- Jump-table relocations used by a claimed function are refused; keep such files out of `cloud/matches/`
  (`track_lighting_setup` lives in `cloud/work/newtargets_hi/`).
- Scorer copies live in `cloud/work/tools/`, never in group dirs. `build/` is gitignored; do not commit caches.
- A match that depends on a quirk (volatile, unused local, defined-not-extern symbol, fewer named locals) is flagged in
  its PR text because it may not be the original source.

## 7. Suggested next steps, in order of expected return per unit of effort

1. **Run autopilot on local compute.** `triage.py --top 400 --class abi`, `autopilot.py --budget 1000 --jobs <cores>
   --parallel 4`, leave it overnight, then read only `worklist.json` entries with `suggest=machine` (retry with more
   budget and other seeds) and hand `machine+llm` / `llm` entries to a model with the residual from `report.py`.
   Because search is deterministic per seed, log the seed with every result.
2. **Wire `probe.py` into `autopilot.py` for IPA items** (currently not integrated): after a group builds but does not
   match, run the probe axes and apply the best variant before searching; this is where under-10% should improve.
3. **Feed the seeds better.** The biggest remaining gap is seed quality: `groupgen` bodies are 68% real m2c and only
   about 20% compile without the stub fallback. Improving m2c seed repair (undefined stack slots, non-pointer derefs,
   call-site type conflicts) removes a manual step for every function. `sigs.py` should replace m2c's register-ordered
   parameters everywhere.
4. **Extend the catalog from misses.** `autopilot.py --bench` lists the corpus functions search cannot solve; each
   missing edit class becomes a new mutation family (known gaps: new named locals, hoisted constants, pointer-stride
   and struct views of externs, scale-then-add FP idiom). The first improvement pass is recorded in section 3a; mine new misses from real autopilot runs (not the corpus) to
   avoid overfitting the development half.
5. **Fix `report.py` disassembly** (prologue/epilogue blocks print raw hex) and fit the `triage.py` priors to data
   from real autopilot runs (they are guesses today).
6. **Maintainer asks carried over:** register the heads in `unregistered-heads-full.md`; add `-r4300_mul` everywhere
   (done in `score.py`; confirm Conveyor jobs); treat `fabsf`/`sqrtf` as built-ins and order register parameters by
   stack slot in the seed generator (`sigs.py` implements the ordering); decide whether empty functions and
   stand-in callees can be supported at splice time; verify local statics/jump tables in a unit's own `.data`/`.rodata`.

## 8. Pitfalls learned (cheap to avoid)

- Score by instruction alignment while working; a positional diff count lies when one word is missing or extra.
- Compile in a per-job temp directory (IDO writes `src.u` into the cwd; parallel jobs collide otherwise).
- Keep multi-line source layout in any generated variant; IDO scheduling depends on it.
- `uopt` allocates per variable, not per web: named locals change allocation; dropping them fixed several functions.
- `search.py` now ranks strict diff first within 8 words of the goal (earlier it ranked aligned words first and could end worse than it started).
- The `flags` axis in `probe.py` has no effect on groups (see 5.5).
- `tools/m2c_patches` are not applied in the `tools/mips_to_c` submodule; `groupgen.py` builds a private patched copy in
  `build/groupgen_m2c/`. The submodule tree reads as modified after running the older scratch tools; reset it with
  `git -C tools/mips_to_c checkout -- . && git -C tools/mips_to_c clean -fdq` (the patches re-apply on demand).
- The earlier GPT review's "342 flagged / 93 covered" numbers could not be reproduced without the conveyor database;
  this toolkit's detector flags 364 and covers 114 with the old bounded rule (emulated).
