# Route tracker: clean storage reconstruction, research only

Base: `52904b374dc5eb8e88d5e003d6ea4666cdde71d2`.
Targets: `audio_mixer_main` `[0x800BA7C4,0x800BAAA0)` and
`audio_priority_find` `[0x800BA46C,0x800BA61C)`.
These historical names identify route/checkpoint setup, not audio mixing.

## Result

The genuine helper still scores strict **MATCH, 108/108 words** after correcting
the shared storage model. The caller remains **NONMATCH: 64/183 words differ,
one nonzero excess word, 736 emitted bytes versus 732 native bytes**. Its frame
is 72 bytes versus native 80. No matching or accepted-byte claim is made.
The helper remains provisional because its genuine caller is not matched.

The fresh proof compiles the real caller and helper with unchanged accepted
`func_800BA2B8`, `func_800BA61C`, and `audio_channel_alloc` source files. All
three accepted bodies now score strict **MATCH** through the upstream scorer's
own-data verification. The separate unchanged `owndata.verify` check also
verifies both complete referenced literals in BA2B8 and BA61C against the
integrity-checked repository data artifact. This packet does not change the
scorer or bypass CI.

The original independently reviewed packet was commit `0c6994ff` on base
`b34deebf`. This refresh moves only its research additions onto `52904b37` and
updates provenance, receipts, assertions and prose. The caller/helper source,
host-test source and compiler recipe are unchanged; the complete compiled
object remains byte-identical. Upstream own-data verification changes the two
callee verdicts, without improving the caller or adding a matching claim.

This is a genuine five-body group proof, not a fresh whole-program shadow-unit,
linked-image, compressed-stream, or ROM proof. No stand-ins enter its compiler
context. No accepted source, lock, scorer, flags, target or shared override was
edited. The work used an isolated cloud worktree.

## Newly established layout

`D_80151CE8` is a 12-byte header followed by ten 80-byte records. This is not
a guessed capacity: the accepted initializer copies 812 bytes, the loader
advances to the next serialized section by exactly 812, and another genuine
consumer independently forms the first record at `0x80151CF4` with stride 80.
See `layout.json` for exact function/address/source evidence.

Each record contains a 20-halfword index array at +34 and a float length at
+76. Consequently the mixer accesses described as global+80*i+46 and
global+80*i+88 are ordinary in-bounds fields of record i. The old w2b source
instead declared `idx[17]` and a separate oversized overlay to preserve a
prefix-anchored 80-byte stride. Its first loop nevertheless wrote twenty
indices. This packet fixes that source-model defect without changing native
addresses or adding record capacity.

Header count is still an input invariant: initialization does not clamp it to
ten. The meanings of header+10 and record+74 remain unknown. Record range is
a signed integer threshold used in a squared-distance comparison; it is not
asserted to be an unsquared geometric radius.

## Prior work and bounded decision

- `cloud/work/frontier/w2b/RESULTS.md` records a 31/183 candidate with artificial
  `s32 pad[2]`, the invalid prefix view, and about 35 unsuccessful loop/counter
  spelling controls. That number is not a clean-source baseline.
- `cloud/work/near_miss_B134.md` already records a no-padding, flat-view caller
  at 155/183, a threshold-order control at 150/183, and separate position/cursor
  views at 175/183 plus four excess words. Its conventional helper-call order
  predates w2b's genuine internal `(who, route)` reconstruction. Those scores
  use a different source/context recipe and do not measure this packet's gain.
- BA2B8 was integrated in `3a7d894f`, making all three accepted callees available.
  Its integration closes a dependency but does not explain the second-loop
  `k+1` common-subexpression residual.

The clean source retains all three native stores to the genuinely unused
position vector. It contains no artificial stack padding, fake dead read,
unused formal, keeper, oversized index array or source-shaping stand-in.
Only the evidence-grounded layout reconstruction was compiled; no spelling
sweep or counter-variant search was run.

Fresh workbench diagnosis reports a structural mismatch with one extra
instruction and an eight-byte frame deficit. Saved-register space is 40 bytes
in both builds; non-save space is 40 native versus 32 candidate. The frame
provenance and second-loop lifetime/CSE remain unexplained. Workbench's target
object has no relocations, so its masked count is not matching evidence; use
the canonical resolved scorer result in `verification.json`.

**Reopen only for** authentic declaration/inlining provenance explaining the
eight-byte frame difference, or measured compiler/native evidence predicting
the second-loop lifetime change. Generic pointer loops, counter-width changes,
padding removal alone, and the existing w2b spelling controls are not new work.
Current ownership inspection found no continuing claim on this caller; the
historical w2c wave label was not treated as a reservation.

## Reproduce

With the pinned IDO toolchain selected by `IDO_DIR`, GNU MIPS assembler/objdump
on `PATH`, and the repository's Python dependencies available:

```
python3 cloud/work/frontier/dot_route_tracker/verify.py
python3 cloud/work/frontier/dot_route_tracker/host_tests.py
```

`verify.py` recompiles source, checks accepted-context source hashes, resolves
complete target bodies with the untouched stock scorer, verifies context
literals separately, runs the workbench diagnosis, and compares the complete
fresh metadata receipt. Raw native inputs and objects exist only in temporary
directories and are never delivered. `--write` explicitly refreshes the receipt.

Host tests establish their stated bounded semantics with three test doubles;
they do not establish matching, full gameplay behavior or accepted helper
implementations. Compiler verification exclusively uses the real accepted
callee bodies. The missing ROM gates remain maintainer-owned.

Fresh GCC C89 `-O2` and `-O2` AddressSanitizer/UndefinedBehaviorSanitizer runs
each pass 395,264 mixer cases and 23,094 priority-helper cases. Coverage includes
all valid tracker-count/route-count/first/last combinations, eight modes, eight
geometry profiles, tracker 9/index 19, and the complete 812-byte state, both
totals and external call order/arguments. Two test-only output corruptions
(index 19 and a total) are detected. Exact flags, source hashes and excluded
input domains are in `host_verification.json`. Seven focused tests pass with:

```
python3 -m pytest -q tests/conveyor/test_dot_route_tracker.py
```

Host position samples remain within the representable signed-half conversion
range. Finite values alone do not establish float-to-s16 safety for every input.

The selected existing scorer and own-data suites were freshly run in both the
candidate tree and an isolated checkout of the exact refreshed base. Both pass
all 669 tests with no failures or skips. `baseline_tests.json` preserves the
paired results. The former 643 passes and 21 own-data failures on `b34deebf`
are retained only as superseded history in `baseline_tests_historical.json`.
These selected suites are not a full CI run. No unrelated CI or scorer repair
was made by this packet.
