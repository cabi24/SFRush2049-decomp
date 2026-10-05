C109 — complete native RDP scheduler completion, exact research

Actual mapped __scHandleRDP at80000ACC is216 bytes/54 original words. The source requires an existing current graphics task, clears its scheduler slot and DP state bit, calls actual completion helper, computes real RSP/RDP availability, schedules real sp/dp output locals, and executes new work if availability changed. Both native check-only assertion branches are retained; no assertion failure routine exists in the native body.

Literal g1/O1 recipe on Rocky returns strict0 including every stack operand. True function extent216 bytes and original40-byte frame match; all54 fully relocated original words match. Emitted .text includes8 zero section-alignment bytes outside the logical function. All original calls resolve to their actual current symbols; zero masks/unresolved/unverified/errors. Original target roundtrip passes. verify.py reproduces native/strict/extent proof with private build/C109 objects.

Existing OSSched/OSScTask carriers and __scExec are used unchanged. Only missing declarations are actual two-input __scExecTask returning s32 and actual four-input __scScheduleCore with two output task pointers returning s32. All four locals (task/sp/dp/state) are consumed, with SDK-proven declaration order; no unused pressure, storage definitions, added inputs/work, qualifier changes or source line tricks.

This is research only pending the supported scheduler prefix split and whole-TU compiler recipe, ordinary locking/promotion, full source-built ROM and test gates. No current flags/header/lock/layout/target/source edits or coverage claim.
