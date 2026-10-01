# Static packet C3 — 2026-10-01

Seven strict static matches from eight selected passthrough targets. All source/target SHA-256 identities, flags, strict scores, and raw word differences are in `static_C3/verification.json`; authoritative layout slots and local corpus provenance are in `targets.json`. Exported target objects remain outside the repo at `/tmp/NAME.target.o` on the Pi and `~/agents/C/scratch/static-C3/NAME.target.o` on Rocky. Worker changed only this packet/log and used serial compilation; no source integration files, locks, layouts, coordinator state, commits, or pushes were modified.

| Function | Segment | Slot bytes | Optimization | Strict score | Raw differences |
|---|---|---:|---|---:|---:|
| guPerspective | 0x9820 | 96 | -O2 | 0 | 0 |
| guOrtho | 0x9660 | 108 | -O2 | 0 | 0 |
| guLookAt | 0x9ab0 | 120 | -O2 | 0 | 0 |
| osDpSetNextBuffer | 0x8700 | 128 | -O2 | 0 | 0 |
| osSetTimer | 0xefd0 | 224 | -O1 | 0 | 0 |
| osContStartQuery | 0xa330 | 124 | -O2 | 0 | 0 |
| osCreateThread | 0x7b30 | 336 | -O1 | 0 | 0 |
| osGetTime — unmatched | 0x7fb0 | 144 | -O1 | 45 | 6 |

Potential new cartridge coverage is seven functions / **1,136 slot bytes**. This is object-matching evidence until coordinator promotion and the full-ROM SHA-1 gate pass. Exact flags are `-g0 -O2 -mips2 -G 0 -non_shared` for O2 and `-g0 -O1 -mips2 -G 0 -non_shared` for O1.

The three GU wrappers initially had raw-word target objects with two absolute JALs and no relocations (strict score10/raw2). The coordinator repaired assembler ABI float-register-alias normalization, refreshed only these targets through supported population, and their newly relocation-aware targets now pass strict/raw0 without any source changes, scorer changes, or masks. Final target hashes are recorded; the obsolete objects are not acceptance evidence.

The canonical local ultralib corpus bodies were adapted only to current target symbol names and verified behavior:

- osDpSetNextBuffer: SDK `__osDpDeviceBusy` maps to current `osDpIsBusy`. Its actual target ABI is `s32 (void *, u64)`, proven by canonical compilation and target a2/a3 spills; existing `include/PR/os.h` declares `void (void *, u32)` and must be corrected by the coordinator. Delivered body uses literal volatile MMIO lvalues to avoid conflicting existing DPC macro forms.
- osSetTimer: SDK `__osInsertTimer` maps to `dll_insert`, with **OSTime/u64 return**; SDK `__osSetTimerIntr` maps to `dll_reschedule(OSTime)`. The explicit verified pre-K body avoids introducing shared BUILD_VERSION macros. An explicit OSTimer* cast before reading existing __OSTimerNode* `__osTimerList->next` preserves strict/raw0 and avoids a conflicting header declaration.
- osContStartQuery: last-command global maps to `__osPfsRequestType`; pack-request routine maps to `__osContRamReset`; PIF buffer maps to `__osSiDmaBuffer`. Canonical O2 is exact.
- osCreateThread: SDK cleanup callback maps to historical `__osExceptionPanic`, and SDK active-thread list maps to current `__osRunQueue`. Correct pre-K behavior is selected directly. Existing OSThread fields/types reproduce every target offset; no struct changes are needed. OSIntMask and SDK constants are enumerated in the declarations file.

`rom_tu_declarations.txt` is the minimal integration reference: GU float prototypes/Matrix, callback/list/timer prototypes, thread constants, and the explicit osDpSetNextBuffer conflict. No large source prelude is copied into this log.

osGetTime remains unmatched. The canonical source accesses one u64 global, but normalized target references its high/low globals separately (`gViTimeAccumHi` and `gViTimeAccumLo`). Explicit u64 composition produced strict1589/raw34; eight union/field-order/same-line variants reduced this to strict45/raw6 with identical frame and instruction count. Remaining differences are temp-ring allocation and high/low load order. Workbench diagnosis (`osGetTime.diagnose.json`) reports `allocation`, frame_delta0, no known lever, ownership unknown. Best full TU is supplied as a lead and excluded by the acceptance verifier. No relocation or stack masking was used to claim a match.

Independent verification after copying packet and target objects into a separate x86 scratch directory:

```sh
python3 STATIC_DIR/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir TARGET_DIR --output D_VERIFIED.json
```

The verifier checks exact hashes/flags, compiles serially, enables stack-sensitive scoring, compares unmasked raw words, deliberately excludes unmatched osGetTime, and exits nonzero unless all seven pass. It passed after every final source update, including the explicit timer-list cast. Coordinator acceptance still requires appropriate flag pins, any segment conversion's byte-preservation baseline, static promotion, and full-ROM/project gates.
