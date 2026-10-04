"""Re-run each gate-refused splice on top of the committed TUs and record the
first compiler error (or link/ROM outcome) as its refusal detail."""
import json, subprocess, sys
from pathlib import Path
sys.path.insert(0, str(Path.cwd()))
import importlib.util
spec = importlib.util.spec_from_file_location("pb", "cloud/work/boot_tail_promotion/promote_batch.py")
pb = importlib.util.module_from_spec(spec); spec.loader.exec_module(pb)
from tools.conveyor.pipeline import layout as L, lock as K

ctx = {json.loads(l)["function"]: json.loads(l) for l in open(pb.HERE / "context.jsonl")}
refused = [json.loads(l) for l in open(pb.REFUSALS)]
mapping = L.derive(); entries = K.load_lock(); out = []
passthrough = {f["name"] for s in mapping["segments"] for f in s["functions"] if f.get("state") == "passthrough"}
gate_refused = sorted({r["function"] for r in refused if "gate failed" in r["reason"]} & passthrough)
for fn in gate_refused:
    seg = next(s for s in mapping["segments"] if any(f["name"] == fn for f in s["functions"]))
    c = dict(fn=fn, seg=seg, src=pb.source_for(fn, entries), row=ctx[fn])
    (tu,) = pb.splice([c])
    rel = str(tu.relative_to(pb.REPO))
    subprocess.run(["rsync", "-a", str(tu), f"{pb.BUILDER}:{pb.BUILDER_REPO}/{rel}"], check=True)
    p = subprocess.run(["ssh", pb.BUILDER, f"cd {pb.BUILDER_REPO} && touch {rel} && make COMPILER=ido build/us/{rel[:-2]}.o 2>&1"],
                       capture_output=True, text=True)
    err = next((l.strip() for l in p.stdout.splitlines() if "Error:" in l), None)
    pb.restore([tu])
    subprocess.run(["rsync", "-a", str(tu), f"{pb.BUILDER}:{pb.BUILDER_REPO}/{rel}"], check=True)
    out.append({"function": fn, "tu": rel, "compile_error": err})
    print(fn, err)
Path(pb.HERE / "gate_refusal_diagnosis.jsonl").write_text("".join(json.dumps(r) + "\n" for r in out))
