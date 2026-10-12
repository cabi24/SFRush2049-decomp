# Projected option callback closure, 2026-10-12

Baseline master: `b37643eec4841043dd5b8157fd8309af92e9b12e`.
Population: runtime image A, stock IDO 5.3 whole-program O3.
Flags: `-g0 -O3 -mips2 -G 0 -non_shared`.

## Result

Fresh code: `func_803ADDA8`, `[0x803ADDA8,0x803AE1C0)`, 1,048 bytes.
Preserved: `func_803AE1C0`, 860 bytes, and `func_8038F744`, 88 bytes.
Every function defined by this group is a member and a claim. The existing
installed group name and real AE1C0 callback root are retained. No cross-group
ownership migration, production-source change or new lock is requested here.
New image acceptance / ROM-verified coverage: **zero pending checker**.

Exact scorer output, with authenticated current-tree image data bound in memory:

```text
Members:
func_803ADDA8:
  MATCH
    own .rodata verified at 0x803B99C0..0x803B99D4
func_803AE1C0:
  MATCH
func_8038F744:
  MATCH
```

Command: `python3 cloud/work/frontier/dot_runtime_a_option_eligibility_20261006/verify.py --score`.
This invokes ordinary `score.py group DIR --targets asm/us/ovl_a`, without
`--claims` or `--allow-unverified`. All comparisons have zero differing words,
unresolved symbols, unverified relocations, errors and extra words.

## Trace-first source changes

The old group freshly scored 6/262 differing words for DDA8, with both installed
members already exact. Workbench diagnosis confirmed a 16-byte frame excess
(128 versus 112) plus four allocation/scheduling words, not a missing instruction.
Its relocation-symbol warnings came from comparing a fully fixed target to a
relocatable object; the strict scorer and independent GNU link resolve them.

The allocator trace had the index-times-four expression at web 51 and the
position address at web 87, both with save=4 and nocs=5. The earlier web won
`s1`; the position address received `s2`. A forced-colour diagnostic demonstrated
that changing the colours alone still left the two instructions reversed.
The trace binary produced identical unforced uopt output and object code;
no patched compiler is used in any verification or deliverable.

The source fix is to name the position pointer and use indexed angle loads,
instead of naming the entire slot. This moves the genuine position lifetime
before the index expression. Keeping the old 48-byte buffer and old declaration
order then leaves only the two frame-adjustment words wrong: **2/262**. Both
installed members stay exact. `verify.py --deep` recreates this control and the
original 6-word baseline, without keeping a directory of speculative variants.

## Buffer capacity and full stack lifetime

The reduction to a 32-byte projected text buffer is separately bounded; matching
a frame alone is not the argument. The original source capacity is unknown.
The previous 48-byte declaration was a reconstruction, not proven ancestry.

1. Authenticated current-tree image A has a simple decimal integer format at
   `D_803B8CDC`; the receipt pins its hash and verifies that format semantically.
2. The sole writer is `fcvt_wrapper(text, D_803B8CDC, j + 1)` inside `j=0..3`.
   The argument is therefore 1..4. No input text, language table, precision,
   width or other unbounded argument is formatted into this buffer.
3. The accepted game wrapper at `src/blob/fcvt_wrapper.c` forwards the ordinary
   o32 varargs to locked `fcvt` in `src/rom/lib_34a0.c`. Its decimal path has
   zero width/default precision and emits one digit; `PRINT` and `FLUSH` in
   `include/static_formatter_context.h` copy the exact length and append NUL.
   This call writes exactly **two bytes**, including the terminator.
4. The buffer address is passed only to that writer, the immediate text-width
   reader `sound_pitch_diff_halved`, and the immediate text-render service
   `state_utility`. The width path (`object_manager_update`) reads the terminated
   narrow string. The renderer either renders synchronously or copies string
   content into its own queue; the caller does not publish or retain its stack
   pointer. Each iteration rewrites the same two bytes before those reads.
5. The native frame is 112 bytes. Exhaustive native stack-reference census
   identifies only address-taken locations 56 (projected point) and 60 (text).
   Point occupies [56,60), text [60,92), position/y/x/j homes are 92/96/100/104,
   and the live spilled outer index is at 108. The return address is at 28;
   outgoing argument stores are at 0, 4 and 16. The frame ends at 112. No object
   overlaps another or extends beyond the frame. The scalar homes are a
   reconstruction from IDO declaration order; only the emitted live homes and
   pointers are independently visible in retail instructions.
6. Declaring the real `i` first and the other four scalars before text produces
   that layout. Stock ugen metadata changes M from 92 to 76, while T=20 and P=4
   remain unchanged; frame128 becomes112. All used stack offsets remain native.
   `trace.py` checks metadata and proves diagnostic flags leave the entire stock
   object unchanged. It emits no raw compiler listings.

Thus 32 bytes is comfortably within the proven write/read requirement while
reproducing the native non-overlapping frame. No pad array, synthetic pressure
local, fake formal, volatile, inline assembly or false return type is introduced.
AE1C0's separate 48-byte buffer is unchanged. A peer independently authenticated
the format and checked the formatter/loop bound before this closure was claimed.

## Additional verification

- Independent ELF reader and GNU MIPS assembler/linker resolve complete native
  extents at their image-A addresses and require every final word equal retail.
  The five owned float values are verified over all 20 meaningful bytes; the
  remaining 12 bytes of the compiler's section are zero alignment padding.
- Actual installer `packet_spec`, `member_slices` and `relocate` reproduce all
  three complete bodies. Missing owned data is refused; independently changing
  each of the five floats is refused by both scorer and installer. No install
  or lock write is performed.
- IDO C89 layout assertions check 32-bit integers/pointers, the 4-byte word-aligned
  by-value `Color4`, 60-byte slots with position offset48, 152-byte camera and
  20-byte language record. Source uses the existing call ABI throughout.
- The two installed function bodies are source-identical to master and freshly
  strict-exact in the changed group. Negative controls also preserve them.
- A host semantic harness checks 640 projected/flat cases spanning four angular
  paths, five availability counts, four player selections and four menu states.
  All formatted outputs are one digit plus NUL. This is supporting evidence,
  not an N64 behavioural oracle or proof about original declarations.
- One cheap pytest fixture uses the shared missing-tool policy. The full
  verification includes no static ROM TU rebuild. If one is added later, it
  must use `tools.conveyor.compilers.tu_cc`, not the overlay compiler default.

Remaining context assumptions: valid language/index tables and slot arrays;
finite representable projection results; existing inferred external prototypes
and color aggregate ABI. No whole-function arcade ancestor has been established.
The independent checker retains installation, ROM identity and merge ownership.

## Independent peer review

Lane 2 independently replayed the frozen source, normal/optimized-Python deep
verification, canonical group scoring, stock memory-area trace and
REQUIRE_TOOLCHAIN test. It authenticated the decimal format and source bound.
Review found four proof-only issues, fixed without changing C: fail-safe scorer
state restoration (including cached None), scorer exit-status propagation,
actual installer corruption controls and explicit GNU owned-section VMA/anchor
checks. The updated proof and synthetic failed-asset restoration checks pass.

Additional local scorer regression command:
`REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_cloud_score.py -q -k "not locked and not match_from and not all_"`
passed all 35 selected tests. The full locked-source corpus is left to CI;
this lane uses a sparse worktree rather than materializing every game source.
