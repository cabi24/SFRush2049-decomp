# func_8008C680 — strict group MATCH, awaiting maintainer review

At repository HEAD `4b4278bdee11711f95f51b989a3b8811432a9d7b`, the target is an
extracted game function at 0x8008C680, 40 words (160 bytes), and is absent from
`blob_matched.lock.json`. Its only callee, `func_8008C5E0`, is already locked.
No source, lock, assembly, manifest, or shared tooling was changed.

The review candidate is `group.c` with `group.json`. Compile it as a
real two-function call group: `-g0 -O3 -mips2 -G 0 -non_shared`, using the scorer's
whole-program stages and `uld -kp` list containing only `func_8008C680`.
The scorer also adds the repository's `as1 -r4300_mul` errata flag.
`func_8008C680` is the sole member/claim. `func_8008C5E0` is real context, with its
own independent strict MATCH; no stand-in, extra formal parameter, pressure work,
inline assembly, or compiler/tool modification is used.

Source SHA256:
`6c538f6747f3a45db0f34e9ff96270cc1c1cfcef6e5b8d3b3568da2063a51f88`

## Reproduce

Run from the repository root:

```
python3 tools/cloud/score.py group cloud/work/ipa-groups/sol_high_c680
python3 tools/cloud/score.py group cloud/work/ipa-groups/sol_high_c680 --claims

```

Both scoring commands exit 0 and print:

```
Members:
func_8008C680:
  MATCH

Context (informational; excluded from exit status):
func_8008C5E0:
  MATCH
```

Every accepted comparison has 0 differing words, 0 extra nonzero words, empty
unresolved/unverified relocation lists, and no relocation errors. The target
region and symbol-table integrity gates are unchanged. Both bodies are 40 words;
all 80 words match, including stack frame/offset words and resolved calls/data.
Repeated final builds produced identical object SHA256 values. Detailed input,
compiler, source, and object hashes are in the local receipt; `score.log` records the
normal strict CLI output. The stock setup controls `sound_handles_clear` and the
`resource_slot_clear` group also passed plain MATCH.

## Purpose and source provenance

C680 applies three argument ranges to a real rational approximation: direct
helper call below E4, reciprocal plus EC correction above E8, otherwise
`(x-1)/(x+1)` plus F0 correction. This resembles positive arctangent range
reduction (hypothesis); coefficient values and an arcade identity were not
assumed. The arcade tree `reference/repos/rushtherock/` is unavailable in this
checkout, and the available cross-reference has no C680 identity.

The C680 logic is unchanged from the historical near-miss seed. C5E0 is the
actual sole callee, reconstructed from the locked
`src/blob/func_8008C5E0.c` (SHA256
`8fdd5b0d81a3689a691ca5f3a8d6f2f61a31c4dca81c3f6fae5270c1ae73aa84`).
The final helper removes historical redundant square copies, an identity multiply,
and dead `if (0)`; ordinary square/coefficient locals preserve the original
rational expression and compile exactly under this group recipe. All thirteen
coefficients/thresholds remain the original external f32 symbols.
A complete retail `jal` scan found C5E0's only call sites at C680 offsets
0x24, 0x54, and 0x7c. This is the genuine callee closure for the chosen external
root, rather than a fabricated context example.

## Attempt log

| Source/build | C680 strict words | C5E0 strict words | Interpretation |
|---|---:|---:|---|
| Historical whole-TU baseline, recorded O2 | 4/40 | — | Reconfirmed current target |
| Genuine locked helper, O3, both names in keep list | 4/40 | 0/40 | External helper visibility keeps residual |
| Same genuine helper, only C680 in keep list | 0/40 | 0/40 | Internal helper closes both bodies |
| Clean helper, only C680 in keep list | 0/40 | 0/40 | Ordinary C89 without dead constructs |
| Same clean source, both names in keep list | 4/40 | 28/40 | Controlled visibility change restores caller residual |
| Final commented source/spec, normal and claims scoring | 0/40 | 0/40 | Repeated strict verification |

This was a bounded directed attempt: four substantive group probes plus baseline
and final verification, stopped after strict matching. Historical source sweeps
were inspected in `workbench_pilot_C1.md`, `near_miss_B.md`, `hand_notes_B.md`,
and `r5_h/t6`; their already exhausted local/operand/literal/dead-read/flag and
global-definition variations were not repeated.

Workbench diagnosis was attempted before edits, but GNU MIPS objdump is missing;
the tool explicitly rejected the unavailable executable. Native decoder evidence
from the local receipt confirms the baseline's only four changes: f4 versus f16 on the
last-range denominator add/div, and f6 versus f18 on the correction load/add.
The historical pilot measured a six-entry candidate FP temp sequence versus a
four-entry target sequence. New controlled evidence shows genuine helper
internalization resolves this difference. IPA-driven FP register reservation is
the likely mechanism; that compiler-internal explanation is an inference, not an
instrumented trace. This is a group match, not a standalone O2 match.

No commit, promotion, splice, publication, image gate, or ROM hash was performed.
No cartridge coverage is claimed. The existing dirty setup/test changes and
other workers' files were preserved. Maintainer integration must preserve the
existing helper lock or adopt the verified group through the proper image/ROM
workflow.
