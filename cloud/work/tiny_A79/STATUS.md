# A79 audio_update_a — frozen MATCH

Target 0x800F4D94..0x800F4FEC, 600 bytes / 150 logical words.
Final: `audio_update_a.shared_offset.c`, SHA256
`84232f12a5668871657abb057b48623b5db99538bf59e6592f6f75324ef3c051`.
Literal flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
Fresh Rocky canonical comparison is bare MATCH; fresh relocated full-word proof is
0/150, no non-full masks, extras, unresolved, unverified, or errors. The object has
152 words: the last two are zero alignment after the true 150-word function.

The original routine updates two actual Stats64 profiles for every active Player76.
The first lives in its resource data at byte offset 1292 plus the selected 64-byte
record; the second lives in D_80151410. Both use the same real byte offset
(selector byte minus 14)*64. Live120 supplies maxima, signed division input,
tallies, and ten signed halfword counters. The actual fallback handle aliases an
entry in D_80146150; a missing resource returns the whole routine. The final real
func_800CD8EC call retains the handle and current selector byte. No extra guards or
invented fourth argument, logical formals, data, dummy helpers, or accepted context.

Initial A source explicitly peeled two counters and addressed resource records via
a typed pointer addition: 130/150 plus one nonzero extra. Letting IDO peel the true
0..9 loop and writing the resource's consumed byte address reduced this to 8/150,
matching B60's independently written native hypothesis before its reservation
conflict was noticed. B stopped; A alone continued and claims the function.
Moving the live pointer before validation did not help. Explicit early profile
initialization regressed (111/150); default fixed stats before override regressed
(94/150). Final repair initializes the actual shared byte offset once per player,
before handle/resource validation, then uses it in both actual record addresses.
This reproduces all eight entry scheduling words. No register-pressure work,
unused locals, padding, source reflow sweeps, synthetic module context, or scoring
exceptions were used.

Source, packet.json, and verification.json are frozen for coordinator replay and
ROM/lock integration gates. Worker does not modify accepted source or commit.
