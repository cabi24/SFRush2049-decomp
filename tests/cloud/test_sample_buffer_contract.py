"""Shared sample/emitter contract regressions, including actual adapted sources."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
WORK = ROOT / 'cloud/work/boot_tail_promotion/sample_buffer_contract'
spec = importlib.util.spec_from_file_location('sample_buffer_contract_verify', WORK / 'verify.py')
proof = importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)


def test_overlay_survives_later_promotion():
    path = 'src/rom/lib_16320.c'
    name = 'func_800163A8'
    source = (ROOT / proof.SOURCES / (name + '.c')).read_text()
    current = (ROOT / path).read_text()
    once = proof.overlay(current, path, name, source)
    assert proof.overlay(once, path, name, source) == once
    with pytest.raises(proof.VerificationError):
        proof.overlay(once + '\n#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_800163A8.s")', path, name, source)


def test_exact_extent_rejects_short_or_long_symbol():
    for size in (292, 300):
        with pytest.raises(proof.VerificationError):
            proof.exact_extent({'fn': {'size': size, 'value': 0}}, 'fn', 296)
    proof.exact_extent({'fn': {'size': 296, 'value': 0}}, 'fn', 296)


def toolchain_available():
    return Path(proof.score.IDO / 'cc').is_file() and shutil.which('mips-linux-gnu-as') and shutil.which('mips-linux-gnu-ld')


@pytest.mark.skipif(not toolchain_available(), reason='requires pinned IDO and MIPS binutils')
def test_real_combined_tus_and_negative_controls(tmp_path):
    result = proof.verify(tmp_path)
    combined = [stages['combined'] for stages in result['stages'].values()]
    assert sum(len(stage['functions']) for stage in combined) >= 24
    assert sum(stage['all_tu_slots_verified'] for stage in combined) == 57
    assert len(result['standalone_candidates']) == 6
    assert sum(row['bytes'] for row in result['standalone_candidates']) == 1356
    assert all(row['rejected'] for row in result['original_conflict_controls'])
    assert all(row['rejected'] for row in result['semantic_mutation_controls'])
    assert result['native_layout']['StateNode']['size'] == 68


@pytest.mark.skipif(not toolchain_available() or not shutil.which('cc'), reason='requires IDO and a host C compiler')
def test_adapted_emitter_defined_path_replay():
    completed = subprocess.run([sys.executable, str(WORK / 'replay_emitter.py')], cwd=ROOT, capture_output=True, text=True)
    assert completed.returncode == 0, completed.stdout + completed.stderr
    assert 'safe=612 undefined_output=172 helper_capacity=2' in completed.stdout


def test_adapted_registry_buffers_and_real_unlink(tmp_path):
    if not shutil.which('cc'):
        pytest.skip('requires a host C compiler')
    candidates = ['800163A8', '8001C508', '8001C7F4', '8001D4CC', '8001E0C0']
    includes = ''.join('#include "' + str(ROOT / proof.SOURCES / ('func_' + name + '.c')) + '"\n' for name in candidates)
    tu = (ROOT / 'src/rom/lib_1cf90.c').read_text()
    bodies = {name: tu[start:end] for name, start, end in proof.extract_functions(tu)}
    # Exercise the real, unchanged unlink callee and saved-next walker.
    actual = bodies['func_8001D084'] + '\n' + bodies['func_8001D578']
    source = includes + r'''
#include <assert.h>
#include <string.h>
unsigned char D_8002C630, D_8004FA18;
int D_800385A0;
RegisteredSamples D_800385A8[3];
SampleBuffer D_8004FA50[3];
StateNode *D_8004FD50;
LinkNode *D_8004FD54;
static int entered, exited, releases, removed, flushed, stopped, voices;
static SampleRecord *removed_records;
static void *release_descriptor;
static unsigned int release_offset, stopped_channel;
static short *flushed_buffer;
static unsigned int flushed_count, last_voice;
void func_80014594(void) { ++entered; }
void func_800145DC(void) { ++exited; }
void func_80014D08(void *descriptor, unsigned int offset) {
    ++releases; release_descriptor = descriptor; release_offset = offset;
}
int func_800161A0(SampleRecord *records) { ++removed; removed_records = records; return 1; }
void func_80014C60(short *buffer, unsigned int samples) {
    ++flushed; flushed_buffer = buffer; flushed_count = samples;
}
void func_8001F954(unsigned int channel) { ++stopped; stopped_channel = channel; }
int func_8001B8C4(unsigned int voice) { ++voices; last_voice = voice; return 1; }
''' + actual + r'''
int main(void) {
    SampleRecord first[4], second[3];
    StateNode a, b, c;
    LinkNode listener;
    short data[8];
    memset(first, 0, sizeof(first)); memset(second, 0, sizeof(second));
    first[0].identifier = 7; first[0].references = 2; first[0].offset = 12;
    first[1].identifier = 7; first[1].references = 1; first[1].offset = 24;
    first[2].identifier = 0xffff;
    first[3].identifier = 7; first[3].references = 9;
    second[0].identifier = 7; second[0].references = 5; second[1].identifier = 0xffff;
    D_800385A8[0].records = first; D_800385A8[1].records = second;
    D_800385A0 = 0;
    assert(func_800163A8(7) == 0 && releases == 0);
    D_800385A0 = 2;
    assert(func_800163A8(8) == 0 && releases == 0);
    assert(func_800163A8(7) == 1);
    assert(first[0].references == 1 && first[1].references == 0);
    assert(first[3].references == 9 && second[0].references == 5);
    assert(releases == 1 && release_descriptor == first[1].descriptor && release_offset == 24);
    assert(removed == 0);
    first[0].references = 1; first[1].identifier = 8;
    assert(func_800163A8(7) == 1 && removed == 1 && removed_records == first);
    assert(releases == 2 && release_descriptor == first[0].descriptor);
    /* Native u16 decrement wraps; no invented saturation or zero guard. */
    first[0].references = 0;
    assert(func_800163A8(7) == 1 && first[0].references == 65535 && removed == 1);
    first[0].identifier = 0xffff;
    assert(func_800163A8(7) == 1 && second[0].references == 4);

    memset(D_8004FA50, 0, sizeof(D_8004FA50)); D_8004FA18 = 3;
    D_8004FA50[1].mode = 1; D_8004FA50[1].buffer = data; D_8004FA50[1].samples = 8;
    D_8004FA50[1].position = 3; D_8004FA50[1].context = 0x1234;
    D_8004FA50[2].mode = 2;
    func_8001C508(); assert(flushed == 1 && flushed_buffer == data && flushed_count == 8);
    D_8002C630 = 0; func_8001C7F4(1); assert(stopped == 0 && entered == 0);
    D_8002C630 = 1; func_8001C7F4(1);
    assert(stopped == 1 && stopped_channel == 1 && D_8004FA50[1].mode == 0);
    assert(D_8004FA50[1].samples == 8 && D_8004FA50[1].position == 3 && D_8004FA50[1].context == 0x1234);
    func_8001C7F4(1); assert(stopped == 1 && entered == 2 && exited == 2);

    memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c));
    a.next = &b; b.previous = &a; b.next = &c; c.previous = &b;
    a.flags08 = b.flags08 = c.flags08 = 0x30005;
    a.identifier34 = 0xffffffffU; b.identifier34 = 0x1234; c.identifier34 = 0xffffffffU;
    D_8004FD50 = &a;
    D_8002C630 = 0; assert(func_8001D4CC(&b) == 0 && a.next == &b);
    D_8002C630 = 1; assert(func_8001D4CC(&b) == 1);
    assert(a.next == &c && c.previous == &a && b.flags08 == 5);
    assert(voices == 1 && last_voice == 0x1234);
    func_8001D578(); assert(D_8004FD50 == 0 && a.flags08 == 5 && c.flags08 == 5);
    assert(voices == 1 && entered == exited);
    D_8004FD50 = &a; D_8004FD54 = &listener;
    func_8001E0C0(); assert(D_8004FD50 == 0 && D_8004FD54 == 0);
    return 0;
}
'''
    path, binary = tmp_path / 'contracts.c', tmp_path / 'contracts'
    path.write_text(source)
    command = ['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined', '-no-pie', '-I' + str(ROOT / 'include'), str(path), '-o', str(binary)]
    built = subprocess.run(command, capture_output=True, text=True)
    assert built.returncode == 0, built.stdout + built.stderr
    result = subprocess.run([str(binary)], capture_output=True, text=True, env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))
    assert result.returncode == 0, result.stdout + result.stderr
