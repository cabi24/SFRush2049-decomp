#!/usr/bin/env python3
"""Protocol-v2 bounded helper-boundary proof; LoadTrack remains unmatched."""
from pathlib import Path
from contextlib import redirect_stdout
import dataclasses, hashlib, io, json, shutil, struct, sys, tempfile
HERE = Path(__file__).resolve().parent

def digest(b):
    return hashlib.sha256(b).hexdigest()

def body(score, obj, name):
    data, secs = score._elf(obj)
    s = next((s for i, sec in enumerate(secs) if sec['type'] == 2 for s in score._symbol_table(data, secs, i) if s['type'] == 2 and s['section'] == score._text_index(secs) and (s['name'] == name)))
    start = s['value']
    end = start + s['size']
    words, _, _, _, _ = score.relocate(obj, score.text_words(obj), start, end, score.image_symbols())
    words = words[start // 4:end // 4]
    return {'bytes': len(words) * 4, 'frame': next((65536 - (w & 65535) for w in words[:40] if w >> 16 == 10173)), 'body_sha256': digest(struct.pack('>' + str(len(words)) + 'I', *words))}

def verify(repo=None, compile=True):
    repo = Path(repo).resolve() if repo else HERE.parents[3]
    r = json.loads((HERE / 'receipt.json').read_text())
    if not (r['protocol_version'] == 2 and r['claims'] == []):
        raise RuntimeError('proof guard failed')
    if not digest(Path(__file__).read_bytes()) == r['verify_sha256']:
        raise RuntimeError('verify mutation')
    for rel, h in r['source_sha256'].items():
        if not digest((HERE / rel).read_bytes()) == h:
            raise RuntimeError(rel + ' mutation')
    sys.path.insert(0, str(repo / 'tools/cloud'))
    import score
    for name, h in r['target_words_sha256'].items():
        ws = score.targets()[name]
        if not digest(struct.pack('>' + str(len(ws)) + 'I', *ws)) == h:
            raise RuntimeError(name + ' target mutation')
    g = json.loads((HERE / 'group/group.json').read_text())
    if not (g['claims'] == [] and g['members'] == r['preserved_locked_members'] and (g['context'] == ['LoadTrack'])):
        raise RuntimeError('proof guard failed')
    if not set(g['members'] + g['context']) == set(r['target_words_sha256']):
        raise RuntimeError('proof guard failed')
    if not compile:
        return {'input_integrity': True, 'compiler_run': False, 'claims': []}
    source = (HERE / 'group/LoadTrack.c').read_text()
    helper = (HERE / 'group/mode_flags_clear.c').read_text()
    variants = {'native': (source, helper), 'repeat': (source, helper), 'historical': (None, None), 'flatten': (source.replace('    mode_flags_clear();', '    D_80110680[0] = 0;\n    D_80110680[1] = 0;\n    audio_dsp_process();'), helper), 'wrong_first_flag': (source, helper.replace('D_80110680.first=0', 'D_80110680.second=0')), 'wrong_checkpoint_callee': (source, helper.replace('audio_dsp_process();', 'audio_effect_remove();').replace('void audio_dsp_process(void);', 'void audio_effect_remove(void);'))}
    if not list(variants) == list(r['expectations']):
        raise RuntimeError('proof guard failed')
    results = {}
    with tempfile.TemporaryDirectory(prefix='loadtrack-helper-') as tmp:
        for tag, (c, h) in variants.items():
            d = Path(tmp) / tag
            shutil.copytree(HERE / 'group', d)
            if tag == 'historical':
                shutil.copy(HERE / 'historical_LoadTrack.c', d / 'LoadTrack.c')
                shutil.copy(HERE / 'historical_group.json', d / 'group.json')
            else:
                (d / 'LoadTrack.c').write_text(c)
                (d / 'mode_flags_clear.c').write_text(h)
                if tag not in ['native', 'repeat']:
                    if not (c != source or h != helper):
                        raise RuntimeError('proof guard failed')
            obj = d / 'result.o'
            score.compile_group(d, obj)
            spec = json.loads((d / 'group.json').read_text())
            rows = {}
            for name in spec['members'] + spec['context']:
                with redirect_stdout(io.StringIO()):
                    v = score.compare(obj, name, show=0)
                if not (not v.unresolved and (not v.unverified) and (not v.errors) and (not v.extra_words)):
                    raise RuntimeError('proof guard failed')
                if name not in ['LoadTrack', 'mode_flags_clear']:
                    if not v.accepted():
                        raise RuntimeError(tag + ': ' + name)
                rows[name] = dataclasses.asdict(v)
            e = r['expectations'][tag]
            if not rows['LoadTrack']['differing'] == e['LoadTrack_differing']:
                raise RuntimeError('proof guard failed')
            if not rows['LoadTrack']['differing'] > 0:
                raise RuntimeError('proof guard failed')
            if tag != 'historical':
                if not rows['mode_flags_clear']['differing'] == e['helper_differing']:
                    raise RuntimeError('proof guard failed')
            extent = body(score, obj, 'LoadTrack')
            if not extent == e['body']:
                raise RuntimeError('proof guard failed')
            results[tag] = {'scores': rows, 'body': extent}
    if not results['native']['body'] == results['repeat']['body'] == results['historical']['body']:
        raise RuntimeError('proof guard failed')
    return {'input_integrity': True, 'compiler_run': True, 'claims': [], 'preserved_locked_matches': len(g['members']), 'resolved_body_identical': True, 'results': results}
if __name__ == '__main__':
    print(json.dumps(verify(sys.argv[1] if len(sys.argv) > 1 else None), indent=2))
