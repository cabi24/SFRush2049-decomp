C105 — exact scheduler readiness source, research only

The actual mapped __scTaskReady at80000BA4 is176 bytes and has two consumed inputs: scheduler and candidate task. It reads current/next VI framebuffers, rejects a mismatch, and rejects a pending swap less than two unsigned retraces after the recorded swap. Otherwise it returns the actual candidate task. Its scheduler retrace field at0x27C and both preexisting globals are native grounded; the source defines no storage.

The complete source __scTaskReady.sdk_flow.debug_o1.c strictly matches at its literal g1/O1 recipe. Actual ELF function and emitted text are176 bytes, frame48; all44 fully relocated original words match. Both calls and both data references resolve through current authoritative symbol files, with no masks, unresolved/unverified relocations, errors, or trailing nonzero words. Original target roundtrip passes. See proof.json and reproducible read-only verify.py.

Three bounded current-O2 source hypotheses fail: separate assignments1433, actual SDK assignment-expression1409, native-current register declaration1409. One authorized unchanged debug diagnostic is200: my unnecessary else yields an extra unreachable branch pair. The separately authorized original SDK final-return flow removes that invented else, strict0. No broad flag/line sweep, padding, pressure, dummy formal, scorer mask, target edit, or artificial work was used.

The accepted lib1050 module remains atg0/O2. C67 already proves its unchanged accepted viDeadlinePassed adds four words atg1/O1. Existing locks/pins are untouched. This packet earns0 coverage until the coordinator establishes a genuine supported whole-module recipe and completes all gates.
