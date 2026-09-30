#!/usr/bin/env python3
"""Score C against Rush 2049 game-code targets using only the repository.

    # one function, compiled alone (the -O2 pipeline):
    python3 tools/cloud/score.py fn  path/to/file.c  func_name [--flags "-g0 -O2 -mips2 -G 0 -non_shared"]

    # an IDO -O3 interprocedural call group (010), from a group directory
    # holding group.json + its .c files:
    python3 tools/cloud/score.py group path/to/group_dir

Targets come from asm/us/blob/*.s: every game-code function is a section
`.text.<name>` of `.word`s, i.e. the exact bytes of the retail image. The
compiled function is compared word by word after resolving relocations with
asm/us/blob/symbols.json. Local data-section references remain unverified,
not matches. --allow-unverified permits those references but never unresolved
symbols or differing words. The maintainers still run the image and ROM hash
gates when a match is spliced (see CloudHandoff.md).

Region files and symbol addresses must match asm/us/blob/SHA256SUMS; integrity
failures are fatal, including when --allow-unverified is used.

Needs tools/cloud/setup.sh (IDO 5.3 into tools/cloud/ido/). A MIPS objdump
(mips-linux-gnu-objdump) is optional and only used to disassemble diffs.
"""
import argparse
from dataclasses import dataclass
import hashlib
import json
import os
import re
import shlex
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
ASM_DIR = REPO / "asm" / "us" / "blob"
IDO = Path(os.environ.get("IDO_DIR", REPO / "tools" / "cloud" / "ido"))
OBJDUMP = "mips-linux-gnu-objdump"
DEFAULT_FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"
# The game was assembled with as1 -r4300_mul (VR4300 multiply errata: a nop
# between back-to-back multiplies). With -non_shared the cc driver does not
# pass it, so every compile here adds it. It cannot change a body that already
# matches: the retail code never has two adjacent multiplies.
# Evidence: cloud/work/R4300_MUL.md.
R4300_AS1 = "-r4300_mul"
R4300_CC = "-Wab,-r4300_mul"
MASKS = {"R_MIPS_26": 0xFC000000, "R_MIPS_HI16": 0xFFFF0000, "R_MIPS_LO16": 0xFFFF0000}


# --- targets ---------------------------------------------------------------

_targets = None
_target_fingerprint = None


def target_manifest():
    path = ASM_DIR / "SHA256SUMS"
    try:
        entries = {}
        for line in path.read_text(encoding="utf-8").splitlines():
            match = re.fullmatch(r"([0-9a-f]{64})  ([A-Za-z0-9_.-]+)", line)
            if not match or match[2] in entries or match[2] in (".", ".."):
                raise ValueError("malformed or duplicate manifest entry")
            entries[match[2]] = match[1]
        if "symbols.json" not in entries:
            raise ValueError("symbols.json is not covered")
        return entries
    except (OSError, ValueError) as exc:
        raise SystemExit(f"target integrity check failed: cannot read {path}: {exc}")


def verified_bytes(path, manifest):
    try:
        expected = manifest[path.name]
        data = path.read_bytes()
    except (OSError, KeyError) as exc:
        raise SystemExit(f"target integrity check failed for {path}: missing file or hash ({exc})")
    if hashlib.sha256(data).hexdigest() != expected:
        raise SystemExit(f"target integrity check failed for {path}: SHA-256 mismatch; "
                         "restore the protected files from a trusted checkout")
    return data


def targets():
    """{function name: [words]} from every region file."""
    global _targets, _target_fingerprint
    manifest = target_manifest()
    paths = sorted(ASM_DIR.glob("*.s"))
    expected = {name for name in manifest if name.endswith(".s")}
    actual = {path.name for path in paths}
    if not expected or actual != expected:
        raise SystemExit("target integrity check failed: region file set differs from SHA256SUMS "
                         f"(missing: {sorted(expected - actual)}, extra: {sorted(actual - expected)})")
    # Recheck even cached targets; parse the same bytes that passed the hash.
    contents = [(path, verified_bytes(path, manifest)) for path in paths]
    fingerprint = (str(ASM_DIR.resolve()), tuple(sorted(manifest.items())))
    if _targets is None or _target_fingerprint != fingerprint:
        parsed = {}
        for path, data in contents:
            current = None
            for line in data.decode("utf-8").splitlines():
                m = re.match(r"\.section \.text\.(\S+?),", line.strip())
                if m:
                    current = parsed.setdefault(m.group(1), [])
                    continue
                if line.strip().startswith(".section"):
                    current = None
                    continue
                m = re.match(r"\s*\.word\s+(0x[0-9A-Fa-f]+)", line)
                if m and current is not None:
                    current.append(int(m.group(1), 16))
        _targets = parsed
        _target_fingerprint = fingerprint
    return _targets


# --- object helpers (a minimal ELF32 big-endian reader: no binutils needed) ---

def _run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def _elf(obj):
    data = Path(obj).read_bytes()
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2:
        raise SystemExit(f"{obj}: not a 32-bit big-endian ELF")
    shoff, = struct.unpack_from(">I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from(">HHH", data, 0x2E)
    secs = []
    for i in range(shnum):
        name, typ, flags, addr, off, size, link, info, align, entsize = struct.unpack_from(
            ">IIIIIIIIII", data, shoff + i * shentsize)
        secs.append(dict(name=name, type=typ, off=off, size=size, link=link, info=info))
    strtab = secs[shstrndx]
    for sec in secs:
        raw = data[strtab["off"] + sec["name"]:]
        sec["name"] = raw[:raw.index(b"\0")].decode()
    return data, secs


def _text_index(secs):
    return next(i for i, s in enumerate(secs) if s["name"] == ".text")


def text_words(obj):
    data, secs = _elf(obj)
    t = secs[_text_index(secs)]
    raw = data[t["off"]:t["off"] + t["size"]]
    return list(struct.unpack(f">{len(raw) // 4}I", raw))


def _symbol_table(data, secs, index):
    """Read the symbol table selected by a relocation section's sh_link."""
    sec = secs[index]
    names = secs[sec["link"]]
    out = []
    for k in range(sec["size"] // 16):
        st_name, value, size, info, other, shndx = struct.unpack_from(
            ">IIIBBH", data, sec["off"] + 16 * k)
        raw = data[names["off"] + st_name:]
        name = raw[:raw.index(b"\0")].decode()
        if (info & 0xF) == 3 and not name and shndx < len(secs):
            name = secs[shndx]["name"]
        out.append(dict(name=name, value=value, size=size,
                        type=info & 0xF, section=shndx))
    return out


def symbols(obj):
    """{name: .text offset} for function symbols defined in .text."""
    data, secs = _elf(obj)
    text = _text_index(secs)
    return {sym["name"]: sym["value"]
            for i, sec in enumerate(secs) if sec["type"] == 2
            for sym in _symbol_table(data, secs, i)
            if sym["section"] == text and sym["type"] == 2}


def image_symbols():
    path = ASM_DIR / "symbols.json"
    try:
        return {name: int(address, 16)
                for name, address in json.loads(verified_bytes(path, target_manifest()))["symbols"].items()}
    except (OSError, ValueError, KeyError, TypeError) as exc:
        raise SystemExit(f"cannot read symbol table {path}: {exc}")


def address_named(name):
    # Same fallback as blob_splice.address_named; cloud tools stay stdlib-only.
    match = re.fullmatch(r"(?:func|D)_([0-9A-Fa-f]{8})", name)
    return int(match.group(1), 16) if match else None


def relocate(obj, words, start, end, addresses):
    """Resolve REL records in the compared slice, retaining all uncertainty.

    .text section addends are object offsets: map through their owning
    function before adding its image address. Other section addresses cannot
    be inferred from the repository and only their relocation bits are masked.
    """
    data, secs = _elf(obj)
    text = _text_index(secs)
    got = list(words)
    masks, unresolved, unverified, errors = {}, [], [], []
    rtypes = {4: "R_MIPS_26", 5: "R_MIPS_HI16", 6: "R_MIPS_LO16"}

    def named(name):
        return addresses[name] if name in addresses else address_named(name)

    for sec in secs:
        if sec["type"] != 9 or sec["info"] != text:  # SHT_REL for .text
            continue
        syms = _symbol_table(data, secs, sec["link"])
        functions = [s for s in syms if s["section"] == text and s["type"] == 2]
        pending_hi = []

        def resolve(sym, addend):
            if sym["type"] == 3:
                if sym["section"] != text:
                    return None, "section"
                owners = []
                for fn in functions:
                    following = min((f["value"] for f in functions
                                     if f["value"] > fn["value"]),
                                    default=len(words) * 4)
                    limit = min(following, fn["value"] + fn["size"]) if fn["size"] else following
                    if fn["value"] <= addend < limit:
                        owners.append(fn)
                for fn in owners:
                    base = named(fn["name"])
                    if base is not None:
                        return base + addend - fn["value"], None
                return None, "unresolved"
            base = named(sym["name"])
            return (base + addend, None) if base is not None else (None, "unresolved")

        def apply(offset, rtype, sym, addend):
            value, reason = resolve(sym, addend)
            detail = f"{sym['name'] or '<unnamed>'}{addend:+#x} at +0x{offset - start:x}"
            if reason == "section":
                masks[offset] = MASKS[rtypes[rtype]]
                unverified.append(detail)
            elif reason:
                unresolved.append(detail)
            else:
                insn = words[offset // 4]
                if rtype == 4:
                    if value & 3:
                        errors.append(f"unaligned call target {detail}")
                        return
                    got[offset // 4] = (insn & 0xFC000000) | ((value >> 2) & 0x03FFFFFF)
                elif rtype == 5:
                    got[offset // 4] = (insn & 0xFFFF0000) | (((value + 0x8000) >> 16) & 0xFFFF)
                else:
                    got[offset // 4] = (insn & 0xFFFF0000) | (value & 0xFFFF)

        for k in range(sec["size"] // 8):
            offset, info = struct.unpack_from(">II", data, sec["off"] + 8 * k)
            if not start <= offset < end:
                continue
            index, rtype = info >> 8, info & 0xFF
            if offset % 4 or offset + 4 > len(words) * 4 or index >= len(syms):
                errors.append(f"invalid relocation at .text+0x{offset:x}")
                continue
            sym, insn = syms[index], words[offset // 4]
            if rtype == 4:
                apply(offset, rtype, sym, (insn & 0x03FFFFFF) << 2)
            elif rtype == 5:
                pending_hi.append((offset, index, insn & 0xFFFF))
            elif rtype == 6:
                lo = insn & 0xFFFF
                lo = lo - 0x10000 if lo & 0x8000 else lo
                his = [h for h in pending_hi if h[1] == index]
                for hi_offset, _, hi in his:
                    apply(hi_offset, 5, sym, (hi << 16) + lo)
                addend = ((his[0][2] << 16) if his else 0) + lo
                apply(offset, rtype, sym, addend)
                pending_hi = [h for h in pending_hi if h[1] != index]
            else:
                errors.append(f"unsupported relocation type {rtype} at .text+0x{offset:x}")
        for offset, index, _ in pending_hi:
            errors.append(f"unpaired R_MIPS_HI16 for {syms[index]['name']} at .text+0x{offset:x}")
    return got, masks, unresolved, unverified, errors


@dataclass
class Comparison:
    differing: int
    total: int
    unresolved: list
    unverified: list
    errors: list
    extra_words: int = 0

    def accepted(self, allow_unverified=False):
        return (self.differing == 0 and self.extra_words == 0
                and not self.unresolved and not self.errors
                and (allow_unverified or not self.unverified))

    def summary(self):
        status = f"{self.differing}/{self.total} words differ" if self.differing else "MATCH"
        if self.unresolved or self.errors:
            if not self.differing:
                status = "NOT VERIFIED"
        details = []
        if self.extra_words:
            if not self.differing:
                status = "MISMATCH"
            details.append(f"{self.extra_words} extra words (nonzero beyond target length)")
        if self.unresolved:
            details.append("unresolved symbols: " + ", ".join(self.unresolved))
        if self.unverified:
            details.append(f"{len(self.unverified)} section-relative relocations unverified: "
                           + ", ".join(self.unverified))
        details.extend(self.errors)
        return status + (" (" + "; ".join(details) + ")" if details else "")


def disasm_word(word):
    """Best-effort disassembly for display (any MIPS objdump), else blank."""
    for tool in (os.environ.get("MIPS_OBJDUMP"), OBJDUMP, "mips64-elf-objdump",
                 str(REPO / "tools" / "cloud" / "objdump")):
        if not tool:
            continue
        try:
            with tempfile.NamedTemporaryFile(suffix=".bin") as f:
                f.write(struct.pack(">I", word))
                f.flush()
                out = _run([tool, "-D", "-b", "binary", "-m", "mips:4300", "-EB", f.name]).stdout
        except OSError:
            continue
        for line in out.splitlines():
            m = re.match(r"\s*0:\s+[0-9a-f]{8}\s+(.*)", line)
            if m:
                return re.sub(r"\s+", " ", m.group(1))
    return ""


def compare(obj, name, start=None, show=12):
    """Full-word comparison and verification status for one function."""
    want = targets().get(name)
    if want is None:
        raise SystemExit(f"no target section .text.{name} in {ASM_DIR}")
    words = text_words(obj)
    functions = symbols(obj)
    if start is None:
        start = functions.get(name)
        if start is None:
            raise SystemExit(f"{name} is not a defined function in the compiled object")
    end = min((offset for offset in functions.values() if offset > start),
              default=len(words) * 4)
    target_end = start + len(want) * 4
    extra_words = sum(word != 0 for word in words[target_end // 4:end // 4])
    resolved, masks, unresolved, unverified, errors = relocate(
        obj, words, start, min(target_end, end), image_symbols())
    got = resolved[start // 4:min(target_end, end) // 4]
    bad = []
    for i, w in enumerate(want):
        g = got[i] if i < len(got) else None
        mask = masks.get(start + 4 * i, 0xFFFFFFFF)
        if g is None or (g & mask) != (w & mask):
            bad.append(i)
    for i in bad[:show]:
        g = got[i] if i < len(got) else None
        print(f"    +0x{4*i:03x}  want {w_(want[i])}  got {w_(g) if g is not None else '(missing)'}")
    return Comparison(len(bad), len(want), unresolved, unverified, errors, extra_words)


def w_(word):
    return f"{word:08x} {disasm_word(word):28s}"


# --- builds ---------------------------------------------------------------------

def ido(tool):
    path = IDO / tool
    if not path.exists():
        raise SystemExit(f"{path} missing: run tools/cloud/setup.sh (or set IDO_DIR)")
    return str(path)


def compile_single(source, flags, out):
    flags = shlex.split(flags)
    if R4300_CC not in flags:
        flags.append(R4300_CC)
    proc = _run([ido("cc"), "-c", *flags, "-o", str(out), str(source)])
    if proc.returncode != 0 or not Path(out).exists():
        raise SystemExit("IDO compile failed:\n" + (proc.stderr or proc.stdout)[:3000])


def compile_group(group_dir, out):
    """Whole-program -O3 with uld -kp <keep list>: the build the ROM used
    (specs/010-ipa-call-groups/research/s2-s5-spikes.md)."""
    spec = json.loads((group_dir / "group.json").read_text())
    out = Path(out).resolve()
    with tempfile.TemporaryDirectory(prefix="grp-") as tmp:
        work = Path(tmp)
        for name in spec["files"]:
            (work / name).write_text((group_dir / name).read_text())
        (work / "keep.txt").write_text("".join(k + "\n" for k in spec["keep"]))
        flags = shlex.split(spec["flags"])
        units = [re.sub(r"\.c$", ".u", f) for f in spec["files"]]
        common = ["-mips2", "-EB", "-g0", "-O3"]
        steps = [
            [ido("cc"), "-j", *flags, *spec["files"]],
            [ido("uld"), "-L/usr/lib/mips2/nonshared", "-_SYSTYPE_SVR4", "-mips2", "-non_shared",
             "-g0", "-no_AutoGnum", "-kp", "keep.txt", *units, "-ko", "linked"],
            [ido("usplit"), "-mips2", "-o", "split", "-t", "st", "linked"],
            [ido("umerge"), "-Olimit", "5000", *common, "split", "-o", "merged", "-t", "st"],
            [ido("uopt"), "-G", "0", "-Olimit", "5000", *common, "merged", "opt", "-t", "st", "optlog"],
            [ido("ugen"), "-G", "0", *common, "opt", "-o", "gen", "-t", "st", "-temp", "ugtmp"],
            [ido("as1"), "-elf", "-G", "0", "-p0", *common, R4300_AS1, "-Olimit", "5000", "gen", "-o", str(out),
             "-t", "st"],
        ]
        for step in steps:
            proc = _run(step, cwd=work)
            if proc.returncode != 0:
                raise SystemExit(f"{Path(step[0]).name} failed:\n" + (proc.stderr or proc.stdout)[:3000])
    return spec


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="mode", required=True)
    f = sub.add_parser("fn")
    f.add_argument("source")
    f.add_argument("name")
    f.add_argument("--flags", default=DEFAULT_FLAGS)
    g = sub.add_parser("group")
    g.add_argument("group_dir")
    g.add_argument("--claims", action="store_true",
                   help="CI mode: judge only the members group.json lists under "
                        "\"claims\" (none listed: report only), honouring its "
                        "\"allow_unverified\"")
    for command in (f, g):
        command.add_argument("--allow-unverified", action="store_true",
                             help="permit local data-section relocations; still reject other failures")
    args = parser.parse_args()

    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "out.o"
        if args.mode == "fn":
            compile_single(args.source, args.flags, obj)
            names = [args.name]
            context = []
        else:
            claims_declared = None
            if args.claims:
                claims_declared = json.loads(
                    (Path(args.group_dir) / "group.json").read_text()).get("claims")
            try:
                spec = compile_group(Path(args.group_dir), obj)
            except SystemExit as exc:
                if args.claims and not claims_declared:
                    # Work in progress that does not build yet: report only.
                    print(f"Does not build (no claims, reported only): {exc}")
                    return 0
                raise
            names = spec["members"]
            context = spec.get("context", [])
            if args.claims:
                claimed = spec.get("claims", [])
                unknown = sorted(set(claimed) - set(names))
                if unknown:
                    raise SystemExit(f"claims name non-members: {unknown}")
                context = [n for n in names if n not in claimed] + context
                names = list(claimed)
                args.allow_unverified = bool(spec.get("allow_unverified"))
                if not names:
                    print("No claims: work in progress, reported only.")
            print("Members:")
        all_match = True
        for name in names:
            print(f"{name}:")
            result = compare(obj, name)
            print(f"  {result.summary()}")
            all_match &= result.accepted(args.allow_unverified)
        if context:
            print("\nContext (informational; excluded from exit status):")
            for name in context:
                print(f"{name}:")
                try:
                    result = compare(obj, name)
                except SystemExit as exc:
                    # Missing targets or compiled-away context are diagnostic,
                    # just like context word differences: only members gate.
                    print(f"  NOT VERIFIED ({exc})")
                else:
                    print(f"  {result.summary()}")
    return 0 if all_match else 1


if __name__ == "__main__":
    sys.exit(main())
