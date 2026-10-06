#!/usr/bin/env python3
"""Recompile, independently link, and exercise runtime B's player reset.

All native words come from manifest-checked repository targets. No binary,
raw instruction, or original-image input is written into this packet.
"""
import contextlib
import hashlib
import io
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

PACKET = Path(__file__).resolve().parent
SOURCE = ROOT / 'cloud/matches/ovl_b/func_8038CA24.c'
NAME = 'func_8038CA24'
ADDRESS = 0x8038CA24
SIZE = 236
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
GLOBALS = {'D_80152818': 0x80152818, 'D_80399118': 0x80399118,
           'D_80399550': 0x80399550, 'D_80399120': 0x80399120}
U32 = 0xffffffff


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def run(args, **kwargs):
    result = subprocess.run([str(a) for a in args], capture_output=True, text=True, **kwargs)
    if result.returncode:
        raise AssertionError((args, result.returncode, result.stdout, result.stderr))
    return result


def signed(value, bits):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def oracle(raw, stack):
    player = signed(raw, 16)
    writes = {}
    def put(address, value, width):
        for i in range(width):
            writes[(address + i) & U32] = (value >> (8 * (width - i - 1))) & 255
    car = (GLOBALS['D_80152818'] + player * 0x3b8) & U32
    group = (GLOBALS['D_80399550'] + player * 0x148) & U32
    slots = (GLOBALS['D_80399120'] + player * 0x10c) & U32
    for offset, value, width in [(0x38c, 0, 4), (0x3a2, 0, 1),
                               (0x3a0, 0, 1), (0x3a4, 0, 4),
                               (0x384, 8, 1), (0x385, 255, 1)]:
        put(car + offset, value, width)
    put(GLOBALS['D_80399118'] + player, 9, 1)
    put(group + 0x104, U32, 4)
    put(group + 0x138, 255, 1)
    put(group + 0x144, 0, 4)
    put(slots + 0x108, 0, 4)
    for slot in range(5):
        put(slots + slot * 4, U32, 4)
    put(stack, raw, 4)  # native O32 argument home, not global source state
    return writes


def execute(words, raw, seed=0):
    """Straight-line MIPS II subset; unexpected flow/opcodes fail closed."""
    regs = [((0x1020304 * (i + 1)) ^ seed) & U32 for i in range(32)]
    fp = [((0x51617181 * (i + 1)) ^ seed) & U32 for i in range(32)]
    regs[0] = 0
    regs[4] = raw & U32
    regs[29] = 0x70000000
    regs[31] = 0x60000000
    initial = regs[:]
    initial_fp = fp[:]
    writes, covered = {}, set()
    returning = False
    for index, word in enumerate(words):
        covered.add(index * 4)
        op = word >> 26
        rs, rt, rd = (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
        shift, fn, imm = (word >> 6) & 31, word & 63, signed(word, 16)
        address = (regs[rs] + imm) & U32
        if returning and index != len(words) - 1:
            raise AssertionError('unexpected branch delay placement')
        if op == 0:
            if fn == 0: regs[rd] = (regs[rt] << shift) & U32
            elif fn == 3: regs[rd] = (signed(regs[rt], 32) >> shift) & U32
            elif fn == 0x21: regs[rd] = (regs[rs] + regs[rt]) & U32
            elif fn == 0x23: regs[rd] = (regs[rs] - regs[rt]) & U32
            elif fn == 8:
                assert rs == 31 and regs[rs] == initial[31] and index == len(words) - 2
                returning = True
            else: raise AssertionError(('unsupported SPECIAL', fn))
        elif op == 9: regs[rt] = (regs[rs] + imm) & U32
        elif op == 15: regs[rt] = (word & 0xffff) << 16
        elif op == 0x11 and rs == 4 and (word & 0x7ff) == 0: fp[rd] = regs[rt]
        elif op in (0x28, 0x2b, 0x39):
            width = 1 if op == 0x28 else 4
            assert address % width == 0
            value = fp[rt] if op == 0x39 else regs[rt]
            for i in range(width):
                writes[(address + i) & U32] = (value >> (8 * (width - i - 1))) & 255
        else: raise AssertionError(('unsupported opcode', op))
        regs[0] = 0
    assert returning
    for r in list(range(16, 24)) + [28, 29, 30, 31]: assert regs[r] == initial[r]
    assert fp[20:] == initial_fp[20:]
    return writes, covered


def prove():
    score.ASM_DIR = ROOT / 'asm/us/ovl_b'
    native = score.targets()[NAME]
    extent_path = score.ASM_DIR / 'extents.json'
    extents = json.loads(extent_path.read_text())
    record = next(x for x in extents['functions'] if x['name'] == NAME)
    assert extents['image'] == 'B'
    assert extents['image_sha256'] == 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
    assert int(record['address'], 16) == ADDRESS
    assert record['size'] == SIZE == len(native) * 4
    symbols = score.image_symbols()
    assert symbols.get(NAME, score.address_named(NAME)) == ADDRESS
    assert all(symbols.get(k, score.address_named(k)) == v for k, v in GLOBALS.items())
    receipt = {'status': 'MATCH', 'image': 'B', 'address': hex(ADDRESS), 'bytes': SIZE,
               'image_sha256': extents['image_sha256'], 'source_sha256': sha(SOURCE),
               'flags': FLAGS + ' -Wab,-r4300_mul',
               'inputs_sha256': {str(p.relative_to(ROOT)): sha(p) for p in
                    [score.ASM_DIR / 'SHA256SUMS', score.ASM_DIR / 'ovl_b_8038a400.s',
                     score.ASM_DIR / 'symbols.json', extent_path, ROOT / 'tools/cloud/score.py',
                     ROOT / 'tools/cloud/owndata.py', PACKET / 'host_test.c', PACKET / 'verify.py']}}
    with tempfile.TemporaryDirectory(prefix='runtime-b-reset-') as tmp:
        tmp = Path(tmp)
        obj = tmp / 'candidate.o'
        score.compile_single(SOURCE, FLAGS, obj)
        with contextlib.redirect_stdout(io.StringIO()): result = score.compare(obj, NAME)
        assert result.accepted(), vars(result)
        data, sections = score._elf(obj)
        all_symbols = [s for i, sec in enumerate(sections) if sec['type'] == 2
                       for s in score._symbol_table(data, sections, i)]
        function = next(s for s in all_symbols if s['name'] == NAME and s['type'] == 2)
        assert function['value'] == 0 and function['size'] == SIZE
        shoff = struct.unpack_from('>I', data, 0x20)[0]
        shentsize = struct.unpack_from('>H', data, 0x2e)[0]
        for index, sec in enumerate(sections):
            sec['flags'] = struct.unpack_from('>I', data, shoff + index * shentsize + 8)[0]
        assert not [sec for sec in sections if sec['flags'] & 2 and sec['size'] and sec['name'] not in ('.text', '.reginfo')]
        relocs = []
        for sec in sections:
            if sec['type'] != 9: continue
            syms = score._symbol_table(data, sections, sec['link'])
            assert sec['info'] == score._text_index(sections)
            for offset in range(sec['off'], sec['off'] + sec['size'], 8):
                site, info = struct.unpack_from('>II', data, offset)
                relocs.append({'offset': site, 'type': info & 255, 'symbol': syms[info >> 8]['name']})
        assert sorted((r['offset'], r['type'], r['symbol']) for r in relocs) == [
            (0x18, 5, 'D_80152818'), (0x1c, 6, 'D_80152818'),
            (0x3c, 5, 'D_80399118'), (0x44, 5, 'D_80399550'),
            (0x68, 6, 'D_80399550'), (0x7c, 6, 'D_80399118'),
            (0x8c, 5, 'D_80399120'), (0x98, 6, 'D_80399120')]
        words = score.text_words(obj)
        assert len(words) * 4 == 240 and words[59:] == [0]
        relocated, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, SIZE, symbols)
        assert not any([masks, unresolved, unverified, errors])
        relocated = relocated[:59]
        assert relocated == native
        script = tmp / 'native.ld'
        script.write_text('SECTIONS { . = 0x8038CA24; .text : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n' +
                          '\n'.join('%s = 0x%08x;' % (k, v) for k, v in GLOBALS.items()))
        linked = tmp / 'linked.elf'
        run(['mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, obj])
        linked_data, linked_sections = score._elf(linked)
        linked_shoff = struct.unpack_from('>I', linked_data, 0x20)[0]
        linked_shentsize = struct.unpack_from('>H', linked_data, 0x2e)[0]
        linked_text_index = score._text_index(linked_sections)
        linked_address = struct.unpack_from('>I', linked_data, linked_shoff + linked_text_index * linked_shentsize + 12)[0]
        assert linked_address == ADDRESS
        raw = tmp / 'linked.bin'
        run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', linked, raw])
        linked_bytes = raw.read_bytes()
        assert len(linked_bytes) == 240 and linked_bytes[SIZE:] == bytes(4)
        linked_words = list(struct.unpack('>59I', linked_bytes[:SIZE]))
        assert linked_words == native
        # Read linked symbol values separately from the project relocation path.
        nm = run(['mips-linux-gnu-nm', '-S', linked]).stdout
        assert any(line.split()[:2] == ['8038ca24', '000000ec'] and line.split()[-1] == NAME for line in nm.splitlines())
        receipt['elf'] = {'function_size': SIZE, 'text_size': 240, 'zero_alignment_bytes_outside_function': 4,
                          'owned_data_bytes': 0, 'relocations': relocs, 'strict_differing_words': 0,
                          'independent_gnu_differing_words': 0,
                          'linked_body_sha256': hashlib.sha256(linked_bytes[:SIZE]).hexdigest()}
        covered = set()
        count = 0
        # Full low-16-bit index domain, including noncanonical O32 upper bits.
        # Outside 0..3 this characterizes hardware address arithmetic only.
        for index in range(65536):
            for high in [0x00000000, 0xa5a50000]:
                argument = high | index
                expected = oracle(argument, 0x70000000)
                got, coverage = execute(native, argument, index * 1664525)
                assert got == expected
                covered |= coverage
                count += 1
        for player in range(4):
            for seed in range(256):
                argument = player | ((seed * 257) << 16)
                for body in [native, relocated, linked_words]:
                    got, coverage = execute(body, argument, seed * 0x01010101)
                    assert got == oracle(argument, 0x70000000)
        assert covered == set(range(0, SIZE, 4))
        bad = native[:]
        bad[0] = 0xffffffff
        try: execute(bad, 0)
        except AssertionError: pass
        else: raise AssertionError('unknown opcode not rejected')
        exe = tmp / 'host'
        common = ['gcc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror', '-O2', '-fsanitize=undefined', '-fno-sanitize-recover=all']
        run(common + [PACKET / 'host_test.c', '-o', exe])
        host = run([exe])
        assert host.stdout.strip() == '16384 complete-state host cases passed' and not host.stderr
        mutants = {
            'mode_changed': ('state->mode = 8;', 'state->mode = 9;'),
            'selection_not_cleared': ('state->selection = -1;', 'state->selection = 0;'),
            'last_slot_omitted': ('i < 5', 'i < 4'),
            'group_timer_nonzero': ('D_80399550[player].timer = 0.0f;', 'D_80399550[player].timer = 1.0f;'),
            'extra_byte_clear': ('state->flags = 0;', 'state->flags = 0; state->unknown3a1 = 0;'),
        }
        rejected = []
        source = SOURCE.read_text()
        for name, (old, new) in mutants.items():
            assert source.count(old) == 1
            path = tmp / (name + '.c')
            path.write_text(source.replace(old, new))
            run(common + ['-DCANDIDATE_PATH="' + str(path) + '"', PACKET / 'host_test.c', '-o', exe])
            adverse = subprocess.run([str(exe)], capture_output=True, text=True)
            assert adverse.returncode == 1 and 'mismatch' in adverse.stderr
            score.compile_single(path, FLAGS, obj)
            with contextlib.redirect_stdout(io.StringIO()): mutant_result = score.compare(obj, NAME)
            assert not mutant_result.accepted()
            rejected.append(name)
        receipt['behavior'] = {'host_complete_state_cases': 16384,
            'host_domain': 'four accessible, nonoverlapping player/table elements, indices 0..3; arbitrary prior object bytes',
            'native_address_characterization_cases': count,
            'native_address_domain': 'all 65536 signed-low-halfword indices, two raw upper-halfword patterns; sparse mapped stores, no C validity or gameplay reachability claim outside 0..3',
            'native_project_gnu_three_way_cases': 1024, 'three_way_executions': 3072,
            'instruction_offsets_covered': len(covered), 'complete_global_write_map_and_argument_home_checked': True,
            'callee_saved_registers_checked': True, 'source_mutants_rejected': rejected,
            'unknown_opcode_rejected': True}
    # Native direct-call census is read only and belongs to the game image.
    score.ASM_DIR = ROOT / 'asm/us/blob'
    blob = score.targets()
    blob_symbols = score.image_symbols()
    callsites = []
    for name, body in blob.items():
        for index, word in enumerate(body):
            if word >> 26 == 3 and ((word & 0x3ffffff) << 2 | 0x80000000) == ADDRESS:
                base = blob_symbols.get(name, score.address_named(name))
                callsites.append({'caller': name, 'site': hex(base + index * 4)})
    assert sorted(x['site'] for x in callsites) == ['0x800c554c','0x800d34bc','0x800fba8c']
    receipt['game_direct_callers'] = callsites
    receipt['inputs_sha256']['asm/us/blob/SHA256SUMS'] = sha(score.ASM_DIR / 'SHA256SUMS')
    return receipt


def tool_provenance():
    tool_names = [('ido_' + name, score.ido(name)) for name in ['cc','cfe','ugen','uopt','as1']]
    tool_names += [(name, shutil.which(name)) for name in ['mips-linux-gnu-ld','mips-linux-gnu-objcopy','mips-linux-gnu-nm','gcc']]
    return {name: sha(path) for name, path in tool_names}


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('output', nargs='?')
    parser.add_argument('--tool-provenance', type=Path)
    args = parser.parse_args()
    receipt = prove()
    text = json.dumps(receipt, indent=2, sort_keys=True) + '\n'
    if args.tool_provenance:
        args.tool_provenance.write_text(json.dumps(tool_provenance(), indent=2, sort_keys=True) + '\n')
    if args.output:
        Path(args.output).write_text(text)
    else:
        print(text, end='')
