#!/usr/bin/env python3
"""Content sweep: compile every ultralib TU, compare each function's reloc-masked words against every unmatched static target."""
import os,sys,glob,struct,subprocess,shlex,json,hashlib,tempfile
HERE=os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0,HERE)
import importlib.util
spec=importlib.util.spec_from_file_location('sc0',HERE+'/score.py'); S=importlib.util.module_from_spec(spec); spec.loader.exec_module(S)
TK=os.path.expanduser('~/rush2049/cache/toolkits/d4c39cbc85750cd02d3494f318c65e770bbf7d4b10aae137bcf5b5f149137c5b')
ul=HERE+'/ul'
MASK={4:0xFC000000,5:0xFFFF0000,6:0xFFFF0000}
def funcs(obj):
    data,secs=S._elf(obj); t=S._text_index(secs)
    words=S.text_words(obj)
    masked=list(words)
    for sec in secs:
        if sec['type']==9 and sec['info']==t:
            for k in range(sec['size']//8):
                off,info=struct.unpack_from('>II',data,sec['off']+8*k)
                ty=info&0xFF
                if off//4<len(masked) and ty in MASK: masked[off//4]&=MASK[ty]
    syms=[]
    for i,sec in enumerate(secs):
        if sec['type']==2:
            for s in S._symbol_table(data,secs,i):
                if s['section']==t and s['type']==2: syms.append(s)
    syms.sort(key=lambda s:s['value'])
    out={}
    for n,s in enumerate(syms):
        end=s['value']+s['size'] if s['size'] else (syms[n+1]['value'] if n+1<len(syms) else len(words)*4)
        w=masked[s['value']//4:end//4]
        while w and w[-1]==0: w=w[:-1]
        out[s['name']]=tuple(w)
    return out
tg={}
for p in glob.glob(HERE+'/tgt/*.o'):
    n=os.path.basename(p)[:-2]
    w=funcs(p)
    # a target .o has one function; take whole text
    words=S.text_words(p)
    data,secs=S._elf(p); t=S._text_index(secs); m=list(words)
    for sec in secs:
        if sec['type']==9 and sec['info']==t:
            for k in range(sec['size']//8):
                off,info=struct.unpack_from('>II',data,sec['off']+8*k)
                if (info&0xFF) in MASK: m[off//4]&=MASK[info&0xFF]
    while m and m[-1]==0: m.pop()
    tg[tuple(m)]=tg.get(tuple(m),[])+[n]
print('targets',len(tg),file=sys.stderr)
files=sorted(glob.glob(ul+'/src/**/*.c',recursive=True))+sorted(glob.glob(ul+'/src/**/*.s',recursive=True))
vers=sys.argv[1].split(',') if len(sys.argv)>1 else ['L','H']
opts=[('-O2','-mips2'),('-O1','-mips2'),('-O2','-mips3 -32'),('-O1','-mips3 -32')]
res=[]
tmp=tempfile.mkdtemp()
def run(f,v,o,m):
    isasm=f.endswith('.s')
    if isasm and o!='-O2': return
    out=tmp+'/x.o'
    if os.path.exists(out): os.unlink(out)
    cmd=[TK+'/ido/cc','-c','-Xcpluscomm','-g0',o]+shlex.split(m)+['-G','0','-non_shared','-DBUILD_VERSION=VERSION_'+v,'-DBUILD_VERSION_STRING="2.0%s"'%v,'-D_FINALROM','-DNDEBUG','-D_LANGUAGE_ASSEMBLER' if isasm else '-D_LANGUAGE_C','-DF3DEX_GBI','-I',ul+'/include','-I',ul+'/include/PR','-I',ul+'/src','-I',os.path.dirname(f),'-I',ul+'/include/compiler/ido','-I',TK+'/shim','-o',out,f]
    try: p=subprocess.run(cmd,capture_output=True,text=True,timeout=60)
    except Exception: return
    if p.returncode or not os.path.exists(out): return
    try: fs=funcs(out)
    except Exception: return
    for name,w in fs.items():
        if w in tg and len(w)>1:
            res.append((name,os.path.relpath(f,ul+'/src'),v,o,m,tg[w]))
            print(name,os.path.relpath(f,ul+'/src'),v,o,m,tg[w],flush=True)
for f in files:
    for v in vers:
        for o,m in opts:
            run(f,v,o,m)
json.dump(res,open(HERE+'/sweep_%s.json'%('_'.join(vers)),'w'))
