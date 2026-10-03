# Research progress on master

Updated after incorporation of PRs #9, #21, #25–39 and #41–49. Matching PRs #24 and #40 are separately integrated through the cartridge gates. Exact reviewed heads and review scope are recorded in [reviewed_prs.json](work/pr_integration_20261003/reviewed_prs.json).

**Current verified matching coverage:** game 666/1,216 functions, 100,656/647,072 bytes (15.56%); static 158/230 functions, 44,808/61,440 bytes (72.93%). Combined byte tracker: **20.53%**.

Research is now preserved on master under `cloud/work/`, with its submitted evidence, reproduction tools and existing regression tests. It contributes readable source, tested behavior within explicit assumptions, ABI contracts, compiler diagnoses and corrected work assignments. It remains outside the source-built ROM until strict native-word and cartridge gates pass. A completed research packet is useful progress even when it is a NONMATCH.

## Reconstruction and behavioral evidence

The eighteen focused packets below document nineteen named routines; some extend or validate prior source rather than recover a new function. This is a research index count, not a coverage count. Numerical residuals are the frozen submitted comparisons, not fresh compiler replays by the integrating maintainer. Read each linked packet for actual ELF body size, excess words, relocation status, preconditions and tested domains.

| PR | Routine | Documented compiler result | Useful result / continuation limit |
|---|---|---|---|
| #21 | [Controller Pak encoder, `func_800A1644`](work/larger_func_800A1644/README.md) | 149/179 words differ | Full normalization/encoding source and bounded native/host differential checks; allocation remains broad. |
| #25 | [Slot allocator, `func_80091B00`](work/dot_slot_allocator/STATUS.md) | 18/42 | Isolates existing actual allocator context and adds boundary/state-preservation evidence. |
| #26 | [Matrix rotation, `func_800B5898`](work/dot_matrix_rotation/STATUS.md) | 5/42 | Removes historical artificial padding while retaining the native frame; float-web exchange remains. |
| #27 | [Array reset, `func_800F7EB0`](work/dot_array_reset/STATUS.md) | 4/35 | Complete natural loops; residual is a four-word invariant-load scheduling block. |
| #28 | [State initializer, `func_800EC270`](work/dot_state_initializer/STATUS.md) | 19/34 | Corrects byte offset 2012 and preserves disabled-path state and 32-bit counter wrap. |
| #29 | [List-count helper, `audio_output_setup`](work/natural_audio_output_setup/STATUS.md) | Typed source 24/37 | Validates acquisition/default-list ordering and wrapping count; lower-score type-erasure control is rejected. |
| #30 | [Pool constructor, `func_800B3704`](work/dot_pool_object/STATUS.md) | 41/57 | Callback-safe resource reread, coordinate preservation and handle narrowing. |
| #31 | [Cylinder predicate, `func_8010C448`](work/dot_cylinder_predicate/STATUS.md) | 45/80 | Actual typed strides, signed selector, radius-before-disable ordering and bounded differential proof. |
| #32 | [Low-cylinder predicate, `func_8010C588`](work/dot_low_cylinder/README.md) | 43/80 | Related complete typed source with the distinct vertical limit; frame/instruction gap remains. |
| #33 | [Viewport initializer, `func_800A5A40`](work/dot_viewport_init/STATUS.md) | 15/63 | Genuine nine-argument call and post-callback dimension reloads; historical name is not an ABI contract. |
| #34 | [Force adjustment, `func_800E1AA0`](work/dot_force_adjustment/STATUS.md) | 40/100 | Explicit O32 field views and causal improvement from genuine accumulation order. |
| #36 | [Filtered bounds, `func_800B9740`](work/dot_vertex_bounds/README.md) | 100/102, 3 nonzero excess words | Portable pointer source, sentinel/range semantics and write-trace proof; does not beat every old control. |
| #37 | [Entity motion, `func_8010E4E4`](work/dot_entity_motion/README.md) | 72/108 | Typed velocity subobject and native scale snapshot, callback mutation and complete bounded differential checks. |
| #38 | [Resource initializer, `func_8010D85C`](work/dot_resource_init/STATUS.md) | Primary O2 73/92 | Correct halfword output contract, signed narrowing and callback-visible state rereads. |
| #39 | [Name cache, `func_800F1D04`](work/dot_name_cache/STATUS.md) | 104/222 | Existing source gains executable evidence and a real all-age-wrap uninitialized-index counterexample. Undefined state remains explicitly excluded from defined host-C claims. |
| #41 | [Range reduction, `func_800A557C`](work/dot_rational_reduction/STATUS.md) | 71/114 | Replaces undefined historical controls with genuine initialized dependencies and bounded numerical/call proof. Native coefficient values and precise identity remain unestablished. |
| #48 | [Solid rectangle, `func_8008A46C`](work/solid_rectangle_8008A46C/README.md) | 109/118 | Additional five-argument caller semantic tests/replay; complete caller/helper source already exists in PR #9 B119. Authentic TU/export-root evidence is still missing. |
| #49 | [Graphics initialization and flags](work/dot_graphics_init_closure/README.md) | `sound_init` 75/100; genuine O3 flags 29/74 | Additional ABI/behavioral evidence for roots already present in B119/A151. Flags reach native geometry; two references to one mode table remain unverified in this research replay. |

## Audits that change the next assignments

| PR | Packet | Result |
|---|---|---|
| #42 | [Very large target scout](work/dot_d08_scout/README.md) | Immediate execution is NO-GO. `render_large_objects` is rank/distance catch-up work, with existing full source and a 24-byte frame deficit, not a fresh renderer. Other candidates retain old context/table blockers. |
| #43 | [Substantial-function scout](work/dot_substantial_scout/STATUS.md) | Zero ready packets from the original four-function shortlist; corrects missing caller and placeholder-source assumptions. D04 suggestions already have complete drafts. |
| #44 | [Tire stack-home hypothesis](work/d06_tire_stack_diagnosis/README.md) | Real boolean expansion failed; do not repeat it. The cloud replay's six pool references remain unverified, distinct from the earlier private protected pool proof. Two-word masked residual is not acceptance. |
| #45 | [HUD receipt/frame audit](work/d05_hud_context_audit/README.md) | Corrects marker source/receipt identity, exposes real group errors/excess, proves signed minimap slot and reload-derived return, and records what the pinned archive lacked. |
| #46 | [C974 helper contract](work/c974_call_contract/README.md) | Proves the three-pointer call and local f12 reaching definition; unknown external `0x8038D798` still blocks full execution. |
| #47 | [Source definition inventory](work/source_inventory/README.md) | Conservative read-only Git source index fixes macro/multiline false negatives. Presence is not completeness; absence is not freshness. Use it before assigning repeated work. |
| #35 | [Historical worker stop](../docs/history/2026-10-02-lane-f-automated-safety-stop.md) | Preserves an unexplained notification with unknown target/action; does not classify game code. Current owner pause for `func_800D1248` is separately authoritative in [dot_response](../dot_response). |

## Earlier archive and recovered interrupted work

PR #9 makes the [research snapshot index](work/research_snapshot_20261002/README.md) available on master. Its original manifest's 1,549 files and extension's 177 files were verified against their recorded hashes before incorporation. Historical exact-object leads, rejected source controls and broad failed closures retain their labels. The integrating maintainer did not rerun every historical compiler experiment or assert that the archive's old scores describe its current source.

The previously unpublished [minimap source packet](work/module_campaign_20261002/reconstruction/minimap_dots/README.md) and [HUD context sources/receipts](work/module_campaign_20261002/ai/hud_dot/frozen_receipts/manifest.json) are now preserved. Twelve source/evidence files and nineteen historical JSON receipts were recovered and checked against the pre-compaction checkpoint. O2/O3 map-offset outcomes remain broad NONMATCHs. Receipt/source binding is incomplete, section sizes are not function extents, and no fresh compiler or full behavioral proof was run for this recovery. PR #45 remains a correct audit of its explicitly pinned older archive, even though current master now has the missing files.

## What was reviewed and how to continue

The maintainer reviewed the focused submitted C, native ABI claims and semantic limits, reproduction-tool operations, added regression scope, exact heads and existing successful PR CI. Source and harness assumptions remain documented per packet; host emulators/mocks and synthetic constants are bounded evidence. The archive was reviewed as preservation with verified manifests. The maintainer independently ran fresh complete native/image/compression/ROM gates for #24 and #40, and did not replay every research harness locally. Final merged-master CI checks the combined repository.

Use [dot_handoff](../dot_handoff.md) for current dispatch. The [reconciled queue](work/program_queue_20261003/README.md) contains zero justified execution-ready packets in the reviewed lanes. Renderer helper bodies already exist in PR #9 B119: archived rectangle 71/118, flags 19/74 and initializer 70/100 are better than the later receipts, without being matches. The next renderer prerequisite is authentic original TU/export-root and caller allocation evidence, plus exact-candidate proof for two references to one 20-byte mode table; see the [renderer checklist](work/graphics_readiness_20261003/README.md). Do not repeat the already tried helper closure. C974 is waiting for an external contract; the [minimal input handoff](work/missing_input_contracts/README.md) specifies identity, ABI, side effects and compiler visibility; D02/D04/D08 should not repeat their rejected shortlist. Frozen numerical near-misses require a new native explanation before more variants.

Keep matching coverage and research status separate in updates. Change a routine's acceptance status only after production integration gates pass; no C file or passing semantic test earns native-byte credit by itself.
