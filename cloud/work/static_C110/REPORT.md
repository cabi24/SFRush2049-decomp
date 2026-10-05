C110 — complete native RSP scheduler completion, exact research

Actual mapped __scHandleRSP at80000950 is380 bytes/95 original words. This complete one-input body clears currentRSPTask, queries actual native OSTask bytes at task+10 for yield status, marks yielded state and requeues a graphics task, otherwise completes its RSP state. It then schedules real availability and executes actual returned sp/dp work. Native current-task and audio/graphics validation branches have no failure payload and are explicitly preserved.

Literal g1/O1 recipe on Rocky is strict0 including all stack offsets. True extent380 and40-byte frame match; all95 fully relocated original words match with zero masks/unresolved/unverified/errors. Original target roundtrip passes. ELF .text has one trailing4-byte zero alignment word outside the actual native function. Workbench on the assembled target sees section padding as a96th word because GNU-as target has no function-size record; the authoritative original95-word extent and direct fully linked comparison prove the actual complete native match.

Existing OSSched/OSScTask carrier fields are used unchanged at native offsets. Missing actual declarations: osSpTaskYielded accepts actual task/list pointer and returns s32; __scExecTask and __scScheduleCore declarations as in C109. task/sp/dp/state are all consumed. No invented pressure/unused locals, inputs, artificial runtime work, storage changes or line manipulation.

This research source earns zero accepted credit until scheduler split/whole-TU recipe and normal lock/promotion/full-ROM/test gates complete. All current production source/headers/layout/locks/targets untouched.
