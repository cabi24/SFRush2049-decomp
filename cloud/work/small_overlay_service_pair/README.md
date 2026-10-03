# Small-image services: full local behavior, bounded contract

Research only: **NONMATCH**, zero accepted bytes. This packet reconstructs every
local branch of the 244-byte service at `8038D3A4` and 736-byte service at
`8038D798` in the **B6FEC4** runtime image. It is not an original-TU recovery,
production replacement, global ABI/header update, compiler match, or universal
residency proof. The source preserves genuine external calls; test doubles live
outside the source corpus and are never proposed as native helper replacements.

Baseline: master `f82c5204edc4ead0f9056f323ff71122209ff259`. Current open work was
checked: PR52 owns V01/V02 and stream census; this packet does not alter that
work, CI, locks, shared headers, acceptance code or the owner-paused target.
The arcade source submodule is absent here; names such as `remaining` are
provisional field labels, not claims about arcade ancestry or gameplay design.

## What C974 can now rely on

`small_8038D798(float *origin, float *endpoint, int source_index,
float radius_squared, int magnitude)` expresses the consumed O32 arguments.
The first two are separate three-float input pointers. `a3` is transferred
bitwise to a float register and used in floating arithmetic. Thus C974's
`0x43C80000` means **400.0f**, a squared-distance threshold; the fifth stack word
**1600 is an integer**, converted to binary32 inside the service. The source
index selects a player record, not a car ID. C974 passes the same pointer for
both vector inputs, which is allowed. The second pointer is only read when an
eligible target has signed byte `+384 == 5`.

`small_8038D3A4(SmallPlayer *source, SmallPlayer *target, int amount)` consumes two
record pointers and a signed integer. `void` here records the absence of a
required result for these callers; it does not prove the original return type
or all-caller ABI. No meaningful single native v0/f0 return invariant was found.
Normal O32 callee-save rules apply; C974 must continue saving its live caller-save
temporaries. No fabricated parameter, pressure variable, keeper caller, or
incoming float-register dependency was added.

The image identity/signature obstacle is resolved for these bodies, but **full
C974 execution/matching is still blocked** by callback lifetime/residency and
its remaining genuine source/context work. No shared declaration or old seed is
silently repaired based on this research alone.

## Exact local behavior

D3A4 reads target signed car index at `+35B`, then model byte `+640` (model
stride2056). A nonzero byte skips everything. Equal source/target car IDs or
equal signed team bytes at `8012E67C` also skip. Target word `+38C & 4` scales
amount by binary32 `0.2f`, then truncates toward zero. This is not interchangeable
with integer division by five.

The old signed halfword `+386` is loaded; low16(amount) is stored to `+388`;
low16(old-amount) is stored to `+386` and reread signed. Only then is nonpositive
remaining clamped to zero. This wrap-before-test is crucial: old=1, amount32770
produces positive32767 and no callback. On depletion it calls
`800C55E4(source ID reread, target ID reread, 1)`, then rereads target ID again
and sets the corresponding model byte. A zero amount can trigger depletion of
already-nonpositive remaining. Negative amounts can increase it or wrap.

D798 iterates 952-byte players, rereading signed16 count `801543CA` each loop.
Eligibility order is model byte zero, player signed byte `+308` nonzero, player
signed byte `+359` zero. Difference components use player `+8/+C/+10`.
Squared distance is `dz*dz + (dx*dx + dy*dy)`, with each operation binary32.
Strict distance<threshold gates weighting. Weight is `(threshold-distance) /
threshold`; amount is `(weight*weight) * float(magnitude)` then truncation.
Kind5 additionally transforms endpoint-origin via `800A61B0` with matrix
pointer target+2C, calls `8008C768(output[0],output[2])`, and multiplies amount
by `0.35f` iff absolute angle<`1.35f`. Equality is unattenuated. Constants' native
pool bit patterns are authenticated by `audit.py`.

No persistent store is directly performed by D798: effects come through D3A4
and its real callback. Input vectors may alias each other or player storage;
there is no restrict assertion or hoisting across effectful calls in the source.

## Immediate helper and effect closure

- `800A61B0`: leaf 3x3 row transform, result[row] = z*m2+(x*m0+y*m1), with
  per-row rereads. D798's stack input/output are separate. Existing accepted
  source evidence is not renewed or modified here.
- `8008C768`: custom atan2-like approximation with special condition
  x==float(x+z), plus helpers `8008C680`, `8008C720`, `8008C5E0` and native pools.
  It is retained as an external, never replaced by host atan2f. Mock angles only
  test this caller's dispatch, not that helper's numerical behavior.
- `800C55E4`: sign-extends the low8 of its three arguments; mode word `8014A110`
  in {4,6} dispatches **another small-image service, `803914B4`**. On the +1 path
  used here it appends an event in the 24-byte ring at `80395E70`, advances signed
  byte index `80395ED0` with wrap at4, and writes IDs/flags/four converted table
  coordinates indexed by signed halfword `80151AD0`. The negative-path helper
  call is unreachable for +1. This bounded path does not modify player IDs or
  count. Valid ring/table indices and residency remain external invariants.

## Model assumptions and limits

- Valid allocated player/model/team storage and indices; native code does not
  validate them. Count <=0 skips on the actual native base; the indexed C loop
  is equivalent on valid storage, not arbitrary wrapping pointer inputs.
- Signed char/short/int are 8/16/32 bits, float is binary32. Signed-halfword
  narrowing uses the verified IDO/host low16 interpretation (implementation-
  defined in abstract C89). Unsigned subtraction avoids C signed-overflow UB.
- Float->int results must be finite/in-range. Nontrapping NaN comparisons that
  skip or choose the unattenuated path are tested; out-of-range conversion,
  hardware denormals, trap/rounding-control changes and FPU-status equality are
  not proved. Conditional-negate angle abs gives the same numeric branch but
  does not claim native sign clearing for -0/NaN or identical exception state.
- Every binary32 operation must keep its stated order. Disable contraction,
  reassociation and fast-math for host tests. No libm oracle equivalence claim.
- Actual count address aliases **player7+0x1AA**, inside opaque padding. The
  symbols in the header are native address views, not independently allocated
  target storage. Host fixtures allocate them separately; opaque-padding writes
  and arbitrary partially overlapping typed records are not covered.
- Helpers, original TU/static visibility, all callers and complete runtime
  ownership are not reconstructed. Synthetic mutation callbacks exercise
  reload/order guarantees; they are deliberately stronger than this particular
  real +1 callback, not claims that it mutates IDs/count in gameplay.

## Residency result

See [RESIDENCY.md](RESIDENCY.md). Both images load at8038A400, so address range
containment is insufficient. C974 is record120's update callback: table+0C is
copied to node+14 and dispatched indirectly at800B08A4, with no local loaded-flag
guard. State transition ordering supplies a bounded load-before-dispatch route;
record lifetime, intervening callbacks and every release/overwrite remain open.
Mode6 alone is not a universal small-image ownership proof. `residency_model.py`
is only a transparent abstract transition model, not native emulation or a proof
of reachable gameplay states.

## Reproduce and evidence levels

From repository root:

```
python3 cloud/work/small_overlay_service_pair/audit.py
python3 cloud/work/small_overlay_service_pair/residency_model.py
python3 -m pytest -q tests/conveyor/test_small_overlay_service_pair.py
```

The audit authenticates the tracked asset SHA, exact raw-DEFLATE stream length,
image SHA, local body hashes, pool words, direct calls, return boundaries and
in-body branch targets. It is not a full overlay function/export registry or a
MIPS interpreter. It prints metadata only. Never commit its binary input,
extracted image, native disassembly, compiler objects or private build outputs.

The C89 host tests cover layout, ordered gates, modular halfwords, depletion,
callback sequencing, distance boundary/weighting, kind5 helper ABI, signed
angle threshold, NaN comparisons, zero/negative threshold, dynamic count in
both directions and player-memory-aliased live vector rereads. Independently
written stepwise binary32 oracles check 30,000 randomized D3A4 fixtures plus
three edges and 3,000 D798 fixture sets. These are bounded host behavior checks,
not native differential execution or game integration.

A single pinned IDO compile with `-g0 -O2 -mips2 -G 0 -non_shared
-Wab,-r4300_mul` succeeds. Candidate sizes are **248/604 bytes** versus native
**244/736 bytes**, respectively. No native scheduling sweep was attempted.
Unresolved target relocation/placement and strict differing-word count are
**not measured**, not zero. No true scorer, overlay link/registry integration,
image/compression/full-ROM gate or new accepted source coverage was run.
`evidence.json` binds source, metadata, test and compile evidence; review is in
[REVIEW.md](REVIEW.md).

Stop/reopen: freeze matching until authentic original visibility/TU and runtime
image ownership are bound. Next useful work is record120 callback lifetime
versus loader transitions, followed by full C974 reconstruction with these
consumed arguments and exact existing helper bodies. A future matching attempt
must regenerate proper image-keyed targets rather than treating this runtime
address as an ordinary game-blob symbol.
