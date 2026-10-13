#!/usr/bin/env python3
"""Replay the 168-byte static match without a ROM or coordinator database."""

def require(condition, message):
    if not condition:
        raise AssertionError(message)
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
FN = 'func_80010DD8'
TU = 'lib_11640'
SOURCE = ROOT / 'cloud/matches/boot_tail/func_80010DD8.c'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'

def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module

def sha(data):
    return hashlib.sha256(data).hexdigest()

def inputs():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    words = score.targets()[FN]
    extent = next((row for row in json.loads((score.ASM_DIR / 'extents.json').read_text())['functions'] if row['name'] == FN))
    names = ('D_80038018', 'D_800382F4', 'D_800382E8', 'D_800382F0')
    symbols = score.image_symbols()
    return {'source_sha256': sha(SOURCE.read_bytes()), 'verify_sha256': sha(Path(__file__).read_bytes()), 'packet_files': {n: sha((HERE / n).read_bytes()) for n in ('native.py', 'host.c', 'semantics.py', 'experiments.py', 'experiments.json')}, 'focused_test_sha256': sha((ROOT / 'tests/cloud/test_static_10dd8_lifetimes.py').read_bytes()), 'neighbor_source_sha256': sha((ROOT / 'cloud/work/frontier/w22c/sources/func_80010E80.c').read_bytes()), 'target_words_sha256': sha(struct.pack('>%dI' % len(words), *words)), 'target_extent': extent, 'target_symbols': {n: symbols.get(n, score.address_named(n)) for n in names}}

def command(args):
    proc = subprocess.run([str(x) for x in args], cwd=ROOT, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    require(proc.returncode == 0, 'command failed: ' + proc.stdout + proc.stderr)
    return proc.stdout

def signature(obj):
    """Allocated code/data and symbolic REL rows, excluding debug and ABI metadata."""
    data, sections = score._elf(obj)
    included = {'.text', '.data', '.rodata', '.rdata', '.bss', '.sdata', '.sbss', '.lit4', '.lit8'}
    result = {}
    for index, sec in enumerate(sections):
        if sec['name'] not in included or not sec['size']:
            continue
        payload = bytes(sec['size']) if sec['type'] == 8 else data[sec['off']:sec['off'] + sec['size']]
        relocs = []
        for reloc in sections:
            if reloc['type'] != 9 or reloc['info'] != index:
                continue
            table = score._symbol_table(data, sections, reloc['link'])
            for offset in range(reloc['off'], reloc['off'] + reloc['size'], 8):
                location, info = struct.unpack_from('>II', data, offset)
                sym = table[info >> 8]
                relocs.append([location, info & 255, sym['name'], sym['value']])
        result[sec['name']] = {'size': sec['size'], 'sha256': sha(payload), 'relocations': sorted(relocs)}
    return result

def function_extent(obj):
    data, sections = score._elf(obj)
    symbols = [symbol for i, section in enumerate(sections) if section['type'] == 2
               for symbol in score._symbol_table(data, sections, i) if symbol['name'] == FN]
    require(len(symbols) == 1 and symbols[0]['size'] == 168, 'wrong ELF function extent')


def no_owned_data(obj, context):
    require(not context.data_sections(obj), 'unexpected allocated data')


def expect_rejection(action, message):
    try:
        action()
    except AssertionError:
        return True
    raise AssertionError('negative control escaped: ' + message)


def run():
    pinned = inputs()
    require(pinned['target_extent']['size'] == 168, 'verification requirement failed')
    cc = compilers.tu_cc(TU, score.IDO / 'cc')
    if not cc.is_file():
        raise SystemExit(str(cc) + ' missing: run tools/cloud/setup.sh (or install IDO 7.1)')
    require(compilers.identify_dir(cc.parent) == 'ido7.1', 'present compiler is not stock IDO 7.1')
    original_ido = score.IDO
    score.IDO = cc.parent
    context = load('static_10dd8_context', ROOT / 'cloud/work/boot_tail_promotion/context_check.py')
    context.CC = cc
    context.COMPILER = 'ido7.1'
    with tempfile.TemporaryDirectory(prefix='static-10dd8-') as raw:
        work = Path(raw)
        obj = work / 'single.o'
        score.compile_single(SOURCE, FLAGS, obj)
        proof = score.compare(obj, FN, show=0)
        require(proof.accepted() and proof.total == 42 and (proof.extra_words == 0), 'verification requirement failed')
        words = score.text_words(obj)
        resolved, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, len(words) * 4, score.image_symbols())
        require(not any((masks, unresolved, unverified, errors)), 'verification requirement failed')
        require(resolved[:42] == score.targets()[FN] and (not any(resolved[42:])), 'verification requirement failed')
        no_owned_data(obj, context)
        function_extent(obj)
        # An otherwise exact object with shortened ELF extent must be refused.
        data, sections = score._elf(obj)
        broken = bytearray(data)
        for index, section in enumerate(sections):
            if section['type'] == 2:
                for position, symbol in enumerate(score._symbol_table(data, sections, index)):
                    if symbol['name'] == FN:
                        struct.pack_into('>I', broken, section['off'] + position * 16 + 8, 164)
        short_object = work / 'short_symbol.o'
        short_object.write_bytes(broken)
        extent_negative = expect_rejection(lambda: function_extent(short_object), 'short ELF extent')
        # Defining an unowned global must not be accepted as harmless context.
        allocated_source = work / 'unowned_data.c'
        original = SOURCE.read_text()
        anchor = 'extern unsigned int D_800382F4;'
        require(original.count(anchor) == 1, 'global definition anchor changed')
        allocated_source.write_text(original.replace(anchor, 'unsigned int D_800382F4;'))
        allocated_object = work / 'unowned_data.o'
        score.compile_single(allocated_source, FLAGS, allocated_object)
        allocation_negative = expect_rejection(lambda: no_owned_data(allocated_object, context), 'unowned allocation')
        native = ROOT / ('asm/us/nonmatchings/rom/' + TU + '/' + FN + '.s')
        assembly = work / 'target.s'
        assembly.write_text((ROOT / 'tools/asm-processor/prelude.inc').read_text() + '\n.text\n.type ' + FN + ', @function\n' + native.read_text())
        target = work / 'target.o'
        command(['mips-linux-gnu-as', '-march=vr4300', '-mabi=32', '-Iinclude', '-o', target, assembly])
        require(context.disasm(target, FN) == context.disasm(obj, FN), 'verification requirement failed')
        require(signature(target) == signature(obj), 'verification requirement failed')
        control = work / 'control.c'
        text = SOURCE.read_text()
        needle = 'count = 192 - count % 192 + count;'
        require(text.count(needle) == 1, 'verification requirement failed')
        control.write_text(text.replace(needle, 'count += 192 - count % 192;'))
        control_obj = work / 'control.o'
        score.compile_single(control, FLAGS, control_obj)
        rejected = score.compare(control_obj, FN, show=0)
        require(not rejected.accepted() and rejected.differing == 36 and (rejected.extra_words == 0), 'verification requirement failed')
        names = pinned['target_symbols']
        linker_script = work / 'function.ld'
        linker_script.write_text('SECTIONS { .text 0x80010dd8 : SUBALIGN(4) { *(.text) } }\n' + ''.join(('%s = 0x%x;\n' % (n, a) for n, a in names.items())))
        linked_words = []
        for label, item in (('source', obj), ('target', target)):
            linked = work / (label + '.elf')
            command(['mips-linux-gnu-ld', '-T', linker_script, '-o', linked, item])
            linked_words.append(score.text_words(linked)[:42])
        require(linked_words[0] == linked_words[1] == score.targets()[FN], 'verification requirement failed')
        native = load('static_10dd8_native', HERE / 'native.py')
        semantics = load('static_10dd8_semantics', HERE / 'semantics.py')
        semantic_result = semantics.run(SOURCE, linked_words[1], work, native)
        row = context.check(FN, work, str(SOURCE.relative_to(ROOT)))
        require(row['status'] == 'ok', row)
        row['non_elided_disassembly_words'] = row.pop('words')
        row.update(function=FN, compiler='ido7.1', flagset=FLAGS)
        baseline = ROOT / ('src/rom/' + TU + '.c')
        pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/' + TU + '/' + FN + '.s")'
        temporary = work / (TU + '.c')
        if pragma in baseline.read_text():
            promote = load('static_10dd8_promote', ROOT / 'cloud/work/boot_tail_promotion/promote_batch.py')
            promote.PREFIX_DEDUP = {'rom/' + TU}
            promote.splice([{'fn': FN, 'seg': {'rom_tu': 'rom/' + TU, 'flagset': FLAGS}, 'src': str(SOURCE.relative_to(ROOT)), 'row': row, 'slot': None}], out_dir=work)
        else:
            require(re.search('\\b' + FN + '\\s*\\([^;]*?\\)\\s*\\{', baseline.read_text(), re.S), 'verification requirement failed')
            temporary.write_text(baseline.read_text())
        tuples = []
        for label, source in (('before', baseline), ('after', temporary)):
            output = work / (label + '.o')
            command([sys.executable, 'tools/asm-processor/build.py', cc, '--', 'mips-linux-gnu-as', '-march=vr4300', '-mabi=32', '-Iinclude', '--', '-c', *context.cflags_for(FLAGS), '-o', output, source])
            tuples.append(signature(output))
        require(tuples[0] == tuples[1], 'current whole TU differs after scratch promotion')
        result = {'result': 'MATCH', 'population': 'static', 'function': FN, 'newly_matched_bytes': 168, 'landed_rom_bytes_claimed': 0, 'compiler': 'ido7.1', 'flags': FLAGS, 'inputs': pinned, 'canonical': proof.summary(), 'native_extent_bytes': 168, 'object_symbol_bytes': 168, 'zero_alignment_bytes': len(words) * 4 - 168, 'gnu_symbolic_relocations': len(signature(obj)['.text']['relocations']), 'gnu_object_equal': True, 'gnu_link_equal': True, 'semantics': semantic_result, 'own_data_sections': [], 'object_negative_controls': {'short_function_symbol': extent_negative, 'unowned_global_allocation': allocation_negative}, 'control_differing_words': rejected.differing, 'rom_tu_context': row, 'current_whole_tu_equal': True, 'whole_tu_text_bytes': tuples[0]['.text']['size']}
    score.IDO = original_ido
    return result
if __name__ == '__main__':
    result = run()
    if len(sys.argv) == 3 and sys.argv[1] == '--write':
        Path(sys.argv[2]).write_text(json.dumps(result, indent=2) + '\n')
    else:
        saved = json.loads((HERE / 'verification.json').read_text())
        require(saved == result, 'stale or changed packet evidence')
    print(json.dumps(result, indent=2))
