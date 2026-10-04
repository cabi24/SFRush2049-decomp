#!/usr/bin/env python3
"""Pinned strict relocated-word and actual ELF-extent checks for complete research."""
import hashlib,json,sys,tempfile,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[3];sys.path.insert(0,str(ROOT))
from tools.cloud import score
SIZES={'80019F48':808,'8001A270':872}
EXPECTED={'80019F48':[(170,2,816),(200,53,1028)],'8001A270':[(195,6,900),(214,55,1108)]}
LAYOUT={'80019F48':[('sizeof(Layer)',12),('sizeof(Voice)',416),('(unsigned int)&((Layer*)0)->priority',6),('(unsigned int)&((Layer*)0)->panning',8),('(unsigned int)&((Voice*)0)->child',16),('(unsigned int)&((Voice*)0)->parent',20)],'8001A270':[('sizeof(Keymap)',8),('(unsigned int)&((Keymap*)0)->transpose',2),('(unsigned int)&((Keymap*)0)->panning',3),('(unsigned int)&((Keymap*)0)->priority',4)]}
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def extent(obj,name):
    data,secs=score._elf(obj);sy=[s for i,se in enumerate(secs)if se['type']==2 for s in score._symbol_table(data,secs,i)if s['name']==name]
    assert len(sy)==1 and sy[0]['type']==2 and sy[0]['value']==0 and sy[0]['section']==score._text_index(secs)
    return sy[0]['size']
def inspect(obj,name):
    result=score.compare(obj,name,show=0);size=extent(obj,name);w=score.text_words(obj);r,m,u,v,e=score.relocate(obj,w,0,size,score.image_symbols());assert not(m or u or v or e)
    target=score.targets()[name];exact=size==len(target)*4 and r[:len(target)]==target
    return dict(differing_words=result.differing,total_words=result.total,extra_nonzero_words=result.extra_words,elf_function_bytes=size,text_bytes=len(w)*4,full_function_relocations_resolved=True,strict_match=result.accepted(),full_relocated_equal_and_exact_extent=exact)
def run():
    pins=json.loads((P/'input_pins.json').read_text())
    for path,want in pins['source_hashes'].items():assert digest(ROOT/path)==want,path
    compiler={p.name:digest(p)for p in score.IDO.iterdir()if p.is_file()};assert compiler==pins['compiler_files_sha256']
    score.ASM_DIR=ROOT/'asm/us/boot_tail';manifest=subprocess.check_output(['sha256sum','-c','SHA256SUMS'],cwd=score.ASM_DIR,text=True).splitlines()
    def rows(p):return {r['address']:r['size']for r in json.loads(p.read_text())['functions']}
    inventory=rows(ROOT/'specs/015-boot-tail-runtime/inventory.json');assert inventory==rows(score.ASM_DIR/'extents.json');assert len(inventory)==439 and sum(inventory.values())==99120
    results=[];calls={};incoming={};layouts={}
    with tempfile.TemporaryDirectory(prefix='constructor-verify-') as td:
        td=Path(td);getter=ROOT/'cloud/matches/boot_tail/func_80010A00.c';score.compile_single(getter,score.DEFAULT_FLAGS,td/'getter.o');assert score.compare(td/'getter.o',getter.stem,show=0).accepted() and extent(td/'getter.o',getter.stem)==12
        for a,expectedSize in SIZES.items():
            name='func_'+a;source=P/'nonmatch'/(name+'.c');target=score.targets()[name];assert len(target)*4==expectedSize and inventory['0x'+a]==expectedSize
            calls[name]=[(i*4,'func_%08X'%(0x80000000|((w&0x3FFFFFF)<<2)))for i,w in enumerate(target)if w>>26==3]
            indirect=[i*4 for i,w in enumerate(target)if w>>26==0 and w&63 in (8,9)];assert len(indirect)==1 and target[indirect[0]//4]>>21&31==31
            incoming[name]=[(f,i*4)for f,words in score.targets().items()for i,w in enumerate(words)if w>>26==3 and (0x80000000|((w&0x3FFFFFF)<<2))==int(a,16)]
            for idx,opt in enumerate(('O2','O1')):
                flags=score.DEFAULT_FLAGS.replace('O2',opt);obj=td/(a+opt+'.o');score.compile_single(source,flags,obj);r=inspect(obj,name)
                assert (r['differing_words'],r['extra_nonzero_words'],r['elf_function_bytes'])==EXPECTED[a][idx]
                assert not r['strict_match'] and not r['full_relocated_equal_and_exact_extent']
                r.update(function=name,target_bytes=expectedSize,source_sha256=digest(source),flags=flags,effective_flags=flags+' '+score.R4300_CC);results.append(r)
            checks=[('sizeof(void*)',4),('sizeof(u32)',4),('sizeof(u16)',2),('sizeof(u8)',1)]+LAYOUT[a]
            src=td/(a+'-layout.c');src.write_text(source.read_text()+'\n'+''.join('typedef char native_layout_%d[(%s == %d)?1:-1];\n'%(i,e,v)for i,(e,v)in enumerate(checks)));score.compile_single(src,score.DEFAULT_FLAGS,td/(a+'-layout.o'));layouts[name]=checks
    assert incoming['func_80019F48']==[('func_8001A270',756),('func_8001A270',832)]
    assert incoming['func_8001A270']==[('func_80017D38',672),('func_8001B1D0',172)]
    return dict(result='PASS',base_commit='1ba71e6c8cca24dcaffd0b831ace067b3046fe19',claim_commit='35f9b0cf',pinned_compiler_files=len(compiler),manifest=manifest,extent_functions=439,extent_bytes=99120,getter_exact_match=True,complete_nonmatch_functions=2,complete_nonmatch_bytes=1680,strict_match_functions=0,strict_match_bytes=0,results=results,layout_assertions=layouts,native_calls=calls,native_incoming=incoming)
if __name__=='__main__':print(json.dumps(run(),indent=2))
