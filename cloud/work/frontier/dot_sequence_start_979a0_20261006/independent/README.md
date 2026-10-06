# Independent sequence-start review

Verdict: suitable for **bounded NONMATCH research publication**, with zero matching-byte credit. Reviewed caller source SHA-256: `2e25793116905b32424d9039497b32801a3a67bc98c53a9e60e94d3952424d04`.

Run from a checkout with the recorded IDO and GNU MIPS toolchains available:

    python3 cloud/work/frontier/dot_sequence_start_979a0_20261006/independent/review.py

Ordinary Python is required; optimized Python is explicitly rejected because this independent research harness uses assertions. The separate producer verifier supports its documented optimized-Python route.

## Verified

- Rebuilt the complete 13-function group. Its 2,060 function bytes occupy 2,064 text bytes; the remaining four bytes are zero alignment padding. Every defined function was inspected, and no owned data/storage appeared.
- Independently parsed protected manifests, native targets and ELF metadata, then GNU-linked every complete function extent. All twelve accepted context bodies match their full native extents. The caller remains 288 bytes against the 292-byte target, with 12 positional word differences and 35 relocation sites.
- Independently compared the three reused C files byte-for-byte with accepted `src/blob/groups/slot_sound` objects at `dea99f09ab19b1d3b324ed7097162f7b378e7096`.
- Executed 960 cases each on native and GNU-relocated candidate instructions. The interpreter executes the actual `display_list_alloc` and `func_80096288` bodies, while ordinary external services poison every caller-save GPR. It verifies complete non-stack read/write/call traces and memory, preserved registers, and the genuinely homed unused second argument.
- Cases exercise disabled/non-disabled gates, current-handle `-1`, equal/different requested sequences, dirty unused arguments, identifiers at unsigned-halfword boundaries, sequence indices exceeding 16 bits, external return boundaries, slot-global mutation during activation, and identifier/payload mutation during registration.
- All 73 native caller instructions, all 72 candidate caller instructions, and both outcomes of every caller conditional branch execute. A separately written byte-offset semantic oracle agrees. The unchanged caller also passes the same 960 cases in a C89 UBSan host build with typed service stubs.
- Negative controls reject forcing the validator to an ordinary kept boundary (35/73), removing the genuinely homed second formal (68/73), removing accepted context (51/73), and changing the global's GNU relocation address.
- The frozen producer receipt replays, and its five scoped tests pass.

The initial review caught the historical integer declaration of the pointer-valued slot getter and inaccurate boot-service declarations. The author corrected them before the frozen source was rebuilt and tested. The accepted context sources were not changed.

## Boundaries

This is finite-case behavior evidence with explicit external-service stubs, not arbitrary alias/concurrency or complete game-runtime proof. GNU relocation covers every complete ELF function independently, not the full production-unit link. The host harness tests the unchanged caller with typed host stubs; the native/candidate interpreter separately executes the real activation and validation helpers.

The terminal address-materialization residual has no authenticated source-level repair. No volatile qualifier, fabricated argument, filler local, helper substitute or instruction patch was introduced. There is no promotion, image, compressed-stream, ROM-hash, hardware, CI or accepted-coverage claim here.

`elf_link.py` is an unchanged reuse of the repository's independent ELF/GNU verifier from `cloud/work/ipa-groups/dot_layer_update_20261005/proof.py` at the review base. Temporary link inputs/objects exist only inside disposable local directories. Publication files in this directory are `README.md`, `review.py`, `review.json`, `elf_link.py`, and `host_harness.c`; exclude Python caches and generated binaries.
