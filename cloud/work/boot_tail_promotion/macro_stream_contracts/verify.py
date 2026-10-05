#!/usr/bin/env python3
"""Read-only strict full-TU proof for eight macro/stream source contracts.

Only temporary copies are overlaid. The real asm-processor, Makefile flags,
protected targets, complete relocated words and exact ELF sizes are required.
No promotion, lock migration, remote builder access or coverage accounting.
"""
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
from tools.conveyor.pipeline.lock import normalize_body
from tools.conveyor.seeds.extract_candidates import extract_functions

BASE = 'cf10b3392d7f00ae42d75c008b79fdc2541aab6b'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
GROUPS = {
    'lib_22300': ('func_80021BF0', 'func_800225FC', 'func_80022678', 'func_80023520'),
    'lib_25bb0': ('func_800254D4', 'func_80025670', 'func_8002574C', 'func_800259A8'),
}
EXISTING = {
    'lib_22300': ('func_80021700', 'func_80021B9C', 'func_80021BC0', 'func_80021F68',
                  'func_8002243C', 'func_8002245C', 'func_800224B0', 'func_800225AC',
                  'func_800225DC', 'func_80022A78', 'func_80022C58', 'func_80023544',
                  'func_80023710', 'func_80023734', 'func_80023818'),
    'lib_25bb0': ('func_80024FB0', 'func_8002506C', 'func_800250AC',
                  'func_80025264', 'func_80025594'),
}


def require(test, reason):
    if not test:
        raise ValueError(reason)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def frozen(path):
    return subprocess.check_output(['git', 'show', BASE + ':' + path], cwd=ROOT, text=True)


def bodies(text):
    return {fn: text[start:end] for fn, start, end in extract_functions(text)}


def body_hash(text):
    return sha(normalize_body(text).encode())


def context_module():
    spec = importlib.util.spec_from_file_location('macro_stream_context', HERE.parent / 'context_check.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.CC = Path(score.ido('cc'))
    return module


def pragma(group, fn):
    return '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/%s/%s.s")' % (group, fn)


def state(group, text, locks):
    found = bodies(text)
    prefix = 'src/rom/' + group + '.c:'
    locked = {key[len(prefix):]: value for key, value in locks.items() if key.startswith(prefix)}
    require(set(EXISTING[group]) <= set(locked), 'baseline accepted lock missing')
    require(set(found) == set(locked), 'TU C bodies and current locks differ')
    for fn, lock in locked.items():
        require(body_hash(found[fn]) == lock['body_sha256'], 'current locked body mismatch: ' + fn)
    pending = []
    for fn in GROUPS[group]:
        if fn in found:
            require(text.count(pragma(group, fn)) == 0, 'promoted candidate still has assembly')
        else:
            require(text.count(pragma(group, fn)) == 1, 'candidate slot missing or duplicated: ' + fn)
            pending.append(fn)
    return pending, sorted(locked)


def splice(group, text, paths, rows):
    """Use the production promotion driver's exact-statement deduplication."""
    for fn, path in paths.items():
        marker = pragma(group, fn)
        require(text.count(marker) == 1, 'expected one passthrough: ' + fn)
        have = {' '.join(line.split()) for line in text.splitlines() if line.strip()}
        decls = [s for s in rows[fn]['preamble'] if s.startswith('#') or ' '.join(s.split()) not in have]
        text = text.replace(marker, '\n'.join(decls) + '\n' + bodies(path.read_text())[fn], 1)
    return text


def build(text, tmp, label, context, failure=None):
    source, obj = tmp / (label + '.c'), tmp / (label + '.o')
    source.write_text(text)
    command = [sys.executable, str(ROOT / 'tools/asm-processor/build.py'), score.ido('cc'),
               '--', 'mips-linux-gnu-as', '-march=vr4300', '-mabi=32', '-I' + str(ROOT / 'include'),
               '--', '-c', *context.CFLAGS, '-o', str(obj), str(source)]
    proc = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if failure:
        require(proc.returncode != 0 and failure in proc.stderr,
                'expected declaration conflict was not reproduced: ' + label)
        return None
    require(proc.returncode == 0, label + ': ' + proc.stdout + proc.stderr)
    return obj


def exact_bytes(obj, fn, standalone=False):
    target = score.targets()[fn]
    words, symbols = score.text_words(obj), score.symbols(obj)
    require(fn in symbols, 'missing compiled function: ' + fn)
    start = symbols[fn]
    end = min((v for v in symbols.values() if v > start), default=len(words) * 4)
    require(end - start >= len(target) * 4, 'short function extent: ' + fn)
    if standalone:
        require(all(v == 0 for v in words[(start + len(target)*4)//4:end//4]), 'nonzero overflow: ' + fn)
    elif (end == len(words) * 4 and end % 16 == 0 and end - start - len(target)*4 < 16
          and all(v == 0 for v in words[(start + len(target)*4)//4:end//4])):
        # The TU's final function (func_80025DC0, promoted after BASE) is
        # followed only by the .text section's 16-byte alignment padding. The
        # ELF size check below still requires the exact function extent.
        pass
    else:
        require(end - start == len(target)*4, 'full-TU extent mismatch: ' + fn)
    data, sections = score._elf(obj)
    sizes = [symbol['size'] for i, section in enumerate(sections) if section['type'] == 2
             for symbol in score._symbol_table(data, sections, i)
             if symbol['section'] == score._text_index(sections) and symbol['type'] == 2 and symbol['name'] == fn]
    require(sizes == [len(target)*4], 'ELF function size mismatch: ' + fn)
    result = score.compare(obj, fn, show=0)
    require(result.accepted(False), fn + ': ' + result.summary())
    relocated, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + len(target)*4, score.image_symbols())
    require(not (masks or unresolved or unverified or errors), 'relocation uncertainty: ' + fn)
    actual = relocated[start//4:(start//4)+len(target)]
    require(actual == target, 'full relocated words differ: ' + fn)
    return {'bytes': len(target)*4, 'elf_size': sizes[0], 'offset': start,
            'span': end-start, 'full_relocated_bytes_equal': True,
            'differing_words': result.differing, 'extra_words': result.extra_words,
            'masked_relocations': len(masks), 'unresolved': unresolved,
            'unverified': unverified, 'errors': errors}


def allocated(obj, include_reginfo=False):
    # .reginfo is compiler/assembler register-use metadata, not a C data payload.
    # asm-processor ORs masks for generated placeholders with real assembly.
    # Report its hashes separately; require its size and GP value unchanged.
    data, sections = score._elf(obj)
    shoff, = struct.unpack_from('>I', data, 0x20)
    stride, = struct.unpack_from('>H', data, 0x2E)
    out = {}
    for i, section in enumerate(sections):
        flags, = struct.unpack_from('>I', data, shoff+i*stride+8)
        if flags & 2 and section['name'] != '.text' and (include_reginfo or section['name'] != '.reginfo'):
            out[section['name']] = (section['size'], '' if section['type'] == 8 else
                                    sha(data[section['off']:section['off']+section['size']]))
    return out


def untouched(baseline, obj, verified):
    before, after = score.text_words(baseline), score.text_words(obj)
    require(len(before) == len(after), 'TU text size changed')
    require(score.symbols(baseline) == score.symbols(obj), 'TU function offsets changed')
    def reginfo(o):
        data, sections = score._elf(o)
        matches = [s for s in sections if s['name']=='.reginfo']
        require(len(matches)==1 and matches[0]['size']==24, 'invalid register metadata')
        section = matches[0]
        return data[section['off']:section['off']+24]
    require(reginfo(baseline)[20:] == reginfo(obj)[20:], 'reginfo GP changed')
    require(allocated(baseline) == allocated(obj), 'allocated non-text bytes changed: ' + str(obj) + ': ' + str(allocated(baseline)) + ' vs ' + str(allocated(obj)))
    allowed = set()
    for fn in verified:
        start = score.symbols(baseline)[fn]//4
        allowed.update(range(start, start+len(score.targets()[fn])))
    require(all(a == b or i in allowed for i, (a,b) in enumerate(zip(before,after))),
            'unverified assembly or padding changed')
    return before == after


def layout_checks(tmp, context, stream_text):
    """IDO target-ABI static checks, never host sizeof(pointer) assumptions."""
    record = re.search(r'typedef struct StreamState_80025264 \{[^\n]+', stream_text)[0]
    checks = {'callback':0x190, 'read_count':0x11A0, 'queue':0x11AC, 'state':0x11DD,
              'scale':0x11DE, 'request_queue':0x11E0, 'request_messages':0x11F8,
              'busy':0x1200, 'value1':0x1201, 'rate':0x1208, 'handle':0x120C,
              'buffer':0x1210, 'request_state':0x1218, 'processed':0x121C,
              'field_1220':0x1220, 'token':0x1224}
    source = '#include "rom_tu.h"\n#define offsetof(T,m) ((unsigned int)&(((T *)0)->m))\n' + record + '\n'
    source += 'typedef char record_size[(sizeof(StreamState_80025264)==0x1228)?1:-1];\n'
    for field, offset in checks.items():
        source += 'typedef char offset_%s[(offsetof(StreamState_80025264,%s)==0x%X)?1:-1];\n' % (field,field,offset)
    source += 'typedef char queue_size[(sizeof(OSMesgQueue)==0x18)?1:-1];\n'
    ok, error = context.compile_c(source, tmp/'layout.o')
    require(ok, 'MIPS layout assertions failed: '+error)
    return {'record_size':0x1228, 'queue_size':0x18, 'offsets':checks, 'passed':True}


def host_semantics(tmp):
    exe = tmp/'host-macro'
    command = ['cc','-std=c89','-Wall','-Wextra','-Werror',str(HERE/'host_macro.c')]
    command += [str(HERE/'sources'/(fn+'.c')) for fn in GROUPS['lib_22300']]
    subprocess.run(command+['-o',str(exe)], check=True, capture_output=True, text=True)
    subprocess.run([str(exe)], check=True, capture_output=True, text=True)
    first = (HERE/'sources/func_800254D4.c').read_text()
    header = first[:first.index('extern StreamState_80025264')]
    (tmp/'stream_contract.h').write_text(header)
    exe = tmp/'host-stream'
    command = ['cc','-std=c89','-Wall','-Wextra','-Werror','-I'+str(tmp),str(HERE/'host_stream.c')]
    command += [str(HERE/'sources'/(fn+'.c')) for fn in GROUPS['lib_25bb0']]
    subprocess.run(command+['-o',str(exe)], check=True, capture_output=True, text=True)
    subprocess.run([str(exe)], check=True, capture_output=True, text=True)
    return {'macro_cases':96,'stream_cases':50,'passed':True}


def run(fixtures=None):
    previous = score.ASM_DIR, score._targets, score._target_fingerprint
    try:
        return _run(fixtures)
    finally:
        score.ASM_DIR, score._targets, score._target_fingerprint = previous


def _run(fixtures=None):
    require(Path.cwd().resolve() == ROOT, 'run from repository root')
    score.ASM_DIR = ROOT/'asm/us/boot_tail'
    score.targets()
    context = context_module()
    pinned = json.loads((ROOT/'cloud/work/boot_tail/packet2/preflight.json').read_text())['compiler_files_sha256']
    compiler = {name:sha((Path(score.ido('cc')).parent/name).read_bytes()) for name in pinned}
    require(compiler == pinned, 'compiler is not pinned IDO')
    locks = json.loads((ROOT/'matched.lock.json').read_text())
    reports, candidates = [], []
    inputs = {ROOT/'src/rom/rom_tu.h', ROOT/'include/rom_auto.h', ROOT/'matched.lock.json',
              ROOT/'symbol_addrs.us.txt', ROOT/'Makefile', ROOT/'tools/cloud/score.py',
              HERE.parent/'context_check.py', HERE.parent/'promote_batch.py', Path(__file__).resolve(), HERE/'host_macro.c', HERE/'host_stream.c'}
    inputs.update(HERE.glob('sources/*.c'))
    inputs.update((ROOT/'asm/us/boot_tail'/name) for name in score.target_manifest())
    inputs.add(ROOT/'asm/us/boot_tail/SHA256SUMS')
    inputs.update(ROOT/path for path in ('include/types.h','include/PR/os_message.h',
                  'include/PR/os_thread.h','include/m2c_types.h',
                  'tools/asm-processor/build.py','tools/asm-processor/asm_processor.py',
                  'tests/cloud/test_macro_stream_contracts.py'))
    for group in GROUPS:
        inputs.update((ROOT/'asm/us/nonmatchings/rom'/group).glob('*.s'))
    inputs.update(ROOT/'src/rom'/('%s.c'%group) for group in GROUPS)
    originals = {path:path.read_bytes() for path in inputs}
    with tempfile.TemporaryDirectory(prefix='macro-stream-contracts-') as directory:
        tmp = Path(directory)
        for group, names in GROUPS.items():
            path = 'src/rom/'+group+'.c'
            baseline_text = frozen(path)
            current_text = (ROOT/path).read_text()
            active_locks = locks
            if fixtures and group in fixtures:
                current_text, active_locks = fixtures[group]
            pending, locked = state(group, current_text, active_locks)
            paths = {fn:HERE/'sources'/(fn+'.c') for fn in names}
            rows = {}
            for fn, source in paths.items():
                require(source.read_text().splitlines()[0] == '/* flags: '+FLAGS+' */', 'source flags changed')
                obj = tmp/(fn+'.standalone.o')
                score.compile_single(source, FLAGS, obj)
                standalone = exact_bytes(obj, fn, True)
                require(not context.data_sections(obj), 'candidate allocated data')
                row = context.check(fn, tmp, str(source))
                require(row['status'] == 'ok', 'header context: '+str(row))
                rows[fn] = row
                candidates.append({'function':fn, 'source':str(source.relative_to(ROOT)),
                                   'source_sha256':sha(source.read_bytes()), 'standalone':standalone,
                                   'make_flags_standalone':exact_bytes(tmp/(fn+'.orig.o'),fn,True),
                                   'rom_header':exact_bytes(tmp/(fn+'.ctx.o'),fn,True)})
            baseline = build(baseline_text,tmp,group+'-baseline',context)
            current = build(current_text,tmp,group+'-current',context)
            combined_text = splice(group,current_text,{fn:paths[fn] for fn in pending},rows)
            combined = build(combined_text,tmp,group+'-combined',context)
            checks = []
            for fn in sorted(set(locked)|set(names)):
                # Bodies promoted by later boot-tail waves were assembly
                # passthroughs at BASE; their asm-processor label spans the TU's
                # alignment padding. They are not this packet's claims: their
                # accepted C is verified in the current/combined objects, and
                # untouched() still compares every other baseline word.
                later = fn not in names and fn not in EXISTING[group]
                checks.append({'function':fn, 'role':'candidate' if fn in names else
                               'promoted_after_base' if later else 'existing_lock',
                               'baseline':None if later else exact_bytes(baseline,fn),
                               'current':exact_bytes(current,fn),
                               'combined':exact_bytes(combined,fn)})
            same_current = untouched(baseline,current,set(locked)|set(names))
            same_combined = untouched(baseline,combined,set(locked)|set(names))
            # Reproduce at least one original same-TU failure in each family.
            original_fn = names[0]
            original_path = HERE.parent/'sources'/(original_fn+'.c')
            original_context = context.check(original_fn,tmp,str(original_path))
            require(original_context['status']=='ok', 'legacy context unexpectedly rejected')
            negative_text = splice(group,baseline_text,{original_fn:original_path},{original_fn:original_context})
            conflict = "redeclaration of 'func_80021BC0'" if group=='lib_22300' else "redeclaration of 'D_80056230'"
            build(negative_text,tmp,group+'-legacy',context,failure=conflict)
            if group=='lib_22300':
                bad_text=combined_text.replace('state->field00 = state->field08;', 'state->field00 = state->field0C;')
                bad_fn='func_80021BF0'
            else:
                bad_text=combined_text.replace('unsigned char unknown_1205[3];', 'unsigned char unknown_1205[7];')
                bad_fn='func_800254D4'
            require(bad_text != combined_text, 'negative mutation missing')
            bad_obj=build(bad_text,tmp,group+'-bad',context)
            verdict=score.compare(bad_obj,bad_fn,show=0)
            require(not verdict.accepted(False), 'wrong source contract accepted by strict comparison')
            reports.append({'tu':path,'baseline_existing_locks':len(EXISTING[group]),
                            'current_locked_functions':len(locked),'pending_candidates':pending,
                            'already_promoted_candidates':sorted(set(names)-set(pending)),
                            'tu_functions':len(score.symbols(combined)),
                            'tu_text_bytes':len(score.text_words(combined))*4,
                            'all_function_offsets_unchanged':True,
                            'unverified_passthrough_and_padding_unchanged':True,
                            'all_raw_text_equal':same_current and same_combined,
                            'allocated_payload_sections_unchanged':True,'allocated_sections':allocated(combined),
                            'reginfo_baseline':allocated(baseline,True).get('.reginfo'),
                            'reginfo_combined':allocated(combined,True).get('.reginfo'),
                            'legacy_conflict_reproduced':True,'negative_differing_words':verdict.differing,
                            'baseline_source_sha256':sha(baseline_text.encode()),
                            'current_source_sha256':sha(current_text.encode()),'results':checks})
        layout = layout_checks(tmp,context,(ROOT/'src/rom/lib_25bb0.c').read_text())
        semantics = host_semantics(tmp)
    require(all(path.read_bytes()==data for path,data in originals.items()), 'replay mutated input')
    return {'result':'PASS','base_commit':BASE,'candidate_count':8,
            'candidate_bytes':sum(len(score.targets()[fn])*4 for group in GROUPS.values() for fn in group),
            'existing_lock_count':20,'temporary_lifecycle_fixture':bool(fixtures),
            'compiler_files_sha256':compiler,'standalone_flags':FLAGS,
            'tu_flags':[flag.replace(str(ROOT),'<repo>') for flag in context.CFLAGS],
            'layout':layout,'host_semantics':semantics,'candidates':candidates,'groups':reports,
            'inputs_sha256':{str(path.relative_to(ROOT)):sha(data) for path,data in sorted(originals.items())},
            'scope':'Source/type repair proof only; no promotion, lock migration, ROM gate or new matching credit.'}


if __name__=='__main__':
    try:
        print(json.dumps(run(),indent=2))
    except (ValueError,OSError,subprocess.CalledProcessError) as error:
        print(json.dumps({'result':'FAIL','error':str(error)}))
        sys.exit(1)
