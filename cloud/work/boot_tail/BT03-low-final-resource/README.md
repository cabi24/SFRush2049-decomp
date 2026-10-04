# BT03 final low-resource pair

Two fresh bodies / 1,120 B, active at central `c37a1d35`:80015A0C /512 B
[15A0C,15C0C) and80016998 /608 B [16998,16BF8). Branch
`dot/boot-tail-bt03-low-final-resource` explicitly stacks on frozen receipt
`9a69f9fdc69b28e93ee2ed051ea9c075574349ad`; master reference is
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Only this research directory changes;
all previous packets, shared ledgers, helper bodies and protected inputs remain
unchanged. No matching submission is added.

## Frozen results: zero matching credit

| Function | Native bytes | O2 differing words | O2 nonzero excess | O2 object bytes |
| --- | ---: | ---: | ---: | ---: |
| 80015A0C | 512 | 66/128 | 6 | 544 |
| 80016998 | 608 | 106/152 | 4 | 640 |

Both are complete NONMATCH research with native/candidate24-byte frames and
zero masked, unresolved, unverified or erroneous O2 relocations. Full canonical
extents are compared without changed boundaries or clipped excess. Ordinary
zero alignment remains distinct from nonzero excess.

Fixed flags are `-g0 -O2 -mips2 -G 0 -non_shared`, followed by O1 controls, with
canonical automatic `-Wab,-r4300_mul`. Eight hash-bound rows are in
`verification.json`. Selected15A0C O1 is126/128+34. Selected16998 O1 is150/152+15
and has an explicitly rejected unpaired HI16 for D_8003DA20 at text+0x254.
The real cached-count control gives the identical O2 object; its O1 is149/152+2
without that error, still not a match. This control is not hidden or substituted
for the retained source's failing O1 result.

Only one source form is attempted for15A0C and two for16998. The first body
agrees until the reverse packed-record copy expansion and its dependent branch
lengths; the second has the same packed-copy expansion plus allocation
mismatches. The one new count-value control represents the actual bucket count
used throughout the search and makes no O2 change. It is archived; the simpler
original body is retained. Previously rejected union/memcpy controls are not repeated. No artificial
alignment, declaration or narrow-formal sweeps are attempted. Initial
whole-body workbench metadata and source hashes are in `diagnosis.json`.
There are no forced registers, keepers, fake formals, padding locals or assembly.

## Actual ABI and native objects

**15A0C:** `int(u16 id, void *payload, u16 parameter)`. The canonical15058 wrapper
previously matched strictly with these three genuine arguments: current key,
resource payload at+12 and the halfword at+10. The entire callee reads all three
saved argument slots after the entry helper. Its signed count isD_8003CE18,
with256 packed12-byte records atD_8003CE20:

- payload pointer+0
- unsigned-short key+4
- genuine unsigned-short parameter+6
- unsigned-short references+8
- two unknown, unmodified bytes+10..11

The tail is real external object storage proved by the12-byte stride and full
three-word copies, not stack padding. Insertion leaves the selected slot's tail
unchanged while preserving it during whole-record shifts.

After entering synchronization, the body finds the first key greater than or
equal to the requested key. A duplicate increments its reference count before
the exit helper, leaves its existing payload/parameter/tail alone and returns0.
A missing key at capacity exits and returns0. Otherwise it shifts complete
records backward, increments the total, writes payload/key/parameter/refcount1,
exits and returns1. Unsigned-short reference increments wrap modulo65536.

**16998:** `int(u16 id)`, explicitly returning0 on every path. The previously
matched150C8 wrapper supplies its one genuine key; an ignored return does not
make the callee a void function. It uses signed totalD_8003DA20,512 packed4-byte
ranges atD_8003DA28 (count+0, first+2), and2048 packed8-byte records atD_8003E228
(payload0, key4, references6). Bucket index is `id >> 6`.

The complete body searches only the selected bucket. On a hit it decrements
references modulo65536; an initial zero becomes65535, so no removal occurs.
Only a result of zero compacts all subsequent records forward, adjusts every
range whose first index is greater than the selected bucket's original first,
and decrements the bucket count and total. The threshold is the bucket start,
not the removed element's index. The final inactive slot is not cleared. Every
path calls the exit helper exactly once.

Both bodies use only actual no-input synchronization wrappers14594 and145DC.
Their complete canonical bodies were inspected: nesting-count and queue
operations, no consumed input argument. These functions ignore any incidental
return-register values. Helpers, callbacks or runtime bodies are not authored
or modified. `abi_proof.py` binds exact addresses, layouts and relocations.

## Three independent semantic views

Four test groups pass:

- **1,032** complete canonical native executions agree byte-for-byte with an
  independent list model:672 insertion and360 removal cases.
- **1,032** retained host-C89 calls agree with those native outcomes, using
  ASan/UBSan and complete field-normalized state fingerprints. Real host pointers
  are mapped to synthetic native pointer identities; they are not dereferenced.
- **1,392** freshly fully relocated IDO O2 runs (both retained sources plus
  the removal count control) agree with native return values, complete state and
  helper ordering. The interpreter checks O32 caller-save clobbers, callee-save
  restoration, stack restoration and absence of uninitialized stack reads.
- Temporary IDO width/offset checks and every source hash pass. The interpreter
  reads already hash-verified canonical targets or freshly relocated objects;
  it embeds no native words and rejects unsupported instructions/callees.

Fixtures include empty/small/almost-full/full tables, sorted insertion positions,
all four parameter values0/1/0x1234/65535, duplicates, both preserved tail bytes,
reference values0/1/2/65535, forward/backward full-record shifts, all512 range
updates and helper-boundary effects. The synthetic entry helper may reset valid
state before body reads; the exit helper may replace a selected live reference
count with500. These are test contracts for observable call boundaries, not
replacement synchronization implementations or concurrency guarantees.

Required domain: configured insertion count0..256 and sorted active records;
removal count0..2048 with valid consistent contiguous bucket slices; removal
key below32768, so its bucket index is below512. The insertion key and third
parameter may be any u16 values. Payload pointers are stored but never
dereferenced by either body. Mutable helper effects must preserve the stated
allocation/list bounds. No malformed-input, original-resource-content,
concurrency, whole-game or cartridge claim is made.

Native pointer width is4: records are12 and8 bytes respectively. The64-bit host
uses16- and12-byte packed records; host field semantics are deliberately separate
from native geometry. Native IDO assertions and whole-object instruction replay
prove the target widths and offsets. The two unknown tail bytes are checked as
preserved data, with no guessed interpretation or contents.

Host C tests use C89, pedantic-errors, Wall/Wextra/Werror, ASan and UBSan. Only
leak detection is disabled for executor ptrace compatibility; candidate/C-harness
code does not allocate heap storage. Packing is the project's existing
`#pragma pack(1)` convention for true external packed objects and is reset after
declarations. No shared header or scorer is changed.

## Reproduction

Fresh setup, all checksum manifests and439 extents /99,120 B agree; the
existing12-byte getter strictly matches. All631 existing cloud setup, guard,
submission, integrity and scorer regressions pass.

```sh
python3 cloud/work/boot_tail/BT03-low-final-resource/preflight.py
python3 cloud/work/boot_tail/BT03-low-final-resource/verify.py --check
python3 cloud/work/boot_tail/BT03-low-final-resource/abi_proof.py --check
python3 cloud/work/boot_tail/BT03-low-final-resource/test_semantics.py
```

The pinned IDO5.3 setup is required for all replay/probe commands; a host C
compiler with ASan/UBSan is required for the semantic harness. Compiler hashes,
source hashes, target hashes, exact results and limits are preserved in the
receipts. No ROM, raw assembly, object, credentials, runtime-image or farm data
is published. Central integration alone owns STATUS/D10 and draft publication.

## Independent review and gates

The parent approved source commit
`07182553efde7004abae48a4ab8c44d424dce999`, tree
`6d06412a9c38b6b8084321b7907bc6f25d4ede44`, after reading both full native/C
bodies and the actual ABI/layout/domain, then reproducing all eight compiler
rows, ABI proof and four semantic tests. See `REVIEW.json`. This receipt-only
update preserves every source/model hash. Protected guard, all161 static locks
and whitespace pass; the canonical checker finds zero matching submissions,
as required for two complete NONMATCHs.
