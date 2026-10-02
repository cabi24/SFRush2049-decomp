"""Publish a reviewed existing-C-TU storage transaction through standard gates."""
import argparse
import json
from pathlib import Path

from ..client import DEFAULT_DATA
from ..coordinator import db
from . import lock, owned_storage, promote, targets


def trusted_targets(conn, members):
    regions = targets.index_asm_regions()
    result = {}
    for fn in members:
        row = conn.execute(
            "SELECT target_o_sha,insn_count,population,address FROM n64_target WHERE target_id=?",
            (fn,),
        ).fetchone()
        original = [r for r in regions.values() if r.name == fn]
        if (row is None or row["population"] != "static" or len(original) != 1
                or not original[0].words or len(original[0].words) > row["insn_count"]
                or original[0].vaddr != row["address"]):
            raise promote.Refusal("missing authoritative static function extent: " + fn)
        # The assembler may align a target's section beyond its endlabel.
        # The original named region supplies the actual instruction extent.
        result[fn] = (row["target_o_sha"], len(original[0].words))
    return result


def publish(conn, repo, row, activate, paths):
    # The original-ROM transaction gate has run before this callback. Repeat
    # the required source-built pipeline and all repository/lock checks before
    # making either the database or Git publication visible.
    for command in (["python3", "-m", "tools.conveyor.pipeline.blob_rom", "rom"],
                    ["python3", "-m", "pytest"],
                    ["python3", "-m", "tools.conveyor.pipeline.blob_splice", "check"],
                    ["python3", "-m", "tools.conveyor.pipeline.blob_group", "check"],
                    ["python3", "-m", "tools.conveyor.pipeline.lock", "check"]):
        run = promote._run(command, timeout=1800)
        if run.returncode:
            raise promote.Refusal("publication gate failed: " + " ".join(command) +
                                  "\n" + (run.stdout + run.stderr)[-2000:])
        if command[-1] == "rom" and "SHA-1 EXACT" not in run.stdout:
            raise promote.Refusal("source-built ROM gate did not report SHA-1 EXACT")
        print(run.stdout[-1200:])
    rels = [str(p.relative_to(repo)) for p in paths]
    tracked = promote._run(["git", "ls-files", "-z", "--", *rels])
    if tracked.returncode:
        raise promote.Refusal("cannot enumerate tracked publication package")
    commit_paths = [p for p in tracked.stdout.split("\0") if p]
    if not commit_paths:
        raise promote.Refusal("empty tracked publication package")
    with db.tx(conn):
        for fn in row["new_members"]:
            if activate:
                conn.execute(
                    "INSERT INTO promotion_record "
                    "(target_id,source_sha,build_ok,sha1_ok,outcome,created_at,source,flags,evidence,rom_tu) "
                    "VALUES (?,?,1,1,'promoted',strftime('%Y-%m-%dT%H:%M:%fZ','now'),?,?,?,?) "
                    "ON CONFLICT(target_id) WHERE outcome='promoted' DO UPDATE SET "
                    "source_sha=excluded.source_sha,build_ok=1,sha1_ok=1,"
                    "created_at=excluded.created_at,source=excluded.source,flags=excluded.flags,"
                    "evidence=excluded.evidence,rom_tu=excluded.rom_tu",
                    (fn, lock.body_sha(repo / row["tu"], fn), row["source"], row["flags"],
                     json.dumps({"storage_owner": row["owner"],
                                 "complete_source_sha256": row["source_sha256"]}),
                     str(Path(row["tu"]).with_suffix(""))[4:]),
                )
            else:
                conn.execute("UPDATE promotion_record SET outcome='reverted' "
                             "WHERE target_id=? AND outcome='promoted'", (fn,))
        action = "Promote" if activate else "Revert"
        run = promote._run(["git", "commit", "-q", "-m",
                            action + " complete existing " + row["owner"] +
                            " storage (ROM SHA-1 exact)", "--", *commit_paths])
        if run.returncode:
            raise promote.Refusal("storage publication failed: " + run.stderr)


def run(owner, activate, reviewed_digest, via_builder=False, data=DEFAULT_DATA):
    from . import owned_existing_storage as transaction
    repo = Path(promote.REPO)
    row = transaction._row(repo, owner)
    paths = transaction.package_paths(repo, row)
    if not promote._git_clean(paths + [Path(__file__)]):
        raise promote.Refusal("existing storage requires a clean complete tracked package")
    conn = db.connect(Path(data) / "conveyor.db")
    try:
        trusted = trusted_targets(conn, row["members"])
        def gate(current_repo, current_paths):
            ok, detail = owned_storage._gate(current_repo, current_paths, via_builder)
            if not ok:
                print(detail)
            return ok
        return transaction.transition(
            repo, owner, activate, reviewed_digest=reviewed_digest,
            trusted_targets=trusted, gate=gate,
            publish=lambda current_repo, current_row, active, current_paths:
                publish(conn, current_repo, current_row, active, current_paths),
        )
    finally:
        conn.close()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("promote", "revert"))
    parser.add_argument("owner")
    parser.add_argument("--proof-sha256", required=True,
                        help="digest of the independently executed and reviewed strict proof")
    parser.add_argument("--via-builder", action="store_true")
    parser.add_argument("--data", type=Path, default=DEFAULT_DATA)
    args = parser.parse_args()
    print(run(args.owner, args.action == "promote", args.proof_sha256,
              args.via_builder, args.data))


if __name__ == "__main__":
    main()
