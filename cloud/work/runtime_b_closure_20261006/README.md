# Genuine runtime-B closure: portable research packet

The complete eight-body diagnostic has a privately byte-exact D328 body (124
bytes), seven NONMATCH bodies, and **zero accepted coverage**. This packet makes
that completed evidence reproducible after later integration; it does not change
source, private parameter order, compiler flags, scorer, production context,
locks, object symbols, or matching admission.

The one source baseline is `baseline/closure.c`, SHA-256
`fc80a9075c6357b1833bc53b7810f8a4ee2c2f874e1bc8ba03f47fcb10e32d95`.
Its bare O3 recipe follows the unchanged canonical build route, including its
mandatory as1 `-r4300_mul` workaround. Only the genuine FCE0 root is kept; the
other seven bodies remain optimizer-visible static definitions. ECOFF procedure,
end, and PDR metadata expose the complete hidden bodies. D328 has all 31 target
words equal, no extra/missing words, and nine ordinary external relocations.
The canonical ELF-name-based scorer still cannot admit it by name.

## Evidence and limits

- 89 target-compiler layout facts
- 414 independently recomputed relocations; unchanged other text/rodata bytes
- 11,588 complete candidate function bytes and 12 zero alignment bytes
- 320 bytes of owned rodata
- 552 native/candidate paired fixtures, 856 FCE0 invocations per image
- Ten fail-closed/negative controls, and a separate complete semantic replay
- All eight genuine private bodies execute; none is an external helper hook
- Historical independent D328 audit retained verbatim
- `admission-review.md`: narrow maintainer proposal and fail-closed rejection
  tests; design only, no implementation or canonical admission

The original source-derived evidence is preserved byte-for-byte in `baseline/`,
including `final.json`, its original source/harness bindings, and independent
review. Paths in those historical receipts describe the original invocation;
they are not dependencies on the original machine. Use the portable entry point
below, not the historical workspace-specific assembly/audit commands.

Remaining limits are unchanged: seven bodies NONMATCH; original TU, exported
root identity and private formal order remain unproven; bounded external models
do not reproduce whole-game state or N64 exceptional floating point; accepted
E398 source declares void despite the consumed native return. No canonical
MATCH, production splice, image/compression/ROM, or acceptance gain is claimed.
Full aggregate integration tests and publication belong to the coordinator.

## Reproduce

From any working directory in a full-history checkout:

```sh
python3 /path/to/repository/cloud/work/runtime_b_closure_20261006/verify.py
python3 -m pytest tests/cloud/test_runtime_b_closure_packet.py -q
```

The full replay requires pinned IDO and the MIPS GNU linker. The compiler-dependent
pytest gate skips cleanly if either is missing. Compiler-free source/assembly
and native-target binding tests still run. `--static-only` provides that smaller
check directly. `--output /tmp/closure-replay.json` optionally saves the compact
result; object/ELF/native bytes are never publication artifacts. Temporary
compiler files are removed when the replay exits.

The portable verifier binds this packet and the actual behavioral C source,
shared schema, Python instruction harness, and source inputs. It reconstructs
all eight complete bodies read-only from the separately retained source packets.
F938, E114, DA78, and D498 standalone packets are included in this batch; existing
FCE0 and E114-child research packets are dependencies from the prior aggregate.
Their standalone controls remain separate from genuine whole-closure evidence.

Historical production context is read with `git show` at base
`6b2e9e506fe3d2267a710e41c85af5364ccd00c7`; current target words come from
`score.targets()`. No mutable live manifest, scorer, production source, lock, or
pytest-file fingerprint is a replay invariant. Fresh cross-build comparisons
retain compiled section/member bytes, extents, relocations, owned data and all
behavior. Full ELF fingerprints and invocation paths are excluded only from
cross-build comparison: each fresh replay still authenticates its actual ELF,
source, and compiler-boundary inputs before executing them.

Negative-control replay retains every control, its order and rejection/detection
outcome, and the full reason for all other failures. Only D498's equivalent
`float division by zero` / `division by zero` diagnostics share one comparison
form. The original bound native controls still execute unchanged; arbitrary
arithmetic errors, changed rejection sites, missing controls and added receipt
fields fail. A remaining mismatch reports its exact receipt field and values.

The first aggregate tree is an external preparation dependency only, not a
pytest invariant. This batch does not duplicate or edit that aggregate's files.
