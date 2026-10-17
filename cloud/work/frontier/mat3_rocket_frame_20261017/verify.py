"""Strict research replay: stack-only Rocket residual and semantic negatives.

This proves a measured partial, never an exact-match or coverage claim.
"""
import argparse
import contextlib
import hashlib
import io
import json
import shutil
import struct
import sys
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

ROCKET = 'entity_tick_main'
FRAME = 'drone_ai_update'
PRESERVED_EXACT = ['entity_anim_texture', 'anim_state_update', 'buffer_swap']


def require(condition, message):
    if not condition:
        raise ValueError(message)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def inspect(obj, names):
    results = {}
    for name in names:
        with contextlib.redirect_stdout(io.StringIO()):
            row = score.compare(obj, name)
        results[name] = vars(row).copy()
    return results


def clean(row):
    require(not any(row[k] for k in ('extra_words', 'unverified', 'unresolved', 'errors')), row)


def residuals(obj, name):
    """Uses the scorer's masks only after its independent own-data proof passed."""
    want = score.targets()[name]
    words = score.text_words(obj)
    functions = score.symbols(obj)
    start = functions[name]
    end = min((v for v in functions.values() if v > start), default=len(words)*4)
    end = min(end, start+4*len(want))
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, end, score.image_symbols())
    require(not unresolved and not errors, 'verification check failed at 53')
    got = resolved[start//4:end//4]
    require(len(got) == len(want), 'verification check failed at 55')
    return [(i, w, g) for i, (w, g) in enumerate(zip(want, got))
            if (w & masks.get(start+4*i, 0xffffffff)) !=
               (g & masks.get(start+4*i, 0xffffffff))]


def is_stack_immediate(w, g):
    return ((w >> 16) == (g >> 16) and ((w >> 21) & 31) == 29
            and (w >> 26) in (9, 35, 40, 43, 49, 57))


def signed16(x):
    x &= 65535
    return x-65536 if x >= 32768 else x


def verify(out, frozen=True):
    out = Path(out)
    out.mkdir(parents=True, exist_ok=True)
    group = HERE/'group'
    spec = json.loads((group/'group.json').read_text())
    require(spec['claims'] == [], 'verification check failed at 76')
    require(spec['members'] == [ROCKET, FRAME], 'verification check failed at 77')
    inputs = {p: digest(HERE/p) for p in ['verify.py', 'trace_fidelity.py', 'test_palette_semantics.py',
        'group/group.json'] + ['group/'+f for f in spec['files']]}
    targets = {name: hashlib.sha256(b''.join(struct.pack('>I', w)
        for w in score.targets()[name])).hexdigest() for name in spec['members']}
    if frozen:
        old = json.loads((HERE/'verification.json').read_text())
        require(inputs == old['inputs_sha256'], 'source changed; reverify receipt')
        require(targets == old['target_words_sha256'], 'native target changed')
    positive_obj = out/'positive.o'
    score.compile_group(group, positive_obj)
    positive = inspect(positive_obj, spec['members']+spec['context'])
    for name, expected in [(ROCKET, 47), (FRAME, 537)]:
        clean(positive[name])
        require(positive[name]['differing'] == expected, positive[name])
        require(not score.Comparison(**{k: positive[name][k] for k in
            ['differing','total','unresolved','unverified','errors','extra_words']}).accepted(), 'verification check failed at 92')
    for name in PRESERVED_EXACT:
        clean(positive[name]); require(positive[name]['differing'] == 0, 'verification check failed at 95')
    differences = residuals(positive_obj, ROCKET)
    require(len(differences) == 47, 'verification check failed at 97')
    require(all(is_stack_immediate(w, g) for _, w, g in differences), 'verification check failed at 98')
    histogram = Counter(signed16(w)-signed16(g) for _, w, g in differences)
    negative = out/'negative'
    if negative.exists():
        shutil.rmtree(negative)
    shutil.copytree(group, negative)
    source = negative/'rocket.c'
    body = source.read_text()
    old = 'if(D_8014A110==2 && slot>0) clr.b[3]=128;'
    require(body.count(old) == 1, 'verification check failed at 107')
    source.write_text(body.replace(old, old.replace('=128;', '=129;')))
    negative_obj = out/'negative.o'
    score.compile_group(negative, negative_obj)
    negative_rows = inspect(negative_obj, [ROCKET, FRAME]+PRESERVED_EXACT)
    clean(negative_rows[ROCKET]); require(negative_rows[ROCKET]['differing'] == 48, 'verification check failed at 112')
    require(negative_rows[FRAME] == positive[FRAME], 'verification check failed at 113')
    nbad = residuals(negative_obj, ROCKET)
    require(sum(not is_stack_immediate(w,g) for _,w,g in nbad) == 1, 'verification check failed at 115')
    for name in PRESERVED_EXACT:
        require(negative_rows[name] == positive[name], 'verification check failed at 117')
    return {'status': 'VERIFIED_RESEARCH_PARTIAL', 'base_commit':
        '58425072b49e95f0fbe5c4c239988fb4885f7c5c', 'claims': [],
        'new_matching_bytes': 0, 'runtime_or_ROM_verification': False,
        'inputs_sha256': inputs, 'target_words_sha256': targets,
        'positive_object_sha256': digest(positive_obj),
        'scorer_sha256': digest(ROOT/'tools/cloud/score.py'),
        'positive': positive, 'negative_control': negative_rows,
        'rocket_residual': {'count': len(differences),
           'kind': 'stack/frame immediates only', 'native_frame_bytes': 368,
           'candidate_frame_bytes': 352, 'immediate_delta_counts': dict(histogram)},
        'negative_mutation': 'Rocket translucent alpha 128 -> 129'}


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--record', action='store_true')
    ap.add_argument('--out', type=Path, default=ROOT/'build/mat3_rocket_frame_20261017')
    args = ap.parse_args()
    receipt = verify(args.out, frozen=not args.record)
    if args.record:
        (HERE/'verification.json').write_text(json.dumps(receipt, indent=2)+'\n')
    print('PASS research: Rocket 47 stack-only words; wrong alpha 48; Frame 537; no claims')


if __name__ == '__main__':
    main()
