# Clipped solid-color rectangle: complete fresh reconstruction

**NONMATCH research, zero accepted source/data coverage.** Target
`func_8008A46C` at `0x8008A46C`, 472 bytes / 118 words. Base
`0bfebc7367ebc1ddb4d6105b2012ed07fb080faf`. This packet changes no production
source, locks, targets, scorer, data ownership, or build settings.

## New result

`candidate.c` reconstructs the entire function, not a prefix, and compiles as
natural C89. It clips four signed inclusive pixel bounds, rejects inverted or
fully clipped rectangles, emits a pipe sync and packed RGBA primitive color,
updates the cached alpha byte, calls the genuine four renderer helpers in native
order, emits the inclusive-end fill rectangle, then syncs and clears the flag.
It reloads the display-list cursor after helpers, which may append commands.
There are no fabricated parameters, synthetic callers, pressure variables,
volatile steering, assembly, or helper stubs in the IDO candidate.

A fresh strict IDO 5.3 O2 baseline differs at **109/118 words**, with no unknown
symbols, unverified relocations, relocation errors, or nonzero excess words.
Candidate `.text` is 464 bytes including zero alignment; workbench counts 113
instructions (452 bytes), versus 118 native instructions. Native/candidate
frames are both 24 bytes, but allocation and scheduling differ broadly.
`verification.json` records source/object/manifest hashes and flags.

## Provenance and genuine ABI

The authoritative inputs are the repository's manifest-verified target section
and symbol map. Prior PR1–45 source-filename and definition inventories were
checked: no implementation of this target was found. Older `func_8008705C`
research mentions this target as a real caller but substitutes synthetic callers;
this packet supplies the previously missing actual caller body.

The target takes left, top, right, bottom in a0–a3 and a color pointer from
incoming sp+16. Its native frame is 24 bytes, saves ra at +20, and obtains that
fifth argument at frame sp+40. The one direct call in the full target population
is `Input_ProcessGameplayPad+0x520`; its delay slot stores the color pointer at
caller sp+16, and its preceding instructions set all four ordinary argument
registers. No hidden entry arguments are invented.

The clip globals are signed words at 8012E60C, 8012E668, 8012E610, 8012E674.
The display-list cursor is at 80149438, the cached alpha byte at 8011EACF.
No jump tables, float pools, ROM data, or unresolved literals are required.
The callee at 8008A148 accepts the color pointer and -1,-1,0; its native -1
branch updates the cached pointer without reading additional color bytes.
The other helper arguments are mode 1, enable mask 0x4000, disable mask 0x4000.
Their bodies are not claimed or replaced here.

## Why this is not a standalone matching claim

Native retains the four coordinates in t2–t5 and spills/reloads them in its
incoming argument home slots around 8008A148 and 800878E0. Crucially, it keeps
them in t2–t5 across 80086A50 without spills. Ordinary separate O2 compilation
must assume all four may be clobbered, so this is real interprocedural context,
not a reason to change the entry ABI. Workbench diagnosis reports structural
and allocation differences, not a tiny scheduling residual. Its relocation
symbol warnings reflect native objects containing already resolved words;
`score.py` resolves all candidate relocations and is the authoritative receipt.

Next justified experiment: compile this actual caller alongside the complete,
natural 8008A148 / 80086A50 / 800878E0 / 8008705C bodies with authentic callers
and inspect every claimed body's full resolved extent. A partial helper or the
old stub/synthetic-caller groups is not an acceptable shortcut. No broad flag
sweep or repeated source permutation was run. This packet freezes the baseline.

## Behavioral validation and limits

The host-only harness observes call order/arguments and deliberately advances
the display-list cursor inside every helper. It is excluded from all IDO builds.
Seven pytest tests cover 6,561 coordinate-grid cases, single-pixel inclusivity,
five rejection cases, three negative/wrapped-coordinate cases, all 256 alpha
values, inverted clip bounds, and 10,000 deterministic randomized cases:
**16,827 calls in total**. They verify packed channels, masks, clipping, alpha
ordering, command order, and untouched trailing storage. These are source
semantic tests, not a native emulator or a renderer integration test.

Assumptions: 32-bit C ints/unsigned ints, a valid four-byte color object and
sufficient command-buffer capacity on accepted paths, and pixel coordinates
where signed right+1/bottom+1 do not overflow (`INT_MAX` is outside the tested
contract). Pointer aliases between color bytes and the output command buffer
are not modeled. Private ROM image/compression/cartridge gates were not run.

## Reproduction

From repository root (existing approved IDO and host compiler required):

```sh
export IDO_DIR=/path/to/approved/ido
python3 tools/cloud/score.py fn src/blob/sound_handles_clear.c sound_handles_clear
python3 tools/cloud/score.py group src/blob/groups/resource_slot_clear
python3 cloud/work/solid_rectangle_8008A46C/replay.py
python3 -m pytest tests/conveyor/test_solid_rectangle_reconstruction.py -q
```

The repository pytest location makes these semantic checks part of ordinary PR
CI. The compiler receipt is separately reproducible, and a green CI does not
mean this research matches. No raw native dumps, binary objects, or ROM data
are published in this packet.

Object hashes include source-path metadata and may differ between checkouts;
compare the separate `.text` hash and strict comparison results across paths.
