#!/usr/bin/env python3
"""Read-only full-TU replay of the eight func_80023754 wrapper repairs.

Run from the repository root. IDO_DIR may select the pinned IDO installation.
No production source, target, lock, coordinator, or builder is changed.
Only numerical comparisons and hashes are emitted; temporary code/objects expire.
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
from tools.conveyor.seeds.extract_candidates import extract_functions
from tools.conveyor.pipeline.lock import normalize_body

BASE = 'abc0f256e8c6b5e2004662af09aca7c5bf915204'
TU = ROOT / 'src/rom/lib_22300.c'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
PAIRS = {
    'func_80023844': (0xD6, 0x00400000),
    'func_80023870': (0xFA, 0x00800000),
    'func_8002389C': (0x11E, 0x01000000),
    'func_800238C8': (0x130, 0x08000000),
    'func_800238F4': (0x142, 0x04000000),
    'func_80023920': (0x154, 0x02000000),
    'func_8002394C': (0xE8, 0x10000000),
    'func_80023978': (0x10C, 0x20000000),
}
EXISTING = [
    'func_80021700', 'func_80021B9C', 'func_80021BC0', 'func_80021F68',
    'func_8002243C', 'func_8002245C', 'func_800224B0', 'func_800225AC',
    'func_800225DC', 'func_80022A78', 'func_80022C58', 'func_80023544',
    'func_80023710', 'func_80023734', 'func_80023818',
]

def require(condition, reason):
    if not condition:
        raise ValueError(reason)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def normalize(text):
    return ' '.join(text.split())


def body(source):
    text = source.read_text()
    (_, start, end), = list(extract_functions(text))
    return text[start:end]


def splice(text, sources, contexts):
    """Pure-text equivalent of promote_batch.splice's declaration deduplication.

    Unlike that transaction, never touch the production TU or claim promotion.
    """
    for name, source in sources.items():
        pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/%s.s")' % name
        require(text.count(pragma) == 1, 'missing/duplicate passthrough: ' + name)
        have = {normalize(line) for line in text.splitlines() if line.strip()}
        declarations = [s for s in contexts[name]['preamble']
                        if s.startswith('#') or normalize(s) not in have]
        text = text.replace(pragma, '\n'.join(declarations) + '\n' + body(source), 1)
    return text


def compile_tu(source, obj, context):
    command = [sys.executable, str(ROOT / 'tools/asm-processor/build.py'),
               score.ido('cc'), '--', 'mips-linux-gnu-as', '-march=vr4300',
               '-mabi=32', '-I' + str(ROOT / 'include'), '--', '-c',
               *context.CFLAGS, '-o', str(obj), str(source)]
    return subprocess.run(command, cwd=ROOT, capture_output=True, text=True)


def extent(words, functions, name, expected, exact=False):
    require(name in functions, 'function missing: ' + name)
    start = functions[name]
    end = min((v for v in functions.values() if v > start), default=len(words) * 4)
    require(end - start >= expected, 'short function: ' + name)
    if exact:
        require(end - start == expected, 'combined-TU slot extent changed: ' + name)
    else:
        require(all(w == 0 for w in words[(start + expected) // 4:end // 4]),
                'nonzero function overflow: ' + name)
    return start, end


def checked_bytes(obj, name, exact=False):
    """Compare all relocated bytes without masks and prove the owned extent."""
    want = score.targets()[name]
    words, functions = score.text_words(obj), score.symbols(obj)
    start, end = extent(words, functions, name, len(want) * 4, exact)
    data, sections = score._elf(obj)
    sizes = [symbol['size'] for index, section in enumerate(sections) if section['type'] == 2
             for symbol in score._symbol_table(data, sections, index)
             if symbol['type'] == 2 and symbol['section'] == score._text_index(sections)
             and symbol['name'] == name]
    require(sizes == [len(want) * 4], name + ': STT_FUNC size differs from target')
    verdict = score.compare(obj, name, show=0)
    require(verdict.accepted(), name + ': ' + verdict.summary())
    relocated, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + len(want) * 4, score.image_symbols())
    require(not (masks or unresolved or unverified or errors),
            name + ': relocation uncertainty is forbidden')
    actual = relocated[start // 4:(start // 4) + len(want)]
    require(actual == want, name + ': full relocated bytes differ')
    return actual, {
        'expected_bytes': len(want) * 4, 'function_symbol_bytes': sizes[0], 'object_start': start,
        'extent_to_next_symbol_or_text_end': end - start,
        'differing_words': verdict.differing, 'extra_words': verdict.extra_words,
        'unresolved': unresolved, 'unverified': unverified, 'errors': errors,
        'masked_relocations': len(masks), 'strict_match': True,
    }


def allocated_sections(obj):
    data, sections = score._elf(obj)
    shoff, = struct.unpack_from('>I', data, 0x20)
    shentsize, = struct.unpack_from('>H', data, 0x2E)
    result = {}
    for i, section in enumerate(sections):
        flags, = struct.unpack_from('>I', data, shoff + i * shentsize + 8)
        if flags & 2 and section['name'] != '.text':
            result[section['name']] = (
                section['size'], b'' if section['type'] == 8 else
                data[section['off']:section['off'] + section['size']])
    return result


def no_data(obj, context):
    require(not context.data_sections(obj), 'unexpected data section: ' + str(obj))


def compiler_identity():
    pinned = json.loads((ROOT / 'cloud/work/boot_tail/packet2/preflight.json').read_text())
    directory = Path(score.ido('cc')).parent
    observed = {name: sha((directory / name).read_bytes())
                for name in pinned['compiler_files_sha256']}
    require(observed == pinned['compiler_files_sha256'], 'pinned IDO files differ')
    return observed


def host_semantics(tmp, sources):
    """Exercise pointer forwarding, masks, return value and no local mutation."""
    entries = {'func_80023818': (0xC4, 0x00200000), **PAIRS}
    header = '''#include <assert.h>
#include <string.h>
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState_80023818 MacroState_80023818;
typedef struct MacroControl_80023818 MacroControl_80023818;
typedef struct MacroCommand_80023818 { u32 word[2]; } MacroCommand_80023818;
static void *seen_state, *seen_control, *seen_command;
static u32 seen_flag;
static int calls;
void func_80023754(MacroState_80023818 *s, MacroControl_80023818 *c,
                  MacroCommand_80023818 *m, u32 f) {
    seen_state = s; seen_control = c; seen_command = m; seen_flag = f; ++calls;
}
'''
    declarations = '\n'.join('extern u8 %s(MacroState_80023818 *, MacroCommand_80023818 *);' % fn
                             for fn in entries)
    cases = '\n'.join('    {%s, 0x%X, 0x%08XU},' % (fn, offset, flag)
                      for fn, (offset, flag) in entries.items())
    harness = header + declarations + '''
struct Case { u8 (*fn)(MacroState_80023818 *, MacroCommand_80023818 *); unsigned offset; u32 mask; };
static struct Case cases[] = {
''' + cases + '''
};
int main(void) {
    union { u32 align; u8 bytes[0x200]; } state;
    u8 before[0x200];
    MacroCommand_80023818 command, copy;
    unsigned i, j;
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        for (j = 0; j < 4; ++j) {
            memset(state.bytes, 0x31 + j, sizeof(state.bytes));
            memcpy(before, state.bytes, sizeof(before));
            command.word[0] = 0xFEDCBA98U ^ j;
            command.word[1] = j ? 0xFFFFFFFFU >> j : 0;
            copy = command; calls = 0;
            assert(cases[i].fn((MacroState_80023818 *)state.bytes, &command) == 0);
            assert(calls == 1);
            assert(seen_state == state.bytes);
            assert(seen_control == state.bytes + cases[i].offset);
            assert(seen_command == &command);
            assert(seen_flag == cases[i].mask);
            assert(memcmp(state.bytes, before, sizeof(before)) == 0);
            assert(memcmp(&command, &copy, sizeof(copy)) == 0);
        }
    }
    return 0;
}
'''
    source, exe = tmp / 'host.c', tmp / 'host'
    source.write_text(harness)
    accepted = ROOT / 'cloud/work/boot_tail_promotion/sources/func_80023818.c'
    command = ['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
               str(source), str(accepted), *map(str, sources.values()), '-o', str(exe)]
    subprocess.run(command, check=True, capture_output=True, text=True)
    subprocess.run([str(exe)], check=True, capture_output=True, text=True)
    return {'wrapper_count': len(entries), 'cases': 4 * len(entries), 'passed': True}


def base_text(relative):
    return subprocess.check_output(['git', 'show', BASE + ':' + relative],
                                   cwd=ROOT, text=True)


def locked_functions(locks):
    return sorted(key.rsplit(':', 1)[1] for key in locks
                  if key.startswith('src/rom/lib_22300.c:'))


def source_body_hashes(text):
    return {name: sha(normalize_body(text[start:end]).encode())
            for name, start, end in extract_functions(text)}


def current_state(text, locks):
    """Validate actual accepted bodies and distinguish promoted candidate slots."""
    locked = locked_functions(locks)
    require(set(EXISTING) <= set(locked), 'baseline accepted lock missing')
    hashes = source_body_hashes(text)
    for name in locked:
        require(hashes.get(name) == locks['src/rom/lib_22300.c:' + name]['body_sha256'],
                'current locked body changed or missing: ' + name)
    promoted, pending = [], []
    for name in PAIRS:
        pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/%s.s")' % name
        if name in locked:
            require(text.count(pragma) == 0, 'locked candidate still has passthrough: ' + name)
            promoted.append(name)
        else:
            require(text.count(pragma) == 1 and name not in hashes,
                    'unlocked candidate must have one passthrough and no C body: ' + name)
            pending.append(name)
    return locked, promoted, pending


def check_unowned_bytes(baseline, observed, verified_names):
    """Only already fully verified C bodies may differ in relocation encoding.

    Keep untouched assembly and all inter-function/terminal padding byte-exact.
    Unknown jump-table symbols in two unrelated passthroughs are not guessed.
    """
    reference, actual = score.text_words(baseline), score.text_words(observed)
    require(len(reference) == len(actual), 'TU text size changed')
    symbols = score.symbols(baseline)
    allowed = set()
    for name in verified_names:
        start = symbols[name] // 4
        allowed.update(range(start, start + len(score.targets()[name])))
    require(all(a == b or index in allowed
                for index, (a, b) in enumerate(zip(reference, actual))),
            'unverified passthrough or padding bytes changed')
    return reference == actual


def _verify(current_tu_text=None, current_lock_entries=None):
    require(Path.cwd() == ROOT, 'run from repository root')
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    spec = importlib.util.spec_from_file_location('wrapper_context', HERE.parent / 'context_check.py')
    context = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(context)
    context.CC = Path(score.ido('cc'))  # Select executable only; retain exact flags/checker.
    toolchain = compiler_identity()
    targets = score.targets()  # Protected target manifest and region-set integrity gate.
    require(all(len(targets[name]) * 4 == 44 for name in PAIRS), 'candidate extent drift')
    require(sum(len(targets[name]) * 4 for name in EXISTING) == 808, 'accepted extent drift')
    manifest = score.target_manifest()
    for name in manifest:
        score.verified_bytes(score.ASM_DIR / name, manifest)
    sources = {name: HERE / 'sources' / (name + '.c') for name in PAIRS}
    text = base_text('src/rom/lib_22300.c')
    baseline_locks = json.loads(base_text('matched.lock.json'))
    require(locked_functions(baseline_locks) == sorted(EXISTING), 'frozen baseline population changed')
    require((current_tu_text is None) == (current_lock_entries is None),
            'a lifecycle fixture must provide both source and lock entries')
    fixture = current_tu_text is not None
    current_text = TU.read_text() if current_tu_text is None else current_tu_text
    locks = (json.loads((ROOT / 'matched.lock.json').read_text())
             if current_lock_entries is None else current_lock_entries)
    locked, promoted, pending = current_state(current_text, locks)
    accepted_sources = {}
    for name in EXISTING:
        marker = '/* PROMOTED 2026-10-04 — ' + name
        block = text.split(marker, 1)[1].split('*/', 1)[0]
        source = re.search(r'Source:\s+(\S+)', block)[1]
        accepted_sources[name] = ROOT / source
    inputs = {Path(__file__).resolve(), HERE / 'test_verify.py',
              TU, ROOT / 'matched.lock.json', ROOT / 'src/rom/rom_tu.h',
              ROOT / 'include/rom_auto.h', ROOT / 'symbol_addrs.us.txt',
              HERE.parent / 'context_check.py', HERE.parent / 'promote_batch.py',
              ROOT / 'tools/cloud/score.py', ROOT / 'Makefile',
              *sources.values(), *accepted_sources.values(),
              *(HERE.parent / 'sources' / (name + '.c') for name in PAIRS),
              *(score.ASM_DIR / name for name in manifest),
              score.ASM_DIR / 'SHA256SUMS'}
    inputs.update((ROOT / 'asm/us/nonmatchings/rom/lib_22300').glob('*.s'))
    originals = {path: path.read_bytes() for path in inputs}
    results, contexts = [], {}
    with tempfile.TemporaryDirectory(prefix='macro-wrapper-contracts-') as directory:
        tmp = Path(directory)
        baseline_source, baseline = tmp / 'baseline.c', tmp / 'baseline.o'
        baseline_source.write_text(text)
        build = compile_tu(baseline_source, baseline, context)
        require(build.returncode == 0, 'baseline TU compile failed: ' + build.stderr)
        current_source, current = tmp / 'current.c', tmp / 'current.o'
        current_source.write_text(current_text)
        build = compile_tu(current_source, current, context)
        require(build.returncode == 0, 'current TU compile failed: ' + build.stderr)
        all_sources = {**accepted_sources, **sources}
        standalone_bytes = {}
        for name, source in all_sources.items():
            require(source.read_text().splitlines()[0] == '/* flags: ' + FLAGS + ' */',
                    'flags changed: ' + name)
            standalone = tmp / (name + '.standalone.o')
            score.compile_single(source, FLAGS, standalone)
            no_data(standalone, context)
            raw, standalone_report = checked_bytes(standalone, name)
            row = context.check(name, tmp, str(source))
            require(row['status'] == 'ok', name + ': header context failed: ' + str(row))
            contexts[name] = row
            header = tmp / (name + '.ctx.o')
            header_bytes, header_report = checked_bytes(header, name)
            make_standalone_bytes, make_standalone_report = checked_bytes(tmp / (name + '.orig.o'), name)
            no_data(header, context)
            require(raw == header_bytes == make_standalone_bytes, name + ': context bytes changed')
            standalone_bytes[name] = raw
            results.append({'function': name, 'role': 'candidate' if name in PAIRS else 'existing_lock',
                            'source': str(source.relative_to(ROOT)), 'source_sha256': sha(source.read_bytes()),
                            'standalone': standalone_report, 'make_flags_standalone': make_standalone_report,
                            'header_context': header_report})
        combined_source, combined = tmp / 'combined.c', tmp / 'combined.o'
        combined_source.write_text(splice(current_text, {name: sources[name] for name in pending}, contexts))
        build = compile_tu(combined_source, combined, context)
        require(build.returncode == 0, 'combined TU compile failed: ' + build.stderr)
        require(score.symbols(baseline) == score.symbols(current) == score.symbols(combined),
                'TU function offsets changed')
        require(allocated_sections(baseline) == allocated_sections(current) == allocated_sections(combined),
                'allocated non-text sections changed')
        for row in results:
            name = row['function']
            old, row['baseline_tu'] = checked_bytes(baseline, name, exact=True)
            present, row['current_tu'] = checked_bytes(current, name, exact=True)
            new, row['combined_tu'] = checked_bytes(combined, name, exact=True)
            require(old == present == new == standalone_bytes[name], name + ': combined bytes changed')
            row['all_full_relocated_bytes_equal'] = True
        additional = []
        for name in sorted(set(locked) - set(all_sources)):
            old, baseline_report = checked_bytes(baseline, name, exact=True)
            present, current_report = checked_bytes(current, name, exact=True)
            new, combined_report = checked_bytes(combined, name, exact=True)
            require(old == present == new, name + ': later accepted body bytes changed')
            additional.append({'function': name, 'baseline_tu': baseline_report,
                               'current_tu': current_report, 'combined_tu': combined_report,
                               'all_full_relocated_bytes_equal': True})
        current_raw_equal = check_unowned_bytes(baseline, current, set(locked) | set(PAIRS))
        combined_raw_equal = check_unowned_bytes(baseline, combined, set(locked) | set(PAIRS))
        # Reproduce the original refusal without touching the production checkout.
        old_sources = {name: HERE.parent / 'sources' / (name + '.c') for name in PAIRS}
        old_contexts = {name: context.check(name, tmp, str(source)) for name, source in old_sources.items()}
        require(all(r['status'] == 'ok' for r in old_contexts.values()), 'legacy header control unexpectedly fails')
        negative_source = tmp / 'legacy-conflict.c'
        negative_source.write_text(splice(text, old_sources, old_contexts))
        legacy = compile_tu(negative_source, tmp / 'legacy-conflict.o', context)
        require(legacy.returncode != 0 and "redeclaration of 'func_80023754'" in legacy.stderr,
                'legacy combined-TU conflict was not reproduced')
        # Incorrect embedded-record offset must fail strict full-word comparison.
        bad_source, bad_obj = tmp / 'wrong-offset.c', tmp / 'wrong-offset.o'
        bad_source.write_text(sources['func_80023844'].read_text().replace('+ 0xD6)', '+ 0xD7)'))
        score.compile_single(bad_source, FLAGS, bad_obj)
        negative = score.compare(bad_obj, 'func_80023844', show=0)
        require(not negative.accepted() and negative.differing > 0, 'wrong offset was not rejected')
        bad_tu_source, bad_tu_obj = tmp / 'wrong-offset-tu.c', tmp / 'wrong-offset-tu.o'
        bad_tu_source.write_text(combined_source.read_text().replace('+ 0xD6)', '+ 0xD7)'))
        bad_build = compile_tu(bad_tu_source, bad_tu_obj, context)
        require(bad_build.returncode == 0, 'negative combined TU failed to compile')
        negative_tu = score.compare(bad_tu_obj, 'func_80023844', show=0)
        require(not negative_tu.accepted() and negative_tu.differing > 0,
                'wrong combined-TU offset was not rejected')
        semantics = host_semantics(tmp, sources)
        text_bytes = len(score.text_words(combined)) * 4
        function_count = len(score.symbols(combined))
        non_text = {name: size for name, (size, _) in allocated_sections(combined).items()}
    require(all(path.read_bytes() == original for path, original in originals.items()), 'input mutated during replay')
    return {
        'schema_version': 2, 'result': 'PASS', 'base_commit': BASE,
        'candidate_functions': 8, 'candidate_body_bytes': 352,
        'baseline_existing_locked_functions': 15,
        'current_locked_functions': len(locked),
        'already_promoted_candidates': promoted, 'overlaid_candidates': pending,
        'temporary_lifecycle_fixture': fixture,
        'baseline_source_sha256': sha(text.encode()),
        'current_source_sha256': sha(current_text.encode()),
        'current_lock_entries_sha256': sha(json.dumps(locks, sort_keys=True).encode()),
        'standalone_flags': FLAGS, 'standalone_effective_flags': FLAGS + ' -Wab,-r4300_mul',
        'tu_flags': [flag.replace(str(ROOT), '<repo>') for flag in context.CFLAGS],
        'assembler_flags': '-march=vr4300 -mabi=32 -I<repo>/include',
        'actual_asm_processed_tu': str(TU.relative_to(ROOT)),
        'tu_text_bytes': text_bytes, 'tu_function_symbols': function_count,
        'all_tu_function_offsets_unchanged': True, 'all_raw_tu_text_bytes_equal': current_raw_equal and combined_raw_equal,
        'unverified_passthrough_and_padding_bytes_unchanged': True,
        'allocated_non_text_sections': non_text,
        'allocated_non_text_bytes_unchanged': True,
        'negative_controls': {'legacy_full_tu_redeclaration_reproduced': True,
                              'wrong_offset_differing_words': negative.differing,
                              'wrong_combined_tu_offset_differing_words': negative_tu.differing},
        'host_semantics': semantics,
        'compiler_files_sha256': toolchain,
        'inputs_sha256': {str(path.relative_to(ROOT)): sha(value)
                          for path, value in sorted(originals.items())},
        'results': results, 'additional_current_locked_bodies': additional,
        'scope': 'Local source/ABI/context/relocated-byte proof only. No lock migration, promotion, full-ROM gate, remote builder or coverage claim.',
    }


def run(current_tu_text=None, current_lock_entries=None):
    # Lifecycle tests call this in process; never retarget another scorer.
    previous = score.ASM_DIR, score._targets, score._target_fingerprint
    try:
        return _verify(current_tu_text, current_lock_entries)
    finally:
        score.ASM_DIR, score._targets, score._target_fingerprint = previous


if __name__ == '__main__':
    try:
        print(json.dumps(run(), indent=2))
    except (ValueError, OSError, subprocess.CalledProcessError) as exc:
        print(json.dumps({'result': 'FAIL', 'error': str(exc)}))
        sys.exit(1)
