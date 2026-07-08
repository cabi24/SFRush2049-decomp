# Feature Specification: Relocation-Aware Target Assembly

**Feature Branch**: `003-reloc-aware-targets`
**Created**: 2026-07-08
**Status**: Draft
**Input**: User description: "Relocation-aware target assembly: make true score 0 reachable for functions that reference global symbols. Assemble targets from splat symbol-relative assembly so target objects carry real relocations; per-target round-trip verification gate with raw-word fallback; tiered coverage; supersede stale matrix evidence when a target object changes; the 19 reloc_only_diff targets upgrade to true score 0 automatically."

## Context

Feature 002 measured the problem precisely: target objects are assembled from raw
ROM instruction words with absolute addresses baked in, while compiled candidates
carry zeroed relocation fields — so **no function that references a global symbol
can ever reach true score 0**. Nineteen targets sit flagged `reloc_only_diff`
(instruction-identical modulo relocation fields, e.g. osCreateMesgQueue true=20 /
reloc-blind=0), the promotion and lock gates are closed to all of them, and every
non-leaf cell in the 281,933-cell matrix carries systematic penalty noise. The
splat disassembly already contains symbol-relative operands (`%hi(sym)`/`%lo(sym)`,
`jal sym`) for 56 of the 88 static assembly files, and targets are already
assembled locally — the raw-word choice was deliberate V1 conservatism ("no
disassembler round-trip can distort the target"), which this feature preserves
through a per-target round-trip verification gate rather than blind trust.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Targets carry real relocations (Priority: P1)

The operator re-runs target extraction. For every static target whose splat
assembly is symbol-relative, the target object is now assembled from that assembly
text and carries real relocations for its global-symbol references. A per-target
round-trip gate proves no distortion: the reassembled instruction words, masked at
the object's own relocation sites, must equal the original ROM words masked
identically — any target failing the gate keeps its raw-word object and is counted,
never silently trusted.

**Why this priority**: This is the mechanism everything else rides on; the gate is
what makes it safe to ship.

**Independent Test**: Re-run extraction; confirm osCreateMesgQueue's target object
contains relocation records for its global reference; confirm every reloc-aware
target passed the round-trip gate; confirm gate failures (if any) fell back to
raw-word and appear in the coverage report.

**Acceptance Scenarios**:

1. **Given** a static target whose splat assembly is symbol-relative, **When**
   extraction runs, **Then** its target object carries relocation records at
   exactly the instructions that reference global symbols, and the round-trip gate
   passes (masked reassembled words equal masked ROM words).
2. **Given** a target whose assembly text does not round-trip cleanly, **When**
   extraction runs, **Then** its raw-word object is kept, the failure is counted
   with a reason, and nothing about its existing evidence changes.
3. **Given** a dynamic game-code target (custom disassembly, no symbol operands),
   **When** extraction runs, **Then** it keeps its raw-word object and is counted
   in the raw-word tier (V1 scope).
4. **Given** extraction has run twice with unchanged inputs, **Then** target
   objects are byte-identical both times (deterministic assembly).

---

### User Story 2 - Stale evidence is superseded, never mixed (Priority: P2)

When a target's object changes (raw-word → reloc-aware, or any future change),
its previously scored evidence is superseded: rankings, reports, promotion
eligibility, and the reloc_only_diff recompute only ever consider evidence scored
against the target's *current* object. Submitting after a target change schedules
fresh work for exactly the changed targets instead of silently deduping against
stale cells.

**Why this priority**: Without this, the feature corrupts the evidence base —
score-0 claims could point at objects that no longer exist. This is the data-model
half of the feature and the reason it needs a spec at all.

**Independent Test**: Score a pairing, change the target's object, re-submit:
fresh cells are scheduled and the report shows only current-object evidence;
unchanged targets are still answered from cache.

**Acceptance Scenarios**:

1. **Given** cells scored against a target's old object, **When** the target
   becomes reloc-aware, **Then** submit schedules new work for its pairings
   (no dedupe against stale cells) and rankings/reports exclude the stale evidence.
2. **Given** a target whose object did not change, **When** submit re-runs,
   **Then** its cells are answered from cache (100% dedupe, no recompute).
3. **Given** stale and current evidence exist in storage, **Then** no report,
   ranking, flag recompute, or promotion decision ever mixes the two.

---

### User Story 3 - The flagged nineteen upgrade automatically (Priority: P3)

After extraction and a corpus re-submit, the 19 `reloc_only_diff` targets re-score
against their new reloc-aware objects through the ordinary 002 upgrade path: those
that are genuinely matched reach true score 0 and enter the normal promotion/lock
eligibility path; the flag clears or upgrades with zero bespoke bookkeeping. The
12 locked leaf functions still verify at score 0 against their new objects.

**Why this priority**: This is the payoff and the end-to-end proof, but it is
entirely a consequence of P1+P2 plus 002's existing machinery.

**Independent Test**: Run the corpus cycle after extraction; confirm
osCreateMesgQueue reports true score 0; confirm `lock verify` still passes for all
12 locked functions; confirm the flag count dropped accordingly.

**Acceptance Scenarios**:

1. **Given** a reloc_only_diff target whose candidate is instruction-identical
   modulo relocations, **When** the corpus cycle re-runs against its reloc-aware
   object, **Then** its true score is 0 and it follows the normal
   candidate-identified/promotion path; the reloc_only_diff flag no longer applies.
2. **Given** the 12 locked functions, **When** re-verified against reloc-aware
   targets, **Then** all still score 0 (the lock stays green; no regression).
3. **Given** a flagged target whose candidate has a genuine difference beyond
   relocations, **When** re-scored, **Then** its true score remains nonzero and
   the flag state follows 002's recompute rules honestly.

---

### Edge Cases

- Assembly text references a symbol absent from the symbol table: the assembler
  still emits a relocation for it (external reference) — acceptable; the round-trip
  gate is the arbiter of correctness.
- A `%hi/%lo` pair whose baked ROM addend differs from the symbol's address (offset
  addressing, e.g. `%lo(sym+4)`): the gate compares masked words, so addend
  differences inside masked fields pass; the relocation records preserve what the
  scorer needs. If splat emitted text that assembles to different *unmasked* bits,
  the gate fails and the target falls back.
- Branch-target labels local to the function: must resolve within the extracted
  assembly region; if the region is incomplete the assembly fails → fallback path,
  counted.
- A target present in both tiers over time (raw-word evidence from before, reloc
  evidence after): US2 rules apply — only current-object evidence counts.
- The smoke fixture and the regression lock verify against inventory objects:
  both must keep passing across the transition (SC-004, SC-005).
- Re-running extraction with an updated symbol table changes some targets' objects
  again: same supersede semantics apply; coverage report reflects the new tier
  split.
- Storage growth from superseded blobs: bounded by the existing GC (unreferenced
  blob reclamation); superseded target blobs become GC-eligible once no live state
  references them.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: Target extraction MUST, for every static target whose splat assembly
  is symbol-relative, produce a target object carrying real relocation records for
  its global-symbol references, assembled deterministically (identical inputs ⇒
  identical bytes).
- **FR-002**: Every reloc-aware target MUST pass a round-trip verification gate
  before replacing its raw-word object: the reassembled object's instruction
  words, masked at that object's own relocation sites (the 002 masking rules),
  MUST equal the original ROM instruction words masked identically. A gate failure
  MUST keep the raw-word object, record the failure reason, and MUST NOT abort the
  rest of extraction.
- **FR-003**: Dynamic game-code targets (custom disassembly without symbol
  operands) remain raw-word in this feature; the tier of every target (reloc-aware
  vs raw-word, with fallback reasons) MUST be recorded and reportable.
- **FR-004**: Every scored evidence cell MUST be attributable to the exact target
  object it was scored against; rankings, reports, promotion eligibility, the
  reloc_only_diff recompute, and lock verification MUST only consider evidence
  scored against each target's current object.
- **FR-005**: Submit-side dedupe MUST treat a changed target object as new work:
  pairings against a target's current object that lack evidence MUST be scheduled
  even when stale cells exist for the same (target, candidate, flag set, toolkit);
  unchanged targets MUST still dedupe to cache.
- **FR-006**: Superseded evidence MUST NOT be destroyed (results remain
  content-addressed and auditable) but MUST be excluded from every live decision
  path; superseded target blobs MUST become eligible for the existing GC.
- **FR-007**: The reloc_only_diff upgrade path MUST require no bespoke
  bookkeeping: 002's recompute over current-object evidence upgrades, clears, or
  retains flags on its own.
- **FR-008**: The true-score and relocation-blind-score definitions are unchanged;
  with relocations present on both sides, the existing scorer's own relocation
  handling determines the true score. Leaf-function scores MUST be unaffected.
- **FR-009**: A coverage report MUST state: targets per tier (reloc-aware /
  raw-word-fallback / raw-word-dynamic), gate failures with reasons, and the
  before/after effect on flagged targets after the first re-score.
- **FR-010**: Determinism end to end: re-running extraction with unchanged inputs
  MUST change no target object; re-running submit with unchanged targets MUST
  schedule zero new compute.

### Key Entities

- **Target object (versioned)**: the per-target comparison object; now carries an
  identity that evidence references, a tier (reloc-aware / raw-word), and a gate
  result with reason on fallback.
- **Round-trip gate record**: per-target pass/fail + reason; the audit trail for
  "no silent distortion".
- **Evidence cell (extended)**: gains attribution to the exact target object it
  was scored against; the unit over which "current evidence only" is enforced.
- **Coverage tier report**: the operator's view of how much of the inventory is
  reloc-aware and why the rest isn't.

## Assumptions

- The splat assembly under `asm/us/` is the authoritative symbol-relative source
  for static targets; the symbol table (`symbol_addrs.us.txt`, 3,406 entries) is
  used as-is — improving symbol coverage is worthwhile but out of scope.
- The local MIPS assembler already used for raw-word targets is available and
  sufficient for symbol-relative assembly on the Pi.
- Extending symbol-relative treatment to the 752 dynamic game-code targets
  (synthesizing hi/lo pairs from the custom disassembly) is explicitly a future
  feature; this one must not block on it and must leave it a clean extension
  (same tier mechanism, same gate).
- The pool (builder node) is available for the end-to-end re-score runs; local
  stages run on the Pi.
- Feature 002's corpus cycle, reloc-blind score, flag recompute, and regression
  lock are the integration points and are live.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: After extraction, at least 150 static targets are reloc-aware with
  the round-trip gate passed (56 of 88 static files carry symbol-relative
  assembly; the count is reported exactly).
- **SC-002**: osCreateMesgQueue reaches **true score 0** through the ordinary
  corpus cycle — no hand intervention — and becomes promotion/lock eligible.
- **SC-003**: Of the 19 reloc_only_diff targets, every one whose candidate is
  genuinely identical modulo relocations upgrades to true score 0 automatically;
  the coverage report states the before/after flag counts.
- **SC-004**: All 12 locked functions re-verify at score 0 against their
  reloc-aware targets (`lock verify` green; zero lock regressions).
- **SC-005**: The smoke gate still passes end to end after the transition.
- **SC-006**: No report or ranking mixes evidence from two different objects of
  the same target (spot-checkable: every displayed cell's target-object identity
  equals the target's current identity).
- **SC-007**: Re-running extraction and submit with unchanged inputs schedules
  zero new compute and changes zero target objects.
