# Static SDK scheduler campaign

## Frozen eligible publication

[publication/sdk_members.c](publication/sdk_members.c) and [publication/scheduler_declarations.h](publication/scheduler_declarations.h) contain eight complete native scheduler bodies with actual current `rom_tu.h` types. The header adds compatible genuine declarations only. This publication defines no storage, rodata, timestamp state or substitute helper bodies. The original SDK ancestor is `reference/repos/ultralib/src/sched/sched.c`; historical native helper names retain their actual observed contracts.

| Member | Exact native bytes | Native frame |
| --- | ---: | ---: |
| osCreateScheduler | 388 | 32 |
| osScAddClient | 104 | 32 |
| __scSchedule | 256 | 40 |
| __scHandleRSP | 380 | 40 |
| __scHandleRDP | 216 | 40 |
| __scTaskReady | 176 | 48 |
| __scExecTask | 280 | 32 |
| __scHandlePreNMI | 96 | 24 |

Total eligible native bodies: **1896 bytes**. All eight independently compile with their current production header snapshot at `-g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Each is canonical strict zero including stack operands and full relocated native byte equality, with zero masks, unresolved references, unverified references, errors or extra nonzero alignment words. The combined eight-body TU also preserves every full native body exactly. All 13 protected native target objects independently round-trip against current original assembly words.

The newly complete __scHandlePreNMI source is SDK task yield: it checks the current task type, sets the yield flag and calls the genuine physical yield helper `osDpWait`. Its complete 96-byte match is now proven under the same native debug recipe. __scExecTask retains the original SDK status local storing the real message-send return, though native code does not read that result again; no synthetic storage or runtime operation was introduced.

[publication/manifest.json](publication/manifest.json) records the exact current header/source hashes. [results/publication_singles.json](results/publication_singles.json) has per-function protected target hashes, canonical strict scores, native geometry and body hashes. [results/publication_current_header.json](results/publication_current_header.json) records the complete combined TU. The include tree and production `rom_tu.h` were copied read-only into an isolated Rocky context and remain unchanged in the workspace.

No cartridge credit is claimed here. Root owns the genuine scheduler-prefix layout transaction, ordinary source/lock promotion and unchanged O2 VI suffix/full-ROM gates. The historical shared lib1050 O2 pin prevents direct promotion into the original unsplit TU.

## Full 13-member closure and bounded negatives

The complete source/header closure at group.c contains every actual scheduler body through __scScheduleCore. Ordinary g1/O1 confirms the eight exact members above; __scScheduleCore has native 872-byte geometry with zero verified differences and two pending switch-table references. Its generated 28-byte table is excluded from this publication until current protected owned-rodata placement is proved.

The actual complete whole-O3 pipeline retains all 13 genuine entries and leaves broad nonmatches. Its one diagnostic is frozen. No O3 flag sweep or fallback helper body was used.

Fresh __scMain and __scExec bodies first had one/two extra timestamp-address instructions. Defining the actual consumed scheduler timestamp objects in the private full closure recovers their complete native 324/360-byte extents. Each remains five scheduling differences in real high/low loads. Those timestamp ownership variants are unregistered source hypotheses: native initial storage ownership is unproved, so they are excluded from publication and receive no data/body credit. ROM container offsets do not establish initial values for runtime RAM timestamps.

__scAppendList retains its known one-instruction scheduling deficit. The current retrace source removes the deleted donor task-drain block and all its unused locals; it remains a nonmatch. The frozen original C108 frame-dependent unused-local hypothesis is not imported. No arbitrary declaration, line, qualifier or assembler-flag controls followed the plateau.

All raw objects, disassembly, diagnostics and native table words are confined to ignored build storage. Accepted sources, flags, locks, target/scorer tooling and infrastructure were not changed. No tests were added or run.
