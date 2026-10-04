"""Actual-source C89 sanitizer tests with explicit external-contract fixtures.

Host pointers test logical behavior; native widths are checked independently by IDO.
The two external thresholds are synthetic values, never inferred image data.
"""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
ROOT=Path(__file__).resolve().parents[4]
WORK=Path(__file__).resolve().parent

class Semantics(unittest.TestCase):
    def check_c(self,name,body):
        source=ROOT/'cloud/matches/boot_tail'/('func_'+name+'.c')
        if not source.exists():source=WORK/'nonmatch'/('func_'+name+'.c')
        text='#include <assert.h>\n#include <string.h>\n#include <math.h>\n#include <stddef.h>\n#include "'+str(source)+'"\n'+body
        with tempfile.TemporaryDirectory(prefix='bt03-spatial-tests-') as tmp:
            p=Path(tmp);(p/'test.c').write_text(text)
            compiled=subprocess.run(['cc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-O1','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined,float-cast-overflow','-no-pie',str(p/'test.c'),'-lm','-o',str(p/'test')],capture_output=True,text=True)
            self.assertEqual(compiled.returncode,0,compiled.stdout+compiled.stderr)
            r=subprocess.run([str(p/'test')],capture_output=True,text=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'))
            self.assertEqual(r.returncode,0,r.stdout+r.stderr)

    def test_portamento_arithmetic_and_live_chain(self):
        self.check_c('80019C8C',r'''
VoiceState D_8004BEB8[8];u8 D_8004FA18;
static unsigned events[128],event_count,start_count,mode;
static unsigned active_mask;static u32 start_result;
static void event(unsigned a,unsigned b){events[event_count++]=(a<<24)|b;assert(event_count<128);}
u8 func_8001467C(u32 i){event(1,i);if(mode==1&&i==0){D_8004BEB8[i].key=300;D_8004BEB8[i].cents=-31;D_8004FA18=3;}return (active_mask>>i)&1;}
void func_8001EB10(VoiceState *p){unsigned i=(unsigned)(p-D_8004BEB8);event(2,i);if(mode==2){p->identifier=0x12340000U|i;p->flags^=0x100;}}
u32 func_8001ECE0(VoiceState *p){unsigned i=(unsigned)(p-D_8004BEB8);event(3,i);start_count++;if(mode==3&&start_count==1)return 0xFFFFFFFFU;return start_result;}
void func_800217E4(VoiceState *p){unsigned i=(unsigned)(p-D_8004BEB8);event(4,i);if(mode==4){p->channel=4;p->set=3;p->key=0x154;}}
void func_80020F4C(u8 a,u8 b,u8 c){event(5,((unsigned)a<<16)|((unsigned)b<<8)|c);}
static u32 model(u8 note,u8 channel,u8 set){
 unsigned i;VoiceState *p,*last=0;u32 first=0xFFFFFFFFU,previous=0,old;int detune;
 for(i=0;i<D_8004FA18;i++){
  p=&D_8004BEB8[i];
  if(p->identifier==0xFFFFFFFFU||p->channel!=channel||p->set!=set)continue;
  if(!(p->flags&16)||((p->flags&8)&&!(p->flags&0x40000000)))continue;
  if(!func_8001467C(i))continue;
  last=p;old=p->key;detune=p->cents;
  p->pitch=old*65536U+(u32)(detune*65536/100);
  p->original_key=(u8)old;p->key=(u16)((unsigned)note+(old&255)-(p->base_key&255));p->base_key=note;
  p->cents=0;p->glide=0;p->flags|=0x80800;func_8001EB10(p);
  if(first==0xFFFFFFFFU){p->child=0xFFFFFFFFU;p->parent=0xFFFFFFFFU;first=func_8001ECE0(p);}
  else{D_8004BEB8[previous&255].child=p->identifier;p->parent=previous;}
  previous=p->identifier;
 }
 if(first!=0xFFFFFFFFU){assert(last!=0);func_800217E4(last);func_80020F4C(last->channel,last->set,(u8)last->key);}
 return first;
}
static void initialize(unsigned n,unsigned seed){unsigned i;memset(D_8004BEB8,0xA5,sizeof(D_8004BEB8));D_8004FA18=(u8)n;event_count=start_count=0;for(i=0;i<8;i++){VoiceState *p=&D_8004BEB8[i];p->identifier=0xABCD0000U|i;p->channel=2;p->set=1;p->flags=16;p->key=(u16)(seed+31*i);p->base_key=(u16)(seed*3+i);p->cents=(s8)(seed+i);p->glide=71;p->pitch=53;}}
int main(void){
 unsigned note,detune,scenario,i,expected_count;u32 r,want,old;VoiceState initial[8],expected[8];unsigned trace[128];u8 initial_count,expected_n;
 mode=0;active_mask=255;start_result=0x12345678;
 for(note=0;note<256;note++)for(detune=0;detune<256;detune++){
  initialize(1,note*257+detune);old=D_8004BEB8[0].key;D_8004BEB8[0].cents=(s8)detune;
  want=old*65536U+(u32)(((int)(s8)detune*65536)/100);
  r=func_80019C8C((u8)note,2,1);assert(r==start_result);assert(D_8004BEB8[0].pitch==want);
  assert(D_8004BEB8[0].key==(u16)(note+(old&255)-(u8)((note*257+detune)*3)));
  assert(D_8004BEB8[0].base_key==note&&D_8004BEB8[0].original_key==(u8)old&&D_8004BEB8[0].cents==0&&D_8004BEB8[0].glide==0);
 }
 for(scenario=0;scenario<640;scenario++){
  mode=scenario%5;active_mask=(scenario*37)&255;start_result=(scenario%7==0)?0xFFFFFFFFU:0x87654321U;
  initialize(scenario%5,scenario*101);
  for(i=0;i<8;i++){
   if((scenario>>(i%7))&1)D_8004BEB8[i].flags|=8;
   if((scenario>>(i%5))&1)D_8004BEB8[i].flags|=0x40000000;
   if((scenario+i)%7==0)D_8004BEB8[i].identifier=0xFFFFFFFFU;
   if((scenario+i)%11==0)D_8004BEB8[i].channel=3;
   if((scenario+i)%13==0)D_8004BEB8[i].set=2;
   if((scenario+i)%17==0)D_8004BEB8[i].flags=0;
  }
  memcpy(initial,D_8004BEB8,sizeof(initial));initial_count=D_8004FA18;
  want=model((u8)scenario,2,1);memcpy(expected,D_8004BEB8,sizeof(expected));expected_n=D_8004FA18;expected_count=event_count;memcpy(trace,events,sizeof(trace));
  memcpy(D_8004BEB8,initial,sizeof(initial));D_8004FA18=initial_count;event_count=start_count=0;
  r=func_80019C8C((u8)scenario,2,1);
  assert(r==want&&D_8004FA18==expected_n&&event_count==expected_count);assert(memcmp(events,trace,event_count*sizeof(events[0]))==0);assert(memcmp(D_8004BEB8,expected,sizeof(expected))==0);
 }
 return 0;
}
''')

    def test_emitter_initialization_slots_exits_and_alias(self):
        self.check_c('8001D1F4',r'''
Emitter *D_8004FD50;Emitter D_8004FD58;
static Emitter *running;static Vector *input_position;
static unsigned trace[16],count,scenario;static u32 start_id;static u16 seen_id;
static float out_volume,out_pitch,out_pan,out_span,out_send;
static void mark(unsigned x){trace[count++]=x;assert(count<16);}
void func_80014594(void){mark(1);if(scenario==8)input_position->x=42.f;}
void func_800145DC(void){mark(5);if(scenario==7)running->handle=0xABCDE123U;}
void func_8001C860(Emitter *p,float *v,float *pitch,float *pan,float *span,float *send){assert(p==&D_8004FD58);mark(2);*v=out_volume;if(out_volume!=0){*pitch=out_pitch;*pan=out_pan;*span=out_span;*send=out_send;}if(scenario==6)p->identifier=321;}
u32 func_80020174(u16 id,u8 a,u8 b){mark(3);seen_id=id;assert(a==127&&b==64);return start_id;}
void func_8001CCDC(Emitter *p,float v,float pan,float span,float send,float pitch){mark(4);assert(p==&D_8004FD58);assert(v==out_volume&&pitch==out_pitch&&pan==out_pan&&span==out_span&&send==out_send);p->flags|=0x100000;}
int main(void){Emitter e,old,expected,before;Vector p,v,expected_p,expected_v;u32 r;unsigned gain,minimum,k;const Vector *pp,*vp;
 p.x=1;p.y=2;p.z=3;v.x=4;v.y=5;v.z=6;input_position=&p;scenario=0;
 for(gain=0;gain<256;gain++)for(minimum=0;minimum<256;minimum++){
  memset(&e,0xA5,sizeof(e));memset(&old,0x5A,sizeof(old));expected=e;D_8004FD50=&old;running=&e;count=0;
  expected.next=&old;expected.previous=0;expected.flags=0x30123;expected.position=p;expected.velocity=v;expected.range=10;expected.curve=.25f;expected.gain=(float)gain/127.f;expected.minimum=(float)minimum/127.f;expected.identifier=0xFEDC;expected.context=0x87654321;expected.handle=0xFFFFFFFFU;expected.counter=0;
  r=func_8001D1F4(&e,&p,&v,10,.25f,0x123,0xFEDC,0x87654321,(u8)gain,(u8)minimum);
  assert(r==0xFFFFFFFFU&&D_8004FD50==&e&&old.previous==&e&&count==2&&trace[0]==1&&trace[1]==5);assert(memcmp(&e,&expected,sizeof(e))==0);
 }
 for(k=0;k<9;k++){
  scenario=k;memset(&D_8004FD58,0xA5,sizeof(D_8004FD58));before=D_8004FD58;running=&D_8004FD58;count=0;seen_id=0;
  out_volume=k==0?0.f:.5f;out_pitch=1;out_pan=.25f;out_span=-.25f;out_send=.75f;start_id=k==1?0xFFFFFFFFU:0x12345678;
  r=func_8001D1F4(0,&p,&v,10,.25f,0x123,17,0x90000011,127,31);
  assert(trace[0]==1&&trace[1]==2&&trace[count-1]==5);
  assert(D_8004FD58.next==before.next&&D_8004FD58.previous==before.previous&&D_8004FD58.counter==before.counter&&memcmp(&D_8004FD58.fade,&before.fade,sizeof(float))==0);
  if(k==0){assert(r==0xFFFFFFFFU&&count==3&&D_8004FD58.handle==before.handle);}
  else if(k==1){assert(r==0xFFFFFFFFU&&count==4&&D_8004FD58.handle==0xFFFFFFFFU);}
  else{assert(r==(k==7?0xABCDE123U:start_id)&&count==5&&trace[2]==3&&trace[3]==4);assert(seen_id==(k==6?321:17));}
  assert(D_8004FD58.position.x==(k==8?42.f:p.x));
 }
 scenario=0;
 for(k=0;k<3;k++){
  memset(&e,0,sizeof(e));e.position=p;e.velocity=v;D_8004FD50=0;running=&e;count=0;
  pp=k==0?&e.position:&e.velocity;vp=k==2?&e.position:&e.velocity;
  expected_p=*pp;expected_v=k==2?expected_p:*vp;
  r=func_8001D1F4(&e,pp,vp,1,.5f,2,9,8,31,15);
  assert(r==0xFFFFFFFFU&&e.next==0&&e.previous==0);assert(memcmp(&e.position,&expected_p,sizeof(Vector))==0&&memcmp(&e.velocity,&expected_v,sizeof(Vector))==0);
 }
 return 0;}
''')

    def test_cached_thresholds_counter_wrap_and_live_lists(self):
        self.check_c('8001DC08',r'''
SpatialGroup D_8004FDA0[32];u8 D_8004FF20;float D_8002D904,D_8002D908;
static Emitter states[8];static SpatialEntry entries[8],active[8];
static unsigned trace[64],count,mode,fail_mask;static float passed[32][5];
static void mark(unsigned x){trace[count++]=x;assert(count<32);}
u32 func_8001B1D0(u16 id,u8 a,u8 b){assert(id<8&&a==127&&b==64);mark(0x100|id);if(mode==1)D_8004FF20=3;if(mode==2)D_8004FF20=1;D_8002D904=1000;D_8002D908=-1000;return (fail_mask&(1U<<id))?0xFFFFFFFFU:0xABCD0000U|id;}
void func_8001CCDC(Emitter *s,float a,float b,float c,float d,float e){unsigned id=(unsigned)(s-states);passed[count][0]=a;passed[count][1]=b;passed[count][2]=c;passed[count][3]=d;passed[count][4]=e;mark(0x200|id);s->flags|=0x820000;if(mode==3)D_8004FDA0[id/2].active=&active[6];if(mode==4&&id==0)entries[0].next=&entries[2];}
static void model(void){unsigned i;SpatialEntry *p;Emitter *s;float lo,hi,d;
 if(!D_8004FF20)return;
 lo=D_8002D908;hi=D_8002D904;
 for(i=0;i<D_8004FF20;i++)for(p=D_8004FDA0[i].pending;p;p=p->next){
  if(D_8004FDA0[i].active){d=p->volume-D_8004FDA0[i].active->volume;if(d<=lo)continue;if(d<=hi){p->emitter->counter=(u16)(p->emitter->counter+1);if(p->emitter->counter<20)continue;}else p->emitter->counter=0;}
  s=p->emitter;s->handle=func_8001B1D0(s->identifier,127,64);
  if(s->handle==0xFFFFFFFFU){if(!(s->flags&2))s->flags=(s->flags|0x40000)&~0x20000U;}
  else{s->fade=0;s->flags|=0x100000;func_8001CCDC(s,p->volume,p->pan,p->span,p->send,p->pitch);s->flags&=~0x20000U;if(D_8004FDA0[i].active)D_8004FDA0[i].active=D_8004FDA0[i].active->next;}
 }
}
static void initialize(unsigned scenario,unsigned counter,float delta){unsigned i;memset(states,0,sizeof(states));memset(entries,0,sizeof(entries));memset(active,0,sizeof(active));memset(D_8004FDA0,0,sizeof(D_8004FDA0));memset(passed,0,sizeof(passed));count=0;D_8004FF20=(u8)(scenario%4);D_8002D904=.125f;D_8002D908=0;
 for(i=0;i<8;i++){states[i].identifier=(u16)i;states[i].counter=(u16)counter;states[i].flags=0x20000|(scenario&2);states[i].fade=.5f;states[i].handle=0x11223344;
  entries[i].next=(i%2==0)?&entries[i+1]:0;entries[i].emitter=&states[i];entries[i].volume=delta+(float)(i%2)*.0625f;entries[i].pan=.25f;entries[i].span=-.5f;entries[i].send=.75f;entries[i].pitch=1;
  active[i].next=(i%2==0)?&active[i+1]:0;active[i].volume=0;
 }
 for(i=0;i<4;i++){D_8004FDA0[i].pending=((scenario+i)%5==0)?0:&entries[2*i];D_8004FDA0[i].active=(scenario&1)?&active[2*i]:0;}
}
int main(void){unsigned scenario,c,d,expected_count;unsigned counters[5]={0,18,19,20,65535};float deltas[6]={-1,0,.0625f,.125f,.25f,1};Emitter expected_states[8];SpatialEntry expected_entries[8],expected_active[8];SpatialGroup expected_groups[32];unsigned expected_trace[64];float expected_passed[32][5],expected_lo,expected_hi;u8 expected_n;
 for(scenario=0;scenario<80;scenario++)for(c=0;c<5;c++)for(d=0;d<6;d++){
  mode=scenario%5;fail_mask=(scenario*53)&255;initialize(scenario,counters[c],deltas[d]);model();memcpy(expected_states,states,sizeof(states));memcpy(expected_entries,entries,sizeof(entries));memcpy(expected_active,active,sizeof(active));memcpy(expected_groups,D_8004FDA0,sizeof(D_8004FDA0));memcpy(expected_trace,trace,sizeof(trace));memcpy(expected_passed,passed,sizeof(passed));expected_count=count;expected_n=D_8004FF20;expected_lo=D_8002D908;expected_hi=D_8002D904;
  initialize(scenario,counters[c],deltas[d]);func_8001DC08();assert(count==expected_count&&D_8004FF20==expected_n&&D_8002D908==expected_lo&&D_8002D904==expected_hi);assert(memcmp(states,expected_states,sizeof(states))==0&&memcmp(entries,expected_entries,sizeof(entries))==0&&memcmp(active,expected_active,sizeof(active))==0&&memcmp(D_8004FDA0,expected_groups,sizeof(D_8004FDA0))==0);assert(memcmp(trace,expected_trace,count*sizeof(trace[0]))==0&&memcmp(passed,expected_passed,sizeof(passed))==0);
 }
 return 0;}
''')

if __name__=='__main__':unittest.main()
