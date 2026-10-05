"""Read-only protected native replay; writes only this private packet."""
import hashlib
import json
import struct
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(REPO))
from tools.conveyor.pipeline import blob_group, blob_layout, blob_splice, targets
from tools.conveyor.jobs import scoring

packet = Path(__file__).parent
work = REPO / "build/C103"
layout = blob_layout.load()
slots = {e["target_id"]: e for r in layout["regions"] for e in r["entries"]
         if e["kind"] == "function"}
name = "func_800E23A4"
entry = slots[name]
image = (REPO / "build/game_code.bin").read_bytes()
base = 0x80086A50
original = image[entry["vaddr"] - base:entry["vaddr"] - base + entry["size"]]
gate = targets.gate_target([f"{v:08x}" for v in struct.unpack(">422I", original)],
                           work / "target.o")
results = []
for stem in ("baseline", "unsigned_boolean", "constant_airfact", "tire_phase_lifetime",
             "donor_index_declaration", "donor_gameoverdrag_declaration",
             "consumed_airfact_reuse", "donor_index_airfact_reuse"):
    source, obj = packet / (stem + ".c"), work / (stem + ".o")
    row = {"source": str(source.relative_to(REPO)),
           "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
           "object_sha256": hashlib.sha256(obj.read_bytes()).hexdigest()}
    symbols = subprocess.run(["mips-linux-gnu-nm", "-S", str(obj)],
                             check=True, capture_output=True, text=True).stdout
    symbol = next(line.split() for line in symbols.splitlines()
                  if line.split()[-1] == name)
    row["true_symbol_bytes"] = int(symbol[1], 16)
    row["canonical_unlinked_strict_including_stack"] = scoring.score(
        work / "target.o", obj, stack_differences=True)
    pool_path = work / (stem + ".rodata.bin")
    subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".rodata",
                    str(obj), str(pool_path)], check=True)
    pool = pool_path.read_bytes()
    native_pool = image[0x801243CC-base:0x801243D8-base]
    row["complete_12_byte_literal_pool_exact"] = pool[:12] == native_pool
    row["pool_meaningful_bytes"] = 12
    row["emitted_pool_bytes"] = len(pool)
    row["emitted_pool_alignment_tail_all_zero"] = not any(pool[12:])
    try:
        slices, ndx = blob_group.member_slices(obj, [name], slots)
        body = blob_group.relocate(obj, slices, ndx, blob_splice.image_symbols(layout),
                                   image=(image, base))[name]
        differences = [i for i in range(422)
                       if body[4*i:4*i+4] != original[4*i:4*i+4]]
        row.update(protected_pool_and_relocation_gate=True,
                   byte_count=len(body), word_differences=len(differences),
                   difference_word_indices=differences)
        row["native_frame_bytes"] = -struct.unpack(">h", body[2:4])[0]
        row["poortract_spill_offsets"] = [struct.unpack(">I", body[4*i:4*i+4])[0] & 0xFFFF
                                          for i in (179, 190)]
        (work / (stem + ".native.bin")).write_bytes(body)
    except Exception as error:
        row["refusal"] = str(error)
    results.append(row)
    print(json.dumps(row))
proof = {"original_roundtrip_gate": gate,
         "protocol": "Actual production member_slices and protected relocate; full native body and source-built pool validation; no masks or substitutions",
         "results": results}
(packet / "independent_proof.json").write_text(json.dumps(proof, indent=2) + "\n")
