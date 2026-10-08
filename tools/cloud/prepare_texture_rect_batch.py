#!/usr/bin/env python3
"""Freeze ten source-grounded texture-rectangle hypotheses without compiling.

Two candidates are exact historical controls. Eight new candidates test genuine
initialization and coordinate-lifetime changes inside the successful alias-close
wrapper family. The pinned SDK macro context, ABI, and unrelated source remain
byte-for-byte unchanged. This is a research experiment, never match acceptance.
"""
from __future__ import annotations

import argparse
from datetime import datetime, timezone
import difflib
import hashlib
import json
from pathlib import Path
import time
import uuid

ROOT = Path(__file__).resolve().parents[2]
BASE_COMMIT = "c35728addbae1626aabc89b7d837e28774f333a0"
SOURCE_PATH = "cloud/work/frontier/w4a/func_80087110/best.c"
SOURCE_SHA256 = "f8355db6884b85c77a25b21c7aae746be236ed10a3dc8fee93264259cf4c6f8f"
SIGNATURE = "void func_80087110(int x,int y,int right,int bottom,int s,int t)\n"
FLAGS = ["-g0", "-O3", "-mips2", "-G", "0", "-non_shared"]
HISTORICAL = {
    "C01": {"name": "both_body_do", "sha256": "98a0adc8027464275b43845a6e223269c96a7cef2e1ec00395d264e0754b9c35", "differing": 8},
    "C02": {"name": "setup_do", "sha256": "3fb16b926eb91443d08b7836685658c76fb683df3143d021f78984aacebd2ea1", "differing": 29},
}
SETUP = (
    "        height=bottom-y;\n"
    "        if(D_8012E608&0x8000) {bottom+=height+1;step=512;offset=16;}\n"
    "        else {step=1024;offset=0;}\n"
)
WRAPPED_SETUP = SETUP.replace("        height=bottom-y;", "        do {height=bottom-y;").replace(
    "else {step=1024;offset=0;}", "else {step=1024;offset=0;}} while(0);")
BOTH_BODY = (
    "            texture_edge=s+right;s=texture_edge-x;t+=height;\n"
    "            gSPTextureRectangle(D_80149438++,x<<2,y<<2,(right+1)<<2,(bottom+1)<<2,0,s<<5,(t<<5)+offset,-1024,-step);\n"
)
BODY_WRAPPED = "            do {\n" + BOTH_BODY + "            } while(0);\n"
EARLY_STEP = "        if(D_8012E608&0x8000) {step=512;} else {step=1024;}\n"
EARLY_OFFSET = "        if(D_8012E608&0x8000) {offset=16;} else {offset=0;}\n"
EARLY_SETTINGS = "        if(D_8012E608&0x8000) {step=512;offset=16;} else {step=1024;offset=0;}\n"


def sha256(source: str) -> str:
    return hashlib.sha256(source.encode("utf-8")).hexdigest()


def replace_once(source: str, before: str, after: str) -> str:
    if source.count(before) != 1:
        raise ValueError("reviewed source anchor is missing or ambiguous")
    return source.replace(before, after, 1)


def variants(source: str) -> dict[str, dict[str, str]]:
    """Return the complete predeclared bounded family, without native execution.

    Equivalence is to the pinned C on its defined domain, not an extension of
    its signed arithmetic, shifts, storage lifetime, or display-list aliasing
    assumptions. Native outcomes below are hypotheses, except named history.
    """
    if sha256(source) != SOURCE_SHA256:
        raise ValueError("baseline differs from the reviewed pinned translation unit")
    if source.count(SETUP) != 1 or source.count(BOTH_BODY) != 1:
        raise ValueError("reviewed stretched source region changed")
    prefix, rest = source.split(SETUP, 1)
    wrapped_draw = replace_once(rest, BOTH_BODY, BODY_WRAPPED)
    baseline_context = source.split(SIGNATURE, 1)[0]
    result: dict[str, dict[str, str]] = {}
    seen = {source}
    common = (
        "Clipping, rejection, mode-zero behavior, global declarations, the ABI and "
        "all eight SDK packet emissions retain their command order. Arithmetic operations "
        "retain their signed types and grouping. All moved computations precede "
        "the first display-list store, and touch only by-value parameters or "
        "existing ordinary locals. No extra caller, dummy use, qualifier, "
        "inline assembly, or instruction padding is introduced. Equivalence is "
        "claimed only within the pinned C's defined behavior. "
    )
    init_reason = (
        "The same scale-bit predicate selects the same initialized values on "
        "every nonzero-mode path. Splitting its selection can repeat an ordinary "
        "non-volatile flag read, but no call or global store intervenes, so its "
        "value is stable under the source's defined single-threaded behavior. "
        "Each initialized value is subsequently consumed by a real packet or "
        "geometry computation; there is no new default overwritten before use. "
    )
    wrapper_reason = (
        "The retained do-while-zero blocks execute their genuine statements "
        "exactly once, with no break, continue, or declaration-scope change. "
    )

    def add(candidate_id: str, setup: str, drawing: str, edit: str, reason: str, effect: str) -> None:
        candidate = prefix + setup + drawing
        if candidate in seen:
            raise ValueError("duplicate or unchanged candidate: " + candidate_id)
        if not candidate.startswith(baseline_context + SIGNATURE):
            raise ValueError("candidate changed pinned macro/signature context")
        seen.add(candidate)
        result[candidate_id] = {
            "source": candidate, "sha256": sha256(candidate), "edit": edit,
            "semantic_justification": common + wrapper_reason + reason,
            "native_effect": effect,
            "patch": "".join(difflib.unified_diff(source.splitlines(keepends=True), candidate.splitlines(keepends=True),
                                                   fromfile="baseline.c", tofile="candidates/" + candidate_id + ".c")),
        }

    add("C01", SETUP, wrapped_draw,
        "Historical source control: exact both_body_do, wrapping only the stretched both-flags drawing body.",
        "No existing expression, statement, or evaluation order is changed.",
        "Historical 8/445: alias close moves before the unconditional exit and original schedule residual disappears, "
        "but height/offset exchange remains. Reproduction is a control, not a new hypothesis or accepted match.")
    add("C02", WRAPPED_SETUP, wrapped_draw,
        "Historical source control: exact setup_do, additionally wrapping complete height/stretch initialization.",
        "No existing expression, statement, or evaluation order is changed.",
        "Historical 29/445: successful alias closure and desired height/offset pair, but s/step exchange. "
        "Reproduction is a control; the cited history used the same O3 recipe plus the multiply-workaround flag.")
    for candidate_id, setup, parent in (("S01", SETUP, "C01"), ("S02", WRAPPED_SETUP, "C02")):
        without_step = setup.replace(";step=512;", ";").replace("step=1024;", "")
        add(candidate_id, EARLY_STEP + without_step, wrapped_draw,
            "From " + parent + ", select the genuinely used vertical step before height/stretch setup, outside any setup wrapper.",
            init_reason,
            "Lengthens step's genuine initialized live range across height/geometry and, for S02, the setup entry. "
            "Tests whether its occurrence/live-IN normalized divisor rises enough to put s ahead again while keeping "
            "the body alias boundary. S01/S02 isolate interaction with the extra setup block; C01 may retain height/offset exchange.")
    inside_step = replace_once(WRAPPED_SETUP, "        do {height=bottom-y;\n", "        do {\n" + EARLY_STEP + "        height=bottom-y;\n")
    inside_step = inside_step.replace(";step=512;", ";").replace("else {step=1024;offset=0;}", "else {offset=0;}")
    add("S03", inside_step, wrapped_draw,
        "From C02, select step first inside the setup wrapper, then compute height/stretch; compare S02 outside entry.",
        init_reason,
        "Distinguishes true step-definition order from crossing the setup entry. Step remains uninitialized at wrapper entry; "
        "if S02 alone changes its divisor, entry live-IN membership rather than spelling is implicated. Alias fix should remain.")
    early_offset_setup = SETUP.replace(";offset=16;", ";").replace(";offset=0;", ";")
    add("O01", EARLY_OFFSET + early_offset_setup, wrapped_draw,
        "From C01, select the genuine vertical texture offset before height/stretch geometry; retain step assignment in scale arms.",
        init_reason,
        "Targets the height/offset mismatch directly by extending offset's initialized range instead of shortening height. "
        "A divisor change may restore their priority tie without the setup wrapper that caused s/step exchange; interference may also change.")
    geometry_only = (
        "        do {height=bottom-y;\n"
        "        if(D_8012E608&0x8000) {bottom+=height+1;}\n"
        "        } while(0);\n"
    )
    add("O02", EARLY_SETTINGS + geometry_only, wrapped_draw,
        "From C02, select genuine step and offset together before the setup wrapper, leaving only height/stretch geometry inside.",
        init_reason,
        "Extends step and offset across wrapper entry together. Tests a settings-before-geometry structure for both allocation "
        "ties while preserving the drawing wrapper's alias boundary. It may collapse, change instruction population, or harm interference.")

    def final_edge(drawing: str) -> str:
        drawing = replace_once(drawing, "texture_edge=s+right;s=texture_edge-x;t+=height;", "texture_edge=s+right;texture_edge=texture_edge-x;t+=height;")
        drawing = replace_once(drawing, "texture_edge=s+right;s=texture_edge-x;\n", "texture_edge=s+right;texture_edge=texture_edge-x;\n")
        drawing = replace_once(drawing, ",0,s<<5,(t<<5)+offset,-1024,-step);", ",0,texture_edge<<5,(t<<5)+offset,-1024,-step);")
        return replace_once(drawing, ",0,s<<5,t<<5,-1024,step);", ",0,texture_edge<<5,t<<5,-1024,step);")

    coordinate_reason = (
        "In either horizontally flipped stretched terminal arm, the existing "
        "texture_edge local holds the same two-step sum then subtraction that "
        "previously overwrote s. Its identical final value is passed to the "
        "identical macro argument. Neither local is observed after this terminal "
        "arm, and no storage or arithmetic operation is added. "
    )
    for candidate_id, setup, parent in (("L01", SETUP, "C01"), ("L02", WRAPPED_SETUP, "C02")):
        add(candidate_id, setup, final_edge(wrapped_draw),
            "From " + parent + ", keep the final flipped horizontal coordinate in existing texture_edge instead of redefining s in stretched arms.",
            coordinate_reason,
            "Separates the original s input web from the final flipped-coordinate web without adding a local or copy. "
            "Tests whether s's occurrence/live-IN membership and priority relative to step change; may canonicalize back. "
            "L01/L02 distinguish the coordinate representation from the setup-wrapper interaction.")
    horizontal_first = "        if(D_8012E608&4) {texture_edge=s+right;s=texture_edge-x;}\n"
    common_draw = replace_once(wrapped_draw, "            texture_edge=s+right;s=texture_edge-x;t+=height;\n", "            t+=height;\n")
    common_draw = replace_once(common_draw, "            texture_edge=s+right;s=texture_edge-x;\n", "")
    add("L03", horizontal_first + WRAPPED_SETUP, common_draw,
        "From C02, compute the shared genuine horizontal flip before setup and remove its duplicate arithmetic in the two flipped drawing arms.",
        "The horizontal bit is stable before any packet store. Exactly the same "
        "sum and subtraction execute once on horizontal-flip paths and never on "
        "other paths. The moved expressions do not use or modify bottom, height, "
        "step, offset or t, and no global write or call is crossed. " + init_reason,
        "Moves s's final horizontal definition before setup, reducing later branch-specific s occurrences and separating "
        "coordinate preparation from geometry setup. Tests a changed s web against step while retaining the successful "
        "body alias boundary; differing branch structure may change instruction population and is not presumed beneficial.")
    for candidate_id, historical in HISTORICAL.items():
        if result[candidate_id]["sha256"] != historical["sha256"]:
            raise ValueError("historical source control changed: " + candidate_id)
    if len(result) != 10:
        raise ValueError("bounded reviewed family changed")
    return result


def _utc() -> str:
    return datetime.now(timezone.utc).isoformat(timespec="microseconds")


def materialize(source_path: Path, output_dir: Path) -> dict[str, object]:
    started_utc, started_monotonic = _utc(), time.monotonic()
    materialization_id = str(uuid.uuid4())
    source_path, output_dir = Path(source_path), Path(output_dir)
    source = source_path.read_bytes().decode("utf-8")
    proposals = variants(source)
    # Fail before touching an existing experiment or creating output for a bad TU.
    output_dir.mkdir(parents=True, exist_ok=False)
    events_path = output_dir / "materialization-events.jsonl"

    def event(kind: str, status: str, **extra: object) -> None:
        value = {"stage": "materialization", "event": kind, "status": status,
                 "materialization_id": materialization_id, **extra}
        with events_path.open("a", encoding="utf-8") as handle:
            handle.write(json.dumps(value, sort_keys=True) + "\n")
            handle.flush()

    event("start", "running", utc=started_utc, monotonic_seconds=started_monotonic)
    try:
        result = _materialize_files(source, output_dir, proposals)
    except BaseException as error:
        finished = time.monotonic()
        event("end", "failed", utc=_utc(), monotonic_seconds=finished,
              duration_seconds=finished - started_monotonic, error_type=type(error).__name__)
        raise
    finished = time.monotonic()
    duration, finished_utc = finished - started_monotonic, _utc()
    event("end", "completed", utc=finished_utc, monotonic_seconds=finished,
          duration_seconds=duration, variants=len(proposals))
    return {**result, "materialization_id": materialization_id, "started_utc": started_utc,
            "finished_utc": finished_utc, "duration_seconds": duration}


def _materialize_files(source: str, output_dir: Path, proposals: dict[str, dict[str, str]]) -> dict[str, object]:
    (output_dir / "candidates").mkdir()
    (output_dir / "patches").mkdir()
    for path in (output_dir / "baseline.c", output_dir / SOURCE_PATH):
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(source.encode("utf-8"))
        path.chmod(0o444)
    predictions = {}
    review = {
        "schema": "rush-texture-rect-review-v1", "base_commit": BASE_COMMIT,
        "source": SOURCE_PATH, "baseline_sha256": SOURCE_SHA256,
        "prefix_sha256": sha256(source.split(SIGNATURE, 1)[0]),
        "status": "source_reviewed_uncompiled", "claims": [],
        "semantic_scope": "Equivalent to the pinned C on its defined domain; no native equivalence, matching or cartridge claim.",
        "historical_controls": HISTORICAL,
        "excluded_families": ["guarded exits", "SDK macro replacement", "shared tails", "fake callers", "dummy or dead operations", "score-only volatile", "inline assembly"],
        "candidates": {},
    }
    for candidate_id, proposal in proposals.items():
        source_name, patch_name = "candidates/" + candidate_id + ".c", "patches/" + candidate_id + ".patch"
        candidate_path = output_dir / source_name
        candidate_path.write_bytes(proposal["source"].encode("utf-8"))
        candidate_path.chmod(0o444)
        (output_dir / patch_name).write_text(proposal["patch"], encoding="utf-8")
        predictions[candidate_id] = {
            "source": source_name,
            **{key: proposal[key] for key in ("sha256", "edit", "semantic_justification", "native_effect")},
            "check": {"kind": "diagnostic", "signal_ids": []},
        }
        review["candidates"][candidate_id] = {**predictions[candidate_id], "patch": patch_name,
            "kind": "historical_source_control" if candidate_id in HISTORICAL else "new_source_hypothesis"}
    experiment = {
        "schema": "decomp-workbench-experiment-v2", "family": "texture-rectangle-alias-and-allocation",
        "baseline": "baseline.c", "parameters": {"variant": list(proposals)},
        "candidates": [{"source": predictions[key]["source"], "parameters": {"variant": key}} for key in proposals],
        "signals": [], "controls": [], "coverage": {},
    }
    context = {"schema": "rush-hypothesis-context-v1", "source": SOURCE_PATH,
               "files": [{"path": SOURCE_PATH, "sha256": SOURCE_SHA256}]}
    batch = {
        "schema": "rush-hypothesis-batch-v1", "base_commit": BASE_COMMIT,
        "function": "func_80087110", "recipe": "single", "purpose": "experiment",
        "experiment": "experiment.json", "baseline": "baseline.c", "baseline_sha256": SOURCE_SHA256,
        "baseline_expectation": {"differing": 4, "total": 445, "extra_words": 0},
        "flags": FLAGS, "targets": "asm/us/blob", "context_manifest": "context.json",
        "hypothesis": "Successful ordinary-C alias-boundary wrappers plus genuine step/offset initialization or horizontal-coordinate lifetime changes can satisfy both scheduling and register-priority constraints.",
        "limits": {"variants": len(proposals), "jobs": 2, "compile_seconds": 120, "score_seconds": 30, "diagnose_seconds": 30},
        "predictions": predictions,
    }
    for filename, value in (("review.json", review), ("experiment.json", experiment), ("context.json", context), ("batch.json", batch)):
        (output_dir / filename).write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")
    return {"output": str(output_dir), "variants": len(proposals), "historical_controls": 2,
            "baseline_sha256": SOURCE_SHA256, "status": review["status"]}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=ROOT / SOURCE_PATH)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    try:
        result = materialize(args.source, args.out)
    except (OSError, ValueError) as error:
        parser.exit(1, "texture-rectangle preparation failed: " + str(error) + "\n")
    print(json.dumps(result, sort_keys=True))


if __name__ == "__main__":
    main()
