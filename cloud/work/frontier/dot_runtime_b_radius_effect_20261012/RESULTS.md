# Image-B radius effect: full source and read-barrier boundary

**NONMATCH research. Empty claims; zero new accepted strict-match, image or ROM bytes.**

The source-first deliverable is `group.c` / `group.json` for B:func_8038D798,
native `[0x8038D798,0x8038DA78)`, 736 bytes. The ordinary nonvolatile source is
complete but emits **760 bytes**, with **125/184 differing words and six extra
nonzero words**. The canonical scorer also reports four unverified own-data
relocation sites because their instruction positions differ. Independent GNU
linkage resolves the entire real object, verifies the literal bytes, and agrees
with every difference and the complete extent. None of this is a match.

Base: master `4f59607f7b15e252cfcbab67b9eccb6d9e6afba5`. The 14 accepted
image-B functions and pending #353 (91650) were excluded from new yield.

## Useful boundary, not a matching claim

The complete native function applies a squared-distance-weighted effect to
eligible players. Kind 5 transforms the direction vector, obtains the actual
native angle helper's result, and attenuates within the strict angle threshold.
The five inputs are two float-vector pointers, a signed player index, a float
squared-radius argument in the fourth O32 argument slot, and an integer
magnitude in the first stack slot. The three vectors are genuine three-float
values. No capacity change, padding local, fake formal, artificial caller or
inline assembly was introduced.

A **diagnostic-only** source control adds `volatile` to the signed player-count
view. Together with moving the two existing pointer declarations around the
three arrays, stock IDO 5.3 then emits all **184 native words and eight native
literal bytes exactly**. Original declaration order is unknown; the native
homes are direction sp+152, transformed sp+140 and delta sp+124. Reverting
only those declaration positions leaves 13 differences, all genuine vector
home displacements, with the same 168-byte frame.

The volatile control is generated only in temporary storage by `verify.py`.
It is **not** the submitted `group.c`, a claim, an installation candidate, or
proof that the original source was volatile. Unconditional native reloads do
not by themselves establish original C qualification or an asynchronous read
contract. No claim that this qualifier is harmless in every caller environment
is made. Current project policy/native semantic evidence must establish that
contract, or a faithful nonvolatile source explanation must be found, before
this control can be considered for matching credit.

## Trace-first findings

The first natural source had 179/184 differing words and 13 extra words. Using
one real distance/weight local rather than two stops the weight division being
sunk separately into all three effect-call branches. The corrected natural
baseline was 137/184 plus six extra words. Recovering the existing local homes
reduced it to the submitted 125/184 plus six extra words.

Workbench diagnosis classified the genuine residual as structural, rather than
a register-name-only mismatch. Stock UGEN diagnostics show the plain source
has three direct symbolic count loads, while the read-barrier control has two
separate count-address materializations and the native loop structure. All
three forms have M64/T16/P20 local-area accounting and a 168-byte frame.
`trace.py` proves that turning stock UGEN tree/listing diagnostics on changes
no object byte for any form.

A separate available instrumented UOPT build, with force controls disabled,
produced entire objects identical to stock. It exposes ordinary count ILOD
versus diagnostic volatile ILDV and ordinary PRE of the player-array endpoint.
Integer pointer round-tripping was investigated, not retained: it restores
much of the geometry but creates a costly converted-address web (save 12,
cost 5), allocates it in s4, displaces the genuine vector address and expands
the frame to 176. A forced split diagnostic does not close the body. Disabling
the endpoint PRE bit fails with UOPT internal error 1115; no inference or score
is taken from that failed run. Genuine kept D3A4 context was also checked: its
old 244-byte body remains exact, but D798 does not improve, so no duplicate
accepted body or group migration is submitted. No blind variant sweep occurred.

## Native contract census

Authenticated direct callers are game func_8010C974+0x950, and image-B
func_8038E114 at +0xBCC/+0xFC8/+0xFE8/+0x10AC/+0x10C8. Full selected caller
bodies are input-bound. C974 supplies the same pointer for origin and endpoint,
400.0f for squared radius and integer 1600 for magnitude. E114 uses the same
five-slot convention. Known caller paths proceed to another call without
consuming a D798 result; the submitted void return describes that consumed
contract, not the unknowable original declaration or every indirect caller.
The genuine D3A4 target is an accepted ordinary-O32 helper and remains external.

F938 was also inspected before source work: its native incoming s0/s2/s3/s5
values and unsaved scratch s1/s4/s6 are a private convention. It is not an
ordinary four-player callback that can be installed alone. Together with
D200/D328/E088/D498/DA78, it needs real FCE0/E114 context work. No F938 body,
stand-in, fake ordinary ABI or matching claim is submitted. F568 and C910 were
not retuned.

## Reproduce and verification

From the repository root with recognized stock IDO 5.3 and GNU MIPS binutils:

    python3 cloud/work/frontier/dot_runtime_b_radius_effect_20261012/verify.py
    python3 -O cloud/work/frontier/dot_runtime_b_radius_effect_20261012/verify.py
    python3 cloud/work/frontier/dot_runtime_b_radius_effect_20261012/trace.py
    python3 -m pytest tests/cloud/test_runtime_b_radius_effect.py -q

Canonical group score, correctly exiting nonzero:

    python3 cloud/work/frontier/dot_runtime_b_radius_effect_20261012/verify.py --score

    Members:
    func_8038D798:
      125/184 words differ (6 extra words (nonzero beyond target length); 4 section-relative relocations unverified: .rodata+0x0 at +0x88, .rodata+0x0 at +0x8c, .rodata+0x4 at +0x90, .rodata+0x4 at +0x9c)

- Independent GNU reconstruction resolves all 27 plain-source relocations and
  all 25 relocations in each diagnostic, using the complete original objects.
- ELF function extents, native next boundary, all words, external bindings,
  allocated sections and zero suffixes are checked. Plain text is 760+8 zero
  alignment bytes; diagnostic text is 736. Each has eight actual literal bytes
  followed by eight zero object-alignment bytes, not eight more native literals.
- MIPS2, big-endian ELF identity is checked. The volatile diagnostics additionally
  carry the normal EF_MIPS_NOREORDER bit; plain source does not. All final links
  have no undefined symbols or remaining relocations. The compiler's unused
  intrinsic fabsf declaration has no relocation and is omitted by GNU linkage;
  an unknown referenced symbol fails closed.
- Missing own data and independent corruption of either exact diagnostic
  literal fail closed. The plain source retains its scorer uncertainty.
- The receipt binds source, recipe, verifier, trace and focused test, selected
  full target/caller/helper inputs, referenced symbols and all eight stock
  compiler executables. No mutable shared-tool, lock, Makefile or asm-text pin.
- Normal and Python -O deep replay pass; the one focused test passes. With IDO
  absent it cleanly skips through the shared conftest. No static ROM TU is built.

This is compiler/native evidence, not a new executed behavioral oracle. Valid
backed storage, indices, helper contracts and finite in-range conversions are
assumed. Arbitrary partial aliases, asynchronous mutation, exceptional FCSR,
real helpers, full gameplay, image composition/recompression and full-ROM SHA-1
remain outside the claim. The unresolved read contract is the stopping point.

Only text source, tests and research receipts are publication artifacts. No ROM
bytes, raw native/assembly dumps, objects, compiler binaries, protected changes,
credentials or unrelated data. Keep draft; the independent checker owns merging.
