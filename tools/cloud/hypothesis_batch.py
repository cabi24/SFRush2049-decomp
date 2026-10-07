#!/usr/bin/env python3
"""Finite, frozen Rush experiments. Workbench diagnostics are never acceptance.

validate PLAN; prepare PLAN --out DIR; run PLAN --jobs 2 --out NEW_DIR;
report DIR. `prepare` runs only the two baseline controls and a negative canary.
All run artifacts are private. Only `public-summary.json` and `summary.md` are
review exports; never publish the snapshot, objects, logs or diagnosis files.
"""
from __future__ import annotations

import argparse
import concurrent.futures
from dataclasses import asdict
import hashlib
import importlib.util
import io
import json
import math
import os
from pathlib import Path
import re
import shlex
import shutil
import signal
import subprocess
import sys
import tarfile
import threading
import time

REPO = Path(__file__).resolve().parents[2]
SCHEMA = "rush-hypothesis-batch-v1"
FLAGS = ["-g0", "-O2", "-mips2", "-G", "0", "-non_shared"]
NAME = re.compile(r"[A-Za-z_][A-Za-z_0-9]*\Z")
HEX = re.compile(r"[0-9a-f]{64}\Z")
_ACTIVE = set()
_ACTIVE_LOCK = threading.RLock()
_CANCELLED = threading.Event()


def digest(data):
    return hashlib.sha256(data).hexdigest()


def file_hash(path):
    return digest(Path(path).read_bytes())


def identity(value):
    return digest(json.dumps(value, sort_keys=True, separators=(",", ":")).encode())


def write_json(path, value):
    """Atomic receipts: interrupted temporary files are never completed stages."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(json.dumps(value, indent=2, sort_keys=True) + "\n")
    temporary.replace(path)


def read_json(path):
    def unique(pairs):
        result = {}
        for key, value in pairs:
            if key in result:
                raise ValueError("duplicate JSON key: " + key)
            result[key] = value
        return result
    return json.loads(Path(path).read_text(), object_pairs_hook=unique)


def fields(value, required, optional=()):
    if not isinstance(value, dict):
        raise ValueError("expected an object")
    missing, extra = set(required) - set(value), set(value) - set(required) - set(optional)
    if missing or extra:
        raise ValueError(f"invalid fields; missing={sorted(missing)}, unknown={sorted(extra)}")


def inside(root, name, exists=True):
    if not isinstance(name, str) or not name or Path(name).is_absolute():
        raise ValueError("expected a relative path")
    parts = Path(name).parts
    if ".." in parts or "." == name:
        raise ValueError("out-of-root path")
    path = root / name
    if any(part.is_symlink() for part in [path, *path.parents] if part != root.parent):
        raise ValueError("symlink input is not a frozen file")
    if not path.resolve().is_relative_to(root.resolve()):
        raise ValueError("out-of-root path")
    if exists and not path.is_file():
        raise ValueError("missing input: " + name)
    return path


def git_bytes(repo, commit, path):
    return subprocess.check_output(["git", "-C", str(repo), "show", f"{commit}:{path}"], stderr=subprocess.PIPE)


def wb_imports(repo=REPO):
    sys.path.insert(0, str(repo / "third_party/n64-decomp-workbench/src"))
    from decomp_workbench.experiments import load_experiment
    return load_experiment


def split_function(source, function):
    """Conservative single-definition boundary check, not a C semantic proof."""
    masked = re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                    lambda match: " " * len(match[0]), source, flags=re.S)
    matches = list(re.finditer(r"\b" + re.escape(function) + r"\s*\([^;{}]*\)\s*\{", masked))
    if len(matches) != 1:
        raise ValueError("expected exactly one selected function definition")
    start = matches[0].end()
    depth, end = 1, start
    while depth and end < len(masked):
        depth += (masked[end] == "{") - (masked[end] == "}")
        end += 1
    if depth:
        raise ValueError("unbalanced selected function")
    return source[:start], source[start:end - 1], source[end - 1:]


def has_preprocessor(text):
    # Deliberately conservative for preprocessed TUs, even inside comments or
    # strings. Reject alternate directive tokens and line-spliced spellings.
    text = text.replace("\\\r\n", "").replace("\\\n", "")
    return any(token in text for token in ("#", "??=", "%:"))


def validate(plan_path, repo=REPO):
    plan_path = Path(plan_path).resolve()
    root, plan = plan_path.parent, read_json(plan_path)
    fields(plan, ("schema", "base_commit", "function", "recipe", "purpose", "experiment", "baseline",
                  "baseline_sha256", "flags", "targets", "context_manifest", "hypothesis", "limits", "predictions"))
    if plan["schema"] != SCHEMA or plan["recipe"] != "single" or plan["purpose"] != "calibration":
        raise ValueError("v1 supports only single-function calibration")
    if not re.fullmatch(r"[0-9a-f]{40}", str(plan["base_commit"])) or not NAME.fullmatch(str(plan["function"])):
        raise ValueError("invalid pinned commit or function")
    if plan["flags"] != FLAGS:
        raise ValueError("v1 requires the explicit standalone O2 recipe; no IPA or injected include flags")
    if plan["targets"] not in ("asm/us/blob", "asm/us/ovl_a", "asm/us/ovl_b"):
        raise ValueError("unsupported target directory")
    limits = plan["limits"]
    fields(limits, ("variants", "jobs", "compile_seconds", "score_seconds", "diagnose_seconds"))
    for name, maximum in (("variants", 20), ("jobs", 2), ("compile_seconds", 120), ("score_seconds", 30), ("diagnose_seconds", 30)):
        if type(limits[name]) is not int or not 1 <= limits[name] <= maximum:
            raise ValueError("excessive or invalid budget: " + name)
    baseline = inside(root, plan["baseline"])
    if not HEX.fullmatch(str(plan["baseline_sha256"])) or file_hash(baseline) != plan["baseline_sha256"]:
        raise ValueError("baseline hash changed")
    context_path = inside(root, plan["context_manifest"])
    context = read_json(context_path)
    fields(context, ("schema", "source", "files"))
    if context["schema"] != "rush-hypothesis-context-v1" or not isinstance(context["files"], list):
        raise ValueError("invalid context manifest")
    context_files = {}
    for entry in context["files"]:
        fields(entry, ("path", "sha256"))
        inside(repo, entry["path"], exists=False)
        if entry["path"] in context_files or not HEX.fullmatch(str(entry["sha256"])):
            raise ValueError("duplicate context path or invalid hash")
        data = git_bytes(repo, plan["base_commit"], entry["path"])
        if digest(data) != entry["sha256"]:
            raise ValueError("context differs from pinned commit: " + entry["path"])
        context_files[entry["path"]] = entry["sha256"]
    if context_files.get(context["source"]) != plan["baseline_sha256"]:
        raise ValueError("baseline is not the actual pinned production TU")
    prefix, _, suffix = split_function(baseline.read_text(), plan["function"])
    # Self-contained TUs are deliberately the MVP boundary. Includes need a
    # dependency-closure verifier before this can accept a general TU.
    if has_preprocessor(baseline.read_text()):
        raise ValueError("v1 requires a self-contained production TU (include closure unsupported)")
    experiment_path = inside(root, plan["experiment"])
    raw_experiment = read_json(experiment_path)
    fields(raw_experiment, ("schema", "family", "baseline", "parameters", "candidates"),
           ("signals", "controls", "coverage", "homologous_parameters", "selected_region", "invariants"))
    if raw_experiment["schema"] != "decomp-workbench-experiment-v2":
        raise ValueError("experiment-v2 required")
    for entry in raw_experiment["candidates"]:
        fields(entry, ("source", "parameters"))
        inside(root, entry["source"])
    inside(root, raw_experiment["baseline"])
    # Baseline repeats and negative canary are owned by this adapter. Reject
    # custom controls rather than silently ignoring Workbench controls.
    if raw_experiment.get("controls"):
        raise ValueError("custom controls unsupported; adapter runs required baseline/canary controls")
    experiment = wb_imports(repo)(experiment_path)
    if experiment.baseline != baseline or not 1 <= len(experiment.candidates) <= limits["variants"]:
        raise ValueError("invalid baseline or variant count")
    predictions = plan["predictions"]
    if not isinstance(predictions, dict) or len(predictions) != len(experiment.candidates):
        raise ValueError("every candidate needs exactly one prediction")
    paths = set()
    signal_ids = {spec.id for spec in experiment.signals}
    for candidate_id, prediction in predictions.items():
        if not re.fullmatch(r"[A-Z][0-9]{2}", candidate_id):
            raise ValueError("invalid candidate ID")
        fields(prediction, ("source", "sha256", "edit", "semantic_justification", "native_effect", "check"))
        source = inside(root, prediction["source"])
        if source not in experiment.candidates or source in paths:
            raise ValueError("candidate missing or duplicated in experiment")
        paths.add(source)
        if file_hash(source) != prediction["sha256"]:
            raise ValueError("candidate source hash changed: " + candidate_id)
        for key in ("edit", "semantic_justification", "native_effect"):
            if not isinstance(prediction[key], str) or not prediction[key].strip():
                raise ValueError("missing candidate justification")
        head, body, tail = split_function(source.read_text(), plan["function"])
        if (head, tail) != (prefix, suffix) or has_preprocessor(body):
            raise ValueError("candidate changes signature or genuine TU context")
        fields(prediction["check"], ("kind", "signal_ids"))
        check = prediction["check"]
        if check["kind"] != "diagnostic" or not isinstance(check["signal_ids"], list) or not set(check["signal_ids"]) <= signal_ids:
            raise ValueError("unknown expected-effect signal")
    return plan, context, experiment


def load_score(repo):
    sys.path.insert(0, str(repo / "tools/cloud"))
    spec = importlib.util.spec_from_file_location("rush_batch_score", repo / "tools/cloud/score.py")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def kill_group(process, *, grace=3.0):
    # Kill the entire group even if its leader has already exited.
    try:
        os.killpg(process.pid, signal.SIGTERM)
        time.sleep(grace)
        os.killpg(process.pid, signal.SIGKILL)
    except ProcessLookupError:
        pass


def process(command, cwd, env, seconds, log_prefix):
    if _CANCELLED.is_set():
        raise KeyboardInterrupt("cancelled before stage spawn")
    started = time.monotonic()
    log_prefix = Path(log_prefix)
    log_prefix.parent.mkdir(parents=True, exist_ok=True)
    with log_prefix.with_suffix(".stdout").open("wb") as stdout, log_prefix.with_suffix(".stderr").open("wb") as stderr:
        with subprocess.Popen(command, cwd=cwd, env=env, stdout=stdout, stderr=stderr, start_new_session=True) as child:
            with _ACTIVE_LOCK:
                _ACTIVE.add(child)
            try:
                deadline = started + seconds
                while True:
                    if _CANCELLED.is_set():
                        raise KeyboardInterrupt("cancelled")
                    remaining = deadline - time.monotonic()
                    if remaining <= 0:
                        raise subprocess.TimeoutExpired(command, seconds)
                    try:
                        code = child.wait(timeout=min(0.1, remaining))
                        break
                    except subprocess.TimeoutExpired:
                        continue
            except subprocess.TimeoutExpired:
                kill_group(child)
                child.wait()
                code = 124
            except BaseException:
                kill_group(child)
                child.wait()
                raise
            finally:
                with _ACTIVE_LOCK:
                    _ACTIVE.discard(child)
    return {"returncode": code, "seconds": time.monotonic() - started,
            "status": "timeout" if code == 124 else "ok" if code == 0 else "failed"}


def tool_copy(source, destination):
    source = Path(source).resolve()
    if not source.is_file():
        raise ValueError("required tool missing: " + str(source))
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, destination)


def freeze(plan_path, output, jobs):
    plan, context, experiment = validate(plan_path)
    output.mkdir(parents=True, exist_ok=False)
    output.chmod(0o700)
    root = Path(plan_path).resolve().parent
    snapshot = output / "snapshot"
    snapshot.mkdir()
    selections = ["tools/cloud/score.py", "tools/cloud/owndata.py", "tools/conveyor", "tools/workbench.py",
                  "third_party/n64-decomp-workbench", plan["targets"], plan["targets"] + "_data",
                  *[entry["path"] for entry in context["files"]]]
    archive = subprocess.check_output(["git", "-C", str(REPO), "archive", plan["base_commit"], *selections])
    with tarfile.open(fileobj=io.BytesIO(archive)) as tar:
        for member in tar:
            if member.isdir():
                continue
            if not member.isfile():
                raise ValueError("non-file snapshot entry")
            dest = inside(snapshot, member.name, exists=False)
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_bytes(tar.extractfile(member).read())
    adapter = snapshot / "tools/cloud/hypothesis_batch.py"
    adapter.write_bytes(Path(__file__).read_bytes())
    inputs = output / "inputs"
    inputs.mkdir()
    for name in (plan["baseline"], plan["experiment"], plan["context_manifest"], *[p["source"] for p in plan["predictions"].values()]):
        dest = inside(inputs, name, exists=False)
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(inside(root, name).read_bytes())
    write_json(inputs / "batch.json", plan)
    if file_hash(inputs / plan["baseline"]) != plan["baseline_sha256"]:
        raise ValueError("baseline changed during snapshot")
    for prediction in plan["predictions"].values():
        if file_hash(inputs / prediction["source"]) != prediction["sha256"]:
            raise ValueError("candidate changed during snapshot")
    if read_json(inputs / plan["context_manifest"]) != context or read_json(inputs / plan["experiment"]) != experiment.raw:
        raise ValueError("experiment or context changed during snapshot")
    ido = Path(os.environ.get("IDO_DIR", REPO / "tools/cloud/ido"))
    if not (ido / "cc").is_file():
        raise ValueError("IDO compiler missing; set IDO_DIR to the existing IDO 5.3 directory")
    for path in sorted(ido.iterdir()):
        if path.is_file():
            tool_copy(path, output / "toolchain/ido" / path.name)
    for tool, value in (("mips-linux-gnu-as", shutil.which("mips-linux-gnu-as")),
                        ("mips-linux-gnu-objdump", os.environ.get("MIPS_OBJDUMP") or shutil.which("mips-linux-gnu-objdump"))):
        if not value:
            raise ValueError("required tool missing: " + tool)
        tool_copy(value, output / "toolchain/bin" / tool)
    # Explicit loader paths are copied, never inherited as mutable compiler inputs.
    for directory in filter(None, os.environ.get("LD_LIBRARY_PATH", "").split(os.pathsep)):
        for path in sorted(Path(directory).glob("*.so*")):
            if path.is_file():
                destination = output / "toolchain/lib" / path.name
                if destination.exists() and file_hash(destination) != file_hash(path):
                    raise ValueError("conflicting runtime libraries")
                tool_copy(path, destination)
    env = {"PATH": str(output / "toolchain/bin") + os.pathsep + "/usr/bin:/bin",
           "IDO_DIR": str(output / "toolchain/ido"), "LC_ALL": "C", "LANG": "C", "TZ": "UTC",
           "LD_LIBRARY_PATH": str(output / "toolchain/lib"), "PYTHONHASHSEED": "0", "PYTHONDONTWRITEBYTECODE": "1"}
    files = {}
    for tree in (snapshot, inputs, output / "toolchain"):
        for path in sorted(tree.rglob("*")):
            if path.is_file():
                files[str(path.relative_to(output))] = file_hash(path)
                path.chmod(0o555 if os.access(path, os.X_OK) else 0o444)
    python_identity = {"path": sys.executable, "sha256": file_hash(sys.executable), "version": sys.version}
    envelope = {"files": files, "python": python_identity, "environment": env,
                "base_commit": plan["base_commit"], "requested_flags": plan["flags"],
                "effective_flags": plan["flags"] + ["-Wab,-r4300_mul"]}
    state = {"schema": SCHEMA, "plan": plan, "context": context, "jobs": jobs,
             "envelope": envelope, "envelope_sha256": identity(envelope), "plan_sha256": identity(plan)}
    write_json(output / "frozen.json", state)
    verify_frozen(output)
    return state


def verify_frozen(output):
    state = read_json(output / "frozen.json")
    if identity(state["envelope"]) != state["envelope_sha256"] or identity(state["plan"]) != state["plan_sha256"]:
        raise ValueError("frozen identity changed")
    for name, expected in state["envelope"]["files"].items():
        if file_hash(inside(output, name)) != expected:
            raise ValueError("frozen input changed: " + name)
    python = state["envelope"]["python"]
    if file_hash(python["path"]) != python["sha256"]:
        raise ValueError("Python executable changed")
    return state


def receipt(output, path, artifact, extra=None):
    state = read_json(output / "frozen.json")
    value = {"complete": True, "envelope_sha256": state["envelope_sha256"],
             "artifact": str(artifact.relative_to(output)), "sha256": file_hash(artifact), **(extra or {})}
    write_json(path, value)


def valid_receipt(output, path):
    try:
        state = verify_frozen(output)
        value = read_json(path)
        return (value.get("complete") is True and value["envelope_sha256"] == state["envelope_sha256"]
                and file_hash(inside(output, value["artifact"])) == value["sha256"])
    except (OSError, ValueError, KeyError):
        return False


def bridge(output, source, destination, slot):
    state = verify_frozen(output)
    plan, context = state["plan"], state["context"]
    work = output / "compiles" / slot
    work.mkdir(parents=True, exist_ok=False)
    for item in context["files"]:
        dest = inside(work, item["path"], exists=False)
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes((output / "snapshot" / item["path"]).read_bytes())
    source_path = work / context["source"]
    source_path.write_bytes(source.read_bytes())
    tmp = work / "tmp"
    tmp.mkdir()
    os.environ["TMPDIR"] = str(tmp)
    os.chdir(work)
    scorer = load_score(output / "snapshot")
    obj = work / "candidate.o"
    # Only this disposable process intercepts subprocesses. Stream directly
    # to retained files so a timeout cannot discard buffered compiler output.
    def logged(command, **kwargs):
        stdout, stderr = work / "compiler.stdout", work / "compiler.stderr"
        with stdout.open("wb") as out, stderr.open("wb") as err:
            result = subprocess.run(command, stdout=out, stderr=err, **kwargs)
        return subprocess.CompletedProcess(command, result.returncode,
            stdout.read_text(errors="replace"), stderr.read_text(errors="replace"))
    scorer._run = logged
    scorer.compile_single(context["source"], shlex.join(plan["flags"]), obj)
    try:
        scorer._elf(obj)
    except (Exception, SystemExit) as exc:
        write_json(work / "compile.status.json", {"status": "invalid_elf"})
        raise ValueError("compiler returned an invalid ELF") from exc
    receipt(output, work / "compile.receipt.json", obj, {"source_sha256": file_hash(source)})
    shutil.copyfile(obj, destination)


def strict_score(output, obj, destination):
    state = verify_frozen(output)
    scorer = load_score(output / "snapshot")
    scorer.ASM_DIR = output / "snapshot" / state["plan"]["targets"]
    try:
        scorer._elf(obj)
    except (Exception, SystemExit):
        write_json(destination, {"status": "invalid_elf"})
        return
    if state["plan"]["function"] not in scorer.symbols(obj):
        write_json(destination, {"status": "missing_symbol"})
        return
    comparison = scorer.compare(obj, state["plan"]["function"], show=0)
    value = {**asdict(comparison), "notes": list(getattr(comparison, "notes", ())), "accepted": comparison.accepted()}
    status = "strict_exact_candidate" if value["accepted"] else "unresolved_unverified" if value["unresolved"] or value["unverified"] else "scored_mismatch"
    if value["errors"]:
        status = "strict_comparison_failure"
    write_json(destination, {"status": status, "strict": value})
    receipt(output, destination.with_suffix(".receipt.json"), destination, {"object_sha256": file_hash(obj)})


def stage(output, mode, arguments, seconds, log):
    state = read_json(output / "frozen.json")
    return process([sys.executable, str(output / "snapshot/tools/cloud/hypothesis_batch.py"), mode,
                    str(output), *map(str, arguments)], output, state["envelope"]["environment"], seconds, output / log)


def score_object(output, slot, obj):
    destination = output / "scores" / (slot + ".json")
    result = stage(output, "_score", (obj, destination), read_json(output / "frozen.json")["plan"]["limits"]["score_seconds"], "logs/score-" + slot)
    if result["status"] != "ok":
        return {"status": "score_timeout" if result["status"] == "timeout" else "strict_comparison_failure", "score_seconds": result["seconds"]}
    try:
        value = read_json(destination)
        if value["status"] not in ("invalid_elf", "missing_symbol") and not valid_receipt(output, destination.with_suffix(".receipt.json")):
            raise ValueError("missing score receipt")
        return {**value, "score_seconds": result["seconds"]}
    except (ValueError, KeyError, OSError, TypeError):
        return {"status": "strict_comparison_failure", "score_seconds": result["seconds"]}


def target_object(output):
    state = verify_frozen(output)
    repo = output / "snapshot"
    scorer = load_score(repo)
    scorer.ASM_DIR = repo / state["plan"]["targets"]
    words = scorer.targets()[state["plan"]["function"]]
    scorer.image_symbols()
    scorer.own_data()  # fail closed on corrupt protected own-data artifacts
    function = state["plan"]["function"]
    assembly = '.set noreorder\n.text\n.globl ' + function + '\n.type ' + function + ', @function\n' + function + ':\n'
    assembly += "".join(f".word 0x{word:08x}\n" for word in words)
    assembly += f".size {function}, .-{function}\n"
    sys.path.insert(0, str(repo))
    from tools.conveyor.pipeline.targets import assemble_text
    obj = output / "target.o"
    assemble_text(assembly, obj)
    emitted = scorer.text_words(obj)
    if emitted[:len(words)] != words or any(emitted[len(words):]) or scorer.symbols(obj).get(function) != 0:
        raise ValueError("private target extent or assembler padding mismatch")
    receipt(output, output / "target.receipt.json", obj, {"words": len(words), "padding_words": len(emitted) - len(words)})


def baseline_controls(output):
    state = read_json(output / "frozen.json")
    plan = state["plan"]
    target = stage(output, "_target", (), plan["limits"]["score_seconds"], "logs/target")
    if target["status"] != "ok" or not valid_receipt(output, output / "target.receipt.json"):
        raise ValueError("protected target preparation failed; inspect private target log")
    controls = []
    for slot in ("baseline-1", "baseline-2"):
        obj = output / (slot + ".o")
        compiled = stage(output, "_compile", (output / "inputs" / plan["baseline"], obj, slot), plan["limits"]["compile_seconds"], "logs/" + slot)
        row = {"id": slot, "compile": compiled}
        if compiled["status"] == "ok" and valid_receipt(output, output / "compiles" / slot / "compile.receipt.json"):
            row.update(score_object(output, slot, obj), object_sha256=file_hash(obj), object=str(obj.relative_to(output)))
        else:
            row["status"] = "compile_timeout" if compiled["status"] == "timeout" else "compile_failure"
        controls.append(row)
        write_json(output / "controls.json", controls)
    if any(row.get("status") != "strict_exact_candidate" for row in controls) or controls[0].get("strict") != controls[1].get("strict"):
        raise ValueError("baseline must be repeatable canonical exact; batch not started")
    # A deliberately nonmatching tool canary is never an experimental variant.
    # Patch one private baseline ELF instruction, keeping valid relocation and
    # object structure, to prove the canonical score distinguishes a mismatch.
    scorer = load_score(output / "snapshot")
    original = output / "baseline-1.o"
    data, sections = scorer._elf(original)
    section = sections[scorer._text_index(sections)]
    altered = bytearray(data)
    offset = section["off"]
    altered[offset:offset + 4] = b"\x00\x00\x00\x00" if altered[offset:offset + 4] != b"\x00\x00\x00\x00" else b"\x24\x02\x00\x01"
    canary = output / "negative-canary.o"
    canary.write_bytes(altered)
    negative = score_object(output, "negative-canary", canary)
    write_json(output / "negative-canary.json", negative)
    if not negative.get("strict") or negative["strict"]["accepted"] or negative["strict"]["differing"] == 0:
        raise ValueError("negative canary did not produce an observed strict mismatch")
    return controls, negative


def campaign(output):
    state = verify_frozen(output)
    plan = state["plan"]
    wb_imports(output / "snapshot")
    from decomp_workbench.campaign import run_campaign, terminate_running_compilers
    from decomp_workbench import campaign as campaign_module
    # Workbench starts a group per compiler. Reap descendants even when the
    # wrapper exits on TERM; do not let its early-exit shortcut strand them.
    campaign_module.terminate_process_group = lambda child, **kw: kill_group(child, grace=0.1)
    from decomp_workbench.experiments import load_experiment
    def cancelled(signum, frame):
        terminate_running_compilers()
        raise SystemExit(128 + signum)
    signal.signal(signal.SIGTERM, cancelled)
    experiment = load_experiment(output / "inputs" / plan["experiment"])
    # Same bytes compile at the same production-relative filename. Every ID
    # remains in the final report even when its exact input is collapsed here.
    groups = {}
    for candidate_id, entry in sorted(plan["predictions"].items()):
        groups.setdefault(entry["sha256"], []).append(candidate_id)
    representatives = [ids[0] for ids in groups.values()]
    sources = [output / "inputs" / plan["predictions"][key]["source"] for key in representatives]
    template = shlex.join([sys.executable, str(output / "snapshot/tools/cloud/hypothesis_batch.py"),
                           "_campaign_compile", str(output), "{source}", "{output}"])
    results, _ = run_campaign(sources, target=output / "target.o", template=template,
        cache_dir=output / "cache", jobs=state["jobs"], objdump=str(output / "toolchain/bin/mips-linux-gnu-objdump"),
        symbol=plan["function"], environment=state["envelope"]["environment"], compile_cwd=output,
        keep_objects=output / "objects", stop_on_exact=False, timeout=plan["limits"]["compile_seconds"],
        artifact_dir=output / "workbench-artifacts", signal_specs=experiment.signals,
        selected_region=experiment.region, compilation_envelope={"rush": state["envelope_sha256"]})
    write_json(output / "campaign.json", {"results": [result.as_dict() for result in results], "source_groups": list(groups.values())})


def effect_checks(prediction, measured):
    by_id = {item["id"]: item for item in measured if "id" in item}
    ids = prediction["check"]["signal_ids"]
    return [by_id.get(key, {"id": key, "status": "UNKNOWN", "reason": "declared measurement unavailable"}) for key in ids] or [
        {"status": "UNKNOWN", "reason": "manual hypothesis; no supported measurement declared"}]


def collect_results(output):
    state = read_json(output / "frozen.json")
    plan = state["plan"]
    wb = read_json(output / "campaign.json") if (output / "campaign.json").is_file() else {"results": []}
    result_by_source = {(Path(row["source"]) if Path(row["source"]).is_absolute() else output / row["source"]).resolve(): row for row in wb["results"]}
    rows, groups = [], {}
    for candidate_id, entry in sorted(plan["predictions"].items()):
        groups.setdefault(entry["sha256"], []).append(candidate_id)
    def collect(ids):
        if _CANCELLED.is_set():
            raise KeyboardInterrupt("cancelled before score")
        key = ids[0]
        obj = output / "compiles" / key / "candidate.o"
        workbench = result_by_source.get((output / "inputs" / plan["predictions"][key]["source"]).resolve(), {})
        row = {"id": key, "source_sha256": plan["predictions"][key]["sha256"], "source_duplicate_ids": ids,
               "measured_signals": workbench.get("signals", [])}
        if valid_receipt(output, output / "compiles" / key / "compile.receipt.json"):
            row.update(score_object(output, key, obj), object_sha256=file_hash(obj), object=str(obj.relative_to(output)))
        else:
            status_file = output / "compiles" / key / "compile.status.json"
            row["status"] = read_json(status_file)["status"] if status_file.is_file() else "compile_timeout" if workbench.get("returncode") == 124 else "compile_failure"
        row["workbench_status"] = "available" if workbench.get("comparison") else "unavailable"
        return row
    with concurrent.futures.ThreadPoolExecutor(max_workers=state["jobs"]) as pool:
        for row in pool.map(collect, groups.values()):
            for candidate_id in row["source_duplicate_ids"]:
                rows.append({**{key: value for key, value in row.items() if key != "measured_signals"},
                             "id": candidate_id, "source_sha256": plan["predictions"][candidate_id]["sha256"],
                             "effect_checks": effect_checks(plan["predictions"][candidate_id], row["measured_signals"])})
    # Confirm byte equality after hashing: never deduplicate by .text or score.
    elf_groups = []
    for row in sorted(rows, key=lambda item: item["id"]):
        if "object" not in row:
            continue
        for group in elf_groups:
            other = group[0]
            if row["object_sha256"] == other["object_sha256"] and (output / row["object"]).read_bytes() == (output / other["object"]).read_bytes():
                group.append(row)
                break
        else:
            elf_groups.append([row])
    for index, group in enumerate(elf_groups, 1):
        for row in group:
            row["elf_group"] = index
            row["elf_duplicate_ids"] = [item["id"] for item in group]
    return sorted(rows, key=lambda item: item["id"])


def rank(row):
    strict = row.get("strict", {})
    return (not bool(strict) or bool(strict.get("errors") or strict.get("unresolved") or strict.get("unverified")),
            strict.get("differing", 10**9) + strict.get("extra_words", 10**9), row["id"])


def select_rows(rows):
    ordered = sorted(rows, key=rank)
    if not ordered:
        return []
    selected = [ordered[0]]
    distinct = next((row for row in ordered[1:] if row.get("elf_group") != selected[0].get("elf_group") and row.get("strict")), None)
    if distinct:
        selected.append(distinct)
    confirmed = next((row for row in ordered if any(check.get("status") == "PASS" for check in row["effect_checks"])), None)
    if confirmed:
        selected.append(confirmed)
    selected.append(ordered[-1])
    selected.append(min(ordered, key=lambda row: (-len(row.get("elf_duplicate_ids", [])), row["id"])))
    seen = set()
    return [row for row in selected if not (row["id"] in seen or seen.add(row["id"]))][:5]


def diagnose_rows(output, rows):
    state = read_json(output / "frozen.json")
    selected = [{"id": "baseline", "object": "baseline-1.o"}, *select_rows(rows)]
    seen = set()
    for row in selected:
        if "object" not in row:
            continue
        obj = output / row["object"]
        sha = file_hash(obj)
        if sha in seen:
            continue
        seen.add(sha)
        prefix = output / "diagnosis" / row["id"]
        result = process([sys.executable, str(output / "snapshot/tools/workbench.py"), "diagnose", str(output / "target.o"),
                          str(obj), "--function", state["plan"]["function"], "--objdump", str(output / "toolchain/bin/mips-linux-gnu-objdump"), "--json"],
                         output, state["envelope"]["environment"], state["plan"]["limits"]["diagnose_seconds"], prefix)
        diagnosis = {"status": "timeout" if result["status"] == "timeout" else "invalid_json", "seconds": result["seconds"]}
        try:
            doc = read_json(prefix.with_suffix(".stdout"))
            if not isinstance(doc["comparison"]["word_mismatches"], int) or not isinstance(doc["view"]["verdict"], str):
                raise ValueError("invalid diagnosis schema")
            if result["status"] != "timeout":
                diagnosis.update(status="available", classification=doc["view"]["verdict"], ownership_basis="heuristic")
        except (OSError, ValueError, KeyError, TypeError):
            pass
        for item in rows:
            if item.get("object_sha256") == sha:
                item["diagnosis"] = diagnosis
    for row in rows:
        row.setdefault("diagnosis", {"status": "not_selected"})


def report(output, summary=None):
    summary = summary or read_json(output / "summary.json")
    rows = summary.get("variants", [])
    public = {key: summary[key] for key in ("schema", "status", "base_commit", "plan_sha256", "envelope_sha256", "jobs", "counts", "timings") if key in summary}
    public["baseline"] = [{"status": row["status"], "object_sha256": row.get("object_sha256"),
                           "differing": row.get("strict", {}).get("differing"), "extra_words": row.get("strict", {}).get("extra_words")} for row in summary.get("baseline", [])]
    public["variants"] = [{"id": row["id"], "status": row["status"], "source_sha256": row["source_sha256"],
                            "object_sha256": row.get("object_sha256"), "elf_group": row.get("elf_group"),
                            "differing": row.get("strict", {}).get("differing"), "extra_words": row.get("strict", {}).get("extra_words"),
                            "unresolved_count": len(row.get("strict", {}).get("unresolved", [])),
                            "unverified_count": len(row.get("strict", {}).get("unverified", [])),
                            "errors_count": len(row.get("strict", {}).get("errors", []))} for row in rows]
    write_json(output / "public-summary.json", public)
    text = ["# Rush hypothesis calibration", "", f"Status: {summary['status']}. {len(rows)} variants recorded. No source was adopted; strict exact candidates require independent checker and image/ROM gates.", "",
            "Ranking uses differing + extra words, with unresolved/unverified/error results blocked. Equal scores do not establish identical programs.", "",
            "| Candidate | Status | Differing | Extra | ELF group |", "|---|---|---:|---:|---|"]
    baseline = summary.get("baseline", [])
    if baseline:
        b = baseline[0]
        text.append(f"| Baseline | {b['status']} | {b.get('strict', {}).get('differing', '?')} | {b.get('strict', {}).get('extra_words', '?')} | control |")
    for row in select_rows(rows):
        strict = row.get("strict", {})
        text.append(f"| {row['id']} | {row['status']} | {strict.get('differing', '?')} | {strict.get('extra_words', '?')} | {row.get('elf_group', '?')} |")
    text += ["", "Expected-effect observations are diagnostic only. Missing measurements remain UNKNOWN. LLM token/active-time accounting and useful-finding counts require human review; no speedup claim."]
    (output / "summary.md").write_text("\n".join(text) + "\n")
    return public


def run(plan_path, output, jobs=2, prepare_only=False):
    output = Path(output).resolve()
    plan, _, _ = validate(plan_path)
    if type(jobs) is not int or not 1 <= jobs <= plan["limits"]["jobs"]:
        raise ValueError("jobs exceeds declared limit")
    started = time.monotonic()
    state = freeze(plan_path, output, jobs)
    summary = {"schema": SCHEMA, "status": "preparing", "base_commit": plan["base_commit"],
               "plan_sha256": state["plan_sha256"], "envelope_sha256": state["envelope_sha256"], "jobs": jobs,
               "flags": state["envelope"]["effective_flags"], "baseline": [], "variants": [], "counts": {}, "timings": {"llm_active_seconds": None, "llm_tokens": None},
               "useful_findings": None, "useful_finding_definition": "Reproducible predicted-effect result, falsified hypothesis, or justified distinct code-generation outcome changing the next decision; duplicates count once."}
    try:
        controls, negative = baseline_controls(output)
        summary.update(baseline=controls, negative_canary=negative)
        if prepare_only:
            summary["status"] = "ready_controls_only"
        else:
            deadline = math.ceil(len(plan["predictions"]) / jobs) * (plan["limits"]["compile_seconds"] + plan["limits"]["diagnose_seconds"]) + 30
            result = stage(output, "_campaign", (), deadline, "logs/campaign")
            summary["timings"]["campaign_seconds"] = result["seconds"]
            rows = collect_results(output)
            diagnose_rows(output, rows)
            summary.update(variants=rows, status="completed" if result["status"] == "ok" else "campaign_failed")
        verify_frozen(output)
    except (ValueError, OSError, subprocess.SubprocessError, SystemExit) as exc:
        summary["status"] = "blocked"
        summary["blocker"] = str(exc)
        if (output / "controls.json").is_file():
            summary["baseline"] = read_json(output / "controls.json")
    summary["counts"] = {"variants": len(summary["variants"]), "strict_exact": sum(row["status"] == "strict_exact_candidate" for row in summary["variants"]),
                         "unique_elf_groups": len({row["elf_group"] for row in summary["variants"] if "elf_group" in row})}
    summary["timings"]["wall_seconds"] = time.monotonic() - started
    write_json(output / "summary.json", summary)
    report(output, summary)
    return summary


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "prepare", "run", "report", "_compile", "_campaign_compile", "_score", "_target", "_campaign"))
    parser.add_argument("path", type=Path)
    parser.add_argument("args", nargs="*")
    parser.add_argument("--jobs", type=int, default=2)
    parser.add_argument("--out", type=Path)
    args = parser.parse_args()
    if args.command in ("prepare", "run"):
        def cancelled(signum, frame):
            # Signal handlers only set state: every active process() polls it,
            # cleans its group, and queued scoring jobs stop before spawning.
            # This also closes the Popen-before-registry signal race.
            _CANCELLED.set()
        signal.signal(signal.SIGTERM, cancelled)
        signal.signal(signal.SIGINT, cancelled)
    try:
        if args.command == "validate":
            plan, _, _ = validate(args.path)
            print(json.dumps({"status": "valid", "variants": len(plan["predictions"]), "base_commit": plan["base_commit"]}))
        elif args.command in ("prepare", "run"):
            if not args.out:
                parser.error("--out requires a new private run directory")
            summary = run(args.path, args.out, args.jobs, args.command == "prepare")
            print(json.dumps({"status": summary["status"], "counts": summary["counts"], "report": str(args.out / "summary.md")}))
            return int(summary["status"] not in ("completed", "ready_controls_only"))
        elif args.command == "report":
            report(args.path.resolve())
            print((args.path / "summary.md").read_text())
        elif args.command in ("_compile", "_campaign_compile"):
            source, destination = map(Path, args.args[:2])
            state = read_json(args.path / "frozen.json")
            slot = args.args[2] if args.command == "_compile" else next(key for key, entry in state["plan"]["predictions"].items() if args.path / "inputs" / entry["source"] == source)
            bridge(args.path, source, destination, slot)
        elif args.command == "_score":
            strict_score(args.path, *map(Path, args.args))
        elif args.command == "_target":
            target_object(args.path)
        elif args.command == "_campaign":
            campaign(args.path)
    except KeyboardInterrupt:
        print("hypothesis batch: cancelled; active process groups terminated", file=sys.stderr)
        return 130
    except (ValueError, OSError, subprocess.SubprocessError) as exc:
        print(f"hypothesis batch: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
