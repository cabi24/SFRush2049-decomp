# Clipped solid-color rectangle: source validation and replay

**NONMATCH research, zero accepted source/data coverage.** Target
`func_8008A46C` at `0x8008A46C`, 472 bytes / 118 words. Base
`0bfebc7367ebc1ddb4d6105b2012ed07fb080faf`. This packet changes no production
source, locks, targets, scorer, data ownership, or build settings.

## Packet result

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
and symbol map. **Provenance correction:** the earlier source inventory missed
PR #9's complete [B119 caller/helper source](../ipa-groups/codex_gfx_rectangle_b119/group.c)
and [71/118 rectangle receipt](../ipa-groups/codex_gfx_rectangle_b119/verification.json).
This packet is not the first reconstruction and did not supply a previously
missing actual caller. Its additional value is seven CI-discovered semantic
tests and standalone replay packaging. The [corrected PR description](https://github.com/cabi24/SFRush2049-decomp/pull/48)
records the same correction; existing source and frozen receipts are unchanged.

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

The complete natural helper closure already exists in B119. A prior-session report records a genuine
five-body follow-up at 75/118 differing rectangle words, worse than the
archived 71/118, and was not republished. Do not repeat that closure as a new
experiment. The next prerequisite is authentic TU/export-root or native caller
allocation evidence that justifies one genuinely new context hypothesis; see
[renderer readiness](../graphics_readiness_20261003/README.md). The mode context
also needs exact-candidate proof for its two references to one local table.
This packet's standalone baseline and test scope remain frozen.

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
