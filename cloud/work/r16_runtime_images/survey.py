#!/usr/bin/env python3
"""R16 runtime images + boot-segment survey. Read-only; prints offsets and counts only.

    python3 cloud/work/r16_runtime_images/survey.py [baserom.us.z64]
"""
import json
import sys
import zlib
from pathlib import Path

ROM = Path(sys.argv[1] if len(sys.argv) > 1 else "baserom.us.z64").read_bytes()
BASE = 0x8038A400                       # shared load address of both images
IMAGES = {"A": 0xB5C534, "B": 0xB6FEC4}  # ROM words at 0x2BC20 / 0x2BC24
SERVICES = [0x8038D798, 0x8038D3A4, 0x8038F454, 0x80391490, 0x8039133C]
JR_RA = 0x03E00008


def word(buf, off):
    return int.from_bytes(buf[off:off + 4], "big")


def internal_calls(img):
    out = set()
    for off in range(0, len(img) - 3, 4):
        w = word(img, off)
        if w >> 26 == 3:
            target = (w & 0x3FFFFFF) << 2 | 0x80000000
            if BASE <= target < BASE + len(img):
                out.add(target)
    return out


def looks_like_start(img, addr):
    off = addr - BASE
    return word(img, off) >> 16 == 0x27BD or (off >= 8 and word(img, off - 8) == JR_RA)


report = {"rom_pointer_table": {}, "images": {}, "services": {}, "boot_segment": {}}
for slot, rom_off in ((0x2BC20, "A"), (0x2BC24, "B")):
    vram = slot - 0x1000 + 0x80000400
    report["rom_pointer_table"][f"0x{vram:08X}"] = f"0x{word(ROM, slot):06X} (image {rom_off})"

images = {}
for name, rom_off in IMAGES.items():
    z = zlib.decompressobj(-15)
    img = z.decompress(ROM[rom_off:rom_off + 0x80000])
    images[name] = img
    calls = internal_calls(img)
    report["images"][name] = {
        "rom": f"0x{rom_off:06X}",
        "compressed": 0x80000 - len(z.unused_data),
        "size": len(img),
        "vram": f"0x{BASE:08X}-0x{BASE + len(img):08X}",
        "internal_call_targets": len(calls),
        "targets_at_function_starts": sum(looks_like_start(img, a) for a in calls),
        "jr_ra": sum(1 for o in range(0, len(img), 4) if word(img, o) == JR_RA),
    }

for addr in SERVICES:
    verdict = {}
    for name, img in images.items():
        off = addr - BASE
        if off >= len(img):
            verdict[name] = "outside"
            continue
        tags = [t for t, ok in (("call-target", addr in internal_calls(img)),
                                ("prologue", word(img, off) >> 16 == 0x27BD),
                                ("after-jr-ra", word(img, off - 8) == JR_RA)) if ok]
        verdict[name] = ",".join(tags) or "mid-function"
    report["services"][f"0x{addr:08X}"] = verdict

# Boot segment: IPL3 copies ROM 0x1000.. to 0x80000400; entry clears BSS from 0x8002E8E0.
lui_t0, addiu_t0 = word(ROM, 0x1000), word(ROM, 0x1008)
bss = ((lui_t0 & 0xFFFF) << 16) + (addiu_t0 & 0xFFFF) - (0x10000 if addiu_t0 & 0x8000 else 0)
end = bss - 0x80000400 + 0x1000
bands = []
for off in range(0x10000, end, 0x400):
    ws = [word(ROM, o) for o in range(off, min(off + 0x400, end), 4)]
    rsp = sum(1 for w in ws if w >> 26 in (0x12, 0x32, 0x3A))
    cpu = sum(1 for w in ws if w == JR_RA or w >> 16 == 0x27BD)
    bands.append("rsp" if rsp > 16 else "cpu" if cpu else "data")
report["boot_segment"] = {
    "rom": f"0x1000-0x{end:X}", "bss_start": f"0x{bss:08X}",
    "counted_static": "0x1000-0x10000",
    "bands_from_0x10000_per_1KB": "".join({"cpu": "c", "rsp": "r", "data": "."}[b] for b in bands),
    "jr_ra_0x10000_to_end": sum(1 for o in range(0x10000, end, 4) if word(ROM, o) == JR_RA),
}
print(json.dumps(report, indent=2))
