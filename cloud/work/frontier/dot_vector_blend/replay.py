#!/usr/bin/env python3
"""Replay the bounded vector-blend research, without changing acceptance state.

Receipts contain hashes, sizes and residual offsets, never native instruction
words. Compiled objects and extracted target diagnostics belong only in build/.
"""
import argparse
from contextlib import redirect_stdout
from dataclasses import asdict
import hashlib
import io
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata

NAME = "func_800E8D50"
PACKET = Path(__file__).resolve().parent
BASELINE = ROOT / "cloud/work/tiny_A76/func_800E8D50.blend.c"
HELPER = ROOT / "src/blob/groups/func_800EA3F4/group.c"
NORMALIZE = ROOT / "src/blob/vector_normalize_length.c"
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared"


def digest(data):
    return hashlib.sha256(data).hexdigest()


def extent(obj, name):
    data, sections = score._elf(obj)
    index = score._text_index(sections)
    found = [symbol for i, section in enumerate(sections)
             if section["type"] == 2
             for symbol in score._symbol_table(data, sections, i)
             if symbol["type"] == 2 and symbol["section"] == index
             and symbol["name"] == name]
    assert len(found) == 1, (name, "ambiguous ELF extent")
    return found[0]["value"], found[0]["size"]


def raw_body(obj, name):
    start, size = extent(obj, name)
    return struct.pack(">%dI" % (size // 4),
                       *score.text_words(obj)[start // 4:(start + size) // 4])


def inspect_object(obj, expected):
    target = score.targets()[NAME]
    start, size = extent(obj, NAME)
    assert size == len(target) * 4 == 448, ("wrong full ELF extent", size)
    with redirect_stdout(io.StringIO()):
        comparison = score.compare(obj, NAME)
    words, masks, unresolved, unverified, errors = score.relocate(
        obj, score.text_words(obj), start, start + size, score.image_symbols())
    assert not (unresolved or errors)
    own = owndata.verify(obj, NAME, target, address=score.image_symbols()[NAME],
                         image=owndata.ImageData.from_artifact(ROOT / "asm/us/blob_data"))
    assert own.ok, "own-data verification failed"
    assert set(masks) == own.sites, "own-data proof does not cover every masked site"
    assert len(unverified) == len(own.sites)
    # Resolve only content-verified own-data references, retaining the original
    # scorer result separately. Every instruction is compared unmasked below.
    parsed = owndata._Object(obj)
    text, _, _ = owndata._locate(parsed, NAME, None, size)
    references, stray = owndata._references(parsed, text)
    bases = own.bases()
    for symbol, ref in references:
        if not start <= ref.lo_site < start + size:
            continue
        section = parsed.sections[ref.section]["name"]
        address = bases[section] + ref.offset
        for site in ref.hi_sites:
            assert site in own.sites
            words[site // 4] = (words[site // 4] & 0xFFFF0000) | (((address + 0x8000) >> 16) & 0xFFFF)
        assert ref.lo_site in own.sites
        words[ref.lo_site // 4] = (words[ref.lo_site // 4] & 0xFFFF0000) | (address & 0xFFFF)
    actual = words[start // 4:(start + size) // 4]
    offsets = [4 * i for i, (a, b) in enumerate(zip(actual, target)) if a != b]
    assert len(actual) == len(target)
    assert offsets == expected, ("unexpected full-word residual", offsets)
    assert comparison.differing == len(offsets)
    assert comparison.total == 112 and comparison.extra_words == 0
    assert not comparison.accepted(), "Research unexpectedly matched: seek independent review"
    return {
        "elf_function_size_bytes": size,
        "target_extent_bytes": len(target) * 4,
        "target_bytes_sha256": digest(struct.pack(">112I", *target)),
        "relocated_body_sha256": digest(struct.pack(">112I", *actual)),
        "residual_offsets": ["0x%03X" % offset for offset in offsets],
        "full_relocated_words_equal": actual == target,
        "comparison": asdict(comparison),
        "own_data_verified_sites": ["0x%03X" % (site - start) for site in sorted(own.sites)],
        "remaining_unverified_after_own_data_proof": 0,
    }


def helper_source():
    """Reuse only the complete genuine setter body, byte-for-byte unchanged."""
    source = HELPER.read_text()
    start = source.index("void func_800E8CB8(void *car, void *vel, void *mat) {")
    end = source.index("\n}", start) + 2
    body = source[start:end]
    return ("typedef signed char s8;\ntypedef unsigned char u8;\n"
            "typedef signed short s16;\ntypedef signed int s32;\ntypedef float f32;\n"
            "extern void math_utility(void *, void *);\n"
            "extern void func_8008D6FC(s16, void *, void *);\n"
            "extern f32 D_801106C0[];\nextern s8 D_801613AB;\n" + body + "\n")


def replay(output):
    output.mkdir(parents=True, exist_ok=True)
    rows = []
    for label, source, expected in [
            ("archive", BASELINE, [0x114, 0x118, 0x160]),
            ("best", PACKET / "best.c", [0x160])]:
        for optimization in ("O2", "O3"):
            obj = output / (label + "_" + optimization + ".o")
            flags = FLAGS.replace("O3", optimization)
            score.compile_single(str(source), flags, obj)
            rows.append({"case": label + "_" + optimization,
                         "source_sha256": digest(source.read_bytes()),
                         "flags": flags, **inspect_object(obj, expected)})
    groups = []
    context = ["vector_normalize_length", "func_800E8CB8"]
    for label, source, expected in [
            ("archive", BASELINE, [0x114, 0x118, 0x160]),
            ("best", PACKET / "best.c", [0x160])]:
        directory = output / (label + "_real_callees")
        directory.mkdir(exist_ok=True)
        (directory / "setter.c").write_bytes(source.read_bytes())
        (directory / "normalize.c").write_bytes(NORMALIZE.read_bytes())
        (directory / "matrix_setter.c").write_text(helper_source())
        spec = {"members": [NAME], "context": context,
                "files": ["setter.c", "normalize.c", "matrix_setter.c"],
                "keep": [NAME] + context, "claims": [], "flags": FLAGS}
        (directory / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
        obj = directory / "candidate.o"
        score.compile_group(directory, obj)
        groups.append(obj)
        rows.append({"case": label + "_real_callees",
                     "source_sha256": digest(source.read_bytes()),
                     "flags": FLAGS, **inspect_object(obj, expected)})
    wrong = output / "wrong_literal.c"
    wrong.write_text((PACKET / "best.c").read_text().replace("0.6f", "0.7f"))
    wrong_obj = output / "wrong_literal.o"
    score.compile_single(str(wrong), FLAGS, wrong_obj)
    try:
        inspect_object(wrong_obj, [0x160])
    except AssertionError as exc:
        assert str(exc) == "own-data verification failed", str(exc)
    else:
        raise AssertionError("wrong literal was not refused")
    preservation = []
    for name in context:
        before, after = [raw_body(obj, name) for obj in groups]
        assert before == after, (name, "context changed")
        preservation.append({"function": name, "raw_body_unchanged": True,
                             "size_bytes": len(before), "sha256": digest(before),
                             "new_matching_claim": False})
    addresses = score.image_symbols()
    return {"schema": 1, "baseline_commit": "cf10b339",
            "function": NAME, "native_start": "0x%08X" % addresses[NAME],
            "native_end_exclusive": "0x%08X" % (addresses[NAME] + 448),
            "status": "NON_MATCHING_RESEARCH", "claims": [],
            "source_inputs": {str(p.relative_to(ROOT)): digest(p.read_bytes())
                              for p in [BASELINE, PACKET / "best.c", HELPER, NORMALIZE]},
            "target_manifest_sha256": digest((score.ASM_DIR / "SHA256SUMS").read_bytes()),
            "own_data_manifest_sha256": digest((ROOT / "asm/us/blob_data/SHA256SUMS").read_bytes()),
            "scorer_sha256": digest((ROOT / "tools/cloud/score.py").read_bytes()),
            "cases": rows, "context_preservation": preservation,
            "wrong_literal_negative_control_refused": True,
            "limits": ["No splice, shadow gate, image build, or ROM hash claim.",
                       "Context preservation compares unchanged raw ELF bodies; it is not a new target match claim.",
                       "The remaining FP multiply operand order must not be masked or ignored."]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / "build/dot_vector_blend/replay")
    args = parser.parse_args()
    result = replay(args.output)
    path = args.output / "verification.json"
    path.write_text(json.dumps(result, indent=2) + "\n")
    print("Six complete-extent replays verified; best remains 1/112 words different.")
    print(path)


if __name__ == "__main__":
    main()
