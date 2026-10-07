# Source-structure-informed camera_transform effort spread

## Result

All 70 requested proposals were evaluated. **H08 improves the canonical score to
511/557 differing words, with zero nonzero extra words**, from the unchanged clean
baseline 533 and the previous spread's best 532. That is 22 fewer differing words
than the clean seed and 21 fewer than the previous best. It remains a broad
nonmatch, not native semantic acceptance, ABI recovery, production adoption,
matched coverage or a ROM claim. Lower differing-word counts are better.

| Requested effort | Proposals | Preparation wall seconds | Review wall seconds | Serialized machine seconds | Best differing /557 | Unique selected bodies |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| low | 25 | 52.669 | 24.498 | 25.096 | 534 | 17 |
| medium | 20 | 94.414 initial; 148.765 through corrected freeze | 35.452 | 21.787 | 534 | 10 |
| high | 15 | 166.271 | 57.531 | 18.911 | 511 | 14 |
| xhigh | 10 | 226.150 | 54.284 | 14.377 | 533 | 8 |

All four lanes explicitly requested model `gpt-6.1-sol` and the stated effort.
These are observed planning, reading, tool and review windows, **not active LLM
inference or CPU time**. Unequal budgets, one trial per effort, independent stochastic
proposals, latency, order and cache effects do not establish an effort-efficiency ranking.

## Identical clean seed and richer evidence

The unchanged starting translation unit is
`cloud/work/lean_camera_transform_20261006/candidate.c`, SHA256
`1bac499d4feded2f749f32b3493e28beba8667bd493d385e1ca1332d5c0160b2`,
pinned to master `c35728addbae1626aabc89b7d837e28774f333a0`.
It has 523 own words versus the 557-word native target at
`[0x80097CA0,0x80098554)`. The prior 532 candidate was a comparison reference,
not the new seed. The [PR #307](https://github.com/cabi24/SFRush2049-decomp/pull/307)
structural reconstruction has 551 words but scores538; it was also not substituted
for the seed or described as an improvement.

Each lane saw the same frozen assignment and diagnosis, including:
- Native MP_TargetSteerPos receives s0/f20/f22 and clobbers f24 unsaved; caller
  saves FP pairs and uses f26 for the -2 sentinel. The exported reconstruction
  retains ordinary a0/a1/a2 ABI. Authentic out-of-line context remains unresolved.
- File separation, definition reordering and adding the known genuine outer
  caller did not stop helper inlining. These no-information probes were excluded.
- Native has separate early-stop, normal-completion and exhausted-retry recycling
  sites, and explicit success/failure setter diamonds. These observations justify
  varying real control flow and lifetimes around those operations.

Declarations, signatures, the actual exported start helper, stop helper, flags,
compiler, protected targets and acceptance scorer remain unchanged. The four real
static setter bodies are additionally editable in this round. No fake caller,
keeper, padding, invented volatile pressure, assembly or compiler flag trick was used.
Exact flags are IDO5.3 `-g0 -O3 -mips2 -G 0 -non_shared`, with canonical mandatory
assembler `-Wab,-r4300_mul`.

## Strongest outcomes and deduplication

- **H08, 511/557, zero extras:** initialize a real setter success local to1, enter
  callback work only for a live voice, and clear success on callback failure;
  combine this with path-local recycling. Callback order, original formulas and
  guarded argument evaluation are retained. Its selected symbol extent is535
  words and each list API has four static calls, including completed-voice cleanup.
  Extent and call counts are diagnostics, not acceptance gates.
- H12 is531/557, zero nonzero extras, combining real stable list/completion-counter
  addresses with diamonds/recycling. Its selected extent is559 words, including
  zero tail padding; the canonical extra-word metric counts nonzero excess words.
- H13, X02 and X10 tie clean533. Low and medium best534, so neither improves.
- Across70 proposals: **68 unique complete sources, 66 complete ELFs and41 relocated
  selected-function bodies**. All41 selected body hashes are new against the prior
  spread's retained-body reference. Cross-lane duplicate outcomes are retained in
  comparison.json; equal hashes were checked on bytes privately. Equal scores alone
  were never treated as identical code.

The score is offset-aligned full-word distance after canonical relocation/own-data
verification. The gain does not demonstrate a proportional behavior improvement or
explain which native register/scheduling decisions were recovered. No new production
candidate was adopted. The independent checker owns acceptance and merging.

## Uniform validation and limits

All70 compiled and scored with zero errors, unresolved or unverified references.
Each lane reproduced two byte-identical 533 controls; the private negative canary
scored534. Every source passed the same C89 host compilation and **2,048 deterministic
baseline-relative trace/state cases**:143,360 comparisons in total. Common semantic
machine checks took2.701/2.080/1.569/1.075 seconds, separate from the table's machine
batches. These checks use host pointers/layouts and bounded callbacks, not native
execution, O32 ABI, arbitrary aliasing/concurrency, complete C equivalence or full
exceptional floating-point/FCSR validation.

Domain qualifications are not hidden by the passing bounded checks. X09 explicitly
routes NaN to integer zero and is not generally equivalent on exceptional inputs;
it scores540 and is not an acceptance lead. X05 and several staged/split rate forms
have ordinary-finite/excess-precision/FCSR qualifications recorded before evaluation.
Neither a domain-limited score nor the host harness can establish native acceptance.
H08 retains the baseline arithmetic forms and is not the NaN-routing candidate.

Independent review freshly reproduced baseline533, H08=511 and H12=531, confirmed
same compiled function bytes and reran both leaders through the unchanged2,048-case
harness. No concrete source defect was found. Review explicitly identified coverage
limits: retries are initialized only0..11, so255-to0 byte wrapping is supported by
unchanged source rather than exercised by those tests; callback mutations do not
exhaustively cover parameter fields or dynamic entry counts. See the two
independent verification/review receipts. This adds no new candidate.

The runner's manifest validator now optionally names at most four existing static
helper bodies for experiments, preserving all signatures and unlisted TU context;
calibration cannot use this extension. Four new regression tests cover the named
body boundary, signature/context preservation, directives/missing definitions and
invalid allowlists. The prior camera negative-canary correction is included as a
prerequisite: target a matched, relocation-free word inside the selected function,
not the earlier helper's first text word. The canonical scorer is unchanged.
**40 focused runner/compiler-boundary tests pass.** No full-ROM build was run.

## Timing and correction disclosure

Initial worker dispatches were observed at21:37:38/21:37:47/21:37:55UTC. Xhigh's first
attempt at21:38:02 hit capacity; successful dispatch was observed around21:38:58,
a56-second startup gap. All sources froze before candidate scoring began at21:44:03.
Short required-document reads before each lane's first planning marker are excluded
from the table's marked interval and disclosed in the receipts. No active inference
duration is available.

Medium self-review caught a full-expression cast-parenthesization transcription
error before any compiler, semantic or score evaluation. The initial freeze was
explicitly invalidated, original hashes preserved, and the same20 hypotheses were
corrected without score feedback. Initial94.414 seconds and start-to-corrected-freeze
148.765 seconds are both retained; the latter includes reporting/authorization wait.
The isolated correction tool window was0.236 seconds, not all correction reasoning.

Lane machine batches were serialized low, medium, high, xhigh, each jobs=2. Freeze to
machine-start waits were307.820,213.125,230.084 and115.714 seconds respectively. These
include waiting for all designs to freeze, preflight, validation and orchestration,
not pure queue or inference time. Independent leader review and publication occur
later and are outside these lane timing windows. Per-stage summed elapsed spans
can overlap and must not be summed into total CPU or elapsed batch time.

## Reproduction and scope

This independent research branch targets [PR #306](https://github.com/cabi24/SFRush2049-decomp/pull/306)'s
runner branch. It does not require the prior camera-spread publication to exist.
The manifests, plan template, baseline context, host harness and hash-only prior-body
reference here are self-contained. Original-context source/report references in
ASSIGNMENT.md describe the files available when lanes were dispatched; PR307 is the
published context diagnosis. Generators are not needed to reproduce frozen sources.

With the existing IDO/toolchain environment configured, from repository root:

```sh
python3 cloud/work/context_spread_camera_20261007/run_all.py build/context-spread/fresh
python3 cloud/work/context_spread_camera_20261007/collect.py build/context-spread/fresh
python3 -m pytest -q tests/cloud/test_hypothesis_batch.py tests/cloud/test_texture_rect_compiler_boundary.py
```

The runner requires a new output directory and preserves its protected target/toolchain
checks. Raw objects, target bytes, compiler streams and disassembly stay in private
build output. Published files are source, tests, numeric metadata, hashes, timing
receipts and research notes only. No ROM bytes, raw assembly, credentials or unrelated
private data are included. This completes the bounded70-proposal experiment, without
an adaptive follow-on or permission to merge.
