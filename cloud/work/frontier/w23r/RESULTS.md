# w23r: recomp Phase 3 (per-call capture, input scripts, static binding, OSThread word mask) — tooling lane, worktree branch w23r

Status: in progress (2026-10-12). Not a matching lane: no src/, asm/, lock or coverage change.
Host: rocky, `~/rush2049/recomp/w23r` (copy of this branch's tracked files; nothing ROM-derived committed).

## Plan
1. Per-call capture at a replaced function's entry/exit in both builds (stock build gets transparent capture hooks).
2. Input scripts under tools/recomp/scripts/*.press (audio options, second track, two players).
3. Static binding of replaced bodies' local statics to retail addresses.
4. OSThread host-pointer word mask instead of unjudged pages.
5. Tests, README, plan section 11, WAVE_BRIEF "Behavioural check".

## Early verdict for w23f (engine_sound_sync draft w21h/engine_sound_sync/best.c), 2026-10-12

Set `draft_engine_sound_sync_probe` (draft + 4 callee probes), race script, 1800 frames:

    FIRST DIFFERING CALL: seq 1444 = entity_transform_calc (probe_entity_transform_calc) call 1, frame 705: args
      f22: stock 3F800000, replaced 00000000
      f28: stock 3F800000, replaced BF800000

- Calls 1-1409 of engine_sound_sync are identical (no effect before the race); the first call that does
  work (seq 1441, frame 705) calls func_80091BA8 / func_80098A54 identically, then its first
  entity_transform_calc (0x80091CA4) call differs.
- Cause: retail passes entity_transform_calc's four values in callee-saved FPRs f22/f24/f26/f28 (target.s
  0x80098F30-0x80098F40 `lwc1 $f22,360($sp)` ... `jal 0x80091CA4; lwc1 $f28,348($sp)`; the callee compares
  each with the -2.0 "unchanged" sentinel in f20: 0x80091CC8 `c.eq.s $f22,$f20`). Retail values on call 1:
  f22=1.0 f24=0 f26=0 f28=1.0. The draft calls `entity_transform_calc(handle,out[3],out[2],out[1],out[0])`
  o32 (a1-a3, sp+16): the callee reads whatever is in f22-f28 (0.0 / -1.0 here). a1-a3 in retail are
  stale array pointers (gains/pan/depth at sp+0xE8/0xC0/0xD4), not arguments.
- Effect without probes (set `draft_engine_sound_sync`): call 1410 (= seq 1410, frame 705) writes 80 words
  vs retail 78; 20 differ, all the +4 float of 12-byte records at D_80142DD8 (stock 1.0/-2.0 alternating,
  draft -2.0/-1.0) = what entity_transform_calc stores from the wrong f22-f28.
- So the draft's arithmetic is not shown wrong yet; its call convention to entity_transform_calc is. That
  convention (arguments in callee-saved FPRs) is what IDO -O3 produces when caller and callee share a unit
  (like audio_effect_setup taking `effect` in s0, w21h); a standalone draft cannot reproduce it, so the next
  verdict needs the draft built in a TU with entity_transform_calc (or a register-passing shim).
