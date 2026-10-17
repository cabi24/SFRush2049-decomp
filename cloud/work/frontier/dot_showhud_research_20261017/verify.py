#!/usr/bin/env python3
"""Fail-closed conditional ShowHUD research proof; every emitted native body is reported."""
import contextlib,dataclasses,hashlib,io,json,re,shutil,struct,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if (p/'tools/cloud/score.py').is_file())
sys.path.insert(0,str(ROOT))
from tools.cloud import score,owndata
PARTIALS=['object_bytes23_sum','object_bytes_sum_global','world_velocity_integrate','sound_handles_clear']
STUBS=['du_gfx_lock','du_gfx_unlock']
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
    require(len(spec['members'])==len(set(spec['members']))==32,'Incomplete native inventory')
    for f,h in pin['tool_sha256'].items():require(digest(ROOT/f)==h,'Changed comparison tool: '+f)
    require(digest(directory/'verify.py')==pin['proof_sha256'],'Changed proof implementation')
    require(set(spec['files'])|{'group.json'}==set(pin['source_sha256']),'Incomplete source inventory')
    require(set(spec['members'])==set(pin['target_words_sha256']),'Incomplete native target inventory')
    for f,h in pin['source_sha256'].items():require(digest(directory/'group'/f)==h,'Changed source: '+f)
    for n,h in pin['target_words_sha256'].items():
        w=score.targets()[n];require(hashlib.sha256(struct.pack('>%dI'%len(w),*w)).hexdigest()==h,'Changed target: '+n)
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
def emitted_sections(path):
    obj=owndata._Object(path)
    return {s['name']:hashlib.sha256(obj.data[s['off']:s['off']+s['size']]).hexdigest() for s in obj.sections if s['name'] and s['name']!='.mdebug'}
def native_layout(group,out):
    # Actual source types, compiled by the target compiler. No host pointer-size inference.
    checks={
      'candidate.c':[('sizeof_RecordSlot','sizeof(RecordSlot)==52'),('sizeof_RecordRow','sizeof(RecordRow)==104'),('row_second','__builtin_offsetof(RecordRow,b)==52'),('multi_extent','sizeof(MultiBlit)==36'),('multi_callback','__builtin_offsetof(MultiBlit,animfunc)==28'),('table_extent','sizeof(ModelTables)==40'),('table_indices','__builtin_offsetof(ModelTables,data1)==12'),('table_strings','__builtin_offsetof(ModelTables,ptrs2)==16')],
      'panes.c':[('blit_extent','sizeof(Blit)==64'),('pane_extent','sizeof(Pane)==32'),('blit_width','__builtin_offsetof(Blit,width)==20'),('blit_callback','__builtin_offsetof(Blit,animFunc)==40'),('pane_children','__builtin_offsetof(Pane,blits)==12')],
      'peer_gr3.c':[('font_header_extent','sizeof(Bank)==16'),('font_entry_extent','sizeof(Entry)==12')],
      'peer_slot_group.c':[('same_font_header_extent','sizeof(Bank)==16'),('resource_slot_extent','sizeof(ResSlot)==20'),('resource_data_offset','__builtin_offsetof(ResSlot,data)==12')]}
    result={}
    for f,tests in checks.items():
        # IDO's constant offsetof spelling avoids needing host-specific headers.
        text=(group/f).read_text()
        for name,expr in tests:
            expr=re.sub(r'__builtin_offsetof\((\w+),(\w+)\)',r'((unsigned int)&((\1 *)0)->\2)',expr)
            text+='\ntypedef char layout_'+name+'[('+expr+') ? 1 : -1];\n'
        p=out/(f+'.layout.c');p.write_text(text);score.compile_single(p,'-g0 -O3 -mips2 -G 0 -non_shared',out/(f+'.layout.o'));result[f]=[n for n,e in tests]
    return result

def verify(directory=HERE):
    directory=Path(directory);spec=validate_inputs(directory);expected=json.loads((directory/'verification.json').read_text());fidelity=json.loads((directory/'fidelity.json').read_text())
    require(fidelity['claims']==[] and fidelity['new_accepted_credit']==0 and fidelity['byte_identical'] and fidelity['observation_only'] and not fidelity['forced_registers'],'Invalid research/fidelity claim')
    with tempfile.TemporaryDirectory(prefix='showhud-research-') as tmp:
        root=Path(tmp);base=root/'base';shutil.copytree(directory/'group',base);obj=root/'base.o';rows=measure(base,obj,spec['members'])
        require(rows==expected,'Complete member receipt changed')
        require(digest(obj)==fidelity['candidate_object_sha256']==fidelity['stock_stage_object_sha256']==fidelity['traced_object_sha256'],'Stock object fingerprint changed')
        require(rows['skid_mark_render']['accepted'] and rows['skid_mark_render']['total']==349,'Conditional ShowHUD equality failed')
        require(set(n for n,r in rows.items() if not r['accepted'])==set(PARTIALS),'Known partial boundary changed')
        require(len(rows['skid_mark_render']['notes'])==2 and all('verified by address only' in s for s in rows['skid_mark_render']['notes']),'BSS address-only proof changed')
        output={'members':rows,'emitted_functions':inventory(obj,spec['members']),'native_layout_checks':native_layout(base,root),'negatives':{},'source_evidence_controls':{},'claims':[],'new_accepted_credit':0,'conditional_clock_qualification':True,'canonical_installation_or_image_acceptance':False}
        s=(base/'candidate.c').read_text()
        anchor='                    D_801543A8 = object_manager_update(D_80114A0C[D_80154394][D_801543B0], -1);\n                }\n            }'
        require(s.count(anchor)==1,'Stale count-loop anchor')
        # Replace the whole loop precisely; keep all surrounding source unchanged.
        while_count=s.replace('            for (D_801543B0 = 0; D_80114A0C[D_80154394][D_801543B0]; D_801543B0++) {','            D_801543B0 = 0;\n            while (D_80114A0C[D_80154394][D_801543B0] != 0) {').replace(anchor,anchor.replace('\n            }','\n                D_801543B0++;\n            }'))
        reverse=s.replace('                x = 280 - D_801543A8;\n                y = 20;','                y = 20;\n                x = 280 - D_801543A8;').replace('                x = 280 - D_801543A8;\n                y = 145 - D_801543B0 * 10;','                y = 145 - D_801543B0 * 10;\n                x = 280 - D_801543A8;')
        old_static=s.replace('    static f32 delay;\n    static f32 nextSwitch;\n','').replace('delay','D_8015462C').replace('nextSwitch','D_80154644')
        old_static='extern float D_8015462C, D_80154644;\n'+old_static
        tests={
          'external_static_storage':old_static,
          'old_while_credit_count':while_count,
          'reversed_corner_assignments':reverse,
          'wrong_credit_divisor':s.replace('viGetTimeToDeadline() / 13.0f','viGetTimeToDeadline() / 14.0f'),
          'wrong_credit_font':s.replace('object_create(11);','object_create(12);'),
          'wrong_native_hud_count':s.replace('D_801158DC, 8)','D_801158DC, 5)'),
          'wrong_coin_record_stride':s.replace('u8 pad02[50];','u8 pad02[46];'),
          'wrong_native_bottom_corner':s.replace('145 - D_801543B0 * 10','149 - D_801543B0 * 10'),
          'wrong_pane_permanence':s.replace('192, 0, 0, 0);','192, 0, 0, 1);')}
        for label,changed in tests.items():
            require(changed!=s,'Missing negative anchor: '+label);child=root/label;shutil.copytree(base,child);(child/'candidate.c').write_text(changed);r=measure(child,root/(label+'.o'),spec['members'])
            require(not r['skid_mark_render']['accepted'],'Source negative escaped: '+label)
            output['negatives'][label]={'rejected':True,'target':'skid_mark_render','receipt':r['skid_mark_render'],'other_member_results_unchanged':all(r[n]==rows[n] for n in spec['members'] if n!='skid_mark_render')}
            require(output['negatives'][label]['other_member_results_unchanged'],'Source negative changed unrelated native body: '+label)
        controls={
          'ordinary_seconds':s.replace('extern volatile f32 D_8002EB90;','extern f32 D_8002EB90;'),
          'three_named_local_diagnostic':s.replace('        s32 i; /* Reconstruction-only loop index; absent from original named-local list. */\n','').replace('for (i = 0; i < 4; i++)','for (x = 0; x < 4; x++)').replace('[i]','[x]')}
        for label,changed in controls.items():
            require(changed!=s,'Missing evidence-control anchor: '+label);child=root/label;shutil.copytree(base,child);(child/'candidate.c').write_text(changed);control_obj=root/(label+'.o');r=measure(child,control_obj,spec['members'])
            if label=='ordinary_seconds':require(not r['skid_mark_render']['accepted'],'Clock source-evidence boundary changed')
            else:require(r==rows and emitted_sections(control_obj)==emitted_sections(obj),'Three-local diagnostic equivalence changed')
            require(all(r[n]==rows[n] for n in spec['members'] if n!='skid_mark_render'),'Evidence control changed unrelated native body')
            output['source_evidence_controls'][label]={'members':r,'emitted_sections_equal':emitted_sections(control_obj)==emitted_sections(obj),'whole_object_equal':digest(control_obj)==digest(obj)}
        return output
if __name__=='__main__':print(json.dumps(verify(),indent=2,sort_keys=True))
