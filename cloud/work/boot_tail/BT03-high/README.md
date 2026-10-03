# Packet 3: bounded BT03-high small functions

Four new strictly matching bodies, **96 B**. Six complete NONMATCH reconstructions,
**312 B**, are research only. No cartridge coverage or promotion is claimed.
Independent paired source/ABI review and fresh strict replay passed;
the aggregate publication-head CI remains required.

- Branch: `dot/boot-tail-p3-bt03-high`.
- Base: completed Packet 2 `76780b3a1b3e26c54b86e1f153344e92dac15b20`.
- Explicit unmerged source-stack dependency: [PR #59](https://github.com/cabi24/SFRush2049-decomp/pull/59)
  head `21e104a22575cf4d639261d2f6d9535913074e6a`. Master prerequisite merges #52/#54
  are present at `b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`.
- The central writer recorded these exact ten claims in `9602cf96b41e7cb718dcf932da7786ffdb3a31f9`.
  This worker edits no central ledger, cluster, research generator, or D10 file.
- Assigned partition: `[0x80017500,0x80020610)`, excluding the entire independently
  owned C11 interval `[0x8001E0E0,0x8001E9B0)`. No second batch has been activated.
- Read-only local preflight: all three target manifest members and existing getter
  pass; canonical starts/sizes remain the Packet 2 census of 439 / 99,120 B.

## Frozen results

| Function | Bytes | Result / flags | Operation evidenced by the native body |
|---|---:|---|---|
| `800175A8` | 12 | strict MATCH, O2 | Clear word `D_8004BE84` |
| `800177EC` | 56 | NONMATCH 4/14, O1; O2 8/14 | Emit byte command 130 with value, shared selector and channel |
| `80017824` | 56 | NONMATCH 4/14, O1; O2 8/14 | Same command wrapper, command 7 |
| `8001989C` | 44 | NONMATCH 2/11, O2 | Return one for sentinel -1, otherwise test clear high bit of input word |
| `80019AA8` | 44 | strict MATCH, O2 | Read word table using state byte +75; remap 255 to index 8 |
| `8001C19C` | 60 | NONMATCH 11/15, O2 | If runtime enabled, write byte to the channel-indexed 40-byte stride |
| `8001C390` | 60 | NONMATCH 12/15, O2 | Clear leading byte of 24-byte records counted by `D_8004FA18` |
| `8001C770` | 12 | strict MATCH, O2 | Calculate unsigned doubled count plus 8 |
| `8001CC9C` | 36 | NONMATCH 6/9, O2 | Clamp an unsigned-byte value to 127 |
| `8001CCC0` | 28 | strict MATCH, O2 | Clamp unsigned word to 16383 and return unsigned halfword |

All matching headers use `-g0 -O2 -mips2 -G 0 -non_shared`; every compile adds
`-Wab,-r4300_mul`. These are whole-body relocated scores, not masked scores.
The four frozen matching files jointly pass 24/24 words, zero extra words,
unresolved symbols, unverified relocations or errors.

## ABI and data evidence

`800175A8` has no input registers or return value; its caller `800199F4` ignores
v0. `80019AA8` receives its state pointer through a0; caller `8001E940` forwards a1
and uses the returned word as an unsigned divisor. It is a word value, not an
invented pointer. `8001C770` takes one word; callers `800252AC` and `80025C68` use
its result as an allocation size. Unsigned arithmetic preserves native wraparound.
`8001CCC0` takes an unsigned word and clamps before halfword return; caller
`8001CCDC` passes a converted value and consumes the low halfword.

The two archived wrappers have exactly two byte inputs. Their sole callee
`80020610` masks all four arguments to bytes. The archive declares three
byte parameters and a word command parameter, which is always the in-range
constant 130 or 7. An all-byte prototype control produced the same residual;
the archived word declaration remains ABI-safe for these calls. Neither wrapper
introduces unused inputs or a callee body. Native argument-home spills are real;
spills alone do not establish O1.

The archive's `[][40]` and `[][24]` arrays express evidenced byte strides, not
stack padding or a claimed full structure. `8001989C` has no identified caller;
only its visible pointer/read/result ABI is claimed. `8001CC9C`'s caller passes
an explicitly byte-masked argument and consumes a byte return. No ABI defect,
extent defect, unresolved local rodata, or protected-path change was encountered.

Original middleware names, complete object layouts and N64 source version remain
unknown. These files were reconstructed from canonical targets. No public-source
body or unauthenticated arcade source was copied.

## Bounded experiments and remaining hypotheses

Initial O2 followed by O1 controls are in `initial_controls.json`. Three trivial
forms matched immediately; `80019AA8` matched on its second source form, expressing
the remapping directly as a conditional subscript. This matches the native
conditional move/branch shape without branch-likely duplication.

Before refinement, `tools/workbench.py diagnose` was run against temporary objects
built from unchanged canonical words. The raw diagnostic objects/instructions are
not committed. The initial nonmatches were structural/register mixtures, not
frame mismatches. On the improved `8001989C` 2/11 residual, diagnosis reported
allocation mismatch only: no opcode, frame, constant or instruction-count
residual, with two sites using a compiler temporary in place of the return
register. The report explicitly provided no proven register-steering mechanism.
No artificial keeper or forced register trick was added.

Directed controls covered natural conditional spelling, meaningful return
intermediates, integer promotion/prototype spelling, and loop induction forms.
No hypothesis exceeded twenty source variants. The closest natural complete
source is retained for each nonmatch; `verification.json` freshly recompiles each
at O2 and O1 and records any excess words. All archived sources remain NONMATCH.

Next hypotheses, requiring new evidence before reopening:

- `177EC` / `17824`: recover authentic byte-formal and prototype context. O1 is four
  schedule/argument-normalization words away, while O2 emits an extra internal copy.
- `1989C`: authenticate the boolean-result type/expression lowering that keeps the
  final comparison in the return register. Two-site allocation plateau remains.
- `1C19C`: recover authentic byte-formal narrowing context; operand widths/stride
  are known but argument normalization and temporary allocation differ.
- `1C390`: recover original loop induction/base-pointer representation. Counted,
  countdown and guarded pointer loops preserve semantics but allocation/scheduling
  differ; indexed form causes additional unrolling.
- `1CC9C`: recover authentic byte clamp/result expression context. Natural early
  returns, conditional forms and meaningful result locals did not close allocation.

## Reproduction and scope

After the standard pinned setup, run:

```sh
python3 cloud/work/boot_tail/BT03-high/verify.py
```

The script fails on a matching-source or extent regression and reports the complete
nonmatch controls without treating them as accepted. `status_delta.csv` is the
central integrator's input, pending peer approval and exact aggregate-head CI.
Only `cloud/matches/boot_tail/*.c` and this packet directory are changed. No ROM,
raw disassembly, object, credentials, target, scorer, layout, lock, symbol,
runtime-image source or production gate is changed or included.

## Independent review and focused checks

The paired BT05/BT07 reviewer independently compiled the exact frozen source
hashes and reproduced all sixteen matching/nonmatching controls. Its receipt is
`independent_review.json`. Only the four submission sources count as matches;
the six archived sources are validated solely as complete nonmatching research.
The 21 boot-tail metadata tests, deterministic ledger check and opcode-screen
check pass. These are focused checks, not ROM/production validation.
