# Callback retirement and the remaining small-overlay lifetime boundary

## Result and scope

This extends [the service-pair residency report](https://github.com/cabi24/SFRush2049-decomp/blob/b5da1df8c9216549e3a515170ab0c356870dcf14/cloud/work/small_overlay_service_pair/RESIDENCY.md)
from draft PR #53 with native callback-retirement evidence. It does **not** prove
that every C974 invocation has the small overlay resident. It does not establish
a reachable bad execution either. No target, lock, scorer, accepted source,
compiler claim, cartridge composition, or coverage metric is changed.

Baseline: master `f82c5204edc4ead0f9056f323ff71122209ff259`. Concurrent draft
PR #54 (`054e12993aa632cfcf5e255f5013b7954317ba12`) supplies image-qualified
work, not a universal callback-lifetime invariant. This packet is independent of
its files and deliberately does not inherit its blanket call-resolution claim.
The paused target and helper-context investigation were not investigated.

## New native evidence

### Installation does not imply immediate collision execution

The 48-byte record at `80118BB0` (index 120 in table `80117530`) contains
`8010C7F4` at +0x08 and `8010C974` at +0x0C. Its unsigned flags at +0x12 are `0x022A`.
The setup body `800AB7D8` tests mask `0x0002` at `800ABA70..800ABA78`, allocates a
node, and installs the update pointer at node+0x14 before linking it at head
`801391F0` (`800ABA94..800ABAB4`).

The optional immediate +0x08 callback at `800ABAD4` requires mask `0x0004`;
the alternative +0x08 call at `800ABB04` requires mask `0x4000`. Both are clear
in record 120. Thus **this setup path installs C974 without invoking C7F4**.
The prior report's pointer association alone did not establish this distinction.
It says nothing about other paths invoking C7F4 or modifying the record.

### Retirement invalidates dispatch before recycling a node

For `8009079C(node, unlink)`, the visible order is:

1. If signed node+0x06 is nonnegative, call `8008AE8C(index, 1, 15)` at
   `800907C0`. The callback remains nonzero during this callee. Its complete
   effects/reentrancy are an explicit open assumption.
2. Clear node+0x14 at `800907D0`; set node+0x06 to -1.
3. If `unlink != 0`, remove the node from active head `801391F0` or from its
   predecessor (`800907EC..80090844`).
4. Put the node on free head `801392C8`; decrement signed halfword count
   `8012E66C`. The `8012E678` comparison is a maximum-like count update,
   not a minimum/free-space proof.

For a valid finite active list, a present node, nonzero unlink, a returning
release callee that preserves the list, and no concurrent mutation, this
removes the node from future active-list dispatch and nulls its old callback.
Do not strengthen that to “cleanup is callback-quiescent from entry”: step 1
comes first. Do not apply it to unlink=0, invalid lists, recycled/reinserted
nodes, or another updater that has already cached a pointer.

`800B0618` repeatedly reloads the active head and calls this retirement with
unlink=1 (`800B063C`). At `800B0650` the loop has observed an empty list.
It then calls `800B0580`; that callee must be summarized separately before
claiming emptiness on the enclosing function's return.

The updater `800B0868` caches next before calling node+0x14 at `800B08A4`
with a1=1. Consequently current-node retirement does not lose the cached
successor in the isolated synchronous replay. Deletion/reuse of a *successor*
by a callback is outside this claim.

### Actual transition paths contain retirement loops

The following loops directly retire active nodes before proceeding:

- `800D5AB4..800D5AD0`, immediately before `800D5AD4` calls `800B0580`.
  A later path writes requested state 4 at `800D5B80`, calls state change at
  `800D5B7C`, then calls large-image service `8038F454` at `800D5B84`.
- `800F8878..800F8894`, before requested state 4 is written at `800F88F4`.
- `800F89D0..800F89EC`, before requested state 4 at `800F8AB8`.
- `800F8C98..800F8CB8`, before requested state 4 at `800F8D1C`.
- `800FCF48..800FCF64`, before subsequent player/resource cleanup.

These are useful **retirement points**, not proofs that all transitions use
them. Each later segment contains callees, including `800B0580`, whose ability
to insert nodes or mutate overlay ownership has not been closed. The direct
loader does not itself drain this list.

### A concrete unclosed callback lies between guard and dispatch

The normal frame path compares current and requested state at `800FD69C`,
then calls `800FBC30`, `800F733C`, and updater `800B0868` in that order.
The other updater path checks `801170FC != 0` at `800FD638..800FD63C`,
then calls `800F733C` before the updater at `800FD64C`.

`800F733C` is not a leaf housekeeping operation: it iterates pointers in
`80149450` using count `80149788`, loads object+0x28, and calls that pointer
at `800F7384` if nonzero and object+0x00 != -1. Zero return then calls
`800B358C` at `800F7394` to remove/recycle that object-array entry.
The callback target set and effects remain unclosed. Therefore neither the
state-equality check nor the earlier `801170FC` guard can be carried across
this call without additional evidence. C974's own guard is re-read at entry,
but its value is not guaranteed by the earlier frame guard alone.

## Bounded native replay

`replay.py` loads the existing local tracked asset, inflates the game image,
and verifies its full SHA-256. It executes only the three explicitly allowlisted
bodies: retirement, drain, updater. It supports only encountered integer
instructions and MIPS delay slots/branch-likely annulment. Unsupported opcodes,
uninitialized memory, instruction fetches outside those bodies, and exceeding
the step bound fail. No extracted instructions or image bytes are committed.

The tests cover head/middle/tail unlinking, empty/one/three-node drain, stable
live dispatch, synchronous current-node retirement, positive resource-index
callee ordering, record flags, and fail-closed reads, unsupported opcodes, escaped/unhooked targets, and cyclic-list step bounds. Region hashes are also independently checked against the manifest.
`8008AE8C`, `800B0580`, and `800B066C` are explicitly stubbed only in cases
that need them. C974 is a callback hook; its full body is **not** executed.
The updater test with no loader-state memory demonstrates a read dependency
of this bounded body, not runtime residency or a reachable stale call.

Run from repo root:

    python -m pytest --tb=short tests/conveyor/test_overlay_callback_lifetime.py

Sixteen cases passed locally. These are native-instruction bounded replays with
synthetic lists and declared callee behavior, not an emulator/gameplay trace,
a complete MIPS implementation, or an exhaustive symbolic proof. Exact region
hashes and typed metadata are in `evidence.json`.

## Narrow next proof obligation

To carry any retirement point through an overlay transition, close the
intervening node producers and callback target sets, especially `800F733C`'s
object+0x28 callback. Also close `8008AE8C` effects before callback invalidation,
`800B0580` after drain, successor mutation/reuse, scheduling, and other
creation/dispatch routes. Until then retain the conditional small-loader
invariant from #53 and keep universal residency explicitly unproved.

Independent review checked the three native bodies, the intervening `800F733C`
callback, the replay's encountered opcode and delay-slot semantics, and all
16 final focused tests. No raw instruction arrays or ROM bytes are included.

Aggregate verification: **1411 passed, 41 skipped, 9 deselected** for
`python -m pytest --tb=short -m 'not node_required'`. Setup uses the repository's
pinned decomp-permuter/mips_to_c submodules, IDO, and MIPS binutils, with the
permuter and repository root on PYTHONPATH. Initial attempts without initialized
submodules or configured binutils were setup failures; the stated final result
is from the properly configured rerun. This is the default Conveyor suite,
not `tests/cloud`, ROM verification, or gameplay execution.
