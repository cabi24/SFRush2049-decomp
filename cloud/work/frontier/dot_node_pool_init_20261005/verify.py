#!/usr/bin/env python3
"""Rebuild complete source-bound ELF/GNU and bounded native/host proofs."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
spec = importlib.util.spec_from_file_location('node_pool_native', HERE / 'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
FN = 'func_800B2BDC'
POP = 'func_80090284'
SOURCE = ROOT / 'cloud/matches/func_800B2BDC.c'
ACCEPTED = ROOT / 'src/blob/func_80090284.c'
BASELINE = ROOT / 'cloud/work/tiny_A46/func_800B2BDC.c'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'


def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def digest(data): return hashlib.sha256(data).hexdigest()
def packed(words): return struct.pack('>%dI' % len(words), *words)
def run(args, **kw): return subprocess.run(args, check=True, capture_output=True, **kw)


def elf(obj):
    data, sections = score._elf(obj)
    syms = [s for i, sec in enumerate(sections) if sec['type'] == 2
            for s in score._symbol_table(data, sections, i)]
    return data, sections, syms


def function(obj, name):
    data, sections, syms = elf(obj)
    fn, = [s for s in syms if s['name'] == name and s['type'] == 2]
    assert fn['size'] % 4 == 0
    sec = sections[fn['section']]
    start = sec['off'] + fn['value'] - sec.get('addr', 0)
    return fn, list(struct.unpack('>%dI' % (fn['size'] // 4), data[start:start + fn['size']]))


def inspect(obj, name, work, must_match=True):
    data, sections, syms = elf(obj)
    fn, _ = function(obj, name)
    target = score.targets()[name]
    address = score.image_symbols()[name]
    relocs = []
    for sec in sections:
        if sec['type'] != 9 or sec['info'] != fn['section']: continue
        table = score._symbol_table(data, sections, sec['link'])
        for off in range(sec['off'], sec['off'] + sec['size'], 8):
            site, info = struct.unpack_from('>II', data, off)
            if fn['value'] <= site < fn['value'] + fn['size']:
                relocs.append(dict(offset=site - fn['value'], type=info & 255,
                                   symbol=table[info >> 8]['name']))
    assert all(row['type'] in (5, 6) for row in relocs)
    bss = [s for s in syms if s['name'] == 'D_80138880' and s['section'] != 0]
    if bss:
        assert len(bss) == 1 and bss[0]['size'] == 2400 and bss[0]['value'] == 0
        assert sections[bss[0]['section']]['name'] == '.bss'
        assert sections[bss[0]['section']]['size'] == 2400
    assert not any(s['size'] for s in sections if s['name'] in ('.data', '.rodata', '.rdata', '.sdata', '.sbss'))
    definitions = []
    for sym in syms:
        if sym['section'] == 0 and sym['name']:
            addr = score.image_symbols().get(sym['name'], score.address_named(sym['name']))
            assert addr is not None, sym['name']
            definitions.append('%s = 0x%x;' % (sym['name'], addr))
    script = work / (name + '.ld')
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) }\n'
                      '.bss 0x80138880 (NOLOAD) : { *(.bss) *(COMMON) }\n'
                      '/DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n' % (address - fn['value']) + '\n'.join(definitions))
    linked = work / (name + '.elf')
    run(['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(linked), str(obj)])
    lddata, ldsections, ldsyms = elf(linked)
    ldfn, = [s for s in ldsyms if s['name'] == name and s['type'] == 2]
    assert ldfn['value'] == address and ldfn['size'] == fn['size']
    text = ldsections[ldfn['section']]
    # Read the section virtual address directly from the standard ELF header.
    shoff, = struct.unpack_from('>I', lddata, 32)
    shsize, = struct.unpack_from('>H', lddata, 46)
    section_addr, = struct.unpack_from('>I', lddata, shoff + shsize * ldfn['section'] + 12)
    offset = text['off'] + ldfn['value'] - section_addr
    body = lddata[offset:offset + ldfn['size']]
    words = list(struct.unpack('>%dI' % (len(body) // 4), body))
    if bss:
        linked_bss, = [s for s in ldsyms if s['name'] == 'D_80138880']
        assert linked_bss['value'] == native.POOL and linked_bss['size'] == 2400
    differing = [4 * i for i in range(max(len(target), len(words)))
                 if i >= len(target) or i >= len(words) or target[i] != words[i]]
    canonical = asdict(score.compare(obj, name, show=0))
    if must_match:
        assert not differing
        assert canonical['differing'] == 0
        assert all(not canonical[k] for k in ['unresolved', 'unverified', 'errors'])
    tail = data[sections[fn['section']]['off'] + fn['value'] + fn['size']:
                sections[fn['section']]['off'] + sections[fn['section']]['size']]
    return dict(canonical=canonical, elf_function_bytes=fn['size'], native_bytes=4 * len(target),
                complete_differing_offsets=differing, complete_gnu_equal=not differing,
                gnu_body_sha256=digest(body), relocations=relocs,
                standalone_trailing_text_bytes=len(tail), standalone_tail_all_zero=not any(tail),
                owned_bss=[dict(symbol=s['name'], address=hex(native.POOL), bytes=s['size']) for s in bss]), words


def compare_runtime(init_words, pop_words, work):
    ninit, npop = score.targets()[FN], score.targets()[POP]
    cases = [(seed, high, pops) for seed in range(4)
             for high in [-32768, -1, 0, 1, 50, 99, 100, 32767]
             for pops in [0, 1, 2, 3, 4, 49, 98, 99, 100, 101]]
    inputs, outputs = [], []
    coverage, popcoverage, branches = set(), set(), set()
    instruction_runs = 0
    for seed, high, pops in cases:
        initial = native.case(seed, high)
        inputs.append(bytes(initial[native.POOL + i] for i in range(2400)) + struct.pack('>Hh', pops, high))
        a, b, expected = initial.copy(), initial.copy(), initial.copy()
        native.oracle_init(expected)
        for _ in range(2):
            na = native.execute(ninit, native.INIT, a, seed)
            nb = native.execute(init_words, native.INIT, b, seed)
            assert a == b == expected and na[1:] == nb[1:]
            assert len(na[2]) == 203 and not na[3]
            coverage.update(na[1]); branches.update(na[4]); instruction_runs += 2
        for _ in range(pops):
            want = native.oracle_pop(expected)
            na = native.execute(npop, native.POP, a, seed)
            nb = native.execute(pop_words, native.POP, b, seed)
            assert na[0] == nb[0] == want and a == b == expected and na[1:] == nb[1:]
            popcoverage.update(na[1]); instruction_runs += 2
        outputs.append(native.compact(expected))
    exe = work / 'host'
    run(['cc', '-std=c89', '-pedantic-errors', '-O2', '-Wall', '-Wextra',
         '-fsanitize=undefined', '-fno-sanitize-recover=all', str(HERE / 'host.c'),
         str(SOURCE), str(ACCEPTED), '-o', str(exe)])
    actual = run([str(exe)], input=b''.join(inputs)).stdout
    assert actual == b''.join(outputs)
    return dict(cases=len(cases), native_and_gnu_invocations=instruction_runs,
                init_instruction_offsets=sorted(coverage), pop_instruction_offsets=sorted(popcoverage),
                init_branch_outcomes=sorted([list(x) for x in branches]),
                full_memory_canaries='passed', native_gnu_access_traces='passed',
                saved_registers_and_stack='passed', unchanged_host_c89_ubsan='passed',
                host_full_object_bytes_preserved='passed', repeated_init='passed',
                corpus_sha256=digest(b''.join(inputs)), output_sha256=digest(actual))


def verify(work):
    work.mkdir(parents=True, exist_ok=True)
    obj = work / 'candidate.o'
    score.compile_single(SOURCE, FLAGS, obj)
    candidate, body = inspect(obj, FN, work)
    score.compile_single(SOURCE, FLAGS.replace('-O3', '-O2'), work / 'o2.o')
    o2, _ = inspect(work / 'o2.o', FN, work)
    context = work / 'context'
    context.mkdir()
    (context / 'candidate.c').write_bytes(SOURCE.read_bytes())
    (context / 'pop.c').write_bytes(ACCEPTED.read_bytes())
    (context / 'group.json').write_text(json.dumps(dict(files=['candidate.c', 'pop.c'],
         members=[FN], context=[POP], keep=[FN, POP], flags=FLAGS, claims=[FN])))
    score.compile_group(context, work / 'context.o')
    context_init, context_body = inspect(work / 'context.o', FN, context)
    context_pop, popbody = inspect(work / 'context.o', POP, context)
    assert body == context_body
    # Compile ABI assertions separately; they never shape the candidate.
    assertions = work / 'layout.c'
    assertions.write_text('#include "' + str(SOURCE) + '"\n#define offsetof(T,m) ((unsigned int)&(((T *)0)->m))\n' +
        '\n'.join('typedef char check_%s[(%s) ? 1 : -1];' % (n, expr) for n, expr in [
            ('size', 'sizeof(Node)==24'), ('id', 'offsetof(Node,field6)==6'),
            ('next', 'offsetof(Node,next)==0'), ('field4', 'offsetof(Node,field4)==4'),
            ('field8', 'offsetof(Node,field8)==8'), ('fieldC', 'offsetof(Node,fieldC)==12'),
            ('field10', 'offsetof(Node,field10)==16'), ('field14', 'offsetof(Node,field14)==20'),
            ('pool', 'sizeof(D_80138880)==2400')]) + '\n')
    score.compile_single(assertions, FLAGS, work / 'layout.o')
    abi, _ = inspect(work / 'layout.o', FN, work)
    assert abi['complete_gnu_equal']
    semantics = compare_runtime(body, popbody, work)
    source = SOURCE.read_text()
    controls = {'archived_pointer': BASELINE.read_text(),
                'extern_array': source.replace('Node D_80138880[100];', 'extern Node D_80138880[100];'),
                'id_before_next': source.replace('        D_80138880[i].next = &D_80138880[i + 1];\n        D_80138880[i].field6 = -1;',
                  '        D_80138880[i].field6 = -1;\n        D_80138880[i].next = &D_80138880[i + 1];'),
                'two_stores_same_line': source.replace('next = &D_80138880[i + 1];\n        D_80138880[i].field6', 'next = &D_80138880[i + 1]; D_80138880[i].field6')}
    proofs = {}
    for label, text in controls.items():
        path = work / (label + '.c'); path.write_text(text)
        score.compile_single(path, FLAGS, work / (label + '.o'))
        proof, _ = inspect(work / (label + '.o'), FN, work, False)
        proofs[label] = dict(source_sha256=digest(text.encode()), proof=proof)
    mutants = {'short_chain': source.replace('i < 99', 'i < 98'),
               'wrong_id': source.replace('.field6 = -1', '.field6 = 0'),
               'cyclic_tail': source.replace('D_80138880[i].next = 0;', 'D_80138880[i].next = D_80138880;'),
               'missing_counter_reset': source.replace('    D_8012E66C = 0;\n', '')}
    rejected = {}
    for label, text in mutants.items():
        path = work / (label + '.c'); path.write_text(text)
        score.compile_single(path, FLAGS, work / (label + '.o'))
        proof, words = inspect(work / (label + '.o'), FN, work, False)
        mem = native.case(1, 0); expected = mem.copy(); native.oracle_init(expected)
        native.execute(words, native.INIT, mem)
        assert mem != expected and not proof['complete_gnu_equal']
        rejected[label] = dict(source_sha256=digest(text.encode()), complete_differing_words=len(proof['complete_differing_offsets']),
                               behavioral_difference_bytes=sum(mem[a] != expected[a] for a in mem))
    # Decode and memory faults are refused, rather than modeled as no-ops.
    adverse = {}
    for label, words in [('unknown_opcode', [0xffffffff] + body[1:]),
                         ('bad_store_address', body[:2] + [0x3c018077] + body[3:])]:
        try: native.execute(words, native.INIT, native.case(1, 0))
        except (AssertionError, KeyError): adverse[label] = 'rejected'
        else: raise AssertionError(label)
    symbols = score.image_symbols()
    constructor = score.targets()['func_80090308']
    start = symbols['func_80090308']
    def insn(at): return constructor[(at - start) // 4]
    # Accepted allocation result is homed, then linked through the active head.
    assert insn(0x80090318) >> 26 == 3
    assert (insn(0x80090318) & 0x3ffffff) << 2 == (native.POP & 0x0fffffff)
    for at, op, rs, rt, immediate in [
        (0x80090324, 43, 29, 2, 268),
        (0x80090704, 9, 2, 2, (-28176) & 65535),
        (0x80090708, 35, 2, 25, 0),
        (0x8009070c, 35, 29, 14, 268),
        (0x80090714, 43, 14, 25, 0),
        (0x80090718, 43, 2, 14, 0)]:
        word = insn(at)
        assert (word >> 26, (word >> 21) & 31, (word >> 16) & 31, word & 65535) == (op, rs, rt, immediate)
    assert insn(0x800906f4) >> 26 == 15 and (insn(0x800906f4) & 65535) == 0x8014
    active_head = dict(symbol='D_801391F0', role='active Node list head',
                       evidence='func_80090308', native_sha256=digest(packed(constructor)),
                       allocation_site='0x80090318', save_site='0x80090324',
                       link_sites=['0x80090714', '0x80090718'], full_constructor_replayed=False)
    callers = []
    for name, words in score.targets().items():
        for i, word in enumerate(words):
            if word >> 26 == 3 and ((symbols[name] & 0xf0000000) | ((word & 0x3ffffff) << 2)) == native.INIT:
                callers.append(dict(name=name, address=hex(symbols[name] + 4 * i)))
    return dict(status='MATCH', claims=[FN], candidate_bytes=216, accepted_byte_gain=0,
         base='e24b47d89a0c8ffade1e4c75ad76b9d390a1c232', flags=FLAGS,
         source_sha256={str(p.relative_to(ROOT)):sha(p) for p in [SOURCE, ACCEPTED, BASELINE, HERE/'native.py', HERE/'host.c', HERE/'verify.py']},
         tool_sha256={n:sha(score.ido(n)) for n in ['cc','cfe','uld','umerge','uopt','ugen','as1']},
         target_manifest_sha256=sha(score.ASM_DIR/'SHA256SUMS'),
         native=dict(address=hex(native.INIT), end_exclusive=hex(native.INIT+216),
                     sha256=digest(packed(score.targets()[FN])), direct_callers=callers, calls=0),
         active_head_contract=active_head, candidate=candidate, o2=o2, compatible_context={FN:context_init, POP:context_pop},
         abi_assertions=9, bss=dict(address='0x80138880', end_exclusive='0x801391e0', bytes=2400,
                                 verified_range=list(owndata.bss_range(native.POOL,2400))),
         semantics=semantics, source_controls=proofs, wrong_contracts=rejected, adverse_controls=adverse)


def main():
    parser = argparse.ArgumentParser(); parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='node-pool-proof-') as tmp: value = verify(Path(tmp))
    data = json.dumps(value, indent=2, sort_keys=True) + '\n'
    if args.output: args.output.write_text(data)
    else: print(data, end='')
if __name__ == '__main__': main()
