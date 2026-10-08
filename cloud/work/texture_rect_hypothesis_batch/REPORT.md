# Texture rectangle: one finite hypothesis batch

**Outcome: no improvement and no match.** The original `func_80087110` remains
4/445 differing words. Ten complete C variants ran once under one fixed O3 recipe:
two historical source controls and eight new hypotheses. Every candidate compiled
and received canonical relocation/own-data scoring. None was adopted or promoted.
The batch is finished; no next target or further source search is implied.

## Frozen scope and controls

- Base: `c35728addbae1626aabc89b7d837e28774f333a0`, still remote master when checked.
  The function was absent from both match locks and the provisional list.
- Source: `cloud/work/frontier/w4a/func_80087110/best.c`, SHA-256
  `f8355db6884b85c77a25b21c7aae746be236ed10a3dc8fee93264259cf4c6f8f`.
- Requested flags: `-g0 -O3 -mips2 -G 0 -non_shared`. Canonical compilation adds
  `-Wab,-r4300_mul`. Flags, signature, macro context, targets and scoring rules
  were fixed throughout the batch. No compiler/vendor/scorer changes were made.
- Explicit experiment mode requires an expected unaccepted baseline, here
  4/445 and zero extras. It does not relax calibration's exactness requirement.
- The control-only preparation and the full run each compiled the original twice
  in fresh directories. Both pairs had identical strict fields and whole-ELF hash
  `be430910eb9738a3637248d6b66f3eaa55c01fd879d3bbaec4cd1e4186116572`.
  Each returned 4/445, zero extras, and no unresolved, unverified or error evidence.
- In both runs, the intentionally altered private ELF canary returned 5/445,
  unaccepted, with no unresolved/unverified/error evidence.

`batch.json`, `review.json`, complete C files and exact patches record every
prediction and semantic justification before compilation. Materialization used
10 unique complete sources, not padded variants or fabricated context. The two
historical controls are clearly labeled and are not counted as new hypotheses.

## All results

| ID | Source hypothesis | Differing / 445 | Extra words | ELF function bytes |
|---|---|---:|---:|---:|
| Original | Unchanged baseline | 4 | 0 | 1,780 |
| C01 | Historical drawing-body wrapper | 8 | 0 | 1,780 |
| C02 | Historical setup plus drawing wrapper | 29 | 0 | 1,780 |
| S01 | Genuine step selection before height, body wrapper | 440 | 1 | 1,784 |
| S02 | Genuine step selection before setup wrapper | 440 | 1 | 1,784 |
| S03 | Genuine step selection first inside setup wrapper | 440 | 1 | 1,784 |
| O01 | Genuine offset selection before height | 224 | 2 | 1,788 |
| O02 | Genuine step/offset selection before geometry wrapper | 401 | 0 | 1,780 |
| L01 | Final flipped coordinate stays in texture_edge, body wrapper | 401 | 0 | 1,780 |
| L02 | Same coordinate representation, setup wrapper | 400 | 0 | 1,780 |
| L03 | Shared genuine horizontal flip before setup | 245 | 0 | 1,776 |

All candidate scores are unaccepted, with zero unresolved, unverified or error
evidence. The best compiled variant is the historical C01 control. The best new
proposal by the disclosed differing-plus-extra ranking is O01 (224 + 2), still
far worse than the original. L03 is the next new result by that ranking.
S03 is a useful counterexample to the expected narrow boundary/allocation effect.

There are **10 distinct complete ELFs but 9 distinct fully relocated function
bodies**. S02 and S03 have the same relocated body hash despite different complete
ELF hashes. Conservative runner deduplication correctly did not skip either strict
check. No exact-source duplicates occurred; no compilation was saved by dedup.

## What this establishes, and what it does not

The historical controls reproduce the earlier two-axis constraint: the ordinary-C
body wrapper changes the scheduling outcome while disturbing allocation, and the
setup wrapper produces the known 29-word residual. Prior causal evidence remains
in `texture_rect_compiler_boundary/REPORT.md`: post-exit no-alias metadata creates
an assembler predecessor that blocks the edge-add hoist. The old missing-v1-read
hypothesis remains superseded.

For this finite set, moving genuine initialized values and changing the horizontal
coordinate representation did not satisfy the scheduling and allocation constraints
together. S02/S03's identical machine bodies specifically falsify the expectation
that their inside/outside setup-entry distinction alone would yield different
native code here. These are two bounded, decision-useful outcomes, not a universal
impossibility theorem or eight independent successes.

The new large scores mix instruction placement, function population and register
changes. They are positional full-word counts, not semantic distance. As a
cross-check, sequence alignment finds 259 equal target words in S02/S03 despite
440 positional differences. This diagnostic alignment does not override the
canonical score, identify the original source, or prove a particular allocator
divisor changed. Native allocator/alias metadata was not re-traced for the new
variants, so the precise causal mechanism of each new regression remains unknown.

Workbench diagnosis ran on the baseline, C01, C02 and S03, then on retained O01
and L03 objects without recompilation. C01/C02 retain register-class residuals;
new O01/L03/S03 produce mixed structural/register diagnoses. These are heuristic.
Workbench also reports 32 constant differences because its private target is a
raw-word diagnostic object and candidate relocations are unresolved in that view;
canonical scoring resolved those references and is authoritative.

## Timing

All times below are UTC. Instrumented events use monotonic elapsed durations and
append-only logs correlated by run, stage, span and candidate. Failures and
cancellation paths also produce terminal summaries and visible interrupted spans.

| Observation | Start | End | Elapsed |
|---|---|---|---:|
| Materialize ten sources | 17:14:09.063814 | 17:14:09.066438 | 0.0026 s |
| Separate control-only preparation | 17:14:09.236450 | 17:14:12.554685 | 3.318 s |
| Complete jobs=2 batch, including repeated controls | 17:16:03.496117 | 17:16:16.782532 | 13.286 s |
| Retained-object analysis and two extra diagnoses | 17:18:14.749694 | 17:18:16.401036 | 1.651 s |

Within the 13.286-second batch:

- Preflight validation: 0.049 s; frozen snapshot: 0.519 s
- Baseline/negative controls: 2.992 s, including target preparation and scoring
- Workbench campaign wall time: 2.372 s
- Ten variant compile-plus-Workbench spans: 3.189 s summed, overlapping at jobs=2
- Actual IDO subprocesses for the ten variants: 0.186 s summed; baseline compilers
  add 0.039 s. Wrapper/snapshot verification and diagnostics dominate this tiny leaf.
- Thirteen strict-score spans (ten variants and three controls): 5.233 s summed
- Selected diagnosis: 3.036 s summed across four objects
- ELF deduplication: under 0.001 s; final integrity verification: 0.122 s
- Initial report serialization: under 0.001 s
- Queue submission-to-worker-entry: 0.243–1.527 s, mean 0.868 s. This includes
  campaign setup; these waits overlap and are not added to total run time.

Nested stages and parallel workers overlap. **Summed process/span wall time is not
CPU time and must not be added to total wall time.** The measured batch boundary
ends after the initial report; final serialization of its timing summary follows
that boundary. The separate analysis first had a failed, retained read-only attempt
because an ELF helper does not expose an entry-size key; the bounded retry used the
ELF32 symbol-entry size and succeeded. It did not recompile or change any candidate.

One-off implementation, planning, review and tool interaction were much longer
than execution. Conversation-observed boundaries for this task are 17:03:17 to
17:16:03 (about 12m46s before launch). Variant-design work occupied 17:05:08 to
17:14:27; review request-to-clearance occupied 17:10:22 to 17:15:07. These intervals
substantially overlap and are observations, not monotonic active-work timers.
Earlier runner implementation lies outside this task's measured record.
No exact active LLM time, token count, CPU cost or human-wait total is available.
There is no comparable logged sequential workflow or jobs=1 replay, so **no LLM or
machine speedup is claimed**. The measured benefit is one bounded machine execution
producing a complete result set without per-variant LLM intervention.

## Verification, exports and stopping condition

55 focused runner, car-select, texture-source and existing diagnosis tests pass.
The source tests interpret the actual generated control skeleton over 1,120 bounded
cases per candidate and detect four semantic mutants; they are not a native runtime
or whole-C equivalence proof. Permanent tests cover explicit O3/nonmatch policy,
blocked evidence, frozen macros, append-only timing, cancellation and terminal
summaries after invalid preflight or failed snapshot creation.

Only source, tests, hashes, counts, offsets, sanitized reports and timing events
are published. Protected target data, ROM bytes, raw assembly/disassembly, objects,
compiler streams and diagnostic JSON remain in ignored build storage. The original
source and promotion state are unchanged. Independent checking, production-context
replay and image/ROM gates remain required for any future matching claim.

Reproduction:

```sh
python3 tools/cloud/hypothesis_batch.py validate cloud/work/texture_rect_hypothesis_batch/batch.json
python3 tools/cloud/hypothesis_batch.py run cloud/work/texture_rect_hypothesis_batch/batch.json --jobs 2 --out build/hypothesis/new-texture-run
```

Use a new output directory and the recorded existing toolchain. This is a completed
negative experiment with useful constraints, not a new accepted function or coverage.
