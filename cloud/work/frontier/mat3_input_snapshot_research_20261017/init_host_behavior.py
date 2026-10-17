"""Exact extracted C body with deterministic queue/poll/query harness boundaries."""
from init_behavior import *
PREFIX=r'''
#include <stdint.h>
#include <stdlib.h>
typedef signed char s8; typedef unsigned char u8; typedef int s32; typedef unsigned int u32;
typedef void *OSMesg; typedef struct {void *a,*b; s32 count,first,n; OSMesg *msg;} OSMesgQueue;
s8 D_80111954,D_80111968,D_80149B74[4],D_80149B64[4];
volatile s8 D_80149AF8[4][2];
OSMesgQueue D_801497A8; OSMesg D_80152730[1];
static u8 query_script[16];static unsigned query_count,query_position,events;
static volatile u8 *query_byte(void) { if(query_position>=query_count) abort();return &query_script[query_position++]; }
#define D_80111950 (*query_byte())
void player_state_set(s32,s32);void player_mode_set(s32,s32);
void osCreateMesgQueue(OSMesgQueue *q,OSMesg *m,s32 n) {if(q!=&D_801497A8 || m!=D_80152730 || n!=1)abort();events=events*10+1;}
s32 osSendMesg(OSMesgQueue *q,OSMesg m,s32 f) {if(q!=&D_801497A8 || m || f)abort();events=events*10+2;return 0;}
void controller_poll(void) {if(query_position!=query_count)abort();events=events*10+3;}
'''
SUFFIX=r'''
void run_case(const uint32_t *in,uint32_t *out) {
 unsigned i,n=0;D_80111954=in[0];D_80111968=in[1];query_count=in[3];query_position=events=0;
 for(i=0;i<query_count;i++)query_script[i]=in[4+i];
 for(i=0;i<8;i++)D_80149AF8[i/2][i%2]=in[2]+i;
 for(i=0;i<4;i++){D_80149B74[i]=33+i;D_80149B64[i]=79+i;}
 func_800E7038();out[n++]=(u8)D_80111954;out[n++]=(u8)D_80111968;
 for(i=0;i<8;i++)out[n++]=(u8)D_80149AF8[i/2][i%2];
 for(i=0;i<4;i++)out[n++]=(u8)D_80149B74[i];
 for(i=0;i<4;i++)out[n++]=(u8)D_80149B64[i];
 out[n++]=events;out[n++]=query_position;
}
'''
def extracted(source):
 a=source.index('void func_800E7038(void) {');b=source.index('/* Transfer controller snapshots',a)
 c=source.index('void player_mode_set(s32 player, s32 value)',b);return source[a:b]+source[c:]
def verify_host(source):
 with tempfile.TemporaryDirectory(prefix='controller-init-host-') as tmp:
  p=Path(tmp);(p/'case.c').write_text(PREFIX+extracted(source)+SUFFIX)
  r=subprocess.run(['cc','-shared','-fPIC','-std=c11','-O2','-Wall','-Wextra','-Werror',str(p/'case.c'),'-o',str(p/'case.so')],text=True,capture_output=True);require(r.returncode==0,r.stderr)
  lib=ctypes.CDLL(str(p/'case.so'));lib.run_case.argtypes=[ctypes.POINTER(ctypes.c_uint32)]*2;lib.run_case.restype=None;count=0
  for c in cases():
   values=[c['flag'],c['ready'],c['cal'],len(c['script']),*c['script']];input=(ctypes.c_uint32*len(values))(*values);output=(ctypes.c_uint32*20)();lib.run_case(input,output)
   m,events,reads=model(c);expected=[m[FLAG],m[READY]]+[m[CAL+i] for i in range(8)]+[m[MODES+i] for i in range(4)]+[m[STATES+i] for i in range(4)]+[123 if not c['ready'] else 3,reads]
   require(list(output)==expected,f'host mismatch case {count}');count+=1
 return dict(status='PASS',host_fixtures=count,source='unchanged extracted initialization and setter bodies',boundary_stubs=['queue creation','queue seed','controller poll','scripted volatile query reads'])
if __name__=='__main__':print(json.dumps(verify_host(Path(sys.argv[1]).read_text()),indent=2))
