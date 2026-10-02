# Natural SDK object split candidates

Fresh isolated IDO replays of the current production `rom_tu.h` and its actual header closure prove both complete native bodies. Canonical strict scoring including stack is zero; protected target objects independently roundtrip to all native words; candidate full relocated bodies have zero differences, masks, unknown relocations, errors or extra nonzero words. Objects contain no data/BSS/rodata/storage sections. Numerical and hash receipts are adjacent. Raw objects and native words remain ignored.

| Member | Native bytes | Flags | Frame | Split boundary |
| --- | ---: | --- | ---: | --- |
| osCreatePiManager | 368 | g0/O2, mips2, G0, non_shared | 48 | ROM8F80 / native80008380 |
| osSendMesg | 336 | g0/O1, mips2, G0, non_shared | 40 | ROM8190 / native80007590 |

These are genuine earlier C10/C7 SDK reconstructions, now replayed with current production types. Their former blocker was a conflicting accepted suffix recipe. Each prefix begins and ends at a 16-aligned complete native SDK function boundary. Keep the old suffix TU names and lock keys: lib_8e10/osCreateViManager stays O1, lib_8040/osViSetMode stays O2. The split itself credits nothing. Root owns supported split, actual whole-TU placement, unchanged suffix proof and full ROM gates.

## Actual contracts

PiManager takes ordinary priority, command queue, message buffer and count. It tests the existing device manager active flag, initializes existing queues/access state, registers the PI event, adjusts/restores current thread priority, initializes all seven native OSDevMgr28 fields, and starts the existing manager thread through ordinary six-argument osCreateThread. The 28-byte typed SDK manager overlays the legacy opaque __osPiMgrState declaration without defining or resizing storage. Historical labels osCreateViManager, osSpTaskLoad_full, osPiStartDma and osPiSetDeviceTiming retain their native addresses/contracts; no callee bodies are added. Existing thread and queue symbols remain extern declarations.

The native canonical osSendMesg body performs SDK jam/prepend semantics: first=(first+count-1)%count. Its sibling native osJamMesg performs append semantics. Preserve these proven canonical labels. It blocks only for flag1, updates the actual running thread state/full queue, wakes a waiting receiver, and restores the interrupt mask.

## Headers and supported transaction

Pi source needs declarations-only pi_manager_declarations.h after unchanged rom_tu.h; message source needs only rom_tu.h. The new prefix TU must contain that header before body promotion: the promote API extracts only the function definition. Publication paths do not automatically resolve bare rom_tu.h through the lock API fixed include/includePR directories. Verify a prepared source under src/rom, or provide the exact local header closure to a lockable publication source; the isolated compilation copied that closure explicitly.

Supported root operations are layout split <segment> <ROM boundary> --new-tu rom/<prefix-name> --keep suffix, pipeline.lock add <source>:<function> --flags '<native flags>', and pipeline.promote run <new-derived-segment>:<function> --from <source> --via-builder. The split parser supports only the standard rom_tu.h preamble, so split before adding declarations. No production file, registry, accepted source or flag was changed by this packet. Accepted bytes remain zero pending root whole-object and cartridge gates.
