# BT03-high: six gated API wrappers

Six initial-form strict O2 matches, **496 B / 124 words**. No NONMATCH source is
submitted in this packet. Independent paired source/ABI review passed; exact publication-head CI is
required; there is no cartridge-coverage or promotion claim.

- Branch `dot/boot-tail-bt03-high-api`, clean source base master
  `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Six claims activated centrally at `7d4a275c`, entirely within BT03-high and
  outside C11. Earlier BT03-high source packets remain frozen.
- This branch has no source dependency on previous matching drafts or research
  PR #59. Central generated status, claims and D10 are not changed here.
- Fresh canonical target manifests, complete 439-start/size inventory and existing
  getter replay pass; the validated pinned IDO installation is reused via IDO_DIR.

## Exact scope and ABI

| Wrapper | Bytes | Body callee | Genuine inputs |
|---|---:|---|---|
| `8001FE58` | 76 | `8001B8C4` | unsigned word identifier |
| `8001FEA4` | 84 | `8001B7C0` | identifier, unsigned byte value |
| `8001FEF8` | 84 | `8001B5F4` | identifier, unsigned halfword value |
| `8001FF4C` | 84 | `8001B3A0` | identifier, unsigned byte value |
| `8001FFA0` | 84 | `8001B4A4` | identifier, unsigned halfword value |
| `8001FFF4` | 84 | `8001B29C` | identifier, unsigned byte value |

Each returns -1 if `D_8002C630` is zero. Otherwise it calls `80014594`, forwards
its actual parameters to the listed body callee, calls `800145DC`, and returns
the body result. The meaningful result local must survive the final call; the
native 32-byte frame and spill/reload at +28 are reproduced exactly. Input
homes and byte/halfword stack reload offsets are also included in equality.

`80014594` and `800145DC` use a global nesting count and queue operations; neither
reads an incoming argument. They are declared as void no-argument functions.
The role description is a synchronization hypothesis, not a recovered public
API name. No implementation of either callee is included.

Each body callee is in the canonical census and first validates its incoming
word identifier through `8001EDF4`. The three byte callees explicitly mask the
second input to 0xFF; the two halfword callees mask it to 0xFFFF. The one-input
callee has no second input. These native entry sequences corroborate the source
prototypes and the wrappers' LBU/LHU argument reloads. Calls and return words
follow ordinary O32, with no invented extra register inputs.

All listed body callees call only in-census `8001EDF4` and, where applicable,
`80020610`. The synchronization pair's counted-static queue calls are also
resolved. No boundary-blocked or out-of-census dependency is introduced.
Original function names, value meanings, full types and middleware version
remain unknown; these are native reconstructions with no copied external body.

## Compiler evidence

All six first natural forms matched with `-g0 -O2 -mips2 -G 0 -non_shared` plus
the scorer's unchanged `-Wab,-r4300_mul`. O1 controls differ in 15/19 words for
`1FE58` and 17/21 for each other wrapper, with no nonzero excess. The O2 frame,
argument-home sequence and real result preservation support the selected level.
No refinement, near-match diagnosis or flag sweep was needed because every
initial O2 form was exact.

`initial_controls.json` preserves the original-source controls. Final commented
sources are bound by `verification.json`: twelve O2/O1 rows, six full relocated
MATCHes and six rejected O1 controls. The accepted rows have zero differences,
extra words, unresolved symbols, unverified relocations or errors. No local
rodata or switch table is involved.

## Replay and integration

```sh
python3 cloud/work/boot_tail/BT03-high-api/verify.py
```

`status_delta.csv` is the sole central writer's integration input. Independent
peer review, publication and exact-head CI are separate gates; merging remains
with the checker. Only six `cloud/matches/boot_tail/*.c` sources and this packet
directory are changed. No target, compiler, scorer, lock, layout, symbol,
runtime-image, farm, forbidden helper work, ROM/raw instruction dump, object
or credential is edited or published.

## Independent review

The paired BT05/BT07 reviewer independently reproduced all twelve O2/O1 rows
and exact source hashes on commit `6c9ae9f3`. It reviewed every complete wrapper,
actual body-callee byte/halfword entry masks, and both complete no-argument
synchronization helpers. `independent_review.json` confirms six strict matching
bodies and no source/ABI blocker; rejected O1 controls remain unaccepted.
