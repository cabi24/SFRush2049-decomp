#!/usr/bin/env python3
"""Replay bounded NONMATCH baselines. Temporary artifacts are never published."""
import argparse
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[3]
SINGLES = [('selector', 'func_80392F28'), ('mode', 'func_803931C0'),
           ('producer', 'func_803930B8'), ('cleanup', 'func_803932E4')]


def run():
    if not __debug__:
        raise SystemExit("verification requires assertions; do not use python -O")
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--write', action='store_true')
    ap.add_argument('--check', action='store_true')
    ap.add_argument('--diagnose', action='store_true')
    args = ap.parse_args()
    sys.path.insert(0, str(REPO / 'tools/cloud'))
    import score
    score.ASM_DIR = REPO / 'asm/us/ovl_a'
    targets = score.targets()  # Enforces protected SHA256SUMS.
    extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', score.target_manifest()))
    assert extents['image'] == 'A' and extents['base'] == '0x8038A400'
    receipt = {'image': 'A', 'image_sha256': extents['image_sha256'],
               'claims': [], 'results': {}, 'hashes': {}}
    paths = list(HERE.glob('*.c')) + [HERE / 'group.json', HERE / 'reproduce.py']
    paths += [REPO / 'tools/cloud/score.py', REPO / 'tools/cloud/owndata.py']
    paths += [score.ASM_DIR / name for name in score.target_manifest()]
    paths += [score.ASM_DIR / 'extents.json']
    for path in paths:
        receipt['hashes'][str(path.relative_to(REPO))] = hashlib.sha256(path.read_bytes()).hexdigest()
    with tempfile.TemporaryDirectory() as tmp:
        temp = Path(tmp)
        for file, name in SINGLES:
            for opt in ('O2', 'O3'):
                obj = temp / (file + opt + '.o')
                score.compile_single(HERE / (file + '.c'), '-g0 -' + opt + ' -mips2 -G 0 -non_shared', obj)
                result = score.compare(obj, name, show=0)
                receipt['results'][file + '_' + opt] = asdict(result)
                print(file, opt, result.summary())
                if file == 'selector' and opt == 'O2':
                    data, secs = score._elf(obj)
                    syms = [s for i, sec in enumerate(secs) if sec['type'] == 2
                            for s in score._symbol_table(data, secs, i)]
                    fn = next(s for s in syms if s['name'] == name)
                    assert fn['size'] == len(targets[name]) * 4 == 220
                    script = temp / 'selector.ld'
                    address = int(name[5:], 16)
                    symbols = score.image_symbols()
                    assignments = []
                    for sym in syms:
                        if sym['section'] == 0 and sym['name']:
                            value = symbols.get(sym['name'], score.address_named(sym['name']))
                            assert value is not None, sym['name']
                            assignments.append('%s = 0x%x;' % (sym['name'], value))
                    script.write_text('SECTIONS { . = 0x%x; .text : SUBALIGN(4) { *(.text) } }\n' % address + '\n'.join(assignments))
                    elf, binary = temp / 'selector.elf', temp / 'selector.bin'
                    subprocess.run(['mips-linux-gnu-ld', '-T', str(script), '-o', str(elf), str(obj)], check=True)
                    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '--only-section=.text', str(elf), str(binary)], check=True)
                    raw = binary.read_bytes()
                    wanted = targets[name]
                    differing = [4*i for i, word in enumerate(wanted)
                                 if int.from_bytes(raw[4*i:4*i+4], 'big') != word]
                    assert differing == [0x84, 0xac]
                    assert not any(raw[220:])
                    receipt['selector_gnu_link'] = {'symbol_bytes': fn['size'],
                        'text_bytes': len(raw), 'differing_offsets': differing,
                        'linked_sha256': hashlib.sha256(raw).hexdigest()}
                    if args.diagnose:
                        # Compare linked ELF: no unresolved-address diagnostic noise.
                        asm = temp / 'native.s'
                        asm.write_text('.set noreorder\n.text\n.globl '+name+'\n.type '+name+',@function\n'+name+':\n' + ''.join('.word '+hex(w)+'\n' for w in wanted) + '.size '+name+',.-'+name+'\n')
                        native = temp / 'native.o'
                        subprocess.run(['mips-linux-gnu-as', '-mips2', '-EB', '-o', str(native), str(asm)], check=True)
                        subprocess.run([sys.executable, str(REPO / 'tools/workbench.py'), 'diagnose', str(native), str(elf), '--function', name, '--objdump', 'mips-linux-gnu-objdump', '--terse'], check=True)
        obj = temp / 'group.o'
        spec = score.compile_group(HERE, obj)
        for name in spec['members']:
            result = score.compare(obj, name, show=0)
            receipt['results']['group_' + name] = asdict(result)
            print('group', name, result.summary())
    if args.check:
        assert receipt == json.loads((HERE / 'verification.json').read_text()), 'receipt changed'
    if args.write:
        (HERE / 'verification.json').write_text(json.dumps(receipt, indent=2, sort_keys=True) + '\n')


if __name__ == '__main__':
    run()
