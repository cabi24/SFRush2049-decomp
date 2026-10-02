# Module campaign acceptance, 2026-10-02

Root independently reviewed and freshly rebuilt the previously completed acceptance branch9c3ecc45 (27 static functions /12,604 bytes,14 game functions /5,544 bytes), then accepted three donor-backed physics functions /1,884 bytes. Coverage:648 game functions /97,852 bytes (15.12%);147 static functions /41,336 bytes (67.28%). Combined coverage using the current tracker denominators:19.65%. This milestone adds20,032 bytes relative to c50cacd5.

The independent review preserved all existing lock records and caught one lost compiler pin: lib_8a80 functions carry identical code-generation flags but differ by the parser-only -Xcpluscomm option. The generator now ignores only that exact option when comparing segment evidence. All other tokens still must agree. Regenerating opt_overrides.mk restores the original module's O1 recipe. Real compiler arguments and lock evidence are unchanged.

All645 baseline game functions were freshly compiled: every standalone source with its locked flags, and all57 genuine groups through the actual whole-program IDO pipeline. The three new functions were separately compiled and spliced by root at ordinaryO2. All648 complete bodies and protected group data references/pools/tables were verified; no missing object was silently replaced by passthrough assembly in the final source-built image. The linked image and its326,180-byte compressed stream reproduce the original byte-for-byte.

The ROM gate used a new isolated watchman2 directory with an empty build/us object cache, the reviewed complete checkout and the genuine pinned IDO toolkit. All static TUs were freshly compiled. The full original ROM SHA-1 passed, MAKE=0 TEST=0. Original inputs and raw machine data remain private/ignored. No separate pytest suite was run for this milestone; older branch receipts retain their original historical test claims.

[gates.json](gates.json) records final gate results and coverage. [fresh_game_rebuild.json](fresh_game_rebuild.json) records the independent baseline body hashes. The compiler lane's individual protected receipts and source SHA-256s are in ../compiler/results/. Drivetrain source/provenance is in ../reconstruction/drivetrain/README.md. The three accepted publication sources are src/blob/func_800E15A0.c (torques), func_800E31D4.c (autoshift), func_800E32CC.c (drivetrain).

The next physics/transmission and static module work remains unaccepted until its own gates pass. No partial source, helper context, literal pool, alignment bytes, or reconstructed data earns additional code credit.

## Scheduler and visual helper acceptance

Eight complete scheduler routines add 1,896 static bytes after the guarded split at ROM 0x1F50. The prefix uses its exact O1/g1 recipe; the existing VI suffix keeps its old source locks and O2/g0 recipe. The structural split itself adds no coverage. The genuine two-callback visual context adds only matrix_scale_apply, 100 game bytes; its unmatched callbacks earn no credit. Header declarations were inlined unchanged for self-contained group compilation.

All 651 accepted game bodies were reconstructed from their compiled objects and checked against every complete native body. Both changed static objects were deleted and freshly built before the original compressed stream and full ROM SHA-1 gates passed, MAKE=0 TEST=0. Original static lock entries and shared rom_tu.h are unchanged. Coverage is now 99,228/647,072 game bytes (15.33%), 43,232/61,440 static bytes (70.36%), and 142,460/708,512 combined bytes (20.11%). [sdk_visual_gates.json](sdk_visual_gates.json) records this gate.

## Pi and message object acceptance

Natural object splits at ROM 0x8F80 and 0x8190 preserve both existing VI suffix lock records and recipes. Fresh complete source-built osCreatePiManager (368 bytes, O2) and native osSendMesg (336 bytes, O1, observed prepend behavior) pass canonical replay, root comparison of all full relocated object words without masks, and full ROM SHA-1, MAKE=0 TEST=0. Prefix and suffix objects were deleted before compiling the final gate. There is no new storage definition. Combined matching coverage is now 143,164/708,512 bytes (20.21%), with static 43,936/61,440 (71.51%) and game 99,228/647,072 (15.33%). See [sdk_two_objects_gates.json](sdk_two_objects_gates.json) and the root full-body proof.
