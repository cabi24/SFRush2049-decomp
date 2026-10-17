"""Fresh stock-IDO full-group replay with source-shape and data controls."""
from dataclasses import asdict
from pathlib import Path
import argparse,contextlib,hashlib,io,json,re,shutil,struct,sys,tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT/'tools/cloud'))
import score
GROUP=HERE/'groups/menu_save_options'
OLD=ROOT/'src/blob/groups/menu_save_options'
FN='hud_setup'

def sha(data): return hashlib.sha256(data).hexdigest()
def public_metadata(value):
    if isinstance(value,str):return re.sub(r'retail [0-9a-fA-F]{8}, got [0-9a-fA-F]{8}', 'data differs (bytes omitted)',value)
    if isinstance(value,list):return [public_metadata(v) for v in value]
    if isinstance(value,dict):return {k:public_metadata(v) for k,v in value.items()}
    return value
def inspect(obj,name):
    data,secs=score._elf(obj); ti=score._text_index(secs)
    syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
    fn,=[s for s in syms if s['name']==name and s['type']==2 and s['section']==ti]
    with contextlib.redirect_stdout(io.StringIO()): c=score.compare(obj,name,show=0)
    return public_metadata(dict(asdict(c),verdict=c.summary(),symbol_bytes=fn['size'],native_bytes=4*len(score.targets()[name]),own_data_notes=list(c.notes)))
def complete(r):
    return r['verdict']=='MATCH' and r['symbol_bytes']==r['native_bytes'] and not any(r[k] for k in ('differing','extra_words','unresolved','unverified','errors'))
def source_bindings():
    return {str(p.relative_to(HERE)):sha(p.read_bytes()) for p in sorted(list(GROUP.glob('*'))+[HERE/'replay.py',HERE/'observe.py']) if p.is_file()}
def target_bindings():
    names=json.loads((GROUP/'group.json').read_text())['members']
    return {n:{'address':hex(score.image_symbols()[n]), 'bytes':4*len(score.targets()[n]), 'words_sha256':sha(struct.pack('>'+str(len(score.targets()[n]))+'I',*score.targets()[n]))} for n in names}
def verify(work):
    work=Path(work);work.mkdir(parents=True,exist_ok=True)
    spec=json.loads((GROUP/'group.json').read_text());old=json.loads((OLD/'group.json').read_text())
    assert spec['name']=='menu_save_options'
    assert spec['members']==old['members']+[FN] and spec['claims']==spec['members']
    assert spec['files']==old['files']+['hud_setup.c']
    assert spec['keep']==old['keep']+[FN] and spec['context']==old['context']
    assert all((GROUP/f).read_bytes()==(OLD/f).read_bytes() for f in old['files'])
    assert len(old['members'])==24
    source=(GROUP/'hud_setup.c').read_text()
    cast='func_800C8738((u32)D_80110264)'
    static='    static s8 D_80110274 = 0;'
    assert source.count(cast)==1 and source.count(static)==1
    variants={
        'candidate':source,
        'without_count_conversion':source.replace(cast,'func_800C8738(D_80110264)'),
        'external_once_flag':source.replace(static,'').replace('extern s8 D_8011023C;','extern s8 D_8011023C;\nextern s8 D_80110274;'),
        'uninitialized_local_flag':source.replace(static,'    static s8 D_80110274;'),
        'static_list_helper':source.replace('void sound_list_init(', 'static void sound_list_init('),
        'wrong_initial_flag':source.replace(static,'    static s8 D_80110274 = 1;'),
    }
    results={}
    for label,src in variants.items():
        d=work/label;shutil.copytree(GROUP,d,dirs_exist_ok=True);(d/'hud_setup.c').write_text(src)
        obj=work/(label+'.o');score.compile_group(d,obj)
        rows={n:inspect(obj,n) for n in spec['members']}
        results[label]={'hud_setup':rows[FN], 'preserved_members':[n for n in old['members'] if complete(rows[n])], 'failed_old_members':{n:r for n,r in rows.items() if n!=FN and not complete(r)}}
        if label=='candidate':
            assert all(complete(r) for r in rows.values())
            assert rows[FN]['symbol_bytes']==1048
            assert any('0x80110274' in n and '4 bytes, all zero' in n for n in rows[FN]['own_data_notes'])
            results[label]['all_members']=rows
            # The named inferred helper residue is not a claimed native function.
            data,secs=score._elf(obj);ti=score._text_index(secs)
            syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
            helper,=[s for s in syms if s['name']=='sound_list_init' and s['type']==2 and s['section']==ti]
            assert helper['size']==8
            results[label]['unclaimed_helper_residue_bytes']=helper['size']
        elif label=='static_list_helper':
            assert complete(rows[FN]) and set(results[label]['failed_old_members'])=={'func_800A370C'}
        else:
            assert not complete(rows[FN])
            assert len(results[label]['preserved_members'])==24
        if label=='without_count_conversion':assert rows[FN]['differing']==24
        if label=='external_once_flag':assert rows[FN]['differing']==8
        print(label+': '+rows[FN]['verdict']+'; prior members preserved '+str(len(results[label]['preserved_members']))+'/24')
    image=score.own_data();assert image.read(0x80110274,4)==bytes(4)
    return {'protocol_version':2,'status':'STRICT_CLOUD_MATCH_PENDING_UNIT_IMAGE_GATES','new_code_bytes':1048,'prior_members':24,'source_sha256':source_bindings(),'target_sha256':target_bindings(),'once_flag':{'logical_bytes':1,'verified_window_bytes':4,'address':'0x80110274','window_sha256':sha(bytes(4))},'results':results}

if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('--record',action='store_true');ap.add_argument('--work',type=Path);args=ap.parse_args()
    with tempfile.TemporaryDirectory(prefix='sound-init-') as tmp:
        result=verify(args.work or Path(tmp))
    if args.record:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    else:assert result==json.loads((HERE/'verification.json').read_text()),'receipt differs'
