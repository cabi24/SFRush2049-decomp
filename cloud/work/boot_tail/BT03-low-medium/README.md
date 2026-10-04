# BT03 low medium: queue tokens, slot state and address conversion

Five standalone strict local matching bodies, **360 B**, and one complete
92-byte nonmatch. These are matching-source results, not cartridge coverage.
Independent review and exact aggregate-head CI are pending at source freeze.

Base: `301d9e7552ad4fd7f54a38796db84671e1000d35`.
Branch: `dot/boot-tail-bt03-low-medium`.
Owner: `/root/match_boot_tail_low_medium`.
Central exclusive six-target claim: `8b45bb52`, acknowledged before source edits.
The coordinator owns central STATUS, README and D10; this packet returns only
its `status_delta.csv`. Reference caller/callee sources were read from the
coordinator's aggregate and were not edited. No third-party source was copied.
Behavioral names below are native-backed descriptions, not recovered API names.

| Address | Full bytes | Native-backed behavior | O2 | O1 |
|---|---:|---|---:|---:|
| 80014550 | 68 | initialize one-slot queue and insert its token at the front | MATCH | MATCH |
| 80014594 | 72 | receive token at nesting zero, then increment nesting count | MATCH | 15/18 + 1 excess |
| 800145DC | 72 | decrement a positive count and return token on transition to zero | MATCH | 18/18 |
| 8001489C | 92 | set release count to 20; optionally enter release state | 11/23 | 22/23 + 8 excess |
| 80014AF0 | 76 | clear an active slot and mark its release pending | MATCH | 19/19 + 10 excess |
| 80014CAC | 72 | convert addresses except top-byte 0x80 and 0xB0 | MATCH | 17/18 + 4 excess |

## ABI and storage audit

- `14550` has no input uses. Callers `143C0` and `14434` call it without
  arranging arguments and ignore its result. Counted-static `80006A00` is
  `osCreateMesgQueue`; `800075E0` is **osJamMesg**, not osSendMesg. The queue is
  at `D_80038368`, the single message storage word at `D_80038380`, and the
  initial null token is nonblocking. An opaque queue declaration avoids
  inventing its internals.
- `14594` and `145DC` consume no arguments and do not define an API return.
  Numerous native callers, including `10628`, bracket their state updates
  with these helpers and ignore v0. The counter `D_8002C5DC` is tested with
  signed `> 0` on release; acquisition calls `osRecvMesg(queue, NULL, 1)`
  only when zero. The acquisition reload after the call is significant and
  retained. The source assumes reachable nesting counts do not overflow
  signed int; no behavior for signed overflow is claimed.
- `1489C` receives a full-word index and a byte flag. The native second input
  is homed, then masked to eight bits. Its caller `22A40` passes an index
  narrowed to eight bits and flag one. The record stride is 104, release
  count is halfword +40, and the callee `11A3C` consumes one record pointer,
  copies release count to +72, and enters state three. That callee's other
  caller `14B3C` does not establish a meaningful second input. No second
  callee formal/actual was invented to influence allocation.
- `14AF0` takes one full-word index; native callers `1F898`, `1F954` and
  `23E9C` ignore its result. It changes only byte +0 and byte +97 when
  active is nonzero, and leaves the whole record untouched otherwise.
- Both slot declarations retain the already matched prefix at bytes +0,
  +1 and word +20, extending only the partial description with halfword
  +40 and byte +97. Unknown storage arrays represent actual unrecovered
  record bytes, not stack padding. A 32-bit C89 compile verifies offsets
  and the 104-byte size; shared headers remain unchanged.
- `14CAC` takes and returns one pointer-sized value. `15228` forwards its
  original second input and then passes the converted result as the data
  argument to `1605C`. Its indirect converter `D_80038014` takes one address
  and returns one address; the independently inspected `25AB4` also calls
  this hook with one pointer. This wrapper compares the top **byte** against
  0x80/0xB0, not just the top nibble. On the direct path the original pointer
  is returned; all other values are passed once to the converter.

## Optimization evidence and complete nonmatch

All retained source starts with exact `-g0 -O2 -mips2 -G 0 -non_shared` flags;
the unchanged scorer adds `-Wab,-r4300_mul`. O2 matches the queue-counter CSE,
argument setup, frameless slot operation and converter branch layout. Only
`14550` is indistinguishable at O1. O1 is recorded for every exact final source.

`1489C` remains **COMPLETE-NONMATCH** under this packet directory and is never
submitted under matching sources. It has the full 23 native words, no excess
at O2, and no unresolved/unverified relocation or rodata dependency. Native
code retains the narrowed flag in a1; the ordinary candidate uses t6, shifting
subsequent temporary allocation and argument-home/ra scheduling.

Workbench diagnosis ran before refinement and reported a mixed structure,
schedule and allocation residual, same 24-byte frame and same instruction
count. Its native-object relocation warnings are expected for the temporary
word-only target; strict `score.py` resolves every real relocation. The
machine-readable summary is in `diagnosis.json`.

Nine natural source forms were bounded: the canonical body, local condition,
register-qualified byte formal, early return, unprototyped callee declaration,
unsigned index, duplicated branch-local store, slot-pointer local and explicit
byte-offset addressing. Six alternatives stayed at 11/23; the split-store
and pointer-local forms worsened. No artificial variable, padding, assembly,
extra formal, dummy call or callee body was added. The canonical source is
retained. Further work should require authentic reference/source evidence for
this flag/callee declaration or a measured compiler-allocation explanation;
a blind retry of the same variants is not useful.

## Verification

`verification.json` binds each final source and object hash to both flag levels,
full native extent/hash, compiler pin, scorer and protected-target manifest.
All five matches are full relocated equality with zero nonzero excess,
zero masks and no unresolved, unverified or errored relocations; each object
has only its intended function symbol. Preflight rechecks 439 starts/sizes,
99,120 B total, the exact approved IDO pin and the existing `80010A00` getter.
No targets, scorer, inventory, symbols, locks or layout were changed.

`test_semantics.py` includes each actual source unchanged in a separate host
harness. It checks queue addresses, arguments and ordering; negative/zero/
positive nesting states; a mocked receive that changes the counter, proving
the post-call reload; all 256 active/flag values at three slot indices with
whole-array preservation checks; and both boundary values and adjacent
regions for the converter. The tests pass strict host C89 O2 and ASan/UBSan
O1. LeakSanitizer is disabled because the execution environment uses ptrace;
no leak check is claimed. Test-only mocks are not matching-source context.
Host pointer tests use nondereferenced 32-bit numeric addresses; native ABI
and exact-byte proof come from IDO, not from the host pointer width.

Reproduce from repository root with `IDO_DIR` pointing at the approved toolchain:

```sh
(cd asm/us/boot_tail && sha256sum -c SHA256SUMS)
python3 cloud/work/boot_tail/BT03-low-medium/verify.py
python3 cloud/work/boot_tail/BT03-low-medium/test_semantics.py
python3 tools/cloud/check_submissions.py --base 301d9e75 --head HEAD
```

Raw target words, disassembly, objects and compiler intermediates stayed in
private temporary storage. No ROM bytes or unrelated data are published.
Production splice, cartridge integration, ROM gates and merging are maintainer-owned.
