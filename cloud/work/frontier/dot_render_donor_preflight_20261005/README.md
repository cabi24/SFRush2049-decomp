# Render donor preflight: three bounded negative results

Base: `e24b47d89a0c8ffade1e4c75ad76b9d390a1c232`, 2026-10-05.
Status: **NONMATCH research only. No matching, accepted-byte or ROM-coverage gain.**

## Useful result and stopping points

Three concrete source hypotheses were checked against existing failed work.
None produces a matching candidate. The packet preserves eight fixed ordinary
O3 builds, source/native/toolchain hashes and complete ELF/GNU comparisons. It
contains no new production source, target bytes, assembly dump or binary.

| Hypothesis | Prior source, freshly built O3 | New fixed control | Conclusion |
|---|---:|---:|---|
| `sound_stop`, N64 RemoveBlit, 160 bytes | w5a: 12/40 | authentic tail recursion: 32/40; recursion with real renderer-release helper: 32/40; loop with that helper: 14/40 | Neither source boundary resolves the post-search allocation problem. |
| `audio_channel_reset`, player visibility callback, 164 bytes | typed real-context source: 15/41 | authentic Hidden helper: 15/41 | Hidden does not change the three loop allocation webs. |
| `func_800A7480`, viewport color setter, 136 bytes | B52 O3: 144-byte ELF, 36 positions differ including two excess words | unsigned red/green byte contract: exact 136-byte ELF, 33/34 | Removes signed-left-shift undefined behavior, but argument homing/scheduling remains unexplained. |

Counts are differing instruction positions, not semantic similarity percentages.
The historical B52 O2 result was 29/34 with an excess instruction and an unpaired
relocation. This packet's fresh O3 baseline is distinct and does not supersede
that better historical count. Its full-body GNU link resolves every relocation
and exposes the complete 144-byte extent; the target-sized canonical comparison
still honestly reports 34/34 plus excess/error information.

No follow-on spelling, local-order, formatting, qualifier, helper-depth, keeper,
or allocator sweep is justified. Reopen RemoveBlit only with original interface
or caller/TU evidence that predicts its register web. Reopen the visibility
callback only with independent evidence for the count/cursor/car-index allocation
order. Reopen the color setter only with evidence for its original parameter
homes or a genuine shared-source boundary.

## Why these controls were new

The primary reference is pinned
[historicalsource/rushtherock 845329d7](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139).

- [LIB/blit.c RemoveBlit](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/blit.c#L32-L54)
  searches the live pointer list, releases its renderer entry, swaps the last
  pointer into the removed position, then recursively removes the child. The
  N64 body omits the arcade Errorf check. The archived w5a packet used an iterative
  'voice' interpretation and approximately 130 controls; no archived self-call
  was found in the searched current-source corpus. Absence is not a proof of
  complete historical novelty. The new controls use the actual accepted 64-byte
  Blit layout from NewMultiBlit and the actual renderer release operation at
  `func_800A79DC`. Its six native words remain independently exact.
- [game/hud.c Hidden](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/hud.c#L1270-L1279)
  is a genuine visibility boundary: compare, write, update, reload the signed
  visibility byte. The earlier visibility packet explicitly lacked arcade
  source. This control replaces only its final expanded visibility operation.
  Both accepted setter definitions remain a literal unchanged prefix and
  independently match all 192 and 64 bytes. No original whole-callback arcade
  identity is claimed.
- Accepted `sfx_position_3d` uses unsigned RGB bytes with the standard RGBA5551
  expression. The real A7480 caller `display_list_flush` loads its red, green and
  blue with unsigned byte loads before the seven-argument call. This supports
  testing unsigned color semantics and avoids B52's undefined signed shifts.
  It does **not** establish the original A7480 C prototype or shared struct
  identity. The record extent is 72 bytes; only its observed final eight bytes
  are named by this control. Existing projection code independently establishes
  that stride. No signed-to-unsigned declaration is installed in shared context.

The existing w5a baseline contains an empty conditional. It is replayed only as
historical comparison; this packet does not endorse or import that construct into
a new candidate. The new controls contain no fake guards, dead reads, unused
formal arguments, volatile additions, padding locals or substitute callees.

## Verification and scope

For each generated source, `verify.py` compiles with stock pinned IDO and the
unchanged scorer, reads exact function symbols, resolves the **complete ELF
extent**, and independently links with GNU MIPS ld. Every body byte agrees
between the complete canonical relocation and GNU route. All original target
words and symbol addresses are read through protected-manifest checks. No own
literals, tables, data or BSS are present. Alignment and preceding helper bodies
are outside each function's extent and receive no credit. Known context symbols
are linked to native addresses separately for each body comparison; this is not
a contiguous whole-game image or shared-unit placement proof.

The eight O3 builds are the retained falsification set. Exploratory ordinary O2
checks of direct recursion and unsigned color did not match; one O1 color check
and one word-argument O1 control were also negative. They are not claimed as
reproduced by this packet or as an exhaustive compiler search.

Six focused metadata/source-binding tests pass, including a changed-source
refusal and the complete-versus-target-sized extent distinction. Two consecutive
fresh compiler runs reproduce the stored receipt. These are machine-code and
source experiments, **not host/native semantic differential proofs**. Invalid
pointers, corrupted lists, aliasing, recursion depth, callback mutation and
full gameplay are untested. No whole-program shadow, image, compression, ROM,
full-suite or remote CI result is claimed.

## Reproduce

```sh
python3 cloud/work/frontier/dot_render_donor_preflight_20261005/verify.py
python3 cloud/work/frontier/dot_render_donor_preflight_20261005/verify.py --compiler
python3 -m pytest -q tests/conveyor/test_dot_render_donor_preflight.py -o addopts=''
```

Default verification checks source and protected-native binding only.
`--compiler` executes all eight builds and both complete relocation routes.
`--record` explicitly rewrites the receipt for a reviewed new snapshot; ordinary
replay never rewrites it. Source files omitted by a sparse checkout are read from
its HEAD. Existing files are read as-is so local changes fail their saved hashes.

Fresh eligibility checks found all three targets unlocked on the stated base,
with no target overlap among visible open PRs #110–115 or the parent's active
lanes. The exact intervals were coordinated before probes. This cannot exclude
unpublished work elsewhere. Merging, acceptance and publication remain with the
independent checker and parent task.
