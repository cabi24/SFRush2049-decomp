#!/usr/bin/env python3
"""Protocol-v2, zero-claim audit of the AA3D8 source-name bound."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('name_boundary_native', HERE / 'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
require = native.require
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
GAME_HASH = 'bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d'
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
GAME_BASE, IMAGE_BASE, ROM_BASE = 0x80086a50, 0x8038a400, 0x283d0
FUNCTIONS = {
    'blob': ('func_800C7EC8', 'draw_speedometer', 'format_string_parse', 'func_80091FBC',
             'draw_text', 'object_render_cleanup', 'func_800BE6A4'),
    'ovl_a': ('func_8039338C', 'func_803AA3D8'),
}
FILE_HANDLE, FILE_RECORD, DATA_HANDLE, SAVE = 0x80200100, 0x80200200, 0x80200300, 0x80201000
PROFILE_HANDLE, PROFILE, SCRATCH = 0x8013c128, 0x8013e5d8, 0x803ba840
CHECKSUM_SPANS = [(0, 4, 24), (28, 32, 21), (56, 60, 11), (72, 76, 16),
                  (92, 96, 12), (108, 112, 16), (128, 132, 8)] + [
    (base + i * stride, base + i * stride + 4, stride - 4)
    for base, count, stride in ((140, 6, 96), (1292, 4, 64), (1548, 8, 12),
                                (1644, 1, 24), (1668, 4, 28), (1780, 1, 76), (1856, 13, 16))
    for i in range(count)]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


def inflate(data, maximum):
    decoder = zlib.decompressobj(-15)
    raw = decoder.decompress(data, maximum + 1)
    require(decoder.eof and not decoder.unconsumed_tail and len(raw) <= maximum, 'invalid deflate resource')
    return raw


def inputs():
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(sha(asset) == ASSET_HASH, 'asset identity changed')
    game = inflate(asset[0xb0cb10 - ROM_BASE:], 647072)
    image = inflate(asset[0xb5c534 - ROM_BASE:], 194128)
    require(sha(game) == GAME_HASH and sha(image) == IMAGE_HASH, 'image identity changed')
    old = score.ASM_DIR
    targets, bindings = {}, {}
    try:
        for population, names in FUNCTIONS.items():
            score.ASM_DIR = ROOT / 'asm/us' / population
            words, symbols = score.targets(), score.image_symbols()
            raw, base = (game, GAME_BASE) if population == 'blob' else (image, IMAGE_BASE)
            for name in names:
                address = symbols.get(name, score.address_named(name))
                body = packed(words[name])
                require(raw[address - base:address - base + len(body)] == body, 'target/image mismatch: ' + name)
                targets[address] = words[name]
                bindings[name] = dict(population=population, address=hex(address),
                                      complete_bytes=len(body), words_sha256=sha(body))
    finally:
        score.ASM_DIR = old
    files = [HERE / 'native.py', HERE / 'verify.py', HERE / 'host.c',
             ROOT / 'tests/cloud/test_runtime_a_name_boundary.py']
    binding = dict(protocol_version=2, claims=[], asset_sha256=ASSET_HASH, game_sha256=GAME_HASH,
                   image_a_sha256=IMAGE_HASH, targets=bindings,
                   sources={str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in files},
                   checksum_jump_table=dict(address='0x80123c20', bytes=24,
                       sha256=sha(game[0x80123c20-GAME_BASE:0x80123c38-GAME_BASE])))
    return binding, targets, asset, game


def checksum(data):
    result = 0
    for value in data:
        kind = value & 7
        if kind == 0: result -= value
        elif kind == 1: result |= value
        elif kind == 2: result &= value
        elif kind == 3: result ^= value
        elif kind == 4: result *= value
        elif kind == 5: result //= value
        else: result += value
        result &= 0xffffffff
    return result


def fixture(game, length):
    require(0 <= length <= 19, 'fixture name length domain')
    m = native.Memory(game)
    m.put(FILE_HANDLE, FILE_RECORD, 4)
    m.put(FILE_RECORD + 64, 2064, 4)
    m.put(FILE_RECORD + 72, DATA_HANDLE, 4)
    m.put(DATA_HANDLE, SAVE, 4)
    # Native menu_save_options initializes this doubly-linked indirect list.
    m.put(0x8012e6d8, 1, 1)
    m.put(0x8012e6d9, 1, 1)
    m.put(0x80117428, 1, 1)
    m.put(SAVE + 12, 16, 1)
    m.write(SAVE + 13, b'A' * min(length, 13))
    if length > 13:
        m.write(SAVE + 60, b'B' * (length - 13))
    for destination, start, size in CHECKSUM_SPANS:
        m.put(SAVE + destination, checksum(m.read(SAVE + start, size)), 4)
    return m


def hooks(events):
    def loaded(m, r):
        require(r[4:7] == [FILE_HANDLE, 0, 2064], 'file-read contract changed')
        require(m.get(FILE_RECORD + 72, 4) == DATA_HANDLE, 'backed save missing')
        events.append(('file_read', 2064))
    def copy(m, r):
        triples = {(PROFILE + 20, SAVE + 13, 13), (PROFILE + 12, SAVE + 4, 8),
                   (PROFILE + 33, SAVE + 60, 11)}
        triple = tuple(r[4:7])
        require(triple in triples, 'unexpected copy contract')
        m.write(r[4], m.read(r[5], r[6])); r[2] = r[4]
        events.append(('copy', *triple))
    def release(m, r):
        require(r[4] == FILE_HANDLE, 'unexpected release handle')
        events.append(('release_file',))
    def click(m, r):
        events.append(('input_sound', r[4]))
    return {0x800a2990:loaded, 0x80007c68:copy, 0x800a25c0:release,
            0x800b5f88:click, 0x800b5fc4:click}


def witness(targets, game, length):
    m = fixture(game, length); events = []
    # Renderer AA3D8 is authenticated for the downstream contract, not executed.
    selected = {a:w for a,w in targets.items() if a != 0x803aa3d8}
    machine = native.Machine(selected, m, hooks(events))
    result = machine.run(0x800c7ec8, {16: FILE_HANDLE})
    require(result == PROFILE_HANDLE, 'loader did not return profile')
    require(m.get(PROFILE_HANDLE, 4) == PROFILE, 'wrong profile storage')
    require(m.get(0x8012e6e0, 4) == PROFILE_HANDLE, 'native list insertion failed')
    require(m.get(PROFILE + 44, 4) == 0, 'native release did not clear data handle')
    loaded_name = m.string(PROFILE + 20)
    require(len(loaded_name) == length, 'loader string length differs')
    require(sum(e[0] == 'copy' for e in events) == 3, 'loader copy census changed')
    # Enter the delete-confirmation state on the selected stored profile.
    m.put(0x803ba800, PROFILE_HANDLE, 4)
    m.put(0x803ba808, 3, 4)
    m.put(0x803ba7e3, 1, 1)
    m.put(0x803ba80c, 1, 1)
    m.put(0x8015694c, 2, 4)
    machine.run(0x8039338c)
    require(m.get(0x803ba84d, 1) == 1 and m.get(0x803ba850, 1) == 0,
            'confirmation flags changed')
    first_name = m.string(SCRATCH)
    # Toggle the actual one-byte confirmation choice, which can expose copied
    # adjacent configuration bytes beyond the temporary NUL at scratch+16.
    m.put(0x8015694c, 0, 4)
    m.put(0x80149784, 0x1000, 4)
    machine.run(0x8039338c)
    require(m.get(0x803ba84d, 1) == 1 and m.get(0x803ba850, 1) == 1
            and m.get(0x803ba854, 1) == 0, 'toggle left confirmation branch')
    final_name = m.string(SCRATCH)
    if length <= 12:
        require(first_name == loaded_name == final_name, 'normal terminated-name control changed')
    if length == 19:
        require(len(first_name) == 16 and len(final_name) == 19, 'missing boundary witness')
        require(final_name[13] == 1 and final_name[16] == 1, 'native flag-byte writes missing')
    coverage = {hex(a):sum(a <= p < a + len(w)*4 for p in machine.coverage)
                for a,w in selected.items()}
    return dict(stored_logical_name_bytes=length, malformed_unterminated_13_byte_field=length >= 13,
                loaded_name_bytes=len(loaded_name), initial_confirmation_name_bytes=len(first_name),
                after_toggle_name_bytes=len(final_name), loader_return=hex(result),
                native_steps=machine.steps, executed_instruction_counts=coverage,
                total_instruction_counts={hex(a):len(w) for a,w in selected.items()},
                hook_events=[list(event) for event in events]), final_name


def tables(data):
    at, rows = 0, []
    for width in (4,2,4,2,2,4):
        require(at + 4 <= len(data), 'missing resource count')
        count = struct.unpack_from('>I', data, at)[0]; at += 4
        require(count <= 65535 and at + count*width <= len(data), 'bad resource size')
        rows.append(struct.unpack_from('>%d%s' % (count, 'I' if width==4 else 'H'), data, at))
        at += count*4 if width==4 else ((count+1)&~1)*2
    require(list(map(len, rows[:3])) == [237,58,374], 'language shape changed')
    return rows, at


def formats(asset, game, name):
    rows, samples = [], []
    for kind in range(6):
        rom = struct.unpack_from('>I', game, 0x8011b5bc-GAME_BASE+4*kind)[0]
        codec = struct.unpack_from('>I', game, 0x80123564-GAME_BASE+4*kind)[0]
        require(codec==2 and ROM_BASE<=rom<0xb0cb10, 'language loader changed')
        data = inflate(asset[rom-ROM_BASE:], 65536); entries, start = tables(data)
        for index in (143,146):
            offset=entries[0][index]; require(start<=offset<len(data), 'invalid text pointer')
            end=data.find(b'\0',offset); require(end>=offset,'unterminated format')
            fmt=data[offset:end]
            require(fmt.count(b'%')==1 and fmt.count(b'%s')==1 and fmt[0]!=255, 'unexpected formatter contract')
            expected=fmt.replace(b'%s',name)+b'\0'
            rows.append(dict(language=kind,slot=index,resource_sha256=sha(data),
                             format_sha256=sha(fmt+b'\0'),fixed_bytes=len(fmt)-2,
                             output_including_nul=len(expected)))
            samples.append((fmt,name,expected))
    require(max(r['fixed_bytes'] for r in rows)==45, 'fixed-format maximum changed')
    require(max(r['output_including_nul'] for r in rows)==65, '19-byte witness length changed')
    return rows,samples


def checksum_controls(targets, game):
    m=native.Memory(game); machine=native.Machine({0x800b466c:targets[0x800b466c]},m,{})
    samples=[bytes([value]) for value in range(256)]+[b'']
    samples += [bytes((seed*37+i*29+i*i)&255 for i in range(1+seed)) for seed in range(32)]
    for data in samples:
        m.write(SAVE,data)
        actual=machine.run(0x800b466c,{4:SAVE,5:len(data)})
        require(actual==checksum(data),'native checksum/Python disagreement')
    expected=set(range(0x800b466c,0x800b4720,4))-{0x800b46f8}
    require(machine.coverage==expected,'checksum instruction coverage changed')
    return dict(cases=len(samples),executed_instructions=len(machine.coverage),
                total_instructions=45,unexecuted='unreachable divide-by-zero break; divisor has low three bits 5')


def negative_controls(targets, game):
    results={}
    for label,mutation in [('checksum',lambda m:m.put(SAVE,m.get(SAVE,4)^1,4)),
                           ('version',lambda m:m.put(SAVE+12,15,1))]:
        m=fixture(game,19);m.put(PROFILE_HANDLE,PROFILE,4);m.put(PROFILE+44,DATA_HANDLE,4)
        mutation(m);machine=native.Machine(targets,m,{})
        result=machine.run(0x800c7818,{30:PROFILE_HANDLE})
        require(result==1,'validator negative control accepted: '+label)
        results[label]='validator returns dirty=1'
    try:
        native.Machine({0x80203000:[0xffffffff]},fixture(game,0),{}).run(0x80203000)
    except ValueError as error:
        require('unsupported opcode' in str(error),'wrong unsupported-instruction refusal')
    else:raise ValueError('unknown instruction accepted')
    results['unsupported_instruction']='rejected'
    try:native.Memory(game).get(0x100,4)
    except ValueError:pass
    else:raise ValueError('unmapped memory accepted')
    results['unmapped_memory']='rejected'
    bad={a:list(words) for a,words in targets.items()}
    index=(0x800c8018-0x800c7ec8)//4
    require(bad[0x800c7ec8][index]&65535==13,'native name-copy size changed')
    bad[0x800c7ec8][index]=(bad[0x800c7ec8][index]&0xffff0000)|12
    try:witness(bad,game,19)
    except ValueError as error:require('copy contract' in str(error),'wrong mutated-copy refusal')
    else:raise ValueError('mutated name-copy count accepted')
    results['changed_native_copy_count']='rejected'
    return results


def replay():
    binding,targets,asset,game=inputs()
    cases=[]; name=None
    for length in range(20):
        row,name=witness(targets,game,length);cases.append(row)
    language_rows,samples=formats(asset,game,name)
    result=dict(inputs=binding,claims=[],new_strict_code_bytes=0,new_accepted_image_bytes=0,
                new_rom_verified_bytes=0,cases=cases,formats=language_rows,
                negative_controls=negative_controls(targets,game),checksum_controls=checksum_controls(targets,game),
                conclusion='Checksum-valid malformed 13-byte name fields can expose 19-byte names to the confirmation formatter after a native selection toggle; a universal 18-byte bound is not established.',
                limits='Logical machine fixture; file-read/copy/release/sound contracts hooked. Normal UI creation, physical pak I/O, formatter execution, renderer execution, original declarations, full gameplay and hardware reachability are not proved.')
    return result,samples


def verify():
    actual,_=replay()
    expected=json.loads((HERE/'verification.json').read_text())
    require(actual==expected,'receipt changed; inspect then deliberately rebind')
    return actual


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--write',action='store_true');parser.add_argument('--host',action='store_true');parser.add_argument('--sanitize',action='store_true');args=parser.parse_args()
    result,samples=replay()
    if args.write:(HERE/'verification.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    else:require(result==json.loads((HERE/'verification.json').read_text()),'receipt changed')
    if args.host or args.sanitize:
        import subprocess,tempfile
        with tempfile.TemporaryDirectory(prefix='name-boundary-host-') as d:
            path=Path(d); command=['cc','-std=c99','-O2','-Wall','-Wextra','-Werror',str(HERE/'host.c'),'-o',str(path/'host')]
            if args.sanitize:command+=['-g','-fsanitize=address,undefined,bounds','-fno-sanitize-recover=all','-fno-omit-frame-pointer']
            subprocess.run(command,check=True)
            payload=b''.join(struct.pack('>III',len(fmt),len(name),len(expected))+fmt+name for fmt,name,expected in samples)
            import os
            env=os.environ.copy()
            if args.sanitize:env['ASAN_OPTIONS']='detect_leaks=0'
            output=subprocess.check_output([str(path/'host')],input=payload,env=env).decode();require(output.strip()=='12 authentic-format cases; maximum 65 bytes including NUL','host output')
    print('20 native loader/confirmation fixtures; 19-byte malformed-name witness; maximum 65 bytes including NUL; zero new coverage')


if __name__=='__main__':main()
