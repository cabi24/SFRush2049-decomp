# Authentic Hidden helper closes the static-blit callback

Status: **strict MATCH, independent review pending**. Only
`state_update_global`, `[0x8010B560, 0x8010B5D0)`, **112 bytes / 28 words**, is
claimed. Accepted-byte and ROM-coverage gain: **zero**. Base is
`cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5`.

## New source contract

The N64 callback updates a Blit's visibility from the nonzero state of
`D_80149D98`. When visible, it replaces the image pointer, updates the Blit,
and then disables its animation callback. It always returns one.

The missing source boundary is the authentic arcade
[`Hidden`](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/hud.c#L1270-L1279)
helper. It sets signed `Hide` only on a difference, invokes `UpdateBlit`, and
returns the **reloaded** `Hide` byte. The same helper appears inside the native
N64 callback. `Input_ApplyPadConfig` is the accepted N64 UpdateBlit adapter.
The candidate preserves Hidden's original external linkage and ordinary C.
The callback itself is N64-specific; no full arcade donor is claimed for it.

Native field evidence is concrete: the signed byte at +26 is visibility,
word +4 is the image reference, and word +40 is the animation-function pointer.
The Blit names and relationships follow
[`LIB/blit.h`](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/blit.h),
with the observed N64 layout. Only this prefix is needed by the callback.
The pointed-to image data are opaque here and never dereferenced.

The historical direct source freshly reproduces **3/28** differing words.
The already archived accepted-callee-context probe also stopped at 3/28;
it never introduced this genuine Hidden boundary. Ordinary helper composition
reproduces the entire callback at **O3**. O2 leaves the helper call and remains
nonmatching. No old register, qualifier, or formatting sweep was repeated.

There are no volatile declarations, artificial reads, unused padding locals,
dead conditions, invented helpers, stand-ins, extra formals, or compiler/scorer
changes. The unchanged existing exported roots are retained for the context
regression. No production keep list, shared header, lock or accepted source is
modified.

## Complete machine-code proof

`verify.py` checks all of the following against the manifest-verified target:

- Exact **112-byte ELF function symbol**, all 28 fully relocated words, strict
  canonical `MATCH`, zero excess words or unresolved/unverified references.
- Independent GNU MIPS linker placement at `0x8010B560`; all **six relocations**
  agree, including both real calls. No owned data, literals or tables occur.
- The standalone global helper's preceding **60 bytes** are excluded. Four
  trailing zero alignment bytes are checked and excluded from the body.
- A genuine three-body O3 context contains the new callback and the unchanged
  accepted `Input_ApplyPadConfig` / `Input_InitPadHandlers`. All three canonical
  results are strict `MATCH`; all three ELF symbol-bounded bodies are separately
  compared without masks. This is not a full-game shadow-unit test.

The submitted source keeps the donor's original external linkage. Its helper
has its own ELF symbol, and all context results pass without weakening any
extent gate. The helper body itself is not claimed, assigned a guessed native
address, or added to any production keep list.

## Behavior and counterexamples

The unchanged candidate C, an independent state-transition oracle, the native
body and independently GNU-linked body agree on **15,744 deterministic cases**
(**31,488 native runs**). The host bridge is compiled with UBSan and immediate
failure recovery disabled. Every one of the native body's 28 instruction offsets
executes. Cases cover every signed Hide value, zero/positive/negative global
states including both signed limits, and adversarial changes during each call.

Audited O32 mocks clobber caller-save registers and can change Hide, image,
animation callback and the global. Ordered call snapshots verify the
post-call Hide reread, image assignment before the visible update, callback
clear after that update, and exactly one observation of the global. Saved
registers, stack canaries and all unrelated object bytes remain unchanged.

Five source mutations are rejected, with saved witnesses: returning the old
Hide request, unsigned Hide, clearing the animation callback too early,
skipping the visible update, and treating only positive globals as true.
Tests additionally reject an unknown native instruction and detect a removed
native callback call.

The host uses its own pointer-sized Blit layout. It validates unchanged C
semantics; ELF/native checks independently establish actual O32 offsets. The
instruction interpreter and callback mocks do not emulate the complete
UpdateBlit implementation, asynchronous updates, or gameplay.

## Selection and verification

`claim.json` records the fresh master, available Claude branch, open drafts,
wave-five results, prior source and donor identity checked before editing. No
published claim overlapped this range. These checks cannot disclose unpublished
work. The other active callback and radar targets were left untouched.

Reproduction, from the repository root with pinned IDO and GNU MIPS binutils:

```sh
python3 tools/cloud/score.py fn cloud/matches/state_update_global.c state_update_global \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 cloud/work/frontier/dot_hidden_callback_20261005/verify.py
python3 -m pytest tests/cloud/test_dot_hidden_callback.py -q -o addopts=''
```

The verifier normally writes no tracked file. `--write` deliberately refreshes
`verification.json` for review. The saved receipt binds the candidate, proof
scripts, host bridge, context, compiler and target manifest by hashes. No native
instruction stream, ROM bytes, binary or assembly dump is stored in the packet.

Local focused verification: **8 tests passed, zero skips**. All **402 existing
static locks** pass their guard. Final changed-submission and protected-path
checks pass for the committed deliverable; `checks.json` records them.

The broad local Cloud/Conveyor suite ran to completion: **2,229 passed, 64
skipped, five failures**. All five failures reproduce in a fresh untouched
`cc4d5fdd` worktree: one missing-permuter fixture error, three unavailable-m2c
source/macro failures, and the smoke test requiring a private Conveyor token.
No unrelated code or CI changes were made. The first collection attempt lacked
the permuter import; adding the existing tool to PYTHONPATH allowed collection
and the complete run above. The suite is not reported as a clean pass.

No splice, source-built linked image, compression identity or full-ROM SHA-1
check was run. The independent checker owns source admission, merging and
those final integration gates.
