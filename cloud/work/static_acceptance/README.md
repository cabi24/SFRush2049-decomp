# Verified isolated acceptance

This branch includes shared main `7c4d12c3`. The main coordinator has accepted the prior 41-function backlog and five additional physics functions. This isolated branch adds two genuine game functions and ten static functions beyond that main revision.

| Population | Accepted C | Native bytes compiled from C | Additional functions / bytes beyond main |
| --- | --- | --- | --- |
| Game | 652/1,216 | 99,864/647,072 (15.43%) | 2 / 736 |
| Static | 157/230 | 44,112/61,440 (71.80%) | 10 / 2,776 |

All 652 game C bodies are present and source-built. The complete linked game image and original compressed stream are exact; the independently built original full ROM passes SHA-1. All 160 static/source locks and all game/group locks hold. Full pytest exits zero with 820 passed and 574 skipped, with its exit captured directly. The branch does not trigger CI; these are actual independent build and test results.

The new game functions are mode_select_input and save_settings. The complete settings routine preserves its real four-player backups and two native unused intermediate ABI slots, necessary for consumed stack arguments. Its genuine hierarchy wrapper is annotated inline because the complete wrapper runtime operations are expanded in the native member; accepted wrapper and transform locks remain unchanged. Complete outer caller and allocator contexts remain disclosed nonmatches, receive no credit and are not placed.

The supported SDK/video split preserves all 22 original functions and 4,512 bytes at the real 0x1f50 boundary. It preserves the original video source, accepted bodies and O2 locks. The separate scheduler prefix uses the independently proven g1/O1 recipe. Nine scheduler functions, including initialization, completion, preemption and the complete recursive scheduler, each passed ordinary pool verification and normal static promotion, full source-built ROM and all lock/test gates before commit. Check-only native assertion paths are retained without invented failure bodies. The split itself adds no coverage.

The prior BSD formatter ownership, complete 89-entry source-built table, all neighboring bytes and all original address/size assertions remain intact. Alignment padding and tables receive no code-byte credit. Cloud PR7 and PR8 were integrated in the prior backlog; the later research archive remains selectively reviewed, with no nonmatches counted.

The main coordinator can review this branch, reconcile later main changes and rerun the supported image/ROM, lock and captured pytest gates before direct main integration. All work here uses a separate clone, data store and watchman2 checkout.

The recursive scheduler includes its complete seven-entry, 28-byte source-built table at8002d4a4. Existing logical-table handling removes only four zero object-alignment bytes and preserves all original nonzero neighboring bytes. Fully relocated native code and the entire table are independently exact. The separate sqrtf unit uses the documented IDO intrinsic: it compiles the original eight executable bytes and preserves the original eight-byte zero tail, which receives no credit. Intrinsic dependence and native check-only logging/assertion paths are disclosed. See SDK_final_batch_gate.json.
