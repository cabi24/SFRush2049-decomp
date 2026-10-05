# Graphics initializer: strict 400-byte match

Base: `cf10b339`. New claim: `sound_init` at `0x800A4934`, **100/100 full
relocated words equal**, exact ELF `st_size` **400**, all **37 relocation sites
resolved**, no masked words, unverified references, excess words, or unresolved
symbols. This is a source match ready for independent review, not an image or
cartridge coverage claim. No splice, locks, accepted sources, protected context,
symbol files, scorer, or target files changed.

The historical name is misleading. This no-argument function begins a new
19,200-byte double-buffered graphics command stream if the write cursor is null.
It resets the render/scissor/texture caches, advances the bank, emits four SDK
commands, selects mode 1, and conditionally sets flag `0x8000` for height >=221.
This is N64-specific graphics code; no arcade ancestor is claimed.

## Why the old attempt now closes

The earlier [closure packet](../dot_graphics_init_closure/README.md) already had
the full behavioral reconstruction. This packet does not claim new recovery of
that behavior. The new evidence is the accepted SDK-macro form of `gfx_modes`
and [wave 2's accepted texture renderer](../frontier/w2i/RESULTS.md).

Only three source-supported stages were needed:

1. Old initializer, unchanged, in the current accepted `gfx_modes` context:
   75/100 differing words, true ELF size 404.
2. Add unchanged `src/blob/object_render.c`, the actual existing owner of
   `s64 D_8012E688`: 27/100 differences, exact size 400. The known aligned
   64-bit definition removes the redundant address setup for its zero store.
   The initializer itself keeps an `extern` declaration. No duplicate or
   hypothetical storage owner is introduced.
3. Express the four display-list operations as SDK-style GBI macros, each with
   its real block-local `Gfx *`: strict zero. The cached image declaration is
   also corrected to `void *`, matching the accepted owner; a checked isolated
   type control left the strict result unchanged.

A diagnostic definition-only control first established the same 27/100 result;
it is not retained as the solution because the real owner is available.
The retained owner-only residual is pure allocation: 100 instructions, identical
24-byte frame, no opcode gaps, and 27 register differences across the cursor,
height-address, bank, and -1 webs. A workbench run confirmed this classification
on the saved intermediate object. Its relocation-symbol cautions reflect the
fully resolved native object versus a relocatable candidate; the canonical
full-relocation scorer supplies the authoritative residual.

The genuine mode helper remains internal. Native `t3` holds the height-global
address across its call. The initializer retains its ordinary no-argument public
ABI. The accepted GBI macro source models the callee's register summary, and the
initializer macros recover its own local lifetimes. There are no fake formals,
keepers, padding locals, volatile steering, assembly, or register-pressure search.

## Source ownership and regression evidence

`recipe.json` records the actual paths and kept roots. `verify.py` materializes
copies only under ignored `build/`, using these unchanged inputs:

- The four files in `src/blob/groups/gfx_modes`
- `src/blob/object_render.c`, including its existing 64-bit global definition
- This packet's `init.c`

Five previously accepted strict functions remain full-word equal with exact ELF
sizes: `func_8008705C` 180 bytes, `func_800878E0` 296, `func_8008A148` 580,
`func_8008A46C` 472, and `object_render` 10,048. Each is freshly compared both in
the new group and in its accepted baseline recipe.

The 1,548-byte mode helper remains zero masked differences, but still has its
**two pre-existing unverified local-table placements** at offsets +0xC/+0x14.
Its masked body hash, `.rodata` section hash/length, and `.rel.rodata` section
hash/length are identical to the accepted group's fresh baseline. This is
non-regression evidence, not a new table-placement proof or strict claim for
that helper. Nothing here upgrades those two references to verified.

`verification.json` includes source/compiler/protected-input fingerprints,
per-function hashes, ELF sizes, complete relocation inventories, final results,
accepted baselines, and both causal controls. All emitted objects and native
words remain under ignored `build/`; no raw assembly or binary data is published.

## Reproduce

Use the established IDO 5.3 toolchain via `IDO_DIR` and a host C compiler:

```sh
python cloud/work/dot_graphics_init_match/verify.py --output build/graphics-init-review.json
cmp cloud/work/dot_graphics_init_match/verification.json build/graphics-init-review.json
python -m pytest -q tests/cloud/test_graphics_init_match.py tests/conveyor/test_graphics_init_closure.py
cc -std=c99 -O1 -g -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer \
  cloud/work/dot_graphics_init_match/host_test.c -o build/graphics-init-host
ASAN_OPTIONS=detect_leaks=0 build/graphics-init-host
```

Results: the compiler receipt reproduced byte-for-byte; **9 focused tests pass**;
ASan/UBSan passes **5,142 initialization cases** plus a full-state non-null no-op.
The contract harness uses event-logging mode/flags stubs. It checks both banks,
safe bank rollover edge values, signed dimensions without arithmetic overflow,
the 220/221 threshold, a height change during the mode call, exact four commands,
untouched buffer regions, every reset field, and complete idempotence. It does
not emulate the target or reimplement the accepted callees. Leak detection is
disabled because the executor's ptrace environment does not support it.

A wider local run of `test_cloud_score.py`, `test_blob_lengths.py`,
`test_blob_group.py`, and the two focused files collected 668 tests: 653 passed,
15 failed exclusively because existing locked singles retain unverified local
`.rodata`/`.data` references. Those sources and tests are unchanged. The wider
suite is not represented as fully passing, and those unrelated baseline issues
are not repaired here.

The focused suite includes a compiler replay of this recipe, the five strict
accepted functions, the unchanged mode-helper uncertainty, and the two causal
controls. Normal cloud CI therefore verifies the group-only source even though
it is outside the generic changed-submission path. The saved initial independent
review is `peer_review.json`; it binds the pre-integration commit and original
eight-test suite. The batch review separately covers the added replay test.

The independent checker still owns any integration, image identity, ROM hash,
and merge decision. A materialized `build/dot_graphics_init_match/final/group.json`
is available after replay for strict root-only `score.py group ... --claims`.
