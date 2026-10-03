# Tasks: 015 boot-segment tail runtime library

Read [spec.md](spec.md) first. Packets are dependency-ordered. Claim each one in
`dot_handoff.md` (packet **D10**) before you edit, using the PROJECT_PLAN §5 claim block:

- packet ID and state;
- owner;
- branch and base SHA;
- exact address intervals;
- files you will edit;
- stop condition.

Hold one active packet and at most one queued packet. Cluster packets must have
disjoint address intervals.

Inputs on master: [inventory.json](inventory.json). It has 439 rows, and `scope`
marks the 412 `in_scope` functions. Each row carries:

- address, ROM offset, size and size band;
- historical `symbol_addrs_name` and the ultralib result;
- start evidence;
- `called_from_game`;
- `callees_tail`, `callers_tail` and `callees_static`;
- `hilo_refs` (lui/lo16 values outside the tail text).

The call and reference columns come from a plain jal and lui/lo16 scan. They are
clustering leads, not proofs.

Seed leads (all **HYPOTHESIS**, from that scan):

- `0x80011104`, `0x800114C0`, `0x80014198` and `0x8001B9F8` reference addresses inside
  the `N64 RSP mixer` microcode/data block at `0x8002D890–0x8002D8D0`. They are
  candidates for the audio driver or RSP task setup.
- Ten functions in `0x80024FD4–0x80025DC0` share the data words `0x8002D480`/`0x8002D484`.
  They are a candidate single translation unit.
- No function reaches the `vsprintf` strings (`0x8002D4F8–0x8002D53C`) through a direct
  lui/lo16 pair. Find the path: a pointer table, a base register plus offset, or a
  string passed in from a caller.
- `0x8000F8D0` (`bcmp` label) and the 6 `stub_unclassified` tiles are non-C candidates.
- The 21 `called_from_game` rows are the public API surface. They are the best anchors
  for names.

---

## Packet 1: orientation, identification and clustering (P1, start now from master)

Needs no targets and no compiler. Status: READY-RESEARCH.

- [ ] T001 Create `cloud/work/boot_tail/README.md` and `STATUS.csv` with one row per
  in-scope function, generated from `inventory.json` by a small committed script
  (`cloud/work/boot_tail/scripts/`). Set every row to `status=open`, except
  `func_80010A00` (`verified_body`, already matched on PR #54).
- [ ] T002 Build the call graph and address-contiguity clusters from `inventory.json`.
  Choose clusters that are contiguous address runs whose calls stay mostly inside the
  run. Give each cluster an ID (`BT01`…) and an interval. Commit the script and its
  output (`clusters.json`).
- [ ] T003 Classify the role of each cluster. Use:
  - shared `hilo_refs`;
  - calls into counted static code (resolve `callees_static` against
    `symbol_addrs.us.txt`, for example osSendMesg, osRecvMesg or osPiStartDma);
  - hardware-register constants (`0xA4xxxxxx` in `hilo_refs`);
  - the data strings.

  Expected families are the libc printf/vsprintf core and helpers, string/memory
  routines, thread/message/event services, PI/DMA and cartridge transfer, the audio
  driver for the mixer microcode, and graphics task submission (F3DEX). Label each
  cluster HYPOTHESIS or SOURCE-LEAD.
- [ ] T004 Compare against public references and cite each with path, licence and
  revision:
  - decompals ultralib, for structure only (the census already excludes exact matches);
  - libreultra or other licensed SDK-adjacent code;
  - public libc printf implementations (for example the format-switch shape of
    `_Printf`).

  Vendor nothing that lacks a compatible licence. For the arcade source, write a
  precise `EXTERNAL-INPUT` lookup request (function role and the strings or constants to
  search for) for the maintainer.
- [ ] T005 Flag `non_c` candidates: hand-written assembly, `cache`, `mtc0`/`mfc0`,
  `eret`, `sync`, non-IDO delay-slot use, or leaf stubs that IDO cannot produce. Give
  the reason for each.
- [ ] T006 Write per-cluster name hypotheses into `STATUS.csv` (`name_hypothesis`,
  `evidence_level`, `cluster`). Write a cluster table into the README, ordered by
  recommended matching order (small, self-contained, few callers first).
- [ ] T007 Open a research PR (`dot/boot-tail-p1`) that touches only
  `cloud/work/boot_tail/` and the D10 claim line in `dot_handoff.md`.

**Exit:** SC-001. Every in-scope row has a cluster and a label. Totals reconcile to
412 functions and 95,012 B.

## Packet 2: target and toolchain check (P1, after PR #52 and #54 merge)

- [ ] T010 `bash tools/cloud/setup.sh`. Then confirm
  `sha256sum -c asm/us/boot_tail/SHA256SUMS` (run in that directory) and a
  `score.py ... --targets asm/us/boot_tail` MATCH on the existing
  `cloud/matches/boot_tail/func_80010A00.c`. **Stop** if either fails.
- [ ] T011 Cross-check `extents.json` against `inventory.json`: same 439 starts and
  sizes. Record any drift in the README and stop on drift.

## Packet 3: small-function sweep, under 64 B (P1)

Covers 97 in-scope functions. Excluding `func_80010A00` and `non_c` rows leaves the
open list.

- [ ] T020 Per function, write natural C. m2c output is acceptable as a starting point
  if it is cleaned up. Score at `-O2`, then `-O1`, and record the native signal for
  the chosen level.
- [ ] T021 Commit each MATCH as `cloud/matches/boot_tail/func_XXXXXXXX.c` with the
  line-1 flags header. Update `STATUS.csv` (`verified_body`, flags, source path).
- [ ] T022 Rows that do not match within the bound get `nonmatch` and a note. Rows that
  are not C get `non_c`.
- [ ] T023 Batch PRs by cluster (about 10–30 functions each). Each PR must pass CI
  rescoring and touch only allowed paths.

## Packet 4: 64–255 B sweep, by cluster (P1/P2)

Covers 211 functions.

- [ ] T030 Take clusters in packet 1's recommended order. Declare each cluster's shared
  types once per source file, and keep a cluster's sources consistent.
- [ ] T031 Same scoring, commit and status rules as T020–T023.
- [ ] T032 For each near-match, run `tools/workbench.py diagnose` and record the
  residual class (schedule, frame, temp ring or allocation) before any further
  variant.

## Packet 5: 256–1,023 B, by cluster (P2)

Covers 96 functions.

- [ ] T040 Reconstruct whole bodies, preferring cluster callers or callees that are
  already matched for type evidence. Functions with switch tables or float literals
  must show that the scorer resolved every table and literal relocation. Otherwise mark
  them `needs-rodata-proof` and escalate.
- [ ] T041 Archive each nonmatch under `cloud/work/boot_tail/<cluster>/` with source,
  flags tried, differing-word count, rejected controls and one next hypothesis.

## Packet 6: 1 KB and larger (P3, last)

Covers 8 functions: 1,084 to 3,840 B, including `0x800268D0`.

- [ ] T050 Do these only once their clusters' small and medium members are matched.
  Write region-by-region reconstruction notes first (control flow, switch targets,
  call sequence and frame layout), then one complete body. A partial body is labelled
  PARTIAL-SOURCE and is never submitted to `cloud/matches/`.

## Continuous

- [ ] T090 After every PR, update `STATUS.csv` and the README totals: verified functions
  and bytes, nonmatch count, `non_c` count and open count. Keep verified bytes separate
  from research, and do not report any cartridge-coverage percentage.
- [ ] T091 Update the D10 entry in `dot_handoff.md` with the claim and state, the last
  PR, the blocker and the next packet.
- [ ] T092 Report ultralib or libc variants missed by the census, extent defects and
  required shared-header changes as maintainer requests in the README. Do not apply
  them.

## Allowed paths (every packet)

- `cloud/work/boot_tail/**`
- `cloud/matches/boot_tail/*.c`
- the D10 section of `dot_handoff.md`

Anything else is a stop-and-escalate condition (spec "Out of scope").
