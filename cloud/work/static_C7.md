# Static packet C7 — frozen

Seven eligible canonical SDK bodies now independently compile to strict (stack-sensitive) score 0 and raw .text word difference 0 on Rocky. Total eligible slot bytes: 1,980. The five initially ready sources were frozen before the parent refreshed alias targets; the two canonical controller loop sources remain unchanged. No score masks or restored endpoint variants are in this packet.

| Target | Segment | Slot bytes | Optimization | Canonical SDK body |
|---|---|---:|---|---|
| __osContBuildRequest | 0xf160 | 196 | O2 | __osPfsRequestOneChannel, io/pfsgetstatus.c (VERSION_J) |
| __osContParseResponse | 0xf160 | 156 | O2 | __osPfsGetOneChannelData, io/pfsgetstatus.c |
| __osContGetStatus | 0xa050 | 172 | O2 | __osContGetInitData, io/controller.c |
| __osContRamReset | 0xa050 | 216 | O2 | __osPackRequestData, io/controller.c |
| __osPackReadData | 0xa330 | 208 | O2 | io/contreaddata.c |
| guLookAtF | 0x9ab0 | 696 | O2, R4300 multiply erratum | gu/lookat.c |
| osJamMesg | 0x81e0 | 336 | O1 | osSendMesg, os/sendmesg.c |

Exact flags, source hashes and current target hashes are in static_C7/verification.json. Common flags are `-g0 -mips2 -G 0 -non_shared`, with the optimization above; guLookAtF additionally requires `-Wab,-r4300_mul`. All target objects exist privately at Rocky `~/agents/C/scratch/static-C7/NAME.target.o`; candidate objects are beside them. Metadata is from read-only supported DB/blob APIs after parent target refresh.

`static_C7/rom_tu_declarations.txt` contains only necessary context additions and one required prototype correction: existing __osContRamReset(s32) must become __osContRamReset(u8). Parent must preserve prior accepted callers and full ROM equality. The historical osJamMesg name actually appends to the queue, while osSendMesg prepends; bodies use their actual target semantics, without renaming public symbols.

Both controller loops originally differed only in one relocation addend: the target endpoint label __osSiDmaRetry is exactly __osSiDmaBuffer+60, OSPifRam.pifstatus. Canonical sources have unchanged source hashes after the parent guarded assembly normalization. `alias_provenance.json` gives symbol/header authority, current hashes and direct linked-word proof. `retail_verification.json` proves every full slot equals retail after relocation through verified image symbols, with no unresolved symbols, unverified addresses, masks or errors.

osSendMesg has O1 strict/raw zero after supported target refresh and full linked-slot zero, but stays blocked by its existing shared 0x8040 O2 pin. It is explicitly excluded by the verifier and must not be locked or promoted under O1.

Independent invocation on Rocky, after copying this directory and its private targets into D scratch:

```
python3 ~/agents/D/scratch/static-C7/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir ~/agents/D/scratch/static-C7 --output ~/agents/D/scratch/static-C7/independent-verification.json
```

The verifier requires all seven eligible strict/raw zeros and exact source/target hashes and first flags lines; exit code was 0 on C. Recommended acceptance is the established parent lock/prepcommit/promote sequence with these literal flags, clean-tree/shared-pin checks, and full ROM gate. Worker changed no integration files or conveyor state.

The supplemental `static_C7/wrapper_flags/` packet independently proves the current promoted guLookAt wrapper remains strict/raw zero under appended `-Wab,-r4300_mul`, using its exact C3 body and current target hash. Its generic verifier requires one result; exit code 0. All supplemental source/metadata and the private guLookAt.target.o are frozen in Rocky `~/agents/C/scratch/static-C7/wrapper_flags/`. This supplies the shared 0x9ab0 flag reconciliation proof; parent retains full-TU/ROM authority.


Coordinator D independently verified all seven ready objects and the existing guLookAt wrapper with appended errata flag. Its normalized body matches the existing lock hash; only verified flag evidence changes for the shared segment. __osContRamReset now declares the actual u8 command, and the prior accepted caller passes constant0. Both new segments were converted, all static TUs forcibly rebuilt after shared context changes, and the passthrough ROM baseline passed. Fifteen C7/C8 individual promotion transactions follow this clean preparation.
