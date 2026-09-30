# r5_f: precursors of track_collision_wall / Input_ProcessGameplayPad / func_8009F058 / particle_system (2026-09-30)

Matched (strict, copied to cloud/matches/): func_8009D45C (-O2), func_8009E9D8 (-O2), func_8008A38C (-O3; -O2 does not hoist the t0 base).
Group: cloud/work/ipa-groups/func_800878E0 (claims func_8008A38C only; func_800878E0 6/74, func_8008A644 12/24 as context).

Near misses (sources in this dir, all ABI, none claimed):
- func_8009D708 (165w): aligned shape 93%. `w` (float in a3) is spilled (`sw a3,12(sp)`) instead of promoted to $f18; any fp branch on `len` triggers it (promoted when the clamp is removed). Target also loads 0x4780 twice.
- func_8009D99C (173w): 172w, shape 99%; only `move t0,a1` missing (ours keeps src in a1 and puts d in a2). Copying `f32 *s = src;` removed the s0 save.
- func_8008A148 (145w): 144w, shape 94%; Gfx pushes as `g->w1=..; g->w0=..` macro; scheduling/register numbering differ only.
- func_8009EA68 (42w): 5/42 words (only f14/f16 register choice); sin spill slot fixed by declaring `f32 c, s, x, y;`.
Not attempted: func_8008A46C (118w, IPA: holds t2-t5 across calls), func_80087110 (445w), func_8009E8B4 (73w), func_80099B30 (51w), func_8009C3F8 (113w), camera_update_d (196w).
