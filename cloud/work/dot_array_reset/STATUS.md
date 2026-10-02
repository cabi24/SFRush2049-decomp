# func_800F7EB0 natural array-reset research (NONMATCH)

This is **not a match** and adds no accepted coverage. The complete native
function is 35 instructions / 140 bytes at 0x800F7EB0–0x800F7F3C.
The natural two-loop candidate differs in **4 of 35 complete resolved words**.
It stays under `cloud/work/`, outside matching submissions and accepted sources.

## Behavior and reconstruction

The signed 16-bit count at D_8014A108 controls parallel arrays. A positive count
clears that many bytes at D_80144018 and copies the float at D_80124618 to the
same number of elements at D_80144DA8. Nonpositive counts skip these arrays.
All paths then set the three five-byte records at D_80151AC0 to signed -1.
This is a zero-argument leaf. It makes no calls and preserves the ordinary ABI.
The existing target inventory has one direct JAL caller, `display_list_flush`
(word 293); indirect callers are not excluded.

The source uses typed arrays and ordinary nested loops. Every local is a used
index. There are no extra operations, dummy locals, helpers, ABI changes,
volatile accesses, integer-pointer laundering, or generated padding. `pad` is
not used anywhere. The bound is reread in C; normal IDO alias analysis hoists it
and converts the native positive-count loop to an end-pointer comparison.
The second nested loop naturally produces the native partial unrolling.
The array extents available at runtime are not established by the target alone;
as in the native function, a positive count requires sufficient backing arrays.
No arcade source equivalence is established: its checkout is unavailable.

Historical `tiny_A47` research reported 34/35 differing words. The new simple
indexed-array source gives the same 35-word body with 31 words exact. Existing
historical sources remain unchanged. Nearby controls (loop bounds, actual
pointer loops, local types, declaration layouts, O3 singleton, and source-line
joins) did not eliminate the remaining scheduling difference. A broad line-join
probe was stopped after 1,664 layouts; a focused 128-layout first-loop sweep also
stayed nonmatching. This is a bounded research result, not a claim of exhaustive
compiler/source search.

## Exact residual

Only the invariant setup at offsets 0x24–0x30 differs:

| Offset | Target | Candidate |
|---|---|---|
| 0x24 | sll t7,v1,2 | lui at,0x8012 |
| 0x28 | lui at,0x8012 | sll t7,v1,2 |
| 0x2C | lwc1 f0,0x4618(at) | addu a1,t7,a0 |
| 0x30 | addu a1,t7,a0 | lwc1 f0,0x4618(at) |

These are the same four instructions in a different dependency-valid order.
All remaining complete words match after resolving all 12 HI16/LO16 relocation
entries. The ELF text is 144 bytes: the complete 140-byte function followed by
one zero alignment word. No extra nonzero text or unresolved relocation is
hidden by that alignment. `verification.json` records complete-body hashes and
all differing words; `verify.py` independently parses protected targets and
uses direct IDO plus GNU ld/objcopy rather than scorer relocation routines.

Rebased onto current master `a12daa63ae67c8dab3404efb666089c884dadfd4`;
its protected manifest and unchanged target were freshly verified.

## Tests and independent review

- Canonical scorer: expected **NONMATCH 4/35**, exit 1. No allow-unverified mode.
- Direct IDO and GNU linked raw comparison: identical four differences, no masks.
- Independently reviewed by a separate worker using a fresh compile/link:
  complete extent, ABI, semantics, natural source, and all 12 relocations checked.
- ASan/UBSan host semantics: **33,693 cases passed**. All 32,769 nonpositive
  signed-half values; positive counts 1–128, 255, 256, 1024 and 32767; seven float
  patterns including positive/negative zero, infinities and quiet NaN; all 15
  signed-byte reset locations; whole-array sentinels outside the written extent.
- C89 pedantic warnings pass. Host allocations/mocks are not matching code.
- Repository CI-style suite: **1,307 passed, 41 skipped, 9 deselected**.
- Scorer suite: **571 passed**. Cloud suite also exits zero with documented skips.
- All **161 static locks** and **664 blob/group source hashes** remain intact.
- All **23 protected target manifest entries** independently hash-verified.

Run the expected nonmatch proof with IDO configured and GNU MIPS binutils on PATH:

```sh
python3 cloud/work/dot_array_reset/verify.py
```

Run host semantics:

```sh
cc -std=c89 -pedantic -Wall -Wextra -Werror -O2 -fsanitize=address,undefined cloud/work/dot_array_reset/host_semantics.c -o /tmp/rush-array-reset-test
ASAN_OPTIONS=detect_leaks=0 /tmp/rush-array-reset-test
```

The proof succeeds only when it reproduces the documented NONMATCH; it does not
replace or relax the matching scorer. Leak detection is disabled under sandbox
ptrace; the test allocates no heap. No accepted C, lock, protected target,
scorer, generated context, build wiring or coverage metric was edited.

No extracted-image or full-ROM gate ran. This candidate must not be spliced,
locked or promoted without first obtaining an honest strict match and then
passing normal integration gates. Draft research only; no merge requested.
