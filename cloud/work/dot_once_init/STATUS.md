# Complete func_800A5158 match

Source: `cloud/matches/func_800A5158.c`.
Native extent: **0x800A5158–0x800A51D8**, 32 instructions / 128 bytes.
Flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

## Reconstruction and interface

This zero-input initializer skips every operation when the signed guard byte is
nonzero. Otherwise it sets the guard first, clears the state halfword, copies an
unsigned configuration byte, calls `audio_frame_sync(byte, 0, 0, 0, NULL)`, saves
its returned identifier, and delegates in order to `display_list_alloc`,
`func_800A4E58`, and `func_800A510C`. The identifier is forwarded even when
negative. No new failure handling or synchronization is introduced.

Historical `near_miss_B33/func_800A5158_init.c` reproduces eight differing leading
words. Naming the actual guard value as `const s8 initialized = 1` preserves its
real source lifetime and naturally reproduces native IDO O2 scheduling. There
are no extra operations, helpers, arguments, padding, volatile accesses, forced
registers, or source/scorer tricks. Historical candidates are unchanged.

The new declaration uses a pointer for the fifth `audio_frame_sync` argument,
following its actual group definition and native stack-slot forwarding to an
allocator. The constant null argument remains the same O32 zero word. This
corrects the historical integer declaration only in this new source.

The native entry has a 32-byte frame and no input arguments. Its direct caller
`tire_compound_set` calls at 0x800A5D10 without setting arguments or consuming a
return. Indirect/computed callers are not excluded. No direct arcade equivalent
is established because the reference checkout is unavailable.

## Verification

- Canonical strict scorer and separate clean-directory IDO replay: **32/32 full
  words exact**, with independent HI16/LO16/JAL arithmetic for all 16 relocations.
- Separate reviewer compiles frozen source and uses GNU ld plus objcopy to compare
  all 128 native bytes. One 128-byte ELF function, no extra text or alignment words.
- Zero masks, unresolved/unverified relocations, relocation errors, or excess words.
- Complete body, native call interfaces, and caller independently reviewed.
- Reproducible host test: **327,680 exhaustive cases plus 327,680 repeated calls**
  pass ASan/UBSan. Covers all signed guard and unsigned input byte values, five
  returned IDs including INT_MIN/-1/0/1/INT_MAX, exact call order, arguments,
  guard-before-calls, configuration copy, ID store/forward, and nonzero no-op paths.
- C89 pedantic syntax/warnings pass. Host mocks are not compiled into the match.
- Scorer tests: **621 passed**. Repository CI-style tests: **1,299 passed,
  41 skipped, 9 deselected**. Cloud tests also exit zero with two skips.
- Initial repository collection lacked materialized submodules; exact pinned
  revisions were restored and the complete suite then passed.
- **160 static locks** intact; all 651 blob/group source hashes clean; all
  23 protected manifest files intact. Target absent from existing source locks,
  cloud matches and group claims. PR1–20 and parallel target checked for overlap.

Reproduce the match from the repository root:

```sh
python3 tools/cloud/score.py fn cloud/matches/func_800A5158.c func_800A5158 --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

Reproduce the host semantics:

```sh
cc -std=c89 -pedantic -Wall -Wextra -Werror -O2 -fsanitize=address,undefined cloud/work/dot_once_init/host_semantics.c -o /tmp/rush-once-init-test
ASAN_OPTIONS=detect_leaks=0 /tmp/rush-once-init-test
```

Leak detection is disabled under sandbox ptrace; the test allocates no heap.
Host mocks supplement exact native identity and do not execute N64 delegates.
The guard is not atomic and establishes no thread-safety guarantee.

## Integration limits

Draft source contribution only: **no accepted cartridge coverage is added**.
Only new source, host test, and sanitized proof documents are added. Accepted C,
locks, targets, scorer, generated context, build wiring, and coverage totals are
unchanged. No ROM or complete extracted image is available. Source-image
splicing, compressed-stream identity, full-ROM SHA-1, and `make test` have not
run. Live LAN ownership and normal image/ROM/lock gates remain integrator work.
