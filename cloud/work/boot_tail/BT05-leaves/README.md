# Packet 3: seven BT05 macro-handler leaves

Seven new strict local matches, **240 bytes**. No cartridge coverage or promotion claim.
Independent paired source/ABI review and fresh strict replay passed with all seven source hashes unchanged. Exact publication-head CI remains required.
The 21 boot-tail metadata tests, reproducible-ledger check and opcode-screen check pass; these are focused checks, not production/ROM validation.

- Branch: `dot/boot-tail-bt05-leaves`.
- Base: completed Packet 2 `76780b3a1b3e26c54b86e1f153344e92dac15b20`.
- Explicit stack dependency: unmerged research [PR #59](https://github.com/cabi24/SFRush2049-decomp/pull/59), head `21e104a22575cf4d639261d2f6d9535913074e6a`.
- Master prerequisites #52/#54 are merged at `b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`.
- Exact seven-function claim was recorded by the central ledger owner at `9602cf96b41e7cb718dcf932da7786ffdb3a31f9`. This packet does not edit that owner's generated files.
- Packet 2 preflight was independently replayed in this working clone: target hashes, all 439 extents/99,120 bytes and the preexisting getter pass unchanged.

## Exact scope and native semantics

| Function | Bytes | Native-evidenced operation |
|---|---:|---|
| `80021F68` | 28 | Zero packed state words at +0x1C and +0x20 |
| `8002243C` | 32 | Store command's high halfword shifted left 15 at packed state +0x28 |
| `800225AC` | 48 | Store command bits 16..23 into halfword table `D_8004BE98`, indexed by bits 8..15 |
| `800225DC` | 32 | Copy command bits 16..23 and 8..15 into state bytes +0x6A/+0x6B |
| `80022A78` | 32 | Set packed state +0x24 flag mask 0x80 |
| `80023710` | 36 | Set packed state +0x24 flag mask 0x40000 |
| `80023734` | 32 | Copy command bits 8..15 and 16..23 into state bytes +0x99/+0x9A |

All return zero and have no callees. Every row is `in_scope`, under 64 bytes, originally open and lacks a boundary-blocker note. The exact half-open extents are address plus the listed size. No out-of-range data was inspected.

## ABI and source review evidence

`func_80023E9C` passes the active-state pointer in a0 and the two-word command pointer in a1 at every one of these seven call sites, and consumes the low byte of v0 afterward. Call offsets are +0x500, +0x674, +0x6AC, +0x6C8, +0x6E4, +0x700 and +0x71C. Thus unused state/command formals are genuine dispatcher ABI inputs, not fake parameters inserted to force spills.

Every source declares the same compact partial `MacroState` layout with documented unknown byte ranges. These are real object-offset gaps, not stack-padding locals. `#pragma pack(1)` explains the native unaligned word operations even at numerically aligned offsets; no unaligned command accesses are needed. `#pragma pack(0)` restores default layout before `MacroCommand`. No conflicting production type/header changes are proposed.

Original field names, opcode names, N64 middleware version and complete object layout remain unknown. These sources are native reconstructions, not copied public MusyX code. The existing `REFERENCES.md` named `macHandleActive` source lead is context only. The authenticated arcade reference checkout is absent; no arcade identity is invented.

## Flags and bounded refinement

All publication headers use `-g0 -O2 -mips2 -G 0 -non_shared`; the scorer adds `-Wab,-r4300_mul`. Six initial forms matched at O2. O1 controls also matched five, but `800225AC` worsened to 11/12 differing words plus one extra word: O2's shared command-word value and leaf shape fit the native body. Tiny spill/leaf patterns alone do not prove O1.

`80021F68` initially differed in 2/7 words at both O2 and O1. Before another variant, `tools/workbench.py diagnose` classified exactly two aligned schedule sites, no register/constant residual and no frame. One natural refinement, chained zero assignment, reproduced the packed stores' order and strictly matched at O2. No padding, keepers, dummy calls, extra ABI inputs, inline assembly, flag sweep, boundary edit or scorer relaxation was used.

`experiments.json` retains numerical controls and the bounded refinement, without raw instruction dumps. `verification.json` records source hashes and strict relocated counts: 60/60 words, zero extra words, unresolved symbols, unverified relocations or errors. Reproduce after standard pinned setup:

```sh
python3 cloud/work/boot_tail/BT05-leaves/verify.py
```

`status_delta.csv` is the per-packet integration input. The central writer owns `STATUS.csv`, `research.json`, generated README/clusters and D10. No targets, lock files, layout, symbols, compiler, scorer, runtime-image sources or production gates changed. No ROM, raw disassembly or object file is included.

## Independent review

The paired BT03-high reviewer inspected all seven actual sources, the canonical caller windows and their complete native leaf operations. All seven source hashes equal `verification.json`; independent strict replay passed 60/60 words, 240 bytes, with zero extra words, unresolved symbols, unverified relocations or errors. No fake ABI parameter, padding local, callee body or source/ABI blocker was found. `independent_review.json` records the hash-bound replay.
