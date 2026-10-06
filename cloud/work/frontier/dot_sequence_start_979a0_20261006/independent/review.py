"""Independent bounded review. Emits counts and hashes, never native code bytes."""
import ctypes
import hashlib
import json
import random
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

if not __debug__:
    raise RuntimeError('Independent assertions require ordinary Python; do not use -O.')

HERE = Path(__file__).resolve().parent
PACKET = HERE.parent
ROOT = PACKET.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from elf_link import Elf, link_function, native

FN = 'func_800979A0'
BASE = 0x800979A0
MASK = 0xFFFFFFFF
SP = 0x30010000
RET = 0x70000000


def sx(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def execute(code, addresses, case):
    """Separate delay-slot interpreter; real slot hook/allocation bodies execute."""
    regs = [(0x21730000 + i * 10711) & MASK for i in range(32)]
    regs[0] = 0
    regs[4], regs[5] = case['index'], case['unused']
    regs[29], regs[31] = SP, RET
    saved = regs[:]
    mem = {}
    stack = set(range(SP - 256, SP + 96))
    for addr in stack:
        mem[addr] = (addr * 31 + 117) & 255
    calls, reads, writes, coverage, edges = [], [], [], set(), set()

    def store(at, size, value, tracked=True):
        assert at % size == 0
        for i, byte in enumerate((value & ((1 << (size * 8)) - 1)).to_bytes(size, 'big')):
            mem[at + i] = byte
        if tracked and at not in stack:
            writes.append((at, size, value & ((1 << (size * 8)) - 1)))

    def load(at, size):
        assert at % size == 0 and all(at + i in mem for i in range(size)), ('unmapped', hex(at), size)
        value = int.from_bytes(bytes(mem[at + i] for i in range(size)), 'big')
        if at not in stack:
            reads.append((at, size, value))
        return value

    globals_ = {'D_80151960': case['disabled'], 'D_8011EAA0': case['old_result'],
                'D_80151A6C': case['last_index'], 'D_80151AD4': 7,
                'D_80151ADC': 0x60601000, 'D_80152464': 0x60602000,
                'D_801525FC': 0x60603000, 'D_80152690': 0x60604000,
                'D_801526D8': 0x60605000}
    for name, value in globals_.items():
        store(addresses[name], 1 if name == 'D_80151960' else 4, value, False)
    for slot in range(8):
        for i in range(20):
            mem[addresses['D_80156D38'] + slot * 20 + i] = (slot * 13 + i) & 255
        store(addresses['D_80156D38'] + slot * 20 + 12, 4, 0x60700000 + slot * 0x1000, False)
    entry = addresses['D_8011F070'] + case['index'] * 4
    store(entry, 2, 0x8BCD, False)
    store(entry + 2, 2, case['identifier'], False)
    initial = dict(mem)
    bodies = {addresses[name] + i * 4: word for name, words in code.items() for i, word in enumerate(words)}
    externs = {addresses[name]: name for name in ('audio_frame_sync', 'audio_loop_control', 'func_8001536C', 'func_800156E8')}
    pc, delayed = BASE, None
    for step in range(2000):
        if pc == RET:
            break
        if pc in externs:
            assert delayed is None
            name = externs[pc]
            argc = {'audio_frame_sync': 5, 'audio_loop_control': 2, 'func_8001536C': 5, 'func_800156E8': 4}[name]
            args = tuple(regs[4:4 + min(argc, 4)]) + ((load(regs[29] + 16, 4),) if argc == 5 else ())
            calls.append((name, args))
            if name == 'audio_frame_sync':
                result = case['handle']
            elif name == 'audio_loop_control':
                if case['mutate_slot']:
                    store(addresses['D_80151AD4'], 4, case['next_handle'])
                result = 0xEBADBEEF
            elif name == 'func_8001536C':
                if case['mutate_registry']:
                    store(entry + 2, 2, case['identifier'] ^ 0xFFFF)
                    store(addresses['D_80151ADC'], 4, 0x60908000)
                result = case['registry_return']
            else:
                result = case['result']
            # Each real external boundary may destroy every ordinary caller-save GPR.
            for r in (1, 2, 3, *range(4, 16), 24, 25):
                regs[r] = (0xC9250103 + r * 0x10305 + len(calls) * 0x1237) & MASK
            regs[2] = result & MASK
            pc = regs[31]
            continue
        assert pc in bodies, ('invalid pc', hex(pc))
        if pc == addresses['func_80096288']:
            calls.append(('func_80096288', tuple(regs[4:7])))
        if pc == addresses['display_list_alloc']:
            calls.append(('display_list_alloc', (regs[4],)))
        coverage.add(pc)
        word = bodies[pc]
        op, rs, rt, rd, shift, fn = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, (word >> 6) & 31, word & 63
        imm, a, b = sx(word, 16), regs[rs], regs[rt]
        prior, delayed = delayed, None
        out = None
        if op == 0:
            if fn == 0:
                out = (rd, b << shift)
            elif fn == 8:
                delayed = a
            elif fn == 0x21:
                out = (rd, a + b)
            elif fn == 0x25:
                out = (rd, a | b)
            else:
                raise AssertionError(('unsupported special', fn))
        elif op == 9:
            out = (rt, a + imm)
        elif op == 12:
            out = (rt, a & (word & 0xFFFF))
        elif op == 15:
            out = (rt, (word & 0xFFFF) << 16)
        elif op in (4, 5):
            taken = a == b if op == 4 else a != b
            edges.add((pc, taken))
            delayed = pc + 4 + imm * 4 if taken else pc + 8
        elif op == 3:
            regs[31] = pc + 8
            delayed = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) * 4)
        elif op in (35, 36, 37):
            size = {35: 4, 36: 1, 37: 2}[op]
            out = (rt, load((a + imm) & MASK, size))
        elif op in (40, 43):
            store((a + imm) & MASK, {40: 1, 43: 4}[op], b)
        else:
            raise AssertionError(('unsupported opcode', op))
        if out and out[0]:
            regs[out[0]] = out[1] & MASK
        if prior is not None:
            assert delayed is None, 'branch in delay slot'
            pc = prior
        else:
            pc += 4
    else:
        raise AssertionError('execution budget')
    assert delayed is None and all(regs[i] == saved[i] for i in [*range(16, 24), 28, 29, 30, 31])
    assert load(SP + 4, 4) == case['unused'], 'genuine second argument home'
    nonstack = {a: b for a, b in mem.items() if a not in stack}
    return (nonstack, calls, reads, writes), coverage, edges, initial


def oracle(addresses, case, initial):
    """Direct byte-offset semantic model, independent of instruction decode."""
    mem = {a: b for a, b in initial.items() if not SP - 256 <= a < SP + 96}
    calls = []
    def get(name):
        at = addresses[name]
        return int.from_bytes(bytes(mem[at + i] for i in range(4)), 'big')
    def put(at, value, size=4):
        mem.update({at + i: b for i, b in enumerate((value & ((1 << (8 * size)) - 1)).to_bytes(size, 'big'))})
    if not case['disabled'] and (case['old_result'] == 0xFFFFFFFF or case['index'] != case['last_index']):
        h = case['handle']
        calls += [('audio_frame_sync', ((case['index'] + 10) & MASK, 0, 0, 0, 0)),
                  ('display_list_alloc', (h,)), ('func_80096288', (h, 0, 0)),
                  ('audio_loop_control', (0x60700000 + h * 0x1000, 0))]
        put(addresses['D_80151AD4'], h)
        if case['mutate_slot']:
            put(addresses['D_80151AD4'], case['next_handle'])
        put(addresses['D_80156D38'] + h * 20 + 2, 1, 1)
        h = get('D_80151AD4')
        calls.append(('func_80096288', (h, 0, 0)))
        stream = 0x60700000 + h * 0x1000
        put(addresses['D_80151ADC'], stream)
        calls.append(('func_8001536C', (0x60602000, case['identifier'], 0x60603000, 0x60604000, 0x60605000)))
        identifier = case['identifier']
        if case['mutate_registry']:
            identifier ^= 0xFFFF
            stream = 0x60908000
            put(addresses['D_8011F070'] + case['index'] * 4 + 2, identifier, 2)
            put(addresses['D_80151ADC'], stream)
        calls.append(('func_800156E8', (identifier, case['index'] & 0xFFFF, stream, 0)))
        put(addresses['D_8011EAA0'], case['result'])
        put(addresses['D_80151A6C'], case['index'])
    return mem, calls


def cases():
    rng = random.Random(0x979A0)
    for disabled in (0, 1, 127, 128, 255):
        for old in (0, 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF):
            for index in (0, 1, 63, 65535, 65536, 65537):
                for equal in (False, True):
                    for change in range(4):
                        yield dict(disabled=disabled, old_result=old, index=index,
                                   last_index=index if equal else index + 1,
                                   unused=rng.getrandbits(32), identifier=rng.choice((0, 1, 32767, 32768, 65535)),
                                   handle=rng.randrange(8), next_handle=rng.randrange(8),
                                   mutate_slot=bool(change & 1), mutate_registry=bool(change & 2),
                                   registry_return=rng.getrandbits(32), result=rng.choice((0, 1, 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF)))


def main():
    targets, addresses = native(ROOT)
    assert targets[FN] == score.targets()[FN]
    group = PACKET / 'group'
    spec = json.loads((group / 'group.json').read_text())
    source_hashes = {f: digest((group / f).read_bytes()) for f in spec['files']}
    receipt = {'base': 'dea99f09ab19b1d3b324ed7097162f7b378e7096', 'status': 'PASS: bounded NONMATCH research only',
               'gain_bytes': 0, 'source_sha256': source_hashes, 'accepted_context': {}, 'negative_controls': {}}
    with tempfile.TemporaryDirectory(prefix='sequence-start-independent-') as td:
        work = Path(td)
        obj = work / 'candidate.o'
        score.compile_group(group, obj)
        elf = Elf(obj)
        for symbol in elf.symbols:
            match = re.fullmatch(r'(?:D|func)_([0-9A-Fa-f]{8})', symbol['name'])
            if match:
                address = int(match[1], 16)
                assert symbol['name'] not in addresses or addresses[symbol['name']] == address
                addresses[symbol['name']] = address
        assert {s['name'] for s in elf.symbols if s['type'] == 2 and s['section'] == elf.text_index} == set(spec['members'])
        assert not any(s['size'] for s in elf.sections if s['name'] in ('.data', '.rodata', '.rdata', '.sdata', '.sbss', '.bss'))
        total_bodies = sum(elf.function(name)['size'] for name in spec['members'])
        occupied = set()
        for name in spec['members']:
            fn = elf.function(name)
            span = set(range(fn['value'], fn['value'] + fn['size']))
            assert not occupied & span
            occupied |= span
        raw_text = elf.data(elf.text)
        assert all(raw_text[offset] == 0 for offset in range(len(raw_text)) if offset not in occupied)
        allwords = score.text_words(obj)
        linked = {}
        for name in spec['members']:
            fn = elf.function(name)
            lo, hi = fn['value'], fn['value'] + fn['size']
            resolved, masks, unresolved, unverified, errors = score.relocate(obj, allwords, lo, hi, addresses)
            assert not (masks or unresolved or unverified or errors), (name, masks, unresolved, unverified, errors)
            words = resolved[lo // 4:hi // 4]
            linked[name], result = link_function(obj, name, addresses, words, work / ('gnu_' + name))
            comparison = score.compare(obj, name, show=0)
            if name != FN:
                assert comparison.differing == 0 and words == targets[name], name
                receipt['accepted_context'][name] = {'bytes': fn['size'], 'strict_match': True, 'gnu_full_extent_match': True}
            else:
                assert fn['size'] == 288 and len(targets[name]) * 4 == 292 and comparison.differing == 12
                differences = [i for i in range(max(len(words), len(targets[name]))) if i >= len(words) or i >= len(targets[name]) or words[i] != targets[name][i]]
                assert len(differences) == 12
                receipt['caller'] = {'native_bytes': 292, 'candidate_elf_bytes': 288, 'differing_words': 12,
                                     'differing_word_indexes': differences, 'gnu_relocations': len(result['relocations']),
                                     'gnu_body_sha256': result['body_sha256'], 'strict_match': False}
        receipt['full_object'] = {'function_count': len(spec['members']), 'body_bytes': total_bodies,
                                  'text_bytes': elf.text['size'], 'text_padding_bytes': elf.text['size'] - total_bodies,
                                  'owned_data_bytes': 0, 'all_defined_functions_checked': True}
        # ABI context controls; sources are mutated only inside this temporary directory.
        for label in ('kept_validator', 'remove_real_second_formal', 'caller_without_context'):
            ctrl = work / label
            shutil.copytree(group, ctrl)
            cs = dict(spec)
            if label == 'kept_validator':
                cs['keep'] = spec['keep'] + ['func_80096288']
            elif label == 'remove_real_second_formal':
                path = ctrl / (FN + '.c')
                text = path.read_text()
                assert text.count('s32 arg0, s32 arg1') == 1
                path.write_text(text.replace('s32 arg0, s32 arg1', 's32 arg0'))
            else:
                cs.update(files=[FN + '.c'], members=[FN], keep=[FN])
            (ctrl / 'group.json').write_text(json.dumps(cs))
            co = work / (label + '.o')
            score.compile_group(ctrl, co)
            comparison = score.compare(co, FN, show=0)
            assert comparison.differing > 12, (label, comparison.differing)
            receipt['negative_controls'][label] = {'differing_words': comparison.differing, 'rejected': True}
        # A wrong global relocation is rejected by the independent GNU verifier.
        wrong = dict(addresses)
        wrong['D_8011EAA0'] += 4
        try:
            link_function(obj, FN, wrong, linked[FN], work / 'wrong_global')
        except AssertionError as exc:
            assert 'GNU linked body differs' in str(exc)
        else:
            raise AssertionError('wrong relocation accepted')
        receipt['negative_controls']['wrong_global_address'] = {'rejected': True}
        native_code = {n: targets[n] for n in (FN, 'func_80096288', 'display_list_alloc')}
        candidate_code = {n: linked[n] for n in native_code}
        host_source = work / 'host.c'
        host_source.write_text((group / (FN + '.c')).read_text() + '\n' + (HERE / 'host_harness.c').read_text())
        host_library = work / 'host.so'
        subprocess.run(['cc', '-std=c89', '-O2', '-fPIC', '-shared', '-Wall', '-Wextra',
                        '-Werror=incompatible-pointer-types', '-fsanitize=undefined',
                        '-fno-sanitize-recover=all', str(host_source), '-o', str(host_library)],
                       check=True, capture_output=True)
        dll = ctypes.CDLL(str(host_library))
        host_case = dll.independent_host_case
        host_case.argtypes = [ctypes.POINTER(ctypes.c_uint32)]
        host_case.restype = ctypes.c_int
        native_coverage, candidate_coverage, native_edges, candidate_edges = set(), set(), set(), set()
        total = 0
        for case in cases():
            nr, nc, ne, initial = execute(native_code, addresses, case)
            cr, cc, ce, _ = execute(candidate_code, addresses, case)
            assert nr == cr, ('native/candidate behavior', total)
            assert nr[:2] == oracle(addresses, case, initial), ('oracle', total)
            host_input = (ctypes.c_uint32 * 12)(*[case[key] for key in
                ('disabled', 'old_result', 'index', 'last_index', 'unused', 'identifier',
                 'handle', 'next_handle', 'mutate_slot', 'mutate_registry', 'registry_return', 'result')])
            assert host_case(host_input) == 0, ('unchanged host source', total)
            native_coverage |= nc
            candidate_coverage |= cc
            native_edges |= ne
            candidate_edges |= ce
            total += 1
        assert {BASE + 4 * i for i in range(73)} <= native_coverage
        assert {BASE + 4 * i for i in range(72)} <= candidate_coverage
        for edge_set in (native_edges, candidate_edges):
            for at in {p for p, result in edge_set if BASE <= p < BASE + 292}:
                assert {(at, False), (at, True)} <= edge_set
        receipt['behavior'] = {'cases': total, 'native_and_candidate_executions': total * 2,
                               'native_caller_instructions_covered': 73, 'candidate_caller_instructions_covered': 72,
                               'all_caller_conditional_branches_both_outcomes': True,
                               'real_display_list_alloc_and_validator_executed': True,
                               'ordinary_external_caller_saves_poisoned': True,
                               'nonstack_reads_writes_calls_memory_equal': True,
                               'independent_byte_offset_oracle_equal': True,
                               'unchanged_c89_ubsan_host_cases': total,
                               'saved_registers_and_real_second_argument_home': True}
        receipt['limitations'] = ['Finite accessible cases and explicit external-service stubs; not full runtime proof.',
                                  'No source matching credit, promotion, splicing, whole-image or ROM hash claim.',
                                  'GNU links complete ELF function extents separately; this is not the complete production unit link.',
                                  'No volatility, fabricated formal, helper substitution, assembly patch, or instruction padding repair.']
    (HERE / 'review.json').write_text(json.dumps(receipt, indent=2) + '\n')
    print(json.dumps(receipt, indent=2))


if __name__ == '__main__':
    main()
