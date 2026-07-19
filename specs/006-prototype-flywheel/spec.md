# Feature Specification: Track B Prototype Layer & Seed Flywheel

**Feature Branch**: `006-prototype-flywheel`
**Created**: 2026-07-19
**Status**: Draft
**Input**: User description: "Track B prototype layer and seed flywheel (006): grow the extracted population's compiled-seed count from 42 toward a majority by auto-generating the shared declaration layer the 005 histogram identified as the top blocker class — function prototypes for named callees (derivable deterministically from the population's own derivation outputs) plus the mechanical decompiler-artifact fixes (M2C_ERROR, saved-register locals) in seed hygiene. Close the flywheel: every seed that compiles gets submitted and scored through the existing pool, unattended. The histogram remains the measurement instrument (deterministic, run-over-run diffable). Out of scope: blob rebuild, struct-shape inference beyond what named blockers require, hand-typing beyond the game-loop cluster precedent, and any promotion path for extracted evidence."

## Overview

Feature 005 proved the Track B pipe on one cluster and built the instrument
that prices the rest: of 688 real extracted functions, 42 seeds compile, 597
are blocked, and 49 fail in the decompiler. The 2026-07-19 histogram
decomposes the 1,073 distinct blockers into exactly the classes 005's
close-out predicted:

- **Missing callee prototypes** — the dominant class. 289 blockers are
  bare `func_<addr>` references, and the largest named blockers are the
  same thing under better names (`math_utility` blocks 26 functions,
  `audio_reverb_update` 21, `model_data_load` 20, `memcpy` 21). Every one
  of these callees is itself a member of the population (or a known static
  target), so its signature is *derivable from the population's own
  decompiler output* — no human judgment required. 005 demonstrated the
  effect at small scale: ~75 hand-added prototypes moved 14 unrelated
  functions to compiled.
- **Decompiler-artifact identifiers** — mechanical emission warts, not
  knowledge gaps: `M2C_ERROR` placeholders (blocks 101), `saved_reg_s0/s1/s2`
  register spills surfacing as undeclared identifiers (~69), and inferred
  `unk0/unk4/unk8` member chains on locals (~74) — the generalization of the
  byte-cursor class 005 already fixed once.
- **Numeric-global residue** (`x80150000`-style, ~19) — the same class
  005's symbol table closes, appearing beyond the surveyed cluster.

This feature turns those findings into an unattended flywheel: a
deterministically **generated declaration layer** (never hand-edited,
regenerated from evidence), **seed-hygiene rules** for the artifact classes
(honest ones — placeholders that mark genuine decompiler failure must not be
laundered into "compiled"), and **automatic submission** of every compiling
seed to the scoring pool so the idle compute stays busy and Track B evidence
accumulates without a human in the loop. The histogram remains the sole
measurement instrument, gains the honesty distinction the artifact classes
force, and stops being clobberable by scoped runs.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Generated prototype layer unlocks the majority blocker class (Priority: P1)

The operator runs the declaration-layer generator. It derives, for every
extracted function and every static target referenced by extracted code, a
best-known signature from the population's own decompiler outputs (falling
back to a safe default signature where inference is silent), and emits a
single generated declaration artifact that the seed context consumes.
Re-running the histogram shows the named-callee and `func_<addr>` blocker
classes collapse, and the compiled bucket grows accordingly. The generator
is deterministic (same inputs ⇒ identical artifact) and regenerable at any
time; it never requires hand edits, and hand-curated context
(`game_types.h`) keeps precedence over generated declarations.

**Why this priority**: It attacks the histogram's top class — the single
highest-leverage context investment the 005 instrument identified — and it
is pure deterministic compute, the project's preferred shape of work.

**Independent Test**: Run the generator twice (byte-identical output), run
the histogram before/after: `func_`-shaped blockers drop from 289 to near
zero; compiled grows from 42 substantially; a spot-checked seed that
previously blocked on `math_utility` now compiles.

**Acceptance Scenarios**:

1. **Given** the current population evidence, **When** the generator runs
   twice with no input change, **Then** the generated artifact is
   byte-identical across runs.
2. **Given** the generated layer is active, **When** the full histogram
   runs, **Then** no `func_<addr>`-shaped blocker remains for any callee
   that is a known target, and the compiled bucket has grown by at least
   100 functions over the 42 baseline.
3. **Given** a symbol defined in the hand-curated context, **When** the
   generator would also emit it, **Then** the hand-curated definition wins
   and the generated artifact omits or defers to it (no redefinition
   errors anywhere in the population).
4. **Given** the static population, **When** the generated layer is active,
   **Then** static seed function bodies are byte-identical to before
   (005's SC-005 guard, unchanged).

---

### User Story 2 - Honest seed hygiene for decompiler-artifact classes (Priority: P2)

The operator enables the artifact-class hygiene rules. Saved-register
identifiers (`saved_reg_*`) and inferred member chains on locals
(`unk0/unk4/unk8` — the generalized byte-cursor class) are mechanically
rewritten into compilable, semantics-preserving forms, since they are
emission warts with a known meaning. `M2C_ERROR` placeholders are **not**
compiled away: a seed containing one is a partial decompilation, and the
histogram classifies it in its own bucket (`partial_decomp`) distinct from
both `compiled` and `blocked`, so the instrument's `compiled` count keeps
meaning "a full-function seed compiles."

**Why this priority**: ~140 functions are blocked purely by these
artifacts, but the value depends on doing it honestly — laundering error
placeholders into compiled seeds would corrupt the instrument that steers
the whole track.

**Independent Test**: Histogram before/after: `saved_reg_*` and local-`unkN`
blockers disappear; affected functions move to `compiled` only if nothing
else blocks them; every function whose seed contains an error placeholder
appears in `partial_decomp`, and bucket counts still sum to the population.

**Acceptance Scenarios**:

1. **Given** a seed blocked only by `saved_reg_*` identifiers, **When**
   hygiene is active, **Then** it compiles and its rewritten form is
   semantics-preserving (declarations added, not code deleted).
2. **Given** a seed whose decompilation contains an error placeholder,
   **When** the histogram runs, **Then** that function is bucketed
   `partial_decomp` — never `compiled` — regardless of whether the text
   could be made to compile.
3. **Given** the new bucket, **When** two consecutive histogram runs
   execute with no input change, **Then** bucket counts are identical and
   sum to the population (the 005 determinism/coverage guarantees extend
   to the new bucket).

---

### User Story 3 - The unattended flywheel: compiling seeds get scored (Priority: P3)

The operator starts the flywheel in the existing steady-state daemon. Every
extracted function whose seed compiles and has no recorded score evidence
is submitted to the scoring pool automatically, budgeted so the pool stays
busy without starving Track A work; results harvest on the existing path.
The operator can walk away: new compiling seeds (e.g. after a context
improvement) are picked up on the next cycle, and a status view shows
scored/unscored coverage of the compiled set.

**Why this priority**: It converts context wins into evidence continuously
and keeps the spare compute earning — but it depends on P1/P2 producing a
compiled set worth scoring.

**Independent Test**: With the daemon running and ≥100 compiling seeds,
observe over one unattended cycle that unscored compiling seeds get
submitted, scores/artifacts appear in the evidence store attributable per
target, and no extracted evidence enters the promotion path.

**Acceptance Scenarios**:

1. **Given** a compiling seed with no score evidence, **When** the flywheel
   cycle runs, **Then** a search is submitted for it without operator
   action, and its score and best-source artifact are recorded on harvest.
2. **Given** limited pool capacity, **When** both static and extracted work
   is queued, **Then** extracted searches do not starve static work
   (priority ordering favors Track A).
3. **Given** the 005 firewall, **When** flywheel results include any
   score-0 match, **Then** it is recorded as evidence only — no lock
   migration or promotion occurs for extracted targets.

---

### Edge Cases

- **Signature inference disagreement**: two call sites imply different
  signatures for the same callee. The generator must resolve
  deterministically (a documented precedence rule), and a wrong-but-
  compilable prototype is acceptable — the score, not the prototype, is
  the arbiter of truth.
- **Callee is a known static target** (e.g. `memcpy`, `osRecvMesg`): the
  layer must use the real known signature, not an inferred one, and must
  not conflict with existing OS/libc context headers.
- **Redefinition hazards**: the generated layer meets hand-curated context
  (`game_types.h`, `m2c_types.h`) and per-seed m2c extern guesses; the
  precedence chain must produce zero redefinition errors across all 688
  derivable functions, not just the ones it unlocks.
- **`extent_conflict` and `no_disasm` targets**: remain excluded from
  seeding and from prototype-inference inputs (their derivations are not
  trustworthy), but their names may still be *referenced* by other
  functions' seeds — references resolve via address, not trust.
- **Artifact-rule overreach**: a hygiene rewrite must never fire on
  legitimate identifiers (e.g. a real global that happens to match
  `unk\d+` naming); rules key on the decompiler's emission context, with
  regression tests per rule.
- **Flywheel churn**: a context change invalidates the derived-asm cache
  and can re-flip functions between buckets; the flywheel must not
  resubmit already-scored targets just because their seed text changed,
  unless the operator asks for re-scoring (evidence is append-only, cache
  keys prevent duplicate work).
- **Scoped runs vs the instrument**: 005's scoped cluster probes overwrite
  the population histogram artifact. The population instrument must be
  protected (scoped runs write elsewhere) so the measurement record is
  never clobbered by a probe.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST generate a declaration layer covering every
  known target callee referenced by extracted code — named and
  `func_<addr>`-shaped — with signatures derived deterministically from
  the population's own decompiler outputs, falling back to a documented
  default signature when inference is unavailable.
- **FR-002**: The generated layer MUST be regenerable and byte-stable
  (identical inputs ⇒ identical bytes), MUST never require hand edits, and
  MUST carry a generation stamp identifying its evidence inputs.
- **FR-003**: Precedence MUST be: hand-curated context > known
  static/library signatures > generated inference > default fallback; the
  combined context MUST produce zero redefinition errors across the
  derivable population.
- **FR-004**: Seed hygiene MUST mechanically resolve the saved-register
  and local-inferred-member artifact classes in a semantics-preserving
  way, each rule covered by regression tests including a no-overreach
  case.
- **FR-005**: Seeds containing decompiler error placeholders MUST be
  classified `partial_decomp` — a new histogram bucket — and MUST NOT be
  counted `compiled`; the histogram's coverage, exclusivity, and
  determinism guarantees (005 SC-003) MUST extend to the new bucket.
- **FR-006**: The population histogram artifact MUST NOT be overwritten by
  scoped runs; scoped probes write to a separate location.
- **FR-007**: The flywheel MUST, without operator action, submit a scoring
  search for every extracted function whose seed compiles and lacks score
  evidence, harvest results on the existing path, and expose
  scored/unscored coverage of the compiled set in the standard reports.
- **FR-008**: Flywheel submissions MUST NOT starve static-population work
  (Track A retains queue priority) and MUST NOT duplicate work for
  already-scored targets absent an explicit re-score request.
- **FR-009**: All 005 invariants carry forward unchanged: extracted
  evidence never enters lock/promotion (005 FR-010), static seed bodies
  stay byte-identical (005 SC-005), and generation/derivation/histogram
  runs remain orchestrator-local with no node dependency.
- **FR-010**: A histogram comparison view MUST show run-over-run bucket
  movement (which functions moved, which blocker classes shrank) so
  context investments are attributable to their effects.

### Key Entities

- **Declaration layer**: the generated artifact of callee
  prototypes/extern declarations for the extracted population; regenerated
  from evidence, never hand-edited; subordinate to hand-curated context.
- **Signature inference**: the per-callee best-known signature extracted
  from the population's own decompiler outputs, with a deterministic
  conflict-resolution rule and a default fallback.
- **Artifact-class hygiene rule**: a mechanical, regression-tested rewrite
  for one decompiler emission wart (saved registers, local member chains);
  explicitly excludes error placeholders.
- **`partial_decomp` bucket**: histogram classification for functions whose
  decompilation contains error placeholders — more than `decompiler_failure`
  (something was emitted), less than `compiled` (it isn't a full seed).
- **Flywheel cycle**: the unattended loop — detect compiling seeds without
  scores → submit budgeted searches → harvest → update coverage.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: The extracted population's `compiled` bucket grows from 42 to
  **at least 200** functions (of 688 derivable), with the `func_<addr>`
  blocker class reduced to zero for known-target callees.
- **SC-002**: The generated layer is byte-stable across consecutive runs
  and introduces zero redefinition errors and zero static-population
  regressions (005's body-identity guard stays green).
- **SC-003**: The histogram (with the new `partial_decomp` bucket)
  maintains 100% coverage, bucket exclusivity, and byte-determinism across
  consecutive runs; scoped probes no longer overwrite the population
  artifact.
- **SC-004**: After one unattended flywheel window, **every** function in
  the `compiled` bucket has recorded score evidence with a retained
  artifact (100% scored coverage of compiled), with no operator
  intervention between start and report.
- **SC-005**: Static-population throughput is unharmed: no static search
  or promotion job is displaced by flywheel load (verifiable from queue
  ordering/evidence timestamps during the window).
- **SC-006**: Zero extracted-population entries in the promotion record
  after the feature's runs (005 firewall holds under flywheel volume).

## Assumptions

- The 2026-07-19 histogram is the baseline: 42 compiled / 597 blocked / 49
  decompiler_failure / 197 extent_conflict over 885 entries (688 real
  functions); 1,073 distinct blockers of which 289 are `func_`-shaped;
  top artifact classes M2C_ERROR=101, saved_reg_*≈69, local unkN≈74.
- Callee signatures inferred from decompiler output are good enough for
  compilation; correctness is settled by scoring, not by the prototype
  (005 established the score-is-arbiter principle).
- The existing steady-state daemon (farm) is the natural host for the
  flywheel cycle; no new service is implied.
- "Majority" in the feature description is directional; SC-001's ≥200 is
  the committed number for this feature (a ~5× improvement), chosen
  because the named blocker classes sum to plausibly more but overlap per
  function (one function often carries several blockers).
- The 49 `decompiler_failure` targets (m2c crashes) are out of scope to
  *fix* (m2c internals) but must remain honestly bucketed.

## Out of Scope

- **Blob rebuild / promotion for extracted evidence** — unchanged from 005;
  the firewall stays.
- **Struct-shape inference** beyond what a named blocker mechanically
  requires; no automated layout recovery.
- **Hand-typing campaigns** beyond the game-loop cluster precedent — this
  feature is deliberately the deterministic-compute counterpart.
- **m2c internals** — jump-table support, expression-merging fixes, and
  the 49 crash targets are decompiler work, not context work.
- **Score-0 chasing** — the flywheel records evidence; pushing individual
  functions to score 0 (longer budgets, near-miss closing) is steady-state
  pool operation, not gated by this feature.
