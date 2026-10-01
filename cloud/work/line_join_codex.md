# Same-line scheduling catalog addition, 2026-10-01

`amatch.mutate` now provides `line_join`: replace one whitespace-only newline gap at a statement or compound boundary with a space. C tokens and ordering stay identical. Repeated mutations can put three stores on one physical line, reproducing pilot lever 33. Existing `mutations`/`apply` interfaces stay unchanged; no verdict API exists here, so no routing or unsafe dead reads were added.

Comments, preprocessing directives, escaped-newline gaps and token interiors are preserved. TUs containing `__LINE__`/`__COUNTER__`, or a continued `//` comment (which the light tokenizer cannot fully parse), are conservatively skipped.

## Validation

Seven dedicated tests cover statement scope, compound loops, block/line/continued comments, preprocessing continuations, strings, escaped lines, token identity, stable mutation IDs and line-dependent macros. All pass locally and on Rocky A. Existing `test_mutate.py`: 57 tests pass, 2 IDO-dependent skips on coordinator. `git diff --check` and Python compilation pass.

Known pilot reproduction on Rocky: expand the last three stores in the already-locked `cloud/matches/func_800D4D84.c` onto separate physical lines, preserving tokens. Baseline is 6/30 strict differing words. All 14 single-join variants compile without errors; best is 4. Applying the family again to that best source reaches strict MATCH, independently confirmed with:

```sh
python3 tools/cloud/score.py fn build/codex_line_pilot_match.c func_800D4D84 --flags "-g0 -O2 -mips2 -G 0 -non_shared"
```

All 12 attempted second-step variants compile too. This reproduces a known training mechanism; it adds no ROM coverage and proves no generalization gain.

## Bounded held-out measurement

Protocol fixed before inspecting results: first eight single-function entries in the existing corpus's `held` half, original recorded source/flags, 100 evaluations per function, seed 0, two concurrent functions with one compiler worker each, timeout 60 seconds. No held-out source was read to design this family. Baseline uses the pre-change catalog, after uses the final catalog; run the same command for each:

```sh
python3 cloud/work/tools/amatch/corpus.py bench --half held --limit 8 --budget 100 --jobs 2 --timeout 60 --search-cmd "python3 cloud/work/tools/amatch/search.py --jobs 1 --seed 0" --json-out build/codex_line_before.json
# With the new catalog installed:
python3 cloud/work/tools/amatch/corpus.py bench --half held --limit 8 --budget 100 --jobs 2 --timeout 60 --search-cmd "python3 cloud/work/tools/amatch/search.py --jobs 1 --seed 0" --json-out build/codex_line_after.json
```

Names: `camera_reset`, `camera_smooth_lerp`, `func_8008AD04`, `func_8008FFD0`, `func_8009002C`, `func_80092FE0`, `func_80096B00`, `func_800A464C`.

**Before: 0/8. After: 0/8. Clean measured gain: 0.** Both runs use all 100 evaluations per function, with no harness errors. This bounded subset/budget does not replace the historical 27-function, budget-1000 benchmark. An earlier exploratory run omitted the nested search's explicit worker setting; the numbers above come from the corrected reproducible commands. Logs/JSON/cache artifacts stay in ignored `build/`; none are committed.
