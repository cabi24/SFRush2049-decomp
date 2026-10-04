# BT03 high runtime follow-on

Claim `e1ed7599` covers six fresh whole bodies / **2,768 B**, branch
`dot/boot-tail-bt03-high-runtime-followon`, from unchanged master source
`301d9e7552ad4fd7f54a38796db84671e1000d35`. The existing sparse worktree is reused;
prior runtime/list head `86b0f12e` remains frozen. Central integration owns shared
status, draft publication and exact-head CI.

## Results and extent proof

| Function | Native bytes | O2 differing words / ELF extent | O1 differing words / ELF extent |
| --- | ---: | --- | --- |
| 80019194 | 476 | MATCH /476 | 119/119 +15 excess /540 |
| 8001EB10 | 464 | MATCH /464 | 113/116 +32 excess /604 |
| 8001D764 | 332 | 21/83 /332 | 82/83 +15 excess /400 |
| 8001C580 | 496 | 33/124 +3 excess /512 | 121/124 +30 excess /636 |
| 8001824C | 508 | 89/127 /488 | 127/127 +63 excess /772 |
| 80018448 | 492 | 87/123 /484 | 123/123 +59 excess /740 |

Two **MATCH** bodies /940 B; four **COMPLETE-NONMATCH** reconstructions /1,828 B
receive no matching credit. `verify.py` requires one correctly named STT_FUNC
at zero, exact native symbol size, full relocated word equality, and no masks,
unknown references, relocation errors or nonzero excess for every accepted row. Symbol size
is distinct from zero section padding. All12 final and14 archived-control rows
record source hashes and ELF extents. Initial O1 HI16 failures remain rejected.

## Actual source, ABI and domains

**19194** takes a real byte, halfword, original word handle and byte flags.
Actual17644 returns FFFFFFFF or validated row0..7 plus bit31. The untagged path
forwards the original handle as the genuine fifth word to1B9F8, then checks64
channel-map entries against the live primary channel and makes the native
five-input calls. Tagged rows are untagged before indexing; flag low-nibble
cases0..3 store byte+4064, clear word+4080 or set flag bits at+4078. Other cases
leave the record unchanged. The full record stride is4,088 bytes, map+1320 and
primary byte+4036. Real map/primary entries must address the configured channel
storage. Tests keep those selectors in0..31 and exercise live callback mutation.
Preserving the original handle in a real local while reusing the incoming
parameter for the translated row naturally closes the native allocation; the
local is consumed as a true helper input, not a keeper or padding value.

**1EB10** invokes genuine one-pointer21844 first, then detaches an identified
voice from its parent/child chain or updates/releases the root sequence node.
It preserves live post-helper reads. A parent receives the child handle; a root
with a child transfers the node's value and ownership; a lone root is removed
from the doubly linked used list and prepended to the free list. It does not
invent clearing of the voice identifier, links or entry pointer afterward.
Native voice stride416, child+16, parent+20, node pointer+24 and identifier+96
are retained; packed nodes are16 bytes. Registered low-byte voice indices,
valid allocated noncyclic lists and nonoverlapping voice/node storage are the
domain. Full-word masks (`identifier &255`) retain native packed-word reads;
narrow casts had caused two later child loads to become bytes. This genuine
word projection closes all116 words without changing the ABI or adding a value.

**1D764** has ten genuine inputs: state pointer, four vector pointers, three
floats, a word and a byte. Its24-byte native frame reads the fourth vector at
old sp+16, floats+20/+24/+28, word+32 and final byte+39. It gates on enabled,
locks, prepends the state to the actual list, copies three whole vectors, negates
the fourth, stores three real scalar fields, invokes the actual matrix helper,
and installs the flags/gain before releasing. The head assignment's value is
the branch condition, preserving the native load/store sequence. Vector
whole-object/self-copy is supported; arbitrary partial overlap or already-linked
self-insertion is not claimed. Native state size136 and ordinary binary32 gain
arithmetic are explicit; aggregate-copy temporary allocation remains nonmatching.

**1C580** also has ten real inputs: byte request, short-buffer pointer, word
sample count and frequency, four bytes, a five-input callback pointer and opaque
word context. The last six arrive at old sp+16..+36, including the callback at+32
and context+36. After enabled/lock/allocation guards it configures the actual
24-byte sample-buffer record and a real packed25-byte sample descriptor, calls
three-input146B4, sets rate and four shifted level words, starts playback and
releases exactly once. The callback ABI matches the reviewed1C3CC consumer;
no artificial callback is implemented here. The frequency numerator converts
unsigned32 to float; the configured denominator is interpreted as the native
signed word and is loaded after the helper. Rate conversion goes through u32
before narrowing to u16. The domain requires a positive configured denominator,
ordinary FP state and finite scaled values whose truncation is representable
as u32. Invalid FP inputs/native exception fallback and downstream hardware
playback safety are not claimed by C89 host tests. Descriptor/buffer pointer
widths are N64-only, and actual helper entries/stack slots were checked.

**1824C /18448** are whole no-argument timed-stream processors over64 native
40-byte tracks at context+0x568. They preserve the special80,00 end marker,
one/two-byte time increments, wrapping unsigned deadlines, live context/cursor
reloads and the two real controller calls. Both long value forms decode signed15;
1824C's short form is signed7, while18448's short form is a signed-byte conversion
of a value whose high bit is clear. The source widens to unsigned before every
left shift, avoiding negative-left-shift UB while preserving native16-bit
truncation stages. Negative right shift and signed-short conversion follow the
verified N64 two's-complement convention, not implementation-independent C89.
Accumulator addition narrows modulo65536; high/low controller components keep
real byte widths. Programs must be allocated, well formed, sufficiently padded
for each native two-byte peek and terminate or reach a future event. Live helpers
must leave subsequent cursor/context accesses valid. Registered controller rows
remain required; there is no malformed-stream or concurrency-safety claim.

`abi_proof.py` independently compiles native sizeof assertions: voice416/node16,
sequence context4088, spatial state136/vector12, sample buffer24/descriptor25,
track40 and partial context3944. That partial view is not a claim that the entire
runtime context ends there. Host LP64 pointer layouts differ and are not native
offset proof. No helper implementation or shared layout/header is modified.

## Bounded diagnosis and refinements

O2 preceded O1. Unchanged workbench diagnosis covered all initial residuals,
and the improved sample-constructor checkpoint before its final control.
Only hashes/classification summaries are committed; native listings/objects are
not included. Strict relocated scoring is authoritative.

-1EB10: initial74/116; real word-mask projection closes it. Two forms.
-19194: initial26/119; explicit preservation of original handle closes it. Two forms.
-1D764: assignment-value head condition improves72/83 to21/83, then stops on the
already measured aggregate-copy register pattern. Two forms, no fake float input.
-1C580: independent descriptor stores improve37/124+3 to33/124+3; allowing the
unchanged u16 helper interface to perform the final narrowing does not improve it.
Three forms, then stop on scheduling/narrow-value lowering.
-1824C: explicit safe16-bit intermediate steps improve96/127 to89/127. Two forms.
-18448: the equivalent safe width-step control remains87/123; original retained.
Two forms. Neither stream is forced with signed-overflow/negative-shift source.

No fake formal, keeper, padding local, assembly, volatile trick, forced register,
callee body, target/scorer/flag-family change or boundary adjustment is used.

## Bounded semantic tests

Six actual-source C89 ASan/UBSan/float-cast-overflow groups pass **70,276 calls**:
961 tagged/untagged routing cases with live channels;64 chain/node protocols with
post-helper fields;2,049 ten-input spatial/list cases;448 ten-input sample cases
with changed global denominator and unsigned conversion boundaries; and33,377
cases per stream covering every short/long delta, timing/wrap boundaries and a
live context switch between the two controller calls. Stream tests preserve
signed7 versus short-byte behavior and safely decode all32,768 long payloads.
Helpers are synthetic contracts, not copied native algorithms. Only leak checking
is disabled for ptrace; no allocation or hardware safety is inferred.

```sh
python3 cloud/work/boot_tail/BT03-high-runtime-followon/verify.py
python3 cloud/work/boot_tail/BT03-high-runtime-followon/verify_controls.py
python3 cloud/work/boot_tail/BT03-high-runtime-followon/abi_proof.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-runtime-followon -p 'test_*.py' -v
```

Pinned compiler/preflight validates all manifests,439 extents/99,120 B and the
existing getter. Only two submissions and this packet change. Baseline sparse
lock/proof inputs are restored unchanged. Shared ledgers/D10, prior packets,
targets/scorer/compiler/symbols/layout/locks, runtime image/farm and restricted
helpers remain untouched. No ROM, raw assembly, object, credential or unrelated
private data is published. Independent review and exact aggregate CI precede
checker-owned merging.

## Independent review

The paired reviewer approved source checkpoint `a059a5bc62a12054c5c8b6edb16704920dfcc48e` / tree `a7e45b1106ba0be6ec194df089d63ed888a662fe`. All 26 final/control rows, exact function extents, native widths and all six sanitizer groups reproduce. The receipt is in `independent_review.json`; source hashes are unchanged.
