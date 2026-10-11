"""Splice verified C into the runtime images A and B (R16 landing path).

    python3 -m tools.conveyor.pipeline.ovl_splice splice [--targets asm/us/ovl_a | --image a] FILE.c ...
    python3 -m tools.conveyor.pipeline.ovl_splice splice --all-matches [--targets ...]
    python3 -m tools.conveyor.pipeline.ovl_splice group  --targets asm/us/ovl_a PACKET_DIR [--name NAME]
    python3 -m tools.conveyor.pipeline.ovl_splice revert --targets ... NAME ...
    python3 -m tools.conveyor.pipeline.ovl_splice revert-group --targets ... GROUP
    python3 -m tools.conveyor.pipeline.ovl_splice rebuild [--targets ...]   # objects from locked sources + gate
    python3 -m tools.conveyor.pipeline.ovl_splice check | coverage

blob_splice / blob_group for the two images at 0x8038A400, with the same
gates and their own lock, sources and objects:

    ovl_matched.lock.json      {"a": {name: entry}, "b": {name: entry}}
    src/ovl_<x>/<name>.c       accepted singles (first line: /* flags: ... */)
    src/ovl_<x>/groups/<g>/    accepted IPA groups (blob_group's group.json)
    build/ovl/<x>/obj/         compiled objects, bound to source by provenance

Both images load at the same address, so a name such as func_8038D798 means
something only per image: every command selects its image with --targets
(the scorer's directory, asm/us/ovl_<x>) or --image; a file under
cloud/matches/ovl_<x>/ selects it by itself.

**Gates**, per function, in order: IDO compiles it on the builder (flags from
the source's first line); blob_splice.link_function links it alone at its
image address against the image's symbols.json, proves its own .rodata /
.data / .bss through tools/cloud/owndata.py against the retail image,
refuses a body shorter than the extent and any nonzero word past it; the
body must equal the retail extent; and the whole image, with every other
locked body, must link byte-identical to the retail image (ovl_layout). A
group's members are relocated by blob_group and gated the same way. Only then
is the lock written. A refusal never changes the lock or the sources.

A packet's "claims" (score.py group --claims) become the group's members;
its other members become unspliced context, as blob_group defines it.

Builder: compiles use $OVL_BUILDER_DIR (default /tmp/ovlsplice) on the
builder, never the game splice's /tmp/blobsplice or /tmp/blobgroup.
"""
import argparse
import datetime
import json
import os
import shutil
import sys
from pathlib import Path

from . import blob_build, blob_group, blob_splice, ovl_image, ovl_layout

REPO = ovl_image.REPO
LOCKFILE = REPO / "ovl_matched.lock.json"
MATCHES = REPO / "cloud" / "matches"
BUILDER = blob_splice.BUILDER
TOOLKIT = blob_splice.TOOLKIT
BUILDER_DIR = os.environ.get("OVL_BUILDER_DIR", "/tmp/ovlsplice")
DEFAULT_FLAGSET = ovl_layout.DEFAULT_FLAGSET


class SpliceError(RuntimeError):
    pass


# --- paths and the lock --------------------------------------------------------

def src_dir(image):
    return REPO / "src" / f"ovl_{image}"


def group_root(image):
    return src_dir(image) / "groups"


def obj_dir(image):
    return ovl_layout.work_dir(image) / "obj"


def load_lock(path=LOCKFILE):
    """{"a": {...}, "b": {...}}. Missing file = empty; anything malformed is an
    error (the check must fail closed, never read a broken lock as empty)."""
    try:
        raw = json.loads(Path(path).read_text())
    except FileNotFoundError:
        return {image: {} for image in sorted(ovl_image.IMAGES)}
    if not isinstance(raw, dict) or set(raw) - set(ovl_image.IMAGES) or not all(
            isinstance(v, dict) and all(isinstance(e, dict) for e in v.values())
            for v in raw.values()):
        raise SpliceError(f"{path}: expected {{image: {{name: entry}}}} for images "
                          f"{sorted(ovl_image.IMAGES)}")
    return {image: dict(raw.get(image, {})) for image in sorted(ovl_image.IMAGES)}


def save_lock(lock, path=LOCKFILE):
    Path(path).write_text(json.dumps(
        {image: lock.get(image, {}) for image in sorted(ovl_image.IMAGES)},
        indent=2, sort_keys=True) + "\n")


def flags_of(text):
    first = text.lstrip().splitlines()[0].strip() if text.strip() else ""
    if first.startswith("/* flags:") and first.endswith("*/"):
        return first[len("/* flags:"):-2].strip()
    return DEFAULT_FLAGSET


def symbols(image):
    """{name: address}: the scorer's symbol table for this image (game and
    static symbols plus the image's own functions)."""
    path = ovl_image.target_dir(image) / "symbols.json"
    return {name: int(addr, 16) for name, addr in
            json.loads(path.read_text())["symbols"].items()}


def extents_of(document):
    return {e["target_id"]: e for r in document["regions"] for e in r["entries"]
            if e["kind"] == "function"}


def retail(document):
    path = Path(document["image"]["path"])
    return (path if path.is_absolute() else REPO / path).read_bytes(), \
        int(document["image"]["base"], 16)


# --- bodies --------------------------------------------------------------------

def link(image, name, obj, document, syms=None, work=None):
    extent = extents_of(document).get(name)
    if extent is None:
        raise blob_build.BuildError(f"{name} is not a function of image {image.upper()}")
    return blob_splice.link_function(
        obj, name, extent["vaddr"], extent["size"],
        provides=symbols(image) if syms is None else syms,
        work=work or ovl_layout.work_dir(image) / "fn" / name, image=retail(document))


def group_bodies(image, group, document, syms=None):
    return blob_group.group_bodies(group, document,
                                   extern=symbols(image) if syms is None else syms,
                                   obj_dir=obj_dir(image) / "groups", root=group_root(image))


def spliced_bodies(image, lock=None, document=None, require_provenance=False):
    """{name: bytes} for EVERY locked function of the image, else
    blob_splice.LockedBodyError (never passthrough in a locked body's place)."""
    entries = load_lock()[image] if lock is None else lock
    if not entries:
        return {}
    document = document or ovl_layout.derive(image)
    syms = symbols(image)
    extents = extents_of(document)
    bodies, problems = {}, {}
    for group in sorted({e["group"] for e in entries.values() if e.get("group")}):
        members = sorted(n for n, e in entries.items() if e.get("group") == group)
        obj = blob_group.object_path(group, obj_dir(image) / "groups")
        stale = obj.is_file() and blob_splice.provenance_problem(
            obj, entries[members[0]], require_provenance)
        if stale:
            problems.update({m: f"group {group}: {stale}" for m in members})
            continue
        try:
            built = group_bodies(image, group, document, syms)
        except (blob_group.GroupError, blob_build.BuildError) as exc:
            problems.update({m: f"group {group}: {' '.join(str(exc).split())[:220]}"
                             for m in members})
            continue
        for member in members:
            if member in built:
                bodies[member] = built[member]
            else:
                problems[member] = f"group {group} built no body for it"
    for name, entry in sorted(entries.items()):
        if entry.get("group"):
            continue
        obj = obj_dir(image) / f"{name}.o"
        if name not in extents:
            problems[name] = "not in the layout"
        elif not obj.is_file():
            problems[name] = f"object missing ({obj})"
        else:
            stale = blob_splice.provenance_problem(obj, entry, require_provenance)
            if stale:
                problems[name] = stale
                continue
            try:
                bodies[name] = link(image, name, obj, document, syms)
            except blob_build.BuildError as exc:
                problems[name] = " ".join(str(exc).split())[:220]
    if problems:
        raise blob_splice.LockedBodyError(problems)
    return bodies


def mismatch(document, name, body):
    """None, or where `body` first differs from the retail extent."""
    data, base = retail(document)
    extent = extents_of(document)[name]
    want = data[extent["vaddr"] - base:extent["vaddr"] - base + extent["size"]]
    if body == want:
        return None
    if len(body) != len(want):
        return f"body is {len(body)} bytes, extent {len(want)}"
    at = next(i for i in range(len(body)) if body[i] != want[i]) & ~3
    return (f"differs from the retail extent at +0x{at:x} "
            f"(built {body[at:at + 4].hex()}, retail {want[at:at + 4].hex()})")


# --- compile -------------------------------------------------------------------

def compile_singles(image, sources, out_dir=None):
    """{name: Path} -> ({name: object}, {name: why}), batched per flagset.
    Objects go to `out_dir` (default: the image's object directory)."""
    by_flags = {}
    for name, path in sources.items():
        by_flags.setdefault(flags_of(Path(path).read_text()), {})[name] = path
    objects, failed = {}, {}
    for flags, batch in sorted(by_flags.items()):
        built, bad = blob_splice.compile_on_builder(
            batch, flags, obj_dir=out_dir or obj_dir(image), workdir=f"{BUILDER_DIR}/{image}",
            staging=ovl_layout.work_dir(image) / "staging")
        objects.update(built)
        failed.update(bad)
    return objects, failed


# --- splice / revert -----------------------------------------------------------

def _gate(image, document, bodies):
    try:
        return ovl_layout.build(document, bodies)
    except blob_build.BuildError as exc:
        return False, "", " ".join(str(exc).split())[:300]


def _entry(image, name, source, flags, origin=None, group=None):
    entry = {"source": str(source.relative_to(REPO)),
             "source_sha256": (blob_group.source_sha(group) if group
                               else blob_splice.source_sha(source.read_text())),
             "flagset": flags, "toolkit_sha": TOOLKIT, "image": image.upper(),
             "verified": "image_gate", "verified_at": datetime.date.today().isoformat()}
    if group:
        entry["group"] = group["name"]
    if origin:
        entry["origin"] = origin
    return entry


def splice(image, files, lockfile=LOCKFILE, compile_fn=compile_singles):
    """Splice singles one at a time, each behind the image gate.

    `files` is {name: source path}. Returns {"spliced", "refused", "image_ok",
    "image_sha", "message"}; the lock gains only accepted functions."""
    document = ovl_layout.derive(image)
    extents = extents_of(document)
    lock = load_lock(lockfile)
    refused, staged = {}, {}
    work = ovl_layout.work_dir(image) / "incoming"
    shutil.rmtree(work, ignore_errors=True)
    work.mkdir(parents=True)
    for name, path in sorted(files.items()):
        if name not in extents:
            refused[name] = f"not a function of image {image.upper()}"
        elif lock[image].get(name, {}).get("group"):
            refused[name] = f"locked by group {lock[image][name]['group']}; revert it first"
        else:
            staged[name] = work / f"{name}.c"
            shutil.copy(path, staged[name])
    # New objects stay in the incoming directory until accepted, so a refused
    # re-splice leaves the locked object (and its provenance) untouched.
    objects, failed = compile_fn(image, staged, work / "obj") if staged else ({}, {})
    refused.update(failed)
    syms = symbols(image)
    accepted = spliced_bodies(image, {n: e for n, e in lock[image].items() if n not in objects},
                              document)
    for name in sorted(objects):
        try:
            body = link(image, name, objects[name], document, syms)
        except blob_build.BuildError as exc:
            refused[name] = " ".join(str(exc).split())[:220]
            continue
        why = mismatch(document, name, body)
        if why:
            refused[name] = why
            continue
        ok, _sha, message = _gate(image, document, {**accepted, name: body})
        if not ok:
            refused[name] = "image gate: " + message.splitlines()[0]
            continue
        accepted[name] = body
        final = obj_dir(image) / f"{name}.o"
        final.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy(objects[name], final)
        shutil.copy(blob_splice.provenance_path(objects[name]), blob_splice.provenance_path(final))
        dest = src_dir(image) / f"{name}.c"
        dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy(staged[name], dest)
        origin = files[name]
        try:
            origin = str(Path(origin).resolve().relative_to(REPO))
        except ValueError:
            origin = str(origin)
        lock[image][name] = _entry(image, name, dest, flags_of(dest.read_text()), origin)
        save_lock(lock, lockfile)
    ok, sha, message = _gate(image, document, accepted)
    return {"spliced": sorted(n for n in objects if n not in refused), "refused": refused,
            "locked": len(lock[image]), "image_ok": ok, "image_sha": sha, "message": message}


def packet_spec(packet):
    """blob_group's group.json from a packet's: claims -> members, the other
    members -> context. Without claims every member is spliced."""
    packet = Path(packet)
    spec = json.loads((packet / "group.json").read_text())
    claims = spec.pop("claims", None)
    if claims is not None:
        unknown = set(claims) - set(spec["members"])
        if unknown:
            raise SpliceError(f"{packet}: claims {sorted(unknown)} are not members")
        if not claims:
            raise SpliceError(f"{packet}: no claims (research only), nothing to splice")
        rest = [m for m in spec["members"] if m not in claims]
        spec["members"] = list(claims)
        spec["context"] = list(spec.get("context", [])) + [
            m for m in rest if m not in spec.get("context", [])]
    spec.pop("external_sources", None)
    return spec


def splice_group(image, packet, name=None, lockfile=LOCKFILE, compile_fn=None):
    """Install a group packet as src/ovl_<x>/groups/<name> and splice it."""
    packet = Path(packet)
    name = name or packet.name
    spec_json = packet_spec(packet)
    document = ovl_layout.derive(image)
    lock = load_lock(lockfile)
    clash = [m for m in spec_json["members"]
             if m in lock[image] and lock[image][m].get("group") != name]
    if clash:
        raise SpliceError(f"already spliced outside group {name}: {clash}; revert them first")
    missing = [m for m in spec_json["members"] + spec_json.get("context", [])
               if m not in extents_of(document)]
    if missing:
        raise SpliceError(f"not functions of image {image.upper()}: {missing}")
    dest = group_root(image) / name
    backup = ovl_layout.work_dir(image) / "group-backup" / name
    shutil.rmtree(backup, ignore_errors=True)
    if dest.exists():
        shutil.copytree(dest, backup)
        shutil.rmtree(dest)
    dest.mkdir(parents=True)

    def restore():
        shutil.rmtree(dest, ignore_errors=True)
        if backup.exists():
            shutil.copytree(backup, dest)

    try:
        for f in spec_json["files"]:
            shutil.copy(packet / f, dest / f)
        spec_json.setdefault("provenance", f"installed from {packet}")
        (dest / "group.json").write_text(json.dumps(spec_json, indent=1) + "\n")
        spec = blob_group.load(name, group_root(image))
        if compile_fn is None:
            blob_group.compile_group(spec, obj_dir=obj_dir(image) / "groups",
                                     workdir=f"{BUILDER_DIR}/{image}-group",
                                     staging_root=ovl_layout.work_dir(image) / "group-staging")
        else:
            compile_fn(image, spec)
        bodies = group_bodies(image, name, document)
        for member, body in sorted(bodies.items()):
            why = mismatch(document, member, body)
            if why:
                raise SpliceError(f"{member} {why}; not splicing")
        others = spliced_bodies(image, {n: e for n, e in lock[image].items()
                                        if e.get("group") != name}, document)
        ok, sha, message = _gate(image, document, {**others, **bodies})
        if not ok:
            raise SpliceError("image gate failed with the group spliced:\n" + message)
    except Exception:
        restore()
        raise
    for member in [n for n, e in lock[image].items() if e.get("group") == name]:
        del lock[image][member]
    for member in spec["members"]:
        lock[image][member] = _entry(image, member, dest / "group.json", spec["flags"],
                                     str(packet), group=spec)
    save_lock(lock, lockfile)
    return {"spliced": sorted(spec["members"]), "image_sha": sha}


def rebuild(image, lockfile=LOCKFILE, compile_fn=compile_singles, group_compile_fn=None):
    """Recompile every locked body of the image from its accepted source (a
    fresh checkout has none of the objects) and gate the image with them."""
    lock = load_lock(lockfile)[image]
    singles = {n: REPO / e["source"] for n, e in lock.items() if not e.get("group")}
    _objects, failed = compile_fn(image, singles) if singles else ({}, {})
    if failed:
        raise SpliceError(f"locked sources no longer compile: {failed}")
    for group in sorted({e["group"] for e in lock.values() if e.get("group")}):
        spec = blob_group.load(group, group_root(image))
        if group_compile_fn is not None:
            group_compile_fn(image, spec)
        else:
            blob_group.compile_group(spec, obj_dir=obj_dir(image) / "groups",
                                     workdir=f"{BUILDER_DIR}/{image}-group",
                                     staging_root=ovl_layout.work_dir(image) / "group-staging")
    document = ovl_layout.derive(image)
    bodies = spliced_bodies(image, lock, document, require_provenance=True)
    ok, sha, message = _gate(image, document, bodies)
    return {"bodies": len(bodies), "image_ok": ok, "image_sha": sha, "message": message}


def revert(image, names, lockfile=LOCKFILE):
    lock = load_lock(lockfile)
    removed = []
    for name in names:
        entry = lock[image].get(name)
        if entry is None:
            continue
        if entry.get("group"):
            raise SpliceError(f"{name} belongs to group {entry['group']}: use revert-group")
        del lock[image][name]
        (src_dir(image) / f"{name}.c").unlink(missing_ok=True)
        obj = obj_dir(image) / f"{name}.o"
        obj.unlink(missing_ok=True)
        blob_splice.provenance_path(obj).unlink(missing_ok=True)
        removed.append(name)
    save_lock(lock, lockfile)
    return removed


def revert_group(image, group, lockfile=LOCKFILE):
    lock = load_lock(lockfile)
    members = sorted(n for n, e in lock[image].items() if e.get("group") == group)
    for member in members:
        del lock[image][member]
    save_lock(lock, lockfile)
    shutil.rmtree(group_root(image) / group, ignore_errors=True)
    return members


# --- check / coverage ----------------------------------------------------------

def check(lockfile=LOCKFILE):
    """[(spec, why)] for drifted, missing or misplaced lock entries. Reads only
    the lock, the sources and extents.json: no build products, no network."""
    try:
        lock = load_lock(lockfile)
    except (SpliceError, ValueError) as exc:
        return [("ovl_matched.lock.json", str(exc))]
    problems = []
    for image, entries in sorted(lock.items()):
        names = {f["name"] for f in ovl_layout.load_extents(image)["functions"]}
        groups = {}
        for name, entry in sorted(entries.items()):
            spec = f"{image}:{name}"
            if name not in names:
                problems.append((spec, f"not a function of image {image.upper()}"))
                continue
            if entry.get("group"):
                groups.setdefault(entry["group"], set()).add(entry.get("source_sha256"))
                continue
            path = REPO / entry.get("source", "")
            if not path.is_file() or not entry.get("source"):
                problems.append((spec, "source missing"))
            elif blob_splice.source_sha(path.read_text()) != entry.get("source_sha256"):
                problems.append((spec, "source hash drifted"))
        for group, shas in sorted(groups.items()):
            try:
                spec = blob_group.load(group, group_root(image))
            except blob_group.GroupError as exc:
                problems.append((f"{image}:group:{group}", str(exc)))
                continue
            if shas != {blob_group.source_sha(spec)}:
                problems.append((f"{image}:group:{group}", "group sources drifted from the lock"))
            locked = {n for n, e in entries.items() if e.get("group") == group}
            if locked != set(spec["members"]):
                problems.append((f"{image}:group:{group}", "lock members differ from group.json"))
    return problems


def coverage(lockfile=LOCKFILE):
    lock = load_lock(lockfile)
    return {image: ovl_layout.coverage(image, lock[image]) for image in sorted(lock)}


def print_coverage(stats, indent="  "):
    for image, c in sorted(stats.items()):
        print(f"{indent}overlay {image.upper()}:   {c['functions']:4d}/{c['total_functions']} functions, "
              f"{c['bytes']}/{c['image_size']} bytes ({100 * c['bytes'] / c['image_size']:.2f}%; "
              f"{100 * c['bytes'] / c['function_bytes']:.2f}% of {c['function_bytes']} B text) "
              f"— image only, stream at {c['rom_offset']} not yet composed")


# --- CLI -----------------------------------------------------------------------

def _images(args, files=()):
    chosen = {ovl_layout.image_name(t) for t in args.targets} | set(args.image or [])
    for f in files:
        parent = Path(f).resolve().parent.name
        if parent.startswith("ovl_") and parent[4:] in ovl_image.IMAGES:
            chosen.add(parent[4:])
    return sorted(chosen)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)

    def selector(p):
        p.add_argument("--targets", action="append", default=[],
                       help="asm/us/ovl_a or asm/us/ovl_b")
        p.add_argument("--image", action="append", choices=sorted(ovl_image.IMAGES))
        return p

    p = selector(sub.add_parser("splice"))
    p.add_argument("files", nargs="*")
    p.add_argument("--all-matches", action="store_true",
                   help="every cloud/matches/ovl_<x>/*.c of the selected images")
    p = selector(sub.add_parser("group"))
    p.add_argument("packet")
    p.add_argument("--name")
    p = selector(sub.add_parser("revert"))
    p.add_argument("names", nargs="+")
    p = selector(sub.add_parser("revert-group"))
    p.add_argument("group")
    selector(sub.add_parser("rebuild"))
    sub.add_parser("check").add_argument("--quiet", action="store_true",
                                         help="print only problems")
    sub.add_parser("coverage")
    args = parser.parse_args(argv)

    if args.command == "check":
        problems = check()
        for spec, why in problems:
            print(f"  {spec}: {why}", file=sys.stderr)
        try:
            n = sum(len(v) for v in load_lock().values())
        except SpliceError:
            n = "?"
        if problems:
            print(f"ovl_matched.lock.json: {len(problems)} problem(s). Revert the edit, or "
                  "re-splice through pipeline.ovl_splice.", file=sys.stderr)
        elif not args.quiet:
            print(f"ovl lock: all {n} runtime-image entries intact")
        return 1 if problems else 0
    if args.command == "coverage":
        print_coverage(coverage(), indent="")
        return 0

    try:
        images = _images(args, getattr(args, "files", ()))
        if args.command == "splice" and args.all_matches and not images:
            images = sorted(ovl_image.IMAGES)
        if args.command == "rebuild" and not images:
            images = sorted(ovl_image.IMAGES)
        if not images:
            raise SpliceError("select the image with --targets asm/us/ovl_<x> or --image <x>")
        if args.command in ("group", "revert", "revert-group") and len(images) != 1:
            raise SpliceError(f"{args.command} takes exactly one image, got {images}")
    except (SpliceError, ovl_layout.LayoutError) as exc:
        sys.exit(f"ovl_splice: {exc}")

    status = 0
    try:
        if args.command == "splice":
            for image in images:
                files = {Path(f).stem: Path(f) for f in args.files
                         if len(images) == 1 or Path(f).resolve().parent.name == f"ovl_{image}"}
                if args.all_matches:
                    lock = load_lock()[image]
                    for f in sorted((MATCHES / f"ovl_{image}").glob("*.c")):
                        same = lock.get(f.stem, {}).get("source_sha256") == \
                            blob_splice.source_sha(f.read_text())
                        if not same:
                            files.setdefault(f.stem, f)
                print(f"image {image.upper()}: splicing {len(files)}")
                r = splice(image, files)
                for name, why in sorted(r["refused"].items()):
                    print(f"  refused {name}: {why}")
                print(f"  spliced {len(r['spliced'])}, refused {len(r['refused'])}, "
                      f"locked {r['locked']}; image gate "
                      f"{'byte-identical, sha256 ' + r['image_sha'] if r['image_ok'] else 'FAILED'}")
                if not r["image_ok"]:
                    print("  " + r["message"].replace("\n", "\n  "))
                    status = 1
                elif r["refused"]:
                    status = status or 2
        elif args.command == "group":
            r = splice_group(images[0], args.packet, args.name)
            print(f"image {images[0].upper()}: group spliced {r['spliced']}; image gate "
                  f"byte-identical, sha256 {r['image_sha']}")
        elif args.command == "rebuild":
            for image in images:
                r = rebuild(image)
                print(f"image {image.upper()}: {r['bodies']} locked bodies rebuilt; image gate "
                      f"{'byte-identical, sha256 ' + r['image_sha'] if r['image_ok'] else 'FAILED'}")
                if not r["image_ok"]:
                    print("  " + r["message"].replace("\n", "\n  "))
                    status = 1
        elif args.command == "revert":
            print(f"reverted {revert(images[0], args.names)}")
        elif args.command == "revert-group":
            print(f"reverted group {args.group}: {revert_group(images[0], args.group)}")
        if args.command in ("revert", "revert-group"):
            document = ovl_layout.derive(images[0])
            ok, sha, message = _gate(images[0], document, spliced_bodies(images[0],
                                                                          document=document))
            print(f"image gate: {'byte-identical' if ok else 'FAILED: ' + message}")
            status = 0 if ok else 1
    except (SpliceError, blob_group.GroupError, blob_splice.LockedBodyError,
            ovl_layout.LayoutError, ovl_image.ImageError) as exc:
        print(f"ovl_splice FAILED: {exc}", file=sys.stderr)
        return 1
    print_coverage(coverage(), indent="")
    return status


if __name__ == "__main__":
    sys.exit(main())
