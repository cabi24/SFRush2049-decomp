"""Deterministic game-code blob to mips_to_c assembly derivation."""
import hashlib
import json
import re
import subprocess
import tempfile
from pathlib import Path


REPO = Path(__file__).resolve().parents[3]
GAME_CODE_BIN = REPO / "build" / "game_code.bin"
CACHE_DIR = REPO / "build" / "m2c_asm"
GAME_CODE_BASE = 0x80086A50

# Evidence for every entry: specs/005-game-context-bootstrap/research/
# cluster-data-refs.md.  The second 2026-07-17 gate requires complete coverage
# of that survey; placeholder names deliberately carry no semantic judgment.
_SURVEYED_PLACEHOLDERS = (
    0x8002AFB4, 0x8002AFB8, 0x8002AFC0, 0x8002AFC4, 0x8002EB70,
    0x8002EBB0, 0x80035470, 0x80035471, 0x80035472, 0x80111958,
    0x80114650, 0x80114654, 0x801146F0, 0x801146F8, 0x801170FC,
    0x80117350, 0x80117354, 0x801174BC, 0x8011ED0B, 0x80123FB4,
    0x80123FB8, 0x80123FBC, 0x801242A8, 0x80124F84, 0x80124FC8,
    0x8012E67C, 0x8012E6E0, 0x8013FECB, 0x80140008, 0x80140618,
    0x801406B8, 0x801407BC, 0x80140804, 0x80140A00, 0x80140AD8,
    0x80140B08, 0x80140BD8, 0x80140C26, 0x80140D70, 0x80141428,
    0x80142510, 0x80142690, 0x80142699, 0x80142760, 0x80143F10,
    0x80143FD8, 0x8014401C, 0x80146108, 0x801461F8, 0x80146204,
    0x80146205, 0x80149414, 0x80149438, 0x80149774, 0x80149794,
    0x801497C4, 0x801497F4, 0x80149D98, 0x8014A160, 0x8014A250,
    0x8014B240, 0x80150000, 0x80150EFC, 0x80150F14, 0x80151AD0,
    0x80151AD8, 0x8015204C, 0x801520C4, 0x8015256C, 0x801525F4,
    0x80152734, 0x80152744, 0x80152F29, 0x80153308, 0x801543CC,
    0x8015698C, 0x80156994, 0x80156CF0, 0x80157244, 0x8015B250,
    0x8015B260, 0x8015F72D, 0x8015F738, 0x80161380, 0x80161398,
    0x801613A4, 0x801613AC, 0x801613B0, 0x80161434, 0x8017A4B0,
    0x8017A508, 0x8017A638,
)
GAME_SYMBOLS = {address: f"D_{address:08X}"
                for address in _SURVEYED_PLACEHOLDERS}
GAME_SYMBOLS.update({
    0x801146EC: "gstate",            # game_loop: byte R/W state dispatch
    0x80142AFC: "frame_counter",     # game_loop: word init/read/increment
    0x801146E8: "game_state_flags",  # game_loop: adjacent word R/W
    0x801174B4: "state_word_a",      # game_loop et al.: word R/W
    0x801174B8: "state_word_b",      # game_loop/playgame: word R/W
    0x8014A118: "input_rec0",        # process_inputs: input-record base
    0x8014A164: "input_rec1",        # process_inputs: second record base
    0x80152818: "player_array",      # gameplay pad: indexed player base
    0x801497C8: "msgq_ptr",          # game mode/attract: pointer dereference
    0x80114658: "playgame_settings", # playgame_state_change: bytes +0x39..+0x3b
    0x8017A4E0: "countdown_state",   # countdown_handler: pointer fields +0x4/+0xc/+0x10
    0x8017A4E4: "countdown_object",  # countdown_handler: pointer, fields +0x1f0..+0x200
    0x80140BF0: "pad_config",        # Input_ProcessGameplayPad: config base/pointer
    0x8002EB64: "game_loop_tick",    # game_loop: word R (base 0x8002e8e8 + 0x27c)
    0x8014A108: "active_player_count", # process/countdown/playgame: halfword R/W
    0x8014A110: "gameplay_mode",     # countdown/playgame: word R
    0x80146108: "D_80146108",        # playgame: byte family +0xc..+0xe
    0x80143FD8: "D_80143FD8",        # race state: pointer fields +0,+2,+0x60,+0x64
    0x8014A250: "D_8014A250",        # countdown: fields +0x7c6,+0x7e8
    # Third-amendment additions (direct derived-asm citation, T019 residue
    # close-out): register-formed array bases indexed by a variable (sll+addu),
    # not the fixed lui+addiu constant the original survey pass covered, so
    # they never appeared in cluster-data-refs.md. Evidence per entry below.
    0x80156978: "D_80156978",  # process_inputs.s .L800C99A0/.L800C99B0
                                # (lui $t2,0x8015 / addiu $t2,$t2,27000); indexed
                                # by sll $v0,$a2,2 (player*4) then lw 0($t7)
    0x80156998: "D_80156998",  # process_inputs.s .L800C999C/.L800C99B4
                                # (lui $t3,0x8015 / addiu $t3,$t3,27032); same
                                # index pattern as D_80156978
    0x80143A00: "D_80143A00",  # process_inputs.s .L800C9998/.L800C99B8
                                # (lui $t4,0x8014 / addiu $t4,$t4,14848); same
                                # index pattern as D_80156978
    0x80156958: "D_80156958",  # process_inputs.s .L800C9994/.L800C99BC
                                # (lui $t5,0x8015 / addiu $t5,$t5,26968); indexed
                                # by sll $t9,$a2,3 (player*8), read as two
                                # adjacent floats (lwc1 0/4)
    0x80156BE0: "D_80156BE0",  # attract_or_transition.s .L800EDDD4/.L800EDDD8
                                # (lui $t9,0x8015 / addiu $t9,$t9,27616); indexed
                                # by sll $t8,$t7,7 (D_8015F72D*128) then
                                # addu $t2,$t8,$t9; fields read at +0x58
                                # (.L800EDDE4) and +0x7C (.L800EE004)
    0x8011ED0C: "D_8011ED0C",  # Input_ProcessGameplayPad.s .L800A0A74/
                                # .L800A0A7C/.L800A0A88 (lui $s3,0x8012 /
                                # addu $s3,$s3,$t7 / lhu $s3,-4852($s3)):
                                # u16 array indexed at runtime, adjacent to
                                # surveyed byte D_8011ED0B
    0x80138670: "D_80138670",  # Input_ProcessGameplayPad.s .L800A0CAC/.L800A0CB0
                                # (lui $t9,0x8014 / addiu $t9,$t9,-31120);
                                # indexed by sra $t7,$t6,0xa / sll $t8,$t7,3
                                # then addu $v1,$t8,$t9; fields read at +0x0
                                # (.L800A0CD0) and +0x4 (.L800A0CC0)
})

_LINE_RE = re.compile(
    r"^\s*([0-9a-fA-F]+):\s+(?:[0-9a-fA-F]{8}\s+)+([.a-zA-Z0-9_]+)"
    r"(?:\s+(.*?))?\s*$"
)
_GPRS = (
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "s8", "fp", "ra",
)
_GPR_RE = re.compile(r"(?<![$\w])(" + "|".join(_GPRS) + r")(?!\w)")
_NUMERIC_RE = re.compile(r"^(?:0x)?([0-9a-fA-F]{7,8})$")
_LOW_RE = re.compile(
    r"^(.*?,)?\s*(-?(?:0x[0-9a-fA-F]+|\d+))\((\$?[a-z0-9]+)\)$"
)
_ADDIU_RE = re.compile(
    r"^\s*(\$?[a-z0-9]+)\s*,\s*(\$?[a-z0-9]+)\s*,\s*"
    r"(-?(?:0x[0-9a-fA-F]+|\d+))\s*$"
)
_ADDU_RE = re.compile(
    r"^\s*(\$?[a-z0-9]+)\s*,\s*(\$?[a-z0-9]+)\s*,\s*(\$?[a-z0-9]+)\s*$"
)

# Bumped whenever normalize_objdump's emission changes; part of the cache key
# so logic changes regenerate cached derivations (the symbol-table sha alone
# does not cover code changes).
DERIVATION_VERSION = 2


class DisassemblyError(RuntimeError):
    """The requested target cannot be derived into assembly."""


def symbol_table_sha():
    encoded = json.dumps(sorted(GAME_SYMBOLS.items()), separators=(",", ":"))
    return hashlib.sha256(encoded.encode()).hexdigest()


def _sha256(path):
    digest = hashlib.sha256()
    with Path(path).open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _integer(text):
    text = text.strip()
    if text.startswith("-0x"):
        return -int(text[3:], 16)
    return int(text, 0)


def _numeric_target(operand):
    match = _NUMERIC_RE.match(operand.strip())
    return int(match.group(1), 16) if match else None


def _signed_imm16(text):
    value = _integer(text)
    if value >= 0 and value & 0x8000:
        value -= 0x10000
    return value


def _is_branch(mnemonic):
    return mnemonic.startswith("b") and mnemonic != "break"


def _written_gpr(mnemonic, operands):
    """Return the conventional destination GPR, if this instruction has one."""
    if mnemonic in {"jal", "bal"}:
        return "ra"
    if mnemonic == "jalr":
        pieces = [part.strip().lstrip("$") for part in operands.split(",")]
        return pieces[0] if len(pieces) > 1 else "ra"
    if (not operands or _is_branch(mnemonic) or mnemonic in {"j", "jr"}
            or mnemonic.startswith(("mt", "s."))
            or mnemonic in {"sb", "sh", "sw", "sd", "swl", "swr",
                            "mult", "multu", "div", "divu", "break",
                            "syscall", "cache", "pref", "nop"}):
        return None
    destination = operands.split(",", 1)[0].strip().lstrip("$")
    return destination if destination in _GPRS else None


def normalize_objdump(output, target_id, targets):
    """Normalize GNU objdump text to the dry-run-validated m2c format."""
    instructions = []
    for line in output.splitlines():
        match = _LINE_RE.match(line)
        if match:
            instructions.append({
                "address": int(match.group(1), 16),
                "mnemonic": match.group(2),
                "operands": (match.group(3) or "").strip(),
            })

    # Track address formation and annotate every low-half consumer.  m2c binds
    # a low half to the most recent LUI for that register, so consumers of one
    # numeric LUI may need private synthetic LUIs when their bindings differ.
    pending_lui = {}
    lineages = []
    for insn in instructions:
        mnemonic = insn["mnemonic"]
        operands = insn["operands"]
        if mnemonic == "lui":
            pieces = [part.strip() for part in operands.split(",")]
            if len(pieces) == 2:
                register = pieces[0].lstrip("$")
                pending_lui.pop(register, None)
                try:
                    lineage = {"lui": insn, "register": register,
                               "consumers": []}
                    lineages.append(lineage)
                    pending_lui[register] = {
                        "value": (_integer(pieces[1]) & 0xFFFF) << 16,
                        "lineage": lineage, "formed": False,
                        "formed_symbol": None,
                    }
                except ValueError:
                    pass
            continue

        addiu = _ADDIU_RE.match(operands) if mnemonic == "addiu" else None
        if addiu:
            destination = addiu.group(1).lstrip("$")
            source = addiu.group(2).lstrip("$")
            prior = pending_lui.get(source)
            # addiu writes its destination even when its source is untracked.
            pending_lui.pop(destination, None)
            if prior:
                value = prior["value"] + _signed_imm16(addiu.group(3))
                symbol = None
                if not prior["formed"]:
                    symbol = GAME_SYMBOLS.get(value)
                    replacement = None
                    if symbol:
                        replacement = (
                            f"{addiu.group(1)},{addiu.group(2)},%lo({symbol})"
                        )
                    prior["lineage"]["consumers"].append(
                        (insn, source, symbol, replacement)
                    )
                pending_lui[destination] = {
                    "value": value, "lineage": prior["lineage"],
                    "formed": True, "formed_symbol": symbol,
                }
            continue

        addu = _ADDU_RE.match(operands) if mnemonic == "addu" else None
        if addu:
            destination = addu.group(1).lstrip("$")
            left, right = (addu.group(2).lstrip("$"), addu.group(3).lstrip("$"))
            tracked = [pending_lui.get(reg) for reg in (left, right)]
            # Contract §5 idiom (c): exactly one raw-page lui operand
            # propagates through runtime indexing; a formed pointer or an
            # ambiguous pair does not.
            live = [(reg, state) for reg, state in zip((left, right), tracked)
                    if state and not state["formed"]]
            pending_lui.pop(destination, None)
            if len(live) == 1:
                base_reg, state = live[0]
                pending_lui[destination] = {
                    "value": state["value"], "lineage": state["lineage"],
                    "formed": False, "formed_symbol": None,
                    "indexed_via": (insn, base_reg),
                }
            continue

        low = _LOW_RE.match(operands)
        if low:
            base = low.group(3).lstrip("$")
            prior = pending_lui.get(base)
            if prior:
                low_value = _signed_imm16(low.group(2))
                address = prior["value"] + low_value
                symbol = GAME_SYMBOLS.get(address)
                if prior["formed"] and symbol == prior["formed_symbol"]:
                    symbol = None
                replacement = None
                if symbol:
                    prefix = low.group(1) or ""
                    replacement = f"{prefix}%lo({symbol})({low.group(3)})"
                indexed_via = prior.get("indexed_via")
                if indexed_via and symbol:
                    # Idiom (c): the binding must hold at the addu, on the
                    # lui's register — never at the access, whose base holds
                    # the formed pointer. The access offset still rewrites.
                    addu_insn, lui_reg = indexed_via
                    prior["lineage"]["consumers"].append(
                        (addu_insn, lui_reg, symbol, None)
                    )
                    insn["operands"] = replacement
                elif not prior["formed"] or symbol:
                    prior["lineage"]["consumers"].append(
                        (insn, base, symbol, replacement)
                    )

        written = _written_gpr(mnemonic, operands)
        if written:
            pending_lui.pop(written, None)

    for lineage in lineages:
        bindings = {}
        original_lui = lineage["lui"]
        original_immediate = original_lui["operands"].split(",", 1)[1]
        if lineage["consumers"]:
            first_binding = next(
                (item[2] for item in lineage["consumers"] if item[2]), None
            )
            if first_binding:
                original_lui["operands"] = (
                    original_lui["operands"].split(",", 1)[0]
                    + f",%hi({first_binding})"
                )
            bindings[lineage["register"]] = first_binding
        for consumer, register, symbol, replacement in lineage["consumers"]:
            binding = symbol  # None means the original numeric high half.
            if bindings.get(register) != binding:
                immediate = (f"%hi({symbol})" if symbol else original_immediate)
                consumer.setdefault("synthetic_before", []).append({
                    "mnemonic": "lui", "operands": f"{register},{immediate}",
                })
                bindings[register] = binding
            if replacement:
                consumer["operands"] = replacement

    lines = [f"glabel {target_id}"]
    for insn in instructions:
        lines.append(f".L{insn['address']:08X}:")
        for synthetic in insn.get("synthetic_before", []):
            operands = _GPR_RE.sub(r"$\1", synthetic["operands"])
            lines.append(f"    {synthetic['mnemonic']:<7}{operands}".rstrip())
        mnemonic = insn["mnemonic"]
        operands = insn["operands"]
        pieces = [part.strip() for part in operands.split(",")]
        if pieces and (_is_branch(mnemonic) or mnemonic in {"j", "jal"}):
            address = _numeric_target(pieces[-1])
            if address is not None:
                if _is_branch(mnemonic):
                    pieces[-1] = f".L{address:08X}"
                else:
                    pieces[-1] = targets.get(address, f"func_{address:08x}")
                operands = ",".join(pieces)
        operands = _GPR_RE.sub(r"$\1", operands)
        lines.append((f"    {mnemonic:<7}{operands}").rstrip())
    return "\n".join(lines) + "\n"


def derive(conn, target_id, image_path=GAME_CODE_BIN, cache_dir=CACHE_DIR,
           objdump="mips-linux-gnu-objdump", context_sha=None):
    """Return the cached assembly path for one extracted database target."""
    row = conn.execute(
        "SELECT address,insn_count,gate_reason FROM n64_target"
        " WHERE target_id=? AND population='extracted'", (target_id,),
    ).fetchone()
    if row is None:
        raise DisassemblyError(f"unknown extracted target: {target_id}")
    if row["gate_reason"] and row["gate_reason"].startswith("extent_conflict"):
        raise DisassemblyError(row["gate_reason"])
    if row["address"] is None or row["insn_count"] is None:
        raise DisassemblyError("missing extent")

    image_path = Path(image_path)
    if not image_path.is_file():
        raise DisassemblyError(f"missing image: {image_path}")
    extent = [row["address"], row["insn_count"]]
    key = {
        "extent": extent,
        "image_sha": _sha256(image_path),
        "symbol_table_sha": symbol_table_sha(),
        "derivation_version": DERIVATION_VERSION,
    }
    if context_sha is not None:
        key["context_sha"] = context_sha
    cache_dir = Path(cache_dir)
    cache_dir.mkdir(parents=True, exist_ok=True)
    asm_path = cache_dir / f"{target_id}.s"
    meta_path = cache_dir / f"{target_id}.json"
    try:
        if asm_path.is_file() and json.loads(meta_path.read_text()) == key:
            return asm_path
    except (FileNotFoundError, json.JSONDecodeError):
        pass

    start = row["address"]
    stop = start + 4 * row["insn_count"]
    command = [
        objdump, "-D", "-b", "binary", "-m", "mips:4300", "-EB",
        f"--adjust-vma=0x{GAME_CODE_BASE:08X}",
        "--start-address", f"0x{start:08X}",
        "--stop-address", f"0x{stop:08X}", str(image_path),
    ]
    proc = subprocess.run(command, capture_output=True, text=True)
    if proc.returncode != 0:
        detail = proc.stderr.strip().splitlines()
        raise DisassemblyError(detail[0] if detail else "objdump failed")
    target_rows = conn.execute(
        "SELECT target_id,address FROM n64_target WHERE address IS NOT NULL"
    ).fetchall()
    targets = {item["address"]: item["target_id"] for item in target_rows}
    normalized = normalize_objdump(proc.stdout, target_id, targets)
    if normalized == f"glabel {target_id}\n":
        raise DisassemblyError("objdump produced no instructions")

    with tempfile.NamedTemporaryFile("w", dir=cache_dir, delete=False) as tmp:
        tmp.write(normalized)
        tmp_asm = Path(tmp.name)
    tmp_asm.replace(asm_path)
    with tempfile.NamedTemporaryFile("w", dir=cache_dir, delete=False) as tmp:
        json.dump(key, tmp, sort_keys=True, separators=(",", ":"))
        tmp.write("\n")
        tmp_meta = Path(tmp.name)
    tmp_meta.replace(meta_path)
    return asm_path
