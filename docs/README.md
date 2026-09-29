# Documentation index

Start with [CLAUDE.md](../CLAUDE.md) for the project rules. Use the links below
for the task at hand; there is no need to load all project documentation.
Paths inside command examples are relative to the repository root.

## Workflows and operations

| Need | Read |
|---|---|
| Analyze and match a function | [Matching skill](../.claude/skills/match-function/SKILL.md) |
| Promote static C or splice game C into the ROM | [Promotion skill](../.claude/skills/promote-match/SKILL.md) |
| Regenerate extracted game context | [Context skill](../.claude/skills/refresh-game-context/SKILL.md) |
| IDO flag evidence and C89 constraints | [Compiler settings](COMPILER_SETTINGS.md) |
| Queue, farm, toolkit, corpus, scoring, and build commands | [Conveyor operations](../tools/conveyor/README.md) |
| Builder access, paths, services, and sync rules | [Build environment](BUILDING.md) |
| General decompilation techniques and MIPS reference | [Decompilation workflow](DECOMPILATION_WORKFLOW.md) |
| Project policies | [Constitution](../.specify/memory/constitution.md) |
| Wiki access and publishing | [Wiki guide](WIKI.md) |

The skills route through the current Conveyor paths. Older workflow examples and
research tables can predate those paths; use current CLI help and target evidence
when details disagree. Source presence, compilation, and byte matching are distinct.

## Source and target navigation

| Path | Purpose |
|---|---|
| [splat.us.yaml](../splat.us.yaml) | ROM extraction and static segment configuration |
| [symbol_addrs.us.txt](../symbol_addrs.us.txt) | Recorded addresses and names |
| [asm/us/](../asm/us/) | Static target assembly and generated blob assembly |
| [src/rom/](../src/rom/) | ROM-aligned static translation units |
| [src/blob/](../src/blob/) | Game-image sources and linker inputs |
| [include/game_types.h](../include/game_types.h) | Hand-maintained game context |
| [matched.lock.json](../matched.lock.json) | Static match locks |
| [blob_matched.lock.json](../blob_matched.lock.json) | Game-image splice locks |
| [reference/repos/rushtherock/](../reference/repos/rushtherock/) | Arcade source |
| [reference/lessons-learned.md](../reference/lessons-learned.md) | Research from other decomps |
| [tools/m2c.py](../tools/m2c.py), [tools/diff.py](../tools/diff.py) | Initial C generation and comparison |

`baserom.us.z64` is the local, untracked comparison ROM; [us.sha1](../us.sha1)
records its hash. `build/` contains generated output. Generated files should be
recreated through the pipeline rather than becoming a second source of truth.

## Research by topic

| Topic | References |
|---|---|
| Memory, ROM identity, compressed code, globals | [Memory map](memory_map.md), [global mappings](globals_mapping.md) |
| Arcade source ancestry and portability | [Arcade cross-reference](arcade_n64_xref.md), [function map](arcade_n64_function_map.md) |
| Bootstrap, game loop, and state transitions | [Game loop](game_loop.md), [state mapping](gamestate_mapping.md) |
| Cars and drivetrain | [Car physics](car_physics.md), [drivetrain](drivetrain.md), [tires](tire_physics.md) |
| AI and paths | [Drone system](ai_drone_system.md), [maxpath mapping](ai_maxpath_mapping.md) |
| Collision and track geometry | [Collision](collision_system.md), [track geometry](track_geometry.md) |
| Rendering, models, and camera | [Graphics pipeline](graphics_pipeline.md), [models](model_system.md), [camera mapping](camera_system_mapping.md) |
| HUD, menus, and controls | [HUD](hud_system.md), [menus](menu_system.md), [input](input_system.md) |
| Audio | [Audio system](audio_system.md), [SFX catalog](audio_sfx_catalog.md) |
| Race timing, replay, and modes | [Checkpoints](checkpoint_system.md), [replay mapping](replay_ghost_mapping.md), [battle mode](battle_mode_system.md), [stunts](stunt_system.md) |
| Save data and multiplayer | [Save system](save_system.md), [multiplayer](multiplayer_system.md) |
| Misidentified symbols and relocation differences | [Symbol misattribution](SYMBOL_MISATTRIBUTION.md), [relocation reconciliation](reloc_reconciliation.md) |

These are analysis documents, not proof that every named function matches. Early
hypotheses moved from `CLAUDE.md` are labeled in the relevant documents. Additional
subsystem notes and `*_mapping.md` files live alongside these entry points.

## History and planning

- [Project milestones](history/project-milestones.md): dated setup and pipeline outcomes.
- [Early inventory](history/early-inventory.md): December 2025–January 2026 function/source tables.
- [January progress report](PROGRESS_REPORT.md): historical phase estimates, not live coverage.
- [Feature specs](../specs/): design, tasks, research, quickstarts, and close-outs by feature.
- [Generated feature context](generated/feature-context.md): Spec Kit plan extracts, loaded only for planning.

For current status, use `make progress`, Conveyor reports, and recent commits.
Keep new facts in the appropriate topic document, new procedures in skills or
operating docs, and dated measurements in history with their source and scope.
