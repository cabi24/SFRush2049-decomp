# s20261004 lane E — results

All six targets are IDO -O3 interprocedural (IPA) functions: `slot_state_setup`
takes its selection in `$s2`, and callers/callees clobber callee-saved regs
unsaved. They only reproduce in a whole-program group build (blob_group style,
see `build.sh`), never standalone at -O2.

Key findings (reusable):
- `osRecvMesg(&D_801461D0,0,1); slot_state_setup(n); osJamMesg(..)` is an
  umerge-inlined static helper (`font_set` with nested param-less
  `gfx_lock`/`gfx_unlock` inlines). This reproduces the dead `sw v0,N(sp)`
  (24-byte per-instance stack slots: 92/68, 84/60, 76/52 in every caller) and
  the dead `move s0,v0`. umerge inlines across TUs; single-call-site functions
  are always inlined (credits_scroll needs its 2 real callers).
- `object_create` = `{s32 old; osRecvMesg; old=slot_state_setup(sel); osJamMesg; return old;}`
  compiles byte-identical in the group (not a lane target; not claimed).
- `D_80118E20` is the `Pair` of locked `func_800B669C`; `func_800B669C(1,1)`
  inlined reproduces the `lui v1/addiu v1/sw 0(v1)/sw 4(v1)` start.
- build/m2c_asm/*.s mis-renders some `sw rX,4(base)` as `lui/%lo(D_80118E24)`
  pairs and so misstates sizes; use raw image disassembly (`tdis.py`).
- slot_state_setup context (var/v1.c) gets `$s2` selection but s0/s1 swapped
  vs retail and inlines object_byte9_set (only one caller in group).

## Per function
| function | result | best / residual |
|---|---|---|
| credits_scroll | MATCHED true-zero | credits_scroll.c, group in credits_scroll_group/, proof in credits_scroll.json |
| game_timer_display | 6 instr lines (same 352B size) | frame 112 vs 120; `h` (s16) slot 110 vs 116 (retail has 8 more bytes of locals/inline slots); `addu a3,t0,t7` operand order for h+D_80110650[2]. `D_80110650[1]+D_80110650[0]` order fixed. Source: src/timer.c |
| func_80100B8C | 16 instr lines (416 vs 408B) | frame 104 vs 144 (+40 unexplained locals; an unused buf[40] closes it but is a padding hack, rejected); retail reuses `v0`=1 for `bne v0,t6` + hoists a0/a1 setup into the branch (ours `li at,1`); end `Pair` stores share one `lui at` in retail. Source: src/timer.c |
| time_result_display | not matched (twin of credits_scroll, compiles identically shaped) | needs its 2 real callers: func_800DB1E0 (m2c fails: jump table) and replay_save_prompt (seed in seeds/); with a stand-in caller it was not measured as identical (single-site inline). |
| func_80101904 | not attempted (time) | IPA caller of func_80100D5C (args in t1-t4) and slot_state_setup; preserves a0/a1 |
| func_80101D84 | not attempted (time) | same family as func_80101904 |

Diagnostic-only stand-in builds (gd*, diag_standin) are NOT valid evidence.
