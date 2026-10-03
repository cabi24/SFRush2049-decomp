# First boot-tail matching wave

**Source-frozen, paired independent review passed.** Exact-source-head CI passed; aggregate replay:
**42 verified strict matches / 1,332 B**, plus **8 complete nonmatches / 404 B**.
These 50 attempts total 1,736 B and are disjoint. Exact-source-head CI passed at `3574dbec` in draft PR #61, so the central
ledger now records the 42 submitted bodies as verified.
This is verified-source work, not cartridge coverage or maintainer acceptance.

| Packet | Attempted | Local strict matches | Nonmatches | Exact reviewed source commit | Independent reviewer |
|---|---:|---:|---:|---|---|
| C11-small | 5 / 148 B | 4 / 104 B | 1 / 44 B | `aae267e9a59d0f74eb7fb558333e9383974d2c2e` | C13 worker |
| C13-small | 9 / 288 B | 9 / 288 B | 0 | `a50d5fa7b09b75c359dc3330c3d21df3a64c5145` | C11 worker |
| BT02-small | 9 / 344 B | 9 / 344 B | 0 | `3f0057926a1854ac1d03b50d7d46667e0f151770` | BT03-low worker |
| BT03-low | 10 / 308 B | 9 / 260 B | 1 / 48 B | `f2c816f9236034261b83db1d766934605c95a6d9` | BT02 worker |
| BT03-high | 10 / 408 B | 4 / 96 B | 6 / 312 B | `53bff645fcba96ae71f1bf29f18aae72054a3897` | BT05 worker |
| BT05-leaves | 7 / 240 B | 7 / 240 B | 0 | `927fb95e45a0d1fb5579e51893a801da7cb18afe` | BT03-high worker |

Each paired review inspected actual C, relevant native/callee ABI evidence,
whole extents and strict replay. The central integration preserved every source
byte from those exact commits; `reviewed_sources.json` binds all 50 source hashes,
flags, original review commits, lengths and outcomes. No merge was performed:
reviewed packet commits were cherry-picked into an isolated aggregate branch.
Merging this draft remains solely the owner's independent checker's decision.

## Base, dependency and ownership

Branch: `dot/boot-tail-p3-wave1`. Completed Packet 2 checkpoint:
`76780b3a1b3e26c54b86e1f153344e92dac15b20`.
Explicit unmerged source dependency: [PR #59](https://github.com/cabi24/SFRush2049-decomp/pull/59)
at `21e104a22575cf4d639261d2f6d9535913074e6a`, tree
`1018b58fb08e0c7abe921095dfac01e70322b4d3`.
Master preflight base: `b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`, with owner-merged
#52 followed by #54. The aggregate targets master because repository CI is
configured only for PRs with that base; shared #59 research and Packet 2 are
explicit dependencies rather than silently merged work.

Packet 2 independently passed pinned IDO setup/hash, all boot-tail manifest
members, all 439 census starts/sizes (99,120 B) and existing getter replay before
matching started. The historical getter remains separately credited as 12 B;
these new 42 bodies add 1,332 B of strict matching source only.

`../claims.json` is the coordinated ownership registry. Source-frozen packets
can release their edit claim only after peer PASS, publication and exact remote
tree verification. Each worker may have one next packet queued. The aggregate
lead owns exact-head CI through terminal status; if CI fails, dependent new work
pauses while authorized repairs are tested and republished.

## Exact acceptance checks

`verification.json` is a new aggregate compile of all 50 actual sources. The 42
submissions all pass strict relocated full-word comparison with zero differing
words, no nonzero excess words, no masked relocation fields, no unresolved
symbols, no unverified section-relative relocations and no relocation errors.
Every target extent agrees with its reviewed inventory size. Only ordinary zero
alignment padding lies outside some native extents; it receives no byte credit.
The eight archives reproduce their exact recorded nonzero residuals.

The aggregator ran the unchanged changed-submission gate, focused packet
receipts, existing 630 cloud setup/guard/submission/integrity/scorer regression
tests (zero failures/skips), the 28 central metadata/C11 tests, C13's three tests
including 144 host forwarding scenarios, BT02 host checks, and BT03-low's ten
host semantic scenarios. All 161 static locks remain intact. Focused tests and
host semantics are not production image/ROM gates; no such gates were run.

Reproduce from the root after `bash tools/cloud/setup.sh`:

```sh
python3 cloud/work/boot_tail/scripts/preflight.py
python3 cloud/work/boot_tail/scripts/verify_wave1.py --check
python3 tools/cloud/check_submissions.py --base 21e104a22575cf4d639261d2f6d9535913074e6a --head HEAD
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 cloud/work/boot_tail/scripts/screen_opcodes.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
python3 -m unittest discover -s cloud/work/boot_tail/C13-small -v
python3 cloud/work/boot_tail/BT02-small/test_host.py
python3 cloud/work/boot_tail/BT03-low/test_semantics.py
```

Exact flag ambiguity is retained: all chosen submissions match O2, but several
tiny bodies also match O1. Per-packet controls and native frame/spill evidence
explain the choice; no original translation-unit or whole-cohort flag identity
is invented. Nonmatches retain complete C, counts, flag controls, bounded effort
and a concrete evidence requirement before reopening. Corpus/external-boundary
questions remain outside this packet; none of the 50 targets intersects the
eight existing boundary blockers.

Only `cloud/matches/boot_tail/*.c`, `cloud/work/boot_tail/**` and D10 in
`dot_handoff.md` change. No ROM/image bytes, raw native dumps, compiled objects,
credentials, protected targets/scorer, symbols, layout, locks, shared headers,
runtime-image/farm work or production-gate edits are included.

## Explicit current-master base refresh

Immediately before publication, master advanced to
`301d9e7552ad4fd7f54a38796db84671e1000d35`. The aggregate was explicitly replayed
onto that exact commit using only allowed boot-tail files and the D10 section.
All unrelated current-master content, including D11, was preserved verbatim.
The 50 reviewed sources and protected boot-tail target/toolchain inputs were
unchanged; aggregate strict replay and the current-master guard were rerun.
PR #59 remains unmerged. Its research is incorporated explicitly by source
provenance from `21e104a2`, rather than merging the PR or claiming current ancestry.
The historical Packet 2 and per-worker review commit identities remain intact.

## Published source-head CI receipt

Draft [PR #61](https://github.com/cabi24/SFRush2049-decomp/pull/61), head
`3574dbec2ecd4a4eee7cf95964cf47329cabbcb2`, exact tree
`33ccf2785a3eb53f749e4db494dad3fc6450bb67`, passed
[Verify run 37152316686](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37152316686).
All 42 matching C files are unchanged in this status-only receipt revision.
`ci.json` records the closed source-head proof. This new bookkeeping head will
also receive exact-head CI before final completion; the PR description carries
that final result so recording it does not create an endless new-commit loop.

The D10 handoff is intentionally unchanged from the published source-bearing
revision: its local-match and pending-CI wording predates this completed CI proof.
A full-file update was blocked because of inherited private metadata outside D10,
so only the independent allowed ledger, README and receipt files are updated.
The CI result and verified-body totals here and in STATUS.csv are authoritative.
No inherited handoff sections were removed or republished in this receipt update.
