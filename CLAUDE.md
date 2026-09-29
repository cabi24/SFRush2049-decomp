# Rush 2049 N64 decompilation

Goal: matching C source that builds a byte-identical San Francisco Rush 2049
US ROM. Arcade source at `reference/repos/rushtherock/` is the primary reference
for game logic and naming; N64 assembly is the matching target.

This file contains shared working rules and routing. Read linked documents only
when relevant to the task; historical notes are not current status or instructions.

## Essential context

- The cartridge contains static boot/library code and a compressed game-code
  image. Conveyor (`tools/conveyor/`) handles matching; static promotion and
  game-blob splicing have separate paths.
- The Pi coordinates jobs; the documented x86 builder is `watchman2`. IDO runs
  there because its recompiled binaries require 4 KB pages (the Pi has 16 KB).
  See [builder operations](docs/BUILDING.md#watchman2-builder) before remote work.
- C must be IDO/C89 compatible. Python tooling targets 3.9+; the coordinator and
  node agent use the standard library. Compiler flags vary by function/module:
  use [confirmed settings](docs/COMPILER_SETTINGS.md) and recorded evidence.

## Matching and build rules

- A relocation-blind score of zero is a lead, not a verified match. Require true
  score zero for matching evidence and the built ROM's SHA-1 for cartridge claims.
- Static `lock`/`promote` commands reject extracted game targets. Game functions
  use `blob_splice` → `blob_rom`, with image byte identity and full-ROM hash gates.
- The ROM build requires `build/blob/game_code.deflate`, produced from the linked
  image; do not substitute the extracted original blob or bypass the build gates.
- Report static and game coverage separately; never merge their denominators.
  Source files, stubs, identified symbols, and compiled seeds are not ROM coverage.
- Extracted extents and population come from scanning/closure. `work/**/info.txt`
  names and sizes are historical labels, not authoritative function boundaries.
- Regenerate derived layouts, linker scripts, assembly, data symbols, and
  prototypes through their tools. Hand-authored game context belongs in
  `include/game_types.h` and `tools/conveyor/pipeline/disasm.py` (`GAME_SYMBOLS`).
- Preserve locked matches. Use the appropriate promotion/splicing workflow when
  changing linked C; see the [project constitution](.specify/memory/constitution.md)
  for matching documentation and review requirements.

## Starting a session

Inspect `git status` and recent commits, then use reports appropriate to the task:

```bash
make progress                              # derived cartridge coverage
python3 -m tools.conveyor.cli status        # coordinator/queue status
python3 -m tools.conveyor.cli attention     # reported blockers
```

For builds and verification, use the [promotion skill](.claude/skills/promote-match/SKILL.md).
`make test` verifies the built ROM and may build prerequisites; it is not a
read-only status command. Old phase percentages and next-session TODOs are archived.

## Read when needed

| Task | Entry point |
|---|---|
| Match or investigate a function | [Matching skill](.claude/skills/match-function/SKILL.md) |
| Put verified C into the cartridge | [Promotion skill](.claude/skills/promote-match/SKILL.md) |
| Refresh game targets, symbols, prototypes, or histograms | [Context skill](.claude/skills/refresh-game-context/SKILL.md) |
| Operate Conveyor, toolkit, farm, or corpus | [Operations guide](tools/conveyor/README.md) |
| Reach the builder or manage services | [Build environment](docs/BUILDING.md) |
| Find memory, arcade, or subsystem research | [Documentation index](docs/README.md) |
| Understand previous decisions and results | [Milestones](docs/history/project-milestones.md) |
| Review feature design and generated plan context | [Specs](specs/) · [Generated notes](docs/generated/feature-context.md) |
| Use specialized agent definitions | [Agent directory](.claude/agents/) |
| Read/write the project wiki | [Wiki access](docs/WIKI.md) |
| Joining from the cloud (repo only, no ROM/LAN) | [Cloud handoff](CloudHandoff.md) |

## Maintaining these instructions

Keep this file short and stable. Put procedures in task skills and canonical
operating docs, research in the existing topic docs, and dated outcomes in history.
Keep hypotheses labeled with provenance; check current source/evidence before
treating them as facts. Link new material here or in the documentation index.
Spec Kit's `update-agent-context.sh claude` writes [generated notes](docs/generated/feature-context.md),
not this file. Update the wiki's `rush2049:status` at project milestones using
the wiki guide.
