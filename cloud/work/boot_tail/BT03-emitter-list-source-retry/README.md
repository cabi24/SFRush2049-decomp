# Emitter list source-family retry

**No new matching body, matching bytes or unique attempted target.** Five bounded
source controls reopen two complete NONMATCHs, 708 bytes. The only numerical
improvement is limited: `1DA74` changes 58/101 to 57/101 and its actual ELF function
extent changes 400 to the native 404 bytes. Agreement is gained at the return
epilogue +0x18C and its delay-slot nop +0x190, but lost at the interior store +0x11C:
two epilogue words gained minus one interior word lost. No interior algorithmic
block is thereby closed. The published
selected sources and central ledger remain unchanged.

Base is current master `cf4b9c619c72e83aa5da2e8b5c110765f9b03693`, whose tree equals
the accepted PR77 source tree. Branch `dot/boot-tail-emitter-list-source-retry`
reuses a clean worktree while retaining its previous branch. `claim.json` records
the parent's exclusive claim and maximum three causal forms per target.

## Evidence and bounded hypothesis

The pinned CC0 [snd3d.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd3d.c)
has `AddRunningEmitter` at lines1115–1153 and `AddStartingEmitter` at1155–1212.
Both use a for-loop traversal and make the final volume-store expression own the
pool-count postincrement. Prior `BT03-high-runtime-lists` controls use while loops
and separate increments; they only tested group-count snapshots and named pointers.
All of those prior sources, readme, source/ABI review and control receipts were
read before the new hypothesis. `references.json` binds freshly fetched source
and CC0 license blobs.

Canonical native inspection independently supports the store-tail lead. `1D944`
stores the new next pointer, object pointer and count before the volume return-
delay-slot store. Its retained source instead writes volume before object and a
standalone count increment. Native `1DA74` keeps an existing large-list head as an
anchor, tests its next pointer, and reloads the pool count after linking; the donor
traversal can express this without replacing it with a conventional sorted insert.

Donor differences are material: its `numRunning` field/stores, 64-entry group and
starting-node capacities, and version2.0.1 extra LPF float/field have no corresponding
native operations. Its running-node capacity is64 through version2.0.0 and128 from
version2.0.1 onward (`RUN_LIST_SIZE`, lines390–399). None is adopted. Existing
32-entry bounds and the actual pointer-plus-five-float
ABI remain exact. `distance` is retained as the historical local field name;
the source-family volume name does not rename canonical symbols or types.

## Exact outcomes

All O2 rows have full resolved references and zero unresolved symbols, unverified
relocations or relocation errors. Nonzero excess instructions still fail.

| Function / source | O2 differing / target | Nonzero excess | Actual ELF bytes |
|---|---:|---:|---:|
| 1D944 frozen baseline |53/76|0|304|
| 1D944 for traversal |53/76|0|304|
| 1D944 postincrement tail |53/76|0|304|
| 1DA74 frozen baseline |58/101|0|400|
| 1DA74 for traversal |57/101|0|404|
| 1DA74 postincrement tail |57/101|1|412|
| 1DA74 combined |59/101|2|416|

The small traversal has byte-identical relocated O2 text to its baseline. Its tail
changes only three store-order sites without reducing the residual; a combined
small control would repeat the same machine-code experiment and was not attempted.
The large traversal changes the loop region, while the tail-only control changes
the terminal store region. This independent localization justified one combined
control, which worsened the result and closes the three-form bound. O1 was replayed
for every source and is uniformly worse, with complete receipts in
`verification.json`. Object section-alignment padding is not function extent.

`controls/func_8001DA74_for_traversal.c` is preserved as a limited research lead.
It is not a matching submission, a central status replacement, or a basis for more
register, declaration, line, flag, operand or storage sweeps. Further work requires
new original-source/translation-unit or measured stock-compiler evidence explaining
the group-count/index allocation and remaining interior geometry.

## Semantics and scope

The source prefixes and all types/formals are unchanged from the reviewed baseline.
There are no calls, jump tables or external decoder inputs in either function. Small
insertion requires pool count<32 and, for a new group, group count<32. Large insertion
checks both capacities, preserving creation of a new group before node exhaustion.
The real list links are valid allocated acyclic nodes; the next free node is not
already in a list. An emitter object and the three fixed global arrays are distinct
valid objects. Shared list-head pointers across groups are allowed and tested.

For traversal, re-reading nonvolatile next pointers with no intervening calls or
writes is equivalent in that domain. No concurrent mutation/data-race guarantee is
made. The tail controls reorder independent fields of a new node and the separately
located count byte, with no intervening callback; final state is preserved. FP
comparisons retain ordered-less-than behavior under the ordinary engine FP state.
Tests include quiet NaN and infinities, without claiming arbitrary FCSR behavior.

All five controls pass the prior source-bound C89/ASan/UBSan list/capacity fixtures:
280 small-control calls and840 large-control calls. An added420-case test binds the
retained large traversal source and checks nonconsecutive node links, shared heads,
the last free slot,32-group/full-pool failures, all five float inputs, object bytes,
all group/node storage and the unrelated small pool. These are host behavior tests;
pointer-containing O32 sizes are not falsely asserted on LP64. Source digests bind
the checks to the actual compiled controls. Only LeakSanitizer is disabled for the
ptrace environment; no heap is allocated.

## Reproduce

With the pinned IDO and MIPS tools configured:

```sh
python3 cloud/work/boot_tail/BT03-emitter-list-source-retry/verify.py --check
python3 -m unittest discover -s cloud/work/boot_tail/BT03-emitter-list-source-retry -p 'test_*.py' -v
```

Fresh preflight checks the pinned compiler files, immutable canonical manifests,
all439 extents/99,120B and getter MATCH. Unchanged workbench diagnosis ran before
candidate editing and is recorded without raw native listings. Full relocated
scoring remains authoritative over workbench relocation-layout heuristics.

Only this research packet changes. No published source, central ledger, scorer,
target, symbol, header, layout, lock, compiler, spec, runtime image, production gate,
D1248 or restricted helper is changed. No ROM bytes, raw assembly dumps, objects,
credentials or unrelated private information are published. Independent review
PASS binds source commit `3144c3e80c3296f94011a2999718b2e251538d87` and all unchanged
candidate/test hashes in `independent_review.json`. The reviewer independently
replayed all14 O2/O1 rows and four sanitizer groups, inspected both complete native
bodies and verified donor/version provenance. Its three documentation corrections
are included above. Merging and cartridge integration remain with the independent
checker.
