# BT02 remaining teardown wrappers

Eight strict relocated matches, **628 native bytes**, all first-form natural C89
at `-g0 -O2 -mips2 -G 0 -non_shared` plus the mandatory `-Wab,-r4300_mul`.
All fixed O1 controls differ. No nonmatch or source-variant search was needed.
Pinned-input replay and host behavior checks pass. Independent review and exact
aggregate-head CI remain required. These are source-matching results, not cartridge
coverage or promotion.

## Claim and scope

- Packet 4 / BT02-remaining; branch `dot/boot-tail-p4-bt02-remaining`.
- Fresh master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central exact claim `b7d5cab0`, `wave2/live_claims/BT02-remaining.json` in the lead's
  registry. All eight starts were open and disjoint from prior and active claims.
- Targets: `80010980` (64), `800109C0` (64), `80011074` (80), `800110C4` (64),
  `800118C0` (80), `80011CD8` (76), `80014488` (104), `800144F0` (96).
- Edits are only those matching sources and this packet directory. No queued packet.
- [Packet 1 PR #59](https://github.com/cabi24/SFRush2049-decomp/pull/59) at
  `21e104a22575cf4d639261d2f6d9535913074e6a` and Packet 2 checkpoint
  `76780b3a1b3e26c54b86e1f153344e92dac15b20` are read-only research provenance.
  Packet 1 is an unmerged research dependency, not an ancestor of this branch.
- Prior BT02-small and BT02-medium work, including both medium nonmatches, stays
  frozen. Their inspected ABI context is not re-counted here. Central STATUS and
  D10 are owned by the aggregate lead and were not edited.

## Reproduction

With the previously approved pinned IDO toolchain available, run from the repo root:

```sh
python3 cloud/work/boot_tail/BT02-remaining/verify.py
python3 cloud/work/boot_tail/BT02-remaining/test_host.py
```

`IDO_DIR` may point to an existing installation. No installation is performed by
these scripts. The verifier checks every installed compiler-file digest against
Packet 2, protected scorer/setup/inventory/getter hashes, all three target-manifest
members, exact equality of all 439 inventory/extent rows (99,120 B), and the original
12-byte getter. Each final source is compiled alone at O2 then O1, and source hashes
are stored with strict relocated results in `verification.json`. The verifier and
host harness hashes are included too. There are zero extra words, unresolved
symbols, unverified relocations or errors on every submission. No local literals or
switch tables are present.

## Actual ABI and behavior

Every matching function takes no arguments and has no consumed return value.
The 24-byte O32 frames and actual direct/indirect call sites were inspected, rather
than inferring leaf status from the inventory's direct-jal list. Names remain
address-based; teardown/audio roles are hypotheses from behavior.

| Function | Behavior and native evidence |
|---|---|
| `80010980` | If byte `8002C630` is nonzero, call `80014488`, then the no-argument `8001E0D4`, then clear the byte. No incoming registers are consumed. |
| `800109C0` | Same gate and final clear, using `800144F0`. The actual game caller in the historically named `car_shadow_render` target invokes it at offset `0x254` without preparing arguments or consuming a result. The historical caller name is not accepted as a semantic identification. |
| `80011074` | If pointer `800382E8` is nonnull, call `800118C0`, reload that pointer, pass it to callback `8003801C`, then clear the pointer. Native `a0` and neighboring release callbacks establish the one-pointer, unused-result callback ABI. |
| `800110C4` | Call `80011074`, then conditionally pass nonnull pointer `800382F0` to the same callback. The pointer is intentionally not cleared by this native body. |
| `800118C0` | If volatile byte `800382CD` is nonzero, call no-argument callback `80038004`, then no-argument `80010110`, then clear the byte. Its callers and both callees' sites require no extra arguments. The adjacent `80011910` setter stores one to the same byte after starting its callback task. |
| `80011CD8` | If volatile byte `800382CC` is nonzero, call `osRecvMesg` with queue address `800382B0`, null message destination and blocking flag 1; clear the byte before calling `80010110`. The queue result is ignored. Native `a0/a1/a2` agree with the existing `include/PR/os_message.h` declaration. |
| `80014488` | Gate once on byte `8002C630`, then call `80014140`, `800118C0`, `800110C4`, `80011848`, `800121BC`, `8001261C`, `8001144C`, `80010D74`, in that order. Every actual callee consumes no incoming argument. The gate itself is not cleared here. |
| `800144F0` | The same once-only gate and first seven calls, omitting `80010D74`. The gate itself is not cleared here. |

The queue source retains the existing byte-array storage view used by prior
BT02-medium's `80011D24`, then converts its address to an opaque `OSMesgQueue *`
for the real API. The forward tag and prototype match the existing header; no
shared type, storage owner or header change is proposed. Volatile byte declarations
retain the prior BT02 state view. `80010110`, `80010D74`, the previously matched
release helpers and the empty `8001E0D4` were inspected as native dependencies;
none is defined or stubbed in a matching source.

## Flag evidence and effort

All eight natural O2 baselines matched. Native global-load hoisting before the
frame, branch-likely exits, and pointer reuse distinguish these forms from fixed
O1 controls. Volatile byte address formation is retained naturally. There was no
allocator, schedule or frame search, so no near-match diagnosis was needed.

| Function | O1 differing/total words | O1 excess nonzero words |
|---|---:|---:|
| `80010980` | 2/16 | 0 |
| `800109C0` | 2/16 | 0 |
| `80011074` | 2/20 | 0 |
| `800110C4` | 11/16 | 0 |
| `800118C0` | 19/20 | 0 |
| `80011CD8` | 15/19 | 0 |
| `80014488` | 2/26 | 0 |
| `800144F0` | 2/24 | 0 |

## Host checks and limits

The host driver links all eight real sources as separate translation units.
Callback doubles are confined to the host-only test file. Checks cover:

- zero and nonzero/max-byte gates; no calls on every inactive path;
- complete nested teardown order, including the full/short path difference;
- stop callback and helper ordering, flag clearing timing, and repeated calls;
- pointer reload after a callback changes `800382E8`, release-before-clear, and
  the intentional retention of `800382F0`;
- exact queue pointer, null message pointer and blocking flag, ignored API return,
  clear-before-helper timing, and no second receive after clearing;
- one-time gate evaluation even if a helper changes the gate, and final top-level
  clear after the last helper.

Strict host C89 at O2 and AddressSanitizer/UndefinedBehaviorSanitizer at O1 pass.
LeakSanitizer is disabled because it reports unsupported execution under ptrace
in this environment; the driver allocates no heap objects. These checks do not
emulate N64 hardware or concurrency and do not prove that callback behavior on a
real machine satisfies the modeled contract.

No third-party source was copied. No ROM, raw native dump, object, credentials or
unrelated data is included. Targets, scorer, symbols, shared types, locks, layout,
runtime images, farm files and production gates remain untouched. Merge and
cartridge integration remain with the independent checker.
