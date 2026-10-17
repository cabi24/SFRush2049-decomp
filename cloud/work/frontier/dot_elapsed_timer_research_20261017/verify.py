#!/usr/bin/env python3
"""Fail-closed elapsed-timer partial research; every emitted native body is reported."""
import contextlib,dataclasses,hashlib,io,json,re,shutil,struct,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if (p/'tools/cloud/score.py').is_file())
sys.path.insert(0,str(ROOT))
from tools.cloud import score,owndata
PARTIALS=['checkpoint_hit','object_bytes23_sum','object_bytes_sum_global','world_velocity_integrate']
STUBS=['Hidden','du_gfx_lock','du_gfx_unlock']
def require(ok,message):
    if not ok:raise RuntimeError(message)
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def describe(r):
    def scrub(s):
        s=re.sub(r'retail [0-9a-f]+, got [0-9a-f]+','compared byte values omitted',s)
        return re.sub(r'retail reads this one object at different addresses [^;)]+','native address mapping is inconsistent (values omitted)',s)
    d={**dataclasses.asdict(r),'accepted':r.accepted(),'summary':scrub(r.summary()),'notes':[scrub(x) for x in r.notes]}
    d['errors']=[scrub(x) for x in d['errors']]
    return d
def validate_inputs(directory=HERE):
    directory=Path(directory);spec=json.loads((directory/'group/group.json').read_text());pin=json.loads((directory/'inputs.json').read_text())
    require(spec['claims']==[] and spec['context']==[] and pin['claims']==[] and pin['new_accepted_credit']==0,'Research scope changed')
    require(len(spec['members'])==len(set(spec['members']))==26,'Incomplete native inventory')
    for f,h in pin['tool_sha256'].items():require(digest(ROOT/f)==h,'Changed comparison tool: '+f)
    require(digest(directory/'verify.py')==pin['proof_sha256'],'Changed proof implementation')
    for f,h in pin['report_sha256'].items():require(digest(directory/f)==h,'Changed saved evidence: '+f)
    require(set(spec['files'])|{'group.json'}==set(pin['source_sha256']),'Incomplete source inventory')
    require(set(spec['members'])==set(pin['target_words_sha256']),'Incomplete native target inventory')
    for f,h in pin['source_sha256'].items():require(digest(directory/'group'/f)==h,'Changed source: '+f)
    for n,h in pin['target_words_sha256'].items():
        w=score.targets()[n];require(hashlib.sha256(struct.pack('>%dI'%len(w),*w)).hexdigest()==h,'Changed target: '+n)
    for f,h in pin['compiler_sha256'].items():require(digest(Path(score.ido(f)))==h,'Changed compiler input: '+f)
    return spec

def measure(group,obj,names):
    score.compile_group(group,obj)
    with contextlib.redirect_stdout(io.StringIO()):return {n:describe(score.compare(obj,n,show=0)) for n in names}
def inventory(obj,names):
    data,secs=score._elf(obj)
    syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
    result={s['name']:s['size'] for s in syms if s['name'] in score.symbols(obj)}
    require(set(result)==set(names+STUBS),'Unexpected emitted function inventory')
    require(all(result[n]==8 for n in STUBS),'Changed queue inlining stub')
    return result
def replace_one(group,file,old,new):
    p=group/file;s=p.read_text();require(s.count(old)==1,'Stale source control: '+file);p.write_text(s.replace(old,new,1))
TABLE_SHA='c528ed12076d15a71b70f5cd0c4c23648859690a2f5c19a8e6bea9ec04b93a61'
STACK={0:(-144,-128),0x174:(124,120),0x180:(119,115),0x190:(120,116),0x198:(119,115),0x19c:(124,120),0x1a0:(120,116),0x240:(144,128)}
def residual_proof(obj,rows):
    row=rows['checkpoint_hit'];require(row['differing']==8 and row['total']==145 and not row['extra_words'] and not row['unresolved'] and not row['unverified'] and not row['errors'],'Changed partial boundary')
    require(rows['animElapsedTimerText']['accepted'] and rows['animElapsedTimerText']['total']==308,'Retained text sibling failed')
    syms=score.symbols(obj);words=score.text_words(obj);want=score.targets()['checkpoint_hit'];start=syms['checkpoint_hit'];end=start+580
    got,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,score.image_symbols())
    require(not unresolved and not errors,'Unresolved comparison')
    own_records={}
    for name in ['checkpoint_hit','animElapsedTimerText']:
        addresses=score.image_symbols();own=owndata.verify(obj,name,score.targets()[name],address=addresses[name],image=score.own_data(),start=syms[name],addresses=lambda n:addresses.get(n,score.address_named(n)))
        require(own.ok and own.references>0,'Owned references failed')
        windows=[(sec,a,b,c,d) for sec,items in own.placements.items() for a,b,c,d in items if c==0x80115c88]
        require(len(windows)==1 and windows[0][2]-windows[0][1]==32 and windows[0][4]=='data','Missing complete owned position table')
        objdata=owndata._Object(obj);sec=next(s for s in objdata.sections if s['name']==windows[0][0]);a,b=windows[0][1:3]
        require(hashlib.sha256(objdata.data[sec['off']+a:sec['off']+b]).hexdigest()==TABLE_SHA,'Wrong owned table contents')
        own_records[name]={'extent':32,'table_sha256':TABLE_SHA,'verified_references':own.references,'notes':own.notes}
        if name=='checkpoint_hit':require(set(masks)<=own.sites,'Unproved masked operand')
    require(hashlib.sha256(score.own_data().read(0x80115c88,32)).hexdigest()==TABLE_SHA,'Native table changed')
    differing=[]
    for i,w in enumerate(want):
        g=got[start//4+i];off=i*4;mask=masks.get(start+off,0xffffffff)
        if (w&mask)!=(g&mask):
            require(off in STACK and mask==0xffffffff,'Difference outside fixed stack sites')
            require(w>>16==g>>16,'Changed opcode/register field')
            want_imm,got_imm=STACK[off];require((w&65535)==(want_imm&65535) and (g&65535)==(got_imm&65535),'Changed stack displacement')
            require(((w>>21)&31)==29,'Difference is not stack-relative')
            differing.append(off)
    require(set(differing)==set(STACK),'Incomplete fixed-site residual proof')
    return {'fixed_stack_sites':{str(k):{'native':a,'candidate':b} for k,(a,b) in STACK.items()},'all_other_instruction_fields_equal':True,'owned_data':own_records,'accepted':False}
def native_layout(group,out):
    tests={'blit_extent':'sizeof(Blit)==64','blit_hide':'(u32)&((Blit *)0)->hide==26','blit_callback':'(u32)&((Blit *)0)->animFunc==40','blit_id':'(u32)&((Blit *)0)->animID==44','blit_depth':'(u32)&((Blit *)0)->state==18','table_extent':'sizeof(elapsedTimerPos)==32'}
    src=(group/'candidate.c').read_text()
    for n,e in tests.items():src+='\ntypedef char layout_'+n+'[('+e+')?1:-1];\n'
    p=out/'layouts.c';p.write_text(src);score.compile_single(p,'-g0 -O3 -mips2 -G 0 -non_shared',out/'layouts.o');return list(tests)
def verify(directory=HERE):
    directory=Path(directory);spec=validate_inputs(directory);expected=json.loads((directory/'verification.json').read_text());fidelity=json.loads((directory/'fidelity.json').read_text())
    require(fidelity['claims']==[] and fidelity['new_accepted_credit']==0 and fidelity['observation_only'] and not fidelity['forced_registers'] and fidelity['byte_identical'],'Invalid research claim')
    with tempfile.TemporaryDirectory(prefix='elapsed-research-') as tmp:
        root=Path(tmp);base=root/'base';shutil.copytree(directory/'group',base);obj=root/'base.o';rows=measure(base,obj,spec['members'])
        require(rows==expected,'Complete member receipt changed')
        require(digest(obj)==fidelity['candidate_object_sha256']==fidelity['stock_stage_object_sha256']==fidelity['traced_object_sha256'],'Object fingerprint changed')
        require(set(n for n,r in rows.items() if not r['accepted'])==set(PARTIALS),'Known partial inventory changed')
        result={'members':rows,'emitted_functions':inventory(obj,spec['members']),'residual_proof':residual_proof(obj,rows),'native_layout_checks':native_layout(base,root),'negatives':{},'controls':{},'claims':[],'new_accepted_credit':0,'canonical_installation_or_image_acceptance':False}
        s=(base/'candidate.c').read_text()
        tests={
          'wrong_animid_mask':s.replace('win = blt->animID;','win = blt->animID & 15;'),
          'wrong_restore':s.replace('object_byte9_set(fixedWidth);','object_byte9_set(0);',1),
          'wrong_depth':s.replace('blt->state = 100;','blt->state = 99;'),
          'wrong_alpha':s.replace('blt->alpha = D_80116187;','blt->alpha = 128;'),
          'wrong_native_field_order':s.replace('    blt->alpha = D_80116187; /* Native HUD alpha, versus GC128. */\n    blt->state = 100; /* Native-only depth field. */','    blt->state = 100; /* Native-only depth field. */\n    blt->alpha = D_80116187; /* Native HUD alpha, versus GC128. */'),
          'wrong_gc_position_table':s.replace('{{272,142},{272,142},{240,197},{160,134}}','{{272,144},{272,144},{240,200},{160,136}}'),
          'externalized_position_table':s.replace('static s32 elapsedTimerPos[4][2] = {{272,142},{272,142},{240,197},{160,134}};','extern s32 D_80115C88[4][2];').replace('elapsedTimerPos[','D_80115C88[')}
        for label,changed in tests.items():
            require(changed!=s,'Stale negative '+label);child=root/label;shutil.copytree(base,child);(child/'candidate.c').write_text(changed);co=root/(label+'.o');rr=measure(child,co,spec['members'])
            rejected=False
            try:residual_proof(co,rr)
            except RuntimeError:rejected=True
            require(rejected,'Mutation escaped full residual/ownership proof: '+label)
            require(all(rr[n]==rows[n] for n in spec['members'] if n not in ['checkpoint_hit','animElapsedTimerText']),'Mutation changed unrelated context')
            result['negatives'][label]={'rejected':True,'blit':rr['checkpoint_hit'],'retained_text':rr['animElapsedTimerText'],'other_24_members_unchanged':True}
        pattern=r's32 Hidden\(Blit \*blt,s32 hide\)\n\{.*?\n\}\n';m=re.search(pattern,s,re.S);require(m is not None,'Missing real Hidden body')
        changed=s[:m.start()]+'s32 Hidden(Blit *,s32);\n'+s[m.end():]+'\n'+m.group(0)
        child=root/'hidden_after';shutil.copytree(base,child);(child/'candidate.c').write_text(changed);rr=measure(child,root/'hidden_after.o',spec['members'])
        require(rr==rows,'Definition-order control changed native results');residual_proof(root/'hidden_after.o',rr);result['controls']['real_hidden_definition_after']={'all_26_members_unchanged':True}
        return result
if __name__=='__main__':print(json.dumps(verify(),indent=2,sort_keys=True))
