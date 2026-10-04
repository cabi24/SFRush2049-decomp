# BT03 low dispatch: five first-try strict matches

Exclusive central claim `387bab9e` covers five fresh functions / **464 B** on
`dot/boot-tail-bt03-low-dispatch`, based on master
`301d9e7552ad4fd7f54a38796db84671e1000d35`. All five whole bodies strictly match
on their first O2 compile. No source variant, artificial local or flag sweep was
needed. O1 controls fail every member.

| Function | Bytes | Helper | Native traversal | O1 residual |
|---|---:|---|---|---|
| 800150C8 | 88 | 80016998 | forward | 22/22 |
| 80015120 | 88 | 80015F28 | forward | 22/22 |
| 80015178 | 88 | 800158D8 | forward | 22/22 |
| 800151D0 | 88 | 80015C0C | forward | 22/22 |
| 800152A8 | 112 | 800163A8 | reverse | 27/28 + 8 nonzero excess |

Every matching source is self-contained C89 with the exact line-one flags
`-g0 -O2 -mips2 -G 0 -non_shared`; the scorer adds `-Wab,-r4300_mul`.
The four forward bodies reproduce a 40-byte frame, three real callee-saved
values (identifier, cursor and sentinel), branch-likely empty-list exit and
post-call halfword reload. The reverse body reproduces its 32-byte frame,
two real pointer values, forward sentinel scan and backward callback loop.
These native signatures support O2 independently of a guessed function name.

`verification.json` binds all ten O2/O1 compiler comparisons to exact source,
target, toolchain and scorer hashes. All five O2 results have complete relocated
word equality and zero nonzero excess, unresolved symbols, unverified references,
masked relocations or relocation errors. Each 88-byte body has 96-byte aligned
object text with two ordinary zero padding words; the 112-byte body has exactly
112 bytes. The target spans are unchanged and no padding receives body credit.

## Genuine caller and helper contracts

Each wrapper takes one pointer to a sequence of unsigned 16-bit identifiers,
terminated by `0xFFFF`. Each actual helper was inspected across its complete
canonical extent before candidates were written. They consume one incoming u16
identifier; no other incoming argument register or stack formal is required.
The first four helpers home a0 then load its low halfword, while `163A8` also
normalizes it directly. Native integer return statuses are zero (always for
`16998`) or zero/one; these wrappers ignore all return values. The `int`
declarations describe the observed word-return ABI, without claiming the
unrecoverable original status typedef or signedness. Callee bodies remain
read-only and are only declared, never stubbed into candidates.

The sole recorded caller `154A4` computes every list pointer from a resource
base, a relative offset, and an additional eight-byte prefix. It dispatches:

- resource offset word +8 to `150C8`;
- offset word +12 to reverse walker `152A8`;
- offset words +16, +20 and +24 to `15120`, `15178` and `151D0`.

`abi.json` records exact canonical caller/callee hashes and these observed
contracts. No resource bytes, table contents or invented helper implementations
are introduced. Names remain descriptive; no external source identity is claimed.

The forward bodies keep a genuine loaded identifier and reload the next entry
only after the helper returns. They preserve live list mutation behavior rather
than caching the entire sequence. The reverse body first scans to the terminator,
then decrements the end pointer and dispatches each preceding element. It does
not rescan the terminator after callbacks or stop at a subsequently changed key.

### Reverse-pointer domain

The native reverse loop forms `start - 2` even for an empty list and after the
last callback, but never dereferences that pointer. Its natural C reconstruction
preserves this exact pre-decrement structure. Tests pass a list interior to one
u16 array with four preceding elements, corresponding to the caller's observed
eight-byte prefix; the predecessor and both relational operands therefore remain
within that same allocation/array. No fake prefix or padding is inserted into the
function or its ABI. This is a bounded valid-input model, not a claim that an
arbitrary standalone array beginning at the list pointer has portable predecessor
semantics. Invalid offsets, missing terminators and unrelated allocations are
outside the established resource contract.

## Verification and limits

Fresh `preflight.py` runs setup, verifies all three target-manifest entries,
compares all 439 starts/sizes (99,120 B), and strictly replays the existing 12-byte
getter before candidates. `preflight.json` records the pinned tool and input hashes.
No diagnosis/variant report is necessary because every initial O2 body matched.
The O1 controls are retained in the compiler receipt rather than discarded.

Six tests compile the five actual matching C files as C89 with pedantic errors,
all warnings treated as errors, AddressSanitizer and UndefinedBehaviorSanitizer.
They perform **336,775 wrapper invocations**: 67,355 per function, including all
65,535 non-sentinel u16 identifiers and a 1,820-case length/seed/live-mutation
matrix. Tests verify callback order, empty and 64-element lists, next-ID changes,
new sentinels, forward extension, reverse fixed scan boundaries, ignored helper
status and all surrounding memory bytes. Test helpers implement explicit synthetic
contracts only; they do not reconstruct the real callees. LeakSanitizer alone is
disabled because this executor's ptrace prevents it; no harness or candidate heap
allocation occurs. Exact scope and source hashes are in `host_verification.json`.

```sh
python3 cloud/work/boot_tail/BT03-low-dispatch/verify.py --check
python3 -m unittest discover -s cloud/work/boot_tail/BT03-low-dispatch -p 'test_*.py' -v
python3 tools/cloud/check_submissions.py --base 301d9e75 --head HEAD
```

Only the five named matching files and this owned packet folder change. Prior
packets and central STATUS/D10 remain untouched. No target/scorer, shared-header,
symbol/layout/lock, runtime-image/farm, production gate or unrelated helper edits;
no ROM bytes, native dumps, compiled objects or private data are committed.
The 631 existing cloud regression tests pass with zero skips.
Independent central review passed exact source commit
`03a5435bdf631477b6a64071d02efe56b57dad0b`, tree
`944c58b8c47ae1ba520b0ed8040354ebdc478c08`; `REVIEW.json` binds its source hashes.
The reviewer independently reproduced all ten flag rows, six sanitizer tests and
the canonical five-submission gate, and inspected all five complete native
callees plus caller `154A4`. The reverse interior-allocation qualification remains
explicit. Aggregate publication and exact-head CI remain lead-owned; local matches are
not counted as cartridge coverage or maintainer acceptance. Merging stays with
the owner's independent checker.
