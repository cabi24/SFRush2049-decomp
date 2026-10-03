# Rush 2049: full-project continuation plan

**Audit date:** 2026-10-03. **Source baseline:** `53d0fba0c5f44e91fe921f722fee7ff63a2dd0fe` on `master`.
**Purpose:** a shared, executable program for Claude, Codex and other contributors to keep advancing the entire US decompilation, including when the nearest compiler matches are frozen.
**Change type:** planning/documentation only. This plan adds no accepted functions, source ownership, matching bytes, compiler results or ROM proof.

## Read this first

1. Read [CLAUDE.md](CLAUDE.md), the [constitution](.specify/memory/constitution.md), and [dot_response](dot_response). The owner's pause on `func_800D1248` and its helper-context investigation remains absolute. It is excluded from every census dispatch, reconstruction, compile and experiment below. Merely preserving an inventory row does not authorize work on it.
2. Use this file for program direction, [work packages](docs/plans/2026-10-03-work-packages.md) for concrete assignments, and the [audit and validation record](docs/plans/2026-10-03-audit.md) for evidence and limitations.
3. Use [dot_handoff.md](dot_handoff.md) for existing packet history and current owner/branch updates. This plan's explicit readiness corrections take precedence over its older shortlist recommendations. Neither document silently assigns an owner.
4. Recheck current `master`, locks, PRs, claims and source history before starting. Dates and baseline SHAs make a snapshot reproducible; they do not make it permanently current.
5. A packet labelled **READY-RESEARCH** is useful work that can begin now. It is not a matching-ready function. A new execution packet must satisfy the admission checklist below.

### Program decision

Do not spend the next wave repeatedly recompiling the same nearly matching functions. Run three kinds of work in parallel:

- **Make acceptance mechanically trustworthy:** fail closed when locked C bodies or their fresh provenance are missing; cover the actual research/tooling test suites and explicit evidence routes.
- **Make new source progress:** reconstruct genuine state-machine regions whose native evidence already exists, repair ABI and archive provenance, and survey every remaining native interval with semantic identities rather than misleading labels.
- **Build a sustainable execution queue:** admit complete, evidence-backed medium/large game packets, retain a small disjoint static lane, and serialize private acceptance through the real image/compression/ROM gates.

The reviewed old D02/D04/D05/D06/D08/D09 lanes do not supply a justified pair of matching-execution-ready packets. This does **not** mean every one of the 550 unmatched game intervals is blocked. A whole-population archive/ABI/source census is work still to do, with concrete partitions below. No matching percentage or completion date is forecast from an unevaluated queue.

## 1. What the project actually builds

There are two production paths, and several supporting research layers:

| Layer | Canonical assets | Meaning and boundary |
|---|---|---|
| Static cartridge code | `splat.us.yaml`, `src/rom/`, static assembly, `matched.lock.json`, linker/owned-data configuration | ROM-aligned translation units; current flags, headers, data ownership and unchanged accepted neighbors must survive promotion. |
| Compressed game image | `src/blob/`, `blob_matched.lock.json`, generated blob layout/TUs, `tools/conveyor/pipeline/blob_*` | Linked C replacements occupy exact native intervals; remaining intervals/opaque regions are retained by the image builder. Exact compressed stream then enters the cartridge. |
| Candidate/research source | `cloud/matches/`, `cloud/work/`, `work/`, `m2c_output/`, `ghidra_output/`, `codex_homework/` | Different confidence levels and ages. A C definition is not necessarily complete, correct, compiled, matched or linked. |
| Historical subsystem model | `src/game/` and other historical explanatory sources; topic docs | Valuable clues, not a coverage instrument. Many names are hypotheses; production source ownership must be traced through Make/TU/blob inputs. |
| Reference/context | `reference/`, `include/game_types.h`, `disasm.GAME_SYMBOLS`, generated symbols/prototypes | Arcade ancestry helps recover semantics, but N64 rewriting, layout and native calling conventions remain authoritative. |
| Proof/operations | Conveyor, cloud scorer, workbench, tests, CI, pinned IDO, coordinator/builder | Supports repeatable evidence. A green research CI result is not a fresh native-body or cartridge proof. |

This distinction prevents two costly mistakes: polishing an unlinked historical subsystem file as though it increases cartridge coverage, and combining plausible helper definitions into a fictional original translation unit to manipulate register allocation.

### Coverage snapshot and denominator policy

The accepted integration receipt at [`cloud/work/pr_integration_20261003/gates.json`](cloud/work/pr_integration_20261003/gates.json) records:

| Metric | Accepted numerator | Denominator | Reported result |
|---|---:|---:|---:|
| Game functions | 666 | 1,216 | Count of accepted game functions |
| Game bytes in the image tracker | 100,656 | 647,072 | 15.56% |
| Static functions | 158 | 230 | Count of promoted static functions |
| Static native bytes | 44,808 | 61,440 | 72.93% |
| Existing combined byte tracker | 145,464 | 708,512 | 20.53% |

**Important:** 647,072 is the size of the whole decompressed game image. It includes opaque/data regions and must not be described as a proven executable-code denominator. Static and game functions remain separate populations; 161 static lock records do not imply 161 promoted cartridge functions. The public tracker is retained here for continuity, with its actual definition stated explicitly. Package P01 turns the audited reconciliation into a reproducible live baseline before any reporting change.

Fresh protected-target metadata reconciles 561,008 registered function-interval bytes plus 86,064 bytes in 11 opaque regions to the 647,072-byte image. Matching every registered function would therefore cover 86.70% of this image denominator; the rest requires separate data/opaque classification. This tracker is also not a percentage of every cartridge asset or ROM byte.

The unmatched-game CSV contains 550 intervals and 460,352 bytes:

| Native interval size | Count | Bytes | Planning consequence |
|---|---:|---:|---|
| Under 256 bytes | 135 | 22,592 | Useful closure helpers or genuine low-risk matches; do not let allocator plateaus dominate the team. |
| 256–1,023 bytes | 274 | 153,048 | Largest count of medium opportunities; partition by address and actual context. |
| 1,024–4,095 bytes | 135 | 243,304 | Largest remaining byte pool; prioritize complete reconstruction and real dependency closure. |
| At least 4,096 bytes | 6 | 41,408 | Scout architecture and data first; these are not automatic high-yield assignments. |

The 72 unpromoted static functions account for 16,632 bytes. Completing only that remainder cannot reach the existing 50% combined milestone. With that tracker held constant, reaching 50% requires another 208,792 accepted bytes; even hypothetical completion of all static code leaves 192,160 game bytes to accept. These are arithmetic constraints, not workload or schedule estimates.

### Evidence levels used everywhere

| Label | What has actually been established |
|---|---|
| HYPOTHESIS | A name, donor or structural explanation to investigate. |
| SOURCE-LEAD | A definition was found; completeness and identity are not established. |
| PARTIAL-SOURCE | Genuine bounded regions reconstructed, with explicit missing regions/contracts. |
| COMPLETE-NONMATCH | Whole intended body exists; semantic assumptions, extent and compiler result still need independent checks. |
| READY-RESEARCH | An executable, bounded research/tooling question with available inputs and a useful output. |
| READY-IMPLEMENTATION | A bounded tooling, test or infrastructure change can begin with available inputs; it is not a matching-ready game function. |
| PRIVATE-INPUT / PRIVATE-GATE | A specifically named private provenance or original-input verification step requires the authorized maintainer; independent offline preparation may proceed. |
| READY-RECONSTRUCTION | Whole native extent and sufficiently established contracts support a credible complete-source route. |
| READY-MATCH | Complete source, recipe, native identity and genuine context are bound; a specific untried mismatch hypothesis can be tested. |
| VERIFIED-BODY | Fresh strict relocated whole-body equality, including length/excess words and all required data/table evidence. |
| ACCEPTED | Production integration preserved existing accepted source bodies and passed complete image, exact compression and full-ROM gates, as applicable. |
| EXTERNAL-INPUT | A specifically identified service, table, original-TU or runtime mapping contract is missing. |
| FROZEN | A prior hypothesis is exhausted; reopen only on stated new evidence. |
| OFF-LIMITS | Owner restriction; no automated reinterpretation or expiry. |

Host behavior tests, bounded mock tests, compiler masked scores and historical receipts retain their separate meanings. Do not turn one into another by renaming a status.

## 2. Immediate first wave

Start with these independently useful packets. They are proposed roles, not assignments. Record actual ownership before editing.

| Lane | First packet | Why it goes first | Output that unlocks the next step |
|---|---|---|---|
| Acceptance engineer | **V01** fail-closed locked-body coverage | A full image hash can be true even when a missing C replacement silently falls back to passthrough. Recent acceptance manually checked all 666; make that invariant automatic. | Negative-tested exact expected/built membership and fresh source/object binding before composition. |
| Test/tooling engineer | **V02** explicit cloud test coverage | Current default/CI test selection does not include `tests/cloud`. | Classified, reproducible suite manifest; repaired environment assumptions; explicit CI coverage. |
| Archive/data steward | **P03** source/recipe/receipt matrix | Existing source was repeatedly mistaken for absent source, and scores were compared across different contexts. | Address-keyed strongest known evidence and exact missing input, starting with the live queue then all remaining intervals. |
| State reconstructor | **R01–R02** FB2C8 known-region source/ABI | Complete native text can be recovered from tracked overlapping targets; no new ROM input is needed to reconstruct known regions. | Reviewed partial source plus prelude/callee contracts, preserving unresolved external calls. |
| Runtime provenance analyst | **R16** loaded-image identity | Several blocked service addresses can belong to different images loaded at the same address. | Image-specific contract requests that unblock state, C974, viewport and collision work. |
| Broad game scout | **S01–S10**, claim one P04 address partition first | The old shortlist is exhausted, not the whole project. | At least one honest admission/no-go packet per reviewed family; every rejected candidate has a reason and reopen condition. |
| Coordinator/reviewer | **P01–P02**, then review above outputs | Prevents double work, false percentages and unreviewed promotion. | Reproducible baseline, live claims, bounded next tasks, independent reviews. |
| Optional disjoint static specialist | **S12** static ownership census | Remaining library work can advance without speculative game-context sweeps. | Three screened slots or an evidence-backed no-go list, not assumed matches. |

With fewer contributors, preserve one reconstruction lane and one verification/archive lane rather than running only compiler sweeps. With more contributors, split by disjoint native intervals and source paths after P02; do not give several people the same frozen residual. A role can be handled by Claude, Codex or a human. Choose reasoning effort and execution environment according to the actual assignment, not as a substitute for evidence.

### What not to dispatch as fresh work

- Renderer B119 already contains complete rectangle, palette, disable/mask-clear, flags, initializer and mode bodies. PRs #48/#49 add useful packaging/ABI/behavioral evidence; they did not discover previously missing roots. The next renderer question is original-TU/export/lifetime evidence, not another invented closure.
- The C974 call to `8038D798` has an unknown service contract. Proving its local three-pointer helper did not close that external boundary.
- FAF6C requires a genuine FB2C8 caller and private calling-convention context. A partial fast branch is meaningful source progress, but not the complete matching closure.
- HUD/minimap receipt recovery does not resolve incomplete source-to-receipt binding or broad frame/register mismatches.
- Tire split-boolean, texture scheduling, matrix/array permutations, rejected inflater closures and old small allocator searches do not become new experiments when ownership changes.
- `func_800D1248`, including the paused helper-context experiment, remains excluded.

## 3. Work breakdown and dependency graph

The [work-package catalog](docs/plans/2026-10-03-work-packages.md) provides entries, deliverables, tests, dependencies, owner roles and stop conditions. IDs are stable. Use child IDs such as `S04.1` for admitted concrete targets; never recycle an ID for a different native body.

### Program lanes

| Family | Scope | Work packages |
|---|---|---|
| P: trustworthy coordination | Baseline/denominators, claims, archive/source identity, target extents, semantic map, docs | P01–P07 |
| V: verification and operations | Fail-closed source ownership, test reach, receipt integrity, toolchain, scorer, generation, farm, builder | V01–V11 |
| R: concrete prerequisite chains | State transition, runtime service contracts, C974, renderer context, HUD, viewport | R01–R16 |
| S: whole-project expansion | Physics, geometry, paths/AI, rendering, UI, game/input, audio, persistence, resources, very large and static targets | S01–S13 |
| F: frozen residual management | Tire, texture, matrix/array, scheduler/inflater and historical small-search families | F01–F05 |
| I: execution and acceptance | Complete reconstruction, compiler diagnosis, static and game integration, reporting, gameplay smoke | I01–I07 |
| M: later milestones | Sustained medium/large program, long tail, whole-source proof, US release and version scope | M01–M06 |

### Critical dependency chains

```text
P01 baseline + P02 claims + P03 archive identity
  -> P04 full native extent census + P05 semantic subsystem map
  -> S01..S12 screened new families
  -> I01 whole reconstruction -> I02 bounded matching -> I03 or I04 acceptance
  -> I05 verified progress -> M01 sustainable waves -> M02 long tail

V01 locked-body/source proof + V03 receipt identity + V05 strict scorer tests
  -> V08 independent acceptance receipt -> I03/I04 -> M03 complete source image

R01 state fragment review -> R02 real prelude/local callees
  -> R04 full state body (also requires R03 external services)
  -> R05 genuine caller/TU experiment -> I02

R16 loader/image identity -> R03 state / R06 C974 / R15 viewport service contracts
R06 C974 external contract -> R07 complete C974 source -> I02
R08 renderer archive delta + R09 genuine TU/lifetime evidence + R10 table proof
  -> R11 one predicted renderer experiment -> I02 only if justified
R12 HUD receipt binding + R13 new layout explanation -> I02 only if justified
R14 viewport wrapper ABI + R15 external contracts -> I01
```

Dependencies apply to the dependent claim, not every useful substep. For example, known state-machine regions can be reconstructed while mode-six services remain unresolved; claiming the full ABI or executing a complete caller-context match cannot. A private table placement receipt can close a proof gap without explaining allocation. Keep those outputs separate.

## 4. Whole-project coverage strategy

Every remaining interval must eventually have an address-keyed record. Partition the work by nonoverlapping native address range first; annotate subsystem only after inspecting evidence. Historical names such as `sound_init`, `transmission_shift`, `world_gravity_apply`, `assign_drones` or `drone_ai_update` can describe the wrong behavior.

For each interval, answer in order:

1. Is it a genuine full function, a suffix, an overlapping alias, data or an uncertain boundary? What scanner/closure and tracked evidence establish its extent?
2. Is it accepted, claimed externally, paused, already represented in source, or dependent on accepted context? Check locks and actual native intervals, not just symbol strings.
3. What is the strongest existing complete source, donor and source-bound compiler receipt? Where do older controls differ semantically or structurally?
4. What incoming registers, outputs, memory effects, tables, pool values, callback mutation, signedness and helper contracts remain unresolved?
5. Which work product is now justified: complete reconstruction, contract/structure audit, table proof, original-context research, a bounded compiler experiment, or a frozen no-go?
6. What disjoint helper/field evidence does the work unlock beyond its own byte count?

The scope includes physics/drivetrain/tires; collision and world geometry; navigation/checkpoints/ranking/drones; camera/model/render commands; HUD/menus/results; state/boot/input/multiplayer; audio and runtime services; save/unlocks/replay; allocation/resources/streaming; static boot/libc/libm/libultra; compressed-image data ownership; the wider cartridge asset/microcode/data layout; and the build, testing, operations and release pipeline. No unverified percentage is assigned to a named subsystem.

### Candidate selection policy

Rank candidates by **unblock value and evidence quality**, then expected accepted bytes:

- Prefer a missing truthful contract or complete source body that serves several genuine callers over a speculative one-word improvement.
- Prefer known whole extents, credible donors, authenticated layouts and disjoint edit scope.
- Favor the 256–4,095-byte game population for sustained accepted-byte progress after admission.
- Keep a small-function lane when its targets are fresh, naturally reconstructable, or unlock a real closure. Do not discard small foundational helpers merely because they are small.
- Treat very large targets as multi-region semantic projects before compiler projects.
- Penalize unknown external ABI, unbound receipts, false historical names, unresolved tables and repeated frozen hypotheses.
- Stop at a useful no-go result when a candidate fails admission. Do not fabricate ready packets to satisfy a queue quota.

Keep at least two admitted next packets per active matching executor when the evidence supports them. Until that exists, allocate more effort to reconstruction and scouting. The backlog can contain many useful research packets while the ready-match queue legitimately remains empty.

## 5. Shared ownership and handoff protocol

Use existing conventions: isolated branch/worktree, `cloud/work/<packet>/` for research, `cloud/matches/<target>.c` or genuine `cloud/work/ipa-groups/<group>/` for matching submissions, and one integration owner. The following procedure makes those conventions usable across independent agents.

### Claim before editing

1. Fetch `origin/master`; record exact SHA and inspect `git status`. Do not clean/reset another person's dirty worktree or submodule.
2. Read `dot_response`, live locks, target aliases/intervals, current PRs and latest `dot_handoff` claims. Treat old `build/overnight_reservations.json` as historical until reconfirmed.
3. Send/record a claim in the existing shared queue with packet ID, owner, branch, full target intervals, edited context members and files. Read-only accepted helpers need no new coverage claim; modifying shared headers or generated contracts needs an explicit shared-file owner.
4. Have the coordinator acknowledge overlap resolution before two contributors touch the same family. Different target names do not guarantee disjoint native bytes.
5. Claim one active packet and at most one next packet. Unclaimed cards remain proposals. If a contributor becomes unavailable, record a handoff, exact source state and evidence before reassigning.

```text
Packet ID / state / named owner:
Branch / base SHA / PR:
Population / native target IDs and complete address intervals:
Edited context members / shared files / read-only accepted context:
Prior archive packet and source-recipe receipt:
Question or predicted compiler effect:
Entry criteria satisfied / missing inputs:
Deliverable paths / required tests / evidence scope:
Next checkpoint / next packet / stop condition:
Reviewer / integration owner:
```

Do not claim the entire game tree for a narrow function. Cross-cutting infrastructure owners coordinate exact files with source workers; merge small independently reviewed changes first and rebase consumers.

### Required handoff artifact

Each finished or paused packet leaves a short README/STATUS plus machine-readable evidence when useful. Include:

- Base and source SHA; target/revision identity; target full length; alias list; actual semantic purpose and confidence.
- Complete source paths or explicit region boundaries/missing code, donor provenance/license and native layout/ABI facts.
- Exact compiler/toolkit flags and hashes, include order, defined/kept roots, source order and whether each body was visible to the optimizer.
- Candidate length, exact differing native words, excess words, unresolved relocations/table proofs and strict versus masked status.
- Tests executed with commands/results; assumptions and exclusions; exact object/source binding; whether results were replayed now or inspected historically.
- Prior experiments rejected, what genuinely changed, predicted effect, observed result, and one bounded next action.
- Any external request reduced to a typed contract/provenance requirement. Do not upload ROMs, raw dumps or private credentials.

Research source belongs in the research lane, explicitly NONMATCH/PARTIAL as appropriate. Do not put a deliberately failing research function in a matching route merely to get it compiled by CI.

## 6. Admission, review and acceptance gates

### Before a complete reconstruction is assigned

- Exact unpaused native interval, full authenticated instruction extent and ownership checks.
- Archive search by name, address, callers and equivalent source; no freshness claim from a grep miss alone.
- All known callee and field contracts enumerated; unknown runtime services and constants remain explicit.
- A credible full-body route with meaningful intermediate region tests; no guessed helper stub presented as source.
- A proposed natural C89 data/layout model; callback mutation, signedness, wrap behavior and returning paths considered.

### Before a matching experiment is assigned

- Whole genuine candidate, current full extent and native target identity.
- Source-to-recipe-to-receipt binding and compiler visibility/roots documented.
- Native ABI grounded by read-before-definition/caller evidence; no invented unused formals, fake keepers, artificial locals or manual pressure devices. Genuinely unused incoming arguments require native homing/caller evidence.
- Tables/pools/relocations accounted for at the level needed by the claimed bodies.
- Workbench/native diagnosis yields one evidence-derived, previously untried hypothesis and an expected observable effect.
- Compare against the best comparable archived baseline; changing optimizer recipe/visibility can invalidate raw score comparisons.

Bound a hypothesis using the existing handoff rule: one working session or roughly 20 directed variants, whichever comes first; extend only when new native evidence or measured movement justifies it. Some packets below are intentionally stricter: one predicted context experiment. A new contributor does not reset exhausted controls.

### Before source is accepted into the cartridge

1. Independent reviewer checks actual full source, ABI/semantics and exact claim set.
2. Freshly compile the intended standalone/group source with source/toolchain/recipe hashes.
3. Resolve and compare complete native bodies, including stack operands, target lengths, excess words, data/table relocations and every claimed member. A group score does not imply every member matches.
4. Reprove all pre-existing affected accepted neighbors, shared-TU flags and data/storage ownership. No protected target/scorer edits to manufacture equality.
5. Enforce expected locked-C body membership and fresh source/object provenance. Missing compiled objects must fail rather than produce assembly/passthrough replacements for accepted source.
6. Game path: complete linked image equality, exact original compressed stream using documented zlib parameters, then full-ROM SHA-1. Static path: real TU promotion, current headers/flags/storage, then complete cartridge gates with current game blob.
7. Verify remote commit and exact-head CI. Record which tests/gates were run and which were unavailable. No full-ROM claim from a cloud-only test run.
8. Refresh derived coverage from actual accepted state, keeping image/function/opaque metrics and research progress distinct.

The integration owner serializes production splices. Match/research contributors do not update locks, protected native targets or accepted sources as part of ordinary submissions. This planning commit changes none of those assets.

## 7. Staged execution schedule

These are outcome-based stages, not calendar promises. Work continues through blocked lanes by selecting independent ready research/reconstruction cards.

### Stage A: establish a reliable shared baseline

Run P01–P03 and V01/V02 in parallel with already justified R01/R02 source work. R01's independently reviewed semantic fragment is now [draft PR #51](https://github.com/cabi24/SFRush2049-decomp/pull/51), with exact-head CI passing; full state source and matching context remain open. Publish current PR/claim state and explicitly list no-go/frozen packets. Exit when the baseline is reproducible, claimed work is disjoint, existing accepted-body evidence is preserved, and high-priority verification fixes have independent tests/review.

### Stage B: expand beyond the exhausted shortlist

Complete P04/P05 and distribute S01–S13 by native address partitions. Finish archive-best/provenance records for each assigned partition. Continue service requests and state/renderer/HUD prerequisites without waiting for every response. Exit each family with either a complete admission packet or a precise no-go/reopen reason. The whole stage need not finish before one sound candidate advances.

### Stage C: produce complete natural source

Execute admitted I01/R04/R07 cards. Region tests are checkpoints, not substitutes for whole bodies. Pair a source author with an ABI/semantic reviewer on nontrivial functions. Stop tuning when a structural/source/ABI problem is found. Exit per function at complete-source identity and a reproducible first compiler baseline.

### Stage D: match and integrate continuously

I02 tests one diagnosed hypothesis at a time. I03/I04 accept disjoint verified candidates promptly and amortize expensive full gates across independently proven batches. I05 reports actual byte gains and separate research outputs. Keep scouts supplying the next admitted packets rather than letting all contributors wait for one private ROM gate.

### Stage E: attack large closures and the long tail

M01/M02 select very large functions only when source/contract/data risks have fallen enough to justify focused teams. Preserve a steady medium lane. Include static residuals, runtime dependencies, generated-context debt and compiler/toolchain reproducibility. Exit based on diminishing unmatched intervals and validated source ownership, not a guessed completion percentage.

### Stage F: prove the final US artifact

M03–M05 establish complete source-owned image/static coverage, deterministic clean rebuilds, preserved data ownership, full-ROM identity, regression/behavior smoke and durable release instructions. M06 explicitly resolves version-scope policy before any EU/JP promise. Finishing a plan, archive or source tree is not finishing this stage.

## 8. Risk register and recovery

| Risk | Detection | Response / owner |
|---|---|---|
| Silent assembly fallback masks missing accepted C | Expected locks versus freshly built source bodies; negative missing-object tests | V01/V08; fail before composition and fix inputs, never quietly reduce claimed coverage. |
| Wrong function name or suffix extent | Address/interval census, native call/field evidence | P04/P05; preserve aliases, correct meaning with confidence labels, regenerate authoritative context through tools. |
| Repeating a frozen hypothesis | P03 source/recipe matrix and packet rejection log | Require new causal evidence; move contributor to another card. |
| Unknown runtime/overlay service | Read-before-definition and mapped module contract | R03/R06/R15; request minimum text contract with provenance, continue independent known regions. |
| False compiler context | Genuine source visibility, roots and caller evidence absent | R05/R09/V06; reject fake context; retain semantic source separately. |
| Source and receipt drift | Hash/recipe/native identity mismatch | V03/P03; mark historical/unbound, never overwrite old receipt with new claims. |
| Concurrent source or protected metadata changes | Claims, fresh master diff, shared-file ownership | P02; isolate, rebase, rerun affected checks; serialize integration. |
| Public research claims outrun private gates | Acceptance receipt missing image/compression/ROM stage | I04/I05; describe verified level only. |
| Builder/coordinator unavailable | Fresh connectivity and service checks | V09/V10; continue repo-only work; hand private integration to authorized maintainer. Do not invent access. |
| Raw native/ROM/private-data publication | Diff/manifest review before submission | V11; publish source/proof summaries and hashes, retain lawful private inputs locally. |
| Documentation staleness | Versioned current-status pointers and link checks | P06/I05; archive dated claims and keep one current entry point. |

### Rebase, rollback and publication safety

- Fetch latest master before preparing a final commit. Reapply only the intended packet diff, preserve concurrent edits, and rerun affected validation after conflict resolution.
- Review `git diff --name-status` and full diff against the fresh base. A planning change must remain documentation-only; do not sweep in temporary compiler output, untracked research or someone else's dirty submodule.
- Publish ordinary implementation/research through separate draft PRs unless specifically authorized otherwise. This plan's direct-master publication authority is limited to the plan and navigation edits; it does not authorize merging PR #50 or any source work.
- Never force-update master. If the head advances, reconstruct the commit on the new head and repeat review/validation for changed content.
- A bad documentation commit is repaired/reverted with a normal follow-up commit preserving unrelated work. A failed candidate integration uses the promotion workflow's rollback/refusal path and retains failed evidence; it must not be hidden by target edits or fabricated locks.
- Verify the exact remote tree/commit and exact-head CI after publication. If a check fails, distinguish a plan-induced regression from a pre-existing/environment failure and report the actual result.

## 9. Copyable agent assignments

### Start an independent research/reconstruction packet

> Read CLAUDE.md, dot_response, PROJECT_PLAN.md and the assigned work-package card. Fetch current master and inspect existing claims, locks, aliases and archive evidence before editing. Claim only the exact native intervals/context files required for [PACKET ID]. Work in an isolated branch. Deliver the card's bounded question and full source/evidence artifact; label partial source, NONMATCH, historical receipts and unresolved contracts explicitly. Use real native ABI and genuine source context; do not fabricate pressure variables, keeper callers or formals. Do not modify accepted/protected targets or the paused 800D1248 investigation. Report useful findings early, then leave reproducible commands, hashes, tests, rejected hypotheses, a stop/reopen condition and one next action. No accepted-byte claim until independent production gates pass.

### Review a packet independently

> Review [PACKET ID / exact SHA] against its pinned base and source/recipe/native identity. Check entire claimed body and extents, callers/callees, type/side-effect assumptions, callback mutation, data/table relocations, excess words and evidence scope. Reproduce the stated focused tests; distinguish fresh replay from historical inspection. Check for previous equal/better source and exhausted controls. Return actionable defects with paths/lines and either READY-RECONSTRUCTION, READY-MATCH, VERIFIED-BODY, RESEARCH-ONLY or BLOCKED, with exact rationale. Do not advance coverage or rewrite protected evidence.

### Integrate an independently verified match

> Use the current promotion skill and the explicitly authorized integration scope. Recompile complete real source with the proven recipe; enforce exact source/object/lock membership, resolve full bodies and preserve every affected accepted neighbor. Run full source-image, original compressed-stream and ROM-hash gates as applicable, with no fallback for missing accepted C. Record exact revision/source/objects/toolchain/gates. Update coverage only after success. Preserve unrelated dirty state and shared builder work; publish only authorized paths and verify exact-head CI.

## 10. Definition of success for this plan

The plan succeeds when contributors can choose useful disjoint work, know what evidence would finish it, and hand it to another agent without relying on a conversation. Program progress is measured in several separate outputs:

- Additional accepted native functions/bytes with complete production receipts.
- Complete genuine source bodies and independently reviewed partial regions with named remaining gaps.
- Closed ABI/data/TU/extent contracts that unlock specific callers or families.
- Fewer stale/unbound receipts and duplicated/exhausted experiments.
- Stronger automated safeguards, deterministic tooling and tests that actually run.
- A replenished, honest execution queue and a shrinking whole-population unknown inventory.

The owner's long-term objective remains a byte-identical, source-built US ROM. This plan provides the work needed to move toward it without confusing research activity with accepted matching coverage.
