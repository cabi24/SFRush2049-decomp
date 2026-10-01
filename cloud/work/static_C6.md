# Static packet C6 — 2026-10-01

Eight ready strict/stack-sensitive and raw-word object matches cover **2,876 potential static slot bytes**. osPfsReAllocate requires a signature/caller audit; __osPiReadDeviceType requires preserving its trailing slot padding. A ninth exact object remains blocked by shared flags. No coverage is claimed before coordinator locks, shared-context integration and full-ROM gates.

| Function | Segment | Slot bytes | Exact flags level | Strict/raw | Status |
|---|---|---:|---|---|---|
| osPfsFreeBlocks | 0xbe40 | 416 | O2 | 0/0 | Ready |
| osAiSetFrequency | 0xca70 | 336 | O2 | 0/0 | Ready |
| __osViSwapContext | 0xd270 | 768 | O2 | 0/0 | Ready |
| __osPiReadDeviceType | 0x8a80 | 168 | O1 | 0/0 | Ready; preserve8-byte slot tail |
| guMtxF2L | 0x9de0 | 256 | O2 + r4300_mul | 0/0 | Ready |
| guOrthoF | 0x9660 | 340 | O2 + r4300_mul | 0/0 | Ready |
| guPerspectiveF | 0x9820 | 560 | O2 + r4300_mul | 0/0 | Ready |
| osPfsReAllocate | 0xc990 | 32 | O2 | 0/0 | Ready after signature audit |
| dll_reschedule | 0xcc50 | 116 | O1 | 0/0 | Blocked by shared O2 pin |
| sprintf | 0x34a0 | 96 | O0 | 320/10 | Unmatched |

Exact first-line flags are `/* flags: -g0 -O2 -mips2 -G 0 -non_shared */`, the corresponding O1/O0 line, or for all three GU bodies **`/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */`**. Final source/target SHA-256, text-word lengths, flags and integration_status are frozen in `static_C6/verification.json`. Current target export metadata and authoritative slot extents are in `targets.json`. Private objects are `/tmp/NAME.target.o` on the Pi and `~/agents/C/scratch/static-C6/NAME.target.o` on Rocky. No objects/ROM words were added to the repository.

All canonical SDK bodies came from local ultralib @ e24c8367. osPfsFreeBlocks (`src/io/pfsfreeblocks.c`) uses VERSION_J+ ERRCK behavior and current target name osPfsReadWriteFile_pages for SDK __osCheckId. The first baseline differed only in sltiu versus slti because my provisional ARRLEN macro omitted the SDK signed-int cast; using exact signed ARRLEN yields0. Existing OSPfs and __OSInode types suffice.

osAiSetFrequency (`src/io/aisetfreq.c`) uses target symbol gAudioDmaBufferPtr for SDK osViClock. __osViSwapContext (`src/io/viswapcontext.c`) uses __osViContext for SDK next context, the existing signed __osViModeInfo value for SDK current pointer (explicit pointer/integer casts preserve shared declarations), and gViMgrEventCount for SDK __additional_scanline. Both select VERSION_J behavior; no K timer minimum is added. Their first residuals were solely absolute global/JAL addresses from stale floating-ABI alias assembly failures. Coordinator supported refresh retired that fallback; this worker re-exported the current normalized objects and repeated strict/raw checks.

__osPiReadDeviceType is a target-assembly rederivation of PI timing snapshots: write type7 and copy domain1/domain2 MMIO timing bytes into the current gSpTaskFlags*/gSpTaskResult* symbols. The selected permuter lead reordered stores and treated MMIO as ordinary global variables. Delivered void(void) body reads explicit volatile MMIO addresses in the exact target order and matches O1; O2 does not. Its isolated candidate/target objects contain160 bytes while the authoritative slot is168. Direct retail-prefix verification has zero differing words; the remaining8 slot bytes are zero padding that must be preserved during conversion/promotion. No semantic instructions were masked or omitted from the prefix proof.

osPfsReAllocate is historically misattributed: target at0x8000BD90 is the SDK osGetThreadId operation (`src/os/getthreadid.c`). It takes only OSThread* a0, substitutes __osRunningThread for NULL, and returns thread->id at+0x14. Final source has **OSId osPfsReAllocate(OSThread*)**. Existing include/PR/os_pfs.h:168 instead declares a PFS operation with two arguments; coordinator must audit callers and preserve prior ROM byte proofs before correcting it. No worker header change was made.

The GU issue was a missing documented assembler flag in my earlier static baselines. Adding **-Wab,-r4300_mul** immediately matches all three canonical GU bodies (`src/gu/mtxutil.c`, `ortho.c`, `perspective.c`), including FP multiply ordering and NOP spacing. I missed this lever before the C5 reflow sweep; that sweep was unnecessary. No C6 broad reflow repeats were run. Mtx is the project's u32[4][4], so the canonical union syntax is adapted to (*m) without changing layout. Existing Makefile ROM TU builds already supply the R4300 flag globally; coordinator still must reconcile exact shared-segment lock flag evidence consistently.

guPerspectiveF now explicitly loads existing **extern const f64 gOrthoScale**, matching the target relocation, instead of generating fresh local .rodata for the SDK literal. Retail bytes at ROM0x2e3f0 / VA0x8002D7F0 are3f91df469d353918, exactly the SDK expression3.1415926/180.0. `literal_provenance.json` records this and final source hash. No replacement data or stand-in constant was introduced. Repeated source verification remains strict/raw0.

`retail_verification.json` provides additional full-word evidence: candidate relocations resolved through the cloud scorer's verified retail symbol table, compared directly to baserom.us.z64 slot words. Seven ready bodies and blocked dll_reschedule match their full slot words with zero masks, unresolved/unverified references or errors. __osPiReadDeviceType matches its full emitted160-byte prefix; two absent trailing zero words account for the reported length differences and the documented padding gate. This is supplementary evidence; the full-ROM transaction remains authoritative.

dll_reschedule is canonical pre-K __osSetTimerIntr from `src/os/timerintr.c`. After coordinator refresh, O1 matches exactly; O2 remains wrong and cc50's O2 pin remains authoritative. It must not be promoted by overriding that pin. sprintf has an O0-shaped variadic wrapper calling current symbol fcvt, with redundant return branches and scheduled argument/return loads. Twenty-one bounded flag/volatile controls found no strict zero. Its final unmatched source preserves ordinary variadic semantics and records the best O0 result. No broad physical-line sweeps were run in C6.

Minimal additive declarations, macro requirements, exact prototypes and signature-conflict evidence are in `rom_tu_declarations.txt`. Independent x86 verifier:

```sh
python3 STATIC_DIR/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir TARGET_DIR --output D_VERIFIED.json
```

It intentionally checks **nine** exact objects, including explicitly blocked dll_reschedule, while skipping unmatched sprintf; validates hashes and exact flags; serially recompiles and requires strict stack-sensitive plus raw-text zero. Final C worker run exited0 for all nine. It does not waive signature, padding or shared-flag gates.

For each ready function, coordinator may use its exact manifest flags:

```sh
python3 -m tools.conveyor.pipeline.lock add cloud/work/static_C6/NAME.c:NAME --flags "FLAGS_FROM_MANIFEST"
python3 -m tools.conveyor.pipeline.promote run SEGMENT:NAME --from cloud/work/static_C6/NAME.c --via-builder
```

Use table segments, existing conversion/clean-tree/lock/ROM gates, and integrate only the ready subset. Source files are frozen after the final verification above. Worker did not touch src/asm/layout/locks/state, commit or push; serial compilation left no searches running.

At coordinator request, already-promoted guOrtho/guPerspective wrappers were separately reverified with appended -Wab,-r4300_mul. Both retain strict/raw0. Exact standalone flag files, hashes/current target metadata, verification manifest and a two-object generic verifier are in **static_C6/wrapper_flags/**. Its independent C worker run also exited0; private objects are in C scratch's wrapper_flags directory. This supplemental wrapper evidence is excluded from the nine-object C6 packet verifier and supplies consistent shared flagset evidence for coordinator reconciliation plus full-TU/ROM proof.


Coordinator acceptance preparation: independent Rocky D verification passed all nine main objects (eight ready plus blocked dll_reschedule) and all five supplemental GU objects. The five promoted GU normalized bodies were asserted identical to their existing lock body hashes before changing only their exact flag evidence to append `-Wab,-r4300_mul`; their original ROM provenance remains intact. Their existing Makefile build already supplies this flag. All ROM TUs were forcibly rebuilt after shared context, signature and conversion changes, and the source-built game/full-ROM SHA-1 gate passed before promotion. This preparation adds no static coverage.

The actual osPfsReAllocate body consumes one OSThread pointer and returns its id; the linked static C tree has no callers of the old two-argument declaration. Corrected the shared os_pfs.h signature, with os_thread.h included for the established types. Frozen game standalone preludes retain their prior declarations; they do not call this function and are separate compiler TUs. No accepted body was rewritten.

The supported guMtxIdent target refresh recovered its actual twelve-word reloc-aware object and passed the assembly gate, replacing a stale padded raw target. Parser integration also found that ERRCK macro semicolons hid osPfsFreeBlocks from function extraction. Masking complete preprocessor directives before the brace/declaration walk fixes this, with regressions for semicolons and continued macro braces; source text and compiler inputs are preserved.
