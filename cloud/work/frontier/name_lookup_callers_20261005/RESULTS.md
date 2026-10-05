# Name-lookup callers: ABI-blocked diagnostic

Base: `55ddfc6b53d96d4c5dcaaa8891dedafb037b3994`, 2026-10-05.
Claims: **none**. No match submission, splice, image claim, or ROM coverage.
Canonical source, targets, symbols, flags, locks, context, and builder settings
are unchanged. This packet is for independent contract review.

## Result

- `collision_sound_play` (`0x800B362C`, 216 bytes): the new research source is
  byte-identical standalone and in a two-file group with the byte-unchanged
  accepted `func_800B24EC`. Exact ELF sizes are 216 and 364 bytes respectively;
  every relocation is resolved, with no masks, uncertainty, or owned data.
  **This is not an accepted source match.** The preserved fifth actual argument
  conflicts with the accepted callee's four-formal definition.
- `physics_collision_test` (`0x800B9194`, 240 bytes): the prior B4 source still
  differs at five of 60 words. Its exact ELF extent and all relocations verify.
  Neither the fresh volatile-byte evidence nor bounded typed reconstructions
  improve on that prior result. The same call-contract blocker also applies.
- Accepted callees regress: `func_800B24EC` is 0/91 standalone and in the real
  two-file diagnostic group; `pool_linked_list_init` is 0/32 at its recorded O2
  flags. These are unchanged sources, not new claims.

## What actually improved

The old B44 collision source reproduces its 29/54 O3 residual. Adopting the
accepted lookup callee's `volatile u8` table-count declaration gives 13/54.
Native code explicitly forms the count address before its unsigned-byte load.

The remaining collision difference was source structure, not an allocator
mystery: the old right-associative chain of five signed-halfword assignments
emits a reload and reverses the write order. Five ordinary field assignments
in native field order remove that reload and give 0/54. There is no dummy
read, padding, extra argument, fake helper, register pressure, or assembly.
The fifth argument was already present in the native caller and is retained.

The source now also uses the accepted `NameEntry` return type, a `char *` name,
and an `s16` output index. The returned record is explicitly viewed through its
observed width/height fields. These genuine pointer-type corrections preserve
0/54; they do not repair arity.

## Why byte equality is insufficient here

Both owned native callers store the full word 1 at outgoing stack offset 16
before calling B24EC. Collision does so at function offset 104 before the call
at offset 128; physics at offset 168 before the call at offset 192.

Across the protected targets, B24EC has 32 direct call sites in 16 functions.
Two independently inspected `sfx_position_3d` sites supply zero at the same
fifth slot. This is evidence that the slot is intentional, not permission to
remove it or invent a meaning for a new formal. `tiny_A51/STATUS.md` already
recorded the same uncertainty for `func_800EF5B0`.

The accepted callee consumes four inputs and has a 104-byte frame; it has no
direct stack-relative loads at or beyond the fifth incoming argument. Its
current source defines four formals. An unused fifth original formal cannot
be excluded from callee machine code alone, and its original declaration and
meaning have not been established by this investigation.

The research caller uses an explicitly **unprototyped** declaration. IDO emits
identical bytes both alone and with the unchanged real callee, but that does
not establish a compatible C source contract. In particular, supplying five
arguments to the four-formal definition is not made valid by hiding the
prototype; narrow formal types/default argument promotions are another reason
not to treat this as an accepted shared declaration.

A negative source control restores the actual four-formal prototype while
leaving the observed fifth actual in place. IDO rejects it with:

> The number of arguments doesn't agree with the number in the declaration.

The only next acceptance step is evidence-backed reconciliation of the real
caller/callee declaration, with independent review and revalidation of the
callee and its affected callers. This lane does not alter the canonical callee,
remove the native fifth argument, add a conjectural formal, or claim a match
across the unresolved contract.

## Physics residual and stop

Fresh O2 and O3 runs both reproduce B4's 5/60. The differing positions remain
`+0x70/+0x78` (global address setup versus name-table base setup) and
`+0xA4/+0xA8/+0xB8` (name load versus fifth-argument setup).

A typed pool/array reconstruction gives 10/60 with name setup inside the
conditional and 12/60 with setup before it. Compact block spelling gives
15/60; postincrement arguments give 26/60 plus one extra nonzero word;
unsigned output halfwords leave 12/60. An equality-based clear-loop control
changes its geometry substantially (45/60 plus 11 extra words) and is not
retained. No new source is better than B4. The previous packet had already
examined its schedule residual in depth, so this lane stops instead of
repeating a formatting/allocation search.

Workbench diagnosis was run before these source-form controls. Its raw target
ELF intentionally lacks relocation records, so relocation-layout warnings and
masked counts are not acceptance evidence. Full relocated word comparison,
ELF extent, and uncertainty checks are recorded by this packet's audit.

## Reproduction and checks

From the repository root with the pinned IDO 5.3 and MIPS toolchain available:

```sh
python3 cloud/work/frontier/name_lookup_callers_20261005/audit.py \
  --output build/name_lookup_callers/fresh-verification.json
REQUIRE_TOOLCHAIN=1 python3 -m pytest \
  tests/conveyor/test_name_lookup_caller_contract.py -q -rs
```

The saved `verification.json` contains compiler/source/target hashes, exact
extents, full relocation status, the direct-call inventory, the four-formal
negative control, and empty claims. All compiled objects, private target
assembly, controls, and raw comparisons remain ignored build artifacts.

The five scoped tests pass. They include an extra-zero declared-extent control
that the ordinary padding-tolerant scorer accepts but the supplemental extent
check rejects, plus a wrong-callee relocation rejection. The supplemental
checker does not modify or weaken the repository scorer.

The ordinary scorer sanity commands also print `MATCH` for
`sound_handles_clear` and every member of `resource_slot_clear`. An adjacent
run of the 630-test `tests/conveyor/test_cloud_score.py` module passes 610 and
fails 20 existing locked-single checks on owned-data relocations that this
scorer cannot verify. The scorer, test file, accepted sources, and lock inputs
are unchanged from the pinned base; no unrelated repair is attempted.

Full aggregate CI and its fresh-master comparison belong to the integration
lane; this packet makes no full-suite or ROM claim. In this cloud checkout,
`frontier show` cannot run without the private derived `build/blob_layout.json`.
Protected section extents and integrity-checked symbols supply the recorded
native identities; no layout was guessed or regenerated.

## Prior evidence

- `cloud/work/near_miss_B44.md` and `near_miss_B44/collision_sound_play_seed.c`
- `cloud/work/near_miss_B4.md` and `near_miss_B4/physics_collision_test_best.c`
- `cloud/work/tiny_A51/STATUS.md`
- `cloud/work/frontier/w2d/RESULTS.md`, accepted `src/blob/func_800B24EC.c`
- `docs/plans/2026-10-04-frontier-plan.md`: matched callees are revisited when
  genuine callers expose unresolved source contracts
