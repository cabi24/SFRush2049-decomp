# Static packet C4 — 2026-10-01

Three ready static object matches, two O0 object matches blocked by an existing shared-TU O2 pin, and three unmatched leads. All targets were authoritative passthrough slots when selected. Exact flags and final source/target hashes are in `static_C4/verification.json`, with an explicit integration_status per target. Layout extents and exported target hashes are in `targets.json`. Objects remain private at `/tmp/NAME.target.o` on the Pi and `~/agents/C/scratch/static-C4/NAME.target.o` on Rocky.

| Function | Segment | Slot bytes | Optimization | Strict/raw | Status |
|---|---|---:|---|---|---|
| osRecvMesg | 0x7c80 | 320 | -O1 | 0 / 0 | Ready after normal context integration |
| osStartThread | 0x7c80 | 336 | -O1 | 0 / 0 | Ready after global-type reconciliation |
| osSetThreadPri | 0x7940 | 96 | -O2 | 0 / 0 | Ready after correcting historical signature |
| viDisableAccum | 0x1050 | 24 | -O0 | 0 / 0 | Blocked: existing shared-TU O2 pin |
| viEnableAccum | 0x1050 | 28 | -O0 | 0 / 0 | Blocked: existing shared-TU O2 pin |
| apply_display_mode | 0x21f0 | 44 | -O2 | 200 / 5 | Unmatched: redundant branch/delay-slot structure |
| dma_signal | 0x3140 | 52 | -O2 | 200 / 9 | Unmatched: redundant branch/delay-slot structure |
| osAiSetNextBuffer | 0xca70 | 144 | -O2 | 335 / 26 | Unmatched: global-address CSE/code-generation structure |

Ready objects cover three functions / **752 potential slot bytes**; blocked O0 objects add two / 52 bytes but must not be promoted under the current O2 segment pin. None is a cartridge coverage claim before promotion and full-ROM gates. Exact flags follow `-g0 -O1|-O2|-O0 -mips2 -G 0 -non_shared` as recorded in each full TU's first line.

osRecvMesg and osStartThread use canonical local ultralib bodies (`src/os/recvmesg.c`, `src/os/startthread.c` @ e24c8367) with external names reconciled to current target relocation records. SDK `__osEnqueueAndYield` maps to `__osCleanupThread`; SDK run-queue head maps to current `__osActiveQueue`. No behavior changes or stand-ins were used. Both shared-0x7c80 functions pass O1 exactly. Existing __osActiveQueue context incorrectly declares OSThread**; the current target uses it as an OSThread* queue head, taking its address when passing an OSThread** queue. This requires coordinator type reconciliation before promotion; the canonical active-list head uses a different historical symbol, `__osRunQueue`, in C3 osCreateThread.

osSetThreadPri is a historical symbol misattribution. Its target at **0x80006D40** is actually a VI set-mode operation, with exactly one consumed argument: an OSViMode pointer. The body disables interrupts, stores arg0 as `__osViContext->modep`, sets state to1, copies `modep->comRegs.ctrl` into control, and restores interrupts. It does not manipulate any thread priority. The two-argument seed stores unused a1 on the stack and shifts 20 instruction words; removing that parameter yields strict/raw0. Final delivered signature is **`void osSetThreadPri(void *mode)`** (the parameter in source is named thread), casting the opaque pointer to OSViMode* before assignment. Existing priority-style declaration in include/PR/os_thread.h must be corrected only after auditing linked callers and preserving earlier ROM gates. No header was changed by this worker.

The two small boot accumulator functions require O0 and no explicit return to reproduce the target's duplicate jr/nop epilogues. O2 produces only one return. An O0 source with explicit return instead emits a branch and fails. An eight-variant flag/return sweep per small wrapper tried O0/O1/O2, explicit/fallthrough returns, and assembler optimization flags. The two accumulator objects match with exactly `-g0 -O0 -mips2 -G 0 -non_shared`. They remain blocked by the shared0x1050 O2 pin, as the coordinator requested. No pin override or source integration was attempted.

apply_display_mode/dma_signal still contain a redundant branch + delay slot before the final epilogue. O2 removes it; O0 retains it but does not schedule the call's argument into the JAL delay slot. The best structural flags remain insufficient; no masked scores count as matches.

osAiSetNextBuffer's selected permuter seed had altered control flow (MMIO update only in one branch) and a bogus two-argument osVirtualToPhysical prototype. This worker re-derived correct behavior from target assembly and local SDK source: busy check, optional 0x2000 adjustment, flag update based on end alignment, unconditional MMIO writes. Current O2 CSE hoists one shared flag address into v0, while the target loads it directly through t6 and uses separate at-based stores. Eighteen bounded declaration/type/volatile/physical-line/compiler-flag variants gave no accepted match. No semantically broken seed was accepted; the supplied lead preserves the target's behavior but remains unmatched.

Minimal declarations and signature/global-type conflict evidence are in `rom_tu_declarations.txt`. Independent x86 object verification command:

```sh
python3 STATIC_DIR/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir TARGET_DIR --output D_VERIFIED.json
```

This verifier intentionally checks all **five object matches**, including the two clearly marked blocked O0 files; passing it does not authorize mixed flags or claim five promotable functions. It excludes the three unmatched leads, checks exact hashes/flags, compiles serially, enables stack differences, and compares unmasked raw text. Final worker run exited0. Coordinator must integrate only the ready subset unless the blocked route is resolved through supported tooling and the full-ROM gate.

Worker touched only this packet/log, used serial compilation, and left no searches running. One attempted uopt help invocation waited on stdin; its exact PID was terminated immediately. No src/asm/layout/lock/coordinator state, commits, or pushes were changed.
