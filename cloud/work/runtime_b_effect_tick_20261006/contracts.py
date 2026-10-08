#!/usr/bin/env python3
"""Compiler-free base-anchored entry census and service contracts."""
import argparse,hashlib,json,re,struct,subprocess,zlib
from pathlib import Path
from semantic import BASE,TARGETS

def audit(root):
    def git(*args):return subprocess.check_output(['git','-C',str(root),*args])
    def read(path):return git('show',BASE+':'+path)
    asset=read('assets/us/data.bin')
    result={'base':BASE,'targets':{},'direct_callers':{},'external_services':{},'pointer_literals_in_B':{}}
    services={0x8008B2E4:('func_8008B2E4','f32 func_8008B2E4(f32 range);'),0x80090770:('func_80090770','void func_80090770(s16 index, u16 value);'),0x8008B0D8:('model_transform_setup','void func_8008B0D8(s32 index, s32 mode, u32 viewports);')}
    addresses={a:n for n,(a,_,_) in TARGETS.items()}
    for pop,rom,base in [('blob',0xB0CB10,0x80086A50),('ovl_b',0xB6FEC4,0x8038A400)]:
        image=zlib.decompress(asset[rom-0x283D0:],-15)
        symbols=json.loads(read('asm/us/'+pop+'/symbols.json'))['symbols']
        for file in git('ls-tree','-r','--name-only',BASE,'asm/us/'+pop).decode().splitlines():
            if not file.endswith('.s'):continue
            for section in read(file).decode().split('.section .text.')[1:]:
                name=section.split(',')[0];start=int(symbols[name],16)
                words=[int(x,16) for x in re.findall(r'\.word 0x([0-9A-Fa-f]{8})',section)]
                raw=struct.pack('>'+str(len(words))+'I',*words)
                if name in TARGETS and pop=='ovl_b':
                    a,n,h=TARGETS[name];assert a==start and len(raw)==n and hashlib.sha256(raw).hexdigest()==h
                    assert raw==image[a-base:a-base+n]
                    result['targets'][name]=dict(address=hex(a),size=n,sha256=h)
                if start in services and pop=='blob':
                    actual,prototype=services[start];assert name==actual and raw==image[start-base:start-base+len(raw)]
                    source_path='src/blob/'+name+'.c';source=read(source_path).decode();assert name in source
                    result['external_services'][name]=dict(address=hex(start),size=len(raw),sha256=hashlib.sha256(raw).hexdigest(),prototype=prototype,base_source=source_path)
                for j,w in enumerate(words):
                    if w>>26 not in (2,3):continue
                    to=((start+4*j+4)&0xF0000000)|((w&0x3FFFFFF)<<2)
                    if to in addresses:result['direct_callers'].setdefault(addresses[to],[]).append(dict(population=pop,caller=name,site=hex(start+4*j),kind='jal' if w>>26==3 else 'jump'))
        if pop=='ovl_b':
            for name,(a,n,h) in TARGETS.items():result['pointer_literals_in_B'][name]=[hex(base+j) for j in range(0,len(image)-3,4) if int.from_bytes(image[j:j+4],'big')==a]
    assert result['direct_callers']['func_80390B10']==[dict(population='ovl_b',caller='func_80390F60',site='0x80390f78',kind='jal')]
    assert result['direct_callers']['func_80390D38']==[dict(population='ovl_b',caller='func_80390F60',site='0x80390f70',kind='jal')]
    assert result['direct_callers']['func_80390F60']==[dict(population='blob',caller='render_viewport_init',site='0x800faa1c',kind='jal')]
    result['limits']='Direct-edge census and aligned B-image pointer literals support, but do not prove, original private visibility. Computed addresses, indirect reachability, other runtime images and original source-file boundaries are not globally proved.'
    return result
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);args=p.parse_args()
    result=audit(args.reference_root);args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
