# BT05 conditional macro-transfer research

**Five complete NONMATCHs, 688 bytes; zero matching bodies or verified bytes.** Independent research/source review and aggregate CI remain required. No cartridge-coverage or promotion claim.

- Branch `dot/boot-tail-bt05-conditions`, clean master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central activation `b5723bfa`: five previously unattempted contiguous functions in `[0x80021C58,0x80021F08)`.
- Exact extents: `21C58+132`, `21CDC+128`, `21D5C+156`, `21DF8+124`, `21E74+148`, each address prefixed `0x800`.
- Earlier source-led residuals remain frozen and are not reopened. Central status, claims, generated research and D10 remain separately owned.
- This temporary sparse worktree contains the same protected target/spec/tooling inputs; sparse checkout conserves inodes and changes no tracked input.

## Complete native operations

Every handler receives a genuine state pointer and aligned two-word command. The source consistently models only the observed packed state prefix: current program pointers +0/+4, saved pointers +8/+12, unsigned volume word +0x30, channel byte +0x4A and unsigned note halfword +0x50. Unknown object-byte ranges express offsets, not padding locals. `MacroCommand` is eight bytes, matching the native command stride.

- `80021C58`: if the note halfword is at least command bits8..15, look up the macro identified by bits16..31; on success, set the current start and offset command pointer.
- `80021CDC`: same conditional transfer, using the full unsigned high halfword of the volume word for comparison. The native code does not truncate this value to a byte; the newer public reference's narrower comparison was not copied.
- `80021D5C`: unless channel is255, obtain the modulation controller, shift it right seven and convert to a byte; compare with command bits8..15, then perform the same successful lookup/update.
- `80021DF8`: compare the low byte of the genuine random-helper result with command bits8..15, then conditionally perform the lookup/update.
- `80021E74`: on successful lookup, save the old current pointer pair into +8/+12 and install the new start/current pair; otherwise return the actual two-argument end-handler result. Native stores support one saved pair, not a fabricated modern multi-entry call-stack layout.

The first four return zero; the last returns zero on success or propagates the fallback byte result. Native dispatcher calls supply the actual two arguments and consume byte statuses. No third incoming argument is introduced.

## Callee ABI and source-family checks

`80016C20` consumes a halfword identifier and returns a pointer or null. Its native entry homes the incoming argument and reads its low halfword; its result is loaded as the macro pointer. Pointer offsets in these handlers are the low command halfword times eight. Pointer stepping assumes valid macro-program offsets within the actual program object on the N64 32-bit pointer model; this packet does not prove arbitrary malformed bytecode safe or claim wider-host pointer behavior. This is ordinary O32 pointer/scalar behavior, with declared-only callees.

The pinned CC0 MusyX [synthdata.h](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/include/musyx/synthdata.h), blob `3170e8ffabc2bd1e8c4212a0d32000d0657cee73`, independently declares `MSTEP* dataGetMacro(u16 mid)`. The analogous [synthmacros.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c) definitions provide conditional-transfer family context only. `references.json` records the revision and license. The actual halfword ABI was not broadened to suppress conversion instructions.

`800214C8` forwards the state pointer and an internal control-record address to `80021150`, whose native return masks v0 to16 bits. Its halfword return is therefore promoted to signed int before the arithmetic right shift in the caller, as in native code. `8001E790` likewise returns a genuinely narrowed halfword; the byte conversion is explicit before comparison. `80021BC0` consumes state and the real command formal, then returns a byte status. Every callee is available in canonical symbols, and no boundary-blocked target, local literal or switch table is required for these caller reconstructions.

Original N64 source release and complete object types remain unknown. Newer PC/Dolphin layouts, return declarations and call-stack behavior are not assumed to be N64 definitions. No third-party body is vendored.

## Bounded source controls and stop

All candidates began with natural O2 source and prescribed O1 controls. Before refinement, the unmodified workbench diagnosed freshly compiled, relocated candidates against canonical target objects held only in temporary storage. Every native frame is24 bytes.

Early-return source forms introduced extra branches and zero-result assignments. Expressing the four conditional operations as guarded blocks with a single common zero return removed those structural/excess differences. The remaining shared limitation is argument lowering: IDO routes the narrowed macro identifier through a temporary plus an extra move where native code keeps it in outgoing a0; subsequent temporary allocation and comparison positions differ. The saved-pointer handler is initially an allocation-only residual.

One representative genuine u16 identifier local and a combined boolean/lookup guard did not improve the residual. A public-source-inspired internal-linkage control caused the unreferenced standalone body to disappear; it was rejected, without adding a fake caller or keeper. A word-sized status control was also ineffective. Those mechanisms were not repeatedly swept across every body. At most six natural source forms were attempted for any member.

| Function | Final O2 differing/target words | Final O1 differing/target words | O1 extra words |
|---|---:|---:|---:|
| `21C58` |21/33|33/33|5|
| `21CDC` |21/32|32/32|5|
| `21D5C` |21/39|38/39|4|
| `21DF8` |19/31|31/31|7|
| `21E74` |16/37|36/37|7|

The final O2 candidates have no nonzero extra words, unresolved symbols, unverified relocations or errors; every one is still NONMATCH. `experiments.json` preserves initial and directed numerical controls. Next useful input is authentic N64 macro/data lookup compiler/declaration context or a focused argument-lowering trace, rather than more repetitions of known ineffective casts, fake formals or flag sweeps.

## Replay and integration

```sh
python3 cloud/work/boot_tail/BT05-conditions/verify.py
```

`verification.json` binds five final source hashes to ten O2/O1 control rows. A successful research replay is not a matching verdict. Every C candidate resides only under `nonmatch/`; no submission is added under `cloud/matches/`. `status_delta.csv` supplies five honest nonmatch rows to the central writer.

Only this packet directory changes. No target, compiler/scorer, symbol, lock, layout, runtime image, farm, spec or production gate is changed; the accepted `800D1248` path and restricted helper work remain untouched. No ROM, raw instruction dump, object or credential is published.

The sparse worktree initially omitted read-only storage-owner dependencies referenced by `rom_owned_data.json`, including `cloud/work/static_C16/candidate.c` and an integration baseline. Restoring the manifest-referenced tracked directories from the unchanged base resolved the missing-file checks; no locked source or manifest was edited or re-pinned. Protected target hashes, the existing getter and all 161 static locks then passed.

The paired reviewer independently reproduced all ten hash-bound controls and
reviewed all five actual bodies/callee ABIs at source commit
`ad2258e4741ceff45ce125aef8ae18210fa0d1c3` (tree
`93c2d042b719cfa5d96085c75b7c92611520f77a`). The valid-program pointer-offset
qualification is retained. `independent_review.json` records PASS for complete
nonmatching research only, with zero matching credit.
