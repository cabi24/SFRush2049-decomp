#!/usr/bin/env python3
"""Protocol-v2 exact group replay and bounded compiler/geometry controls."""
import argparse
import contextlib
import hashlib
import io
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[5]
sys.path.insert(0,str(ROOT))
from tools.cloud import score, owndata

NAME='camera_play_script'
OLD=['func_800AD650','handbrake_apply','func_800C36A0','camera_trigger_check','camera_victory','func_800C3AD0']
COPIES=['effect_cleanup','func_800C54F0','func_800A61B0','math_utility','func_800B61A8','func_8009E820']
STRICT=OLD+[NAME]+['func_800AD4C8','input_process_controller','func_800AD5D0']+COPIES
SOURCE='camera_play_script.c'

def result(obj,name):
    with contextlib.redirect_stdout(io.StringIO()): r=score.compare(obj,name,show=0)
    return dict(differing_words=r.differing,target_words=r.total,extra_nonzero_words=r.extra_words,
                unresolved=r.unresolved,unverified=r.unverified,errors=r.errors,strict_match=r.accepted())

def group_result(obj): return {n:result(obj,n) for n in STRICT+['func_800AC9BC']}

def replace_once(s,a,b):
    if s.count(a)!=1: raise RuntimeError('ambiguous control anchor: '+a)
    return s.replace(a,b,1)

def natural(s):
    if s.count('ex * 1.0f * (hz - pz)')!=2: raise RuntimeError('identity anchors changed')
    return s.replace('ex * 1.0f * (hz - pz)','ex * (hz - pz)')

def object_layout(obj):
    elf=owndata._Object(obj); funcs=sorted(n for n,_,_ in elf.functions())
    words=score.text_words(obj);start=score.symbols(obj)[NAME]//4
    frames=[65536-(w&65535) for w in words[start:start+100] if w>>16==0x27bd and w&0x8000]
    return dict(emitted_functions=funcs,root_frame_bytes=frames[0] if frames else None,
                deleted_helper_has_symbol='lsq' in funcs)

def inputs():
    spec=json.loads((HERE/'group.json').read_text());targets=score.targets();symbols=score.image_symbols()
    source='\n'.join((HERE/n).read_text() for n in spec['files'])
    tokens=set(re.findall(r'\b[A-Za-z_]\w*\b',re.sub(r'/\*.*?\*/|//[^\n]*','',source,flags=re.S)))
    refs={n:symbols.get(n,score.address_named(n)) for n in sorted(tokens) if n in symbols or score.address_named(n) is not None}
    return dict(protocol_version=2,population='game_blob',compiler='ido5.3',recipe=spec,
        files={n:hashlib.sha256((HERE/n).read_bytes()).hexdigest() for n in spec['files']+['group.json','replay.py','semantics.c']},
        targets={n:dict(address=hex(symbols[n]),size=len(targets[n])*4,words_sha256=hashlib.sha256(b''.join(struct.pack('>I',w) for w in targets[n])).hexdigest()) for n in STRICT+['func_800AC9BC']},references=refs)

def preserved_sources():
    base=ROOT/'src/blob/groups/func_800AD4C8'
    for n in ['camera_victory.c','camera_trigger_check.c']:
        if (HERE/n).read_bytes()!=(base/n).read_bytes(): raise RuntimeError('preserved source changed: '+n)
    if (HERE/'group.c').read_text()!=(base/'group.c').read_text().replace('static f32 lsq(', 'f32 lsq('):
        raise RuntimeError('unexpected old-group edit')
    for n in COPIES:
        if (HERE/(n+'.c')).read_bytes()!=(ROOT/'src/blob'/(n+'.c')).read_bytes(): raise RuntimeError('callee copy changed: '+n)

def semantic_check(directory):
    cc=shutil.which('cc')
    if cc is None: raise RuntimeError('host compiler unavailable')
    s=natural((HERE/SOURCE).read_text());body=s[s.index('void camera_play_script(CCar *car, Poly *poly, CCol *col) {'):]
    (directory/'natural_body.inc').write_text(body.replace('camera_play_script','natural_camera_play_script'))
    for n in [SOURCE,'semantics.c']: shutil.copyfile(HERE/n,directory/n)
    exe=directory/'semantics'
    subprocess.run([cc,'-std=c11','-O2','-fno-strict-aliasing','-fsanitize=undefined','-Wno-int-to-pointer-cast','-Wno-unknown-pragmas',str(directory/'semantics.c'),'-lm','-o',str(exe)],check=True,capture_output=True,text=True)
    return subprocess.run([str(exe)],check=True,capture_output=True,text=True).stdout.strip()

def verify():
    preserved_sources();spec=json.loads((HERE/'group.json').read_text())
    if spec['members']!=OLD+[NAME] or spec['claims']!=OLD+[NAME]: raise RuntimeError('complete member claims changed')
    source=(HERE/SOURCE).read_text();oldgroup=(HERE/'group.c').read_text()
    separate=source
    for m in list(re.finditer(r'    for \(ka = 0; ka < 4; ka\+\+\) \{.*?\n    \}',source,re.S))[::-1]:
        separate=separate[:m.start()]+re.sub(r'\bka\b','k',m.group())+separate[m.end():]
    if separate==source: raise RuntimeError('corner loop anchors changed')
    controls={
        'natural_without_identity':natural(source),
        'separate_corner_index':separate,
        'donor_dy_first':replace_once(source,'            dx = pts[kb][0] - pts[ka][0];\n            dz = pts[kb][2] - pts[ka][2];\n            dy = db - da;','            dy = db - da;\n            dx = pts[kb][0] - pts[ka][0];\n            dz = pts[kb][2] - pts[ka][2];'),
        'wrong_surface_depth':replace_once(source,'dz = -3.0f;','dz = -3.5f;'),
    }
    report=dict(inputs=inputs(),positive={},negative_controls={},preserved_members=OLD,
        new_individually_strict_bytes=3520,accepted_image_bytes=0,whole_unit_gate='not run',image_gate='not run',rom_gate='not run',
        fp_scope='Exact stock-IDO target output; bounded finite host geometry only, not a portable IEEE-754 identity claim.')
    with tempfile.TemporaryDirectory(prefix='rush-carbody-proof-') as t:
        d=Path(t)
        for n in spec['files']+['group.json']: shutil.copyfile(HERE/n,d/n)
        for label in ['first','repeat']:
            obj=d/(label+'.o');score.compile_group(d,obj);report['positive'][label]=group_result(obj)
            if label=='first': report['object_layout']=object_layout(obj)
        for label,s in controls.items():
            (d/SOURCE).write_text(s);obj=d/(label+'.o');score.compile_group(d,obj)
            report['negative_controls'][label]=group_result(obj)
        (d/SOURCE).write_text(source);(d/'group.c').write_text(oldgroup.replace('f32 lsq(', 'static f32 lsq(',1))
        obj=d/'static_helper.o';score.compile_group(d,obj);report['negative_controls']['unbounded_static_residue']=group_result(obj)
        report['host_semantics']=semantic_check(d)
    if any(not group[n]['strict_match'] for group in report['positive'].values() for n in STRICT): raise RuntimeError('strict closure failed')
    if report['object_layout']['root_frame_bytes']!=608 or not report['object_layout']['deleted_helper_has_symbol']: raise RuntimeError('frame/helper boundary changed')
    for label in controls:
        if report['negative_controls'][label][NAME]['strict_match']: raise RuntimeError('control incorrectly accepted: '+label)
    residue=report['negative_controls']['unbounded_static_residue']
    if residue['effect_cleanup']['extra_nonzero_words']!=1: raise RuntimeError('residue control changed')
    if report['negative_controls']['natural_without_identity'][NAME]['differing_words']!=1: raise RuntimeError('natural residual changed')
    return report

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--check',action='store_true');args=ap.parse_args()
    if args.check:
        saved=json.loads((HERE/'proof.json').read_text())
        if inputs()!=saved['inputs']: raise RuntimeError('receipt inputs changed')
    report=verify()
    if args.check and report!=saved: raise RuntimeError('fresh receipt differs')
    print(json.dumps(report,indent=2,sort_keys=True))
if __name__=='__main__':main()
