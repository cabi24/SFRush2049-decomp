# Independent review: small-overlay service pair

Disposition: **PASS for bounded behavioral reconstruction only**. No blocking source defect found in the documented valid-storage, nontrapping, finite/in-range binary32 domain. This is not VERIFIED-BODY, matching-ready, a universal residency guarantee, or a production integration approval. Zero accepted bytes.

## Scope and findings

Independently inspected both complete authenticated native bodies (D3A4..D498 and D798..DA78), exact arithmetic and field signedness, and immediate transform/angle/depletion-dispatch contracts. Native inputs remained private. Reviewed source/header, host and independent randomized tests, provenance checker, README, and residency report/model. The residency report's complete caller/lifetime trace was authored separately: this review checks its stated limits and abstract model consistency, not an independent re-proof of every listed caller address.

The reconstruction preserves integer magnitude versus float threshold, squared-distance grouping, ordered eligibility, per-iteration count/vector rereads, signed car/team fields, low16 wrap-before-depletion, callback-before-model-flag order, and target car-ID reread after callback. Matrix and custom angle helpers remain real external boundaries. Native callback +1 reaches only the bounded event-ring branch; synthetic callback mutations intentionally stress stronger external-boundary behavior.

Review caught and corrected a weak alias test: simultaneously disabling the next player would have masked a hoisted-origin defect. The final case aliases origin into player memory, leaves the next player enabled, and isolates the changed origin's effect. Native old-remaining load order before last-amount store is retained. No changes to locks, shared headers, production bodies or the owner-paused investigation were part of this review.

## Executed validation

- Final focused pytest wrapper: **4 passed**, run with `/tmp/rush-master-plan-venv/bin/python -m pytest tests/conveyor/test_small_overlay_service_pair.py -q` from the isolated checkout. Default environment Python lacked pytest; the existing project virtual environment supplied it.
- C89 host compilation with pedantic/errors/warnings, O2, no fast-math and disabled contraction passes.
- Separately authored step-rounded binary32 oracle: **30,000 D3A4 fixtures plus 3 wrap/callback edge cases**, and **3,000 D798 fixture sets**, pass. These use synthetic data and mock the native angle result as zero. They are independent host arithmetic checks, not MIPS differential execution.
- Native provenance test authenticates asset/image/body hashes, exact inflation lengths, semantic pool values, outgoing branch containment and direct calls. It does not prove no incoming interior entries or complete image exports/function census.
- Abstract residency model assertions pass; they demonstrate only implications of supplied loader-transition assumptions, not reachability or universal callback residency.

## Limits retained

Original return types, original TU/static visibility, native ABI/compiler equality, helper approximation output, all-memory partial overlaps, aliased opaque player padding/count storage, FPU exception/rounding/denormal state and out-of-range float-to-int behavior remain outside proved host equivalence. Conditional-negate absolute angle is numeric-branch equivalent under these assumptions, not bitwise abs.s for -0/NaN. The pinned IDO compile/size findings are implementation-worker evidence, not independently rerun by this reviewer. Full regression and image/compression/ROM gates are not claimed here.

## Reviewed file identities

SHA256 hashes below bind this review; edits afterward require an appropriate recheck.

- `cloud/work/small_overlay_service_pair/service_pair.c`: `636e7d23969a75761efa708c270e4ad68da6f657203726012587938849945758`
- `cloud/work/small_overlay_service_pair/service_pair.h`: `2ce2c460a582b0af12b9f52c93350b91635a78ffb90a883f8a18469a9c198861`
- `cloud/work/small_overlay_service_pair/audit.py`: `4dac5b726a655b76733abed26f88c7f699a7a3fd6e5230b39e68876bb3afa2f5`
- `cloud/work/small_overlay_service_pair/README.md`: `2f936e1a2201024b63a0f22017fb82cc167dfb095d00ce7bfa677ac6363fa078`
- `cloud/work/small_overlay_service_pair/RESIDENCY.md`: `b83ec742874f967e6dc65219ffa81503de00bd726aab3fca641cef169a663c42`
- `cloud/work/small_overlay_service_pair/residency_model.py`: `4af466a00439d815c6170e504ab426c7a2c1f57c30038627b036f741ac605421`
- `tests/conveyor/test_small_overlay_service_pair.py`: `0aa4128120d3f69fefafffd01e20a341159ecb9fdb7d9c7bf0e08bc36d488d77`
- `tests/conveyor/fixtures/small_overlay_service_pair/host_test.c`: `757c428b3280af4fb9b986c1216f482d22f79fba3f3454844b215d0a48d69d9e`
- `tests/conveyor/fixtures/small_overlay_service_pair/independent_harness.c`: `b30d98dceb493215dd54f2a0d0d51f9c5d63f48c32784cb7d288f45d83e08e12`
- `tests/conveyor/fixtures/small_overlay_service_pair/independent_oracle.py`: `f13cc0773f083cdb417f075eef91d2cf6be57e170605c9dc944396c37b2c5b1d`
