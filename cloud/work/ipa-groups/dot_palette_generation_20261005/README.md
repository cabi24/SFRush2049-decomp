# RGB5551 interpolation: strict 192-byte match with real palette context

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.
Only `func_800B0EA0` at `0x800B0EA0` is claimed. Its complete ELF `st_size`
and relocated extent are both **192 bytes**, equal to the authenticated native
body, with zero differing words, excess words, unresolved references or
unverified data. `verification.json` records matching complete-body hashes.
No image, compression or cartridge gate was run, and no accepted input or lock
was modified. The independent checker still owns integration and ROM acceptance.

## What changed

The older standalone A42 source left 30/48 words different. The missing
context is the complete **genuine** `sound_bank_unload` caller at `0x800B10D4`,
which calls the interpolator at three real sites. The historical caller name
is misleading: it generates and installs a car palette. Defining that full
caller and leaving the interpolator internal to O3 produces the native
four-register temporary ring without fake callers, extra parameters, keepers,
inline blockers, assembly or frame padding.

The same closure includes the accepted heap-release group unchanged. Its
`audio_reverb_update` is actually a heap free operation whose IPA arguments are
in `a1/a2`. The caller saves `s0/s1` for the genuine release call even though it
does not itself otherwise use those registers. The two context files are
byte-for-byte copies of the accepted `codex_heap_release_a25` files; every one
of their **13** emitted functions retains exact full-extent bytes. No new
matching credit is claimed for them. Their existing real roots and keep list
are preserved, with only the actual palette root added.

The interpolator preserves the signed ratio endpoint tests. Zero returns the
entire left endpoint, and any value at least 255 returns the entire right
endpoint. Other values use low-32-bit products and sums, signed shifts and
RGB5551 masks, forcing the alpha bit. Unsigned inverse subtraction avoids C
signed overflow for `INT_MIN` while compiling identically. Negative ratios are
not clamped; callers here use only 0, 8, ..., 240 (or ..., 232).

## Complete caller reconstruction and its limit

`palette.c` reconstructs the entire 1,248-byte native routine:

1. Set the indexed dirty byte, allocate 512 bytes, format the palette name,
   resolve the named 24-byte resource descriptor, and copy its 512-byte palette.
2. If the first selector is zero, brighten entries 1–31; otherwise, if the
   second is zero, brighten entries 33–63. Brightening extracts the red channel,
   expands by eight, scales by 1.25, clamps to 255 and repacks grayscale RGB5551.
3. Read three 32-bit color-table entries. Store endpoints at 32/64/96 and make
   ramps at 33–63 (31 entries), 65–95 (31), and 97–126 (30). Entry 127 and the
   remaining copied palette are untouched. The second brightening range is
   subsequently overwritten by the first ramp, as in the native routine.
4. Copy the resulting palette to the per-car descriptor's data pointer, receive
   the heap queue token, free the temporary allocation and return the token.
5. Publish the descriptor pointer into field 56 of two 68-byte model records,
   selected by signed halfword IDs at offsets 2 and 18 in a 64-byte handle record.

There is no 24-byte descriptor copy: 24 is the descriptor array stride; both
actual `memcpy` operations copy **512 palette bytes**. The native fifth lookup
argument is the literal one even though the accepted lookup body consumes only
four arguments; the external call is preserved with all five native inputs.

The native filename format is read through its existing authenticated symbol,
without republishing string bytes. Its maximum result for this signed-halfword
palette parameter is 15 bytes including the terminator, fitting the reconstructed
32-byte name buffer. Native IDO compilation independently confirms the modeled
record sizes 24/64/68 and accessed offsets 20/56/2/18. Other record fields and
table capacities remain unnamed/unproved; the routine expects valid indexes,
successful allocation and successful resource lookup just as the native body does.

**The caller is NONMATCH.** Final ordinary pointer-ramp source has 262/312
positional words different, 192 nonzero excess words, ELF `st_size` 2,048,
and 2,060 bytes to padded text end. Its frame is 128 rather than 192 bytes,
with the correct three saved registers. Direct brightening unrolls by four,
where native unrolls by two. The broad caller residual does not become matching
credit merely because the callee now matches.

The nearby accepted empty `func_800B0F60` could be evidence of an inlined color
helper, but neither its original signature nor body is established here. It
was not modified or replaced. Further progress requires genuine source/context
evidence for that loop and frame structure, not invented locals or ABI formals.

## Bounded rejected controls

The complete sources under `controls/` are research only; `verify.py` replays
all three using the same real context and records source-bound counts:

- Indexed ramps: 263/312 different, 191 nonzero excess; interpolator exact.
- Separately scaled RGB channels: 279/312 different, 40 nonzero excess;
  interpolator exact. This different unrolling shape does not justify the
  added channel declarations as original-source recovery.
- Natural out-of-line brightening abstraction: 309/312 different, two unresolved
  local helper calls. The helper was not inlined; the canonical scorer also
  reports 25 excess words after the interpolation symbol. This is rejected and
  **not** a second matching recipe. No forced-inline or deletion trick was tried.

Workbench diagnosis on the first full baseline reported a structural mismatch,
including equal 12-byte register-save areas but a 72-byte non-save frame deficit
(120 versus 192). After two genuine consumed ramp locals, the deficit is 64.
The diagnosis's relocation warnings compare resolved native words with raw ELF
relocations; canonical full-word relocation scoring is the authoritative proof.
No allocation permutation sweep was used.

## Reproduce

Use pinned IDO 5.3 through `IDO_DIR` and the existing MIPS binutils. From the
repository root:

```
python3 cloud/work/ipa-groups/dot_palette_generation_20261005/verify.py
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_palette_generation_20261005 --claims
python3 -m pytest -q tests/conveyor/test_palette_generation.py
```

The JSON replay is deterministic and must equal `verification.json`.
Objects, diagnosis output and assembly stay under ignored `build/`.
The existing changed-submission CI discovers this group and strictly checks
its single `claims` entry.

The host test checks 983,040 interpolation cases: every 16-bit left color with
15 ratios including signed extremes, using a different unsigned 64-bit reference
formula. It also checks 4,098 complete palette cases, event order, queue arguments,
copy extents, untouched entries and guard words, plus exactly two model-pointer
publications. The host services are checked stubs, not tests of those services
or N64 execution. The same test passes AddressSanitizer and UBSan with
`ASAN_OPTIONS=detect_leaks=0`; LeakSanitizer is not claimed under the executor.
