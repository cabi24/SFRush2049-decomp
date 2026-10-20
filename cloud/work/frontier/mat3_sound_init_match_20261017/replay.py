"""Fresh stock-IDO full-group replay with source-shape and data controls."""
from dataclasses import asdict
from pathlib import Path
import argparse,contextlib,hashlib,io,json,re,shutil,struct,sys,tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT/'tools/cloud'))
import score
GROUP=HERE/'groups/menu_save_options'
# The 24-member group this packet extends; installed by w34i, so its pre-install copy (handoff §5).
OLD=ROOT/'cloud/work/frontier/superseded/menu_save_options'
FN='hud_setup'

def require(condition,message):
    if not condition:raise RuntimeError(message)

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
    return r['verdict']=='MATCH' and r['total']*4==r['symbol_bytes']==r['native_bytes'] and not any(r[k] for k in ('differing','extra_words','unresolved','unverified','errors'))
def source_bindings():
    return {str(p.relative_to(HERE)):sha(p.read_bytes()) for p in sorted(list(GROUP.glob('*'))+[HERE/'replay.py',HERE/'observe.py',HERE/'README.md']) if p.is_file()}
def compiler_bindings():
    return {'compiler_executables':{n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','ujoin','uld','usplit','umerge','uopt','ugen','as1']},
            # w34i: blob_matched.lock.json and SHA256SUMS change with every splice (handoff §5): not bound.
            'verification_tools':{n:sha((ROOT/n).read_bytes()) for n in ['tools/cloud/score.py','tools/cloud/owndata.py']},
            'target_metadata':{n:sha((score.ASM_DIR/n).read_bytes()) for n in ['symbols.json']}}

def target_bindings():
    names=json.loads((GROUP/'group.json').read_text())['members']
    return {n:{'address':hex(score.image_symbols()[n]), 'bytes':4*len(score.targets()[n]), 'words_sha256':sha(struct.pack('>'+str(len(score.targets()[n]))+'I',*score.targets()[n]))} for n in names}
def verify_report(receipt):
    spec=json.loads((GROUP/'group.json').read_text());old=json.loads((OLD/'group.json').read_text());members=spec['members']
    require(receipt['protocol_version']==3 and receipt['status']=='STRICT_CLOUD_MATCH_PENDING_UNIT_IMAGE_GATES' and receipt['new_code_bytes']==1048 and receipt['prior_members']==24 and receipt['accepted_image_bytes']==0,'false credit or protocol')
    require(receipt['claims']==members and members==old['members']+[FN],'claim inventory changed')
    targets=target_bindings();results=receipt['results']
    require(set(results)=={'candidate','without_count_conversion','external_once_flag','uninitialized_local_flag','static_list_helper','wrong_initial_flag'},'control inventory changed')
    for label,result in results.items():
        require(set(result['all_members'])==set(members) and set(result['context'])==set(spec['context']),'complete inventory changed')
        require(result['hud_setup']==result['all_members'][FN],'root duplicate changed')
        expected_failures=set() if label=='candidate' else ({'func_800A370C'} if label=='static_list_helper' else set())
        for name,row in result['all_members'].items():
            require(row['native_bytes']==targets[name]['bytes'] and row['total']*4==targets[name]['bytes'],'native extent changed: '+name)
            if (name!=FN and name not in expected_failures) or label=='candidate' or label=='static_list_helper' and name==FN:
                require(complete(row),'strict member changed: '+name)
        require(set(result['failed_old_members'])==expected_failures,'preserved failure inventory changed')
        require(result['preserved_members']==[n for n in old['members'] if complete(result['all_members'][n])],'preserved member inventory changed')
        require(result['failed_old_members']=={n:row for n,row in result['all_members'].items() if n!=FN and not complete(row)},'failure rows changed')
        if label not in {'candidate','static_list_helper'}:require(not complete(result['hud_setup']),'negative became a match')
    candidate=results['candidate']
    require(candidate['unclaimed_helper_residue_bytes']==8,'unclaimed helper residue changed')
    require(any('0x80110274' in n and '4 bytes, all zero' in n for n in candidate['hud_setup']['own_data_notes']),'own-data window changed')
    require(receipt['once_flag']=={'logical_bytes':1,'verified_window_bytes':4,'address':'0x80110274','window_sha256':sha(bytes(4))},'once-flag contract changed')
    require(results['without_count_conversion']['hud_setup']['differing']==24,'count conversion control changed')
    require(results['external_once_flag']['hud_setup']['differing']==8,'external flag control changed')
    for label in ['uninitialized_local_flag','wrong_initial_flag']:require(bool(results[label]['hud_setup']['unverified']),'data negative did not reject')

def check_trace_identity(stock,traced):
    require(stock==traced,'instrumentation changed output')

def verify_trace_bindings(trace,result):
    require(set(trace)=={'candidate','without_count_conversion'},'trace inventory changed')
    source=(GROUP/'hud_setup.c').read_text()
    expected_sources={'candidate':source,'without_count_conversion':source.replace('func_800C8738((u32)D_80110264)','func_800C8738(D_80110264)')}
    for label,row in trace.items():
        require(row['source_sha256']==sha(expected_sources[label].encode()),'trace variant source changed')
        require(bool(row['allocation_decisions']),'empty allocation observation')
        require(row['inputs_sha256']==result['source_sha256'],'trace input binding changed')
        require(row['complete_stock_object_identity'] and row['stock_object_sha256']==row['traced_object_sha256']==result['results'][label]['object_sha256'],'trace object binding changed')
        require(row['target']==result['results'][label]['hud_setup'],'trace comparison changed')
        require(all(d['forced']=='-2' for d in row['allocation_decisions']),'forced compiler observation')

def verify(work,check_receipt=True):
    work=Path(work);work.mkdir(parents=True,exist_ok=True)
    spec=json.loads((GROUP/'group.json').read_text());old=json.loads((OLD/'group.json').read_text())
    require(spec['name']=='menu_save_options','group identity changed')
    require(spec['members']==old['members']+[FN] and spec['claims']==spec['members'],'claim inventory changed')
    require(spec['files']==old['files']+['hud_setup.c'],'source inventory changed')
    require(spec['keep']==old['keep']+[FN] and spec['context']==old['context'],'context or visibility changed')
    require(all((GROUP/f).read_bytes()==(OLD/f).read_bytes() for f in old['files']),'prior source changed')
    require(len(old['members'])==24,'prior member count changed')
    if check_receipt:
        frozen=json.loads((HERE/'verification.json').read_text())
        require(source_bindings()==frozen['source_sha256'],'source receipt changed')
        require(target_bindings()==frozen['target_sha256'],'target receipt changed')
        require(compiler_bindings()==frozen['verification_inputs'],'toolchain/verifier inputs changed')
        verify_report(frozen)
        require(sha((HERE/'compiler-decisions.json').read_bytes())==frozen['compiler_observations_sha256'],'compiler observation receipt changed')
    source=(GROUP/'hud_setup.c').read_text()
    cast='func_800C8738((u32)D_80110264)'
    static='    static s8 D_80110274 = 0;'
    require(source.count(cast)==1 and source.count(static)==1,'ambiguous source mutation')
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
        results[label].update(all_members=rows,context={n:inspect(obj,n) for n in spec['context']},object_sha256=sha(obj.read_bytes()))
        if label=='candidate':
            require(all(complete(r) for r in rows.values()),'strict candidate member failed')
            require(rows[FN]['symbol_bytes']==1048,'candidate extent changed')
            require(any('0x80110274' in n and '4 bytes, all zero' in n for n in rows[FN]['own_data_notes']),'own-data window changed')
            results[label]['all_members']=rows
            # The named inferred helper residue is not a claimed native function.
            data,secs=score._elf(obj);ti=score._text_index(secs)
            syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
            helper,=[s for s in syms if s['name']=='sound_list_init' and s['type']==2 and s['section']==ti]
            require(helper['size']==8,'unclaimed residue changed')
            results[label]['unclaimed_helper_residue_bytes']=helper['size']
        elif label=='static_list_helper':
            require(complete(rows[FN]) and set(results[label]['failed_old_members'])=={'func_800A370C'},'static-helper control changed')
        else:
            require(not complete(rows[FN]),'negative unexpectedly matched')
            require(len(results[label]['preserved_members'])==24,'negative broke prior members')
        if label=='without_count_conversion':require(rows[FN]['differing']==24,'count conversion control changed')
        if label=='external_once_flag':require(rows[FN]['differing']==8,'external flag control changed')
        print(label+': '+rows[FN]['verdict']+'; prior members preserved '+str(len(results[label]['preserved_members']))+'/24')
    image=score.own_data();require(image.read(0x80110274,4)==bytes(4),'native initialized-data window changed')
    result={'verification_inputs':compiler_bindings(),'compiler_observations_sha256':sha((HERE/'compiler-decisions.json').read_bytes()),'claims':spec['claims'],'accepted_image_bytes':0,'protocol_version':3,'status':'STRICT_CLOUD_MATCH_PENDING_UNIT_IMAGE_GATES','new_code_bytes':1048,'prior_members':24,'source_sha256':source_bindings(),'target_sha256':target_bindings(),'once_flag':{'logical_bytes':1,'verified_window_bytes':4,'address':'0x80110274','window_sha256':sha(bytes(4))},'results':results}
    verify_report(result)
    if check_receipt:
        verify_trace_bindings(json.loads((HERE/'compiler-decisions.json').read_text()),result)
        require(result==frozen,'fresh sealed receipt differs')
    return result

if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('--record',action='store_true');ap.add_argument('--work',type=Path);args=ap.parse_args()
    with tempfile.TemporaryDirectory(prefix='sound-init-') as tmp:
        result=verify(args.work or Path(tmp),check_receipt=not args.record)
    if args.record:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    else:print('PASS: 25 strict members, complete controls, sealed source/data/trace receipts')
