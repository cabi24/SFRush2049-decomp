# Low-cylinder proximity: func_8010C588 (NONMATCH research)

Complete target at 0x8010C588–0x8010C6C8: 320 bytes, 80 instructions.
The new natural typed source differs in **43/80 complete resolved words**.
Candidate function size is **316 bytes (79 words)**; ELF text is 320 bytes after
one zero alignment word. Native frame32 versus candidate24. This is research
only: `claims: []`, no accepted coverage, integration, splice or ROM claim.

## Useful reconstruction

The signed halfword selector indexes a signed enabled byte at
D_8014AA3A + index*2056 and a player position at D_80152818 + index*952 +8.
The record types are offset views with unknown intervals, not proven complete
class definitions. Array bounds remain unknown; valid accessible selected
records are a precondition. The harness supplies indices0–3. No direct arcade
equivalent is established because the arcade checkout is absent.

The radius is read before the disabled early return. Disabled entries return0
without reading the origin/position or writing the optional output. Enabled
entries snapshot the three origin values, calculate player minus origin, add
3.5f to the supplied radius, and form z*z + x*x. The optional output receives
that sum minus radius*radius, including on failed tests. A strictly positive
radial error rejects; the vertical interval is strictly **(-2,4)**, not the
(-2,18) interval of neighboring func_8010C448. Radius is not clamped: negative
inflated radii still get squared. IEEE NaNs retain native ordered-comparison
behavior, so a NaN radial error alone does not reject an otherwise valid Y.

All inputs needed by the result are captured before the output store. Output
may alias origin components, radius, or player components; tests check these
cases. No concurrency, input/record byte reinterpretation aliases, signaling
NaN payload propagation, alternate FCSR rounding or exception flags are claimed.

Historical heads_B10 used volatile position plus byte-stride pointer casts to
reach55/80. This candidate uses ordinary typed arrays with no volatile or
pointer laundering, fake calls, local padding, invented parameters, or pressure
operations. Field-by-field struct position was substantially less faithful; real array locals
reproduce the native stack vector accesses and give45/80. Naming the genuinely
used radial-error scalar preserves the subtraction and gives43/80. Natural
result/index declaration ordering did not improve it. Struct assignment gave
69/80 and a40-byte frame. A negative-form bounds control was rejected because
it would change NaN behavior. Existing historical research is unchanged.

Workbench diagnosis confirms structural/scheduling/FP residuals and an8-byte
frame deficit. No artificial storage was introduced to hide it. The first32
words after entry are identical except the frame and branch extent; downstream
FP scheduling, final boolean register and branch shape remain different.

## Evidence and tests

- `verify.py`: independently hashes all23 protected manifest entries, directly
  compiles pinned IDO, resolves all relocations with GNU ld, and compares every
  full target word. Missing words count as differences; no masks or omitted
  extent. It asserts expected NONMATCH43/80 and exact ELF function/text sizes.
- `differential.py`: **11,430 three-way native-instruction / linked-IDO / host-C
  cases PASS**. Includes nine output arrangements, signed enabled values,
  radius/sign/vertical boundaries with adjacent floats, infinities, subnormals,
  quiet NaNs, disabled access behavior, and deterministic random bit patterns.
- `host_semantics.c`: **100,000 ASan/UBSan cases PASS**; C89 pedantic warnings
  pass. Leak detection disabled only for sandbox ptrace (no heap allocation).
- Final CI-style repository suite: **1,311 passed,41 skipped,9 deselected**;
  all161 static locks intact; strict single and three-member group controls MATCH.
- Four pytest regression tests cover the host harness and fail-closed emulator,
  including branch-likely annulment.
- The small fail-closed interpreter was shared from the independently studied
  C448 investigation. The C588 target, linked candidate, three-way harness and
  expected upper bound are separate. It is not a hardware emulator certificate.

Reproduce from repo root with IDO_DIR pointing at the pinned IDO5.3 directory,
GNU MIPS binutils on PATH and their runtime library path configured:

```
python3 cloud/work/dot_low_cylinder/verify.py
python3 cloud/work/dot_low_cylinder/differential.py
cc -std=c89 -pedantic -Wall -Wextra -Werror -O2 -DSTANDALONE -fsanitize=address,undefined cloud/work/dot_low_cylinder/host_semantics.c -o /tmp/c588
ASAN_OPTIONS=detect_leaks=0 /tmp/c588
pytest tests/conveyor/test_dot_low_cylinder.py
```

Source starts from fresh master a12daa63ae67c8dab3404efb666089c884dadfd4.
No accepted source, lock, target, scorer, generated context, build wiring or
coverage metric changes. No private ROM/image available; full-ROM SHA-1 and
integration gates have not run. Draft research only, no merge requested.
