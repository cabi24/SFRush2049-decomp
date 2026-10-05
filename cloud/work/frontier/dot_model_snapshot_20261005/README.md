# Model-to-reckon snapshot: donor-backed strict match

`func_800D4DFC`, **0x800D4DFC–0x800D4EF4**, is a complete **248-byte / 62-word**
matching candidate. The unmodified IDO O3 and O2 pipelines both report plain
`MATCH`, including the function-owned floating coefficient. Accepted-byte and
ROM-coverage gain: **zero**. No splice or cartridge build is claimed.

Base: `cc4d5fdd`. Master was fetched before work. Open draft PRs 98–104,
repository claim files, current locks and frontier waves were checked. This
range was announced before edits and does not overlap the other active lanes.
The check cannot reveal unpublished work on another machine.

## Source evidence and the new lead

Pinned donor: [historicalsource/rushtherock at 845329d7](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139).
The exact file blob IDs and SHA-256 hashes are in `provenance.json`.

- `game/communic.c::mcommunication`, lines 90–102, supplies the distinctive
  sum of two rear-wheel angular-velocity/radius products and the mph conversion.
  The N64 stores a signed halfword and uses four times that coefficient,
  `2.72727275f`, rather than the arcade floating mph field. Quarter-mph units
  are an inference from this scale, not a separately established consumer contract.
- `game/reckon.c::game_reckon_all` and `update_reckon_base` supply the genuine
  suspension/shadow-array snapshots and vector-copy organization. This N64 body
  is an adaptation; no single complete arcade function is claimed as its donor.
- `game/vecmath.h`, line 21, defines the actual `mveccopy` three-component macro.
  Its expansion is used without adding statements, locals, hidden reads or helpers.
- The native neighboring accepted `func_800D4D84` writes the three base vectors
  at 1844, 1856 and 1868 from the acceleration, scaled velocity and position.
  It corroborates that the old A98 `state_c[9]` is three vectors, not a matrix.
  The accepted `menu_control_settings` source independently corroborates
  suspension compression at 1484 and position snapshots at 1868/1940.

The historical A98 source freshly reproduces **4/62** differing words. Replacing
its fake external literal with the correct owned float preserves that residual.
The actual donor vector macro closes it immediately. A bounded control keeps
ordinary separate statements for the first two vectors and the authentic macro
only for the final vector: it also matches. Expanding all three macros into
separate source statements reopens the residual. The full controls are generated
and freshly compiled by `verify.py`; no blind layout or register sweep was used.

The mechanism is source grouping inside the authentic macro: the compiler's
last-vector FP temporary choices differ from A98's hand-ordered 8/7/6 assignments.
The macro provides a source-backed form, not proof that the original N64 source
used this exact spelling. Opaque fields describe observed record layout, not
artificial stack padding. No volatile qualifiers, dead conditions, extra formals,
stand-ins, recipe changes, or protected-file changes are present.

## Recovered operation

The function:
1. Copies the nine-float orientation from offset 748 to 1952 through the real
   accepted `math_utility` matrix-copy callee.
2. Computes the signed scaled-speed snapshot at 1880 from the two rear wheels.
3. Copies four suspension values 1484→1884 and four shadow-distance values
   1516→1900, using the natural paired loop.
4. Copies three base vectors 1844→1916, 1856→1928 and 1868→1940.

The only direct native caller is accepted `players_race_update`, at call site
`0x800D50B0`. It traverses six 2056-byte model records and applies its existing
in-game/player-state guards. This packet does not change the caller or claim
indirect-call exhaustiveness.

## Verification

- Complete ELF function symbol: 248 bytes; independently GNU-linked equality
  of all 62 words. All three relocations resolve, including the actual matrix call.
- All four owned literal bytes at `0x801241A4` equal the protected data artifact.
  Eight text-alignment and twelve rodata-alignment bytes are zero and excluded
  from the claim. No other owned data or BSS occurs.
- Genuine five-body O3 regression: candidate, `math_utility`, its actual caller
  `players_race_update`, base-vector setter `func_800D4D84`, and
  `battle_mode_setup` all remain strict matches at exact symbol extents. The four
  accepted sources and their exported status are unchanged. This tests separate
  existing type views and is not a unified full-game shared-type model.
- **4,096** deterministic cases compare native target execution, independently
  GNU-linked execution, an offset-based arithmetic oracle and the unchanged
  candidate compiled as host C89 with UBSan. Complete 2056-byte outputs agree.
- All **62** target instructions and all **19** instructions of the actual native
  matrix-copy callee execute. Delay slots, call arguments, saved registers,
  untouched object bytes and stack canaries are checked. The host uses an ordinary
  nine-float copy implementation of that callee's already-established contract.
- Five wrong-contract source controls are rejected: coefficient, rear-wheel
  radius, shadow-array source, final position vector and reversed matrix copy.
- Eight focused tests replay code, context, source controls, 512 runtime cases,
  all five mutations, caller discovery and fail-closed decoder behavior.

Run from the repository root with IDO and GNU MIPS binutils:

```sh
python3 cloud/work/frontier/dot_model_snapshot_20261005/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_dot_model_snapshot.py -q
```

`--write` is the explicit receipt-regeneration mode for reviewed changes.
Normal verification does not update the saved receipt. Native instruction streams,
objects and raw disassembly stay in temporary or ignored build storage.

## Limits

Arithmetic cases use finite binary32 values and signed-halfword-range conversion
results; the tests do not establish signaling-NaN behavior, FCSR/exception state,
out-of-range float conversion, arbitrary pointer validity, hardware timing or
full gameplay. The readonly target/literal manifests are validated before use.

This is a reviewable matching candidate, with no production source/lock changes.
Full-game shadow-unit, source-built image, compression, ROM SHA-1 and final source
admission remain with the independent checker. No merging or CI monitoring is
performed by this worker.
