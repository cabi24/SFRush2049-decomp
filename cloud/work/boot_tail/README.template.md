# D10: boot-tail runtime research and matching

The 412 assigned functions account for **95,012 B**.
The current ledger records **@@OPEN_FUNCTIONS@@ open functions / @@OPEN_BYTES@@ B**,
**@@NON_C_FUNCTIONS@@ confirmed non-C rows**, **@@PREEXISTING_VERIFIED_FUNCTIONS@@
pre-existing verified bodies / @@PREEXISTING_VERIFIED_BYTES@@ B**, and
**@@NEW_VERIFIED_FUNCTIONS@@ newly verified bodies / @@NEW_VERIFIED_BYTES@@ B**.
There are **@@CLAIMED_FUNCTIONS@@ claimed functions / @@CLAIMED_BYTES@@ B** and
**@@NONMATCH_FUNCTIONS@@ nonmatches / @@NONMATCH_BYTES@@ B**.

Packet 1 itself introduced zero matching bodies or verified bytes. Its initial
verified entry imports the existing 12-byte `func_80010A00` PR #54 receipt, now
present on master; Packet 1 does not locally replay that match or claim a fresh
current-master scorer/CI proof. No cartridge-coverage claim is made.

Base: `b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`, after the owner merged
[#52](https://github.com/cabi24/SFRush2049-decomp/pull/52) then
[#54](https://github.com/cabi24/SFRush2049-decomp/pull/54). Original spec/inventory
commit: `92e91f12`. The spec is read-only. Matching edit scope adds
`cloud/matches/boot_tail/*.c` to this directory and D10 in `dot_handoff.md`.
Packet 1 was research-only. Later compiler/scorer checks are separately recorded
below. Generated targets, runtime-image/farm work, production gates, locks,
layout, shared types and paused targets remain untouched.

## Packet 2 preflight (2026-10-03)

Completed on explicit stacked PR #59 base
`21e104a22575cf4d639261d2f6d9535913074e6a`, whose tree is
`1018b58fb08e0c7abe921095dfac01e70322b4d3`. Fresh master was
`b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`; #52 and #54 were
confirmed merged in order. PR #59 is unmerged and remains an explicit dependency.

All gates passed: fresh pinned IDO 5.3 archive download/hash, all three protected
manifest entries, exact 439-function start/size equality (99,120 B), and strict
replay of the existing 12-byte getter. The replay has zero differing/extra words,
unresolved symbols, unverified relocations or relocation errors. This is a new
replay, not a new body or byte gain. See `packet2/preflight.json` for hashes and
`packet2/README.md` for reproduction. Packet 2 is released; later packets need
separate claims. Nothing was merged or promoted.

## Packet 3 coordinated small-function wave

User requested six concurrent workers. `claims.json` records their disjoint
partitions, exact targets, owners, branches and stops. Initial assignment:
50 functions / 1,736 B. Each worker holds one active packet; only the C11 lead
updates this central ledger. Others submit per-packet status deltas.

The published first-wave source checkpoint has **42 verified matching bodies /
1,332 B** and **8 complete nonmatches / 404 B**. All six packets passed paired
independent source/ABI/strict-replay review. Exact-source-head
[CI passed](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37152316686) at
`3574dbec2ecd4a4eee7cf95964cf47329cabbcb2` in draft
[PR #61](https://github.com/cabi24/SFRush2049-decomp/pull/61); its tree exactly matched
the independently reviewed local tree. Every C file remains unchanged in this
status/receipt update. See `wave1/ci.json`, `wave1/README.md` and the source-hash
manifest. This is verified matching source only; merging and cartridge promotion
remain with the owner's independent checker.

The first-wave editing claims are released and frozen. Historical queued requests
in `claims.json` are superseded by separately coordinated successor work under
later user direction. No subsequent matching source is added to this first-wave
receipt update. Its exact new-head CI must also be checked before final completion.
## Read first

- `STATUS.csv`: exactly one row for each `in_scope` inventory function.
- `clusters.json`: disjoint primary dependency partitions, nested research
  cohorts, edge counts, shared references, historical static-call labels and
  small-first candidate lists.
- `research.json`: reviewed boundaries, rationale and per-function annotations.
- `REFERENCES.md`: pinned source paths, licenses, evidence and limits.
- `opcode_audit.json`: bounded opcode classification, including six identify-only
  stubs. No instruction words or raw disassembly are copied here.
- `REVIEW.md`: local verification and independent-review receipt.

The matching state `verified_body` on the getter is imported historical work,
not a new result. Its `VERIFIED-BODY` evidence label must be read with that
explicit provenance qualification in its row. Packet 2 has now completed the
formal fresh target/toolchain/replay checks recorded above.

## Scope and caveats

The source inventory has 439 rows / 99,120 function bytes: 412 assigned rows /
95,012 B, 21 excluded ultralib rows / 4,048 B, and six identify-only tiny stubs /
60 B. Neither excluded population becomes an open matching target. Interleaved
excluded rows remain visible inside interval metadata but contribute no assigned
bytes. Address ranges are half-open; alignment gaps and excluded bytes are never
counted by subtracting endpoints.

There are **21 game-called rows in the complete census, but only 20 in-scope**.
The other is excluded `0x800205E4`. Likewise, 671 all-tail distinct directed
`jal` edges are metadata leads, not runtime call proof or complete call-graph
coverage. Indirect calls, tail calls, data-flow pairing and reachability require
later native analysis. Historical names are carried as labels only, never proof.
In particular `0x800075E0` resolves to `osJamMesg` in the current symbol file;
queue-send semantics or a renamed symbol are not silently substituted.

### Meaningful identification results

See `REFERENCES.md` for exact target offsets and reference comparisons. The
MusyX sound-macro family is a source lead; surrounding address-run membership
and original translation units remain hypotheses. The final body family is a
packed-bitstream decoder candidate, not a located printf implementation.

The spec's `0x8002D890–0x8002D8D0` references need qualification: several are
floating-point loads and a jump-table base, so their mere proximity to mixer
strings does not establish microcode pointers. Audio task evidence comes from
AI operations and native task construction instead. No in-scope `0xA4xxxxxx`
reference was found by the supplied hi/lo scan; PI/DMA/cartridge and F3DEX roles
are **unlocated**, not inferred from expected family names.

**Printf is located in pre-existing counted-static code, outside this lane.**
`sprintf` at `0x80004990` calls the body at `0x80002CD0` (historical label
`fcvt`), which compares format bytes with `%`, dispatches format characters,
and directly references all four cited string locations. `REFERENCES.md`
records the exact tracked static paths and instruction addresses. No tail row
is relabelled printf, and no static symbol/source/extent is changed.

SC-001 is satisfied as a classification deliverable. F3DEX/PI identities,
original TUs, specific N64 middleware version, candidate assembly provenance,
and newly exposed out-of-census callees remain explicitly unresolved; no claim
is made that all deeper research questions are closed.

## Clustering method

1. Start with 17 reviewed, contiguous address cohorts in `research.json`, based
   on the supplied call/reference leads and bounded native/reference review.
2. Preserve explicit semantic barriers around the isolated comparison lead,
   broad audio/runtime region, trailing math utilities, buffered-stream family,
   and packed decoder family. This prevents a trivial whole-image cluster.
3. Within each barrier, find the first address-ordered run having fewer than
   half its outgoing tail edges internal. Merge it with the permitted adjacent
   run giving the best internal-edge fraction; break ties by fewer assigned
   bytes, then lower address. Repeat until no such merge remains. Fractions are
   rounded to six decimal places before comparison; the merge trace is saved.
4. Count **all** outgoing tail edges in the denominator, including destinations
   excluded from this assignment. Report internal, cross-cluster, excluded-tail,
   and static edges separately. Internal means an in-scope member of that run.
5. Keep the 17 narrow cohorts for useful role and source-family navigation.
   Primary clusters are provisional dependency partitions. Even majority
   internal calls do not prove a recovered original translation unit.

`0/0` means no scanned outgoing tail edge, not proven self-containment. External
static calls, incoming callers and excluded helpers remain in metadata. A
broad audio partition is deliberately broad because narrower wrapper cohorts
would misleadingly imply independent TUs despite their cross-calls.

### Suggested small-first matching order

This is a proposal, not a claim on matching work. Each cluster is ranked by its
best still-open under-256-byte candidate: under-64 first, then no scanned calls,
then fewer scanned callers, then size/address. The same tie-breaks generate
`small_first_candidates`; source/ABI checks can override the proposal. Large
roots come last within their family after helper contracts are known. The
existing getter is excluded from the queue. Packet 2 must pass first.

@@CLUSTERS@@

## Census-boundary escalation

Read-only direct-call decoding found **nine calls to four destinations at or
beyond the declared text end**, absent from the inventory call columns:

- `0x80010450+0xA0`, `0x80012234+0xD8`,
  `0x80012730+0x104/+0x380/+0x460`, `0x80012D18+0x78`
  call `0x8002C5E0`.
- `0x80026360+0x1CC` calls `0x80027B44`.
- `0x80026558+0x60/+0x74` call `0x800277D0` and `0x800278C4`.

No destination bytes were inspected. These addresses need maintainer
classification before dependent matching; they do not by themselves establish
incorrect individual function extents or specific external roles. The six
direct callers and two downstream decoder callers carry explicit matching
blockers in the ledger; unrelated validated leaves remain eligible. The
majority metrics above describe the supplied inventory graph only. In
particular BT08 is **not closed under the supplementary native call graph**.
Do not interpret the spec's broad statement about following microcode/data as
proof that all four destinations are non-CPU data.

## Native/non-C screen

Protected targets became available on master during this packet. The optional
read-only `scripts/screen_opcodes.py` checks the assembly file against its
tracked SHA256 manifest and screens the 412 bodies plus six identify-only stubs
for `cache`, `mfc0`, `mtc0`, other COP0, `eret`, and `sync` encodings.

- **Zero in-scope positive functions.** This does not establish that every body
  is C or that delay-slot idioms are compiler-generated.
- Identify-only `0x8000FB90`: `mtc0` at entry establishes privileged assembly;
  it remains outside the 412-row ledger.
- The other five tiny stubs remain **unknown**, not automatically non-C.
- `0x8000F8D0` remains an assembly candidate from the spec/historical `bcmp`
  label; the bounded screen provides no sufficient non-C proof.

The core ledger/clustering generator needs no targets, compiler or ROM. If
protected targets are absent, the optional screen must report missing evidence;
never reconstruct opcode claims from metadata alone.

## Reproduce and maintain

From the repository root, with Python 3.9+ and no third-party dependencies:

```sh
python3 cloud/work/boot_tail/scripts/generate.py
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 cloud/work/boot_tail/scripts/screen_opcodes.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
```

`generate.py` recreates `STATUS.csv`, `clusters.json` and this README from the
read-only inventory, historical symbol map, `research.json` and
`README.template.md`. Change annotations/template inputs deliberately, then
regenerate; do not hand-edit generated outputs. `--check` writes nothing and
fails on drift. The script fails closed on population/size drift, overlapping
function extents, unknown edge destinations or a cohort that cuts a function.
Native-byte investigation remains read-only and separate from generation.

The test script is explicitly invoked here because Packet 1's permitted paths
do not include repository test discovery/configuration. Exact PR-head CI runs
repository gates separately. A focused metadata test pass is not a full
repository regression, native equality proof, target/toolchain check or ROM test.

## Maintainer inputs and stop conditions

1. **EXTERNAL-INPUT: authentic arcade/source provenance.** In the maintainer's
   licensed `reference/repos/rushtherock/` checkout, search for
   `bug in vsprintf: bad base`, lower/upper hexadecimal digit strings,
   `N64 RSP mixer`, `MERRILL BLACKOUT`, and `0xA8351D63`/`2822053219`.
   Report exact file/function, revision and license. The static printf path is
   already located; a separate owner can investigate source identity or the
   historical `fcvt` label. No arcade contents have been assumed from memory.
2. **EXTERNAL-INPUT: MusyX version/layout.** Search authenticated N64 middleware
   records for macro handling, master-volume groups `0x15/0x16`, matrix helper
   names and any associated release/build stamp. Report version, revision,
   headers/layout and redistribution license; no incompatible vendoring.
3. **EXTERNAL-INPUT: census-boundary call targets.** Classify `0x800277D0`,
   `0x800278C4`, `0x80027B44` and `0x8002C5E0` using maintainer-owned native
   data/layout evidence; report CPU function extents/identity, veneers, or
   corrected targets as appropriate. Resolve whether this changes the census
   boundary. Do not have this research packet inspect or edit beyond-scope data.
4. Treat discovered ultralib/libc corpus variants as maintainer corpus work,
   extent errors as generator-owner work, and conflicting shared types as
   proposals. Do not silently fix protected inputs or broaden Packet 1.
5. Packet 1 stopped after research publication; Packet 2 subsequently passed
   its checks. Packet 3 claims are recorded separately above. Merging remains
   with the owner's independent checker.

## Reconciled machine totals

```json
@@SUMMARY@@
```

## Cohort role hypotheses

All cohort labels are HYPOTHESIS. Individual named source leads, where justified,
are annotated separately in `STATUS.csv` and `REFERENCES.md`.

@@COHORTS@@

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

The D10 handoff is intentionally unchanged from the published source-bearing
revision: its local-match and pending-CI wording predates this completed CI proof.
A full-file update was blocked because of inherited private metadata outside D10,
so only the independent allowed ledger, README and receipt files are updated.
The CI result and verified-body totals here and in STATUS.csv are authoritative.
No inherited handoff sections were removed or republished in this receipt update.

## Second published cut: 31 additional peer-reviewed matches

The second cut contains 31 additional independently reviewed strict matches /
1,728 B and 17 complete nonmatches / 1,156 B from 48 attempts. Its C source hashes
exactly preserve six frozen peer-reviewed packets. These new matches remain
`claimed` pending exact-head CI; the first 42 matches retain their completed
source-head CI proof. See `wave2/README.md`, `wave2/reviewed_sources.json` and
`wave2/verification.json`. Later successor claims are metadata only here; their
candidates are excluded from this frozen cut. D10 is unchanged as documented above.

## Third frozen source cut and unique totals

The first two published cuts have 73 new CI-verified matching bodies / 3,060 B,
plus the existing 12-byte getter. Third cut adds 30 peer-reviewed local strict
matches / 1,864 B and 7 complete nonmatches / 624 B. If its exact-head CI passes,
the unique new matching total becomes 103 bodies / 4,924 B. No source is counted
twice. See wave3/README.md and the exact source manifests; later candidates are
excluded. The prior #62 CI receipt is now carried in wave2/ci.json. D10 stays
unchanged; authoritative ledger/receipts remain in the allowed cloud files.

## Fourth cut: 21 more bodies and one table-proof gap

Prior published cuts have 103 new exact-CI-verified bodies / 4,924 B. This cut
adds 21 peer-reviewed local strict matches / 2,012 B, pending its own CI. The
union is 124 unique new bodies / 6,936 B plus the historical 12-byte getter.
New research comprises ten complete nonmatches / 1,240 B and one 96-byte
SOURCE-LEAD at 80021548, explicitly needs-rodata-proof: its canonical six-entry
case mapping and local-table relocation are unproved. It is not credited as a
complete native reconstruction or match. See wave4/unique_totals.json. Later
work and source-led rechecks are excluded. D10 remains untouched.

## Fifth cut: 24 additional peer-reviewed bodies

Historical cut summaries above describe their publication-time CI state. The
first four source cuts now have 124 unique new exact-CI-verified bodies / 6,936 B;
wave4/ci.json records #64's successful exact-head Verify run. Fifth cut adds
24 peer-reviewed local strict matches / 3,548 B and two complete nonmatches /
336 B, pending its own exact-head CI. Its unique union is 148 new bodies /
10,484 B plus the existing 12-byte getter. Across 193 distinct attempts, 44
complete nonmatches / 3,760 B and the one 96-byte blocked source lead remain
outside match credit. No already-attempted address is counted twice. See
wave5/README.md and wave5/unique_totals.json. Later sources are excluded from
this frozen cut; atomic successor claims do not confer matching credit. D10 is
unchanged and the allowed ledger/receipts remain authoritative.

## Sixth cut: nine additional bodies and bounded research

The first five source cuts now have 148 unique new exact-CI-verified bodies /
10,484 B. Sixth cut adds nine peer-reviewed strict local bodies / 752 B, pending
its exact-head CI. It also archives six complete nonmatches / 1,984 B. Across
208 unique attempted addresses the proposed union is 157 new matching bodies /
11,236 B, 50 complete nonmatches / 5,744 B, and the earlier 96-byte blocked lead.
The 12-byte getter is separate. Three qualified source-led rechecks add no unique
attempts or matches; the improved signed donor probe does not replace the old
full-width reconstruction. See wave6/README.md and wave6/unique_totals.json.
Prior source-head proof is carried in wave5/ci.json; D10 remains unchanged.

## Seventh cut: eight additional peer-reviewed bodies

The first six source cuts now have 157 unique new exact-CI-verified bodies /
11,236 B. Seventh cut adds eight peer-reviewed strict local bodies / 824 B and
eight complete nonmatches / 940 B. If its exact-head CI passes, the unique new
matching union becomes 165 bodies / 12,060 B, plus the separate 12-byte getter.
Across 224 distinct attempts, 58 complete nonmatches /6,684 B and one 96-byte
blocked source lead remain outside match credit. See wave7/README.md and
wave7/unique_totals.json. Prior proof is in wave6/ci.json; later candidates
are excluded from the frozen cut. D10 remains unchanged.
