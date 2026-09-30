# billboard_render -> 186/244 words aligned, register-blind 0.96 (real function, extent 244 words)

Real head 0x800F64D4 (`addiu sp,-144`, saves s0-s8 and `$f20/$f22`), one caller: `countdown`.
The label is a misnomer: it is a mode state machine (`switch (D_80140B20)` over 5 cases, jump table
`0x80124604`) that calls `func_800F1210(new_state)` (IPA param `$s2`) to change state:
1 `func_800F2718`; 2 loop over `D_8014A108` records (76 bytes at `D_8014A118`) calling
`resource_type_select`/`func_800F1210(3)` when `w4&3` or `viDeadlinePassed()`; 3 clear per-slot
resources then go to state 4 or 5; 4 `func_800C813C(0,0); func_800F1930()`; 5 the
`audio_doppler`/`viScheduleTick(15.0f)` toggle of `D_801148D0`. INDEX's 665 insns are wrong.

Draft: `br.c`, context stand-in `ctx.c` (`func_800F1210`, 454 words retail; the stand-in only
forces the caller to save s0-s8 and two float registers). Verify:
`python3 cloud/work/ipa-groups/billboard_render/extscore.py cloud/work/ipa-groups/billboard_render --norm`
(the `.rodata` warning is the jump table).

Remaining differences (247 vs 244 words):
- frame 104 vs 144 (retail has 40 more bytes of frame, locals at 84/92/100/112; mine 96/100);
- one spill in the state-2 loop (`sw s6,92(sp); sw s7,100(sp)` around `func_800F1210`) is
  ordered differently;
- state 5 branch polarity at one `bnezl/beqzl`.
Address constants are checked by `extscore` (relocation-resolved); note `D_803BA7EB` is an
address argument to `func_800CC50C` (lui 0x803C / addiu -22549).

Blockers: the real `func_800F1210` (454 words, unregistered in this group) and `func_800F1930`
fix the register/frame; a stand-in cannot reproduce the 144-byte frame.
