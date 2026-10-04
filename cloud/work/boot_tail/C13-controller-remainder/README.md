# C13 controller remainder: complete nonmatching reconstruction

Scope: `800206AC–80020820` (372 B) and `80020A04–80020DA8` (932 B),
**two COMPLETE-NONMATCH bodies / 1,304 B; zero new matching bytes**.
Branch `dot/boot-tail-c13-controller-remainder`, base
`96b9dd1979f7c797f97a1dd2317fc0852092c0dc`; central claim acknowledged at
`c3b07951`. The disjoint `C13-controller-pair` packet owns `20820`/`20DA8`.
No shared ledger/D10, header, target, scorer, lock, layout or prior candidate changed.
No ROM, runtime-image or farm work was performed.

## Native ABI and dependencies

`206AC` is `void(u8 controller, u8 channel, u8 set, u16 value)`. Its genuine
caller `2165C` loads channel/set from voice bytes +74/+75 and passes the clamped
value through a halfword mask. The body itself does not clamp the value and has
no channel-255 guard. It uses a 48-byte frame, preserves s0/s1/s2, and calls only
`20610`. Inspection of that callee establishes four byte arguments and a final
seven-bit store mask; no invented formal or inserted helper implementation is
used in the reconstruction.

The setter sends high then low components for selectors below 64 (base selector
masked with 31, second selector +32), 128/129 (even selector, +1), and 132/133
(even selector, +1). Other selectors send the high component only. Argument
conversion retains `(value >> 7) & 255`, even for values above the ordinary
14-bit caller range; the real callee subsequently stores only seven bits.

`20A04` is `u16(u8 controller, u8 channel, u8 set)`, a frameless leaf. Both real
callers `21150` and `215A8` provide the same channel/set bytes and consume its
halfword result. Native arithmetic proves regular set stride 2,144 bytes,
regular channel stride 134 bytes, and effects channel stride 134 bytes. Set
255 selects `D_80055000`; other sets select `D_80050D00`. The declarations
`D_80050D00[][16][134]` and `D_80055000[][134]` preserve these strides without
inventing overall allocation bounds. They agree with the existing C13 accessors.

The getter combines high and low bytes using bitwise OR for the paired selector
categories above. Selectors 64..69 return 0 or 16,383 according to whether the
stored byte is below 64. Selectors 96..101 return zero without reading the table.
Other row-valid selectors return the stored byte shifted left seven. Arbitrary
stored bytes are preserved in the reconstruction; values are not presumed
seven-bit or clamped. There are no local-rodata relocations or switch tables.

Overall native set/channel allocation bounds remain unproved. Well-formed C
access requires an allocated outer record, regular channel 0..15, and selector
0..133. The native getter has no bounds checks and can address beyond these
logical rows for other byte inputs. This packet does not prove those accesses
valid or infer that every caller enforces the row domain.

## Compiler evidence and bounded controls

Pinned compiler verification checks all 24 files against the prior v1.2 IDO 5.3
preflight. All target hashes, the 439-start/99,120-byte inventory, and the existing
12-byte getter strict replay pass. Every source is compiled at O2 first, then O1,
with `-g0 -mips2 -G 0 -non_shared` and the scorer's automatic
`-Wab,-r4300_mul`. `scores.json` records source/input/compiler hashes, relocated
scores, actual ELF STT_FUNC sizes, text alignment, frame sizes and uncertainty.

| Retained source | O2 result | O1 control |
|---|---|---|
| `206AC` | 91/93 differing words; 10 nonzero excess; function 412/372 B; frame 48/48 B | 91/93; zero nonzero excess; function 372/372 B; frame 24/48 B |
| `20A04` | 233/233; 8 nonzero excess; function 968/932 B; frameless | 230/233; 19 nonzero excess; function 1012/932 B; 8-byte frame |

All final rows have zero unresolved symbols, unverified references, relocation
errors and relocation masks. Neither compiler level is a match. In particular,
zero nonzero-excess is not an extent proof: the initial setter O1 function was
only 364 bytes against the 372-byte target. Its retained O1 successor has the
correct extent but still has a different frame and 91 differing words.

Only **two natural source forms per function** were evaluated:

1. The initial setter named the common masked selector in a genuine local. After
   `tools/workbench.py diagnose` identified the 56-versus-48-byte frame and
   displaced local home, repeating the needed mask at both calls moved its
   call-crossing value into the compiler's temporary home. This restored the
   native 48-byte frame and improved 92 to 91 positional differences. It is
   retained. Necessary byte conversions still create extra copies and shifts.
2. The getter initially named the genuine pair pointer. After diagnosis,
   repeating the array expressions was tested, independently supported by the
   public source lead. O2 output stayed identical; O1 grew to 1,252 bytes.
   The simpler named-pointer source is retained.

Diagnosis ran before refinement and again on the retained sources. Metadata-only
results are in `diagnosis.json`; raw objects/instructions remain temporary and
are not published. The next hypothesis requires authenticated N64 narrowing,
coalescing and source-context evidence. The session stops at this familiar
entry/result-copy plateau rather than repeating exhausted declaration, flag,
prototype or instrumentation sweeps. No padding, redundant keeper expression,
fake formal, forced register, dummy call or assembly is used.

## Source lead, not native-version proof

[AxioDL/musyx `snd_midictrl.c`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_midictrl.c)
at `78d2e16e4905fc675952162d331c24d5198b2687`, licensed
[CC0-1.0](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE),
contains `inpSetMidiCtrl14` and `inpGetMidiCtrl` definitions with the same selector
families and split/merge operations. The public version adds channel guards and
orders the getter's tests differently; its outer array bounds do not establish
native bounds. Native instructions remain authoritative. No third-party source
file, header or table is vendored. The public names remain source-lead labels.

The [Snowboard Kids notes](https://github.com/cdlewis/snowboardkids-decomp/blob/main/DECOMPILATION_LEARNINGS.md)
on narrow-parameter homing and prototype context were read only as caveated
background; their unlicensed text is not copied into this packet. The vendored
CC0 workbench's lever advice was tested on the actual prescribed flags.

## Semantic verification

`test_semantics.py` compiles retained and directed-control C under host C89,
pedantic warnings and Werror, then compares them with an independent integer
model and a fail-closed replay of the unchanged canonical target words.

- 11,264 setter cases cover every byte selector; representative ordinary,
  effects and sentinel channel/set values; 11 boundary halfwords through 65,535;
  high/low call ordering; and all three paired categories. Native replay poisons
  every ordinary O32 caller-saved register after the external call and verifies
  stack/callee-save restoration. No external call effects are silently assumed.
- 13,668 getter cases cover all 134 row-valid selectors, six arbitrary synthetic
  data patterns including full-range random bytes, both table bases, set/channel
  stride boundaries, thresholds and the no-read category. Native read addresses,
  returned values and lack of table mutation agree with the C/model. Dirty high
  bits on native entry test the byte/halfword ABI normalization.
- The interpreter rejects unknown opcodes, unmapped accesses and uninitialized
  stack reads. Its memory is synthetic; target words come from the checked-in
  integrity-verified targets. The eight-set/64-effects fixture bounds are test
  storage only, not recovered native global sizes.

Commands (set `IDO_DIR` to the existing pinned shared installation):

```sh
python3 cloud/work/boot_tail/C13-controller-remainder/verify.py --check
python3 -m unittest discover -s cloud/work/boot_tail/C13-controller-remainder -p 'test_*.py' -v
```

`status_delta.json` is a proposed two-row update for the central sole writer.
Independent peer review passed at `d25da8a1` (see `REVIEW.json`).
Aggregate publication remains separate. No cartridge coverage,
maintainer acceptance or promotion is claimed.

The existing cloud setup/guard/submission/integrity/scorer regression passed all
631 tests. Its first sparse-worktree run lacked unrelated tracked fixtures;
materializing those read-only test inputs resolved those missing-file failures.
No runtime image was regenerated or changed.
