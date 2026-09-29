"""Splice verified C into the game-code image (008, stage 1, phase 3).

    python3 -m tools.conveyor.pipeline.blob_splice splice <target> [--from PATH]
    python3 -m tools.conveyor.pipeline.blob_splice splice --all-matched
    python3 -m tools.conveyor.pipeline.blob_splice revert <target>
    python3 -m tools.conveyor.pipeline.blob_splice coverage

A spliced function is compiled by IDO on the builder into its own object,
whose `.text` is renamed to the section the linker script already places at
that function's image address. The region's passthrough for it is dropped, so
exactly one definition exists.

The image gate is the verification. A stored score of 0 is evidence that a
body *should* be byte-exact; the gate proves it is, in place, with its
neighbours around it — so a splice is committed only if the whole image still
hashes identically, and otherwise the tree is restored exactly (contract §14).

Coverage from this feature is IMAGE coverage. The cartridge still embeds the
original compressed stream; nothing here promotes anything into the ROM
(contract §16, 005 FR-010).
"""
import argparse
import hashlib
import json
import re
import shutil
import subprocess
import tempfile
import sys
import time
from pathlib import Path

from ..client import DEFAULT_DATA
from . import blob_build, blob_layout, blob_tu
from . import targets as targetsmod

REPO = targetsmod.REPO
SRC_DIR = REPO / "src" / "blob"
OBJ_DIR = REPO / "build" / "blob" / "obj"
LOCKFILE = REPO / "blob_matched.lock.json"
BUILDER = blob_build.__dict__.get("BUILDER") or "watchman2"
BUILDER_TMP = "/tmp/blobsplice"
TOOLKIT = "796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5"
OBJCOPY = "mips-linux-gnu-objcopy"


def load_lock(path=LOCKFILE):
    try:
        return json.loads(Path(path).read_text())
    except FileNotFoundError:
        return {}


def save_lock(entries, path=LOCKFILE):
    Path(path).write_text(json.dumps(entries, indent=2, sort_keys=True) + "\n")


def source_sha(text):
    return hashlib.sha256(text.encode() if isinstance(text, str) else text).hexdigest()


def spliced_targets(lock=None):
    return set((lock if lock is not None else load_lock()).keys())


def verified_flagsets(conn, target_ids):
    """{target_id: flagset that scored 0} from sweep evidence.

    A body matches under ONE optimization level — audio_distance_atten scores
    0 at -O2 and 745 at -O1, others the reverse — so splicing everything at
    the default silently guarantees a mismatch for half the population."""
    if not target_ids:
        return {}
    placeholders = ",".join("?" for _ in target_ids)
    rows = conn.execute(
        f"SELECT target_id, flagset, score FROM matrix_entry"
        f" WHERE candidate_id LIKE 'm2c:%' AND score = 0"
        f" AND target_id IN ({placeholders}) ORDER BY target_id, flagset",
        tuple(target_ids)).fetchall()
    return {row["target_id"]: row["flagset"] for row in rows}


def compile_on_builder(sources, flagset, builder=BUILDER, toolkit=TOOLKIT,
                       obj_dir=OBJ_DIR):
    """Compile {target_id: Path} with IDO on the builder, rename each object's
    .text to its placed section, and return {target_id: object path}.

    Batched into one round trip: per-function ssh would dominate the runtime
    of a 76-function splice."""
    obj_dir = Path(obj_dir)
    obj_dir.mkdir(parents=True, exist_ok=True)
    if not sources:
        return {}, {}
    staging = Path(blob_build.BUILD_DIR) / "staging"
    if staging.exists():
        shutil.rmtree(staging)
    staging.mkdir(parents=True)
    for target_id, path in sources.items():
        shutil.copy(path, staging / f"{target_id}.c")

    subprocess.run(["ssh", builder, f"rm -rf {BUILDER_TMP} && mkdir -p {BUILDER_TMP}"],
                   check=True, capture_output=True)
    subprocess.run(["scp", "-q", "-r", *[str(p) for p in staging.glob("*.c")],
                    f"{builder}:{BUILDER_TMP}/"], check=True, capture_output=True)
    script = (
        f"T=~/rush2049/cache/toolkits/{toolkit}; cd {BUILDER_TMP}; "
        f"for f in *.c; do n=${{f%.c}}; "
        f"$T/ido/cc -c {flagset} -I $T/shim $f -o $n.o 2>$n.err || echo \"FAIL $n\"; "
        f"done; tar czf objs.tgz *.o 2>/dev/null; echo DONE")
    proc = subprocess.run(["ssh", builder, script], capture_output=True, text=True)
    failures = {line.split()[1] for line in proc.stdout.splitlines()
                if line.startswith("FAIL ")}
    subprocess.run(["scp", "-q", f"{builder}:{BUILDER_TMP}/objs.tgz",
                    str(staging / "objs.tgz")], check=True, capture_output=True)
    subprocess.run(["tar", "xzf", str(staging / "objs.tgz"), "-C", str(staging)],
                   check=True, capture_output=True)

    objects = {}
    for target_id in sources:
        raw = staging / f"{target_id}.o"
        if target_id in failures or not raw.is_file():
            failures.add(target_id)
            continue
        out = obj_dir / f"{target_id}.o"
        # IDO emits .reginfo (LINK_ONCE), .options and .mdebug. GNU ld
        # SEGFAULTS merging them into a script-driven link rather than
        # reporting anything, so they are stripped here, where the failure is
        # attributable, instead of being discarded in the linker script.
        rename = subprocess.run(
            [OBJCOPY, f"--rename-section=.text=.text.{target_id}",
             # IDO aligns .text to 16 bytes. Concatenated at image addresses
             # that is not padding, it is corruption: a 72-byte function pads
             # to 80 and shoves every later function 8 bytes late. Instruction
             # alignment is 4.
             f"--set-section-alignment=.text.{target_id}=4",
             "-R", ".options", "-R", ".reginfo", "-R", ".mdebug",
             "-R", ".comment", "-R", ".pdr", "-R", ".MIPS.abiflags",
             str(raw), str(out)],
            capture_output=True, text=True)
        if rename.returncode != 0:
            failures.add(target_id)
            continue
        objects[target_id] = out
    return objects, {t: "compile failed under IDO" for t in sorted(failures)}


def link_function(object_path, target_id, vaddr, size, provides=None, work=None):
    """Bytes a compiled function contributes at its image address.

    Linked alone so its relocations resolve against the PROVIDE table (data
    globals inside opaque runs, and libultra/libc out in the cartridge), then
    cut to the extent only after verifying excess words are zero padding.
    IDO pads .text to 16 bytes; a following function bounds that padding.
    `provides` is {name: address}: several names may
    share an address (a data-symbol label and the function's own target_id),
    and every one of them must resolve."""
    if provides is None:
        provides = {name: addr for addr, name in blob_tu.data_symbols().items()}
    provides = dict(provides)
    for name in undefined_symbols(object_path):
        addr = address_named(name)
        if name not in provides and addr is not None:
            provides[name] = addr
    work = Path(work or tempfile.mkdtemp(prefix="blobfn-"))
    work.mkdir(parents=True, exist_ok=True)
    script = work / f"{target_id}.ld"
    provides = "\n".join(f"    PROVIDE({name} = 0x{addr:08X});"
                          for name, addr in sorted(provides.items())
                          if name != target_id)
    script.write_text(
        "SECTIONS\n{\n"
        f"    . = 0x{vaddr:08X};\n"
        f"    .out : {{ *(.text.{target_id}) }}\n"
        f"{provides}\n"
        "    /DISCARD/ : { *(.pdr) *(.mdebug*) *(.comment) *(.note*)"
        " *(.reginfo) *(.options) *(.MIPS.abiflags) }\n}\n")
    elf = work / f"{target_id}.elf"
    proc = subprocess.run(
        [blob_build.LD, "-T", str(script), "--no-warn-rwx-segments",
         "-o", str(elf), str(object_path)], capture_output=True, text=True)
    if proc.returncode != 0:
        raise blob_build.BuildError(
            "link failed: " + " ".join(proc.stderr.split())[:220])
    binary = work / f"{target_id}.bin"
    dump = subprocess.run(
        [blob_build.OBJCOPY, "-O", "binary", "--only-section=.out",
         str(elf), str(binary)], capture_output=True, text=True)
    if dump.returncode != 0:
        raise blob_build.BuildError("objcopy failed: " + dump.stderr.strip()[:200])
    data = binary.read_bytes()
    # Use input-section offsets: the linker can align .out above vaddr, while
    # objcopy emits only its contents (without that leading address gap).
    functions = blob_build.function_symbols(object_path)
    if target_id not in functions:
        raise blob_build.BuildError(f"{target_id}: no defined function symbol")
    start, section = functions[target_id]
    if start != 0:
        raise blob_build.BuildError(f"{target_id}: function does not start its input section")
    end = min((value - start for value, ndx in functions.values()
               if ndx == section and value > start), default=len(data))
    end = min(end, len(data))
    if end < size:
        raise blob_build.BuildError(
            f"compiled body is {end} bytes, shorter than the "
            f"{size}-byte extent")
    if size % 4 or end % 4:
        raise blob_build.BuildError(f"{target_id}: function extent is not word aligned")
    extra = sum(any(data[offset:offset + 4]) for offset in range(size, end, 4))
    if extra:
        raise blob_build.BuildError(f"{target_id}: {extra} extra words "
                                    "(nonzero beyond target length)")
    return data[:size]


_ADDRESS_NAME = re.compile(r"^(?:func|D)_([0-9A-Fa-f]{8})$")


def address_named(name):
    """The address an m2c-style `func_XXXXXXXX` / `D_XXXXXXXX` name encodes.

    m2c names an unknown callee or global by its address, so the name IS the
    address — including callees outside the image (static ROM, or the
    0x8038xxxx overlay range) that no table knows."""
    match = _ADDRESS_NAME.match(name)
    return int(match.group(1), 16) if match else None


def undefined_symbols(object_path):
    """Names the object references but does not define."""
    proc = subprocess.run([blob_build.NM, "-u", str(object_path)],
                          capture_output=True, text=True)
    if proc.returncode != 0:
        raise blob_build.BuildError("nm failed: " + proc.stderr.strip()[:200])
    return [line.split()[-1] for line in proc.stdout.splitlines()
            if line.strip()]


def image_symbols(document=None, symbols=None):
    """{name: address} for everything a standalone function link must resolve.

    The whole-image link resolves calls between game functions because every
    region defines its own `.globl`s. A function linked ALONE sees only
    itself, so its calls to siblings (func_80091B00, entity_flags_apply, …)
    are undefined — 37 of 42 refusals on the first full run. The map knows
    every function's address. Keyed by NAME: a data-symbol label often sits
    at a function's address (`frame_sync` at entity_flags_apply's 0x80092360),
    and an address-keyed table silently kept only one of the two."""
    document = document or blob_layout.load()
    symbols = blob_tu.data_symbols() if symbols is None else symbols
    provides = {name: addr for addr, name in symbols.items()}
    for region in document["regions"]:
        for entry in region["entries"]:
            if entry["kind"] == "function":
                provides[entry["target_id"]] = entry["vaddr"]
    return provides


def spliced_bodies(lock=None, document=None, symbols=None):
    """{target_id: bytes} for everything in the lock with a built object."""
    lock = load_lock() if lock is None else lock
    if not lock:
        return {}
    document = document or blob_layout.load()
    extents = {e["target_id"]: e for region in document["regions"]
               for e in region["entries"] if e["kind"] == "function"}
    symbols = image_symbols(document, symbols)
    bodies = {}
    groups = sorted({e["group"] for e in lock.values() if e.get("group")})
    if groups:
        # IPA call groups: members are slices of one whole-program object,
        # each relocated to its own image address (010 Phase 4).
        from . import blob_group
        for group in groups:
            try:
                built = blob_group.group_bodies(group, document, symbols)
            except blob_group.GroupError:
                continue
            bodies.update({t: b for t, b in built.items()
                           if lock.get(t, {}).get("group") == group})
    for target_id in lock:
        if lock[target_id].get("group"):
            continue
        obj = OBJ_DIR / f"{target_id}.o"
        entry = extents.get(target_id)
        if not obj.is_file() or entry is None:
            continue
        try:
            bodies[target_id] = link_function(obj, target_id, entry["vaddr"],
                                              entry["size"], symbols)
        except blob_build.BuildError:
            continue
    return bodies


def _regenerate(document=None):
    blob_tu.generate(document or blob_layout.load())


def build_with(bodies, document=None, **kwargs):
    """Regenerate regions with these spliced bodies and gate the image."""
    blob_tu.generate(document, spliced=bodies)
    return blob_build.build(document, **kwargs)


def splice(conn, target_ids, source_for, flagset=None, document=None,
           lockfile=LOCKFILE, dry_run=False, flagsets=None):
    """Splice targets one at a time, each behind the image gate.

    Sequential rather than batched on purpose: a batch that fails tells you
    nothing about which body broke it."""
    document = document or blob_layout.load()
    flagset = flagset or blob_layout.DEFAULT_FLAGSET
    flagsets = verified_flagsets(conn, list(target_ids)) if flagsets is None else flagsets
    lock = load_lock(lockfile)
    SRC_DIR.mkdir(parents=True, exist_ok=True)

    wanted = {}
    for target_id in target_ids:
        try:
            text = source_for(target_id)
        except Exception as exc:
            wanted[target_id] = exc
            continue
        wanted[target_id] = text
    sources, refused = {}, {}
    for target_id, text in wanted.items():
        if not isinstance(text, str):
            refused[target_id] = f"no source: {text}"
            continue
        path = SRC_DIR / f"{target_id}.c"
        path.write_text(text)
        sources[target_id] = path

    groups = {}
    for target_id, path in sources.items():
        groups.setdefault(flagsets.get(target_id, flagset), {})[target_id] = path
    objects, compile_failures = {}, {}
    for group_flags, group in sorted(groups.items()):
        built, failed = compile_on_builder(group, group_flags)
        objects.update(built)
        compile_failures.update(failed)
    refused.update(compile_failures)
    for target_id in compile_failures:
        (SRC_DIR / f"{target_id}.c").unlink(missing_ok=True)
        sources.pop(target_id, None)

    extents = {e["target_id"]: e for region in document["regions"]
               for e in region["entries"] if e["kind"] == "function"}
    symbols = image_symbols(document)
    accepted = {}
    for target_id in sorted(objects):
        entry = extents.get(target_id)
        if entry is None:
            refused[target_id] = "not in the layout map"
            continue
        try:
            body = link_function(objects[target_id], target_id, entry["vaddr"],
                                 entry["size"], symbols)
        except blob_build.BuildError as exc:
            refused[target_id] = str(exc)[:220]
            (SRC_DIR / f"{target_id}.c").unlink(missing_ok=True)
            continue
        candidate = dict(accepted)
        candidate[target_id] = body
        lock_candidate = dict(lock)
        lock_candidate[target_id] = {
            "source": f"src/blob/{target_id}.c",
            "source_sha256": source_sha(sources[target_id].read_text()),
            "flagset": flagsets.get(target_id, flagset), "toolkit_sha": TOOLKIT,
            "verified": "image_gate", "verified_at": time.strftime("%Y-%m-%d"),
        }
        save_lock(lock_candidate, lockfile)
        try:
            ok, _sha, message = build_with(candidate, document)
        except blob_build.BuildError as exc:
            # A body that cannot link (an unresolved symbol, usually) is a
            # refusal for THAT function — never an abort of the run. The
            # contract requires refusals to be reported with their reason.
            ok, message = False, str(exc).replace("\n", " ")[:300]
        if ok:
            accepted = candidate
            lock = lock_candidate
        else:
            refused[target_id] = message.splitlines()[0]
            (SRC_DIR / f"{target_id}.c").unlink(missing_ok=True)
            save_lock(lock, lockfile)
    save_lock(lock, lockfile)
    try:
        ok, sha, message = build_with(accepted, document)
    except blob_build.BuildError as exc:
        ok, sha, message = False, "", str(exc)
    return {"spliced": sorted(accepted), "refused": refused,
            "image_ok": ok, "image_sha": sha, "message": message}


def revert(target_ids, document=None, lockfile=LOCKFILE):
    document = document or blob_layout.load()
    lock = load_lock(lockfile)
    removed = []
    for target_id in target_ids:
        if lock.pop(target_id, None) is not None:
            (SRC_DIR / f"{target_id}.c").unlink(missing_ok=True)
            (OBJ_DIR / f"{target_id}.o").unlink(missing_ok=True)
            removed.append(target_id)
    save_lock(lock, lockfile)
    for stale in OBJ_DIR.glob("*.o"):
        if stale.stem not in lock:
            stale.unlink()
    ok, sha, message = build_with(spliced_bodies(lock, document), document)
    return {"reverted": removed, "image_ok": ok, "image_sha": sha,
            "message": message}


def check(lockfile=LOCKFILE):
    """Refuse a body whose source drifted from its locked hash."""
    problems = []
    for target_id, entry in sorted(load_lock(lockfile).items()):
        if entry.get("group"):
            continue                  # checked per group below
        path = REPO / entry["source"]
        if not path.is_file():
            problems.append((target_id, "source missing"))
            continue
        if source_sha(path.read_text()) != entry["source_sha256"]:
            problems.append((target_id, "source hash drifted"))
    from . import blob_group
    problems += [(f"group:{g}", why) for g, why in blob_group.check(lockfile)]
    return problems


def coverage(document=None, lockfile=LOCKFILE):
    document = document or blob_layout.load()
    lock = load_lock(lockfile)
    size = document["image"]["size"]
    linked = [e for region in document["regions"] for e in region["entries"]
              if e["kind"] == "function" and e["target_id"] in lock]
    total_functions = document["totals"]["functions"]
    linked_bytes = sum(e["size"] for e in linked)
    return {"functions": len(linked), "total_functions": total_functions,
            "bytes": linked_bytes, "image_size": size,
            "percent": 100.0 * linked_bytes / size}


def _print_coverage(stats):
    print(f"image coverage: {stats['functions']}/{stats['total_functions']} functions, "
          f"{stats['bytes']}/{stats['image_size']} bytes ({stats['percent']:.2f}%)")
    print("  the ROM's game-code blob is compressed from this image "
          "(pipeline.blob_rom), so spliced functions are cartridge coverage.")


def winning_search_source(conn, target_id, blobs=None):
    """The permuter's best.c from a search that reached score 0, or None.

    A permuter win is NOT the m2c seed — the seed scored nonzero and the
    permuter mutated it into a match — so regenerating the seed for a
    function the permuter matched splices the wrong body and the gate
    refuses it. Latest winning search first."""
    import tarfile

    from ..client import DEFAULT_DATA

    blobs = Path(blobs or Path(DEFAULT_DATA) / "blobs")
    rows = conn.execute(
        "SELECT result_sha FROM work_unit WHERE job_type='permuter_search'"
        " AND target_id=? AND state='DONE' AND result_sha IS NOT NULL"
        " ORDER BY updated_at DESC", (target_id,)).fetchall()
    for row in rows:
        path = blobs / row["result_sha"]
        try:
            with tarfile.open(path) as tar:
                result = json.loads(tar.extractfile("result.json").read())
                if (result.get("payload") or {}).get("final_best_score") != 0:
                    continue
                if "best.c" in tar.getnames():
                    return tar.extractfile("best.c").read().decode()
        except (OSError, tarfile.TarError, KeyError, ValueError):
            continue
    return None


def main():
    from ..coordinator import db as dbmod
    from . import autodecomp, disasm

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("splice")
    p.add_argument("targets", nargs="*")
    p.add_argument("--all-matched", action="store_true")
    p.add_argument("--from", dest="source", default=None)
    p.add_argument("--limit", type=int, default=None)
    p = sub.add_parser("revert")
    p.add_argument("targets", nargs="+")
    sub.add_parser("coverage")
    sub.add_parser("check")
    args = parser.parse_args()

    document = blob_layout.load()
    if args.command == "coverage":
        _print_coverage(coverage(document))
        return 0
    if args.command == "check":
        problems = check()
        for target_id, why in problems:
            print(f"  {target_id}: {why}")
        print(f"blob lock: {len(load_lock())} entries, {len(problems)} problems")
        return 1 if problems else 0
    if args.command == "revert":
        result = revert(args.targets, document)
        print(f"reverted {len(result['reverted'])}; image "
              + ("OK" if result["image_ok"] else "FAILED"))
        if not result["image_ok"]:
            print(result["message"])
            return 1
        _print_coverage(coverage(document))
        return 0

    conn = dbmod.connect(Path(args.data) / "conveyor.db")
    if args.all_matched:
        rows = conn.execute(
            "SELECT DISTINCT t.target_id FROM n64_target t"
            " LEFT JOIN matrix_entry m ON m.target_id=t.target_id"
            "   AND m.candidate_id LIKE 'm2c:%' AND m.score=0"
            " LEFT JOIN function_status f ON f.target_id=t.target_id"
            " WHERE t.population='extracted'"
            "   AND (m.target_id IS NOT NULL OR f.status IN ('matched','verified'))"
        ).fetchall()
        targets = sorted(r["target_id"] for r in rows)
    else:
        targets = args.targets
    mapped = {e["target_id"] for r in document["regions"] for e in r["entries"]
              if e["kind"] == "function"}
    targets = [t for t in targets if t in mapped and t not in spliced_targets()]
    if args.limit:
        targets = targets[:args.limit]
    print(f"splicing {len(targets)} targets")

    addresses = {r["target_id"]: r["address"] for r in conn.execute(
        "SELECT target_id,address FROM n64_target WHERE population='extracted'")}
    context_sha = autodecomp._context_sha()

    def source_for(target_id):
        if args.source:
            return Path(args.source).read_text()
        winner = winning_search_source(conn, target_id)
        if winner is not None:
            return winner
        asm = disasm.derive(conn, target_id, context_sha=context_sha)
        seed = autodecomp.m2c_seed(target_id, addresses[target_id],
                                   {target_id: asm})
        if not seed:
            raise RuntimeError("m2c produced no seed")
        return seed

    result = splice(conn, targets, source_for, document=document)
    print(f"spliced {len(result['spliced'])}, refused {len(result['refused'])}")
    for target_id, why in sorted(result["refused"].items()):
        print(f"  refused {target_id}: {why}")
    if not result["image_ok"]:
        print("IMAGE GATE FAILED after splicing:\n" + result["message"])
        return 1
    _print_coverage(coverage(document))
    return 0


if __name__ == "__main__":
    sys.exit(main())
