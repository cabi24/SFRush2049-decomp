# BT03 low larger: record insertion and descriptor registration

One standalone strict local match, **332 B**, plus one complete 440-byte
nonmatch. This is source-matching evidence, not cartridge coverage.
Independent review and exact aggregate-head CI are pending at source freeze.

- Base: `301d9e7552ad4fd7f54a38796db84671e1000d35`, freshly fetched master.
- Branch: `dot/boot-tail-bt03-low-larger`.
- Owner: `/root/match_boot_tail_low_larger`.
- Exact claim: `80015720–800158D8` and `800164D0–8001661C`, 772 B total.
  The parent activated both after the fresh central open/unclaimed scan.
- Only this packet and its one matching source are changed. Central STATUS,
  claims and D10 remain coordinator-owned. No individual PR is opened.

| Function | Bytes | Native-backed behavior | Final O2 | Final O1 |
|---|---:|---|---:|---:|
| `80015720` | 440 | sorted insertion or reference increment | 6/110 | 108/110 + 44 excess |
| `800164D0` | 332 | unique descriptor-table registration and state initialization | MATCH | 82/83 + 25 excess |

Names describe observed behavior; no original API names or third-party source
are claimed. Neither target was previously attempted or part of the small,
service, lookup, removal, or active registry packets. Their only calls are
already verified queue guards `80014594` and `800145DC`, both argument-free
with ignored return values. There is no callback, local rodata, jump table,
or dependency on the active `1536C/1605C/161A0` registry work.

## Native ABI and representation

`15720` takes a `u16` identifier and one payload pointer. Native caller `14FEC`
passes a loaded halfword and an address eight bytes beyond a returned record;
it ignores the result. The target homes the two actual inputs, reloads the
identifier as a halfword after acquisition, and returns one for a newly
inserted record or zero for an existing/full table. It acquires the guard
before reading live count `D_8003C610`. Records at `D_8003C618` are eight bytes:
32-bit payload at +0, unsigned halfword ID at +4, unsigned reference count at
+6. A sorted search identifies insertion position; existing IDs increment the
reference count modulo 65536, leaving the original payload untouched. New
records are inserted only below capacity 2048, shifting later records backward.
Every path releases exactly once.

`164D0` takes a `u16` identifier, a descriptor pointer, and a `u16` count.
Native caller `15318` narrows its first input, loads the count from input +0,
and passes descriptors at input +4; it forwards the integer result. The table
at `D_80042230` has eight-byte records: ID +0, count +2, pointer +4. Its signed
live count is `D_80042228`. Existing IDs and a pre-acquisition count at least
128 return zero without acquiring. Accepted registration acquires, reloads
live count, writes the new record, initializes byte +9 of each twelve-byte
descriptor to 31, increments the live count, releases, and returns one. The
source preserves the native reload rather than assuming acquisition leaves
the count unchanged. As in the native code, callers must provide valid table
counts and enough descriptor storage; this packet adds no invented bounds
checks or concurrency guarantees.

Both sources describe packed field access and aligned record storage through
a local union. Native indexed halfword field accesses use byte operations,
while shifts copy aligned full words. This extends no shared header. A separate
32-bit C89 compile checks all sizes and offsets. Host behavioral tests use the
host pointer width and are not treated as native layout proof.

## Matching and bounded nonmatch

The final `164D0` body has complete relocated equality over 83 words, including
its 32-byte frame, actual argument homes, counter spill, branch layout and
four-way descriptor loop. The search index is naturally reused for descriptor
initialization, and the descriptor pointer advances in the loop body. This
produced the native allocation and loop unroll. No fabricated local, extra
formal, artificial padding, dummy call, callee body, context, assembly, target
edit or flag sweep was used. The two unknown byte arrays in `Descriptor` denote
actual unrecovered bytes in the twelve-byte native record, not stack storage.

`15720` remains COMPLETE-NONMATCH, with no match credit. Its final O2 frame and
110-word extent agree with native; only six words in the eight-byte record
shift differ. Workbench diagnosis ran on the initial seven-word near-match
before further variants. Reversing the source comparison operands resolved
one branch-order difference. The remaining copy uses the assembler temporary
for the first word and schedules source-pointer decrement differently from
the native two-temporary copy. The differing offsets are `+E0`, `+E8`, `+EC`,
`+F0`, `+F4`, and `+FC`.

Fourteen directed source forms (including final pointer typing) checked union,
packed and aligned struct copies; separate member copies; copy pointers;
explicit reverse pointer walking; a rejected 64-bit storage probe; a standard
memcpy probe; and comparison order. The best natural C89 union source remains
six words away. The non-C89 64-bit probe was rejected and is not retained.
Six forms were checked for `164D0`, including indexed and advancing-pointer
loops and the actual reused index. `controls.json` records outcomes. No more
forms are justified without new evidence for the original record-copy
representation or a measured compiler explanation; blind replay of the same
copy variants is not a useful next step. O1 is clearly inconsistent with both
native bodies' length and scheduling.

## Verification

`verification.json` binds the exact sources and compiled object hashes to
protected target hashes, scorer hash, approved compiler pin, native extents,
and both O2/O1 outcomes. Target SHA256SUMS passes; all 439 extent starts and
sizes agree with the inventory (99,120 B); the existing `80010A00` getter strictly
matches. There are no masks, unresolved symbols, unverified relocations, or
relocation errors in either final body, and each object defines only its own
function. Compiler-added zero alignment words receive no target-byte credit.

`test_semantics.py` includes each final source unchanged. Its 1,412 cases pass
strict C89/O2 and ASan+UBSan/O1, including sorted insertion positions, duplicate
IDs, reference overflow, full tables, record preservation, descriptor count
zero and 65535, scalar/unrolled loop remainders, rejected calls without guard
traffic, and a guard that changes live count to prove the post-acquire reload.
LeakSanitizer is disabled because this environment uses ptrace; no leak check
is claimed. Test mocks are not matching-source context.

Reproduce from repository root with `IDO_DIR` set to the approved shared pin:

```sh
(cd asm/us/boot_tail && sha256sum -c SHA256SUMS)
python3 cloud/work/boot_tail/BT03-low-larger/verify.py
python3 cloud/work/boot_tail/BT03-low-larger/test_semantics.py
python3 tools/cloud/check_submissions.py --base 301d9e75 --head HEAD
```

One sparse worktree and private temporary outputs were used. The pinned
compiler and binutils were reused without copying or changing shared stores.
No active-directory deletion was performed. Raw native instructions, object
files, compiler intermediates and rejected sources remain private; no ROM,
raw assembly, credentials or unrelated data are published. Promotion, merging,
layout/lock changes, runtime-image work and production ROM gates remain outside
this packet.
