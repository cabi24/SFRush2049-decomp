"""Bounded event-dispatch tests against an independent high-level state model.

Synthetic u16-backed streams are aligned whole allocations. External mocks test
callee contracts and live effects, not the complete callee implementations.
"""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
ROOT=Path(__file__).resolve().parents[4]
WORK=Path(__file__).resolve().parent
SOURCE=WORK/'nonmatch/func_80017D38.c'
class Semantics(unittest.TestCase):
 def test_events_timing_and_live_context(self):
  body=r'''
SequenceContext *D_8004BE80;u32 D_8004BE78;u8 D_8004BE7B,D_8004BE7C;
static SequenceContext contexts[2];static SequenceNote nodes[4];
static u16 streams[2][64][8],alternate[8];static u32 results[2];
static unsigned mode,allocations,calls,frees,constructors,current_track;
typedef struct Call {unsigned kind;u32 arg[10];} Call;
static Call trace[16];
static Call *mark(unsigned kind){Call *c=&trace[calls++];assert(calls<16);memset(c,0,sizeof(*c));c->kind=kind;return c;}
static void switch_context(void){D_8004BE80=&contexts[1];D_8004BE78=1;D_8004BE7B=1;D_8004BE7C=1;}
SequenceNote *func_800173B4(void){mark(1);if(mode==1)return 0;if(mode==5)switch_context();assert(allocations<4);return &nodes[allocations++];}
void func_80017410(SequenceNote *n){Call *c=mark(3);c->arg[0]=(u32)(n-nodes);frees++;}
void func_80017720(SequenceContext *c,u8 key,u8 channel){Call *t=mark(4);t->arg[0]=(u32)(c-contexts);t->arg[1]=key;t->arg[2]=channel;c->programs[channel]=0x42000000|key;if(mode==4)switch_context();}
void func_800177EC(u8 key,u8 channel){Call *t=mark(5);t->arg[0]=key;t->arg[1]=channel;if(mode==4)switch_context();}
void func_80019490(SequenceRequest *r,u32 *out,u8 flag){Call *t=mark(6);assert(r==&D_8004BE80->request&&out==D_8004BE80->result);t->arg[0]=(u32)(D_8004BE80-contexts);t->arg[1]=flag;*out=0xAABBCCDD;r->flags^=8;if(mode==6)switch_context();}
void func_80020610(u8 a,u8 b,u8 c,u8 d){Call *t=mark(7);t->arg[0]=a;t->arg[1]=b;t->arg[2]=c;t->arg[3]=d;if(mode==4)switch_context();}
u32 func_8001A270(u32 p,u8 key,u8 vol,u8 pan,u8 channel,u8 set,u16 offset,u16 section,u8 group,s16 priority){Call *t=mark(2);t->arg[0]=p;t->arg[1]=key;t->arg[2]=vol;t->arg[3]=pan;t->arg[4]=channel;t->arg[5]=set;t->arg[6]=offset;t->arg[7]=section;t->arg[8]=group;t->arg[9]=(u32)priority;constructors++;if(mode==3)switch_context();if(mode==3||mode==7){D_8004BE80->tracks[current_track].events=(u8*)alternate;D_8004BE80->tracks[current_track].time.fraction=0xAABBCCDD;D_8004BE80->tracks[current_track].time.whole=100;D_8004BE80->tracks[current_track].event_time=87;}return mode==2?0xFFFFFFFFU:0;}
static u8 model(void){unsigned i;u8 active=0;SequenceTrack *t;SequenceContext *c;u32 due,program,id;int key,vel,number,step;u8 channel;SequenceNote *node;
 for(i=0;i<64;i++){
  if(D_8004BE80->tracks[i].events)active=1;
  for(;;){
   c=D_8004BE80;t=&c->tracks[i];if(!t->events)break;due=t->event_time+((u16*)t->events)[0];if(t->time.whole+c->lookahead<due)break;t->event_time=due;
   t=&D_8004BE80->tracks[i];key=t->events[2];vel=t->events[3];channel=t->channel;
   if(key==255&&vel==255){t->events=0;continue;}
   step=4;
   if(key>=128&&vel==0)func_80017720(D_8004BE80,(u8)(key-128),channel);
   else if(key>=128&&vel==1)func_800177EC((u8)(key-128),channel);
   else if(key>=128&&vel>=128){if((vel-128)==104){if(D_8004BE80->pending){func_80019490(&D_8004BE80->request,D_8004BE80->result,1);D_8004BE80->pending=0;}}else func_80020610((u8)(vel-128),channel,D_8004BE7B,(u8)(key-128));}
   else if(key||vel){
    step=6;number=t->number;c=D_8004BE80;
    if(c->enabled[number/32]&(1U<<(number%32))){program=c->programs[channel];if(program!=0xFFFFFFFFU){
     key+=t->transpose;if(key<0)key=0;if(key>127)key=127;vel+=t->velocity_offset;if(vel<0)vel=0;if(vel>127)vel=127;
     node=func_800173B4();if(node){id=func_8001A270(program,(u8)key,(u8)vel,64,channel,(u8)D_8004BE78,0,(u16)number,D_8004BE80->groups[number],D_8004BE7C?-1:0);
      if(id==0xFFFFFFFFU)func_80017410(node);else{node->identifier=id;node->due=((u16*)D_8004BE80->tracks[i].events)[2]+due;node->time=D_8004BE80->tracks[i].time;}
     }
    }}
   }
   D_8004BE80->tracks[i].events+=step;
  }
 }
 return active;}
static unsigned event_words(unsigned key,unsigned vel){return key==255&&vel==255?2:((key>=128&&(vel==0||vel==1||vel>=128))||(!key&&!vel)?2:3);}
static void put(u16 *p,u16 delta,u8 key,u8 vel,u16 length){unsigned n;u8 *bytes=(u8*)p;p[0]=delta;bytes[2]=key;bytes[3]=vel;n=event_words(key,vel);if(n==3)p[2]=length;p[n]=0;bytes=(u8*)(p+n);bytes[2]=255;bytes[3]=255;}
static void init(unsigned key,unsigned vel,unsigned scenario){unsigned c,i,words;memset(contexts,0,sizeof(contexts));memset(nodes,0xA5,sizeof(nodes));memset(streams,0,sizeof(streams));memset(trace,0,sizeof(trace));memset(results,0,sizeof(results));memset(alternate,0,sizeof(alternate));allocations=calls=frees=constructors=0;D_8004BE80=&contexts[0];D_8004BE78=0;D_8004BE7B=0;D_8004BE7C=scenario&1;current_track=scenario%64;
 for(c=0;c<2;c++){contexts[c].enabled[0]=contexts[c].enabled[1]=0xFFFFFFFFU;contexts[c].result=&results[c];contexts[c].pending=(u8)(scenario&1);for(i=0;i<16;i++)contexts[c].programs[i]=0x00010000U|(i<<8)|7;for(i=0;i<64;i++){SequenceTrack *t=&contexts[c].tracks[i];contexts[c].groups[i]=(u8)(23+c);t->time.fraction=0x12345678U+i;t->time.whole=10;t->event_time=5;t->channel=(u8)(scenario%16);t->number=(u8)(scenario%64);t->transpose=(s8)((scenario%5)*64-128);t->velocity_offset=(s8)(((scenario+2)%5)*64-128);}}
 put(streams[0][current_track],0,(u8)key,(u8)vel,9);contexts[0].tracks[current_track].events=(u8*)streams[0][current_track];
 put(streams[1][current_track],0,(u8)key,(u8)vel,31);
 if(mode==3||mode==4||mode==5||mode==6)contexts[1].tracks[current_track].events=(u8*)streams[1][current_track];
 words=event_words(key,vel);(void)words;put(alternate,0,60,90,37);
 if(mode==8){contexts[0].tracks[current_track].event_time=(scenario&8)?0xFFFFFFE0U:0xFFFFFFFEU;contexts[0].tracks[current_track].time.whole=(scenario&1)?0xFFFFFFFFU:1;contexts[0].lookahead=(scenario&2)?3:0;streams[0][current_track][0]=3;if(event_words(key,vel)==3&&(scenario&4))streams[0][current_track][2]=65535;}
 if(mode==9)contexts[0].enabled[scenario%64/32]&=~(1U<<(scenario%32));
 if(mode==10)contexts[0].programs[scenario%16]=0xFFFFFFFFU;
 if(mode==11){put(streams[0][current_track],2,60,100,4);put(streams[0][current_track]+3,3,12,70,5);}
 if(mode==12){put(streams[0][current_track],1,0x95,0,0);put(streams[0][current_track]+2,1,60,100,4);}
 if(mode==13){put(streams[0][current_track],1,0,0,0);put(streams[0][current_track]+2,1,60,100,4);}
}
static void check(unsigned key,unsigned vel,unsigned scenario){SequenceContext expected[2];SequenceNote expected_nodes[4];Call expected_trace[16];u32 expected_results[2];unsigned a,f,n,c,index;u32 set;u8 byte,fade,r,want;
 init(key,vel,scenario);want=model();memcpy(expected,contexts,sizeof(expected));memcpy(expected_nodes,nodes,sizeof(nodes));memcpy(expected_trace,trace,sizeof(trace));memcpy(expected_results,results,sizeof(results));a=allocations;f=frees;n=constructors;c=calls;index=(unsigned)(D_8004BE80-contexts);set=D_8004BE78;byte=D_8004BE7B;fade=D_8004BE7C;
 init(key,vel,scenario);r=func_80017D38();assert(r==want&&allocations==a&&frees==f&&constructors==n&&calls==c&&(unsigned)(D_8004BE80-contexts)==index&&D_8004BE78==set&&D_8004BE7B==byte&&D_8004BE7C==fade);assert(memcmp(contexts,expected,sizeof(expected))==0&&memcmp(nodes,expected_nodes,sizeof(nodes))==0&&memcmp(trace,expected_trace,sizeof(trace))==0&&memcmp(results,expected_results,sizeof(results))==0);
}
int main(void){unsigned key,vel,s;u8 r;
 mode=0;for(key=0;key<256;key++)for(vel=0;vel<256;vel++)check(key,vel,(key*17+vel)%64);
 for(mode=1;mode<=10;mode++)for(s=0;s<64;s++){check(60,100,s);check(0x9A,0,s);check(0x9A,1,s);check(0x9A,0xE8,s);check(0x9A,0xAA,s);}
 for(mode=11;mode<=13;mode++)for(s=0;s<64;s++)check(60,100,s);
 mode=0;init(255,255,0);r=func_80017D38();assert(r==1&&contexts[0].tracks[0].events==0);assert(func_80017D38()==0);
 init(60,100,0);contexts[0].tracks[0].event_time=5;streams[0][0][0]=100;assert(func_80017D38()==1&&contexts[0].tracks[0].events==(u8*)streams[0][0]&&contexts[0].tracks[0].event_time==5&&calls==0);
 return 0;}
'''
  text='#include <assert.h>\n#include <string.h>\n#include "'+str(SOURCE)+'"\n'+body
  with tempfile.TemporaryDirectory(prefix='sequence-events-test-') as tmp:
   p=Path(tmp);(p/'test.c').write_text(text)
   r=subprocess.run(['cc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-O1','-fsanitize=address,undefined','-no-pie',str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stdout+r.stderr)
   r=subprocess.run([str(p/'test')],capture_output=True,text=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'))
   self.assertEqual(r.returncode,0,r.stdout+r.stderr)
if __name__=='__main__':unittest.main()
