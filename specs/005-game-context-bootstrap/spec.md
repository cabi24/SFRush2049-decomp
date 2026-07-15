# Feature Specification: Game-Code Context Bootstrap (Track B Walking Skeleton)

**Feature Branch**: `005-game-context-bootstrap`
**Created**: 2026-07-15
**Status**: Draft
**Input**: User description: "Game-code context bootstrap (Track B walking skeleton): make m2c emit compilable, scoreable seeds for the gstate/game-loop cluster (~10 functions around func_800FD464), by defining the minimal game structs from the arcade source. Include an m2c-failure histogram across all 885 dynamic targets as the feature's measurement instrument. Success = seeds for the cluster compile and score through the existing pipeline; the histogram ranks what blocks the rest. Explicitly out of scope: blob rebuild (game code can't reach ROM hash yet — evidence only), full struct inference, re-planning Track A."

## Overview

Track A (the 246 static ROM targets) has an end-to-end path: automated
decompilation seeds a candidate, the search pool refines it, verified matches
lock and splice into a SHA-1-exact ROM. Track B — the 885 functions extracted
from the compressed game-code blob — has target objects and disassembly in the
evidence store but **zero automated path from disassembly to a scoreable
candidate**: the seeding loop and its failure-analysis report are both
hard-limited to the static population, and the automated decompiler lacks the
game's data-structure vocabulary (game state, car/player state, input records),
so game functions that touch those structures cannot even be attempted.

This feature is the Track B walking skeleton: extend the existing
seed → compile → score loop to the extracted population, give the decompiler
just enough game-structure context (hand-derived from the arcade source, which
shares this code's ancestry) to make one concrete, high-value cluster work —
the ~10 functions the main per-frame game loop (`game_loop` @ 0x800FD464)
dispatches to — and, in the same pass, run the failure analysis across **all
885** extracted targets to produce a ranked histogram of what blocks the rest.
The cluster proves the pipe; the histogram is the measurement instrument that
turns "the other 875" from an unknown into a prioritized worklist.

Scoring evidence for extracted targets is exactly as meaningful as it was for
static targets before promotion existed: a score-0 match is a verified
decompilation. What it does *not* yet buy is ROM bytes — the game-code blob is
recompressed data, so no promotion/splice path exists for this population.
That boundary is explicit and out of scope.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Game-loop cluster seeds compile and score (Priority: P1)

The project operator runs the automated seeding command against the game-loop
cluster (the dispatch function at 0x800FD464 and the ~10 functions it calls:
mode handling, input processing, attract/transition, playgame update, race
state machine, sound control, countdown, gameplay input). For each function,
the system produces a structurally-correct C seed from its disassembly, the
seed compiles under the project compiler, and a similarity score against the
function's target object is recorded in the evidence store — the same
score/evidence lifecycle static targets use.

**Why this priority**: This is the walking skeleton itself. One cluster
flowing end-to-end proves every joint in the Track B pipe (asm sourcing from
the evidence store, game-type context, compile, score, evidence recording) and
delivers the first scoreable game-code candidates the search pool can refine.

**Independent Test**: Run the seeding command for the named cluster on a
fresh checkout with a builder node available; inspect the evidence store for
recorded scores on those target functions.

**Acceptance Scenarios**:

1. **Given** the game-loop cluster functions exist as extracted targets with
   target objects, **When** the operator runs automated seeding for the
   cluster, **Then** at least 8 of the ~10 functions yield a seed that
   compiles without errors.
2. **Given** a compiled cluster seed, **When** it is submitted through the
   existing scoring path, **Then** a numeric score against the function's
   target object is recorded in the evidence store with the seed retained as
   a provenance-stamped artifact.
3. **Given** recorded cluster scores, **When** the operator views the standard
   report, **Then** extracted-population results are visible alongside static
   ones and clearly distinguished by population.

---

### User Story 2 - Failure histogram across all 885 extracted targets (Priority: P2)

The operator runs the failure-analysis command across the entire extracted
population. For every function, the system attempts a seed and classifies the
outcome (compiled / blocked, and for blocked: which missing type, structure,
or symbol caused it, or that the decompiler itself failed on the input). The
output is a ranked report: each blocker with the count of functions it blocks,
so the operator knows exactly which struct definition or fix unlocks the most
functions next.

**Why this priority**: The cluster (P1) proves the pipe for ~10 functions;
the histogram prices the remaining ~875. Without it, the next context
investment is guesswork. It reuses the P1 machinery, so it lands second.

**Independent Test**: Run the analysis command with no limit; verify the
report covers all extracted targets with objects, and that every function is
accounted for in exactly one outcome bucket.

**Acceptance Scenarios**:

1. **Given** 885 extracted targets, **When** the analysis runs, **Then** a
   report is produced in which every target appears in exactly one outcome
   bucket (seed compiled / blocked by named symbol(s) / decompiler failure /
   no disassembly available).
2. **Given** the report, **When** the operator reads the blocker ranking,
   **Then** each blocker lists the count and names of functions it blocks and,
   where the arcade source contains a same-named definition, a pointer to it.
3. **Given** a re-run after a context improvement (e.g., a new struct
   definition), **Then** the histogram reflects the change (functions move
   from blocked to compiled) so progress is measurable run-over-run.

---

### User Story 3 - Minimal game-structure context from arcade source (Priority: P3)

The operator (or an assisting agent) consults the arcade source to define the
minimal set of game data structures the cluster functions need — game state
variable, player/car state, input record — in the project's decompiler
context, with a provenance comment tracing each definition to its arcade
origin. The definitions are additive: existing static-population seeding
continues to work unchanged.

**Why this priority**: It is the enabling content for P1 rather than an
independently valuable outcome; it ships as part of making the cluster
compile, and its size is deliberately capped at "what the cluster needs."

**Independent Test**: With the context definitions in place, re-run the P2
histogram and confirm the cluster's former blockers no longer appear; run
static-population seeding for a known-good function and confirm its seed
still compiles.

**Acceptance Scenarios**:

1. **Given** the new game-structure definitions, **When** static-population
   seeding runs on a previously-compiling function, **Then** its seed still
   compiles (no regression).
2. **Given** a game-structure definition, **When** a reviewer inspects it,
   **Then** its arcade-source origin (file and structure name) is recorded
   alongside it.

---

### Edge Cases

- **Trivial/stub targets**: some cluster members are 2-instruction stubs
  (e.g., the countdown pair). Seeds for these are near-empty functions; they
  must still flow through compile/score rather than being special-cased away,
  but the "≥8 of ~10 compile" criterion must not be satisfiable by stubs
  alone — the large dispatch and update functions are the point.
- **Decompiler crashes**: the decompiler is known to assert on some inputs
  (the existing game headers already crash its parser). A crash on one target
  must be recorded as that target's outcome and must not abort the batch.
- **Extracted disassembly quality**: extracted-population disassembly is
  derived from the raw game-code image, not the curated static asm tree; it
  carries raw address literals where static asm has symbols, and known
  function names must be re-applied to call targets for the seeds to be
  readable. Seeding must tolerate this (the score is against the target
  object, which gates truth).
- **Duplicate/ambiguous names**: extracted target identifiers are
  human-assigned names; if a name collides with a static target or an arcade
  symbol, evidence must attach to the correct target (targets are keyed by
  identity, not display name).
- **Scoring semantics for extracted targets**: extracted target objects were
  assembled from raw words (no relocations); score interpretation must not
  silently assume the reloc-aware tier that most static targets now have.
- **No builder available**: compile/score requires the builder node; the
  histogram's *seed generation and blocker classification* portion must be
  runnable locally on the orchestrator so measurement doesn't depend on farm
  capacity.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The automated seeding path MUST accept extracted-population
  targets, sourcing their code from the extracted game-code image (their
  code bytes and address ranges are already recorded; disassembly is derived
  from them deterministically) rather than the static assembly tree.
- **FR-002**: The system MUST provide a way to run seeding for a named set of
  targets (the game-loop cluster) as one operation.
- **FR-003**: The decompiler's type context MUST be extensible with
  game-structure definitions without breaking existing static-population
  seeding; each added definition MUST carry a provenance note naming its
  arcade-source origin.
- **FR-004**: Game-structure definitions added under this feature MUST be
  limited to what the game-loop cluster functions reference (bounded scope;
  full struct inference is out of scope).
- **FR-005**: Seeds produced for extracted targets MUST flow through the
  existing compile/score/evidence lifecycle unmodified: scores recorded per
  target with candidate provenance, artifacts retained, and results visible
  in the standard reports, distinguishable by population.
- **FR-006**: The failure-analysis run MUST cover every extracted target with
  a target object, classify each into exactly one outcome bucket (compiled /
  blocked-by-named-symbols / decompiler-failure / no-disassembly), and rank
  blockers by number of functions blocked.
- **FR-007**: The failure-analysis report MUST be written as a durable
  evidence artifact (re-runnable, diffable run-over-run) and include, per
  blocker, the blocking symbol, blocked-function count and names, and an
  arcade-source pointer when a same-named definition exists there.
- **FR-008**: Seed generation and blocker classification MUST run on the
  orchestrator alone (no compute nodes required); only compile/score requires
  the builder.
- **FR-009**: A decompiler crash or malformed disassembly for one target MUST
  be recorded as that target's outcome without aborting the batch.
- **FR-010**: Extracted-population match evidence MUST NOT enter the
  promotion/splice path: no lock may migrate to a ROM-TU path and no ROM
  rebuild may be attempted from extracted-population results under this
  feature.

### Key Entities

- **Extracted target**: one of the 885 functions recovered from the
  compressed game-code blob; has an identity, an address, a target object,
  and addressable code bytes in the extracted game-code image; belongs to
  the `extracted` population (distinct from the 246 `static` targets).
- **Game-loop cluster**: the named set of ~10 extracted targets centered on
  the per-frame dispatch function at 0x800FD464 and its direct callees
  (mode handler, input processing, attract/transition, playgame update, race
  state machine, sound control, countdown pair, gameplay-pad input).
- **Game-structure context**: the set of type/structure/global definitions
  supplied to the decompiler so game functions type correctly; grown
  hand-over-hand from the arcade source with provenance.
- **Seed**: a structurally-correct C rendering of one target's disassembly,
  self-contained enough to compile; the input to scoring and search.
- **Failure histogram**: the ranked report of blockers across the extracted
  population; each entry is (blocker symbol → blocked functions), plus
  per-target outcome buckets; the feature's measurement instrument.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: At least 8 of the game-loop cluster's ~10 functions produce
  seeds that compile, including at least 4 functions of 60+ instructions
  (the dispatch/update/state-machine members, not just stubs).
- **SC-002**: Every compiling cluster seed has a numeric score recorded in
  the evidence store, attributable to its target and retained artifact.
- **SC-003**: The failure histogram accounts for 100% of extracted targets
  with target objects (885 at time of writing), each in exactly one outcome
  bucket, and is reproducible: two consecutive runs with no intervening
  context change produce the same bucket counts.
- **SC-004**: The histogram's top-10 blockers collectively account for a
  majority of blocked functions (validating "define few structs, unlock
  many" as the Track B strategy) — or, if they don't, the report itself
  demonstrates that and redirects the strategy; either way the operator can
  name the next-highest-value context investment from the report alone.
- **SC-005**: Static-population seeding produces byte-identical seeds for a
  sampled known-good function before and after the game-context additions
  (no regression).
- **SC-006**: No promotion, lock migration, or ROM rebuild occurs from
  extracted-population evidence (verified by inspecting the promotion record
  after the feature's runs).

## Assumptions

- The ~10 cluster functions are the dispatch function at 0x800FD464 plus its
  direct callees already identified in project notes (addresses 0x800C9AE0,
  0x800EDDC0, 0x800C997C, 0x800B37E8, 0x800CA3B4, 0x800DB81C, 0x800FBF88,
  0x800FBC30, 0x800A04C4). All ten are confirmed present in the evidence
  store with target objects; disassembly for extracted targets is not
  pre-stored — it is derived on demand from the extracted game-code image,
  whose bytes and per-function address ranges are already recorded.
- The arcade source (`reference/repos/rushtherock/`) is the authoritative
  reference for game-structure shapes; N64-side layout differences are
  expected and resolved empirically (the score is the arbiter), not by
  inference tooling.
- "Score through the existing pipeline" means the same compile-and-compare
  scoring the matching pipeline already performs for candidates; no new
  scoring semantics are introduced for extracted targets beyond the
  population label they already carry.
- The two confirmed compiler flagsets used for library matching are the
  starting point for game code; if game code needs a different flagset, that
  discovery is *recorded* by this feature's runs, but flag tuning is not a
  requirement here.
- The existing failure-analysis machinery (outcome classification, blocker
  tokenization, arcade hinting) is sound; this feature widens its population
  and hardens its coverage guarantees rather than redesigning it.

## Out of Scope

- **Blob rebuild / ROM-hash promotion for game code**: the game-code blob is
  compressed in ROM; no splice/promotion path exists for extracted targets.
  This feature produces match *evidence only* for them.
- **Full struct inference**: no automated struct-layout recovery; context
  grows hand-over-hand, capped at the cluster's needs.
- **Re-planning Track A**: the static-population pipeline (matrix, corpus,
  reloc-aware targets, promotion) is untouched except for the shared
  machinery this feature extends, which must remain backward compatible.
- **Search-pool refinement of the new seeds**: submitting cluster seeds to
  the permuter farm at scale is the natural next step but not gated by this
  feature; this feature ends at recorded scores.
