# Static packet C2 — 2026-10-01

Eight genuine passthrough static targets were selected using `layout.derive()` before inspecting candidate evidence. Accepted C1 targets and `osPiRawReadWord` were excluded. Worker changed only `cloud/work/static_C2/` and this log, used one serial Rocky compiler process, and did not lock, promote, regenerate, mutate coordinator state, commit, or push.

All eight exact full TUs pass both `scoring.score(target, candidate, stack_differences=True)` and exact raw ELF text word comparison on Rocky. `targets.json` contains exported target hashes, authoritative layout extents, and source provenance. `verification.json` records exact source/target SHA-256 identities, flags, object sizes, and strict/raw results. Target objects remain outside the repository at `/tmp/NAME.target.o` on the Pi and `~/agents/C/scratch/static-C2/NAME.target.o` on Rocky.

| Function | Segment | Slot bytes | Flags optimization | Strict score | Raw differences |
|---|---|---:|---|---:|---:|
| osViModeNtscLpn1 | 0x8440 | 76 | -O2 | 0 | 0 |
| osContGetQuery | 0xa330 | 36 | -O2 | 0 | 0 |
| osSpTaskYielded | 0x8330 | 96 | -O2 | 0 | 0 |
| osVirtualToPhysical | 0xe1c0 | 128 | -O1 | 0 | 0 |
| __osSiRelAccess | 0xf160 | 44 | -O2 | 0 | 0 |
| __osSiGetAccess | 0xf160 | 68 | -O2 | 0 | 0 |
| osViGetCurrentFramebuffer | 0x8330 | 64 | -O2 | 0 | 0 |
| osContGetReadData | 0xa330 | 140 | -O2 | 0 | 0 |

Potential newly promoted coverage is eight functions / **652 slot bytes**. This is object-matching evidence, not a cartridge coverage claim. All O2 files use exactly `-g0 -O2 -mips2 -G 0 -non_shared`; osVirtualToPhysical uses exactly `-g0 -O1 -mips2 -G 0 -non_shared`.

Seven targets use canonical ultralib corpus bodies already available locally, adapting only their external names to the current normalized target object identities. Mapping evidence comes from relocation records in the exported target objects, not relocation masking:

- osContGetQuery: `__osContGetInitData` → current `__osContGetStatus`.
- osSpTaskYielded: SDK `__osSpGetStatus` → historical target `bzero_alt`.
- osVirtualToPhysical: `__osProbeTLB` → current `__osTLBLookup`.
- SI access functions: `__osSiAccessQueue` → current `__osSiMesg`; enabled flag → `__osSiInitialized`; queue-creation function → `osSiInit`; SDK send → current `osJamMesg`.
- osViGetCurrentFramebuffer: `__osViCurr` → current `__osViModeInfo`. C1 declares this global `s32`, so the delivered source keeps that declaration and explicitly casts its loaded value to `__OSViContext *` before reading `framep`. This preserves strict/raw identity and avoids a conflicting shared-header declaration.
- osContGetReadData: `__osContPifRam` → current `__osSiDmaBuffer`; `__osMaxControllers` → current `__osPfsRequestType2`.

`osViModeNtscLpn1` is an existing historical `work/auto/osViModeNtscLpn1/matched.c` implementation, strictly verified unchanged. The name is historically misattributed: this is executable SP status code, not a VI data-mode struct. Its isolated object has 64 text bytes while its ROM-aligned layout slot is 76 bytes; original placement/alignment padding must be preserved by the full-TU ROM gate.

Both SI access bodies first passed with the corpus's O1 flags, then independently passed at O2. The delivered files pin O2 to remain compatible with the accepted `osSiInit` in the same 0xf160 TU. No mixed-flag override is needed. osVirtualToPhysical's own segment retains a genuine O1 requirement.

`rom_tu_declarations.txt` lists only the additive declarations, struct, and macros needed for shared-ROM-TU integration. Existing OSTask, OSContStatus, OSContPad, __OSViContext, and OSPifRam types suffice. C1 declarations remain valid.

Independent x86 verification after copying this packet and the eight exported target objects to a separate directory:

```sh
python3 STATIC_DIR/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir TARGET_DIR --output D_VERIFIED.json
```

This checks exact source/target hashes and the exact flags line, compiles serially, enables stack-sensitive scoring, checks unmasked raw words including object alignment padding, and exits nonzero unless all eight pass. This exact generic verifier exited 0 in C scratch after all final source changes.

Coordinator acceptance: independently verify, lock each with its exact proven flags, check shared-header context, convert any unconverted segment with the supported layout tool and prove byte-preservation, promote using the static transaction, then require full-ROM SHA-1 and normal project gates. No source-body substitutions or stand-in functions were used.

Coordinator full-TU baseline found the accepted bzero_alt definition returns s32. Shared declaration preserves that signed return type; the new task-yield body converts the result to u32 status bits. Existing body remains unchanged; full-ROM promotion must verify the new body in this context.
