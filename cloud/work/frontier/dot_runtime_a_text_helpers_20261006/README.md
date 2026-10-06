# Four runtime-A text-helper candidates: 624 bytes

Frozen base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Stock IDO 5.3 whole-group pipeline, actual flags:
`-g0 -O3 -mips2 -G 0 -non_shared`.

New local code/relocation matching candidates:

| Function | Full native bytes | Matching words |
|---|---:|---:|
| func_8039B908 | 128 | 32/32 |
| func_8039B988 | 152 | 38/38 |
| func_8039BA28 | 184 | 46/46 |
| func_8039BAE0 | 160 | 40/40 |

All four report zero differing words, unresolved symbols, unverified
relocations, relocation errors and nonzero extra words. These are local
candidate results awaiting the independent checker, not accepted cartridge
coverage. Only these four functions appear in `claims`.

## Real caller closure

The helpers are called by complete BB80 and A5948 bodies. With only BB80,
IDO inlines them; the real second caller naturally retains their native private
ABI. B908 uses destination/player in s0/s1, B988 in s0/s2, and BA28/BAE0 use
s0 for destination. Natural C formals are preserved; no dummy formal, artificial
barrier, pressure routine, asm or dead call is used.

The group keeps complete real C140 and A5948 roots. The low-A reconstruction
contributes the four helpers and BB80; the high-A reconstruction contributes
complete A5948 and its A5658 child. This is one joint source context, not two
independent claims for the same helpers.

Pointer-valued singular/plural selection, rather than dynamically indexing
the string table with a ternary integer, closed BA28 and BAE0. The explicit
`!= 1 ? text[71] : text[70]` orientation preserves the native branch layout.

## Contracts and unresolved context

- `object_manager_update` is `s32(u8 *,s16)`, supported by locked
  `src/blob/groups/codex_sound_channel_extra/object_manager_update.c`.
- `object_type_byte2_get` returns `u8`; the protected 40-byte body returns a
  byte load. Copy and append use actual pointer-returning BE6A4/BE4F0 contracts.
- The real variadic formatter wrapper remains external. No result is consumed
  by these helpers. Language-table and format-string values remain external.
- Number buffers are reconstructed as 32 bytes, consistent with emitted native
  frames. Original source capacity is not asserted; no new bounds-safety claim.
- The main-blob returning `sign_extend_call` definition is a separate documented
  sidecar in `external_sources`, never a runtime-A compiler input.

Other observed group scores: B120/B214/BE48/A448/B00C retain their previously
reported zero code differences; C140 differs at 7/496 words; A4D4 at 623/716;
BB80 at 9/178; A5658 at 32/184; A5948 at 1025/1080. These are unclaimed context.
A5948 additionally has six unverified own-data references and a data-verification
failure associated with shifted geometry. Its literal-data reconstruction is
not accepted. Renderer float anchors elsewhere also retain unresolved values
and original ownership as documented in #224/#208.

Original local capacities in the high renderers remain hypotheses (rank text64,
name32, root text200); BB80 text80 is a frame-guided hypothesis. All complete
paths are present, but these research bodies are not asserted behaviorally
verified. The inherited clock qualifier is the pinned existing source-contract
hypothesis documented in #224, not an asynchronous-writer assertion.

This extends #224 with the genuine second caller and new helpers. Earlier
#190/#199/#208/#211 functions are context only, with no duplicate credit. Do not
install overlapping copies together; the checker chooses the integration unit.

## Reproduce

With stock IDO and GNU MIPS tools configured:

```
python tools/cloud/score.py group \
  cloud/work/frontier/dot_runtime_a_text_helpers_20261006 --claims --targets asm/us/ovl_a
```

Only matching compilation/scoring was used for this lean publication. There is
no new proof packet, acceptance suite, CI wait, lock or production promotion.
