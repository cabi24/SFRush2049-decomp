# dot_handoff — work queue for Rush 2049 helpers

For the full-project continuation strategy, current readiness corrections and detailed agent work packages, start with [PROJECT_PLAN.md](PROJECT_PLAN.md). Existing packet history remains below; check the plan before dispatch.

Updated after matching integration **d77aabb8** and incorporation of research PRs #9, #21, #25–39 and #41–49. Original snapshot: 2026-10-02.
This is the assignment and replenishment plan. Update its claims and results as work arrives.
No helpers are newly assigned by this document; names and ownership must be recorded before starting.

## Start here

1. Read [project rules](CLAUDE.md). Repo-only helpers also read [CloudHandoff.md](CloudHandoff.md); use the current numbers below because its opening statistics are historical.
2. Choose an available packet below, record its owner and exact addresses, and check current locks, open PRs and reservations.
3. Work in an isolated branch/worktree. Return complete C, compiler evidence and a short continuation note.
4. A maintainer reviews matching submissions and runs image/compression/ROM gates. Only then update accepted percentages.

The recommended split is one maintainer handling integration, two helpers handling game reconstruction/compiler work, and one helper scouting the next packets. With two helpers, alternate scouting and execution; with more, duplicate game lanes using disjoint addresses. GPT-6.1-sol at High reasoning is the user's preferred model for agent work when available.

## Project state and where the bytes are

| Population | Accepted functions | Accepted native bytes | Matching bytes |
|---|---:|---:|---:|
| Game image | 666 / 1,216 | 100,656 / 647,072 | 15.56% |
| Static cartridge code | 158 / 230 | 44,808 / 61,440 | 72.93% |

Combined byte tracker: **145,464 / 708,512 = 20.53%**. The separate population percentages retain their own denominators. There are 824 linked C functions. Existing static lock records include entries outside the counted cartridge population; lock-record counts are not coverage counts.

Latest accepted batch: PRs #24 and #40, 176 game bytes, with fresh complete image/compression/ROM gates. [Integration evidence](cloud/work/pr_integration_20261003/README.md). The preceding batch accepted thirteen PRs (#10–20, #22, #23), 1,252 game bytes. The preceding scheduler core added 872 code bytes; its verified 28-byte switch table earns no code credit. The full source-built image, original compressed stream and ROM hash passed; the earlier published CI run37071306233 passed; merged research master43a4302a also passed CI run37084943832. See [acceptance evidence](cloud/work/pr_match_review_20261002/README.md).

| Unmatched game function size | Functions | Native bytes remaining |
|---|---:|---:|
| Below 256 bytes | 135 | 22,592 |
| 256–1,023 bytes | 274 | 153,048 |
| 1,024–4,095 bytes | 135 | 243,304 |
| 4,096 bytes and above | 6 | 41,408 |
| Total | 550 | 460,352 |

There are also 72 remaining static functions occupying 16,632 bytes. Even accepting all of those would put the combined tracker at only **22.88%**. Reaching 50% requires **208,792 more matching bytes** from the current base, or **192,160 game bytes** after hypothetical completion of all static code. There is no evidence supporting a short completion date.

**Allocation:** put most effort into the 256–4,095-byte game population, build genuine caller/callee context where needed, and integrate completed matches promptly. Keep small residual work bounded. Scout the six very large functions before committing a team to one.

[Complete unmatched game inventory](docs/dot_handoff_targets.csv) lists all 550 targets, sorted by native bytes, with addresses and incorporated research PRs. It is a snapshot derived from `build/blob_layout.json` minus `blob_matched.lock.json`; it does not assert that a target is fresh, correctly named, or unclaimed. Recheck before assigning.

## Incorporated matching and research work

PRs #24 (`func_800B7360`,132 bytes) and #40 (`func_800A7830`,44 bytes) are merged and integrated, with complete original image/compressed-stream/ROM equality and all prior game locks preserved. D01 is complete for these two targets.

All 26 research/audit PRs in the reviewed #9/#21/#25–39/#41–49 batch are now merged into master. Their sources, tests and evidence are discoverable in the [research progress index](cloud/RESEARCH_INDEX.md). They earn no matching credit. Exact reviewed heads and review scope are recorded in [reviewed_prs.json](cloud/work/pr_integration_20261003/reviewed_prs.json).

The research audits supersede the original readiness assumptions:

- D02 returned **zero ready execution packets**; all four proposed substantial functions have real source/context prerequisites. PR #46 proves C974's three-pointer helper contract, but external `0x8038D798` remains unknown.
- D04's race_setup_1 and assign_drones already had complete broad nonmatches; their historical names describe the wrong subsystem. `func_800D1248` is **off limits** under the owner's newer [dot_response](dot_response).
- D08's best demonstrated closure is existing rank/distance catch-up code (`render_large_objects`), with a structural frame deficit. None of that shortlist is a fresh ready reconstruction.
- D06's proposed boolean expansion was tried and rejected by PR #44. Do not repeat it.
- PR #45 audits the pinned old HUD archive and corrects source/receipt and signedness/return assumptions. The interrupted minimap files and original JSON receipts have now been recovered on master, with incomplete source-to-receipt binding explicitly documented.
- PRs #48/#49 supply genuine renderer source and a useful closure continuation; see new D09 below.

Historical packet notes preserve their original dates/bases. Research CI and behavioral tests establish only their documented scope. Update the live queue using these findings before assigning more variants.

## Assignment queue

Owners start **unassigned**. Each helper should hold one active packet and one queued next packet. Reserve all actual context members that will be edited as well as the target. A read-only copy of an accepted helper is permitted; its bytes receive no repeat credit.

### D01 — integrate completed matches

- **Owner:** maintainer with private ROM inputs.
- **Completed:** #24 and #40,176 game bytes, integrated in d77aabb8; full-body/image/compression/ROM proofs passed.
- **Next:** review further completed matching PRs as they arrive. Batch independent verified functions to amortize ROM gates while preserving a per-function audit.

### D02 — scout beyond the rejected substantial shortlist

- **Owner:** scout; repo-only access is sufficient for native ABI/callgraph inspection.
- **Historical shortlist, now blocked under PR #43:** `func_800D91A0` @ `0x800D91A0` (3,860 bytes), `func_80102F30` @ `0x80102F30` (3,560), `func_80100E58` @ `0x80100E58` (2,732), `func_8010C974` @ `0x8010C974` (2,636).
- **Task:** use PR #47’s conservative source-definition inventory and the incorporated archive before expanding to other 256–4,095-byte functions. Audit native ABI, data and genuine closure requirements. C974 can resume only after the unknown external contract is established. Do not redispatch the four rejected targets as ready matching work.
- **Deliverable:** two ready packets, each containing target/hash identity, full source or a concrete full-reconstruction plan, actual field/prototype evidence, compiler recipe, context members, baseline and one testable next hypothesis. Explain rejection of the other candidates.
- **Next:** refill from the 256–4,095-byte inventory; maintain at least two ready packets per execution helper.

### D03 — reconstruct and match one selected large function

- **Owner:** game helper; target chosen from D02 after preflight.
- **Task:** reconstruct the entire body in natural IDO/C89 C. Establish true ABI and genuine helper contracts before tuning allocation. Compile a complete baseline, then diagnose the dominant mismatch.
- **Deliverable:** full source, provenance, strict full-word receipt and a concise residual explanation. If a real IPA closure is required, identify and reconstruct actual members; claims include only complete matching bodies.
- **Next:** second accepted D02 packet. For a difficult function, pair a reconstruction helper with a compiler helper on that one function using separate source/evidence paths.

### D04 — donor-backed medium game batches

- **Owner:** second game helper, independent addresses from D03.
- **Retired shortlist:** `race_setup_1` @ `0x800BD2C8` (1,884 bytes), `assign_drones` @ `0x800F4FEC` (1,236), `drone_set_catchup` @ `0x800A2990` (852), `func_800D1248` @ `0x800D1248` (324).
- **Task:** replace this shortlist. PR #43 establishes that race_setup_1 is palette/display-list animation and assign_drones is ranking/statistics work, both already complete broad nonmatches. Check older drone_set_catchup work before assigning it. Do not assign, compile, or investigate func_800D1248 until the owner lifts the restriction. Select a new disjoint batch only after source/context preflight.
- **Deliverable:** separate eligibility/baseline records, complete sources and per-member claims. A packet with no credible new hypothesis returns to scouting.
- **Next:** another disjoint medium batch from the inventory, then D03 if the scout produces a stronger large-function lead.

### D05 — shared HUD context and interrupted minimap research

- **Owner:** context/reconstruction helper; reserve both callbacks and any edited shared callees.
- **Targets:** `func_80109A60` @ `0x80109A60` (1,268 bytes), direct arcade `hud.c:AnimateDot`; `game_results_input` @ `0x800FF724` (1,732), actual player-marker callback.
- **Start:** [minimap reconstruction](cloud/work/module_campaign_20261002/reconstruction/minimap_dots/README.md) and [marker reconstruction](cloud/work/large_hud_marker/reconstruction/README.md). The marker has broad residuals; this is a context investigation, not an easy match.
- **Task:** the interrupted source and nineteen historical receipts are now preserved, with hashes and limitations. Map-offset O2/O3 remain broad nonmatches; no source-to-receipt identity is inferred where hashes are absent. PR #45’s marker replay also has documented source/receipt and relocation/excess issues. Reopen only with a specific new explanation. Compare actual `Input_ApplyPadConfig`, `SelectBlit`/`stat_race_update` and Hidden inline/call behavior across both real callers. Produce a specific explanation for the native frame/register geometry before another compile sequence.
- **Deliverable:** published, honestly labeled research packet or a complete strictly matching genuine closure. Local raw objects/native instructions remain ignored. Archive source/proof text through the existing research branch once reviewed.
- **Next:** a new scout-selected HUD/render function if no new context explanation emerges. Do not repeat the previous broad O2/O3 controls.

### D06 — bounded closure of the nearest substantial residuals

- **Owner:** compiler specialist; one reserved target at a time.
- **Frozen first lead:** `func_800E23A4` @ `0x800E23A4` (1,688), arcade `drivsym.c:forces1`. [Existing evidence](cloud/work/large_tire_forces/reconstruction/README.md) records 422 native words, native frame and verified pool, with two remaining stack-offset differences (120 versus 124) for a genuinely consumed rear traction boolean.
- **Frozen second lead:** `func_80087110` @ `0x80087110` (1,780), [clipped rectangle](cloud/work/large_texture_rect/layout_probe/README.md), four scheduling words remaining in an edge-addition block.
- **Then:** PR #27 array reset and PR #26 matrix rotation, if a new source/context hypothesis is justified.
- **Task:** PR #44 rejected the split traction-boolean control. Its cloud replay leaves six pool sites unverified, whereas the older protected private proof resolves them; keep that evidence distinction explicit. Reopen only with a different native source/context explanation, then independently replay any complete MATCH. Existing scalar splits, expression controls and broad sweeps are already exhausted in the substantial packets.
- **Deliverable:** actual complete match or a useful diagnosis stating why the hypothesis failed. A repeated unchanged residual returns the target to frozen status. Artificial locals, pressure, fake callers and ABI parameters are excluded.

### D07 — static remainder with ownership preflight

- **Owner:** static specialist; lower priority than game execution lanes.
- **Task:** derive current unpromoted cartridge slots and rank genuine libc/libm/libultra ancestry. Exclude already accepted SDK/VI work and distinguish hand-written assembly routines from viable C targets. Pick three compatible slots before reconstruction.
- **Deliverable:** target identities, current shared-TU flags and headers, exact body proofs and any genuine data/storage ownership proposal. Maintainer promotes through the static path and full ROM gate.
- **Constraints:** scheduler core and its table are done. Remaining scheduler timestamp variants lack storage proof; the complete `lib_5610` inflater closure is a frozen failure. Neither is a fresh default assignment. Mixed flags require proof that every already accepted neighbor stays exact.
- **Next:** next viable canonical-library packet; if none clears preflight, help D02 with game scouting rather than implementing speculative ownership infrastructure.

### D08 — very large functions: frozen reconnaissance

- **Owner:** scout after the ready queue is stocked.
- **Candidates:** `render_large_objects` @ `0x800F93A0` (5,652), `entity_spawn_init` @ `0x8008EA10` (5,544), `func_8009F058` @ `0x8009F058` (5,228), `stunt_combo_display` @ `0x800D3B28` (4,700). Larger candidates are `object_render` @ `0x80087A08` (10,048) and `render_display_list` @ `0x80099BFC` (10,236).
- **Task:** PR #42 found no immediate execution assignment. render_large_objects is rank/distance catch-up code with existing complete source, 1,361/1,413 differing words and a24-byte non-save frame deficit. Resume its native live-range/array audit only with a new structural explanation. Other shortlisted bodies retain earlier table/context blockers. Historical renderer names are hypotheses.
- **Deliverable:** a go/no-go packet for one function with the lowest unresolved context risk. Assign two helpers only after a credible complete-reconstruction route is identified.
- **Next:** execute that packet alongside a steady medium lane; keep the medium queue stocked during the longer investigation.

### D09 — genuine renderer closure from the new research

- **Owner:** source/context helper; reserve the whole edited family together.
- **Targets:** PR #48’s complete func_8008A46C (472 bytes), PR #49’s sound_init (400) and func_800878E0 (296). The already accepted func_80086A50 is genuine mode context with no repeated credit. Native helpers func_8008A148 and func_8008705C require complete actual-body/ABI audits before the closure is ready.
- **Task:** use the two new complete caller roots to establish authentic caller visibility and live values across calls. Recover/audit the remaining actual helper source; resolve the mode helper’s two local table references through protected original placement. Keep real mode/flags operations and observed command-buffer reloads.
- **Deliverable:** full source-context packet with exact per-member native geometry and protected relocations. The flag root’s O3 29/74 residual is allocation, not an isolated almost-match. No fabricated keepers, helper stubs or extra formal arguments.
- **Next:** execute a bounded natural closure experiment only after these prerequisites pass. Otherwise return the concrete missing contract to D02 and preserve the sources without more allocator sweeps.

### D10 — boot-segment tail runtime library (412 functions)

- **Owner:** Astra (repo-only dot worker); unclaimed until recorded here. Spec: [specs/015-boot-tail-runtime/](specs/015-boot-tail-runtime/spec.md), packets in [tasks.md](specs/015-boot-tail-runtime/tasks.md), inventory in `inventory.json`.
- **Targets:** 412 functions, 95,012 bytes, `0x8000F8D0–0x800268D0`. They are uncounted boot code past ROM `0x10000` and match no ultralib build. The 21 ultralib-identified functions, the 6 sub-16-byte stubs and the already matched `func_80010A00` are excluded. Ordinary single-function `-O1`/`-O2` matching applies; there is no `-O3` IPA.
- **Prerequisites:** packet 1 (identification and clustering into translation units) can start **now from master**. Scoring packets 2–6 need the owner to merge PR #52 and then PR #54, which add the `asm/us/boot_tail/` targets and CI rescoring of `cloud/matches/boot_tail/*.c`.
- **Deliverable:** true-zero matches in `cloud/matches/boot_tail/` and research/status in `cloud/work/boot_tail/` (`STATUS.csv`). This is not cartridge coverage. Promotion needs a maintainer static-layout extension to `0x800277D0`.
- **Do not touch:** the static layout, splat, `symbol_addrs`, locks, the generated targets, ovl_a/ovl_b, the 21 ultralib functions or production gates.

## Keeping the helpers supplied

For each assignment record:

```text
Packet / owner / branch / base commit:
Target IDs, addresses, full native byte extents:
Edited context members and overlap checks:
Status: ready | active | review | frozen | accepted
Complete baseline and dominant residual:
Current evidence-backed hypothesis:
Deliverable location / PR:
Next packet:
```

- Maintain a ready queue at least twice the number of execution helpers. Scouts replenish before it reaches zero.
- At each completed packet, log matching bytes accepted, research progress separately, blocker and next assignment. Notify the user of the refreshed percentages after actual accepted progress.
- Review after the first complete compile. If semantics/frame/ABI are wrong, repair those before scheduling experiments. If a function has broad allocation differences and no authentic context route, freeze it and take the next ready packet.
- Bound a hypothesis to one working session or roughly 20 directed variants, whichever comes first. Extend only for measurable movement or new native evidence. Historical plateaus are not reset by a new helper.
- Keep reconstruction, compiler evidence and integration ownership distinct through paths/branches. One maintainer serializes production splices and ROM gates; helpers submit research/matching sources.
- Refresh GitHub and current locks before dispatch. The old `build/overnight_reservations.json` contains stale reservations, including already accepted `osScAddClient`; it is not a live ownership board. Reconfirm external work on `func_800F8EC8` and `func_800D1AB0` before touching those closures.

## Submission and acceptance

**Matching submission:** complete self-contained C in `cloud/matches/<target>.c`, or a genuine group under `cloud/work/ipa-groups/<group>/`; exact flags/toolkit/source hashes; complete native extents; all relocations resolved; strict full-word MATCH including stack operands; actual ABI/caller/callee audit; table/pool proof where applicable. Group claims list only matching members. Include reproduction commands and assumptions about types/semantics.

**Research submission:** `cloud/work/<packet>/` with full source, status, native/candidate sizes, exact differing-word count, provenance, rejected controls and next hypothesis. Explicit NONMATCH label, no accepted coverage. Preserve existing licenses. Raw native instruction dumps, objects, ROM data and disassembly stay out of the new archival packet.

**Maintainer acceptance:** review the current exact PR revision and its CI; freshly compile actual standalone/group C; prove full relocated native bodies and unchanged accepted neighbors; rebuild the complete source-owned game image from real objects; prove original compressed stream and full ROM SHA-1. Static candidates additionally require current-TU/header/flag and data/storage integration. Protected target/scorer edits are not a way to close a residual.

Cloud examples:

```bash
bash tools/cloud/setup.sh
python3 tools/cloud/score.py fn cloud/matches/<target>.c <target>
python3 tools/cloud/score.py group cloud/work/ipa-groups/<group>
python3 tools/workbench.py guide
```

Use [current compiler settings](docs/COMPILER_SETTINGS.md) and actual recipe evidence; `-Wab,-r4300_mul` is required. Refer to [Conveyor operations](tools/conveyor/README.md) and the promotion workflow for maintainer integration rather than guessing CLI flags. Successful host semantics tests are useful evidence within their stated assumptions; they do not establish compiler or ROM equality. This handoff adds no tests and runs no new compiler/ROM gates.

## Frozen work and recovery notes

- Accepted large wins such as `camera_dolly` (actual tire friction), `init_state_begin`, `camera_smooth_follow` and `func_800E0B20` are already locked. Do not reassign based on historical percentages in their README files.
- Navigation `world_gravity_apply` (actual path-distance routine), controls/sym, full model audio, collision/position closures and tire-helper families have complete failed context controls archived in PR #9. Resume only with a specific new semantic/compiler explanation.
- Historical `drone_ai_update` and `Input_ProcessGameplayPad` are renderers; `mode_select_handler` at `800DEF68` is model audio. Use addresses and native evidence before a subsystem name.
- Preserve the pre-existing dirty `tools/mips_to_c` submodule and untracked research. Do not reset or clean them to prepare a helper checkout.
- Private recovery checkpoint: `.git/recovery/pre-compaction-20261002/handoff.json`, with twelve interrupted HUD source/evidence backups and nineteen compiler JSON receipts. The source/evidence files and historical JSON receipts are now published under the minimap/HUD packet paths, with incomplete source/receipt binding labeled. The private checkpoint itself remains local.
- Private maintainer integration checkout: `/tmp/rush2049-module-integration`; isolated builder: `watchman2:~/agents/module-integration-root/repo`. The older `/tmp/rush2049-pr-review.qchVXD` contains an already published staged acceptance patch and private octopus ancestry; do not republish it.
- Preserve actual object caches, `build/blob_layout.json`, `build/m2c_datasyms.json` and private original inputs. Missing compiled objects must not silently fall back to assembly when proving source coverage.
- The game blob sync does not sync static C/asm/config/linker/owned-data changes. Static integration needs an explicit sync to the isolated builder before its ROM gate. The shared builder checkout must not be overwritten.
- Two previous GPT-6.1-sol High HUD workers stopped at a usage limit. They are not producing background progress. Check availability before reusing that plan; do not silently substitute another model.

## Next dispatch after incorporation

1. Maintainer handles new completed matches and merged-master CI; D01’s current batch is done.
2. Source/context helper takes D09’s missing genuine renderer-helper and local-table prerequisites.
3. Scout takes D02 using the source inventory and merged research to find disjoint new packets. Ready packet count is currently zero; do not relabel blocked targets to fill that quota.
4. Extra helpers can independently audit compatible static slots (D07) or prove a genuinely missing contract from a frozen packet. Every assignment needs a concrete source/ABI/data question and a bounded deliverable.
5. Dispatch full reconstruction/matching once that prerequisite produces a credible whole-function route.

The current 50% target remains open. Matching byte gains and completed research/audit packets are reported separately.
