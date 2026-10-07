# Large-function frozen effort comparison: camera_transform

## Result

All 70 prescribed proposals compiled, received clean canonical scoring and passed the same 2,048-case bounded host-C differential check. Low L17 improves the frozen baseline from **533/557 to 532/557 differing words**, with zero extras. Medium, high and xhigh do not improve the baseline. This is a one-word positional-score gain in a broadly nonmatching function, not an exact match or a demonstrated gameplay correction. No production source was adopted, no match was promoted, and no new matched bytes are claimed.

| Requested effort | Proposals | Marked preparation, s | Completed review window, s | Machine batch, s | Best differing | Unique target bodies |
|---|---:|---:|---:|---:|---:|---:|
| low | 25 | 106.775 | 27.431 resumed | 21.710 | 532 / 557 | 20 |
| medium | 20 | 151.082 | 39.564 | 19.025 | 533 / 557 | 16 |
| high | 15 | 185.102 | 40.145 | 16.102 | 533 / 557 | 13 |
| xhigh | 10 | 206.164 resumed | 54.756 | 14.243 | 533 / 557 | 10 |

Preparation includes reading, reasoning, tools, source materialization and the prescribed lightweight structural check. Review windows include result reading and tools. These are observed elapsed windows, **not active LLM inference time**. CPU time, tokens and exact active-LLM time are unavailable. Machine batches were serialized low, medium, high, xhigh, each jobs=2; common semantic validation was performed separately.

## Target selection and immutable baseline

Target camera_transform is the historical name of an effect-command queue processor, with native extent `[0x80097CA0,0x80098554)`, **2,228 bytes / 557 words**. Current master at selection was `c35728addbae1626aabc89b7d837e28774f333a0`. The target is absent from its accepted static/game locks. Current-master research and the specific GitHub PR search found no newer clean or close result than [PR #251](https://github.com/cabi24/SFRush2049-decomp/pull/251), which is merged research only.

The frozen source is `cloud/work/lean_camera_transform_20261006/candidate.c`, SHA256 `1bac499d4feded2f749f32b3493e28beba8667bd493d385e1ca1332d5c0160b2`. It is a complete executable C reconstruction. The older scaffold's FCSR placeholders, wrong API arities and synthetic keeper are excluded. This target was preferred over the smaller minimap/countdown leads because its unchanged genuine whole C TU reproduces the latest baseline through the existing single-TU runner, without an added group adapter.

Two controls reproduce 533/557, zero extras, unresolved/unverified relocations or errors, with identical whole ELF bytes. Each lane repeats these controls. The baseline's own body is 2,092 bytes / 523 instructions with an 80-byte stack frame, versus native 2,228 bytes and 88 bytes. Exact flags are IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`, with canonical mandatory assembler `-Wab,-r4300_mul`. Protected target and own-data integrity checks remain active; the baseline has no unresolved own-data verification diagnostics.

All declarations, types, helper bodies, source prefix/suffix, flags, compiler and target remain frozen. MP_TargetSteerPos's actual body stays exported to preserve its call using the ordinary ABI; its native private register contract is still unrepresented. The five genuine inline helper algorithms are unchanged. This known source-context limitation applies equally to every lane. No fake caller, padding/pressure storage, artificial volatile, inline assembly or compiler-flag trick is added.

## Findings and deduplication

- L17 names the actual completion-history counter address and uses it for the two real increments. It is the sole score improvement: 532/557, zero extras, unchanged 2,092-byte own extent. It remains a research candidate, with no adoption recommendation from this experiment alone.
- Medium's 20 candidates include ten baseline ties. High's 15 include seven ties. Xhigh's best X02/X09/X10 tie baseline; its other seven regress.
- High H06 and the equivalent path-local recycle variants increase own extent to 2,156 bytes, closer to retail, but worsen the strict score. Matching extent is not itself progress.
- Across all 70 proposals there are **56 distinct complete sources, 45 distinct complete ELFs and 31 distinct relocated selected-function bodies**. Independent lanes converged repeatedly on equivalent forms. The 14 baseline-equivalent proposals are low L06/L13/L16/L19/L20/L23, medium M11/M13/M14/M15/M18 and high H08/H10/H12.
- Every canonical candidate check was retained. Cross-lane hashes did not suppress scoring or turn tied scores into equivalence claims. Full selected-function symbol extents were relocated and hashed; equal retained bodies were checked on bytes. No body bytes or objects are published.

Per-candidate scores, source identities, extents and result reviews accompany this report. `comparison.json` includes all cross-lane identical-body groups and numeric timing evidence.

## Common verification and runner correction

Every author used the same exact-prefix/suffix and parser self-check. Authors ran no compilers, custom semantic suites or exploratory score probes before freeze. The coordinator applied the same C89 compilation/context checks and deterministic 2,048-case host-C comparison to all 70 candidates: **143,360 candidate-case comparisons, all passed**. The checker captures callback traces and final entry/node/list state; scenarios cover flags, history rotation, command kinds, credit availability, queue lengths, retry cutoffs, callback outcomes, table/entry reloads and list mutations. Three negative mutants of history reset, retry cutoff and blocked-state clearing are detected.

This is bounded comparison against the clean C baseline, not a native runtime, O32 ABI, arbitrary aliasing, concurrency or whole-C equivalence proof. Host pointers/layouts differ from O32. The candidate source assumptions and the existing helper-visibility limitation remain material.

Initial preflight correctly blocked before any experiments: the old negative canary patched the first .text word, which belonged to a genuine helper ahead of camera_transform. The canary now selects an already matching, relocation-free word **inside the selected function**. It produces 534/557 versus the baseline's 533/557. Three focused regression tests cover a preceding helper, already mismatching prologue, relocation exclusion and the next-function boundary. This small adapter correction changes neither source compilation nor the canonical scorer/acceptance gate. All **45 focused adapter, experiment and compiler-boundary tests pass**. No full-ROM build, image integration or independent acceptance is claimed.

## Timing interruption and limits

The first brief read precedes the initial preparation marker in each lane; disclosed short startup gaps are not backdated. UTC and monotonic records are retained. At approximately 20:41:47 the coordinator became inactive until 21:01:59, while already launched low/medium machine batches completed normally. High's freeze-to-machine-start delay was 1,246.070 seconds and includes that interruption. Low and medium delays were 65.708 and 42.148 seconds; xhigh's was 16.402 seconds. These delays include delivery, validation and scheduling, not pure queue time or inference.

Low had an initial result-review start at 20:41:57, then resumed at 21:02:35. The table shows its completed resumed 27.431-second window; total active review work before interruption is unknown. Xhigh preserves an initial 62.554-second completed evidence pass and an unclosed generation phase, then a resumed marked preparation window of 206.164 seconds. Its original preparation-start-to-freeze wall interval is 1,632.010 seconds, including interruption; it is not comparable to uninterrupted lane preparation. These receipts do not support subtracting an invented active-LLM duration.

One run per effort with unequal candidate budgets, stochastic proposals, variable service/tool latency, interruption and cache/order effects cannot establish a causal reasoning-efficiency ranking. All used the same parent-default model without a model override; requested effort was low/medium/high/xhigh. No cross-lane ideas or result feedback were supplied during design. Sources froze before own scores; there was no adaptive second round or candidate replacement.

Summed compiler and scoring spans in `comparison.json` are wall durations, not CPU time; parallel/nested spans overlap and cannot be summed into batch time. Common validation, coordinator preparation/recovery and publication are separate from lane preparation/review. The finite 70-proposal experiment is complete; this report does not authorize more experiments or merging.

## Reproduction and publication

This research follow-on is based on [PR #306](https://github.com/cabi24/SFRush2049-decomp/pull/306), which depends on PR #305. Use the pinned IDO/toolchain environment and a fresh output directory:

```sh
python3 tools/cloud/hypothesis_batch.py validate cloud/work/large_effort_camera_20261007/low/batch.json
python3 cloud/work/large_effort_camera_20261007/verify_sources.py cloud/work/large_effort_camera_20261007/low build/semantic/fresh-low
python3 tools/cloud/hypothesis_batch.py run cloud/work/large_effort_camera_20261007/low/batch.json --jobs 2 --out build/hypothesis/fresh-large-low
```

Replace low with medium/high/xhigh. Manifests retain current-master source/target context, baseline expectations and all finite time/worker limits. Publication is allowlisted C/Python, tests, patches, hashes, numeric receipts and sanitized research notes. No ROM bytes, raw assembly/disassembly, objects, compiler streams, credentials or unrelated private data are included. Independent checker owns acceptance, integration and merging.
