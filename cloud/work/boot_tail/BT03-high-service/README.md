# BT03-high: six gated service routines

Five initial-form strict O2 matches, **424 B / 106 words**. One complete 120-byte
NONMATCH remains research only. Independent paired review passed; aggregate publication-head CI
remains required. No cartridge coverage or promotion is claimed.

- Branch `dot/boot-tail-bt03-high-service`, fresh-master source base
  `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Exact claim activation `cb98b99b`: `18FA4+72`, `19144+80`, `19370+88`,
  `19850+76`, `1C77C+120`, `1C7F4+108`; all start in `0x800...`.
- All six remain inside BT03-high and outside C11. Earlier packets are frozen;
  no source dependency on previous drafts or unmerged research is introduced.
- Central generated status, claims and D10 are untouched. Fresh manifests,
  439-start/size equality (99,120 B) and the preexisting getter replay pass with
  unchanged pinned IDO through IDO_DIR.

## Matching contracts and genuine ABI

All bodies are gated by `D_8002C630` and bracket enabled work with the previously
audited no-argument `80014594` / `800145DC` nesting/queue helpers.

| Function | Bytes | Enabled operation |
|---|---:|---|
| `80018FA4` | 72 | Forward word identifier and unsigned halfword to `80018F20` |
| `80019144` | 80 | Forward identifier and two raw words to `800190AC` |
| `80019370` | 88 | Forward byte, halfword, identifier word and byte to `80019194` |
| `80019850` | 76 | Forward state pointer, result-word pointer and zero byte to `80019490` |
| `8001C7F4` | 108 | For an active 24-byte record, clear its leading byte and call `8001F954(index)` |

The first three body callees validate the actual identifier through `80017644`.
Their native homes, LHU/ANDI widths and raw word stores corroborate the declared
scalar types. No floating-point interpretation is invented for the two forwarded
words in `19144`.

`19850` genuinely takes two pointers. `19490` dereferences its first input to
obtain an identifier and other state fields. It writes a word through its second
input (including failure -1); the third input is read as a byte and is zero in
this wrapper. The submitted source therefore uses opaque `void *state` and
`unsigned int *result`, rather than a false no-input or scalar-only ABI.

`1C7F4`'s external `[][24]` byte array expresses only the evidenced record stride.
The first byte equals one for the branch being handled; it is cleared before the
body call. The actual index remains a word, and `1F954` tests it against -1 before
using it in an indexed record address. No invented object layout or header edit
is required. All body addresses are in the canonical census; their existing
internal calls and data remain unclaimed.

## Complete nonmatch and bounded controls

`8001C77C` has five genuine inputs: a word channel/index and four unsigned bytes.
When enabled and the channel is not -1 it calls `80014A74` with the index plus
four byte values shifted left sixteen bits, then leaves the nesting helper.
The fifth incoming byte is read from the caller argument area; the fifth outgoing
scaled word is stored at stack+16. Callee `14A74` reads all five actual words,
including incoming stack+16. These are authentic arguments, not frame padding.

The complete archived natural source is NONMATCH: O2 differs in 19/30 native
words with two nonzero excess words; O1 differs in 29/30 with three extras.
Before refinement, unmodified workbench diagnosis found a structural/register
mixture: compiler argument copies add three real instructions while the frame
size remains -32. `diagnosis.json` binds this pre-refinement observation to the
original source hash and timestamp.

Three directed natural controls tested signed fixed-value prototype spelling,
explicit unsigned shift operands, and word formals narrowed to the same real
byte domain at the call. Every control retained the original O2 residual, so the
initial byte-formal source is archived. No further sweep is justified without
new authentic argument-lowering context. Next hypothesis: establish how the
original compiler context coalesced the shifted byte arguments directly into
outgoing argument registers. Do not add dummy formals, keepers, padding, assembly
or edit protected tooling to remove the copies.

## Flags and proof

The five matching bodies succeeded on their first natural O2 forms. Four also
match at O1; only `1C7F4` distinguishes these levels (20/27 words differ at O1).
O2 is the prescribed first successful level, not an assertion that the other
four tiny wrappers uniquely prove that historical level. All sources use base
`-g0 -O2 -mips2 -G 0 -non_shared`; the scorer automatically adds
`-Wab,-r4300_mul`.

`verification.json` binds twelve final O2/O1 rows to source hashes. Five distinct
O2 bodies total 106 words with zero differences, extra words, unresolved symbols,
unverified relocations or errors. Exact O1 controls are not additional matches.
The archived nonmatch receives no matching credit. No local-rodata assumption
is involved in the submitted wrappers. Original names, full types and middleware
release remain unknown; no third-party body or arcade identity is invented.

## Replay and scope

```sh
python3 cloud/work/boot_tail/BT03-high-service/verify.py
```

`status_delta.csv` is the sole central writer's integration input. Only this
packet directory and five submission C files are changed. No target, compiler,
scorer, symbol, lock, layout, runtime image, farm, forbidden helper work, ROM/raw
instruction dump, object, credential or production gate is changed or published.
Independent review, exact-head CI and checker-owned merging remain separate gates.

## Independent review

The BT05/BT07 paired reviewer independently replayed all twelve controls and
exact source hashes on commit `7c2d08d8`. It reviewed actual wrappers/callee ABIs,
including `19850` state/result pointers and `1C77C`'s real fifth incoming byte.
`independent_review.json` confirms five distinct O2 bodies and the honest
120-byte NONMATCH. Four exact O1 rows are only flag evidence. No source/ABI
blocker was found.
