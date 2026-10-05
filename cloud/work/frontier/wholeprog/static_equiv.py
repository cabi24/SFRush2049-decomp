#!/usr/bin/env python3
"""Is "internal under uld -kp" the same thing as C `static` under the stock
`cc -O3` link (uld -preserve_dead_code, no keep list)?

    python3 wp/static_equiv.py

For every single-file locked group: mark each function the group does NOT keep
`static` (definition and earlier prototypes), build with -preserve_dead_code
and no -kp, and compare the whole .text (relocated fields masked) with the
group's own -kp build.  kp_match counts identical objects.
"""
import json
import re
import shutil
import sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import wp  # noqa: E402

score = wp.score


def make_static(text, names):
    defs = wp.scan_defs(text)
    out, pos = [], 0
    for d in defs:
        if d["name"] in names and not d["static"]:
            head = text[d["head"]:d["open"]]
            lead = len(head) - len(head.lstrip())
            out.append(text[pos:d["head"] + lead])
            out.append("static ")
            pos = d["head"] + lead
    out.append(text[pos:])
    text = "".join(out)
    # earlier prototypes of those names: `extern T f(...)` / `T f(...);` at line start
    for n in names:
        def fix(m):
            line = m.group(0)
            if re.match(r"\s*static\b", line):
                return line
            return "static " + re.sub(r"^\s*extern\s+", "", line)
        text = re.sub(r"^[A-Za-z_][^;{}()=\n]*?[\s\*]%s\s*\([^;{}]*\)\s*;" % re.escape(n), fix, text, flags=re.M)
    return text


def build(work, fname, keep_mode):
    ido = score.ido
    common = ["-mips2", "-EB", "-g0", "-O3"]
    link = (["-kp", "keep.txt"] if keep_mode == "kp" else ["-preserve_dead_code"])
    steps = [
        [ido("cc"), "-j", *wp.O3.split(), fname],
        [ido("uld"), "-L/usr/lib/mips2/nonshared", "-_SYSTYPE_SVR4", "-mips2", "-non_shared", "-g0",
         "-no_AutoGnum", *link, fname[:-2] + ".u", "-ko", "linked"],
        [ido("usplit"), "-mips2", "-o", "split", "-t", "st", "linked"],
        [ido("umerge"), "-Olimit", "5000", *common, "split", "-o", "merged", "-t", "st"],
        [ido("uopt"), "-G", "0", "-Olimit", "5000", *common, "merged", "opt", "-t", "st", "optlog"],
        [ido("ugen"), "-G", "0", *common, "opt", "-o", "gen", "-t", "st", "-temp", "ugtmp"],
        [ido("as1"), "-elf", "-G", "0", "-p0", *common, score.R4300_AS1, "-Olimit", "5000", "gen",
         "-o", "o.o", "-t", "st"],
    ]
    for st in steps:
        proc = score._run(st, cwd=work)
        if proc.returncode != 0:
            return Path(st[0]).name + ": " + (proc.stderr or proc.stdout).strip()[-300:]
    return None


def masked_text(obj):
    """.text words with every relocated field masked out."""
    import struct
    data, secs = score._elf(obj)
    text = score._text_index(secs)
    words = score.text_words(obj)
    for sec in secs:
        if sec["type"] != 9 or sec["info"] != text:
            continue
        for k in range(sec["size"] // 8):
            off, info = struct.unpack_from(">II", data, sec["off"] + 8 * k)
            mask = {4: 0xFC000000, 5: 0xFFFF0000, 6: 0xFFFF0000}.get(info & 0xFF, 0)
            words[off // 4] &= mask
    return words


def main():
    lock = wp.load_lock()
    _, groups = wp.source_files()
    tot = dict(groups=0, built=0, members=0, match=0, kp_match=0, with_internal=0)
    fails = []
    for group, members in sorted(groups.items()):
        gdir = wp.ROOT / "src" / "blob" / "groups" / group
        spec = json.loads((gdir / "group.json").read_text())
        if len(spec["files"]) != 1:
            continue
        text = wp.preprocessed(gdir / spec["files"][0], spec["flags"])
        defs = [d["name"] for d in wp.scan_defs(text) if not d["static"]]
        internal = [n for n in defs if n not in spec["keep"]]
        if not internal:
            continue
        tot["groups"] += 1
        tot["with_internal"] += len(internal)
        work = wp.RUNS / "static" / group
        if work.exists():
            shutil.rmtree(work)
        work.mkdir(parents=True)
        (work / "g.c").write_text(make_static(text, set(internal)))
        err = build(work, "g.c", "static")
        if err:
            fails.append((group, "BUILD " + err.replace("\n", " ")[:200]))
            continue
        tot["built"] += 1
        # the same file through -kp with the group's keep list, for a text comparison
        work2 = wp.RUNS / "static" / (group + "__kp")
        if work2.exists():
            shutil.rmtree(work2)
        work2.mkdir(parents=True)
        (work2 / "g.c").write_text(text)
        (work2 / "keep.txt").write_text("".join(k + "\n" for k in spec["keep"]))
        err = build(work2, "g.c", "kp")
        same = (not err) and masked_text(work / "o.o") == masked_text(work2 / "o.o")
        tot["kp_match"] += bool(same)
        if not same:
            a, b = masked_text(work / "o.o"), ([] if err else masked_text(work2 / "o.o"))
            nd = sum(x != y for x, y in zip(a, b))
            fails.append((group, f"TEXT differs from -kp build: {len(a)}w vs {len(b)}w, {nd} words differ {err or ''}"))
        continue
        res = wp.score_members(work / "o.o", members)
        tot["members"] += len(members)
        ok = [m for m in members if res[m]["status"].startswith("MATCH")]
        tot["match"] += len(ok)
        for m in members:
            if m not in ok:
                fails.append((group, f"{m} {res[m]['status']} differ {res[m].get('differing')}"))
    print(tot)
    for f in fails:
        print("  ", *f)


if __name__ == "__main__":
    main()
