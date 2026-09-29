---
name: match-function
description: Analyze and match Rush 2049 N64 functions against target assembly using arcade or library source and IDO scoring.
---

# Match a function

Run commands from the repository root. Read [compiler settings](../../../docs/COMPILER_SETTINGS.md)
for flag evidence and C89 constraints, and the relevant section of
[Conveyor operations](../../../tools/conveyor/README.md) for scoring commands.

1. Resolve the target address and population (`static` or `extracted`). For static
   targets, inspect the splat assembly region. For extracted targets, use the
   scanner/closure-derived extent; `work/**/info.txt` can describe a function suffix.
   Refresh missing or stale context with the [context skill](../refresh-game-context/SKILL.md).
2. Search `reference/repos/rushtherock/` for game logic and naming clues. Use the
   [arcade cross-reference](../../../docs/arcade_n64_xref.md) as leads, retaining
   confidence labels. For library targets without arcade ancestry, use the
   canonical corpus path described under “Corpus candidates” in the operations guide.
3. Generate initial C if useful (`tools/m2c.py` for static assembly; Conveyor
   `autodecomp` for extracted targets), then refine it against assembly. Keep hand
   context in `include/game_types.h` and `disasm.GAME_SYMBOLS`. Treat `M2C_ERROR`
   output as partial even if later cleanup makes it compile.
4. Compile/score with the target's recorded flags or the confirmed `-O1`/`-O2`
   candidates. Do not assume all game code uses one optimization level. Retain
   provenance: target object, candidate source, toolkit, flagset, and true score.
5. Distinguish a compiled seed, `reloc_only_diff`, and a true-zero match.
   Relocation-blind zero does not authorize locking or promotion. When the target
   object changes, old scores are superseded and queued bundles may still contain
   the old object; follow the guide's supersession procedure.
6. Record purpose, arcade equivalent if known, N64 differences, and required flags
   with the source. Use the [promotion skill](../promote-match/SKILL.md) when the
   requested task includes putting the verified C into the cartridge.

For instruction patterns and general analysis techniques, consult
[DECOMPILATION_WORKFLOW.md](../../../docs/DECOMPILATION_WORKFLOW.md) as needed.
