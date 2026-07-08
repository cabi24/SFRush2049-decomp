# Feature Specification: Promotion Splicing — matches become ROM

**Feature Branch**: `004-promotion-splicing`
**Created**: 2026-07-08
**Status**: Draft
**Input**: User description: "Promotion splicing: the function-to-TU layout map and ROM-hash-gated promotion flow that turns verified matches into compiled C linked into the real ROM. ROM-aligned TUs generated from the layout map (initially all GLOBAL_ASM, byte-identical by construction); per-function promotion replaces the pragma with verified C; every promotion gated by the full-ROM SHA-1. First consumers: the 12 locked functions and the 19 reloc_only_diff adoptions."

## Context

Every match the pipeline produces today terminates in `matched.lock.json` or a
`work/` artifact — none of it moves the project's actual metric, because the
matching ROM links **only** assembly and binary objects (`O_FILES` in the
Makefile excludes every C object; `make cc` compile-tests them and stops).
"Matching percentage" is therefore structurally zero no matter how many
functions verify at score 0. This feature is the bridge: a layout map from
function → original translation unit → ROM range, ROM-aligned TUs where each
function is either verified C or a byte-exact assembly passthrough, and a
promotion transaction whose only acceptance authority is the full-ROM SHA-1.
It is the endgame mechanism — everything the pipeline wins from now on flows
through it — and it is deliberately incremental: converting a TU with zero
promoted functions must reproduce the ROM bit-for-bit by construction.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - ROM-aligned TU scaffolding, hash-neutral by construction (Priority: P1)

The operator derives the layout map (which functions, in which order, at which
addresses, constitute each original TU — the splat file boundaries) and converts
one splat assembly segment into a generated C translation unit in which every
function is an assembly passthrough. The matching build (on the x86 builder,
since real-compiler TUs join the link) still produces a ROM with the correct
SHA-1. Nothing is promoted yet; the scaffolding proves the plumbing.

**Why this priority**: the walking skeleton. If an all-passthrough TU can't
reproduce the ROM byte-for-byte, nothing else in the feature is safe to build.

**Independent Test**: convert exactly one segment (the one holding strlen);
`make` + SHA-1 verify on the builder; convert it back; verify again.

**Acceptance Scenarios**:

1. **Given** the layout map, **When** one segment is converted to a generated
   all-passthrough TU, **Then** the full build produces the exact original
   SHA-1, and the linked object covers the identical ROM byte range the
   assembly object covered.
2. **Given** the generated TU, **Then** every function in it appears in layout
   order with its address, name, and provenance recorded, and regenerating the
   TU is deterministic (identical bytes).
3. **Given** a segment whose layout information is incomplete (unknown
   boundaries or missing symbols), **When** conversion is attempted, **Then**
   it is refused with a reason — never a silently wrong TU.

---

### User Story 2 - Function promotion, gated by the ROM hash (Priority: P2)

The operator promotes one verified function: its C body replaces the
passthrough in the ROM-aligned TU, the TU compiles with the pinned flags, the
full ROM links, and the SHA-1 gate decides. On pass, the promotion is committed
and the regression lock follows the function to its new home; on fail, the
splice is rolled back cleanly and the failure is recorded with the evidence.

**Why this priority**: the transaction everything else calls. Depends on US1.

**Independent Test**: promote strlen (locked, score-0, flags pinned) into its
converted TU; SHA-1 passes; deliberately promote a wrong body; gate refuses and
rolls back; repo left green both times.

**Acceptance Scenarios**:

1. **Given** a verified function and its converted TU, **When** promoted,
   **Then** the ROM SHA-1 matches, the function's C is live in the linked ROM,
   the lock entry now pins the body in the ROM-aligned TU, and the promotion is
   recorded (function, source, flags, evidence, commit).
2. **Given** a promotion whose build or hash fails, **Then** the working tree
   is restored to its pre-promotion state, the failure and its diff evidence
   are recorded, and the function's status reflects the rollback.
3. **Given** a function whose flags are not pinned for its TU, **When**
   promotion is attempted, **Then** it is refused with the flag-sweep path
   named as the remedy (never guessed flags).
4. **Given** several functions promoted into one TU over time, **Then** each
   promotion re-runs the same full gate; passthrough neighbors are never
   disturbed.

---

### User Story 3 - The backlog flows through, and progress becomes real (Priority: P3)

The 12 locked functions are promoted as the first batch (adopting the
reloc_only_diff targets follows the same per-function flow as they verify).
The progress metric reports linked-C coverage — functions and bytes of the
static code range actually compiled from C in the shipping ROM — alongside the
existing counts, and the conveyor's promotion job performs splice-promotions
through the builder node so future pipeline wins land without a human driving
each one.

**Why this priority**: the payoff and the pipeline closure; pure consumer of
US1+US2.

**Independent Test**: after the batch, `make progress` (and the pipeline
report) show nonzero linked-C bytes; ROM SHA-1 still exact; every promoted
function's lock re-verifies.

**Acceptance Scenarios**:

1. **Given** the 12 locked functions, **When** the batch promotion runs,
   **Then** all 12 are live C in a SHA-1-exact ROM, with locks migrated and
   green.
2. **Given** a conveyor-verified future match, **When** the promotion job runs
   on the builder, **Then** the same transaction executes end to end
   (splice → build → gate → commit/rollback) without manual steps.
3. **Given** any state of promotions, **Then** the progress report's linked-C
   figures are derived from the layout map and the live TU contents — never a
   hand-maintained count.

---

### Edge Cases

- A function's assembly region contains data the compiler would place
  differently (jump tables, literal pools): the passthrough must carry it
  byte-exactly or the TU conversion must be refused for that segment.
- Two adjacent functions where one is promoted and its neighbor's passthrough
  begins mid-alignment: link order and section layout must keep every
  unpromoted byte where the ROM has it.
- A locked function's src/ reference copy and its promoted ROM-TU copy drift
  apart: the lock follows promotion (single source of truth moves); the
  reference copy is marked superseded or removed as part of the promotion.
- The pinned toolchain differs per TU (mixed -O1/-O2 confirmed): flags come
  from the flag registry per TU; a TU with unpinned flags refuses promotion.
- Promotion on a machine that cannot run the real compiler (the Pi): refused
  with a clear message naming the builder path; never a silent gcc fallback
  into a "matching" build.
- A splat re-extraction regenerates the linker script: converted segments must
  survive regeneration (the conversion lives in the extraction configuration,
  not in hand-edits to generated files).
- Rollback after a partial failure (build passed, hash failed, or interrupted
  mid-transaction): the transaction must be atomic from the repo's point of
  view — either fully committed or fully absent.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The system MUST derive a layout map for the static code range:
  for each original-TU segment, the ordered list of functions with name,
  address, size, and source segment — derived from the existing extraction
  artifacts (segment boundaries, per-function regions, symbol table), never
  hand-maintained.
- **FR-002**: The system MUST generate, from the layout map, a ROM-aligned TU
  per converted segment in which every function is initially a byte-exact
  assembly passthrough; generation MUST be deterministic and refuse incomplete
  layout information.
- **FR-003**: A converted segment with zero promotions MUST produce a ROM with
  the original SHA-1 (hash-neutral scaffolding).
- **FR-004**: Promotion MUST be a per-function transaction: splice verified C
  over the passthrough, compile the TU with its pinned flags, link the full
  ROM, verify SHA-1; commit on pass, restore the pre-promotion state on any
  failure. Partial states MUST NOT survive.
- **FR-005**: Promotion MUST require prior verification evidence (score-0 or
  an explicit human override recorded with rationale) and pinned flags for the
  TU; absent either, it MUST refuse with the remedy named.
- **FR-006**: The regression lock MUST follow promotion: the promoted body in
  the ROM-aligned TU becomes the locked artifact; stale lock entries pointing
  at superseded reference copies MUST be migrated in the same transaction.
- **FR-007**: Every promotion MUST record provenance: function, TU, source of
  the body (origin repo/path/commit or in-repo path), flags, evidence scores,
  and the resulting commit — queryable later.
- **FR-008**: The matching build with converted TUs MUST run where the real
  compiler runs (the x86 builder); attempting it elsewhere MUST fail loudly.
  The non-matching build path MUST remain buildable as today.
- **FR-009**: Progress reporting MUST include linked-C coverage (promoted
  functions and bytes over the static code range) derived from the layout map
  and live TU contents.
- **FR-010**: The conveyor promotion job MUST execute the same transaction on
  the builder node, so pipeline-verified matches promote without manual steps;
  its gate is identical (full-ROM SHA-1), never weaker.
- **FR-011**: The compressed game-code range (dynamic population) is OUT of
  scope; the layout map MUST record it as unpromotable-for-now rather than
  silently omitting it.

### Key Entities

- **Layout map**: derived, versioned description of the static ROM's TU
  structure — segments, ordered functions, addresses, sizes; single source of
  truth for generation and progress.
- **ROM-aligned TU**: generated C translation unit mirroring one original
  segment; each slot either passthrough or promoted C with provenance header.
- **Promotion transaction**: the atomic splice→build→gate→commit/rollback
  unit, with its record.
- **Linked-C coverage**: the progress figures (functions, bytes) computed from
  the map + TU state.

## Assumptions

- The splat configuration and its generated linker script remain the build's
  source of truth; segment conversion is expressed there (an `asm` subsegment
  becomes a `c` subsegment) so re-extraction preserves it.
- The assembly-passthrough mechanism is the standard decomp-community
  processor (asm-processor) driving the pinned IDO; it is not yet vendored —
  vendoring it is part of this feature's setup.
- Matching builds with linked C run on watchman (IDO does not run on the Pi);
  the conveyor builder node is the natural home for promotion.
- The 003 region index and `symbol_addrs.us.txt` provide per-function
  addresses/sizes/names; splat.us.yaml provides segment boundaries.
- The 12 locked functions (with pinned flags) are the acceptance batch; the
  19 reloc_only_diff targets join as they are adopted and verified.
- The compressed game-code blob (752 dynamic functions) is a separate future
  feature (blob rebuild); nothing here may depend on it.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: One segment converted to an all-passthrough TU builds a ROM with
  the original SHA-1, before any promotion (hash-neutrality proven).
- **SC-002**: strlen is promoted end to end — its C body is live in a
  SHA-1-exact ROM — via the transaction, not hand edits.
- **SC-003**: A deliberately wrong promotion is refused by the gate and leaves
  the repo in its pre-promotion state (rollback proven).
- **SC-004**: All 12 locked functions are promoted with the ROM SHA-1 exact
  and all locks green at their new homes.
- **SC-005**: `make progress` (and the pipeline report) show linked-C coverage
  > 0 functions / > 0 bytes, derived automatically; the numbers change only
  when promotions land.
- **SC-006**: A conveyor-driven promotion executes the identical transaction
  on the builder with no manual intervention.
- **SC-007**: Every promotion (and refusal) has a queryable record with full
  provenance.
