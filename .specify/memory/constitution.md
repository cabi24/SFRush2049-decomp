<!--
Sync Impact Report
==================
Version change: 0.0.0 → 1.0.0
Added sections: All (initial constitution)
Templates requiring updates: ✅ All templates initialized with project
Follow-up TODOs: None
-->

# Rush 2049 N64 Decompilation Constitution

## Core Principles

### I. Matching First

All decompiled code MUST compile to byte-identical assembly when using the original IDO compiler. Non-matching implementations are acceptable only as temporary scaffolding during development, clearly marked with `#ifdef NON_MATCHING`. The ultimate goal is a ROM that matches the original SHA-1 hash exactly.

**Rationale**: Byte-matching ensures we have accurately reverse-engineered the original developers' intent and produces a buildable, verifiable artifact that proves correctness.

### II. Arcade Source as Rosetta Stone

The Rush The Rock arcade source code at `reference/repos/rushtherock/` is our primary reference for understanding game logic, function purposes, and data structures. When decompiling N64 functions, ALWAYS search for equivalent arcade code first. Function names, variable names, and comments should align with arcade source conventions where applicable.

**Rationale**: Having original source code is an extraordinary advantage. It eliminates guesswork about algorithm intent and provides naming conventions that make the codebase maintainable.

### III. Progressive Disclosure

Start with non-matching functional code (`NON_MATCHING=1`), then iterate toward matching. Use `GLOBAL_ASM` for stubborn functions rather than blocking progress. Track matching percentage continuously. A working game with 50% matching is more valuable than 10% perfect matching with nothing runnable.

**Rationale**: Maintaining a buildable, testable ROM at all times enables rapid iteration and keeps contributors motivated with visible progress.

### IV. Platform-Aware Portability

Recognize and document the three-tier architecture from arcade source:
- **Portable tier**: Physics, AI, game logic (cars.c, drones.c, checkpoint.c) — preserve unchanged
- **Adaptation tier**: Math, memory, data formats — adapt for N64 constraints
- **Rewrite tier**: Rendering, audio, I/O — N64-specific implementations

Each decompiled module MUST be categorized and documented accordingly.

**Rationale**: Understanding what's portable vs platform-specific prevents wasted effort trying to match code that was rewritten for N64.

### V. Documentation as Artifact

Every matched function MUST include:
- Arcade source equivalent (file:function if known)
- Brief description of purpose
- Any compiler flags or quirks required for matching
- Known differences from arcade version

This documentation lives in code comments and is as important as the code itself.

**Rationale**: Future contributors need context. A matched function with no explanation is a maintenance burden.

## Technical Standards

### Build System Requirements
- Multi-version support (US, EU, JP if applicable) from day one
- `make VERSION=us` produces matching ROM
- `make VERSION=us NON_MATCHING=1` produces functional ROM
- `make progress` reports decompilation percentage
- `make test` verifies SHA-1 match
- CI/CD pipeline validates all PRs

### Code Organization
- Mirror MK64/SM64 directory structure conventions
- `src/racing/` for vehicle physics, AI, paths
- `src/game/` for state machine, menus, save
- `courses/` for per-track data
- `include/` for shared headers
- `asm/` for unmatched assembly stubs

### Matching Workflow
1. Identify target function in N64 binary
2. Search arcade source for equivalent
3. Write C implementation based on arcade logic
4. Compile with `NON_MATCHING=1` to verify functionality
5. Use `diff.py` to compare assembly
6. Iterate until matching or mark as `GLOBAL_ASM`
7. Document and commit

### Tool Requirements
- IDO compiler (via ido-static-recomp)
- asm-processor for GLOBAL_ASM support
- splat for ROM extraction
- asm-differ for matching verification
- progress.py for tracking

## Quality Gates

### For Matching PRs
- Function compiles to identical assembly
- Arcade source equivalent documented
- No regression in overall matching percentage
- Passes CI build and test

### For Non-Matching PRs
- Clearly marked with `#ifdef NON_MATCHING`
- Game remains playable
- Documents why matching is difficult
- Creates tracking issue for future matching

### For Infrastructure PRs
- Build system changes tested on all versions
- Tool changes documented
- No breaking changes to existing workflow

## Governance

This constitution supersedes all other project practices. Amendments require:
1. Written proposal with rationale
2. Review period (minimum 1 week)
3. Consensus among active contributors
4. Version increment and changelog entry

All pull requests and code reviews MUST verify compliance with these principles. Complexity or deviation from standards MUST be justified in PR description.

Use `CLAUDE.md` and agent definitions in `.claude/agents/` for AI-assisted development guidance.

**Version**: 1.0.0 | **Ratified**: 2025-12-06 | **Last Amended**: 2025-12-06
