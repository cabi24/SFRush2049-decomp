# Runtime B battle attachment cleanup: first source checkpoint

PARTIAL-SOURCE. Complete natural C for A95C/AA14 and the real AA8C entry/cleanup
prefix, with an independent bounded native behavioral check. No candidate has
been compiled and no matching bytes are claimed.

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.

The full target extents are A95C [8038A95C,8038AA14), 184 bytes;
AA14 [8038AA14,8038AA8C), 120 bytes; and AA8C [8038AA8C,8038C910),
7,812 bytes. The root prefix is [8038AA8C,8038AB8C), 256 bytes. Its genuine
cleanup path jumps into the existing epilogue at 8038C8E8; it is never shortened
into a fictional complete function. The continuing path reaches 8038AB90 after
executing the original branch-likely delay-slot color load at 8038AB18.

## Source and ABI

`cleanup_helpers.c` contains the two complete semantic static helper bodies.
`aa8c_entry.c.fragment` deliberately leaves the genuine root incomplete and
cannot be compiled. There is no fake keeper, invented formal, helper wrapper,
synthetic root or standalone MIPS helper experiment. The exact requested O3
header is present on the helper source as the future required recipe, not
as evidence of an O3 build. No compiler or matching experiment was performed.

Native A95C receives its one signed-short source argument in s4; AA14 receives
it in a0. Both use unsaved s0-s3. These private layouts must be explained by the
future complete genuine root/callee unit. The ordinary callback ABI is supported
by C910 registration and descriptor dispatchers, as recorded in the predecessor
scout. Original source declarations and translation-unit identity remain unknown.

`layouts.h` records observed external persistent structures and typed transforms;
all unknown byte spans preserve actual memory offsets, not invented frame padding.
The two arrays are adjacent: four 0x10C slot records end at 80399550, and four
0x148 group records end at 80399A70. The descriptor is genuinely embedded at
player+0x2F0. No new storage or ownership claim is introduced. Numeric field
names deliberately distinguish the two trailing group bytes at +0x138/+0x139.
A95C clears only +0x139, only after removing a live extra handle.

## Contracts established in this bounded region

- Cleanup dispatch reads the separate signed-byte cached kind at 80399118;
  player.mode at +0x384 is the reset destination, not the dispatch selector.
- Update is narrowed to signed 16 bits before testing; update==0 short-circuits
  the physics inhibit read. Any nonzero signed inhibit byte triggers cleanup.
- The shared color word at 80394884 is loaded before that exit decision. Its
  later offset-table alias does not justify a guessed C array extent here.
- The selected player pointer is captured before calling either cleanup helper.
  The descriptor handle is loaded after cleanup. The root resets mode/selection
  to 8/-1, then hides the signed-short handle using mode 0 and viewport mask 15.
- Each helper compares all 32 handle bits with -1, then narrows only the service
  argument to signed 16 bits. The sentinel store happens after the removal call.
  A95C reads the extra handle after all five earlier removal calls.
- Each record slot is tested independently. Transform arrays, timers, active
  state and neighboring records remain unchanged under scene-only helpers.
- Historical `sound_call_minimal` is scene-node removal: the base-commit accepted
  source calls entity_spawn_callback(handle,0,0), not an audio operation. Its
  downstream accepted source mutates scene links/free state and high-water count.
  Mode-0 model_data_load sets the object's hidden flag; it is not a callback
  dispatcher. For valid scene handles and disjoint live storage these effects
  do not alias the player/cache/attachment arrays. Invalid or truncated scene
  indices are not established safe by these tests.

The four-record arrays do NOT prove that every real caller passes owner 0..3.
A parallel caller audit found a registration path ranging over six physics/player
records, conditionally active, without a local C910 or AA8C bound. Therefore all
fixtures here expressly restrict owners to 0..3. Global upstream invariants and
owners 4/5 remain unresolved, and no negative indexing is declared valid.

## Verification and replay

From a normal checkout:

    python3 cloud/work/runtime_b_aa8c_entry_20261006/verify.py --check

From this source-only overlay, add `--reference-root /path/to/repository`.
The overlay needs current canonical tools/cloud/score.py and owndata.py. The
verifier calls score.targets(), binds the three complete native word sequences,
and authenticates them against image B inflated in memory from the base-commit
asset. It reads accepted service context with git show at that same base.
Neither the changing target manifest/scorer nor accepted production source is
hash-pinned. No live lock assertion is used. The receipt binds only this packet,
the native targets, base context, and proved behavior; it does not hash its pytest
file. No compiler-dependent tests exist, so this packet requires no IDO skip.

`native.py` is a bounded, fail-closed integer MIPS interpreter including branch
likely annulment, delay slots, private calls and caller-save poisoning at real
external-service boundaries. `reference.py` is a separate direct semantic
algorithm with no register/frame knowledge. They compare terminal status, the
entire mapped nonstack state and ordered service calls including pre-removal
slot state. Both complete helpers and all 64 prefix words plus the ten executed
real epilogue words are covered. All conditional branch outcomes are covered.

Fixtures independently vary every cached-kind byte, every inhibit byte, each
of the 32 five-slot live/sentinel masks, full-word versus low-half-minus-one
handles, extra-handle state, sign narrowing, update 0/1 and wider raw register
values. This is orthogonal coverage, not a full Cartesian-product claim.
Synthetic helper mutation controls test retained pointers, fresh later-slot and
extra-handle reads, post-call stores, and the late descriptor-handle read.
Those deliberately broader hooks test ordering resilience; they do not assert
that the actual scene remover mutates descriptor fields. Unknown opcodes,
unmapped owners and unaligned descriptors fail closed. Ten wrong semantic
contracts are rejected. The exact replay count is in verification.json.

Important limit: this verifies the independently specified bounded behavioral
contract against native execution. It does not compile or execute the proposed
C source, recover the unseen root, establish complete gameplay, or prove the
scene helper internals against machine code. Source review is still needed.

## Ownership and next step

This packet owns only its research files and focused test. It changes no
production/context/lock/tool/native asset. Source was reconstructed from native
behavior; the predecessor whole-donor search found no battle-callback donor in
rushtherock. No third-party source was copied into this packet.

Finish genuine AA8C [8038AB8C,8038C8E4) next, resolving shared offset storage,
caller bounds, field lifetimes and service aliases. Then compile the full genuine
closure once through the unchanged O3 scorer route, retaining mandatory backend
flags. Only a full relocated equality proof could admit matching claims.
Publication still requires the parent's independent review and current-master
full with/without-IDO suites. No publication or CI watcher was started here.
