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
# cluster-data-refs.md. Keep this intentionally small; unknown data references
# must remain numeric so raw-word target scoring stays correct.
GAME_SYMBOLS = {
    0x801146EC: "gstate",            # game_loop: byte R/W state dispatch
    0x80142AFC: "frame_counter",     # game_loop: word init/read/increment
    0x801146E8: "game_state_flags",  # game_loop: adjacent word R/W
    0x801174B4: "state_word_a",      # game_loop et al.: word R/W
    0x801174B8: "state_word_b",      # game_loop/playgame: word R/W
    0x8014A118: "input_rec0",        # process_inputs: input-record base
    0x8014A164: "input_rec1",        # process_inputs: second record base
    0x80152818: "player_array",      # gameplay pad: indexed player base
    0x801497C8: "msgq_ptr",          # game mode/attract: pointer dereference
}

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


def _is_branch(mnemonic):
    return mnemonic.startswith("b") and mnemonic not in {"break", "bal"}


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

    # Symbolize only a proven lui + signed-low access through the same base.
    pending_lui = {}
    for insn in instructions:
        mnemonic = insn["mnemonic"]
        operands = insn["operands"]
        if mnemonic == "lui":
            pieces = [part.strip() for part in operands.split(",")]
            if len(pieces) == 2:
                try:
                    pending_lui[pieces[0].lstrip("$")] = (
                        _integer(pieces[1]) & 0xFFFF, insn)
                except ValueError:
                    pass
            continue
        low = _LOW_RE.match(operands)
        if low:
            base = low.group(3).lstrip("$")
            prior = pending_lui.get(base)
            if prior:
                low_value = _integer(low.group(2))
                if low_value & 0x8000:
                    low_value -= 0x10000
                address = (prior[0] << 16) + low_value
                symbol = GAME_SYMBOLS.get(address)
                if symbol:
                    prior[1]["operands"] = (
                        prior[1]["operands"].split(",", 1)[0]
                        + f",%hi({symbol})"
                    )
                    prefix = low.group(1) or ""
                    insn["operands"] = (
                        f"{prefix}%lo({symbol})({low.group(3)})"
                    )

    lines = [f"glabel {target_id}"]
    for insn in instructions:
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
        lines.append(f".L{insn['address']:08X}:")
        lines.append((f"    {mnemonic:<7}{operands}").rstrip())
    return "\n".join(lines) + "\n"


def derive(conn, target_id, image_path=GAME_CODE_BIN, cache_dir=CACHE_DIR,
           objdump="mips-linux-gnu-objdump"):
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
    }
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
