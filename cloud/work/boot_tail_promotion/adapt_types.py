"""Give a body's file-local types unique names so several bodies fit one TU.

Bodies in one ROM TU may each define a file-local struct/typedef under the
same name with a different (partial) layout (e.g. MacroState). Type names do
not reach the object code, so each listed body's preamble-defined typedef
names and struct/union/enum tags are suffixed with the function's address.
The adapted source is written to sources/<fn>.c with a provenance note, and
must be re-proved (lock add) and context-checked like any other body.
Usage: python3 cloud/work/boot_tail_promotion/adapt_types.py fn...
"""
import json, re, sys
from pathlib import Path
sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.seeds.extract_candidates import extract_functions
from tools.conveyor.pipeline import lock as K

HERE = Path("cloud/work/boot_tail_promotion")
entries = K.load_lock()
context = {json.loads(l)["function"]: json.loads(l) for l in open(HERE / "context.jsonl")}
for fn in sys.argv[1:]:
    specs = [s for s in entries if s.endswith(":" + fn) and not s.startswith("src/rom/")]
    src = Path(specs[0].rpartition(":")[0]) if specs else Path(f"cloud/matches/boot_tail/{fn}.c")
    text = src.read_text()
    # Only types the body still defines inside a ROM TU: the kept preamble
    # statements of its context_check row (rom_tu.h duplicates were dropped).
    kept = context[fn]["preamble"]
    names = set()
    for stmt in kept:
        names.update(re.findall(r"\b(?:struct|union|enum)\s+([A-Za-z_]\w*)\s*\{", stmt))
        if stmt.startswith("typedef"):
            flat = stmt
            while re.search(r"\{[^{}]*\}", flat):
                flat = re.sub(r"\{[^{}]*\}", "", flat)
            m = (re.search(r"\(\s*\*\s*([A-Za-z_]\w*)\s*\)\s*\(", flat)
                 or re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*;$", flat.strip()))
            if m:
                names.add(m.group(1))
    suffix = "_" + fn[5:]
    first, rest = text.split("\n", 1)
    for n in sorted(names, key=len, reverse=True):
        rest = re.sub(r"\b" + re.escape(n) + r"\b", n + suffix, rest)
    origin = f"cloud/matches/boot_tail/{fn}.c"
    note = (f"/* Adapted from {origin}: file-local type names "
            f"{', '.join(sorted(names))} suffixed {suffix} so several bodies share one ROM TU;"
            " no other change. */\n")
    if src != Path(origin):
        note = note.replace(origin, f"{origin} via {src}")
    (HERE / "sources" / f"{fn}.c").write_text(first + "\n" + note + rest)
    print(fn, sorted(names))
