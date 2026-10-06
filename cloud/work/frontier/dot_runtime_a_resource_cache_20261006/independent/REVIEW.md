# Independent review: runtime-A resource cache lookup

Verdict: RESEARCH-ONLY / complete semantically reviewed NONMATCH. No source-level defect found in the tested native domain. This packet does not qualify for matching submission or acceptance: five native words still differ.

## Frozen identity and fresh replay

- Candidate: `cloud/work/frontier/dot_runtime_a_resource_cache_20261006/candidate.c` at the packet source path.
- Source SHA-256: `29d1dfaaeed5d9f51862bb456ee677280f72e9da259d562a98caae42a135694a`.
- Runtime image A, authenticated image identity `0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667`; interval `[0x80390BC0,0x80390D2C)`, exactly 364 bytes / 91 words.
- Native body SHA-256: `02e3869e40edbc543340d6887bda6ec3743edb8d793990921b7432c6d935231e`.
- Independently recompiled standalone using the candidate's exact IDO O3 header, with no fabricated caller or added pressure source.
- GNU-linked complete ELF: native-sized function plus four zero alignment bytes; zero owned-data bytes; no unresolved relocations, extra allocated storage, or trailing executable body. Every object relocation was inventoried and all four external names resolved to exact native addresses.
- External bindings: cache `0x803BA230`; halfword handle table `0x80142B08`; loader `0x80097798`; setup helper `0x800BB02C`.
- Strict differences remain at body offsets `0x120`, `0x124`, `0x12c`, `0x130`, `0x134`: reload ordering and load/store/constant scheduling. This is not a relocation-only or exact match.
- Ten malformed complete-ELF view controls were rejected: entry, section address, symbol address, function extent, nonzero padding, excess text, wrong binding, extra storage, unresolved relocation, and changed text.

## Native contract

The record array contains 16 entries, each 12 bytes: a word handle at +0, signed-byte loaded flag at +4, signed-byte id at +5, and six untouched bytes at +6..11. Independent fresh recompilation and full linking reproduced both unchanged context bodies: initializer A:8039156C (116 bytes) and release A:80390D2C (160 bytes). These witnesses establish field accesses and strides; the six-byte remainder is a partial layout view, not recovered semantic fields.

The lookup first selects the earliest entry with matching signed id and any nonzero loaded byte. It returns that handle and success without calling either helper. Otherwise it prefers the earliest matching unloaded id. Only if no id matches does it take the earliest entry with any negative signed id byte. If all 16 ids are nonnegative and none matches, it returns failure without modifying the output.

On a load it stores a new id only when selecting a free entry, invokes the loader with `(signed_kind + 88, 1, 1, 0, null)`, stores the returned word in the record and result, sets loaded to 1, narrows the handle into the indexed halfword table, invokes the setup helper `(signed_id, signed_kind, null)`, and returns success. There is no negative-handle failure check. Call-order snapshots confirm the setup helper sees all these stores.

Native entry and reload instructions prove both input values are signed halfwords. Thus kind+88 ranges from -32680 to 32855 and cannot overflow a 32-bit signed int. Loop counts and index scaling are bounded. Byte and halfword narrowing preserve the observed target low bits; out-of-range conversion is implementation-defined C behavior rather than a language-portability promise.

The actual loader's fifth argument comes from caller sp+16. Its relevant forced-load branch has no hidden live-in register requirement. The setup helper consumes a0/a1/a2, computes its own 24-byte indexed record, and does not require a fabricated extra argument. Their bodies are native-bound external-service evidence, not newly accepted helper source. The existing historical name `audio_frame_sync` does not establish an audio-only resource purpose.

## Independent behavioral evidence

7,264 cases compared three representations: actual host-compiled candidate C, protected native instructions interpreted independently, and freshly IDO-compiled/GNU-linked candidate instructions. Compared complete record fields, untouched reserved bytes, result, indexed halfword, helper argument lists, and full state snapshots at both helper boundaries. All passed.

Coverage includes all 91 native instruction words and all 20 feasible branch outcomes; every possible loaded-byte pattern; every negative signed-byte free sentinel; each of 16 record positions; all-full failure; matching unloaded entry ahead of free allocation; duplicate-id loaded-hit preference; signed-kind endpoints and resource-bias boundaries; negative/large helper returns; and helper side effects. Helpers deliberately clobbered all caller-saved GPRs in the interpreter. Stack restoration, argument homes and callee-saved registers were checked.

Nine independently generated source mutants were rejected by state or helper-trace comparisons: loaded==1, only -1 is free, count15, resource bias87, wrong loader argument, wrong halfword-table index, rejecting negative handles, unsigned record id, and missing setup call.

## Domain and limits

- The real-service working domain is caller-valid id 0..12, valid mapped 16-record storage, a live nonaliasing result word, initialized cache invariants, an actual valid resource for kind+88, and both genuine helpers' preconditions.
- The 13-entry initializer concerns a related 8-byte slot table; it does not prove the complete allocation extent of `D_80142B08`. No guessed global table size is offered as production evidence.
- Wider positive ids, all signed-kind endpoints and arbitrary 32-bit helper handles were tested only with a deliberately enlarged synthetic table and bounded helper doubles. These are local branch/narrowing stress tests, not proof that the actual services accept those values.
- Negative-id native cases were compared with host C only on a loaded-hit or all-full path, neither of which indexes the halfword table or invokes a real helper. No negative-index C access was authorized by this harness.
- The interpreter models this exact function, valid memory and ordinary synchronous helper boundaries. It does not prove asynchronous shared-memory behavior, game-resource validity, external helper success, whole-runtime-image identity, compression, cartridge hash, hardware behavior or gameplay.
- Current accepted context was read-only and freshly recompiled. No protected target, lock, source TU, scorer, publication or CI watcher was changed by this review.

## Local reproduction

Configure `IDO_DIR` and MIPS GNU binutils in `PATH`. Run `python3 review.py CANDIDATE`, `python3 mutants.py CANDIDATE`, and `python3 context.py CANDIDATE` from this directory, passing the packet candidate path. `RUSH_REVIEW_REPO` optionally selects a separate complete input checkout. `build/runtime_a_cache_independent/{review,mutants,context}.json` record the results. The scripts use local protected inputs; binaries and extracted bytes are private test artifacts and must not be published. The published scripts use repository-relative input and ignored build-output paths.

Stop condition: keep this body as complete NONMATCH research. Reopen compiler work only with a new native-context or scheduling hypothesis that is distinct from the already measured source controls.
