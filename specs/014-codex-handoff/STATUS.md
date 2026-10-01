# Coordinator status (Codex maintains this; newest entry first)

## 2026-10-01 second game integration and static preparation

- Game cartridge C: **512/1,216 functions, 59,204/647,072 bytes (9.15%)**. Static cartridge C remains **34/230 functions, 2,456/61,440 bytes (4.00%)**, pending sixteen additional object matches / 1,852 slot bytes.
- Accepted eight game functions / 1,208 bytes: random_seed_init, func_8008D0C0, task_complete_signal, input_aux_handler, mode_byte_set, mode_byte2_set, object_type_byte2_get, object_type_byte3_get. Coordinator independently strict-rescored every delivered single/group and passed image/full-ROM gates.
- Real sound closure uses audited real sound body and empty debug callee; unreachable switch inhibits inlining. Only four exact callers are claimed. Real input group uses audited real game_loop caller, no synthetic context. Task uses already-locked real func_8008AD04 context for compiler module alignment; invented PG helpers remain unaccepted. random_seed_init uses a code-free argument read to reorder webs; D0C0 simplifies its natural loop without a quirk.
- Fixed context relocation range overlap with a shorter compiled context function: stop address mapping at next compiled function. Regression verifies correct following-callee address and preserves refusal for uncovered stand-ins.
- Static targets: refreshed osPiRawReadWord through supported scoped regeneration; converted-slot index restores real relocation metadata. Three GU wrappers needed canonical float-register alias declarations in isolated target assembly. Existing round-trip gate passes all four, no scorer masks widened. Standard supersession retires stale target evidence.
- Independently verified exact C2 eight and C3 seven sources plus osPiRawReadWord on Rocky D: stack-sensitive score zero and raw object words zero. Locked all sixteen with their exact flags. Converted nine additional static segments and explicitly synced sources/assembly/config/linker to builder.
- Full rebuild of every ROM TU caught and resolved signed bzero_alt declaration conflict, preserving existing locked body. Corrected osDpSetNextBuffer prototype to target s32(void*, u64). All shared-header additions and passthrough conversions now pass full-ROM SHA-1 EXACT.
- Gates: blob lock 512 entries, zero problems; group lock zero problems; full pytest exit 0 captured separately; full source-built image and ROM exact. Latest prior push de6f73cf passed CI (run 36886013870). No open cloud PRs.
- Three workers continue next static, near-miss and real IPA packets. Farm harvesting remains paused for integration; active jobs preserved. Wiki host-key confirmation remains pending.

## 2026-10-01 follow-up integration

- Game cartridge C: **504/1,216 functions, 57,996/647,072 bytes (8.96%)**. Static cartridge C: **34/230 functions, 2,456/61,440 bytes (4.00%)**. Session gains: seven game functions / 1,580 bytes, eleven static functions / 1,008 bytes.
- Accepted save_load_data (436 bytes): replacing callback address literal with the real drone_ai_update symbol resolves the final ori/addiu difference. Coordinator independently obtained strict MATCH, then image and full-ROM SHA-1 EXACT. No dead-read or unused-local quirk introduced.
- Added safe whitespace-only line_join cloud mutation family. Seven regressions pass; known pilot reproduces strict MATCH, adding no coverage. Fixed bounded held-out protocol: before 0/8, after 0/8, measured gain zero. Detailed reproducible evidence in cloud/work/line_join_codex.md.
- Full pytest passed with exit 0 captured separately; blob lock 504 entries and group lock have zero problems. Live farm fixture now skips already-ROM-promoted strlen before seeding, preserving the production re-seed guard and accepted source.
- Previous integration push passed GitHub CI (run 36882861035); this follow-up push will be checked separately. Farm harvesting resumes after commit/push; active searches preserved.
- Remaining one-word mode_byte2_set and object_type_byte2_get/object_type_byte3_get need the real sound_update_channel IPA closure. Stand-ins remain unclaimable. Wiki host-key confirmation remains pending.

## 2026-10-01 first complete integration milestone

- Game cartridge C: **503/1,216 functions, 57,560/647,072 bytes (8.90%)**. Static cartridge C: **34/230 functions, 2,456/61,440 bytes (4.00%)**.
- Gain this batch: six game functions / 1,144 bytes; eleven static functions / 1,008 bytes. Six switch heads registered separately (9,472 bytes of extents; no C credit).
- All eleven static sources promoted through lock/promote into the actual ROM TUs: osGetActiveQueue, inflate_flush_window, viDeadlinePassed, osPiReleaseAccess, osPiGetAccess, osViGetFramebuffer, osPiInit, osSiInit, osSetIntMask, osContStartReadData2, osViSetSpecialFeatures. Every promotion rebuilt its TU and passed the full-ROM hash gate before its automatic commit.
- Static context mismatch caught and resolved: inflate_flush_window's first promotion failed because shared gDisplayListSize is volatile. Rollback restored the exact ROM. Candidate now uses a nonvolatile view for that RAM store, independently reverified and relocked; existing huft_alloc declaration/source preserved. This is a documented source quirk.
- Refreshed near-miss VERDICTS from the committed diagnose tool (195b7bc1) on real Rocky objects, exit 0. Diagnostic counts remain leads, not strict matches.
- Gates: final source-built game image and full-ROM SHA-1 EXACT; blob lock 503 entries, zero problems; group lock zero problems; all 37 static/source locks intact. Final full pytest passed (exit 0 captured separately; environment-dependent tests skipped).
- Workstreams now cover both local and cloud handoffs. Remaining priorities: forty close static candidates; real closures for input_aux_handler/C7200/8ABE4; new head func_8010C2E4 (three words away); r4300_mul historical resweep; verdict-routed mutators judged on clean held-out data; IPA probe integration/seed repair. Existing cloud known blockers retained; no extra brute-force budget assigned to exhausted closures.
- No open cloud PRs at intake. First integration/head-registration push CI passed: run 36881032233. Final promotion push CI will be checked separately.
- Persisted coordinator reconciliation: 36 forward status transitions, static promotions marked verified, image locks marked matched, zero open jobs to cancel.
- Farm harvesting will resume after the final gates/push; searches were preserved. Rocky orphaned uopt count is zero and its current load is below 1.
- Wiki publishing remains blocked by changed SSH host key at 192.168.50.30; awaiting independent fingerprint confirmation. No host-key trust bypass performed.

## 2026-10-01 static promotion preparation

- Converted seven static segments to passthrough ROM TUs: 0x79a0, 0x83d0, 0x8920, 0xa330, 0xd260, 0xe7c0, 0xf160. No new static C coverage yet.
- Explicitly synced converted ROM sources, assembly (including stale whole-segment removals), splat config and linker script to watchman2. Rebuilt all seven new TUs and verified full-ROM SHA-1 exact before any promotion. `blob_rom rom` alone only syncs the compressed game blob/Makefile, so it cannot establish this static conversion baseline without that source sync.
- Conversion exposed a real seeding/extraction gap: static assembly indexes only read top-level asm/us/*.s. Both now include converted asm/us/nonmatchings/rom slots, excluding game blob assembly; synthetic regression plus existing static context regression exercise the new path.
- Eleven static candidates independently verified on Rocky D (stack-sensitive score zero and exact raw object words, content hashes recorded in build/codex-static-verify/results.json); pool locks are supplemental evidence because compile_score defaults to stack-masked scoring.
- Wiki update blocked by changed SSH host key at 192.168.50.30; requested independent fingerprint confirmation, preserved known_hosts. Repository status remains authoritative.

## 2026-10-01 switch-head registration

- Registered all six previously proved switch heads: func_8010221C (548 bytes), func_80102F30 (3,560), func_80104B14 (2,404), func_80105480 (1,780), func_8010D3C0 (704), func_8010D680 (476). Added 9,472 bytes of registered extents, no new C coverage claimed.
- Game population now 1,216 functions; accepted C remains 503 functions / 57,560 of 647,072 bytes (8.90%). Static remains 23/230 functions / 1,448 of 61,440 bytes (2.36%).
- Seventh head func_80104704 still refuses: branch at 0x80104A48 enters existing highscore_entry_anim at 0x80104A58. No shared-tail registration attempted.
- Removed stale blob_80104b14.s; generated current regions/linker/symbols/manifest through blob_tu. Cloud head tests updated to the six registered extents and sole remaining audited head, with explicit extent assertions.
- ROM from the regenerated layout: SHA-1 EXACT; both blob checks zero problems. Full pytest gate passed (exit 0) before commit.
- Small-head worker final log and func_8010C2E4 best (three strict words away) retained as a nonmatch. Next: static passthrough conversions and eleven promotions.

## 2026-10-01 Codex integration batch 1

- Environment verified: SSH and IDO compile smoke on Rocky/watchman2, local Git/Conveyor write probes, remote Git read/push dry run. Removed 21 confirmed orphaned uopt processes (PPID 1, deleted permjob directories, 51–61 hours old); active search workers preserved.
- Cloud workstreams incorporated from CloudHandoff.md, CloudHandoffV2.md, and cloud/HANDOFF_AUTOMATION.md. No open cloud PRs at intake. Three concurrent workers (session limit) covered static candidates, near-misses/new heads, and real IPA groups/tooling.
- Game code: 503/1,210 functions, 57,560/647,072 bytes (8.90%). Static cartridge code remains 23/230 functions, 1,448/61,440 bytes (2.36%) pending promotion.
- Spliced: real IPA group func_800B73E4 + func_800B7438 (180 bytes; input_init_flag_get locked context, no stand-ins), refused singles func_800BB7F4 + render_post_process (324 bytes), and new-head singles func_8010C7F4 + func_80105DA8 (640 bytes). Coordinator independently rescored exact delivered files, then passed image gates and full-ROM SHA-1 EXACT.
- Fixed standalone placement of known externally visible unit-defined data: PROVIDE does not override ELF definitions, so linker now assigns those symbols their image addresses. Tests exercise .data, .bss, common definitions, LO16 carry, and preserving function placement. Source quirks (defined symbols and C7F4 unused local for frame/home offsets) are intentional and documented.
- Static worker delivered eleven independently verified stack-sensitive/raw-word matches (1,008 slot bytes); pool locks verified too. Two historical stack-masked false zeros repaired by local declaration order. osPiRawReadWord remains blocked by raw target relocation attribution; no coverage credited.
- Cloud suite already passes; the seven stale ipakit failures in the handoff are resolved in this checkout. Fixed live known-pairing test prerequisite: an unscored arcade pairing cannot be ranked from m2c-only cells; synthetic test proves scored out-of-top-five pairings still fail.
- Cloud report disassembly now falls back to the existing stdlib MIPS decoder when binutils is absent/broken; four regressions and real Rocky report checked. Scores unchanged.
- Exhausted near-miss packet: 62 bounded variants, no new matches (2/2/3/4/8-word residuals). Real func_8008ABE4 group still six words away; func_800C7200 real seed blocked by signatures/missing real callee. Empty claims retained, no coverage credited.
- Farm harvesting temporarily stopped while coordinator integrates; active node jobs continue. Resume after gates/commits. Next: six proven switch-head registrations and static promotion.

## 2026-10-01 handoff (Claude coordinating session)

- Game code: 497/1,210 functions, 56,416/647,072 bytes (8.72%). Static code: 23/230 functions, 1,448/61,440 bytes (2.36%). ROM SHA-1 exact.
- Done this session: group search job + farm automation (on), worker-leak fix, 45 heads registered (OpenAI lane 1), vendored workbench,
  pilot of 18 near-misses (7 matched), `diagnose` pipeline step accepted, all cloud PRs (#1-#6) merged.
- Open: lanes 1-5 in PROMPT.md. Uncommitted in the tree at handoff: nothing relevant (check `git status`).
- Known blockers: the 7 `tests/cloud/test_ipakit_*` failures (cloud tests assume unregistered heads); `func_800BB7F4`/`render_post_process` refused by
  the image gate; stand-in groups are not spliceable.
