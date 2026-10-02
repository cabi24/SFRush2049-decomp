# Game packet C25 — canonical scalar vector helper

One frozen ready ordinary claim so far: func_8008B424,80bytes. Exact flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Source `game_C25/func_8008B424.c` SHA256 `664db61fb4774987464f508b30983e10157c46ae847634478876d92d0fb74ca5`. Current actual blob locks were checked before claiming it; the nearby 8B3C8/8B3F4 helpers are already accepted and are excluded from new credit.

Canonical stack-sensitive score0 and complete unmasked linked20 retail words0, with no unresolved/unverified references, relocation errors or extra words. `verification.json` records exact compile command, flags and source/target/object hashes. Actual supported target SHA256 is `3946bb49871107c5a23a9b7240f68bb393b6164d3669e633ce5a6e470ccd15c8`, privately exported at Rocky ~/agents/C/scratch/game-C25/func_8008B424.target.o; final object is alongside it.

Real callee assembly proves one vector pointer in a0, three f32 loads, a lower bound using original named D_80123888, intrinsic scalar sqrt and reciprocal f32 result in f0. It has no other input formals. The direct source uses that same sum/clamp/sqrt behavior and original operand order; the first canonical compile already matches. No invented constants, initialization, storage ownership, pressure variables or synthetic caller was used. Entire80-byte function is accounted for, without slot padding.

Minimal context is typedef float f32, original threshold declaration and standard IDO sqrtf intrinsic declaration/pragma. Parent owns independent replay, normal lock/image/ROM gates and final credit.

```sh
python3 PACKET/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target PRIVATE_TARGET/func_8008B424.target.o --output D_C25.json
```

Other selected ordinary sources are ongoing nonmatches: vector_normalize_length, E114C and camera_update_d. Their real callee contracts and missing intrinsic declarations are corrected privately. AB18C is blocked on the seed's synthesized cfc1 conversion-error placeholders and will not be accepted without recovering the actual unsigned-cast source operation. No shared source/header/layout/locks/state changes were made.
