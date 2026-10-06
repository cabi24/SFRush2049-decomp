# Image A select-screen stat-bar callback

Candidate: `cloud/matches/ovl_a/func_803A4134.c`, ordinary IDO 5.3 O2,
`-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The full native interval is `[0x803A4134,0x803A4340)`: 524 bytes / 131 words.
This is source/object evidence only; newly accepted bytes and ROM coverage are zero.

## Native contract and provenance

The protected image-A manifest/extents mark the entry by data reference and
prologue. It is an ordinary one-pointer O32 callback with a 40-byte frame,
return address save, and genuine argument/selector spills around external calls.
No hidden input register, unused local, dummy read, volatile pressure, assembly,
stand-in callee, or fabricated whole-program context is used.

Arcade ancestry is `game/select.c:2383 AnimateBar` from
[historicalsource/rushtherock](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/select.c#L2383).
It supports the select-screen bar-animation purpose, but is not an exact N64
donor. The N64 callback decodes segment bits 0..3, bar bits 4..7, player bits
8..11, and once-only initialization bit 31. The N64 geometry, visibility,
player guards, high-bit handling and float-product lookup differ substantially.

`func_800EF5B0` is the actual RenameBlit helper. `input_new_data_wrapper`
(0x80094F88) is Hidden: it compares the requested state with signed Hide at +26,
stores it when changed, calls UpdateBlit, and reloads the signed result.
`Input_ApplyPadConfig` (0x80094EC8) is the actual UpdateBlit.
All three complete native helper bodies are bound by address, extent and hash.

The complete native `sound_control`/NewMultiBlit body (0x800B37E8, 468 bytes)
loads descriptor callback +28, stores that function at BLIT +40 in its indirect
call delay slot, and passes the BLIT pointer as its one argument. It stores the
masked descriptor AnimID at BLIT +44. This proves the two fields independently
of historical inconsistent names. `state_update_global` further binds the
initializer prefix. The declared 48-byte record is only an accessed prefix;
no claim is made that 48 bytes is the complete BLIT size.

The distinct complete 404-byte producer `func_8039D300` is hash-bound as
context, not a claimed match. It fills four products per selected player from
five selected float vectors. Its selected row domain, the actual callback
descriptor table and texture/vector asset data are not present in this packet.
The declaration `D_803BA190[][4]` follows its four-component producer and the
consumer's 16-byte player stride / 4-byte component stride. We do not prove
that every decoded nibble is a valid table index or every original asset value
falls in the portable conversion domain.

Initialization optionally renames for count >=3/player <2, then ORs the high bit
into the reloaded AnimID. Only player < count ==1 and mode !=2 proceeds to
geometry; otherwise it hides, clears AnimFunc, and returns one. Geometry divides
signed Height by eight toward zero, then stores signed halfword coordinates.
The per-player signed flag hides only when exactly 1. Segment 1 evaluates the
binary32 product/offset, narrows Right, and applies minimum 2; other segments
store Width-1. A final UpdateBlit follows visible work.

## Complete proof

`verify.py` freshly compiles the source and proves all 131 words with the
unchanged canonical scorer, including all 16 actual relocations. The ELF symbol
is exactly 524 bytes; .text is 528 bytes and its four alignment bytes are zero.
There are no owned data bytes, unresolved symbols, unverified relocations or
extra nonzero words. An independent ELF reader and GNU linker resolve every
anchor at its native address and reproduce the full body plus alignment.
A 32-bit C syntax check asserts the eleven accessed BLIT offsets.

`semantics.py` runs 5,105 deterministic cases through four routes: protected
native MIPS, independently GNU-linked candidate MIPS, an arithmetic oracle,
and the unchanged source compiled by GCC with undefined/bounds/float-cast-overflow
sanitizers. The two MIPS runs total 10,224 including seven native-only wide
conversion probes. Hooks destroy every caller-save integer/FP register and
HI/LO; saved registers, stack bounds, memory read bounds, and store confinement
are checked. Helpers have explicitly synthetic side effects, including changing
count/geometry/AnimID during RenameBlit and changing Hide after its nested
UpdateBlit. These stress reload versus captured-value behavior; they are not
claims about the actual renderer/texture helper internals.

130 of the 131 instruction offsets execute. The duplicate Width load at offset
0x1E4 is unreachable in the bounded function CFG: the branch-likely non-segment-1
path executes the equivalent load in its delay slot and jumps to 0x1E8; both
segment-1 clamp arms branch to 0x1F0. It remains covered by full static equality.
Each table cell has a distinct binary32 value (except zero inputs) and each
visibility slot differs, testing both player and component address calculations.
Every conditional branch has both outcomes exercised. Nine wrong-contract
source mutations are rejected by both host semantics and strict native scoring:
wrong signed division, player nibble, visibility predicate, clamp minimum,
missing callback disable, omitted final update, wrong stat component/player,
and wrong visibility player.

## Float conversion and input limits

Native execution performs binary32 multiply and subtract, `trunc.w.s` to signed
32 bits, stores the low halfword, then compares that reloaded signed halfword
against two. Host/source behavior is claimed only for finite results whose
truncated value fits signed16. In particular this source's direct float-to-s16
assignment is not portable for wider results. Seven native/GNU-only probes
establish the observed halfword wrapping without claiming defined C behavior.
NaN, infinities, signed32 overflow, alternative FPU exception/rounding modes,
concurrency, resource validity, descriptor reachability and gameplay are unproved.
Table-reading cases use player/bar 0..3. Rejected uninitialized player nibbles
4..15 and non-table segment/bar cases are separately covered.

## Reproduce

With the documented IDO/binutils environment:

```
python3 cloud/work/frontier/dot_runtime_a_stat_bar_20261006/verify.py
python3 cloud/work/frontier/dot_runtime_a_stat_bar_20261006/verify.py --check
python3 -m pytest tests/cloud/test_runtime_a_stat_bar_contract.py -q
```

A minimal checkout can pass `--repo /path/to/complete/trusted/repo` and
`--tools-repo /path/to/complete/trusted/repo`. These are read-only proof inputs.
The receipt excludes path-sensitive object hashes; local provenance stays in
ignored build output. No ROM bytes, raw disassembly, binaries, credentials or
unrelated private data are published. Image splice/compression/full-ROM gates,
merging and final coverage acceptance remain with the independent checker.
