# func_80091B00: tested NONMATCH allocator research

**NONMATCH: 18 of 42 complete instruction words differ.** This is research only,
not a matching submission or accepted cartridge coverage. The candidate's
function symbol and native extent are both 168 bytes (0x80091B00–0x80091BA8).
There are two trailing zero alignment words, no excess nonzero text, unresolved
symbols, relocation masks or unverified data references.

## Behavior and provenance

The native leaf takes no arguments and returns a pointer in v0. It scans 128
24-byte records at D_80142DD8 (exclusive end 0x801439D8). The first record whose
signed byte at offset 3 is zero gets active=1 and signed 16-bit ID=-1, then its
address is returned. If all records are active, the return is null. The remaining
21 bytes of each record are preserved. Compiler-unrolled native traversal checks
four slots per iteration; ordinary C expresses the same 128-slot scan.

This loop is already present as **unclaimed context** in
`src/blob/groups/codex_entity_helpers_a10/group.c` and
`cloud/work/ipa-groups/entity_lookup/group.c`. This work does not claim novel
recovery or a better match. It isolates the ordinary source, records complete
resolved-word evidence, adds a reusable boundary/state-preservation harness,
and independently verifies behavior. It does not modify those existing groups.

Remaining differences are store scheduling and temporary-register allocation.
No padding operations, helper bodies, ABI parameters, volatile declarations,
pointer/integer laundering, register controls or scorer changes were introduced.
33 direct J/JAL call sites in 23 functions are listed in `verification.json`;
indirect callers are not excluded. The no-argument/pointer-return ABI is unchanged.
No direct arcade equivalent is established because reference sources are absent.

## Validation on master a12daa63ae67c8dab3404efb666089c884dadfd4

- Strict scorer: **24/42 full words equal, 18/42 differ**. Expected NONMATCH.
- Reproducible fresh direct IDO compile and independent explicit HI16/LO16
  arithmetic compare all native words, with hashes and full word lists recorded.
- Separate reviewer independently compiles and uses GNU ld/objcopy to resolve
  relocations, confirming all 18 residuals, exact function extent, ABI and loops.
- Author host test: **133,025 ASan/UBSan cases pass**. Covers every first-free
  position including exhaustion for all 255 nonzero active-byte values, 100,000
  random whole-pool states, and repeated allocation through exhaustion. Full
  3,072-byte pool comparison checks untouched bytes and other records.
- Reviewer: **33,024 additional independent cases** (all 256 active-byte values
  across 129 free/exhausted positions), full pool preservation. Also replays the
  author's sanitizer test successfully.
- C89 strict syntax/warnings pass.
- Repository suite: **1,307 passed, 41 skipped, 9 deselected**, exit 0.
- All **161 static locks** intact. Protected native SHA256 manifest: 23/23 pass.
- Current accepted locks, cloud singles and explicit group claims exclude this
  target. Historical context membership is not a claim. PR1–23 inventory and
  current target-name PR search show no competing claimed contribution.
- Fresh master rebase did not change the candidate or its complete target words.

The first suite attempt lacked initialized submodules; these were then initialized.
A subsequent attempt lacked binutils in PATH; the final suite uses the installed
MIPS tools and passes. LeakSanitizer cannot operate under the runtime's ptrace,
so host runs use `ASAN_OPTIONS=detect_leaks=0`; address and undefined-behavior
checks remain enabled. The test performs no dynamic allocation.

## Reproduce

Run from the repository root with the pinned IDO and MIPS tools available:

```sh
python3 cloud/work/dot_slot_allocator/verify.py
cc -std=c89 -pedantic -Wall -Wextra -Werror -fsyntax-only cloud/work/dot_slot_allocator/func_80091B00.c
cc -std=c99 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -Wall -Wextra -Werror cloud/work/dot_slot_allocator/host_test.c -o /tmp/slot-test
ASAN_OPTIONS=detect_leaks=0 /tmp/slot-test
python3 -m pytest tests/conveyor -o addopts='' -q -m 'not node_required'
make check-matched
```

The verifier exits successfully only when it reproduces the explicitly expected
18-word NONMATCH; it does not mean the source matches. The canonical scorer
returns nonzero, correctly rejecting this candidate as a match.

## Limits

Only research files are added under `cloud/work/dot_slot_allocator`. Native
assembly, accepted source, locks, scorer, build wiring and coverage are unchanged.
Host tests check sequential behavior, not N64 execution or concurrent observation
of writes. This allocator has no locking or atomicity guarantee. IDO and host
layout checks establish the documented 24-byte structure used here.

No ROM, complete image or derived blob layout is available. Image splicing,
compressed-stream identity, full-ROM SHA-1 and `make test` were not run. No
promotion should use this NONMATCH candidate. A future exact reconstruction
must pass the normal live ownership, independent matching and image/ROM gates.
