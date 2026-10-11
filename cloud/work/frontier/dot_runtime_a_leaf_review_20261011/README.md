# Independent image-A availability-family review

PASS for the complete candidate bodies at A:80392F28 (220 bytes),
A:803930B8 (264), A:803931C0 (276), and A:803932E4 (168): **928 distinct
new candidate bytes**. Existing A:80393004 (180 bytes) remains exact and earns
no new credit. Accepted-image and cartridge gain remain zero.

The separately authored source lives in
`../dot_runtime_a_callers_deep_20261011/group.c`. The reviewed source hash is
`ab75c2190e76898e7f677e815d230398aac8f6feceaf8910add12a378996019f`.
All seven complete group functions are retained, with the genuine A:80394058
entry as the only keep root. A:8039338C and A:80394058 remain nonmatching.
This review does not establish that those callers or the whole group can be
installed as-is.

## What changed the selector result

The previous standalone 92F28 reconstruction was 2/55, differing only in the
register carrying constant 5. Its function body is **unchanged**. Complete real
caller context makes it 0/55 with stock IDO 5.3 and ordinary O3 whole-group
compilation. The matching producer and mode-dispatcher bodies are also
unchanged from their archived research bodies. The author's cleanup uses the
ordinary signed index over the 23 witnessed records.

The deeper diagnostic answers the earlier reservation question without a
compiler modification. Stock UOPT output marks the standalone selector
external and emits its GP register descriptor for registers 2–7. The real
group marks it internal and emits descriptors for 2–13, 16–23 and 30.
These are directly observed optimizer-to-generator descriptors, not register
roles guessed from the final instruction listing. They establish a change
before UGEN under the authentic context. Retail compiler IR is unavailable;
the review does not fabricate retail reservation metadata or claim a complete
live UGEN allocation trace.

`compiler_evidence.py` rebuilds both pipelines, reads the stock UGEN diagnostic
tree, and proves that diagnostics leave the **whole output object** identical.
The captured explicit pipelines also equal the canonical stock driver/group
outputs. Original intermediates and generated listings remain temporary.

## Independent checks

- Authenticates the protected A manifest, exact extents, complete body hashes,
  asset hash, loader pointer and decompressed image hash.
- Compiles the source afresh with stock IDO. Strict scores are 0/55, 0/45,
  0/66, 0/69 and 0/42, without masked fields, uncertainty or excess words.
- Reads ELF extents independently and re-sections each complete emitted body
  with all relocations. GNU as/ld independently resolve symbols at the native
  addresses. Local text call addends are translated through their ELF function
  owner, never treated as already-resolved native calls.
- Audits every generated call to the five reviewed functions. The three real
  selector calls remain in 931C0 and 94058; the receipt records all 14 family
  callsites. These are source/group ownership checks, not end-to-end execution
  of the nonmatching large callers.
- Executes 7,103 availability cases per route: 2,048 selector cases covering
  every signed-byte input with eight initial sublevel values; 2,274 producer
  cases; and 2,781 dispatcher-family cases. Producer tests cover all five
  witnessed outcome categories in every four-slot combination, empty and
  mixed lists, out-of-range unsigned slot bytes, and seeded random lists.
  Mode tests cover every cyclic start/end pair in all six 13-element rows,
  reset gates, all signed-byte levels and default switch arms.
- Executes 1,364 cleanup cases per route, covering all 23 positions, both
  pointer gates, sentinel handling, signed-16 narrowing boundaries and random
  full-width values. For example, a full-width value of 65535 must invoke the
  signed-16 helper with -1; it must not be mistaken for the full-width -1
  skip sentinel.
- Protected-native and independently GNU-linked routes have identical complete
  register, memory, branch, read, write and external-call traces. Guards and
  permitted write addresses are checked. External fixtures deliberately
  clobber caller-saved registers instead of accidentally preserving them.
- Unchanged extracted C89 function bodies pass the same 8,467 cases on the
  host with UBSan and bounds sanitization. Host pointer layouts model the real
  fields logically; they are not a claim of host/N64 struct ABI identity.
- Executes every selector, initializer, dispatcher and cleanup instruction.
  The producer executes 65/66: its redundant load at +0x94 is unreachable
  under the native branch-likely schedule. Its byte is still checked by both
  complete-body routes.
- Wrong selector boundary, wrong cleanup record count, the standalone source,
  an unknown opcode and an escaped-code execution all fail as expected.

## ABI and limits

92F28 takes no incoming register arguments and preserves t0–t5. The producer
intentionally clobbers s0–s7 and fp without saving them; mode 0 of the dispatcher
inherits those clobbers. Cleanup intentionally clobbers s0–s2, preserving the
remaining saved registers, sp and ra. These private whole-group contracts must
be preserved during integration. Treating every reviewed helper as a standalone
O32 replacement would be wrong.

External helper implementations, real object ownership/lifetime and full
caller execution are not verified here. List fixtures assume finite valid
nodes and nonoverflowing counts; menu-row searches assume a reachable nonzero
entry. The GNU route links extracted complete functions, not the original
contiguous translation unit. No image splice, compression, full-ROM hash,
hardware or gameplay claim is made.

No pressure locals, dummy formals, fabricated caller, synthetic keep root,
volatile trick, instruction encoding, compiler patch, protected-input edit,
or overlay-B expansion was used. No ROM bytes, raw assembly or credentials
are included in this packet.

## Replay

Configure `IDO_DIR` and MIPS GNU binutils, then run from the repository root:

```
python3 cloud/work/frontier/dot_runtime_a_leaf_review_20261011/review.py
python3 cloud/work/frontier/dot_runtime_a_leaf_review_20261011/compiler_evidence.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_family_independent_review.py -q
```

Both scripts support `--group PATH` for a separate author checkout and
`--output PATH` for a fresh receipt. The regression accepts that alternate
group through `RUNTIME_A_FAMILY_GROUP`. Scorer image/cache state is isolated.
All review guards remain active under optimized Python.

Timeline UTC: isolated baseline at 13:35; baseline diagnosis and stock metadata
at 13:36–13:37; independent fresh match confirmation at 13:38; full three-body
semantic review at 13:43; cleanup review and source hash binding at 13:45–13:46.
The receipt reports instrumented replay wall time separately. Independent
review was expanded after the author's real callers resolved the original
selector blocker; no unsupported spelling sweep was substituted.

## Additional E200 peer review

`review_record.py` reviews the separate record-context packet at frozen source
hash `c7ea7c6f1d1b8595882f3b2776579b6226be8d77570c6195c81c67c42128f651`.
Its new A:8039E200 claim is **444 bytes**, separate from the 928-byte family
above. It preserves the existing E3BC/D494/ED20 matches without new credit.

A separate fresh stock build and this review's ELF/GNU resolver reproduce
all 111 E200 words and 15 relocations. The separately authored 6,546-case
native/GNU/host C89 sanitizer suite is rerun, not accepted from its receipt.
The reviewer also adds 2,250 topology cases with an independent ordering
oracle: every permutation of six physical list nodes, sparse physical-node
sets, forward/backward/zero directions, synthetic-tail transitions, selected
records, predicate gates and replacement-object callbacks. These extra cases
execute all 111 instructions, 2,080 predicate calls and 1,550 handle rewrites.
They use the reviewed author decoder with independently constructed memory;
they are not represented as a second independently written instruction decoder
or additional host C89 cases.

The shared 16-byte list-owner interpretation is grounded in the existing
`src/blob/audio_volume_pan.c` use of D_80152020 and the author's MIPS-compiled
layout probes. Inputs remain private s1/s2 and result v0; all three native and
three generated callsites are reviewed, including s5/s6 save/restore and the
incoming sp+4 direction home. Source callback reload behavior is exercised.
D05C and the large parent remain nonmatching and unclaimed.

Run `python3 cloud/work/frontier/dot_runtime_a_leaf_review_20261011/review_record.py`.
It accepts `--group PATH` and `--output PATH`; the regression accepts a separate
record checkout through `RUNTIME_A_RECORD_GROUP`. The peer receipt records
the exact source, helper-script hashes, callsites and remaining limits.
