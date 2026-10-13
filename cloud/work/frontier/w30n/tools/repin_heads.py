#!/usr/bin/env python3
"""w30n: re-pin the receipts that a head registration (heads.json) moves.

    python3 cloud/work/frontier/w30n/tools/repin_heads.py [--write]

Registering func_80104704 changes the asm/us/blob target table: one function
of 260 words replaces the 47-word suffix row highscore_entry_anim (retired by
`closure run`'s suffix rule). Receipts that summarise the WHOLE table move with
it. Each re-pin is proved before anything is written (dry run by default):

  census digests   (the w29n rebind.py CENSUS_PINS receipts) the digest of
                   heads_undo.undo(today's table) must equal the pinned value;
                   the digest of today's table is then pinned.
  slot-contract    dot_slot_contract_audit_20261006: audit.verify() over
  audit            undo(today's table) must accept the saved evidence; then the
                   evidence is regenerated (= `audit.py --write`) and the frozen
                   counts in tests/cloud/test_slot_contract_audit.py follow the
                   new summary (func_80104704 is a new slot_state_setup caller:
                   one call site at 0x80104724, s0 capture).
  source-boundary  dot_source_boundary_scout_20261005: its native_summary()
  scout            computed over undo(today's table) must equal the receipt
                   (stable_native, i.e. without the region-manifest provenance);
                   the top-level fields that differ today (word_count) are then
                   replaced by today's values.
  symbols.json     dot_nearest_point_donor_20261005 / dot_target_orientation_20261005
  pins             pin asm/us/blob/symbols.json: the file at --base must hash to the
                   pin and today's file must be exactly it plus the registered heads'
                   names (same addresses, nothing else moved); then today's hash.
  recomp census    tools/recomp/census/census.json: a retired label is not a
                   function; its entry (probed at the label, so its counts are passes
                   through the label, not calls) is removed in the generator's format.
                   The head is absent from the census (draft_check then runs every
                   script for it) until the next census refresh probes it.
Run after `blob_tu generate`/`symbols` on the registered layout.
"""
import argparse
import importlib.util
import json
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[4]
sys.path.insert(0, str(REPO))
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(REPO / "cloud/work/frontier/w29n/tools"))
import heads_undo  # noqa: E402
import rebind  # noqa: E402  (w29n: CENSUS_PINS, digests, blob_targets, replace_once)
from tools.cloud import score  # noqa: E402

FR = REPO / "cloud/work/frontier"
SCOUT = FR / "dot_source_boundary_scout_20261005"


def load(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def census(write, log):
    today = rebind.blob_targets()
    before = heads_undo.undo(today)
    for receipt, fn in rebind.CENSUS_PINS:
        rebind.replace_once(FR / receipt, fn(before), fn(today), write, log)


def scout(write, log):
    mod = load(SCOUT / "verify.py", "w30n_scout")
    path = SCOUT / "verification.json"
    receipt = json.loads(path.read_text())
    real = score.targets
    today = mod.score.targets()
    if mod.stable_native(mod.native_summary()) == mod.stable_native(receipt["native"]):
        log.append(f"{path.relative_to(REPO)}: already pinned")
        return
    try:
        mod.score.targets = lambda *a, **k: heads_undo.undo(today)
        before = mod.native_summary()
    finally:
        mod.score.targets = real
    if mod.stable_native(before) != mod.stable_native(receipt["native"]):
        raise SystemExit("refusing: scout summary of the undone table differs from the receipt")
    actual = mod.native_summary()
    changed = sorted(k for k, v in mod.stable_native(actual).items() if receipt["native"].get(k) != v)
    for k in changed:
        log.append(f"{path.relative_to(REPO)}: native.{k} {json.dumps(receipt['native'][k])[:60]}"
                   f" -> {json.dumps(actual[k])[:60]}")
        receipt["native"][k] = actual[k]
    if not changed:
        log.append(f"{path.relative_to(REPO)}: already pinned")
    if write and changed:
        path.write_text(json.dumps(receipt, indent=2, sort_keys=True) + "\n")


SLOT = FR / "dot_slot_contract_audit_20261006"
SLOT_TEST = REPO / "tests/cloud/test_slot_contract_audit.py"


def slot_audit(write, log):
    import re
    mod = load(SLOT / "audit.py", "w30n_slot_audit")
    real = mod.score.targets
    today = real()
    current = mod.collect()
    saved = json.loads((SLOT / "evidence.json").read_text())
    if current != saved:
        try:
            mod.score.targets = lambda *a, **k: heads_undo.undo(today)
            mod.verify()        # raises AuditError unless the undone table gives the saved evidence
        finally:
            mod.score.targets = real
    if current == saved:
        log.append(f"{(SLOT / 'evidence.json').relative_to(REPO)}: already pinned")
    else:
        log.append(f"{(SLOT / 'evidence.json').relative_to(REPO)}: regenerated (audit.py --write)")
        if write:
            (SLOT / "evidence.json").write_text(json.dumps(current, indent=2) + "\n")
    s = current["summary"]
    text = SLOT_TEST.read_text()
    new = re.sub(r"\(result\['callers'\], result\['sites'\], result\['adjacent_call_label_pairs'\]\) == \(\d+, \d+, \d+\)",
                 "(result['callers'], result['sites'], result['adjacent_call_label_pairs']) == (%d, %d, %d)"
                 % (s["callers"], s["sites"], s["adjacent_call_label_pairs"]), text)
    new = re.sub(r"sum\(result\['captures'\]\.values\(\)\) == \d+",
                 "sum(result['captures'].values()) == %d" % sum(s["captures"].values()), new)
    new = re.sub(r"result\['unaccepted_caller_bytes'\] == \d+",
                 "result['unaccepted_caller_bytes'] == %d" % s["unaccepted_caller_bytes"], new)
    if new != text:
        log.append(f"{SLOT_TEST.relative_to(REPO)}: frozen counts -> callers {s['callers']}, sites {s['sites']}, "
                   f"pairs {s['adjacent_call_label_pairs']}, captures {sum(s['captures'].values())}, "
                   f"unaccepted bytes {s['unaccepted_caller_bytes']}")
        if write:
            SLOT_TEST.write_text(new)


SYMBOL_PINS = [FR / "dot_nearest_point_donor_20261005/verification.json",
               FR / "dot_target_orientation_20261005/verification.json"]


def symbols_pins(base, write, log):
    import hashlib
    import subprocess
    rel = "asm/us/blob/symbols.json"
    old_raw = subprocess.run(["git", "show", f"{base}:{rel}"], cwd=REPO, capture_output=True, check=True).stdout
    new_raw = (REPO / rel).read_bytes()
    old, new = json.loads(old_raw), json.loads(new_raw)
    added = {h["name"]: "0x%08X" % int(h["vram"], 16) for h in heads_undo.heads()}
    want = dict(old, symbols=dict(old["symbols"], **added))
    if want != new:
        extra = sorted(set(new["symbols"].items()) ^ set(want["symbols"].items()))[:6]
        raise SystemExit(f"refusing: {rel} is not the {base} table plus the registered heads ({extra})")
    o, n = hashlib.sha256(old_raw).hexdigest(), hashlib.sha256(new_raw).hexdigest()
    for receipt in SYMBOL_PINS:
        rebind.replace_once(receipt, o, n, write, log)


def recomp_census(write, log):
    path = REPO / "tools/recomp/census/census.json"
    doc = json.loads(path.read_text())
    retired = [t["name"] for h in heads_undo.heads() for t in h["retires"] if t["name"] in doc["functions"]]
    if not retired:
        log.append(f"{path.relative_to(REPO)}: no retired label left")
        return
    for name in retired:
        del doc["functions"][name]
    log.append(f"{path.relative_to(REPO)}: removed retired label(s) {', '.join(retired)}")
    if write:
        path.write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--write", action="store_true")
    p.add_argument("--base", default="HEAD", help="revision before the registration (symbols.json pins)")
    a = p.parse_args()
    log = []
    symbols_pins(a.base, a.write, log)
    recomp_census(a.write, log)
    census(a.write, log)
    slot_audit(a.write, log)
    scout(a.write, log)
    for line in log:
        print(line)
    print("written" if a.write else "dry run: nothing written")


if __name__ == "__main__":
    main()
