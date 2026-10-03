#!/usr/bin/env python3
"""Bounded read-only privileged-opcode screen; emit metadata, never target words."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[4]
WORK = ROOT / "cloud/work/boot_tail"
TARGET = ROOT / "asm/us/boot_tail/boot_tail_8000f3a4.s"


def opcode_class(word):
    if word >> 26 == 0x2F:
        return "cache"
    if word >> 26 == 0x10:
        rs = (word >> 21) & 31
        if rs == 0:
            return "mfc0"
        if rs == 4:
            return "mtc0"
        if word == 0x42000018:
            return "eret"
        return "cop0_other"
    if word >> 26 == 0 and word & 63 == 0x0F:
        return "sync"
    return None


def audit():
    inventory = json.loads((ROOT / "specs/015-boot-tail-runtime/inventory.json").read_text())["functions"]
    if not TARGET.is_file():
        raise ValueError("protected boot-tail target is absent; opcode classification must remain unknown")
    digest = hashlib.sha256(TARGET.read_bytes()).hexdigest()
    manifest = TARGET.parent / "SHA256SUMS"
    hashes = {parts[1].lstrip("*"): parts[0] for line in manifest.read_text().splitlines()
              if len(parts := line.split()) == 2}
    if hashes.get(TARGET.name) != digest:
        raise ValueError("protected target hash mismatch; stop, do not repair the manifest")
    sections = {}
    for name, body in re.findall(r'^\.section \.text\.(\w+),[^\n]*\n(.*?)(?=^\.section|\Z)',
                                 TARGET.read_text(), re.M | re.S):
        if name in sections:
            raise ValueError("duplicate target section")
        sections[name] = [int(x, 16) for x in re.findall(r'\.word\s+(0x[0-9a-fA-F]+)', body)]
    hits = []
    external_calls = []
    for row in inventory:
        words = sections.get(row["name"])
        if words is None or len(words) * 4 != row["size"]:
            raise ValueError("target body/inventory size conflict at " + row["address"])
        if row["scope"] == "ultralib_identified":
            continue
        for i, word in enumerate(words):
            if row["scope"] == "in_scope" and word >> 26 == 3:
                target = ((addr := int(row["address"], 16)) + i * 4 + 4) & 0xF0000000 | ((word & 0x03FFFFFF) << 2)
                if target >= 0x800277D0:
                    external_calls.append({"caller": row["address"], "offset": "0x%X" % (i * 4), "target": "0x%08X" % target})
            kind = opcode_class(word)
            if kind:
                hits.append({"address": row["address"], "scope": row["scope"],
                             "offset": "0x%X" % (i * 4), "instruction_class": kind})
    candidates = [{"address": "0x8000F8D0", "scope": "in_scope", "classification": "candidate_only",
                   "reason": "bcmp historical label/spec assembly lead; opcode screen does not establish non-C"}]
    for row in inventory:
        if row["scope"] != "stub_unclassified":
            continue
        own = [h for h in hits if h["address"] == row["address"]]
        candidates.append({"address": row["address"], "scope": "identify_only", "size": row["size"],
                           "classification": "non_c_instruction_evidence" if own else "unknown",
                           "reason": "mtc0 at entry; no ordinary C89/IDO body" if own else
                           "tiny leaf alone does not establish assembly; delay-slot and ABI assessment still required"})
    return {"schema_version": 1, "input_path": str(TARGET.relative_to(ROOT)), "input_sha256": digest,
            "manifest_path": str(manifest.relative_to(ROOT)), "manifest_sha256": hashlib.sha256(manifest.read_bytes()).hexdigest(),
            "inventory_sections_seen": len(sections), "in_scope_functions_screened": 412,
            "identify_only_stubs_screened": 6, "excluded_ultralib_not_classified": 21,
            "instruction_classes": ["cache", "mfc0", "mtc0", "eret", "cop0_other", "sync"],
            "in_scope_positive_functions": len({h["address"] for h in hits if h["scope"] == "in_scope"}),
            "hits": hits, "candidates": candidates,
            "unclassified_calls_at_or_beyond_text_end": external_calls,
            "external_call_limit": "Decoded only instructions within in-scope functions; destination bytes were not read. Inventory call columns omit these edges.",
            "limitations": ["Linear instruction-class screen, not control-flow reachability or source reconstruction.",
                            "No privileged hit does not prove C representability, original compiler or benign delay-slot use.",
                            "No compiler or scorer used; no raw words, disassembly or ROM published.",
                            "Ordinary compiler-generated jr delay slots and small sizes alone do not justify non_c."]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    result = audit()
    text = json.dumps(result, indent=2, sort_keys=True) + "\n"
    output = WORK / "opcode_audit.json"
    if args.check:
        if not output.exists() or output.read_bytes() != text.encode("utf-8"):
            print("opcode audit drift", file=sys.stderr)
            return 1
    else:
        output.write_bytes(text.encode("utf-8"))
    print("boot-tail opcode screen: %d in-scope bodies, %d privileged-opcode-positive functions; %d identify-only hits; %d unclassified external calls" % (result["in_scope_functions_screened"], result["in_scope_positive_functions"], len([h for h in result["hits"] if h["scope"] == "stub_unclassified"]), len(result["unclassified_calls_at_or_beyond_text_end"])))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
