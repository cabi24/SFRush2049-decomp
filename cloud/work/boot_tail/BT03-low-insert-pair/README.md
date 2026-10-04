# BT03 low insertion pair: complete bounded nonmatches

Two exclusively assigned targets / 1,084 B: 80015D68 / 448 B, ending at15F28,
and8001671C / 636 B, ending at16998. Parent activation was persisted at central
`ca010641157fc3e230de206d7707245a240bf805`. Branch
`dot/boot-tail-bt03-low-insert-pair` stacks on frozen mixer receipt
`469eff3b6a6d0ee642fffe011daf8075d3599655`; master anchor remains
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Only this packet directory changes.
Earlier source/receipts, shared ledgers, targets, tooling and helpers stay frozen.

## Outcomes and bounded controls

**Zero matching credit.** Both retained bodies are complete C research:

| Function | Bytes | Retained O2 | O1 control | Native/candidate O2 frame |
| --- | ---: | --- | --- | --- |
| 80015D68 | 448 | 62/112 differing +3 nonzero excess | 107/112 +32 | 40 / 48 B |
| 8001671C | 636 | 133/159 differing +1 nonzero excess | 158/159 +56 | 24 / 24 B |

Object text lengths are464 and656 B respectively; the complete fixed native
extents remain448 and636 B. All retained O2 references are fully relocated with
zero masks, unresolved symbols, unverified references or relocation errors.
Ordinary zero alignment words are recorded separately from nonzero excess;
there is no altered boundary or truncated comparison.

All22 retained/control rows use O2 then O1 with fixed
`-g0 -mips2 -G 0 -non_shared` and automatic `-Wab,-r4300_mul`. There are three
distinct forms for15D68 and six for1671C; the archive additionally duplicates
initial/retained checkpoints to bind diagnosis. `verification.json` records
all source/input/compiler/target hashes, lengths and rejection fields.

- Initial15D68 gives62/112+3. Its packed whole-record assignment expands to
  unaligned transfers and a48-byte frame, while native uses aligned whole-word
  copying and40 bytes. A genuine union word-transfer view worsens the body to
  107/112 (object shorter than target); a memcpy control is112/112+14 and adds
  a non-native external copy call. The original whole-struct form is retained.
- Initial1671C is143/159+33. A value-bearing assignment of the actual stored
  first index reaches125/159+29. An actual range-pointer iterator over all512
  buckets reaches133/159+1 and is retained for its much smaller whole-body
  excess, despite the higher positional difference count. This is an explicit
  tradeoff, not a claim that133 is fewer than125. A cached real global-total
  value regresses to146/159+55. Union and memcpy controls also fail to match.
- The union experiment overlays the genuine packed pointer/id/reference fields
  with the actual two-word transfer representation. Both words are read/written;
  no dummy member is added solely as a keeper. It preserves the native8-byte
  object under the verified32-bit pointer ABI, but changes field-access
  alignment/allocation and is rejected. Because its explicit two-word copy
  does not describe a64-bit host pointer-containing record, it is tested only
  as a fully relocated IDO/native object, not claimed host-portable.
- The memcpy controls are rejected, introduce an extra call and are not
  substituted into the retained body. No new helper body is authored or claimed
  as proved. All experiments stop here; no forced registers, artificial local,
  padding, fake formal, inline assembly or declaration sweep follows.

`diagnosis.json` contains metadata from initial and retained fully relocated
objects. Frame/classifier suggestions are heuristic, not measured ownership.
No raw diagnostic assembly or object is published.

## Genuine interfaces, layouts and order

Both functions have the real signature `int(u16 id, void *payload)` and exactly
two actual no-input calls:80014594 and800145DC. Their entire canonical bodies
were inspected: they operate a nesting counter and queue synchronization; no
input argument is consumed. The insertion functions ignore incidental return
register contents. Their resource callers14F80 and14F14 were previously strict
matches and corroborate key/payload slots. No callee source is changed.

Native `ResourceEntry` is8 bytes: a real payload pointer at+0, unsigned-short
key at+4 and unsigned-short reference count at+6. Field access is packed; the
array base and8-byte stride make the native backward two-word copies aligned.
Host pointer width is different: retained packed records are12 bytes on this
64-bit host. Host tests compare field/algorithm semantics with real host
pointers, never assert the host record is the N64 object. Temporary IDO
assertions and complete native/relocated execution prove native geometry.

**15D68** uses signed countD_80038608 and2048 entries atD_80038610. After entering
the real synchronization helper, it finds the first key greater than or equal
to the request. A duplicate calls the exit helper **before** rereading and
incrementing the selected entry's reference count, then returns0. A missing
key at capacity returns0 after the exit helper. Otherwise it shifts whole
records backward, increments the live count, installs payload/key/refcount1,
calls the exit helper and returns1. Returning0 therefore does not always mean
that no state changed.

**1671C** uses signed totalD_8003DA20,512 packed4-byte ranges atD_8003DA28
(count+0, first+2), and2048 entries atD_8003E228. The512-range endpoint equals
the independently addressed entry-array start. Bucket index is `id >> 6`.
An empty bucket writes its first index from the total **before** the capacity
check, so a full-table failure can still update this metadata. A nonempty
bucket searches only its contiguous slice. A duplicate increments references
**before** the exit helper and returns0. Successful insertion adjusts every
range whose first index is greater than the selected bucket's original first,
shifts whole entries backward, installs the new record, increments the bucket
count and total, exits synchronization and returns1. The threshold is the
original bucket start, not the insertion position.

All mutable reads/stores preserve these actual helper boundaries. No snapshot
of a field is moved across a call unless the native body does so.

## Independent semantic oracles and limits

Four test groups pass:

- **528** canonical whole-native runs agree byte-for-byte with an independent
  list-based reference, including all synthetic table storage and range metadata:
 168 flat-list cases and360 bucket cases.
- **528** retained host-C89 calls agree with native outcomes under ASan and
  UBSan. Complete field-normalized table fingerprints preserve pointer identity,
  inactive records, reference wrap and copy direction. Synthetic pointer words
  map to valid host-array addresses; no fabricated pointer is dereferenced.
- **1,056** freshly fully relocated IDO executions, covering both retained
  bodies and both rejected union-word controls on the same fixtures, agree with
  native state, return and helper order. The decoder enforces O32 caller-save
  clobbers, callee-save/stack restoration and no uninitialized stack reads.
- Native width/offset assertions and every recorded source hash pass. The test
  decoder loads canonical hash-verified words at runtime, supports the required
  big-endian unaligned word transfers and delay/likely branches, and rejects
  unsupported instructions/callees. It is a limited test interpreter, not an
  emulator or a proof of all possible states.

Fixtures cover empty/small/almost-full/full tables, sorted insertion at different
positions, duplicates with reference65535 wrapping to0, complete reverse shifts,
all512 range updates, an empty bucket's full-table metadata side effect and
helper-boundary mutation. The entry helper can reset the synthetic state before
body reads. The exit helper can change the selected reference count to500:
15D68 must finish with501 on duplicate, whereas1671C must leave500. These are
explicit test contracts, not replacement implementations of synchronization.

Required domain: configured totals0..2048; valid sorted active entries; consistent
nonoverlapping contiguous bucket slices; total/range counts and indices within
allocated capacity; bucket keys below32768 (`id >> 6` below512); valid payload
pointer representations that are stored but never dereferenced by these bodies.
The flat list accepts all u16 keys. Only valid helper mutations preserving those
bounds are tested. No concurrency safety, malformed-input safety, original
resource contents, whole-game integration or cartridge claim is made.

The retained sources use the project's established `#pragma pack(1)` for actual
external packed fields, with packing reset after declarations. No fake padding
or shared header is introduced. Host tests compile C89 with pedantic-errors,
Wall/Wextra/Werror and sanitizers. Only leak detection is disabled for executor
ptrace compatibility; candidate/C-harness code does not allocate heap storage.

## Reproduction and gates

Fresh setup, all target checksums and439 canonical extents /99,120 B agree;
the existing12-byte getter strictly matches. All631 existing cloud setup, guard,
submission, integrity and scorer regressions pass.

```sh
python3 cloud/work/boot_tail/BT03-low-insert-pair/preflight.py
python3 cloud/work/boot_tail/BT03-low-insert-pair/verify.py --check
python3 cloud/work/boot_tail/BT03-low-insert-pair/abi_proof.py --check
python3 cloud/work/boot_tail/BT03-low-insert-pair/test_semantics.py
```

No matching submission is added. Central integration alone updates shared
status/D10 and publishes source-frozen reviewed research. No ROM, raw assembly,
object, runtime-image or farm output is included.

## Independent review

The parent independently approved source commit
`d0e06dce3d3029b54083552badb37b6034f4fea6`, tree
`11bdff51cfb550a4821d54dfa2aaf8cb38992e24`, after reading both full native/C
bodies, actual ABI/layout/domain evidence and reproducing all22 compiler rows,
ABI metadata and four semantic tests. See `REVIEW.json`. This final receipt-only
update changes no source or test-model hashes. Protected guard, all161 locks
and whitespace pass; the canonical checker correctly finds zero new matching
submissions.
