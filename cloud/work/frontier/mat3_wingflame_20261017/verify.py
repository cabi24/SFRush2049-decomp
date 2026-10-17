"""Strict whole-group replay and a wrong-flag negative control; no ROM needed."""
import argparse
import contextlib
import hashlib
import io
import json
import shutil
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

TARGET = "entity_anim_texture"
OLD_MEMBERS = ["matrix_scale_apply", "anim_state_update", "buffer_swap"]
UNCHANGED = ["buffer_swap.c", "anim_tire_donor.c", "rand.c", "visibility_wrappers.c",
             "func_8008B2E4.c", "codex_transform_b109.c"]


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def target_digest(name):
    return hashlib.sha256(b"".join(struct.pack(">I", word) for word in score.targets()[name])).hexdigest()


def inspect(obj, names):
    rows = {}
    for name in names:
        with contextlib.redirect_stdout(io.StringIO()):
            result = score.compare(obj, name)
        rows[name] = json.loads(json.dumps(vars(result)))
    return rows


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def assert_match(row):
    require(row["differing"] == 0, "strict comparison differs")
    require(not any(row[key] for key in ("extra_words", "unverified", "unresolved", "errors")), "strict comparison has unresolved, unverified, error or excess words")


def symbol_extents(obj, names):
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    symbols = [s for i, section in enumerate(sections) if section["type"] == 2
               for s in score._symbol_table(data, sections, i)
               if s["type"] == 2 and s["section"] == text_index]
    result = {}
    for name in names:
        matches = [s for s in symbols if s["name"] == name]
        require(len(matches) == 1, "missing or ambiguous symbol: " + name)
        result[name] = matches[0]["size"]
    return result


def verify_report(receipt):
    members = OLD_MEMBERS + [TARGET]
    require(receipt["status"] == "STRICT_GROUP_MATCH" and receipt["new_function"] == TARGET
            and receipt["new_bytes"] == 636 and not receipt["cartridge_coverage_claimed"], "false credit")
    require(receipt["claims"] == members, "claim inventory changed")
    for name in members:
        row = receipt["positive"][name]
        assert_match(row)
        require(row["total"] * 4 == receipt["symbol_bytes"][name] == len(score.targets()[name]) * 4,
                "complete member extent changed: " + name)
    negative = receipt["negative_control"]
    require(negative[TARGET]["differing"] == 1 and negative[TARGET]["total"] == 159,
            "semantic negative result changed")
    require(not any(negative[TARGET][key] for key in ("extra_words", "unverified", "unresolved", "errors")),
            "semantic negative has diagnostics")
    for name in OLD_MEMBERS:
        assert_match(negative[name])


def verify(out, check_receipt=True):
    out = Path(out)
    out.mkdir(parents=True, exist_ok=True)
    group = HERE / "group"
    spec = json.loads((group / "group.json").read_text())
    require(spec["name"] == "codex_visual_tire_module_20261002", "group identity changed")
    require(spec["members"] == OLD_MEMBERS + [TARGET], "member inventory changed")
    require(spec["claims"] == spec["members"], "claim inventory changed")
    paths = ["verify.py", "trace_fidelity.py", "group/group.json"] + ["group/" + f for f in spec["files"]]
    inputs = {name: digest(HERE / name) for name in paths}
    targets = {name: target_digest(name) for name in spec["members"]}
    if check_receipt:
        frozen = json.loads((HERE / "verification.json").read_text())
        require(inputs == frozen["inputs_sha256"], "packet input changed; reverify and update receipt")
        require(targets == frozen["target_words_sha256"], "native target words changed")
        verify_report(frozen)
        trace = json.loads((HERE / "trace_fidelity.json").read_text())
        require(trace["inputs_sha256"] == inputs, "trace input binding changed")
        require(trace["complete_objects_identical"] and trace["stock_object_sha256"] == trace["trace_object_sha256"], "trace identity receipt changed")
    # Pin copied context, not future canonical tool/lock/source revisions.
    if check_receipt:
        for name in UNCHANGED:
            require(digest(group / name) == frozen["unchanged_context_sha256"][name], "copied context changed")
    obj = out / "positive.o"
    score.compile_group(group, obj)
    positive = inspect(obj, spec["members"] + spec["context"])
    extents = symbol_extents(obj, spec["members"] + spec["context"])
    if check_receipt:
        require(digest(obj) == trace["stock_object_sha256"], "complete stock/trace object changed")
    for name in spec["members"]:
        assert_match(positive[name])
    # Exactly one real semantic constant is wrong; keep all source and context otherwise.
    negative = out / "negative"
    if negative.exists():
        shutil.rmtree(negative)
    shutil.copytree(group, negative)
    source = negative / "entity_anim_texture.c"
    body = source.read_text()
    require(body.count("flags^=0x80000000U;") == 1, "ambiguous semantic mutation")
    source.write_text(body.replace("flags^=0x80000000U;", "flags^=0x40000000U;"))
    nobj = out / "negative.o"
    score.compile_group(negative, nobj)
    negative_rows = inspect(nobj, spec["members"])
    require(negative_rows[TARGET]["differing"] == 1, "wrong-mask negative changed")
    require(not any(negative_rows[TARGET][key] for key in ("extra_words", "unverified", "unresolved", "errors")), "wrong-mask negative has diagnostics")
    for name in OLD_MEMBERS:
        assert_match(negative_rows[name])
    receipt = {"status": "STRICT_GROUP_MATCH", "population": "game", "new_function": TARGET,
            "new_bytes": 636, "cartridge_coverage_claimed": False,
            "base_commit": "58425072b49e95f0fbe5c4c239988fb4885f7c5c",
            "inputs_sha256": inputs, "target_words_sha256": targets,
            "unchanged_context_sha256": {name: digest(group / name) for name in UNCHANGED},
            "positive": positive, "negative_control": negative_rows,
            "claims": spec["claims"], "symbol_bytes": extents, "object_sha256": digest(obj),
            "negative_mutation": "translucency XOR mask 0x80000000U -> 0x40000000U"}
    verify_report(receipt)
    if check_receipt:
        require(receipt == frozen, "fresh sealed receipt changed")
    return receipt


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--record", action="store_true")
    parser.add_argument("--out", type=Path, default=ROOT / "build/mat3_wingflame_20261017")
    args = parser.parse_args()
    receipt = verify(args.out, check_receipt=not args.record)
    if args.record:
        (HERE / "verification.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print("PASS: four strict members; wrong-flag control differs by one word; no extra/unverified words")


if __name__ == "__main__":
    main()
