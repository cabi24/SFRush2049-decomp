#!/usr/bin/env python3
"""Reproduce a bounded, image-qualified available-list match and its controls."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import sys
import tempfile
import time
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
BASELINE = Path('cloud/work/lean/runtime_a_car_select_20261006')
NAME = 'func_8039E3BC'
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'

def require(condition, message):
    if not condition:
        raise ValueError(message)

def sha(data):
    return hashlib.sha256(data).hexdigest()

def expected_source(source):
    return (source.replace('func_8039D494(s32 item, s32 player)',
                           'func_8039D494(s32 player, s32 item)')
            .replace('func_8039D494(item,player)', 'func_8039D494(player,item)')
            .replace('D_803BA050[player]=0;\n    for(item=0;',
                     'D_803BA050[player]=0; for(item=0;'))

def verify(repo=ROOT):
    start = time.monotonic()
    sys.path.insert(0, str(repo / 'tools/cloud'))
    # Isolate per-image module state from callers running other image tests.
    spec = importlib.util.spec_from_file_location('available_list_wave_score', repo/'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = score
    spec.loader.exec_module(score)
    import owndata
    score.ASM_DIR = repo / 'asm/us/ovl_a'
    manifest = score.target_manifest()
    extents = json.loads(score.verified_bytes(score.ASM_DIR/'extents.json', manifest))
    require(extents['image'] == 'A' and extents['rom_offset'] == '0xB5C534', 'wrong image')
    targets = score.targets()
    target = next(f for f in extents['functions'] if f['name'] == NAME)
    require(target['address'] == '0x8039E3BC' and target['size'] == 136, 'wrong extent')
    require(sha(b''.join(w.to_bytes(4,'big') for w in targets[NAME])) ==
            'c56db54a7f5e9b363c23161f3f2049a5593a0343d19b6edaf1926e2e6abebb85', 'target drift')
    original = (repo / BASELINE/'group.c').read_text()
    source = (HERE/'group.c').read_text()
    require(source == expected_source(original), 'unexpected source change')
    original_config = json.loads((repo/BASELINE/'group.json').read_text())
    config = json.loads((HERE/'group.json').read_text())
    expected_config = dict(original_config, claims=[NAME],
        members=['func_8039D494', NAME, 'func_8039ED20'],
        context=[n for n in original_config['members'] if n not in ('func_8039D494', NAME, 'func_8039ED20')])
    require(config == expected_config, 'unexpected group recipe')
    asset = (repo/'assets/us/data.bin').read_bytes()
    require(sha(asset) == 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8', 'asset drift')
    require(int.from_bytes(asset[0x3850:0x3854], 'big') == 0xB5C534, 'loader pointer drift')
    image = zlib.decompress(asset[0xB5C534-0x283D0:], -15)
    require(len(image) == 194128 and sha(image) == IMAGE_HASH, 'image drift')
    score._own_data[owndata.artifact_dir(score.ASM_DIR)] = owndata.ImageData.from_image(image,0x8038A400)
    with tempfile.TemporaryDirectory() as temp:
        build = Path(temp)
        obj = build/'candidate.o'
        score.compile_group(HERE,obj)
        scores = {fn:dataclasses.asdict(score.compare(obj,fn,show=0)) for fn in config['members'] + config.get('context', [])}
        # Keep counts and diagnostics without publishing extracted target bytes.
        for result in scores.values():
            result['errors'] = ['owned-data comparison mismatch' if 'retail' in e else e for e in result['errors']]
        for fn in (NAME,'func_8039D494','func_8039ED20'):
            result=score.compare(obj,fn,show=0)
            require(result.accepted(), fn+' no longer exact: '+result.summary())
        data,sections=score._elf(obj)
        text_index=score._text_index(sections)
        symtab=next(i for i,s in enumerate(sections) if s['type']==2)
        symbols=score._symbol_table(data,sections,symtab)
        symbol=next(s for s in symbols if s['name']==NAME and s['section']==text_index)
        require(symbol['size']==136,'full ELF function extent differs')
        words=score.text_words(obj);off=symbol['value']
        resolved,masks,unresolved,unverified,errors=score.relocate(obj,words,off,off+136,score.image_symbols())
        require(not any((masks,unresolved,unverified,errors)), 'relocation uncertainty')
        require(resolved[off//4:(off+136)//4] == targets[NAME], 'full resolved body differs')
        controls={}
        variants={
            'original_source': original,
            'reordered_formals_only': source.replace('D_803BA050[player]=0; for(item=0;', 'D_803BA050[player]=0;\n    for(item=0;'),
            'wrong_loop_bound': source.replace('for(item=0;item<19;item++)', 'for(item=0;item<18;item++)'),
        }
        for label,variant in variants.items():
            directory=build/label;directory.mkdir()
            (directory/'group.c').write_text(variant)
            (directory/'group.json').write_text(json.dumps(config))
            other=directory/'candidate.o';score.compile_group(directory,other)
            result=score.compare(other,NAME,show=0)
            require(not result.accepted(),label+' was not rejected')
            controls[label]=dataclasses.asdict(result)
        require(controls['original_source']['differing']==5, 'baseline changed')
        require(controls['reordered_formals_only']['differing']==2, 'line control changed')
    return {'image':'A','baseline_commit':'390d40f6f271eecc83a5e82ba7b12f6e8a1c8625',
            'claim':NAME,'new_candidate_bytes':136,'accepted_bytes':0,'elf_extent_bytes':136,
            'source_sha256':sha(source.encode()),'group_sha256':sha((HERE/'group.json').read_bytes()),
            'image_sha256':IMAGE_HASH,'flags':config['flags']+' -Wab,-r4300_mul',
            'scores':scores,'negative_controls':controls,'elapsed_seconds':round(time.monotonic()-start,3)}

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path)
    args=parser.parse_args()
    result=verify()
    output=json.dumps(result,indent=2)+'\n'
    if args.output:args.output.write_text(output)
    print(output)
