#!/usr/bin/env python3
"""Read-only precondition replay (w16g precheck.py) after `targets_rename apply`.

    python3 cloud/work/frontier/w16i/precheck.py DATA_DIR      (Pi, repo root)

DATA_DIR is the scratch COPY of ~/.conveyor after targets_rename (never the live one).
For each renamed boot function and the libgcc functions, with the lock entry that
`lock add SRC:FN --flags F --target <current name>` would write injected in memory,
it replays promote's checks in order: inventory row, promotable population, segment,
converted (an unconverted segment is replayed as if `layout convert` had run, and
says so), passthrough slot, pinned flagset == lock flagset. For comparison it also
prints the flagset the same segment derives when the lock names the OLD inventory id.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.coordinator import db as dbmod  # noqa: E402
from tools.conveyor.pipeline import layout as layoutmod  # noqa: E402
from tools.conveyor.pipeline import lock as lockmod  # noqa: E402

O2 = "-g0 -O2 -mips2 -G 0 -non_shared"
O1 = "-g0 -O1 -mips2 -G 0 -non_shared"
LL = "-g0 -O1 -mips3 -32 -G 0 -non_shared"
# (segment, slot = new target id, old inventory id, flags)
ROWS = [
    ("0x10a00", "__osEPiRawWriteIo", "osEPiRawStartDma", O2),
    ("0x10b60", "__osEPiRawReadIo", "osEPiRawReadIo", O2),
    ("0x105f0", "__osContDataCrc", "__osPfsDataChecksum", O2),
    ("0x10970", "__osSiDeviceBusy", "__osPiDeviceBusy", O2),
    ("0x109a0", "__osResetGlobalIntMask", "osEPiRawWriteIo", O1),
    ("0x10cc0", "__osSetGlobalIntMask", "__osPiGetCmdQueue", O1),
    ("0x107a0", "osDestroyThread", "__osEnqueueAndYield", O1),
] + [("0xe4f0", f, f, LL) for f in "__lshrdi3 __umoddi3 __udivdi3 __ashldi3 __umoddi3_alt "
     "__divdi3 __muldi3 __qdivrem __moddi3 __ashrdi3".split()] \
  + [("0xa5f0", f, f, LL) for f in "__fixdfdi __fixsfdi __floatdidf __floatdisf __floatundidf "
     "__floatundisf".split()]


def derive_with(extra):
    real = layoutmod.load_flagsets
    sim = dict(real())
    sim.update(extra)
    layoutmod.load_flagsets = lambda: dict(sim)
    try:
        return layoutmod.derive()
    finally:
        layoutmod.load_flagsets = real


def main(data):
    conn = dbmod.connect(Path(data) / "conveyor.db")
    new = derive_with({fn: flags for _s, fn, _o, flags in ROWS})
    old = derive_with({o: flags for _s, _f, o, flags in ROWS})
    bad = 0
    for seg_name, fn, old_id, flags in ROWS:
        pre = f"{seg_name:8} {fn:24}"
        row = conn.execute("SELECT population, address FROM n64_target WHERE target_id=?",
                           (fn,)).fetchone()
        try:
            if row is None:
                raise SystemExit(f"no inventory row {fn!r}")
            lockmod.require_promotable_population(fn, data)
            seg = layoutmod._segment_by_name(new, seg_name)
            note = "" if seg["converted"] else " (after `layout convert`)"
            slot = next((f for f in seg["functions"] if f["name"] == fn), None)
            if slot is None or slot["state"] != "passthrough":
                raise SystemExit(f"slot {fn} not passthrough in {seg_name}")
            if seg["flagset"] != flags:
                raise SystemExit(f"segment flagset {seg['flagset']!r} != lock {flags!r}")
            oldseg = layoutmod._segment_by_name(old, seg_name)
            print(f"{pre} PRECONDITIONS OK{note}: target row @0x{row['address']:08X}, "
                  f"flagset {seg['flagset']!r} (lock on old id {old_id}: {oldseg['flagset']!r})")
        except SystemExit as e:
            bad += 1
            print(f"{pre} REFUSE: {e}")
    print(f"{len(ROWS) - bad}/{len(ROWS)} preconditions OK")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))
