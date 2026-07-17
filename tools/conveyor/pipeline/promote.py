"""The promotion transaction (004): verified C replaces a passthrough slot,
and the full-ROM SHA-1 decides.

    python3 -m tools.conveyor.pipeline.promote run <segment>:<function> \
        --from <repo-path> [--via-builder] [--override --reason "..."]
    python3 -m tools.conveyor.pipeline.promote batch --locked [--via-builder]

Per contracts/promotion-transaction.md: preconditions refuse with the remedy
named; the splice touches exactly one TU + the lockfile; the gate is the full
matching build + SHA-1 (never weakened); pass commits, any failure restores
the pre-promotion state byte-for-byte. The conveyor's verify_promote job calls
run_promotion() as a library — one implementation of the transaction.
"""
import argparse
import hashlib
import json
import subprocess
import sys
import time
from pathlib import Path

from ..coordinator import db as dbmod
from ..client import DEFAULT_DATA
from ..seeds.extract_candidates import REPO, extract_named_function
from . import layout as layoutmod
from . import lock as lockmod

BUILDER = "watchman"
BUILDER_REPO = "~/projects/rush2049-decomp"


class Refusal(SystemExit):
    pass


def _run(cmd, cwd=REPO, timeout=1200):
    return subprocess.run(cmd, cwd=cwd, capture_output=True, text=True,
                          timeout=timeout)


def _git_clean(paths):
    out = _run(["git", "status", "--porcelain", "--"] + [str(p) for p in paths])
    return not out.stdout.strip()


def _have_local_ido():
    cc = REPO / "tools/ido-static-recomp/build/out/cc"
    if not cc.is_file():
        return False
    probe = _run([str(cc), "-version"], timeout=30)
    return probe.returncode == 0 or "cc" in (probe.stderr + probe.stdout).lower()


def _build_and_gate(via_builder, tu_rel):
    """Full matching build + SHA-1 verify. Returns (ok, detail)."""
    if via_builder:
        sync = _run(["rsync", "-az", str(REPO / tu_rel),
                     f"{BUILDER}:{BUILDER_REPO}/{tu_rel}"])
        if sync.returncode != 0:
            return False, "rsync to builder failed: " + sync.stderr.strip()[:300]
        # rsync -a preserves the Pi's mtime, which can predate the builder's
        # last-built object — make would then skip the rebuild and the gate
        # would verify a STALE ROM (caught live by the SC-003 drill). Touch
        # the TU so the rebuild is unconditional, and demand the object is
        # actually newer afterwards.
        obj_rel = "build/us/" + str(tu_rel)[:-2] + ".o"
        build = _run(["ssh", BUILDER,
                      f"cd {BUILDER_REPO} && touch {tu_rel} && "
                      f"make COMPILER=ido -j16 && make test && "
                      f"[ {obj_rel} -nt {tu_rel} ]"])
    else:
        if not _have_local_ido():
            raise Refusal(
                "refusing: matching build requires IDO, which is not runnable "
                "here — pass --via-builder (watchman) [FR-008]")
        build = _run(["make", "COMPILER=ido", "-j4"], timeout=1800)
        if build.returncode == 0:
            build = _run(["make", "test"], timeout=600)
    ok = build.returncode == 0 and "ROM matches!" in build.stdout
    tail = (build.stdout + build.stderr).strip().splitlines()[-3:]
    return ok, " / ".join(tail)


def _provenance_header(func, source_desc, flagset, evidence):
    today = time.strftime("%Y-%m-%d")
    return (
        f"/* PROMOTED {today} — {func}\n"
        f" * Source:   {source_desc}\n"
        f" * Flags:    {flagset}\n"
        f" * Evidence: {evidence}\n"
        " * Gate:     full-ROM SHA-1 (promotion transaction)\n"
        " */"
    )


def _splice(tu_path, func, seg, body, header):
    text = tu_path.read_text()
    pragma = (f'#pragma GLOBAL_ASM('
              f'"asm/us/nonmatchings/{seg["rom_tu"]}/{func}.s")')
    if pragma not in text:
        raise Refusal(f"refusing: {func} has no passthrough slot in "
                      f"{tu_path.name} (already promoted?)")
    tu_path.write_text(text.replace(pragma, header + "\n" + body.rstrip() + "\n", 1))


def run_promotion(spec, source, via_builder=False, override_reason=None,
                  data=None):
    """The transaction. spec='<segment>:<function>', source=repo path whose
    file defines the function (e.g. src/libc/string.c). Returns the commit
    hash on success; raises Refusal otherwise."""
    seg_name, _, func = spec.partition(":")
    entries = lockmod.load_lock()
    lock_spec = f"{source}:{func}"
    target_id = entries.get(lock_spec, {}).get("target_id", func)
    lockmod.require_promotable_population(target_id, data or DEFAULT_DATA)
    mapping = layoutmod.derive()
    seg = layoutmod._segment_by_name(mapping, seg_name)
    if seg is None:
        raise Refusal(f"refusing: no segment {seg_name!r}")
    if not seg["converted"]:
        raise Refusal(f"refusing: {seg_name} is not converted — run "
                      f"`pipeline.layout convert {seg_name}` first")
    slot = next((f for f in seg["functions"] if f["name"] == func), None)
    if slot is None:
        raise Refusal(f"refusing: {func} is not in segment {seg_name}")

    # Evidence (FR-005): a lock entry for this body, or an explicit override.
    evidence = None
    if lock_spec in entries:
        evidence = f"lock:{lock_spec} ({entries[lock_spec]['verified']})"
    elif override_reason:
        evidence = f"override: {override_reason}"
    else:
        raise Refusal(
            f"refusing: no verification evidence — {lock_spec} is not in "
            "matched.lock.json; verify it (`pipeline.lock add`) or pass "
            "--override --reason [FR-005]")

    flagset = seg["flagset"]
    if not flagset:
        raise Refusal(f"refusing: segment {seg_name} has no pinned flagset — "
                      "run the per-TU flag sweep first [FR-005]")

    tu_rel = Path("src") / f"{seg['rom_tu']}.c"
    tu_path = REPO / tu_rel
    lockfile = REPO / "matched.lock.json"
    if not _git_clean([tu_path, lockfile]):
        raise Refusal("refusing: working tree dirty under the TU or lockfile")

    body = extract_named_function(REPO / source, func)
    header = _provenance_header(func, f"{source} (in-repo, locked)",
                                flagset, evidence)
    _splice(tu_path, func, seg, body, header)

    ok, detail = _build_and_gate(via_builder, tu_rel)
    conn = dbmod.connect(Path(data or DEFAULT_DATA) / "conveyor.db")
    if not ok:
        _run(["git", "checkout", "--", str(tu_rel)])
        if via_builder:
            # Leave the builder green too: restore its TU and rebuild so the
            # next transaction starts from a verified state (drill finding).
            _run(["rsync", "-az", str(REPO / tu_rel),
                  f"{BUILDER}:{BUILDER_REPO}/{tu_rel}"])
            _run(["ssh", BUILDER,
                  f"cd {BUILDER_REPO} && touch {tu_rel} && "
                  f"make COMPILER=ido -j16 >/dev/null 2>&1 && make test"],
                 timeout=1800)
        with dbmod.tx(conn):
            conn.execute(
                "INSERT INTO promotion_record (target_id, source_sha, build_ok,"
                " sha1_ok, outcome, created_at, source, flags, evidence, rom_tu)"
                " VALUES (?, ?, 0, 0, 'failed',"
                " strftime('%Y-%m-%dT%H:%M:%fZ','now'), ?, ?, ?, ?)",
                (func, hashlib.sha256(body.encode()).hexdigest(), source,
                 flagset, json.dumps({"refusal": detail}), seg["rom_tu"]))
        raise Refusal(f"GATE FAILED for {spec}; splice rolled back. {detail}")

    # Lock migration: the promoted body in the ROM TU is now the single
    # source of truth; the reference-path entry is superseded.
    new_spec = f"{tu_rel}:{func}"
    entries[new_spec] = {
        "body_sha256": lockmod.body_sha(tu_path, func),
        "target_id": func,
        "flagset": flagset,
        "verified": "rom-sha1",
        "toolkit_sha": None,
        "verified_at": time.strftime("%Y-%m-%d"),
    }
    entries.pop(lock_spec, None)
    lockmod.save_lock(entries)

    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO promotion_record (target_id, source_sha, build_ok,"
            " sha1_ok, outcome, created_at, source, flags, evidence, rom_tu)"
            " VALUES (?, ?, 1, 1, 'promoted',"
            " strftime('%Y-%m-%dT%H:%M:%fZ','now'), ?, ?, ?, ?)",
            (func, hashlib.sha256(body.encode()).hexdigest(), source, flagset,
             json.dumps({"evidence": evidence}), seg["rom_tu"]))
    commit = _run(["git", "commit", "-q", "-m",
                   f"Promote {func} into {seg['rom_tu']} (ROM SHA-1 exact)\n\n"
                   f"Source: {source} | Flags: {flagset} | {evidence}\n\n"
                   "Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>",
                   "--", str(tu_rel), "matched.lock.json"])
    if commit.returncode != 0:
        raise Refusal("commit failed after a PASSED gate — resolve manually:\n"
                      + commit.stderr[-500:])
    sha = _run(["git", "rev-parse", "--short", "HEAD"]).stdout.strip()
    print(f"PROMOTED {spec} @ {sha} (ROM SHA-1 exact)")
    return sha


def cmd_run(args):
    run_promotion(args.spec, args.source, via_builder=args.via_builder,
                  override_reason=args.reason if args.override else None,
                  data=args.data)


def cmd_batch(args):
    if not args.locked:
        sys.exit("only --locked batch mode exists (V1)")
    mapping = layoutmod.derive()
    entries = lockmod.load_lock()
    plan = []
    for spec, e in sorted(entries.items()):
        src, _, func = spec.rpartition(":")
        if src.startswith("src/rom/"):
            continue  # already promoted
        for seg in mapping["segments"]:
            if seg["converted"] and any(
                    f["name"] == func and f["state"] == "passthrough"
                    for f in seg["functions"]):
                plan.append((f"{seg['yaml_name']}:{func}", src,
                             e.get("target_id", func)))
    if not plan:
        print("nothing to promote (no locked functions with passthrough "
              "slots in converted segments)")
        return
    print(f"batch: {len(plan)} promotions, one full gate each")
    # Preflight the complete plan before the first transaction mutates a TU.
    for _spec, _src, target_id in plan:
        lockmod.require_promotable_population(target_id, args.data)
    for spec, src, _target_id in plan:
        run_promotion(spec, src, via_builder=args.via_builder, data=args.data)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("run")
    p.add_argument("spec", help="<segment>:<function>")
    p.add_argument("--from", dest="source", required=True,
                   help="repo path whose file defines the verified body")
    p.add_argument("--via-builder", action="store_true")
    p.add_argument("--override", action="store_true")
    p.add_argument("--reason", default=None)
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.set_defaults(func=cmd_run)
    p = sub.add_parser("batch")
    p.add_argument("--locked", action="store_true")
    p.add_argument("--via-builder", action="store_true")
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.set_defaults(func=cmd_batch)
    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
