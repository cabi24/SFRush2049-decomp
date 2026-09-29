---
name: refresh-game-context
description: Refresh Rush 2049 extracted game targets, generated symbols and prototypes, and complete population histograms for matching work.
---

# Refresh extracted game context

Use this for game-blob context and population work. Run from the repository root;
the commands update derived files and Conveyor state. Detailed behavior lives in
[population closure](../../../tools/conveyor/README.md#population-closure-and-generated-data-symbols-007)
and [prototype/histogram operations](../../../tools/conveyor/README.md#prototype-layer-and-extracted-flywheel-006).

When initial targets need extracting, use `pipeline.matrix extract` as described
under feature 005 in the operations guide. Then refresh in dependency order:

```bash
python3 -m tools.conveyor.pipeline.closure run
python3 -m tools.conveyor.pipeline.datasyms generate
python3 -m tools.conveyor.pipeline.protos generate
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted --limit 0
```

- Use scanner/closure-derived extents. Inventory names and `info.txt` sizes are
  historical; `extent_conflict:<id>` can identify an old suffix inside a real function.
- Check `build/closure_report.json` for failures or `cap_hit`; do not describe a
  capped run as population closure. Repeated closure with unchanged inputs should
  register zero new targets; prototype regeneration should be byte-stable.
- Hand judgments belong in `include/game_types.h` and `disasm.GAME_SYMBOLS`.
  Regenerate `build/m2c_datasyms.json`, `build/m2c_protos.h`, and cached assembly;
  hand declarations and symbols win on collision.
- Only an unfiltered, untruncated run is the population instrument:
  `build/m2c_histogram.{json,md}`, with `run.population_complete=true`. Scoped
  `--targets` or limited runs are probes and must not replace that artifact.
- Preserve the six outcome buckets and `M2C_ERROR` honesty rule. A partial
  decompilation is not a compiled seed merely because cleanup made it compile.
- The farm submits eligible compiled, unscored extracted seeds at priority 60,
  below static work. Use explicit re-scoring when needed; do not overwrite history.

If the task also requires relocation-aware target regeneration, read
[the extracted-target procedure](../../../tools/conveyor/README.md#reloc-aware-extracted-targets-2026-09-24)
before running `pipeline.targets --relocate-extracted`: object changes supersede
scores, and queued searches embed the old objects. Reconcile that queued work
through the documented procedure before resubmission.
