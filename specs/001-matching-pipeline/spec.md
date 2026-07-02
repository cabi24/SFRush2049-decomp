# Feature Specification: Deterministic Function-Matching Pipeline

**Feature Branch**: `001-matching-pipeline`
**Created**: 2026-07-02
**Status**: Draft
**Input**: User description: "Deterministic function-matching pipeline for the Rush 2049 N64 decompilation (Phase 4 reboot). Build a suite of traditional-compute tools that convert the matching phase from manual guesswork into a mostly-automated conveyor: (1) an arcade match matrix tool that extracts every function from the Rush The Rock arcade source, compiles each with IDO at each known flag set, and scores every arcade-candidate-to-N64-target pair to produce a ranked assignment matrix for the 752 extracted game functions; (2) a per-file compiler flag sweeper that pins per-file optimization levels; (3) a duplicate/near-clone clustering tool so one matched function unlocks its siblings; (4) a permuter farm harness that runs jobs in parallel on the watchman machine with automatic promotion of matched source; (5) progress/reporting integration so match status is tracked per function. Human attention is reserved for struct/global data-layout inference and functions with no arcade ancestry."

## Overview

The decompilation project has completed extraction and initial C authoring; the remaining work is **matching** — producing C source that compiles to byte-identical assembly for each of the 752 game functions extracted from the ROM (plus the 228 statically identified functions). Today, matching is manual: a person guesses which arcade-source function corresponds to an N64 function, adapts it, compiles, eyeballs the diff, and iterates. This feature replaces that guesswork with an automated conveyor: **deterministic seed generation** (pairing N64 functions with their arcade-source ancestors and pinning compiler settings) feeding **automated finishing** (a parallel search farm that mutates near-matching code until it matches exactly). Human effort is reserved for the two things automation cannot do well: data-layout inference and functions with no arcade ancestry.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Arcade Match Matrix (Priority: P1)

As the project maintainer, I want every N64 target function automatically scored against every candidate function from the arcade source, so that instead of guessing "which arcade function is this?", I receive a ranked list of likely ancestors for each target and can start every matching effort from real source code.

**Why this priority**: Candidate pairing is the single slowest manual step and gates everything downstream. A ranked match matrix converts weeks of human guesswork into an unattended batch job, and every other tool in this feature consumes its output.

**Independent Test**: Run the matrix build against the existing 752 extracted targets and the arcade source tree. Verify that functions already matched by hand (e.g., the main game loop, `dll_remove`, `dll_init`) appear as the top-ranked candidate for their known N64 counterparts.

**Acceptance Scenarios**:

1. **Given** the arcade source tree and the extracted N64 target assembly, **When** the matrix build runs to completion, **Then** every N64 target has a ranked candidate list with similarity scores, persisted to a queryable results file.
2. **Given** a previously hand-confirmed pairing (e.g., game loop ↔ arcade `game()`), **When** the matrix is consulted for that target, **Then** the confirmed arcade function appears in the top candidates.
3. **Given** an arcade function that fails to compile in isolation (missing headers, arcade-specific constructs), **When** the matrix build runs, **Then** the failure is recorded and reported, the function is skipped, and the build continues without manual intervention.
4. **Given** a completed matrix, **When** the build is re-run with unchanged inputs, **Then** the scores and rankings are identical (deterministic output).

---

### User Story 2 - Permuter Farm (Priority: P2)

As the project maintainer, I want to queue every near-matching function as a search job on the build machine's 20 cores, prioritized by how close each is to matching, so that the "last mile" of matching (register allocation, statement ordering, minor structural tweaks) is ground out by compute overnight instead of by hand.

**Why this priority**: This is the automated finishing step that turns good seeds into confirmed matches. It is second only to the matrix because it needs seeds to operate on, and the matrix provides them.

**Independent Test**: Queue a small set of known near-matches (e.g., functions currently within a few instructions of matching). Verify jobs run in parallel, results are harvested, and at least one function reaches a perfect score and is promoted.

**Acceptance Scenarios**:

1. **Given** a set of seed functions with nonzero scores, **When** the farm runs, **Then** jobs execute in parallel up to the configured core limit, ordered by priority (closest to matching first).
2. **Given** a job that reaches a perfect score, **When** harvesting runs, **Then** the winning source is captured, the full build is verified, and the function is promoted into the codebase with its match status updated.
3. **Given** a job that stalls (no score improvement after a configured budget), **When** the budget is exhausted, **Then** the job stops, the best-so-far result is recorded, and the function is flagged for human attention with its lowest achieved score.
4. **Given** the farm is interrupted (machine reboot, network loss), **When** it is restarted, **Then** completed work is not lost and the queue resumes from recorded state.

---

### User Story 3 - Compiler Flag Sweeper (Priority: P3)

As the project maintainer, I want each translation unit's compiler settings determined automatically by compiling it under every plausible flag combination and scoring the results, so that per-file optimization levels are pinned once, deterministically, instead of being discovered one painful surprise at a time.

**Why this priority**: The ROM was built with mixed optimization levels per file. Wrong flags make every downstream search run futile. Pinning flags shrinks the search space for the entire conveyor, but it can be run after the matrix exists because it operates on the same scoring infrastructure.

**Independent Test**: Run the sweeper against files with already-confirmed settings (string functions at one level, OS functions at another) and verify it reports the known-correct flags as best-scoring.

**Acceptance Scenarios**:

1. **Given** a translation unit with unknown build settings, **When** the sweeper runs, **Then** it reports a ranked list of flag combinations by aggregate match score and records the winner in a per-file settings registry.
2. **Given** a file whose flags were previously confirmed by hand, **When** the sweeper runs on it, **Then** the confirmed flags are the top result.
3. **Given** a pinned flag registry, **When** any other pipeline stage compiles a file, **Then** it uses the pinned flags automatically.

---

### User Story 4 - Near-Clone Clustering (Priority: P4)

As the project maintainer, I want the N64 target functions scored against each other and grouped into similarity clusters, so that when one function in a cluster is matched, its near-clones are immediately seeded from the matched source and become quick wins.

**Why this priority**: A force multiplier rather than a gate: it doesn't unlock the pipeline, but it amortizes every success across sibling functions.

**Independent Test**: Run clustering over the 752 targets; verify known families (e.g., repeated small handlers, per-car or per-viewport variants) land in shared clusters, and that marking one member matched surfaces its siblings as high-priority seeds.

**Acceptance Scenarios**:

1. **Given** the extracted target assembly, **When** clustering runs, **Then** each target is assigned to a cluster (possibly a singleton) with recorded intra-cluster similarity.
2. **Given** a cluster where one member becomes matched, **When** the queue is next prioritized, **Then** the remaining members are automatically seeded from the matched member's source and promoted in queue priority.

---

### User Story 5 - Match Status Tracking (Priority: P5)

As the project maintainer, I want a single per-function status record covering all 980 functions (752 extracted + 228 static) — unmatched, candidate-identified, seeded, in-search, matched, verified — with progress roll-ups, so that at any moment I can see pipeline health, where human attention is needed, and honest overall progress.

**Why this priority**: Reporting rides on top of the other stages; it delivers visibility, not matches. But without it the conveyor cannot be steered.

**Independent Test**: After running any earlier stage, query status and verify counts reconcile with that stage's outputs (e.g., number of matched functions equals number of promoted search wins plus hand-matched functions).

**Acceptance Scenarios**:

1. **Given** any pipeline stage completes work on a function, **When** status is queried, **Then** the function's stage, best score, candidate source, and last-updated time are reported.
2. **Given** the full function inventory, **When** a progress report is generated, **Then** it shows counts per status, percentage matched, and a ranked "needs human attention" list.

---

### Edge Cases

- **Arcade function has no N64 counterpart** (arcade-only features, 3dfx rendering): appears as a candidate but never as a top match; must not block or distort rankings.
- **N64 function has no arcade ancestor** (N64-specific code): all candidate scores are poor; the function must be clearly flagged as "no ancestry — human required" rather than silently assigned a bad candidate.
- **Two N64 targets claim the same arcade candidate**: legitimate (inlining, duplication) — both pairings are kept; assignment is not exclusive.
- **Arcade source is C89-hostile or references arcade-only headers**: candidate compilation must degrade gracefully (skip and log), and the coverage report must state what fraction of arcade functions compiled.
- **Scoring ties**: rankings must be stable across runs (deterministic tie-breaking).
- **Remote build machine unreachable**: orchestration reports the outage clearly and queues work rather than failing silently; no partial or corrupt results are recorded.
- **A promoted "match" breaks the full build**: promotion must be gated on full-build verification; a function that matches in isolation but breaks the ROM build is rolled back and flagged.
- **Search never converges for a structurally wrong seed**: budgets prevent infinite spend; the function returns to the human queue with diagnostics rather than consuming the farm forever.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST extract an inventory of candidate functions from the arcade source tree, recording for each its name, source file, and body.
- **FR-002**: The system MUST compile each arcade candidate in isolation under each known-good compiler configuration, recording success or the reason for failure, and MUST report overall candidate-compilation coverage.
- **FR-003**: The system MUST score every (arcade candidate × N64 target) pair using the established assembly-similarity scoring method and persist the full matrix in a queryable form.
- **FR-004**: The system MUST produce, for each N64 target, a ranked candidate list with scores, and MUST flag targets whose best score falls below a configurable "no ancestry" threshold.
- **FR-005**: Matrix generation MUST be deterministic: identical inputs produce identical scores and rankings, with stable tie-breaking.
- **FR-006**: Matrix generation MUST be incremental: unchanged candidates and targets are not re-compiled or re-scored on subsequent runs.
- **FR-007**: The system MUST sweep each unmatched translation unit across the set of plausible compiler configurations, rank them by aggregate score, and record the winning configuration in a per-file settings registry consumed by all other stages.
- **FR-008**: The system MUST cluster N64 targets by mutual assembly similarity and record cluster membership; when a cluster member is matched, the system MUST seed its siblings from the matched source and raise their queue priority.
- **FR-009**: The system MUST run search jobs in parallel up to a configured core budget on the remote build machine, prioritized by proximity to matching, with per-job iteration/time budgets.
- **FR-010**: The system MUST harvest search results: capture best-so-far source and score for every job, and for perfect scores, promote the source into the codebase only after full-build verification passes; failed verification MUST roll back and flag the function.
- **FR-011**: The system MUST maintain a durable per-function status record (unmatched → candidate-identified → seeded → in-search → matched → verified) covering all 980 functions, updated by every stage.
- **FR-012**: The system MUST generate a progress report showing per-status counts, match percentage, recent activity, and a ranked human-attention queue (no-ancestry functions and stalled searches).
- **FR-013**: Compile, score, and search work MUST be packaged as fully self-contained work units (source, headers, target, build/scoring configuration, budget) executable on any compatible compute node with no access to the project tree; nodes pull work from the orchestrator and return results, and MUST be safely ephemeral — a node that disappears mid-job causes the job to be re-issued, never lost or double-counted.
- **FR-014**: The maintainer MUST be able to add or remove compute nodes without reconfiguring the pipeline; the orchestrator maintains the node roster and re-leases expired work automatically. Full-build verification and promotion remain centralized on a single designated builder holding canonical project state.
- **FR-015**: The maintainer MUST be able to manually override any automated pairing or pinned flag setting, and overrides MUST be preserved across re-runs.
- **FR-016**: All pipeline stages MUST run unattended end-to-end with no model or human intervention in the steady-state loop; human/model touchpoints are limited to seed repair for non-compiling candidates, authoring seeds for no-ancestry functions, and data-layout inference.

### Key Entities

- **N64 Target**: A function extracted from the ROM — address, size, target assembly, current match status, best score, cluster membership.
- **Arcade Candidate**: A function from the arcade source — name, origin file, compilation status per configuration, compiled output per configuration.
- **Match Matrix Entry**: A scored (candidate, target, configuration) triple; the collection yields per-target rankings.
- **Flag Registry Entry**: A translation unit paired with its pinned compiler configuration and the evidence (scores) behind the pin.
- **Cluster**: A group of mutually similar N64 targets with intra-cluster similarity scores.
- **Search Job**: A queued unit of search work — seed source, target, priority, budget, best-so-far score, state (queued/running/stalled/won).
- **Function Status Record**: The authoritative per-function pipeline state consumed by reporting.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: At least 50% of the 752 extracted game functions receive a high-confidence arcade candidate (top candidate clearly separated from the field) without any human pairing work.
- **SC-002**: The full match matrix builds unattended in under 12 hours on the existing build hardware, and incremental re-runs complete in under 1 hour.
- **SC-003**: Every previously hand-confirmed pairing is reproduced by the matrix (its known arcade ancestor ranks in the top candidates) — zero regressions against established knowledge.
- **SC-004**: At least 25 functions progress from "candidate identified" to "verified matched" via the conveyor with no human editing of the function body (seed → search → promote → verify, end to end).
- **SC-005**: Every file with hand-confirmed compiler settings has those settings reproduced by the flag sweeper as its top result.
- **SC-006**: A single command answers "what is the pipeline state?" in under 10 seconds, including the human-attention queue.
- **SC-007**: Promoted matches never regress the build: ROM verification continues to pass after every promotion (100% of promotions gated).
- **SC-008**: Human matching effort shifts measurably: after the pipeline is live, the human-attention queue consists only of no-ancestry functions, stalled searches, and data-layout work — no manual candidate hunting.

## Assumptions

- The known-good compiler configurations are the ones already documented from confirmed matches (two optimization levels across the established flag baseline); the sweeper explores plausible variations of these rather than an unbounded flag space.
- The existing assembly-similarity scoring method used by the search tooling is accepted as the canonical metric for the matrix, sweeper, and clustering — no new scoring research is in scope.
- "Matched" means the function's compiled output is byte-identical to the target assembly; "verified" additionally means the full ROM build passes hash verification with the function integrated.
- Both function populations are in scope (752 extracted + 228 static), with the 752 extracted game functions as the priority; the pipeline treats them uniformly.
- Promotion into the codebase is automatic when full-build verification passes; no human review gate is required for byte-identical, build-verified matches. Manual overrides remain available.
- The original-compiler toolchain runs on any x86-64 Linux node (it cannot run on the orchestration machine); the toolchain is distributed to nodes as a cached, content-addressed bundle rather than pre-installed. The existing build machine serves as the designated verification builder; additional compute nodes may join and leave the work pool freely.
- The arcade source is treated as read-only reference material; candidate compilation adapts copies, never the reference tree.

## Out of Scope

- Struct/global data-layout inference tooling (explicitly reserved for human analysis, aided by arcade headers).
- New decompiler or scoring research; the pipeline composes existing proven components.
- Matching for non-code ROM content (assets, audio data, textures).
- Public/internet-scale distribution (untrusted volunteers, cloud autoscaling) — the pool is trusted LAN nodes owned by the maintainer; the encapsulated-work-unit design should not preclude wider distribution later, but it is not built now.
