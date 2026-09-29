# F3 validation — 2026-09-29

The cloud scorer, standalone linker, and group relocator now bound a compiled
function at the next function symbol in its section or at the section end.
Any nonzero word beyond the target length fails; zero padding is permitted.
Short functions cannot consume instructions from the next function. Local
labels and same-address aliases do not shorten the checked extent.

Group length checks apply to spliced members. Context remains available for
address resolution without requiring a matching body; the existing context
function `func_800988D8` is 36 bytes longer than its retail target. Explicit
`include_context=True` comparisons validate context lengths too. This does
not change the cloud CLI's member/context policy (F7).

## Tests

On watchman2, using IDO in an isolated scratch checkout:

```sh
python -m pytest tests/conveyor/test_cloud_score.py \
  tests/conveyor/test_blob_lengths.py tests/conveyor/test_blob_group.py -q
```

**172 passed**, including all 123 standalone locks and both groups' members.
The negative tests reject an extra instruction in all three paths; positive
tests accept padding and exclude neighboring functions. Tests also cover
short bodies, nonzero group offsets, linker alignment, and context handling.

On the Pi, `python3 -m pytest tests/conveyor/ -q -m 'not node_required'
--tb=short -o addopts=''` completed with **341 passed, 129 skipped, 5 deselected,
1 pre-existing failure** in
`test_closure.py::test_populate_keeps_suffix_row_conflicted_against_discovered_extent`.
The IDO tests skipped on the Pi passed on watchman2. Live-node tests were excluded.

## Image and cartridge gates

- `blob_splice check`: 127 entries, zero problems.
- `spliced_bodies()` returned all 127 locked bodies under the new checks;
  its keys were explicitly checked against the lock to detect omitted bodies.
- `blob_rom.rom()` included all 127 bodies, passed the image gate, and produced
  a 326,180-byte compressed stream identical to the cartridge's stream.
- The isolated builder completed with `MAKE=0 TEST=0` and `ROM matches!`.

Hashes:

```text
Image SHA-256: bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d
Blob SHA-256:  9e632d21b7d0d293563755d9f2033557cb1a4d91d7df6e8d41e68d8afe3e5f7d
Built ROM SHA-1: 3f99351d7bb61656614bdb2aa1a90cfe55d1922c
```

The builder's main checkout had existing changes, so `blob_rom.BUILDER_REPO`
was overridden for this run to
`/home/cburnes/rush2049/tmp/f3-score-YC3GmHeU/rom-repo`. That directory used the
committed ROM sources, the Pi's generated linker inputs and extracted assets,
and the builder's IDO toolchain. The normal image, compression, Makefile, and
ROM verification paths ran unchanged. No generated target, locked source, or
lockfile changed as part of F3.
