# w14x results (wave 14): func_800CC50C, func_800D63EC

No function reached a strict MATCH. Nothing is deliverable to cloud/matches, no group has claims, no splice or
lock change was made. Builder scratch: ~/rush2049/scratch/frontier/w14x (base copy, src/blob, include, tools/cloud,
asm/us/blob and blob_matched.lock.json synced from the Pi). Lane tools: `tools/dperm.py` (decl-order permutations),
`tools/gdiff.py` (aligned want/got disassembly for a group file, runs on the builder).

## Table

| Function | Bytes | State | Flags | Scorer line (this session, `score.py group`) |
|---|---|---|---|---|
| func_800CC50C | 760 (190 w) | best lead 146/190 words differ; not matched; shaping devices in the lead (see below) | -g0 -O3 -mips2 -G 0 -non_shared | `final/cc50c`: `146/190 words differ` (context func_800C7200 `36/64`, informational) |
| func_800D63EC | 324 (81 w) | best 68/81 words differ; structure close (mnemonic-missing 9); register/frame residual | -g0 -O3 -mips2 -G 0 -non_shared | `final/d63ec`: `68/81 words differ` |

Group dirs (lane-local, members only, no claims): `cloud/work/frontier/w14x/final/cc50c/` (member func_800CC50C,
context func_800C7200, keep both) and `cloud/work/frontier/w14x/final/d63ec/` (member func_800D63EC, keep it).

## func_800CC50C

Retail facts (tdis.py, 190 words): frame 104, saves ra and s0-s4; arg0 kept in s3, base (`ctx+0x4C`) and
`len` (`ctx+0x54`) re-read from arg0 after each call; `D_80111754[D_8014978C]+10.0` as f32 times `D_8002AFB4`
truncated to the offset; two `func_800A47C0` calls; `n = 3*len`; format_string_parse(base,n); `ctx+0x24 = 0`;
sound_play_menu(0,n); car_angular_velocity_clamp(base,n,v,n,9,10,4); the osRecvMesg(&D_80152770)/
audio_reverb_update/osJamMesg bracket twice; `ctx+0x4C = 0`; slot = func_800C7200(); task = *slot, memset 44;
P1 = audio_task_complete(0, n-derived+0x58) stored at task+0x28; memcpy(*P1, ctx, 88); the record Q = *P1 is
re-read through task+0x28 at each access (IDO does not CSE it across calls); returns slot (v0); `*arg1` byte =
(audio_buffer_sync(P1)+255)>>8.
Arity note: the retail callee audio_reverb_update takes its address in a1 (moves a1 into the func_80095F8C call),
so the prototype is 3-argument `(unused, addr, tag)`. The codex_heap_release_a25 group treats it as 2-argument
(`(address, tag)`), which disagrees with this call site; the first argument is never written in the caller.
Callers in src/blob declare `s32 func_800CC50C(void *, s8 *)`; the retail return value is the slot pointer, so the
draft uses `void **` (not integrated).

Rounds (vbatch, `--keep func_800CC50C,func_800C7200`, score.txt in each dir):
- r1 `f1/b1` (5 choice points, 32 variants): best strict 169 (size -26). Draft first: 170/190 words differ.
- r2 `f1/b2` (14 choice points incl. `TQ(task)` re-read form, 300 variants): best 168, mnem-missing 13 (from 51).
- r3 `f1/b3` (decl-order permutations of b2 best, 300 variants): best 167.
- r4 `f1/b4` (inline-float options, 300 variants): best 168 (no gain).
- r5 `f1/b5` (dead reads `if (base) {}` etc., 300 variants): best 152 (`v0074`, kept as `f1/best2.c`).
- r6 `f1/b6` (decl perms of best2, 300 variants): best 150 (`v0054`). Control `f1/ctl` reproduces 152 on
  best2 (control checked before every climb).
- climb `func_800CC50C/g1` (wbgen, 1061 variants + control): best 149 (`v0242`, `v0245`). Reviewed by reading
  the diff: v0242 writes `off = ctx->0x54` (off is dead after that point: value-preserving) and v0245 writes a
  dead `sp44` (value-preserving). Accepted as `f1/best3.c` as a lead only.
- r7 `f1/b7` (decl perms of best3, 300 variants): best 146 (`v0180`, `f1/best4.c`).
- climb `func_800CC50C/g2` (wbgen, 1060 variants + control): best strict 146 (`v0641`, mnem-missing 18). The
  one diff, `base = TQ(task)` before the record store, writes a dead value (base is not read after that point).
  Accepted as `f1/best5.c` (= final/cc50c), lead only.
- r8 `f1/b8` (decl perms of best5, 300 variants): best 146 (control). Stop: three rounds (g2, b8 and the
  permutations) with no strict improvement.

Diagnose (`tools.conveyor.pipeline.diagnose one func_800CC50C --source f1/best1.c --flags ...`, on best1 =
167): verdict `mixed(constant:10, structural:30, register:82)`, frame_delta 0, lever `declare-the-pair-later`.
The register residual is the s-register assignment (retail s2 base, s3 arg0, s4 n, and s0/s1 saved but unused).

Shaping devices in the 146 lead (must be disclosed in any deliverable; none is justified as original):
1. `u32 unused;` (never assigned) passed as the first argument of the first `audio_reverb_update` call; the
   second call also passes it. Reading an uninitialised local is undefined; it is a placeholder for the a0
   value retail leaves in place. The first call in best5 has `0` (choice option), not the unused local.
2. `f32 f;` is declared but not used in best5 (the inline option was chosen); `off` is reused for `n`.
3. `if (base) {}` (compiled out, a priority dial), `base = TQ(task)` (dead store, lever 40-type shaping).
4. `func_800C7200` context body is copied from the claimed src/blob/groups/codex_func_800C7200_2 unchanged.

Not resolved: the frame. The lead has a different callee-saved set than retail (retail saves s0-s4 and has a
104-byte frame). No pad arrays were used.

## func_800D63EC

Retail facts (tdis.py, 81 words): frame 48, saves only ra; guard `if (!D_8011025C) return -1;` (the -1 path
branches to the epilogue); osRecvMesg(&D_80142728, 0, 1); packet = D_80146170.head (ctx `+8`);
func_8009211C(&D_80146170, packet); packet->type (+8) = 0; result = packet; copies a,b,c,d (x,y,z each, lwc1 ring
$f4,$f6,$f8,$f10,$f16,$f18) into packet+12/24/36/48; func_80091FBC(&D_80146188, packet, D_80146188.head);
packet->ready (+9) = 1; osJamMesg(&D_80142728, 0, 0); return result. The pointers and packet are spilled and
reloaded around each call (no callee-saved registers).

Rounds (vbatch `--keep func_800D63EC`):
- r1 `d1/b1` (5 choice points, 16 variants): best strict 68 (`v0002`, mnem-missing 9, size -1). Draft first:
  79/81 words differ (size -9).
- r2 `d1/b2` (decl permutations of the best; only 2 distinct orders exist in this function): 68. Not a real
  300-variant round; the permutation tool cannot widen a 2-declaration function.
- r3 `d1/b3` (structural alternatives: a `Link *first` alias for the head, `Vec3 *dA` destination pointers,
  64 variants): 68, no gain. Stop (three rounds with no improvement).
- `-O2` check on the draft: 81 (no structure gain) standalone, 68 as a keep group at -O2. -O3 kept.

Residual: the retail allocation spills packet and the four pointers to the stack and uses no s-register, while
the draft keeps `packet` in s0 (frame 32, not 48). The draft's frame and prologue differ (addiu sp -32 vs -48).
Best file: `d1/b1/v0002.c` (= final/d63ec/group.c). No shaping device is in the best, but the draft still
needs the register/frame change before it can match.

## Integration notes
- No overrides, no superseded groups, no claims, nothing for cloud/matches.
- func_800D63EC also appears as context in the recipe groups ipa-groups/codex_record_indexed_a150 and
  codex_record_messages_a150 (those are not superseded by this lane).
- func_800CC50C: the claimed context group codex_func_800C7200_2 has a `void **func_800C7200(void)` body that the
  lead reuses unchanged.

## Permission denials
None.
