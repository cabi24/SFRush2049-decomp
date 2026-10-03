"""Runtime images from C: compile, splice, image gate, exact recompression (R16).

    python3 -m tools.conveyor.pipeline.ovl_rom verify [--image a|b ...] [--sources DIR]

The prototype of blob_rom for the two runtime images at 0x8038A400. For each
image it:

1. compiles every `<sources>/ovl_<x>/<func>.c` with IDO on the builder, using
   the flags on its first line (`/* flags: ... */`, as in cloud/matches). The
   build uses private remote and local directories, so it cannot disturb a
   concurrent blob splice.
2. links each object alone at the function's image address
   (blob_splice.link_function) and refuses excess non-zero words.
3. composes the image: text words from asm/us/ovl_<x> (the protected targets),
   compiled bodies spliced over their functions, and the bytes after the text
   carried through from the original image.
4. gates the image: it must be byte-identical to the decompressed original.
5. recompresses it with deflate104 (zlib 1.0.4, level 9, raw) and requires
   the stream to equal the cartridge's stream at the image's ROM offset.

No production asset changes. The cartridge build does not consume these
streams yet; composing them into the ROM is the remaining production step.
"""
import argparse
import hashlib
import json
import re
import shutil
import subprocess
import sys
import tempfile
import zlib
from pathlib import Path

from . import blob_build, blob_rom, blob_splice, ovl_targets

REPO = ovl_targets.REPO
SOURCES = REPO / "cloud" / "matches"
WORK = REPO / "build" / "ovl"
BUILDER = blob_splice.BUILDER
TOOLKIT = blob_splice.TOOLKIT
FLAGS = re.compile(r"/\* flags: (.+?) \*/")


class OvlRomError(RuntimeError):
    pass


def sources_for(image, root=SOURCES):
    return sorted((Path(root) / f"ovl_{image}").glob("*.c"))


def compile_remote(image, sources, builder=BUILDER, toolkit=TOOLKIT):
    """{function: object path}, failures raise. One round trip per image."""
    if not sources:
        return {}
    local = WORK / image / "obj"
    if local.exists():
        shutil.rmtree(local)
    local.mkdir(parents=True)
    remote = f"/tmp/ovlrom-{image}"
    jobs = []
    for src in sources:
        header = FLAGS.fullmatch(src.read_text().splitlines()[0].strip())
        if not header:
            raise OvlRomError(f"{src}: line 1 must be /* flags: <IDO flags> */")
        flags = header[1]
        if "-r4300_mul" not in flags:
            flags += " -Wab,-r4300_mul"
        jobs.append((src.stem, flags))
    subprocess.run(["ssh", builder, f"rm -rf {remote} && mkdir -p {remote}"],
                   check=True, capture_output=True)
    subprocess.run(["scp", "-q", *map(str, sources), f"{builder}:{remote}/"],
                   check=True, capture_output=True)
    script = f"T=~/rush2049/cache/toolkits/{toolkit}; cd {remote}; " + " ".join(
        f"$T/ido/cc -c {flags} -I $T/shim {name}.c -o {name}.o 2>{name}.err || echo FAIL {name};"
        for name, flags in jobs) + " tar czf objs.tgz *.o; echo DONE"
    proc = subprocess.run(["ssh", builder, script], capture_output=True, text=True)
    failed = [line.split()[1] for line in proc.stdout.splitlines() if line.startswith("FAIL ")]
    if failed or "DONE" not in proc.stdout:
        raise OvlRomError(f"image {image.upper()}: IDO failed for {failed or 'the batch'}")
    subprocess.run(["scp", "-q", f"{builder}:{remote}/objs.tgz", str(local / "objs.tgz")],
                   check=True, capture_output=True)
    subprocess.run(["tar", "xzf", str(local / "objs.tgz"), "-C", str(local)],
                   check=True, capture_output=True)
    objects = {}
    for name, _ in jobs:
        raw, out = local / f"{name}.o", local / f"{name}.text.o"
        rename = subprocess.run(
            [blob_splice.OBJCOPY, f"--rename-section=.text=.text.{name}",
             f"--set-section-alignment=.text.{name}=4",
             "-R", ".options", "-R", ".reginfo", "-R", ".mdebug", "-R", ".comment",
             "-R", ".pdr", "-R", ".MIPS.abiflags", str(raw), str(out)],
            capture_output=True, text=True)
        if rename.returncode:
            raise OvlRomError(f"{name}: objcopy failed: {rename.stderr.strip()[:200]}")
        objects[name] = out
    return objects


def load_targets(image):
    out = ovl_targets.OUT / f"ovl_{image}"
    extents = json.loads((out / "extents.json").read_text())
    symbols = {name: int(addr, 16) for name, addr in
               json.loads((out / "symbols.json").read_text())["symbols"].items()}
    words, current = {}, None
    for line in next(out.glob("*.s")).read_text().splitlines():
        m = re.match(r"\.section \.text\.(\S+?),", line)
        if m:
            current = words.setdefault(m.group(1), [])
            continue
        m = re.match(r"\s*\.word 0x([0-9A-Fa-f]{8})", line)
        if m and current is not None:
            current.append(int(m.group(1), 16))
    return extents, symbols, words


def compose(image, original, objects):
    extents, symbols, words = load_targets(image)
    base = int(extents["base"], 16)
    text_end = int(extents["text_end"], 16)
    built = bytearray(original)
    text = bytearray(text_end - base)
    functions = {f["name"]: f for f in extents["functions"]}
    for name, fn in functions.items():
        off = int(fn["address"], 16) - base
        text[off:off + fn["size"]] = b"".join(w.to_bytes(4, "big") for w in words[name])
    spliced = {}
    with tempfile.TemporaryDirectory(prefix="ovlrom-link-") as work:
        for name, obj in sorted(objects.items()):
            if name not in functions:
                raise OvlRomError(f"{name} is not a function of image {image.upper()}")
            fn = functions[name]
            try:
                body = blob_splice.link_function(obj, name, int(fn["address"], 16), fn["size"],
                                                 provides=symbols, work=Path(work) / name)
            except blob_build.BuildError as exc:
                raise OvlRomError(f"{name}: {exc}") from exc
            off = int(fn["address"], 16) - base
            text[off:off + fn["size"]] = body
            spliced[name] = body
    built[:len(text)] = text
    return bytes(built), spliced


def verify(image, sources_root=SOURCES):
    rom = ovl_targets.BASEROM.read_bytes()
    rom_offset = ovl_targets.image_rom_offset(rom, image)
    window = rom[rom_offset:rom_offset + 0x100000]   # may be cut short by the ROM's end
    z = zlib.decompressobj(-15)
    original = z.decompress(window)
    if not z.eof:
        raise OvlRomError(f"image {image.upper()}: ROM stream did not end")
    stream_len = len(window) - len(z.unused_data)
    objects = compile_remote(image, sources_for(image, sources_root))
    built, spliced = compose(image, original, objects)
    differing = [name for name, body in spliced.items()
                 if body != original[int(next(
                     f["address"] for f in load_targets(image)[0]["functions"]
                     if f["name"] == name), 16) - ovl_targets.BASE:][:len(body)]]
    image_ok = built == original
    cli = blob_rom.build_compressor()
    work = WORK / image
    work.mkdir(parents=True, exist_ok=True)
    (work / "image.bin").write_bytes(built)
    proc = subprocess.run([str(cli), str(work / "image.bin"), str(work / "image.deflate")],
                          capture_output=True, text=True)
    if proc.returncode:
        raise OvlRomError("deflate104 failed: " + proc.stderr[:200])
    stream = (work / "image.deflate").read_bytes()
    return {
        "image": image.upper(),
        "rom_offset": f"0x{rom_offset:06X}",
        "spliced": sorted(spliced),
        "differing_bodies": differing,
        "image_ok": image_ok,
        "image_sha256": hashlib.sha256(built).hexdigest(),
        "stream_bytes": len(stream),
        "stream_matches_rom": stream == rom[rom_offset:rom_offset + stream_len],
        "stream_sha256": hashlib.sha256(stream).hexdigest(),
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    v = sub.add_parser("verify")
    v.add_argument("--image", action="append", choices=sorted(ovl_targets.POINTERS))
    v.add_argument("--sources", default=str(SOURCES))
    args = parser.parse_args()
    ok = True
    for image in args.image or sorted(ovl_targets.POINTERS):
        try:
            r = verify(image, Path(args.sources))
        except OvlRomError as exc:
            print(f"image {image.upper()}: FAILED: {exc}")
            ok = False
            continue
        print(f"image {r['image']}: {len(r['spliced'])} C bodies spliced; image "
              f"{'byte-identical' if r['image_ok'] else 'DIFFERS'}; stream {r['stream_bytes']} B "
              f"{'== ROM @ ' + r['rom_offset'] if r['stream_matches_rom'] else 'DIFFERS from ROM'}")
        if r["differing_bodies"]:
            print("  differing bodies: " + ", ".join(r["differing_bodies"]))
        ok &= r["image_ok"] and r["stream_matches_rom"]
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
