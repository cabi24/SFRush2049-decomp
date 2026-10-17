# ReadPathData: exact local body, open timing dependency

Research only. `claims: []`; **zero new accepted bytes**. No canonical integration.

## Result

At base `58425072b49e95f0fbe5c4c239988fb4885f7c5c`, the natural N64
`ReadPathData` (`audio_effect_apply`, 0x800BAAA0) is strict exact: 174 words,
696 bytes, frame 80. All relocations are verified and the extent is exact.
Its actual `CalcCheckpointTimes` callee (`physics_response`, 0x800B93A8)
remains 211/230 words different, 916 compiled bytes against 920 native bytes.
This packet does not convert a local body match into dependency-closure credit.
Ten established exact functions are replayed and preserved in the same group.

The old timing candidate used two artificial `do {} while (0)` block ends.
They are absent here. Its shaped exact result is not counted or included as an
accepted source. The existing canonical route-cross source retains its two
previously disclosed unused scalar locals unchanged. This packet adds no unused
local, dummy caller, forced register, target-byte patch, or artificial timing block.

## Source and identity evidence

Later MAT3 separates `InitCPS` (a 20-byte race-state reset) from
`CalcCheckpointTimes` (the real timing callee). Original GameCube ELF evidence:
ReadPathData 0x8003220C/944 bytes, DIE 0xA6E6D; CalcCheckpointTimes
0x80031C48/1436 bytes, DIE 0xA6BB2. The full donor source and original loader
DWARF supplied six locals in this order: signed-short i, signed-short p,
ResPath pointer tResPath, DronePath pointer tDronePath, char pointer ptr,
signed-short j. Both pointer locals here are meaningful memcpy sources.

The donor's real byte-swap calls establish conversion-loop locations, not N64
swapping. Native N64 instructions retain the counter loops but have no converted
field accesses. The empty native conversion bodies reproduce those evidenced
loops; they are not arbitrary allocator ballast. The original duplicated restart
call chain is retained. The normal path begins after the native 812-byte
checkpoint header. All N64 field widths, offsets, four paths, pointer strides,
and native-endian bytes are preserved. `numAlt > 0` is ordinary unsigned-count
logic; spelling it as a truth test is a one-word code-generation negative with
the same bounded behavior.

The donor checkpoint/path sources were leads with stated reconstruction limits,
not independently resealed per-function matches. Original ELF SHA256:
`d4a06e4516ab9a75e6de780beee10ea07b8358016e4964a8baf9e8eaa282908c`.
Donor pathcommon source SHA256:
`fcbf8a5601adb0c98e96ac92a7c750adcad71e973e3237bd64972ce5a75b79df`.
Donor checkpoint source SHA256:
`9949d21bba5a9910073e793143fc21b226e86fc1edf29a8b2c89c58f89b6b080`.
No original ELF, ROM data, or assembly dump is included.

`path_graph.c` is the canonical frontier_path_graph_links source, unchanged.
`route_cross.c` retains canonical executable bodies; its genuine FindPathDistances
and unsigned route-next helper have named internal-group linkage to prevent
unlabelled deleted-procedure stubs from contaminating a neighbor's extent.
These helpers are not new native identity claims. The signed canonical
func_800B930C contract is unchanged. `bounds.c` is the exact PR380 candidate.
`ctx.c` preserves the existing NextPathPt body, with redundant generated header
removed. Historical narrow context views (including Tracker.idx[17] in the
canonical route-cross source) are inherited, not validated as a new full-game
C type/aliasing proof here.

## Bounded proof

- Observation-only traced uopt reproduces the complete stock object byte for
  byte; the source-bound trace receipt records that identity and no forced colors.
- Two fresh normal builds agree across all allocated code/data and relocation
  sections; all twelve scored function roles are sealed by target hashes.
- Five fresh builds replay natural, repeat, truth-test, wrong-header, and
  wrong-path-count controls. The latter two are rejected by an independent
  packed-data/event model, not only matching scores.
- IDO-emitted constants verify sixteen layout/type facts. Three genuine type
  mutations are rejected. No warning-only negative-array typedef assertion.
- 160 deterministic fixtures cover 128 parses, 32 nonzero signed restart flags,
  zero through 255 alternate paths, and zero/nonzero drone counts. Native and
  compiled MIPS are separately executed against a field/offset model, covering
  all 174 root instructions. memcpy copies bytes; the four gameplay callees are
  call-boundary stubs. GPR, FPR and SP restoration is checked, including the
  internal timing callee's loader-owned saved-register convention.
- The model assumes trusted native-endian packed buffers with in-range counts,
  enough storage, and correctly aligned four-byte header/alternate objects.
  Fixtures keep total path counts below signed-short overflow. Invalid buffers,
  callee side effects, timing semantics, full-game runtime and integration are
  not proved. The model does not execute the unmatched timing helper.
- Guards use explicit exceptions and are tested under Python `-O` for false
  claims, source/script/model/target changes and unverified relocations.

Run with the existing IDO/binutils toolchain and protected artifacts:

    python3 cloud/work/frontier/dot_mat3_readpath_20261017/verify.py
    python3 -O cloud/work/frontier/dot_mat3_readpath_20261017/verify.py
    python3 -m pytest tests/cloud/test_mat3_readpath_20261017.py

The verifier accepts a repository path argument for an isolated packet against
another unchanged checkout's scorer/protected targets. The focused test accepts
`N64_RESEARCH_REPO` for that same supported external-repository replay.
