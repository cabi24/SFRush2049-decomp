# Project milestones

Extracted and organized from `CLAUDE.md` on 2026-09-28. Dates below are the
dates recorded in those notes. Counts, blockers, TODOs, and completion claims
describe those sessions, not the current checkout. Some early reports disagree;
they have not been converted into new coverage claims.

Use `make progress` and Conveyor reports for current evidence. Procedures live in
the [skills](../../CLAUDE.md#read-when-needed) and [operations guide](../../tools/conveyor/README.md).

## December 2025: extraction and initial C scaffolding

- Spec Kit planning, reference clones (Rush The Rock, SM64, MK64, Perfect Dark,
  Banjo-Kazooie), lessons learned, agent definitions, and project structure were
  established. The US 12 MB ROM was verified and converted from V64 to Z64.
- Splat initially found 85 file boundaries; the notes later counted 88 assembly
  files and about 18K assembly lines. Hardware-register definitions and function
  comparison tooling were added. The build was reported as matching, but see the
  July 11 discovery about the verification gate below.
- December 7 analysis located compressed game code at ROM 0xB0CB10, inflating to
  0x80086A50. The early scan counted 767 call targets and 752 prologues. The
  `game_loop` lead was `func_800FD464`; OS bootstrap `game_init` was distinguished
  from arcade gameplay initialization.
- The December 27 library additions and original function/source tables are
  preserved in [early-inventory.md](early-inventory.md). The original “Complete”
  labels included stubs; the 120 C / 88 assembly file ratio (“136% coverage”)
  measured file scaffolding, not matched bytes.
- By December 29, IDO 5.3 was set up on the original `watchman` x86 builder
  (OptiPlex 3080, reported as 20 cores and 31 GB RAM). IDO 7.1 was also available.
  The notes reported 102/111 C files compiling, with about nine files still
  requiring C89 work. Compiler binaries were under
  `/home/cburnes/projects/rush2049-decomp/tools/ido-static-recomp/build/out/`
  and `tools/ido7.1/` on that retired builder.
- C89 cleanup removed GNU inline assembly from the IDO path, moved declarations
  to function starts, replaced compound literals, resolved typedef conflicts,
  and replaced system math headers with local declarations. The maintained
  guidance is in [compiler settings](../COMPILER_SETTINGS.md#c89-source-compatibility).

The old “Not Started” and “Next Steps” lists still asked for IDO setup and a
progress script after they existed. Other old TODOs asked to analyze the extracted
blob, find graphics/arcade equivalents, identify game structures, and try a simple
game-function decompilation. These are superseded session notes, not today's queue.
The historical bypass-approvals command example was not carried into active guidance.

## January 2026: compiler evidence and progress snapshots

- January 2 matching experiments established mixed optimization levels:
  `strlen` and `guMtxIdentF` matched at `-O2`, `osCreateMesgQueue` at `-O1`, with
  `-g0 -mips2 -G 0 -non_shared`. See [compiler settings](../COMPILER_SETTINGS.md).
- January 5 `CLAUDE.md` reported 228 identified static functions, 752 extracted
  game functions, 3,406 symbol entries, 120 C files (~98,517 lines), and 1,319
  work-directory functions (1,310 WIP, zero TODO). It also reported all target
  assembly extracted and no remaining `func_80` call-site names.
- Its phase estimate was ~65% overall, with setup/source creation at 100%,
  decompilation at 99%, matching at 5%, and documentation at 20%. The separate
  [January progress report](../PROGRESS_REPORT.md) contains different estimates.
  Neither is a live matched-ROM metric; source creation and naming were being
  counted separately from byte matching.

## July 2: distributed matching pipeline (001)

Conveyor introduced the Pi HTTP/SQLite coordinator, content-addressed job/toolkit
bundles, pull-based x86 nodes, and compile-score, flag-sweep, permuter-search, and
verify-promote jobs. The snapshot recorded 45/48 tasks done, 1,131 targets with
object blobs, 2,526 arcade candidates, and 34 clone clusters. T019 (real IDO smoke)
and T048 (hardware walkthrough/24-hour soak) awaited the original builder.

Source: [001 design and quickstart](../../specs/001-matching-pipeline/).

## July 8: canonical corpus candidates (002)

The ultralib corpus supplied name-paired candidates for library targets without
arcade ancestry. Ingestion recorded 702 candidates at `e24c8367`. The first run
(toolkit `b613fc5d…`) paired 85 targets, scored 72, found 12 true-zero matches,
and flagged 19 `reloc_only_diff` targets. `osCreateMesgQueue` had true score 20
and relocation-blind score zero; `strlen` and `guMtxIdentF` were true zero.

The ≥80 scored-target criterion was missed because 13 candidates depended on
file-local static helpers stripped by reduced-TU extraction (scheduler, sprintf,
I/O-manager clusters). Keeping static callees or improving extraction was a
follow-up. Relocation-blind matches were evidence only, never promoted as true zero.

Source: [002 design and results](../../specs/002-corpus-candidates/).

## July 8: relocation-aware static targets (003)

Targets gained real assembly relocations behind a masked-word round-trip gate;
object changes superseded scores with target-object SHA attribution. The first
live run (toolkit `1e21f523…`) produced 178 relocation-aware targets, deterministic
re-extraction, and zero attribution mismatches.

Reported blockers were symbol-name differences (`D_8002C3D0` versus
`__osThreadTail`) preventing 18 relocation-only upgrades, and regressions in four
MMIO locks (`osDpGetCounters`, `__osSpSetPc`, `__osSpDeviceBusy`, `__osSpSetStatus`)
because splat symbolized addresses that IDO emitted literally. These are dated
findings; consult current evidence before treating them as open work.

Source: [003 quickstart and results](../../specs/003-reloc-aware-targets/quickstart.md).

## July 11: static promotion and a meaningful ROM gate (004)

The first promotion run linked 11/12 locked functions into a SHA-1-exact ROM:
eight converted segments, 11/230 static functions, 920/61,440 static bytes (~1%).
Splat re-extraction was made idempotent to ROM bytes using symbol sanitization and
splat 0.41; assembly text itself was not required to match across splat versions.

The SC-003 rollback drill discovered that verification had been vacuous since
December: it hashed `baserom` instead of the built ROM, swallowed failure with
`|| echo`, and could skip rebuilds because rsync preserved timestamps. The notes
recorded fixes in `17c70f5`. The enduring lesson is to test the built artifact and
prove that an intentional byte mismatch fails the gate.

Follow-ups then were T012 (builder verify-promote integration) and a missing
derived region for `__osAiDeviceBusy` at 0x8000FB60; T008 was recorded as done.
Nineteen relocation-only candidates were expected to use the same promotion path
once actually verified.

Source: [004 design and results](../../specs/004-promotion-splicing/).

## July 17: extracted game context (005)

The context bootstrap replaced trusted `info.txt` sizes with scanner-derived
function extents from `build/game_code.bin`, exposing nested/suffix conflicts.
Extracted targets initially remained evidence-only behind the static promotion
firewall. Features 008/009 later added a separate image/cartridge path.

Source: [005 design and operations](../../specs/005-game-context-bootstrap/).

## July 28: replacement builder

`CODEX.md` records that `watchman` failed and was replaced by `watchman2`.
Uncommitted work had already been merged through `watchman-master`; the x86 IDO
toolchain was recovered from the Pi's pinned toolkit blob. Current documented
paths and service controls are in [builder operations](../BUILDING.md#watchman2-builder).

## Prototype flywheel (006; no shipment date in the original section)

Generated declarations enabled complete extracted-population histograms. Scoped
runs became separate probes; the farm required `run.population_complete=true`.
Compiled, unscored extracted seeds entered the queue at priority 60, below static
work. Operational detail: [006 guide](../../tools/conveyor/README.md#prototype-layer-and-extracted-flywheel-006).

## September 10: population closure (007)

Closure discovered missing in-blob `j`/`jal` targets to a fixpoint, registering
`func_<ADDR8>` names. The notes identified 178 old inventory entries as suffixes,
now marked `extent_conflict:<func_id>`. Generated data symbols and externs were
merged beneath hand-authored context, with the refresh order closure → datasyms →
protos → histogram.

Source: [007 actuals](../../specs/007-population-closure/quickstart.md).

## September 24: game code reaches the cartridge (008/009)

Feature 008 linked the 647,072-byte image from sources behind a byte-identity gate.
Feature 009 reproduced the 326,180-byte raw DEFLATE stream at ROM 0xB0CB10 using
zlib 1.0.4 (level 9, windowBits −15, memLevel 8, default strategy), then composed
it into the ROM data segment. Other tested zlib versions produced streams 140
bytes too long; the old wording “140 bytes long” omitted “too.”

Game splices became cartridge coverage under the full-ROM SHA-1 gate. The build
requires `build/blob/game_code.deflate` produced from the linked image, with no
fallback to the original compressed bytes. Static and game coverage retain
separate denominators. The vendored deflate code is a build-time reproduction
tool, not a general-purpose service for untrusted data.

Sources: [compressor research](../../specs/008-blob-image-rebuild/research/compressor-identified.md),
[009 close-out](../../specs/009-blob-rom-splice/CLOSEOUT.md).

## Where the former root material now lives

| Former content | Maintained location |
|---|---|
| Function/source tables and library additions | [Early inventory](early-inventory.md) |
| Arcade leads, confidence, source navigation | [Arcade cross-reference](../arcade_n64_xref.md#early-matching-hypotheses-2025-12-07) |
| Bootstrap distinction and early call hypotheses | [Game loop](../game_loop.md#early-call-identification-notes-2025-12-07) |
| ROM identity, extraction notes, size/global hypotheses | [Memory map](../memory_map.md#rom-identity-and-compressed-image-build) |
| Compiler flags and C89 fixes | [Compiler settings](../COMPILER_SETTINGS.md) |
| Pipeline commands, gates, and operational details | [Conveyor guide](../../tools/conveyor/README.md) and linked skills |
| Generated technology/recent-change entries | [Feature context](../generated/feature-context.md) |
