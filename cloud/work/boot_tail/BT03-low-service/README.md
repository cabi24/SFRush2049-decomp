# BT03 low service: five strict matching bodies

Parent activation on 2026-10-03 at 23:58 UTC covered five fresh targets / **876 B**
after checking all central claim records and STATUS at
`20621601707048145031c548cfc5b5f0b7859860`. All five now strictly match at O2.

To preserve files while storage was constrained, this isolated branch
`dot/boot-tail-bt03-low-service` reuses the preceding worktree and stacks on frozen
resource checkpoint `15491f806bed915e5d17651ee0fdf952783b76ac`. The unchanged
master/tool/target reference is `301d9e7552ad4fd7f54a38796db84671e1000d35`.
Only this packet and its five matching files differ from that frozen parent;
prior authored source and receipts remain byte-identical. The integration handoff
contains only this new delta, not another copy of the previous packet.

| Function | Bytes | O2 | O1 control |
|---|---:|---|---|
| 80015228 | 128 | MATCH after one refinement | 31/32 + 2 excess |
| 800162AC | 252 | MATCH after one refinement | 62/63 + 13 excess |
| 80017108 | 172 | first-try MATCH | 33/43 + 3 excess |
| 8001734C | 104 | first-try MATCH | 25/26 + 5 excess |
| 80017644 | 220 | first-try MATCH | 55/55 |

Every source uses `-g0 -O2 -mips2 -G 0 -non_shared`; prescribed O1 controls keep
all other flags fixed. The scorer adds `-Wab,-r4300_mul`. Every selected body has
complete relocated word equality and zero nonzero excess, unresolved symbols,
unverified references, masked relocations or relocation errors. Object text sizes
are 128, 256, 176, 112 and 224 B respectively; only ordinary trailing zero alignment
lies beyond the unchanged native spans. No padding receives matching credit.

## Actual whole-body contracts

`15228` receives a u16 identifier list, one source address and a sample-record
pointer. It maps the address through real `14CAC`, then passes the record pointer
and mapped base to real `1605C`. Only a successful preparation walks the live list
and calls `162AC` for each identifier. A failed preparation does not read the list.
The cursor advances naturally in the call argument and subsequent entries are
read after the call. Native uses a 40-byte frame and four real persistent values.

`162AC` finds the registered sample-table entry for the incoming record pointer,
then searches its 28-byte records for the u16 identifier. The genuine registry
index is retained because the matched record needs the corresponding base.
For zero references it computes base+record offset, constructs a real descriptor
pointer to record+12, and passes the addresses of that local and the data field
to the existing two-input `14CFC` hook. The callback can change the data/reference
fields; references are reloaded and incremented afterward. Nonzero references
increment without that hook. It returns one even when the identifier is absent.
The native frame is 40 bytes, with a meaningful pointer local, not a keeper.

The sample record has id +0, references +2, offset +4, data pointer +8 and a
16-byte descriptor at +12. Registry entries are 12 bytes on N64: records pointer,
base pointer, u16 count and an unknown final halfword. These pointer-containing
layouts are proved on the 32-bit compiler/native target, not by host sizeof.
The paired caller establishes membership: canonical `1605C` returns success for
an existing or newly registered record array, and `15228` gates `162AC` on that
result. Direct use with an unregistered table, bad offset or missing terminator
is outside the claimed valid-input contract; no new guard changes native behavior.

`17108` clears six real global counters, zeroes both u16 fields in all 512 packed
four-byte resource-range records, and calls existing no-input `14CF4`. The packed
representation is supported by native byte-store lowering and the already
observed range count/first-index fields. Its O2 loop naturally unrolls four records
per iteration and uses the native 24-byte frame. No manual unrolling or fake calls.

`1734C` traverses the two linked lists at context +0xF78 and +0xF7C. Each native
node has next/previous pointers at +0/+4 and a full identifier at +8. It calls real
`1FA18` and reads the current node's next pointer afterward; the second head is
also read only after the first traversal. Call results are ignored. This partial
context view adds genuine native pointer evidence at the previously opaque F78
field, while keeping earlier files frozen. Pointer offsets are N64 evidence,
not assertions about the host's wider-pointer layout. Lists must remain allocated
and finite across callbacks. The complete 32-byte native frame is reproduced.

`17644` scans eight 4,088-byte records. It skips any nonzero inactive byte at +4033,
compares stored id +0 with the input's low 31 bits, and returns the first matching
index with the input high bit retained. No match returns all ones. The full
pointer-free record view is taken from peer-reviewed routing checkpoint
`78469c93` (`175B4`), and remains compatible with earlier field observations.
Its naturally unrolled four-record O2 loop is frameless. Host assertions also
confirm this pointer-free stride and the id/inactive offsets.

All actual external helpers and relevant predecessor structures were inspected
read-only. `abi.json` records canonical helper hashes and limits. The source uses
real pointer/scalar ABI slots only; no helper body is inserted or altered. Names
are descriptive, not claims of exact original source identity.

## Diagnosis and directed controls

The initial `15228` source had four differing words: cursor advancement occurred
after the helper instead of in the native call setup. After workbench diagnosis,
the ordinary `*ids++` argument closes all four differences. Two body forms total.

The initial `162AC` for-loop with an inner break had 56/63 differences, including
a shorter pointer-induction loop. Workbench diagnosis preceded refinement. A
natural combined search condition, retaining the same meaningful index, gives
native indexed traversal and exact equality. The explicit unsigned comparison
records the observed count comparison without changing a formal or adding state.
Two body forms total. No further variants were attempted once strict matches held.

The other three bodies match first form. `verification.json` contains fourteen
hash-bound O2/O1 rows: five retained bodies and the two archived initial controls.
`diagnosis.json` stores metadata only from fully relocated temporary objects.
No raw native listings, objects or ROM bytes are archived; no boundary/scorer,
flag sweep, artificial local, padding, forced register, volatile trick or dummy
callee was used.

## Tests and bounds

Fresh setup/manifests, all 439 starts/sizes totaling 99,120 B, and the existing
getter passed before candidates. Six strict C89 ASan/UBSan tests execute the
actual source in **911 bounded cases**:

- 36 preparation/registration list cases, including failure without reading ids;
- 300 registered sample/reference cases, including hook mutations and u16 wrap;
- 3 complete packed-table resets observed by the final hook;
- 32 two-list cases, including live next/head changes;
- 540 eight-record lookup cases, including duplicates, inactive values and bit31.

Source hashes and proof fields are checked separately. Tests preserve callback
ordering and memory effects under explicit synthetic contracts; they do not
reconstruct helper algorithms or guarantee malformed-data safety. LeakSanitizer
alone is disabled because ptrace prevents it here; no candidate/harness heap
allocation occurs. See `host_verification.json` for exact hashes and layout limits.

```sh
python3 cloud/work/boot_tail/BT03-low-service/verify.py --check
python3 -m unittest discover -s cloud/work/boot_tail/BT03-low-service -p 'test_*.py' -v
python3 tools/cloud/check_submissions.py --base 15491f80 --head HEAD
```

No central STATUS/D10 or prior packet edits; no shared-header, target/scorer,
symbol/layout/lock, production gate, runtime-image/farm or unrelated helper changes.
The 631 existing cloud regression tests pass with zero skips.
Independent BT03-high review passed exact source commit
`ac88b78f20f150ec27bddb4865029359b6687088`, tree
`64b0ee8972fef7a7a9ea9d095d67e4a135870897`, for this delta from `15491f80`.
The reviewer replayed all fourteen rows from immutable Git-blob source copies,
reran all six sanitizer groups, and audited actual descriptor/hook arguments,
registry membership, packed reset and live-list behavior. `REVIEW.json` records
that source-bound PASS. Aggregate publication and exact-head CI remain lead-owned. Local
matching bytes are not cartridge coverage or maintainer acceptance; merging
remains checker-owned.
