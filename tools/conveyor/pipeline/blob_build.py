"""Build the game-code image and gate it on byte identity (008, stage 1).

    python3 -m tools.conveyor.pipeline.blob_build build [--keep]

Assembles every region, links them at 0x80086A50 with the generated script,
extracts the loaded range, and compares its sha256 to `build/game_code.bin`.
There is no tolerance: a difference is reported with the first offset, its
vram, and the region and function that own it, and the build exits non-zero
(contract §11-§13).

Feature 004's ROM hash gate was found to have been vacuous since December —
it hashed the wrong file and swallowed failures. This gate is therefore
written to fail loudly by default and is not trusted until it has been made
to fail on purpose (SC-003).
"""
import argparse
import hashlib
import subprocess
import sys
import tempfile
from pathlib import Path

from . import blob_layout
from . import blob_tu
from . import targets as targetsmod

REPO = targetsmod.REPO
BUILD_DIR = REPO / "build" / "blob"
AS = "mips-linux-gnu-as"
LD = "mips-linux-gnu-ld"
OBJCOPY = "mips-linux-gnu-objcopy"
NM = "mips-linux-gnu-nm"


class BuildError(RuntimeError):
    pass


def assemble(asm_path, out_o):
    proc = subprocess.run(
        [AS, "-march=vr4300", "-mabi=32", "-I", str(REPO),
         "-o", str(out_o), str(asm_path)],
        capture_output=True, text=True)
    if proc.returncode != 0:
        first = next((l for l in proc.stderr.splitlines() if ": Error:" in l),
                     proc.stderr.strip()[:200])
        raise BuildError(f"assembling {asm_path.name}: {first}")


def link(objects, script, out_elf):
    proc = subprocess.run(
        [LD, "-T", str(script), "--no-warn-rwx-segments", "-o", str(out_elf),
         *[str(o) for o in objects]],
        capture_output=True, text=True)
    if proc.returncode != 0:
        raise BuildError("link failed: " + proc.stderr.strip()[:400])


def extract_image(elf, out_bin):
    proc = subprocess.run(
        [OBJCOPY, "-O", "binary", "--only-section=.image", str(elf), str(out_bin)],
        capture_output=True, text=True)
    if proc.returncode != 0:
        raise BuildError("objcopy failed: " + proc.stderr.strip()[:300])
    return Path(out_bin).read_bytes()


def locate(document, offset):
    """(region, entry) owning an image offset — for naming a gate failure."""
    vaddr = blob_layout.BASE + offset
    for region in document["regions"]:
        if region["vaddr_start"] <= vaddr < region["vaddr_end"]:
            for entry in region["entries"]:
                if entry["vaddr"] <= vaddr < entry["vaddr"] + entry["size"]:
                    return region, entry
            return region, None
    return None, None


def describe_difference(document, expected, actual):
    """Human-readable first difference (contract §13)."""
    if len(expected) != len(actual):
        detail = f"length {len(actual)} != expected {len(expected)}"
        limit = min(len(expected), len(actual))
    else:
        detail = None
        limit = len(expected)
    offset = next((i for i in range(limit) if expected[i] != actual[i]), None)
    if offset is None:
        return detail or "images differ but no differing byte was found"
    word = offset & ~3
    region, entry = locate(document, word)
    owner = "unmapped"
    if entry is not None:
        owner = (entry["target_id"] if entry["kind"] == "function"
                 else f"opaque@0x{entry['vaddr']:08X}")
    lines = [
        f"first difference at image offset {offset} "
        f"(vram 0x{blob_layout.BASE + offset:08X})",
        f"  region:   {region['name'] if region else '?'}",
        f"  owner:    {owner}",
        f"  expected: {expected[word:word + 4].hex()}",
        f"  built:    {actual[word:word + 4].hex()}",
    ]
    if detail:
        lines.append(f"  note:     {detail}")
    return "\n".join(lines)


def build(document=None, image_path=blob_layout.IMAGE, asm_dir=blob_tu.ASM_DIR,
          script=None, work_dir=None, keep=False, extra_objects=()):
    """Assemble, link, extract, compare. Returns (ok, sha256, message)."""
    document = document or blob_layout.load()
    expected = Path(image_path).read_bytes()
    expected_sha = hashlib.sha256(expected).hexdigest()
    if expected_sha != document["image"]["sha256"]:
        raise BuildError(
            f"image {image_path} sha256 {expected_sha[:12]}… does not match the "
            f"map's {document['image']['sha256'][:12]}… — re-derive the layout")
    script = Path(script or blob_tu.LINKER_SCRIPT)
    asm_dir = Path(asm_dir)

    holder = Path(work_dir) if work_dir else Path(tempfile.mkdtemp(prefix="blob-"))
    holder.mkdir(parents=True, exist_ok=True)
    objects = []
    for region in document["regions"]:
        source = asm_dir / f"{region['name']}.s"
        if not source.is_file():
            raise BuildError(f"missing region source {source} — run blob_tu generate")
        out_o = holder / f"{region['name']}.o"
        assemble(source, out_o)
        objects.append(out_o)
    for extra in extra_objects:
        extra = Path(extra)
        if not extra.is_file():
            raise BuildError(f"missing spliced object {extra}")
        objects.append(extra)
    elf = holder / "blob.elf"
    link(objects, script, elf)
    built = extract_image(elf, holder / "image.bin")
    built_sha = hashlib.sha256(built).hexdigest()
    if built_sha == expected_sha:
        return True, built_sha, f"image matches ({len(built)} bytes)"
    return False, built_sha, describe_difference(document, expected, built)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--layout", default=str(blob_layout.LAYOUT_JSON))
    parser.add_argument("--image", default=str(blob_layout.IMAGE))
    parser.add_argument("--work-dir", default=None)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("build")
    p.add_argument("--keep", action="store_true",
                   help="keep intermediate objects in --work-dir")
    args = parser.parse_args()

    document = blob_layout.load(args.layout)
    try:
        ok, sha, message = build(document, args.image, work_dir=args.work_dir,
                                 keep=args.keep)
    except BuildError as exc:
        sys.exit(f"blob build FAILED: {exc}")
    if not ok:
        sys.exit(f"blob build FAILED — image is not byte-identical\n{message}")
    print(f"blob build OK — {message}")
    print(f"  sha256 {sha}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
