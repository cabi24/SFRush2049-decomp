# Feature Specification: Corpus Candidates — search-don't-synthesize matching for library code

**Feature Branch**: `002-corpus-candidates`
**Created**: 2026-07-08
**Status**: Draft
**Input**: User description: "Corpus candidates: search-don't-synthesize matching for library code. Extend the conveyor matching pipeline with a second candidate corpus beyond the arcade source: canonical, known-good library implementations from local git clones (V1: decompals/ultralib). For N64 targets that are generic library code — permanently no_ancestry in the arcade matrix — compile canonical source against the target with pinned IDO flagsets and score it, using exact function-name pairing instead of the size-window matrix. Includes a reloc-blind secondary score. Candidates gain origin and provenance. Score-0 hits feed the regression lock; reloc-blind-0 hits produce a match artifact with provenance and a reloc_only_diff flag. decomp.me out of scope for V1. No network in the pipeline; deterministic dedupe preserved."

## Context

Roughly 150 of the 228 static-ROM functions are generic library code (OS, message
queues, controller pak, matrix utilities, C runtime). The arcade matrix can never
match them — they are not arcade code — so today they are permanently parked as
`no_ancestry`. Canonical, community-verified implementations of exactly this code
exist as local git clones; 85 of the 1,131 inventoried targets already name-match a
function in the first corpus (ultralib). Separately, hand verification proved that a
function whose instructions are identical to its target can still never reach score 0
today, because target objects carry absolute addresses where compiled code carries
zeroed relocation fields (osCreateMesgQueue: score 20, instruction-identical). Both
gaps are addressed here; full relocation-aware target assembly remains a separate
future feature.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Name-paired canonical matching (Priority: P1)

The operator registers a local corpus clone, ingests its functions, and submits
name-paired compile-and-score jobs: every inventoried target whose name exactly
matches a corpus function gets that function compiled under the pinned flag sets and
scored against it. Results appear in the same rankings and reports as arcade-matrix
results.

**Why this priority**: This is the feature's reason to exist — it converts ~85
permanently-unmatchable library targets into targets with scored, provenance-backed
evidence, using a few minutes of pool time instead of another full matrix run.

**Independent Test**: With the corpus already cloned and the pool up, run
ingest + submit + result ingest end to end; confirm the report shows scored evidence
for the name-matched library targets, and that leaf functions already proven matched
(strlen, guMtxIdentF) score 0 from corpus source too.

**Acceptance Scenarios**:

1. **Given** a registered corpus clone at a recorded commit, **When** the operator
   runs corpus ingest, **Then** every extractable corpus function is recorded as a
   candidate with its origin and provenance (repo, path, commit), and re-running
   ingest records nothing new.
2. **Given** ingested corpus candidates, **When** the operator submits name-paired
   jobs, **Then** only exact target-name matches generate work, each pairing is
   scored under every pinned flag set, and identical resubmissions are answered from
   the result cache without new compute.
3. **Given** completed corpus jobs, **When** results are ingested, **Then** they are
   queryable through the existing rankings/report path alongside arcade results, and
   arcade-origin results are unchanged.

---

### User Story 2 - Relocation-blind secondary score (Priority: P2)

Every scored cell (corpus and arcade alike) additionally records a relocation-blind
score: the same comparison after normalizing the instruction fields that relocations
patch (high/low half-word immediates and absolute jump targets) on both sides. A
function that is instruction-identical to its target except for unresolved symbol
addresses reports relocation-blind score 0.

**Why this priority**: Library functions reference global symbols almost by
definition, so without this score the P1 evidence is uninterpretable noise — the
proven instruction-identical case reads as "score 20" with nothing distinguishing it
from a near miss.

**Independent Test**: Score the hand-verified osCreateMesgQueue pairing; it must
report true score nonzero and relocation-blind score 0. Score strlen (a known exact
match); both scores must be 0 — normalization must never break an existing exact
match.

**Acceptance Scenarios**:

1. **Given** a compiled function identical to its target except in relocation-patched
   fields, **When** it is scored, **Then** relocation-blind score is 0 and the true
   score is unchanged from today's behavior.
2. **Given** a compiled function with a genuine instruction difference inside a
   relocation-patched instruction's opcode or registers, **When** it is scored,
   **Then** relocation-blind score is nonzero (only the patched fields are
   normalized, not the whole instruction).
3. **Given** existing verified matches (true score 0), **When** re-scored with the
   new metric present, **Then** both scores are 0.

---

### User Story 3 - Match artifacts and honest promotion gates (Priority: P3)

For each target whose best corpus evidence is a true score 0, the pairing becomes
eligible for the existing promotion and regression-lock flow. For each target whose
best evidence is relocation-blind 0 but true score nonzero, the pipeline writes a
match artifact — the candidate source with a provenance header (origin repo, path,
commit, flags, both scores) — into that function's work directory and flags the
target `reloc_only_diff`. Flagged targets are never auto-promoted; they upgrade to
full verification automatically when relocation-aware target assembly lands, because
re-scoring is content-addressed and recomputes on new target objects.

**Why this priority**: Depends on P1+P2 output. Turns evidence into artifacts a human
(or later feature) can act on, while keeping the promotion gate strict — nothing
enters the repo or the lock without byte-identity.

**Independent Test**: After a P1 run, confirm flagged targets have a match artifact
with complete provenance, appear in a dedicated report section, and that no
`reloc_only_diff` target was promoted or locked.

**Acceptance Scenarios**:

1. **Given** a pairing with relocation-blind score 0 and true score nonzero,
   **When** results are ingested, **Then** a match artifact with full provenance
   exists in the function's work directory and the target is flagged
   `reloc_only_diff`.
2. **Given** a `reloc_only_diff` flag, **When** the operator views the attention or
   corpus report, **Then** the target is listed with both scores and provenance.
3. **Given** a pairing with true score 0, **When** results are ingested, **Then** the
   target follows the existing candidate-identified/promotion path and is eligible
   for the regression lock.

---

### Edge Cases

- Corpus clone is missing, at the wrong path, or has uncommitted local edits: ingest
  must refuse and say why (provenance would be a lie) rather than record a commit
  that doesn't describe the bytes.
- Two corpus roots (or a corpus root and the arcade tree) define a function with the
  same name: both become candidates with distinct identities; name-pairing submits
  both against the target.
- A corpus function fails to compile under a pinned flag set: recorded as a compile
  failure visible to the existing failure-clustering report, distinguishable by
  origin.
- A name-matched target has no target object in the inventory: skipped with an
  explicit count in the submit output, not silently dropped.
- The same function exists in multiple SDK-version variants inside one corpus
  (different paths): each variant is a separate candidate; rankings naturally sort
  the best.
- Corpus source that matches an already-verified (locked) target: harmless — evidence
  lands in rankings; the lock and promotion flow ignore already-verified targets.
- A scoring-behavior change (new secondary metric) alters what nodes compute: results
  produced by old and new toolkits must never be merged into one comparison set
  (existing single-toolkit comparability rule applies).

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST support registering one or more corpus roots, each a
  local git clone identified by path and recorded commit; registration and all later
  pipeline stages MUST NOT require network access.
- **FR-002**: Corpus ingest MUST extract every function definition from a registered
  root and record it as a candidate carrying an origin (distinguishing it from arcade
  candidates and from other corpus roots) and provenance: repository identity,
  file path within it, and the clone's commit at ingest time.
- **FR-003**: Ingest MUST be idempotent, MUST refuse a missing or dirty clone unless
  explicitly overridden, and MUST NOT modify existing arcade candidates or their
  identities.
- **FR-004**: The system MUST generate compile-and-score work by exact function-name
  pairing: each inventoried target whose identifier equals a corpus candidate's name
  is paired with that candidate under every configured flag set, at higher priority
  than bulk matrix work. The size-window filter MUST NOT apply to name pairings.
- **FR-005**: Name-paired candidate compilation MUST use the candidate's own
  repository context (its translation unit and that repository's headers), not the
  arcade compatibility shim.
- **FR-006**: Every scored comparison MUST additionally record a relocation-blind
  score, defined as the identical comparison after normalizing, in both instruction
  streams, only the fields relocations patch: high/low half-word immediate fields and
  absolute jump target fields. Register operands, opcodes, and all other fields MUST
  remain significant. A comparison equal except in normalized fields MUST yield
  relocation-blind score 0.
- **FR-007**: The relocation-blind score MUST be recorded for corpus and arcade
  results alike, and a true score of 0 MUST always imply relocation-blind 0.
- **FR-008**: Corpus results MUST flow through the existing result-ingest, rankings,
  and report paths, comparable only within a single toolkit as today; reports MUST be
  able to distinguish evidence by origin.
- **FR-009**: A target whose best corpus evidence is true score 0 MUST follow the
  existing promotion-eligibility path and be eligible for the regression lock.
- **FR-010**: A target whose best corpus evidence is relocation-blind 0 with true
  score nonzero MUST be flagged `reloc_only_diff` and MUST NOT be auto-promoted,
  auto-locked, or committed to the repository; the system MUST write a match artifact
  (candidate source plus provenance header including both scores, flags, origin,
  path, and commit) into the target's work directory.
- **FR-011**: `reloc_only_diff` targets MUST be re-evaluated by ordinary re-scoring
  when target objects change (no bespoke bookkeeping): the flag upgrades or clears
  based on fresh evidence.
- **FR-012**: Determinism and dedupe MUST be preserved: identical corpus commit,
  candidate source, flag set, and toolkit MUST produce cache hits instead of new
  compute; changed inputs MUST produce new work.
- **FR-013**: A corpus coverage report MUST state, at minimum: candidates ingested
  per origin, name-paired target count, targets skipped (and why), compile success
  rate per origin, and per-target best true and relocation-blind scores.

### Key Entities

- **Corpus Root**: a registered local clone (path, repository identity, recorded
  commit) serving as a candidate source; the arcade tree is conceptually the
  pre-existing root with origin `arcade`.
- **Corpus Candidate**: one extractable function from a corpus root — name, source
  location, body identity, origin, provenance; participates in scoring exactly like
  an arcade candidate.
- **Scored Cell**: the evidence unit — (target, candidate, flag set, toolkit) with
  a true score and a relocation-blind score.
- **Match Artifact**: a per-target file in the function's work directory holding the
  best corpus candidate's source and full provenance, produced only for
  `reloc_only_diff` targets.
- **`reloc_only_diff` flag**: a target state meaning "instruction-identical modulo
  relocation fields; blocked only on relocation-aware target assembly"; never a
  promotion state.

## Assumptions

- The first corpus root is `reference/repos/ultralib` (decompals/ultralib), already
  cloned; its commit is recorded at ingest. Additional roots (other decomp clones,
  C-runtime sources) are registerable later without schema or workflow changes.
- The two confirmed IDO flag sets remain the initial configured flag sets; the
  existing per-TU flag sweep path remains the tool for discovering others.
- The x86-64 builder/compute node (watchman) is available for the end-to-end runs;
  all local stages run on the Pi.
- The regression lock (matched.lock.json) and failure-clustering report from the
  001 feature exist and are the integration points named here.
- decomp.me is out of scope for V1: scripted access is blocked upstream, and the
  chosen corpus dominates it for library code. Revisit only for functions no local
  corpus covers.
- Relocation-aware target assembly (true relocations in target objects) is a
  separate future feature; this feature must leave it a clean upgrade path via
  FR-011.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: At least 80 library targets gain scored canonical-source evidence in a
  single ingest→submit→report cycle (85 exact name-matches are known today).
- **SC-002**: The hand-verified instruction-identical pairing (osCreateMesgQueue)
  reports relocation-blind score 0 through the pipeline — machine-visible without
  human disassembly diffing.
- **SC-003**: Known exact matches (strlen, guMtxIdentF) still report true score 0
  and relocation-blind score 0 — the new metric breaks no existing exact match.
- **SC-004**: Re-running the identical corpus cycle schedules zero new compute
  (100% cache answers).
- **SC-005**: The operator can answer "which library functions are blocked only on
  relocation handling, and from which canonical source" from one report, with
  count and provenance, in under a minute.
- **SC-006**: Arcade-matrix results and rankings are bit-for-bit unchanged for
  arcade-origin evidence under the same toolkit (no regression in the existing
  pipeline).
- **SC-007**: No target lacking true score 0 is promoted, locked, or committed by
  anything this feature adds.
