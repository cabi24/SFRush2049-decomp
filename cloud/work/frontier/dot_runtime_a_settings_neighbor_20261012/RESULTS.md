# Image-A settings values: 2,656 newly strict candidate bytes

## Deliverable

`groups/dot_runtime_a_settings_values_20261012/` contains the complete kept
callback **A:func_803AECE4**, native extent `0x803AECE4..0x803AF744`:
**664/664 words MATCH, 2,656 bytes**. Its sole member is its sole claim.
No prior exact function is recounted and no installed group is migrated.

The prior complete reconstruction in
`cloud/work/lean/runtime_a_settings_values_20261006/candidate.c` (research
PR #221) reproduces 270/664 differing words on master `3bec9ce30`.
New matched candidate bytes: **2,656**. Preserved older match bytes: **0**.
New accepted-image and ROM bytes: **0**. Keep the PR draft; installation,
cartridge verification and merging belong to the independent checker.

## Trace-first source fixes

Workbench diagnosis identified mixed local-layout, floating temporary and
expression-evaluation differences, with identical 664-word geometry and a
216-byte frame. Native use sites and read-only stock-fidelity uopt/ugen
traces supplied specific fixes, applied without a combinatorial sweep:

| Source step | Differing words |
| --- | ---: |
| Existing complete reconstruction | 270 |
| Reorder existing declarations to native homes | 232 |
| Split the existing position-Y assignment/subtraction | 192 |
| Group label table-base indexing before its language offset | 188 |
| Order existing vector writes Z, X, Y/subtraction | 180 |
| Reuse existing label before the label draw | 171 |
| Reuse label for switch case 0 before coordinate narrowing | 159 |
| Reuse label likewise for switch case 1 | 119 |
| Group six conditional table bases before their language offsets | 0 |

The native homes are: i=sp+204, text=sp+124, label=sp+120,
point=sp+116, position=sp+104 and index=sp+100. Moving existing scalar
names, rather than changing any object capacity, recovers every home.
The 80-byte text object ends at sp+204 in the unchanged 216-byte frame.

The vector rewrite makes the existing Y component a coloured f0 value and
recovers the floating temporary sequence. Existing label assignments put a2
in place before coordinate narrowing. For conditional values,
`(indexed + selected_boolean)[language_offset]` expresses the native
base-plus-selected-offset calculation before indexing by the language base;
the old `indexed[selected_boolean + language_offset]` evaluated a different
expression tree. All operands and accesses are preserved.

Read-only diagnostic baseline and final objects equal their respective stock
objects byte for byte; compact facts and hashes are in `trace.json`. W5D's
level-3 debug output crashed on this TU, so it is not evidence; ordinary
colour and ugen traces completed. No forced/oracle object, compiler change,
unused local, synthetic call, ABI trick, padding or capacity reduction is used.

## ABI and data boundaries

The single real callback parameter is retained, and the callback stays kept.
Native callback creation/dispatch and every relevant native callee body are
input-bound in the receipt. The authenticated native callback pointer at
A:803B7CC8 targets AECE4. The font selector returns its previous selection;
this callback ignores the genuine integer result. Font/color/draw helpers
retain their actual signatures, including signed 16-bit coordinate arguments,
float alpha/render state, a five-argument projection with a two-halfword
output, and the two four-byte aggregate colors passed by value. Color4 is a
word-aligned view of those native RGBA words, not a change to their ABI.

This is the full earlier reconstruction: title/warning, selected option loop,
all value cases and final render-state restoration. Native table/record fields
and the same 80-byte text, two-short point and three-float position objects are
preserved. No exact arcade donor, original identifier or full record layout is
claimed. As before, selected indices/language tables must be valid, formatted
strings must fit the existing text object, and projected numeric values must
be representable. Instruction identity is not a broader memory-safety claim.

## Verification

```
python3 cloud/work/frontier/dot_runtime_a_settings_neighbor_20261012/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_settings_neighbor_20261012/verify.py
python3 cloud/work/frontier/dot_runtime_a_settings_neighbor_20261012/verify.py --score
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_settings_values_match.py -o addopts='' -q
```

Canonical output without `--claims` or `--allow-unverified`:

```
Members:
func_803AECE4:
  MATCH
    own .rodata verified at 0x803B99E0..0x803B9A4C
```

The wrapper authenticates the committed asset, loader pointer and complete
image A before supplying in-memory owned data to the unchanged scorer.
Without image data, the 14 section references remain unverified and fail.
Corrupted literal and jump-table inputs are separately rejected. This input-bound v2 receipt pins source,
verifier, compact trace facts and consumed target rows, not shared tool hashes,
whole locks or assembly text. Replay needs no non-ancestor Git object.

Independent GNU linking resolves all **145 text and 21 table relocations**,
verifies every native instruction, the 2,656-byte STT_FUNC extent, all 108 owned
literal/table bytes plus four zero data-padding bytes, and exact section
placement. No retained relocations or unexpected allocated section is allowed.
There is no extra text padding. Normal and optimized-Python replay pass.
One cheap test checks receipt inputs before its shared missing-toolchain skip.
No ROM bytes, raw assembly dumps, objects, image files or secrets are published.
No full-image, compression, gameplay, hardware or ROM result is claimed.
