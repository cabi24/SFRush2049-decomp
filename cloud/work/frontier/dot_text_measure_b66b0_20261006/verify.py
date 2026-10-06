#!/usr/bin/env python3
"""Portable complete-function proof. No target bytes are serialized in receipts."""
import argparse
import hashlib
import json
import os
import random
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
GROUP = ROOT / 'cloud/matches/dot_text_measure_b66b0_20261006'
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
FN = 'func_800B66B0'
CALLER = 'menu_input_process'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score


def require(condition, message):
    if not condition:
        raise AssertionError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def function_text(source, signature):
    start = source.index(signature)
    left = source.index('{', start)
    depth = 0
    for end in range(left, len(source)):
        depth += (source[end] == '{') - (source[end] == '}')
        if depth == 0:
            return source[start:end + 1]
    raise AssertionError('incomplete function')


def historical_source(history):
    def read(path):
        return subprocess.check_output(['git', '-C', str(history), 'show', BASE + ':' + path], text=True)
    own = (GROUP / 'group.c').read_text()
    original = read('src/blob/groups/codex_defaults_a16/group.c')
    require(function_text(own, 'void menu_input_process(u8 *arg0, s16 arg1) {') ==
            function_text(original, 'void menu_input_process(u8 *arg0, s16 arg1) {'),
            'complete actual caller changed from production-context base')
    old_helper = read('cloud/work/frontier/agentA/func_800B66B0/best.c')
    signature = 's32 func_800B66B0(u8 *str, s16 max) {'
    require(function_text(own, signature) == function_text(old_helper, signature),
            'archived helper body changed')
    require(FN not in json.loads(read('blob_matched.lock.json')), 'already locked at base')
    return {'base_commit': BASE,
            'caller_source': 'src/blob/groups/codex_defaults_a16/group.c',
            'helper_source': 'cloud/work/frontier/agentA/func_800B66B0/best.c',
            'caller_body_unchanged': True, 'helper_body_unchanged': True}


def historical_targets(history):
    """Native caller context is fixed at BASE, not tied to later integration."""
    def read(path):
        return subprocess.check_output(['git', '-C', str(history), 'show', BASE + ':' + path], text=True)
    manifest = read('asm/us/blob/SHA256SUMS')
    targets = {}
    for line in manifest.splitlines():
        name = line.split()[-1]
        if not name.endswith('.s'):
            continue
        current = None
        for source_line in read('asm/us/blob/' + name).splitlines():
            match = re.match(r'\.section \.text\.(\S+?),', source_line.strip())
            if match:
                current = targets.setdefault(match[1], [])
            elif source_line.strip().startswith('.section'):
                current = None
            match = re.match(r'\s*\.word\s+(0x[0-9A-Fa-f]+)', source_line)
            if match and current is not None:
                current.append(int(match[1], 16))
    addresses = json.loads(read('asm/us/blob/symbols.json'))['symbols']
    addresses = {name: int(value, 0) if isinstance(value, str) else value for name, value in addresses.items()}
    return targets, addresses

def signed(value):
    return value - (1 << 32) if value & 0x80000000 else value


def oracle(data, raw):
    maximum = raw & 65535
    if maximum >= 32768:
        maximum -= 65536
    budget = None if maximum < 0 else maximum + 1
    if data[0] == 255:
        count = 0
        while True:
            count += 1
            pos = count * 2 - 1
            if data[pos + 1] == 0 and data[pos] == 0:
                return 1 + 2 * count
            if budget is not None and count >= budget:
                return 1 + 2 * count
    else:
        for pos, value in enumerate(data):
            if value == 0 or (budget is not None and pos + 1 >= budget):
                return pos + 1
    raise AssertionError('unterminated oracle fixture')


def execute(words, data, raw, coverage=None, branches=None):
    origin = 0x100000
    stack = 0x200000
    stop = 0x300000
    r = [((0x76543210 + i * 0x1020304) ^ raw) & 0xffffffff for i in range(32)]
    r[0] = 0
    r[4], r[5], r[29], r[31] = origin, raw, stack, stop
    before = list(r)
    homes = bytearray(b'\xa5' * 32)
    pc = 0
    pending = None
    steps = 0
    while pc != stop:
        require(pc % 4 == 0 and 0 <= pc < len(words) * 4, 'instruction fetch outside full function')
        steps += 1
        require(steps <= max(2048, len(data) * 25), 'instruction budget exceeded')
        if coverage is not None:
            coverage.add(pc)
        word = words[pc // 4]
        op, rs, rt, rd, shift, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word >> 6 & 31, word & 63
        immediate = word & 65535
        simm = immediate - 65536 if immediate & 32768 else immediate
        next_pc = pc + 4
        prior_pending = pending
        pending = None
        is_branch = False
        if op == 0:
            if fn == 0: r[rd] = r[rt] << shift
            elif fn == 3: r[rd] = signed(r[rt]) >> shift
            elif fn == 8:
                is_branch = True
                pending = r[rs]
            elif fn == 33: r[rd] = r[rs] + r[rt]
            elif fn == 37: r[rd] = r[rs] | r[rt]
            elif fn == 42: r[rd] = int(signed(r[rs]) < signed(r[rt]))
            else: raise AssertionError('unsupported SPECIAL opcode')
        elif op == 9: r[rt] = r[rs] + simm
        elif op == 36:
            address = (r[rs] + simm) & 0xffffffff
            require(origin <= address < origin + len(data), 'unmapped input byte')
            r[rt] = data[address - origin]
        elif op == 43:
            address = (r[rs] + simm) & 0xffffffff
            require(address == stack + 4, 'unexpected memory write')
            homes[4:8] = struct.pack('>I', r[rt])
        elif op in (4, 5, 20) or (op == 1 and rt == 2):
            is_branch = True
            if op in (4, 20): taken = r[rs] == r[rt]
            elif op == 5: taken = r[rs] != r[rt]
            else: taken = signed(r[rs]) < 0
            if branches is not None:
                branches.setdefault(pc, set()).add(taken)
            if taken: pending = pc + 4 + simm * 4
            elif op in (1, 20): next_pc = pc + 8
            else: pending = pc + 8
        else: raise AssertionError('unsupported instruction')
        if prior_pending is not None:
            require(not is_branch, 'branch in delay slot')
            next_pc = prior_pending
        if op == 0 and fn != 8:
            r[rd] &= 0xffffffff
        elif op in (9, 36):
            r[rt] &= 0xffffffff
        r[0] = 0
        pc = next_pc
    allowed = {1, 2, 3, 4, 5, 6, 14, 15, 24, 25}
    require(all(r[i] == before[i] for i in range(32) if i not in allowed), 'preserved register changed')
    expected_home = bytearray(b'\xa5' * 32)
    expected_home[4:8] = struct.pack('>I', raw)
    require(homes == expected_home, 'argument home or stack canary mismatch')
    return r[2]


def fixtures():
    # Every s16 bit pattern, with noisy incoming upper bits, in both encodings.
    for maximum in range(65536):
        raw = ((maximum * 40503 + 17) & 65535) << 16 | maximum
        yield b'abc\0', raw
        yield b'\xff\x12\0\0\x13\0\0', raw
    # Every possible first wide pair; termination requires both bytes zero.
    for pair in range(65536):
        yield bytes([255, pair >> 8, pair & 255, 0, 0]), 0xabcfffff
    for first in range(255):
        for maximum in (0, 1, 2, 32767, 32768, 65535):
            yield bytes([first, 1, 0]), maximum
    rng = random.Random(0xB66B0)
    for _ in range(1024):
        length = rng.randrange(1, 257)
        maximum = rng.choice([-32768, -1, 0, 1, length - 1, length, length + 1, 32767])
        wide = rng.randrange(2)
        if wide:
            data = bytes([255]) + bytes(rng.randrange(1, 256) for _ in range(length * 2)) + b'\0\0'
        else:
            data = bytes(rng.randrange(1, 255) for _ in range(length)) + b'\0'
        yield data, rng.randrange(65536) << 16 | (maximum & 65535)
    for length in (1, 2, 255, 256, 32767, 32768):
        for maximum in (0, 1, 32766, 32767, 32768, 65535):
            yield b'x' * length + b'\0', 0xffff0000 | maximum
            yield b'\xff' + b'\x01\x02' * length + b'\0\0', maximum


def run_behavior(native, linked, work):
    coverage, branch_coverage = set(), {}
    fixture_file = work / 'fixtures.bin'
    cases = 0
    with fixture_file.open('wb') as out:
        out.write(b'\0' * 4)
        for data, raw in fixtures():
            expected = oracle(data, raw)
            for words in (native, linked):
                require(execute(words, data, raw, coverage, branch_coverage) == expected, 'native/linked behavior differs from oracle')
            out.write(struct.pack('>III', raw, len(data), expected) + data)
            cases += 1
        out.seek(0); out.write(struct.pack('>I', cases))
    require(len(coverage) == len(native), 'incomplete native instruction coverage')
    require(all(outcomes == {False, True} for outcomes in branch_coverage.values()), 'incomplete branch outcomes')
    host = work / 'host'
    subprocess.run(['gcc', '-std=c89', '-O2', '-fstrict-aliasing', '-fsanitize=undefined,bounds',
                    '-fno-sanitize-recover=all', '-ffunction-sections', '-fdata-sections',
                    '-I', str(ROOT), str(PACKET / 'host.c'), '-Wl,--gc-sections', '-o', str(host)], check=True, capture_output=True, text=True)
    result = subprocess.run([str(host), str(fixture_file)], check=True, capture_output=True, text=True)
    require(str(cases) in result.stdout, 'host count absent')
    # Compile independent wrong-source mutants; none changes the published group.
    host_mutants = 0
    source = (GROUP / 'group.c').read_text()
    signature = 's32 func_800B66B0(u8 *str, s16 max) {'
    start = source.index(signature)
    for tag, old, new in [('zero_limit', 'max < 0', 'max <= 0'),
                          ('wide_terminator', 'str[len - 1] == 0 && str[len - 2] == 0', 'str[len - 1] == 0 || str[len - 2] == 0'),
                          ('tag', '*str == 0xFF', '*str == 0xFE'),
                          ('length', 'return len;', 'return len - 1;')]:
        require(old in source[start:], 'mutation source absent')
        mutant_root = work / tag
        mutant_file = mutant_root / GROUP.relative_to(ROOT) / 'group.c'
        mutant_file.parent.mkdir(parents=True)
        mutant_file.write_text(source[:start] + source[start:].replace(old, new))
        binary = mutant_root / 'host'
        subprocess.run(['gcc', '-std=c89', '-O2', '-fstrict-aliasing', '-fsanitize=undefined,bounds',
                        '-fno-sanitize-recover=all', '-ffunction-sections', '-fdata-sections',
                        '-I', str(mutant_root), '-I', str(ROOT), str(PACKET / 'host.c'),
                        '-Wl,--gc-sections', '-o', str(binary)], check=True, capture_output=True, text=True)
        rejected = subprocess.run([str(binary), str(fixture_file)], capture_output=True, text=True)
        require(rejected.returncode != 0, 'host wrong-source mutant escaped: ' + tag)
        host_mutants += 1
    # Native mutations test a changed return, wrong mode, and truncated extent.
    negatives = 0
    for mutation in ('mode', 'return', 'truncated', 'unknown'):
        altered = list(native)
        if mutation == 'mode': altered[3] = (altered[3] & 0xffff0000) | 254
        elif mutation == 'return': altered[-1] = 0x24020000
        elif mutation == 'truncated': altered = altered[:-1]
        else: altered[0] = 0xffffffff
        caught = False
        for data, raw in ((b'abc\0', 0xffff), (b'\xff\x01\x02\0\0', 0xffff)):
            try:
                caught |= execute(altered, data, raw) != oracle(data, raw)
            except AssertionError:
                caught = True
        require(caught, 'native negative control escaped: ' + mutation)
        negatives += 1
    return {'fixtures': cases, 'native_and_linked_executions': 2 * cases,
            'host_c89_ubsan_bounds_cases': cases, 'native_instruction_offsets': len(coverage),
            'conditional_branches_both_outcomes': len(branch_coverage),
            'native_negative_controls_rejected': negatives,
            'compiled_wrong_source_mutants_rejected': host_mutants}


def proof(history, behavior=True):
    require((score.IDO / 'cc').is_file() and shutil.which('mips-linux-gnu-ld'), 'pinned IDO and MIPS GNU linker required')
    source = (GROUP / 'group.c').read_text()
    require(source.splitlines()[0] == '/* flags: ' + FLAGS + ' */', 'bare O3 header changed')
    spec = json.loads((GROUP / 'group.json').read_text())
    require(spec['flags'] == FLAGS and spec['claims'] == [FN] and spec['keep'] == [CALLER], 'group recipe changed')
    context = historical_source(history)
    native = score.targets()[FN]
    base_targets, addresses = historical_targets(history)
    require(len(native) == 38 and addresses[FN] == 0x800b66b0, 'native identity changed')
    calls = []
    for name, words in base_targets.items():
        for index, word in enumerate(words):
            if word >> 26 in (2, 3) and (word & 0x3ffffff) << 2 == 0xb66b0:
                calls.append({'caller': name, 'address': hex(addresses[name] + index * 4)})
    require(len(calls) == 2 and {x['caller'] for x in calls} == {CALLER}, 'base direct-call census changed')
    with tempfile.TemporaryDirectory(prefix='b66b0-proof-') as tmp:
        work = Path(tmp)
        obj = work / 'whole.o'
        score.compile_group(GROUP, obj)
        data, sections = score._elf(obj)
        symbols = [sym for i, sec in enumerate(sections) if sec['type'] == 2 for sym in score._symbol_table(data, sections, i)]
        funcs = [sym for sym in symbols if sym['type'] == 2 and sym['section'] == score._text_index(sections)]
        require({s['name'] for s in funcs} == {FN, CALLER}, 'unexpected function/stub')
        target = next(s for s in funcs if s['name'] == FN)
        require(target['size'] == len(native) * 4 and target['value'] == 0, 'full ELF target extent mismatch')
        text = score.text_words(obj)
        got, masks, unresolved, unverified, errors = score.relocate(obj, text, 0, target['size'], addresses)
        require(not (masks or unresolved or unverified or errors), 'unclean target relocation proof')
        require(got[:38] == native, 'full target words differ')
        target_relocations = sum(1 for sec in sections if sec['type'] == 9 and sec['info'] == score._text_index(sections)
                                 for offset in range(sec['off'], sec['off'] + sec['size'], 8)
                                 if struct.unpack_from('>I', data, offset)[0] < target['size'])
        require(target_relocations == 0, 'unexpected target relocation')
        alloc_data = [sec for sec in sections if sec['name'] in ('.data', '.rodata', '.rdata', '.bss', '.sdata', '.sbss') and sec['size']]
        require(not alloc_data, 'unexpected owned data')
        end = max(s['value'] + s['size'] for s in funcs)
        alignment = struct.pack('>' + str(len(text)) + 'I', *text)[end:]
        require(alignment == b'\0' * len(alignment), 'nonzero alignment tail')
        externs = {s['name']: addresses.get(s['name'], score.address_named(s['name'])) for s in symbols if s['section'] == 0 and s['name']}
        require(all(value is not None for value in externs.values()), 'unbound object external')
        script = work / 'link.ld'
        script.write_text('SECTIONS { . = 0x800b66b0; .text : { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.options) } }\n' +
                          '\n'.join(name + ' = ' + hex(value) + ';' for name, value in sorted(externs.items())))
        linked = work / 'linked.elf'
        subprocess.run(['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(linked), str(obj)], check=True, capture_output=True, text=True)
        linked_words = score.text_words(linked)
        require(linked_words[:38] == native, 'independent GNU target byte mismatch')
        all_relocated = score.relocate(obj, text, 0, len(text) * 4, addresses)
        require(not any(all_relocated[i] for i in (1, 2, 3, 4)), 'whole object relocation uncertainty')
        require(all_relocated[0] == linked_words, 'whole-object project/GNU relocation mismatch')
        caller_symbol = next(sym for sym in funcs if sym['name'] == CALLER)
        caller_native = base_targets[CALLER]
        caller_body = all_relocated[0][caller_symbol['value'] // 4:
                                      (caller_symbol['value'] + caller_symbol['size']) // 4]
        caller_differences = sum(a != b for a, b in zip(caller_native, caller_body)) + max(0, len(caller_native) - len(caller_body))
        caller_excess = sum(word != 0 for word in caller_body[len(caller_native):])
        own_hashes = {str(path.relative_to(ROOT)): digest(path.read_bytes()) for path in
                      [GROUP / 'group.c', GROUP / 'group.json', PACKET / 'verify.py', PACKET / 'host.c']}
        receipt = {'schema': 1, 'result': 'MATCH', 'target': FN, 'native_start': hex(addresses[FN]),
                   'native_end': hex(addresses[FN] + target['size']), 'native_bytes': target['size'],
                   'native_sha256': digest(struct.pack('>38I', *native)), 'compiled_sha256': digest(struct.pack('>38I', *got[:38])),
                   'flags': FLAGS, 'mandatory_backend_flag': score.R4300_AS1,
                   'context': context, 'native_direct_calls': calls, 'own_files': own_hashes,
                   'elf_function_bytes': {s['name']: s['size'] for s in funcs}, 'text_bytes': len(text) * 4,
                   'zero_alignment_bytes': len(alignment), 'target_relocations': target_relocations,
                   'whole_object_relocations': sum(sec['size'] // 8 for sec in sections if sec['type'] == 9),
                   'external_bindings': len(externs), 'owned_data_bytes': 0, 'gnu_full_target_equal': True,
                   'gnu_whole_object_relocations_equal': True,
                   'caller_context': {'result': 'NONMATCH', 'comparison_base_commit': BASE,
                                      'differing_words': caller_differences,
                                      'native_words': len(caller_native), 'extra_words': caller_excess,
                                      'unresolved': [], 'unverified': [], 'errors': []},
                   'accepted_byte_gain': 0, 'rom_coverage_gain': 0}
        if behavior:
            receipt['behavior'] = run_behavior(native, linked_words[:38], work)
        return receipt


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--history-repo', type=Path, default=ROOT)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--check', action='store_true')
    parser.add_argument('--no-behavior', action='store_true')
    args = parser.parse_args()
    result = proof(args.history_repo, not args.no_behavior)
    if args.check:
        require(not args.no_behavior, '--check requires complete behavior replay')
        require(result == json.loads((PACKET / 'verification.json').read_text()), 'fresh receipt differs')
    rendered = json.dumps(result, indent=2, sort_keys=True) + '\n'
    if args.output:
        args.output.write_text(rendered)
    print(rendered, end='')

if __name__ == '__main__':
    main()
