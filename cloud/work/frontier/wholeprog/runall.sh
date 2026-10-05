#!/bin/sh
# usage: wp/runall.sh TAG [wp.py build options]   -> one-line summary + failure classes
tag=$1; shift
python3 wp/wp.py build "$tag" "$@" > wp/runs/$tag.log 2>&1
python3 - "$tag" <<'PY'
import json,sys
d=json.load(open(f"wp/runs/{sys.argv[1]}/result.json"))
s=d.get("summary",d)
t=s.get("tally",{})
ok=sum(v for k,v in t.items() if k.startswith("MATCH"))
print(f"{s['tag']}: files {s['files']} members {s['members']} defined {s['defined']} keep {s['keep']} standins {s['standins']} -> OK {ok} {t} {s.get('error','')[:600]}")
PY
