# Packet 4: five self-contained BT05 medium leaves

**Three strict local matches, 296 bytes; two COMPLETE-NONMATCHs, 248 bytes.** Independent paired source/ABI review and fresh strict replay passed; publication-head CI remains required. No cartridge-coverage or promotion claim.

- Branch `dot/boot-tail-bt05-medium`, clean master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Atomic central claim `837622e6`: `800218CC` (112), `800223D0` (108), `8002245C` (84), `800224B0` (100), `80023190` (140), totaling 544 bytes.
- Half-open extents are each listed address plus size. All five are in-scope 64–255-byte BT05 rows, previously open, with no callees, jump tables, local literals or boundary-blocker annotation.
- This source branch has no dependency on earlier matching drafts. Those sources remain frozen; central generated status, claims and D10 are owned separately.

## Native reconstruction and ABI

`800218CC` has no inputs. Caller `8001C1D8` invokes it as part of initialization. It resets an 8-by-16 byte table at `D_80056160` and a 32-byte table at `D_800561E0` to 255. The first source shape uses nested integer-index loops; the second uses one byte-index loop. IDO leaves the inner nested loop scalar but unrolls the independent byte loop fourfold, reproducing both native loop shapes without padding, opaque bounds or optimization pragmas.

The other four functions are macro handlers with genuine state/command pointers in a0/a1 and zero byte results. `80023E9C` passes and consumes those values at call offsets +0x43C (`23190`), +0x690 (`223D0`), +0x770 (`2245C`) and +0x78C (`224B0`). No extra formal is invented.

All four use the same self-contained partial packed state type. The relevant fields are unsigned word +0x28, unsigned halfword +0x2C, unsigned word +0x30, signed word +0x38 and unsigned halfword +0x50. Unknown byte ranges express object offsets, not stack-padding locals. Aligned two-word commands are declared separately after restoring default packing. Original field meanings, opcode names, N64 middleware release and complete object types remain unknown. No shared header is changed; the authenticated arcade checkout is absent.

- `800223D0` (NONMATCH): add the signed high halfword of command word 0 to state+0x28 shifted right 15; clamp below zero and above 65535; store the result shifted left 15. The complete reconstructed body retains native 32-bit arithmetic and packed stores, but register assignment differs.
- `8002245C` (MATCH): read command word 1 as divisor; when nonzero, store `(state_word_28 >> 8) / divisor` as the packed halfword at +0x2C, otherwise store zero. Native divisor guard, division checks, packed byte stores and return words all match.
- `800224B0` (MATCH): compute `((command_word_1 & 65535) * (state_word_30 >> 16)) >> 7`, add command word 0's high halfword, clamp above 60000 and store shifted left 15 at +0x28. Word-mask spelling retains the native full-word command read; commuting the two independent unsigned addition terms reproduces native operand order without changing semantics.
- `80023190` (NONMATCH): combine packed halfword +0x50, command bytes and a signed byte gain using 32-bit fixed-point arithmetic, clamp to `[0, 0x7F0000]` and store at +0x38. Unsigned intermediates preserve low-word multiplication before signed shifting; explicit clamp result is meaningful source state, not an unused keeper. Residual register/allocation and conditional lowering remain.

No body is copied from an external source. No semantic identity is claimed beyond these native observations.

## Flags, diagnosis and bounded effort

All accepted source headers use `-g0 -O2 -mips2 -G 0 -non_shared`. The scorer adds `-Wab,-r4300_mul` unchanged. The first natural `2245C` form matched immediately; O1 is worse. Before refining the other four, the unmodified workbench diagnosed read-only canonical targets and freshly compiled, relocated candidate objects in temporary storage.

- `218CC`: initial pointer-based loop formulation differed in 28/28 words plus two extras due to inner-loop unrolling and induction allocation. Reconstructing the actual two table dimensions produced a strict match. The final O1 control is 28/28 plus six extras; O2's mixed scalar/unrolled loops are the native signature.
- `224B0`: initial narrowing halfword cast differed in 25/25 words; replacing it with a word mask reduced the residual to one commutative operand site. One natural operand-order refinement closed it. Final O1 is 12/25 plus three extras.
- `223D0`: initial source differed in 17/27 register-only sites; explicit meaningful command/delta/result locals improved to 11/27. Bounded casts, local types, extraction ordering and documented O1 controls did not close the allocation residual. Final O1 is 27/27 plus seven extras. Next hypothesis: authentic handler declarations or a focused IDO allocation trace for the sign-extended high-halfword value; no arbitrary padding, keepers or fake arguments.
- `23190`: initial source differed in 21/35 words. Explicit gain extraction and a meaningful clamp-result local improve the best complete source to 18/35. Final O1 is 35/35 plus thirteen extras. Next hypothesis: recover the original signed-byte extraction and clamp helper spelling or use trace evidence to explain the native colored gain and accumulator webs. Do not reopen with blind flag sweeps or extra formals.

`experiments.json` contains numerical controls and source-form descriptions, with no raw assembly. Attempted directed-control counts stay below twenty for every hypothesis; the final two residuals are intentionally honest nonmatches. Explicit register storage-class controls at O1 were rejected: they did not reproduce the native leaf allocation. No compiler, scorer or target edits were made.

## Replay and integration

`verification.json` binds final source SHA-256 hashes to ten O2/O1 control rows. The three accepted O2 rows are 74/74 relocated words /296 bytes, zero extras, unresolved symbols, unverified relocations or errors. Both nonmatching complete sources reside only under `nonmatch/`. Reproduce after standard pinned setup:

```sh
python3 cloud/work/boot_tail/BT05-medium/verify.py
```

`status_delta.csv` is the integration input for the sole central ledger writer. Only this packet directory and three matching C files are changed. No targets, symbols, layouts, locks, scorer/compiler, runtime image, farm, spec or production gate are edited. The accepted `800D1248` path and helper restriction remain untouched. No ROM, raw instruction dump or object is published.

## Independent review

Paired review inspected all five actual source bodies, unsigned fixed-point arithmetic, packed field offsets and genuine inputs. Dispatcher call windows +0x43C/+0x690/+0x770/+0x78C confirm handler a0/a1 inputs and byte results; the initialization caller at +0x198 has no inputs. Reset tables cover exactly 128 plus 32 bytes. All ten O2/O1 control rows and source hashes independently reproduced: three strict matches, 74 words /296 bytes, and two accurately archived nonmatches, 248 bytes. No artificial source or ABI blocker was found. Receipt: `independent_review.json`. Changed-submission replay passed all three, all 161 static locks remain intact, and protected target hashes pass.
