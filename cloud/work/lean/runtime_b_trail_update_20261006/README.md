# Image B trail update: lean research

Target: `B:func_8038CB20`, `[0x8038CB20,0x8038D054)`, 1,332 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result and reproduction

Standalone IDO 5.3 O3: **54/333 words differ**, no extra words, unresolved
symbols, unverified relocations or errors with authenticated image-B data.
The initial complete source differed in 282/333 words plus seven extra.
A shared cleanup label reproduces the native backward cleanup edge, and
separate existing/new fade-node variables remove an allocation collision.
This is a NONMATCH research candidate; frame, allocation and scheduling
residuals remain.

```
python3 cloud/work/lean/runtime_b_trail_update_20261006/reproduce.py
```

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The helper authenticates the frozen native asset and image B in memory and
supplies the required own-data context to the unchanged canonical scorer.
Seven float references at B:0x80394D90..0x80394DA8 correspond to binary32
0.0333333, 0.2, 0.0333333, 0.025, 0.35, 0.1 and 0.16. No bytes/dumps included.

## Source and assumptions

First walks emitted fade nodes: subtract elapsed time, retire expired quads,
and reduce alpha in steps of 23 when their fade timer fires. Then walks live
trail records: free removal-marked sources, update delayed endpoints, or
allocate a colored quad between old/new endpoints and advance the source.
Flag-selected width/lifetime/color cases and the three-plus-player adjustment
are retained. Capturing next links before callbacks allows removal while
walking either list.

The source record at B:80395EE8 is also initialized by the separate D054
match candidate. Native views expose next pointers, mode flags, timers, four
position triples and a four-byte color. Existing pool, polygon and disposal
helpers remain external. Record names, complete pool definitions and original
source/TU boundaries are unknown; no whole-function arcade donor is claimed.
Unknown fields preserve actual record storage. No artificial padding, unused
pressure locals, fake callers/helpers, volatile or inline assembly.

Inputs require valid finite timers/positions, backed linked records and valid
associated polygon/source pointers. Pool/list lifetime and mutation beyond
the observed external helper boundaries are not proved here. Only compile/
scoring was performed. These observed scores are not accepted coverage;
independent checker owns further validation, acceptance and integration.
