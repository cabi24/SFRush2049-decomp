#!/usr/bin/env python3
"""Keep local reference copies of third-party documents.

    python3 tools/external_docs.py fetch [--force] [NAME ...]
    python3 tools/external_docs.py status

`docs/external/SOURCES.json` lists the documents (name, raw URL, upstream repo
and path, licence note). Copies are written to `docs/external/files/`, which is
git-ignored: some sources declare no licence, so we read them locally and link
to them instead of republishing them. Each copy has a `<name>.meta.json` with
its URL, SHA-256, fetch time and, when GitHub answers, the upstream commit that
last touched it. When a document changes the previous copy is kept as
`<name>.prev` and a one-line summary of the change is printed. A failed
download never replaces a good copy. Stdlib only (Python 3.9+).
"""
import argparse
import datetime
import hashlib
import json
import sys
import urllib.error
import urllib.request
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
EXTERNAL = REPO / "docs" / "external"
SOURCES = EXTERNAL / "SOURCES.json"
FILES = EXTERNAL / "files"
TIMEOUT = 30


def load_sources(path=SOURCES):
    return json.loads(Path(path).read_text())["documents"]


def _get(url):
    request = urllib.request.Request(url, headers={"User-Agent": "rush2049-external-docs"})
    with urllib.request.urlopen(request, timeout=TIMEOUT) as response:
        return response.read()


def upstream_commit(doc):
    """(sha, date) of the last upstream commit touching the file, or (None, None).
    Best effort: unauthenticated GitHub API, no error if it is unavailable."""
    repo, path = doc.get("repo"), doc.get("path")
    if not repo or not path:
        return None, None
    url = f"https://api.github.com/repos/{repo}/commits?path={path}&per_page=1"
    try:
        commit = json.loads(_get(url))[0]
        return commit["sha"][:10], commit["commit"]["committer"]["date"]
    except (urllib.error.URLError, OSError, ValueError, KeyError, IndexError):
        return None, None


def _line_delta(old, new):
    old_lines, new_lines = set(old.splitlines()), set(new.splitlines())
    return len(new_lines - old_lines), len(old_lines - new_lines)


def fetch_one(doc, force=False, files_dir=FILES, now=None):
    """Fetch one document. Returns (status, message); status is one of
    'new', 'updated', 'unchanged', 'failed'."""
    files_dir = Path(files_dir)
    target = files_dir / doc["file"]
    meta_path = files_dir / (doc["file"] + ".meta.json")
    try:
        data = _get(doc["url"])
    except (urllib.error.URLError, OSError, ValueError) as exc:
        return "failed", f"{doc['name']}: download failed ({exc}); keeping the existing copy"
    if not data.strip():
        return "failed", f"{doc['name']}: empty response; keeping the existing copy"
    digest = hashlib.sha256(data).hexdigest()
    stamp = (now or datetime.datetime.now(datetime.timezone.utc)).strftime("%Y-%m-%dT%H:%M:%SZ")
    files_dir.mkdir(parents=True, exist_ok=True)

    old = target.read_bytes() if target.exists() else None
    if old is not None and hashlib.sha256(old).hexdigest() == digest and not force:
        meta = json.loads(meta_path.read_text()) if meta_path.exists() else {}
        meta["checked_at"] = stamp
        meta_path.write_text(json.dumps(meta, indent=2) + "\n")
        return "unchanged", f"{doc['name']}: unchanged ({digest[:10]})"

    if old is not None:
        target.with_name(target.name + ".prev").write_bytes(old)
    target.write_bytes(data)
    sha, date = upstream_commit(doc)
    meta_path.write_text(json.dumps({
        "name": doc["name"], "url": doc["url"], "sha256": digest,
        "fetched_at": stamp, "checked_at": stamp,
        "upstream_commit": sha, "upstream_commit_date": date,
        "licence": doc.get("licence", "unknown"),
    }, indent=2) + "\n")
    if old is None:
        return "new", f"{doc['name']}: fetched {len(data)} bytes ({digest[:10]})"
    added, removed = _line_delta(old.decode(errors="replace"), data.decode(errors="replace"))
    return "updated", (f"{doc['name']}: UPDATED +{added}/-{removed} lines "
                       f"({digest[:10]}); previous copy kept as {target.name}.prev")


def status_lines(docs, files_dir=FILES):
    out = []
    for doc in docs:
        meta_path = Path(files_dir) / (doc["file"] + ".meta.json")
        if not (Path(files_dir) / doc["file"]).exists():
            out.append(f"{doc['name']}: not fetched yet")
            continue
        meta = json.loads(meta_path.read_text()) if meta_path.exists() else {}
        out.append(f"{doc['name']}: fetched {meta.get('fetched_at', '?')}, last checked "
                   f"{meta.get('checked_at', '?')}, upstream {meta.get('upstream_commit') or '?'} "
                   f"({meta.get('upstream_commit_date') or '?'}), licence: {meta.get('licence', '?')}")
    return out


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(dest="cmd", required=True)
    f = sub.add_parser("fetch")
    f.add_argument("--force", action="store_true")
    f.add_argument("names", nargs="*")
    sub.add_parser("status")
    args = parser.parse_args(argv)
    docs = load_sources()
    if args.cmd == "status":
        print("\n".join(status_lines(docs)))
        return 0
    if args.names:
        docs = [d for d in docs if d["name"] in args.names]
    failed = False
    for doc in docs:
        status, message = fetch_one(doc, force=args.force)
        print(message)
        failed |= status == "failed"
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
