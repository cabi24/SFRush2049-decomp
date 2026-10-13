#!/usr/bin/env python3
"""Replay the bounded source-family investigation; no compiler patches or oracle.

The source lineages are explicit controls, not an exhaustion claim. Scratch C
and objects exist only during the replay. No raw target or object is published.
"""

def require(condition, message):
    if not condition:
        raise AssertionError(message)
import hashlib
import importlib.util
import json
from pathlib import Path
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
BASELINE = ROOT / 'cloud/work/boot_tail/BT02-medium/nonmatch/func_80010DD8.c'

def replace(text, old, new):
    require(text.count(old) == 1, ('ambiguous source anchor', old))
    return text.replace(old, new)

def variants():
    base = BASELINE.read_text()
    result = {'baseline': base}
    for name, declaration in (('quantum_local_u32', 'unsigned int'), ('quantum_local_s32', 'int'), ('register_quantum', 'register unsigned int')):
        value = replace(base, 'unsigned int count;', 'unsigned int count;\n    ' + declaration + ' quantum = 192;')
        result[name] = replace(value, '192 - count % 192', 'quantum - count % quantum')
    result['reuse_rate'] = replace(base, 'unsigned int count;', '').replace('count', 'rate')
    result['reuse_duration_quantum'] = replace(base, 'count += 192 - count % 192;', 'duration = 192;\n        count += duration - count % duration;')
    value = replace(base, 'unsigned int count;', 'unsigned int count;\n    unsigned int sample_rate = rate;\n    unsigned short milliseconds = duration;')
    value = replace(value, 'if (duration != 0)', 'if (milliseconds != 0)')
    result['copies'] = replace(value, 'duration * rate', 'milliseconds * sample_rate')
    result['unsigned_quantum'] = base.replace('192', '192U')
    result['unsigned_all'] = result['unsigned_quantum'].replace('1000', '1000U')
    for name, parent in (('holder_parameters', base), ('unsigned_holder_parameters', result['unsigned_quantum'])):
        result[name] = replace(parent, 'D_800382F4 = count;', 'if (rate) {}\n        if (duration) {}\n        D_800382F4 = count;')
    result['block_quantum'] = replace(result['quantum_local_u32'], 'count += quantum - count % quantum;', 'do {} while (0);\n        count += quantum - count % quantum;')
    result['const_parameters'] = replace(base, 'unsigned int rate, unsigned short duration', 'const unsigned int rate, const unsigned short duration')
    result['register_parameters'] = replace(base, 'unsigned int rate, unsigned short duration', 'register unsigned int rate, register unsigned short duration')
    result['knr_parameters'] = replace(base, 'void func_80010DD8(unsigned int rate, unsigned short duration)', 'void func_80010DD8(rate, duration)\nunsigned int rate;\nunsigned short duration;')
    result['entry_holders'] = replace(base, 'if (duration != 0)', 'if (rate) {}\n    if (duration) {}\n    if (duration != 0)')
    result['post_holders'] = replace(base, '    } else {', '        if (rate) {}\n        if (duration) {}\n    } else {')
    result['callback_unprototyped'] = replace(base, '(*D_80038018)(unsigned int, unsigned int)', '(*D_80038018)()')
    helper = 'static __inline unsigned int round_samples(unsigned int samples, unsigned int quantum)\n{ return quantum - samples % quantum + samples; }\n'
    result['unused_inline_control'] = replace(base, 'void func_80010DD8', helper + 'void func_80010DD8')
    result['inline_round'] = replace(result['unused_inline_control'], 'count += 192 - count % 192;', 'count = round_samples(count, 192);')
    helper = 'static __inline unsigned int samples_for_delay(unsigned int rate, unsigned short duration, unsigned int quantum)\n{ unsigned int count; count = duration * rate / 1000; return quantum - count % quantum + count; }\n'
    value = replace(base, 'void func_80010DD8', helper + 'void func_80010DD8')
    result['inline_all'] = replace(value, 'count = duration * rate / 1000;\n        count += 192 - count % 192;', 'count = samples_for_delay(rate, duration, 192);')
    helper = 'static __inline unsigned int round_samples(unsigned int samples)\n{ return 192 - samples % 192 + samples; }\n'
    value = replace(base, 'void func_80010DD8', helper + 'void func_80010DD8')
    result['inline_one_quantum'] = replace(value, 'count += 192 - count % 192;', 'count = round_samples(count);')
    result['baseline_reassociated'] = replace(base, 'count += 192 - count % 192;', 'count = 192 - count % 192 + count;')
    return result

def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    score.IDO = compilers.tu_cc('lib_11640', score.IDO / 'cc').parent
    require(compilers.identify_dir(score.IDO) == 'ido7.1', 'verification requirement failed')
    helper_spec = importlib.util.spec_from_file_location('static_10dd8_object_signature', HERE / 'verify.py')
    helper = importlib.util.module_from_spec(helper_spec)
    helper_spec.loader.exec_module(helper)
    rows = []
    with tempfile.TemporaryDirectory(prefix='10dd8-experiments-') as raw:
        work = Path(raw)
        for name, text in variants().items():
            source, obj = (work / (name + '.c'), work / (name + '.o'))
            source.write_text(text)
            score.compile_single(source, score.DEFAULT_FLAGS, obj)
            proof = score.compare(obj, 'func_80010DD8', show=0)
            data, sections = score._elf(obj)
            table = next((score._symbol_table(data, sections, i) for i, s in enumerate(sections) if s['type'] == 2))
            symbol = next((s for s in table if s['name'] == 'func_80010DD8'))
            rows.append({'name': name, 'source_sha256': hashlib.sha256(text.encode()).hexdigest(), 'allocated_sections_and_relocations_sha256': hashlib.sha256(json.dumps(helper.signature(obj), sort_keys=True).encode()).hexdigest(), 'function_bytes': symbol['size'], 'differing_words': proof.differing, 'target_words': proof.total, 'extra_nonzero_words': proof.extra_words, 'accepted': proof.accepted()})
    require([r['name'] for r in rows if r['accepted']] == ['baseline_reassociated'], 'verification requirement failed')
    return {'compiler': 'ido7.1', 'flags': score.DEFAULT_FLAGS, 'baseline_source': str(BASELINE.relative_to(ROOT)), 'baseline_sha256': hashlib.sha256(BASELINE.read_bytes()).hexdigest(), 'experiment_script_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(), 'rows': rows}
if __name__ == '__main__':
    result = run()
    if len(sys.argv) == 2:
        Path(sys.argv[1]).write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))
