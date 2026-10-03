# Packet 3: nine BT05 embedded-control wrappers

Nine new strict local matches, **396 bytes** (99 words). Independent paired source/ABI review and fresh strict replay passed with all hashes unchanged; exact publication-head CI remains required. No promotion or cartridge-coverage claim.

- Branch: `dot/boot-tail-bt05-controls`, clean base master `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central activation: `6dd6c6a4`, `wave2/claims.json` and D10; prior seven-leaf BT05 packet is frozen for integration.
- Exact range `[0x80023818,0x800239A4)`, nine contiguous 44-byte functions. Every row is in scope, under 64 bytes, previously open and absent from the research ledger's boundary blockers.
- This branch has no source dependency on the first-wave draft or PR #59. The existing Packet 1 research is a read-only reference; the central integrator owns its generated status and D10 updates.
- Fresh-base preflight passed: all three protected target-manifest members, exact 439-start/size census totaling 99,120 bytes, and strict 3/3-word `80010A00` replay.

## Reconstructed contract

Each function receives state in a0 and a two-word command pointer in a1. It calls `func_80023754(state, embedded_control, command, flag_mask)`, then returns byte zero.

| Function | State-relative control offset | Flag mask |
|---|---:|---:|
| `80023818` | 0xC4 | 0x00200000 |
| `80023844` | 0xD6 | 0x00400000 |
| `80023870` | 0xFA | 0x00800000 |
| `8002389C` | 0x11E | 0x01000000 |
| `800238C8` | 0x130 | 0x08000000 |
| `800238F4` | 0x142 | 0x04000000 |
| `80023920` | 0x154 | 0x02000000 |
| `8002394C` | 0xE8 | 0x10000000 |
| `80023978` | 0x10C | 0x20000000 |

The opaque `MacroState` and `MacroControl` types avoid inventing complete object layouts. Byte-pointer arithmetic selects evidenced embedded records. `MacroCommand` retains the same two-word layout used by the prior leaf packet. The external callee is declared only; no body, fake argument, local padding or inline assembly is supplied.

## ABI and dependency validation

Canonical `func_80023E9C` call offsets +0x7A8, +0x7C4, +0x7E0, +0x7FC, +0x818, +0x834, +0x850, +0x86C and +0x888 pass state/command pointers in a0/a1 and consume low byte v0. Thus both wrapper parameters and the return type have native call-site evidence.

`func_80023754` is a complete in-census 196-byte body ending at `80023818`. It reads packed state flags at +0x24 from a0, a 16-byte entry area plus count byte at +0x10 from a1, command words at +0/+4 from a2, and the supplied mask from a3. It only calls in-census leaf `80021548`, whose complete 96-byte extent contains no further calls. The helper is available in target symbols, so every JAL relocates exactly. No boundary-blocked dependency or nonstandard register-passed extra argument is involved. The helper itself is not reconstructed or claimed in this packet.

Original opcode names, control semantics, N64 middleware release and complete layouts remain unknown. The authenticated arcade checkout is absent; these are native reconstructions, not copied third-party source or invented arcade identities.

## Flags and proof

All nine initial natural source forms strictly matched at `-g0 -O2 -mips2 -G 0 -non_shared`. The native 24-byte frame, a1-to-a2 forwarding and mask in the call delay slot support O2. Prescribed O1 controls each differ in 10/11 words plus three nonzero extra words. `experiments.json` records both exact flagsets' numerical results; no refinement, flag sweep or diagnose step was necessary because each initial O2 form matched.

`verification.json` binds nine source SHA-256 hashes to 99/99 relocated full-word equality, zero extra words, unresolved symbols, unverified relocations or errors. The scorer adds `-Wab,-r4300_mul` unchanged. Reproduce after standard pinned setup:

```sh
python3 cloud/work/boot_tail/BT05-controls/verify.py
```

Only this packet directory and the nine `cloud/matches/boot_tail/*.c` sources are changed. `status_delta.csv` is an input for the sole central ledger writer. No target, symbol, layout, lock, scorer, runtime-image source, farm, spec or production gate is changed. Owner-accepted `800D1248` and the helper-path restriction remain untouched. No ROM data, raw assembly dump or object is published.

## Independent review and focused checks

The paired BT03-high reviewer inspected all nine actual sources and their offset/mask pairs, the dispatcher forwarding and result consumption, and the complete native callee contract. Independent fresh strict replay exactly reproduced `verification.json`: nine functions, 99 words, 396 bytes, unchanged source hashes, zero extras/unresolved/unverified/errors. No fake formal, padding, callee body or ABI blocker was found; receipt `independent_review.json`. Fresh changed-submission CI-equivalent rescoring passed all nine; all 161 static locks remain intact.
