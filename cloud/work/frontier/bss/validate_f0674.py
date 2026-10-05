#!/usr/bin/env python3
"""Dry-run validation of own-.bss placement on func_800F0674 (no lock write).

    # on the builder: compile w2g/func_800F0674/best.c with IDO -O3 to f0674.o
    PYTHONPATH=. python3 cloud/work/frontier/bss/validate_f0674.py f0674.o

1. The IDO object is prepared exactly as blob_splice.compile_on_builder does
   (.text renamed to .text.<name>, alignment 4, IDO-only sections stripped)
   and linked alone with blob_splice.link_function against the layout's
   retail image: the body must equal the image bytes.
2. The scorer's compare() (tools/cloud/score.py, unmodified) on the raw object
   must be a strict MATCH.
3. Variants of the retail image with one static's address shifted must be
   refused by both: one `sp` reference moved (disagreement), every `sp`
   reference moved two bytes into `op1` (overlap), every `cnt` reference moved
   past the zeroed range (unverified -> body differs / not accepted), and one
   moved into the initialised image (failure).
Nothing is written outside a temporary directory.
"""
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

from tools.cloud import owndata, score
from tools.conveyor.pipeline import blob_build, blob_layout, blob_splice

NAME = "func_800F0674"


def prepare(raw, out):
    subprocess.run([blob_splice.OBJCOPY, f"--rename-section=.text=.text.{NAME}",
                    f"--set-section-alignment=.text.{NAME}=4",
                    "-R", ".options", "-R", ".reginfo", "-R", ".mdebug",
                    "-R", ".comment", "-R", ".pdr", "-R", ".MIPS.abiflags",
                    str(raw), str(out)], check=True)
    return out


def main():
    raw = Path(sys.argv[1])
    layout = blob_layout.load()
    image = (Path(layout["image"]["path"]).read_bytes(), int(layout["image"]["base"], 16))
    data, base = image
    entry = next(e for r in layout["regions"] for e in r["entries"]
                 if e.get("target_id") == NAME)
    vaddr, size = entry["vaddr"], entry["size"]
    retail = data[vaddr - base:vaddr - base + size]
    want = list(struct.unpack(f">{size // 4}I", retail))
    work = Path(tempfile.mkdtemp(prefix="bssval-"))
    obj = prepare(raw, work / f"{NAME}.o")

    # sites of each static in the retail words
    result = owndata.verify(obj, NAME, want, address=vaddr,
                            image=owndata.ImageData.from_image(data, base),
                            addresses=blob_splice.address_named)
    print("owndata.verify:", "ok" if result.ok else "NOT OK", f"{len(result.sites)} sites,",
          f"{result.references} references;", result.bss())
    for note in result.notes:
        print("   ", note)
    sites = {}
    for _section, offset, his, lo, _address in result.bss_sites:
        sites.setdefault(offset, set()).update((lo,) + tuple(his))
    pairs = {off: sorted({(h, lo) for (_s, o, hs, lo, _a) in result.bss_sites if o == off
                          for h in hs}) for off in sites}

    body = blob_splice.link_function(obj, NAME, vaddr, size, provides=None, work=work / "ok",
                                     image=image)
    print("link_function:", "BYTE-IDENTICAL" if body == retail else "DIFFERS",
          f"({size} bytes)")
    print("    script:", [l.strip() for l in (work / "ok" / f"{NAME}.ld").read_text()
                          .splitlines() if ".own" in l])

    scored = score.compare(str(raw), NAME, show=0)
    print("score.compare:", scored.summary(), "accepted" if scored.accepted() else "REFUSED")

    def moved(changes):
        """Retail image with the given {(hi, lo) pair: new address} applied."""
        img = bytearray(data)
        words = list(want)
        for (hi, lo), address in changes.items():
            for site, value in ((hi, ((address + 0x8000) >> 16) & 0xFFFF),
                                (lo, address & 0xFFFF)):
                i = site // 4
                words[i] = (words[i] & 0xFFFF0000) | value
        img[vaddr - base:vaddr - base + size] = struct.pack(f">{len(words)}I", *words)
        return bytes(img), words

    variants = {
        "one sp reference +4 (disagreement)": {pairs[4][0]: 0x8015694C},
        "every sp reference -> 0x80156942 (overlaps op1)": {p: 0x80156942 for p in pairs[4]},
        "every cnt reference -> 0x80180000 (outside game .bss)": {p: 0x80180000 for p in pairs[8]},
        "every op1 reference -> 0x80110000 (initialised data)": {p: 0x80110000 for p in pairs[0]},
    }
    refused_all = True
    for label, changes in variants.items():
        img, words = moved(changes)
        try:
            got = blob_splice.link_function(obj, NAME, vaddr, size, provides=None,
                                            work=work / "bad", image=(img, base))
            link = ("refused by the image gate (body differs)"
                    if got != img[vaddr - base:vaddr - base + size] else "ACCEPTED")
        except blob_build.BuildError as exc:
            link = f"BuildError: {str(exc)[:150]}"
        original = score.targets
        score.targets = lambda: {NAME: words}
        try:
            s = score.compare(str(raw), NAME, show=0)
        finally:
            score.targets = original
        verdict = "ACCEPTED" if s.accepted() else (
            "not accepted" + ("" if s.accepted(True) else ", not even with --allow-unverified"))
        refused_all &= link != "ACCEPTED" and not s.accepted()
        print(f"variant {label}:\n    link: {link}\n    score: {verdict}: {s.summary()[:160]}")
    ok = body == retail and scored.accepted() and refused_all
    print("RESULT:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
