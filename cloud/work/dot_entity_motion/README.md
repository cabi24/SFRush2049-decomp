# Typed entity-motion callback: NONMATCH research

`func_8010E4E4`, retail `0x8010E4E4..0x8010E694`, is 108 instructions /
432 bytes. The natural C candidate has **72/108 differing full resolved words**,
including four missing final target words. Its ELF function is **412 bytes**
(103 words), with one zero alignment word making 416 bytes of `.text`.
Native frame104 versus candidate72. This is research only: `claims: []`.
No accepted coverage, splice, integration, matching, or ROM claim is made.

## New work and historical baseline

Fresh master is `a12daa63ae67c8dab3404efb666089c884dadfd4`. Accepted locks,
single/group claims, prior PR1–35 targets, and the prior rounds' rejected
candidates were checked. This target was not one of those submitted functions.
Existing `registered-heads` and `heads_B13` work is retained unchanged.
The latter already corrected the zero-mode call and supplied a typed seed;
those discoveries are **not new here**. Its recorded minimal-repair result was
95/108 and used byte-address casts and an untyped global descriptor table.

This contribution introduces a typed 48-byte descriptor view (flags at18),
an actual pointer to the model's velocity array, and an explicit once-per-call
acceleration-scale snapshot evidenced by the native `lwc1 f0` before the loop.
It adds complete linked/native/host behavioral verification, including external
callback mutation and call-order effects. O3 with the real velocity subobject
pointer reaches72/108 without artificial storage, volatile qualifiers, pointer
integer laundering, invented parameters, fake helper calls, or padding the
function. All `unknown` bytes describe observed record offsets, not stack filler.

## Semantics and inferred layouts

The first parameter is an inferred state view: object pointer at12 and timer
at16. The signed16-bit second parameter is tested before the global pause word.
Zero mode calls `entity_transform_apply(state,1)` even while paused. Nonzero
mode and nonzero `D_801170FC` return without dereferencing the object.

The object view has signed index14, signed kind16, position vector20,
accumulated vector56, and model pointer108. The model starts with two float
vectors at0 and12. These are offset views, not proven complete game classes.
The descriptor array at `D_80117530` uses a48-byte stride and flags at18.
Array bounds are unknown; accessible records and valid descriptor indices are
preconditions. Tests use descriptors0–3. Names remain provisional; no direct
arcade equivalent was established because the arcade checkout is unavailable.

Each axis updates model velocity by acceleration times the snapshotted scale,
then adds updated velocity plus a second acceleration step to the accumulated
vector. Do not simplify that second term away or reassociate float operations.
The original model position times `D_8002EB94` is passed as a real local vector
to `sound_position_set`, together with the object's position array.

After that callback, the current timer and current `D_8002EB94` are reloaded.
The timer is decremented. A nonpositive ordered result optionally invokes
`entity_spawn_callback(index,0,0)` if the current descriptor flags contain
0x2000, then `func_800AFA84(D_80143FC8, original_object)`, then
`entity_transform_apply(state,1)`. NaN timers do not expire. The object pointer
is captured before the callback, so replacing `state->obj` does not redirect
the later descriptor lookup or removal. Callback effects are tested explicitly.

All four callees use ordinary argument registers in the native target. Direct
`jal` caller scanning finds no callers to this callback; no call-table closure
or full-game integration is claimed. Function names do not establish what the
external routines do, so behavioral tests use declared, bounded callback stubs.

## Causal controls and residual

All trials used pinned IDO5.3 and `-g0 -mips2 -G0 -non_shared` plus
`-Wab,-r4300_mul`. Counts are full target-word differences; extras are nonzero
words beyond the target. No rejected control replaces the final source.

| Natural form | O1 | O2 | O3 |
|---|---:|---:|---:|
| Typed arrays + cached scale |106/108 +13|90/108|99/108|
| Same with do/while loop |106/108 +13|90/108|99/108|
| Reload scale in loop |107/108 +12|107/108|96/108|
| Actual velocity-array pointer + cached scale |106/108 +14|108/108|72/108|

Workbench diagnosis was run before further layout experiments. It identifies
structural, register, and frame differences. The final 72/108 residual includes
scale/global-address scheduling, initial loop-counter placement, constant-load
reuse, FP allocation, frame offsets, descriptor address construction, branch
extent, and the shorter body. Relocation-masked workbench similarity is not the
reported score; every word in the claimed extent is compared after GNU linking.
No invented locals or storage were added to inflate the native104-byte frame.

## Verification

- `verify.py`: hashes all23 protected manifest entries, compiles fresh IDO,
  resolves all19 relocations with GNU ld, and compares full target words and
  extents. Missing words count as differences. Assertions require72/108,
  function412, text416, and only one zero alignment word. `verification.json`
  includes source/target/compiler hashes, symbol values and all differences.
- `differential.py`: **5,893 three-way native / linked IDO / host C cases PASS**.
  Covers zero/nonzero and truncated signed modes, paused returns without object
  accesses, exact/adjacent timer thresholds, both flag outcomes, NaNs/infinities,
  subnormals, signed zero, random binary32 patterns, callback order/arguments,
  callbacks mutating timer/step/kind/index/state object, and ABI caller clobbers.
- `host_semantics.c`: **50,000 ASan/UBSan callback-contract cases PASS**, and
  C89 pedantic warnings-as-errors pass. Leak detection disabled for sandbox
  ptrace limitations; this harness does not allocate heap memory.
- Six regression tests cover the host harness and fail-closed interpreter,
  including callback delay slots, caller-save poisoning, unknown opcodes,
  absent callback handlers and branch-likely annulment.
- Independent lane D review replayed the complete72/108 proof and all5,893
  differential cases and approved the source, ABI, record offsets, and scoped
  research result. Review details are in `peer_review.json`.

The interpreter models round-to-nearest binary32; NaNs compare by class.
Signaling-NaN payload propagation, FCSR exception flags, concurrency, and
arbitrary reinterpretation/overlap of the typed records are outside the claim.
Callback stubs are a contract test, not proof of full game-callee behavior.
No private ROM is available; cartridge hash and integration gates were not run.

Reproduce with pinned IDO in `IDO_DIR` and GNU MIPS binutils on PATH (configure
their runtime library path if needed), from repository root:

```
python3 cloud/work/dot_entity_motion/verify.py
python3 cloud/work/dot_entity_motion/differential.py
cc -std=c89 -pedantic -Wall -Wextra -Werror -O2 -DSTANDALONE -fsanitize=address,undefined cloud/work/dot_entity_motion/host_semantics.c -o /tmp/entity-motion
ASAN_OPTIONS=detect_leaks=0 /tmp/entity-motion
pytest tests/conveyor/test_dot_entity_motion.py
```

No protected sources, targets, locks, scorer, generated context, build wiring,
or accepted coverage metrics are modified. Draft research PR only; no merge.

Final CI-style local checks: all161 static locks intact; canonical single
`sound_handles_clear` and three-member `resource_slot_clear` controls MATCH;
full `tests/conveyor` suite: **1,313 passed,41 skipped** with the repository's
`not node_required` selection. Pinned submodules were initialized after an initial missing
`src.scorer` collection error; the final full run completed successfully.
