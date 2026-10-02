# Verified isolated acceptance

This branch includes shared main `7c4d12c3`. The main coordinator has accepted the prior 41-function backlog and five additional physics functions. This isolated branch adds one genuine game function and four scheduler functions beyond that main revision.

| Population | Accepted C | Native bytes compiled from C | Additional functions / bytes beyond main |
| --- | --- | --- | --- |
| Game | 651/1,216 | 99,280/647,072 (15.34%) | 1 / 152 |
| Static | 151/230 | 42,212/61,440 (68.70%) | 4 / 876 |

All 651 game C bodies are present and source-built. The complete linked game image and original compressed stream are exact; the independently built original full ROM passes SHA-1. All 154 static/source locks and all game/group locks hold. Full pytest exits zero with 820 passed and 573 skipped, with its exit captured directly. The branch does not trigger CI; these are actual independent build and test results.

The new game member is mode_select_input. Its genuine hierarchy wrapper is annotated inline because the complete wrapper runtime operations are expanded in the native member; accepted wrapper and transform locks remain unchanged. Complete outer caller and allocator contexts remain disclosed nonmatches, receive no credit and are not placed.

The supported SDK/video split preserves all 22 original functions and 4,512 bytes at the real 0x1f50 boundary. It preserves the original video source, accepted bodies and O2 locks. The separate scheduler prefix uses the independently proven g1/O1 recipe. osScAddClient, __scTaskReady, __scHandleRDP and __scHandleRSP each passed ordinary pool verification and normal static promotion, full source-built ROM and all lock/test gates before commit. Check-only native assertion paths are retained without invented failure bodies. The split itself adds no coverage.

The prior BSD formatter ownership, complete 89-entry source-built table, all neighboring bytes and all original address/size assertions remain intact. Alignment padding and tables receive no code-byte credit. Cloud PR7 and PR8 were integrated in the prior backlog; the later research archive remains selectively reviewed, with no nonmatches counted.

The main coordinator can review this branch, reconcile later main changes and rerun the supported image/ROM, lock and captured pytest gates before direct main integration. All work here uses a separate clone, data store and watchman2 checkout.
