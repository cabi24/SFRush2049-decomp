"""Task-enqueue research tests; none of these award a match or ROM coverage."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/frontier/task_enqueue_20261005"
SPEC = importlib.util.spec_from_file_location("task_enqueue_verify", PACKET / "verify.py")
verify = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(verify)


def test_explicit_extent_gate_rejects_zero_padding_illusion():
    exact_words = verify.score.Comparison(0, 32, [], [], [], 0)
    assert verify.accepts_exact(exact_words, 128, 128)
    assert not verify.accepts_exact(exact_words, 124, 128)
    assert not verify.accepts_exact(exact_words, 132, 128)
    uncertain = verify.score.Comparison(0, 32, [], ["own section"], [], 0)
    assert not verify.accepts_exact(uncertain, 128, 128)
    extra = verify.score.Comparison(0, 32, [], [], [], 1)
    assert not verify.accepts_exact(extra, 128, 128)


def test_receipt_preserves_honest_nonmatch_and_prior_extent_corrections():
    receipt = json.loads((PACKET / "verification.json").read_text())
    assert receipt["claims"] == []
    assert receipt["coverage_delta_bytes"] == 0
    candidate = receipt["candidate"]
    assert candidate["source_sha256"] == hashlib.sha256((PACKET / "candidate.c").read_bytes()).hexdigest()
    assert candidate["comparison"] == {
        "differing": 30, "total": 32, "unresolved": [], "unverified": [],
        "errors": [], "extra_words": 0,
    }
    assert candidate["elf_function_bytes"] == 124
    assert candidate["text_section_bytes"] == 128
    assert candidate["trailing_section_padding_bytes"] == 4
    assert candidate["all_function_relocations_verified"]
    assert not candidate["strict_match_with_extent"]
    expected = {
        "cloud/work/tiny_A116/func_8010FBE0.c": (132, 144),
        "cloud/work/tiny_A116/control.scalar_globals.c": (132, 144),
        "cloud/work/tiny_A116/control.packet_view.c": (124, 128),
        "cloud/work/tiny_A50/func_8010FBE0.c": (124, 128),
    }
    for name, (size, section_size) in expected.items():
        old = receipt["prior_extent_corrections"][name]
        assert (old["elf_function_bytes"], old["text_section_bytes"]) == (size, section_size)
        assert old["comparison"]["extra_words"] == 0
        assert not old["strict_match_with_extent"]


def test_bounded_sweep_did_not_hide_a_match():
    sweep = json.loads((PACKET / "sweep.json").read_text())
    assert sweep["variant_count"] == len(sweep["results"]) == 48
    assert sweep["claims"] == []
    ordinary = [r for r in sweep["results"] if not r["volatile_control"]]
    assert len(ordinary) == 24
    assert {r["comparison"]["differing"] for r in ordinary} == {30}
    assert {r["elf_function_bytes"] for r in sweep["results"]} == {124}
    assert min(r["comparison"]["differing"] for r in sweep["results"]) == 26


def test_ido_replay_and_native_abi_when_compiler_is_available():
    if not (verify.score.IDO / "cc").is_file():
        pytest.skip("IDO is unavailable; receipt checks are not a fresh replay")
    fresh = verify.verify()
    saved = json.loads((PACKET / "verification.json").read_text())
    assert fresh == saved


def test_host_behavior_copies_task_then_enqueues_in_order(tmp_path):
    """Host semantics only; the separate IDO probe establishes the 32-bit ABI."""
    cc = shutil.which("gcc")
    if not cc:
        pytest.skip("A host C compiler is unavailable")
    harness = tmp_path / "behavior.c"
    harness.write_text('#include "' + str(PACKET / "candidate.c") + '"\n' + r'''
struct OSMesgQueue { int tag; };
OSScTask D_80155238;
OSMesgQueue D_80152750, D_8002E960, D_8002E928;
static int phase, failure, first_result;
static OSTask *expected_source;
void *memcpy(void *destination, const void *source, u32 count) {
    unsigned char *out = destination;
    const unsigned char *in = source;
    u32 index;
    if (phase != 0 || destination != &D_80155238.list ||
        source != expected_source || count != sizeof(OSTask)) failure = 1;
    if (D_80155238.next != 0 || D_80155238.msgQueue != &D_80152750 ||
        D_80155238.msg != 0 || D_80155238.flags != 2) failure = 2;
    for (index = 0; index < count; ++index) out[index] = in[index];
    phase = 1;
    return destination;
}
s32 osJamMesg(OSMesgQueue *queue, OSMesg message, s32 blocking) {
    if (blocking != 1) failure = 3;
    if (phase == 1) {
        if (queue != &D_8002E960 || message != &D_80155238) failure = 4;
        phase = 2;
        return first_result;
    }
    if (phase != 2 || queue != &D_8002E928 || message != (OSMesg)670) failure = 5;
    phase = 3;
    return -1;
}
int main(void) {
    static OSTask source;
    OSScTask previous;
    unsigned char *input = (unsigned char *)&source;
    unsigned char *output = (unsigned char *)&D_80155238.list;
    u32 index;
    int seed;
    expected_source = &source;
    for (seed = 0; seed < 4; ++seed) {
        for (first_result = -1; first_result <= 0; ++first_result) {
            for (index = 0; index < sizeof(source); ++index)
                input[index] = (unsigned char)(index * 37 + seed * 71);
            D_80155238.next = &previous;
            D_80155238.state = 73;
            D_80155238.flags = 0xFFFFFFFF;
            D_80155238.framebuffer = &previous;
            D_80155238.msgQueue = &D_8002E928;
            D_80155238.msg = &previous;
            phase = failure = 0;
            func_8010FBE0(&source);
            if (failure || phase != 3) return 10 + failure;
            if (D_80155238.state != 73 || D_80155238.framebuffer != &previous) return 20;
            for (index = 0; index < sizeof(source); ++index)
                if (input[index] != output[index]) return 21;
        }
    }
    return 0;
}
''')
    executable = tmp_path / "behavior"
    subprocess.run([cc, "-std=c89", "-fno-builtin", "-Wall", "-Wextra", "-Werror",
                    str(harness), "-o", str(executable)], check=True, capture_output=True, text=True)
    subprocess.run([str(executable)], check=True, capture_output=True, text=True)
