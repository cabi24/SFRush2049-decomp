"""Splice IDO -O3 interprocedural call groups into the game-code image (010 Phase 4).

    python3 -m tools.conveyor.pipeline.blob_group compile <group>
    python3 -m tools.conveyor.pipeline.blob_group splice <group>
    python3 -m tools.conveyor.pipeline.blob_group revert <group>
    python3 -m tools.conveyor.pipeline.blob_group check
    python3 -m tools.conveyor.pipeline.blob_group seed <group> [--force]

Functions shaped by IDO -O3 interprocedural register allocation (IPA) only
reproduce when compiled together, as one whole program, with the linker told
which procedures stay external. See specs/010-ipa-call-groups/research/ (S1-S5).

A group lives in src/blob/groups/<group>/:

    group.json   {"members": [...], "files": [...], "keep": [...], "flags": "..."}
    *.c          the group's sources, members as ordinary globals

`members` are the image functions this group supplies. Optional `context`
lists image functions compiled in the unit but NOT spliced (not matched
yet): their presence shapes IPA for the members, and calls into them resolve
to their image addresses. The sources may also contain stand-in functions
that exist only to shape the compile (keep a leaf out of line). `keep` is the uld -kp list: procedures that stay external.
Everything else is internal to the whole program and takes part in IPA.

Build (on the builder, the IDO host): `cc -j` (ucode only) per the flags to get each
file's ucode, then uld -kp <keep> -> usplit -> umerge -> uopt -> ugen -> as1,
staged by hand because the cc driver always passes -preserve_dead_code, which
keeps every global external and so defeats cross-file IPA.

Splice: each member's slice of the group object is relocated to its own image
address (calls between members resolve to the members' image addresses) and
the image gate runs with every other locked body. Lock entries carry
"group": <group>; blob_splice.spliced_bodies() rebuilds them from the group
object, so blob_rom composes groups into the ROM like any other splice.
"""
import argparse
import datetime
import hashlib
import json
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

from . import blob_build, blob_layout, blob_splice

REPO = blob_splice.REPO
GROUP_DIR = blob_splice.SRC_DIR / "groups"
OBJ_DIR = blob_splice.OBJ_DIR / "groups"
BUILDER = blob_splice.BUILDER
TOOLKIT = blob_splice.TOOLKIT
BUILDER_TMP = "/tmp/blobgroup"
READELF = "mips-linux-gnu-readelf"
OBJCOPY = blob_build.OBJCOPY


class GroupError(Exception):
    pass


# --- definitions -------------------------------------------------------------

def load(group, root=GROUP_DIR):
    path = Path(root) / group / "group.json"
    try:
        spec = json.loads(path.read_text())
    except FileNotFoundError:
        raise GroupError(f"no group definition at {path}")
    for key in ("members", "files", "keep", "flags"):
        if key not in spec:
            raise GroupError(f"{path}: missing {key!r}")
    if "-O3" not in spec["flags"].split():
        raise GroupError(f"{path}: an IPA group must be compiled at -O3")
    spec.setdefault("context", [])
    overlap = set(spec["members"]) & set(spec["context"])
    if overlap:
        raise GroupError(f"{path}: {sorted(overlap)} both member and context")
    spec["name"] = group
    spec["dir"] = path.parent
    return spec


def source_sha(spec):
    """One hash over the definition and every source file, in order."""
    h = hashlib.sha256()
    h.update((spec["dir"] / "group.json").read_bytes())
    for name in spec["files"]:
        h.update(b"\0" + name.encode() + b"\0")
        h.update((spec["dir"] / name).read_bytes())
    return h.hexdigest()


def object_path(group, obj_dir=OBJ_DIR):
    return Path(obj_dir) / f"{group}.o"


# --- build -------------------------------------------------------------------

def builder_script(spec, toolkit=TOOLKIT, workdir=BUILDER_TMP):
    """Shell for the builder: ucode per file, then the whole-program stages
    with uld -kp. Mirrors the cc -O3 -c stage arguments exactly, apart from
    -kp in place of -preserve_dead_code."""
    flags = spec["flags"]
    files = " ".join(spec["files"])
    units = " ".join(re.sub(r"\.c$", ".u", f) for f in spec["files"])
    opt = "-O3"
    return (
        f"set -e; T=~/rush2049/cache/toolkits/{toolkit}/ido; cd {workdir}; "
        f"$T/cc -j {flags} {files} >cc.log 2>&1; "
        f"$T/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 "
        f"-no_AutoGnum -kp keep.txt {units} -ko linked; "
        f"$T/usplit -mips2 -o split -t st linked; "
        f"$T/umerge -Olimit 5000 -mips2 -EB -g0 {opt} split -o merged -t st >/dev/null; "
        f"$T/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 {opt} merged opt -t st optlog >/dev/null; "
        f"$T/ugen -G 0 -mips2 -EB -g0 {opt} opt -o gen -t st -temp ugtmp >/dev/null; "
        f"$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 {opt} -Olimit 5000 gen -o group.o -t st >/dev/null; "
        f"echo DONE")


def compile_group(spec, builder=BUILDER, obj_dir=OBJ_DIR):
    """Build the group on the builder; returns the fetched object's path."""
    staging = Path(blob_build.BUILD_DIR) / "group-staging" / spec["name"]
    if staging.exists():
        shutil.rmtree(staging)
    staging.mkdir(parents=True)
    for name in spec["files"]:
        shutil.copy(spec["dir"] / name, staging / name)
    (staging / "keep.txt").write_text("".join(f"{k}\n" for k in spec["keep"]))
    subprocess.run(["ssh", builder, f"rm -rf {BUILDER_TMP} && mkdir -p {BUILDER_TMP}"],
                   check=True, capture_output=True)
    subprocess.run(["scp", "-q", *[str(p) for p in staging.iterdir()],
                    f"{builder}:{BUILDER_TMP}/"], check=True, capture_output=True)
    proc = subprocess.run(["ssh", builder, builder_script(spec)],
                          capture_output=True, text=True)
    if "DONE" not in proc.stdout:
        log = subprocess.run(["ssh", builder, f"cat {BUILDER_TMP}/cc.log"],
                             capture_output=True, text=True).stdout
        raise GroupError("group build failed: "
                         + " ".join((proc.stderr + log).split())[:400])
    out = object_path(spec["name"], obj_dir)
    out.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(["scp", "-q", f"{builder}:{BUILDER_TMP}/group.o", str(out)],
                   check=True, capture_output=True)
    return out


# --- relocation per member ---------------------------------------------------

def _text(obj):
    return bytearray(subprocess.run(
        [OBJCOPY, "-O", "binary", "-j", ".text", str(obj), "/dev/stdout"],
        capture_output=True, check=True).stdout)


def _symbols(obj):
    """{name: (value, section_index or 'UND')} from the symbol table."""
    out = subprocess.run([READELF, "-sW", str(obj)], capture_output=True,
                         text=True, check=True).stdout
    syms = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 8 and parts[0].rstrip(":").isdigit():
            syms[parts[7]] = (int(parts[1], 16), parts[6])
    return syms


def _text_relocations(obj):
    out = subprocess.run([READELF, "-rW", str(obj)], capture_output=True,
                         text=True, check=True).stdout
    rels, section, others = [], None, set()
    for line in out.splitlines():
        m = re.match(r"Relocation section '(\.rel\S+)'", line)
        if m:
            section = m.group(1)
            continue
        m = re.match(r"\s*([0-9a-f]{8})\s+[0-9a-f]{8}\s+(R_MIPS_\w+)\s+[0-9a-f]{8}\s+(\S+)", line)
        if not m:
            continue
        if section == ".rel.text":
            rels.append((int(m.group(1), 16), m.group(2), m.group(3)))
        else:
            others.add(section)
    return rels, others


def member_slices(obj, members, extents):
    """{member: (object offset, image vaddr, size)} located by symbol."""
    syms = _symbols(obj)
    text_ndx = None
    slices = {}
    for member in members:
        if member not in syms or syms[member][1] == "UND":
            raise GroupError(f"member {member} is not defined in the group object")
        entry = extents.get(member)
        if entry is None:
            raise GroupError(f"member {member} has no extent in the layout")
        value, ndx = syms[member]
        text_ndx = text_ndx or ndx
        if ndx != text_ndx:
            raise GroupError(f"member {member} is not in .text")
        slices[member] = (value, entry["vaddr"], entry["size"])
    return slices, text_ndx


def relocate(obj, slices, text_ndx, extern):
    """Bytes of each member slice with its .text relocations applied at the
    member's image address. Supports R_MIPS_26 and REL HI16/LO16 pairs;
    anything else (and any relocation outside .text) is refused."""
    text = _text(obj)
    syms = _symbols(obj)
    rels, others = _text_relocations(obj)
    if others:
        raise GroupError(f"relocations outside .text not supported yet: {sorted(others)}")
    covered = sorted((off, off + size, vaddr) for off, vaddr, size in slices.values())

    def text_addr(offset):
        for lo, hi, vaddr in covered:
            if lo <= offset < hi:
                return vaddr + (offset - lo)
        raise GroupError(f"relocation targets .text+0x{offset:x}, which no member "
                         "slice covers (a call into a stand-in?)")

    def resolve(name, addend):
        sym = syms.get(name)
        if name == ".text":
            return text_addr(addend)
        if sym and sym[1] == text_ndx:
            return text_addr(sym[0] + addend)
        if name in extern:
            return extern[name] + addend
        addr = blob_splice.address_named(name)
        if addr is not None:
            return addr + addend
        raise GroupError(f"unresolved symbol {name}")

    def word(o):
        return struct.unpack(">I", text[o:o + 4])[0]

    def put(o, w):
        text[o:o + 4] = struct.pack(">I", w & 0xFFFFFFFF)

    in_member = lambda o: any(lo <= o < hi for lo, hi, _ in covered)
    pending_hi = []
    for offset, rtype, name in rels:
        if not in_member(offset):
            continue            # a stand-in's own relocation: not image bytes
        insn = word(offset)
        if rtype == "R_MIPS_26":
            target = resolve(name, (insn & 0x03FFFFFF) << 2)
            put(offset, (insn & 0xFC000000) | ((target >> 2) & 0x03FFFFFF))
        elif rtype == "R_MIPS_HI16":
            pending_hi.append((offset, name, insn))
        elif rtype == "R_MIPS_LO16":
            lo = struct.unpack(">h", struct.pack(">H", insn & 0xFFFF))[0]
            his = [h for h in pending_hi if h[1] == name]
            hi_imm = (his[0][2] & 0xFFFF) if his else 0
            value = resolve(name, (hi_imm << 16) + lo)
            for h_off, _, h_insn in his:
                put(h_off, (h_insn & 0xFFFF0000) | (((value + 0x8000) >> 16) & 0xFFFF))
            pending_hi = [h for h in pending_hi if h[1] != name]
            put(offset, (insn & 0xFFFF0000) | (value & 0xFFFF))
        else:
            raise GroupError(f"unsupported relocation {rtype} at .text+0x{offset:x}")
    if pending_hi:
        raise GroupError(f"unpaired R_MIPS_HI16 at {[hex(h[0]) for h in pending_hi]}")
    return {member: bytes(text[off:off + size])
            for member, (off, vaddr, size) in slices.items()}


def group_bodies(group, document=None, extern=None, obj_dir=OBJ_DIR, root=GROUP_DIR,
                 include_context=False):
    """{member: bytes} for a compiled group."""
    spec = load(group, root)
    document = document or blob_layout.load()
    extents = {e["target_id"]: e for region in document["regions"]
               for e in region["entries"] if e["kind"] == "function"}
    obj = object_path(group, obj_dir)
    if not obj.is_file():
        raise GroupError(f"group {group} is not compiled ({obj} missing)")
    slices, text_ndx = member_slices(obj, spec["members"] + spec["context"], extents)
    extern = blob_splice.image_symbols(document) if extern is None else extern
    bodies = relocate(obj, slices, text_ndx, extern)
    if not include_context:
        bodies = {m: b for m, b in bodies.items() if m in spec["members"]}
    return bodies


# --- splice / revert / check -------------------------------------------------

def word_diffs(bodies, document):
    """{member: number of 32-bit words differing from the extracted image}."""
    image = Path(document["image"]["path"])
    image = (REPO / image if not image.is_absolute() else image).read_bytes()
    base = int(document["image"]["base"], 16)
    vaddrs = {e["target_id"]: e["vaddr"] for r in document["regions"]
              for e in r["entries"] if e["kind"] == "function"}
    out = {}
    for member, data in bodies.items():
        off = vaddrs[member] - base
        out[member] = sum(1 for i in range(0, len(data), 4)
                          if image[off + i:off + i + 4] != data[i:i + 4])
    return out


def image_mismatches(bodies, document):
    """{member: first differing byte offset} against the extracted image."""
    image = Path(document["image"]["path"])
    image = (REPO / image if not image.is_absolute() else image).read_bytes()
    base = int(document["image"]["base"], 16)
    vaddrs = {e["target_id"]: e["vaddr"] for r in document["regions"]
              for e in r["entries"] if e["kind"] == "function"}
    bad = {}
    for member, data in bodies.items():
        off = vaddrs[member] - base
        if image[off:off + len(data)] != data:
            bad[member] = next(i for i in range(len(data))
                               if image[off + i] != data[i])
    return bad


def splice(group, document=None, lockfile=blob_splice.LOCKFILE, root=GROUP_DIR):
    spec = load(group, root)
    document = document or blob_layout.load()
    lock = blob_splice.load_lock(lockfile)
    clash = [m for m in spec["members"] if m in lock and lock[m].get("group") != group]
    if clash:
        raise GroupError(f"already spliced outside this group: {clash}")
    compile_group(spec)
    bodies = group_bodies(group, document)
    for member, first in image_mismatches(bodies, document).items():
        raise GroupError(f"{member} differs from the image at +0x{first:x}; not splicing")
    others = {t: b for t, b in blob_splice.spliced_bodies(document=document).items()
              if t not in bodies}
    ok, sha, message = blob_splice.build_with({**others, **bodies}, document=document)
    if not ok:
        blob_splice.build_with(others, document=document)
        raise GroupError("IMAGE GATE FAILED with the group spliced:\n" + message)
    stamp = datetime.date.today().isoformat()
    ssha = source_sha(spec)
    for member in spec["members"]:
        lock[member] = {
            "group": group,
            "source": str((spec["dir"] / "group.json").relative_to(REPO)),
            "source_sha256": ssha,
            "flagset": spec["flags"],
            "toolkit_sha": TOOLKIT,
            "verified": "image_gate",
            "verified_at": stamp,
        }
    blob_splice.save_lock(lock, lockfile)
    return {"spliced": list(spec["members"]), "image_sha256": sha}


def revert(group, document=None, lockfile=blob_splice.LOCKFILE):
    lock = blob_splice.load_lock(lockfile)
    members = [t for t, e in lock.items() if e.get("group") == group]
    for member in members:
        del lock[member]
    blob_splice.save_lock(lock, lockfile)
    ok, _, message = blob_splice.build_with(
        blob_splice.spliced_bodies(document=document), document=document)
    return members, ok, message


def check(lockfile=blob_splice.LOCKFILE, root=GROUP_DIR):
    """(group, problem) for group lock entries whose sources drifted."""
    problems = []
    groups = {e["group"] for e in blob_splice.load_lock(lockfile).values() if e.get("group")}
    for group in sorted(groups):
        try:
            spec = load(group, root)
        except GroupError as exc:
            problems.append((group, str(exc)))
            continue
        want = {e["source_sha256"] for e in blob_splice.load_lock(lockfile).values()
                if e.get("group") == group}
        if want != {source_sha(spec)}:
            problems.append((group, "group sources drifted from the lock"))
    return problems


# --- seeding a group from discovery --------------------------------------

def _definition(src, name):
    """(start, end) of a function definition `name` in C text, or None."""
    m = re.search(r"^[^\n;{}]*\b" + re.escape(name) + r"\s*\([^;{]*\)\s*\{", src, re.M)
    if not m:
        return None
    depth, j = 0, m.end() - 1
    while j < len(src):
        if src[j] == "{":
            depth += 1
        elif src[j] == "}":
            depth -= 1
            if depth == 0:
                return m.start(), j + 1
        j += 1
    return None


def _param_count(signature):
    inner = signature[signature.index("(") + 1:signature.rindex(")")].strip()
    if inner in ("", "void"):
        return 0
    return inner.count(",") + 1


def seed_group(entry, conn, calls, root=GROUP_DIR, force=False):
    """Write src/blob/groups/<id>/ from a discovered group (build/ipa_groups.json
    entry): IPA-mode m2c seeds for every member in one TU after one shared
    prelude, member prototypes from their definitions, a stand-in caller for
    every member that has an in-group caller (so -O3 keeps it out of line),
    and a keep list of the roots plus the stand-ins."""
    from . import autodecomp, disasm
    group = entry["id"]
    out_dir = Path(root) / group
    if (out_dir / "group.json").exists() and not force:
        raise GroupError(f"{out_dir} exists (use --force to regenerate)")
    members = list(entry["members"])
    addresses = {r["target_id"]: r["address"] for r in conn.execute(
        "SELECT target_id,address FROM n64_target WHERE population='extracted'")}
    seeds, prelude = {}, None
    for member in members:
        seed = autodecomp.m2c_seed(member, addresses[member],
                                   {member: disasm.derive(conn, member)}, ipa=True)
        if not seed:
            raise GroupError(f"m2c produced no seed for {member}")
        span = _definition(seed, member)
        if span is None:
            raise GroupError(f"no definition of {member} in its seed")
        seeds[member] = seed[span[0]:span[1]]
        if prelude is None:
            prelude = seed[:span[0]]
    # One prelude: drop every member's own prototype (m2c's context declares
    # IPA callees with their ABI parameters only), then declare each member
    # from its definition.
    for member in members:
        prelude = re.sub(r"^[^\n;{}]*\b" + re.escape(member) + r"\s*\([^;{]*\)\s*;[^\n]*$",
                         "", prelude, flags=re.M)
    signatures = {m: seeds[m][:seeds[m].index("{")].strip() for m in members}
    in_group_callers = {m: [c for c in members if m in calls.get(c, ())] for m in members}
    roots = [m for m in members if not in_group_callers[m]]
    standins = []
    body = [prelude, "/* group members */"]
    body += [signatures[m] + ";" for m in members]
    body += [""] + [seeds[m] + "\n" for m in members]
    for m in members:
        if not in_group_callers[m]:
            continue
        name = f"__standin_{m}"
        args = ", ".join(["0"] * _param_count(signatures[m]))
        body.append(f"/* stand-in caller: keeps {m} out of line under -O3 */\n"
                    f"void {name}(void)\n{{\n    {m}({args});\n}}\n")
        standins.append(name)
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "group.c").write_text("\n".join(body))
    spec = {"members": members, "files": ["group.c"], "keep": roots + standins,
            "flags": "-g0 -O3 -mips2 -G 0 -non_shared",
            "generated": "blob_group seed from build/ipa_groups.json"}
    (out_dir / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
    return out_dir


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    for name in ("compile", "splice", "revert"):
        sub.add_parser(name).add_argument("group")
    sub.add_parser("check")
    sp = sub.add_parser("seed")
    sp.add_argument("group")
    sp.add_argument("--force", action="store_true")
    args = parser.parse_args()
    try:
        if args.command == "seed":
            from ..client import DEFAULT_DATA
            from ..coordinator import db as dbmod
            from . import ipa as ipamod
            doc = json.loads(ipamod.GROUPS_JSON.read_text())
            entry = next((g for g in doc["groups"] if g["id"] == args.group), None)
            if entry is None:
                raise GroupError(f"{args.group} is not a discovered group ({ipamod.GROUPS_JSON})")
            conn = dbmod.connect(Path(DEFAULT_DATA) / "conveyor.db")
            calls, _ = ipamod.call_graph(conn, Path(blob_layout.IMAGE).read_bytes())
            out = seed_group(entry, conn, calls, force=args.force)
            print(f"seeded {args.group} ({len(entry['members'])} members) -> {out}")
            return 0
        if args.command == "compile":
            spec = load(args.group)
            out = compile_group(spec)
            doc = blob_layout.load()
            bodies = group_bodies(args.group, doc, include_context=True)
            diffs = word_diffs(bodies, doc)
            print(f"compiled {args.group} -> {out}")
            for name, n in diffs.items():
                role = "member" if name in spec["members"] else "context"
                words = len(bodies[name]) // 4
                print(f"  {role:7s} {name}: " + ("matches the image" if n == 0
                      else f"{n}/{words} words differ"))
            return 1 if any(diffs[m] for m in spec["members"]) else 0
        elif args.command == "splice":
            result = splice(args.group)
            print(f"spliced {len(result['spliced'])} members of {args.group}; image gate OK")
            blob_splice._print_coverage(blob_splice.coverage())
        elif args.command == "revert":
            members, ok, message = revert(args.group)
            print(f"reverted {members}; image " + ("OK" if ok else "FAILED\n" + message))
            return 0 if ok else 1
        else:
            problems = check()
            for group, why in problems:
                print(f"  {group}: {why}")
            print(f"group lock: {len(problems)} problems")
            return 1 if problems else 0
    except GroupError as exc:
        print(f"blob_group: {exc}")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
