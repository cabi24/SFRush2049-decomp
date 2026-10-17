# Race-to-results transition: genuine allocator context closes 428 bytes

Base: `58425072b49e95f0fbe5c4c239988fb4885f7c5c`, refreshed 2026-10-17.
Population: game. New source-exact function: **func_800D5A04, 428 bytes / 107 words**.
No image integration, full-ROM verification, independent acceptance or coverage claim.

## Deliverable

`groups/frontier_slot18_alloc/` supersedes the existing group under its existing
name. All 13 emitted functions are members and claims, with no hidden context
or uncertainty waiver. The old allocator member and all four real roots are
preserved byte-for-byte. Six additional locked source files are copied unchanged
from current `src/blob`; their additional defined helper
`player_conditional_check` is explicitly kept and included as a member.

All 13 strict comparisons pass, totaling 2,548 body bytes. The 12 existing
functions (2,120 bytes) receive no new-match credit. The only newly closed
source is the 428-byte transition. Source body comes from the w27d candidate,
with the current `ShowCountdown` symbol name; there is no novel source-shaping
operation in this function.

## Traced cause and controls

The genuine call at transition offset `+0xF0` is `func_80091B00`, the internal
24-byte message-slot allocator. Its complete accepted group supplies IPA
register-reservation information missing from w27d's six-context recipe.
The transition remains **kept**; making the transition internal was an
unsuccessful diagnostic and is not the proposal.

Procedure-scoped trace, with byte-identical complete objects versus stock:

- External allocator control: initial removed registers v0/v1; temporary
  allocations t6,t7,t8,t9,t0,t1,t2,t3,t4,t5,t6. Strict result 13/107 differing words.
- Genuine internal allocator: initial removed registers v0/v1 and t0–t5;
  allocations t6,t7,t8,t9,t6,t7,t8,t9,t6,t7,t8. Strict result MATCH.

This corrects the earlier w27d claim that the temporary-ring phase was inherited
from procedures compiled before the transition. The trace shows the decisive
per-procedure pool membership. The preserved caller body, loop structure and
allocation operation order do not need speculative phase-changing expressions.
Adding only the current menu-input or countdown groups did not change the
13-word residual. Removing the allocator group from this exact packet restores
that clean 13-word residual. Changing the real operation from 7 to 6 is rejected
with one differing word.

The complete-group audit initially found an omitted definition:
`player_conditional_check` was internal and nonmatching in the inherited six-file
recipe. Keeping that existing helper explicitly restores its strict native body
without disturbing the other 12 members. It is included in all claims here.

## Preserved source qualifications

The allocator's existing `volatile` slot-table declaration and
`func_800D5524`'s documented compiled-out `if (i) {}` remain unchanged. They are
inherited accepted-source shaping, not new discoveries or proof of original C.
Large existing declaration preludes are retained byte-for-byte for source
provenance. No fake return prototypes, synthetic callers, padding arrays,
register constraints, target/scorer modifications or forced-color output are
introduced. The packet proves compilation and relocation identity; it does not
claim new runtime, corruption, concurrency or aliasing testing.

## Reproduce

With the repository's IDO 5.3 toolchain and MIPS GNU binutils configured:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/mat3_ui_transition_20261017/groups/frontier_slot18_alloc
python3 cloud/work/frontier/mat3_ui_transition_20261017/verify.py
python3 -O cloud/work/frontier/mat3_ui_transition_20261017/verify.py
python3 -m pytest tests/cloud/test_ui_transition_real_allocator.py -q
```

Flags: `-g0 -O3 -mips2 -G 0 -non_shared`; canonical whole-group pipeline adds
mandatory `as1 -r4300_mul`, `-Olimit 5000`, and the explicit kept roots.
Normal and optimized Python verification outputs are identical. The dedicated
pytest passed with `REQUIRE_TOOLCHAIN=1`.

The independent GNU proof links the untouched original object with `.text`
anchored on the internal allocator, whose call relocation is section-relative.
Exported/undefined symbols receive their native addresses through ordinary
linker-script assignments. All **428/428 target bytes** then compare equal.
This verifies function relocations without rewriting any instruction; it is not
a complete image-layout or ROM build. This group has no nonempty owned-data
sections and requires no split-section exception.

Optional procedure-scoped trace replay, using already-built workbench instruments:

```sh
IDO_TRACE_DIR=/path/to/instruments python3 cloud/work/frontier/mat3_ui_transition_20261017/trace_verify.py
```

The trace script checks whole-object equivalence in both cases and emits only
register-pool summaries. Objects, raw compiler logs and assembly stay temporary.
`RECEIPT.json` pins source and target-row inputs, not tools or lock files.

## Exact canonical output

```text
Members:
func_800D5A04: MATCH
func_80091B00: MATCH
func_800D5828: MATCH
func_800D5524: MATCH
records_screen: MATCH
players_frame_update: MATCH
func_800B0580: MATCH
entity_audio_update: MATCH
object_type7_create: MATCH
object_type1_create: MATCH
func_800D6348: MATCH
sync_entry_register: MATCH
player_conditional_check: MATCH
```

Zero extra words, unresolved symbols, unverified relocations or errors for every
member. Independent checker owns group installation, image/ROM gates and merging.
