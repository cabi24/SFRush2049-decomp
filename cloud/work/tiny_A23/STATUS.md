# A23: nine genuine ordinary matches

Frozen full translation units, all with literal flags `-g0 -O2 -mips2 -G 0 -non_shared`. Nine claimed functions:104 words/416 bytes. All canonical score.py fn exits0, strict0 differing words, exact emitted extents, zero extra words, unverified/unresolved relocations or verification errors. All claims were absent from the authoritative shared lock at freeze. No IPA contexts or accepted-source modifications.

| Claimed target | Retail/emitted words | Full TU SHA256 |
|---|---:|---|
| func_800F75B0 | 8 | `3fb179f842533c013541ca06e8c4832e0f6ca2538ea9b9b0346bda74380db067` |
| func_800F7620 | 9 | `7bac951eca9d2619aa58d7415b13b43c469c7195dfe5b4c99df58969be6356b8` |
| func_800A13C4 | 9 | `4034846d857bff8149381e839369b039a8d0710d9867f131fe046f599f16c2ff` |
| func_800A553C | 9 | `8f005dc42d692a5bb1342ee0edbd78d403fb9f18024ba13fbe7a765c83181995` |
| func_800B930C | 11 | `114f1d30da1723799ba88d20e928d028bcbc33fc8fa36004c3b3cf07a0299756` |
| mode_flags_clear | 11 | `a94bc655ae8ce01f94ffd7935568411760f5603e21f064d7a0acdac663adacd6` |
| func_800C0828 | 13 | `5150cc069d8d16e43b80377a2ca0c6cfee740c9d1ad3fd3881a30c4cc82a1f33` |
| Input_InitPadHandlers | 16 | `d3585bfeae938bb28ae8648da69a1da6c837f7d2463be58af12290a21321cac1` |
| func_80095198 | 18 | `31f60dd7ce5eafe8a63590a7a3ff51b0907138af499bfcdd7c58b1466b245eab` |

Sixteen candidates inventoried from actual retail assembly;14 full TUs compiled. inventory.json records all decisions and sanitized counts. packet.json's claims lists only the nine matches. Five other sources remain explicit NONMATCH leads: sign_extend_call4/10;907707/11;FD7245/12;8B2E48/18;FD41C16/18, all exact extents and zero extras. A7830 andF7E30 were excluded because home-store/unused-formal interpretation was not justified. No inferred unused parameter was invented.

## Claimed semantic inventory

- F75B0 returns signed byte at0x80150F40+row*6+column; F7620 at0x80150E30+row*13+column. True full-word indices, signed-byte outputs, unsized row bounds; no guessed capacities or new bounds checks.
- A13C4 writes the low input byte at0x801440AD+index*772. The direct byte address avoids guessing a complete car-record layout. Initial one-word difference was a transcription error in the field address (256 bytes too far), repaired against the actual immediate and reverified; it was not an allocation trick.
- A553C writes four real u16 outputs at offsets0/2/4/6. All five inputs are consumed: output pointer plus four unsigned16-bit values. The actual first three value homes and the fifth stack argument's unsigned halfword load are reproduced by normal typed formals. Readable separate physical store lines repair two instruction-scheduling swaps from the single-line baseline, without changing order, width, or any runtime operation.
- B930C compares full-word index+1 with the signed16-bit count at0x80151CE8+8; equal returns signed16-bit first at+2, otherwise index+1. The private Path type is a minimal offset view, not a newly asserted full object capacity. No forced return/guard is introduced.
- mode_flags_clear zeros two actual byte flags at0x80110680/+1, then invokes the genuine declared zero-input audio_dsp_process. No substitute callee body or fabricated wrapper.
- C0828 swaps actual float matrix offsets4/12,8/24,20/28 with three genuine temporaries; preserves all other floats. Exact target access/store ordering follows the real three transpositions.
- Input_InitPadHandlers has eight consumed full-word inputs. Real32-byte record at0x80140BF0+index*32: stores third word at+4, fourth at+0, low halves of second/fifth/sixth/seventh/eighth at+8/+10/+12/+16/+18. Field+14 padding and12 trailing bytes describe actual stride; neither forces a stack frame. Pointer/callback meaning of the two raw word fields is not assumed; source retains exact32-bit words. No incoming narrow formals were invented because retail loads full words from its four stack inputs.
- 95198 uses real f32 x/delta and float threshold0x80152748: sum=x+delta; if threshold<x subtract14400.0f; return sum<threshold. It preserves actual float comparisons and operand order; no hand-coded alternate wrap behavior or hidden input.

## Bounded verification

Seven faithful baselines matched immediately; byte-address correction and separate-line u16 stores added two. Other controls: FD724 three real named products regressed11/12, so its exact original5/12 source was restored; sign_extend_call corrected the real fourth input omitted by a preliminary description, inspected the true four-consumed-input callee, compared narrow-formal versus full-word call declaration, and used equivalent bit sign extension to reach4/10 without inventing an ABI pressure formal. No further allocator/reflow/parent probes. Every claimed final file was canonically verified under the literal standard flags, with exit codes captured. No nonstandard assembler option, opaque stand-in, fabricated guard, or accepted-source mutation.

Reproduce each claimed NAME: `python3 tools/cloud/score.py fn cloud/work/tiny_A23/NAME.c NAME --flags '-g0 -O2 -mips2 -G 0 -non_shared'`. scores.json contains verdict/count/error/hash and canonical exit only. Raw score output and disassembly stay ignored build/codex-A23 or private Rocky A scratch; no ROM/object instruction stream is stored here. Root must independently score and pass image/full-ROM gates before acceptance.
