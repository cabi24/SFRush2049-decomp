#!/usr/bin/env python3
"""Derive metadata-only workbench evidence; native words stay in temporary files."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    objdump = Path(os.environ['MIPS_OBJDUMP'])
    assembler = objdump.with_name('mips-linux-gnu-as')
    rows = []
    for address in ['80013DEC', '80014198']:
        name = 'func_' + address
        for checkpoint, source in [('baseline', PACKET / 'controls' / (name + '_baseline.c')),
                                    ('final', PACKET / 'nonmatch' / (name + '.c'))]:
            with tempfile.TemporaryDirectory(prefix='bt02-audio-remaining-diagnose-') as directory:
                tmp = Path(directory)
                obj = tmp / 'compiled.o'
                score.compile_single(source, score.DEFAULT_FLAGS, obj)
                words = score.text_words(obj)
                resolved, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, len(words) * 4, score.image_symbols())
                assert not (masks or unresolved or unverified or errors)
                data, sections = score._elf(obj)
                symbols = [symbol for index, section in enumerate(sections) if section['type'] == 2
                           for symbol in score._symbol_table(data, sections, index) if symbol['name'] == name]
                assert len(symbols) == 1 and symbols[0]['value'] == 0
                body_words = symbols[0]['size'] // 4
                assert not any(resolved[body_words:]), 'nonzero object alignment padding'
                for label, values in [('target', score.targets()[name]), ('candidate', resolved[:body_words])]:
                    assembly = '.text\n.set noreorder\n.globl ' + name + '\n.type ' + name + ', @function\n' + name + ':\n'
                    assembly += ''.join('.word 0x%08x\n' % word for word in values)
                    assembly += '.size ' + name + ', .-' + name + '\n'
                    (tmp / (label + '.s')).write_text(assembly)
                    subprocess.run([str(assembler), '-mips2', '-EB', '-o', str(tmp / (label + '.o')), str(tmp / (label + '.s'))], check=True)
                command = [sys.executable, str(ROOT / 'tools/workbench.py'), 'diagnose', str(tmp / 'target.o'), str(tmp / 'candidate.o'),
                           '--function', name, '--objdump', str(objdump), '--json']
                diagnosis = json.loads(subprocess.check_output(command, text=True))
                comparison = diagnosis['comparison']
                rows.append({'name': name, 'checkpoint': checkpoint, 'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                             'routing': diagnosis['routing'], 'owning_pass': diagnosis['owning_pass'], 'ownership_basis': diagnosis['ownership_basis'],
                             'lever_class': diagnosis['lever']['lever_class'], 'lever_reason': diagnosis['lever']['reason'],
                             **{key: comparison[key] for key in ['verdict', 'words', 'raw', 'target_frame_size', 'candidate_frame_size',
                                                                'target_instructions', 'candidate_instructions', 'aligned_insertions',
                                                                'aligned_deletions', 'aligned_register', 'aligned_constant', 'aligned_structural']}})
    return {'result': 'PASS', 'scope': 'Exact ELF symbol extent after strict relocation. Only verified zero alignment padding is omitted from workbench diagnostics; unchanged score.py remains the acceptance gate.', 'checkpoints': rows}

if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
