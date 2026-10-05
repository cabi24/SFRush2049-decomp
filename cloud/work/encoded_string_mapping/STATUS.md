# A150C encoded-string mapping: bounded one-word nonmatch

2026-10-05, based on master `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.

## Result

`func_800A150C`, `[0x800A150C, 0x800A1644)`, remains **NONMATCH**.
The fresh ordinary IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared` compile
has the full **312-byte ELF function extent**, 78 target words, zero frame,
and exactly **one differing word** after all four table relocations resolve.
The object's extra eight bytes are zero section alignment outside the function;
they earn no credit. There are no unresolved symbols, unverified references,
nonzero extra words, owned data, or scorer errors.

The remaining word at function offset `0xD4` exchanges the two operands of a
symmetric equality-test branch. That establishes equivalent execution for this
one difference, not byte identity or acceptance. No source-level impossibility
is claimed. Claims remain empty; no image/ROM gate or cartridge credit is claimed.

## Actual contract

- Three consumed arguments: output byte pointer, encoded input byte pointer,
  and an unsigned-byte output limit. No calls or non-ABI input registers.
- A first byte of 255 selects big-endian two-byte codepoints; only a zero pair
  terminates that encoding. Other inputs terminate at a zero byte.
- Each codepoint is searched in the real 256-entry unsigned-halfword table
  `D_8011EAEC`. The first matching index is written as a byte, including index
  zero. Unmapped codepoints consume input without producing output.
- The original equality check stops after the requested number of output bytes.
  Remaining requested length is zero-filled, with no added terminator.
- **Original zero-limit quirk:** there is no entry capacity guard. If the first
  symbol maps, output can continue to the input terminator. If the first symbol
  does not map, count remains zero and the equality test stops immediately.
  This reconstruction deliberately does not invent safer behavior.
- The sole direct caller, `track_process_main`, calls at offsets 164 and 184
  with actual limits 16 and 4 and distinct stack destination regions. No local
  temporary-buffer capacity is assumed for this leaf.

No arcade ancestor has been established; the arcade reference checkout was not
present in this cloud worktree. Neighbor `func_800BE744`'s verified unsigned-byte
cursor pattern was a useful family clue, not proof of this routine's source.

## Replayed historical controls and bounded source work

The frozen `tiny_A110` files are untouched. Fresh controls are in
`verification.json` and are reproduced by `verify.py`:

| Source | Flags | Differing words | Nonzero extra words |
|---|---|---:|---:|
| Historical final with promoted length snapshot | O2 | 70/78 | 0 |
| Historical genuine separate output cursor | O2 | 77/78 | 3 |
| Same genuine output-cursor source | O3 | 4/78 | 0 |
| Final real byte-codepoint capture after index initialization | O3 | 1/78 | 0 |

The change from four words to one expresses the consumed byte-mode value capture
in the lookup loop's initializer, after its actual index initialization. This
recovers the native copy schedule without adding runtime work. Separate real
input/output cursors and the genuine narrow limit retain the original register
and zero-frame shape. The source contains no fake arguments, padding, dead reads,
volatile pressure, helpers, masks, assembly, or forced instruction encodings.

A bounded set of 24 semantic source controls explored genuine codepoint widths
and scope, first-byte cursor use, direct table/input reads, the lookup initializer,
condition capture, and equality/inequality source forms. These exploratory files
remain local; none improved the one-word result. Widened map-entry captures changed
the operand order but put the real load in a different register, leaving two
words. Natural halfword codepoint carriers worsened allocation. Direct byte
comparisons removed the native promoted-value copy. This is evidence to stop
this pass, not proof that the compiler cannot emit the remaining form.

Independent review confirms the ABI, full extent, relocations, symmetric residual,
and original edge behavior. Its final receipt is `peer_review.json`. The earlier
workbench L67 rule concerns constant-versus-variable comparisons and is not used
to claim this variable-versus-variable residual is unreachable.

## Reproduction and limits

With the pinned IDO host environment:

```sh
python3 cloud/work/encoded_string_mapping/verify.py \
  --build-dir build/encoded_string_mapping/replay \
  --output build/encoded_string_mapping/replay.json
python3 -m pytest -q tests/conveyor/test_encoded_string_mapping_reconstruction.py
```

Four focused tests pass. Host C89 compilation uses warnings as errors. Fifteen
directed and 1,000 deterministic generated cases cover both encodings, first
matching duplicates, unmapped symbols, index-zero output, empty input, output
limits including 255, byte normalization, padding, and the zero-limit quirk.
These host tests compare against an independent dictionary-based semantic model;
they are not MIPS execution or ROM acceptance tests.

`verify.py` separately recompiles the real source, checks ELF symbol extent,
checks every relocated target word, identifies the sole symmetric residual,
and rejects unknown or unverified relocation state. It does not weaken or change
any acceptance tool. Protected assembly, symbols, targets, locks, accepted source,
compiler settings, and existing research remain unchanged. No remote builder was
used. Only source, tests, metadata, and this research note are included; no ROM
bytes, raw assembly dumps, or generated binary artifacts are published.
