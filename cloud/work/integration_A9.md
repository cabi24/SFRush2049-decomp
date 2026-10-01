# A9: read-only C10 integration audit — frozen

2026-10-01. Shared source, headers, layout, locks and coordinator state were not edited. Audit uses a current root header/source snapshot including C9 declarations, taken before C9's eight promotions completed. Independent compile work ran sequentially in private Rocky A scratch.

**All seven eligible C10 bodies independently retain strict stack-sensitive score 0 and raw-word difference 0 under the adapted CURRENT root header snapshot. All 53 currently promoted ROM TUs retain exactly identical raw .text under their existing O1/O2 pins before versus after those header adaptations.** C10's osCreatePiManager O2-only candidate remains excluded under the accepted 8e10 O1 pin.

## Minimal integration changes proven privately

Replace the existing include/PR/os_pfs.h __OSDir definition, retaining 32-byte size and all leading offsets:

```c
typedef struct __OSDir {
    u32 game_code;              /* 0x00 */
    u16 company_code;           /* 0x04 */
    __OSInodeUnit start_page;   /* 0x06 */
    u8 status;                 /* 0x08 */
    s8 reserved;               /* 0x09 */
    u16 data_sum;              /* 0x0A */
    u8 ext_name[4];            /* 0x0C */
    u8 game_name[16];          /* 0x10 */
} __OSDir;
```

Current layout has game_name@0xA, ext_name@0x1A, data_sum@0x1E. Moving data_sum before ext_name/game_name and changing the name bytes to u8 is necessary for the proven retail loads/comparisons. Signed reserved follows canonical context; no accepted body reads that field. Keep OSPfsState unchanged: its existing char arrays are copy destinations, and all three C10 PFS bodies prove exact against that existing type.

Replace only these two existing PR/os_pfs.h prototypes, preserving historical retail symbol names:

```c
s32 osPfsAllocate(OSPfs *, u16, u32, u8 *, u8 *, s32 *);
s32 osPfsDeleteFile(OSPfs *, s32, OSPfsState *);
```

Their actual bodies are SDK FindFile and FileState respectively. osPfsRename has the five-argument DeleteFile body. There is NO current include/ declaration for osMotorStart; adding its three-argument MotorInit signature is additive, despite the packet's provisional “public one-argument replacement” note. C9's __osTimerInterrupt(OSPfs*) and __osMotorAccess(int,OSPifRam*) declarations already suffice and should not be duplicated.

The exact tested private additive context is in proof/private_header_patch.diff. It adds __osContLastChannel, unsigned-byte __osPfsDataChecksum, osPfsRename, osMotorStart, read-pak command constants, PFS_ID_BANK_256K, and the eight-byte queue/message __OSEventState with OSEvent/u32 globals and PRENMI 14. No current header declarations conflict with __osShutdown, gEventTypeFlag, __osPfsDataChecksum, OSEvent or __OSEventState. __osPfsDataChecksum is not in current static locks. Existing C6–C9 macros/types supply all other required context; do not paste the full standalone source prelude.

## Independent new-body proof

Extracted the seven actual definitions from frozen C10 files and compiled each with `#include "rom_tu.h"` against privately corrected root headers. No standalone typedefs or obsolete standalone public prototypes were copied. Exact candidate flags were used (six O2, event setter O1), plus include paths and _LANGUAGE_C only. All seven are strict/raw 0:

| Function | Candidate / target words | Optimization |
|---|---:|---|
| __osContRamRead |140/140|O2|
| __osTimerInterrupt |68/68|O2|
| osMotorStart |88/88|O2|
| osPfsAllocate |116/116|O2|
| osPfsDeleteFile |120/120|O2|
| osPfsRename |120/120|O2|
| osSetEventMesgAlt |48/48|O1|

proof/results.json records successful compiler exit codes, empty stderr, raw differences and independent stack-sensitive scores against C's exact private target objects. Toolkit: 796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5. The target object identity/slot-padding caveats remain the frozen C10 verification packet's responsibility and root's ROM gate authority.

## Existing accepted source/caller audit

Static lock-body scan found no __OSDir/name/data_sum field access. Its only reference to an affected symbol is accepted C9 osPfsInitPak→__osTimerInterrupt, already using the correct existing prototype. Frozen locked blob bodies have no affected field or function uses. Old blob standalone preludes often contain obsolete __OSDir/public declarations, but their matched function bodies do not use them; do not rewrite those locked source snapshots to chase unused typedefs.

Registered retail game callers found by scanning actual jal words:

| Callee | Registered callers | Current lock |
|---|---|---|
| osMotorStart |audio_queue_process(3), check_mpath_save, track_render_process|all unlocked|
| osPfsDeleteFile |func_800A1A60, track_process_main, track_render_process|all unlocked|
| osPfsRename |AdjustSpeed|unlocked|
| osSetEventMesgAlt |func_800E7710|unlocked|
| osPfsAllocate |none found|—|

Full offsets and sources are in proof/blob_uses.json and proof/locked_uses.json. These callers currently remain raw retail passthroughs, so C prototype correction does not rewrite their instructions. Old guessed src/game/game.c/save.c and src/libultra reconstructions use conventional SDK labels/signatures that conflict with the actual historical symbol mapping; they are outside this accepted ROM-body packet. Keep future caller reconstruction anchored to actual assembly arguments, not those guessed labels.

## 53 promoted-TU regression proof

Compiled each existing src/rom TU containing a PROMOTED body twice, under its existing opt_overrides.mk O1/O2 level, first with the baseline current root header snapshot and then with corrected headers. Used normal MIPS2/non_shared/G0/multiply-errata/Xcpluscomm options; GLOBAL_ASM passthrough portions are omitted from these direct-C object comparisons. **53/53 builds succeed and all 53 complete C .text byte streams are exactly identical.**

proof/header_regression.json gives TU names, optimization and word counts. proof/tu_text_hashes.json retains SHA-256 identities, word counts and equality for every TU. Full hex streams are excluded from commit artifacts and retained only in ignored `build/codex-A9/tu_texts.json` and the private Rocky workspace. proof/snapshot_sha256.json identifies every snapshot header and ROM source/override file used. This proves the header change does not alter the current promoted C text; it supplements rather than replaces root's full-TU placement, lock and full-ROM gates, and does not cover C9 promotions completed after this snapshot.

Private reproducible workspace: `Rocky:~/agents/A/scratch/codex_A9/`, with baseline_headers/, headers/, rom/, proof/ extracted sources and objects, plus private tu_texts.json. Source-only verifier scripts are retained under integration_A9/proof/. From private A checkout:

```sh
cd ~/agents/A/wt
python3 ~/agents/A/scratch/codex_A9/run.py
python3 ~/agents/A/scratch/codex_A9/compare.py
```

No shared integration writes were performed. Header reconciliation can proceed after the root's in-progress C9 transaction completes.

## Event-setter gate follow-up: ordering, not candidate drift

Root's first full-ROM event-setter promotion failed despite unchanged standalone/adapted-body strict zero. Root isolated the actual prerequisite: 7a10 conversion ran before adding the new O1 static lock, so the generated opt_overrides.mk initially lacked lib_7a10's O1 line. Later lock addition changed derive()'s segment flagset but did not regenerate the fragment. The Makefile then compiled that TU at default O2.

Read-only audit finds no test call to write_opt_overrides or _do_convert and no evidence that test fixtures overwrote the real fragment. No tooling/test patch is justified. Root regenerated the actual fragment from current locks with write_opt_overrides(derive()), restored baseline ROM success, and is retrying the unchanged body.

For new static pins the operational dependency is **new lock → current derive → write_opt_overrides → sync fragment → matching compile/ROM gate**. Conversion alone cannot generate a pin whose lock has not yet been added. This does not change the seven-body or 53-TU proofs above; the failed build used a different optimization level than the proven O1 body.
