# BT03 low registry: three complete bounded nonmatches

Exclusive claim `edb8cc44` covers three fresh whole bodies / **904 B**. This packet
adds **zero matching bytes**. Branch `dot/boot-tail-bt03-low-registry` explicitly
stacks on frozen service head `b51423528a2971b16f2dcf111b7c96375cabd188` in the
existing isolated worktree; earlier authored files remain unchanged. Master,
targets and tools are still referenced at
`301d9e7552ad4fd7f54a38796db84671e1000d35`.

| Function | Bytes | Retained O2 | Retained O1 |
|---|---:|---|---|
| 8001536C | 312 | 6/78 | 72/78 + 13 nonzero excess, unpaired HI16 |
| 8001605C | 324 | 48/81 + 1 nonzero excess | 81/81 + 34 excess, unpaired HI16 |
| 800161A0 | 268 | 20/67 | 67/67 + 12 excess |

All three O2 bodies have resolved references and zero masks/unverified fields.
None has full relocated equality. O1 relocation failures remain explicitly
recorded and rejected; no scorer or proof rule is changed. Every retained/source
control uses the prescribed O2 then O1 flags with fixed `-g0 -mips2 -G 0
-non_shared` and the scorer's automatic `-Wab,-r4300_mul`.

## Whole-body source and ABI

`1536C` is the resource-push counterpart to the earlier pop dispatcher. It has
exactly five genuine arguments: group-list pointer, u16 requested identifier,
source address, sample-record pointer and resource-table pointer. When enabled
and stack capacity permits, it walks relative group records until the all-ones
sentinel, finding the first matching id. It stores the selected group in the
current stack slot, invokes the five real registration wrappers with the correct
relative payload offsets, optionally calls `15318` for a type-one group, then
rereads and increments the signed stack count. The final callback receives the
original requested identifier even if earlier callbacks change the group id.
Later offsets/type are read live. The six-offset header is 32 bytes; no unused
formal, fake keeper or padding local is added. Native and candidate frames are
both 32 bytes.

`1605C` searches the existing sample registry by pointer. An existing entry
returns one without hooks or record reads. A full eight-entry registry rejects
a new entry. Otherwise it counts the sentinel-terminated 28-byte sample records,
enters the real synchronization helper, installs record/base/count into the live
registry slot, clears exactly the counted reference fields, increments the live
registry count, exits synchronization and returns one. The count and cursor
are real values used across the synchronization call. Native and candidate
frames are both 48 bytes, but their count/pointer stack homes differ (native
count at +38 and pointer +40, candidate +42 and +36), with extra narrow-counter
normalization copies. No padding or invented value is used to move those homes.

`161A0` searches that same registry. Absence returns zero without synchronization.
A found entry enters synchronization, rereads the current count, shifts later
whole records forward over the removed slot, decrements the count, exits and
returns one. The trailing old slot is not fabricated as cleared. A separate
meaningful index names the following record during the shift. Both frames are
32 bytes; return/control and aggregate-copy register/schedule differences remain.

The sample and registry views are identical to the already reviewed `162AC`
service source: native 28-byte sample and 12-byte registry entries. They contain
real pointers, so host sizeof is not native layout proof. IDO compile assertions
confirm those widths. `14594` and `145DC` are actual no-argument synchronization
functions, not stubbed into these files. Registration-wrapper signatures agree
with their reviewed bodies; `15318` is the existing u16/payload wrapper. Helpers
and shared headers remain untouched.

## Bounded controls and stop

Workbench diagnosis preceded refinements and was repeated at improved retained
checkpoints. Reports in `diagnosis.json` contain metadata only from canonical
and fully relocated temporary objects; raw words/listings/objects are not stored.

- `1536C`: introducing a real local list cursor reduces 69/78 to 7/78. A normal
  field-first id comparison closes one commuted compare operand, leaving six
  commuted payload-address ADDU operands at +0x6C/+0x84/+0x98/+0xAC/+0xC0/+0xE4.
  Reversing pointer-add spelling, one unsigned-long address representation, and
  array-index address spelling do not close them. Five source forms total.
- `1605C`: initial source, reordering genuine local declarations, and ordinary
  post-increment spelling all retain 48/81 plus one nonzero excess. Three forms
  total; stop rather than sweep narrow types, scopes or forced homes.
- `161A0`: a separate real following-index local improves 31/67 to 20/67. An
  early return and ordinary whole-record memcpy control fail to improve it.
  Four forms total; the natural aggregate assignment is retained.

The address-word control is **rejected**, not part of the retained source.
Temporary IDO checks prove unsigned-long and pointer width are both four bytes;
host checks require equal widths on LP64 and test bounded in-allocation/no-wrap
resource addresses. This is the tested target address convention, not portable
C89 pointer/integer conversion or recovered original-source identity. The
retained source uses ordinary pointer arithmetic. Semantic commutativity does
not turn the six differing machine words into a strict match.

`verification.json` reproduces thirty rows: three retained sources and twelve
archived controls, each at O2 and O1. Retained forms also appear in their historical
control slot, but never receive duplicate function or byte credit. The next useful
input is authentic original source/compiler context for payload address lowering,
narrow-counter homes and registry aggregate copying. Do not reopen these bounded
forms without new evidence, and do not relax the scorer to remove differences.

## Tests and limits

Fresh setup, all three manifests, 439 starts/sizes totaling 99,120 B and the
existing getter pass before candidates. Five tests compile actual retained C89
with pedantic diagnostics, warnings as errors, ASan and UBSan. They execute **875
retained-source calls**, plus 110 separately labeled rejected-address-control
calls. Cases include missing/present/sentinel groups, disabled/full guards,
live offset/type/id/count changes, existing/new/full sample registries, saved
pre-hook sample counts, callback-time registry growth, reference clearing and
whole-record removal. Native-width compile assertions and source/receipt hashes
are checked. LeakSanitizer alone is disabled under ptrace; candidates/harnesses
do not allocate heap memory.

The domain requires allocated aligned records, valid relative offsets and
terminators, configured nonnegative stack/registry counts, and callback effects
that keep writes within capacity. Synthetic helpers establish caller behavior,
not original helper algorithms or concurrency safety. No malformed-resource,
whole-game, ROM or cartridge proof is claimed. `host_verification.json` records
exact scope and hashes; passing host tests does not confer matching credit.

```sh
python3 cloud/work/boot_tail/BT03-low-registry/verify.py --check
python3 -m unittest discover -s cloud/work/boot_tail/BT03-low-registry -p 'test_*.py' -v
```

Only this owned research directory changes. There are no matching submissions,
central ledger/D10 edits, prior packet changes, protected target/scorer/header/
layout/lock changes, runtime-image/farm work, native dumps, ROM bytes or objects.
The 631 existing cloud regression tests pass with zero skips.
Independent BT03-high review passed exact source commit
`ee8dfe299a8e33a92bd6635aa51dfaebdafb426b`, tree
`8bdcc252d5019da13f6a5e704cbe8b81ac4e4be3`, for this delta from `b5142352`.
The reviewer replayed all thirty rows and five tests from immutable Git-blob
copies, inspected complete native/source bodies and their actual ABI, and
confirmed the qualified address experiment remains rejected. `REVIEW.json`
records that source-bound PASS for complete research only. Aggregate publication
remains lead-owned; merging is reserved
for the owner's independent checker.
