#!/usr/bin/env python3
"""Exercise actual C under C89 O2 and ASan/UBSan against independent fixtures."""
import hashlib,json,os,struct,subprocess,tempfile
from pathlib import Path
from test_native import cases,behavior,MASK,LAYERS,KEYS,VOICES,get
P=Path(__file__).resolve().parent
def run():
    rows=[]
    with tempfile.TemporaryDirectory(prefix='constructor-host-') as td:
        for which,address in enumerate((0x80019F48,0x8001A270)):
            fixtures=[f for _,f in cases()if f['a']==address];inputs=bytearray();expected=[]
            for f in fixtures:
                words=[int(f['missing']),f['count'],f.get('count_after_port',MASK),len(f['ports']),len(f['created']),len(f['roots'])]+f['args']
                for key in ('ports','created','roots'):words+=f[key]+[0]*(16-len(f[key]))
                inputs+=b''.join(struct.pack('>I',w&MASK)for w in words)+bytes(f['regions'][LAYERS]).ljust(60,b'\0')+f['regions'][KEYS]
                cursors={};events=[]
                def call(addr,args):
                    ev=[0 if x=='count' else x for x in args];events.append([addr]+ev+[0]*(11-len(ev)))
                    slot=cursors.get(addr,0);cursors[addr]=slot+1
                    seq=f['ports'] if addr==0x80019ED0 else f['created'] if addr==0x80024988 else f['roots'] if addr==0x8001ECE0 else []
                    default=MASK if addr==0x80019ED0 else (0x12340000|(slot+3)) if addr==0x80024988 else 0x76543210
                    return seq[slot]if slot<len(seq)else default
                result,regions=behavior(f,call);expected.append((result,events,[(get(regions[VOICES],i*416+16),get(regions[VOICES],i*416+20))for i in range(256)]))
            for label,flags in [('c89_O2',['-O2']),('asan_ubsan',['-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie'])]:
                exe=Path(td)/(str(which)+label);cmd=['gcc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-fno-inline','-DWHICH=%d'%which,*flags,str(P/'host_behavior.c'),'-o',str(exe)]
                subprocess.run(cmd,check=True,capture_output=True)
                proc=subprocess.run([str(exe)],input=inputs,capture_output=True,check=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'));data=proc.stdout;pos=0
                def read():
                    nonlocal pos
                    v=struct.unpack_from('>I',data,pos)[0];pos+=4;return v
                for result,events,links in expected:
                    assert read()==result;n=read();assert n==len(events)
                    assert [[read()for _ in range(12)]for _ in range(n)]==events
                    if which==0:assert [(read(),read())for _ in range(256)]==links
                assert pos==len(data)
                rows.append(dict(function='func_%08X'%address,configuration=label,cases=len(fixtures),result='PASS'))
    return dict(result='PASS',rows=rows,source_hashes={q.name:hashlib.sha256(q.read_bytes()).hexdigest()for q in [P/'host_behavior.c',*sorted((P/'nonmatch').glob('*.c'))]},limits='Synthetic initialized resource records and valid voice indices; O32 layout established separately. Real helpers are audited contract stubs, not integration tests.')
if __name__=='__main__':print(json.dumps(run(),indent=2))
