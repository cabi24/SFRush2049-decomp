# D10 Packet 1: boot-tail runtime identification and clustering

**Research only.** The 412 assigned functions account for **95,012 B**.
The current ledger records **@@OPEN_FUNCTIONS@@ open functions / @@OPEN_BYTES@@ B**,
**@@NON_C_FUNCTIONS@@ confirmed non-C rows**, **@@PREEXISTING_VERIFIED_FUNCTIONS@@
pre-existing verified bodies / @@PREEXISTING_VERIFIED_BYTES@@ B**, and
**@@NEW_VERIFIED_FUNCTIONS@@ newly verified bodies / @@NEW_VERIFIED_BYTES@@ B**.

Packet 1 itself introduced zero matching bodies or verified bytes. Its initial
verified entry imports the existing 12-byte `func_80010A00` PR #54 receipt, now
present on master; Packet 1 does not locally replay that match or claim a fresh
current-master scorer/CI proof. No cartridge-coverage claim is made.

Base: `b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`, after the owner merged
[#52](https://github.com/cabi24/SFRush2049-decomp/pull/52) then
[#54](https://github.com/cabi24/SFRush2049-decomp/pull/54). Original spec/inventory
commit: `92e91f12`. The spec is read-only. Edit scope is this directory and the
D10 section of `dot_handoff.md`. No runtime-image/farm work, compiler experiment,
scoring, generated-target write, production gate, lock, layout, shared-type or
paused-target investigation was performed.

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
explicit provenance qualification in its row. Packet 2 must run the formal
fresh target/toolchain/replay checks before any new matching claim.

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
5. Stop after research review/publication. Subsequent work needs a fresh Packet 2
   claim and its manifest/toolchain/getter replay checks. Merging remains with
   the owner's independent checker.

## Reconciled machine totals

```json
@@SUMMARY@@
```

## Cohort role hypotheses

All cohort labels are HYPOTHESIS. Individual named source leads, where justified,
are annotated separately in `STATUS.csv` and `REFERENCES.md`.

@@COHORTS@@
