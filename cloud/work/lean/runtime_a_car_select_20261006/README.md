# Image A car-selection caller family: lean research

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

Three complete new source bodies cover 13,780 native bytes:

| Function | Native bytes | Differing words | Unverified references |
| --- | ---: | ---: | ---: |
| A:803A0498 | 2,880 | **154/720** | 0 |
| A:803A0FD8 | 3,796 | 874/949 | 14 |
| A:803A1EAC | 7,104 | 1642/1776 | 12 |

All three have zero unresolved symbols, data errors and extra words. A0498's
standalone O3 baseline was 705/720 differences plus thirteen extra words. Its
two genuine callers restore the native interprocedural register-save context.
The other two bodies retain broad scheduling/control and own-data-layout gaps.
Empty claims: observed improvement is not accepted coverage.

## Reproduce

```
python3 cloud/work/lean/runtime_a_car_select_20261006/reproduce.py
```

Actual IDO 5.3 recipe: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical
`uld`/`usplit`/`umerge`/`uopt`/`ugen`/`as1`, with `as1 -r4300_mul`.
Only the real A1EAC entry is kept. The helper authenticates the frozen asset and
full image A in memory before using the unchanged canonical scorer. Six setup
literals, the native 19-way option mapping and adjustment literals were decoded
from that image; the nonzero unverified results above are retained. No bytes,
dumps or objects are included. Only compilation/scoring was performed.

The existing `cloud/work/runtime_a_float_product/candidate.c` supplies D300 as
an explicitly pre-existing semantic context body, compiled here at actual O3.
Its known two genuine callers are now present. Its comparison remains 101/101
differing words in this context; no new reconstruction or matching claim is
made for that 404-byte body. The older standalone preservation warning remains
in force. No context body is proposed as an unverified native replacement.

## Source and remaining context

A0498 rebuilds selected ghost-record handles from linked and fixed arrays. It
preserves matching-track/mode/profile filtering, the real eligibility calls,
three-entry ordered insertion, the five-entry neighborhood around the owner's
best record, list compaction and limited-mode resource release. The five local
handles are real stored selection data, not register-pressure filler.

A0FD8 initializes the selection scene, per-player car/part choices, projected
models, viewport settings and display state. A1EAC refreshes record validity,
handles controller/cancel/ready paths, wraps the nineteen real option cases,
writes selected save values, lays out ready/ghost frames and performs the
observed final transition. The repeated native compaction pass and asymmetric
scale thresholds are retained rather than normalized away.

Low-image D05C, D6A4, E200, E3BC, EF08 and FF04 remain external services. Natural
arguments are inferred from their native consumed inputs (including D6A4/FF04
signed-byte player and E200 record-handle/direction); their complete private
callee visibility is still missing. They are not replaced by invented bodies
or padded formal lists. This missing context is a concrete blocker to treating
the broad caller objects as ABI-correct native replacements.

The source uses observed 76-byte player, 44-byte record-prefix, 64-byte animated
slot, 152-byte camera and 12-byte car-position views. Record names start at +10;
profile names at +20. The original complete types and names are unknown. Valid
acyclic lists, live handles, bounded player/selector indices, stable backing
for the product vectors, terminated names and finite representable conversion
inputs are required. Availability loops assume a reachable eligible choice.
Local buffers store actual projections and vectors; no assembly, volatile,
unused pressure expressions or artificial retention barriers are present.

Independent checker owns further validation, acceptance and ROM integration.
