# Research: Promotion Splicing

Grounded 2026-07-08 against the live repo. The load-bearing facts were checked.

## D1. The ROM currently links zero C (verified)

`Makefile:123`: `O_FILES := $(ASM_O_FILES) $(BIN_O_FILES)` with the comment
"C files compiled but not linked yet". `make cc` compile-tests 120 C files;
none reach the link. Consequence: this feature is the only path from any
match to ROM percentage.

## D2. Mechanism: splat `c` subsegments + asm-processor (the community standard)

- **Decision**: convert `[0xXXXX, asm]` subsegments in `splat.us.yaml` to
  `[0xXXXX, c, rom/<name>]` one at a time. splat then (a) exports per-function
  asm to `asm/us/nonmatchings/<name>/<func>.s`, (b) expects
  `build/us/src/rom/<name>.o` in the generated linker script. The generated TU
  uses `#pragma GLOBAL_ASM("asm/us/nonmatchings/<name>/<func>.s")` per
  unpromoted function; asm-processor + pinned IDO compile it so the object is
  byte-identical to pure assembly for passthroughs.
- **Rationale**: this is the architecture every shipped N64 decomp uses
  (sm64/mk64/perfect_dark, all in reference/repos to crib build rules from);
  the constitution's Tool Requirements and Matching Workflow already name
  asm-processor and GLOBAL_ASM. Inventing a linker-level splice instead was
  rejected: section-placement games are fragile and unreviewable.
- **To vendor**: asm-processor (pin a commit; it is a small two-file tool).
  Crib the Makefile integration pattern from reference/repos/sm64 or mk64
  (asm-processor wraps the compiler invocation per TU).

## D3. Layout map derivation (all inputs already exist)

- **Decision**: `pipeline/layout.py` derives, per splat code subsegment: the
  ordered function list with vaddr, size, and canonical name — from
  splat.us.yaml (segment boundaries), 003's `index_asm_regions()` (per-function
  regions, ROM words), and `symbol_addrs.us.txt` (canonical names). Emit as a
  deterministic JSON artifact (`build/layout.us.json`) plus a `layout report`
  command. Refuse (per segment) when: a region is missing, regions don't tile
  the segment contiguously, or a function lacks a canonical name — refusal
  reasons mirror 003's gate style (recorded, never silently wrong).
- **Rationale**: FR-001 "never hand-maintained" — every input is already
  derived and checked in.
- **Risk to verify at T-early**: whether the 0x8800 segment's regions tile
  contiguously (splat sometimes leaves inter-function padding/data in code
  segments). The walking skeleton settles this before generalization.

## D4. Data in code segments (the known hard edge)

- **Decision**: V1 converts only segments whose entire range tiles into known
  function regions (D3 refusal otherwise). Late-rodata/jumptables inside
  functions ride inside GLOBAL_ASM per-function files (asm-processor handles
  late_rodata in the standard pattern) — but any segment where splat emitted
  non-function content stays unconverted in V1.
- **Rationale**: hash-neutrality by construction beats coverage; unconvertible
  segments are a report line, not a blocker.

## D5. Per-TU flags

- **Decision**: `flag_registry` is the authority (pins exist for string.c=-O2,
  os_message.c=-O1, gu.c=-O2 from 001 seeding; keyed today by reference-TU
  path). The layout map records each ROM TU's flagset by joining its member
  functions' verified flags; a TU whose members verified under DIFFERENT
  flagsets, or with no pin, refuses promotion (FR-005) — remedy: per-TU flag
  sweep. Migration: registry keys gain the ROM-TU path when the first function
  promotes there.
- **Note for the implementer**: __osSpSetStatus verified at -O1 while
  __osSpSetPc verified at -O2 in the same reference file (os_sp.c) — but they
  came from the ROM's OWN TUs, which are the segments; the map join is per ROM
  segment, and the mixed evidence must be re-checked against the segment's
  single real flagset during the batch (expect one of the two flag records to
  be a coincidental dual-match — tiny functions often match under both).

## D6. The transaction (promote.py)

- **Decision**: library + CLI. Steps: (1) preconditions — evidence (lock entry
  or score-0 record, or `--override` with recorded rationale), pinned flags,
  clean TU in git; (2) splice — replace the pragma with the body + provenance
  header; (3) build — full `make` with SHA-1 verify, executed on the machine
  itself if IDO present else via `ssh watchman make -C ...` (refuse with a
  clear message if neither — FR-008); (4) on pass: `lock add`-migrate (pin the
  body at its ROM-TU path, drop the superseded reference-file entry), write
  `promotion_record`, `git commit` the TU + lockfile; (5) on any failure:
  `git checkout` the TU (it was clean at step 1), record the refusal. Atomic
  because the only mutated files are the TU + lockfile and both are committed
  or restored together.
- **Conveyor integration (FR-010)**: `verify_promote` job calls the same
  library on the builder (it already runs `make test` and commits — it gains
  real splicing and loses the `work/matched.c` V1 stopgap).

## D7. Matching-build guard

- **Decision**: Makefile: when any converted `c` subsegment exists and
  `COMPILER=ido` tools are absent, `make` fails with "matching build requires
  IDO (run on the builder)" instead of linking a wrong-compiler object into a
  "matching" ROM. `NON_MATCHING=1` keeps building anywhere as today.

## D8. Progress metric

- **Decision**: `layout coverage` computes promoted functions/bytes by parsing
  the generated TUs (pragma vs C body per slot) against the map;
  `make progress` shells to it; the conveyor report gains the same line.
  Derived, never counted by hand (FR-009/SC-005).

## D9. First conversions

- **Decision**: acceptance batch = the segments holding the 12 locked
  functions: 0x8800 (strlen, strchr, memcpy — plus memchr/memset/bzero slots
  as passthroughs), the guMtx segment (0x9DE0), os_sp/os_ai/os_dp/os_phys/
  checksum segments per the layout map. Walking skeleton = 0x8800 alone.
