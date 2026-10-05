#!/usr/bin/env python3
"""Read-only strict replay of the bounded steering reconstruction and controls.

Run from the repository root on an IDO-capable host. This does not splice,
change acceptance state, or contact a remote builder. Raw objects stay ignored.
"""
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

HERE = Path(__file__).resolve().parent
NAME = "steering_sensitivity"
CONTEXT = ["vector_diff_process", "traction_control", "func_800A61B0", "math_utility"]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def body(text, name):
    start = text.index("void " + name + "(")
    while ";" in text[start:text.index("{", start)]:
        start = text.index("void " + name + "(", start + 1)
    end = text.index("{", start) + 1
    depth = 1
    while depth:
        depth += (text[end] == "{") - (text[end] == "}")
        end += 1
    return text[start:end]


def run(directory, destination):
    score.compile_group(directory, destination)
    data, sections = score._elf(destination)
    symbols = [symbol for i, section in enumerate(sections) if section["type"] == 2
               for symbol in score._symbol_table(data, sections, i)
               if symbol["type"] == 2 and symbol["name"] in [NAME] + CONTEXT]
    extent = {symbol["name"]: symbol["size"] for symbol in symbols}
    words = score.text_words(destination)
    start = score.symbols(destination)[NAME] // 4
    frame = words[start] & 0xffff
    if frame & 0x8000:
        frame -= 0x10000
    return {
        "comparison": {name: asdict(score.compare(destination, name, show=0))
                       for name in [NAME] + CONTEXT},
        "function_bytes": extent,
        "steering_frame_bytes": -frame,
        "object_sha256": sha(destination.read_bytes()),
    }


def main():
    source = (HERE / "group.c").read_text()
    specification = json.loads((HERE / "group.json").read_text())
    provenance = json.loads((HERE / "context_provenance.json").read_text())
    assert specification["claims"] == []
    for name in CONTEXT:
        assert sha(body(source, name).encode()) == provenance["body_sha256"][name]
    expected = score.targets()[NAME]
    assert len(expected) * 4 == 928
    build = ROOT / "build" / "dot_steering_medium"
    build.mkdir(parents=True, exist_ok=True)
    proof = run(HERE, build / "final.o")
    proof.update({
        "status": "NONMATCH",
        "claims": [],
        "source_sha256": sha(source.encode()),
        "flags": specification["flags"],
        "target_manifest_sha256": sha((score.ASM_DIR / "SHA256SUMS").read_bytes()),
        "compiler_sha256": {name: sha((score.IDO / name).read_bytes())
                            for name in ["cc", "cfe", "uopt", "ugen", "as1"]},
        "expected_bytes": 928,
        "context_bodies_unchanged": CONTEXT,
        "limitations": ["No match claim", "No source-image or full-ROM build", "No coverage change"],
    })
    assert proof["function_bytes"][NAME] == 928
    assert proof["steering_frame_bytes"] == 104
    assert proof["comparison"][NAME] == {
        "differing": 106, "total": 232, "unresolved": [], "unverified": [], "errors": [], "extra_words": 0}
    for name in CONTEXT:
        result = proof["comparison"][name]
        assert result["differing"] == result["extra_words"] == 0
        assert not (result["unresolved"] or result["unverified"] or result["errors"])
    if "--all" in sys.argv:
        controls = json.loads((HERE / "controls.json").read_text())
        for row in controls:
            replacement = (HERE / "variants" / (row["control"] + ".inc")).read_text().rstrip("\n")
            assert sha(replacement.encode()) == row["body_sha256"]
            with tempfile.TemporaryDirectory(prefix="steering-control-") as temporary:
                directory = Path(temporary)
                (directory / "group.c").write_text(source.replace(body(source, NAME), replacement))
                (directory / "group.json").write_text(json.dumps(specification))
                result = run(directory, build / (row["control"] + ".o"))
                assert result["comparison"][NAME] == row["canonical"], row["control"]
                print(row["control"], result["comparison"][NAME]["differing"],
                      result["function_bytes"][NAME], result["steering_frame_bytes"])
        proof["controls_reproduced"] = len(controls)
    destination = build / "verification.json"
    destination.write_text(json.dumps(proof, indent=2) + "\n")
    print("Steering: NONMATCH, 106/232; exact 928-byte ELF extent and 104-byte frame.")
    print("Four accepted context bodies remain strict MATCH.")
    print(destination.relative_to(ROOT))
    return 0


if __name__ == "__main__":
    sys.exit(main())
