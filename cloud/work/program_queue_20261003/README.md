# Rush execution queue reconciliation

Base: `53d0fba0c5f44e91fe921f722fee7ff63a2dd0fe`. Reviewed 2026-10-03.
Prepared for maintainer review. This packet coordinates existing work; it does not
run another decompilation batch or grant new native-byte credit.

## Decision

**No two matching-execution-ready packets are established by the reviewed
queue.** Do not fill an execution lane by renaming frozen work. Prioritize the
two prerequisite requests below; start with the small C974 metadata lookup
(Request B) if a maintainer map/header is available. Renderer context (Request A)
requires stronger compiler evidence. Return to execution only when entry criteria pass. These requests are useful bounded work, not ready-to-match labels.

The latest maintainer acceptance remains **176 game bytes from #24/#40**.
Game: 666/1,216 functions and 100,656/647,072 bytes (15.56%). Static:
158/230 functions and 44,808/61,440 bytes (72.93%). Combined byte tracker:
145,464/708,512 (20.53%). The 26 incorporated research/audit PRs contribute
source, tests and diagnoses, with zero matching credit. Those counts describe
the documented incorporation batch, not every historical PR.

Evidence: [integration README](../pr_integration_20261003/README.md),
[full gates](../pr_integration_20261003/gates.json),
[28 exact reviewed revisions](../pr_integration_20261003/reviewed_prs.json).
Live GitHub metadata independently confirmed #9/#24/#40/#48/#49 are merged at
the reviewed heads. This review did not rerun private gates or infer acceptance
from merged state. No additional acceptance is claimed.

## Dispatch corrections included in this packet

1. **D09 has existing helper source, not a missing-body gap.** PR #9's B119
   already includes complete rectangle, palette, mask-clear, flags, initializer
   and mode source. Its frozen recorded results are rectangle 71/118, palette
   101/145, mask-clear 11/45, flags 19/74 and initializer 70/100. These are
   nonmatches; mode is 0/387 masked with two unverified local table references.
   [B119 explanation](../near_miss_B119.md),
   [source](../ipa-groups/codex_gfx_rectangle_b119/group.c),
   [recipe](../ipa-groups/codex_gfx_rectangle_b119/group.json),
   [receipt](../ipa-groups/codex_gfx_rectangle_b119/verification.json).
2. **PR #48's live description is corrected; its merged README was not; this change brings it into agreement.** The
   README's “no implementation” and “previously missing actual caller” claims
   are false. Its contribution is additional semantic tests and standalone
   replay packaging. The prior-session report cited in PR #48 records a genuine five-body closure at 75/118, worse
   than archived 71/118, and was correctly not republished. [Corrected PR #48
   description](https://github.com/cabi24/SFRush2049-decomp/pull/48).
3. **PR #49's prior merged README likewise overstated source novelty.** B119/A151
   already contained complete initializer and flags roots. Preserve #49's
   documented ABI analysis and tests, but do not present 75/100 and 29/74 as
   new best compiler scores. Compare like-for-like source/context recipes;
   archived scores above were inspected, not freshly replayed in this review.
   [PR #49 packet](../dot_graphics_init_closure/README.md).
4. This documentation change updates `dot_handoff.md` D09 and the renderer continuation in
   `cloud/RESEARCH_INDEX.md` to require a **new original-TU/export visibility
   explanation**, not recovery of already present helpers. The two
   packet READMEs are corrected in the same change so later workers do not
   rediscover the same false gap. Do not rewrite frozen receipts.

Supporting detail: [renderer input checklist](../graphics_readiness_20261003/README.md)
and [ranked external/caller input contracts](../missing_input_contracts/README.md).

## Queue ownership and stop conditions

Owners below are proposed responsibilities, not claims that someone already
accepted an assignment. Record a named owner and branch before execution.

| Queue | State | Responsible role | Next bounded action / reopening criterion |
|---|---|---|---|
| D01 / #24, #40 | Accepted; complete | Private integration maintainer | No resubmission. Preserve prior locks; accept later work only through full gates. |
| D09 renderer | Blocked on new context evidence | Compiler/context owner; private maintainer for table proof | Complete Request A; no repeat five-body closure or narrow-caller sweep. |
| D02 C974 | Blocked on external service | Maintainer able to identify runtime service; then reconstruction owner | Complete Request B before reconstructing the whole body. |
| D02 other old shortlist | Not ready | Program coordinator | Retain PR #43 prerequisites; no broad rescout in this task. |
| D04 historical shortlist | Retired | Program coordinator | Existing source is not a fresh target; check specific new evidence before redispatch. |
| D05 HUD/minimap | Frozen research | HUD/context owner | Current master recovered missing files, but source-to-receipt identity remains incomplete. Need a new native frame/register explanation; no repeat O2/O3 sweeps. |
| D06 tire/texture/array/matrix | Frozen research | Compiler owner | Require a new native causal explanation. Rejected boolean, scheduling and FP permutations stay rejected. |
| D07 static | No ready packet established here | Static owner | Genuine ownership/TU/header preflight; no default scheduler/inflater replay. |
| D08 very large | Frozen reconnaissance | Program coordinator | PR #42 gives no immediate execution target; existing rank/distance source is broad nonmatch. |
| FAF6C / FA9B4 | Blocked prerequisite leads | Caller-context / service owner | See reserve list below; do not substitute a speculative ABI or wrapper. |
| 800D1248 investigation | Off limits | Repository owner alone can lift pause | No assignment, compile, reconstruction or proposed helper-context investigation. |

Source: [current handoff](../../../dot_handoff.md), [owner restriction](../../../dot_response),
[PR #43](../dot_substantial_scout/STATUS.md), [PR #42](../dot_d08_scout/README.md),
[PR #44](../d06_tire_stack_diagnosis/README.md), [PR #45](../d05_hud_context_audit/README.md).

## Request A: establish a real renderer compiler context

**State: BLOCKED; no new matching run authorized by this packet.**

- Targets: rectangle `8008A46C` (472 bytes), flags `800878E0` (296), graphics
  initializer `sound_init` (400). Shared read-only accepted mode `80086A50`
  (1,548); palette `8008A148` (580); disable `8008705C` (180).
- Existing assets: all six complete bodies in B119, individual/root ABI evidence
  in #48/#49, manifest-protected target words, IDO recipes and frozen receipts.
  Do not ask for the helper source again.
- Smallest useful input: authenticated original graphics translation-unit or
  whole-program partition/exported-root information, **or** a concrete native
  caller/register-lifetime explanation that distinguishes an untried genuine
  context from B119, the five-body closure and already tried narrow callers.
  Naming more possible callers alone is insufficient. The `Input_ProcessGameplayPad`
  and `audio_doppler_calc` work/game bodies remain empty stubs; available
  `object_render` drafts do not establish a reviewed genuine compiler context.
- Separate verification input: for the mode context, a maintainer-produced
  source/object-bound placement and relocation receipt for the two sites
  (`+0xc`, `+0x14`) referring to one 20-byte, five-case table at `80123870`. A table receipt closes a proof gap; by itself it does not fix
  rectangle/flags allocation or make the packet ready.
- Readiness: evidence explains a specific allocation difference; exact edited
  members and kept roots are listed; actual source/ABI is complete; recipe and
  source hashes bind the baseline; protected table evidence is available for
  any strict full-closure claim. Accepted sources remain read-only.
- Bounded next action after readiness: replay the fixed archived baseline,
  run **one** evidence-derived natural-context experiment, compare every claimed
  full resolved body, and stop if the predicted effect is absent. No fake
  callers, keeper functions, pressure variables or invented parameters.

## Request B: identify the C974 external service

**State: BLOCKED; target is not a ready full reconstruction.**

- Target: `func_8010C974` at `8010C974`, 2,636 bytes / 659 words.
- Existing assets: PR #46 corrects `camera_trigger_check` to three pointers,
  with local f12 definition. It authenticates the call at `8010D2C4` to
  `8038D798`. Passing `0x43C80000` is not proof of float 400.0's source type.
- Smallest useful input: a trustworthy typed declaration/contract or readable
  authenticated implementation of `8038D798`, with provenance identifying its
  runtime module/address mapping and exact US revision. Include source/header
  revision or hash and whether the body is external or visible to IDO in the
  genuine translation unit. Need parameter order/types, return behavior,
  relevant memory side effects and calling convention/clobbers. The observed
  a0/a1 alias, signed-byte a2, a3 word and fifth stack word must be explained.
  A name-only symbol or a guessed prototype is insufficient.
- Private input handling: the maintainer can inspect the private implementation
  locally and return a text contract/evidence receipt; no ROM upload or raw
  native/object publication is requested.
- Readiness: contract accounts for the actual call and eliminates speculative
  ABI; remaining whole-body fields/callees have evidence; owner reserves exact
  source/context members and records the full reconstruction plan and recipe.
- Bounded next action after readiness: reconstruct the complete natural body
  with the corrected three-pointer call, compile one baseline, then diagnose
  the dominant residual before deciding whether further tuning is justified.

Evidence: [C974 contract and stop](../c974_call_contract/README.md),
[authenticated audit receipt](../c974_call_contract/evidence.json).

## Reserve list; do not silently promote it

- `init_state_continue` / `800FAF6C` (712 bytes): round12 caller preflight found
  an unsaved-s-register leaf. Need the native-faithful `800FB2C8`
  `setup_state_main` / `display_list_flush` body (2,356 bytes) and actual callee
  contracts, then justify a bounded real caller context. The graphics
  placeholder is not that body; a `countdown` wrapper is not an ABI root. If no
  faithful C exists, this is high-cost reconstruction work, not missing native
  bytes or a reason to request a ROM. Its 712-byte matching payoff is uncertain.
- `render_viewport_init` / `800FA9B4` (924 bytes): round12 stopped on external services
  `8038FCE0` / `80390F60` and a conflicting `save_write_data` prototype. Do not
  assign until exact service contracts and the native wrapper ABI are proved.
- These two round12 findings were prior-session reports, not ready candidates
  or acceptance evidence. The supporting [input-contract packet](../missing_input_contracts/README.md)
  checks current tracked source and preserves the minimally sufficient requests.

## Validation and delivery boundary

This is a documentation-only reconciliation. Existing integration JSON,
research receipts, B119 source and current merged PR metadata were inspected.
No new candidate compiler experiments or private ROM gates were run.
Documentation checks and repository regression results are recorded in [REVIEW.md](REVIEW.md).
No source or protected files changed. The queue does not claim the entire 550-function unmatched
population is blocked; it says the reviewed lanes contain no justified pair
of ready execution packets. No arbitrary percentage/date forecast is inferred.

The coordinator owns synthesis and publication. The maintainer owns protected
input examination, acceptance and serialization of image/compression/ROM
integration. A future matching submission still requires complete relocated
native-word equality, preserved neighbors, and the full private gates.
