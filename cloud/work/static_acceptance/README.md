# Verified isolated acceptance

This branch includes shared master `c50cacd5` and adds 41 genuine native functions. Source, lock and layout integration took place in an isolated clone with its own watchman2 checkout and data store. Shared main source, locks, builders and Git history remain unchanged.

| Population | Accepted C on this branch | Bytes compiled from C | Additional functions / bytes beyond main |
| --- | --- | --- | --- |
| Game | 645/1,216 | 95,968/647,072 (14.83%) | 14 / 5,544 |
| Static | 147/230 | 41,336/61,440 (67.28%) | 27 / 12,604 |

The complete source-built game image, original compressed stream and original full ROM are exact. All 645 game bodies are available and compiled. All 150 static/source locks and all blob/group locks hold. Full pytest exits zero with 776 passed and 569 skipped, with the exit captured directly. See `final_acceptance.json` and the committed per-packet image and independent replay proofs. The branch does not trigger CI; these are actual independent build and test results.

The game additions are func_800B78F0, dispatch_handler, menu_audio_settings, the five genuine transform-group members, func_800A1E94, vsync_wait, state_change_preprocess, track_info_display, func_8008C680 and audio_dsp_process. The complete original 48-byte table for func_800A1E94 is source-built but receives no code-byte credit. Full genuine group contexts are retained; only eligible exact bodies are placed. Previously accepted helpers and their locks remain unchanged. A129–A137 rematches and A159 zero-scoring helper contexts add no duplicate credit.

The 27 static additions occupy six actual ROM translation units. The formatter module includes all nine native functions (8,528 bytes) and its complete 89-entry, 356-byte source-built table at the original address. Its checked logical extent and actual input alignment preserve every neighboring byte. Other owned-data defaults and all original address/size assertions remain in force. Original common SDK context remains unchanged; BSD-derived formatter helpers retain their complete notices. Historical GNU formatter candidates are excluded.

The resource-loader context preserves its original skip/full-pool ambiguity; the accepted caller uses its grounded skip=0 path. The 388-byte initializer preserves its actual three-iteration loop with no visible writes, which IDO retains without invented volatility or extra operations. Both quirks were reviewed and disclosed in their acceptance commits.

Cloud PR7 and PR8 were reviewed and merged at their exact heads on this branch. PR7 retains the compiler download and pinned checksum while fixing extraction ownership, with four focused setup regressions passing. PR8 adds the exact 160-byte rational-approximation caller while preserving its existing helper. Both GitHub PRs remain open against shared main. The later PR9 research archive was reviewed selectively and not merged; its close tire-force and texture candidates remain nonmatches.

The main coordinator can review this branch and reconcile later main changes, then rerun supported source-image/ROM, lock and captured pytest gates before direct main integration. Protected source/lock/assembly paths continue to use maintainer/coordinator integration rather than weakening the repository research-PR policy.
