# Coordinator status (Codex maintains this; newest entry first)

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
