# Name-cache allocation: executable semantic audit, NONMATCH

Target `func_800F1D04`, `0x800F1D04..0x800F207C`, 222 words / 888 bytes.
No match or cartridge coverage is claimed. No protected files are changed.

## Provenance and added value

The reconstruction is retained **unchanged** from `cloud/work/tiny_A68/func_800F1D04.c`
on master a12daa63. A68 already documented its source, layout, signed-age behavior,
and 104/222 mismatch. This packet does not claim a new reconstruction or score
improvement. Its new contribution is executable native/linked/host differential
validation, exact state and external-call traces, sanitizer coverage, and an
executed counterexample for the previously unproved all-age-wrap eviction case.

`verify_semantics.py` runs 1,162 cases: all 20 slot positions across seven unsigned
age boundaries on hit, first empty, first unreferenced, and eviction paths,
plus signed-age cases and 600 deterministic randomized states. It compares:
- protected native instruction words;
- fresh IDO candidate, relocated independently by GNU ld/objcopy;
- optimized host compilation of the same candidate source;
- independent Python state-transition oracle.

Every result, all 20 names (including bytes after their terminators), all 20 ages,
and all 180 references agree. Native and linked-candidate strcmp/strcpy call traces
also agree. The bounded interpreter rejects unknown instructions, unmapped and
unaligned accesses, bad control flow, and ABI preservation failures. External
calls use only verified byte-compare/copy contracts, with caller-saved registers
clobbered. It is not a whole-game emulator.

`sanitizer_test.c` passes 20,000 bounded randomized cases under ASan and UBSan.
Leak checking is disabled because LeakSanitizer cannot operate under this
container's ptrace; this allocation-free harness does not test leaks.

## Important semantics and limits

The cache has 20 names of 13 bytes, unsigned-half ages, and a 12x3x5 full-word
reference table. Ages wrap before searching. Existing name wins, then first
empty name, then first unreferenced slot, then signed-short age selection.
It is **not** an ordinary unsigned oldest-age policy: assigning an age >=32768
to the signed-short accumulator makes it negative, so later smaller ages can win.
References outside 0..19 are ignored when counting, but only exact selected-slot
references are invalidated on eviction.

Native code speculatively reads an uninitialized best-index halfword before
scanning eviction candidates. In 433 covered defined evictions that value is
subsequently overwritten. If every age wraps from 65535 to zero in a full,
fully-referenced cache, no candidate is selected: zero-filled stack chooses slot
0, while 0x01-filled stack attempts an unmapped name access via slot257. This
undefined state is deliberately excluded from host C tests and is **not repaired**
by inventing an initializer. Strings must terminate within 13 bytes; incoming
name must not overlap cache storage. No claim extends to larger/unterminated names.

## Matching evidence and extent correction

`replay.py` freshly compiles with pinned IDO 5.3 and canonical O2 flags, compares
all resolved words, and independently links all five undefined symbols using GNU
ld. Canonical relocation and GNU bytes agree with no masks, unresolved symbols,
or unverified relocations. Result remains **104/222 differing words**.

Crucially, ELF symbol size is **884 bytes / 221 instructions**, followed by one
zero alignment word, versus the native 888-byte body. A68's “222 compiled words”
was section extent, not actual function extent. The native/candidate frames are
120/112 bytes. Workbench diagnosis confirms a one-instruction structural gap
alongside allocation and stack offsets. No padding, unused locals, fake helpers,
argument inventions, or protected-source changes have been introduced. O3 and
real-helper group controls already exhausted in A68 are not blindly repeated.

## Reproduction

Set IDO_DIR to the pinned compiler directory and put GNU MIPS binutils on PATH.

    python3 cloud/work/dot_name_cache/replay.py
    python3 cloud/work/dot_name_cache/verify_semantics.py
    cc -std=c89 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer cloud/work/dot_name_cache/sanitizer_test.c -o /tmp/name_cache_test
    ASAN_OPTIONS=detect_leaks=0 /tmp/name_cache_test

`verification.json` and `semantics.json` contain machine-readable evidence.
Independent review is recorded separately. Image/splice/ROM checks remain unrun.
