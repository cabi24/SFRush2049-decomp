# Game C27 — real unsigned-conversion audio wrapper

One frozen ordinary source is ready: music_track_control,404bytes. `game_C27/music_track_control.c` SHA256 `3349968fe5e759e259f4bb53abac117b84130d8638f4b376c78b7754b8758769`; exact first-line flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Canonical stack-sensitive score0 and complete unmasked101retailwords exact, no unresolved/unverified references, relocation errors or extra words. Target SHA256 `3b776a9481a161c6f822c78c43394167cefeb1e3b0ee34e813c280e8746965d0`, privately exported at Rocky `~/agents/C/scratch/game-C27/music_track_control.target.o`.

Original assembly proves f32 value in f12, u16 channel in a1 via its word save and halfword reload, and two real integer enable arguments in a2/a3. The source emits the two actual func_80020494 calls with complementary enable values. Both amplitudes use genuine `(u8)(u32)` conversions and the original127 multiplier; the first additionally multiplies named readonly D_80123A6C. Compiler-generated CFCSR conversion/fallback sequences match retail exactly; no manual CSR model, extra formal, pressure local or owned data is involved.

Two source compiles were needed: initial source misread the second literal as255 and differed exactly that one word; original `lui0x42fe` proves127. Corrected source and a fresh independent frozen replay both match all101words. Minimal source context is local scalar typedefs, named original f32 constant declaration, and the actual external callee declaration. Full404-byte body is accounted for with no padding. Shared accepted tree/state were not changed.

Ready artifact list: music_track_control.c, verification.json, frozen_verification.json, context_provenance.json, verify.py. Independent replay:

```sh
python3 PACKET/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target PRIVATE/music_track_control.target.o --output D_C27.json
```

Parent owns normal lock, full-image/ROM gates and final coverage credit. No remote job remains active.
