# BT03 low lookup helpers: complete nonmatches

Five complete natural C89 bodies cover **784 bytes of research**. None is a
strict match. No source from this packet belongs in `cloud/matches/` yet.
Independent ABI/source review and all three receipt replays passed on immutable
source commit `05b040aeb0f994f3ee57ca3c094101005709ed2c`
(tree `a3d8b221ea3d8196146cd12009b294a66485d50e`); see `independent_review.json`.
These results do not establish cartridge coverage or promotion.

- Base: `301d9e7552ad4fd7f54a38796db84671e1000d35`
- Branch: `dot/boot-tail-bt03-low-lookups`
- Central exclusive claim: `39fc4212`, acknowledged before source edits
- Owner: `/root/match_boot_tail_lookup_five`
- Peer reviewer: `/root/match_boot_tail_bt02_finalsmall` (passed for research only)
- Allowed edits: this packet directory and the five named match paths; only the
  packet directory changed. Central status and D10 remain coordinator-owned.
- No reference source was copied. Behavioral names below are hypotheses based
  on the complete native bodies and their callers.

## Retained source and residuals

| Function | Bytes | Behavior | Final O2 | Final O1 |
|---|---:|---|---:|---:|
| `80016C20` | 192 | Select a key group, search its 8-byte entries, return the entry's pointer value | 22/48 | 48/48, 4 excess |
| `80016E68` | 120 | Search one 8-byte table by its key at +4; cache entry and return pointer value | 7/30 | 12/30 |
| `80016EE0` | 120 | Same operation on the second table | 7/30 | 12/30 |
| `80016F80` | 152 | Search 12-byte records, output halfword +6, then cache entry and return pointer value | 13/38 | 24/38, 5 excess, bounded-relocation error |
| `80017040` | 200 | Search each 8-byte bank descriptor's 12-byte records and return the first hit | 46/50 | 50/50 |

All final O2 bodies have zero excess words and zero unresolved, unverified,
or erroneous relocations. The 16F80 O1 negative control also reports an unpaired
HI16 for its cache global at +0x94. Its low half lies beyond the target-sized
comparison window, alongside the extra candidate tail. That control is not a
valid match and is not represented as a clean relocation proof. No scorer or
target change was attempted.

`verification.json` records exact source and target hashes, every flag set, and
all comparison fields. `status_delta.csv` is the proposed coordinator-only
status update. It deliberately reports the retained complete source's score,
not a lower score from a rejected control.

## Actual ABI and data evidence

The complete native 8001E864 callee was read, including its indirect callback.
It is an ordinary o32 five-argument binary search: key in a0, table base in a1,
signed count in a2, stride in a3, and comparator in stack argument slot +16.
The callee saves the first, second, and fourth arguments and loads the fifth
from its caller's slot after its own 56-byte frame allocation. It calls the
comparator with key and candidate pointers in a0/a1, uses its signed comparison
result, and returns either the selected entry or null. There is no hidden
argument, unsaved callee-saved write, or IPA assumption. This packet changes
neither that callee nor its declarations elsewhere.

Every claimed body has exactly one direct call site, to 8001E864, and no
indirect call site. The callback addresses are 80016BF8 for C20, 80016E40 for
E68/EE0, 80016F58 for F80, and 80017018 for 17040. The first four callbacks read
unsigned packed halfwords at +4 from both arguments; the last reads at +0.
They subtract left key minus right key. Function-pointer declarations express
the opaque pointer ABI; existing recovered callback sources use equivalent
local packed-prefix views. A future shared-header integration remains outside
this packet.

- C20/E68/EE0/F80 home and reload the low unsigned halfword of their first
  argument. F80's second argument is a real pointer to an unsigned halfword.
  17040 directly stores the low halfword and also homes its genuine argument.
- C20 uses key >> 6 to select a 4-byte packed descriptor, whose +0 halfword is
  the count and +2 is the first 8-byte entry index. It always publishes the
  selected group, but leaves the first index, search key, and cached entry
  unchanged when the count is zero. A nonempty group records the key and first
  index, searches, publishes the nullable entry, and loads pointer field +0
  only after a hit.
- E68/EE0 publish their unsigned halfword key at 80042674/80042684, search the
  table at 80038610/8003C618 using its signed count at 80038608/8003C610, and
  cache the nullable entry at 80042678/80042688. Their returned pointer comes
  from an unaligned native 32-bit field +0.
- F80 searches base 8003CE20 using count 8003CE18 and stride 12. On a hit it
  writes packed halfword +6 to the caller, loads the return pointer at +0,
  and then publishes the entry at 8004269C. On a miss it clears that cache
  and preserves the caller's halfword. The retained source preserves this
  load-before-publication ordering. LookupEntry describes only the first
  eight bytes of the 12-byte record; it does not assert the full record size.
- 17040 stores key 800426A0, then loops over 8-byte packed descriptors at
  80042230 while the signed, dynamically reread count at 80042228 permits.
  Each descriptor supplies a packed halfword count +2 and pointer +4. A hit
  returns immediately; exhausting the banks, zero count, and negative count
  return null. It does not cache the result.
- Return callers confirm pointer use: 21C58 offsets the C20 result; 22F24 and
  2321C dereference E68; 1A270 offsets and reads EE0; 19F48 pairs F80's return
  with its output count; 20370 accesses byte +9 of a 17040 result.

The search keys use address-spelled global views for their opaque prefix and
separately written halfword. This reflects distinct native references, without
claiming recovered object ownership or adding a shared layout. All native
packed loads are generated by IDO's existing `#pragma pack(1)` support. No
manual byte-load emulation, inline assembly, or artificial padding locals are
used. No local rodata or switch-table proof is needed.

## Bounded experiments and stopping reason

O2 was tried before O1 for each source. Native compact call frames, grouped
loads, branch-likely loop continuation in 17040, and the surrounding verified
callback leaves support O2. O1 is retained as a negative control, not selected
because its positional word score happens to be smaller for two wrappers.

The initial O2 body of every function was diagnosed with `tools/workbench.py diagnose` before directed refinements. Final sources were diagnosed again.
The numeric/metadata-only receipts are in `diagnosis.json`; full temporary
assembly, object files, and detailed instruction dumps were not committed.
Workbench ownership is heuristic and did not authorize any tool changes.

Directed variants are fully preserved in `controls/` and `experiments.json`:
- Replace the overbroad key-record hypothesis with directly referenced native
  key storage; this restores the complete pre-call sequence in E68/EE0/F80.
- Put assignment in the null gate; this did not remove the cache-forwarding
  difference. Opaque pointer and direct packed-pointer views were likewise
  equivalent, after callers established that the returned word is a pointer.
- In F80, move cache publication into each result branch. This reached 5/38,
  but publishes the cache before loading the return pointer. It is a rejected
  ordering control, not the final body. Correctly ordered local return-value
  and shared-tail spellings stop at 13/38. Source declaration order and comma
  spelling did not resolve that plateau.
- Reuse C20's group/index local for its nonoverlapping lifetimes; this regressed
  to 24/48 and was rejected. The retained separate semantic locals score 22/48.
- Give 17040's key an array view; it emits the same coalesced address as the
  scalar and does not move 46/50.

No target exceeded ten natural source variants (including its seed), well below
20. The session stops on the observed coalescing/forwarding and allocation
plateaus rather than sweeping declarations, adding volatile qualifiers without
semantic evidence, fake parameters, fake locals, dummy calls, or assembly.

Next hypotheses require new evidence: original storage/type ownership for the
key/cache globals; compiler evidence explaining the native cache reread in
E68/EE0; and a natural correctly ordered F80 expression that allocates the
entry to v1 and return value to v0. C20 additionally retains a v0/v1 index/base
allocation difference. 17040 needs evidence for why the initial key store uses
a separate address materialization from the loop-invariant key pointer. None
of those gaps is grounds to change the scorer, protected inputs, or compiler.

## Reproduction and limits

With the pinned compiler selected through IDO_DIR:

```sh
python3 cloud/work/boot_tail/BT03-low-lookups/verify.py --output /tmp/lookups-verification.json
python3 cloud/work/boot_tail/BT03-low-lookups/test_layout.py --output /tmp/lookups-layout.json
python3 cloud/work/boot_tail/BT03-low-lookups/test_host.py --output /tmp/lookups-host.json
```

Verification rehashes every pinned IDO file and protected preflight input,
checks the target manifest, compares all 439 extents/99,120 bytes against the
inventory, strictly replays the existing getter, and recompiles every final
body at O2 then O1. The compiler, scorer, and target assets are unchanged.

IDO C89 assertions confirm pointer width through the exact packed entry/bank
sizes and key/count/value offsets. Host tests pass 43 cases using C89, warnings
as errors, and undefined-behavior sanitizer: zero/nonempty groups, missing and
found entries, boundary keys, output preservation, bank early exits, negative
and zero bank counts, and mutation of the bank count by a mocked search call.
Host pointers are wider and byte order differs. Host tests mock binary search
and validate its five arguments and effects; they do not prove the actual
search implementation, native packed layout, or adjacency of separately named
global views. The independent IDO layout checks and native address audit cover
those claims only to the extent explicitly stated above.

No aggregate CI or cartridge-production gates have run for this packet.
Independent source/ABI and strict replay review passed for research only. No ROM bytes, raw
assembly dumps, objects, credentials, private data, locks, layout changes,
shared-header changes, symbols, runtime-image edits, scorer edits, or farm work
are included.
