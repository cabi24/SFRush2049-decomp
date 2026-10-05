"""Research evidence stays honest; host execution checks the mask/LCG semantics."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/frontier/dot_fresh_masked_rng"
# Protected manifests and the scorer are a frozen provenance snapshot, not
# locks on future work: asm/us/blob/SHA256SUMS changes with every game splice.
FROZEN_PROVENANCE = ("target_manifest_sha256", "scorer_sha256")


def test_source_bound_negative_evidence():
    evidence = json.loads((PACKET / "verification.json").read_text())
    assert evidence["source_sha256"] == hashlib.sha256((PACKET / "best.c").read_bytes()).hexdigest()
    assert evidence["status"] == "nonmatch"
    proof = evidence["verification"]
    assert proof["target_bytes"] == proof["elf_symbol_bytes"] == 268
    assert proof["target_words"] == 67
    assert proof["full_word_differences"] == len(proof["differing_word_offsets"]) == 13
    assert proof["elf_extent_exact"] and not proof["strict_match"]
    assert not any(proof[key] for key in ("unresolved", "unverified", "relocation_errors", "masked_relocations"))
    trials = json.loads((PACKET / "experiments.json").read_text())
    assert trials["compiles"] == len(trials["rows"]) == 41
    assert trials["strict_matches"] == 0 and trials["best_word_differences"] == 13


def reference(seed, mask):
    for _ in range(10000):
        seed = (seed * 1103515245 + 12345) & 0xffffffff
        choice = ((seed >> 16) & 0x7fff) // 1024
        if mask & (1 << choice):
            return choice, seed
    raise AssertionError("synthetic test exceeded rejection bound")


def test_compiled_source_replays(tmp_path):
    compiler = Path(os.environ.get("IDO_DIR", ROOT / "tools/cloud/ido")) / "cc"
    if not compiler.is_file():
        pytest.skip("IDO toolchain not installed")
    output = tmp_path / "replay.json"
    subprocess.run([sys.executable, str(PACKET / "verify.py"), "--output", str(output)],
                   cwd=ROOT, check=True, capture_output=True, text=True)
    replay = json.loads(output.read_text())
    saved = json.loads((PACKET / "verification.json").read_text())
    for key in FROZEN_PROVENANCE:
        replay.pop(key, None)
        saved.pop(key, None)
    assert replay == saved


def test_host_semantics_and_undefined_behavior(tmp_path):
    cc = shutil.which("cc")
    if not cc:
        pytest.skip("host C compiler not installed")
    harness = tmp_path / "harness.c"
    harness.write_text('#include <stdio.h>\n#include "' + str(PACKET / "best.c") + '"\n' + """
s32 D_8011735C;
u32 D_80123418[256];
int main(void) {
 unsigned int seed, mask, index, choice;
 while (scanf("%x %x %u", &seed, &mask, &index) == 3) {
  D_8011735C = (s32)seed;
  D_80123418[index] = mask;
  choice = func_800B23E0((u8)index);
  printf("%u %u\\n", choice, (u32)D_8011735C);
 }
 return 0;
}
""")
    executable = tmp_path / "harness"
    subprocess.run([cc, "-std=c89", "-O2", "-Wall", "-Wextra", "-Werror",
                    "-fsanitize=undefined", "-fno-sanitize-recover=undefined",
                    str(harness), "-o", str(executable)], check=True, capture_output=True, text=True)
    seeds = [0, 1, 0x7fffffff, 0x80000000, 0xffffffff, 12345, 0x12345678, 0xdeadbeef]
    masks = [1 << bit for bit in range(32)] + [0xffffffff, 0x55555555, 0xaaaaaaaa, 0x80000001]
    cases = [(seed, mask, index) for seed in seeds for mask in masks for index in (0, 255)]
    inputs = "".join("%x %x %d\n" % case for case in cases)
    run = subprocess.run([str(executable)], input=inputs, capture_output=True, text=True, check=True, timeout=10)
    assert not run.stderr
    actual = [tuple(map(int, line.split())) for line in run.stdout.splitlines()]
    assert actual == [reference(seed, mask) for seed, mask, _ in cases]
