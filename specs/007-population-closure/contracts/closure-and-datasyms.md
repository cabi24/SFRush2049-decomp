# Contract: Callee Closure & Generated Data Symbols

Consumers: `pipeline/closure.py`, `pipeline/datasyms.py`, `disasm.py`,
`protos.py`.

## Closure pass (normative)

1. **Discovery**: for every extracted target with a gate-passed extent
   (not `extent_conflict`/scan-failure), decode `j` (opcode 2) and `jal`
   (opcode 3) from the raw extent words; absolute target =
   `((word & 0x3FFFFFF) << 2) | (pc & 0xF0000000)`. A discovered address
   is a **candidate** iff it lies in `[0x80086A50, blob_end)`, is
   4-aligned, and resolves to no `n64_target.address` (either population).
   Static-range targets (< blob base) are never candidates (declaration-
   layer business).
2. **Gate**: each candidate runs the 005 extent scanner
   (`contracts/extent-repair.md` in specs/005, as amended — `pc >=
   furthest`, delay-slot inclusion, jump-table non-termination, 16 KiB
   cap). Outcomes, exactly one per candidate:
   - `registered`: scan succeeded and the extent does not start inside an
     existing registered extent → insert `n64_target` row
     (`target_id=func_<addr8>`, `population='extracted'`,
     `tier='raw_word'`, `gate_reason='discovered'`), carve words, build
     object, store sha — identical code path to 005 registration.
   - `inside_existing_extent`: address strictly inside a registered
     extent → record, do not register.
   - `scan_failure`: overrun/decode failure → record, do not register.
   - `invalid`: misaligned or out of image → record.
3. **Fixpoint**: newly registered targets are scanned for candidates in
   the next iteration; terminate when an iteration registers zero.
   Caps: 10 iterations, 2000 total registrations; hitting a cap is an
   explicit report outcome (`cap_hit`), never silent truncation.
4. **Report**: `build/closure_report.json` — per-candidate outcome with
   discovery provenance (first discovering `target_id` + instruction
   vaddr), per-iteration counts, totals, caps. Deterministic ordering.
5. **Idempotency**: an immediate second run discovers the same candidates
   already registered ⇒ zero registrations, zero evidence changes.
   Supersession fires only for a target whose own extent/object changed
   (never as a side effect of registration).
6. **Naming**: `func_<addr8>` placeholders; target identity is the
   address; renames are out of scope.

## Generated data symbols (normative)

7. **Scan**: over the derived asm of the enlarged population, collect
   every formed effective data address the symbolizer idioms recognize
   (direct `lui`+access, `lui`+`addiu` formation, idiom (c)
   `lui`+`addu`+access) that is present in **no** table (hand
   `GAME_SYMBOLS`, prior generated layer).
8. **Types**: per address, the widest observed access wins
   (`word > half > byte`); an address with only FP accesses types `f32`
   (`f64` for `ldc1/sdc1`); a same-width integer/FP conflict records the
   conflict and types integer. Each entry cites its deriving accesses
   (`target.s` + `.L<vaddr>` + mnemonic).
9. **Emission**: `build/m2c_datasyms.json` — `{address: {name:
   "D_<ADDR8>", type, accesses[], conflicts[]}}` + stamp (image sha, hand
   table sha, derivation version). Byte-stable, sorted, regenerable;
   never hand-edited.
10. **Consumption**: `disasm.py` performs ONE merged lookup — hand table
    wins on address collision (generated entry recorded as omitted).
    `symbol_table_sha()` must cover hand + generated content so every
    cached derivation regenerates when either changes. All symbolizer
    safety rules (all-consumers-same-symbol, no mismatched `%hi/%lo`,
    synthetic lui re-emission, invalidation on conflicting writes) apply
    to generated names identically.
11. **Externs**: typed externs for generated symbols are emitted by the
    generated declaration artifact (`protos generate` datasyms section),
    subordinate to hand context under 006's precedence-by-omission; they
    never enter `game_types.h`.

## Acceptance oracles

- Closure run 1 registers ≥ 135 targets (the known-missing set at
  baseline) or explains the difference in classifications; run 2
  registers 0.
- After layer regeneration: zero `func_<addr>` histogram blockers whose
  address is in-blob; the `x<addr>` blocker class is eliminated.
- Datasyms double run: byte-identical JSON (stamp timestamp excluded);
  zero entries colliding with hand-table addresses.
- Full suite green; 005 SC-005 static body-identity guard green; the six
  bucket invariants hold over the enlarged population.
- Unit-test obligations: jal/j decode correctness (incl. the
  `pc & 0xF0000000` page rule at region edges), candidate filtering,
  each closure outcome class, fixpoint termination + caps, idempotency
  against a fixture DB; width rule incl. FP and conflict cases; merged
  lookup precedence; cache-key coverage of the generated table.
