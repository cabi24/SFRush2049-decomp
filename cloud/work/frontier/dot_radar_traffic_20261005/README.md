# Radar traffic callback: complete source, bounded proof, NONMATCH

Target: `func_80108F40`, `0x80108F40–0x80109468`, 1,320 bytes / 330 words.
Base: `cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5`.
Status: **NONMATCH research. No native-byte or accepted-coverage gain.**

## Useful result

This is the N64 per-view radar traffic callback, a descendant of arcade
`AnimateTraffic`, with `Hidden` inlined at its actual call sites. It is not the
historical `skill_rating_update` / `matchmaking` mid-function labels. The entire
body is reconstructed in readable C, including hidden-state reloads, remote-car
eligibility, relative position, clipping, mirrored horizontal coordinates,
per-view origin, atlas selection, centering, and the final display update.

The clean stock-IDO O3 candidate is **307/330 differing positions in the canonical
scorer**, which still reports four unverified own-section references and one
own-data inference error. The independent GNU-linked comparison resolves every
relocation and measures **308/330 differing positions**. Both are NONMATCHs.
The ELF function is **1,316 bytes**, one instruction shorter than the native
body, and its stack frame is **152 rather than 168 bytes**. A shifted instruction
changes later positional comparisons; neither score is a semantic percentage.

The candidate passes **1,748 cases** against four routes: protected native
instructions, independently linked candidate instructions, an arithmetic oracle,
and the real candidate C compiled for the host with UBSan. The corpus executes
328/330 native instruction offsets and detects five deliberately wrong
contracts. This establishes the documented bounded behavior, not a match.

## Provenance and eligibility

Authentic donor commit: `845329d7b36f5a384c5625ed9a0aef584ab46139` in
[historicalsource/rushtherock](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139).
Relevant sources:

- [game/hud.c AnimateTraffic](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/hud.c#L797-L868)
- [game/hud.c Hidden](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/hud.c#L1270-L1278)
- [LIB/blit.h](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/blit.h)
- [game/vecmath.c vecsub](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/vecmath.c#L74-L79), with the conditional macro alternative in `game/vecmath.h`

Donor file SHA-256s:

- `game/hud.c`: `9b4b0507db6d83eb25ec2066bd7fe9eb55ca1b04bf2aa69bcdc7bc79be7bc7a0`
- `LIB/blit.h`: `91516a3935024254702dbb1b66d3f87258cfbe3ca878f22ddc5e9cd10e4a1c6a`
- `game/vecmath.c`: `7909033d4e95a86d94dc14b96539b272dbd9e8fd7f7b7e67f6b8cbbc0ca2ee31`
- `game/vecmath.h`: `76526aaa1894d9cef992ea07fa524cf7348e02cbede5a646a66f04d6e317cf3f`

The donor files are not republished. Fresh master, current blob locks, wave-5
results, the prior registered-head seed, and the coordinating worker's active
claims were checked before the claim and source edit. EF62C, 108DA8, FE5B0 and
87110 were excluded. This is no guarantee about unpublished work. The earlier
registered-head seed was already complete decompiler output with a historical
323/330 O2 result; this packet does not claim first discovery of the function.

## Native contracts and N64 differences

The observed Blit prefix has X/Y at 14/16, signed dimensions at 20/22, signed
Hide at 26, callback at 40, and unsigned AnimID at 44. The record is not declared
complete and is never indexed by its partial size. The actual car record stride
is 952 bytes: position at 8, matrix at 44, per-view mode byte at 239. The model
stride is 2,056 bytes: atlas index at 1,990, live halfword at 1,992, and kind byte
at 1,996. Opaque arrays express those observed record gaps, not stack filler.

AnimID bits 8–11 identify the view and bits 0–3 the displayed car. Disabled
views and inactive model records clear the callback and return the reloaded
signed Hide value. Other hidden paths return 1. The local car bypasses the
remote solidity query. Position is transformed relative to the viewed car;
N64 omits the arcade direction-angle selection and uses eight horizontal icons.
The two scale literals equal `1.0f / 480.0f`, and are independently checked at
`0x801248CC..0x801248D4`. Width/height clipping snapshots precede callbacks;
SelectBlit arguments reread the dimensions. The per-view origin is added before
centering with truncating signed division by two.

All four external calls have ordinary consumed O32 arguments. Synthetic callees
clobber caller-save GPRs/FPRs, and the interpreter checks callee-save GPR/FPRs,
stack restoration, untouched globals, and owner writes confined to X/Y, Hide,
and callback. The broader runtime validity of arbitrary view/model identifiers
is not asserted.

## Compiler controls and stopping point

`verify.py` reproduces three bounded controls:

- Stock O2: 323/330 differing, plus unresolved internal Hidden calls. Not a match.
- Authentic `vecsub` function instead of its macro-like expansion: 305/330
  canonical differing positions, still 1,316 bytes. Its consumed pointer
  parameters restore the 168-byte frame, but the local homes remain wrong.
  This is a useful source-context lead, not acceptance or a retained filler.
- A real five-function O3 context copies four accepted sources read-only.
  `func_800CF604`, `func_800A61B0`, `Input_ApplyPadConfig`, and
  `Input_InitPadHandlers` each retain exact complete ELF function-symbol bodies.
  The target remains unchanged and nonmatching.

The full context does **not** earn a blanket canonical pass. IDO leaves an
8-byte out-of-line residue from the genuine inlined Hidden helper between
function symbols; the scorer's next-symbol extent check charges its nonzero
instruction to `Input_ApplyPadConfig`. The latter's actual 192-byte symbol is
fully exact. The receipt preserves both facts; no slicing or relaxed acceptance
is used to call the group matched. Reordering the separate units merely changes
which neighboring function receives that extent warning.

The linked workbench diagnostic gives 329 versus 330 instructions, a 16-byte
frame deficit, and a mixture of register and structural differences. Its LCS
alignment is explicitly marked noncomparable because it inserts seven gaps.
The first additional native address materialization is the signed-halfword
count at `D_801543CA`; that early instruction and temp-register difference
propagate through later code. Scalar/address/array declaration controls did not
recover it. The plain-inline spelling also did not change the result. Replacing
floating eighth-width arithmetic with integer division is worse and contradicts
the observed native computation, so it was rejected.

No volatile shaping, dummy calls, dead conditions, unused donor locals, padding
locals, assembler edits, optimizer patches, stand-ins, or protected-file changes
were introduced. The next credible step is original N64 declaration/helper/TU
context or a traced explanation of the missing address materialization and
local homes. The authentic donor has obsolete direction locals; merely adding
unused declarations to occupy the missing frame is deliberately not the result.

## Verification limits

The oracle and test callees are synthetic. The vector transform implements the
accepted callee's arithmetic with an identity-matrix corpus; this does not test
all orientations or real callee side effects. Hidden callbacks can overwrite
Hide on their first update, which checks reload semantics. Test memory includes
16 synthetic car/model records; it does not prove 16 are valid in gameplay.
All generated float-to-integer values are finite and representable. Null Blits,
invalid backing storage, NaNs, overflow, and arbitrary callback mutation of
other fields are outside the host proof. The two unexecuted native offsets are
listed in the receipt. A pass is not universal equivalence.

The canonical scorer's own-data error is not suppressed: shifted instructions
make its position-based inference inspect unrelated addresses. The GNU link uses
the separately verified literal placement and resolves all normal relocations;
its complete-symbol comparison still fails by 308 positions. There are no
nonzero excess words after the candidate function, and zero alignment outside
its symbol is checked. Source/packet/compiler/context hashes bind the evidence.

Nine focused pytest tests pass. The public evidence serializer redacts only the
compared byte payloads in own-data diagnostics; their failure verdicts, addresses,
offsets and compared lengths remain visible. The canonical scorer is unchanged. No game shadow unit, source-built image,
compression, ROM hash, or integration gate was run here. There are no production
source, lock, keep-list, or coverage edits. Merging and integration remain with
the independent checker.

## Reproduce

```sh
python3 cloud/work/frontier/dot_radar_traffic_20261005/verify.py
python3 -m pytest tests/cloud/test_radar_traffic_contract.py -q
```

Use the repository's stock IDO 5.3 toolchain and GNU MIPS binutils. The compiler
flags are `-g0 -O3 -mips2 -G 0 -non_shared`; the scorer supplies its normal
`-Wab,-r4300_mul` handling. Generated objects, native words and disassembly remain
in temporary local scratch, outside this source/evidence submission.
