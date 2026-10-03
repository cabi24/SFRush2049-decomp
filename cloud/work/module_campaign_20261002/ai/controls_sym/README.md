# Controls and sym module research

Frozen nonmatching research. No matching credit or source promotion.

## Source and real module boundary

`func_800E3430` (756 bytes) implements controls; `func_800E3724` (600 bytes) implements the physics sym wrapper. Direct related arcade ancestors are `game/controls.c:controls()` and `game/drivsym.c:sym()`. The previous complete A85 packet was audited first. Its indexed wheel capture and unrelated scalar peak arrays were replaced by the genuine Tire92 cursor, direct angular velocity reads, BODYFORCE[4][3] and peak[2][3] arrays. Field roles are corrected: offset1588 is dt,1592 is idt,1816 is fast input modeltime. Actual clutch/brake/throttle/gear inputs and N64 offsets are preserved.

Only sym is kept by the real O3 linker. `actual_callers.json` proves that controls has exactly one native direct caller (sym), and sym has two call sites in the real model-update caller. Controls deliberately clobbers four FP pairs without saving them; sym preserves those pairs. Keeping the private helper as a separate public entry would impose a different ABI. No invented keepers or formals are used.

Native differences from the arcade code govern the reconstruction: the native brake rolloff is zero at low speed and uses0.05 rather than the arcade0.1 coefficient; finish/mode predicates and callback order follow the native body. Removed steering feedback/game-over blocks and unused arcade locals were not recreated. Donor copyright notices are retained; this packet asserts no additional license grant.

## Independent compile result

Compilation uses the complete two-source IDO O3 pipeline in `compile_recipe.json`, fixed toolkit796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5, and `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

| Member | Native words | Generated section words | Aligned opcodes | Aligned opcode/register | Diagnostic differing | Frame |
|---|---:|---:|---:|---:|---:|---|
| controls |189|183|177|145|163|private leaf, no frame|
| sym |150|153, including trailing padding|142|123|113 plus1 nonzero extra|56, exact|

Removing the inherited local angular-velocity capture and reading the actual Tire92 field directly restored the exact native56-byte frame and four saved FP pairs. The initial donor-array control saved five pairs in64 bytes. Helper aligned exact words improved104→136 and opcode/register lanes113→145. This is useful structural evidence, with extensive remaining body differences. No allocator or flag sweep followed the bounded donor controls.

## Literal evidence and proof limit

The source-built pool begins with the exact six native floats, in order:0.7,0.33,0.99,0.05,9.549305,0.9. All24 bytes equal the retail pool at0x801243F0. This proves values and ordering only. The protected whole-group resolver refuses the shortened helper and overlong wrapper before data placement; accordingly `placement_verified` remains false, and all12 HI/LO section-relative references remain unverified. Diagnostic comparison excludes those sites and is not a strict acceptance result. No resolver, score masks or flags were changed to bypass this refusal.

`proof.json` records source and object hashes, all comparison errors/unverified sites, alignment evidence and the pool limitation. `provenance.json` records donor hashes and the earlier packet. Raw objects, disassembly and literal bytes remain in ignored build storage. No accepted source, lock, target, build infrastructure, tests or commits were changed by this lane.
