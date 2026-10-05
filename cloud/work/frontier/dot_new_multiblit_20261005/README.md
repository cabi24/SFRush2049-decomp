# N64 NewMultiBlit: complete strict match

`sound_control`: **[0x800B37E8, 0x800B39BC), 468 bytes / 117 words**.
Base: `cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5`.
Status: strict matching candidate, pending independent review and integration.
Accepted-byte and ROM-coverage credit: **zero**.

## Authentic source and the new result

The historical symbol is misleading: this function is N64 `NewMultiBlit`.
The source is based on [LIB/blit.c NewMultiBlit, lines 325–371](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/blit.c#L325-L371)
and [LIB/blit.h Blit/MULTIBLIT](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/blit.h)
at pinned arcade revision `845329d7b36f5a384c5625ed9a0aef584ab46139`.
`claim.json` records source hashes, the fresh master/accepted-lock/wave/open-PR
checks, and the coordinated exact-range claim. These checks cannot exclude
unpublished work elsewhere. No upstream donor files are redistributed here.

The previous complete A78 source used a descriptor cursor and a separate local
callback. It freshly reproduces **9/117** differing words. The first donor-shaped
candidate, with direct `mblit[i]` and invocation through the assigned
`curblit->AnimFunc`, is strict **MATCH**. Two bounded reversions independently give
4/117 (local callback) and 7/117 (cursor); these are not assumed additive.
The O2 control is 115/117. No spelling sweep was needed.

Final source: `cloud/matches/sound_control.c`.
Flags: `-g0 -O3 -mips2 -G 0 -non_shared`, with the scorer's normal
`-Wab,-r4300_mul` handling. No artificial padding, unused pressure locals, dead
reads/checks, volatile additions, helper stand-ins, compiler/scorer changes,
production root changes or extra callee formals are used.

## Recovered N64 contract

The four inputs are signed-half X/Y origin, a descriptor pointer, and signed-half
count. A nonpositive count returns null. Each 36-byte descriptor supplies a
texture name, signed coordinate/dimension/crop halfwords, depth and alpha words,
a callback/address carrier, and an unsigned animation ID.

The constructor receives name, summed coordinates and the animation ID high bit.
The N64 caller then sets depth, alpha, crops and nonnegative width/height overrides,
and updates the Blit. With a callback it masks the animation ID to 31 bits. A
name of `(char *)-1` selects the N64 special path: the address carrier goes into
Info, Image points at the Blit, and a second update occurs. Otherwise it installs
and calls AnimFunc. A zero callback return removes that new Blit and returns the
already-built prefix without attaching the rejected element. Successful elements
are joined through child; the first becomes the returned root.

The native fourth constructor call value is preserved through an old-style
external declaration. The callee's native body consumes only three values; this
packet does not invent a fourth formal. Non-null successful construction is a
precondition inherited from the original caller, which has no allocation-failure
guard. Names and callbacks have ordinary native pointer representations. The
sentinel branch's function-pointer-to-object-pointer cast is implementation-defined
ISO C, matching IDO's observed 32-bit representation and the tested host platform.

## Complete-function and behavioral proof

`verify.py` checks the real ELF function symbol, exact 468-byte extent, and every
fully relocated word. All four direct-call relocations resolve; the indirect
animation call remains part of the instruction proof. Independent GNU ld places
the complete function at its native address and reproduces all 117 words. Twelve
zero .text alignment bytes lie outside the function. No own data or literals exist.

A genuine seven-body O3 context remains exact: this target, and six unchanged
accepted bodies (ambient_sound_set, RenameBlit, InitBlit, texture lookup,
UpdateBlit, and Input_InitPadHandlers). Source hashes are pinned. NewBlit and
RemoveBlit remain external calls; neither unaccepted callee is replaced by a
stand-in or claimed as a match. This is narrower than the full-game shadow unit.

The protected native body, independently GNU-linked body, normalized independent
oracle, and actual unchanged C compiled for the host with UBSan agree on
**3,198 cases / 6,396 native executions**. Cases cover zero/negative and narrowed
counts, 1–8-element lists, coordinate and field truncation, signed dimensions,
missing callbacks, ordinary/null/sentinel names, animation ID high bits, failure
at every chain position, nonzero positive and negative callback returns, and
constructor/update/animation effects that mutate later descriptor observations.
Ordered call snapshots, complete semantic field state, child topology, untouched
padding, stack restoration, callee-saved registers and write confinement are checked.
External calls poison caller-saved registers. These are explicit synthetic callee
contracts, not a claim of gameplay execution or of those callees' full semantics.

All **116 reachable instruction offsets** execute. Offset +0x158 is the lone
unexecuted word: a duplicate load after an unconditional branch and its delay slot,
with no direct branch entry. The verifier records and checks that structural fact;
it is still included in every exact-byte comparison.

Six compiled wrong-contract controls are rejected: missing ID mask, wrong width
boundary, swapped crop source, returning the last rather than first element,
omitted failure cleanup, and reversed callback-result test.

## Ancillary NewBlit scout

The family search first identified `func_800B3704` as arcade NewBlit. Its archived
complete source lacked the donor. Authentic `u32 AnimID` versus `s32 AnimDTA`
explains two separate native -1 materializations and improves the old 41/57
result to **14/57 at the exact 228-byte extent**. `new_blit_nonmatch.c` preserves
that bounded lead, with no match claim. A real accepted caller context did not
improve it. Although both native callers pass a fourth value, declaring a fourth
unused formal worsened the result to 55/57 with excess output; it was rejected.
No unused formal or artificial read is retained. NewMultiBlit became the sole
matching target after coordination.

## Reproduce and limits

From the repository root with the pinned IDO and GNU MIPS tools available:

```sh
python3 tools/cloud/score.py fn cloud/matches/sound_control.c sound_control \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 cloud/work/frontier/dot_new_multiblit_20261005/verify.py
python3 -m pytest tests/cloud/test_multiblit_contract.py -q
```

The receipt contains hashes, metadata and results only. No ROM bytes, raw assembly,
binary objects, credentials or unrelated private data are included. No protected
files, production sources, locks or build recipes change. Full shadow-unit, image,
compression, ROM SHA-1, merging and final acceptance remain with the independent
checker.
