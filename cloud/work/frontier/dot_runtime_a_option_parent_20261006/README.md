# Runtime-A option parent: genuine closure research

Fixed base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`; stock IDO 5.3,
actual `-g0 -O3 -mips2 -G 0 -non_shared` group pipeline.

This extends #238 with complete real E5DC (3,704 bytes), AEB0 (1,240), B3EC
(1,096), B940 (628), AE50 (88) and B388 (100) source bodies. E5DC's real outer
context improves DFEC from 380/380 to **267/380 differing words**. C93C remains
at **14/234**, with its correct 72-byte frame and integer allocation.

All claims are empty. The other observed differences are ACB0 97/104 (+3 extra
words), B834 45/63, AE50 22/22, B388 25/25 (+13), B940 118/157 (+13), E5DC
916/926 (+2), AEB0 308/310 (+5), and B3EC 272/274 (+8). E5DC, AEB0 and B3EC
retain respectively four, four and six unverified own-section relocation sites
because their emitted geometry differs. These are complete reconstruction
research bodies, not new accepted or matching-byte credit.

## Authenticated control flow

E5DC contains two computed tables. Both were recovered from authenticated image
A, rather than inferred from case order: D_803B9354 has all 21 option cases;
D_803B93A8 maps modes 0/1/2 to the basic range, 3/5 to no update, 4 to the
14–18 range and 6 to the 6–14 range. The source preserves all recovered paths.
The initial scouting impression that this root had no table was corrected
before this reconstruction/publication.

`reproduce.py` uses the same fixed-asset in-memory authentication as updated
#208: asset length 12,418,096 and SHA-256
`f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8`, loader pointer
0xB5C534 at asset offset0x3850, raw-deflate image length194,128 and SHA-256
`0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667`, image base
0x8038A400. It binds these bytes to the normal scorer without emitting a dump.

## Source/context limits

All four native direct AE50 callers are present: AEB0, B388, B3EC and B940.
Stock O3 still inlines AE50 into callers and leaves an eight-byte retained stub.
An authenticated image scan found no address-taken AE50 data reference. No
artificial barrier, dummy caller, forced register or invented callback table
was introduced to hide that remaining compilation-context problem.

E5DC currently saves fewer FP registers because the genuine A820 renderer is
still external. Its emitted frame is104 versus native120; this explains an
important portion of the root's shifted comparison, but is not a match claim.
Other private-register, scheduling, frame and literal-layout gaps remain.

AEB0 contains ordinary typed display-list construction, including vertex and
triangle commands with native field values. Those are generated RSP command
fields, not CPU assembly or an embedded binary dump. Its initial four-byte
color snapshot is an actual native load/copy; no later use was observed.
Typed sprite/resource views document accessed native offsets, not complete
original SDK object layouts. Native float values used in the new AEB0/B3EC
bodies come from the authenticated image; original source expressions remain
unknown. C93C's existing external float anchors are still unclaimed.

Polling D_8002EB70 uses the same volatile-s16 contract already present at the
fixed base in `src/blob/groups/codex_input_aux/group.c:623,3904` and
`src/blob/groups/audio_frame_sync/group.c:569`. The frame-clock qualifier is
the inherited same-symbol hypothesis documented in #224, not a new assertion
of an interrupt writer. B834 passes its actual index to the main-blob F7644
predicate, as corrected in #238. Main-blob wrapper definitions remain external;
`wrapper_return.c` is documentation, not a runtime-A compiler input.

This is an alternative expanded context for #238. Do not install overlapping
copies together or count its earlier functions again.

## Reproduce

```
python cloud/work/frontier/dot_runtime_a_option_parent_20261006/reproduce.py \
  --repo . --reference-root .
```

Only matching compilation/scoring was done. No proof packet, acceptance suite,
CI wait, production change, lock or promotion is part of this lean draft.
