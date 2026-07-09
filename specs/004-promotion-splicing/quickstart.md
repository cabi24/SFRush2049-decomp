# Quickstart: Promotion Splicing end-to-end

Order is the rollout: skeleton → one promotion → rollback drill → batch.
Matching builds run on watchman. Record MEASURED numbers in place.

## 0. Preconditions
```bash
python3 -m pytest tests/conveyor -m "not node_required" -q   # green
python3 -m tools.conveyor.pipeline.lock check                # 12 intact
ls tools/asm-processor/ && git -C tools/asm-processor log -1 --format=%h  # vendored+pinned
ssh watchman 'cd ~/projects/rush2049-decomp && make test'    # baseline ROM matches
```

## 1. Layout map (SC-001 prep)
```bash
python3 -m tools.conveyor.pipeline.layout derive && python3 -m tools.conveyor.pipeline.layout report
```
Expect: 88 static segments listed; the 0x8800 segment derives cleanly (no
refusal); dynamic range marked unpromotable. **MEASURED (2026-07-09)**:
clean/refused = **87/1** — all 87 code subsegments derive; the only refusal is
the synthetic dynamic-population entry (FR-011). 0x8800 derives clean: strchr
(64b) / strlen (40b) / memcpy (56b), flagset `-g0 -O2 -mips2 -G 0 -non_shared`
joined from the lockfile; alignment nop padding absorbed into function sizes,
non-zero data gaps would refuse (none in the .text segments — splat auto-links
rodata separately). `layout derive`/`report`/`coverage` all work; map hash
stable and deterministic.

## 2. Walking skeleton — convert 0x8800, prove hash-neutral (SC-001)
```bash
python3 -m tools.conveyor.pipeline.layout convert 0x8800
rsync + ssh watchman make test        # SHA-1 must MATCH with zero promotions
```
**MEASURED (2026-07-09) — BLOCKED, STOPPED per HANDOFF rule 1 + settled-research
rule.** `layout convert` depends on `make extract` (splat re-split) to emit the
per-function `nonmatchings` asm and regenerate the linker script (D2/D3; rule 5
forbids hand-editing generated files). **splat cannot run against this repo's
current inputs**, so conversion, the build, and the SHA-1 gate could not be
reached. Evidence (each a hard splat error, fixing one exposes the next):

1. `symbol_addrs.us.txt` has 158 comment lines with `arcade:file:func`
   provenance tokens → splat "Too many ':'s" (each space-separated comment token
   with a colon is parsed as an attribute; >1 colon is fatal).
2. `type:data` (490 lines use colon attrs incl. type:data) → splat rejects it:
   valid types are only `func/jtbl/jtbl_label/label`, spimdisasm types, or
   uppercase struct names. `data` is not one.
3. Duplicate addresses (e.g. `__osMotorProbe`/`__osPfsRequestType` both at
   0x80037AE0) → "Duplicate symbol detected".
4. Duplicate names (e.g. `render_state_init` twice — the same game-code name
   collisions found in 003) → "already been defined".

Even a fully-sanitized symbols file (strip freeform, drop `type:data`, dedupe)
would make splat re-disassemble differently from the checked-in asm (different
splat/spimdisasm version + dropped type info), so re-split cannot reproduce the
existing asm — i.e. the hash-neutral baseline SC-001/FR-003 requires cannot be
established through the sanctioned path. The checked-in `asm/us/*.s` predate a
splat-consumable `symbol_addrs`. See tasks.md for the reviewer decision needed.

## 3. One real promotion + rollback drill (SC-002, SC-003)
```bash
python3 -m tools.conveyor.pipeline.promote run 0x8800:strlen --from src/libc/string.c --via-builder
# then the drill: promote memcpy --from <a deliberately wrong body>; expect
# refusal, git status clean, make test still exact
```
**MEASURED**: strlen ________; drill ________

## 4. The batch (SC-004) + coverage (SC-005)
```bash
python3 -m tools.conveyor.pipeline.promote batch --locked
make progress | grep -i linked        # >0 functions, >0 bytes, derived
python3 -m tools.conveyor.pipeline.lock check && lock verify
```
**MEASURED**: promoted __/12; coverage ________; locks ________

## 5. Conveyor path (SC-006) + suite
One promotion driven through the upgraded verify_promote job on the builder;
then full local suite green. **MEASURED**: ________
