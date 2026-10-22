"""Build the cartridge's game-code blob from sources (009, stage 2).

    python3 -m tools.conveyor.pipeline.blob_rom produce      # Pi: image -> blob
    python3 -m tools.conveyor.pipeline.blob_rom rom          # + builder ROM build + SHA-1
    python3 -m tools.conveyor.pipeline.blob_rom rom --drill  # prove the gate bites

`rom` first recompiles, on the builder, exactly the locked bodies whose
object in build/blob/obj is missing or bound to other source (after a rename;
`blob_splice rebuild-stale`, w24t), unless `--no-rebuild-stale`; `produce
--rebuild-stale` does the same. Provenance is still checked when composing.

`produce` links the game-code image from sources (008, gated on byte
identity), compresses the BUILT image with vendored zlib 1.0.4 at the
cartridge's exact parameters, and writes `build/blob/game_code.deflate`.
The Makefile composes that file into the ROM's data segment; the original
compressed bytes are never read (`tools/compose_data.py`).

`rom` syncs what the builder needs and runs the full ROM build plus `make
test`. The full-ROM SHA-1 is the authoritative gate. Because identical bytes
would flow through either path, that gate is only meaningful once it has been
shown to fail when the blob is wrong — which is what `--drill` does.
"""
import argparse
import hashlib
import subprocess
import sys
import tempfile
from pathlib import Path

from . import blob_build, blob_layout, blob_splice, blob_tu
from . import targets as targetsmod

REPO = targetsmod.REPO
ZLIB_DIR = REPO / "tools" / "zlib-1.0.4"
CLI_SOURCE = REPO / "tools" / "deflate104" / "deflate104.c"
CLI = REPO / "build" / "blob" / "deflate104"
BLOB_OUT = REPO / "build" / "blob" / "game_code.deflate"
# The runtime-image streams (pipeline.ovl_rom produce): the Makefile composes
# them unconditionally, so every ROM build needs them as it needs the blob.
OVL_STREAMS = {"a": REPO / "build" / "ovl" / "ovl_a.deflate",
               "b": REPO / "build" / "ovl" / "ovl_b.deflate"}
BASEROM = REPO / "baserom.us.z64"

ROM_OFFSET = 0xB0CB10
LENGTH = 326180
BUILDER = "watchman2"
BUILDER_REPO = "~/rush2049/repo"


class BlobRomError(RuntimeError):
    pass


def build_compressor(force=False):
    """Compile deflate104 against the vendored deflate half of zlib 1.0.4."""
    sources = [CLI_SOURCE] + [ZLIB_DIR / n for n in
                              ("deflate.c", "trees.c", "zutil.c", "adler32.c")]
    newest = max(p.stat().st_mtime for p in sources)
    if CLI.is_file() and not force and CLI.stat().st_mtime >= newest:
        return CLI
    CLI.parent.mkdir(parents=True, exist_ok=True)
    proc = subprocess.run(
        ["gcc", "-O2", "-w", "-I", str(ZLIB_DIR), "-o", str(CLI),
         *[str(s) for s in sources]], capture_output=True, text=True)
    if proc.returncode != 0:
        raise BlobRomError("building deflate104 failed: " + proc.stderr[:300])
    return CLI


def original_stream(baserom=BASEROM):
    return Path(baserom).read_bytes()[ROM_OFFSET:ROM_OFFSET + LENGTH]


def produce(out=BLOB_OUT, document=None):
    """Image from sources -> gate -> compress -> length + stream pre-flight.

    The image compressed is the one LINKED from sources, never
    build/game_code.bin: compressing the extracted image would make the whole
    pipeline decorative."""
    document = document or blob_layout.load()
    drift = blob_splice.check()
    if drift:
        raise BlobRomError("locked sources drifted — refusing to compose\n"
                           + "\n".join(f"  {t}: {why}" for t, why in drift))
    try:
        bodies = blob_splice.spliced_bodies(document=document)
    except blob_splice.LockedBodyError as exc:
        hint = ""
        if any("different" in why or "object missing" in why for why in exc.problems.values()):
            hint = ("\n  stale objects: `blob_splice rebuild-stale` (or `blob_rom produce "
                    "--rebuild-stale`) recompiles exactly those")
        raise BlobRomError(str(exc) + hint) from exc
    blob_tu.generate(document, spliced=bodies)
    work = Path(tempfile.mkdtemp(prefix="blobrom-"))
    ok, sha, message = blob_build.build(document, work_dir=work)
    if not ok:
        raise BlobRomError("image gate failed — refusing to compress\n" + message)
    image = (work / "image.bin").read_bytes()

    cli = build_compressor()
    out = Path(out)
    out.parent.mkdir(parents=True, exist_ok=True)
    proc = subprocess.run([str(cli), str(work / "image.bin"), str(out)],
                          capture_output=True, text=True)
    if proc.returncode != 0:
        raise BlobRomError("deflate104 failed: " + proc.stderr[:300])
    blob = out.read_bytes()
    if len(blob) != LENGTH:
        raise BlobRomError(f"compressed blob is {len(blob)} bytes, slot holds {LENGTH}")
    matches = blob == original_stream()
    return {
        "spliced": len(bodies),
        "unbound_objects": len(blob_splice.unbound_objects()),
        "image_sha256": sha,
        "image_bytes": len(image),
        "blob_bytes": len(blob),
        "blob_sha256": hashlib.sha256(blob).hexdigest(),
        "stream_matches_rom": matches,
        "path": out,
    }


def _ssh(command, timeout=3600):
    return subprocess.run(["ssh", "-o", "BatchMode=yes", BUILDER, command],
                          capture_output=True, text=True, timeout=timeout)


# What the builder's ROM build needs beyond its checkout (local, remote).
# rom_compilers.json and opt_overrides.mk select the per-TU compiler and
# flags; a stale copy on the builder would build the ROM with the wrong
# compiler and still match (passthrough TUs are compiler-neutral).
SYNC_FILES = (("Makefile", "Makefile"),
              ("tools/compose_data.py", "tools/compose_data.py"),
              ("rom_compilers.json", "rom_compilers.json"),
              ("src/rom/opt_overrides.mk", "src/rom/opt_overrides.mk"))


def missing_rom_inputs(blob=BLOB_OUT, streams=None):
    """The build products a ROM build needs that are absent locally."""
    streams = {**OVL_STREAMS, **(streams or {})}
    return [str(p) for p in [Path(blob)] + [Path(streams[k]) for k in sorted(streams)]
            if not p.is_file()]


def sync_to_builder(blob=BLOB_OUT, repo=BUILDER_REPO, streams=None):
    """What the builder's ROM build needs beyond its checkout: SYNC_FILES, the
    game blob and both runtime-image streams. Refuses (before syncing
    anything) when any of the three build products is absent: the Makefile
    has no rule for them and no fallback to the extracted bytes."""
    missing = missing_rom_inputs(blob, streams)
    if missing:
        raise BlobRomError("ROM inputs absent: " + ", ".join(missing) +
                           " (blob: blob_rom produce; streams: ovl_layout build, "
                           "then ovl_rom produce)")
    streams = {**OVL_STREAMS, **(streams or {})}
    for local, remote in SYNC_FILES:
        subprocess.run(["rsync", "-az", str(REPO / local),
                        f"{BUILDER}:{repo}/{remote}"], check=True)
    _ssh(f"mkdir -p {repo}/build/blob {repo}/build/ovl")
    subprocess.run(["rsync", "-az", str(blob),
                    f"{BUILDER}:{repo}/build/blob/game_code.deflate"],
                   check=True)
    for key in sorted(streams):
        subprocess.run(["rsync", "-az", str(streams[key]),
                        f"{BUILDER}:{repo}/build/ovl/ovl_{key}.deflate"], check=True)


def builder_rom_build(repo=BUILDER_REPO, make_args="-j16", log="/tmp/blobrom"):
    """Full ROM build + `make test` on the builder. Returns (ok, tail)."""
    # Exit codes are captured BEFORE anything is piped: `make test | tail`
    # followed by `$?` reports tail's status — always 0 — which would make
    # this gate pass no matter what the ROM hashed to (the 004 lesson, nearly
    # repeated). Success also requires the positive "ROM matches!" line.
    proc = _ssh(f"cd {repo} && rm -f build/us/assets/us/data.o "
                f"build/us/assets/us/data.composed.bin build/us/*.z64 && "
                f"make COMPILER=ido {make_args} >{log}-make.log 2>&1; M=$?; "
                f"make test {make_args} >{log}-test.log 2>&1; T=$?; "
                f"tail -3 {log}-make.log; tail -2 {log}-test.log; "
                f"echo MAKE=$M TEST=$T")
    out = proc.stdout
    ok = ("MAKE=0 TEST=0" in out and "ROM matches!" in out
          and "does NOT match" not in out)
    return ok, out.strip()


def rebuild_stale_objects(log=print):
    """Before composing: recompile exactly the locked bodies whose object is
    missing or bound to other source (after a libultra/ll.c rename, the
    renamed game callers' objects in build/blob/obj; w23m). The provenance
    check in spliced_bodies is unchanged; a body that is still stale after
    the rebuild (its source drifted from the lock) is refused there."""
    try:
        return blob_splice.rebuild_stale(log=log)
    except blob_splice.LockedBodyError as exc:
        raise BlobRomError(str(exc)) from exc


def rom(drill=False, rebuild_stale=True):
    absent = [str(p) for _, p in sorted(OVL_STREAMS.items()) if not p.is_file()]
    if absent:
        raise BlobRomError("runtime-image streams absent: " + ", ".join(absent) +
                           " — run ovl_layout build and ovl_rom produce first "
                           "(the ROM build composes them; there is no fallback)")
    if rebuild_stale:
        rebuild_stale_objects()
    result = produce()
    if not result["stream_matches_rom"]:
        raise BlobRomError("pre-flight: compressed blob differs from the ROM's stream; "
                           "the ROM SHA-1 cannot pass — refusing to spend a build")
    sync_to_builder()
    ok, tail = builder_rom_build()
    report = {"produce": result, "rom_ok": ok, "rom_tail": tail}
    if drill:
        corrupted = bytearray(result["path"].read_bytes())
        corrupted[LENGTH // 2] ^= 0x01
        with tempfile.NamedTemporaryFile(suffix=".deflate", delete=False) as handle:
            handle.write(bytes(corrupted))
            bad = Path(handle.name)
        sync_to_builder(bad)
        drill_ok, drill_tail = builder_rom_build()
        # A drill that failed for the wrong reason proves nothing.
        drill_ok = drill_ok or "does NOT match" not in drill_tail
        sync_to_builder()                     # restore the real blob
        restored_ok, _ = builder_rom_build()
        report.update(drill_rom_passed=drill_ok, drill_tail=drill_tail,
                      restored_ok=restored_ok)
    return report


def cartridge_coverage():
    """Static code promoted into ROM TUs + game code spliced into the image.

    Both halves are cartridge coverage since 009: the ROM's game-code blob is
    compressed from the linked image, and the full-ROM SHA-1 gates the result
    (proven by the drill). Denominators stay separate — they are different
    populations and summing them would hide how thin each still is."""
    from . import layout as layoutmod

    from . import asm_lock, boot_data, ovl_splice

    static = layoutmod.coverage()
    # Static functions that were assembly in the original build, locked in
    # asm_matched.lock.json (owner policy (b), 2026-10-21): reported beside C.
    asm = asm_lock.static_coverage()
    # Hand-written CPU routines still inside the ROM data bin (pipeline.boot_data):
    # static functions no source builds yet, so they join the denominator only.
    boot = boot_data.coverage()
    game = blob_splice.coverage()
    return {
        # Runtime images A/B (pipeline.ovl_splice): their own rows and
        # denominators, never summed into static or game.
        "overlays": ovl_splice.coverage(),
        "static_functions": static["promoted_functions"],
        "static_total": static["static_functions"] + boot["functions"],
        "static_bytes": static["promoted_bytes"],
        "static_total_bytes": static["static_bytes"] + boot["bytes"],
        "static_in_data_bin": boot["functions"] - asm.get("boot_functions", 0),
        "static_asm_functions": asm["functions"],
        "static_asm_bytes": asm["bytes"],
        "game_functions": game["functions"],
        "game_total": game["total_functions"],
        "game_bytes": game["bytes"],
        "game_total_bytes": game["image_size"],
        # The image's tail after the last function is proven .data/.rodata
        # (blob_layout.DATA_SECTIONS), so game code is reported against its
        # code bytes; the whole-image figure follows it.
        "game_function_bytes": game["function_bytes"],
    }


def print_coverage(cov):
    total_fn = cov["static_functions"] + cov["game_functions"]
    total_b = cov["static_bytes"] + cov["game_bytes"]
    in_bin = cov.get("static_in_data_bin")
    asm_f, asm_b = cov.get("static_asm_functions", 0), cov.get("static_asm_bytes", 0)
    total = cov["static_total_bytes"]
    split = (f": from C {cov['static_functions']} / {100 * cov['static_bytes'] / total:.2f}%, "
             f"from assembly {asm_f} / {100 * asm_b / total:.2f}%") if asm_f else ""
    print(f"  static code: {cov['static_functions'] + asm_f:4d}/{cov['static_total']} functions, "
          f"{cov['static_bytes'] + asm_b}/{total} bytes "
          f"({100 * (cov['static_bytes'] + asm_b) / total:.2f}%{split}"
          + (f"; {in_bin} hand-written routines still in the ROM data bin, see boot_data"
             if in_bin else "") + ")")
    print(f"  game code:   {cov['game_functions']:4d}/{cov['game_total']} functions, "
          f"{cov['game_bytes']}/{cov['game_function_bytes']} code bytes "
          f"({100 * cov['game_bytes'] / cov['game_function_bytes']:.2f}%; "
          f"{100 * cov['game_bytes'] / cov['game_total_bytes']:.2f}% of the "
          f"{cov['game_total_bytes']} B image) "
          f"— compressed into the ROM at 0x{ROM_OFFSET:X}")
    if cov.get("overlays"):
        from . import ovl_splice
        ovl_splice.print_coverage(cov["overlays"])
    print(f"  linked from C: {total_fn} functions, {total_b} bytes (static + game)")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("produce")
    p.add_argument("--rebuild-stale", action="store_true",
                   help="first recompile stale build/blob/obj objects on the builder")
    sub.add_parser("coverage")
    p = sub.add_parser("rom")
    p.add_argument("--drill", action="store_true")
    p.add_argument("--no-rebuild-stale", action="store_true",
                   help="refuse on stale objects instead of recompiling them first")
    args = parser.parse_args()
    if args.command == "coverage":
        print_coverage(cartridge_coverage())
        return 0
    try:
        if args.command == "produce":
            if args.rebuild_stale:
                rebuild_stale_objects()
            r = produce()
            print(f"blob_rom: image {r['image_bytes']} B ({r['spliced']} spliced) -> "
                  f"blob {r['blob_bytes']} B sha256 {r['blob_sha256'][:16]}…")
            print(f"  byte-identical to the ROM's stream: {r['stream_matches_rom']}")
            if r["unbound_objects"]:
                print(f"  {r['unbound_objects']} locked bodies come from objects built "
                      "before provenance records; rebuild them to bind source")
            print(f"  -> {r['path']}")
            return 0 if r["stream_matches_rom"] else 1
        r = rom(drill=args.drill, rebuild_stale=not args.no_rebuild_stale)
    except BlobRomError as exc:
        sys.exit(f"blob_rom FAILED: {exc}")
    print(f"ROM build with the source-built blob: {'SHA-1 EXACT' if r['rom_ok'] else 'FAILED'}")
    print("  " + r["rom_tail"].replace("\n", "\n  "))
    if args.drill:
        print(f"drill (one bit flipped in the blob): ROM "
              f"{'PASSED — THE GATE IS VACUOUS' if r['drill_rom_passed'] else 'failed, as it must'}")
        # Show HOW it failed: a broken build also "fails", and would prove far
        # less than a hash mismatch does.
        print("  " + r["drill_tail"].replace("\n", "\n  "))
        print(f"restored: {'SHA-1 EXACT' if r['restored_ok'] else 'FAILED'}")
        return 0 if (r["rom_ok"] and not r["drill_rom_passed"] and r["restored_ok"]) else 1
    return 0 if r["rom_ok"] else 1


if __name__ == "__main__":
    sys.exit(main())
