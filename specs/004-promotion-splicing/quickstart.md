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
refusal); dynamic range marked unpromotable. **MEASURED**: clean/refused = __/__

## 2. Walking skeleton — convert 0x8800, prove hash-neutral (SC-001)
```bash
python3 -m tools.conveyor.pipeline.layout convert 0x8800
rsync + ssh watchman make test        # SHA-1 must MATCH with zero promotions
```
**MEASURED**: ________. If this fails, STOP (HANDOFF rule 1).

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
