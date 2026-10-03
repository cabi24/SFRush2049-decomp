# D10 Packet 4: remaining C13 controller helpers

Claim activated centrally at `b8762cc8`: 80021548/96 B, 800215A8/180,
8002165C/152, 80021700/100, 80021764/128, 800217E4/96, 80021844/136;
7 functions / 888 B. Branch `dot/boot-tail-p4-c13-tail`, exact fresh-master
base `301d9e7552ad4fd7f54a38796db84671e1000d35`.

Own only this directory and these seven matching filenames. All prior C13
sources remain frozen; the central lead alone edits the ledger/D10/claims.
Fresh setup, target checksums, 439-function start/size census and existing-getter
strict replay precede experiments. No target/scorer/extent/type-header changes,
ROM/image/farm work, private-input reads or promotion.

Stop on scope, target, ABI or local-rodata proof failure. The native 21548
translator uses a six-entry jump table; any unresolved local rodata is explicitly
blocked. Other functions remain independent single-function candidates.
O2 then O1, diagnose before directed variants, at most 20 per hypothesis. Do not
repeat the previously frozen narrow-formal declaration sweeps without new evidence.

## Frozen result

**One strict local MATCH / 100 B**, `func_80021700`, first natural source.
**Five complete NONMATCHs / 692 B** and **one SOURCE-LEAD blocked on jump-table
proof / 96 B** are research only. All seven claimed targets account for 888 B.
Independent source/ABI/strict-replay review passed at `f6f0664e` (see
`REVIEW.json`); exact aggregate-head CI is pending. No cartridge
coverage, maintainer acceptance or promotion is claimed.

| Function | Bytes | Retained result | Other prescribed control |
|---|---:|---|---|
| `80021700` | 100 | O2 MATCH, 25/25 words | O1 24/25 different + 2 nonzero excess words |
| `80021548` | 96 | O1 15/24, **2 unverified rodata relocations** | O2 24/24 + 1 excess, same proof blocker |
| `800215A8` | 180 | O2 28/45 + 1 excess | O1 43/45 + 5 excess |
| `8002165C` | 152 | O2 35/38 + 1 excess | O1 34/38 + 2 excess |
| `80021764` | 128 | O2 14/32 | O1 31/32 + 7 excess |
| `800217E4` | 96 | O2 7/24 | O1 22/24 + 9 excess |
| `80021844` | 136 | O2 14/34 | O1 33/34 + 12 excess |

Flags are `-g0 -O2/-O1 -mips2 -G 0 -non_shared`; the scorer automatically adds
`-Wab,-r4300_mul`. The accepted local body has 100 native bytes in 112 aligned
text bytes; three trailing zero alignment words are not native function bytes.
All 25 native words compare exactly after relocation, with zero masks, unresolved
symbols, unverified references, relocation errors or nonzero excess. Its O2
branch-likely checks and packed accesses fit the native leaf. O1 differs in
normalization/scheduling and body length. No cluster-wide flag identity is asserted.

## Matching body and shared packed layout

`21700` accepts one genuine 32-bit handle. It rejects all-ones, selects one of
`D_8004BEB8`'s records by the low byte, checks the full stored ID, ORs bit 3 into
the record's flag word and returns zero. All-ones handles and stored-ID mismatches return -1 without
writing. The actual global array bound is not inferred here. Native address arithmetic independently proves a **416-byte record
stride**, the ID word at +96 and flag word at +36. Unaligned paired loads/stores
prove the packed access contract. There are no helper calls or local rodata.

The six pointer-using sources use one consistent self-contained packed
`VoiceState` model: flags +36, channel +74, set +75, ID +96, signed LFO halfwords
+368/+380 and total stride 416. Unknown arrays represent genuine unmodeled object
ranges, not stack padding or invented live variables. Only the global-array
member needs the full stride. This does not recover the original complete type
or semantic field names; no shared header is modified.

The two extended-control callers preserve their real external callees:
`215A8` reads a signed packed LFO for translated selectors 160/161, computes
low16(value*2 + 8192), or calls `20A04` with the original selector and channel/set.
`2165C` clamps a genuine signed-halfword input to 0..16383, skips translated
selectors 160/161, and otherwise calls `206AC` if the channel is valid. Real
helper declarations appear alone, without inserted bodies. Multiplication by
two avoids undefined left-shift behavior on negative signed host inputs.

The three cache functions gate on valid ID/channel, derive the low-byte voice
index, and select effects cache `D_800561E0[index]` for set 255, or
`D_80056160[set][channel]` otherwise. `21764` tests ownership, `217E4` installs the
index, and `21844` clears to 255 only if that index still owns the cache entry.
The guard against clearing a replacement owner is retained. These five complete
bodies remain NONMATCHs despite their coherent native-operation reconstruction.

## Jump-table blocker: source lead, not established native mapping

`21548` has a six-case native jump table addressed at `0x8002D930`. Its code
contains the six candidate return constants, but the canonical repository
inventory/extents/symbols do **not** supply the six case-to-block entries.
The source candidate uses the licensed MusyX mapping for inputs 128..133;
that correspondence remains a **SOURCE-LEAD**, not proof of the native table.

Both prescribed compiler levels emit local `.rodata` relocations that the
unchanged scorer cannot verify. This member stopped at **needs-rodata-proof**;
no flag search, table substitution, target/scorer change, forced register,
ROM/data read or partial-match credit was used. The maintainer must provide
canonical table-entry provenance and a supported local-rodata proof path before
this can become a verified body. A host test of the candidate mapping proves
only that candidate's behavior.

## Diagnosis and bounded source refinements

All ordinary nonmatches ran `tools/workbench.py diagnose` before refinement,
using temporary native and fully relocated candidate objects. `diagnosis.json`
retains only verdict/frame/ownership metadata. Raw reports, words and objects
are not committed. Caller frames agree at 24 and 32 bytes; the three cache leaves
are frameless. The tool found structural/coalescing mixtures without a proven
lever. Only `217E4` had a heuristic CFE-spelling attribution, not a certainty.

Native caller arithmetic narrows results in-place, while current IDO output
inserts temporary-result copies. This is the same entry/narrowing plateau seen
in already frozen C13/BT03 research. The two callers were not subjected to more
unsupported declaration sweeps.

Directed cache controls after diagnosis:

- A meaningful full-width index followed by its essential low-byte mask was
  tried on all three leaves. It improved `217E4` from 8/24 to 7/24 and is retained;
  it worsened `21764` and `21844` and is rejected there. The mask is required to
  extract a slot from the full handle; it is not redundant keeper code.
- `217E4` also tried binding the full handle once before the guards, and a
  signed sentinel-bearing ID field. Both failed to improve; the unsigned shared
  layout is retained. No extra formal or fake live value was introduced.
- `21764`'s native conditional-return shape motivated explicit success returns
  rather than a boolean-expression result. This improved 17/32 to 14/32 and is
  retained. The residual still includes the native copy/coalescing placement.
- `21844` retained its original complete source after the one worsening control.

At most four natural source forms were tried for any member, well under 20.
Original and directed source controls remain under `variants/`, and all receive
both flag-level receipts in `scores.json`. Next work requires authentic narrowing/
coalescing context or the canonical table proof, rather than repeated spelling
sweeps. No padding local, dummy call, fake formal, assembler block or register
forcing is present in submitted C.

## Reference leads and tests

Pinned [AxioDL/musyx snd_midictrl.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_midictrl.c)
([CC0-1.0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE))
contains `inpTranslateExCtrl`, `inpGetExCtrl` and `inpSetExCtrl` family leads.
Its newer PC/Dolphin layout/version is not asserted to be this N64 source.
No source translation unit, header, data table or raw native dump is vendored.

Reproduce:

- `python3 cloud/work/boot_tail/C13-tail/verify.py --check`
- `python3 -m unittest discover -s cloud/work/boot_tail/C13-tail -p 'test_*.py' -v`

Six focused tests compile every final source under host C89/pedantic/Werror,
assert the fixed-width packed offsets/stride, exercise stale/invalid handle
writes, all byte inputs of the **reference mapping candidate**, signed LFO
boundaries, clamps, and cache install/query/conditional-release ownership.
Caller tests use a test-only external translator contract with independently
chosen return values, so they do not assume the unproved native jump-table
mapping. Other external helper stubs exist only in that test harness. Host tests
award no native matching credit. `scores.json` binds all 32 final/control
comparisons to exact source/target/compiler/tool hashes and strict outcomes.

`status_delta.json` is for the central sole writer. This packet changes only
its owned directory and one new matching source. All prior C13 files stay
frozen. The central lead owns aggregate publication and exact-head CI; merging
remains with the independent checker.

Final existing cloud setup/guard/submission/integrity/scorer regression: **631
passed**, zero skips. The review/receipt commit changes no C source or scoring
input.
