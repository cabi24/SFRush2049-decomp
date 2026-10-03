# Unaccepted decompilation research snapshot — 2026-10-02

This draft preserves candidate C, real helper context, ABI/type research and numerical compiler reports that were still local after the recent large-function rounds and older overnight work. It is a research archive, not an integration milestone. Accepted coverage stays **13.97% game** and **46.76% static**.

## Recent large-function leads

| Target | Native bytes | Retained work | Remaining issue |
| --- | ---: | --- | --- |
| `func_800E23A4` | 1,688 | [Complete tire-force C](../large_tire_forces/reconstruction/native_expression_scoped_tsc.c), [independent linked receipt](../large_tire_forces/compiler/native_expression_scoped_tsc_linked.json) | Two stack-spill offsets; all 422 opcode/register lanes and 12 pool bytes agree |
| `func_80087110` | 1,780 | [Complete texture rectangle C](../large_texture_rect/compiler/baseline.c), [audit](../large_texture_rect/compiler/REPORT.md) | Four scheduled words |
| `func_800F7F3C` | 1,396 | [Corrected ranking C](../large_rank_sort/compiler/native_semantics.c), [audit](../large_rank_sort/compiler/REPORT.md) | Correct complete opcode geometry; 270 register-operand differences |
| `entity_update_callback` | 2,184 | [Particle C with native literals](../large_particle_quad/reconstruction/native_literals.c), [findings](../large_particle_quad/reconstruction/README.md) | Frame/allocation differences; literal pool not accepted |
| `game_results_input` | 1,732 | [Complete HUD C](../large_hud_marker/reconstruction/baseline.c), [ABI audit](../large_hud_marker/compiler/ABI_REPORT.md) | Broad allocation/scheduling differences; real input-helper context also remains a nonmatch |
| `net_state_validate` | 2,728 | [Prior strict baseline](../large_net_state_validate/compiler/base_strict289.c), [audit](../large_net_state_validate/compiler/REPORT.md) | Structural/register differences; no new strict match |
| `wheel_render_full` | 2,072 | [Viewport cursor/address control](../large_viewport/compiler/record_cursor_intptr.c), [audit](../large_viewport/compiler/ABI_REPORT.md) | Correct frame in this control; instruction and register differences remain |

## Older research

The snapshot also preserves local textual packets under `near_miss_B*`, `tiny_A*`, `game_C*`, `static_C*`, and `ipa-groups/`. Read each packet's report and manifest before resuming it. These archives include competing controls and explicitly rejected historical drafts, so a file's presence does not endorse its semantics or make it the best candidate. Some older packets report exact isolated comparisons while still awaiting original-target, image, or ROM integration gates; they also receive zero new coverage in this PR.

The GNU-derived C79 formatter packet is excluded per the existing coordinator review; separately sourced BSD C81 replacements remain.

Existing tracked history is inherited from the base commit. The new [manifest](manifest.json) records every additional snapshot file's SHA-256 and byte size. Recorded results are historical evidence, not fresh replays performed for this PR. Paths naming ignored build artifacts or isolated compiler machines describe private reproduction artifacts and are not included files.

## Integration requirements

No accepted sources, locks, target masks, scorer, linker/build wiring, ROM bytes, raw instruction arrays, objects, or disassembly are changed by this snapshot. No tests or ROM gates were run for this research-only PR. A candidate must independently reproduce its protected full extent and all relocations/pools/tables, then pass the existing source-built image and full-ROM gates before integration or coverage credit.

The active visibility-cell reconstruction remains ongoing separately; it is not a frozen result in this snapshot.

## Follow-up module controls

The [module freeze manifest](module_freezes.json) preserves the later genuine inflater closure, corrected navigation/path types and actual tire-helper contexts. These are frozen nonmatches with zero new credit; their reports distinguish preserved accepted helper bodies from unaccepted callers. Current accepted master7c4d12c3 has19.83% combined matching bytes (15.32% game,67.28% static), independently gated outside this PR. The earlier snapshot percentages above describe its original base.

The follow-up manifest also preserves the bounded direct controls/sym donor reconstruction, position/corner updater, and tire donor controls. The controls wrapper recovers the native frame; the position updater recovers the full body length. Both remain nonmatches, with unresolved frame/allocation or protected literal-placement limitations explicitly recorded in their packets.

Further frozen packets preserve the complete model/audio reconstruction and independent ABI/table audit, collision controls, scheduler residual contexts, and visual donor work. Current accepted master ccc12360 is 20.11% combined; this research snapshot adds zero credit. Exact scheduler publication and the 100-byte visual helper were integrated separately on master.
