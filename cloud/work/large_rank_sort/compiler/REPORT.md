# func_800F7F3C independent compiler packet

Not accepted. Target `0x800F7F3C`, 349 words / 1,396 bytes. Ordinary zero-argument
leaf; retail saves all three used callee registers s0–s2 in a 16-byte frame.
No calls, floating-point operations, jump tables, hidden inputs, or IPA ABI.

Native signed-byte rank IDs map through signed-short model indices at offset
1990 of 2,056-byte records. Score entries are 120 bytes with a signed word at20;
player records are 952 bytes with signed bytes at238 and931. Mode4 sorts score
in descending order. Mode6 sorts byte931 descending, then uses the real
`D_8012E67C` byte mapping and `D_8015256C` byte priority table. Other modes sort
byte238 ascending. Each mode detects ties with its leader and updates real
`D_80150B60` count and `D_80150B68` flags without clearing them.

The old archived source had two semantic errors: ascending fallback comparison
was reversed, and its secondary mapping used wrong address `D_8012E77C`.
Both are corrected in `native_semantics.c`. Unread n/last/p/q declarations and
a redundant initialization were removed before baseline controls.

The concrete native structure gains were:

1. Capture the actual competitor profile index in existing signed-short ib
   before comparing both scores: 334 → 343 aligned opcodes.
2. Increment the actual tie count before assigning its flag, matching retail's
   early count load and later stores: 343 → 349 aligned opcodes.

The best full source is `native_semantics.c`, SHA256
`73f6bf0ca04b032617ac54b150f0d6f489343e96861f1583b27afa187663e90e`.
Flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
It emits all349 executable words, frame16, no extra nonzero words and no
unresolved/unverified/error evidence. Strict linked comparison still differs
in270words, all register operands. Workbench confirms zero opcode differences
or alignment gaps; this is not a match. Source O3 baseline was unchanged.

A genuine pointer carrier inside unchanged indexed bounds worsened geometry.
Fresh complete m2c source with all actual captures emitted460words under both
O2/O3; no match. Bounded module/debug ownership controls also failed: O1 gives
508words; g2/O2 gives564words; g3/O2 retains the extent but only345aligned
opcodes. No broad declaration or flag sweeps were used.

Protected target SHA256:
`79b9ecc0871a23fca245e73e0ed975f40a318812621c5a7a6f6e2fd7259cc29f`.
Toolkit SHA256:
`796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`.
Fresh tools-only Rocky harness: `~/agents/large-rank-compiler`; candidates and
results started empty. Raw objects/instructions remain under ignored build.
All sanitized comparisons are in `control_results.json`. No locks, source
providers, image/ROM gates, commits, pushes, or tests were changed or run by
this worker. Current accepted coverage remains13.97% game and46.76% static.
