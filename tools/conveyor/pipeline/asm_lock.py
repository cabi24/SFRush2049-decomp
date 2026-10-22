"""Assembly-source lock: static functions that were assembly in the original build.

    python3 -m tools.conveyor.pipeline.asm_lock check [--quiet]
    python3 -m tools.conveyor.pipeline.asm_lock list
    python3 -m tools.conveyor.pipeline.asm_lock add NAME --evidence ultralib:src/os/getcount.s
    python3 -m tools.conveyor.pipeline.asm_lock add NAME --evidence "handwritten:REASON"
    python3 -m tools.conveyor.pipeline.asm_lock remove NAME

Owner decision 2026-10-21 (policy (b), docs/static-asm-ceiling.md): a static
function that was assembly in the original build has no C form, so it counts
as static coverage "from assembly", reported apart from C. The source of truth
is the repo's own assembly (splat's disassembly), which the ROM build already
assembles byte-identically; ultralib is cited as provenance, not vendored
(it carries no licence).

`asm_matched.lock.json` (repo root) holds one entry per function:

    source        the .s the ROM build assembles for it: a whole-asm splat
                  segment (asm/us/<SEG>.s) or the GLOBAL_ASM slot its converted
                  TU includes (asm/us/nonmatchings/<tu>/<name>.s)
    vaddr, words  where it is and how many instruction words its region has
    words_sha256  sha256 of those words, as assembled from the source
    evidence      why it has no C form: `ultralib:<file>` (an IDENTICAL row of
                  the wave-28 proof log, cloud/work/frontier/w28s/ceiling/
                  prove.log, covers its address) or `handwritten:<reason>`

`add` refuses a function outside the static population, already promoted to
C, holding a C lock (matched.lock.json), whose source words differ from the
ROM, or whose source is not what the build assembles. `check` re-proves the
pins against the repo's assembly at commit time; it needs the ROM (layout
derive reads function words from it) and skips without one, as in clones that
cannot carry the ROM.
"""
import argparse
import hashlib
import json
import re
import struct
import sys
from pathlib import Path

from . import boot_data, layout
from . import lock as clock

REPO = layout.REPO
LOCKFILE = REPO / "asm_matched.lock.json"
PROVE_LOG = REPO / "cloud" / "work" / "frontier" / "w28s" / "ceiling" / "prove.log"
BASEROM = REPO / "baserom.us.z64"
VRAM_DELTA = layout.VRAM_DELTA

_IDENTICAL = re.compile(r"^(?P<stem>\S+)\s+(?P<vaddr>0x[0-9A-Fa-f]+)\s+RESULT: IDENTICAL \((?P<words>\d+) words")


class AsmLockError(RuntimeError):
    pass


def load_lock(path=LOCKFILE):
    path = Path(path)
    return json.loads(path.read_text()) if path.is_file() else {}


def save_lock(entries, path=LOCKFILE):
    Path(path).write_text(json.dumps(entries, indent=2, sort_keys=True) + "\n")


def _words_sha(words):
    return hashlib.sha256("".join(f"{w:08X}" for w in words).encode()).hexdigest()


def _function(name, mapping):
    for seg in mapping["segments"]:
        for f in seg.get("functions", []):
            if f["name"] == name:
                return seg, f
    raise AsmLockError(f"{name}: not in the static population (layout derive)")


def _vaddr(f):
    v = f.get("vaddr")
    return int(v, 16) if isinstance(v, str) else v


def source_for(seg, f, repo=REPO):
    """The .s the ROM build assembles for this function, or AsmLockError."""
    name = f["name"]
    if seg["converted"]:
        rel = f"asm/us/nonmatchings/{seg['rom_tu']}/{name}.s"
        tu = Path(repo) / "src" / (seg["rom_tu"] + ".c")
        if f'#pragma GLOBAL_ASM("{rel}")' not in tu.read_text():
            raise AsmLockError(f"{name}: {tu.relative_to(repo)} does not include {rel}")
    else:
        rel = f"asm/us/{int(seg['yaml_name'], 16):X}.s"
    path = Path(repo) / rel
    if not path.is_file() or not re.search(rf"^glabel {re.escape(name)}\s*$",
                                           path.read_text(), re.M):
        raise AsmLockError(f"{name}: {rel} does not define it")
    return rel


def region_words(vaddr):
    region = layout.collect_regions().get(vaddr)
    if region is None:
        raise AsmLockError(f"no assembly region at 0x{vaddr:08X}")
    return [int(w, 16) for w in region.words]


def _proof_rows(path=PROVE_LOG):
    rows = []
    for line in Path(path).read_text().splitlines():
        m = _IDENTICAL.match(line)
        if m:
            rows.append((m["stem"], int(m["vaddr"], 16), int(m["words"])))
    return rows


def check_evidence(evidence, vaddr, nwords, prove_log=PROVE_LOG):
    kind, _, detail = evidence.partition(":")
    if kind == "handwritten":
        if not detail.strip():
            raise AsmLockError("handwritten evidence needs a reason")
        return
    if kind != "ultralib":
        raise AsmLockError(f"evidence must be ultralib:<file> or handwritten:<reason>, not {evidence!r}")
    stem = Path(detail).stem
    for row_stem, start, words in _proof_rows(prove_log):
        if row_stem == stem and start <= vaddr and vaddr + 4 * nwords <= start + 4 * words:
            return
    raise AsmLockError(f"no IDENTICAL row for {stem} covering 0x{vaddr:08X} in {prove_log}")


def _boot_routines():
    return {n: (v, size) for v, size, n, _ in boot_data.CPU_ROUTINES}


def boot_source(name, repo=REPO):
    """(source, vaddr, words) of a boot-segment-tail routine built from
    asm/us/boot_data (pipeline.boot_data), or AsmLockError."""
    vaddr, size = _boot_routines()[name]
    src = boot_data.source_of(vaddr)
    rel = f"asm/us/boot_data/{src}"
    if boot_data.makefile_slots(Path(repo) / "Makefile").get(src) != boot_data.expected_slots()[src]:
        raise AsmLockError(f"{name}: the Makefile does not compose {rel} into its data.bin slot")
    words = boot_data.source_words(Path(repo) / rel)
    missing = [v for v in range(vaddr, vaddr + size, 4) if v not in words]
    if missing:
        raise AsmLockError(f"{name}: {rel} lacks 0x{missing[0]:08X}")
    return rel, vaddr, [words[v] for v in range(vaddr, vaddr + size, 4)]


def _boot_entry(name, evidence, repo, baserom):
    rel, vaddr, words = boot_source(name, repo)
    rom = Path(baserom).read_bytes()
    off = boot_data.rom_offset(vaddr)
    if list(struct.unpack(f">{len(words)}I", rom[off:off + 4 * len(words)])) != words:
        raise AsmLockError(f"{name}: {rel} words differ from the ROM")
    check_evidence(evidence, vaddr, len(words))
    return {"target_id": name, "source": rel, "vaddr": f"0x{vaddr:08X}", "words": len(words),
            "words_sha256": _words_sha(words), "slot_bytes": 4 * len(words), "evidence": evidence}


def make_entry(name, evidence, mapping=None, repo=REPO, baserom=BASEROM, c_lock=None):
    if name in _boot_routines():
        return _boot_entry(name, evidence, repo, baserom)
    mapping = mapping or layout.derive()
    seg, f = _function(name, mapping)
    if f.get("state") == "promoted":
        raise AsmLockError(f"{name}: already promoted to C")
    c_lock = clock.load_lock() if c_lock is None else c_lock
    if any(e.get("target_id") == name for e in c_lock.values()):
        raise AsmLockError(f"{name}: has a C lock in matched.lock.json; promote the C instead")
    vaddr = _vaddr(f)
    rel = source_for(seg, f, repo)
    words = region_words(vaddr)
    rom = Path(baserom).read_bytes()
    off = vaddr - VRAM_DELTA
    if list(struct.unpack(f">{len(words)}I", rom[off:off + 4 * len(words)])) != words:
        raise AsmLockError(f"{name}: {rel} words differ from the ROM")
    check_evidence(evidence, vaddr, len(words))
    return {"target_id": name, "source": rel, "vaddr": f"0x{vaddr:08X}", "words": len(words),
            "words_sha256": _words_sha(words), "slot_bytes": f["size"], "evidence": evidence}


def check(entries, mapping=None, repo=REPO):
    """[(name, problem)] for every entry the repo no longer supports."""
    mapping = mapping or layout.derive()
    problems = []
    for name, e in sorted(entries.items()):
        try:
            if name in _boot_routines():
                rel, vaddr, words = boot_source(name, repo)
                if rel != e["source"] or f"0x{vaddr:08X}" != e["vaddr"] or _words_sha(words) != e["words_sha256"]:
                    problems.append((name, "assembly words drifted from the locked form"))
                continue
            seg, f = _function(name, mapping)
            if f.get("state") == "promoted":
                problems.append((name, "promoted to C: remove the asm entry"))
                continue
            if source_for(seg, f, repo) != e["source"]:
                problems.append((name, "the build assembles a different source now"))
            words = region_words(_vaddr(f))
            if f"0x{_vaddr(f):08X}" != e["vaddr"] or _words_sha(words) != e["words_sha256"]:
                problems.append((name, "assembly words drifted from the locked form"))
        except AsmLockError as exc:
            problems.append((name, str(exc)))
    return problems


def coverage(mapping, entries=None):
    """{name: slot_bytes} of locked functions that are still assembly in the build."""
    entries = load_lock() if entries is None else entries
    out = {}
    for seg in mapping["segments"]:
        if seg["refusal"]:
            continue
        for f in seg.get("functions", []):
            e = entries.get(f["name"])
            if e and f.get("state") != "promoted" and e["vaddr"] == f"0x{_vaddr(f):08X}":
                out[f["name"]] = f["size"]
    return out


def boot_coverage(entries=None):
    """{name: bytes} of boot-segment-tail routines built from locked assembly."""
    entries = load_lock() if entries is None else entries
    return {n: size for n, (_, size) in _boot_routines().items() if n in entries}


def static_coverage():
    """{"functions", "bytes"} of the static population built from locked assembly,
    including the boot segment tail's routines ("boot_functions")."""
    entries = load_lock()
    by_name = coverage(layout.derive(), entries)
    boot = boot_coverage(entries)
    return {"functions": len(by_name) + len(boot), "bytes": sum(by_name.values()) + sum(boot.values()),
            "boot_functions": len(boot)}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("check")
    p.add_argument("--quiet", action="store_true")
    sub.add_parser("list")
    p = sub.add_parser("add")
    p.add_argument("names", nargs="+")
    p.add_argument("--evidence", required=True)
    p = sub.add_parser("remove")
    p.add_argument("names", nargs="+")
    args = parser.parse_args(argv)

    entries = load_lock()
    if args.command == "check":
        if not BASEROM.is_file():
            if not args.quiet:
                print("asm lock: skipped (needs baserom.us.z64)")
            return 0
        problems = check(entries)
        for name, problem in problems:
            print(f"asm lock: {name}: {problem}", file=sys.stderr)
        if not problems and not args.quiet:
            print(f"asm lock: {len(entries)} assembly functions intact")
        return 1 if problems else 0
    if args.command == "list":
        for name, e in sorted(entries.items(), key=lambda kv: kv[1]["vaddr"]):
            print(f"{e['vaddr']}  {e['slot_bytes']:5d}  {name:24s} {e['source']}  [{e['evidence']}]")
        return 0
    if args.command == "remove":
        for name in args.names:
            entries.pop(name, None)
        save_lock(entries)
        return 0
    mapping = layout.derive()
    c_lock = clock.load_lock()
    for name in args.names:
        try:
            entries[name] = make_entry(name, args.evidence, mapping, c_lock=c_lock)
        except AsmLockError as exc:
            sys.exit(f"asm lock: {exc}")
        print(f"asm lock: {name} <- {entries[name]['source']} [{args.evidence}]")
    save_lock(entries)
    return 0


if __name__ == "__main__":
    sys.exit(main())
