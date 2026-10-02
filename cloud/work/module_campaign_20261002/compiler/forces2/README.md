# Independent forces2 verification

Verified ordinary O2 complete `func_800E1C30`, 848 bytes / 212 words. Frozen source: `../../reconstruction/forces2/forces2_native_body_sum.c`, SHA256 `75fa8e12c16df3f7ec17b94bdf42cd7bba0adc01564ff6ddf13da971589ac36d`.

Direct ancestor: `rushtherock/game/drivsym.c:forces2`, with the original `vecmath.h:vecadd` operand ownership. It sums tire, gravity, drag, body contact and center forces. Native adds selective rear thrust blending, gravity scaling, and body contact magnitude. Y is vertical and Z is longitudinal in the native axes. The original body-force operand order fixes all seven remaining FP register/commutative instruction differences; the workbench was run on the preceding seven-word residual before the winner.

ABI audit: one actual Model pointer in a0; 56-byte frame saving ra only. The model's vectors are three floats, and basis is nine floats. The local body-contact vector contains three genuinely consumed floats. The transform receives three ordinary pointers; magnitude receives one vector pointer and returns float. No dummy formals, helper bodies, local capacity, pressure storage, retained unused donor declarations, or assembly emission is present.

Independent isolated IDO compile used `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`, toolkit `796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`. Fresh current database protected target object SHA256 is `e44990b068ae728eba270aca60b69cd17a12ce537793bd06f807b03ac16b99d2`. The unchanged strict scorer returns zero. Full protected relocation proves all 848 compiled body bytes equal, with no masks, unresolved/unverified sites, errors, or excess words. Compiled/native body SHA256: `0bb385d718a988715ca584ce9c092ac3100911430baf168641515337d1532ece`.

Receipt: `independent_replay.json`. Source and proof are frozen. Root owns source splice, image and final ROM gates; this receipt alone makes no cartridge hash claim. No accepted source, target, scorer, masks, locks or main tooling was modified by this verification lane. No tests were added or run.
