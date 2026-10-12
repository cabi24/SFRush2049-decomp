"""Rename static (boot segment) functions in the repo as one simultaneous permutation.

    python3 -m tools.conveyor.pipeline.static_rename plan    MAP.json
    python3 -m tools.conveyor.pipeline.static_rename symbols MAP.json
    python3 -m tools.conveyor.pipeline.static_rename source  MAP.json FILE...
    python3 -m tools.conveyor.pipeline.static_rename lock    MAP.json [--base REV]
    python3 -m tools.conveyor.pipeline.static_rename owned   MAP.json [--base REV]

MAP.json is a rename table ({"renames": [{"vram", "old", "new", "status"}...]});
only rows with status "accepted" are applied. The names form a permutation
that may contain cycles (osSendMesg <-> osJamMesg), so every step rewrites all
names at once from the ORIGINAL text, never one name after another.

`symbols` rewrites the definition lines of symbol_addrs.us.txt (each old name
must be defined exactly once, at the row's vram; a new name must not be defined
elsewhere). A `dup-addr` comment line that spells a new name at another address
is dropped (it was a guess the rename contradicts).

`source` rewrites identifiers in C code and the asm paths of GLOBAL_ASM/INCLUDE_ASM
strings. Comments and other string literals are left alone: a comment that
names a function by its true identity ("retail implementation is SDK X") would
be inverted by a blind permutation, so comments are reviewed by hand.

`lock` migrates matched.lock.json: key (`path:function`) and `target_id` follow
the map, and a body_sha256 changes only with proof: the function's body at
--base (default HEAD) must hash to the pinned value, and the same body with the
permutation applied must hash to the body now on disk. Anything else refuses
(nothing is written). Entries whose body did not change are kept verbatim.

`owned` rebinds rom_owned_data.json: member/function/alias names follow the
map, prior lock pins follow the `lock` rules, pins of live files (TUs, headers)
are refreshed only when the file at --base held the recorded hash, and pinned
research files (a storage owner's complete source and compiler-context hash
list, a text boundary's canonical source and endpoint proof) are re-issued as
renamed copies under --rebind-dir; the originals stay as provenance.

The Conveyor inventory is renamed separately, in place, by
`targets_rename rename MAP.json` (cycle-safe, one transaction).
"""
import argparse
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path

from .lock import LOCKFILE, extract_functions, normalize_body

REPO = Path(__file__).resolve().parents[3]
SYMBOL_ADDRS = REPO / "symbol_addrs.us.txt"
OWNED = REPO / "rom_owned_data.json"
_IDENT = re.compile(r"[A-Za-z_]\w*")
_DEF = re.compile(r"^(\s*)([A-Za-z_]\w*)(\s*=\s*)(0x[0-9A-Fa-f]+)(\s*;)(.*)$")


class RenameError(Exception):
    """The rename cannot be applied safely; nothing was written."""


def load_map(path):
    """[(vram, old, new)] for the accepted rows; validates the permutation."""
    doc = json.loads(Path(path).read_text())
    rows = [r for r in doc["renames"] if r.get("status", "accepted") == "accepted"]
    olds = [r["old"] for r in rows]
    news = [r["new"] for r in rows]
    vrams = [int(r["vram"], 16) for r in rows]
    for label, seq in (("old name", olds), ("new name", news), ("vram", vrams)):
        dup = sorted({str(x) for x in seq if seq.count(x) > 1})
        if dup:
            raise RenameError(f"{label} listed twice: {', '.join(dup)}")
    for n in news:
        if not _IDENT.fullmatch(n):
            raise RenameError(f"{n!r} is not a C identifier")
    return [(v, o, n) for v, o, n in zip(vrams, olds, news) if o != n]


def mapping(rows):
    return {o: n for _v, o, n in rows}


def permute_identifiers(text, names):
    return _IDENT.sub(lambda m: names.get(m.group(0), m.group(0)), text)


# --- C source -------------------------------------------------------------------

_ASM_PATH = re.compile(r'^"[^"\n]*\.s"$')


def permute_c(text, names):
    """Rename identifiers in code and in asm-path string literals only."""
    out, code = [], []
    i, n = 0, len(text)

    def flush():
        if code:
            out.append(permute_identifiers("".join(code), names))
            code.clear()

    while i < n:
        c = text[i]
        if c == "/" and text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j == -1 else j + 2
            flush()
            out.append(text[i:j])
            i = j
        elif c == "/" and text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j == -1 else j
            flush()
            out.append(text[i:j])
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            flush()
            lit = text[i:j]
            out.append(permute_identifiers(lit, names) if _ASM_PATH.match(lit) else lit)
            i = j
        else:
            code.append(c)
            i += 1
    flush()
    return "".join(out)


# --- symbol_addrs ---------------------------------------------------------------

def rewrite_symbols(text, rows):
    """symbol_addrs.us.txt with every old definition renamed (one pass)."""
    by_old = {o: (v, n) for v, o, n in rows}
    news = {n for _v, _o, n in rows}
    seen, out, dropped = {}, [], []
    for line in text.splitlines(keepends=True):
        m = _DEF.match(line.rstrip("\n"))
        if m and m.group(2) in by_old:
            name, addr = m.group(2), int(m.group(4), 16)
            vram, new = by_old[name]
            if addr != vram:
                raise RenameError(f"{name} is defined at 0x{addr:08X}, the table says 0x{vram:08X}")
            seen[name] = seen.get(name, 0) + 1
            keep = " ".join(re.findall(r"\b(?:size|rom|segment|align|name_end):\S+", m.group(6)))
            comment = (f" // type:func {keep + ' ' if keep else ''}libultra {new}"
                       f" (renamed from {name}, docs/SYMBOL_MISATTRIBUTION.md)")
            nl = "\n" if line.endswith("\n") else ""
            out.append(f"{m.group(1)}{new}{m.group(3)}{m.group(4)}{m.group(5)}{comment}{nl}")
            continue
        if m and m.group(2) in news and m.group(2) not in by_old:
            raise RenameError(f"{m.group(2)} is already defined at {m.group(4)}")
        d = re.match(r"\s*//\s*dup-addr\([^)]*\):\s*(\w+)\s*=", line)
        if d and d.group(1) in news:
            dropped.append(line.strip())
            continue
        out.append(line)
    missing = sorted(set(by_old) - set(seen))
    twice = sorted(k for k, c in seen.items() if c > 1)
    if missing or twice:
        raise RenameError(f"not defined exactly once: {', '.join(missing + twice)}")
    return "".join(out), dropped


# --- locks ----------------------------------------------------------------------

def _git_show(rev, rel):
    try:
        return subprocess.run(["git", "show", f"{rev}:{rel}"], cwd=REPO, check=True,
                              capture_output=True, text=True).stdout
    except subprocess.CalledProcessError:
        return None


def _body(text, name):
    if text is None:
        return None
    for got, start, end in extract_functions(text):
        if got == name:
            return text[start:end]
    return None


def _sha(text):
    return hashlib.sha256(text.encode()).hexdigest()


def _path_after(rel, names):
    """A source path whose basename is a renamed function follows the rename."""
    p = Path(rel)
    return str(p.with_name(names.get(p.stem, p.stem) + p.suffix)) if p.stem in names else rel


def migrate_entry(spec, entry, names, base, repo=REPO):
    """(new_spec, new_entry, changed) for one lock entry, or raise RenameError."""
    rel, _, fn = spec.partition(":")
    new_rel, new_fn = _path_after(rel, names), names.get(fn, fn)
    old_body = _body(_git_show(base, rel), fn)
    if old_body is None:
        raise RenameError(f"{spec}: no body at {base}")
    if _sha(normalize_body(old_body)) != entry["body_sha256"]:
        raise RenameError(f"{spec}: the body at {base} does not hold its lock")
    path = Path(repo) / new_rel
    new_body = _body(path.read_text(errors="replace") if path.is_file() else None, new_fn)
    if new_body is None:
        raise RenameError(f"{spec}: {new_rel}:{new_fn} not found on disk")
    want = _sha(normalize_body(permute_c(old_body, names)))
    got = _sha(normalize_body(new_body))
    if got != want:
        raise RenameError(f"{spec}: {new_rel}:{new_fn} differs from the renamed locked body")
    new = dict(entry)
    tid = entry.get("target_id")
    if tid is not None:
        new["target_id"] = names.get(tid, tid)
    new["body_sha256"] = got
    new_spec = f"{new_rel}:{new_fn}"
    return new_spec, new, (new_spec != spec or new != entry)


def migrate_lock(entries, names, base, repo=REPO):
    out, changed = {}, []
    for spec, entry in sorted(entries.items()):
        new_spec, new, moved = migrate_entry(spec, entry, names, base, repo)
        if new_spec in out:
            raise RenameError(f"{new_spec}: two lock entries would share this key")
        out[new_spec] = new
        if moved:
            changed.append((spec, new_spec))
    return out, changed


# --- rom_owned_data.json -------------------------------------------------------

def _file_sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def _rebind_file_pin(holder, path_key, sha_key, base, repo, log):
    """Refresh holder[sha_key] for the file at holder[path_key] if the rename changed it."""
    rel = holder.get(path_key)
    if not rel or sha_key not in holder:
        return False
    on_disk = _file_sha(Path(repo) / rel)
    if on_disk == holder[sha_key]:
        return False
    before = _git_show(base, rel)
    if before is None or _sha(before) != holder[sha_key]:
        raise RenameError(f"{rel}: pinned {sha_key} did not hold at {base}")
    log.append(f"{rel}: {sha_key} {holder[sha_key][:12]} -> {on_disk[:12]}")
    holder[sha_key] = on_disk
    return True


def _rebind_copy(holder, path_key, sha_key, names, rebind_dir, repo, log, live=None, name=None):
    """Point holder[path_key] at a renamed copy of the pinned research file.

    The copy is the original with permute_c applied, written under rebind_dir;
    the original stays as provenance. With `live` (the file the pin must equal,
    e.g. a storage owner's TU) the copy is that file's text, accepted only when
    it equals the renamed original up to comments and whitespace (the rename
    also retitles PROMOTED headers)."""
    rel = holder[path_key]
    src = Path(repo) / rel
    if _file_sha(src) != holder[sha_key]:
        raise RenameError(f"{rel}: pinned {sha_key} does not hold")
    text = src.read_text()
    new = permute_c(text, names) if rel.endswith((".c", ".h")) else text
    if live is not None:
        live_text = Path(live).read_text()
        if normalize_body(new) != normalize_body(live_text):
            raise RenameError(f"{rel}: {live} is not its renamed copy (beyond comments)")
        new = live_text
    out = Path(rebind_dir) / (name or Path(rel).name)
    if out.exists() and out.read_text() != new:
        raise RenameError(f"{out}: rebind name collision")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(new)
    digest = _file_sha(out)
    holder[path_key] = str(out.relative_to(repo))
    holder[sha_key] = digest
    log.append(f"{rel} -> {holder[path_key]} ({digest[:12]})")


def rebind_owned(doc, names, base, rebind_dir, repo=REPO):
    """rom_owned_data.json with names renamed and every pin the rename breaks rebound.

    Live files (TUs, headers) get their pins refreshed after proving the file
    held the pin at `base`; pinned research copies (a storage owner's complete
    source, its compiler-context hash list, a text boundary's canonical source
    and endpoint proof) are re-issued as renamed copies under rebind_dir."""
    repo = Path(repo)
    log = []
    for row in doc.get("storage_blocks", []):
        for key in ("members", "new_members"):
            if key in row:
                row[key] = [names.get(m, m) for m in row[key]]
        prior = row.get("prior_locks")
        if prior:
            new_prior = {}
            for fn, entry in prior.items():
                new_spec, new, _ = migrate_entry(f"{row['tu']}:{fn}", entry, names, base, repo)
                new_prior[new_spec.rpartition(":")[2]] = new
            row["prior_locks"] = new_prior
        live = _file_sha(repo / row["tu"])
        if live != row["source_sha256"]:
            if _sha(_git_show(base, row["tu"]) or "") != row["source_sha256"]:
                raise RenameError(f"{row['tu']}: source_sha256 did not hold at {base}")
            _rebind_copy(row, "source", "source_sha256", names, rebind_dir, repo, log,
                         live=repo / row["tu"])
        for item in row.get("context_files", []):
            _rebind_file_pin(item, "path", "sha256", base, repo, log)
        if row.get("context_proof"):
            ctx = json.loads((repo / row["context_proof"]).read_text())
            fresh = {p: _file_sha(repo / p) for p in ctx}
            if fresh != ctx:
                for p, digest in ctx.items():
                    if fresh[p] != digest and _sha(_git_show(base, p) or "") != digest:
                        raise RenameError(f"{p}: context hash did not hold at {base}")
                out = Path(rebind_dir) / f"{row['owner']}_context_hashes.json"
                out.write_text(json.dumps(fresh, indent=2) + "\n")
                log.append(f"{row['context_proof']} -> {out.relative_to(repo)} "
                           f"({sum(fresh[p] != ctx[p] for p in ctx)} headers rehashed)")
                row["context_proof"] = str(out.relative_to(repo))
                row["context_proof_sha256"] = _file_sha(out)
    for row in doc.get("text_boundaries", []):
        old_fn = row.get("function")
        if old_fn in names:
            row["function"] = names[old_fn]
            body = _body((repo / row["tu"]).read_text(), row["function"])
            old = _body(_git_show(base, row["tu"]), old_fn)
            if body is None or old is None or _sha(normalize_body(old)) != row["body_sha256"] \
                    or _sha(normalize_body(permute_c(old, names))) != _sha(normalize_body(body)):
                raise RenameError(f"text boundary {row.get('owner')}: body pin cannot be migrated")
            row["body_sha256"] = _sha(normalize_body(body))
            _rebind_copy(row, "source", "source_sha256", names, rebind_dir, repo, log,
                         name=row["function"] + Path(row["source"]).suffix)
            auth_rel = row["asm_authority"]
            if _file_sha(repo / auth_rel) != row["asm_authority_sha256"]:
                raise RenameError(f"{auth_rel}: pinned asm_authority_sha256 does not hold")
            auth = json.loads((repo / auth_rel).read_text())
            auth["function"] = names.get(auth.get("function"), auth.get("function"))
            if isinstance(auth.get("authority"), str):
                auth["authority"] = permute_identifiers(auth["authority"], names)
            auth["rebind"] = (f"renamed copy of {auth_rel} (sha256 {row['asm_authority_sha256']}):"
                              f" {old_fn} -> {row['function']}, docs/SYMBOL_MISATTRIBUTION.md")
            out = Path(rebind_dir) / f"{row['owner']}_{Path(auth_rel).name}"
            out.write_text(json.dumps(auth, indent=2) + "\n")
            log.append(f"{auth_rel} -> {out.relative_to(repo)}")
            row["asm_authority"] = str(out.relative_to(repo))
            row["asm_authority_sha256"] = _file_sha(out)
        for alias in row.get("aliases", []):
            alias["symbol"] = names.get(alias["symbol"], alias["symbol"])
    return doc, log


# --- historical inputs ----------------------------------------------------------
# Research packets compile sources written before lane w22g's libultra rename
# (pinned cloud/matches/boot_tail submissions, `git show BASE:` baselines)
# against today's symbol tables, where e.g. osJamMesg names another address.
# They pass such text through renamed_source() (and baseline lock pins through
# renamed_locks()) so old and new code resolve to the same functions.

LIBULTRA_RENAMES = REPO / "cloud" / "work" / "frontier" / "w22g" / "renames.json"
_libultra_cache = None


def libultra_names():
    """{old: new} of the w22g libultra rename."""
    global _libultra_cache
    if _libultra_cache is None:
        _libultra_cache = mapping(load_map(LIBULTRA_RENAMES))
    return _libultra_cache


def renamed_source(text):
    """C text written before the rename, with the rename applied (code only)."""
    return permute_c(text, libultra_names())


def renamed_locks(texts, locks):
    """Pre-rename lock entries for the given pre-rename texts ({rel: text}),
    re-keyed and re-pinned for renamed_source(text): an entry moves only when
    its body held the pin; anything else is returned unchanged."""
    names = libultra_names()
    out = {}
    for spec, entry in locks.items():
        rel, _, fn = spec.partition(":")
        body = _body(texts.get(rel), fn)
        if body is not None and _sha(normalize_body(body)) == entry.get("body_sha256"):
            new = dict(entry, body_sha256=_sha(normalize_body(permute_c(body, names))))
            if new.get("target_id") in names:
                new["target_id"] = names[new["target_id"]]
            out[f"{rel}:{names.get(fn, fn)}"] = new
        else:
            out[spec] = entry
    return out


# --- command --------------------------------------------------------------------

def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("command", choices=("plan", "symbols", "source", "lock", "owned"))
    p.add_argument("map")
    p.add_argument("files", nargs="*")
    p.add_argument("--base", default="HEAD", help="revision holding the pre-rename tree")
    p.add_argument("--lockfile", default=str(LOCKFILE))
    p.add_argument("--rebind-dir", default="cloud/work/frontier/w22g/rebind",
                   help="where `owned` writes renamed copies of pinned research files")
    args = p.parse_args(argv)
    try:
        rows = load_map(args.map)
        names = mapping(rows)
        if args.command == "plan":
            for v, o, n in sorted(rows):
                print(f"0x{v:08X}  {o:24} -> {n}")
            print(f"{len(rows)} renames")
        elif args.command == "symbols":
            text, dropped = rewrite_symbols(SYMBOL_ADDRS.read_text(), rows)
            SYMBOL_ADDRS.write_text(text)
            for line in dropped:
                print(f"dropped contradicted dup-addr line: {line}")
            print(f"symbol_addrs: {len(rows)} definitions renamed")
        elif args.command == "source":
            changed = 0
            for f in args.files:
                path = Path(f)
                old = path.read_text()
                new = permute_c(old, names)
                if new != old:
                    path.write_text(new)
                    changed += 1
            print(f"source: {changed} of {len(args.files)} files rewritten")
        elif args.command == "lock":
            entries = json.loads(Path(args.lockfile).read_text())
            out, changed = migrate_lock(entries, names, args.base)
            Path(args.lockfile).write_text(json.dumps(out, indent=2, sort_keys=True) + "\n")
            for old, new in changed:
                print(f"  {old}" + (f" -> {new}" if new != old else " (body rehashed)"))
            print(f"lock: {len(changed)} of {len(entries)} entries migrated")
        elif args.command == "owned":
            doc = json.loads(OWNED.read_text())
            doc, log = rebind_owned(doc, names, args.base, Path(args.rebind_dir).resolve())
            OWNED.write_text(json.dumps(doc, indent=2) + "\n")
            for line in log:
                print(f"  {line}")
            print(f"owned: {len(log)} file pins refreshed")
    except RenameError as exc:
        sys.exit(f"refusing: {exc}")


if __name__ == "__main__":
    main()
