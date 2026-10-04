# Packet 3: final six small BT05 wrappers

**Four strict local matches, 152 bytes; two COMPLETE-NONMATCHs, 100 bytes.** Independent paired source/ABI review and fresh strict replay passed; publication-head CI remains required. No production or cartridge-coverage claim.

- Branch `dot/boot-tail-bt05-wrappers`, clean master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Claim activation recorded centrally at `dc6e8b67`, `wave2/claims.json`; previous seven-leaf and nine-control BT05 packets remain frozen for integration.
- Exact functions: `80021B9C` (36), `80021BC0` (48), `80022580` (44), `80022A40` (56), `80023520` (36), `80023544` (32), totaling 252 bytes. Every half-open extent is its address plus listed size, in the under-64-byte BT05 population. No boundary-blocker row is included.
- This source branch starts directly on master and does not depend on the first-wave draft. The central writer owns generated status and D10 changes.

## Natural reconstructions and ABI

All six entry points receive a state pointer in a0 and an aligned two-word command pointer in a1 from `func_80023E9C`, whose dispatch cases consume low-byte v0. The unused command formal in `80021BC0` and `80022A40` is real dispatcher ABI; its native spill is not a pretext for inventing a parameter. `MacroState` is a partial packed object layout: unknown object bytes to +0x30, one word there, unknown bytes to +0x60, and one word there. These ranges are object-offset evidence, not local stack padding. The original field names and N64 middleware version remain unknown.

- `80021B9C`: write byte 1 into command+6, then forward state and command to `8002193C` and return its byte result. The complete 608-byte callee reads both inputs and explicitly produces byte-sized boolean results; the wrapper's command must be mutable. Character-pointer access to the command representation performs the evidenced byte update.
- `80021BC0`: call `8001EB10(state)`, then `8001F6EC(state)`, then return 1. Both complete in-census callees consume only the state input. The native wrapper saves/restores a0 around the first call, and spills the unused second formal.
- `80022580` (NONMATCH): pass state and command bits 8..15 to `8001EF8C`, then return 0. The complete 432-byte callee uses the pointer and truncates its second argument to a byte. The residual is register choice, not an invented argument or missing work.
- `80022A40` (NONMATCH): pass the low byte of packed state word +0x60 and constant 1 to `8001489C`, then return 0. The complete 92-byte callee consumes the index and byte mode; its only call is in-census `80011A3C`. A natural word mask preserves the native packed-word read but does not yet reproduce its argument scheduling.
- `80023520`: pass state, command and packed state word +0x30 to `800233B0`; return its byte result.
- `80023544`: same callee and state/command inputs, with third value zero. The complete 368-byte callee consumes all three inputs, stores the third at state+0x30 and returns zero. No extra register-passed parameter is needed.

Every callee address is present in the canonical target symbols. The reviewed complete callee extents and inventory have no boundary-blocker annotation. No callee body is supplied or claimed here. No conflicting shared-header changes are made. The authenticated arcade reference checkout is absent; these are native reconstructions with no invented arcade identity or third-party code copy.

## Flags, experiments and bounded stop

All four matches use the initial natural source form at `-g0 -O2 -mips2 -G 0 -non_shared`; prescribed O1 controls worsen the 24-byte-frame wrappers and are recorded numerically. The scorer's mandatory `-Wab,-r4300_mul` is unchanged.

Before refining either nonmatch, the unmodified workbench `diagnose` ran on canonical target and candidate objects held only in temporary storage:

- `80022580`: 2/11 O2 words differ, two register sites, no schedule/constant difference and the same 24-byte frame. Fifteen natural source forms including byte/word locals, masks, casts, parameter storage classes and expression order did not close the residual. The archived natural initial form remains best. Final O1 control is 10/11 words plus three extra words. Next hypothesis: measure how IDO's expression spelling/argument lowering keeps the shifted command value in the outgoing a1 register; authentic macro-handler declarations would help. Do not fake register retention.
- `80022A40`: the explicit-byte-cast initial source let IDO narrow a packed word read to one byte, causing 11/14 differing words. Diagnosis exposed three missing aligned instructions with the same frame. Replacing the cast with a word mask restores the packed word access and improves to 7/14. Seventeen natural forms plateaued there. The archived final O1 control is 13/14 plus one extra word. Next hypothesis: recover the authentic packed state-field/argument-expression declaration or obtain expression-lowering trace evidence. Volatile qualifiers, dummy ABI inputs, padding and inline assembly were not used to force the load shape.

All attempted forms are below the per-hypothesis bound. `experiments.json` preserves source-form descriptions and numerical controls, not raw disassembly. The two complete nonmatching sources reside only under `nonmatch/`; no match claim is made for them.

## Replay and integration

`verification.json` records final source hashes and eight fresh flag/control rows: four true matches (38 words /152 bytes), and both nonmatches at O2/O1. Reproduce after standard pinned setup:

```sh
python3 cloud/work/boot_tail/BT05-wrappers/verify.py
```

`status_delta.csv` is the central ledger integration input. Only this packet folder and four matching source files are changed. No target, symbol, layout, lock, scorer, runtime image, farm, spec or production gate is edited. The owner-accepted `800D1248` path and helper restriction remain untouched. No ROM, raw assembly or object is published.

## Independent review

The paired BT03-high reviewer inspected all six actual sources, the six dispatcher windows and the named callee ABI entries/returns. Mutable command+6, packed state+0x30/+0x60, genuine unused command formals and declared-only callees are supported. All eight frozen control rows and source hashes independently reproduced: four matches, 38 words /152 bytes; two accurately archived nonmatches, 100 bytes. No artificial source or ABI blocker was found. Receipt: `independent_review.json`. Protected target hashes and getter replay pass, all 161 static locks remain intact, and CI-equivalent changed-submission replay passes all four matches.
