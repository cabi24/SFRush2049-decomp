"""Layout, assembly and image gate for the runtime images A and B (R16 landing path).

    python3 -m tools.conveyor.pipeline.ovl_layout build  [--targets asm/us/ovl_a | --image a] [--passthrough]
    python3 -m tools.conveyor.pipeline.ovl_layout derive [--targets ... | --image ...]

The game-code image path (blob_layout -> blob_tu -> blob_build, 008) applied to
the two raw-deflate images loaded at 0x8038A400. Every product is derived and
lives under build/ovl/<x>/:

    image.bin      the retail image, inflated from the ROM (pipeline.ovl_image)
    layout.json    blob_layout's document format: ordered, complete entries
    asm/*.s        one file per region; one `.text.<name>` section per function
    ovl_<x>.ld     places every section at its image address from 0x8038A400

**Tiling.** Functions and their extents come from asm/us/ovl_<x>/extents.json
(pipeline.ovl_targets: scanned, evidence per start, switch-table merges
recorded). Everything else is an opaque run carried byte for byte from the
image: inter-function padding and the data tail after `text_end`, which holds
the switch tables, literals and initialised data. A switch table is refused if
it would fall inside a function's extent (it must stay opaque); an overlap,
a hole, a misaligned extent or an image whose sha256 differs from
extents.json is a LayoutError. Nothing is guessed.

**Reuse.** The game-code modules already take a layout document; only two of
their helpers read the game's base constant, and they now accept the image's
base (blob_tu.render_region) or read it from the document
(blob_build.describe_difference). This module is a thin parametrisation:
region grouping is blob_layout.regions, rendering is blob_tu, assembling,
linking and the byte-identity gate are blob_build.build. Nothing is copied.

**The image gate** is blob_build's: the linked `.image` must equal image.bin
byte for byte, else the first differing offset, its vram, region and owner
are reported and the command fails. `--passthrough` gates the all-passthrough
link (no C at all); the default gates the image with every body locked in
ovl_matched.lock.json (pipeline.ovl_splice).
"""
import argparse
import hashlib
import json
import sys
from pathlib import Path

from . import blob_build, blob_layout, blob_tu, ovl_image, ovl_targets

REPO = ovl_image.REPO
DEFAULT_FLAGSET = blob_layout.DEFAULT_FLAGSET
LayoutError = blob_layout.LayoutError


def image_name(targets=None, image=None):
    """"a"/"b" from `--targets asm/us/ovl_a` or `--image a` (exactly one)."""
    if targets is not None:
        name = Path(targets).resolve().name
        if not name.startswith("ovl_") or name[4:] not in ovl_image.IMAGES:
            raise LayoutError(f"--targets {targets}: not a runtime image directory "
                              f"(expected asm/us/ovl_a or asm/us/ovl_b)")
        if image is not None and image != name[4:]:
            raise LayoutError(f"--targets {targets} disagrees with --image {image}")
        return name[4:]
    if image not in ovl_image.IMAGES:
        raise LayoutError(f"unknown runtime image {image!r}")
    return image


def work_dir(image, build=None):
    return Path(build or REPO / "build") / "ovl" / image


def layout_path(image, build=None):
    return work_dir(image, build) / "layout.json"


def asm_dir(image, build=None):
    return work_dir(image, build) / "asm"


def script_path(image, build=None):
    return work_dir(image, build) / f"ovl_{image}.ld"


def load_extents(image, target_dir=None):
    path = Path(target_dir or ovl_image.target_dir(image)) / "extents.json"
    return json.loads(path.read_text())


def entries(extents, image_bytes):
    """Ordered, complete, non-overlapping [{kind, vaddr, size, target_id?}]."""
    base = int(extents["base"], 16)
    end = base + len(image_bytes)
    if extents.get("size") is not None and extents["size"] != len(image_bytes):
        raise LayoutError(f"image is {len(image_bytes)} bytes, extents.json says {extents['size']}")
    functions = sorted((int(f["address"], 16), f["size"], f["name"]) for f in extents["functions"])
    out, cursor = [], base
    names = set()
    for start, size, name in functions:
        if name in names:
            raise LayoutError(f"duplicate function name {name}")
        names.add(name)
        if start % 4 or size % 4 or size <= 0:
            raise LayoutError(f"{name}: extent 0x{start:08X}+{size} is not word aligned")
        if start < cursor:
            raise LayoutError(f"overlapping extents: {name} starts at 0x{start:08X}, "
                              f"before 0x{cursor:08X}")
        if start + size > end:
            raise LayoutError(f"{name}: extent ends past the image (0x{end:08X})")
        if start > cursor:
            out.append({"kind": "opaque", "vaddr": cursor, "size": start - cursor})
        out.append({"kind": "function", "vaddr": start, "size": size, "target_id": name})
        cursor = start + size
    text_end = int(extents["text_end"], 16)
    if cursor != text_end:
        raise LayoutError(f"functions end at 0x{cursor:08X}, text_end is 0x{text_end:08X}")
    if cursor < end:
        out.append({"kind": "opaque", "vaddr": cursor, "size": end - cursor})
    if sum(item["size"] for item in out) != len(image_bytes):
        raise LayoutError("entries do not cover the image exactly")
    return out


def switch_table_runs(image_bytes, extents, items):
    """[(vaddr, size)] of the image's switch tables, each proven to lie inside
    an opaque entry (never inside a function, whose body C would replace)."""
    base = int(extents["base"], 16)
    tables = ovl_targets.switch_tables(image_bytes, int(extents["text_end"], 16), base)
    opaque = [(e["vaddr"], e["vaddr"] + e["size"]) for e in items if e["kind"] == "opaque"]
    out = []
    for table, dests in sorted(tables.items()):
        size = 4 * len(dests)
        if not any(lo <= table and table + size <= hi for lo, hi in opaque):
            raise LayoutError(f"switch table 0x{table:08X}+{size} is not inside an opaque run")
        out.append((table, size))
    return out


def derive(image, image_path=None, target_dir=None, output=None, write=True):
    """The layout document for one runtime image (blob_layout's format)."""
    image_path = Path(image_path or ovl_image.ensure(image))
    data = image_path.read_bytes()
    extents = load_extents(image, target_dir)
    sha = hashlib.sha256(data).hexdigest()
    if sha != extents["image_sha256"]:
        raise LayoutError(f"{image_path}: sha256 {sha[:12]}... is not the image "
                          f"extents.json describes ({extents['image_sha256'][:12]}...)")
    base = int(extents["base"], 16)
    items = entries(extents, data)
    tables = switch_table_runs(data, extents, items)
    regions = blob_layout.regions(items)
    for region in regions:
        region["name"] = f"ovl_{image}_{region['vaddr_start']:08x}"
    functions = [e for e in items if e["kind"] == "function"]
    opaque = [e for e in items if e["kind"] == "opaque"]
    try:
        rel = str(image_path.resolve().relative_to(REPO))
    except ValueError:
        rel = str(image_path)
    document = {
        "image": {"name": image, "path": rel, "base": f"{base:08X}", "size": len(data),
                  "sha256": sha, "text_end": extents["text_end"]},
        "totals": {
            "regions": len(regions),
            "functions": len(functions),
            "function_bytes": sum(e["size"] for e in functions),
            "opaque_runs": len(opaque),
            "opaque_bytes": sum(e["size"] for e in opaque),
            "switch_tables": len(tables),
        },
        "switch_tables": [{"vaddr": f"0x{t:08X}", "size": n} for t, n in tables],
        "regions": regions,
    }
    if write:
        output = Path(output or layout_path(image))
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_text(json.dumps(document, indent=2, sort_keys=True) + "\n")
    return document


def generate(document, spliced=None, asm=None, script=None):
    """Region assembly and the linker script. `spliced` is {name: bytes}."""
    image = document["image"]["name"]
    base = int(document["image"]["base"], 16)
    path = Path(document["image"]["path"])
    data = (path if path.is_absolute() else REPO / path).read_bytes()
    asm = Path(asm or asm_dir(image))
    asm.mkdir(parents=True, exist_ok=True)
    for stale in asm.glob("*.s"):
        stale.unlink()
    written = []
    for region in document["regions"]:
        out = asm / f"{region['name']}.s"
        out.write_text(blob_tu.render_region(region, data, path, spliced or {}, base=base,
                                             source_dir=f"src/ovl_{image}"))
        written.append(out)
    script = Path(script or script_path(image))
    # The whole-image link holds no relocations (spliced C arrives as linked
    # bytes), so the script needs no PROVIDE table.
    script.write_text(blob_tu.render_linker_script(document, symbols={}))
    return written, script


def build(document, spliced=None, work=None):
    """Generate and gate. (ok, sha256, message), as blob_build.build."""
    image = document["image"]["name"]
    _written, script = generate(document, spliced)
    path = Path(document["image"]["path"])
    return blob_build.build(document, path if path.is_absolute() else REPO / path,
                            asm_dir=asm_dir(image), script=script,
                            work_dir=work or work_dir(image) / "link")


def coverage(image, lock=None, target_dir=None):
    """Locked functions of one image against its own denominators."""
    from . import ovl_splice

    extents = load_extents(image, target_dir)
    lock = ovl_splice.load_lock()[image] if lock is None else lock
    sizes = {f["name"]: f["size"] for f in extents["functions"]}
    linked = [n for n in lock if n in sizes]
    return {"image": image, "functions": len(linked), "total_functions": len(sizes),
            "bytes": sum(sizes[n] for n in linked),
            "function_bytes": sum(sizes.values()), "image_size": extents["size"],
            "rom_offset": extents.get("rom_offset")}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    for name in ("derive", "build"):
        p = sub.add_parser(name)
        p.add_argument("--targets", action="append", default=[],
                       help="asm/us/ovl_a or asm/us/ovl_b (repeatable)")
        p.add_argument("--image", action="append", choices=sorted(ovl_image.IMAGES))
        if name == "build":
            p.add_argument("--passthrough", action="store_true",
                           help="gate the all-passthrough link (no locked C)")
    args = parser.parse_args(argv)
    try:
        images = sorted({image_name(t) for t in args.targets} | set(args.image or [])) \
            or sorted(ovl_image.IMAGES)
    except LayoutError as exc:
        sys.exit(f"ovl_layout: {exc}")
    ok = True
    for image in images:
        try:
            document = derive(image)
        except (LayoutError, ovl_image.ImageError) as exc:
            print(f"image {image.upper()}: LAYOUT FAILED: {exc}")
            ok = False
            continue
        t = document["totals"]
        print(f"image {image.upper()}: {document['image']['size']} B @ 0x{document['image']['base']}, "
              f"{t['functions']} functions ({t['function_bytes']} B), {t['opaque_runs']} opaque "
              f"runs ({t['opaque_bytes']} B, {t['switch_tables']} switch tables inside), "
              f"{t['regions']} regions")
        if args.command == "derive":
            continue
        label = "all-passthrough" if args.passthrough else "locked C"
        try:
            if args.passthrough:
                spliced = {}
            else:
                from . import ovl_splice
                spliced = ovl_splice.spliced_bodies(image, document=document)
                label = f"{len(spliced)} locked C bodies"
            good, sha, message = build(document, spliced)
        except Exception as exc:     # noqa: BLE001 - every failure is a FAILED gate
            good, sha, message = False, "", f"{type(exc).__name__}: {exc}"
        print(f"  image gate ({label}): "
              f"{'byte-identical, sha256 ' + sha if good else 'FAILED'}")
        if not good:
            print("  " + message.replace("\n", "\n  "))
        ok &= good
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
