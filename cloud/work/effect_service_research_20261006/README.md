# Tagged effect service and image-B effect tick: portable research

This additive, source-only package preserves two complete semantic C closures,
their original one-shot canonical O3 **NONMATCH** baselines, and independent
source/native and existing-linked-object reviews. It supplies no production
replacement, match submission, acceptance lock, image/ROM coverage, original
translation-unit claim, or merge recommendation.

- [AF06C tagged-input effect service](../af06c_tagged_effect_service_20261006/README.md):
  complete 1,200-byte native root and 1,128-byte genuine private child. The
  candidate naturally inlines the private body and emits a 2,096-byte root;
  298/300 native words differ, with 220 nonzero excess words.
- [Runtime-B effect tick](../runtime_b_effect_tick_20261006/README.md): complete
  988-byte native root and two 552-byte genuine private children. The emitted
  root is 1,560 bytes, D38 is 548 bytes, and B10 is a natural 8-byte deleted-static
  stub. The root has 245/247 differing native words, 141 nonzero excess words,
  an unresolved private reference and two unverified own-data references.

Every C file, Python verifier, existing receipt and original README was copied
byte-for-byte. All behavioral domains, fields, path limits, ABI assumptions,
original visibility/TU uncertainty, and honest nonmatch results remain intact.
The packet-specific READMEs and independent reviews define the full proof bounds;
this index does not broaden them. The original canonical routes and the exact
bare O3 headers remain unchanged, including mandatory backend-flag provenance.

## Fresh replay and receipt continuity

Seven complete replay receipts were checked against their historical versions.
Every actual proof field was equal: native identities, source identities,
fixture counts, behavior, complete extents, relocation verification, owned-data
accounting, failures, and limitations. Only two provenance differences occur:

1. AF06C's host command names the copied host source at its new location. The
   historical receipt is retained; this diagnostic path is not an equality gate.
2. The historical independent effect-tick source receipt binds an earlier
   `native.py` digest. Its original `independent/receipt.json` is retained intact.
   A fresh full replay against the final owner interpreter is included as
   `independent/receipt-current.json`. The only changed value is that interpreter
   binding. All actual proof fields, including 2,498 fixtures, 2,528 root runs,
   120 private runs, 1,800 player oracles and the complete stated limits, are equal.

No source, verifier, loader or compiler-wrapper change was required. Both
existing canonical scorer loaders establish their own `tools/cloud` import path
before loading `score.py`, so its sibling `owndata` import works from a foreign
working directory. The new registered tests exercise that isolation directly.
Original machine-local artifact paths in historical receipts remain diagnostic
provenance only, not reproduction requirements or receipt equality predicates.

## Focused reproduction

Use a repository containing historical commit
`6b2e9e506fe3d2267a710e41c85af5364ccd00c7` and its asset as the reference. Native
images are reconstructed only in memory from `git show BASE:path`; they are not
included in this source package. Mutable production source, tools, manifests,
locks and symbols are not receipt-hash predicates or live lock assertions.

Set `TMPDIR` to your own writable temporary directory. To exercise the registered
suite, run from any working directory:

```sh
RUSH_REFERENCE_ROOT="$REFERENCE" \
SFRUSH_REFERENCE_ROOT="$REFERENCE" \
SFRUSH_SCORE_ROOT="$SCORE_ROOT" \
TMPDIR="$OWNED_TMP" \
python3 -m pytest "$REPOSITORY/tests/cloud/test_effect_service_research_packets.py" -q
```

The registered suite invokes each packet's tests in a fresh subprocess, avoiding
same-name module collisions between the two independent research harnesses.
It also verifies packet-owned receipt bindings and missing-IDO loader guards.
It deliberately removes any ambient `tools/cloud` entry from `PYTHONPATH` in
child processes. No test hashes its own test source into a receipt.

To enable effect-tick's existing-object test, provide
`SFRUSH_EFFECT_TICK_ARTIFACTS` pointing to your already built `effect.o` and
`effect.elf`. It skips cleanly without these optional artifacts or without pinned
IDO/MIPS GNU linker support. Neither the registered suite nor either packet's
suite recompiles target C. Host-dependent tests skip when `cc` is absent.
See each packet's original reproduction section for full owner/independent
semantic and existing-object CLI commands.

The fresh checks use existing one-shot artifacts, not a new target compilation.
The separate direct missing-IDO and missing-linker entry-point tests return SKIP
before any compile marker. The original IDO layout proof for effect-tick is
retained; AF06C's 28-fact host ILP32 syntax-only layout check was rerun.

## Scope of this preparation

`verification.json` records the focused matrix and replay comparisons. These
checks are bounded research verification, not a repository-wide aggregate pass.
No full checkout, integration-tree edits, protected edits, external writes,
publication retry, PR creation, merge, CI observation, or new target compilation
was performed. Publication remains stopped pending explicit confirmation.

Only C, Python, JSON and Markdown are included. Native/compiled images, ELF,
objects, disassembly, raw logs, caches, compiler scratch files and local
preservation archives are excluded.
