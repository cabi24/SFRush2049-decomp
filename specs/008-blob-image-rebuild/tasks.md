# Tasks: Game-Code Image Rebuild (blob stage 1)

**Input**: `/specs/008-blob-image-rebuild/` (spec, plan, contracts).
**Baseline** (2026-09-24): image 647,072 bytes at `0x80086A50`–`0x801249F0`;
912 gate-passed functions covering 514,324 bytes (79.5%); 213 interior gaps
(47,612 bytes) + 85,136-byte tail; 76 byte-identical matches (5,820 bytes).

**Tests**: included per task; the image gate is itself the acceptance test.

---

## Phase 1: Map

- [x] T001 Preflight: verify `build/game_code.bin` reproduces from the ROM
      (`tools/extract_game_code.py`) and record its sha256; confirm the raw
      DEFLATE stream at `0xB0CB10` (326,180 bytes) inflates to it exactly.
      Correct CLAUDE.md's image end (`0x80124AF0` → `0x801249F0`).

- [x] T002 [P] Tests first in `tests/conveyor/test_blob_layout.py`: complete
      coverage (every byte in exactly one region), `extent_conflict` rows
      excluded, opaque runs identified, deterministic output across two runs,
      region splitting at large gaps, per-TU flagset carried.

- [x] T003 Implement `pipeline/blob_layout.py` (`derive`, `report`) emitting
      `build/blob_layout.json`. Make T002 pass.

- [x] T004 Live derive ×2 on the Pi; record actuals in `quickstart.md` §1
      (region count, functions per region, opaque bytes, determinism).

**Checkpoint**: the image is fully described and the description is stable.

---

## Phase 2: Translation units

- [x] T005 [P] Tests first in `tests/conveyor/test_blob_tu.py`: a generated TU
      is all-passthrough, `.incbin` slices name the right offsets and lengths,
      the linker script places `0x80086A50`, regeneration is byte-identical,
      and a TU never contains a function marked `extent_conflict`.

- [x] T006 Implement `pipeline/blob_tu.py` (`generate`) writing `src/blob/
      blob_<vaddr>.c`, `src/blob/blob.ld` and a Makefile fragment. Make T005
      pass.

- [x] T007 Implement `pipeline/blob_build.py` (`build`): rsync the TU set to
      the builder, compile with IDO + asm-processor, link, `objcopy` the
      loaded range, compare sha256 against `build/game_code.bin`. Refuse on
      any difference, naming the first differing offset plus the region and
      function that own it (FR-010).

- [x] T008 **Gate (SC-001)**: all-passthrough build on the builder produces a
      byte-identical image. Record the actuals; if it fails, report the first
      differing offset and stop — do not hand-patch the TUs to force a pass.

**Checkpoint**: the image rebuilds from checked-in sources.

---

## Phase 3: Splice

- [x] T009 [P] Tests first in `tests/conveyor/test_blob_splice.py`: splice
      replaces exactly one passthrough, rollback leaves no partial edit, the
      provenance lock records target/score/flagset/toolkit/source-hash, and a
      body whose source hash drifts is refused.

- [x] T010 Implement splice/revert in `blob_build.py` (`splice <target>
      --from <path>`, `revert <target>`), gated on image hash, committing only
      on success, writing `blob_matched.lock.json`. Make T009 pass.

- [x] T011 Splice one matched function end to end; verify image hash unchanged
      and coverage +1. Then `splice --all-matched`.

- [x] T012 **Gate (SC-002, SC-005)**: ≥50 of the 76 matched functions spliced
      with a byte-identical image; each refusal reported with its reason
      (extent conflict, unresolved data reference, flagset mismatch). Revert
      one and confirm the prior hash returns.

**Checkpoint**: verified C is linked into the image.

---

## Phase 4: Drill, report, document

- [x] T013 **Gate (SC-003) — corruption drill**: alter one instruction in a
      spliced body, confirm the build fails and names the offset, restore.
      The 004 drill found a hash gate that had been vacuous for months; this
      gate is not trusted until it has failed on purpose.

- [x] T014 Coverage reporting (FR-008, SC-004): image coverage (functions and
      bytes of 647,072) reported separately from ROM coverage, with the
      cartridge caveat in the output. Wire into `make progress`.

- [x] T015 [P] Ops docs: 008 section in `tools/conveyor/README.md` (derive →
      generate → build → splice, builder requirements, what the gate proves
      and what it does not), plus a CLAUDE.md note that image coverage is not
      cartridge coverage.

- [x] T016 Full local suite green; quickstart actuals complete; close-out
      scorecard naming any SC not met, in the 006/007 format. Update the wiki
      status page.

---

## Dependencies

```
T001 → T002 → T003 → T004
         └→ T005 → T006 → T007 → T008        (builder required from T007)
                              └→ T009 → T010 → T011 → T012
                                           └→ T013 → T014 → T015 → T016
```

- **MVP**: T001–T008 (the image rebuilds). T009–T012 make it useful.
- **Builder dependency**: everything from T007 on needs watchman2 and IDO.
- **Stop rules**: T008 and T012 stop rather than force a pass; a failing gate
  is the finding, not an obstacle to route around. Never bypass the pre-commit
  hook.
- **Out of scope throughout**: the compressed stream (stage 2), cartridge
  promotion, any change to how functions are matched.

## Task counts

Total: 16 (Map 4, TUs 4, Splice 4, Polish 4).
