#!/usr/bin/env python3
"""Independent native-vs-host checkpoint behavior proof.

Reads the complete current packet source and protected target words. Generated
C, shared objects and source mutants live only in a temporary directory.
No ROM words, assembly dumps, live integration hashes or lock state are saved.
"""
from pathlib import Path
import argparse, sys, subprocess, struct, json, hashlib, random, ctypes, itertools
import shutil, tempfile
PACKET = Path(__file__).resolve().parent.parent
ROOT = PACKET.parents[3]
SOURCE = PACKET / 'group/candidate.c'
sys.path.insert(0, str(ROOT))
from tools.cloud import score
WORK = None
if not __debug__:
    raise RuntimeError('Verification requires Python assertions; do not use -O')
FN='race_countdown_display'; START=0x800D24C8; MODEL=0x8014A250; STACK=0x803FF000; RET=0x80000000
sha=lambda b:hashlib.sha256(b).hexdigest()
pack=lambda ws:struct.pack('>%dI'%len(ws),*ws)
signed=lambda n,b=32: (n&((1<<b)-1))-(1<<b) if n&(1<<(b-1)) else n&((1<<b)-1)
ADDR={'D_80151CE8':(0x80151CE8,10),'D_80153E88':(0x80153E88,48),'D_80152818':(0x80152818,0x3B8*6),'D_8014A110':(0x8014A110,4),'D_801174B4':(0x801174B4,4),'D_80152734':(0x80152734,2),'D_80152014':(0x80152014,1),'D_80152015':(0x80152015,1),'D_80110680':(0x80110680,6),'D_80110668':(0x80110668,24),'D_8002EB90':(0x8002EB90,4)}
# Byte-addressed, bounds checked MIPS II interpreter. All reads/writes require a mapped region.
class Native:
 def __init__(self,words): self.words=words;self.coverage=set()
 def run(self,inputs,time_bits=0x42F78000):
  mem={}; trace=[]
  def load(addr,n,s=False):
   assert all(addr+i in mem for i in range(n)),('unmapped read',hex(addr),n)
   return int.from_bytes(bytes(mem[addr+i] for i in range(n)),'big',signed=s)&0xffffffff
  def save(addr,n,v):
   assert all(addr+i in mem for i in range(n)),('unmapped write',hex(addr),n)
   for i,b in enumerate((v&((1<<(n*8))-1)).to_bytes(n,'big')):mem[addr+i]=b
  for name,raw in inputs.items():
   addr=MODEL if name=='model' else ADDR[name][0]
   mem.update({addr+i:b for i,b in enumerate(raw)})
  mem.update({STACK-0x1000+i:0xA5 for i in range(0x1100)})
  r=[0xBAD00000+i for i in range(32)]; r[0]=0;r[4]=MODEL;r[5]=time_bits;r[29]=STACK;r[31]=RET;f=[0]*32
  pc=START;pending=None
  def hook(dest):
   nonlocal r
   a=[r[4],r[5],r[6],r[7]]
   if dest==0x800D197C: a += [load(r[29]+j,4) for j in (16,20,24)]
   trace.append([hex(dest),a])
   if dest==0x800D1CE0:save(ADDR['D_80152818'][0]+signed(a[0])*0x3B8+0xEF,1,1)
   for i in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25]: r[i]=0xCDD00000+i
   r[2]=0xDEADBEEF
  count=0
  while pc!=RET:
   count+=1;assert count<2000
   assert START<=pc<START+len(self.words)*4,hex(pc)
   idx=(pc-START)//4;w=self.words[idx];self.coverage.add(idx)
   op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sh=(w>>6)&31;fn=w&63;imm=signed(w&65535,16);immu=w&65535
   transfer=pending;pending=None;nextpc=pc+4
   if op==0:
    if fn==0:r[rd]=(r[rt]<<sh)&0xffffffff
    elif fn==2:r[rd]=r[rt]>>sh
    elif fn==3:r[rd]=(signed(r[rt])>>sh)&0xffffffff
    elif fn==8:pending=(r[rs],False)
    elif fn==0x21:r[rd]=(r[rs]+r[rt])&0xffffffff
    elif fn==0x23:r[rd]=(r[rs]-r[rt])&0xffffffff
    elif fn==0x24:r[rd]=r[rs]&r[rt]
    elif fn==0x25:r[rd]=r[rs]|r[rt]
    elif fn==0x26:r[rd]=r[rs]^r[rt]
    elif fn==0x2a:r[rd]=int(signed(r[rs])<signed(r[rt]))
    elif fn==0x2b:r[rd]=int(r[rs]<r[rt])
    else:raise AssertionError(('special',hex(w)))
   elif op in (2,3):
    if op==3:r[31]=pc+8
    pending=((pc&0xf0000000)|((w&0x3ffffff)<<2),op==3)
   elif op in (4,5,20,21):
    take=(r[rs]==r[rt])==(op in (4,20))
    if take:pending=(pc+4+imm*4,False)
    elif op in (20,21):nextpc=pc+8
   elif op==9:r[rt]=(r[rs]+imm)&0xffffffff
   elif op==10:r[rt]=int(signed(r[rs])<imm)
   elif op==11:r[rt]=int(r[rs]<(imm&0xffffffff))
   elif op==12:r[rt]=r[rs]&immu
   elif op==13:r[rt]=r[rs]|immu
   elif op==15:r[rt]=immu<<16
   elif op in (32,33,35,36,37):r[rt]=load((r[rs]+imm)&0xffffffff,{32:1,33:2,35:4,36:1,37:2}[op],op in (32,33))
   elif op in (40,41,43):save((r[rs]+imm)&0xffffffff,{40:1,41:2,43:4}[op],r[rt])
   elif op==49:f[rt]=load((r[rs]+imm)&0xffffffff,4)
   elif op==57:save((r[rs]+imm)&0xffffffff,4,f[rt])
   elif op==17 and rs==4:f[rd]=r[rt]
   else:raise AssertionError(('opcode',hex(w)))
   r[0]=0
   if transfer:
    dest,call=transfer
    if call:hook(dest);nextpc=r[31]
    else:nextpc=dest
   pc=nextpc
  assert r[29]==STACK
  return {name:bytes(mem[(MODEL if name=='model' else ADDR[name][0])+i] for i in range(len(raw))) for name,raw in inputs.items()},trace

def build_host(candidate=None,suffix=''):
 if candidate is None: candidate=SOURCE
 src='''#include <stddef.h>\n#include <stdarg.h>\n#include "group/candidate.c"\nTrack D_80151CE8; Player D_80153E88[6]; GameCar D_80152818[6]; s32 D_8014A110; u32 D_801174B4; s16 D_80152734; s8 D_80152014,D_80152015,D_80110680[6],D_8010FFC0; f32 D_80110668[6]; volatile f32 D_8002EB90;\nu32 logs[16][9];int nlogs;\nu32 fb(f32 f) {union {f32 f;u32 u;} v;v.f=f;return v.u;}\nvoid rec(u32 id,u32 a,u32 b,u32 c,u32 d,u32 e,u32 f,u32 g) {u32 *p=logs[nlogs++];p[0]=id;p[1]=a;p[2]=b;p[3]=c;p[4]=d;p[5]=e;p[6]=f;p[7]=g;}\nvoid func_800D2458(s32 a,f32 t){rec(0x800D2458,a,fb(t),0,0,0,0,0);}\nvoid func_800D2054(s32 a,f32 t){rec(0x800D2054,a,fb(t),0,0,0,0,0);}\nvoid car_setup_confirm(s32 a,f32 t){rec(0x800D1CE0,a,fb(t),0,0,0,0,0);D_80152818[a].place_locked=1;}\ns32 func_800B61A8(s32 a,s32 b,s32 c,u8 d){if(!D_8010FFC0 || a==-1)return -1;rec(0x80092360,a,b,c,d,0,0,0);return 0xDEADBEEF;}\nu32 car_stats_display(s32 a,f32 b,s32 c,s32 d,s32 n,...){va_list ap;s32 v1,v2;va_start(ap,n);v1=va_arg(ap,s32);v2=va_arg(ap,s32);va_end(ap);rec(0x800D197C,a,fb(b),c,d,n,v1,v2);return 0xDEADBEEF;}\nsize_t layout(int id){switch(id){case 0:return sizeof(Model);case 1:return sizeof(GameCar);case 2:return sizeof(Player);case 3:return sizeof(Track);case 4:return offsetof(Model,slot);case 5:return offsetof(Model,flags);case 6:return offsetof(Model,lap_enabled);case 7:return offsetof(Model,last_cp);case 8:return offsetof(Model,laps);case 9:return offsetof(GameCar,finish_timer);case 10:return offsetof(GameCar,flags311);}return 0;}\n'''
 src += '\nvoid invoke(Model *m,u32 u){union{u32 u;f32 f;}v;v.u=u;race_countdown_display(m,v.f); }\n'
 src=src.replace('group/candidate.c',str(candidate))
 host_c=WORK/('host'+suffix+'.c');host_so=WORK/('host'+suffix+'.so')
 host_c.write_text(src)
 subprocess.run(['cc','-std=c89','-O2','-fPIC','-shared','-fwrapv','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-fstrict-aliasing','-Wall','-Wextra','-o',str(host_so),str(host_c)],check=True,capture_output=True)
 return ctypes.CDLL(str(host_so))
# Host and MIPS have opposite endian order; this precise field list converts only scalar numeric bytes.
FIELDS={'model':[(0x7C6,2),(0x7D4,4),(0x7E2,2),(0x7E4,2)],'D_80151CE8':[(i,2) for i in range(0,10,2)],'D_80152818':[(j*0x3B8+0x30C,4) for j in range(6)],'D_8014A110':[(0,4)],'D_801174B4':[(0,4)],'D_80152734':[(0,2)],'D_80110668':[(i,4) for i in range(0,24,4)],'D_8002EB90':[(0,4)]}
def flip(name,raw):
 raw=bytearray(raw)
 for off,n in FIELDS.get(name,[]):raw[off:off+n]=raw[off:off+n][::-1]
 return bytes(raw)
def host_run(lib,inputs,time_bits=0x42F78000):
 model=ctypes.create_string_buffer(flip('model',inputs['model']))
 for name,raw in inputs.items():
  if name=='model':continue
  dest=(ctypes.c_ubyte*len(raw)).in_dll(lib,name);ctypes.memmove(dest,flip(name,raw),len(raw))
 ctypes.c_int.in_dll(lib,'nlogs').value=0
 lib.invoke.argtypes=[ctypes.c_void_p,ctypes.c_uint32]
 lib.invoke(model,time_bits)
 out={name:flip(name,model.raw[:len(raw)] if name=='model' else bytes((ctypes.c_ubyte*len(raw)).in_dll(lib,name))) for name,raw in inputs.items()}
 logs=(ctypes.c_uint32*(16*9)).in_dll(lib,'logs');trace=[]
 for i in range(ctypes.c_int.in_dll(lib,'nlogs').value):
  row=list(logs[i*9:(i+1)*9]);n=7 if row[0]==0x800D197C else 4 if row[0]==0x80092360 else 2
  trace.append([hex(row[0]),row[1:1+n]])
 return out,trace

def inputs(seed,slot=0,cp=3,enabled=1,laps=1,total=3,mode=0,debug=0,kind=6,locked=0,place=0,remaining=3,sound=1,count=5,finish=3,before=2,loop=1):
 rng=random.Random(seed);d={name:bytearray(rng.randbytes(n)) for name,(a,n) in ADDR.items()};d['model']=bytearray(rng.randbytes(0x808))
 def put(name,off,n,v):d[name][off:off+n]=(v&((1<<(n*8))-1)).to_bytes(n,'big')
 for off,n,v in [(0x7C6,2,slot),(0x7E4,2,cp),(0x7DC,1,enabled),(0x7E8,1,laps)]:put('model',off,n,v)
 for off,v in [(2,loop),(4,finish),(6,before),(8,count)]:put('D_80151CE8',off,2,v)
 put('D_80153E88',slot*8+7,1,kind);put('D_80152818',slot*0x3B8+0xEF,1,locked);put('D_80152818',slot*0x3B8+0xEE,1,place)
 for name,n,v in [('D_8014A110',4,mode),('D_801174B4',4,debug),('D_80152734',2,total),('D_80152014',1,remaining),('D_8002EB90',4,0x43ABCDEF),('D_8010FFC0',1,sound)]:put(name,0,n,v)
 return {k:bytes(v) for k,v in d.items()}

def behavior():
 ADDR['D_8010FFC0']=(0x8010FFC0,1)
 lib=build_host();native=Native(score.targets()[FN]);results=[]
 cases=[]
 for slot,cp,enabled,laps,total,mode,debug,kind,locked,place,remaining,sound in itertools.product([0,1,5],[0,2,3,4],[0,1],[-1,0,1,2],[1,3],[0,1,2],[0,8],[0,6],[-1,0,1],[0,1,2,3],[0,1,3],[0,1]):
  # Deterministic sub-sampling plus targeted branch tests below limits cost without losing all predicate combinations.
  if len(cases)<1 or random.Random(slot*997+cp*89+enabled*5+laps*13+total*23+mode*3+debug+kind+locked*7+place*11+remaining*19+sound*101).randrange(151)==0:
   cases.append(dict(slot=slot,cp=cp,enabled=enabled,laps=laps,total=total,mode=mode,debug=debug,kind=kind,locked=locked,place=place,remaining=remaining,sound=sound))
 for slot,place,locked,sound in itertools.product([0,1,5],[-128,-1,0,1,2,3,127],[-128,-1,0,1,127],[-128,0,1,127]):cases.append(dict(slot=slot,place=place,locked=locked,sound=sound,laps=2))
 for cp,enabled,laps,total,remaining in itertools.product([-32768,-1,2,3,4,32767],[-128,0,1,127],[-128,-1,0,2,126,127],[-32768,-1,0,3,32767],[-128,0,1,127]):cases.append(dict(cp=cp,enabled=enabled,laps=laps,total=total,remaining=remaining))
 for sound in [-128,0,1,127]:cases.append(dict(laps=1,total=3,remaining=3,sound=sound))
 digest=hashlib.sha256()
 for i,case in enumerate(cases):
  inp=inputs(i,**case);time_bits=[0,0x80000000,0x42F78000,0x7F800000,0xFF800000,0x7FC12345,0x7F812345,1,0x7F7FFFFF][i%9];inp['D_8002EB90']=struct.pack('>I',[0,0x80000000,0x43ABCDEF,0x7FCFEDCB,0x7F800001][i%5]);out,trace=native.run(inp,time_bits);host,htrace=host_run(lib,inp,time_bits)
  trace=[[a,b[:7 if int(a,16)==0x800D197C else 4 if int(a,16)==0x80092360 else 2]] for a,b in trace]
  assert out==host,('memory',i,case,[k for k in out if out[k]!=host[k]])
  assert trace==htrace,('trace',i,case,trace,htrace)
  digest.update(b''.join(out.values()));digest.update(json.dumps(trace).encode())
 results={'cases':len(cases),'time_bit_patterns':9,'clock_bit_patterns':5,'host_flags':'-std=c89 -O2 -fPIC -shared -fwrapv -fsanitize=undefined,bounds -fno-sanitize-recover=all -fstrict-aliasing -Wall -Wextra','native_instruction_words_covered':len(native.coverage),'native_instruction_words':len(native.words),'covered_offsets':[hex(x*4) for x in sorted(native.coverage)],'output_sha256':digest.hexdigest(),'host_layout':{k:lib.layout(i) for i,k in enumerate(['Model','GameCar','Player','Track','Model.slot','Model.flags','Model.lap_enabled','Model.last_cp','Model.laps','GameCar.finish_timer','GameCar.flags311'])},'source_sha256':sha(SOURCE.read_bytes()),'native_sha256':sha(pack(native.words))}
 return results

def negative_controls():
 src=SOURCE.read_text();native=Native(score.targets()[FN]);out={}
 mutants={
 'wrap_ge':('current + 1 == D_80151CE8.count','current + 1 >= D_80151CE8.count',dict(cp=7)),
 'unsigned_laps':('s8 laps;','u8 laps;',dict(laps=127,total=3)),
 'reject_finished_lock':('D_80152818[player].place_locked != -1','D_80152818[player].place_locked == 0',dict(laps=2,total=3)),
 'wrong_sound_mode':('func_800B61A8(64, player, 1, 1)','func_800B61A8(64, player, 1, 0)',dict(laps=2,total=3)),
 'wrong_event_arg':('D_80152014 + 79','D_80152014 + 78',dict(laps=0,total=4,remaining=4)),
 'wrong_finish_mask':('m->flags &= ~8','m->flags &= ~16',dict(laps=2,total=3)),
 'premature_lap_enable':('D_80151CE8.before_finish == m->last_cp','D_80151CE8.finish_line == m->last_cp',dict(cp=2,enabled=0))}
 for i,(name,(a,b,case)) in enumerate(mutants.items()):
  assert src.count(a)==1;(WORK/(name+'.c')).write_text(src.replace(a,b))
  lib=build_host(WORK/(name+'.c'),'_'+name);inp=inputs(i,**case);m=bytearray(inp['model']);m[0x7D4:0x7D8]=b'\xff'*4;inp['model']=bytes(m);n,nt=native.run(inp);h,ht=host_run(lib,inp)
  nt=[[a,b[:7 if int(a,16)==0x800D197C else 4 if int(a,16)==0x80092360 else 2]] for a,b in nt]
  assert n!=h or nt!=ht,name
  out[name]={'rejected':True,'different_objects':[k for k in n if n[k]!=h[k]],'different_call_trace':nt!=ht}
 return out


def missing_toolchain():
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        return 'pinned IDO and MIPS GNU linker required'
    if not shutil.which('cc'):
        return 'host C compiler required'
    return None


def verify():
    global WORK
    ADDR['D_8010FFC0'] = (0x8010FFC0, 1)
    expected = '0498412f32c960ca1b267af9cb41677f34375ad415e14ee596d453dc3cf75ad3'
    assert sha(pack(score.targets()[FN])) == expected
    with tempfile.TemporaryDirectory(prefix='checkpoint-independent-') as tmp:
        WORK = Path(tmp)
        return {'schema': 1, 'status': 'PASS', 'behavior': behavior(),
                'compiled_mutants': negative_controls(),
                'limits': [
                    'Disjoint synthetic backing and valid slots 0, 1 and 5 only.',
                    'Called helper internals are explicit boundary hooks; the finish hook sets place_locked=1.',
                    'Native execution poisons O32 caller-saved integer registers.',
                    'Exceptional float patterns test bit transport only, not arithmetic or floating exceptions.',
                    'Signed narrowing follows the tested GCC/IDO implementations; no portable ISO C claim.',
                    'No unrestricted aliasing, invalid-pointer, concurrency, gameplay or image/ROM proof.']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    reason = missing_toolchain()
    if reason:
        print(json.dumps({'status': 'SKIP', 'reason': reason}))
        return 0
    result = verify()
    receipt = Path(__file__).resolve().with_name('verification.json')
    if args.record:
        receipt.write_text(json.dumps(result, indent=2, sort_keys=True) + '\n')
    if args.check:
        assert result == json.loads(receipt.read_text()), 'independent receipt mismatch'
    print(json.dumps(result, indent=2, sort_keys=True))
    return 0


if __name__ == '__main__':
    sys.exit(main())
