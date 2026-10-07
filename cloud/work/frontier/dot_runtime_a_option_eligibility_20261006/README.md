# Runtime-A option eligibility: 88-byte local candidate

Frozen base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Stock IDO 5.3 group pipeline, actual
`-g0 -O3 -mips2 -G 0 -non_shared` flags.

`func_8038F744`, `[0x8038F744,0x8038F79C)`: **88 bytes, 22/22 words MATCH**.
The fresh comparison reports zero differing words, unresolved symbols,
unverified relocations, relocation errors and extra words. The helper owns no
literal data. This is a local candidate, not accepted cartridge coverage.

The helper reads one signed-byte flag by a real index, gates option 7 on the
native input bit and overrides option 6 with the native status byte. The old
standalone seed differed at 18/22 words. A natural word-sized accumulator and
logical `index == 6` condition reduce that to two words: final t0 versus native
t6. Complete actual DDA8/E1C0 callers from #225 naturally supply the temporary
reservation and close those two words. No dummy formal, forced register,
pressure routine, fake caller, volatile or asm was added.

F744 is therefore group-only. Its high caller declaration is aligned with the
real signed-byte-return definition; both complete high bodies retain their
previous scores: DDA8 24/262 and E1C0 14/215, with no extra, unresolved,
unverified or error fields using the authenticated image. Only F744 is claimed;
the earlier high-A bodies are context with no duplicate byte credit.

## Context limits and reproduction

The #225 context assumptions remain: reconstructed 60-byte slot views, word-
aligned by-value color aggregates, 48-byte text buffers, valid language tables
and projection results. Original names, exact buffer capacities, full object
types and whole-function ancestry remain unproven. No behavioral acceptance
claim is attached to the unclaimed callers.

```
python cloud/work/frontier/dot_runtime_a_option_eligibility_20261006/reproduce.py \
  --repo . --reference-root .
```

The short helper authenticates the fixed existing asset and image A in memory
using the pinned SHA-256 identities and loader pointer, then binds the ordinary
scorer's owned-data checks. No native bytes, raw image or binaries are included.
This result was freshly reproduced after workspace restoration using surviving
stock IDO and GNU 2.44 diagnostic utilities; no linker/ROM acceptance was implied.

This expands #225's genuine context. Do not install both groups together or
count the high bodies twice. Matching compilation/scoring only: no proof packet,
acceptance suite, CI wait, production edit, lock or promotion.
