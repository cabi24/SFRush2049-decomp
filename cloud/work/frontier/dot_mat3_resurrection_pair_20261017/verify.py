#!/usr/bin/env python3
"""Protocol-v2 research replay: preserved locks, exact helper, honest nonmatch."""
from pathlib import Path
from contextlib import redirect_stdout
import hashlib, io, json, shutil, struct, sys, tempfile
HERE = Path(__file__).resolve().parent

def _extent(score, obj, name):
    data, secs = score._elf(obj)
    entries = [s for i, sec in enumerate(secs) if sec['type'] == 2 for s in score._symbol_table(data, secs, i) if s['type'] == 2 and s['section'] == score._text_index(secs)]
    entry = next((s for s in entries if s['name'] == name))
    words = score.text_words(obj)[entry['value'] // 4:(entry['value'] + entry['size']) // 4]
    frame = next((65536 - (w & 65535) for w in words[:40] if w >> 16 == 10173 and w & 32768), 0)
    return {'bytes': entry['size'], 'frame': frame}

def verify(repo=None, compile=True):
    repo = Path(repo).resolve() if repo else HERE.parents[3]
    receipt = json.loads((HERE / 'receipt.json').read_text())
    if not (receipt['protocol_version'] == 2 and receipt['claims'] == []):
        raise RuntimeError('proof guard failed')
    if not hashlib.sha256(Path(__file__).read_bytes()).hexdigest() == receipt['verify_sha256']:
        raise RuntimeError('verify mutation')
    for rel, digest in receipt['source_sha256'].items():
        if not hashlib.sha256((HERE / rel).read_bytes()).hexdigest() == digest:
            raise RuntimeError(rel + ' mutation')
    sys.path.insert(0, str(repo / 'tools/cloud'))
    import score
    for name, digest in receipt['target_words_sha256'].items():
        w = score.targets()[name]
        if not hashlib.sha256(struct.pack('>' + str(len(w)) + 'I', *w)).hexdigest() == digest:
            raise RuntimeError(name + ' target changed')
    g = json.loads((HERE / 'group/group.json').read_text())
    if not g['claims'] == receipt['claims']:
        raise RuntimeError('proof guard failed')
    if not g['members'] == receipt['preserved_locked_members']:
        raise RuntimeError('proof guard failed')
    if not g['context'] == receipt['scored_context']:
        raise RuntimeError('proof guard failed')
    if not set(g['members'] + g['context']) == set(receipt['target_words_sha256']):
        raise RuntimeError('proof guard failed')
    if not compile:
        return {'input_integrity': True, 'compiler_run': False, 'claims': []}
    code = (HERE / 'group/camera_pair.c').read_text()
    variants = {'natural': code, 'inherited_six_unused_diagnostic': code.replace('    Player *player;', '    Player *player;\n    s32 xv, yv, zv, last_cp_index;\n    f32 *dp;\n    void *chkpnt;'), 'wrong_resurrection_point_stride': code.replace('typedef s16 Position[3];', 'typedef s16 Position[4];'), 'wrong_checkpoint_stride': code.replace('s16 pathPts[20];u8 tail[6];', 's16 pathPts[20];u8 tail[10];'), 'flatten_fasin_wrapper': code.replace('angle = camera_update_c(pt[0]);', 'angle = func_8009C3F8(pt[0],0);'), 'sequential_fallback_stores': code.replace('        matrix[5]=-sine;\n', '').replace('        matrix[4]=cosine;', '        matrix[4]=cosine;\n        matrix[5]=-sine;'), 'wrong_fasin_axis': code.replace('angle = camera_update_c(pt[0]);', 'angle = camera_update_c(pt[1]);')}
    if not list(variants) == receipt['variants']:
        raise RuntimeError('proof guard failed')
    results = {}
    with tempfile.TemporaryDirectory(prefix='resurrection-pair-') as tmp:
        for tag, source in variants.items():
            if not (tag == 'natural' or source != code):
                raise RuntimeError('unchanged control ' + tag)
            p = Path(tmp) / tag
            shutil.copytree(HERE / 'group', p)
            (p / 'camera_pair.c').write_text(source)
            obj = p / 'result.o'
            score.compile_group(p, obj)
            rows = {}
            for name in g['members'] + g['context']:
                with redirect_stdout(io.StringIO()):
                    c = score.compare(obj, name, show=0)
                if name in g['members']:
                    if not c.accepted():
                        raise RuntimeError(tag + ' ' + name + ': ' + c.summary())
                if name == 'camera_follow_path':
                    if not c.accepted() == (tag in ['natural', 'inherited_six_unused_diagnostic']):
                        raise RuntimeError(tag + ': ' + c.summary())
                if name == 'func_800C4F68' and tag in ['natural', 'inherited_six_unused_diagnostic']:
                    if not c.differing == (17 if tag == 'natural' else 0):
                        raise RuntimeError('proof guard failed')
                    if not not any([c.extra_words, c.errors, c.unresolved, c.unverified]):
                        raise RuntimeError('proof guard failed')
                rows[name] = c.summary()
            extents = {name: _extent(score, obj, name) for name in ['camera_follow_path', 'func_800C4F68']}
            if tag in ['natural', 'inherited_six_unused_diagnostic']:
                if not extents['camera_follow_path'] == {'bytes': 1484, 'frame': 104}:
                    raise RuntimeError('proof guard failed')
                if not extents['func_800C4F68'] == {'bytes': 1416, 'frame': 80 if tag == 'natural' else 104}:
                    raise RuntimeError('proof guard failed')
            results[tag] = {'scores': rows, 'extents': extents}
    return {'input_integrity': True, 'compiler_run': True, 'claims': [], 'results': results}
if __name__ == '__main__':
    print(json.dumps(verify(sys.argv[1] if len(sys.argv) > 1 else None), indent=2))
