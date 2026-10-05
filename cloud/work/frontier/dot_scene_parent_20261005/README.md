# Scene-parent search: full natural-C matching candidate

`func_800A7BF8`, **[0x800A7BF8, 0x800A7C9C), 164 bytes / 41 words**.
Base: `e24b47d89a0c8ffade1e4c75ad76b9d390a1c232`.
Initially scouted on `31b2799e`; rebased after unrelated PR #109 integration.
The selected target, symbols, accessor sources, archived seed and scorer are unchanged,
and the target remains unlocked. The complete proof was rerun on this base.

Ordinary IDO O3 and O2 both report strict **MATCH**.
**Accepted-byte and ROM-coverage gain: zero.** Review and integration remain separate.

## New source evidence and result

The only preserved full body found for this target was an old m2c/permuter seed.
It uses a signed-half loop counter and raw 68-byte table arithmetic. A fresh replay
is 35/41 different, with an incomplete 160-byte ELF function. Merely correcting
that counter produces 34/41 at 168 bytes, so that fix alone does not establish a
match.

The protected native body instead has a full-width record counter, signed-half
input/result, and two distinct traversals: the first child of each candidate owner,
then that child's sibling chain. Accepted `func_8008AE2C` and `func_8008AE10`
independently corroborate the scene-record stride 68 and child/sibling offsets
22/24. This is **N64-specific source reconstruction**. No complete arcade donor
was found or claimed. The earlier generic function name does not establish any
additional game semantics.

The complete typed source uses a real local record pointer, actual first-child and
sibling values, a widened next index, and a consumed search-key snapshot. No local
is padding or an unused pressure variable. No dead reads, empty predicates,
unsupported volatile, extra formals, stand-ins, synthetic keepers, inline assembly,
or protected compiler/scorer/context changes are present.

Twenty bounded natural C forms (including the final candidate) were explored,
plus the O2, archived and accepted-accessor controls; one malformed generated scratch control failed before
compilation and was corrected. Their useful fixed comparisons are replayed by
`verify.py`:

- Preserved m2c source: 35/41, 160-byte symbol.
- That same source with only the full-width counter correction: 34/41, 168 bytes.
- Final typed source with the named record replaced by direct array field access:
  11/41, exact 164-byte extent. Workbench identifies an a1/a2 allocation exchange.
- Final genuinely dereferenced record pointer: strict MATCH, 164 bytes.
- Real unchanged accepted accessor calls replacing the field reads: 37/41,
  176-byte extent and two nonzero excess words. Both accessor bodies stay MATCH.

The last control is negative evidence for that accessor composition. The record
pointer comparison isolates a source-level allocation effect; it does not recover
an original declaration. No whitespace, compiler, or forced-register sweep was
used. Raw compiler objects and instruction listings remain private build scratch.

## Recovered contract

Starting at record zero, return the first record whose child/sibling chain contains
the signed-half search key. The outer index is full-width and narrowed only on
return. Nonpositive record counts return -1.

The equality check occurs **before** testing the -1 terminator, at both first-child
and sibling positions. Searching for -1 therefore returns the first applicable
owner rather than necessarily reporting failure. The source intentionally preserves
that native detail. Negative non-sentinel links can be safely observed only when
equality returns before dereference; ordinary traversal requires valid accessible
indices and terminating chains.

Direct native calls occur twice in `transmission_shift`, at 0x800ABC34 and
0x800ABC98. That caller is not reconstructed or executed by this packet. The
routine has standard O32 argument/return behavior, no stack frame and no calls.
It stores the incoming full argument word in its caller-provided stack home before
narrowing. It leaves scene records and the count unchanged.

## Verification

- Complete 164-byte ELF symbol, all 41 words, all six HI16/LO16 relocations, and an
  independent GNU link at the native address. Both global addresses are resolved.
- Twelve zero bytes of section alignment are separately checked outside the
  function. No own data, strings, literals, tables, or hidden executable prefix.
- Genuine three-body O3 regression: candidate plus both **unchanged accepted**
  child/sibling accessors remain exact. Their separate historical type views stay
  intact. This is a shared-record regression, not the complete caller closure.
- **9,220** protected-native/GNU-linked/independent tuple-oracle/unchanged-host-C89
  cases. **18,440 native executions** cover all 41 instruction offsets and both
  outcomes of all six conditional branches.
- Exhaustive all finite sibling graphs and all child choices through three nodes,
  then deterministic larger graphs and long chains through 512 records; negative,
  zero and extreme counts; signed-half input truncation; duplicate owners; early
  and late matches; absent values; sentinel equality; safe negative-link
  short-circuits; shuffled sibling order; and unrelated record bytes.
- Full record/count preservation, exact argument-home write, stack canaries and
  all O32 saved registers are checked. Host code is the unchanged submitted C,
  compiled as C89 with UBSan. The independent oracle operates on child/sibling
  tuples; the interpreter executes real protected/linked instructions.
- Five compiled wrong-contract source controls are rejected: wrong owner index,
  omitted first-child match, omitted sibling traversal, sentinel tested before
  first-child comparison, and wrong failure result. Unknown instructions fail
  closed. Tests also reject redirected argument-home writes.

The final focused packet/scorer/integrity/submission/guard suite reports
**733 passed, zero failed or skipped**. Both scorer sanity examples pass, and all
**402 static locks** are intact. Protected-path and changed-submission checks are
run on the committed final tree. No full-suite or remote CI result is claimed.

Full-object hashes are retained as provenance. IDO's nonallocated `.mdebug` can
encode build paths, so portable replay compares strict full-body/link results,
relocations, extents, actual source/context hashes and behavioral receipts instead
of pretending the complete ELF file is path independent.

## Reproduce

From repository root, with pinned IDO and GNU MIPS binutils:

```sh
python3 tools/cloud/score.py fn cloud/matches/func_800A7BF8.c func_800A7BF8 \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 cloud/work/frontier/dot_scene_parent_20261005/verify.py
python3 -m pytest tests/cloud/test_scene_parent_contract.py -q
```

Only source, tests, reproduction code and text receipts belong in this submission.
No ROM bytes, raw assembly dumps, binary objects, credentials, private data,
production source, target manifest, shared headers, keep recipes or locks change.
Finite fixtures do not prove arbitrary corrupt/cyclic graphs, concurrency, parent
indices over 32767 or gameplay. Full-game shadow compilation, source-image,
compression, ROM SHA-1, merging and acceptance remain with the independent checker.
