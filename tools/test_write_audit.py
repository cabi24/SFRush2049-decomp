#!/usr/bin/env python3
"""Find test files that write the same non-unique path (pytest-xdist safety).

Runs one suite serially under strace (Linux), attributes every file write
(open for writing, mkdir, rename, unlink, ...) to the test running at the time,
and reports the paths that more than one test file writes. Such files must not
run at the same time: tests/conftest.py's CONCURRENCY_GROUPS keeps each group on
one xdist worker. The report ends with the groups the trace implies and whether
CONCURRENCY_GROUPS already covers them (exit 1 if not).

    python3 tools/test_write_audit.py tests/conveyor [--trace FILE] [--keep]
    python3 tools/test_write_audit.py tests/cloud --from-trace FILE

Per-test-unique paths are ignored: tmp_path, tempfile.mkdtemp/mkstemp names,
gcc/IDO/binutils temporaries, __pycache__, content-addressed caches written by
rename (build/amatch_cache). Writes relative to a directory fd (shutil.rmtree's
walk) are skipped; the tree's root still shows. Reads are not traced: a test
that only reads another file's output is not found by this audit.

Marker plugin: with `-p tools.test_write_audit` (the script passes it) each test
start/end shows in the trace as a failing readlink of /__TEST_WRITE_AUDIT__/...
"""
import argparse
import collections
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
MARK_ROOT = "/__TEST_WRITE_AUDIT__/"


# --- pytest plugin (loaded in the traced run) ---------------------------------

def _mark(kind, nodeid):
    try:
        os.readlink(MARK_ROOT + kind + "/" + nodeid.replace("/", "|"))
    except OSError:
        pass


def pytest_runtest_logstart(nodeid, location):
    _mark("START", nodeid)


def pytest_runtest_logfinish(nodeid, location):
    _mark("END", nodeid)


# --- trace analysis -------------------------------------------------------------

SYSCALLS = ("openat,open,creat,mkdir,mkdirat,rename,renameat,renameat2,unlink,unlinkat,"
            "rmdir,chdir,symlink,symlinkat,link,linkat,truncate,readlink,readlinkat,"
            "clone,clone3,fork,vfork")
KEEP = re.compile(r"O_WRONLY|O_RDWR|O_CREAT|mkdir|rename|unlink|rmdir|chdir|symlink|link|"
                  r"truncate|clone|fork|" + re.escape(MARK_ROOT))
MARK = re.compile(r'readlink(?:at)?\((?:AT_FDCWD, )?"' + re.escape(MARK_ROOT) +
                  r'(START|END)/([^"]*)"')
CALL = re.compile(r"^(\d+)\s+(\w+)\((.*)")
RESUMED = re.compile(r"^(\d+)\s+<\.\.\. (\w+) resumed>")
STR = re.compile(r'"((?:[^"\\]|\\.)*)"')
WRITE_OPEN = re.compile(r"O_WRONLY|O_RDWR|O_CREAT|O_TRUNC")
CHILD = re.compile(r"= (\d+)\s*$")
UNIQUE = [
    re.compile(r"^/tmp/pytest-of-[^/]+/"),                        # tmp_path
    re.compile(r"^/tmp/[^/]*[_a-z0-9]{8}(\.\w+)*(/|$)"),          # tempfile.mkdtemp/mkstemp
    re.compile(r"^/tmp/(cc|ctm[a-z]{0,4}|file)[A-Za-z0-9]{6}[.\w]*$"),  # gcc/IDO/binutils
    re.compile(r"/__pycache__/"),
    re.compile(r"^/(dev|proc)/"),
    re.compile(r"/\.pytest_cache/"),
    re.compile(r"/build/amatch_cache/"),                          # content-addressed, renamed
]
RANDOM_PART = re.compile(r"(replay|proof|tmp)-?[a-z0-9_]{8}")


def writes_by_test(trace, cwd0):
    """{test nodeid: {absolute path: {syscalls}}} from an `strace -f` log."""
    cwd, pending_chdir = {}, {}
    current = None
    found = collections.defaultdict(lambda: collections.defaultdict(set))

    def resolve(pid, path):
        if path.startswith("/"):
            return os.path.normpath(path)
        return os.path.normpath(os.path.join(cwd.get(pid, cwd0), path))

    def inherit(parent, line):
        child = CHILD.search(line)
        if child and child.group(1) not in cwd:
            cwd[child.group(1)] = cwd.get(parent, cwd0)

    with open(trace, errors="replace") as fh:
        for line in fh:
            m = MARK.search(line)
            if m:
                current = m.group(2).replace("|", "/") if m.group(1) == "START" else None
                continue
            r = RESUMED.match(line)
            if r:
                pid, sc = r.groups()
                if sc == "chdir" and pid in pending_chdir and line.rstrip().endswith("= 0"):
                    cwd[pid] = pending_chdir.pop(pid)
                elif sc in ("clone", "clone3", "fork", "vfork"):
                    inherit(pid, line)
                continue
            m = CALL.match(line)
            if not m:
                continue
            pid, sc, rest = m.groups()
            strs = STR.findall(rest)
            if sc == "chdir":
                if strs and "<unfinished" in line:
                    pending_chdir[pid] = resolve(pid, strs[0])
                elif strs and "= 0" in line:
                    cwd[pid] = resolve(pid, strs[0])
                continue
            if sc in ("clone", "clone3", "fork", "vfork"):
                inherit(pid, line)
                continue
            if not strs or re.match(r'\d+, "', rest):
                continue
            if sc in ("openat", "open", "creat"):
                if sc != "creat" and not WRITE_OPEN.search(rest):
                    continue
                targets = strs[:1]
            elif sc in ("mkdir", "mkdirat", "unlink", "unlinkat", "rmdir", "truncate",
                        "symlink", "symlinkat"):
                targets = strs[-1:]
            elif sc in ("rename", "renameat", "renameat2", "link", "linkat"):
                targets = strs[:2]
            else:
                continue
            if "= -1" in line and sc not in ("unlink", "unlinkat", "rmdir"):
                continue
            for t in targets:
                p = resolve(pid, t)
                if not any(u.search(p) for u in UNIQUE):
                    found[current or "<collection>"][p].add(sc)
    return found


def shared_paths(found):
    """{path with random parts normalised: {test file}} for paths written by >1 file."""
    writers = collections.defaultdict(set)
    for test, paths in found.items():
        if test.startswith("<"):
            continue
        for p in paths:
            writers[RANDOM_PART.sub(r"\1-RAND", p)].add(test.split("::", 1)[0])
    return {p: files for p, files in writers.items() if len(files) > 1}


def groups(shared):
    parent = {}

    def find(x):
        parent.setdefault(x, x)
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x
    for files in shared.values():
        files = sorted(files)
        for f in files[1:]:
            parent[find(f)] = find(files[0])
    out = collections.defaultdict(set)
    for f in list(parent):
        out[find(f)].add(f)
    return sorted((sorted(g) for g in out.values()), key=lambda g: g[0])


def configured_groups():
    sys.path.insert(0, str(REPO / "tests"))
    try:
        import conftest  # tests/conftest.py
    finally:
        sys.path.pop(0)
    return {p: name for name, paths in conftest.CONCURRENCY_GROUPS.items() for p in paths}


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("suite", help="test path, e.g. tests/conveyor")
    ap.add_argument("--from-trace", metavar="FILE", help="analyse an existing trace")
    ap.add_argument("--trace", metavar="FILE", help="where to write the trace")
    ap.add_argument("--marker", default="not node_required", help="pytest -m expression")
    ap.add_argument("--verbose", action="store_true", help="list every shared path")
    args = ap.parse_args()
    trace = args.from_trace
    if not trace:
        trace = args.trace or tempfile.mkstemp(prefix="test-write-audit-", suffix=".trace")[1]
        cmd = ["strace", "-f", "-qq", "-s", "1024", "-e", "trace=" + SYSCALLS,
               "-o", trace + ".raw", sys.executable, "-m", "pytest", args.suite,
               "-m", args.marker, "-p", "no:cacheprovider", "-p", "tools.test_write_audit"]
        print("running:", " ".join(cmd), file=sys.stderr)
        proc = subprocess.run(cmd, cwd=REPO)
        print("pytest exit %d (LeakSanitizer fails under ptrace; such failures are "
              "expected here)" % proc.returncode, file=sys.stderr)
        with open(trace + ".raw", errors="replace") as src, open(trace, "w") as dst:
            dst.writelines(line for line in src if KEEP.search(line))
        os.unlink(trace + ".raw")
        print("trace:", trace, file=sys.stderr)
    shared = shared_paths(writes_by_test(trace, str(REPO)))
    if args.verbose:
        for p in sorted(shared):
            print("%s\n    %s" % (p, "\n    ".join(sorted(shared[p]))))
    have = configured_groups()
    missing = 0
    for g in groups(shared):
        names = {have.get(f) for f in g}
        ok = len(names) == 1 and None not in names
        missing += not ok
        print("%s group: %s" % ("covered" if ok else "MISSING", ", ".join(g)))
    print("%d implied group(s), %d not covered by CONCURRENCY_GROUPS" %
          (len(groups(shared)), missing))
    return 1 if missing else 0


if __name__ == "__main__":
    sys.exit(main())
