# Image-A per-track statistics renderer: strict match

## Deliverable

Fresh master: `4f59607f7`.
Source-first group: `groups/runtime_a_stats_menu_20261006/`.

New `func_803AD0D4`: complete `[0x803AD0D4,0x803AD574)`, **1,184 bytes**,
296 words. Its complete 14-entry switch table is separately verified over
`[0x803B9948,0x803B9980)`, **56 bytes**.

All nine accepted statistics members remain byte-identical, retained in both
`members` and `claims`: AAF0C 1,776; AB5FC 588; AB848 1,664; AC330 540;
AC54C 680; AC7F4 664; ACA94 712; ACD5C 512; ACF5C 376. These **7,512
preserved bytes** earn no new credit. Same group identity, same real AD57C
root, no lock migration. AA3D8, ABEC8 and AD57C remain complete NONMATCH
context. Zero accepted-image or ROM-verified bytes claimed. Keep draft;
installation and merging belong to the independent checker.

## Trace-first source correction

The fresh genuine-group baseline is 145/296 differing words with three extra
nonzero words, true 1,196-byte extent and a mismatching switch table. Initial
workbench diagnosis identifies a real three-instruction structural increase,
plus stack-home differences; relocation-blind diagnostic counts are not
acceptance evidence.

1. Native code initializes its average to zero before conditionally dividing
   the stored total time by the unsigned-halfword count. An explicit useful
   average local and ordinary if recover that ordering; the prior ternary
   emitted an extra branch-likely instruction and shifted all later cases.
2. The two bit-count arguments retain their unsigned-halfword narrowing.
   This is the idiom already proven in accepted ACA94. These casts preserve
   the mathematical values but restore native argument and temporary order.
3. Existing declaration order is recovered from native homes: track/i precede
   x/y, label precedes the unchanged text array, and data follows it. The
   native x/y/text positions are sp+132/sp+128/sp+76.

Stock UGEN confirms frame 144, M108/T20/P4 and internal visibility for final
and every control. Text capacity stays 48 bytes. No added unused local,
capacity reduction, artificial access, fake formal/helper/caller, volatile,
forced register, assembly or modified compiler is used. The useful average
local does not enlarge the memory area. Original spelling is not claimed.
Every diagnostic-on/off object is byte-identical.

Full causal rollback results, all with nine preserved bodies still exact:

- Prior ternary only: 143/296, one extra word, true extent 1,188 bytes.
- Wide bit-mask arguments only: 66/296, two extra words, 1,192 bytes.
- Prior declaration order only: 5/296, exactly five native-home differences,
  same complete 1,184-byte extent and owned table.
- Entire current-master baseline: 145/296, three extra words, 1,196 bytes.

Independent GNU comparison includes the complete extra suffix and reports
144 / 68 / 5 / 148 full-word differences respectively. Every old member and
all 96 bytes of its owned tables remain exact in every GNU control.

## Canonical score

Run `python3 cloud/work/frontier/dot_runtime_a_track_stats_20261012/verify.py --score`.
The wrapper authenticates the current-tree image in memory and invokes the
unchanged group scorer without `--claims` or `--allow-unverified`:

```text
Members:
func_803AAF0C:
  MATCH
func_803AB5FC:
  MATCH
func_803AB848:
  MATCH
func_803AC330:
  MATCH
func_803AC54C:
  MATCH
    own .rodata verified at 0x803B98E8..0x803B990C
func_803AC7F4:
  MATCH
    own .rodata verified at 0x803B990C..0x803B9928
func_803ACA94:
  MATCH
    own .rodata verified at 0x803B9928..0x803B9948
func_803ACD5C:
  MATCH
func_803ACF5C:
  MATCH
func_803AD0D4:
  MATCH
    own .rodata verified at 0x803B9948..0x803B9980
```

## Complete proof and behavior checks

- Independent original-REL GNU reconstruction resolves 429 member text
  relocations, including AD0D4's 71, and 38 table relocations. All ten full
  STT_FUNC extents, native next boundary, full words, zero suffix, placements
  and 152 owned bytes verify. Table destinations must belong to one of the
  four actual owners. Undefined bindings, residual relocations, unexpected
  allocated sections and unsupported local references fail closed.
- Each of the four table owners rejects missing and corrupted owned data.
- Native direct caller census is exactly AD57C+0x574. AD0D4 is a no-argument
  internal procedure with the native private clobber convention; only the
  genuine complete AD57C callback is kept. No standalone o32 claim is made.
- All source outside the target body, apart from one disclosed header, is
  checked byte-for-byte against the current-tree earlier packet. The archived
  baseline remains available on master; no non-ancestor git object is needed.
- 4,800 cases execute the actual complete submitted C with deterministic
  draw/formatter doubles: both profile modes, both distance units, all 12
  selectors and 100 value patterns. They cover zero/full halfwords, unsigned
  distance wraparound, both flag masks, zero/nonzero times and averages,
  exact output strings, selected label/position order and unchanged records.
  The same matrix passes ASan/UBSan, LeakSanitizer disabled only.
- Native formats are authenticated. Halfword decimal writes need at most six
  bytes; the distance quotient/remainder needs at most 12 including NUL;
  AC330 time formatting/fixed placeholder needs ten; bit counts need four.
  No language string enters this text buffer. The existing 48-byte capacity
  stays unchanged. The formatter/copy implementation is not newly executed.

Normal and Python `-O` deep replay pass. One focused test passes in about
1.2 seconds. No IDO cleanly skips after input validation. The packet binds
source/recipe/verifier/test/trace/harness, actual selected native rows,
referenced symbol bindings, genuine caller and ten helper target rows, and
eight stock IDO executables. No shared-tool, lock, Makefile or assembly-text
hash pins. No static ROM TU is rebuilt. All 619 static locks and 89 overlay
entries remain intact in the complete selected checkout.

Reproduce with stock IDO 5.3, GNU MIPS binutils and a host C compiler:

```sh
python3 cloud/work/frontier/dot_runtime_a_track_stats_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_a_track_stats_20261012/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_track_stats.py -o addopts='' -q
```

## Independent review

Paired-lane independent review replayed normal and optimized-Python deep proofs,
the strict focused test (1.10 s), and whitespace checks; audited source/ABI,
complete extents, original-REL GNU reconstruction, four owned tables, caller
census and input bindings. No blocking findings. One stale verifier docstring
was corrected from three tables to four, then the receipt was rebound and the
same checks repeated.

## Limits

Valid records and language pointers, selectors 0..11, synchronous helpers
and finite representable time conversions are assumed. Native behavior for
invalid selectors, aliases, asynchronous writes, corrupted helpers, unusual
FCSR modes, NaN/infinity/subnormal inputs and full gameplay/hardware is not
established by the host checks. The inherited unsigned time conversion has
its usual representability precondition. No original whole-TU boundary,
array capacity, source identity, image splice/recompression or cartridge hash
is asserted. Three context functions remain unclaimed NONMATCH. Publication
contains source/tests/notes only, no ROM bytes, raw assembly dumps, objects,
credentials, protected edits or unrelated data.
