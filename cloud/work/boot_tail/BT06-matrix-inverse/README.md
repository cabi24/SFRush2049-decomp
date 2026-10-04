# BT06 affine matrix inverse

Central claim `c37a1d35` assigns the fresh whole `80024D74` body,564 B, on
`dot/boot-tail-bt06-matrix-inverse`, explicitly stacked on frozen sample-pair head
`24ff33d5dd7e27ca2d957cfbd822959ee72a2a43`. The existing isolated repository and
pinned compiler are reused. This packet is outside the previously frozen cut12.

**MATCH: all141 relocated words /564 B**, with one correctly named ELF function
symbol at offset0 and exact564-byte size. No masks, unresolved/unverified fields,
errors or nonzero excess words. Section alignment is not counted as body bytes.

## Actual source, ABI and operation order

The function has exactly two genuine pointers: output then read-only input,
with no scalar/float parameters or useful return value. Each points to48 bytes:
a row-major3x3 matrix followed by three translation floats. Native accesses span
aligned offsets0..44. Actual caller `1D5C0` builds all twelve input floats at
sp+24 and passes the output object's+72 region; the observed call uses distinct
input/output storage. The56-byte frame and saved floating register are compiler
consequences of real cofactor/reciprocal values, not invented parameters or locals.
There are no callees or external/local data relocations. Literal1.0f is immediate.

The native body computes three first-column cofactors before any output write.
Its determinant combines `m02*c` with the rounded sum `m00*a + m01*b`, takes one
reciprocal, then stores inverse columns in order0,1,2. Later input entries are
read live after earlier writes. Each translation is a left-associated pair of
subtractions starting with `-input_translation0 * output_row0`. The complete
native floating operation/read/store sequence was reviewed, including signs,
operand ordering and scalar rounding boundaries.

The public CC0 family lead is
[snd_math.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_math.c),
revision `78d2e16e4905fc675952162d331c24d5198b2687`, blob
`fe8a671021560d3ca9b59efb0d312d77b1d1154f`. Its `salInvertMatrix` expression form
is adapted to the independently proved native type/name; modern line comments
are removed for C89. CC0 license blob is
`0e259d42c996742e9e3cba14c677129b2c1b6311`. These pinned sources are provenance,
not a substitute for native ABI, layout or strict proof.

## Domains, aliasing and singular inputs

For the routine to provide a useful mathematical inverse, the objects must be
valid aligned matrices, distinct, and have suitable finite inputs with a nonzero
**computed binary32** determinant and representable intermediate/reciprocal
results. Mathematical nonsingularity alone is insufficient near cancellation.
Numerical stability for arbitrarily ill-conditioned matrices is not promised.

There is **no singularity branch or fallback** in native code or this source.
The division executes even when the computed determinant is zero. Under the
ordinary nontrapping IEEE model this can produce signed infinities and NaNs;
enabled target FP exceptions can behave differently. No invented safe result,
clamp or error return is introduced. Portable C89 zero-division/overflow behavior,
arbitrary FCSR modes, signaling-NaN behavior and payload preservation are not
claimed by the host tests.

Exact same-object aliasing is permitted by the pointer types and was tested
against the actual native access order. Its later input loads see earlier output
stores, including translation writes affecting later translation reads. Thus
in-place execution is **not generally a mathematical inverse**. The source does
not add a hidden input snapshot or assume `restrict`. Partial overlap between
separate matrix views is not independently established; the actual caller uses
distinct storage. Out-of-range, unaligned or invalid object pointers remain outside
the supported source domain.

## Bounded matching process

The initial hand-written native-operation spelling has114/141 differing words
but the correct564-byte ELF extent. Unchanged workbench diagnosis ran before
one directed control. The pinned family's natural expression spelling reproduces
IDO's complete native register/operand schedule and closes all141 words. It is
then frozen. Two total forms, O2 followed by O1, no flag or declaration sweep.

The retained O1 control is140/141 with13 nonzero excess words and616-byte ELF
extent; it is explicitly rejected. Two final rows and four initial/directed rows
bind exact source hashes and function sizes. `diagnosis_summary.json` records
only hashes/classification counts. Native listings and objects stay temporary.
No fake formal, keeper, padding local, assembly, volatile trick, target change or
altered function boundary is used in the matching source.

## Independent numerical and memory tests

The actual C89 source is compiled at host O0 and O2 with ASan/UBSan, no fast math
or floating contraction, and an explicit default nontrapping FP environment with
round-to-nearest. A separate algebraic model rounds every operation to binary32
and preserves the reviewed native store/read order. The generated fixture bits
are mathematical test data, never copied image bytes.

There are623 fixtures, each tested with distinct objects and exact same-object
aliasing at both optimization levels: **2,492 executions**. They include identity,
42 signed power-of-two diagonals,512 bounded integer matrices,24 near-singular
families,8 zero/singular matrices and36 infinity/quiet-NaN/subnormal/extreme-value
fixtures. Bit comparisons preserve signed zero and infinity signs; NaN comparisons
check classification only. Input preservation and guards around both objects are
checked. For555 regular distinct-object fixtures, independent double-precision
matrix-composition checks also confirm the approximate inverse/translation.

Twenty-one fixtures have a computed zero determinant, including thirteen of the
near-singular family. The harness observes the expected host divide-by-zero flag
and nonfinite results rather than hiding them. These special-value observations
are bounded host IEEE checks, not target hardware or exceptional-FCSR acceptance.
The exact target instruction equality and ELF-size proof remain separate.
Only LeakSanitizer is disabled under ptrace. Results are summarized in
`test_results.json`.

Run `verify.py`, `verify_controls.py` and unittest discovery for `test_*.py`.
Fresh preflight validates all439 target extents/99,120 B, protected manifests and
the preexisting getter. The canonical submission gate,161 static locks and
whitespace pass. Central ledgers/D10, earlier sources, targets/scorer/compiler/
symbols/layouts/locks/specs, runtime image/farm, production gates, accepted800D1248
and restricted helper work are unchanged. No ROM, raw native dump, object,
credential or unrelated private data is included. Paired actual-source/ABI review
precedes central integration and exact-head CI; the independent checker alone
merges.

Independent paired review PASS binds immutable source commit
`fd1d7b439dfc70e14c0f3e90d4a18e95c267936c`, tree
`0b17eea2def85a29b20b8e247fa060f35c85241e`. Exact Git sources reproduce both
final rows and four archived controls; all2,492 host sanitizer executions pass.
The reviewer additionally decoded the native float/load/store sequence in a
private register/memory model and independently reproduced all1,246 distinct/
exact-alias fixture results, including translation overwrites and computed-zero
determinants. Native Matrix48/O32 assertions and the entire caller/body ABI were
checked. No singularity fallback, hidden alias snapshot or exceptional-FP safety
claim is introduced. `independent_review.json` contains numerical/source findings
only, without raw target words or instruction dumps. Matching source is unchanged.
