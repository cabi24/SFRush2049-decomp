# Packet 3 C11-small: four strict matches and one frozen nonmatch

Local result: **4 strict matching bodies / 104 B**; **1 complete NONMATCH / 44 B**.
Independent final review and exact aggregate PR-head CI are pending. The central
ledger therefore keeps the four submissions claimed until that evidence closes.
No cartridge coverage, maintainer acceptance or promotion is claimed.

## Scope, base and provenance

- Exact source base: completed Packet 2 commit `76780b3a1b3e26c54b86e1f153344e92dac15b20`.
- Explicit unmerged dependency: draft PR #59 at `21e104a22575cf4d639261d2f6d9535913074e6a`.
- Master rechecked at `b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`, containing #52 then #54.
- Branch: `dot/boot-tail-p3-c11-small`. Only the five claimed functions in C11
  `[0x8001E0E0, 0x8001E9B0)` were reconstructed. Two are indirect-call wrappers,
  despite the inventory recording zero direct callees; they are not leaves.
- Packet 2 passed fresh pinned compiler download/hash, all three target manifest
  members, all 439 starts/sizes, and strict getter replay before any new candidate.
- All five are in-scope, fresh-open on the base, outside the eight census-boundary
  blockers, and disjoint from other workers and Claude runtime-image/farm work.
- Shared compiler/tool hashes are in `../packet2/preflight.json`. Exact source
  hashes, flags, residuals and relocation results are in `scores.json`.

## Strict results and flag evidence

All submitted files use C89 and line-one flags
`-g0 -O2 -mips2 -G 0 -non_shared`; the unchanged scorer adds `-Wab,-r4300_mul`.

| Function | Native half-open extent | O2 result | O1 control | Native signal |
|---|---|---|---|---|
| `func_8001E740` | `[8001E740,8001E768)` 40 B | MATCH, 0/10 | 6/10 + 1 excess word | 24-byte real call frame; callback loaded before frame adjustment; a0 forwarded, a1 zero in indirect-call delay slot; no argument spill |
| `func_8001E768` | `[8001E768,8001E790)` 40 B | MATCH, 0/10 | 6/10 | same real call frame/load scheduling; a0 forwarded to indirect callback without argument spill |
| `func_8001E930` | `[8001E930,8001E940)` 16 B | MATCH, 0/4 | MATCH | frameless load, unsigned eight-bit scale, return-delay-slot store; both levels emit the same body |
| `func_8001E9A0` | `[8001E9A0,8001E9A8)` 8 B | MATCH, 0/2 | MATCH | frameless unsigned shift in return delay slot; both levels emit the same body |
| `func_8001E790` | `[8001E790,8001E7BC)` 44 B | NONMATCH, 2/11 | 9/11 + 1 excess word | frameless state multiply/store, no argument spill; O1 diverges structurally |

The first four have zero unresolved symbols, unverified local relocations,
relocation errors or nonzero excess words. Ordinary zero assembler alignment
padding is not counted as native function bytes. O2 fits the wrappers and PRNG
structure, but the two tiny conversions do not independently distinguish O2
from O1. This does not prove all of C11 was one original translation unit.

## Actual ABI and semantics audit

`E740` reads callback `D_80038018`, passes its real size argument in a0 and zero
mode in a1, and returns the callback's v0 unchanged. The declaration models a
pointer-returning allocation callback. Read-only supporting usage at
`80010C68+0x68..0xB4` passes size 0x400 and mode 0x80 to the same callback; its
return is stored at +0x98 and used to form the thread stack end at +0x90/+0xA0. This is a bounded allocation
role hypothesis, not a recovered original callback typedef.

`E768` reads `D_8003801C`, forwards one actual pointer in a0, and does not create
an artificial return value. Supporting release-shaped calls at
`80010D74+0x24..0x50` pass existing pointer globals to this same callback and
ignore its return. Neither submission contains a stubbed callback body or fake
formal; test-only callbacks live solely in the host semantic harness.

`E790` has no inputs, updates unsigned 32-bit state `D_8002CC50` modulo 2^32 by
multiplier 2822053219, and returns low16(state >> 6). The public `sndRand` source
lead is retained; this body remains NONMATCH. Four scanned callers are the ones
already recorded in the inventory. There is no new ABI or extent assertion.

`E930` has one genuine pointer to a 32-bit unsigned time value and multiplies the
stored value by 256 modulo 2^32. `E9A0` has one genuine unsigned 32-bit value and
returns floor(value / 256). Native uses logical shifts, including for bit31-set
inputs; signed arithmetic is not substituted. The existing callers recorded in
the inventory pass a pointer/value respectively, and no helper body is inserted.

No shared-header declaration for these address-spelled targets/globals was found
in the searched `include/`, `src/` or existing `cloud/matches/` files. Submission
types are self-contained; no shared type or symbol file was changed.

## Reference-source evidence and limits

Pinned primary reference: [AxioDL/musyx snd_service.c at
78d2e16e4905fc675952162d331c24d5198b2687](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_service.c),
[CC0-1.0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE).
The named definitions `sndRand`, `sndConvertMs`, and `sndConvert2Ms` support the
observed unsigned operations. Small standalone C reconstructions use native
address-spelled symbols; no reference translation unit, proprietary header,
table or raw native instruction dump is vendored. This Dolphin/PC/versioned
source is not proof of the exact N64 version or original C type spelling.

## Frozen PRNG residual and bounded controls

`func_8001E790_NONMATCH.c` is the complete best source, identical to variant 1.
Strict O2 comparison is 2/11 differing words, no excess, no unresolved/unverified
relocations and no errors. Native/candidate operation counts and all constants
agree. At byte offsets +0x1C and +0x28, the shifted result is held in temporary t8
rather than return register v0, and the final zero-extension consumes it.

Before additional variants, `tools/workbench.py diagnose` classified locally
materialized target and fully relocated candidate object slices as
`allocation-mismatch`: 2 register differences, 0 opcode/constant/frame changes.
`diagnosis.json` retains only classification/offset metadata. Raw diagnosis,
native slices and objects remain local. The tool did not prove a source lever;
no forced-register instrumentation, altered compiler or flag sweep was used.

Eleven directed, semantics-preserving source controls are archived under
`variants/` with both O2 and O1 receipts:

1. direct unsigned-short return (best, 2/11);
2. explicit unsigned-short result local (6/11);
3. explicit unsigned-int result local (6/11);
4. explicit narrowing cast (2/11);
5. unsigned-int result signature plus narrowing cast (2/11);
6. unsigned-int result plus explicit low16 mask (2/11);
7. register-qualified real unsigned-short result local (6/11);
8. unsigned-int local/result plus mask (6/11);
9. unsigned division by 64 replacing the shift (2/11);
10. unsigned remainder by 65536 replacing narrowing (2/11);
11. signed-int result carrying the nonnegative masked value (2/11).

These controls test the actual result's representation, not padding/keeper
variables. Rejected wider/signed signatures are not published as matches.
O1 controls are worse; some larger O1 bodies also report unpaired relocation
halves because the target-length comparison window ends before those later
instructions. No scorer change is proposed to rescue them. The packet froze
below its 20-variant bound because there was no improvement.

Next hypothesis requires new evidence: authenticate original N64 return typedef
and IDO conversion/coalescing context, or an instrumented return-web trace that
explains v0 ownership. Do not restart the same expression variants or alter the
generated target to erase two words.

## Verification and reproduction

```sh
python3 cloud/work/boot_tail/scripts/preflight.py
python3 cloud/work/boot_tail/scripts/score_c11.py
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 cloud/work/boot_tail/scripts/screen_opcodes.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
python3 tools/cloud/check_submissions.py --base 76780b3a1b3e26c54b86e1f153344e92dac15b20 --head HEAD
```

The 28 focused tests cover existing ledger/native metadata checks, fail-closed
census drift and duplicate starts, disjoint claim extents, and C89 host semantics:
callback forwarding/return/counts, unsigned boundary values, and 500 canonical
PRNG state/output transitions. Host tests validate stated semantics only; they
cannot turn the PRNG NONMATCH into matching bytes. Compiler receipts use the
unchanged strict relocated scorer. No production image/ROM gate was run.

Publication is one peer-reviewed aggregate first-wave draft PR, explicitly
stacked on #59 but targeting master so repository CI triggers. The lead owns
central status and exact remote-tree/head verification. Merging is left to the
owner's independent checker.
