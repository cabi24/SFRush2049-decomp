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

Composing into the cartridge (w18b):

    python3 -m tools.conveyor.pipeline.ovl_rom produce [--image-file a=PATH ...]
    python3 -m tools.conveyor.pipeline.ovl_rom rom [--drill] [--builder-repo DIR] [--jobs N]

`produce` compresses each image with deflate104 (ovl_image.deflate), refuses
a stream that does not fill its fixed slot exactly, and writes
build/ovl/ovl_<x>.deflate. The image comes from `--image-file` (the layout/
splice lane's rebuilt image); without one, the retail image inflated from the
ROM is used as a labelled stand-in, which proves the round trip but not that
anything was rebuilt. `rom` additionally refuses unless both streams equal
the ROM's (the pre-flight), syncs what the builder's ROM build needs
(blob_rom.SYNC_FILES, the game blob and both streams), and runs the full ROM
build with OVL_COMPOSE=1 plus `make test`. `--drill` then changes one byte
of each image (keeping the stream length, so compose accepts it) and requires
the ROM hash -- not the build -- to fail, then restores and rebuilds.
"""
import argparse
import hashlib
import json
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from . import blob_build, blob_rom, blob_splice, ovl_image

REPO = ovl_image.REPO
SOURCES = REPO / "cloud" / "matches"
WORK = ovl_image.OUT
# verify's own products: never build/ovl/<x>/ (ovl_splice's locked objects
# and the extracted image live there).
VERIFY_WORK = WORK / "verify"
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
    local = VERIFY_WORK / image / "obj"
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
    out = ovl_image.target_dir(image)
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
    rom = ovl_image.BASEROM.read_bytes()
    rom_offset = ovl_image.stream(image).rom_offset
    original = ovl_image.inflate(image, rom)        # slot end + sha256 gated
    objects = compile_remote(image, sources_for(image, sources_root))
    built, spliced = compose(image, original, objects)
    differing = [name for name, body in spliced.items()
                 if body != original[int(next(
                     f["address"] for f in load_targets(image)[0]["functions"]
                     if f["name"] == name), 16) - ovl_image.BASE:][:len(body)]]
    image_ok = built == original
    work = VERIFY_WORK / image
    work.mkdir(parents=True, exist_ok=True)
    (work / "image.bin").write_bytes(built)
    stream = ovl_image.deflate(built)
    (work / "image.deflate").write_bytes(stream)
    return {
        "image": image.upper(),
        "rom_offset": f"0x{rom_offset:06X}",
        "spliced": sorted(spliced),
        "differing_bodies": differing,
        "image_ok": image_ok,
        "image_sha256": hashlib.sha256(built).hexdigest(),
        "stream_bytes": len(stream),
        "stream_matches_rom": stream == ovl_image.rom_stream(image, rom),
        "stream_sha256": hashlib.sha256(stream).hexdigest(),
    }


# --- composing the streams into the cartridge -----------------------------------

def stream_path(image):
    """Where the Makefile's OVL_COMPOSE=1 build reads image `image`'s stream."""
    return WORK / f"ovl_{image}.deflate"


def produce(image_files=None, out=None):
    """{image: report}. Compresses each image and writes its stream.

    `image_files` maps 'a'/'b' to a rebuilt image; a missing entry uses the
    retail image (inflated from the ROM) as a stand-in and says so."""
    image_files = image_files or {}
    unknown = set(image_files) - set(ovl_image.STREAMS)
    if unknown:
        raise OvlRomError(f"unknown image(s): {', '.join(sorted(unknown))}")
    reports = {}
    for key in sorted(ovl_image.STREAMS):
        s = ovl_image.stream(key)
        if key in image_files:
            image, source = Path(image_files[key]).read_bytes(), str(image_files[key])
        else:
            image, source = ovl_image.inflate(key), "retail image (stand-in)"
        stream = ovl_image.deflate(image)
        if len(stream) != s.length:
            raise OvlRomError(f"image {s.image}: stream is {len(stream)} bytes; the slot at "
                              f"0x{s.rom_offset:X} holds exactly {s.length} — refusing")
        path = Path(out) / f"ovl_{key}.deflate" if out else stream_path(key)
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(stream)
        reports[key] = {
            "image": s.image, "source": source, "path": path,
            "image_sha256": hashlib.sha256(image).hexdigest(),
            "image_is_retail": hashlib.sha256(image).hexdigest() == s.image_sha256,
            "stream_bytes": len(stream),
            "stream_sha256": hashlib.sha256(stream).hexdigest(),
            "stream_matches_rom": stream == ovl_image.rom_stream(key),
        }
    return reports


def sync_streams(repo, streams=None):
    """Sync the shared ROM-build inputs, the game blob and both streams."""
    if not blob_rom.BLOB_OUT.is_file():
        raise OvlRomError(f"{blob_rom.BLOB_OUT} is absent: run blob_rom produce first")
    blob_rom.sync_to_builder(repo=repo)
    blob_rom._ssh(f"mkdir -p {repo}/build/ovl")
    for key in sorted(ovl_image.STREAMS):
        src = (streams or {}).get(key, stream_path(key))
        subprocess.run(["rsync", "-az", str(src),
                        f"{BUILDER}:{repo}/build/ovl/ovl_{key}.deflate"], check=True)


def length_preserving_change(key, image):
    """(offset, stream): a one-byte change to `image` whose stream still fills
    the slot, so compose accepts it and only the ROM hash can catch it."""
    s = ovl_image.stream(key)
    for offset in range(len(image) // 2, len(image), 4):
        changed = bytearray(image)
        changed[offset] ^= 0x01
        stream = ovl_image.deflate(bytes(changed))
        if len(stream) == s.length:
            return offset, stream
    raise OvlRomError(f"image {s.image}: no length-preserving one-byte change found")


def rom(image_files=None, drill=False, repo=blob_rom.BUILDER_REPO, jobs=16):
    produced = produce(image_files)
    differing = [r["image"] for r in produced.values() if not r["stream_matches_rom"]]
    if differing:
        raise OvlRomError(f"pre-flight: stream(s) for image {', '.join(differing)} differ from "
                          "the ROM's; the ROM SHA-1 cannot pass — refusing to spend a build")
    make_args, log = f"-j{jobs} OVL_COMPOSE=1", f"{repo}/build/ovlrom"
    sync_streams(repo)
    ok, tail = blob_rom.builder_rom_build(repo, make_args, log)
    report = {"produce": produced, "rom_ok": ok, "rom_tail": tail, "drills": {}}
    if drill:
        with tempfile.TemporaryDirectory(prefix="ovlrom-drill-") as work:
            for key in sorted(ovl_image.STREAMS):
                image = (Path(image_files[key]).read_bytes() if image_files and key in image_files
                         else ovl_image.inflate(key))
                offset, bad = length_preserving_change(key, image)
                bad_path = Path(work) / f"ovl_{key}.deflate"
                bad_path.write_bytes(bad)
                sync_streams(repo, {key: bad_path})
                passed, drill_tail = blob_rom.builder_rom_build(repo, make_args, log)
                # A drill that failed for the wrong reason proves nothing.
                vacuous = passed or "does NOT match" not in drill_tail
                report["drills"][key] = {"image_offset": offset, "rom_passed": vacuous,
                                         "tail": drill_tail}
        sync_streams(repo)                    # restore the real streams
        report["restored_ok"], _ = blob_rom.builder_rom_build(repo, make_args, log)
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    v = sub.add_parser("verify")
    v.add_argument("--image", action="append", choices=sorted(ovl_image.STREAMS))
    v.add_argument("--sources", default=str(SOURCES))
    for name in ("produce", "rom"):
        p = sub.add_parser(name)
        p.add_argument("--image-file", action="append", default=[], metavar="X=PATH",
                       help="rebuilt image for x (a or b); default: retail stand-in")
        if name == "rom":
            p.add_argument("--drill", action="store_true")
            p.add_argument("--builder-repo", default=blob_rom.BUILDER_REPO)
            p.add_argument("--jobs", type=int, default=16)
    args = parser.parse_args()
    if args.command in ("produce", "rom"):
        return compose_main(args)
    ok = True
    for image in args.image or sorted(ovl_image.STREAMS):
        try:
            r = verify(image, Path(args.sources))
        except (OvlRomError, ovl_image.OvlImageError) as exc:
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


def compose_main(args):
    try:
        files = dict(item.split("=", 1) for item in args.image_file)
    except ValueError:
        sys.exit("ovl_rom: --image-file takes X=PATH")
    try:
        if args.command == "produce":
            produced, r = produce(files), None
        else:
            r = rom(files, args.drill, args.builder_repo, args.jobs)
            produced = r["produce"]
    except (OvlRomError, ovl_image.OvlImageError) as exc:
        sys.exit(f"ovl_rom FAILED: {exc}")
    for p in produced.values():
        print(f"image {p['image']}: {p['source']} -> {p['stream_bytes']} B stream "
              f"{p['stream_sha256'][:16]}… "
              f"{'== ROM' if p['stream_matches_rom'] else 'DIFFERS from ROM'} -> {p['path']}")
    if r is None:
        return 0 if all(p["stream_matches_rom"] for p in produced.values()) else 1
    print("ROM build with both runtime-image streams composed: "
          + ("SHA-1 EXACT" if r["rom_ok"] else "FAILED"))
    print("  " + r["rom_tail"].replace("\n", "\n  "))
    ok = r["rom_ok"]
    for key, d in sorted(r["drills"].items()):
        print(f"drill image {key.upper()} (image byte +0x{d['image_offset']:X} flipped): ROM "
              + ("PASSED — THE GATE IS VACUOUS" if d["rom_passed"] else "failed, as it must"))
        # Show HOW it failed: a broken build also "fails".
        print("  " + d["tail"].replace("\n", "\n  "))
        ok &= not d["rom_passed"]
    if args.drill:
        print("restored: " + ("SHA-1 EXACT" if r["restored_ok"] else "FAILED"))
        ok &= r["restored_ok"]
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
