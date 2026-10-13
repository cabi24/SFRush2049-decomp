# Static audio book-cache fetch: full native match

## Deliverable

`cloud/matches/boot_tail/func_80012234.c` is a complete standalone IDO 7.1
reconstruction of the 372-byte static function, native interval
`[0x80012234, 0x800123A8)`. Base master: `55e179413`.

```
IDO_DIR=tools/cloud/ido71 python3 tools/cloud/score.py fn \
  cloud/matches/boot_tail/func_80012234.c func_80012234 \
  --targets asm/us/boot_tail --flags '-g0 -O2 -mips2 -G 0 -non_shared'
func_80012234:
  MATCH
```

This is one new strict source candidate, **372 bytes pending checker installation**.
No accepted-lock, production TU, target, splice, coverage-inventory or ROM change.
No ROM identity or gameplay claim. Keep the pull request draft; the independent
checker owns acceptance, installation and merging.

## Reconstruction and contracts

The first natural C reconstruction matches, with no variants or shaping devices.
There is no proven arcade ancestor. The former missing-callee blocker is resolved
by the accepted `w21c/sources/func_80012730.c` contract: two unsigned arguments to
`func_8002C5E0`, an unsigned result, and twelve-byte records `{length, source,
destination}`. This packet does not pretend to implement that helper.

The twenty-byte `AudioNode` matches the existing lib_11640 declaration: next at 0,
previous at 4, buffer at 8, key at 12, unsigned countdown at 16, spare halfword at
18. Its accepted initialization is `func_80011F60`; `func_80012200` decrements
nonzero countdowns. Both and accepted 12730 recompile strictly unchanged as
independent controls. Thirteen IDO C89 assertions check all native fields/sizes.

- Find the first matching key in the active list, refresh countdown to two and
  return its buffer without touching the DMA queue.
- Otherwise remove the free-list head and prepend it to the active list. Retail
  leaves the consumed head's previous field unchanged. If there is no free node,
  reuse the first active node whose countdown is zero, without moving it.
- Set countdown/key before the command call, advance the command cursor by twelve,
  then append a 0x100-byte DMA record and increment the unsigned-halfword count.
- The only direct caller across all four protected target populations is
  `func_80012D18+0x490`; its delay slot supplies one word from caller record +0x2c
  in a0. The return is an ordinary buffer pointer. No private ABI is invented.

Valid inputs require a matching, free or expired node and valid queue/list/buffer
storage. Native code dereferences null if the pool is exhausted; the source keeps
this precondition rather than inventing a guard. The fixture suite deliberately
confirms both empty and nonempty all-live exhaustion failures. Arbitrary aliases, cycles, invalid storage,
asynchronous races, the real command helper, original source identity and full
gameplay remain outside the finite proof.

## Verification

`verify.py --deep` checks:

1. Manifest-authenticated full native extent and next boundary; stock pinned
   IDO 7.1 selected by `compilers.tu_cc('lib_11640', ...)`, never a generic compiler
   for the actual ROM TU. The canonical single score has zero differing words,
   unresolved symbols, unverified sites, errors or extra nonzero words.
2. Independent ELF32 big-endian reader and GNU linker operate on the entire
   original object. All fifteen original relocations and seven external bindings
   are enumerated and consumed. All 93 unmasked words are equal. The actual
   STT_FUNC is 372 bytes, complete text 384 bytes, separate zero suffix 12 bytes,
   frame 32 bytes, and owned data zero. Legacy IDO ELF declares MIPS2 but does not
   set an explicit o32 flag; C layout, caller and register replay establish the
   ordinary o32 contract separately.
3. A temporary promotion-shaped copy using the current context/prefix-dedup tools
   compiles at the recorded TU compiler. The new body remains strict and all
   **71 existing compiled functions** preserve their full accepted scores. This
   creates only scratch files and writes no production source or lock.
4. **2,491 independent field-oracle fixtures / 4,982 native and GNU-linked candidate
   machine executions** agree on return, complete mapped nonstack memory and
   helper-entry snapshots. All 93 instructions of both bodies execute. Cases
   cover hits at every position, duplicate keys, empty/nonempty active lists,
   singleton/multiple free lists, expired-node reuse, high-bit keys/countdowns,
   unsigned cursor and queue-count wrap, and synchronous helper mutations of
   queue index, queue base and selected buffer. Caller-save GPRs and LO are
   poisoned on helper return; callee-save registers, stack and return address
   must be restored. The helper is a bounded hook, not authentic gameplay.
5. **2,491 actual-source C89 cases** pass independently at O0, O2 and under
   ASan/UBSan/bounds with recovery disabled. Only LeakSanitizer is disabled for
   the container. Host pointers are logical values, not an N64 layout claim.
6. Fresh wrong-countdown and wrong-transfer-length sources fail both strict and
   behavioral checks. Unknown instructions and unmapped memory fail closed.
   An extra owned allocation and an extended STT_FUNC consuming zero alignment
   are rejected by the original-object validation gates.
7. Protocol-v2 receipt binds owned source/proof/host/test files, selected accepted
   contract sources/header, native parsed words/extent, symbol addresses, the
   parsed-word direct-call census and stock compiler fingerprints. No shared
   tool, Makefile, lock or raw-assembly-text hash is pinned.

Normal and Python-O deep replays pass. The single focused strict test passes in
about 0.3 s; absent IDO skips cleanly in about 0.3 s through shared conftest handling.
`make check-matched` passes all 630 static locks and 102 runtime-image entries.
No raw native words, assembly listings, object files or ROM payloads are included
in the packet. Publication adds source, tests, research and a coordination line
only. Hosted CI and independent review are separate publication gates.

## Reproduction

From the repository root, with the standard pinned toolchains and MIPS binutils:

```
python3 cloud/work/frontier/dot_static_12234_20261013/verify.py --deep
python3 -O cloud/work/frontier/dot_static_12234_20261013/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_static_12234_match.py -q -o addopts=''
make check-matched
```

Use `--write` only when intentionally rebinding a fully rerun owned-input receipt.
The cheap test checks current inputs, a fresh compiler output and exact geometry;
the larger GNU, native, host and temporary-TU experiments remain opt-in.
