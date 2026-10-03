# BT03-low-B: two leaves and six bounded wrapper nonmatches

Base `301d9e7552ad4fd7f54a38796db84671e1000d35`, branch
`dot/boot-tail-p3-bt03-low-b`. Exact central claim acknowledged at
`6dd6c6a4` in the coordinator's second-wave claim record.
This packet is independent of the frozen first-wave source changes and leaves
all shared STATUS/D10 editing to the coordinator. Target/scorer/setup bytes
are unchanged from the completed Packet 2 checkpoint `76780b3a`.
The preflight checked the target manifest, all 439 census extents, the existing
getter's strict MATCH, and the full pinned compiler-file hash set.

## Outcome

- Two local strict matches, **24 B**: `80014E00`, `80014E10`.
- Six **COMPLETE-NONMATCH** bodies, **260 B**, retained here only.
- No integrated cartridge-coverage, production, promotion or merge claim.
- Independent source review and exact-head CI are required before final
  VERIFIED-BODY status is applied to the two submissions.

| Function | Bytes | Selected baseline | O2 diff / excess | O1 diff / excess |
|---|---:|---|---|---|
| 80014E00 | 12 | O2 MATCH | 0/3, 0 | 0/3, 0 |
| 80014E10 | 12 | O2 MATCH | 0/3, 0 | 0/3, 0 |
| 80014E64 | 44 | O2 NONMATCH | 8/11, 0 | 10/11, 1 |
| 80014E90 | 44 | O2 NONMATCH | 8/11, 0 | 10/11, 1 |
| 80014EBC | 44 | O2 NONMATCH | 8/11, 0 | 10/11, 1 |
| 80014EE8 | 44 | O2 NONMATCH | 8/11, 0 | 10/11, 1 |
| 80015318 | 48 | O2 NONMATCH | 3/12, 0 | 12/12, 4 |
| 80015348 | 36 | O1 NONMATCH | 6/9, 0 | 1/9, 0 |

`verification.json` binds every actual source hash to both strict scorer
results, compiler-pin digest, target-manifest hash and unmodified scorer hash.
The table describes full native extents. Nonzero padding beyond a target is
counted by the strict scorer; a displaced terminal no-op can make a candidate
longer without a nonzero excess count, which is why word equality is still
required. None of these six nonmatches is credited as matching.

## Whole-body and ABI reconstruction

- `14E00` returns the address `D_8002C604`, not its contents. No arguments are
  read and no known direct caller is in the inventory. The pointee remains
  opaque; no stronger historical API name is asserted.
- `14E10` clears halfword `D_80038390`. Caller `107E0` supplies no arguments.
- `14E64`, `14E90`, `14EBC`, `14EE8` select one of four word offsets at +0,
  +4, +8 or +12 in a resource header. They add it in **bytes** to the resource
  base and pass the resulting pointer plus a low-16-bit identifier to
  `14E1C`, returning that helper's pointer-or-null result. Corresponding
  callers `14F14`, `14F80`, `14FEC`, `15058` test the returned pointer and
  use its payload. `14E1C` was independently inspected: it traverses relative
  next offsets and compares halfword identifiers; no third argument is read.
- `15318` reads a halfword count at data+0 and passes payload at data+4,
  that count, and a low-16-bit identifier to `164D0`. The callee reads three
  arguments. The native a3 copy is a temporary for the original data pointer,
  not a fourth argument. The wrapper's known caller ignores the callee result.
- `15348` forwards a low-16-bit identifier to `1661C`; the caller ignores
  the result. `1661C` reads the identifier, not extra stale argument registers.
- All six native wrappers home the incoming first argument. Narrow unsigned
  formals model this real ABI evidence; no fake keeper formal was added.
  The resource types are partial headers and make no allocation-size claim.
- The two leaves match at both O2 and O1. O2 is the documented default and
  fits their frameless emission, but their tiny native bodies do not uniquely
  distinguish the optimization level.

## Diagnosed boundary and rejected controls

Workbench `diagnose` was run on each first residual, for O2 and O1, before
source variants. It reports structural/argument-lowering differences rather
than a missing relocation or table. Native temporary objects for diagnosis
contain the pinned words without symbolic relocations; the workbench's
relocation-site warning is therefore expected. The authoritative strict
scorer resolves all candidate references before counting words.

The common native form homes a0, then narrows **in a0** before the call.
Canonical ANSI u16 source at O2 instead introduces an argument temporary and
move. That cascades instruction positions and, for the offset wrappers,
pointer-add operand allocation. O1 is closer for `15348`, but reloads the
homed halfword instead of masking the current incoming register: **1/9 words
is still NONMATCH**.

Bounded controls (at most fourteen source forms per representative hypothesis):
- O2 followed by O1 on every canonical body;
- signed/unsigned narrow source parameter, promoted caller-facing callee
  declarations, and old-style callee declarations;
- explicit narrow conversion, K&R definition and standard C89 register hint;
- full-word input with an explicit u16 conversion: removes the home store in
  the simple O2 wrapper, so it does not reproduce the target ABI;
- promoted local identifier and a call-result return form;
- byte-pointer operand commutation and resource-parameter reassignment;
- a real count local and payload-pointer advance in `15318`, improving its
  O2 baseline from 9/12 to **3/12** without extra arguments or padding.

The shared four-offset family had identical baseline residuals. Variants were
bounded on its first representative rather than blindly multiplied across
four equivalent functions. Caller-facing widened prototypes moved the first
representative to 7/11 but were not retained as the canonical archive because
they do not establish the real shared header contract. All retained sources
use consistent narrow callee declarations. No hand-written assembly, explicit
register binding, artificial padding, fake formals, volatile tricks, forced
home stores or identity-mask source tricks were used.

**Next hypothesis / maintainer request:** obtain an authentic caller-facing
prototype and original compiler recipe for the narrow-identifier wrapper
family. The source-level IDO parameter-lowering decision is the unresolved
boundary; do not mask stack operands, change targets/scorer, or synthesize
redundant stores to close it. A separate peer observed the same home-store /
in-place-mask residual in BT02, supporting a shared investigation rather
than another unbounded per-function type sweep.

## Reproduce

```
(cd asm/us/boot_tail && sha256sum -c SHA256SUMS)
python3 cloud/work/boot_tail/BT03-low-B/verify.py > /tmp/bt03b-replay.json
cmp cloud/work/boot_tail/BT03-low-B/verification.json /tmp/bt03b-replay.json
python3 cloud/work/boot_tail/BT03-low-B/test_semantics.py
python3 tools/cloud/check_submissions.py --base 301d9e75 --head HEAD
```

`IDO_DIR` may select an already installed pinned compiler. The verifier checks
its complete file-set digest and fails if either claimed matching leaf stops
matching. Host tests compile all actual sources as C89 with isolated callee
mocks. They test both return paths and all four offset slots, count/payload
forwarding including 0 and 65535, and the two leaf effects. This is behavior
support under the declared types, not native hardware or ROM proof.

Only matching sources and this packet's C/tests/evidence are changed. Native
word dumps, compiled objects, private paths, ROM data, locks, runtime images,
layout, shared headers, protected tooling and farm files are not published.
