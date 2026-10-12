#!/usr/bin/env python3
"""Input-bound stock/GNU/native/host replay of the complete input gate."""
import argparse
from contextlib import contextmanager
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
SOURCE = ROOT / 'cloud/matches/ovl_a/func_8038F648.c'
NAME, ADDRESS, SIZE = 'func_8038F648', 0x8038F648, 236
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


elf = module('input_sequence_elf', HERE / 'elf.py')
behavior = module('input_sequence_behavior', HERE / 'behavior.py')
require = elf.require


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


@contextmanager
def targets():
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    try:
        yield
    finally:
        score.ASM_DIR = previous


def shell(*args):
    result = subprocess.run(list(map(str, args)), capture_output=True, text=True)
    require(result.returncode == 0, (args, result.stdout, result.stderr))
    return result.stdout


def inputs():
    with targets():
        manifest = score.target_manifest()
        words = score.targets()[NAME]
        metadata = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        row = next(item for item in metadata['functions'] if item['name'] == NAME)
        require(row['size'] == len(words) * 4 == SIZE, 'target extent changed')
        symbols = score.image_symbols()
        names = sorted(set(re.findall(r'\bD_[0-9A-F]{8}\b', SOURCE.read_text())))
        files = [SOURCE, HERE / 'verify.py', HERE / 'elf.py', HERE / 'behavior.py']
        return dict(sources_sha256={str(path.relative_to(ROOT)): digest(path.read_bytes()) for path in files},
                    target=dict(name=NAME, address=hex(ADDRESS), bytes=SIZE,
                                words_sha256=digest(packed(words)), image='A',
                                image_sha256=metadata['image_sha256']),
                    referenced_symbols={name: symbols.get(name, score.address_named(name)) for name in names},
                    flags=FLAGS, claims=[NAME])


def gnu_link(obj, temporary, symbols):
    sections, syms, rels = elf.read(obj)
    text_index, row, raw = sections['.text']
    require(syms[NAME] == (0, SIZE, 2, text_index), 'complete compiler ELF extent')
    require(not any(raw[SIZE:]), 'nonzero trailing code')
    require(all(dest == text_index and typ in (5, 6) and name in symbols
                for offset, typ, name, dest in rels), 'unknown relocation/data ownership')
    script = temporary / 'link.ld'
    script.write_text('SECTIONS { .text 0x%X : SUBALIGN(4) { *(.text) }\n'
                      '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n' % ADDRESS
                      + ''.join('%s = 0x%X;\n' % (name, address) for name, address in sorted(symbols.items())))
    linked = temporary / 'linked.elf'
    shell('mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, obj)
    sections2, symbols2, rels2 = elf.read(linked)
    index2, row2, raw2 = sections2['.text']
    require(not rels2 and row2[3] == ADDRESS, 'GNU relocation/placement')
    require(symbols2[NAME] == (ADDRESS, SIZE, 2, index2), 'complete GNU ELF extent')
    require(not any(raw2[SIZE:]), 'GNU nonzero trailing code')
    return list(struct.unpack('>%dI' % (SIZE // 4), raw2[:SIZE])), len(rels)


def host_library(temporary):
    path = temporary / 'host.c'
    path.write_text('#include <string.h>\n#include "' + str(SOURCE) + '"\n' + r'''
InputSequence *D_803B4278;
float D_803B427C;
s32 D_803B4280;
volatile float D_8002EB90;
u32 D_8015694C, D_80156944;
void replay(const u32 *input, const u32 *masks, u32 *out) {
    InputSequence sequence;
    float clock;
    int i;
    memset(&sequence, 0, sizeof(sequence));
    sequence.fields.count = (u8) input[1];
    for (i = 0; i < 20; i++) {
        sequence.fields.held[i] = masks[i];
        sequence.fields.pressed[i] = masks[20+i];
    }
    D_803B4278 = input[0] ? &sequence : 0;
    D_803B4280 = input[2];
    memcpy(&clock, &input[3], 4);
    D_8002EB90 = clock;
    memcpy(&D_803B427C, &input[4], 4);
    D_80156944 = input[5]; D_8015694C = input[6];
    out[0] = func_8038F648(); out[1] = D_803B4280;
    memcpy(&out[2], &D_803B427C, 4);
}
''')
    library = temporary / 'host.so'
    shell('gcc', '-std=c89', '-pedantic-errors', '-O2', '-fPIC', '-shared', path, '-o', library)
    loaded = ctypes.CDLL(str(library))
    pointer = ctypes.POINTER(ctypes.c_uint32)
    loaded.replay.argtypes = [pointer, pointer, pointer]
    loaded.replay.restype = None
    return loaded


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'stock IDO 5.3 required')
    with targets(), tempfile.TemporaryDirectory(prefix='input-sequence-replay-') as tmp:
        temporary = Path(tmp)
        obj = temporary / 'candidate.o'
        score.compile_single(SOURCE, FLAGS, obj)
        comparison = score.compare(obj, NAME, show=0)
        require(comparison.accepted(), comparison.summary())
        native = score.targets()[NAME]
        linked, relocation_count = gnu_link(obj, temporary, binding['referenced_symbols'])
        require(linked == native, 'complete GNU/native mismatch')
        controls = {}
        source = SOURCE.read_text()
        for label, identity, commute in [('ordinary', False, False), ('identity_only', True, False),
                                         ('commute_only', False, True)]:
            candidate = source
            for mask, global_name in [('held', 'D_80156944'), ('pressed', 'D_8015694C')]:
                left = '(%s * 1)' % mask if identity else mask
                right = '(%s & %s)' % (global_name, mask)
                expression = left + ' == ' + right if commute else right + ' == ' + left
                candidate = candidate.replace('(%s * 1) == (%s & %s)' % (mask, global_name, mask), expression)
            control = temporary / (label + '.c')
            control.write_text(candidate)
            control_obj = temporary / (label + '.o')
            score.compile_single(control, FLAGS, control_obj)
            result = score.compare(control_obj, NAME, show=0)
            require(result.differing == 2 and not any((result.errors, result.unresolved,
                    result.unverified, result.extra_words)), 'control changed: ' + label)
            controls[label] = asdict(result)
        host = host_library(temporary)
        coverage, branches, count = set(), set(), 0
        for count, case in enumerate(behavior.cases(), 1):
            expected = behavior.model(case)
            a = behavior.run(native, case, count)
            b = behavior.run(linked, case, count)
            require(a == b and a['output'] == expected, 'native/GNU/model state or trace mismatch')
            values = (ctypes.c_uint32 * 7)(case['present'], case['count'], case['step'],
                                          case['clock'], case['deadline'], *case['buttons'])
            masks = (ctypes.c_uint32 * 40)(*(case['held'] + case['pressed']))
            output = (ctypes.c_uint32 * 3)()
            host.replay(values, masks, output)
            require(tuple(output) == expected, 'host/model output mismatch')
            coverage.update(a['coverage'])
            branches.update(a['branches'])
        require(len(coverage) == SIZE // 4, 'not every native instruction exercised')
        try:
            behavior.run([0xFFFFFFFF], next(behavior.cases()), 0)
        except ValueError as error:
            require(str(error) == 'unsupported opcode', 'decoder guard changed')
        else:
            raise ValueError('unknown opcode accepted')
        return dict(inputs=binding, result=asdict(comparison), controls=controls,
                    independently_linked_sha256=digest(packed(linked)), gnu_relocations=relocation_count,
                    behavior=dict(cases=count, instructions=len(coverage), branch_outcomes=len(branches),
                                  native_gnu_traces_equal=True, host_and_model_equal=True,
                                  saved_integer_fp_and_stack_preserved=True),
                    compiler='stock IDO 5.3', claimed_bytes=SIZE, new_claim_bytes=SIZE,
                    accepted_image_bytes=0)


def verify():
    receipt = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == receipt['inputs'], 'receipt input binding changed')
    actual = replay()
    require(actual == receipt, 'receipt replay changed')
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'], 'receipt input binding changed')
    else:
        verify()
    print('PASS: input sequence, one strict 236-byte claim; complete stock/GNU proof and bounded behavior')
