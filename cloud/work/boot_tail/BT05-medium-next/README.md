# Packet 4: five additional BT05 macro handlers

**Two strict local matches, 228 bytes; three COMPLETE-NONMATCHs, 276 bytes.** Independent paired source/ABI review and fresh strict replay passed; publication-head CI remains required. No cartridge-coverage or promotion claim.

- Branch `dot/boot-tail-bt05-medium-next`, clean master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Exact central activation `8743a9c5`: `80021BF0` (104), `80021F08` (96), `80022514` (108), `80022C58` (124), `80023D70` (72), totaling 504 bytes. Each extent is address plus listed size.
- All are previously open, in-scope 64–255-byte BT05 rows. Read-only caller/callee review found no boundary-blocked destination, local literal, switch table or nonstandard incoming argument.
- Prior packets are frozen. This source branch has no dependency on their unmerged source changes; central status, claims, generated research and D10 remain separately owned.

## Source and ABI evidence

All five are called from the resident macro dispatcher `80023E9C` with state in a0 and an aligned two-word command in a1, followed by low-byte result consumption. Shared partial state fields have consistent observed offsets, with packed words/pointers and explicit unknown object-byte ranges. These gaps describe object storage, not padding locals. No additional wrapper argument is invented.

- `80021BF0` (MATCH): if command bits 8..15 are zero or saved state word +8 is zero, return `80021BC0(state, command)`. Otherwise copy saved words +8/+12 into +0/+4 and return zero. The source's natural short-circuit branch exactly reproduces the fallback call and packed-state copies. `80021BC0` is an in-census two-input macro handler, independently reconstructed in the prior packet; its source is not supplied here.
- `80021F08` (NONMATCH): call `80016C20` with command word 0's high halfword. If the returned macro pointer is non-null, store it at state+0x1C and store the pointer advanced by command word 1's low halfword times eight at +0x20. The complete callee consumes a genuine halfword input and returns a loaded pointer or null; the eight-byte `MacroCommand` stride is justified by the native shift-by-three. No fake length or unused parameter is introduced.
- `80022514` (NONMATCH): add the command's signed high halfword to state byte +0x2E, retain a signed-halfword result, clamp it to 0..255, pass the byte to `8001EF8C(state, value)` and return zero. That callee reads state and explicitly narrows its second input to a byte. The complete reconstruction retains signed conversion semantics; CFE conversion/clamp lowering remains different.
- `80022C58` (MATCH): extract a command byte index and unsigned high-halfword duration, pass the duration's actual address to `8001E930`, then update a packed 12-byte control record at state+0x168+index*12. If record+4 was nonzero, clear record+0; write the scaled duration at record+4. `8001E930` consumes an unsigned-word pointer and scales that pointed value by 256, corroborated by its complete native body and the prior C11 reconstruction. The real address-taken local explains the native frame and stack slot. No count or maximum valid index is invented for the control-record region.
- `80023D70` (NONMATCH): forward state, two command bytes and a signed low halfword to `80023B50`, then return zero. The complete callee entry explicitly masks the second and third arguments to bytes and sign-extends the fourth halfword. Its four-argument O32 declaration is real; changing the prototype merely to remove conversions is not a remedy.

All external functions are declared only, with no stub or embedded body. Data and JAL relocations resolve against protected canonical symbols. Original opcode names, complete state types, middleware version and arcade equivalence remain unknown. The authenticated arcade checkout is absent and no third-party source was copied.

## Flags, diagnosis and bounded outcomes

All selected source headers use `-g0 -O2 -mips2 -G 0 -non_shared`, plus the scorer's unchanged `-Wab,-r4300_mul`.

- `21BF0` matched the first natural source. Its O1 control differs in 25/26 words plus eight extra words.
- `22C58` initially differed in 26/31 words. Workbench diagnosis identified equal 40-byte frames but command-read/temporary differences. A common word local was rejected because it extended liveness and expanded the frame. The natural order “extract index, then initialize address-taken duration” produced the native single read, byte spill and delayed duration store, closing the match in the third source form. Final O1 differs in 30/31 plus five extra words.
- `21F08` initially differed in 14/24 words, including a narrowed halfword read. A word mask restores the native read and improves to 9/24 register sites with no excess. Natural identifier-local and result-width controls did not close it. Final O1 is 24/24 plus seven extra words. Next hypothesis: recover the original helper/handler declaration and IDO argument-extraction spelling that keeps the high-halfword conversion in a0; do not invent a register argument.
- `22514` remains 23/27 at O2 after explicit signed-word/halfword conversions improved the initial 25/27 form. O1 is 27/27 plus eleven extras. Next hypothesis: authentic signed conversion and clamp-expression source or a focused CFE trace; avoid fake keepers or weakened arithmetic semantics.
- `23D70` retains its natural initial form at 5/18 O2 words differing; word masks and extra meaningful locals did not improve it. O1 is 17/18 with no excess. Next hypothesis: original command extraction spelling and prototype evidence for the native word-read/argument-conversion lowering, without fabricating a different callee ABI.

The unmodified workbench ran before each non-exact function's refinements, using canonical target and relocated candidate objects only in temporary storage. `experiments.json` records numerical diagnostics and bounded controls. Each hypothesis used at most six directed source forms. These are deliberate honest stops; no padding, keepers, fake formal, volatile trick, inline assembly, compiler edit or flag sweep was used. Only the two exact bodies are submitted under `cloud/matches/`.

## Replay and integration

`verification.json` binds all five final source hashes to ten O2/O1 rows. The two accepted O2 bodies are 57/57 relocated words /228 bytes, with no differing or extra words, unresolved symbols, unverified relocations or errors. Three complete nonmatches reside only under `nonmatch/`.

```sh
python3 cloud/work/boot_tail/BT05-medium-next/verify.py
```

`status_delta.csv` is the central writer's integration input. Only this packet folder and two matching C files are edited. No targets, symbols, locks, layouts, scorer/compiler, runtime image, farm, spec or production gate are touched. The accepted `800D1248` path and helper restriction remain untouched. No ROM, raw disassembly dump or object is published.

## Independent review

Paired review inspected all five actual source bodies, packed prefixes/control records, the genuine mutable duration local, fallback operation and signed-halfword semantics. Dispatcher offsets +0x324/+0x658/+0x754/+0x8A4/+0x960 corroborate genuine state/command inputs and byte results; relevant complete callee entries corroborate pointer, u16, u8/u8/s16 and pointed-word mutation contracts. All ten control rows and source hashes independently reproduced: two strict bodies, 57 words /228 bytes; three accurately archived nonmatches, 276 bytes. No artificial source/ABI/layout blocker was found. Receipt: `independent_review.json`. All changed submissions, protected target hashes and 161 static locks pass.
