# Resource initializer research: func_8010D85C

**NONMATCH research only; claims remain empty.** Fresh master
`a12daa63ae67c8dab3404efb666089c884dadfd4`. PR1–35 source/claim inventory,
accepted locks, single/group submissions and round7/8 rejection notes were
checked before selecting this previously unimplemented body.

## Reconstruction and useful findings

Retail code conditionally initializes the object at `node + 0x0c`. A zero signed
halfword input calls `entity_transform_apply(node, 1)`; a nonzero global
`D_801170FC` skips all work. Otherwise flag bit 0 gates one-time initialization.
The source preserves the timer write, ordered substring searches, three-way
resource selection, signed-byte count argument, handle store, suffix-byte
conversion, flag update, callback installation and zero time store.

Field names and subsystem labels are hypotheses; the callback and callee names
are existing repository symbols, not independently verified names. The arcade
reference checkout is absent, so no direct arcade equivalent is claimed. An
old mapping associates this address with obstacle avoidance; the actual call
and memory-access evidence supports the neutral resource-initializer description
used here, rather than adopting that mapping as established fact.

Two important details are now tested:

- The second `sound_bank_load` argument is a halfword output. Its native body
  writes `sh` through that pointer (0x800B107C and 0x800B10AC). It is not an
  uninitialized float vector. The output is unused by this caller. The fifth
  stack argument is visibly 1 in the caller, though this callee does not read it.
- The slot index is read **after** the loader returns, and the text pointer and
  flags are reread afterward. A combined `table[state->slot] = call()` expression
  permits evaluating the destination before the call. Splitting return capture
  and indexing preserves retail behavior even when callbacks change the slot.
  The differential harness actively changes slot, text and flags to test this.

The O32 node size is 24 bytes, state 112 bytes, and output slot stride 68 bytes.
The relevant state offsets are flags 4, slot 14, timer 90 and text 108. Node offsets
are variant 4, state 12, time 16 and callback 20. Unknown byte arrays denote observed
layout gaps, not invented stack padding. The global handle view begins at
0x8012E738; the original allocation bounds and handle's semantic type are unknown.
An integer bit carrier retains the complete native 32-bit return/store.

## Residual and experiments

Canonical `-g0 -O2 -mips2 -G 0 -non_shared` gives **73/92 differing words**.
Target extent 368 bytes; candidate ELF function 356 bytes plus 12 zero alignment
bytes. Thus matching text-section length alone would hide a 12-byte body deficit.
All candidate relocations resolve and independently GNU-linked words agree with
the scorer. Full target/candidate words and positional differences are saved.

Workbench diagnose reports structure mismatch, three fewer true instructions,
64-byte versus 88-byte frame, and different temporary/register allocations.
No fabricated locals, frame padding, helper calls, argument changes or pointer
integer roundtrips were introduced to imitate the target's frame/registers.

`controls.json` records final-source O1/O2/O3 controls. O3 improves the positional
score to 62/92 but remains nonmatching; this is not promoted as verified source
or a proven original compiler setting. O1 produces 89/92 plus 8 excess words and
a boundary-limited HI16 diagnostic. Natural nested-default selection and local
reuse controls were also explored, without resolving the underlying structure.
The final source keeps distinct names for distinct semantic values and explicit
post-callback sequencing. These rejected controls are not matching claims.

## Verification

- 13,840 three-way cases compare complete native and compiled-candidate execution
  with host C. Cases cover all guard combinations, return branches, byte extremes,
  signed-byte wrap, high incoming argument bits, every fixture slot and arbitrary
  32-bit handles. Loader mutation forces slot/text/flags rereads.
- Native interpreter implements delay slots and branch-likely annulment, poisons
  caller-saved registers, checks stack/callee-saved restoration, enforces mapped
  aligned memory and rejects unknown instructions/calls. Changed bytes and call
  order/arguments are checked; regression tests include invalid controls.
- 100,000 host cases pass ASan/UBSan; host expected-state checks include all object,
  state and output-table bytes. Leak detection alone is disabled for this ptrace
  environment. The ordinary regression also runs 100,000 UBSan cases.
- Four new pytest tests pass; native O32 structure sizes compile-check with IDO.
- Independent lane F source/ABI audit, complete GNU link/word replay, all 13,840
  differential cases, 100,000 ASan/UBSan cases and four pytest tests PASS as research.
- All 161 static locked functions remain intact.

Reproduce with pinned `IDO_DIR` and GNU MIPS tools in PATH:

    python3 cloud/work/dot_resource_init/verify.py /tmp/resource-proof.json
    python3 -m pytest tests/conveyor/test_resource_init_research.py -o addopts='' -q

Research verifier success means the bounded behavior and documented NONMATCH
reproduced. Callback stubs model effects rather than execute complete game
subsystems. The fixture uses eight nonnegative slots; original table bounds,
negative-index validity, function-pointer signature and full-game behavior are
unverified. No private ROM is available, so image splice, compressed identity,
full-ROM SHA-1 and `make test` were not run. Protected source, locks, scorer,
manifest and build wiring are unchanged. No accepted coverage or ROM claim.

Full repository conveyor run passed: **1,311 passed, 41 skipped, 9 deselected**
with the pinned submodules initialized. The four added tests are included.
