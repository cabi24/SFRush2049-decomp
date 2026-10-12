# Image-B nearest-target steering: 684 newly strict-matched candidate bytes

Source-first deliverable: `group.c` plus `group.json`. Sole member, keep and claim:
`func_8038DDDC`, complete native interval `[0x8038DDDC, 0x8038E088)`, 684 bytes.
No context body or internal/private ABI is needed. All 171 words strictly match
under stock recognized IDO 5.3. No accepted-image or ROM byte gain is claimed.

## Source change and native homes

The current-master lean source first reproduces 14/171 differences. Every one
is exactly a four-byte displacement in an existing float-vector stack home;
all other words and all four owned literals already agree. The native frame is
136 bytes, unchanged in the baseline and revised body.

Moving the existing `Player *player` declaration before the three unchanged
`float[3]` arrays closes the entire body. Native homes are best at sp+96, delta
at sp+108 and local at sp+120, versus +100/+112/+124 in the baseline. No array
capacity, statement, helper, prototype, expression, literal or control-flow
change is made. No extra declaration, dummy/padding local, assembly, volatile,
compiler change or preprocessor line directive is used. The original declaration
order and variable names remain unknown; this is a layout reconstruction.

The function walks eligible opposing players, transforms their relative
positions, chooses the nearest in its forward cone, and turns the object toward
that player's horizontal and vertical signs. All three vectors are genuine
three-component values. Player and vehicle views retain their existing native
952-byte and 2,056-byte strides. Valid count/index/pointer/finite-coordinate
preconditions and the external helper boundaries are disclosed in the source.
No whole-function arcade donor has been established.

## Canonical score

Run from the repository root:

```
python3 cloud/work/frontier/dot_runtime_b_nearest_steering_20261012/verify.py --score
```

The verifier authenticates current-tree image B in memory and invokes unchanged
`score.py group` without `--claims` or `--allow-unverified`:

```
Members:
func_8038DDDC:
  MATCH
    own .rodata verified at 0x80394DE4..0x80394DF4
```

Standalone compilation is also strict MATCH with that same authenticated
own-data context. The source is packaged as a one-member group because the
current repository has no `asm/us/ovl_b_data` artifact: a bare standalone CLI
without image context reports owned-data uncertainty. No data artifact or ROM
bytes are submitted. The explicit focused test supplies whole-group CI coverage;
ordinary changed-submission discovery does not cover frontier groups.

## Full verification

- Source/group/verifier, current lean baseline, selected native extent/words,
  image and owned-literal inputs are bound. Shared tool, lock, generated assembly
  text and Makefile hashes are deliberately not pinned. No historical git show
  is used. No static ROM TU is rebuilt; the applicable compiler is IDO 5.3.
- Independent GNU linkage resolves all 23 text relocations and all external
  symbol bindings. Complete 684-byte function and next boundary, 688-byte text
  section including four zero alignment bytes, and all 16 owned literal bytes
  are checked. No unresolved symbols, residual relocations or unknown allocated
  linked sections are permitted. ELF machine, byte order and MIPS2 flags check.
- Fresh GNU-linked baseline proves exactly the 14 listed home differences,
  each +4 bytes, and no other native-word or owned-data difference.
- Absent own-data and each individually corrupted literal fail closed.
- Stock as1 `-R` diagnostics on/off produce identical whole standalone objects;
  the standalone control is strict MATCH. No modified compiler provides evidence.
- Normal and optimized-Python full replay pass. The single focused test is within
  the packet protocol time budgets and follows the shared no-tool skip handling.

This proof establishes full code/literal identity in the stated native context.
It does not execute the external transform/rotation helpers, full gameplay,
hardware or the private replacement oracle. Unrestricted pointer/aliasing safety,
image installation/compression and full-ROM SHA-1 remain the independent
coordinator's gates. Keep the PR draft and leave merging to the checker.

## Independent review and publication scope

The renderer lane independently reran normal/optimized replay, canonical score,
and the focused test (1 passed in 0.17s), plus the protected-path guard. It checked
that the only non-comment source difference from the inherited lean candidate
is the same pointer declaration moving before the arrays. Independent stock-IDO
layout assertions also passed: Player 952, Vehicle 2056; player active/blocked/
index at 776/857/859; vehicle disabled at 1600; Object owner/position/uv at 4/32/56.
No blocking finding. The original source was research PR #202, not an existing
strict match; this packet claims 684 newly strict-matched candidate bytes.

The GNU linker is explicitly run with `--hash-style=sysv`, giving the same
MIPS ABI-version byte 0 as the object and avoiding the known GNU 2.42/2.44
hash-default difference. Both candidate and baseline links use it; strict ABI,
function/data/relocation checks stay intact. After this deterministic proof-only
repair, normal/optimized replay and the focused test pass; the entire receipt
changes only its verifier source digest. C source and all proof facts stay fixed.
Hosted CI is separate and must pass on the final published head.

Seven text source/test/proof files only. No accepted lock, production source,
protected target, raw assembly dump, object, ROM bytes or unrelated data changes.
