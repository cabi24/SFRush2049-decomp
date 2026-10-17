#!/usr/bin/env python3
"""Supporting MBOX source-cleanup proof; the audio baseline remains unchanged."""
from pathlib import Path
import argparse, dataclasses, hashlib, importlib.util, json, os, shutil, struct, sys, tempfile
HERE=Path(__file__).resolve().parent
ROOT=Path(os.environ['N64_RESEARCH_REPO']).resolve() if 'N64_RESEARCH_REPO' in os.environ else next((p for p in HERE.parents if (p/'tools/cloud/score.py').is_file()),Path.cwd())
sys.path.insert(0,str(ROOT))
from tools.cloud import score
NAMES=['model_transform_setup','model_data_load','func_8008D870']
STUBS=['MBOX_GetChild','MBOX_GetNext','ZOID_ObjectFlags','ZOID_SetObjectFlags']
def require(ok,message):
    if not ok: raise RuntimeError(message)
def digest(path): return hashlib.sha256(path.read_bytes()).hexdigest()
def target_hash(name):
    w=score.targets()[name]
    return hashlib.sha256(struct.pack('>%dI'%len(w),*w)).hexdigest()
def inputs(directory=HERE):
    pin=json.loads((directory/'inputs.json').read_text())
    require(pin['claims']==[] and pin['new_accepted_bytes']==0,'False supporting credit')
    spec=json.loads((directory/'group/group.json').read_text())
    require(spec['members']==NAMES and spec['context']==[] and spec['claims']==[], 'Changed complete native boundary')
    require(spec['keep']==NAMES and spec['files']==['helpers.c'],'Changed helper scope')
    for name,h in pin['source_sha256'].items():require(digest(directory/name)==h,'Changed supporting input: '+name)
    for name,h in pin['target_sha256'].items():require(target_hash(name)==h,'Changed target: '+name)
    require(set(pin['target_sha256'])==set(NAMES),'Incomplete targets')
    for name,h in pin['baseline_sha256'].items():require(digest(ROOT/name)==h,'Changed baseline: '+name)
    return pin,spec
def row(obj,name):
    r=score.compare(obj,name,show=0)
    d=dataclasses.asdict(r)
    # These bodies have no owned literals; retain any diagnostic without bytes.
    d['errors']=[x.split(' expected ')[0].split(' (+0x')[0] for x in d['errors']]
    d['accepted']=r.accepted()
    return d
def inventory(obj):
    raw,secs=score._elf(obj)
    syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(raw,secs,i)]
    result={s['name']:s['size'] for s in syms if s['name'] in score.symbols(obj)}
    require(set(result)==set(NAMES+STUBS),'Unexpected emitted body')
    for n in NAMES:require(result[n]==len(score.targets()[n])*4,'Changed native extent: '+n)
    for n in STUBS:require(result[n]==8,'Changed inline stub: '+n)
    return result
def measure(group,obj,names):
    score.compile_group(group,obj)
    return {n:row(obj,n) for n in names}
def verify(directory=HERE,out=None):
    directory=Path(directory);pin,spec=inputs(directory)
    # This supporting proof depends on the unchanged, separately pinned packet.
    loader=importlib.util.spec_from_file_location('audio_parent',HERE.parent/'verify.py')
    parent=importlib.util.module_from_spec(loader);loader.loader.exec_module(parent)
    parent_spec=parent.verify_inputs(HERE.parent)
    root=Path(out) if out else Path(tempfile.mkdtemp(prefix='mbox-support-'))
    root.mkdir(parents=True,exist_ok=True)
    text=(directory/'group/helpers.c').read_text();results={}
    before={}
    lock=json.loads((ROOT/'blob_matched.lock.json').read_text())
    for n in NAMES:
        obj=root/(n+'-before.o');score.compile_single(ROOT/lock[n]['source'],lock[n]['flagset'],obj)
        before[n]=row(obj,n);require(before[n]['accepted'],'Locked before-body changed: '+n)
    boundary=text.index('\nint MBOX_GetChild(int dlidx)');setter=text.index('\ntypedef struct TexDef TexDef;');header=text.index('void model_data_load(int dlidx,')
    variants={'natural':text,'definitions_before':text[:header]+text[boundary:setter]+'\n'+text[header:boundary]+text[setter:],
      'wrong_show_mask':text.replace('~((camlink << 8) | 0x80000000)','~((camlink << 7) | 0x80000000)'),
      'wrong_hide_bit':text.replace('flags | 0x80000000','flags | 0x40000000'),
      'wrong_child_field':text.replace('return D_8012E700[dlidx].child;','return D_8012E700[dlidx].next;'),
      'wide_flag_reader':text.replace('ZOID_ObjectFlags(s16)','ZOID_ObjectFlags(int)').replace('ZOID_ObjectFlags(signed short objnum)','ZOID_ObjectFlags(int objnum)'),
      'wrong_texture_slot':text.replace('obj->texture[3]=tex','obj->texture[2]=tex')}
    expected={'wrong_show_mask':[3,0,0],'wrong_hide_bit':[0,1,0],'wrong_child_field':[7,7,0],'wide_flag_reader':[95,89,0],'wrong_texture_slot':[0,0,14]}
    for label,source in variants.items():
        require(label=='natural' or source!=text,'Missing control anchor: '+label)
        group=root/label;shutil.copytree(directory/'group',group,dirs_exist_ok=True);(group/'helpers.c').write_text(source)
        obj=root/(label+'.o');r=measure(group,obj,NAMES)
        if label in ('natural','definitions_before'):
            require(all(x['accepted'] for x in r.values()),'Genuine helper no longer exact')
            emitted=inventory(obj)
        else:
            require([r[n]['differing'] for n in NAMES]==expected[label],'Negative result changed: '+label)
            require(any(not x['accepted'] for x in r.values()),'Negative accepted: '+label)
        results[label]={'functions':r,'object_sha256':digest(obj)}
        if label in ('natural','definitions_before'):results[label]['emitted_functions']=emitted
    trace=json.loads((directory/'fidelity.json').read_text())
    require(trace['whole_object_identical'] and trace['stock_object_sha256']==trace['traced_object_sha256']==results['natural']['object_sha256'],'Trace fingerprint changed')
    # Preserve all 48 existing outcomes, including all six failures, before/after
    # exposing the authentic three helpers to their actual audio callers.
    before_obj=root/'audio-before.o';audio_before=parent.measure(HERE.parent/'group',before_obj,parent_spec['members'])
    group=root/'audio-after';shutil.copytree(HERE.parent/'group',group,dirs_exist_ok=True)
    spec=json.loads((group/'group.json').read_text());spec['members']+=NAMES;spec['keep']+=NAMES;spec['files']+=['mbox_helpers.c']
    (group/'mbox_helpers.c').write_text(text);(group/'group.json').write_text(json.dumps(spec,indent=2)+'\n')
    r=parent.measure(group,root/'audio-after.o',spec['members']);parent.emitted_inventory(root/'audio-after.o',spec['members'])
    require({n:r[n] for n in parent_spec['members']}==audio_before,'Inherited audio outcome changed')
    require(all(r[n]['accepted'] for n in NAMES),'Helper failed in genuine caller context')
    return {'claims':[],'new_accepted_bytes':0,'before_locked_sources':before,'helper_controls':results,'audio_before':audio_before,'audio_after':r,'baseline_48_unchanged':True,'canonical_installation_or_ROM_verification':False}
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--out',type=Path);a=p.parse_args()
    print(json.dumps(verify(out=a.out),indent=2,sort_keys=True))
