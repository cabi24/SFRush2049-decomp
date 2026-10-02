# Game C30 — canonical affine matrix packer

Frozen ordinary source `game_C30/func_8009E8B4.c`,292bytes, SHA256 `494644cc85913d801df8404bbe8b2fee1a3766b07f842a0e02a57bde45012121`. Exact first-line flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Canonical stack-sensitive score0 and all73complete unmasked linked retail words exact, no unresolved/unverified references, relocation errors or extra words. Target SHA256 `c30bd90acb7fc203257ff0b2c549e86704728e8d3c48914b9c49ec5ef3323999`, privately exported at Rocky `~/agents/C/scratch/game-C30/func_8009E8B4.target.o`.

The real two-pointer leaf packs three input rows with a16-byte float stride into the standard64-byte fixed N64 matrix layout. Each first pair uses signed float-to16.16 conversion, placing its integer high16 halves in the first output half and fraction halves in the second half32bytes later. The third converted component is paired with a zero fourth component; final affine row is0,0,0,1. The local SDK guMtxF2L body proves the fixed layout and packing operation; original target assembly proves this genuine affine specialization. No extra inputs, borrowed callee context, data definition, dummy work, stack pressure or anonymous constant mapping was introduced.

Initial source tracked independent integer/fraction output pointers and differs70/73words with5extra words. Four directed meaningful controls tested canonical masks, signed pointer view, flat float input and fixed parallel output offsets. Parallel offsets reduce to only4word differences and1extra instruction. Explicit stores of the actual final affine row remove that unnecessary pointer advancement and give strict0/all73words exact. Fresh frozen replay confirms the identical final source. These stores represent required matrix output; no runtime operation was added solely to force code or padding. Entire292-byte extent is accounted for.

Minimal ready artifact list: func_8009E8B4.c, verification.json, frozen_verification.json, context_provenance.json, verify.py. Replay:

```sh
python3 PACKET/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target PRIVATE/func_8009E8B4.target.o --output D_C30.json
```

Current actual lock eligibility and full prior reports were checked first; r5_f had explicitly left this target unattempted. Context is three scalar typedefs, with no named data/callee/header dependency. Baseline and four directed controls are retained separately. Parent owns independent normal lock/image/full-ROM gates. Shared accepted paths and coordinator state remain untouched. No remote jobs remain active.

Root whitespace review: only trailing whitespace on the otherwise blank declaration line was removed; physical line boundaries and all C tokens preserved. Fresh independently compiled frozen replay remains strict0/all73words exact.
