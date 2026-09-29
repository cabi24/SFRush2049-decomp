# Feature plan context

On-demand Spec Kit notes, moved from `CLAUDE.md` on 2026-09-28 and deduplicated.
The original per-feature extracts included truncated lines; the complete source
plans remain under [specs/](../../specs/). These notes describe planned technology,
not live deployment status or a replacement for the operating guide.

`.specify/scripts/bash/update-agent-context.sh claude` updates this file.
Keep shared working rules in [CLAUDE.md](../../CLAUDE.md), procedures in skills
and [Conveyor operations](../../tools/conveyor/README.md), and dated outcomes in
[project history](../history/project-milestones.md).

## Active Technologies

- Python 3.9+; standard-library coordinator and node agent (`http.server`,
  `sqlite3`, `tarfile`, `hashlib`, `json`, `urllib`). Compute toolkits include IDO
  via ido-static-recomp, MIPS binutils, and decomp-permuter; arcade extraction
  uses pycparser. [001 plan](../../specs/001-matching-pipeline/plan.md).
- SQLite WAL at `~/.conveyor/conveyor.db` with a single writer and a SHA-256
  content-addressed blob store for bundles, toolkits, and results. Shared by
  features 001–007; [002 plan](../../specs/002-corpus-candidates/plan.md).
- Relocation-aware target assembly uses `mips-linux-gnu-as` and `objdump`.
  [003 plan](../../specs/003-reloc-aware-targets/plan.md).
- C89 translation units, GNU Make, splat, generated layout maps in `build/`, and
  checked-in segment conversion state. [004 plan](../../specs/004-promotion-splicing/plan.md).
- Python extraction, disassembly, and compile probes use existing MIPS binutils
  and generated/hand-authored game context. [005 plan](../../specs/005-game-context-bootstrap/plan.md).
- Prototype generation and histogram/flywheel work reuse the existing pipeline
  modules and SQLite state without a 006 schema migration.
  [006 plan](../../specs/006-prototype-flywheel/plan.md).

## Recent Changes

- 001-matching-pipeline: Introduced the distributed Python coordinator/node
  stack, IDO/binutils/permuter toolkits, SQLite state, and arcade candidate extraction.

<!-- MANUAL ADDITIONS START -->
The entry above is the historical generated change entry migrated from the
root file. New plan runs may add entries here; this is not a project changelog.
<!-- MANUAL ADDITIONS END -->
