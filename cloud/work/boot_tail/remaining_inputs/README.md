# Remaining input closure after cut14

The source-free snapshot reconciles all 412 assigned functions / 95,012 bytes:
390 published attempts / 69,596 B, the historical getter / 12 B, four later-owned
bodies / 2,908 B, and 17 still-unclaimed bodies / 22,496 B. Scouts alone add no
attempts or matching credit. Later results supersede this dated snapshot.

Eight unclaimed rows have recorded boundary or decoder-dependency stops. Eight
are at least 1 KB and remain behind spec task T050; three rows belong to both
sets. Three additional sub-1KB bodies require unproved case mappings. The last
row is a handwritten assembly-family lead. These are proof gaps or scope gates,
not permission to restart exhausted code-generation searches.

## Case-table gaps

`8001B9F8` and `8001BE14` dispatch selectors 250..255 through six-entry tables at
`8002D8D0` and `8002D8E8`. Full native body inspection establishes the indexed
loads and transfers, but does not establish which target belongs to each selector.
The canonical extent metadata has a switch-table count, not per-table entries;
neither function record nor symbols supplies the mappings. Family-source case
order is not substituted for original target data. `80023BDC` has the previously
recorded five-entry gap, and the larger `80023E9C` is also table-dependent and gated.
Maintainer-supplied, independently authenticated mapping metadata can close these
gaps without publishing original data bytes.

## Handwritten byte-comparison lead

`8000F8D0` has the genuine three-input byte-comparison contract: two regions and a
signed count, returning zero/equal or one/unequal. Nonpositive counts do not load
memory. The complete alignment and partial-word prefix structure is explained by
[decompals/ultralib bcmp.s](https://github.com/decompals/ultralib/blob/e24c836796df4bf520ff8b11a5c9d2cea3a66cbd/src/libc/bcmp.s),
pinned blob `def3235503047cafe9c4219bd08fafa7c734c24c`. This is a concrete
handwritten-assembly family lead, not verified assembled binary identity. No donor
source is vendored and no donor assembly or C variant was compiled in the scout.
Exact corpus identity remains with the maintainer corpus process.

## Conditional pitch handler

`80022CD4` has a closed two-pointer, byte-result interface and the pinned CC0
[DoSetPitch family lead](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c#L597).
Its external halfword anchor is `8002D476`; the unchecked scan moves backward and
uses the selected adjacent endpoint. Neither a 13-entry object nor its original
contents, extent, monotonicity or terminating-input invariant is proved.

A narrow conditional reconstruction is active. It must use actual address
arithmetic without fabricated array bounds or values, require valid backing
storage for every visited and adjacent halfword, finite termination and nonzero
divisors, and reject invalid paths before executing plain C tests. No absent native
guards may be added. The donor table is not a substitute for native data: equal
lower endpoints can force a scan below its nominal first element, while wrapping
32-bit normalization can produce zero and make a strict unsigned-less-than stop
impossible. Those counterexamples and unknown original producer bounds remain.

## Decoder-facing callers

A bounded interface audit separately closed `8002574C` and `80025F74`, now active
as caller-only work. Actual registration/consumer evidence gives the first a
no-input callback contract; the second consumes one stream pointer. Existing
native entry/live-in/return evidence supports the opaque decoder declaration
`int (StreamState *, void *)`; this proves no decoder algorithm or descendant
behavior. The audit used entry/exit and stack/control-transfer metadata only.

Caller reconstructions must preserve the independently supported 4,648-byte state
view, unsigned wrap/halfwords, signed pending/state/budget fields, live helper
reloads, aligned buffers, positive capacity and callback lifetime. The decoder
and all out-of-census descendants remain outside these implementation claims.

## Recovery provenance

This dated source-free report was reconstructed from retained authored text after
local working files became unavailable. Its numerical snapshot remains a
cut14-era snapshot, not a claim that later packet files survived unchanged.
The independent native/source scouts added no implementation credit. Subsequent
source-cut manifests and fresh reviews are authoritative for recovered bodies.
No original Git blob hash is asserted for this reconstructed report.
