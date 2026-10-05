# N64 HUD bar callback: complete strict match

`func_800EF62C`: **0x800EF62C–0x800EF8F4, 712 bytes / 178 words**.
Base `cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5`.
Status: strict candidate, pending independent review and integration.
Accepted-byte gain claimed: **zero**.

## Provenance and the new source reason

This is an improved **N64-specific decompilation**, not a pasted complete arcade
callback. Archived A70 reconstructed the complete body and stopped nine temporary
register words away. The newly accepted RenameBlit and the SelectBlit result
made the authentic Blit contracts worth inspecting again.

Shared donor source at pinned arcade revision
`845329d7b36f5a384c5625ed9a0aef584ab46139`:

- [game/hud.c Hidden, lines 1270–1280](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/hud.c#L1270-L1280): compare requested visibility with Hide, write changed Hide, call UpdateBlit, return Hide.
  File SHA-256 `9b4b0507db6d83eb25ec2066bd7fe9eb55ca1b04bf2aa69bcdc7bc79be7bc7a0`.
- [LIB/blit.h Blit](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/blit.h): semantic field names and animation callbacks; N64 offsets differ.
- [LIB/blit.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/blit.c): InitBlit, RenameBlit, UpdateBlit, SelectBlit contracts.
  File SHA-256 `34652da79c592dfd0c77e93ba717bd0d3e52e9ae869c01d69b01d650da93a080`.

The matching callback, packed player/kind selection, split-screen position tables,
car strides/fields, and numerical scale are recovered from the complete N64 body.
No full-function arcade ancestor is claimed. The donor source stays outside this
submission.

Fresh checks found no accepted lock, current wave-5 assignment, or visible open
PR for this function. Open PR #98 concerned another target and was untouched.
The second originally scouted target, func_80108DA8, was transferred to a separate
worker before any source edits. The claim file records that handoff. This cannot
rule out unpublished concurrent work.

## Result and bounded experiment

Final source: `cloud/matches/func_800EF62C.c`.
Flags: `-g0 -O3 -mips2 -G 0 -non_shared`, plus the scorer's normal
`-Wab,-r4300_mul` handling. O2 also produces MATCH.

1. A70 reproduces **9/178 NONMATCH**, exact extent.
2. Replace its fake extern float symbols with owned `1.35f` and `0.0001f`:
   **9/178**. Both literal values are directly authenticated at 0x801245A4.
3. Express both visibility sequences through the genuine arcade Hidden body:
   **9/178**, showing equivalent inline behavior rather than creating a new ABI.
4. Write Right before Bot: **5/178**. Workbench identifies allocation-only
   residual, exact frame and extent; raw resolved target calls have no relocation
   records, so its symbol warning is not an independent match verdict.
5. The natural independent crop-field order **Right, Top, Bot** gives MATCH.
   The other bounded order controls give 13 or 42 differences. No numeric or
   semantic behavior changes; the source traversal changes IDO's temporary cycle.

The final source expands the two genuine Hidden sequences directly. A static
Hidden control emits an unused eight-byte local stub before the identical target;
that control is not submitted as a claimed extra function. The final object has
one defined function, exactly 712 bytes and no pre-function executable bytes.
No pressure arrays, unused locals, dead reads, compiler changes, helper stand-ins,
keepers, or production override changes are used.

## Recovered contract

AnimID bits 4–7 choose a player and bits 0–3 choose crop kind. An unavailable
player is hidden; sufficiently out-of-range callbacks are disabled. Active players
are hidden by a global flag, the Car2056 byte at 10, or Object952 signed byte 239
selected through Car2056 signed halfword 1990. Changed visibility calls UpdateBlit.
The car value used for the bar is **signed halfword 2000**, not 1990.

The callback optionally renames the split-screen texture, computes X/Y from a
four-player row indexed by count minus one, and uses signed-halfword dimensions.
Kind zero crops to half-height minus one. Kind one uses rounded single-precision
`car_value * 1.35f * 0.0001f`, absolute value, clamp to one, then width-minus-one
scaling. Top is half-height with division toward zero; Bot is height minus one.
The final alpha is 254 and the callback returns one.

Blit is an observed prefix. Car2056/Object952 carry native array strides. The
opaque record spans establish offsets; they are not local frame-shaping storage.
The callback uses one standard O32 pointer argument and a 32-byte frame. No hidden
incoming register or unsaved callee-saved use is required.

## Verification

`verify.py` proves the full ELF symbol, all 178 relocated words, all 35 relocation
records (including five real calls), and the two owned float literals. Independent
GNU ld reproduces the entire function at its native address and both literals at
0x801245A4. The eight trailing .text bytes and eight trailing .rodata bytes are
separately checked as zero alignment outside the claimed function/literals.

A genuine O3 group includes five unchanged accepted context bodies:
RenameBlit, InitBlit, texture lookup, UpdateBlit, and Input_InitPadHandlers.
All six complete bodies remain exact. No protected root/keep recipes are edited.
This is narrower than the full-game shadow unit, which was not run here.

`verify_semantics.py` checks **4,363 deterministic cases** by four routes:
protected native MIPS, independently GNU-linked candidate, arithmetic oracle,
and the actual unchanged C compiled for the host with UBSan. All 178 instruction
offsets execute. Tests cover inactive players, visibility changes, texture choice,
signed values, saturation, both crop kinds, truncated coordinates, and halfword
boundary dimensions. The interpreter checks stack restoration, preserved integer
and FP registers, and write confinement. Calls use synthetic effects and destroy
caller-save registers. This checks the caller under those contracts, not gameplay
or the actual callees. Table domains are count 1–4, indexed players 0–3; excluded
players up to 15 are covered without indexing them.

Six wrong-contract controls are rejected: wrong coefficient, omitted clamp,
wrong packed slot, arithmetic-right-shift half-height, wrong mode predicate,
and omitted final UpdateBlit. Discriminating cases are in `verification.json`.

## Reproduce

```sh
python3 tools/cloud/score.py fn cloud/matches/func_800EF62C.c func_800EF62C \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 cloud/work/frontier/dot_blit_family_20261005/verify.py
python3 -m pytest tests/cloud/test_blit_bar_contract.py -q
```

No native bytes, assembly dumps, ROM files, objects, compiler binaries, credentials,
or unrelated private data are included. Full image, compression, ROM SHA-1,
merging and final coverage acceptance remain with the independent checker.
