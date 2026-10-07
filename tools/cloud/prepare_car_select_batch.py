#!/usr/bin/env python3
"""Materialize the finite, source-grounded car-select calibration plan.

This generator performs no compilation, scoring, source adoption, or publication.
It is intentionally pinned to the reviewed translation unit: an upstream change
requires a new review rather than silently applying old transformations.
"""

from __future__ import annotations

import argparse
import difflib
import hashlib
import itertools
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
BASE_COMMIT = "c35728addbae1626aabc89b7d837e28774f333a0"
SOURCE_PATH = "src/blob/car_select_handler.c"
SOURCE_SHA256 = "d124cb7690ed1182722d3ee7e026a7c884c350a0f1957fd736864623b4847190"
SIGNATURE = "void car_select_handler(s16 arg0)\n"
DECLARATIONS = "  GameCar *temp_v0;\n  s32 *temp_a1;\n"
REVERSED_DECLARATIONS = "  s32 *temp_a1;\n  GameCar *temp_v0;\n"
OUTER_GUARD = "((s8) temp_v0->pad0EC[0x26D]) <= 0"
INNER_GUARD = "(*((s8 *) (((s8 *) temp_a1) + 0x640))) != 0"
FLAGS = ["-g0", "-O2", "-mips2", "-G", "0", "-non_shared"]
BASE_ORDER = ("0", "2/3", "1")


def _sha256(source: str) -> str:
    return hashlib.sha256(source.encode("utf-8")).hexdigest()


def _replace_once(source: str, before: str, after: str) -> str:
    if source.count(before) != 1:
        raise ValueError("reviewed source anchor is missing or ambiguous")
    return source.replace(before, after, 1)


def _parts(source: str) -> tuple[str, str, dict[str, str], str]:
    """Split only the reviewed function; retain all unrelated bytes verbatim."""
    prefix, signature, body = source.partition(SIGNATURE)
    if not signature or SIGNATURE in body:
        raise ValueError("expected exactly one reviewed function definition")
    start0 = body.index("    case 0:\n")
    start23 = body.index("    case 2:\n")
    start1 = body.index("    case 1:\n")
    end = body.rindex("  }\n\n}\n")
    return prefix, signature + body[:start0], {
        "0": body[start0:start23],
        "2/3": body[start23:start1],
        "1": body[start1:end],
    }, body[end:]


def _guard_variants(case0: str, case1: str) -> dict[str, tuple[str, str]]:
    outer_open = "      if (" + OUTER_GUARD + ")\n    {\n\n"
    inner_open = "      if (" + INNER_GUARD + ")\n      {\n"
    outer_early = (
        "      if (" + OUTER_GUARD.replace("<= 0", "> 0") + ")\n"
        "      {\n        return;\n      }\n\n"
    )
    inner_early = (
        "      if (" + INNER_GUARD.replace("!= 0", "== 0") + ")\n"
        "      {\n        return;\n      }\n"
    )
    both = _replace_once(case0, outer_open, outer_early)
    both = _replace_once(both, inner_open, inner_early)
    both = _replace_once(both, "      }\n    }\n\n    return;", "\n    return;")
    outer = _replace_once(case0, outer_open, outer_early)
    outer = _replace_once(outer, "      }\n    }\n\n    return;", "      }\n\n    return;")
    inner = _replace_once(case0, inner_open, inner_early)
    inner = _replace_once(inner, "      }\n    }\n\n    return;", "    }\n\n    return;")

    condition = "    case 1:\n      if (gameplay_mode == 6)\n"
    arms = case1.removeprefix(condition)
    if arms == case1:
        raise ValueError("reviewed gameplay-mode branch is missing")
    yes_arm, separator, no_arm_and_break = arms.partition("    else\n")
    if not separator:
        raise ValueError("reviewed gameplay-mode else arm is missing")
    no_arm, break_text, trailing = no_arm_and_break.partition("      break;\n\n")
    if not break_text or trailing:
        raise ValueError("reviewed case-1 terminator changed")
    reversed_arms = (
        condition.replace("== 6", "!= 6") + "    {\n" + no_arm
        + "    }\n    else\n" + yes_arm + break_text
    )
    return {
        "G01": (both, case1),
        "G02": (outer, case1),
        "G03": (case0, reversed_arms),
        "G04": (inner, case1),
    }


def _reverse_float_comparisons(case1: str) -> str:
    lines = []
    changed = 0
    for line in case1.splitlines(keepends=True):
        if "> 2.25f)" in line or "> 3.5f)" in line:
            indent, _, expression = line.partition("if (")
            left, _, right = expression.rpartition(" > ")
            if not left or right not in {"2.25f)\n", "3.5f)\n"}:
                raise ValueError("reviewed floating-point comparison changed")
            line = indent + "if (" + right[:-2] + " < " + left + ")\n"
            changed += 1
        lines.append(line)
    if changed != 2:
        raise ValueError("expected both reviewed floating-point comparisons")
    return "".join(lines)


def variants(source: str) -> dict[str, dict[str, str]]:
    """Return twenty complete, unique C proposals and their review metadata.

    The proof obligation is equivalence to this existing C, within its defined
    behavior. These transformations do not repair or reinterpret its pointer
    arithmetic, aliasing, layout assumptions, or implementation-defined casts.
    Predicted native effects are hypotheses, not established measurements.
    """
    if _sha256(source) != SOURCE_SHA256:
        raise ValueError("baseline differs from the reviewed pinned translation unit")
    prefix, opening, groups, closing = _parts(source)
    result: dict[str, dict[str, str]] = {}
    seen = {source}

    def add(candidate_id: str, body: str, edit: str, justification: str, effect: str) -> None:
        candidate = prefix + body
        if candidate in seen:
            raise ValueError("accidental duplicate proposal: " + candidate_id)
        if not candidate.startswith(prefix + SIGNATURE):
            raise ValueError("proposal changed authentic translation-unit context")
        seen.add(candidate)
        result[candidate_id] = {
            "source": candidate,
            "sha256": _sha256(candidate),
            "edit": edit,
            "semantic_justification": justification,
            "native_effect": effect,
            "patch": "".join(difflib.unified_diff(
                source.splitlines(keepends=True), candidate.splitlines(keepends=True),
                fromfile="baseline.c", tofile="candidates/" + candidate_id + ".c",
            )),
        }

    def join(order: tuple[str, ...], replacements: dict[str, str] | None = None) -> str:
        case_groups = {**groups, **(replacements or {})}
        return opening + "".join(case_groups[key] for key in order) + closing

    orders = [order for order in itertools.permutations(("0", "1", "2/3")) if order != BASE_ORDER]
    case_reason = (
        "Every moved complete case group terminates in return or break. Cases 2 and 3 "
        "remain paired, and no other group falls through. Dispatch values, bodies, "
        "read/write/call order and the implicit no-op default are unchanged."
    )
    local_reason = (
        "The two existing pointer locals have no initializers, volatile qualifiers, "
        "or observable declaration effects. Types, assignments, lifetimes, uses "
        "and the function signature remain unchanged; no storage is added."
    )
    for index, order in enumerate(orders, 1):
        add("C%02d" % index, join(order),
            "Move complete case groups from [0, 2/3, 1] to [" + ", ".join(order) + "].",
            case_reason, "May change block emission, branch layout or scheduling; may collapse to the baseline.")
    add("D01", join(BASE_ORDER).replace(DECLARATIONS, REVERSED_DECLARATIONS, 1),
        "Reverse the declarations of GameCar *temp_v0 and s32 *temp_a1 only.",
        local_reason, "May change register allocation or frame homes without adding storage; may collapse to the baseline.")

    guard_edits = {
        "G01": "Flatten both nested case-0 guards into early returns using > 0 and == 0.",
        "G02": "Flatten only the outer case-0 guard into an early return using > 0.",
        "G03": "Change case-1 gameplay_mode == 6 to != 6 and exchange both complete arms.",
        "G04": "Flatten only the inner case-0 guard into an early return using == 0.",
    }
    guard_reason = (
        "The integer predicates are replaced by exact complements. Rejected paths "
        "originally reached the unconditional case-0 return without later effects. "
        "The outer signed-byte read still precedes pointer assignment and the inner "
        "signed-byte read; rejected outer paths do not evaluate either. Accepted "
        "paths retain the exact store, call, state store, then float load/store order. "
        "The post-call float load remains after the call, including if it mutates data."
    )
    branch_reason = (
        "The gameplay_mode integer comparison is complemented once and its complete "
        "arms are exchanged, with braces preventing dangling-else rebinding. Mode 6 "
        "still tests 2.25f and returns after the state store; every other mode still "
        "tests 3.5f. The subtraction, reads, stores and remaining break are unchanged."
    )
    guards = _guard_variants(groups["0"], groups["1"])
    for candidate_id, (case0, case1) in guards.items():
        add(candidate_id, join(BASE_ORDER, {"0": case0, "1": case1}),
            guard_edits[candidate_id], branch_reason if candidate_id == "G03" else guard_reason,
            "May change branch/block layout and delay-slot scheduling; may collapse to the baseline.")

    for index, parent_id in enumerate(["C%02d" % n for n in range(1, 6)] + list(guards), 1):
        parent = result[parent_id]
        body = parent["source"][len(prefix):]
        add("X%02d" % index, _replace_once(body, DECLARATIONS, REVERSED_DECLARATIONS),
            "Combine D01 with " + parent_id + ": " + parent["edit"],
            local_reason + " " + parent["semantic_justification"],
            "Tests interaction between declaration order and " + parent_id
            + " block structure; allocation/scheduling effects need not be additive and may collapse.")

    add("E01", join(BASE_ORDER, {"1": _reverse_float_comparisons(groups["1"])}),
        "Replace the two tests (unchanged subtraction) > 2.25f/3.5f with 2.25f/3.5f < (unchanged subtraction).",
        "Exchanging operands and reversing > to < preserves each ordered floating "
        "comparison, including false for an unordered NaN result, signed zero, "
        "infinities and boundary equality. The subtraction operands, rounding, "
        "literals and branches are unchanged. The exchanged operand is a constant, "
        "so no side-effecting read or call changes evaluation order. Neither test "
        "is rewritten as a negated <= or >=, which would change NaN behavior.",
        "May change operand/register scheduling while retaining ordered comparison semantics; may collapse to the baseline.")
    return result


def materialize(source_path: Path, output_dir: Path) -> dict[str, object]:
    """Write a private plan into a fresh directory and return a review receipt."""
    source_path, output_dir = Path(source_path), Path(output_dir)
    source = source_path.read_bytes().decode("utf-8")
    proposals = variants(source)
    # Refuse an existing directory so prior review/run evidence is never replaced.
    output_dir.mkdir(parents=True, exist_ok=False)
    (output_dir / "candidates").mkdir()
    (output_dir / "patches").mkdir()
    snapshot_path = output_dir / SOURCE_PATH
    snapshot_path.parent.mkdir(parents=True)
    snapshot_path.write_bytes(source.encode("utf-8"))
    snapshot_path.chmod(0o444)
    (output_dir / "baseline.c").write_bytes(source.encode("utf-8"))
    (output_dir / "baseline.c").chmod(0o444)
    predictions = {}
    review = {
        "schema": "rush-car-select-review-v1",
        "base_commit": BASE_COMMIT,
        "source": SOURCE_PATH,
        "baseline_sha256": SOURCE_SHA256,
        "prefix_sha256": _sha256(_parts(source)[0]),
        "status": "source_reviewed_uncompiled",
        "claims": [],
        "semantic_scope": "Equivalent to the pinned C within its defined behavior; no native equivalence or matching claim.",
        "candidates": {},
    }
    for candidate_id, proposal in proposals.items():
        source_name = "candidates/" + candidate_id + ".c"
        patch_name = "patches/" + candidate_id + ".patch"
        candidate_path = output_dir / source_name
        candidate_path.write_bytes(proposal["source"].encode("utf-8"))
        candidate_path.chmod(0o444)
        (output_dir / patch_name).write_text(proposal["patch"], encoding="utf-8")
        predictions[candidate_id] = {
            "source": source_name,
            **{key: proposal[key] for key in ("sha256", "edit", "semantic_justification", "native_effect")},
            "check": {"kind": "diagnostic", "signal_ids": []},
        }
        review["candidates"][candidate_id] = {**predictions[candidate_id], "patch": patch_name}

    experiment = {
        "schema": "decomp-workbench-experiment-v2",
        "family": "car-select-source-structure-calibration",
        "baseline": "baseline.c",
        "parameters": {"variant": list(proposals)},
        "candidates": [{"source": predictions[key]["source"], "parameters": {"variant": key}} for key in proposals],
        "signals": [], "controls": [], "coverage": {},
    }
    context = {
        "schema": "rush-hypothesis-context-v1", "source": SOURCE_PATH,
        "files": [{"path": SOURCE_PATH, "sha256": SOURCE_SHA256}],
    }
    batch = {
        "schema": "rush-hypothesis-batch-v1", "base_commit": BASE_COMMIT,
        "function": "car_select_handler", "recipe": "single", "purpose": "calibration",
        "experiment": "experiment.json", "baseline": "baseline.c", "baseline_sha256": SOURCE_SHA256,
        "flags": FLAGS, "targets": "asm/us/blob", "context_manifest": "context.json",
        "hypothesis": "Natural local and branch structure changes can alter IDO allocation or scheduling while preserving behavior.",
        "limits": {"variants": 20, "jobs": 2, "compile_seconds": 120, "score_seconds": 30, "diagnose_seconds": 30},
        "predictions": predictions,
    }
    for filename, value in (("review.json", review), ("experiment.json", experiment), ("context.json", context), ("batch.json", batch)):
        (output_dir / filename).write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")
    return {"output": str(output_dir), "variants": len(proposals), "baseline_sha256": SOURCE_SHA256, "status": review["status"]}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=ROOT / SOURCE_PATH)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    try:
        result = materialize(args.source, args.out)
    except (OSError, ValueError) as error:
        parser.exit(1, "car-select preparation failed: " + str(error) + "\n")
    print(json.dumps(result, sort_keys=True))


if __name__ == "__main__":
    main()
