#!/usr/bin/env python3
"""Portable complete proof of the revived real-caller route-crossing match."""
import argparse
import contextlib
import hashlib
import io
import json
import math
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
GROUP = ROOT / 'cloud/matches/dot_route_crossing_20261006'
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
ORIGINAL = 'cloud/work/frontier/dot_route_tracker/group.c'
FN = 'audio_priority_find'
CALLER = 'audio_mixer_main'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score


def require(condition, message):
    if not condition:
        raise AssertionError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def signed(value):
    return (value & 0x7fffffff) - (value & 0x80000000)


def narrow(value):
    return (value & 0x7fff) - (value & 0x8000)


def f32(value):
    return struct.unpack('>f', struct.pack('>f', value))[0]


def historical_source(history):
    def read(path):
        return subprocess.check_output(['git', '-C', str(history), 'show', BASE + ':' + path], text=True)
    source = (GROUP / 'group.c').read_text()
    old = read(ORIGINAL)
    require(source[source.index('typedef signed char'):] == old[old.index('typedef signed char'):],
            'real helper/caller declarations or bodies changed from base')
    require(FN not in json.loads(read('blob_matched.lock.json')), 'target already locked at base')
    return {'base_commit': BASE, 'source': ORIGINAL, 'complete_source_unchanged_except_header_comment': True}


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


def oracle(case):
    who, route = narrow(case['who']), narrow(case['route'])
    if route >= case['count']:
        return -1
    if case['type'] == 2:
        return 0 if who == 0 else -1
    anchor = None
    for index, (x, y, z) in enumerate(case['points']):
        x, z = f32(x - case['px']), f32(z - case['pz'])
        distance = f32(f32(x*x) + f32(z*z))
        if distance <= f32(case['range']):
            side = -1 if f32(f32(x*case['dx']) + f32(z*case['dz'])) < 0 else 1
            if anchor is None:
                anchor = side
            elif anchor != side:
                return index
    return -1


def execute(words, case, coverage=None, branches=None):
    """Fail-closed R4300 subset; binary32 round-to-nearest, finite fixtures only."""
    base, stop, stack = 0x800ba46c, 0x01000000, 0x02000100
    addresses = score.image_symbols()
    routes, points = 0x03000000, 0x04000000
    regions = [(addresses['D_801407F0'], bytearray(b'\x5a' * 16)),
               (addresses['D_80151CE8'], bytearray(b'\xa5' * 812)),
               (routes, bytearray(b'\x7e' * 240)),
               (points, bytearray(max(1, len(case['points'])) * 6)),
               (stack - 24, bytearray(b'\xa5' * 56))]
    def locate(address, size):
        require(address % min(size, 4) == 0, 'unaligned memory access')
        for origin, data in regions:
            if origin <= address and address + size <= origin + len(data):
                return data, address - origin
        raise AssertionError('memory access outside complete mapped storage')
    def put(address, size, value):
        data, at = locate(address, size)
        data[at:at+size] = (value & ((1 << (8*size))-1)).to_bytes(size, 'big')
    def get(address, size):
        data, at = locate(address, size)
        return int.from_bytes(data[at:at+size], 'big')
    def float_bits(value):
        return struct.unpack('>I', struct.pack('>f', value))[0]
    put(addresses['D_801407F0'] + 8, 1, case['count'])
    put(addresses['D_801407F0'] + 12, 4, routes)
    for index in range(15):
        put(routes + 16*index, 1, case['type'])
        put(routes + 16*index + 10, 2, len(case['points']))
        put(routes + 16*index + 12, 4, points)
    for index, point in enumerate(case['points']):
        for axis, value in enumerate(point):
            put(points + 6*index + axis*2, 2, value)
    record = addresses['D_80151CE8'] + 12 + narrow(case['who'])*80
    for offset, key in [(0, 'px'), (8, 'pz'), (12, 'dx'), (20, 'dz')]:
        put(record + offset, 4, float_bits(case[key]))
    put(record + 24, 4, case['range'])
    snapshots = [bytes(data) for _, data in regions[:-1]]
    r = [(0x11223344 ^ index*0x01020408) & 0xffffffff for index in range(32)]
    r[0], r[4], r[5], r[29], r[31] = 0, case['route'], case['who'], stack, stop
    before = list(r)
    fp = [float_bits(3.5 + i) for i in range(32)]
    before_fp = list(fp)
    cc, lo, pc, pending, steps = False, 0, base, None, 0
    def val(index):
        return struct.unpack('>f', struct.pack('>I', fp[index]))[0]
    while pc != stop:
        require(pc % 4 == 0 and base <= pc < base + len(words)*4, 'instruction fetch outside full target')
        steps += 1
        require(steps < 300 + len(case['points'])*80, 'instruction budget exceeded')
        offset = pc-base
        if coverage is not None:
            coverage.add(offset)
        word = words[offset//4]
        op, rs, rt, rd, sh, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word >> 6 & 31, word & 63
        imm = narrow(word)
        nxt, delayed = pc+4, pending
        pending = None
        def branch(taken, likely=False):
            nonlocal pending, nxt
            if branches is not None:
                branches.setdefault(offset, set()).add(bool(taken))
            if taken:
                pending = pc + 4 + imm*4
            elif likely:
                nxt = pc + 8
        if op == 0:
            if fn == 0: r[rd] = r[rt] << sh
            elif fn == 3: r[rd] = signed(r[rt]) >> sh
            elif fn == 8: pending = r[rs]
            elif fn == 18: r[rd] = lo
            elif fn == 25: lo = (r[rs]*r[rt]) & 0xffffffff
            elif fn == 33: r[rd] = r[rs] + r[rt]
            elif fn == 37: r[rd] = r[rs] | r[rt]
            elif fn == 42: r[rd] = int(signed(r[rs]) < signed(r[rt]))
            else: raise AssertionError('unsupported integer instruction')
        elif op in (4, 20): branch(r[rs] == r[rt], op == 20)
        elif op == 5: branch(r[rs] != r[rt])
        elif op == 22: branch(signed(r[rs]) <= 0, True)
        elif op == 9: r[rt] = r[rs] + imm
        elif op == 15: r[rt] = (word & 65535) << 16
        elif op in (33, 35, 36, 37, 49):
            size = 1 if op == 36 else 2 if op in (33, 37) else 4
            value = get((r[rs]+imm) & 0xffffffff, size)
            if op == 49: fp[rt] = value
            else: r[rt] = narrow(value) if op == 33 else value
        elif op in (41, 43, 57):
            address = (r[rs]+imm) & 0xffffffff
            size = 2 if op == 41 else 4
            require(stack-24 <= address and address+size <= stack+8, 'write outside local frame/argument homes')
            put(address, size, fp[rt] if op == 57 else r[rt])
        elif op == 17:
            if rs == 4: fp[rd] = r[rt]
            elif rs == 8: branch(cc if rt & 1 else not cc, bool(rt & 2))
            elif rs == 20 and fn == 32: fp[sh] = float_bits(f32(signed(fp[rd])))
            elif rs == 16:
                if fn in (0, 1, 2):
                    a, b = val(rd), val(rt)
                    result = a+b if fn == 0 else a-b if fn == 1 else a*b
                    fp[sh] = float_bits(f32(result))
                elif fn == 60: cc = val(rd) < val(rt)
                elif fn == 62: cc = val(rd) <= val(rt)
                else: raise AssertionError('unsupported floating instruction')
            else: raise AssertionError('unsupported coprocessor instruction')
        else:
            raise AssertionError('unsupported instruction')
        r = [value & 0xffffffff for value in r]
        r[0] = 0
        pc = delayed if delayed is not None else nxt
    require([bytes(data) for _, data in regions[:-1]] == snapshots, 'native changed read-only data')
    require(all(r[i] == before[i] for i in list(range(16, 24)) + [28, 29, 30, 31]), 'saved register clobber')
    require(fp[20:] == before_fp[20:], 'saved floating register clobber')
    return signed(r[2])


def fixtures():
    rng = random.Random(0xba46c)
    base = dict(who=0, route=0, count=1, type=0, range=4, px=0., pz=0., dx=1., dz=0., points=[(-2,-32768,0),(2,32767,0)])
    yield base
    for key, values in [('who', [0,1,9,0x12340000,0xffff0009]), ('route', [0,1,14,15,32767,0x12340000]),
                        ('count', range(16)), ('type', [0,1,2,3,127,255]), ('range', [-2147483648,-1,0,1,3,4,5,2147483647]),
                        ('dx', [-2.,-1.,-0.,0.,0.5,1.,2.]), ('dz', [-2.,-1.,0.,1.,2.])]:
        for value in values:
            yield dict(base, **{key:value})
    for points in [[],[(0,0,0)], [(-2,0,0),(100,0,0),(2,0,0)], [(-100,0,0),(-2,0,0),(2,0,0)],
                   [(0,0,-2),(0,0,2)], [(-1,0,0),(0,0,0)], [(-32768,0,32767),(32767,0,-32768)], [(1,0,0)]*256]:
        yield dict(base, points=points)
    for _ in range(4096):
        n = rng.choice([0,1,2,3,4,8,16,32])
        points = [(rng.randrange(-128,129),rng.choice([-32768,0,32767]),rng.randrange(-128,129)) for i in range(n)]
        yield dict(who=rng.randrange(10) | rng.choice([0,0x12340000,0xffff0000]), route=rng.randrange(18),
                   count=rng.randrange(16), type=rng.choice([0,0,0,1,2,3,255]), range=rng.choice([-1,0,1,4,100,1000,10000,2147483647]),
                   px=rng.randrange(-32,33)/4., pz=rng.randrange(-32,33)/4.,
                   dx=rng.randrange(-8,9)/4., dz=rng.randrange(-8,9)/4., points=points)
    # Last safe signed-halfword loop limit. Higher counts can wrap the native loop.
    yield dict(base, points=[(1,32767,0)]*32767)


def run_behavior(native, linked, work):
    coverage, branches, cases = set(), {}, list(fixtures())
    path = work/'fixtures.txt'
    with path.open('w') as out:
        for case in cases:
            expected = oracle(case)
            require(execute(native, case, coverage, branches) == expected, 'native/oracle mismatch')
            require(execute(linked, case) == expected, 'GNU/oracle mismatch')
            values = [case[k] for k in ('who','route','count','type')] + [len(case['points']),case['range']] + [case[k] for k in ('px','pz','dx','dz')] + [expected]
            out.write(' '.join(map(str,values))+'\n')
            for point in case['points']:
                out.write(' '.join(map(str,point))+'\n')
    common = ['gcc','-std=c89','-pedantic-errors','-O2','-fstrict-aliasing','-ffp-contract=off',
              '-fsanitize=undefined,bounds,float-cast-overflow','-fno-sanitize-recover=all','-I',str(ROOT)]
    binary = work/'host'
    subprocess.run(common+[str(PACKET/'host.c'),'-o',str(binary)],check=True,capture_output=True,text=True)
    output = subprocess.check_output([str(binary),str(path)],text=True).strip()
    require(output == str(len(cases))+' cases passed', 'host fixture count mismatch')
    # Reuse the original bounded real-caller tests against this exact matching file.
    legacy = work/'legacy'
    subprocess.run(common+[str(PACKET/'legacy_host.c'),'-o',str(legacy)],check=True,capture_output=True,text=True)
    old_output = subprocess.check_output([str(legacy)],text=True)
    require('395264 mixer cases; 23094 priority cases' in old_output, 'legacy regression count mismatch')
    source = (GROUP/'group.c').read_text()
    mutants = [('range','route >= D_801407F0.count','route > D_801407F0.count'),
               ('type','r->type == 2','r->type == 3'), ('zero_side','< 0.0f','<= 0.0f'),
               ('distance','<= D_80151CE8.tracks[who].range','< D_80151CE8.tracks[who].range'),
               ('crossing','prev != side','prev == side')]
    for tag, old, new in mutants:
        require(old in source, 'mutant site absent')
        directory = work/tag
        file = directory/GROUP.relative_to(ROOT)/'group.c'
        file.parent.mkdir(parents=True)
        file.write_text(source.replace(old,new,1))
        executable = directory/'host'
        args = common.copy(); at = args.index('-I'); args[at:at] = ['-I',str(directory)]
        subprocess.run(args+[str(PACKET/'host.c'),'-o',str(executable)],check=True,capture_output=True,text=True)
        require(subprocess.run([str(executable),str(path)],capture_output=True).returncode != 0, 'wrong-source mutant escaped: '+tag)
    for alteration in ('unknown','truncated','return'):
        words = list(native)
        if alteration == 'unknown': words[0] = 0xffffffff
        elif alteration == 'truncated': words = words[:-1]
        else: words[0x180//4] = 0x24020000
        rejected = False
        try:
            rejected = execute(words,cases[0]) != oracle(cases[0])
        except AssertionError:
            rejected = True
        require(rejected, 'native negative control escaped')
    conditional = {offset:outcomes for offset,outcomes in branches.items() if len(outcomes) > 1}
    return {'fixtures':len(cases),'native_and_gnu_executions':2*len(cases),'host_c89_ubsan_bounds_cases':len(cases),
            'legacy_caller_cases':395264,'legacy_helper_cases':23094,'native_offsets_executed':len(coverage),
            'conditional_branches_both_outcomes':len(conditional),
            'uncovered_offsets':[hex(i) for i in range(0,len(native)*4,4) if i not in coverage],
            'wrong_source_mutants_rejected':len(mutants),'native_negative_controls_rejected':3}


def proof(history, behavior=True):
    require((score.IDO/'cc').is_file() and shutil.which('mips-linux-gnu-ld'), 'pinned IDO and MIPS GNU linker required')
    require((GROUP/'group.c').read_text().splitlines()[0] == '/* flags: '+FLAGS+' */','bare O3 header changed')
    spec = json.loads((GROUP/'group.json').read_text())
    require(spec['flags'] == FLAGS and spec['claims'] == [FN] and spec['keep'] == [CALLER], 'group recipe changed')
    context = historical_source(history)
    native, addresses = score.targets()[FN], score.image_symbols()
    require(len(native) == 108 and addresses[FN] == 0x800ba46c, 'native identity changed')
    base_targets, base_addresses = historical_targets(history)
    calls = []
    for name, words in base_targets.items():
        for index, word in enumerate(words):
            if word >> 26 in (2,3) and (word & 0x3ffffff)<<2 == 0xba46c:
                calls.append({'caller':name,'address':hex(base_addresses[name]+index*4)})
    require(len(calls) == 2 and {row['caller'] for row in calls} == {CALLER}, 'base direct-call census changed')
    with tempfile.TemporaryDirectory(prefix='route-crossing-proof-') as temp:
        work = Path(temp)
        obj = work/'group.o'; score.compile_group(GROUP,obj)
        data, sections = score._elf(obj)
        symbols = [sym for i,sec in enumerate(sections) if sec['type'] == 2 for sym in score._symbol_table(data,sections,i)]
        funcs = [sym for sym in symbols if sym['type'] == 2 and sym['section'] == score._text_index(sections)]
        require({sym['name'] for sym in funcs} == {FN,CALLER}, 'unexpected function/stub')
        target = next(sym for sym in funcs if sym['name'] == FN)
        require(target['value'] == 0 and target['size'] == 432, 'wrong full ELF target extent')
        words = score.text_words(obj)
        got,masks,unresolved,unverified,errors = score.relocate(obj,words,0,target['size'],addresses)
        require(not(masks or unresolved or unverified or errors),'unclean target relocations')
        require(got[:108] == native,'complete target mismatch')
        relocs = [struct.unpack_from('>II',data,off) for sec in sections if sec['type']==9 and sec['info']==score._text_index(sections)
                  for off in range(sec['off'],sec['off']+sec['size'],8)]
        target_relocs = [offset for offset,info in relocs if offset<432]
        require(target_relocs == [12,16,120,124], 'target relocation sites changed')
        storage = [sec for sec in sections if sec['name'] in ('.data','.rodata','.rdata','.bss','.sdata','.sbss') and sec['size']]
        require(not storage,'unexpected owned data/storage')
        end = max(sym['value']+sym['size'] for sym in funcs)
        tail = struct.pack('>'+str(len(words))+'I',*words)[end:]
        require(tail == b'\0'*len(tail),'nonzero alignment tail')
        externs = {sym['name']:addresses.get(sym['name'],score.address_named(sym['name'])) for sym in symbols if sym['section']==0 and sym['name']}
        require(all(value is not None for value in externs.values()),'unbound whole-object external')
        script = work/'link.ld'
        script.write_text('SECTIONS { .text 0x800ba46c : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.options) } }\n'+
                          '\n'.join(name+' = '+hex(value)+';' for name,value in sorted(externs.items())))
        linked = work/'linked.elf'
        subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(obj)],check=True,capture_output=True,text=True)
        require(score.symbols(linked)[FN] == addresses[FN], 'GNU function placement mismatch')
        linked_words = score.text_words(linked)
        require(linked_words[:108] == native,'GNU whole-target mismatch')
        all_relocated = score.relocate(obj,words,0,len(words)*4,addresses)
        require(not any(all_relocated[i] for i in (1,2,3,4)), 'whole-object relocation uncertainty')
        require(all_relocated[0] == linked_words,'whole-object GNU/project relocation mismatch')
        with contextlib.redirect_stdout(io.StringIO()):
            result = score.compare(obj,FN)
        require(result.accepted(),'canonical scorer refused match')
        caller_symbol = next(sym for sym in funcs if sym['name'] == CALLER)
        caller_native = base_targets[CALLER]
        caller_body = all_relocated[0][caller_symbol['value']//4:(caller_symbol['value']+caller_symbol['size'])//4]
        caller_differences = sum(a != b for a,b in zip(caller_native,caller_body)) + max(0,len(caller_native)-len(caller_body))
        caller_excess = sum(word != 0 for word in caller_body[len(caller_native):])
        bindings = {str(path.relative_to(ROOT)):digest(path.read_bytes()) for path in
                    [GROUP/'group.c',GROUP/'group.json',PACKET/'verify.py',PACKET/'host.c',PACKET/'legacy_host.c']}
        receipt = {'schema':1,'result':'MATCH','status':'revived verified submission; no new discovery',
                   'target':FN,'native_start':hex(addresses[FN]),'native_end':hex(addresses[FN]+432),
                   'native_bytes':432,'native_sha256':digest(struct.pack('>108I',*native)),
                   'compiled_sha256':digest(struct.pack('>108I',*got[:108])),
                   'flags':FLAGS,'mandatory_backend_flag':score.R4300_AS1,'context':context,'own_files':bindings,
                   'native_direct_calls':calls,'elf_function_bytes':{sym['name']:sym['size'] for sym in funcs},
                   'text_bytes':len(words)*4,'zero_alignment_bytes':len(tail),'target_relocations':len(target_relocs),
                   'target_relocation_offsets':[hex(n) for n in target_relocs],'whole_object_relocations':len(relocs),
                   'external_bindings':len(externs),'owned_data_bytes':0,'gnu_full_target_equal':True,
                   'gnu_whole_object_relocations_equal':True,
                   'caller_context':{'result':'NONMATCH','comparison_base_commit':BASE,'differing_words':caller_differences,'native_words':len(caller_native),
                                     'extra_words':caller_excess,'unresolved':[],'unverified':[],'errors':[]},
                   'accepted_byte_gain':0,'rom_coverage_gain':0}
        if behavior:
            receipt['behavior'] = run_behavior(native,linked_words[:108],work)
        return receipt


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--history-repo',type=Path,default=ROOT)
    parser.add_argument('--output',type=Path)
    parser.add_argument('--check',action='store_true')
    parser.add_argument('--no-behavior',action='store_true')
    args = parser.parse_args()
    result = proof(args.history_repo,not args.no_behavior)
    if args.check:
        require(not args.no_behavior,'--check requires behavior')
        require(result == json.loads((PACKET/'verification.json').read_text()),'fresh portable receipt differs')
    output = json.dumps(result,indent=2,sort_keys=True)+'\n'
    if args.output: args.output.write_text(output)
    print(output,end='')

if __name__ == '__main__':
    main()
