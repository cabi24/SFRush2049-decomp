# Collision callback match and two bounded research results

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b` (2026-10-05).

## Complete object-match submission

`cloud/matches/func_8010C02C.c` is a byte-identical copy of the independently
reviewed source in `../dot_seed_vector/`. It selects one model/record collision
pair, rejects separated bounding spheres, transforms up to four corners, and
calls the genuine collision-force routine for the first inclusive box hit.

This is **696 bytes of new complete object-match evidence**, not newly
integrated cartridge coverage. Standalone O2, standalone O3, and the O3 group
with all three unchanged real direct callees each match all 174 relocated words
and the exact 696-byte ELF function extent. Independent GNU linking agrees.
The separate submission copy passes the same complete-extent proof and the
stock changed-submission gate. An equality regression binds it to the packet.

The independent review extends the packet's direct-call search: the normal
object-dispatch path loads callback-table `D_80117518` slot 4 and calls it from
`race_setup_2` at `0x800BEDB0`. It supplies the object pointer, a pointer to the
signed-halfword model index, and zero in a2/a3. This supports the genuine four
incoming slots. It does not establish precise source-language types for the
unused slots, complete registration/reachability, or slot-4 reachability through
the separate mode-6 generic dispatch path. No caller or callee was changed.

## Nonmatching research

- `../dot_vector_blend/`: `func_800E8D50` improves from 3/112 to **1/112 differing
  words**, at the exact 448-byte extent. The remaining difference is FP multiply
  operand order. Two own-literal relocation sites are verified by content and
  then compared without masks. All six source/context controls reproduce; a
  wrong-literal negative control is refused. The current packet plus own-data
  suite has **39 tests**, correcting the packet's historical count of 38.
- `../dot_resource_initializer/`: `func_8010D85C` improves from 62/92 to **12/92
  differing words**, at the exact 368-byte extent. The differences are stack
  operands: native frame 88 bytes, candidate frame 64. The native fifth outgoing
  argument conflicts with the accepted four-parameter callee contract and
  remains unresolved. The honest bounded source and genuine-arity control are
  research only; no fake frame padding or extra accepted formal was added.

Neither research source is a matching submission or earns accepted-byte credit.

## Final integration validation

The final source tree passes:

- Protected-path guard and one changed cloud submission: strict MATCH
- Unchanged single/group scorer sanity checks; all 383 static locks intact
- Three collision complete-extent builds plus the separately compiled submission
- Both research replays, including 13,840 resource differential cases and
  100,000 resource host ASan/UBSan cases
- 52 focused tests: 39 vector/own-data, eight resource, and five collision tests
- Independent packet reviews, including 1,033 collision host ASan/UBSan cases,
  eight rejected collision semantic/ABI/extent mutations, and six rejected
  resource semantic/ABI mutations

The complete Conveyor + Cloud suite reports **1,935 passed, 17 failed,
41 skipped, nine deselected, and 25 subtests passed**. All 17 failure messages
are identical to a fresh checkout of the base: 15 locked-single own-section
relocation refusals and two existing builder prose-in-flags failures. That
baseline replay reports 575 passed, 17 failed, and 22 subtests passed. The
integration receipt binds both runs and compares every failure individually.
This is not a green suite/CI claim; no unrelated CI repair is included.

The first aggregate invocation stopped during collection because the new
worktree lacked two pinned submodule checkouts. Their exact committed source
was materialized and the complete suite rerun; no tracked code was changed.
Pinned IDO binaries were supplied at both the configured and conventional
ignored tool paths. The 41 remaining skips are nine missing MIPS cross-GCC
cases and 32 unavailable SDK/ROM/generated/private-input cases.

Collision and vector receipts reproduce in full. Resource semantic receipts,
source hashes, extents and relocated body hashes agree. Its whole-object hashes
vary with source worktree/temp paths; an identical-source control independently
confirms that only `.mdebug` differs and every other ELF section is identical.
Whole-object equality across paths is not claimed.

## Acceptance boundary

This packet changes no production source, caller, callee, shared context,
symbol, target, lock, scorer, compiler flags, build gate or remote builder.
Other matching batches and contract-repair work are separate.

No blob splice, lock migration, full-program shadow, source-image build,
compressed-stream check, full-ROM hash check, gameplay validation or merge is
claimed. Final production gates and merging remain with the independent
checker. Only source, tests, research and verification counts/hashes are
submitted; native objects, raw assembly dumps, ROM bytes, credentials and
private execution logs are excluded.
