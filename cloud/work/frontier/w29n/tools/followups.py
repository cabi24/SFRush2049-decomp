#!/usr/bin/env python3
"""w29n: rename the old game names in files outside lane w29n's remit (dry run by default).

    python3 cloud/work/frontier/w29n/tools/followups.py [--write]

For the integrator, after merging w29n (and w29r, which edits the same recomp files):
  - tools/recomp/ipa_callees.json: entry keys and prototype names (func_80096CA8 ->
    SetupWaveModel), so draft_check finds the register convention of a draft's callee
    by its new name; evidence text that names the old label is left as history;
  - tests/cloud/test_recomp_phase8.py: the committed-table assertion on that key;
  - tools/recomp/census/*.json: function names in the generated census (or regenerate
    the census on rocky instead).
Only whole identifiers that are an accepted old name of renames.json are changed.
"""
import argparse
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
MAP = REPO / "cloud/work/frontier/w29n/renames.json"


def names():
    return {r["old"]: r["new"] for r in json.loads(MAP.read_text())["renames"]
            if r.get("status", "accepted") == "accepted" and r.get("kind") == "function"}


def ipa(text, table):
    doc = json.loads(text)
    callees = doc.get("callees", {})
    out = {}
    for key, entry in callees.items():
        new = table.get(key, key)
        if new != key and "prototype" in entry:
            entry = dict(entry, prototype=re.sub(r"\b%s\b" % re.escape(key), new, entry["prototype"]))
        out[new] = entry
    doc["callees"] = out
    for indent in (1, 2):
        if json.dumps(json.loads(text), indent=indent) + "\n" == text:
            return json.dumps(doc, indent=indent) + "\n"
    raise SystemExit("ipa_callees.json: unknown json format")


def scripts():
    return {p.stem for p in (REPO / "tools/recomp/scripts").glob("*.press")}


def census(path, table):
    """The census file with FUNCTION names renamed structurally (census.json: the
    `functions` keys; <script>.json: the `functions` keys), written in the generator's
    own format (tools/recomp/census.py). Script names are not function names: w29n's
    first version renamed the script `race_finish` along with the function race_finish
    (-> animPlace); a script key that is not a script but whose inverse rename is one
    is restored here."""
    inverse = {}
    for o, n in table.items():
        inverse.setdefault(n, o)
    known = scripts()

    def script(s):
        return inverse[s] if s not in known and inverse.get(s) in known else s
    doc = json.loads(path.read_text())
    funcs = {table.get(k, k): v for k, v in doc["functions"].items()}
    if len(funcs) != len(doc["functions"]):
        raise SystemExit(f"{path.name}: a rename merges two census functions")
    if path.name == "census.json":
        for f in funcs.values():
            f["scripts"] = {script(s): n for s, n in f["scripts"].items()}
        doc = dict(doc, functions=funcs, scripts=sorted(script(s) for s in doc["scripts"]))
        return json.dumps(doc, indent=1, sort_keys=True) + "\n"
    doc = dict(doc, functions=funcs, script=script(doc["script"]))
    return CENSUS.compact_json(doc)


def _census_module():
    import importlib.util
    spec = importlib.util.spec_from_file_location("recomp_census", REPO / "tools/recomp/census.py")
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


CENSUS = _census_module()      # compact_json: the per-script file format


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--write", action="store_true")
    a = p.parse_args()
    table = names()
    edits = []
    path = REPO / "tools/recomp/ipa_callees.json"
    edits.append((path, ipa(path.read_text(), table)))
    path = REPO / "tests/cloud/test_recomp_phase8.py"
    text = path.read_text()
    edits.append((path, text.replace('t["func_80096CA8"]', 't["SetupWaveModel"]')))
    for path in sorted((REPO / "tools/recomp/census").glob("*.json")):
        edits.append((path, census(path, table)))
    for path, new in edits:
        if new != path.read_text():
            print(f"{'rewrite' if a.write else 'would rewrite'} {path.relative_to(REPO)}")
            if a.write:
                path.write_text(new)
    return 0


if __name__ == "__main__":
    sys.exit(main())
