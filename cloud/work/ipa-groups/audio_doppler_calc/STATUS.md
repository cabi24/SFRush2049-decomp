# audio_doppler_calc (really a text-string blitter) -- draft, 2026-09-30

**Not a match: 281-word target, our function emits 279 words, 161/281 words aligned exactly after diffing
(register-blind structure ratio 0.86).** New group, extscore.py technique (copied from `../draw_number`):
`python3 cloud/work/ipa-groups/audio_doppler_calc/extscore.py cloud/work/ipa-groups/audio_doppler_calc --norm`
(target words come from `score.targets()`, the function is a registered symbol at 0x800B6788).

The name `audio_doppler_calc` is wrong: the function draws a string with a proportional font. IPA params:
`$s2` = `u8 *str`, `$s0` = `u16 len` (home slot at sp+4, `andi 0xffff` at entry). If `str[0] == 255` the text is
two bytes per character (`len -= 2`, `str += 1`, step 2), else one byte (`len -= 1`, step 1). Steps:
`sound_update_channel(force=0)` (its parameter is IPA `$t0`), `func_8008A3E4(0,0,D_8002AFC0-1,D_8002AFC4-1)`,
`func_800878E0(flags)`, `func_8008705C(~flags)` (already a matched group), `D_80149B4B = (u32)D_80114748`
when `flags & 0x20`, `func_8008A148(&D_80149B48, ...)`, `object_render` of texture page 0
(`D_80149820[0]`: `w@0x10, h@0x12, img@0x18`), then per character: `10` = newline (x reset to
`D_80149D92`, y += line height bytes 2,3 + `D_80149B60`), `32`/`>=256`/no glyph (`D_80149878[c] < 0`)
= advance by space (`b6 + D_80149B70` if byte 9 else byte 7), otherwise a glyph record
(`*D_80149800`, 12 bytes: `kern` table ptr, page byte 5, `b6..b9` box) with optional kerning (`kern[prev]` when
text byte 10 is set and byte 9 clear), page switch (`object_render` again), centring, and
`func_80087110(x,y,x2,y2,w,h)`.

What is done: control flow and calls match the ROM in order; the "space" case is shared via a `goto`
(the ROM tail-merges it: `idx < 0` jumps into it); `cur = 0` before the first `object_render`; `i = 0` before the
`str[0] == 255` test (goes in the branch delay slot as in the ROM); K&R parameter list (`u16 len`).

Blockers:
- Frame 120 vs 112 and the spill slots for `two`/`step` (ROM sp+72 / sp+92 next to the `t2` spill at 76; ours
  sp+92/96). Declaration-order permutations of the ten locals did not change it.
- `prev` lives in `$a0` (ROM `$v1`), string base `$t1` (ROM `$t2`); a few delay-slot placements.
- `sound_update_channel`'s `$t0` parameter cannot be expressed from C (ours passes `$a0`).
- Callers `menu_input_process` and `world_velocity_integrate` are absent (two stand-ins, `caller_a/b`, give IPA
  registers), so this is not spliceable.
