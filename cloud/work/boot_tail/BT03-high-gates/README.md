# BT03-high: six additional gated wrappers

Six first-form strict O2 matches, **452 B / 113 words**. Independent paired review passed;
aggregate exact-head CI remains required. No cartridge coverage or promotion
is claimed.

- Branch `dot/boot-tail-bt03-high-gates`, clean source base master
  `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Exact six-function activation acknowledged in central `3f7c32b8`. All targets
  remain within BT03-high, outside C11. Prior packets are frozen for integration.
- No source dependency on unmerged research or preceding matching drafts. Central
  status, claims, generated clustering/research files and D10 are untouched.
- Fresh target manifests, all 439 starts/sizes totaling 99,120 B, and the existing
  getter replay pass using the unchanged pinned IDO installation via IDO_DIR.

## Native contracts

Every wrapper first checks byte `D_8002C630`. When enabled it calls no-argument
`80014594`, invokes the body operation, then calls no-argument `800145DC`.
These two helpers use global nesting/queue state; their complete native bodies
require no incoming argument. Their exact original names remain unknown.

| Wrapper | Bytes | Input types | Body operation | Result |
|---|---:|---|---|---|
| `80018D00` | 64 | unsigned word identifier | `80018C2C(identifier)` | void |
| `80018E2C` | 64 | unsigned word identifier | `80018D40(identifier)` | void |
| `8001906C` | 64 | unsigned word identifier | `80018FEC(identifier)` | void |
| `80020174` | 92 | halfword, byte, byte | `8001B1D0` with those inputs | body word, or -1 when disabled |
| `800203EC` | 80 | byte, halfword, byte | `8001BE14` with those inputs | void |
| `8002043C` | 88 | byte, halfword, byte | `8001B9F8` with those inputs and two zeros | void |

The first three body callees pass their word input to `80017644`, which masks
and compares an identifier rather than dereferencing it as a pointer. The other
callees' entry spills and narrowing operations corroborate the genuine halfword
and byte inputs. No unused wrapper formal is introduced.

The five-argument call in `8002043C` is real O32 ABI, not a frame-shaping device.
The body receives a fourth byte and fifth word: `8001B9F8` homes a3 at its new
sp+68 and later loads its low byte from sp+71; it loads the fifth word at sp+72
(original incoming sp+16) and stores both into state fields. The wrapper passes
zero for both. Its native a3 zero and outgoing stack+16 zero therefore represent
actual arguments. Only a declared prototype is supplied, never a callee body.

`80020174` uses one meaningful result local to preserve the operation result
through the final helper call; it returns -1 without calls when disabled. The
remaining five wrappers do nothing when disabled. Every actual frame, input
home, narrow reload, result spill and delayed instruction is in the strict proof.
All dependencies are in the canonical census or known counted-static queue
functions; no boundary-blocked destination is used. The unclaimed body callee's
internal switch table is not reconstructed, copied or needed for the wrapper's
fully resolved JAL relocation.

## Compiler evidence and bounds

All six initial natural forms matched with `-g0 -O2 -mips2 -G 0 -non_shared`,
plus mandatory `-Wab,-r4300_mul`. Five also match at O1. Their small wrapper
shapes therefore do not uniquely identify the original optimization level;
O2 remains the prescribed first successful level. The result-preserving
`80020174` differs in 20/23 words at O1, with no nonzero excess, supporting O2
for that member. No unsupported universal flag claim is made.

No source refinement, near-match diagnosis, fake keeper, artificial formal,
local padding, inline assembly or flag sweep was needed. Original public names,
full types and middleware release remain unknown. Sources are native
reconstructions with no third-party body copy.

## Replay and integration

```sh
python3 cloud/work/boot_tail/BT03-high-gates/verify.py
```

`verification.json` binds all twelve O2/O1 rows to exact final source hashes.
Only one O2 row per function is a submission claim; the five additional exact
O1 controls are not extra functions or bytes. All six submitted bodies have
zero differing/extra words, unresolved symbols, unverified relocations or errors.
`initial_controls.json` preserves the original-source experiments.

`status_delta.csv` is the central sole writer's six-row integration input. Only
these six submission files and this packet directory are edited. No target,
compiler/scorer, symbol, lock, layout, runtime image, farm, forbidden helper,
ROM, raw instruction dump, object, secret or production gate is modified or
published. Independent review and exact-head CI precede checker-owned merging.

## Independent review

The paired BT05/BT07 reviewer independently reproduced all twelve rows and exact
source hashes at `62570082`, reviewed actual wrapper and callee-entry ABIs, and
confirmed the real fifth outgoing argument. Its `independent_review.json` counts
six distinct O2 bodies only; the five exact O1 rows are flag controls, not extra
functions or bytes. No source/ABI blocker was found.
