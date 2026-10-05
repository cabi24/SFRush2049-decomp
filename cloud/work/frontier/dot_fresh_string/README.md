# Encoded string-length match: func_800BE744

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b` (2026-10-05).
Target: the complete authenticated game-image function at
`0x800BE744..0x800BE7BC`, 120 bytes / 30 words. It was absent from the current
lock, provisional set, and `cloud/matches/` at claim time. This is game-image
work under the frontier plan, not a boot-tail Packet 6 / T050 target.

## Result

`cloud/matches/func_800BE744.c` is a strict full-word MATCH with IDO 5.3 at both
`-g0 -O3 -mips2 -G 0 -non_shared` and `-g0 -O2 -mips2 -G 0 -non_shared`.
The unchanged scorer adds `-Wab,-r4300_mul`. Both builds have a 120-byte ELF
function symbol, 30 equal unmasked words, and zero unresolved/unverified
references, errors or nonzero extra words. A kept single-source O3 group also
passes. That group is a compiler control, not the whole-program shadow gate.

The routine counts characters in the game's two string encodings. An initial
0xFF selects two-byte characters terminated by a zero pair; otherwise the first
zero byte terminates the ordinary string. A single zero within a two-byte
character is not a terminator. The actual ABI is one byte-pointer parameter,
integer return, and no calls or hidden incoming registers. The two actual game
callers are `race_position_update` and `func_80105480`; both pass the pointer in
a0 and consume v0. Neither caller is claimed or modified.

The game text layer is the likely subsystem. Arcade ancestry remains
unestablished: the referenced arcade checkout is unavailable in this cloud
checkout. The source is reconstructed from the authenticated complete target,
not inferred from its historical name.

## What changed

Prior sources were searched first: `tiny_A24/func_800BE744.c`, the five
`near_miss_B32` cursor/length variants, and
`bigfish/libhunt/nearmiss/func_800BE744_strlen2.c`. Fresh O2/O3 baselines reproduce
27/30 differences plus three extra words for the prior cursor form.

The useful new hypothesis was a single cursor initialized from the input before
the encoding branch. Incrementing that cursor inside the wide branch prevents
IDO's extra input-address fold/copy. This reaches 3/30 with the exact extent.
The remaining differences were the narrow loop's one byte-value register, a0
instead of a2. Workbench independently diagnosed an allocation-only residual.
Removing an unnecessary cached first-byte local and reading `*cursor` directly,
with `while (*cursor++) count++` for the ordinary branch, closes all three sites.
No fake formals, pressure variables, padding, data definitions or context were
introduced. `cached_first_nonmatch.c` retains that representative three-word
nonmatch; it is not a match submission.

The bounded pass examined 46 genuine source-form controls plus three prior
baselines. Most of the late type/local spelling controls did not move the
three-word residual. No random permutations, forced registers, compiler patches,
scorer changes or target changes were used.

## Reproduction and tests

From the repository root with the pinned IDO toolchain available:

```
python3 tools/cloud/score.py fn cloud/matches/func_800BE744.c func_800BE744 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
python3 cloud/work/frontier/dot_fresh_string/verify.py --output build/be744-verification.json
```

Exact scorer output:

```
func_800BE744:
  MATCH
```

`verify.py` records only sanitized metadata. It independently checks the ELF
symbol extent and every unmasked relocated word, at O3, O2 and the kept O3
single-source group. A wrong-return source control is rejected. Host C89 with
`-Wall -Wextra -Werror` passes 66,311 encoding cases, including all 65,535 nonzero
two-byte characters, empty strings, embedded 0xFF ordinary characters, and
multicharacter strings. These host tests support the documented encoding only;
they do not replace the byte proof. `verification.json` records hashes and counts.

The target/scorer/symbol/context/locks/flags/accepted source tree are unchanged.
Only source, a representative failed-source control, and sanitized verification
notes are submitted. No raw target instructions, objects, ROM or credentials are
included.

## Acceptance boundary

This is a verified object-match submission. The extracted image and authoritative
layout are unavailable here, so the full whole-program shadow, source-image,
exact compressed-stream and full-ROM hash gates have not run. No cartridge
coverage is claimed. Independent review precedes a draft PR; production
integration and merging remain with the independent checker.
