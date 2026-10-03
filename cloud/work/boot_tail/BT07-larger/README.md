# BT07 larger pair

Two complete C89 bodies are strict local MATCH: **552 native bytes**. Independent
source/ABI review and exact aggregate-head CI remain required before VERIFIED-BODY
credit. This is matching-source research, not cartridge coverage or promotion.

- Base: `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Branch: `dot/boot-tail-bt07-larger`; exact central claim: `b34484c9`.
- Targets: 800259A8–80025AB4 (268 B), 80025C68–80025D84 (284 B), ends exclusive.
- Only these sources and this directory change. The lead owns central STATUS,
  claims, totals and D10; no D10 or protected path is modified.
- The previously approved IDO 5.3 toolchain is reused read-only. Its 24 file hashes
  equal the approved prior receipt, with archive SHA-256
  `ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
- All three protected manifest members pass. All 439 extents and 99,120 B equal
  the inventory. The existing getter passes strict replay and adds no credit.

## Results and native optimization evidence

| Address | Bytes | Behavior hypothesis | O2 | O1 negative control |
|---|---:|---|---|---|
| 800259A8 | 268 | Query guarded stream progress ratio and stored float value | MATCH | 65/67 + 13 extra; truncated-window HI16 error |
| 80025C68 | 284 | Initialize two streams, optional buffers, queue and callback | MATCH | 71/71 + 29 extra |

Both first complete source forms match. No tuning variants were needed, and no
near-match diagnosis was required. The O1 trials are fixed negative controls,
not accepted candidates. 259A8's O1 expansion leaves an unpaired HI16 at the native
comparison-window boundary; this is preserved unchanged in the receipt. It is
absent at O2. The final O2 trials have zero differing/extra words, masks, unresolved
and unverified references, and relocation errors. Every relocated full native body
equals its protected target, and trailing object alignment words are zero.
The scorer adds mandatory `-Wab,-r4300_mul`.

259A8 exhibits O2 branch-likely gates, a shared indexed-record address, reuse of
the actual token home for the selected index, and one float spill across unlock.
25C68 has O2 induction pointers for both array loops, shared flags/hook addresses,
and a branch-likely allocation loop. Neither uses local rodata or a switch table.
259A8's floating zero and unsigned-to-float adjustment constant are immediate
compiler sequences, so no unchecked literal/table placement is involved.

## Whole-body reconstruction and actual ABI

`reconstruction.md` records the region-by-region reconstruction before candidate
source creation. These are original native reconstructions; no external source
was imported. The arcade source is unavailable in this repo-only checkout.
Behavior names and original qualifier spelling remain hypotheses.

259A8 reads two actual inputs and returns a float. On sentinel/disabled/failed
lookup paths it does not touch the output pointer. A successful lookup always
copies float +4640, but computes unsigned(+4636)/unsigned(+4616) only when the
busy byte is 2. It returns queue helper 250F0's actual integer token to 25120.
The latter accepts the call argument even though the helper body does not use
its value. Both helpers and 25264 were previously independently reconstructed;
no extra formal or optimizer-visible fake helper is present here.

25C68 consumes one actual flags word. The two five-input calls to 25EB0 agree
with its actual initializer and the callback ABI verified in the medium packet.
24FB0 receives the same record. 250AC takes no inputs and creates the queue.
The actual 1C770 body implements the two-times-count-plus-eight unsigned size
calculation; its original signedness is not uniquely identified by these bytes.
The hook at D_80038000+24 is the actual two-input allocator slot; the table's
32-byte extent is independently supported by 20598's eight-word copy, and
25DC0 consumes its +28 release slot. The returned pointers are each passed to
osInvalDCache, whose pointer/signed-size declaration agrees with `include/PR/os.h`
and whose symbol resolves to 800084E0. The callback 2574C reads no input arguments.

The matching local stream views are identical across these two sources, preserving
4,648-byte stride and the earlier state, busy, handle, buffer, processed, float and
token offsets. +4616 is exposed as an unsigned count/rate field based on native
conversion and previous field consumers. Unknown arrays model actual object
storage, never artificial stack padding. No production shared type is modified.

## Reproduction and behavior checks

With the approved toolchain directory selected through `IDO_DIR`:

```sh
python3 cloud/work/boot_tail/BT07-larger/verify.py
python3 cloud/work/boot_tail/BT07-larger/test_semantics.py
```

`input_pins.json` binds compiler/protected/scoring inputs; `verification.json`
binds the matching sources, full target bodies, notes and verification scripts.
The verifier is read-only. `experiments.json` contains all four initial O2/O1
trials without native words or object data.

The host harness compiles the actual two submitted C bodies. 32-bit C89 static
assertions verify stream/hook sizes and field offsets. **631 ASan/UBSan cases**
cover sentinel/disabled/failed lookup with a null output pointer, both indices,
all 256 busy values, unsigned conversion boundaries including values above 2^31,
zero-denominator infinity/NaN behavior, output-value copying, gate/queue-token
flow, six ownership flag patterns, both initializer passes, and allocator/null
return/cache-invalidation order. Tests also assert untouched fields and buffers.
Test-only callee stubs exist solely in a temporary host executable. LeakSanitizer
is disabled under ptrace; ASan and UBSan remain enabled. Host tests complement the
32-bit strict compiler proof and do not execute a ROM or target runtime.
