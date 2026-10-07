# Image A statistics family: three matching candidates

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

**A:803AC330, 540 bytes, 135/135 words MATCH.**
**A:803AAF0C, 1,776 bytes, 444/444 words MATCH.**
**A:803ACF5C, 376 bytes, 94/94 words MATCH.**

All three have zero differing/extra words, unresolved symbols, unverified references
or errors. These are observed candidates, not accepted coverage. The group
contains thirteen complete genuine bodies (14,784 native bytes); only those
three are claimed. Remaining observations:

| Function | Differing words | Extra | Unverified | Data errors |
| --- | ---: | ---: | ---: | ---: |
| AA3D8 | 439/717 | 0 | 8 | 0 |
| AB5FC | 124/147 | 0 | 0 | 0 |
| AB848 | 394/416 | 0 | 0 | 0 |
| ABEC8 | 243/282 | 0 | 2 | 0 |
| AC54C | 6/170 | 0 | 0 | 0 |
| AC7F4 | 7/166 | 0 | 0 | 0 |
| ACA94 | 166/178 | 5 | 2 | 0 |
| ACD5C | 6/128 | 0 | 0 | 0 |
| AD0D4 | 145/296 | 3 | 2 | 1 |
| AD57C | 348/523 | 0 | 8 | 0 |

No member has unresolved symbols. The near panels retain frame/local-address
residuals. Larger bodies retain scheduling, control and own-data-layout gaps.

Follow-on to [draft #253](https://github.com/cabi24/SFRush2049-decomp/pull/253).
The new candidate is ACF5C only: moving its existing 48-byte text-buffer
declaration after the scalar/pointer locals changes its sole differing address
from sp+64 to native sp+48, retaining the same 112-byte frame. No buffer size,
statement, parameter or other function changes. AC330 and AAF0C retain their
previous exact scores; they are not new candidate credit in this follow-on.

## Reproduce

```
python3 cloud/work/lean/runtime_a_stats_menu_20261006/reproduce.py
```

Actual IDO 5.3 recipe: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical
`uld`/`usplit`/`umerge`/`uopt`/`ugen`/`as1`, with `as1 -r4300_mul`.
Only the genuine AD57C callback root is kept. The complete children and their
real call edges provide interprocedural context; no keeper or fake caller.
The minimal helper authenticates the frozen tracked asset/full image A in
memory and uses the unchanged canonical scorer. No bytes, dumps or objects
are published. Only compile/scoring was performed.

Standalone O3 baselines were AC330 132/135 (+1 extra) and AAF0C 431/444
(+5 extra). The first full group already matched AC330. AAF0C improved from
31/444 to exact by measuring the previous font's height before switching to
the next heading font, preserving the native external-call order.

## Required context and assumptions

AC330 formats nonzero seconds as minute/second/millisecond fields, records the
input in D_803BA90C, and copies the real placeholder string for zero. Its six
native call sites pass the destination and time through the private ABI and
write zero to outgoing word +8. The third source formal records that observed
argument; its original role is unknown and the specialized body does not read
it. This arity/order remains a source-level hypothesis. It is not fabricated
register pressure. Float-to-u32 conversions require finite nonnegative values
whose scaled results fit u32 for portable C behavior.

AAF0C temporarily selects the inspected profile and one-player mode, invokes
the genuine unlock-table refresh, then restores both globals. It renders four
unlock categories in two columns, including each empty-category message and
the all-empty footer. Signed-short loop and row behavior is retained.

The other bodies provide the complete profile picker, detail dispatch, unlock
lists, cumulative statistics, lap/time/count panels and frame drawing. The
profile view preserves next/previous/file handles, name at +20, and statistics
handle at +44. Byte-offset statistic accessors describe aligned native fields;
they do not invent a complete save-file structure. Fallback table addresses
are interior aliases and require the native selector domains (including their
subtracted record bases). Language tables, handles and selectors must be valid.

Candidate text buffers use 48 or 80 bytes and projection results use two shorts;
original capacities are unknown. Native local-address differences remain
explicit rather than filled with unused arrays, padding or volatile. The
four-byte by-value colors retain their observed word alignment. Original
names, complete record types and TU/private visibility remain hypotheses.

The independent checker owns further validation, acceptance and ROM integration.
