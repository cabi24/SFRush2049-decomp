# Image A waiting screen: matching candidate

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

Five complete new bodies cover 10,344 native bytes:

| Function | Native bytes | Differing words | Extra words | Unverified references | Data errors |
| --- | ---: | ---: | ---: | ---: | ---: |
| A:803A7B58 | 3,196 | 710/799 | 36 | 8 | 3 |
| A:803A87D4 | 920 | **0/230** | 0 | 0 | 0 |
| A:803A8B74 | 1,300 | **56/325** | 0 | 0 | 0 |
| A:803A9088 | 3,668 | 562/917 | 0 | 8 | 2 |
| A:803A9EEC | 1,260 | 281/315 | 0 | 2 | 1 |

A87D4 is a **920-byte matching candidate, 230/230 words exact**, with zero
uncertainty, unresolved symbols, extra words or errors. Follow-on to
[draft #241](https://github.com/cabi24/SFRush2049-decomp/pull/241): reordering
its existing two-short projection local fixes the three stack offsets;
regrouping the genuine right edge as `(x+4)+width/2` fixes the commuted add.
No capacities, formals or statement order change. Other bodies remain context.
All members have zero unresolved symbols. Its standalone
O3 baseline was 227/230 differences plus four extra words. The keyboard child's
standalone baseline was 307/325 plus five extra words; moving its genuine row
and character limits next to their loop reduced the first group attempt from
296/325 (+1) to 56/325. Replacing an out-of-line `abs` declaration with the
observed integer absolute-value expression also removed four unresolved calls
across the two large renderers. Those bodies still have substantial control,
allocation, local-frame and own-data-layout differences. Only A87D4 is claimed.

## Reproduce

```
python3 cloud/work/lean/runtime_a_profile_menu_20261006/reproduce.py
```

Actual recipe: IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`, canonical
`uld`/`usplit`/`umerge`/`uopt`/`ugen`/`as1`, plus `as1 -r4300_mul`.
Only the actual A9EEC callback root is kept. All four real renderer children
are complete, and the three pre-existing low-A eligibility bodies are genuine
callee context. Original whole-TU/private visibility remains a hypothesis.

The existing low helpers' historical source seeds contain dummy argument slots.
This packet uses their actual semantics with natural `(owner, player)` and
`(player)` arguments and lets interprocedural compilation choose the ABI. No
invented unused formals, keeper functions or retention barriers are added. In
this context A95E24 differs at 17/26; A96EE8 and A96F44 are reduced to small
out-of-line remnants while their logic is incorporated into their caller.
They are context only and are neither new reconstruction nor matching claims.

The helper authenticates the frozen asset and full image A in memory and uses
the unchanged canonical scorer. The three native angle literals are
1.5707963705062866, 2.4434609413146973 and 0.6981316804885864. Both six-way
status mappings and the root's seven-way mapping were decoded from that image.
The nonzero own-data results above are retained; instruction-layout shifts mean
these references are not established matches. No raw bytes, assembly dumps or
objects are included. Only compilation/scoring was performed.

## Source and assumptions

The family renders per-player profile browsing, another-player waiting,
character entry, deletion/confirmation and two status messages. It preserves
both forward/backward profile-handle walks, fallback head/tail traversal,
controller filtering, selected/unavailable colors, projection/angle clipping,
name clipping, and the original repeated font-height calls.

Observed views are 64-byte animated slots, 22 slots per player, 152-byte camera
records, 76-byte player records with owner handle at +72, and 24-byte layout
records. Profile handles point to records with next/previous handles at +0/+4,
file handle at +8 and name at +20. The one-byte trailing name view denotes an
opaque variable-size payload; it does not claim the original allocation size.
The two by-value four-byte colors are word aligned as observed at their callers.

Original buffer capacities and complete source types are unknown. Candidates
use 40 bytes for the browsing title, 128 for deletion text, 64 per root status
message, two bytes for a single keyboard character plus NUL, and two shorts for
each projection result. Native stack-offset residuals are not filled with
invented padding. Valid language indices, 1..4 players, finite projection
values, acyclic consistent profile lists and strings fitting these buffers are
required. The absolute-value expression requires representable differences.
The callback context is unused and its original full type is unknown.

Observed improvement is not accepted coverage. Independent checker owns further
validation, acceptance and integration, including the unresolved ABI/data context.
