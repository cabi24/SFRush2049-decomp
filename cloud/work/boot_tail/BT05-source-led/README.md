# BT05 source-led residual research

This is a narrowly bounded reopen of three **already attempted** functions, totaling 356 bytes. It adds **zero unique targets, zero verified bodies and zero verified bytes** at this stage. Earlier source and nonmatch archives are immutable.

- Branch `dot/boot-tail-bt05-source-led`, clean master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central activation `1022b34f`: `800223D0` (108), `80022514` (108), `80023190` (140).
- Initial authorization: exactly one donor-derived natural source control per target at O2, followed by O1. Unchanged/worse targets stop; extending an improved target requires fresh diagnosis and central acknowledgment.
- Protected target/callee ABI and native extents are unchanged. Candidate C stays under this research folder unless a later full strict proof justifies a submission.

## New primary-source input

The pinned public [synthmacros.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c) in AxioDL/musyx, revision `78d2e16e4905fc675952162d331c24d5198b2687`, has named definitions with distinctive operations matching the three native roles:

- lines 365–381: `mcmdAddAgeCounter`, source-family lead for `800223D0`;
- lines 404–412: `mcmdAddPriority`, source-family lead for `80022514`;
- lines 911–921: `mcmdSetPianoPanning`, source-family lead for `80023190`.

The connected GitHub reader returned full-file blob SHA `a3203f9e0032fa5e9d08fa5e51da80acf4cab0c0` for those pinned ranges. The pinned [LICENSE](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE), blob `0e259d42c996742e9e3cba14c677129b2c1b6311`, explicitly identifies CC0 1.0 Universal. `references.json` records provenance and the adaptations. No complete external source file is vendored.

This is PC/Dolphin, version-dependent public source, not a recovered N64 release. Extra donor hardware-priority calls and extra panning stores are absent from the canonical N64 bodies and are omitted. N64 packed field offsets, genuine state/command inputs, actual state/u8 callee declaration and zero-byte return are preserved. The source-family names are leads, not symbol or layout changes.

## Why this was a distinct controlled probe

Equivalent source expressions do not establish identical compiler lowering. The exact archived controls were inspected before reopening:

- The older `223D0` unsigned-short control declared an unsigned local, assigned a signed conversion, then cast to signed again at use. The donor declares a signed-halfword local and assigns an explicitly unsigned-halfword extraction before signed assignment. The conversion order differs; the control is not a relabeling of that earlier form.
- `22514` already had two signed-halfword locals. Their existence was not new evidence. The donor difference is the explicit unsigned-halfword extraction before signed assignment plus one complete nested conditional expression. Native code sign-extends both the extracted delta and summed result before clamping.
- Earlier `23190` controls split an unsigned mask and signed-byte conversion into assignments, staged multiplication/shift and used an explicit clamp-result local. The donor uses a nested unsigned-byte→signed-byte conversion, a combined arithmetic expression and a complete nested clamp, without the raw-word/limited locals.

The probes change complete source context. No claim is made that a single cast alone caused a result.

## Initial bounded results

| Function | Prior best O2 | Donor-derived O2 | O1 control | Disposition |
|---|---:|---:|---:|---|
| `223D0` | 11/27 | 14/27 | 27/27 +6 extras | Stop; do not replace the better old source |
| `22514` | 23/27 | 23/27 | 27/27 +10 extras | Stop; no matching credit or forced extension |
| `23190` | 18/35 | 8/35 | 34/35 +10 extras | Measured improvement; still NONMATCH |

All comparisons are strict full-word relocated comparisons with the unchanged scorer and mandatory `-Wab,-r4300_mul`. There are no unresolved symbols, unverified relocations or errors. O2/O1 are the two prescribed levels; no additional flags were swept.

The fresh relocated `23190` diagnosis reports exactly eight register sites, no schedule or constant differences, no insertions/deletions and no frame. The entire 35-word native control-flow structure is reproduced. The initial gain extraction now occupies a2, but signed extension uses temporary registers instead of staying in a2; the shifted product likewise passes through a temporary instead of remaining in v1. `diagnosis_summary.json` binds this observation to the donor-control source hash. Temporary objects and raw disassembly are not published.

## Open input-range / full-domain semantic proof

The selected `23190` donor-derived probe uses signed left shift and signed multiplication. It therefore relies on a bounded note domain. The public `curNote` name suggests such a domain, but this packet has not proved that every native writer/caller enforces it. In particular, the probe is **not established as defined C for every possible u16 value at state+0x50**. The prior unsigned low-word reconstruction remains preserved under `BT05-medium/nonmatch/func_80023190.c` and is the full-width arithmetic reference.

The 18→8 word improvement establishes closer compiled instruction/register structure only. It does not establish full-domain source equivalence, a strict match, or permission to promote this donor form. Native input-range proof or a separately authorized full-domain-preserving reconstruction remains open. The same qualification applies to both unsuccessful extension controls. No additional source variant was run to address this note.

## Replay and scope

```sh
python3 cloud/work/boot_tail/BT05-source-led/verify.py
```

`experiments.json` preserves the exact one-control-per-target initial results. `verification.json` binds final source hashes and flags to ten O2/O1 comparison rows (six initial donor rows and four authorized extension rows). A successful replay of research is not a matching verdict. No source is placed under `cloud/matches/` without strict zero, independent source/ABI review and aggregate exact-head CI.

Only this packet directory may change. Earlier archives and all targets, symbol maps, layouts, locks, scorer/compiler, runtime images, farm, specs, production gates and the accepted `800D1248`/restricted helper paths remain unchanged. No ROM, raw instruction dump, object or credential is published.

## Integration decision

Only the improved `23190` nonmatch appears in `status_delta.csv`, changing its research residual from 18 to 8 words without matching credit. `unchanged_targets.json` explicitly preserves the prior better/equal `223D0` and `22514` source selections. All three source-family findings are retained in `references.json`. Central extension `b34484c9` authorized exactly two further `23190` controls after the measured improvement: a signed-byte scale local and separate product/arithmetic-shift statements. Both remain 8/35 at O2; each O1 control is 34/35 plus eleven extras. They are archived under `controls/` with hashes and counts in `extension_controls.json`. The original donor form remains the selected 8/35 research source, and no further extension is attempted.

## Independent review

Paired review independently fetched the pinned CC0 license and three source-family definitions, verified their blob SHAs, and inspected all five candidate/control source bodies. All ten NONMATCH rows and source hashes exactly reproduced, with zero new targets, matching functions or verified bytes. The review explicitly accepts `23190` only as improved compiler-closeness research with the open signed-arithmetic input-range/full-domain proof recorded above. Better/equal older `223D0`/`22514` source selections and the unsigned `23190` reference remain preserved. Receipt: `independent_review.json`. No further variants are requested or authorized by this packet.
