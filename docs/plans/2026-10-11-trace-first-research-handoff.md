# Research archive and trace-first handoff (2026-10-11)

This is a documentation-only successor to the completed experiments in
[PR305](https://github.com/cabi24/SFRush2049-decomp/pull/305),
[PR306](https://github.com/cabi24/SFRush2049-decomp/pull/306),
[PR307](https://github.com/cabi24/SFRush2049-decomp/pull/307), and
[PR308](https://github.com/cabi24/SFRush2049-decomp/pull/308).
Disposition: archive and close those drafts, preserving their branches, commits,
sources, tests, and receipts. Closing does not adopt or merge their code. This
handoff preserves the useful findings and recovery paths without importing the
large experiment packets or adding routine replay cost.

**No new experiment, source adoption, exact match, accepted-image bytes, or ROM
coverage is claimed.** PR311 remains a separate open research checkpoint. Merging
and acceptance remain with the independent checker. The current
[cloud plan](2026-10-11-astra-cloud-plan.md),
[frontier handoff section 0](2026-10-06-frontier-handoff.md#0-wave-1317-update-2026-10-11-after-wave-17-and-the-ido-71-boot-tail-promotion),
and [Wave 15–17 brief](../../cloud/work/frontier/WAVE_BRIEF.md) guide future work.
This document prepares the next investigation; it does not launch a wave.

## 1. What the archived experiments actually established

| PR / immutable head | Result worth retaining | Closure rationale |
| --- | --- | --- |
| 305 / `4fc630b03952183b591d3f53d47c2ef6ce4205e2` | Frozen runner and timing infrastructure; 10 texture-rectangle proposals, baseline 4/445, none improved. Historical controls reproduced 8/445 and 29/445. | Mixed tooling/experiment draft completed; retain runner recovery inventory below, rather than merge 40 files for history. |
| 306 / `1b732d7850af1ed41b3d851b9969d2957b71c543` | 70 texture proposals, no improvement over 4/445, no exact match. 66 source / 61 ELF / 54 relocated-body identities. | Completed comparison; no adaptive continuation or reason to carry its 207-file packet in the queue. |
| 307 / `e6cbc232db455f0b9565924c5e435a7c3c793bd5` | Genuine camera private-ABI and control-flow diagnosis; context probes did not recover the native out-of-line helper. | Preserve diagnosis below and complete immutable evidence; its structural candidate is not an acceptance lead. |
| 308 / `44e65ff18d93bdf3e73f662115edeccb3c9f46e4` | 70 context-informed camera proposals; H08 improves 533 to 511/557 differing words, zero nonzero extras; H12 is 531. Neither is exact. | Preserve the genuine numeric gain, source leads, and runner corrections; stop the broad sweep. |

The plan's shorthand "zero improvement" must not erase PR308's **22-word numeric
improvement**. All four drafts produced zero exact matches/adoptions. Positional
word distance does not establish proportional semantic improvement, a causal
compiler explanation, or helper-ABI recovery. Effort comparisons had unequal
budgets, one trial per effort, and latency/cache/self-validation confounders;
their observed wall windows do not support an LLM-efficiency ranking.

Full reports, frozen sources, negative results, and bounded-test caveats remain at:

- [305 texture report and packet](https://github.com/cabi24/SFRush2049-decomp/tree/4fc630b03952183b591d3f53d47c2ef6ce4205e2/cloud/work/texture_rect_hypothesis_batch).
- [306 effort comparison](https://github.com/cabi24/SFRush2049-decomp/tree/1b732d7850af1ed41b3d851b9969d2957b71c543/cloud/work/effort_spread_20261007).
- [307 private-ABI diagnosis, audit, and probes](https://github.com/cabi24/SFRush2049-decomp/tree/e6cbc232db455f0b9565924c5e435a7c3c793bd5/cloud/work/camera_context_20261007).
- [308 camera report, independent review, and comparison receipts](https://github.com/cabi24/SFRush2049-decomp/tree/44e65ff18d93bdf3e73f662115edeccb3c9f46e4/cloud/work/context_spread_camera_20261007),
  including [H08](https://github.com/cabi24/SFRush2049-decomp/blob/44e65ff18d93bdf3e73f662115edeccb3c9f46e4/cloud/work/context_spread_camera_20261007/high/candidates/H08.c)
  and [H12](https://github.com/cabi24/SFRush2049-decomp/blob/44e65ff18d93bdf3e73f662115edeccb3c9f46e4/cloud/work/context_spread_camera_20261007/high/candidates/H12.c).

## 2. Unmerged runner: preserve, do not silently adopt

The reusable runner is **not present on inspected master `e36410de1`**. Its code
remains unmerged and accessible at the pinned links below. Any future extraction
needs its own review against current master, protocol-v2 packaging, and tests;
these links are an inventory, not a runtime dependency or instruction to rerun
the sweeps.

- [Original runner](https://github.com/cabi24/SFRush2049-decomp/blob/4fc630b03952183b591d3f53d47c2ef6ce4205e2/tools/cloud/hypothesis_batch.py),
  [contract and operating limits](https://github.com/cabi24/SFRush2049-decomp/blob/4fc630b03952183b591d3f53d47c2ef6ce4205e2/docs/hypothesis-batches.md),
  and [adapter tests](https://github.com/cabi24/SFRush2049-decomp/blob/4fc630b03952183b591d3f53d47c2ef6ce4205e2/tests/cloud/test_hypothesis_batch.py).
- The original [source-family utilities](https://github.com/cabi24/SFRush2049-decomp/tree/4fc630b03952183b591d3f53d47c2ef6ce4205e2/tools/cloud)
  are `prepare_car_select_batch.py` and `prepare_texture_rect_batch.py`; their
  [tests](https://github.com/cabi24/SFRush2049-decomp/tree/4fc630b03952183b591d3f53d47c2ef6ce4205e2/tests/cloud)
  are `test_car_select_batch.py` and `test_texture_rect_batch.py`. The prepared
  20-variant car-select calibration was not executed; it must not be counted as a run.
- [306 cap change and test](https://github.com/cabi24/SFRush2049-decomp/compare/4fc630b03952183b591d3f53d47c2ef6ce4205e2...1b732d7850af1ed41b3d851b9969d2957b71c543):
  finite cap 20 to 25; 26 remains rejected and other budgets are unchanged. This is
  a historical experiment choice, not a required default for future trace work.
- **Do not reuse the original negative canary unchanged.** The
  [latest archived runner](https://github.com/cabi24/SFRush2049-decomp/blob/44e65ff18d93bdf3e73f662115edeccb3c9f46e4/tools/cloud/hypothesis_batch.py)
  selects a matched, relocation-free word inside the selected function. Mutating
  the first text word can touch an earlier helper or an already-mismatching word.
  [Regression tests](https://github.com/cabi24/SFRush2049-decomp/blob/44e65ff18d93bdf3e73f662115edeccb3c9f46e4/tests/cloud/test_hypothesis_batch.py)
  cover those cases, relocation sites, and the next function boundary.
- That same 308 revision adds an optional allowlist of at most four existing
  static helper bodies for experiments only. Signatures and all unlisted context
  remain frozen; malformed/missing helpers and directives fail closed. Its tests
  and selected-function canary correction are dependencies of that later design,
  not justification to import the 149-file camera packet.

Reusable safeguards include two identical whole-ELF baseline builds, explicit
strict baseline expectations, canonical relocation/own-data scoring, bounded
deadlines and two-worker concurrency, process-group cancellation, whole-object
deduplication with byte confirmation, and allowlisted public exports. The runner
does not prove C equivalence or produce production acceptance. Its old snapshot
and mutable-tool hash contract is **not** the protocol-v2 receipt contract.

## 3. Camera diagnosis and stopping point

PR307 establishes a native private helper ABI: `MP_TargetSteerPos` receives the
entry in `s0`, volume in `f20`, pan in `f22`, and clobbers `f24` without saving it.
The helper's frame is 24 bytes, with only `ra` saved; the reconstructed exported
helper instead uses ordinary argument registers and a 32-byte frame. The native
caller saves four FP pairs, uses `f26` for its sentinel, and has an 88-byte frame;
the exported baseline saves two pairs and has an 80-byte frame.

Making the helper internal, separating files, changing definition order, and
adding the known genuine outer caller did not prevent inlining. The committed
target corpus contains one direct helper call; that does not rule out original
eliminated or address-taken source context. A historical synthetic keeper is not
evidence of the real translation unit. No successful private-context repair is
claimed.

Native recycling uses separate early-stop, completion, and retry-exhaustion
paths; setter results use branch diamonds. Reconstructing these shapes produced
551 instructions versus native 557 (baseline 523), but worsened the positional
score to 538/557 from 533. PR308 H08 combines path-local recycling with a genuine
setter-success local and scores 511; the original helper context stays unresolved.
The shared-setter probe with unresolved references is invalid, not a match.

The 2,048-case host checks are relative to the reconstructed baseline, not native
execution. Exceptional FP/FCSR behavior, arbitrary aliasing/callback mutation,
and full retry-wrap coverage remain outside their proof. Keep these restrictions
when reusing H08, the structural candidate, or the semantic harness.

**Next justified camera question:** which real inlining/visibility or live-range
decision accounts for the private helper contract and the remaining allocation?
Obtain a target-specific trace before choosing a source lever. Do not repeat the
failed file/order/outer-caller probes or invent a keeper to claim closure.

## 4. Trace-first next-target preparation

The following are investigation choices, not target reservations or established
causes. Refresh the frontier/claims and coordinate ownership before starting.
Instrumented or forced compiler output is diagnostic only. Re-establish fidelity
against stock output for the actual chosen context; accept only an ordinary
canonical rebuild with complete relocation and owned-data checks.

### Priority A: real frame ownership in PR311

Use the [complete frozen PR311 checkpoint](https://github.com/cabi24/SFRush2049-decomp/tree/a74d3332fb630087d1e811fb5bd20e0cb6c7f445/cloud/work/frontier/dot_runtime_a_two_waves_20261011),
without claiming its inherited 1,508 bytes again. Existing precise starting points:

| Target | Strict residual | Native / candidate frame | First discriminating observation |
| --- | --- | --- | --- |
| A:80394058 | 2/323 | 128 / 120 | Separate CFE local area, merged inline contribution, and coloured-expression homes. Native top eight bytes have no observed access; every other word already agrees. |
| A:803A6F2C | 2/149 | 320 / 312 | Same census in the real four-sibling context; its addressed buffer homes already agree. |
| A:803A7180 | 6/296 | 168 / 192 | Explain the excess 24 bytes and both homes shifted +24; adding locals cannot fix this case. |
| A:803A7620 | 4/190 | 144 / 128 | Reconcile homes shifted -12 with frame short by 16, distinguishing capacity hypotheses from expression-home allocation. |
| A:803A7918 | 7/144 | 608 / 600 | Explain the second-case home +8 while the first agrees, within the real caller and switch ownership. |

[Root evidence](https://github.com/cabi24/SFRush2049-decomp/tree/a74d3332fb630087d1e811fb5bd20e0cb6c7f445/cloud/work/frontier/dot_runtime_a_roots_20261011)
already recovers the native FP-save set through the genuine renderer. Six real-value
capture probes either optimized away or changed code. The
[sibling evidence](https://github.com/cabi24/SFRush2049-decomp/tree/a74d3332fb630087d1e811fb5bd20e0cb6c7f445/cloud/work/frontier/dot_runtime_a_menu_renderer_closure_20261011)
has whole-object-identical stock UGEN diagnostics and complete code/owned-switch
verification. Buffer capacities remain hypotheses, not established original sizes
or unrestricted safety guarantees.

The [tracing toolkit](../../cloud/work/frontier/tools/trace/README.md) documents
that coloured expression webs can receive homes without spilling; home reuse
depends on block conflicts and merged-frame size. Wave17 also demonstrated that
missing nested inline levels enlarge frames. Inspect `umerge` decisions and
`spill`/`spcensus` evidence before selecting a declared-local, reuse, or expression
lever. Constant spelling (`0`, `0.0f`, `0.f`) affects web identity in established
Wave17 examples; it is **not proven to cause these particular frame gaps**.

The owner's Wave16 rule permits disclosed unused locals with exact frame-residual
arithmetic and no numeric cap. That is less restrictive than these packets'
earlier voluntary no-unused-locals posture. A permitted local must still be
disclosed as reconstruction; byte identity would not prove original source
ownership. Do not disguise padding arrays or buffer-capacity inflation as proof.

### Priority B: D05C's specific colour permutation

[A:8039D05C](https://github.com/cabi24/SFRush2049-decomp/tree/a74d3332fb630087d1e811fb5bd20e0cb6c7f445/cloud/work/frontier/dot_runtime_a_cleanup_lifetime_20261011)
is a separate **18/169 register-allocation residual**, not a frame-only case.
Stock partial tracing establishes allocation order: bounds 2816 and 572, player
copy, player counter take s4–s7; native puts the counter first. All 31 grounded
source forms failed to improve it. The listing does not expose priority costs.

First reproduce that order under the genuine group. A fidelity-checked forced
colour oracle should establish whether all remaining words follow from the
permutation; then inspect priority, live-block counts, interference and split
decisions. Wave17 priority oracles and Wave16 split logging are concrete tools,
not evidence that a tie is already the cause here. Choose one justified source
lever only after observing the decision. Do not repeat the denied ptrace route.

### Priority C: the main-image D816C / D91A0 cluster

The [Wave17 lane report](../../cloud/work/frontier/w17e/RESULTS.md) is more precise
than the summary table: D91A0 is 8 mnemonic rows / 212 aligned word rows off, but
its canonical score is **239/965 with a 966-word body**. Those metrics are not
interchangeable. D816C remains 627/699 and needs real reconstruction in the same
group (t2 parameter, unsaved s0–s8 clobbers). The pair spans 6,656 bytes; four
already-provisional partners total another 1,572 bytes, with no current new credit.

Start from `w17e/d91a0/best.c` and `w17e/base/`, retaining the genuine D8154
signature and DA0B4 getter evidence. The oracle removes 48 aligned word rows by
changing four callee-saved decisions, but leaves short-web, spill and schedule
residuals. Inspect the constant-1 versus constant-21 priority/source types before
changing them. Resolve D8154's context conflict explicitly: D91A0 currently needs
an inline blocker while native D816C inlines the same helper. An exact unit result
alone would not prove portable group packaging.

If static work is chosen later, use the pinned IDO 7.1 boot-tail route and the
[w17g inventory](../../cloud/work/frontier/w17g/inv_table.md), not the old 5.3
assumption. No static matching work starts from this handoff.

## 5. Verification and archival boundary

Inspected master: `e36410de188a00fc0d6bc9958d578251e0a049ba`. The four old
PR heads, states and discussions were refreshed on 2026-10-11 before disposition.
Exact-head hosted Verify passed for
[305, run 37658868855](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37658868855)
and [307, run 37690279234](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37690279234).
No workflow runs were returned for the stacked 306/308 heads; their reported local
tests are not CI passes. Closing does not change those historical facts.

This handoff adds no executable packet and no default-suite replay. Its validation
is source/link inventory, factual cross-checks, UTF-8/whitespace checks, and the
protected-path guard; it does not rerun old compiler experiments or inherit their
tests as a fresh pass. Immutable links are archival provenance, **not** nonancestor
`git show` dependencies for current-master replay. Future executable packets pin
sources, relevant target-checksum rows and verifier inputs, use `tu_cc` for a
production TU, and fit one test under 5 seconds without IDO / under 30 with IDO.

No branch, commit, research artifact, accepted lock, protected target, scorer,
production source, or image-B input is deleted or modified by this cleanup. No ROM
bytes, raw assembly dumps, binaries, credentials, or unrelated private data are
included. The PR timelines record the actual closure events and successor link.
