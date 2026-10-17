# LoadCars: locally exact, research-only closure

This packet reconstructs `audio_interrupt_handler` / `LoadCars` at `0x800BB140` from the genuine three-local MAT3 body and independently checked native data/call differences. The complete native root is locally strict: 343 words, 1,372 bytes, frame 168. Its ten already-locked context bodies remain strict, including the 8-byte `FindWaveInstance` wrapper. None receives new credit.

The full group still contains unmatched `audio_frame_sync` / `LoadWave`: 130/130 different words, emitted extent 432 bytes versus native 520, frame 48 versus native 112. Removing that genuine context leaves LoadCars at 119/343 differences. Consequently `claims` is empty, new candidate bytes are zero, and the packet is not eligible for complete-group promotion. A root-only scorer exit status is insufficient. No image, ROM, canonical integration, or whole-unit gate is claimed.

## Genuine source boundary

The donor supplies `LoadCars(int)` with exactly `i`, `j`, and `numLoadedCars`, float-returning `Random(float)`, the public `FindWaveInstance(int)` wrapper around the signed-byte-filter leaf, and the unsigned `gWaveSizes[]` array. Restoring those boundaries, with the real native setup/heap/random/resource context, resolves the old root residual without arbitrary frame padding, register forcing, or altered ABI declarations. The extra 8-byte wrapper is already locked; it is not a new match.

Native adaptations are documented in PROVENANCE.md. Notably the N64 mode-2 no-fit fallback calls `func_800BB02C`; the donor's corresponding branch does not call its different palette setup. Removing the native call fails both compilation comparison and host semantics. The actual array replaces an artificial 352-byte-prefixed archive view. Vehicle records retain their genuine first two pointer fields and native mode offset/stride.

All copied helper definitions are source-bound to current repository files. The sole resource-context declaration change makes the wave-size table unsigned, consistent with the donor and the root. This does not repair the unmatched loader body. Its inherited allocator exhaustion behavior and native private ReadWave ABI are outside this proof. Existing setup/heap source includes reconstruction details documented in those accepted files; this packet preserves them rather than asserting newly recovered original compilation-unit boundaries.

## Replay and tests

With the repository's stock IDO 5.3 compiler and normal toolchain environment:

    python3 cloud/work/frontier/dot_mat3_loadcars_research_20261017/replay.py
    python3 -O cloud/work/frontier/dot_mat3_loadcars_research_20261017/replay.py
    python3 -m pytest tests/cloud/test_mat3_loadcars_research.py -q

The replay binds all source, verifier/scorer, actual IDO stage executables, context, recipe, and protected target hashes; requires full extents and all relocation/error fields; verifies exact object repetition; and explicitly gates the unmatched boundary and empty claims. Type checks inspect IDO-emitted constants, not compile-success assertions. Runtime guards remain active under Python `-O`.

Structural controls remove resource context, flatten the real Find wrapper call, restore the artificial table view, and independently change six semantic facts. The host test compares the actual candidate body with an independently structured model across 4,224 finite fixtures, complete selection/handle state, and ordered boundary-service traces. It covers random uniqueness, two search passes, sharing, mode-2 load/fallback, select dispatch, signed metadata, unsigned size arithmetic including wraparound, and 16-bit handle conversion. Six mutations must fail with the exact differential error, not an arbitrary crash. UBSan is fatal.

The host harness substitutes controlled heap, load, find, random, setup, and select services. It does not execute the unmatched LoadWave or claim native runtime execution. Setup models only documented payload-copy/budget effects. The PRNG service uses defined unsigned 32-bit arithmetic to model native wraparound; the preserved canonical signed-overflow spelling is not claimed ISO-C-defined for all seeds. On the 64-bit host, only the two unused pointer fields of the vehicle record are replaced with 32-bit integers; the unmodified target layout is separately measured by IDO. Fixtures assume valid array storage and terminating random selection; they do not establish all possible runtime states or allocator exhaustion semantics.

## Missing-tool portability follow-up

Compiler fingerprints now resolve through the scorer's standard IDO availability check. When the toolchain is absent, tests follow the shared local-skip policy; REQUIRE_TOOLCHAIN=1 still makes those skips failures. The new diagnostic test pins that boundary. C, recipe, target data and all comparison/semantic results are unchanged. The refreshed proof and narrow availability review supplement the original source/semantics review. No claims or byte credit are added.

The compiler fingerprint includes `ujoin`, which stock `cc -j` invokes after
`cfe` on this group-build path. A stale `ujoin` digest is rejected under Python
`-O`; source, comparison rows, semantic results and credit are unchanged.
