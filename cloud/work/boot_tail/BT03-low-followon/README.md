# BT03 low follow-on: two strict bodies, three bounded residuals

Five fresh targets / 492 B were exclusively claimed at central `07997e6a` on
`dot/boot-tail-bt03-low-followon`, based on master
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Fresh setup, target checksums,
439-start/99,120-byte inventory equality and the existing 12-byte getter all
pass before candidate work; see `preflight.json` and `setup.log`.

## Frozen outcomes

| Function | Bytes | O2 differing words | O1 differing words | Result |
|---|---:|---:|---:|---|
| 80014A0C | 104 | 24/26 | 22/26 | complete NONMATCH |
| 80014A74 | 124 | 0/31 | 28/31 + 1 excess | first-try strict MATCH |
| 80014B3C | 116 | 0/29 | 28/29 + 15 excess | first-try strict MATCH |
| 80014C60 | 76 | 12/19 | 19/19 + 10 excess | complete NONMATCH |
| 80014E1C | 72 | 7/18 | 18/18 + 5 excess | complete NONMATCH |

Two local strict matches total **240 B**. Three complete nonmatches total
**252 B** and receive no matching credit. Matching submissions are only
`cloud/matches/boot_tail/func_80014A74.c` and `func_80014B3C.c`.
All retained O2 bodies have zero nonzero excess words, unresolved symbols,
unverified references, masked relocations or relocation errors. Matches have
full relocated equality, not relocation-blind equality. Aligned object text is
128 B for each matching body, with one and three ordinary zero padding words
respectively. No candidate boundary is enlarged to include that padding.

Every compile uses `-g0 -O2` then `-O1 -mips2 -G 0 -non_shared`; the scorer also
adds `-Wab,-r4300_mul`. More precisely, each flag string in `verification.json`
contains one selected optimization level and the same fixed remaining flags.
O2 reproduces the native 40-byte wrapper frame and 24-byte release-dispatch
frame exactly; O1 adds body instructions. The other bodies' native leaf homes,
branch-likely loop and 24-byte copy frame support O2 structurally, but do not
make those residuals matches.

## ABI and real object layout

The same naturally aligned `AudioSlot` prefix is repeated in the two submissions
and the cap/store research source. It retains the previously matched 104-byte
record stride of `D_80038294`, active byte +0, pending byte +1 and word +20.
Newly read fields are halfword +4, release counter +40 and output halfwords
+64/+66/+68/+70. Unknown arrays represent actual unexamined object ranges, never
stack padding. No packed attribute or fake local changes layout. Earlier exact
bodies `149DC`, `14BB0` and `14C18` independently corroborate the stride and
shared active/pending/position fields; their source bytes remain unchanged.

`14A74` has exactly five real inputs: a record index and four 32-bit values.
It calls actual external `8001E0E0` with eight arguments, in this order:
record+66, record+64, input 2, input 3, input 4, record+68, input 5, record+70.
The native callee reads full unsigned words and writes halfwords through all
four output pointers. Its internal floating-point tables and constants are
outside this packet; the caller needs only the verified external call relocation.
Names such as volume/pan/span are descriptive leads, not a source-identity claim.

`14B3C` does nothing when inactive. Otherwise it sets release count to 20, clears
a nonzero pending flag, or calls the already matched `80011A3C` on the record.
The declaration preserves that callee's genuine opaque `struct AudioState *`;
the explicit pointer view does not invent another argument or helper body.
The callee's known release/count/state fields corroborate the counter at +40.
Native callers `1FA18` and `1FAE4` supply the real index.

`14C60` copies four signed samples sequentially from the buffer start to its
sample-count offset, then writes back exactly eight bytes using real
`osWritebackDCache`. Sequential semantics matter for overlapping ranges: this is
not replaced by snapshot copying. Caller `1C508` passes a buffer pointer and a
full-word count from its record. The host test includes offsets 0 through 60.

`14E1C` walks relative resource records, checking the all-ones next-offset sentinel
before comparing the 16-bit key at +4. A found record pointer is returned; absent
keys return null. A real offset local reuses the same loaded word for termination
and pointer advance. Native wrappers `14E64`, `14E90`, `14EBC` and `14EE8`
corroborate the narrow key and resource-pointer inputs. The retained typed pointer
is an opaque record view with no change to the two real O32 argument slots.
No resource table or raw data is fabricated.

## Directed controls and stops

`diagnosis.json` records workbench metadata from fully relocated candidate objects
before refinements and again at improved frozen checkpoints. Temporary native
listings and objects are not committed. Workbench counts may include ordinary
zero alignment words; the strict scorer independently tests the full native span
and rejects nonzero excess. Ownership/lever labels are heuristic, not proof.

- `14A0C`: one complete form, O2 and O1. Native homes the u16 formal then masks
  its original argument register; O2 instead masks a temporary. This is the
  already documented shared narrow-formal plateau. No repeated prototype,
  register, K&R, padding or forced-home sweep was attempted.
- `14C60`: seven ordinary forms total: direct indexing, real destination pointer,
  explicit byte offset, signed count, typed flush expression, signed loop index
  and unsigned loop index. The genuine destination pointer improves 17/19 to
  12/19. Remaining differences concern pointer allocation/final forwarding;
  bounded refinements no longer move the result. Native stores and flush order
  stay intact. Next input is authentic pointer/count source context, not an
  artificial live value or alternate calling convention.
- `14E1C`: initial opaque-pointer/local view is 17/18 plus one excess word. A
  typed advancing pointer and a real loaded-offset local improve to 7/18 with
  zero excess; reversing the sentinel comparison produces the same object.
  Three forms total, then stop. Remaining home/mask allocation needs authentic
  caller-facing/compiler context, not another narrow-formal sweep.

Ten archived control sources plus the five retained sources produce 30 O2/O1
rows in `verification.json`. The first-try matches need no rejected controls.
No external source was copied; current native targets and already reviewed
local caller/callee evidence are the reconstruction basis. No fake formal,
keeper, padding local, dummy call, volatile trick, assembly or helper stub exists
in any candidate.

## Tests and reproduction

Six tests compile actual retained C as C89 with pedantic diagnostics, warnings as
errors, AddressSanitizer and UndefinedBehaviorSanitizer. They cover **524,455
candidate calls**: every u16 cap value at seven valid indices, all output pointer
positions and full-word forwarding, active/pending dispatch classes, overlapping
sample copies with flush-after-store observation, and every u16 key against a
finite relative list plus an empty sentinel. Host layout assertions cover all
shared offsets and the 104-byte stride. Helpers in test harnesses are explicitly
synthetic contracts, never candidate bodies. LeakSanitizer alone is disabled
because it cannot run under this executor's ptrace; these tests allocate no heap.
See `host_verification.json` for exact source/test hashes and scope limitations.

```sh
python3 cloud/work/boot_tail/BT03-low-followon/verify.py --check
python3 -m unittest discover -s cloud/work/boot_tail/BT03-low-followon -p 'test_*.py' -v
python3 tools/cloud/check_submissions.py --base 301d9e75 --head HEAD
```

Only the two matching files and this owned research directory change. Central
STATUS/D10 are sole-writer coordinator work. No prior frozen body, shared header,
target/scorer, symbol/layout/lock, production gate, runtime image or farm changes;
no native dumps, ROM bytes, objects or private data are included. The 631 existing cloud regression tests pass with zero skips. Independent BT03-high review passed source commit
`ccca6488dc1b318d704469f33ac16cc12d0ebabf`, tree
`d3f7f3c753763a1264b0a1c26df4a4ed4ae5b8e6`; see `REVIEW.json`.
The reviewer independently replayed all 30 compiler rows, read the actual
caller/callee ABI and record fields, and reran all six sanitizer-backed tests.
Aggregate publication and exact-head CI remain lead-owned and pending. Merging stays checker-owned.
