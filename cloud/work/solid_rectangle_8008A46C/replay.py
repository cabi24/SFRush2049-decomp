#!/usr/bin/env python3
"""Fresh strict compiler receipt; writes only the explicitly supplied output."""
import argparse
import contextlib
import dataclasses
import hashlib
import io
import json
from pathlib import Path
import sys
import struct
import tempfile

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools/cloud"))
import score

def replay():
    source = Path(__file__).with_name("candidate.c")
    with tempfile.TemporaryDirectory(prefix="solid-rectangle-") as tmp:
        obj = Path(tmp) / "candidate.o"
        score.compile_single(source, score.DEFAULT_FLAGS, obj)
        with contextlib.redirect_stdout(io.StringIO()):
            result = score.compare(obj, "func_8008A46C", show=0)
        return {
            "status": "MATCH" if result.accepted() else "NONMATCH",
            "native_bytes": len(score.targets()["func_8008A46C"])*4,
            "candidate_text_bytes": len(score.text_words(obj))*4,
            "comparison": dataclasses.asdict(result),
            "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
            "object_sha256": hashlib.sha256(obj.read_bytes()).hexdigest(),
            "candidate_text_sha256": hashlib.sha256(b"".join(struct.pack(">I", word) for word in score.text_words(obj))).hexdigest(),
            "compiler_sha256": hashlib.sha256((score.IDO/"cc").read_bytes()).hexdigest(),
            "flags": score.DEFAULT_FLAGS + " " + score.R4300_CC,
            "native_manifest_sha256": hashlib.sha256((ROOT/"asm/us/blob/SHA256SUMS").read_bytes()).hexdigest(),
            "claims": [],
        }

if __name__ == "__main__":
    parser=argparse.ArgumentParser()
    parser.add_argument("--output", type=Path)
    args=parser.parse_args()
    text=json.dumps(replay(), indent=2)+"\n"
    if args.output: args.output.write_text(text)
    else: print(text, end="")
