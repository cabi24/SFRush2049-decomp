# BT03-low-C: packed-key comparators and small ABI helpers

Base `301d9e7552ad4fd7f54a38796db84671e1000d35`, branch
`dot/boot-tail-p3-bt03-low-c`. Central active claim acknowledged at `5b44ebd3`.
This is the final seven open under-64-byte starts in the assigned BT03-low
interval. Earlier A/B packets remain immutable, independently reviewed inputs;
this branch neither edits nor re-credits them. Shared STATUS/D10 editing belongs
to the coordinator.

## Outcome

**Six local strict matches, 200 B**, and one **COMPLETE-NONMATCH, 48 B**.
Every matching source matched its first natural C form. All six also match
at O1; O2 is the selected standard recipe, not a uniquely recovered original
optimization level. `verification.json` binds both levels to actual source
hashes, unmodified scorer/manifest hashes and the complete compiler-pin digest.
No cartridge coverage, promotion or merge is claimed. Independent review and
exact aggregate-head CI remain required for final VERIFIED-BODY credit.

| Function | Bytes | Behavior | O2 | O1 |
|---|---:|---|---|---|
| 80015574 | 24 | home four unused incoming words; return zero | MATCH | MATCH |
| 800156E8 | 48 | forward two u16 IDs and two opaque words with fifth flag zero | 8/12 + 1 excess | 8/12 + 3 excess |
| 80016BF8 | 40 | compare unaligned unsigned halfword keys at +4 | MATCH | MATCH |
| 80016CE0 | 16 | compare aligned unsigned halfword keys at +0 | MATCH | MATCH |
| 80016E40 | 40 | compare unaligned unsigned halfword keys at +4 | MATCH | MATCH |
| 80016F58 | 40 | compare unaligned unsigned halfword keys at +4 | MATCH | MATCH |
| 80017018 | 40 | compare unaligned unsigned halfword keys at +0 | MATCH | MATCH |

All chosen matches have zero differing/excess words and no unresolved,
unverified or errored relocations. No target extent was split or merged;
the identical comparator bodies remain distinct native functions.

## ABI and layout evidence

- `15574` natively homes a0/a1/a2/a3 to the four incoming argument slots
  before returning zero. Four unused word formals are therefore directly
  evidenced, not invented register keepers. There are no known direct callers.
  Their historical names, pointer-versus-integer roles and signedness are
  unresolved; the source makes only an opaque word/constant-result claim.
- The five comparator bodies consume exactly two incoming record pointers,
  make no calls, read two unsigned halfword values and return their signed
  integer difference. Integer promotion safely represents the full
  -65535 through 65535 result range. None writes the records.
- `16BF8`, `16E40`, `16F58`, and `17018` use byte-based unaligned halfword
  loads in the native code. Ordinary C89 implementation-defined
  `#pragma pack(1)` partial-record views reproduce these loads exactly with
  IDO 5.3; packing is restored after each declaration. It describes real
  one-byte alignment rather than inserting source or stack padding. The
  three +4 views have an opaque four-byte prefix. The +0 view has a separate
  `PackedLeadingKey` type. No full record allocation size is asserted.
- `16CE0` instead uses aligned halfword loads and is expressed through
  `const unsigned short *` inputs. Applying a packed type there would be a
  different ABI/storage assumption and is not done.
- `156E8` has a normal 32-byte call frame, homes its first two arguments,
  narrows both IDs and explicitly passes zero as the fifth stack argument
  to `1558C`. The latter was read through its complete body: it consumes
  both IDs, forwards the third and fourth opaque words to `178B0`, and
  reads the fifth argument as a byte flag. This directly supports four
  wrapper formals and the added zero flag, not a guessed stale register.
  Caller `19490` prepares IDs from two halfwords and consumes the returned
  integer/negative-one status. The fourth forwarded word's semantic role
  remains unknown; no stronger context/option API name is asserted.

## Bounded nonmatch: 800156E8

The whole body is retained here, never under matching submissions.
Canonical O2 yields **8/12 differing words, 1 nonzero excess word**;
O1 yields **8/12, 3 excess**. All relocations resolve strictly.
Workbench diagnosis at both levels reports a structural argument-lowering
mismatch with the correct 32-byte frame: the two source narrow formals go
through extra temporaries/moves, where the native narrows a1 then a0 in place.
The native temporary diagnostic object has no symbolic relocations, so
workbench relocation-site cautions are interpreted separately from the
strict relocated scorer result.

This is the same narrow-identifier lowering hypothesis already bounded in
BT03-low-B and independently observed in BT02. Its history was not reset:
only the natural four-formal body and fixed O2/O1 controls were tested here.
No forced home stores, identity-mask tricks, padding, fake formals, inline
assembly or flag sweep was introduced. The next useful step is an authentic
shared declaration/compiler-recipe explanation for the narrow-formal family,
not another independent spelling search for this wrapper.

## Reproduce and test limits

```
(cd asm/us/boot_tail && sha256sum -c SHA256SUMS)
python3 cloud/work/boot_tail/BT03-low-C/verify.py > /tmp/bt03c-replay.json
cmp cloud/work/boot_tail/BT03-low-C/verification.json /tmp/bt03c-replay.json
python3 cloud/work/boot_tail/BT03-low-C/test_semantics.py
python3 tools/cloud/check_submissions.py --base 301d9e75 --head HEAD
```

`IDO_DIR` may select an existing pinned compiler. The verifier checks its
complete file-set digest, the 439-function/99,120-byte census against extents,
and the original getter, in addition to all seven actual sources. It fails
if a matching source stops matching or the archived O2 residual changes.

Host C89 tests cover the constant-return helper and all wrapper forwarded
values/statuses. Each packed comparator is exercised in a naturally
one-byte-offset record inside a double-aligned union, with 25 pairs spanning
0, 1, 32767, 32768 and 65535. They verify unsigned interpretation, equality,
ordering, field offsets and deliberately unaligned storage. The aligned
comparator covers both extreme differences and equality. These tests support
C behavior under the declared layouts; native word equality separately proves
N64 layout/code emission. They do not prove hardware, concurrency or ROM identity.

Only the six matching C files and this packet's source/tests/evidence are
published. Targets, shared headers, scorers, locks, layout, runtime-image work
and the farm are unchanged. No ROM/image bytes, raw native dumps, objects,
credentials or private data are included.
