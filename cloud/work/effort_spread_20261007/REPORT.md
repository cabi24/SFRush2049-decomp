# Frozen reasoning-effort comparison: func_80087110

## Result

All 70 proposals compiled and received clean canonical scoring. None improved the frozen baseline of 4/445 differing words and zero extras. No exact candidate, source adoption, promotion, or new matched bytes. This research-only follow-on depends on PR #305; independent review and all production/image/ROM gates remain unchanged.

| Requested effort | Proposals | Instrumented preparation, s | Result review only, s | Machine batch, s | Best differing | Unique relocated bodies |
|---|---:|---:|---:|---:|---:|---:|
| low | 25 | 102.563 | 28.558 | 20.795 | 4 / 445 | 23 |
| medium | 20 | 257.116 | 37.131 | 18.916 | 8 / 445 | 20 |
| high | 15 | 581.241 | 79.787 | 16.050 | 4 / 445 | 15 |
| xhigh | 10 | 408.274 | 83.224 | 13.243 | 8 / 445 | 10 |

Preparation includes evidence reading, design, materialization, validation, tools, and interaction. Result-review windows include tools and interaction. Neither is measured active LLM inference time. CPU usage, token counts, and exact active-LLM time are unavailable. Machine batches were serialized in readiness order low, medium, xhigh, high; each used jobs=2.

## Timing boundaries and waiting

- low: first observed 2026-10-07T19:39:38+00:00; frozen 2026-10-07T19:41:30.560971+00:00; observed preparation 112.561s versus instrumented 102.563s. Freeze-to-machine-start 163.249s includes transit, coordinator validation, and serialization, so is not pure queue time.
- medium: first observed 2026-10-07T19:39:53+00:00; frozen 2026-10-07T19:44:15.821748+00:00; observed preparation 262.822s versus instrumented 257.116s. Freeze-to-machine-start 101.399s includes transit, coordinator validation, and serialization, so is not pure queue time.
- high: first observed 2026-10-07T19:40:07+00:00; frozen 2026-10-07T19:50:16.887486+00:00; observed preparation 609.887s versus instrumented 581.241s. Freeze-to-machine-start 45.856s includes transit, coordinator validation, and serialization, so is not pure queue time.
- xhigh: first observed 2026-10-07T19:40:27+00:00; frozen 2026-10-07T19:47:30.054367+00:00; observed preparation 423.054s versus instrumented 408.274s. Freeze-to-machine-start 35.147s includes transit, coordinator validation, and serialization, so is not pure queue time.

The low lane's approximately 1m52s observed preparation and 102.564s instrumented preparation differ by the uninstrumented initial 9.997s. Low read the supplementary shared brief after source freeze; that observation is separate, and no proposals changed. The initial required evidence was identical for all lanes. High's final receipt-incorporation addendum followed its measured 79.787s review window; it is not included in that number. Other end-to-end waiting and coordinator/report/publication work are not mislabelled as worker review.

| Lane | Variant compiler-process sum, s | Compile + Workbench sum, s | Variant strict-score sum, s | Common source verification, s |
|---|---:|---:|---:|---:|
| low | 0.413 | 6.587 | 8.750 | 0.874 |
| medium | 0.298 | 4.929 | 7.063 | 0.735 |
| high | 0.213 | 3.925 | 5.520 | 0.503 |
| xhigh | 0.153 | 2.524 | 3.535 | 0.357 |

Summed process/span durations are elapsed wall time, not CPU time. Parallel and nested spans overlap and cannot be summed into batch wall time. Machine totals include repeat baseline controls, negative canary, snapshot/tool verification, canonical scoring, selected diagnosis, and reporting. All UTC/monotonic boundaries and stage sums are retained in the numeric receipts. Selected diagnosis and fixed overhead mean these batches are not a pure compiler throughput test.

## Findings

- Low L16/L18/L20 reproduce the entire baseline relocated function body. Low has 25 distinct source/ELF files but 23 relocated bodies. Its best changed result is L19 at 7/445; L24 is 12/445.
- Medium's best M07 is 8/445. Its pointer-holder placement is less disruptive than region-wide M06, but no new alias metadata trace establishes the cause. M10/M11 both score 289 yet have different relocated bodies and extents.
- High H05 reproduces the baseline body. H07 is its best changed body at 69/445. Explicit terminal returns did not remove the residual in H05.
- Xhigh X06 scores 8/445 and has the same full relocated-function SHA-256 as historical C01. X02 is the best vertical-carrier proposal at 21/445. All ten xhigh bodies differ within the lane.
- Across lanes, the 70 proposals contain 66 distinct complete sources, 61 distinct complete ELFs and 54 distinct relocated function bodies. Independent designers converged on some equivalent dispatch proposals. Cross-lane hashes are diagnostic; the conservative runner never skipped canonical acceptance checks based on body hashes.

Full per-candidate scores and frozen source identities are in each lane's machine-public-summary.json. Complete sources, exact patches, manifests, precompile source rationale, result reviews and timing receipts accompany this report. Manifest-only normalization mapped LOW01–LOW25 to L01–L25 while preserving source bytes and filenames.

## Fairness and limitations

One trial per effort with unequal budgets is not a causal reasoning-efficiency ranking. Budget, stochastic proposal choice, variable inference/tool latency, start time, cache/order effects, and different self-validation work are confounders. The same parent-default model was used without a model override; effort was low/medium/high/xhigh as requested. No cross-lane proposal or result feedback was supplied during design; each source set froze before receiving its own scores. No adaptive second round or replacement source was run.

Authors chose different preparation checks: low supplied source rationale, medium interpreted 1,120 cases/source, high 9,600, and xhigh 6,120. These costs are inside their preparation windows. Separately, the coordinator applied the same frozen-context/C89 checks and common 1,120-case source interpreter to all 70; all passed. The common checker initially lacked high-lane default/goto syntax; extending those supported constructs and rerunning source interpretation across every lane produced the final uniform pass. No native recompile was needed. Source checks validate bounded control/SDK argument traces, not whole-C equivalence, arbitrary alias-overlap/storage lifetimes, graphics hardware behavior, or native runtime equivalence.

Xhigh had one recorded author-side validation logging failure and source-only retry. A coordinator normalization attempt encountered its copied read-only manifest; since its X01–X10 IDs were already valid, the runner used the unchanged manifest and performed normal preflight. No matching candidate was altered or retried. There were zero native compile/scoring failures across the 70 candidates.

The adapter's sole policy change is increasing the finite variant cap from 20 to 25. A focused test accepts 25 and still rejects 26, jobs=3, compile=121s, score=31s and diagnose=31s. All 56 existing/focused adapter, source-family and diagnosis tests passed. No flags, compiler, target, scorer, source context, acceptance conditions, or other budget gate changed.

## Reproduction and stopping condition

Use the existing pinned IDO/toolchain environment documented for PR #305. For any lane, run:

```sh
python3 tools/cloud/hypothesis_batch.py validate cloud/work/effort_spread_20261007/low/batch.json
python3 tools/cloud/hypothesis_batch.py run cloud/work/effort_spread_20261007/low/batch.json --jobs 2 --out build/hypothesis/fresh-effort-low
```

Replace low with the chosen lane and use a new output directory. The predeclared nonmatching baseline must repeat 4/445 with no extras; a negative canary must be rejected. Publication includes only C/Python, tests, patches, hashes, numeric evidence, and sanitized reports. No ROM bytes, raw assembly/disassembly, objects, compiler/diagnostic streams, credentials, or unrelated private data are included.

The requested finite comparison is complete. Preserve the baseline and stop; this report does not authorize another matching round or merging.
