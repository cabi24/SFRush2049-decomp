"""Compile the readable C reconstruction and compare bounded outputs to the model."""
from pathlib import Path
import ctypes,json,subprocess,tempfile
from behavior import *

PREFIX=r'''
#include <string.h>
#include <stdint.h>
typedef signed char s8;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef void *OSMesg;
typedef struct OSMesgQueue { void *a,*b; s32 count,first,n; OSMesg *messages; } OSMesgQueue;
OSMesgQueue D_801497A8;
s8 D_80111954;
volatile s8 D_80149AF8[4][2], D_80149B50[4][2];
s8 D_80149B74[4];
u8 D_80156CF0[64];
s32 D_8013FED0[4],D_801403C0[4],D_80149B30[4],D_80149B10[4];
f32 D_80140620[4][2];
#define gStickRaw D_80149B50
#define gStickCal D_80149AF8
#define gHeld D_80149B30
#define gPrevPressed D_80149B10
static unsigned events;
void func_800E7038(void) { int i; events=events*10+1; D_80111954=1; for(i=0;i<4;i++) {D_80149B74[i]=1; D_80149AF8[i][0]=D_80149AF8[i][1]=70;} }
s32 osRecvMesg(OSMesgQueue *q, OSMesg *m, s32 flag) { if(q!=&D_801497A8 || m || flag!=1) __builtin_trap(); events=events*10+2; return 0; }
s32 osSendMesg(OSMesgQueue *q, OSMesg m, s32 flag) { if(q!=&D_801497A8 || m || flag) __builtin_trap(); events=events*10+3; return 0; }
f32 func_800C9590(f32 range,f32 cal,s32 raw) { f32 v=(raw*range)/cal; if(v < -range) v=-range; else if(range<v) v=range; return v; }
'''
SUFFIX=r'''
unsigned run_case(const int32_t *in, unsigned char *out) {
 int i,j,n=1;
 memset(D_80156CF0,0xA5,sizeof(D_80156CF0));
 memset((void*)D_80149B50,0xA5,sizeof(D_80149B50));
 memset(D_8013FED0,0xA5,sizeof(D_8013FED0)); memset(D_801403C0,0xA5,sizeof(D_801403C0)); memset(D_80140620,0xA5,sizeof(D_80140620));
 D_80111954=in[0];events=0;
 for(i=0;i<4;i++) {
  NativePadState *p=(NativePadState *)&D_80156CF0[16*i];
  p->connected=in[n++];p->buttons=in[n++];p->stick_x=in[n++];p->stick_y=in[n++];
  for(j=0;j<2;j++) D_80149AF8[i][j]=in[n++];
  D_80149B74[i]=in[n++];D_80149B30[i]=in[n++];D_80149B10[i]=in[n++];
 }
 func_800E7134(); n=0;
 for(i=0;i<4;i++) {
  unsigned values[8];
  out[n++]=D_80149AF8[i][0];out[n++]=D_80149AF8[i][1];out[n++]=D_80149B50[i][0];out[n++]=D_80149B50[i][1];
  values[0]=D_80149B30[i];values[1]=D_80149B10[i];values[2]=D_8013FED0[i];values[3]=D_801403C0[i];
  memcpy(&values[4],&D_80140620[i][0],4);memcpy(&values[5],&D_80140620[i][1],4);
  for(j=0;j<6;j++) {out[n++]=values[j]>>24;out[n++]=values[j]>>16;out[n++]=values[j]>>8;out[n++]=values[j];}
 }
 return events;
}
'''
def extracted_source():
 source=(HERE/'group/group.c').read_text();start=source.index('/* Transfer controller snapshots');end=source.index('\nvoid player_mode_set',start)
 return source[start:end]

def verify_host(source=None):
 if source is None:source=extracted_source()
 with tempfile.TemporaryDirectory(prefix='input-state-host-') as td:
  p=Path(td);(p/'case.c').write_text(PREFIX+source+SUFFIX)
  r=subprocess.run(['cc','-shared','-fPIC','-std=c11','-O2','-Wall','-Wextra','-Werror',str(p/'case.c'),'-o',str(p/'case.so')],capture_output=True,text=True);require(r.returncode==0,r.stderr)
  lib=ctypes.CDLL(str(p/'case.so'));lib.run_case.argtypes=[ctypes.POINTER(ctypes.c_int32),ctypes.POINTER(ctypes.c_ubyte)];lib.run_case.restype=ctypes.c_uint
  count=0
  for c in fixtures():
   vals=[c['init']]
   for i in range(4):vals+=list(c['pads'][i])+list(c['cal'][i])+[c['mode'][i],c['held'][i],c['prev'][i]]
   inp=(ctypes.c_int32*len(vals))(*vals);out=(ctypes.c_ubyte*(4*28))();ev=lib.run_case(inp,out);expected,events=model(c);raw=bytearray()
   for i in range(4):
    for a in (CAL+2*i,RAW+2*i):raw+=bytes(expected[a+j] for j in range(2))
    for a in (HELD+4*i,PREV+4*i,OUTHELD+4*i,OUTPRESS+4*i,STICK+8*i,STICK+8*i+4):raw+=bytes(expected[a+j] for j in range(4))
   require(bytes(out)==raw and ev==(23 if c['init'] else 123),f'host mismatch case {count}')
   count+=1
 return {'status':'PASS','host_fixtures':count,'source':'body extracted from group/group.c','matching_claim':False}
