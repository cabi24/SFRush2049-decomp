#!/usr/bin/env python3
"""Read-only, compiler-free viewport service identity and boundary audit.

Read all production context at BASE. Emit metadata, never native bytes.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import zlib

BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
TARGETS = {
    'render_viewport_init': ('blob', 0x800FA9B4, 924),
    'save_write_data': ('blob', 0x800AF06C, 1200),
    'func_80090308': ('blob', 0x80090308, 1128),
    'func_80090284': ('blob', 0x80090284, 132),
    'InitMaxPath': ('blob', 0x800A1244, 136),
    'PrevMaxPath': ('blob', 0x800A11E4, 96),
    'display_enable': ('blob', 0x800C8FA4, 316),
    'playgame_state_change': ('blob', 0x800CA3B4, 2544),
    'func_8038FCE0': ('ovl_b', 0x8038FCE0, 3056),
    'func_80390F60': ('ovl_b', 0x80390F60, 988),
    'func_80390B10': ('ovl_b', 0x80390B10, 552),
    'func_80390D38': ('ovl_b', 0x80390D38, 552),
}
EXPECTED = {
    'save_write_data': '3d7f79a62ea3664248e76b8cd9fc06dbd8142ab8d31e6167e8c1ca246323fca1',
    'func_8038FCE0': '3574383f8241f87f1aa201bc4a7265e49cd818b8b6f7e6564ecca4131b6682e2',
    'func_80390F60': '6d91e9692637c1d96f48b3a1afcc63d3348212a42baf882e7e71034426a30bb3',
}

def sha(data): return hashlib.sha256(data).hexdigest()
def signed16(value): return value - 65536 if value & 32768 else value

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--reference-root', type=Path, required=True)
    p.add_argument('--output', type=Path, default=Path(__file__).with_name('identity.json'))
    args = p.parse_args()
    root = str(args.reference_root.resolve())
    def git(*argv):
        return subprocess.check_output(['git', '-C', root, *argv])
    def read(path): return git('show', BASE + ':' + path)
    asset = read('assets/us/data.bin')
    images = {}
    metadata = {}
    for population, rom, base, size, digest in [
        ('blob', 0xB0CB10, 0x80086A50, 647072, 'bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d'),
        ('ovl_a', 0xB5C534, 0x8038A400, 194128, '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'),
        ('ovl_b', 0xB6FEC4, 0x8038A400, 43888, 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd')]:
        z = zlib.decompressobj(-15)
        image = z.decompress(asset[rom-0x283D0:])
        assert z.eof and len(image) == size and sha(image) == digest
        images[population] = image
        metadata[population] = dict(rom=hex(rom), base=hex(base), bytes=size, sha256=digest)
    assert int.from_bytes(asset[0x2BC20-0x283D0:0x2BC24-0x283D0], 'big') == 0xB5C534
    assert int.from_bytes(asset[0x2BC24-0x283D0:0x2BC28-0x283D0], 'big') == 0xB6FEC4
    populations = {}
    for pop in ('blob', 'ovl_b'):
        syms = json.loads(read('asm/us/'+pop+'/symbols.json'))['symbols']
        bodies = {}
        for path in git('ls-tree', '-r', '--name-only', BASE, 'asm/us/'+pop).decode().splitlines():
            if not path.endswith('.s'): continue
            for section in read(path).decode().split('.section .text.')[1:]:
                name = section.split(',')[0]
                words = [int(x, 16) for x in re.findall(r'\.word 0x([0-9A-Fa-f]{8})', section)]
                bodies[name] = (int(syms[name], 16), words, path)
        populations[pop] = bodies
    targets = {}
    for name, (pop, address, size) in TARGETS.items():
        actual_address, words, path = populations[pop][name]
        raw = struct.pack('>'+str(len(words))+'I', *words)
        assert address == actual_address and len(raw) == size, (name, len(raw), size)
        start = address-int(metadata[pop]['base'], 16)
        assert raw == images[pop][start:start+size]
        if name in EXPECTED: assert sha(raw) == EXPECTED[name]
        calls = []
        save, restore, fsave, frestore = {}, {}, {}, {}
        for i, word in enumerate(words):
            op, rs, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
            if op in (2, 3):
                calls.append(dict(site=hex(address+4*i), target=hex(((address+4*i+4)&0xF0000000)|((word&0x3FFFFFF)<<2)), kind='jal' if op==3 else 'j'))
            if rs == 29 and op in (43, 35, 61, 53):
                dest = {43:save, 35:restore, 61:fsave, 53:frestore}[op]
                dest.setdefault(str(rt), []).append(signed16(word & 65535))
        pair = lambda a, b: sorted(int(k) for k in a if set(a[k]) & set(b.get(k, [])))
        targets[name] = dict(population=pop, address=hex(address), end=hex(address+size), bytes=size, sha256=sha(raw), historical_target_file=path, direct_calls=calls, paired_stack_gpr_saves=[x for x in pair(save, restore) if x in list(range(16,24))+[30,31]], paired_stack_fpr_pair_saves=pair(fsave, frestore))
    # These ordinary outer roots preserve exactly the registers their private
    # descendants need. These are save/restore facts, not a generic C ABI proof.
    for name, regs, floats in [
        ('render_viewport_init', list(range(16,23))+[31], [20]),
        ('save_write_data', list(range(16,24))+[30,31], [20,22,24,26,28,30]),
        ('func_8038FCE0', list(range(16,24))+[30,31], [20,22,24,26,28,30]),
        ('func_80390F60', [16,17,31], [20]),
        ('func_80390B10', [31], []), ('func_80390D38', [31], []), ('func_80090308', [31], [])]:
        assert targets[name]['paired_stack_gpr_saves'] == regs, name
        assert targets[name]['paired_stack_fpr_pair_saves'] == floats, name
    edges = []
    wanted = {0x800A1244, 0x8038FCE0, 0x80390F60}
    for name, (address, words, _) in populations['blob'].items():
        for i, w in enumerate(words):
            dest = ((address+4*i+4)&0xF0000000)|((w&0x3FFFFFF)<<2)
            if w >> 26 == 3 and dest in wanted:
                edges.append(dict(caller=name, caller_start=hex(address), site=hex(address+4*i), target=hex(dest)))
    extents = json.loads(read('asm/us/ovl_a/extents.json'))['functions']
    wrong_image = {}
    for address in (0x8038FCE0, 0x80390F60):
        matching = [f for f in extents if int(f['address'],16) <= address < int(f['address'],16)+f['size']]
        assert len(matching)==1 and int(matching[0]['address'],16) != address
        name = 'func_'+hex(address)[2:].upper()
        size = targets[name]['bytes']
        start = address-0x8038A400
        assert sha(images['ovl_a'][start:start+size]) != targets[name]['sha256']
        wrong_image[hex(address)] = dict(containing_function=matching[0]['name'], start=matching[0]['address'], bytes=matching[0]['size'], result='REJECTED: interior of different A-image body; B native target hash fails')
    result = dict(status='READ-ONLY CONTRACT AUDIT; ready for scoped ordinary-boundary reconstruction; no matching claim', base=BASE, source_sha256=sha(Path(__file__).read_bytes()), compiler_invocations=0, images=metadata, targets=targets, main_direct_call_census=edges, wrong_image_controls=wrong_image, limits='Save/restore and complete target identity checks are mechanized. Semantic, pointer-lifetime, loader-state and read-before-definition findings are the accompanying manual native audit. No executable replay or whole-game load-state proof is claimed.')
    args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(dict(status=result['status'], targets=len(targets), wrong_image_controls=len(wrong_image), output=str(args.output)), indent=2))
if __name__=='__main__': main()
