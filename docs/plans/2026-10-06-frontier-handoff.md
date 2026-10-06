# Frontier handoff: 2026-10-06 (after wave 12)

**For the next coordinating session.** This covers where the frontier matching effort stands, how a wave is run end to end, what the owner has decided, and every open item. Read it with `CLAUDE.md`, `cloud/work/frontier/WAVE_BRIEF.md` (the brief every lane agent reads) and `docs/plans/2026-10-04-frontier-plan.md` (the original method).

## 1. Where things stand (master `4d81f8dd`, CI green)

| | Functions | Bytes | % |
|---|---|---|---|
| Game code (compressed image) | 929 / 1216 | 271,600 / 647,072 | 41.97 |
| Static code (boot/library) | 399 / 668 | 73,292 / 160,720 | 45.60 |
| Combined bytes | | 344,892 / 807,792 | ≈42.7 |

`make progress` prints the first two rows; combined is just the sum. Report game and static separately, never as one denominator (CLAUDE.md).

**Frontier** (`python3 -m tools.conveyor.pipeline.frontier report`):
- **Remaining:** 287 unmatched game functions, 289 KB.
- **Ready now:** 29 singles (24 KB), 33 group functions (32 KB) and 5 unit functions (3 KB). "Ready" means every callee is already locked.
- **Provisional:** 17 functions (13 KB) are recorded in `cloud/work/frontier/provisional.json`. Their bodies are proven only with stand-in callers, so they are not ROM coverage.

**Yield per wave (game code):**

| Wave | Added | Kind of work |
|---|---|---|
| 9 | 15.4 KB | fresh large functions |
| 10 | 6.3 KB | |
| 11 | 10.0 KB | |
| 12 | 4.9 KB | near-miss closing |

The easy large functions are gone. What is left is near-misses, clusters that have to land together, and large functions whose callers are still unmatched.

## 2. How a wave is run

1. **Choose targets.** Use these commands:
   - `frontier next --limit 200`: ready functions with recipe single, group or unit.
   - `frontier show NAME`: callees, callers, register facts (IPA), blockers and prior source paths.
   - Also read the previous wave's `cloud/work/frontier/w<N><lane>/RESULTS.md` files for near-misses and their traced causes.
2. **Size the wave.** Use 5–9 lanes, each with 1–6 functions grouped by subsystem or call cluster. Keep within the session's agent guideline (under about 10).
3. **Write a rules file** in the scratchpad (the last one was `w12common.txt`; text in §8) and launch one background `Agent` per lane.
   - Each lane prompt names its lane id, local dir `cloud/work/frontier/w<N><x>/` and builder scratch `~/rush2049/scratch/frontier/w<N><x>`.
   - It lists the targets with byte sizes, the current residual and where the best prior source is.
   - It ends with "read the shared rules file and follow it". Template in §8.
4. **Agents write only to their lane dir**, plus `cloud/matches/NAME.c` for functions that match standalone. They never splice, commit or edit `src/`, `asm/`, locks or `tools/`.
5. **Integrate as lanes report.** Do it on a branch, then push to master. The full sequence is in §3.
6. **Report to the owner.** Give each lane's result in a few lines, a running tally, and an updated coverage table after every push.

The shared tracing toolkit lives in `cloud/work/frontier/tools/trace/`; read its `README.md`. Lanes run `install.sh <their scratch>` or `--reuse ~/rush2049/scratch/frontier/wtk`. It provides:
- `us.sh`: unit score plus row counts.
- `udiff.py --ops/--norm`: aligned and opcode diffs.
- `ctrace.sh` / `sum.sh`: register-colouring decisions.
- `force.sh`: the forced-colour oracle.
- `spill.sh` / `spcensus.sh`: frame and spill layout.
- `ugt.sh`: code generator (ugen) evaluation order.
- `as1t.sh`: assembler (as1) scheduling.
- `asm.sh`: reassemble a hand-edited listing.

Every traced binary is checked to give output identical to the stock compiler.

## 3. Integration procedure (do not skip steps)

Run on the Pi from the repo root, on a branch such as `git checkout -b wave13`.

1. **Single functions** (the file in `cloud/matches/NAME.c` has a bare `/* flags: … */` on line 1):
   `PYTHONPATH=. python3 cloud/work/frontier/tools/splice_singles.py NAME …`
   - It compiles each function standalone and runs the image gate.
   - A refusal like "compiled body is shorter than the extent" or "first difference at …" means the match depends on whole-program inlining or IPA. Package it as a group (step 2); w12l did this for three functions.
2. **Groups:**
   `PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py <lane>/groups/<g> <name> [--revert-group OLD] [--revert-single STUB] [--provenance TEXT]`
   - **Name:** use the same name as the locked group the new one supersedes. Tests and research packets look groups up by name. Renaming broke `compare_group` once; renaming back fixed it.
   - **Members:** the group's `members` must keep every old member, plus every locked stub it now gives a real body. If it doesn't, the install fails "functions locked before are unlocked now". The stubs are reverted with `--revert-single`.
   - **Overrides:** stubs that become real inlined helpers need entries in `src/blob/unit_overrides.json`:
     - `prefer_definition` pointing at the group file;
     - `force_internal`.
     Add them **before** installing. If `blob_unit` complains that an override points at a missing group file, the group isn't installed yet.
   - **Missing objects:** if an install fails and a later one reports "object missing build/blob/obj/X.o", rebuild the object:
     `PYTHONPATH=. python3 -c "from pathlib import Path; from tools.conveyor.pipeline import blob_splice; blob_splice.compile_on_builder({'X': Path('src/blob/X.c')}, '<flagset from lock>')"`
     The rollback now recompiles reverted singles too.
3. **Gates. All must pass:**
   - `python3 -m tools.conveyor.pipeline.blob_unit check` → N locked, N equal, 0 differ.
   - `python3 -m tools.conveyor.pipeline.blob_group check` → 0 problems.
   - `python3 -m tools.conveyor.pipeline.blob_splice check` → 0 problems.
   - `python3 -m tools.conveyor.pipeline.lock check` → all 403 static locks intact.
   - `python3 -m tools.conveyor.pipeline.blob_rom rom` → `MAKE=0 TEST=0` (full-ROM SHA-1).
   - `make progress` → the new numbers.
4. **If a match closes a provisional entry,** remove that entry from `provisional.json`. Keep the file's 1-space JSON indent so the diff is only the removed lines. `tests/conveyor/test_frontier.py` checks the seeded entries.
5. **Commit**, ending the message with the co-author line. Include the lane dirs.
6. **Tests, both environments, before pushing:**
   - **Pi:** `python3 -m pytest tests/conveyor tests/cloud -q -m "not node_required" -p no:cacheprovider`. The Pi has no IDO, so compiler tests skip there.
   - **Builder (CI-equivalent):**
     1. On the Pi, bundle HEAD **plus all remote branches**: `git bundle create X.bundle HEAD --remotes=origin`.
     2. scp it to watchman2.
     3. In `~/rush2049/scratch/ciclone6` (a disposable clone; submodules are copied from `~/rush2049/scratch/ci`):
        `git checkout -q . && git clean -qfd cloud tests && git fetch -q ../X.bundle HEAD "refs/remotes/origin/*:refs/remotes/origin/*" && git checkout -q FETCH_HEAD`
     4. Run `REQUIRE_TOOLCHAIN=1 ~/rush2049/scratch/civenv/bin/python -m pytest tests/conveyor` and the same for `tests/cloud`, both with `-m "not node_required"`.

     Remote branches are needed because some research packets `git show` pre-purge commits that exist only on old unmerged branches such as `origin/dot/palette-steering-20261005`. **Never delete unmerged remote branches.**
7. **Push** with `git push origin HEAD:master`, then `git branch -f master HEAD`. Watch GitHub CI with `gh run watch <id>` until it passes.
   - CI's "Rescore changed cloud submissions" step rescores every changed `cloud/matches/*.c` standalone.
   - A function that is EQUAL only in a group or the unit must **not** be in `cloud/matches/`; its source lives in the group dir.

**Research-packet breakage** (Astra's `cloud/work/**/verify.py` plus `tests/cloud/test_*`) is the usual post-splice test failure. Fix it in the established ways only (§5).

## 4. Owner decisions and standing rules

- **Astra's functions:** do not work on `func_80087110`, `stat_race_update` or `func_800FE5B0`. Astra (GitHub `cabi24`, PRs from `dot/*`, `work/*` and `research/*` branches) owns them.
- **Unused local declarations** are acceptable only when they are the **exact frame residual** and are disclosed in the file header. The owner approved this in wave 11. Measured rule: every function-level named local takes a slot, used or not.
  Pad arrays standing in for unknown structure elsewhere are not acceptable.
- **Shaping devices** must always be disclosed in the header, and a match needing them is accepted with that disclosure:
  - compiled-out reads such as `if (x) {}`;
  - compiled-out debug switches (`func_800CCB40`'s `DEBUG_FORCE_PAL`, which the owner approved);
  - shaping helpers of unknown identity (`eu_nop`, `attach`).
- **Not acceptable:**
  - a false prototype, such as declaring a void function as returning `s32`;
  - pinning stand-ins as matches.
  Implicit int (no declaration in scope) is **an open decision**; see §6.
- **Provisional rule:** a body that matches only with stand-in callers goes in `provisional.json` and is not spliced. It becomes a match when its real callers match in the same group.
- **Pushing:** push gate-passing, test-passing work to master as it lands; the owner wants master current. Force-push only with explicit owner approval (one history purge was approved).
- **Hook-protected files:** do not edit `tools/cloud/score.py` directly. Changes go through a reviewed patch with owner approval.
- **Permission denials:** if the permission classifier denies an action, do not retry it another way. Surface it to the owner.
  - Lanes are told the same; two wave-9 lanes broke this with `rm`.
  - The owner's blanket approval did not override a "Git Destructive" denial for branch deletion; see §6.

## 5. Research-packet fixes: the established patterns

Astra's verification packets bind receipts to hashes. Integration breaks them when it changes something they pin. These fixes are owner-approved (the "test fix" class):
- **Whole-manifest or lock pins.** Hashes of `asm/us/blob/SHA256SUMS`, per-function `asm/us/blob/*.s` text (each splice adds a `/* compiled from … */` line), or `blob_matched.lock.json`: drop those keys from the comparison, inside the test or the packet's `--check`. Selected native bodies stay bound.
- **Superseded context group.** Copy the old group byte-identically into `cloud/work/frontier/superseded/<group>/`, with a README naming the packet. Make the packet resolve the old path there, keeping the receipt's recorded paths unchanged.
  - Examples: `dot_directional_sound`, `dot_audio_force`, `dot_object_sound` (via `context_path()`), `dot_quadtree_donor` and `dot_heap_max`.
  - Update the packet's own `verify.py` hash in its receipt.
- **"Function not yet locked" assertions.** Allow the promoted state: `NAME not in locks or locks[NAME]['source'] == 'src/blob/NAME.c'`. Applied to `masked_random` and `nearest_point_donor`.
- **Missing IDO** is handled centrally in `tests/conftest.py`: it becomes a toolchain skip, which in turn fails under `REQUIRE_TOOLCHAIN=1`.

## 6. Open items: what needs communicating, and to whom

**To the owner:**
1. **Merged remote branches can be deleted by the owner.** This was blocked for Claude as "Git Destructive". 70 merged remote branches can go; deleting them loses nothing:
   ```
   ! open=$(gh pr list --state open --json headRefName -q '.[].headRefName'); br=$(git branch -r --merged origin/master | grep '^  origin/' | grep -v 'origin/master\|HEAD' | sed 's|  origin/||'); for b in $open; do br=$(echo "$br" | grep -vx "$b"); done; git push origin --delete $br
   ```
   It must stay restricted to `--merged`; unmerged branches hold commits that research tests need.
2. **Decision: implicit int.** `func_800B59F0` (1,372 B) compiles identically with no false prototype by leaving the void callee `func_8008D870` undeclared (C89 implicit int), plus `(s16)` casts (w12h). This is the same mechanism as the rejected false prototype, arising from a missing header. The function is provisional until its caller `physics_sym` matches, so the decision isn't urgent.
3. **Attribution doubt on `func_800AC660`.** It is in the locked `frontier_level_objects` group as `transmission_ratio_get`'s inlined getter, a naming hypothesis. w12d found that a static getter defined after `differential_output` leaves its stub exactly at that address, so it is probably `differential_output`'s flags getter. The bytes are unaffected (`jr ra; nop`). Settle it when `differential_output` lands: the stub should then move to that group.
4. **Cluster-landing decisions to expect:** both near-complete clusters (below) carry disclosed shaping.
   - Mode-select: two unused locals, the `for(i=0,n=4;i!=n;i++)` form, and a `func_800E0048` camera-wrapper identity hypothesis.
   - Camera: the empty-then `if (flags) {} else {…}` form.
   These fall within the standing rules; mention them when landing.

**To Astra:**
5. **PR #163 (draft, 443 files).** It merges cleanly but fails five of its own replays with pinned IDO on x86. Hold it until Astra rebases. The note was given to the owner to forward; if it wasn't sent:
   > #163 merges cleanly onto master but these fail with pinned IDO on an x86 Ubuntu builder (REQUIRE_TOOLCHAIN=1): test_runtime_a_flags::test_fresh_o3_replay pins gnu_ld/host_cc version strings (drop them from the compared receipt); late_packet_portability[153/156/160] (event_ring, lives_text, e114_parent) report frozen/source-bound receipt drift; runtime_b_closure reports "negative control drift". Please rebase on current master (waves 9–12 spliced ~45 functions, superseded frontier_list_alloc_sound / camera_scene_manager / frontier_traction_control / func_800AD4C8 / codex_vsync_a145 / audio_heap / camera_aspect_ratio / func_8008B640, and changed func_800B61A8's source) and rerun the full matrix with IDO before un-drafting.

   #163 also carries `func_8001F954`'s production-compatible adaptation (`cloud/matches/voice_unblock_production/`), a static promotion worth 124 B. Promote it with `.claude/skills/promote-match` once #163 lands. That command needs a clean tree and commits on success.
6. **Astra's own items** (keep off them): `func_80087110`, `stat_race_update` / `func_800FE5B0`.

**Matching work. Open near-misses**, all with a traced cause. Paths are under `cloud/work/frontier/`.

| Target | Bytes | Gap | Cause and best source | Lands with it |
|---|---:|---|---|---|
| func_800E05F0 | 1328 | 30 word rows | One `.alias $8,$sp` that uopt places after `jal player_conditional_call`. Moving it in the listing leaves only the unverified 0.6f/0.1f words. `w12i/RESULTS.md`, `w12i/comp/mode.c`, traced alias patch `w12i/tools/uopt_alias_patch.py`. | **Mode-select cluster, ≈8.4 KB.** 9 bodies already EQUAL with real callers (`w12i/comp/run.sh`): mode_select_handler, func_800E0050, func_800D5E64, func_800DFBA0, best_times_display, func_800DED78, mode_select_input, func_800DEF60, func_800E0048. Landing recipe in `w12c/RESULTS.md` plus `w12i/RESULTS.md`. |
| camera_update | 2876 | 17 words | 8 words and 4 words are each a register that one forced colour fixes (an "invisible holder" in retail); 5 words are compare operand order. `w12f/RESULTS.md`, `w12f/camera_update/best.c`, group `w12f/groups/camera_aspect_ratio/`. | camera_free_look, camera_track_spline (both EQUAL), ≈4.2 KB |
| entity_process_main | 1424 | 2 words | as1 operand order shift, load, not. `w11a`, `w12b/RESULTS.md`. | |
| func_80096130 | 264 | 3 words | One `.alias $3,$sp`; the hand-edited listing gives 0 rows. `w12e`. | |
| audio_channel_setup | 332 | 3 words | as1 copy coalescing into an argument register. `w12e`. | |
| differential_output | 648 | 13 words | Register only: forcing 2 colours gives 0. `w12d`. | settles `func_800AC660` |
| func_800E7A98 | 172 | 14 words | `w11a`, `w12e` | |
| object_bytes_sum_global / object_bytes23_sum | 84 / 72 | 8 / 4 | Not `func_80096288`'s body (refuted). `w12e`. | |
| audio_channel_priority | 468 | 11 words | Register only (6 forced colours give 0). `w12e/audio_channel_priority/stride.c`. | |
| audio_doppler_full | 952 | 39 words | One statement makes 3 fewer ugen temp allocations. `w12g`. | |
| camera_look_at_point | 796 | 38 words | `w12f`, `w11c` | |
| physics_sym | 1252 | 4 rows with 8 forced colours | New draft, `w12h/physics_sym/` | func_800B59F0 (see item 2) |
| input_deadzone_apply | 3580 | 122 words | ugen compiles it twice; its float free-list carries over from the first pass. `w12a`. | input_process_controller (provisional) |
| func_800E847C + func_800E7FA0 | 2100 + 1244 | 232 / 35 | Web numbering: retail's late values must be compiler-created. `w11g`. | |
| race tail / drone pair | | far | drone_ai_update + entity_tick_main frames are ≈300 B unexplained. `w11e/drone`. | |

Newly unblocked and untouched: **entity_spawn_init**, after func_8008E408 matched.

## 7. Techniques that close functions (cumulative, highest yield first)

1. **Inlined locked helpers.** Big functions inline already-locked kept functions: rand `func_8008B2B4`/`func_8008B2E4`, LCG `func_800D50E4`, `func_800B930C`, `func_800B61A8`. Paste the definitions into the candidate, or put the locked file in the group as context.
2. **Caller-less `jr ra; nop` stubs are deleted inlined statics.** The number of stubs caps how many helpers a file can have. Name the helper after its stub (non-static, made internal through overrides); otherwise the scorer reports "1 extra words".
3. **Frame layout is fully determined:**
   - Named locals take slots top-down in declaration order, used or not.
   - Block-scoped locals come next.
   - Each inlined call's area follows, 8-aligned, in call order.
   - Spill temps come last: one slot per coloured expression web, in first-appearance order.
   - So a +4 shift of the temp area means the last inlined area has an odd word count (add or move an inlined call), not a missing variable.
   - Measure with `Udef Mmt` (`w12h/tools/mu.sh`) or `spill.sh`.
4. **Register allocation works in two phases:**
   - Phase 1 colours webs with at least 22 interferences, by priority.
   - Phase 2 assigns the rest in web-number order (first appearance in source); copy-propagated values are numbered after all named ones.
   - Each procedure has one callee-saved cost; a web whose total saving is below it gets split.
   - Run `force.sh` first: if forcing reaches 0 rows, the residual is register colour only.
5. **Levers for colour ties:**
   - A compiled-out `if (x) {}` adds a use with no code; `if (a|b|c) {}` raises several webs at once.
   - `t = a; t -= b;` priority splits.
   - Reusing one variable across loops.
   - A dead `x = y = 0` renumbering.
   - Operand order of `==` and commutative operators.
   - Unsigned casts set multiply operand order.
   - Each dead read raises the callee-saved cost, so don't stack them blindly (w12d).
6. **Directive and assembler residuals:**
   - uopt (not ugen) emits `.noalias`/`.alias`.
   - A `.alias` between an unconditional `b` and the next label blocks as1's cross-block hoist.
   - A pointer to a global assigned early in another block gets no `.noalias`.
   - Prove these by editing the listing and reassembling with `asm.sh`; w12b's debug as1 is `tools/build_as1dbg.sh`.
7. **Source idioms:**
   - Index global arrays directly rather than through pointer locals; stores through a pointer alias every global.
   - Write natural float literals rather than `extern f32 D_8012xxxx`; the scorer verifies own rodata.
   - Use `volatile` where retail reloads.
   - A function-local static for run-once flags.
   - Line layout changes as1 scheduling: write multi-line wrappers.
   - An empty-then `if (c) {} else {…}` forces a reload.
   - Recursion where a small tree walk gets the wrong saved registers.
   - The register-parameter order of a callee follows the caller's argument set-up order.
8. **Re-score old drafts in the unit** before writing new source; two wave-10 matches came from early drafts that later lanes had replaced with worse ones. `blob_unit score NAME --internal NAME` can show that a "single" label is wrong.
9. **Check older drafts' global addresses** against the retail disassembly; w9e found `D_801493D0` where the real global is `D_801392D0`.

## 8. Templates

**Shared rules file** (scratchpad `w<N>common.txt`; lanes substitute their name for `<lane>`):
```
First read `cloud/work/frontier/WAVE_BRIEF.md` in full and follow it — especially "Wave 9 techniques",
"Shared tracing toolkit", "Wave 12 rules" and "Permission denials" — plus what it points to. Run
`python3 -m tools.conveyor.pipeline.frontier show NAME` and grep cloud/work for prior attempts first.
Hard rules:
- Do NOT work on func_80087110, stat_race_update, or func_800FE5B0 (another contributor's), nor on other lanes' functions.
- Do not commit, splice, push, or edit src/, asm/, *.lock.json, tools/, include/, tools/cloud/score.py,
  src/blob/unit_overrides.json, provisional.json, or other agents' dirs. Deliverables: cloud/matches/NAME.c ONLY
  for standalone `score.py fn` matches; group dirs under your lane dir with "claims" and a complete "members"
  list (same name and all old members of any locked group superseded; locked stubs given bodies are members);
  RESULTS.md with exact integration notes (overrides needed, groups superseded).
- Confirm every match in the whole-program unit (`blob_unit --tag <lane> score ... --neighbours`, 0 locked
  bodies differing) and quote the output. A match with stand-in callers is provisional.
- Verification scripts must replay against the current tree (no whole-manifest/asm-text/lock pins, no
  `git show` of non-ancestor commits).
- Builder: at most 2 cores, only your own scratch. Breadth over depth: ~50 variants without movement → write up, move on.
- If a permission check denies anything, do not retry it in another form; note it and continue.
Use the shared toolkit cloud/work/frontier/tools/trace/ (README.md; install.sh into your scratch, or --reuse ~/rush2049/scratch/frontier/wtk).
Final message: RESULTS table (function, bytes, state, flags, exact scorer output line), integration notes, what generalises.
```

**Lane prompt:**
```
You are matching-wave agent **w13x** in the Rush 2049 N64 decompilation at /home/cburnes/projects/rush2049-decomp
(Raspberry Pi coordinator; IDO only on builder `watchman2`). Lane dir `cloud/work/frontier/w13x/`; builder scratch
`~/rush2049/scratch/frontier/w13x` (copy from `~/rush2049/scratch/frontier/base`, sync src/blob, include,
tools/cloud, asm/us/blob and the lock from the Pi first).
**Assignment:** <functions with bytes, current gap, traced cause, best prior source path, what lands with it>.
Read the shared rules file `<scratchpad>/w13common.txt` and follow it (substitute w13x for <lane>).
```

## 9. Suggested wave 13 (small, focused)

| Lane | Work |
|---|---|
| a, b | Two different approaches on `func_800E05F0`'s `.alias` placement. Use `w12i/tools/uopt_alias_patch.py` to find what keeps the pointer's live range open across the camera and set paths. Each lane works in its own copy of `w12i/comp/`. **Payoff: ≈8.4 KB cluster.** |
| c | `camera_update`'s invisible-holder registers and compare order. **Payoff: ≈4.2 KB with free_look and track_spline.** |
| d | The 2–3-word residuals (entity_process_main, func_80096130, audio_channel_setup), plus differential_output (settles func_800AC660). |
| e | `physics_sym`, which would land func_800B59F0 if the owner accepts implicit int, plus the fresh `entity_spawn_init`. |
| f (optional) | Ready singles and groups nobody has touched: `frontier next`, minus everything listed in §6. |

## 10. Pointers

- Wave lane results: `cloud/work/frontier/w9a` … `w12l/RESULTS.md`. The newest are the most useful.
- Superseded groups kept for packets: `cloud/work/frontier/superseded/`.
- Tools: `cloud/work/frontier/tools/` (`splice_singles.py`, `install_group.py`, `trace/`).
- Brief that every lane reads: `cloud/work/frontier/WAVE_BRIEF.md`; add each wave's lessons there.
- Builder: `ssh watchman2`. The toolkit IDO is at `~/rush2049/cache/toolkits/796ae99a…bfbf5/ido`. Never touch `~/rush2049/repo` or the shared `/tmp/blobsplice` and `/tmp/blobgroup`.
