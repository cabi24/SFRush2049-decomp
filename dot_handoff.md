# dot_handoff — work queue for Rush 2049 helpers

Snapshot: **2026-10-02**, accepted `master` **a12daa63ae67c8dab3404efb666089c884dadfd4**.
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
| Game image | 664 / 1,216 | 100,480 / 647,072 | 15.53% |
| Static cartridge code | 158 / 230 | 44,808 / 61,440 | 72.93% |

Combined byte tracker: **145,288 / 708,512 = 20.51%**. The separate population percentages retain their own denominators. There are 822 linked C functions. Existing static lock records include entries outside the counted cartridge population; lock-record counts are not coverage counts.

Latest accepted batch: thirteen PRs (#10–20, #22, #23), 1,252 game bytes. The preceding scheduler core added 872 code bytes; its verified 28-byte switch table earns no code credit. The full source-built image, original compressed stream and ROM hash passed; published CI run 37071306233 passed. See [acceptance evidence](cloud/work/pr_match_review_20261002/README.md).

| Unmatched game function size | Functions | Native bytes remaining |
|---|---:|---:|
| Below 256 bytes | 137 | 22,768 |
| 256–1,023 bytes | 274 | 153,048 |
| 1,024–4,095 bytes | 135 | 243,304 |
| 4,096 bytes and above | 6 | 41,408 |
| Total | 552 | 460,528 |

There are also 72 remaining static functions occupying 16,632 bytes. Even accepting all of those would put the combined tracker at only **22.85%**. Reaching 50% requires **208,968 more matching bytes** from the current base, or **192,336 game bytes** after hypothetical completion of all static code. There is no evidence supporting a short completion date.

**Allocation:** put most effort into the 256–4,095-byte game population, build genuine caller/callee context where needed, and integrate completed matches promptly. Keep small residual work bounded. Scout the six very large functions before committing a team to one.

[Complete unmatched game inventory](docs/dot_handoff_targets.csv) lists all 552 targets, sorted by native bytes, with addresses and known open PRs. It is a snapshot derived from `build/blob_layout.json` minus `blob_matched.lock.json`; it does not assert that a target is fresh, correctly named, or unclaimed. Recheck before assigning.

## Immediate incoming work

GitHub was queried during this handoff. All nine open PRs below had successful `verify` checks. Compiler/semantic counts here are submission claims, except where earlier local evidence is explicitly cited; this handoff did not independently replay the new PRs.

| PR | Function / bytes | Current submission status | Next action |
|---|---|---|---|
| [#24](https://github.com/cabi24/SFRush2049-decomp/pull/24) | `func_800B7360`, 132 | Strict MATCH claimed; source-only draft | Maintainer: review actual C at head `b2b41ad324c03fcfea3ba843acd9b79ef7ea5553`, freshly compile and resolve every word, then splice and run all cartridge gates. Zero accepted credit so far. |
| [#27](https://github.com/cabi24/SFRush2049-decomp/pull/27) | `func_800F7EB0`, 140 | NONMATCH, 4/35 words differ | One bounded genuine scheduling investigation; return diagnosis if unchanged. |
| [#26](https://github.com/cabi24/SFRush2049-decomp/pull/26) | `func_800B5898`, 168 | NONMATCH, 5/42 words differ | One bounded float-web investigation; coordinate any related matrix-family context. |
| [#25](https://github.com/cabi24/SFRush2049-decomp/pull/25) | `func_80091B00`, 168 | NONMATCH, 18/42 words differ | Reuse verified allocator semantics as actual context; current source already existed as unclaimed context. |
| [#28](https://github.com/cabi24/SFRush2049-decomp/pull/28) | `func_800EC270`, 136 | NONMATCH, 19/34 words differ | Preserve corrected byte offset 2012 and counter wrap; revisit only with a new address-allocation/context hypothesis. |
| [#29](https://github.com/cabi24/SFRush2049-decomp/pull/29) | `audio_output_setup` @ `800BAF98`, 148 | Typed source NONMATCH, 24/37 words differ | Use natural typed source; rejected pointer-erasure control is not an acceptance candidate. |
| [#30](https://github.com/cabi24/SFRush2049-decomp/pull/30) | `func_800B3704`, 228 | NONMATCH, 41/57 words differ | Preserve callback order and resource reread; actual pool-family context may support a future closure. |
| [#21](https://github.com/cabi24/SFRush2049-decomp/pull/21) | `func_800A1644`, 716 | NONMATCH, 149/179 words differ | Archived Controller Pak encoder; resume only with a specific ABI/allocation explanation. |
| [#9](https://github.com/cabi24/SFRush2049-decomp/pull/9) | Large research archive | Frozen sources and evidence; no coverage | Read its snapshot index before selecting old targets. Audit and preserve interrupted minimap work as the next archive update. |

Do not merge research submissions under the matching review policy. Successful PR CI checks protected paths and existing matches; it does not imply that unclaimed research matches or that its standalone behavioral harness ran in CI. Read each submitted `STATUS.md` and `verification.json` before continuation.

Some linked older research packets exist locally and on PR #9's research branch rather than accepted master. Repo-only helpers should inspect that PR in a separate checkout to obtain its indexed packets; the interrupted minimap additions still need publication. New PR #25–30 sources likewise live on their respective branches. Do not treat an absent master file as permission to repeat the archived work.

## Assignment queue

Owners start **unassigned**. Each helper should hold one active packet and one queued next packet. Reserve all actual context members that will be edited as well as the target. A read-only copy of an accepted helper is permitted; its bytes receive no repeat credit.

### D01 — integrate the pending completed match

- **Owner:** maintainer with private ROM inputs.
- **Target:** `func_800B7360` @ `0x800B7360`, 132 bytes; PR #24.
- **Deliverable:** complete native-word proof, preserved previous locks, source-built image equality, compressed-stream equality, full ROM SHA-1 and final CI result. Record exact reviewed revision.
- **Next:** review further completed matching PRs as they arrive. Batch independent verified functions to amortize ROM gates while preserving a per-function audit.

### D02 — scout the next substantial game functions

- **Owner:** scout; repo-only access is sufficient for native ABI/callgraph inspection.
- **First shortlist:** `func_800D91A0` @ `0x800D91A0` (3,860 bytes), `func_80102F30` @ `0x80102F30` (3,560), `func_80100E58` @ `0x80100E58` (2,732), `func_8010C974` @ `0x8010C974` (2,636).
- **Task:** audit previous complete work, registered extents, entry ABI, frame, actual callers/callees, literals/tables and ancestry. These are unlocked inventory candidates, not established fresh opportunities. Choose the first two with tractable native context and complete semantics.
- **Deliverable:** two ready packets, each containing target/hash identity, full source or a concrete full-reconstruction plan, actual field/prototype evidence, compiler recipe, context members, baseline and one testable next hypothesis. Explain rejection of the other candidates.
- **Next:** refill from the 256–4,095-byte inventory; maintain at least two ready packets per execution helper.

### D03 — reconstruct and match one selected large function

- **Owner:** game helper; target chosen from D02 after preflight.
- **Task:** reconstruct the entire body in natural IDO/C89 C. Establish true ABI and genuine helper contracts before tuning allocation. Compile a complete baseline, then diagnose the dominant mismatch.
- **Deliverable:** full source, provenance, strict full-word receipt and a concise residual explanation. If a real IPA closure is required, identify and reconstruct actual members; claims include only complete matching bodies.
- **Next:** second accepted D02 packet. For a difficult function, pair a reconstruction helper with a compiler helper on that one function using separate source/evidence paths.

### D04 — donor-backed medium game batches

- **Owner:** second game helper, independent addresses from D03.
- **Shortlist for preflight:** `race_setup_1` @ `0x800BD2C8` (1,884 bytes), `assign_drones` @ `0x800F4FEC` (1,236), `drone_set_catchup` @ `0x800A2990` (852), `func_800D1248` @ `0x800D1248` (324).
- **Task:** prove native purpose first; historical names are hypotheses. Check older packets and actual arcade ancestry. Select two or three related functions with ordinary ABI or a small genuine closure, and deliver incremental matches instead of waiting on the full family.
- **Deliverable:** separate eligibility/baseline records, complete sources and per-member claims. A packet with no credible new hypothesis returns to scouting.
- **Next:** another disjoint medium batch from the inventory, then D03 if the scout produces a stronger large-function lead.

### D05 — shared HUD context and interrupted minimap research

- **Owner:** context/reconstruction helper; reserve both callbacks and any edited shared callees.
- **Targets:** `func_80109A60` @ `0x80109A60` (1,268 bytes), direct arcade `hud.c:AnimateDot`; `game_results_input` @ `0x800FF724` (1,732), actual player-marker callback.
- **Start:** [minimap reconstruction](cloud/work/module_campaign_20261002/reconstruction/minimap_dots/README.md) and [marker reconstruction](cloud/work/large_hud_marker/reconstruction/README.md). The marker has broad residuals; this is a context investigation, not an easy match.
- **Task:** audit the interrupted map-offset compiler receipts, correct the pending status, and freeze the complete source/evidence. Compare actual `Input_ApplyPadConfig`, `SelectBlit`/`stat_race_update` and Hidden inline/call behavior across both real callers. Produce a specific explanation for the native frame/register geometry before another compile sequence.
- **Deliverable:** published, honestly labeled research packet or a complete strictly matching genuine closure. Local raw objects/native instructions remain ignored. Archive source/proof text through the existing research branch once reviewed.
- **Next:** a new scout-selected HUD/render function if no new context explanation emerges. Do not repeat the previous broad O2/O3 controls.

### D06 — bounded closure of the nearest substantial residuals

- **Owner:** compiler specialist; one reserved target at a time.
- **First:** `func_800E23A4` @ `0x800E23A4` (1,688), arcade `drivsym.c:forces1`. [Existing evidence](cloud/work/large_tire_forces/reconstruction/README.md) records 422 native words, native frame and verified pool, with two remaining stack-offset differences (120 versus 124) for a genuinely consumed rear traction boolean.
- **Second:** `func_80087110` @ `0x80087110` (1,780), [clipped rectangle](cloud/work/large_texture_rect/reconstruction/README.md), four scheduling words remaining in an edge-addition block.
- **Then:** PR #27 array reset and PR #26 matrix rotation, if a new source/context hypothesis is justified.
- **Task:** start with workbench diagnosis and the prior rejected controls. Try one evidence-backed hypothesis with a bounded experiment, then independently replay any complete MATCH. Existing scalar splits, expression controls and broad sweeps are already exhausted in the substantial packets.
- **Deliverable:** actual complete match or a useful diagnosis stating why the hypothesis failed. A repeated unchanged residual returns the target to frozen status. Artificial locals, pressure, fake callers and ABI parameters are excluded.

### D07 — static remainder with ownership preflight

- **Owner:** static specialist; lower priority than game execution lanes.
- **Task:** derive current unpromoted cartridge slots and rank genuine libc/libm/libultra ancestry. Exclude already accepted SDK/VI work and distinguish hand-written assembly routines from viable C targets. Pick three compatible slots before reconstruction.
- **Deliverable:** target identities, current shared-TU flags and headers, exact body proofs and any genuine data/storage ownership proposal. Maintainer promotes through the static path and full ROM gate.
- **Constraints:** scheduler core and its table are done. Remaining scheduler timestamp variants lack storage proof; the complete `lib_5610` inflater closure is a frozen failure. Neither is a fresh default assignment. Mixed flags require proof that every already accepted neighbor stays exact.
- **Next:** next viable canonical-library packet; if none clears preflight, help D02 with game scouting rather than implementing speculative ownership infrastructure.

### D08 — very large rendering functions: reconnaissance only first

- **Owner:** scout after the ready queue is stocked.
- **Candidates:** `render_large_objects` @ `0x800F93A0` (5,652), `entity_spawn_init` @ `0x8008EA10` (5,544), `func_8009F058` @ `0x8009F058` (5,228), `stunt_combo_display` @ `0x800D3B28` (4,700). Larger candidates are `object_render` @ `0x80087A08` (10,048) and `render_display_list` @ `0x80099BFC` (10,236).
- **Task:** validate actual purpose, ancestry, prior work, native boundaries, data dependencies and compiler closure. Full native function extents are authoritative; substring/prefix matches do not establish a whole-body match.
- **Deliverable:** a go/no-go packet for one function with the lowest unresolved context risk. Assign two helpers only after a credible complete-reconstruction route is identified.
- **Next:** execute that packet alongside a steady medium lane; keep the medium queue stocked during the longer investigation.

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
- Private recovery checkpoint: `.git/recovery/pre-compaction-20261002/handoff.json`, with twelve interrupted HUD source/evidence backups and nineteen compiler JSON receipts. This private checkpoint is not available to repo-only helpers; publish the audited textual research needed for D05.
- Private maintainer integration checkout: `/tmp/rush2049-module-integration`; isolated builder: `watchman2:~/agents/module-integration-root/repo`. The older `/tmp/rush2049-pr-review.qchVXD` contains an already published staged acceptance patch and private octopus ancestry; do not republish it.
- Preserve actual object caches, `build/blob_layout.json`, `build/m2c_datasyms.json` and private original inputs. Missing compiled objects must not silently fall back to assembly when proving source coverage.
- The game blob sync does not sync static C/asm/config/linker/owned-data changes. Static integration needs an explicit sync to the isolated builder before its ROM gate. The shared builder checkout must not be overwritten.
- Two previous GPT-6.1-sol High HUD workers stopped at a usage limit. They are not producing background progress. Check availability before reusing that plan; do not silently substitute another model.

## First dispatch

1. Maintainer takes D01 and handles incoming completed matches.
2. Scout takes D02, returning two preflighted substantial targets and replenishing the queue.
3. Game helper takes D04 while those packets are being prepared, then D03.
4. Compiler/context helper takes one justified D06 hypothesis, then D05 or the second D03 packet.
5. Extra helpers take disjoint D04 batches, D07, or D08 reconnaissance as queue depth permits.

The next material milestone should be a repeatable flow of accepted game batches, with verified byte gains per completed packet. The current 50% target remains open.
