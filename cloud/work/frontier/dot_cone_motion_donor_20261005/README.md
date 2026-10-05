# Cone-motion callback: donor-backed 17-word nonmatch

`func_8010E4E4`, **0x8010E4E4–0x8010E694**, is **432 bytes / 108 words**.
This packet remains **NONMATCH: 17 words differ, all stack-address immediates**.
The candidate has the exact complete ELF function extent. There is no matching,
accepted-byte, source-splice, image, compression, ROM or gameplay claim.

Base: `e24b47d89a0c8ffade1e4c75ad76b9d390a1c232`. Current master, locks,
repository claim files, the existing archive and open draft PRs through #117
were checked before the exact interval was announced. E4E4 remains unlocked.
No other target or accepted source is edited; the parent coordinates the live
reservation. This check cannot exclude unpublished work on another machine.

## Source evidence

The pinned [Rush The Rock source](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139)
provides a concrete new ancestor for the earlier
[`dot_entity_motion`](../../dot_entity_motion/README.md) reconstruction, whose
notes explicitly stated that the arcade checkout was unavailable.

- [targets.c, AnimateCone, lines 1540–1571](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/targets.c#L1540-L1571):
  the genuine signed-short loop index, target pointer, `dt = 0.15`, three-float
  scratch and two-stage gravity integration. In particular, the second gravity
  term remains present after velocity has already been updated.
- [targets.h, Target, lines 126–145](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/targets.h#L126-L145):
  position, orientation, linear velocity and angular velocity are distinct
  concepts. The N64 moves the last two into a separate 24-byte allocation.
- The complete native body and accepted `sound_position_set` establish that
  object offset 20 is a nine-float orientation basis, not a three-float position.
  Offset 56 is the three-float position. The old names are corrected without
  changing offsets or bytes.
- The native callback performs three separately loaded frame-time multipliers
  before the rotation call and one frame-time load afterward. Its immediately
  adjacent accepted `func_8010E694` and other accepted consumers use the same
  volatile scalar view of `D_8002EB94`. This supports preserving those explicit
  reads. It does not prove the original declaration or an asynchronous update
  contract; no concurrent clock behavior is claimed.
- The descriptor table is a 48-byte record array with an unsigned halfword
  flag at offset 18. The selected record pointer is genuinely dereferenced.
  Naming that pointer reproduces the observed full address computation before
  the flag load. Its original source declaration remains unestablished.

The donor is an **algorithmic ancestor**, not an exact original N64 function.
The N64 uses an owned binary32 `0.15f`, a runtime frame-time scalar, separate
motion data and a timer-driven release path. It does not reproduce the arcade
object-definition reset path. `provenance.json` binds the donor revision and
both file blobs/hashes. No unused donor declaration is retained as frame filler.

## What changed and where it stops

Fresh, fixed compiler controls under ordinary O3:

| Source condition | Differing words | ELF bytes |
|---|---:|---:|
| Exact archived entity-motion candidate | 72/108 | 412 |
| Archive with genuine owned 0.15f only | 68/108 | 412 |
| Archive with supported repeated-clock reads only | 42/108 | 428 |
| Final source without its consumed descriptor pointer | 38/108 | 428 |
| Final source with ordinary cached clock | 68/108 | 416 |
| Selected natural source | **17/108** | **432** |

Workbench diagnosis preceded the changes. The earlier broad structural and
allocation residual collapses to frame geometry. The frame is **80 bytes versus
native 104**; the actual three-float scratch starts at **52 versus native 60**;
the object spill is at **68 versus native 96**. The incoming callback argument
homes follow their respective frames. All 17 different immediates are included
in the full comparison; none is ignored or normalized for a matching claim.

O2 is retained as a failed control: 105/108, 408-byte ELF body, with two own-rodata
references still unverified because the changed instruction shape does not
establish the native placement. It is neither a verified-body nor a runtime
proof input. O3 is the sole selected recipe.

No declaration-order sweep, fake arrays, dead reads, dead conditions, new formals,
artificial helpers, stand-ins, keepers, compiler/scorer changes or target changes
were used. The concrete donor evidence does not explain the remaining storage.
**Reopen only for substantive original local/helper/type evidence accounting for
the 24-byte frame deficit and distinct scratch/spill placements.** Another
padding or arbitrary declaration search is not justified.

## Complete compiler evidence

- Exact 432-byte ELF `STT_FUNC` extent with no text prefix or trailing alignment.
- All **19 text relocations** resolved independently by GNU MIPS ld. The full
  108-word comparison yields exactly the recorded 17 differing offsets.
- All four owned literal bytes at `0x801249CC` equal the protected data artifact.
  Twelve zero rodata alignment bytes are reported separately. There is no own
  writable data, BSS or jump table.
- A real six-body O3 context includes the candidate and unchanged accepted
  `sound_position_set`, `entity_transform_apply`, `entity_spawn_callback`,
  `func_800AFA84` and neighboring `func_8010E694`. All five accepted full bodies
  remain strict matches. Canonical relocation plus content-verified own-data
  placement proves the complete context candidate equal to the standalone
  GNU-linked candidate; no relocation masks remain in that comparison.
- This is a focused compiler regression using the existing separate type views,
  not a unified whole-game shared-type model. No helper is altered or credited
  again. The function is a callback; direct J/JAL scanning finds no direct caller.

## Runtime and adverse controls

**5,978** protected-native/GNU-linked/independent-binary32-oracle/unchanged-host-C
cases pass, including **11,956 MIPS executions**. Every instruction executes;
both outcomes of all five conditional branches execute. Cases cover signed
16-bit callback narrowing, paused/cleanup order, both descriptor-flag decisions,
timer equality and adjacent values, both signed zeros, finite/extreme floats,
infinities, subnormals, NaNs, randomized records and selected callback mutations.

Rotation hooks inspect all three angular inputs and the genuine matrix pointer.
A mutation hook changes the clock, timer, original object type/index and state
object pointer. The later release still uses the captured original object while
reloading its current type/index and the current timer/clock. Complete mapped
nonstack memory, exact allowed stack-write locations, stack canaries, O32 saved
registers/FPRs, stack restoration and return address are checked.

The decoder and host fixture extend the prior entity-motion packet; the separate
formula oracle, exact stack-canary rules and new source controls are added here.
The C89 host includes `candidate.c` unchanged and uses UBSan with recovery
disabled. Five compiled wrong-contract mutants are rejected: omit the second
gravity term, replace accumulation, use strict expiry, test the wrong descriptor
flag, and skip final cleanup. Unknown native instructions and redirected stack
writes fail closed. Separate tests reject a wrong literal and a truncated ELF
function extent.

**Limits:** dependency calls use explicit O32 hooks, not executed real helper
bodies. The hook changes only the first three matrix words for observability;
it is not a rotation simulation. The real helpers are checked in compiler context.
Fixtures use accessible nonoverlapping records and valid descriptor indices 2
and 3. The coefficient is fixed to the authenticated 0.15f. Gravity is varied as
a fixture to test dataflow, not a claim that the game mutates its table. NaNs are
compared by class; payloads, hardware FCSR effects, invalid pointers/indices,
concurrency and gameplay are outside the proof. Finite cases are not a universal
behavioral proof.

## Reproduce

From the repository root with the pinned IDO 5.3 and GNU MIPS tools configured:

```sh
python3 cloud/work/frontier/dot_cone_motion_donor_20261005/verify.py
python3 -m pytest -q tests/cloud/test_cone_motion_donor.py
python3 -m pytest -q tests/cloud/test_cone_motion_donor.py \
  tests/conveyor/test_cloud_score.py tests/conveyor/test_cloud_guard.py \
  tests/conveyor/test_cloud_submissions.py
python3 -m tools.conveyor.pipeline.lock check --quiet
```

The verifier freshly compiles, checks the full receipt and never changes it
unless explicitly given `--record`. The receipt binds the candidate, old source,
support code, donor provenance and compiler tools. It contains metadata only.

Validation: **716 scoped tests pass**, zero failures/skips; all **402 static
source locks intact**. Protected-path/diff guards pass. The changed-submission
scanner correctly schedules zero matching jobs for these research-only paths.
No full-suite or remote CI result is claimed. No ROM bytes, raw assembly,
objects, credentials or unrelated private data are included. Independent review,
publication and any later production integration remain with the parent/checker.
