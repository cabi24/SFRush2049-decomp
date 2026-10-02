#!/usr/bin/env python3
"""Fresh, read-only coordinator replay of a frozen game single on an IDO host.

python3 tools/cloud/review_single.py SOURCE FUNCTION --expected-bytes N --output proof.json

Flags come only from the exact publication header. A successful object proof
still needs independent source review, the image gate and full-ROM gates before
publication. This tool never changes targets, locks, source TUs or coverage.
"""
import argparse
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import re
import sys
import tempfile

if __package__:
    from . import score
else:
    import score


def review(source, function, expected_bytes):
    source = Path(source).resolve()
    data = source.read_bytes()
    header = data.split(b"\n", 1)[0].decode("utf-8")
    match = re.fullmatch(r"/\* flags: ([^\r\n]+) \*/", header)
    if not match:
        raise ValueError("source needs an exact /* flags: ... */ first line")
    flags = match[1]
    if not flags.strip() or flags != flags.strip():
        raise ValueError("flags must be nonempty with no surrounding whitespace")
    if expected_bytes <= 0 or expected_bytes % 4:
        raise ValueError("expected byte count must be positive and word aligned")
    targets = score.targets()  # Includes the protected manifest integrity gate.
    if function not in targets:
        raise ValueError("no canonical target section for " + function)
    if len(targets[function]) * 4 != expected_bytes:
        raise ValueError("requested extent differs from the canonical target")
    target_fingerprint = hashlib.sha256(
        (score.ASM_DIR / "SHA256SUMS").read_bytes()).hexdigest()
    with tempfile.TemporaryDirectory(prefix="coordinator-review-") as tmp:
        obj = Path(tmp) / "fresh.o"
        score.compile_single(source, flags, obj)
        if source.read_bytes() != data:
            raise ValueError("source changed during its review")
        comparison = score.compare(obj, function, show=0)
        if hashlib.sha256((score.ASM_DIR / "SHA256SUMS").read_bytes()).hexdigest() != target_fingerprint:
            raise ValueError("target manifest changed during its review")
        if comparison.total * 4 != expected_bytes:
            raise ValueError("comparison extent differs from the requested extent")
        return {
            "function": function,
            "source_sha256": hashlib.sha256(data).hexdigest(),
            "flags": flags,
            "target_manifest_sha256": target_fingerprint,
            "expected_bytes": expected_bytes,
            "canonical_verdict": comparison.summary(),
            "object_accepted": comparison.accepted(),
            "linked": asdict(comparison),
        }


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("function")
    parser.add_argument("--expected-bytes", type=int, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args(argv)
    try:
        proof = review(args.source, args.function, args.expected_bytes)
    except (OSError, ValueError, SystemExit) as exc:
        proof = {"function": args.function, "object_accepted": False,
                 "error": str(exc)}
    # Also replace a prior success with explicit failure evidence on rejection.
    args.output.write_text(json.dumps(proof, indent=2) + "\n")
    print(args.function + ": " + proof.get("canonical_verdict", proof.get("error", "")))
    return 0 if proof["object_accepted"] else 1


if __name__ == "__main__":
    sys.exit(main())
