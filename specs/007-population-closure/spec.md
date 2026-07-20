# Feature Specification: Track B Population Closure

**Feature Branch**: `007-population-closure` (off `006-prototype-flywheel`)
**Created**: 2026-07-19
**Status**: Draft
**Input**: User description: "Track B population closure: discover and register the blob functions the inventory missed (callee-closure via extent scan, to fixpoint), auto-register data symbols from derived-asm access patterns, regenerate the declaration layer, and retake the compiled>=200 goal 006 recorded as structurally short. Out of scope: m2c internals, blob rebuild, promotion of extracted evidence."

## Overview

Feature 006's declaration layer worked perfectly and still fell structurally
short of SC-001 (60 compiled vs ≥200), and its stop-rule analysis
(`specs/006-prototype-flywheel/research/t009-shortfall.md`, both sections)
proved why: **the extracted-population inventory itself is incomplete**. The
work-inventory that seeded the population in 005 named 885 entries (688 real
functions after extent repair), but extracted code demonstrably calls at
least 135 further real functions inside the game-code blob that have no
target rows at all — so no extent, no object, no derivation, no declaration,
no score can exist for them, and every caller stays blocked on an
unresolvable `func_<addr>`. The 006 separator fix made this *worse* in the
best way: newly-decompiled FP bodies exposed even more unknown callees
(`func_` blocker classes rose 161 → 184). A parallel wall exists for data:
~119 raw-address symbols (~84 blocked targets) name globals beyond the
hand-curated symbol table — the exact class 005 closed by hand for one
cluster, now needing the automated treatment.

This feature closes the population: **discover** every blob function
reachable by call from the known population (iterating to a fixpoint, since
new functions call further new functions), **register** them through the
existing extent-scan/objectify machinery with full gate discipline,
**auto-register data symbols** from derived-asm access patterns under the
generated-artifact precedence rules 006 established, then regenerate the
declaration layer and **retake the ≥200-compiled goal** with the same honest
instrument. Everything is deterministic compute in the established mold; no
new services, no promotion path, no decompiler surgery.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Callee-closure discovery and registration (Priority: P1)

The operator runs the closure pass. It scans the derived assembly of every
derivable extracted target for call targets that land inside the game-code
blob but match no known target, extent-scans each such address with the 005
scanner (same oracle discipline: furthest-forward-branch / return+delay-slot,
conflict classification against existing extents), registers the survivors
as new extracted targets with objects, and repeats over the newly registered
functions until no new callee appears (bounded fixpoint). The population
report shows the growth with full provenance (which caller(s) discovered
each new target), and re-running the pass discovers nothing further
(idempotent).

**Why this priority**: It is the single largest quantified wall (130+
blocked callers; 135+ missing functions at last count, growing as more code
decompiles), and it compounds: every registered function becomes a
declarable callee *and* a scoreable target.

**Independent Test**: Run the closure pass twice; first run registers a
population delta with per-target provenance, second run registers zero. The
`func_<addr>` histogram blocker class for in-blob addresses drops to zero
after the declaration layer regenerates.

**Acceptance Scenarios**:

1. **Given** the current population, **When** the closure pass runs, **Then**
   every call target inside the blob that resolves to no known target is
   either registered as a new extracted target (with extent, object, and
   discovery provenance) or recorded with a classification for why not
   (conflict with an existing extent, scan overrun, misaligned/invalid).
2. **Given** newly registered targets, **When** the pass iterates, **Then**
   their own unknown callees are discovered too, and the pass terminates
   with an explicit fixpoint report (iterations, registered, rejected).
3. **Given** a completed closure pass, **When** it runs again with no input
   change, **Then** it registers nothing and changes no evidence
   (idempotency; supersession fires only if an extent genuinely changed).
4. **Given** the enlarged population, **When** the declaration layer
   regenerates and the full histogram runs, **Then** no `func_<addr>`
   blocker naming an in-blob address remains.

---

### User Story 2 - Automated data-symbol registration (Priority: P2)

The operator runs the data-symbol pass. It scans the derived assembly of the
(now enlarged) population for formed data addresses — the same
`lui`/`addiu`/`addu`/access idioms the 005/006 symbolizer already tracks —
that are not covered by the hand-curated symbol table, and emits a
**generated** symbol layer: placeholder-named (`D_<ADDR>`) entries with
width-derived types, consumed at lower precedence than the hand table
(hand names always win; a hand entry for the same address suppresses the
generated one). The derivation symbolizer consumes hand + generated tables
identically; typed externs for generated symbols ride the generated
declaration artifact, not the hand-curated header.

**Why this priority**: ~84 blocked targets are walled by exactly this
(`x<addr>` classes, and the numeric-deref → `void *` failure mode 005
documented); it reuses the generated-artifact discipline 006 built
(deterministic, regenerable, precedence-by-omission, evidence-stamped).

**Independent Test**: Double run is byte-stable; the `x<addr>` blocker
class collapses in the next histogram; a hand-table address never appears
in the generated layer; the static population is untouched.

**Acceptance Scenarios**:

1. **Given** derived assembly with a formed address not in any table,
   **When** the pass runs, **Then** the address appears in the generated
   symbol layer with a width-derived type and the deterministic
   widest-access rule applied (word > half > byte; float recorded when
   only FP accesses exist), with the deriving accesses cited.
2. **Given** an address present in the hand-curated table, **When** the
   pass runs, **Then** the generated layer omits it (recorded omission),
   and the symbolizer behavior for it is unchanged.
3. **Given** the generated symbol layer, **When** derivation re-runs,
   **Then** cache keys reflect the layer (changed layer ⇒ regeneration)
   and all 005/006 symbolizer safety rules (all-consumers, no mismatched
   pairs, synthetic lui re-emission) apply to generated names identically.

---

### User Story 3 - Retake the compiled goal with the honest instrument (Priority: P3)

With closure and data symbols in place and the declaration layer
regenerated, the operator runs the full histogram (which now covers the
enlarged population) and the flywheel picks up newly compiling seeds
automatically. The run-over-run diff attributes the movement to this
feature's passes.

**Why this priority**: It is the measurement, not new machinery — 006's
SC-001 target (≥200 compiled) carries over against the enlarged
denominator, and the flywheel (006) needs no changes to absorb the growth.

**Independent Test**: Full histogram twice (deterministic, 100% coverage of
the enlarged population, buckets sum); diff against the 006 close-out
baseline attributes movements; flywheel submits the new compiles unattended.

**Acceptance Scenarios**:

1. **Given** the enlarged population, **When** the histogram runs, **Then**
   every target (old and new) appears in exactly one bucket and the
   instrument's determinism/coverage guarantees hold.
2. **Given** the regenerated context, **When** compiled counts are read,
   **Then** compiled ≥ 200, or the stop rule fires with a residual-class
   analysis naming what remains (the 006 precedent: honest shortfall over
   forced numbers).
3. **Given** newly compiling seeds, **When** the flywheel cycles, **Then**
   they are submitted and scored without operator action under the
   existing priority/dedupe rules.

---

### Edge Cases

- **Callee inside an existing extent**: a discovered call target that lands
  strictly inside a registered function's extent is a mid-function call
  (tail-merged code or a data-in-text artifact) — classify and record, do
  not register (mirrors 005's `extent_conflict`).
- **Callee whose scan overruns or misaligns**: record with classification;
  never register a target without a gate-passed extent and object.
- **Discovery provenance vs. inventory names**: new targets have no
  human-assigned names; they register under the placeholder `func_<addr>`
  convention (renameable later), and target identity remains the address —
  a later hand-rename must not re-key evidence.
- **Fixpoint runaway**: a decode error could hallucinate call targets; the
  pass bounds iterations and total registrations (documented caps) and
  reports hitting a cap as an explicit outcome, not silent truncation.
- **Static-range callees**: the shortfall analysis found 5 call targets
  below the blob base (static range) — these resolve against the static
  population's known addresses and are declaration-layer business (006),
  never registration business (the static inventory is authoritative).
- **Width conflicts on data symbols**: the same address accessed at
  different widths takes the widest (documented rule); float-vs-integer
  same-width conflicts record the conflict and fall back to the integer
  type (m2c tolerates integer-typed loads better than wrong float types).
- **Hand-table precedence at derivation time**: the symbolizer must treat
  hand + generated as one lookup with hand winning on collision — never
  two lookups that could disagree mid-function.
- **Evidence safety**: registering new targets must not disturb existing
  targets' evidence; supersession applies only to targets whose own
  extent/object changed.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The closure pass MUST discover every call target inside the
  game-code blob unresolvable to a known target, iterating over newly
  registered targets to a bounded fixpoint, and MUST classify every
  discovered address into exactly one outcome: registered /
  inside-existing-extent / scan-failure / invalid.
- **FR-002**: Registration MUST go through the existing extent-scan
  gate and object build (005 contract), with discovery provenance (first
  discovering caller + instruction address) recorded per new target.
- **FR-003**: The closure pass MUST be idempotent (second run registers
  zero, changes no evidence) and MUST print a fixpoint report
  (iterations, per-outcome counts, caps hit if any).
- **FR-004**: The data-symbol pass MUST emit a generated symbol layer
  (deterministic, byte-stable, evidence-stamped, regenerable) covering
  formed data addresses observed in the population's derived assembly that
  the hand table does not cover, with width-derived types under the
  documented widest-access rule and per-entry access citations.
- **FR-005**: Precedence MUST be hand table > generated layer, resolved in
  a single lookup at derivation time; a hand entry suppresses the
  generated entry for the same address (recorded omission).
- **FR-006**: Generated data-symbol externs MUST ride generated artifacts
  (never hand-curated headers); all existing symbolizer safety rules
  apply to generated names identically; cache keys MUST cover the
  generated layer.
- **FR-007**: After closure + data symbols, the declaration layer (006)
  MUST regenerate over the enlarged population, and the histogram MUST
  cover the enlarged population with all 006 guarantees (six buckets,
  determinism, probe separation, diff attribution).
- **FR-008**: The flywheel (006) MUST absorb the enlarged compiled set
  with no changes beyond configuration-free scale (evidence dedupe,
  priority, firewall all unchanged).
- **FR-009**: All standing invariants carry forward: extracted evidence
  never enters lock/promotion; static population untouched (005 SC-005
  guard); orchestrator-local execution for everything except scoring.
- **FR-010**: If the compiled goal is not reached, the run MUST stop with
  a residual-class analysis (the 006 stop-rule pattern) rather than any
  scope-expanding workaround.

### Key Entities

- **Discovered callee**: an in-blob call target with no target row;
  carries discovery provenance; becomes a target only through the gate.
- **Fixpoint report**: the closure pass's durable output — iterations,
  outcome counts, caps, per-target provenance.
- **Generated symbol layer**: the data-symbol counterpart of 006's
  declaration layer — placeholder names, width-derived types, access
  citations, hand-table precedence by omission.
- **Enlarged population**: the extracted population after closure; the
  histogram/flywheel denominator (`population_complete` refers to it).

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: After one closure pass, a second pass registers zero new
  targets, and no `func_<addr>` blocker naming an in-blob address remains
  in the population histogram (the ≥135 known-missing functions are
  registered or explicitly classified).
- **SC-002**: The generated symbol layer is byte-stable across consecutive
  runs, never collides with the hand table, and the `x<addr>` blocker
  class (~84 targets at baseline) is eliminated from the histogram.
- **SC-003**: The histogram covers 100% of the enlarged population in
  exactly one bucket each, deterministically, and the diff view attributes
  this feature's movements against the 006 close-out baseline.
- **SC-004**: `compiled` reaches ≥ 200 (the inherited 006 target against
  the enlarged denominator) — or the FR-010 stop-rule report names the
  residual classes with counts (honest-shortfall precedent).
- **SC-005**: The flywheel scores 100% of the (enlarged) compiled set
  unattended; zero extracted entries in the promotion record; static
  throughput and static seed bodies unchanged.

## Assumptions

- Baseline is 006's close-out state: 60 compiled / 298 blocked / 281
  partial_decomp / 49 decompiler_failure / 197 extent_conflict over 885;
  184 `func_`-shaped blocker classes of which ≥135 addresses are in-blob
  and unregistered; ~119 raw-address symbols over ~84 targets.
- The 005 extent scanner and its contract are the registration gate; no
  new scanning theory is needed (its oracle discipline carries over).
- New-function discovery uses derived assembly (jal/j targets), which the
  disasm layer already extracts reliably; discovery does not require
  decompilation to succeed for the *discovering* function.
- The remaining `partial_decomp` mass (m2c "unset register" conventions,
  `cfc1`, stack-arg detection) is decompiler-capability work and is
  expected to survive this feature; SC-004's stop-rule branch exists
  partly for it.
- 006's flywheel, buckets, probe separation, and diff view are merged
  behavior this feature builds on (007 branches from 006).

## Out of Scope

- **m2c internals** — the unset-register/cfc/stack-arg placeholder causes;
  jump tables; expression merging. A future feature may vendor-patch m2c;
  this one does not.
- **Blob rebuild / promotion for extracted evidence** — firewall stands.
- **Hand-naming or hand-typing** of newly registered functions/symbols —
  placeholders only; semantic naming happens when functions get matched
  or studied, not at registration.
- **Static population changes** — static-range callees are resolved by
  the declaration layer against the existing static inventory only.
