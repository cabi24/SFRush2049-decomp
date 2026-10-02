# Game C29 — canonical unsigned numeric formatter

Frozen ordinary source `game_C29/func_800D2128.c`,816bytes, SHA256 `27dcdbe55792d42aa4b1bc776a025a5429d15c2fa3ba6a1f0cac73aed505ee47`. Exact first-line flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Canonical stack-sensitive score0 and all204 complete unmasked linked retail words exact, no unresolved/unverified references, relocation errors or extra words. Target SHA256 `dc749d7ccbe3c97e8efb15a66000e4cbe59c864d0af8e0f3b257f4ba664816f3`, privately exported at Rocky `~/agents/C/scratch/game-C29/func_800D2128.target.o`.

Source is reconstructed directly from complete original assembly. Real arguments are f32 value/f12, byte-output pointer/a1, and unsigned format-byte/a2; the original saves that format at8(sp), masks255, and compares ASCII99/102. Value is clamped nonnegative. The c case converts to unsigned, saturates999, writes three numeric digit bytes and a zero terminator, returns3. The f case extracts minutes/seconds from the unsigned integer value and hundredths from unsigned(value*100)%100; it writes six numeric bytes plus zero and returns6. Other formats return0. No ASCII digit offset is added because original output stores numeric0–9. All conversion, quotient/remainder and break guards are genuine compiler output from ordinary C, not hand-modeled hardware or substitute asm. No data definitions, external callee, borrowed context, unused formal, pressure variable or initialization guess is present.

First source compile matches; fresh frozen independent replay also matches all204words. Entire816-byte body is accounted for, including actual branch/division code, without padding. Minimal context is four scalar typedefs. Ready artifact list: func_800D2128.c, verification.json, frozen_verification.json, context_provenance.json, verify.py.

```sh
python3 PACKET/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target PRIVATE/func_800D2128.target.o --output D_C29.json
```

Parent owns normal independent lock/image/full-ROM gates. Shared accepted sources/header/layout/state/locks were untouched.

The initially screened audio_output_setup stays an honest nonmatch. It was discovered to overlap prior A26 exhausted work after seven source controls; that overlap was promptly reported and further controls stopped. Actual typed node offsets and queue APIs yield correct links but best strict678/24of37word differences. No credit or acceptance is claimed. Additional advisory register-storage controls had no effect; one initial broad text replacement made an illegal register struct field and failed compilation, was narrowed to function locals, and the successful controls alone are recorded. Earlier complete-head screening rejected truncated engine_particle_effect/collision_query_sphere, hidden-register DED78 and prior exhausted small wrappers. Those receive no source/ABI claims. All remote jobs are complete.
