# Deeper overlay-A wave: 1,372 distinct new candidate bytes

This continuation recovers five complete image-A bodies with stock IDO:

| Target | New bytes | Strict final score |
|---|---:|---:|
| A:80392F28 | 220 | 0/55 |
| A:803930B8 | 264 | 0/66 |
| A:803931C0 | 276 | 0/69 |
| A:803932E4 | 168 | 0/42 |
| A:8039E200 | 444 | 0/111 |

Total **1,372 new candidate bytes**, zero accepted-image and cartridge gain.
The unchanged 136-byte E3BC result is inherited from draft PR309, excluded
from this total. Earlier 93004, D494 and ED20 matches remain exact and receive
no duplicate credit. D05C improves to 18/169 but remains unclaimed.

## Integration and independent evidence

Experiments began at local 4296906ea, whose tree equals reviewed PR309 head
df8a01e891c5ab660659eb2370596503909367f6. The additive submission is rebased on
master f38968ecbec24dc23e5f178cb37f2cd892992d1f, preserving its unrelated wave
and static-promotion changes. The unchanged PR309 prerequisite files are
included explicitly so this master-based draft can run normal Verify CI.
No protected targets, source production files, locks, build policy or tools
are changed by this submission relative to that master base.

- Availability family: separately read full ELF extents, GNU-resolved complete
  bodies, all 14 generated family callsites, 8,467 protected-native/GNU/unchanged
  C89 UBSan/bounds cases. Real 9338C/94058 caller source supplies the context;
  those complete callers remain nonmatching and are not claimed installable.
- Selector compiler evidence: stock optimizer marks the standalone function
  external and the genuine group internal, with different register descriptors.
  Both diagnostic builds are whole-object identical to canonical builds.
  No compiler modification or claimed retail IR is involved.
- Record navigation: complete 444-byte body and 15 relocations, 6,546 cases per
  native/GNU/host route, 798 callback handle-rewrite cases and all 111 instructions.
  Peer review adds 2,250 independently modeled topology cases, including all 720
  permutations of six linked nodes. Large callers remain source-audited only.
- D05C: 25 grounded source forms and stock compiler diagnostics leave 18 register
  differences. No fabricated pressure or broad blind sweep was substituted.

All private ABI, external-helper fixture, data-lifetime, valid-index and finite
execution limits are retained in the individual packet READMEs. Independent
GNU links contain complete extracted functions; they do not prove original
contiguous-TU placement, full-image integration or game execution.

## Local aggregate verification

Full source tree at 062be968cedf589d623c4e82614fd61c22d2d5eb:

- 3,003 passed, 44 skipped, 9 node-required deselected, 27 subtests passed
- Zero failures and zero errors; 635.65 seconds
- Correct current-checkout RUSH_RECOVERY_ROOT and pinned submodule revisions
- REQUIRE_TOOLCHAIN=1; local IDO and GNU MIPS binutils present
- Only unsupported LeakSanitizer disabled with ASAN_OPTIONS=detect_leaks=0;
  ASan and UBSan remain enabled in tests that use them. Leak checking is unverified

The 44 skips comprise nine absent cross-GCC checks, private/generated ROM,
layout or SDK inputs, and superseded research contexts. They are not described
as passing checks.

The later E200 peer-review script/receipt/test was added at adadeb854 without
changing matching source. A final mixed replay on that tree passes all 747
selected tests in 45.82 seconds, including the added peer test, both new author
packets, family independent review and main-image scorer regression. This
separate replay is distinguished from the earlier full-suite collection.
The prior family-focused mixed replay passed 838 tests; the integrated family
replay passed 8. All 435 current-master static locks remain intact.

Reproduction commands and all author/peer receipts are in:

- `dot_runtime_a_callers_deep_20261011/`
- `dot_runtime_a_leaf_review_20261011/`
- `dot_runtime_a_record_context_20261011/`

Only safe UTF-8 source, tests, research and verification notes are published.
No ROM bytes, raw assembly dumps, binary artifacts, credentials or unrelated
private data are included. No merge, image splice, compression, ROM SHA-1,
hardware or gameplay gate is performed by this wave. The independent checker
owns acceptance and merging. Hosted CI status belongs to the exact published
commit and is reported in the draft PR after it runs.
