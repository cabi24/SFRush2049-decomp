"""N64 target inventory: populate n64_target rows and produce per-function
target objects (.o) for the Scorer.

Sources:
- work/**/info.txt          — name, address, category, flags (1,319 functions)
- build/game_code.bin       — decompressed game code (RAM 0x80086A50+), the
                              752-function "extracted" population
- baserom.us.z64            — static population; ROM offset = vaddr - 0x7FFFF400

Target .o files are built by assembling the raw instruction words (`.word`)
so no disassembler round-trip can distort them; they land in the coordinator
blob store and n64_target.target_o_sha points at them.
"""
import hashlib
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
GAME_CODE_BASE = 0x80086A50
GAME_CODE_BIN = REPO / "build" / "game_code.bin"
BASEROM = REPO / "baserom.us.z64"
STATIC_ROM_DELTA = 0x7FFFF400  # vaddr - delta = ROM offset (verified: strlen)

_SIZE_RE = re.compile(r"\((\d+)\s*bytes\)")


def load_work_inventory(work_dir=None):
    """Parse every work/**/info.txt into {name, address, category, flags, size?}."""
    work_dir = Path(work_dir or REPO / "work")
    entries = []
    for info in sorted(work_dir.rglob("info.txt")):
        fields = {}
        for line in info.read_text().splitlines():
            if ":" in line:
                key, _, value = line.partition(":")
                fields[key.strip()] = value.strip()
        if "address" not in fields or "name" not in fields:
            continue
        size = None
        m = _SIZE_RE.search(fields.get("comment", ""))
        if m:
            size = int(m.group(1))
        entries.append(
            {
                "name": fields["name"],
                "address": int(fields["address"], 16),
                "category": fields.get("category", ""),
                "flags": fields.get("compiler_flags", ""),
                "size": size,
            }
        )
    # Dedup by address (first name wins), then infer missing sizes from the
    # gap to the next function.
    by_addr = {}
    for e in entries:
        by_addr.setdefault(e["address"], e)
    ordered = sorted(by_addr.values(), key=lambda e: e["address"])
    for i, e in enumerate(ordered):
        gap = ordered[i + 1]["address"] - e["address"] if i + 1 < len(ordered) else None
        if e["size"] is None:
            e["size"] = gap if gap is not None else 256
        elif gap is not None:
            e["size"] = min(e["size"], gap)
    return ordered


_image_cache = {}


def _image(path):
    if path not in _image_cache:
        _image_cache[path] = path.read_bytes()
    return _image_cache[path]


def function_words(address, size):
    """Raw big-endian instruction words for a function, from whichever image
    holds that address. Returns list of 8-hex-digit strings."""
    if GAME_CODE_BASE <= address < GAME_CODE_BASE + GAME_CODE_BIN.stat().st_size:
        data = _image(GAME_CODE_BIN)
        off = address - GAME_CODE_BASE
    else:
        data = _image(BASEROM)
        off = address - STATIC_ROM_DELTA
        if off < 0 or off >= len(data):
            raise ValueError(f"address {address:#x} outside known images")
    chunk = data[off : off + (size // 4) * 4]
    return [f"{w:08X}" for (w,) in struct.iter_unpack(">I", chunk)]


def assemble_words(words, out_o, func_name="func"):
    asm = [".set noreorder", ".text", f".globl {func_name}", f"{func_name}:"]
    asm += [f"    .word 0x{w}" for w in words]
    with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as f:
        f.write("\n".join(asm) + "\n")
        src = f.name
    subprocess.run(
        ["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-o", str(out_o), src],
        check=True,
    )


def populate(conn, store, work_dir=None, limit=None):
    """Fill n64_target rows and build target .o blobs. Returns summary dict."""
    from ..coordinator import db as dbmod

    inventory = load_work_inventory(work_dir)
    if limit:
        inventory = inventory[:limit]
    built, skipped = 0, 0
    with tempfile.TemporaryDirectory() as tmp:
        for e in inventory:
            population = (
                "extracted"
                if GAME_CODE_BASE <= e["address"] < GAME_CODE_BASE + 0x9E0A0
                else "static"
            )
            try:
                words = function_words(e["address"], e["size"])
                if not words:
                    raise ValueError("empty function body")
                out_o = Path(tmp) / "t.o"
                assemble_words(words, out_o, e["name"])
                o_sha = store.put_file(out_o)
                asm_sha = hashlib.sha256("\n".join(words).encode()).hexdigest()
            except (ValueError, subprocess.CalledProcessError) as exc:
                print(f"  skip {e['name']}: {exc}", file=sys.stderr)
                skipped += 1
                continue
            with dbmod.tx(conn):
                conn.execute(
                    "INSERT INTO n64_target (target_id, address, population,"
                    " insn_count, target_asm_sha, target_o_sha)"
                    " VALUES (?, ?, ?, ?, ?, ?)"
                    " ON CONFLICT(target_id) DO UPDATE SET address=excluded.address,"
                    " population=excluded.population, insn_count=excluded.insn_count,"
                    " target_asm_sha=excluded.target_asm_sha,"
                    " target_o_sha=excluded.target_o_sha",
                    (e["name"], e["address"], population, len(words), asm_sha, o_sha),
                )
                conn.execute(
                    "INSERT INTO function_status (target_id, status, updated_at)"
                    " VALUES (?, 'unmatched', strftime('%Y-%m-%dT%H:%M:%fZ','now'))"
                    " ON CONFLICT(target_id) DO NOTHING",
                    (e["name"],),
                )
                conn.execute(
                    "INSERT OR IGNORE INTO blob (sha256, kind, size_bytes, created_at)"
                    " VALUES (?, 'target', ?, strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
                    (o_sha, store.size(o_sha) or 0),
                )
            built += 1
    return {"built": built, "skipped": skipped, "total": len(inventory)}


def main():
    import argparse

    from ..coordinator import db as dbmod
    from ..coordinator.store import BlobStore

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(Path("~/.conveyor").expanduser()))
    parser.add_argument("--limit", type=int, default=None)
    args = parser.parse_args()
    conn = dbmod.connect(Path(args.data) / "conveyor.db")
    store = BlobStore(Path(args.data) / "blobs")
    summary = populate(conn, store, limit=args.limit)
    print(f"targets: {summary['built']} built, {summary['skipped']} skipped, "
          f"{summary['total']} in inventory")


if __name__ == "__main__":
    main()
