# func_800A7830: indexed record word setter

## Result

Exact object match: all 11 canonical words / 44 bytes, no differences,
unknown relocations, section-relative masks or nonzero excess. The compiler
emits 4 bytes of zero alignment after the 44-byte symbol. This is not image
or full-ROM verification and grants no accepted coverage.

Source: `cloud/matches/func_800A7830.c`, SHA-256
`448e617d25e0d56e2ecb87b421cb492f3db7c8c1986036a8518a6b9cb09b3f86`.
Recipe is the first-line O2 recipe with the scorer's documented VR4300
multiply erratum assembler option. `verification.json` is a fresh strict
canonical replay. Base master: `a12daa63ae67c8dab3404efb666089c884dadfd4`.

## Reconstruction and provenance

The native entry sign-extends the low 16 bits of argument a0, multiplies the
result by 68, and stores a1 at `0x8012E734 + index*68`. Adjacent native
`func_800A7884` materializes `0x8012E700 + index*68 + 60`, independently
supporting the record base and stride. `func_800A785C` stores the next word
at offset 56. Unknown bytes are genuine layout gaps, not stack padding.

The native body explicitly homes a0 at incoming stack offset 0 and a2 at
offset 8. The third incoming word is unused by the game-memory operation;
it is represented as an unused unsigned 32-bit carrier, not pressure work.
There are no calls, helpers, invented operations, volatile accesses or
optimizer-steering casts. No direct caller or materialized function-address
reference was found by `cloud/work/tools/callers.py`; indirect callers may
exist outside the scanned code. The third word's semantic type is unknown.
The stored word's semantic type is likewise unknown: u32 preserves its bits
without guessing pointer ownership or signed arithmetic.

Only native assembly supplies semantics; the arcade reference tree is not
available in this checkout. Original table bounds are unknown. Defined C
execution requires a valid in-bounds record index. Tests use a synthetic
32768-entry fixture and do not claim this is the game's table size. Negative
indices are checked in the native address interpreter only, never executed
as out-of-bounds C.

## Freshness

The PR1–35 target/source inventory, current accepted single/group claims,
and round7/8 claims/rejected lists contained no claim for this function.
Prior `tiny_A23/inventory.json` lists the address, but no manual C definition
or experiment was found. This is the first native source in that search.
The first natural reconstruction compiled to MATCH, so no arbitrary variant
sweep or rejected optimizer controls were needed.

## Verification

- Scorer sanity: `sound_handles_clear` and all three `resource_slot_clear`
  group members MATCH.
- Fresh canonical replay: full 44 bytes, all resolved words, no masks.
- 32768 host/native cases: every nonnegative signed-half index, random
  full-width stored and unused words, poisoned high index bits on the native
  side, exact expected stack homes and record address.
- Full fixture byte comparison proves no extra host writes outside the
  selected word fields; each call also verifies its record's other 64 bytes.
- 32768 additional native-only negative-index address checks.
- 196608 ASan/UBSan executions: six edge word values at each valid fixture
  index. Leak detection is disabled because the execution environment's
  ptrace prevents LeakSanitizer; address and undefined-behavior checks remain
  enabled. The harness uses static storage and no heap allocation.
- Seven pytest tests pass, including fail-closed unknown-opcode rejection.

Reproduce from the repo root:

```
python3 tools/cloud/review_single.py cloud/matches/func_800A7830.c func_800A7830 --expected-bytes 44 --output /tmp/record-proof.json
python3 cloud/work/dot_record_word_set/verify_semantics.py /tmp/record-semantics.json
python3 -m pytest tests/conveyor/test_record_word_set.py -q
```

## Independent review

Lane E independently reviewed the source, argument homing and layout, and
freshly compiled/linked with IDO and GNU ld. All 11 native words are exactly
identical, with a 44-byte symbol plus 4 zero alignment bytes. E independently
replayed all semantic/sanitizer checks above and approved this exact source
hash. Review explicitly retains unknown table bounds and carrier types;
no extra arguments were introduced beyond the native incoming-home evidence.
Maintainer image and full-ROM gates remain required before promotion.

Aggregate checks: the complete `tests/conveyor` suite with
`-m 'not node_required'` passes after initializing the pinned repository
submodules; environment-dependent tests skip under their existing markers.
`make check-matched` reports all 161 static locked functions intact.
