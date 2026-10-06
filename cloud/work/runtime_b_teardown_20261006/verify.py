#!/usr/bin/env python3
"""Complete-object and bounded behavioral proof of a NONMATCH reconstruction.

Targets are read only from hash-checked repository inputs. No native bytes,
assembly, objects, or original images are emitted to the research packet.
"""
import argparse
import contextlib
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
PACKET = Path(__file__).resolve().parent
SOURCE = PACKET / 'candidate.c'
NAME, ADDRESS, SIZE = 'func_8039244C', 0x8039244C, 380
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
SYMBOLS = {'D_80395ED4': 0x80395ED4, 'D_80151AD0': 0x80151AD0,
           'D_803950C0': 0x803950C0, 'D_80395C00': 0x80395C00,
           'D_80395CD0': 0x80395CD0, 'D_80395DA0': 0x80395DA0,
           'sound_stop': 0x800B358C, 'sound_call_minimal': 0x80090254}
RELOCS = [(8, 5, 'D_80395ED4'), (12, 6, 'D_80395ED4'),
          (60, 4, 'sound_stop'), (72, 5, 'D_80151AD0'),
          (76, 6, 'D_80151AD0'), (80, 5, 'D_80395C00'),
          (84, 6, 'D_80395C00'), (92, 5, 'D_80395CD0'),
          (96, 5, 'D_80395DA0'), (100, 5, 'D_803950C0'),
          (104, 6, 'D_80395CD0'), (108, 6, 'D_80395DA0'),
          (112, 6, 'D_803950C0'), (140, 4, 'sound_call_minimal'),
          (172, 4, 'sound_call_minimal'), (200, 4, 'sound_call_minimal'),
          (232, 4, 'sound_call_minimal'), (260, 5, 'D_80151AD0'),
          (264, 6, 'D_80151AD0'), (268, 5, 'D_803950C0'),
          (272, 6, 'D_803950C0')]
BASE, END, COUNT, SP, RETURN = 0x803950C0, 0x80395ED8, 0x80151AD0, 0x70001000, 0x60000000
U32 = 0xffffffff
COUNTS = [-32768, -1, 0, 1, 2, 3, 4]


def check(condition, message='verification failed'):
    if not condition:
        raise AssertionError(message)


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def run(args, **kwargs):
    r = subprocess.run([str(a) for a in args], capture_output=True, text=True, **kwargs)
    check(r.returncode == 0, (args, r.returncode, r.stdout, r.stderr))
    return r


def signed(x, n=32):
    x &= (1 << n) - 1
    return x - (1 << n) if x & (1 << (n - 1)) else x


class Memory:
    def __init__(self, other=None):
        self.banks = {BASE: bytearray(END - BASE), COUNT: bytearray(2), SP - 64: bytearray(64)} if other is None else {a: bytearray(b) for a, b in other.banks.items()}

    def locate(self, address, width):
        check(address % width == 0, ('unaligned access', address, width))
        for base, data in self.banks.items():
            if base <= address and address + width <= base + len(data):
                return data, address - base
        raise AssertionError(('unmapped access', hex(address), width))

    def get(self, address, width):
        data, offset = self.locate(address, width)
        return int.from_bytes(data[offset:offset + width], 'big')

    def put(self, address, value, width):
        data, offset = self.locate(address, width)
        data[offset:offset + width] = (value & ((1 << (width * 8)) - 1)).to_bytes(width, 'big')

    def snapshot(self):
        return bytes(self.banks[BASE]), bytes(self.banks[COUNT])


def fixture(seed, count, resource):
    m = Memory()
    state = seed + 1
    for data in m.banks.values():
        for i in range(len(data)):
            state = (state * 1664525 + 1013904223) & U32
            data[i] = state >> 24
    m.put(COUNT, count, 2)
    m.put(SYMBOLS['D_80395ED4'], 0x50001000 if resource else 0, 4)
    for player in range(4):
        addresses = [0x80395C02 + player * 52, 0x80395CD2 + player * 52, 0x80395DA0 + player * 52]
        addresses += [BASE + player * 720 + i * 72 + 2 for i in range(10)]
        for i, address in enumerate(addresses):
            values = [-1, 0, 1, -32768, 32767, 0x1234]
            m.put(address, values[(seed + player * 3 + i) % len(values)], 2)
    return m


def hook(memory, address, argument, scenario, events):
    check(address in [0x800B358C, 0x80090254], 'unknown helper')
    if address == 0x80090254:
        check(argument == (signed(argument, 16) & U32), 'handle must be sign extended')
    events.append((address, argument, memory.snapshot()))
    ordinal = len(events)
    if scenario == 1 and ordinal == 1:
        memory.put(COUNT, 4, 2)
    elif scenario == 2 and ordinal == 1:
        memory.put(COUNT, 0, 2)
    elif scenario == 3:
        memory.put(0x80395ED4, 0x50002000, 4)
        memory.put(0x80395CD2 + (ordinal % 4) * 52, 32000 - ordinal, 2)
        memory.put(BASE + (ordinal % 40) * 72 + 8, ordinal * 31, 4)
    elif scenario == 4:
        memory.put(COUNT, [4, 1, 3, 2, 0][ordinal % 5], 2)
        memory.put(0x80395C00 + (ordinal % 4) * 52, -ordinal, 2)
    elif scenario == 5 and ordinal == 1:
        memory.put(COUNT, 2, 2)


def oracle(original, scenario):
    memory, events = Memory(original), []
    def release(address):
        value = signed(memory.get(address, 2), 16)
        if value != -1:
            hook(memory, 0x80090254, value & U32, scenario, events)
            memory.put(address, -1, 2)
    pointer = memory.get(0x80395ED4, 4)
    if pointer:
        hook(memory, 0x800B358C, pointer, scenario, events)
        memory.put(0x80395ED4, 0, 4)
    if signed(memory.get(COUNT, 2), 16) > 0:
        player = 0
        while True:
            check(player < 4, 'fixture domain escaped')
            release(0x80395C02 + player * 52)
            memory.put(0x80395C00 + player * 52, 8, 2)
            release(0x80395CD2 + player * 52)
            memory.put(0x80395CD0 + player * 52, 0, 2)
            release(0x80395DA0 + player * 52)
            for i in range(10):
                p = BASE + player * 720 + i * 72
                release(p + 2)
                memory.put(p, 11, 2)
            player += 1
            if player >= signed(memory.get(COUNT, 2), 16):
                break
    return memory.snapshot(), events


def execute(words, original, scenario):
    """Bounded MIPS II execution with explicit O32 helper hooks and delay slots."""
    memory, events, covered, branches = Memory(original), [], set(), set()
    regs = [(0x13579bdf * (i + 1)) & U32 for i in range(32)]
    regs[0], regs[29], regs[31] = 0, SP, RETURN
    initial = regs[:]
    pc, pending = 0, None
    for step in range(20000):
        if pc == RETURN:
            check(pending is None, 'return pending')
            for r in list(range(16, 24)) + [28, 29, 30, 31]:
                check(regs[r] == initial[r], ('preserved register', r))
            return memory.snapshot(), events, covered, branches
        check(pc % 4 == 0 and 0 <= pc < len(words) * 4, ('bad PC', pc))
        covered.add(pc)
        w = words[pc // 4]
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        imm, shift, fn = signed(w, 16), (w >> 6) & 31, w & 63
        address = (regs[rs] + imm) & U32
        current, pending = pending, None
        nextpc = pc + 4
        if op == 0:
            if fn == 0: regs[rd] = (regs[rt] << shift) & U32
            elif fn == 0x21: regs[rd] = (regs[rs] + regs[rt]) & U32
            elif fn == 0x23: regs[rd] = (regs[rs] - regs[rt]) & U32
            elif fn == 0x25: regs[rd] = regs[rs] | regs[rt]
            elif fn == 0x2b: regs[rd] = int(regs[rs] < regs[rt])
            elif fn == 8:
                check(rs == 31 and regs[rs] == RETURN, 'bad return')
                pending = ('jump', RETURN)
            else: raise AssertionError(('unsupported SPECIAL', fn))
        elif op == 9: regs[rt] = (regs[rs] + imm) & U32
        elif op == 15: regs[rt] = (w & 0xffff) << 16
        elif op == 0x21: regs[rt] = signed(memory.get(address, 2), 16) & U32
        elif op == 0x23: regs[rt] = memory.get(address, 4)
        elif op == 0x29: memory.put(address, regs[rt], 2)
        elif op == 0x2b: memory.put(address, regs[rt], 4)
        elif op in (4, 5, 6, 0x14):
            check(current is None, 'branch in delay slot')
            taken = regs[rs] == regs[rt] if op in (4, 0x14) else regs[rs] != regs[rt] if op == 5 else signed(regs[rs]) <= 0
            branches.add((pc, taken))
            if op == 0x14 and not taken:
                nextpc += 4
            else:
                pending = ('jump', pc + 4 + imm * 4 if taken else pc + 8)
        elif op == 3:
            check(current is None, 'call in delay slot')
            regs[31] = ADDRESS + pc + 8
            pending = ('call', ((w & 0x3ffffff) << 2) | 0x80000000)
        else: raise AssertionError(('unsupported opcode', op))
        regs[0] = 0
        if current is not None:
            check(pending is None, 'control transfer in delay slot')
            kind, destination = current
            if kind == 'call':
                hook(memory, destination, regs[4], scenario, events)
                for r in [1, 2, 3] + list(range(4, 16)) + [24, 25]:
                    regs[r] = (0xabcdef00 ^ r * 0x1020304 ^ len(events)) & U32
            else:
                nextpc = destination
        pc = nextpc
    raise AssertionError('execution limit')


def extract_target(region, name):
    old = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/blob'
    try:
        data = score.verified_bytes(score.ASM_DIR / region, score.target_manifest()).decode()
        body = data.split('\n' + name + ':\n', 1)[1].split('.section', 1)[0]
        return [int(x, 16) for x in re.findall(r'\.word (0x[0-9A-Fa-f]+)', body)]
    finally:
        score.ASM_DIR = old


def elf_proof(tmp):
    score.ASM_DIR = ROOT / 'asm/us/ovl_b'
    native = score.targets()[NAME]
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())
    record = next(r for r in extents['functions'] if r['name'] == NAME)
    check(extents['image'] == 'B' and extents['rom_offset'] == '0xB6FEC4', 'image identity')
    check(extents['image_sha256'] == 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd', 'image hash')
    check(int(record['address'], 16) == ADDRESS and record['size'] == SIZE == len(native) * 4, 'native extent')
    symbols = score.image_symbols()
    for name, address in dict(SYMBOLS, **{NAME: ADDRESS}).items():
        check(symbols.get(name, score.address_named(name)) == address, ('address drift', name))
    obj = tmp / 'candidate.o'
    score.compile_single(SOURCE, FLAGS, obj)
    with contextlib.redirect_stdout(io.StringIO()): result = score.compare(obj, NAME)
    check(not result.accepted(), 'unexpected match requires independent re-review')
    data, sections = score._elf(obj)
    syms = [s for i, sec in enumerate(sections) if sec['type'] == 2 for s in score._symbol_table(data, sections, i)]
    functions = [s for s in syms if s['type'] == 2 and s['section'] != 0]
    check(len(functions) == 1 and functions[0]['name'] == NAME and functions[0]['size'] == SIZE and functions[0]['value'] == 0, 'complete ELF function')
    shoff, stride = struct.unpack_from('>I', data, 32)[0], struct.unpack_from('>H', data, 46)[0]
    for i, sec in enumerate(sections):
        flags = struct.unpack_from('>I', data, shoff + stride * i + 8)[0]
        check(not (flags & 2 and sec['size'] and sec['name'] not in ['.text', '.reginfo']), ('unexpected owned data', sec['name']))
    relocs = []
    for sec in sections:
        if sec['type'] != 9: continue
        check(sec['info'] == score._text_index(sections), 'relocation outside text')
        symbols_here = score._symbol_table(data, sections, sec['link'])
        for i in range(sec['off'], sec['off'] + sec['size'], 8):
            offset, info = struct.unpack_from('>II', data, i)
            relocs.append((offset, info & 255, symbols_here[info >> 8]['name']))
    check(sorted(relocs) == RELOCS, 'relocation shape drift')
    words = score.text_words(obj)
    check(len(words) == 96 and words[95:] == [0], 'alignment tail')
    relocated, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, SIZE, symbols)
    check(not any([masks, unresolved, unverified, errors]), 'incomplete relocation')
    relocated = relocated[:95]
    script = tmp / 'native.ld'
    script.write_text('SECTIONS { . = 0x8039244C; .text : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n' + '\n'.join('%s = 0x%x;' % pair for pair in SYMBOLS.items()))
    linked, raw = tmp / 'linked.elf', tmp / 'linked.bin'
    run(['mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, obj])
    run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', linked, raw])
    linked_data = raw.read_bytes()
    check(len(linked_data) == 384 and linked_data[SIZE:] == bytes(4), 'GNU complete text')
    gnu = list(struct.unpack('>95I', linked_data[:SIZE]))
    check(gnu == relocated, 'GNU disagrees with project relocation')
    nm = run(['mips-linux-gnu-nm', '-S', linked]).stdout
    check(any(line.split()[:2] == ['8039244c', '0000017c'] and line.split()[-1] == NAME for line in nm.splitlines()), 'GNU placement/extent')
    residual = [i * 4 for i, (a, b) in enumerate(zip(native, gnu)) if a != b]
    check(len(residual) == 34, ('residual changed', residual))
    return native, relocated, gnu, {'function_bytes': SIZE, 'text_bytes': 384, 'separate_zero_alignment_bytes': 4, 'owned_data_bytes': 0, 'relocations': relocs, 'differing_words': len(residual), 'difference_offsets': residual, 'native_sha256': hashlib.sha256(struct.pack('>95I', *native)).hexdigest(), 'gnu_body_sha256': hashlib.sha256(linked_data[:SIZE]).hexdigest(), 'gnu_equals_project_relocation': True}


def prove():
    with tempfile.TemporaryDirectory(prefix='rush-b-teardown-') as directory:
        tmp = Path(directory)
        native, project, gnu, elf = elf_proof(tmp)
        controls = {}
        for level, expected_size, expected_diff, expected_extra in [('O1', 460, 94, 19), ('O3', 380, 34, 0)]:
            control = tmp / ('control-' + level + '.o')
            flags = FLAGS.replace('-O2', '-' + level)
            score.compile_single(SOURCE, flags, control)
            with contextlib.redirect_stdout(io.StringIO()): control_result = score.compare(control, NAME)
            data, sections = score._elf(control)
            syms = [v for i, sec in enumerate(sections) if sec['type'] == 2 for v in score._symbol_table(data, sections, i)]
            function = next(v for v in syms if v['name'] == NAME and v['type'] == 2)
            check(function['size'] == expected_size and control_result.differing == expected_diff and control_result.extra_words == expected_extra, 'control drift')
            check(not any([control_result.unresolved, control_result.unverified, control_result.errors]), 'control relocation error')
            values, masks, unresolved, unverified, errors = score.relocate(control, score.text_words(control), 0, expected_size, score.image_symbols())
            check(not any([masks, unresolved, unverified, errors]), 'control complete relocation')
            body = values[:expected_size // 4]
            if level == 'O3': check(body == project, 'O3 body changed')
            controls[level] = {'flags': flags + ' -Wab,-r4300_mul', 'elf_bytes': expected_size, 'differing_native_words': expected_diff, 'nonzero_excess_words': expected_extra, 'body_sha256': hashlib.sha256(b''.join(v.to_bytes(4, 'big') for v in body)).hexdigest()}
        covered = [set(), set(), set()]
        branches = [set(), set(), set()]
        total = 0
        for seed in range(16):
            for count in COUNTS:
                for resource in [False, True]:
                    original = fixture(seed, count, resource)
                    for scenario in range(6):
                        expected = oracle(original, scenario)
                        for i, body in enumerate([native, project, gnu]):
                            got = execute(body, original, scenario)
                            check(got[:2] == expected, ('behavior difference', seed, count, resource, scenario, i))
                            covered[i] |= got[2]
                            branches[i] |= got[3]
                        total += 1
        for body, offsets, outcomes in zip([native, project, gnu], covered, branches):
            check(offsets == set(range(0, SIZE, 4)), 'instruction coverage incomplete')
            sites = {i * 4 for i, w in enumerate(body) if w >> 26 in (4, 5, 6, 0x14)}
            check(outcomes == {(site, taken) for site in sites for taken in [False, True]}, 'branch outcome coverage incomplete')
        negatives = {}
        for name, change in [('unknown_instruction', lambda w: w.__setitem__(0, 0xffffffff)), ('wrong_state', lambda w: w.__setitem__(39, 0x240f0009)), ('bad_memory_base', lambda w: w.__setitem__(2, 0x3c100000))]:
            body = native[:]
            change(body)
            try:
                check(execute(body, fixture(1, 1, True), 0)[:2] == oracle(fixture(1, 1, True), 0), 'mutant mismatch')
            except AssertionError:
                negatives[name] = True
            else:
                raise AssertionError(('negative escaped', name))
        common = ['gcc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror', '-O2', '-fstrict-aliasing', '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all']
        exe = tmp / 'host'
        run(common + [PACKET / 'host_test.c', '-o', exe])
        host = run([exe])
        check(host.stdout.strip() == '1344 complete-state host fixtures passed' and not host.stderr, 'host fixture result')
        mutants = {'wrong_state': ('first->state = 8;', 'first->state = 9;'), 'last_slot_omitted': ('i < 10', 'i < 9'), 'sentinel_not_cleared': ('second->handle = -1;', 'second->handle = 0;'), 'resource_not_cleared': ('D_80395ED4 = 0;', 'D_80395ED4 = D_80395ED4;')}
        for name, (old, new) in mutants.items():
            source = SOURCE.read_text()
            check(source.count(old) == 1, 'mutant anchor')
            path = tmp / (name + '.c')
            path.write_text(source.replace(old, new))
            run(common + ['-DCANDIDATE_PATH="' + str(path) + '"', PACKET / 'host_test.c', '-o', exe])
            result = subprocess.run([str(exe)], capture_output=True, text=True)
            check(result.returncode == 1 and 'mismatch' in result.stderr, ('host mutant escaped', name, result.stderr))
        helper = extract_target('blob_8008d0c0.s', 'sound_call_minimal')
        release = extract_target('blob_800aeb54.s', 'sound_stop')
        check(len(helper) * 4 == 48 and len(release) * 4 == 160, 'helper extent')
        helper_source = ROOT / 'src/blob/sound_call_minimal.c'
        locks = json.loads((ROOT / 'blob_matched.lock.json').read_text())
        lock = locks['sound_call_minimal']
        # Lock hashes are normalized by the production system, so raw file hash
        # is retained separately. The existing source is compiled unchanged.
        helper_obj = tmp / 'helper.o'
        score.compile_single(helper_source, lock['flagset'], helper_obj)
        old = score.ASM_DIR
        score.ASM_DIR = ROOT / 'asm/us/blob'
        symbols = score.image_symbols()
        score.ASM_DIR = old
        raw_words = score.text_words(helper_obj)
        rw, masks, unresolved, unverified, errors = score.relocate(helper_obj, raw_words, 0, 48, symbols)
        check(not any([masks, unresolved, unverified, errors]) and rw[:12] == helper and all(w == 0 for w in rw[12:]), 'accepted helper reproduction')
        inputs = [SOURCE, PACKET / 'verify.py', PACKET / 'host_test.c', ROOT / 'tools/cloud/score.py', ROOT / 'tools/cloud/owndata.py', ROOT / 'asm/us/ovl_b/SHA256SUMS', ROOT / 'asm/us/ovl_b/extents.json', ROOT / 'asm/us/ovl_b/symbols.json', ROOT / 'asm/us/ovl_b/ovl_b_8038a400.s', ROOT / 'asm/us/blob/SHA256SUMS', ROOT / 'asm/us/blob/symbols.json', ROOT / 'asm/us/blob/blob_8008d0c0.s', ROOT / 'asm/us/blob/blob_800aeb54.s', helper_source, ROOT / 'blob_matched.lock.json']
        return {'status': 'NONMATCH', 'image': 'B', 'address': hex(ADDRESS), 'bytes': SIZE, 'flags': FLAGS + ' -Wab,-r4300_mul', 'accepted_bytes': 0, 'elf': elf, 'optimization_controls': controls, 'behavior': {'fixtures': total, 'native_project_gnu_executions': total * 3, 'host_fixtures': total, 'instruction_offsets_covered': [len(x) for x in covered], 'branch_outcomes_covered': [len(x) for x in branches], 'full_memory_and_helper_entry_snapshots': True, 'o32_caller_clobbers_and_saved_registers': True, 'native_negative_controls': negatives, 'compiled_host_mutants_rejected': sorted(mutants)}, 'boundaries': {'sound_call_minimal_native_bytes': 48, 'sound_call_minimal_source_recompiled_equal': True, 'sound_stop_native_bytes': 160, 'sound_stop_has_no_accepted_source': True, 'helper_bodies_execute_in_behavior_tests': False}, 'inputs_sha256': {str(p.relative_to(ROOT)): sha(p) for p in inputs}}


def tool_provenance():
    paths = [('ido_' + n, score.ido(n)) for n in ['cc', 'cfe', 'ugen', 'uopt', 'as1']]
    paths += [(n, shutil.which(n)) for n in ['gcc', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy', 'mips-linux-gnu-nm']]
    return {name: sha(path) for name, path in paths}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--tool-provenance', type=Path)
    args = parser.parse_args()
    result = prove()
    text = json.dumps(result, indent=2, sort_keys=True) + '\n'
    if args.output: args.output.write_text(text)
    else: print(text, end='')
    if args.tool_provenance: args.tool_provenance.write_text(json.dumps(tool_provenance(), indent=2, sort_keys=True) + '\n')
