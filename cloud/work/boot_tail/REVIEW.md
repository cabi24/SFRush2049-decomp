# Packet 1 verification and review

Base: `b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`.
Scope: `cloud/work/boot_tail/**` and D10 only. No new accepted or matching bytes.

## Local checks

- 21 focused standard-library tests pass, including deterministic regeneration,
  exact 412-row / 95,012-byte reconciliation, half-open disjoint intervals,
  call-graph metrics, annotated-state candidate filtering, structural-field
  protection, reverse-edge validation, and negative drift/cut/overlap cases.
- `generate.py --check` passes; repeat generation is byte-identical.
- `screen_opcodes.py --check` passes against the tracked target-file hash.
  Zero in-scope privileged-opcode-positive bodies; one identify-only `mtc0`
  stub; nine out-of-census calls. Destinations remain unread/unclassified.
- The matching queue excludes the existing 12-byte getter and explicit blockers.
- `source_hashes.json` binds native/static observations to tracked source files.
- All 161 existing static locked-function records pass the read-only lock check.
- `git diff --check` passes. Only permitted paths are edited.

## Independent review

Independent reviewer reproduced the complete ledger, extents, cluster member
sets, edge counts, all focused tests and both regeneration checks. It independently
validated the MusyX macro interpreter, random/volume source anchors, CC0 license,
static printf/string path, and nine out-of-census calls. Its mutation probes led
to hardening annotation structural keys, final-status candidate filtering and
reverse-call validation; those probes now pass.

Final independent review: **PASS, RESEARCH-ONLY**, 2026-10-03. Technical
snapshot: local commit `a58e7bc9bde1eea84408488dfb16656e0af61651`, tree
`beb7b4e4bdf0d78242ad820dc1113a8c90cb86b3`. All 21 tests, both
regeneration checks, protected-path guard and base-to-head whitespace check
were independently rerun. All eight source leads were validated at their
stated depth. Publication and exact-head CI remain separate checks.
Receipt-only approval/PR/CI bookkeeping may follow this snapshot.

## Deliberately not run

No compiler setup, function compile, score, native matching, full local repository
regression, production image/compression/ROM gate, farm job or game test. Packet 1
is metadata/source-identification work. Required repository CI remains a separate
exact-head publication check; green research CI cannot establish fresh native-body
or cartridge proof. Packet 2 must separately perform its formal toolchain/getter
replay checks before matching starts.

## Outstanding scope/evidence limits

Original translation units and the precise N64 middleware version are unproved.
The printf family is evidenced in counted-static code, outside the tail. Generic
matrix math or queue/cache use does not establish F3DEX/PI identity. Tiny size or
absence of privileged instructions does not establish C representability.
Specific at/beyond-end call destinations need maintainer classification before
affected matching; no protected input may be changed by this packet.
