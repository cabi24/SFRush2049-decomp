#!/usr/bin/env python3
"""Read-only precondition replay of `pipeline.promote run <seg>:<fn> --from SRC` (Pi, repo root).

promote.py has no dry-run/check mode. This replays its preconditions in order, with
the lock entry that `pipeline.lock add SRC:FN --flags F [--target T]` WOULD write
injected in memory only (matched.lock.json is never written), and stops at the first
refusal promote.py would raise, quoting its message. It does not splice, build or commit.

    python3 cloud/work/frontier/w16g/precheck.py
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.client import DEFAULT_DATA  # noqa: E402
from tools.conveyor.coordinator import db as dbmod  # noqa: E402
from tools.conveyor.pipeline import layout as layoutmod  # noqa: E402
from tools.conveyor.pipeline import lock as lockmod  # noqa: E402

O2 = "-g0 -O2 -mips2 -G 0 -non_shared"
O1 = "-g0 -O1 -mips2 -G 0 -non_shared"
LL = "-g0 -O1 -mips3 -32 -G 0 -non_shared"
SRC = "cloud/work/frontier/w16g/sources/"
# (segment, slot function, inventory target_id lock add must name, flags)
ROWS = [
    ("0xf700", "__osPfsRWInode", "__osPfsRWInode", O2),
    ("0x10280", "__osContRamWrite", "__osContRamWrite", O2),
    ("0x10a00", "__osEPiRawWriteIo", "osEPiRawStartDma", O2),
    ("0x10b60", "__osEPiRawReadIo", "osEPiRawReadIo", O2),
    ("0x105f0", "__osContAddressCrc", "__osContAddressCrc", O2),
    ("0x105f0", "__osContDataCrc", "__osPfsDataChecksum", O2),
    ("0xca70", "osAiSetNextBuffer", "osAiSetNextBuffer", O2),
    ("0xab20", "osMotorStop", "osMotorStop", O2),
    ("0x109a0", "__osResetGlobalIntMask", "osEPiRawWriteIo", O1),
    ("0x10cc0", "__osSetGlobalIntMask", "__osPiGetCmdQueue", O1),
    ("0x10d10", "osYieldThread", "osYieldThread", O1),
    ("0x107a0", "osDestroyThread", "__osEnqueueAndYield", O1),
    ("0x10970", "__osSiDeviceBusy", "__osPiDeviceBusy", O2),
    ("0x10d60", "func_800103A0", "func_800103A0", O2),
    ("0x11640", "func_80014650", "func_80014650", O2),
    ("0x25bb0", "func_8002517C", "func_8002517C", O2),
] + [("0xe4f0", f, f, LL) for f in "__lshrdi3 __umoddi3 __udivdi3 __ashldi3 __umoddi3_alt __divdi3 __muldi3 __qdivrem __moddi3 __ashrdi3".split()] \
  + [("0xa5f0", f, f, LL) for f in "__fixdfdi __fixsfdi __fixunsdfdi __fixunssfdi __floatdidf __floatdisf __floatundidf __floatundisf".split()]


def main():
    real = layoutmod.load_flagsets()
    sim = dict(real)
    for _seg, fn, tid, flags in ROWS:          # what `lock add` would record
        sim[tid] = flags
    layoutmod.load_flagsets = lambda: dict(sim)
    mapping = layoutmod.derive()
    conn = dbmod.connect(Path(DEFAULT_DATA) / "conveyor.db")
    for seg_name, fn, tid, flags in ROWS:
        row = conn.execute("SELECT population, address FROM n64_target WHERE target_id=?", (tid,)).fetchone()
        pre = f"{seg_name}:{fn:24} lock-target={tid:22}"
        if row is None:
            print(pre, "REFUSE(lock add): no target .o in inventory"); continue
        try:
            lockmod.require_promotable_population(tid)
        except SystemExit as e:
            print(pre, "REFUSE:", e); continue
        seg = layoutmod._segment_by_name(mapping, seg_name)
        if seg is None:
            print(pre, f"REFUSE: no segment {seg_name!r}"); continue
        if not seg["converted"]:
            print(pre, f"REFUSE: {seg_name} is not converted - run `pipeline.layout convert {seg_name}` first"
                  f" [then: flagset={seg['flagset']!r}]"); continue
        slot = next((f for f in seg["functions"] if f["name"] == fn), None)
        if slot is None:
            print(pre, f"REFUSE: {fn} is not in segment {seg_name}"); continue
        if slot["state"] != "passthrough":
            print(pre, f"REFUSE: slot state {slot['state']}"); continue
        if not seg["flagset"]:
            print(pre, f"REFUSE: segment {seg_name} has no pinned flagset - run the per-TU flag sweep first [FR-005]"); continue
        note = "" if seg["flagset"] == flags else f" (segment flagset {seg['flagset']!r} != lock {flags!r}: promote_batch refuses)"
        print(pre, f"PRECONDITIONS OK flagset={seg['flagset']!r}{note}; next: clean tree, splice, ROM gate")


if __name__ == "__main__":
    main()
