# Worker B6 — strict near-miss reconstruction

Private Rocky B checkout/scratch only for compilation. Verified these heads remain unaccepted; copied current read-only DB target objects to workbench privately. Strict scorer reads protected target assembly/symbols. No source, asm, lock, farm state, tooling, commits or integration changes. No MATCH deliveries in this packet.

| Target | Strict baseline | Best retained | Interpretation |
|---|---|---|---|
| func_80096C28 | 24/32 | 20/32, O2 | Correct shape, unresolved allocation |
| func_800DCD58 | 23/39 + 2 nonzero extra words | same, O2 | Requires genuine IPA context |
| func_800EF5B0 | 27/31 | 26/31, O3 | Corrected natural behavior; wrong frame/register homes |
| func_800D2C10 | 43/49 + 1 nonzero extra word | 26/49, O2 | Unsigned loop index avoids unrolling; still not close |

Best complete TUs are retained in `cloud/work/near_miss_B6/`; all are NONMATCH and must not be spliced.

96C28: reconstructed typed relocation input/output headers and actual 12-byte entries. Removed named intermediate load variables to reduce persistent webs: 24 to20. The target's sign test deliberately contains `sll t9,t8,0`; replacing the mask with >=0 loses it. Typed arrays, byte-offset versus element-index loops, reusing the dead base carrier, declaration ordering and code-free demand changes stay20 or worsen. Earliest residual is target t6 load versus candidate colored v0; target offset pointer uses t1 while candidate uses another colored pointer. No statement-shape mismatch was accepted as evidence of a match. About20 new probes; known prior typed-loop attempts were checked.

DCD58: target clobbers s0/s1 without preserving them and delays stack adjustment until after initial table loads, as documented in hand_notes_B. Standalone O2/O3 both23/39 plus2 nonzero extras. Removing the seed's undefined uninitialized `if(temp_s1){}` preserves score and is the retained source. Passing cached flags to audio_doppler worsens25+3. Source-only brute force cannot establish a correct group; genuine callees/callers are needed. Four baseline/context probes, deliberately bounded here.

EF5B0: reconstructed initial owner store before either branch, conditional allocation or collision_sound_play, then Input_ApplyPadConfig. Seed put owner initialization in only one branch. Corrected D_80140BDC from a word to a byte. Target uses t0 for arg0 with home32 and frame32; standalone keeps arg0 in saved s0, spills arg1, and uses frame40. Target materializes a distinct address for the byte read while scalar/array candidate folds its load. Parameter types, explicit byte cast, prototypes, aliases, declaration-home probes, code-free formal reads and O1/O3 were flat26–30. O3 retained26. About25 probes. No invented IPA helper used.

D2C10: source comes from near-miss/base.c; target has the actual R4300 multiply interlock nop, and strict scoring includes the errata assembler flag. Baseline diagnostic without that flag is misleading41; strict is43+1extra. Changing s16 loop index to s32 triggers the known131-extra-word unroll. New u16/u32 index trials avoid it: u16 gives27, u32 gives26. Explicit signed comparison restores huge unroll. A diagnostic-only result[16] array reproduced frame32 and result home6 but did not improve26, so the retained source uses the original scalar result rather than padding. Reusing the input pointer, volatile count and supported no-unroll flag did not improve. Remaining index/comparison/GP allocation and frame differences prevent acceptance. About13 new probes. Two speculative unsupported UOPT options failed compilation and were discarded; no flags/tooling changes were made.
