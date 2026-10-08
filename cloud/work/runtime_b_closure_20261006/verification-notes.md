# Portable packet verification

The closure C, schema, group recipe, and all four standalone source packets are
unchanged from their completed research baselines. Packaging only adds a portable
read-only assembler/proof comparison entry point, test gates, and explanatory
research notes. The original independent D328 review remains historical evidence;
the new replay does not represent a new independent admission audit.

A fresh exact-source build and genuine whole-closure replay passed with the
current unchanged canonical route. `portability-verification.json` records
89 layout facts, 414 relocations, 552 pairs/856 root invocations per image,
ten controls, full private D328 equality, and zero accepted coverage. The fresh
compiler directory was removed. No native bytes or object/ELF is retained here.

Focused checks performed during packaging:

- With pinned IDO and MIPS GNU linker: 21 passed in 177.75 seconds. This included
  all five complete replay tests (closure plus F938/E114/DA78/D498 standalone).
- A comparison-normalization regression was then added. On the final test
  collection the compiler-free subset passed: 17 passed, 5 replay tests deselected.
- With IDO deliberately unavailable: 17 passed, 5 clean skips. No hidden compiler
  call or unguarded score.ido exit was encountered.
- Source/assembly and complete current native-target bindings also passed through
  the portable CLI, independent of pytest.

The focused command is:

```sh
python3 -m pytest tests/cloud/test_runtime_b_closure_packet.py \
  tests/cloud/test_runtime_b_f938_setup.py tests/cloud/test_runtime_b_e114_update.py \
  tests/cloud/test_runtime_b_da78_position.py tests/cloud/test_runtime_b_d498_visual.py -q
```

All five replay test bodies passed in the with-IDO run; all 17 noncompiler tests
passed after the added comparison regression. The new regression proves that
cross-build metadata differences are ignored while changed allocated bytes,
procedure extents, and ordered behavioral effects are still rejected. These
focused checks do not replace the coordinator's required full current-master
with/without-IDO integration matrix. No publication, CI watching, production
edit, scorer change, or acceptance occurred in this packaging task.

## Fresh-master negative-control portability check

The reported `negative control drift` was investigated against master
`9651ca53bfd16b015adb63e1b3d76487a995a84f` and the integrated research packet.
The unmodified packet's six focused tests passed with pinned IDO,
`REQUIRE_TOOLCHAIN=1`, and Python 3.12.14 in 92.47 seconds. Standalone native
controls also matched the archived receipts exactly with Python 3.12.14 and
3.13.5. The reported builder's exact differing field was not reproduced here;
its Python version and full receipt were unavailable.

The cross-host vulnerability is the full equality gate on host exception text.
The repair is deliberately narrow: the D498 zero-distance control's two
equivalent `float division by zero` and `division by zero` diagnostics compare
equally. This is a receipt-level regression fixture, not a claim that the latter
spelling was observed on either local interpreter. Other control reasons,
identities, order, outcomes and unknown fields remain strict. Real mismatches now
report their first differing field and both values, rather than only the generic
message.

No historical baseline file, C body, compiler recipe, native target binding,
instruction harness, positive proof or native control was changed. The tests
exercise the precise diagnostic-only difference and reject altered/missing
control fields, different error categories, accepted mutants, lost/added/
duplicate/reordered controls, unknown fields and a second meaningful change
hidden alongside the spelling variation. Final focused results were:

- Pinned IDO and `REQUIRE_TOOLCHAIN=1`: 9 passed in 112.17 seconds, including
  the full compiled closure, all 552 pairs/856 invocations per image, and ten
  genuine negative controls.
- Deliberately missing IDO: 8 passed, 1 clean skip in 0.11 seconds.
- Missing IDO with `REQUIRE_TOOLCHAIN=1`: the complete-replay test failed as
  required, with the explicit missing-tool diagnostic (0.03 seconds).

These focused checks do not replace the full integration matrix.
