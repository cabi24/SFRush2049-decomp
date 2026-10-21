"""Image layout map for the decompressed game code (008, stage 1).

    python3 -m tools.conveyor.pipeline.blob_layout derive|report

The game code is not in the cartridge as code: it is a raw DEFLATE stream at
ROM 0xB0CB10 that inflates at boot to 0x80086A50. Matched game functions are
therefore unlinkable and uncounted. This module describes the 647,072-byte
image as an ordered, complete list of entries — functions with gate-passed
extents, the proven .data/.rodata tail (kind "data"), and opaque runs for
everything else — so the image can be rebuilt
from pieces and individual functions replaced with verified C.

Contract: specs/008-blob-image-rebuild/contracts/image-layout-and-gate.md §1-§6.
Nothing here is guessed: an overlap or a coverage hole is a hard error, since
either would corrupt the image silently.
"""
import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

from ..client import DEFAULT_DATA
from . import targets as targetsmod

REPO = targetsmod.REPO
IMAGE = targetsmod.GAME_CODE_BIN
BASE = targetsmod.GAME_CODE_BASE
LAYOUT_JSON = REPO / "build" / "blob_layout.json"

DEFAULT_FLAGSET = "-g0 -O2 -mips2 -G 0 -non_shared"
SPLIT_GAP = 4096       # an opaque run this large ends the current region
MAX_FUNCS = 60         # ... as does this many functions, to keep TUs reviewable

# The retail image's tail after its last function is .data, then .rodata
# (float literals and switch jump tables). Proven 2026-10-21: no j/jal in game,
# static or runtime-image code targets it and it holds no jr ra
# (tests/conveyor/test_blob_data_tail.py). derive() labels it as data entries.
DATA_SECTIONS = ((".data", 0x8010FD7C), (".rodata", 0x80123870))
RETAIL_IMAGE_END = 0x801249F0
JR_RA = 0x03E00008


class LayoutError(RuntimeError):
    """The map cannot be built without guessing. Never downgraded to a warning."""


def gate_passed_rows(conn):
    """Extracted rows with a usable extent (contract §1)."""
    rows = conn.execute(
        "SELECT target_id, address, insn_count, gate_reason FROM n64_target"
        " WHERE population='extracted' AND address IS NOT NULL"
        " AND insn_count IS NOT NULL ORDER BY address, target_id"
    ).fetchall()
    out = []
    for row in rows:
        reason = row["gate_reason"] or ""
        if reason.startswith("extent_conflict") or reason.startswith("scan_overrun"):
            continue
        out.append(row)
    return out


def entries(conn, image_size):
    """Ordered, complete, non-overlapping [{kind, vaddr, size, ...}]."""
    end = BASE + image_size
    functions = []
    for row in gate_passed_rows(conn):
        start = row["address"]
        size = row["insn_count"] * 4
        if start < BASE or start + size > end:
            continue                      # outside the image; reported by derive
        functions.append((start, size, row["target_id"]))
    functions.sort()

    out, cursor = [], BASE
    for start, size, target_id in functions:
        if start < cursor:
            previous = out[-1]["target_id"] if out and out[-1]["kind"] == "function" else "?"
            raise LayoutError(
                f"overlapping extents: {target_id} starts at {start:#010x}, "
                f"inside {previous} which ends at {cursor:#010x}")
        if start > cursor:
            out.append({"kind": "opaque", "vaddr": cursor, "size": start - cursor})
        out.append({"kind": "function", "vaddr": start, "size": size,
                    "target_id": target_id})
        cursor = start + size
    if cursor < end:
        out.append({"kind": "opaque", "vaddr": cursor, "size": end - cursor})

    covered = sum(item["size"] for item in out)
    if covered != image_size:
        raise LayoutError(f"coverage {covered} != image size {image_size}")
    return out


def mark_data_sections(items, image):
    """Replace the retail image's trailing opaque run with its .data/.rodata.

    Other images (test fixtures) pass through unchanged. For the retail image the
    tail must still be one opaque run from .data to the end with no `jr ra` in
    it; anything else means the data proof has to be redone, not assumed."""
    if BASE + len(image) != RETAIL_IMAGE_END:
        return items
    start = DATA_SECTIONS[0][1]
    tail = items[-1] if items else None
    if not tail or tail["kind"] != "opaque" or tail["vaddr"] != start:
        found = f"{tail['kind']} at {tail['vaddr']:#010x}" if tail else "nothing"
        raise LayoutError(f"image tail is {found}, not the opaque .data run at "
                          f"{start:#010x}; re-prove the tail before labelling it data")
    if any(word == JR_RA for (word,) in struct.iter_unpack(">I", image[start - BASE:])):
        raise LayoutError("a jr ra inside the image's .data/.rodata tail")
    bounds = [address for _, address in DATA_SECTIONS[1:]] + [RETAIL_IMAGE_END]
    return items[:-1] + [
        {"kind": "data", "section": section, "vaddr": lo, "size": hi - lo}
        for (section, lo), hi in zip(DATA_SECTIONS, bounds)]


def regions(items, split_gap=SPLIT_GAP, max_funcs=MAX_FUNCS,
            flagset=DEFAULT_FLAGSET):
    """Group entries into TU-sized regions (contract §5)."""
    out, current = [], None

    def start_region(item):
        return {"vaddr_start": item["vaddr"], "vaddr_end": item["vaddr"],
                "flagset": flagset, "entries": []}

    for item in items:
        if current is None:
            current = start_region(item)
        elif ((item["kind"] != "function" and item["size"] >= split_gap
               and any(e["kind"] == "function" for e in current["entries"]))
              or (item["kind"] == "function"
                  and sum(1 for e in current["entries"] if e["kind"] == "function")
                  >= max_funcs)):
            # A big gap starts a region (it is a natural boundary); the
            # function-count limit only splits before another function, so the
            # cap never strands a data-only region. Consecutive non-function
            # entries (.data then .rodata) stay in one region.
            out.append(current)
            current = start_region(item)
        current["entries"].append(item)
        current["vaddr_end"] = item["vaddr"] + item["size"]
    if current is not None:
        out.append(current)
    for index, region in enumerate(out):
        region["name"] = f"blob_{region['vaddr_start']:08x}"
        region["index"] = index
    return out


def derive(conn, image_path=IMAGE, output=LAYOUT_JSON, **options):
    image_bytes = Path(image_path).read_bytes()
    items = mark_data_sections(entries(conn, len(image_bytes)), image_bytes)
    grouped = regions(items, **options)
    functions = [e for e in items if e["kind"] == "function"]
    opaque = [e for e in items if e["kind"] == "opaque"]
    data = [e for e in items if e["kind"] == "data"]
    document = {
        "image": {
            "path": str(Path(image_path).relative_to(REPO))
                    if Path(image_path).is_absolute() and str(image_path).startswith(str(REPO))
                    else str(image_path),
            "base": f"{BASE:08X}",
            "size": len(image_bytes),
            "sha256": hashlib.sha256(image_bytes).hexdigest(),
        },
        "totals": {
            "regions": len(grouped),
            "functions": len(functions),
            "function_bytes": sum(e["size"] for e in functions),
            "opaque_runs": len(opaque),
            "opaque_bytes": sum(e["size"] for e in opaque),
            "data_bytes": sum(e["size"] for e in data),
        },
        "regions": grouped,
    }
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    # No timestamp: the map is a pure function of its inputs (contract §6).
    output.write_text(json.dumps(document, indent=2, sort_keys=True) + "\n")
    return document


def load(path=LAYOUT_JSON):
    return json.loads(Path(path).read_text())


def report(document):
    totals = document["totals"]
    size = document["image"]["size"]
    print(f"image {size} bytes @ 0x{document['image']['base']} "
          f"sha256 {document['image']['sha256'][:12]}…")
    print(f"regions:  {totals['regions']}")
    print(f"functions:{totals['functions']:6d}  {totals['function_bytes']:8d} bytes "
          f"({100 * totals['function_bytes'] / size:.1f}%)")
    print(f"opaque:   {totals['opaque_runs']:6d}  {totals['opaque_bytes']:8d} bytes "
          f"({100 * totals['opaque_bytes'] / size:.1f}%)")
    for entry in (e for r in document["regions"] for e in r["entries"] if e["kind"] == "data"):
        print(f"data:     {entry['section']:>6}  {entry['size']:8d} bytes "
              f"@ 0x{entry['vaddr']:08X}")
    widest = sorted((e for r in document["regions"] for e in r["entries"]
                     if e["kind"] == "opaque"), key=lambda e: -e["size"])[:5]
    print("largest opaque runs: "
          + ", ".join(f"0x{e['vaddr']:08X}+{e['size']}" for e in widest))


def main():
    from ..coordinator import db as dbmod

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    parser.add_argument("--image", default=str(IMAGE))
    parser.add_argument("--output", default=str(LAYOUT_JSON))
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("derive")
    sub.add_parser("report")
    args = parser.parse_args()

    if args.command == "report":
        report(load(args.output))
        return 0
    conn = dbmod.connect(Path(args.data) / "conveyor.db")
    try:
        document = derive(conn, args.image, args.output)
    except LayoutError as exc:
        sys.exit(f"blob_layout: {exc}")
    report(document)
    print(f"map -> {args.output}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
