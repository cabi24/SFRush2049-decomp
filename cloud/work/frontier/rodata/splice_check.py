#!/usr/bin/env python3
"""Dry image-gate check of blob_splice.link_function on IDO objects.

    python3 cloud/work/frontier/rodata/splice_check.py DIR

DIR holds `<name>.o` or `wrong_<name>.o` as IDO wrote them (cc -c). Each is
prepared exactly as blob_splice.compile_on_builder does (objcopy rename and
strip), linked alone with link_function, and its bytes compared with the
retail image at the function's extent. Nothing is written outside a temporary
directory: no lock, no src/blob, no region files.

Needs build/game_code.bin and build/blob_layout.json (git-ignored).
"""
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))

from tools.conveyor.pipeline import blob_build, blob_layout, blob_splice  # noqa: E402


def main():
    document = blob_layout.load()
    image = (REPO / document["image"]["path"]).read_bytes()
    base = int(document["image"]["base"], 16)
    extents = {e["target_id"]: e for region in document["regions"]
               for e in region["entries"] if e["kind"] == "function"}
    symbols = blob_splice.image_symbols(document)
    status = 0
    with tempfile.TemporaryDirectory() as tmp:
        for raw in sorted(Path(sys.argv[1]).glob("*.o")):
            name = raw.stem[len("wrong_"):] if raw.stem.startswith("wrong_") else raw.stem
            extent = extents[name]
            obj = Path(tmp) / f"{raw.stem}.o"
            subprocess.run(
                [blob_splice.OBJCOPY, f"--rename-section=.text=.text.{name}",
                 f"--set-section-alignment=.text.{name}=4",
                 "-R", ".options", "-R", ".reginfo", "-R", ".mdebug",
                 "-R", ".comment", "-R", ".pdr", "-R", ".MIPS.abiflags",
                 str(raw), str(obj)], check=True)
            want = image[extent["vaddr"] - base:extent["vaddr"] - base + extent["size"]]
            try:
                body = blob_splice.link_function(obj, name, extent["vaddr"], extent["size"],
                                                 symbols, work=Path(tmp) / raw.stem)
            except blob_build.BuildError as exc:
                print(f"{raw.name}: REFUSED {exc}")
                status |= not raw.stem.startswith("wrong_")
                continue
            same = body == want
            differing = sum(body[i:i + 4] != want[i:i + 4] for i in range(0, len(want), 4))
            print(f"{raw.name}: " + (f"IMAGE BYTES EQUAL ({len(want)} bytes)" if same
                                     else f"{differing} words differ from the image"))
            status |= same == raw.stem.startswith("wrong_")
    return int(status)


if __name__ == "__main__":
    sys.exit(main())
